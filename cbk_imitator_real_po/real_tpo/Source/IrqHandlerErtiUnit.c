
/*! \file IrqHandlerErtiUnit.c
 * \author Гапонов Р.В.
 * \date 2013.05.06
 * \version 1.0.0
 * \brief Модуль содержит функции обработки прерываний от аппаратуры, специфичных для комплекта аппаратуры ЕРТИ
 */

#include "MainUnit.h"
//================================================================================

#ifdef SPO_ERTI

/*! \fn void IrqHandler02 (void);
 * \brief Функция обработки прерываний от блока 732 (маска прерывания 0x0004)
 * \details Передает по МКО сообщение о прерывании (если разрешено в конфигурации СПО), затем производит
 * обработку прерывания в зависимости от состояния блока 732 (для детальной информации см. ТЗ на СПО).
 */

void IrqHandler02 (void)
{
int Result, Index, VmMask, DataSize;
int DevMkoLine, DevMkoChan;
int BaseMkoLine, BaseMkoChan;
WORD Config732[15], Kpi732[8], VmState, FirstState, PupsState;
UINT ShbkMask, MsgMask, Mbk04ReplyEnabled;
BYTE PupsList[4], ShbkBuffer[32];
WORD PupsData[4][8];

MSGS("IrqHandler02()");
SendIrqMessageToKPA(0x0004);

// параметры линий обмена
DevMkoLine = SpoConfig.DevMkoLine & 1;
DevMkoChan = SpoConfig.DevMkoChan & 1;

BaseMkoLine = SpoConfig.KpaMkoLine & 1;
BaseMkoChan = SpoConfig.KpaMkoChan & 1;

// маска типов передаваемых сообщений
MsgMask = SpoConfig.ErtiIrq2MsgMask;
// разрешение на передачу квитанций
Mbk04ReplyEnabled = SpoConfig.Mbk04Irq2ReplyMode;

// пауза перед обработкой (управляемая)
if (SpoConfig.ErtiIrq2Pause1) SleepMSec(SpoConfig.ErtiIrq2Pause1);

// пауза перед обработкой (неуправляемая)
SleepMSec(1);

// сброс запроса прерывания 732
if (sprIQClear(2) == ERROR)
  {
  MkoBufB[0][0] = REPLY_IRQ_02_DONE | REPLY_FLAG_ERROR;
  MkoBufB[0][1] = 1;
  if (MsgMask & IRQ2_MSG_ERROR) MkoXchgBn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_KPI, MKO_TX, 2), 0);
  return;
  }

// чтение текущей конфигурации 732
Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(2, 17, MKO_RX, 15), 0);
if (Result)
  {
  MkoBufB[0][0] = REPLY_732_INFO_ERROR;
  if (MsgMask & IRQ2_MSG_ERROR) MkoXchgBn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_KPI, MKO_TX, 1), 0);
  return;
  }
SleepMSec(10);

// буфер с конфигурацией 732
memmove(&Config732, &MkoBufB[0][0], sizeof(Config732));

// расчет маски включенных каналов ВЧМ 732
for (Index = 0, VmMask = 0; Index < 4; Index++)
  {
  if ((Config732[Index + 5]) & 0x0100) // канал ВЧМ включен
    {
    VmMask |= (1 << Index);
    }
  }

// сообщение "Все ВЧМ 732 выключены"
if (!VmMask)
  {
  MkoBufB[0][0] = REPLY_732_ALL_VM_OFF;
  if (MsgMask & IRQ2_MSG_ERROR) MkoXchgBn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_KPI, MKO_TX, 1), 0);
  return;
  }

// сообщение "Конфигурация ВЧМ 732"
MkoBufB[0][0] = REPLY_732_VM_STATE;
MkoBufB[0][1] = VmMask;
if (MsgMask & IRQ2_MSG_PROCESS) MkoXchgBn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_KPI, MKO_TX, 2), 0);

// проверка битов ошибок в ВЧМ 732
for (Index = 0; Index < 4; Index++)
  {
  VmState = Config732[Index + 5];
  if (VmState & 0x0100) // канал ВЧМ включен
    {
    if (VmState & 0x3800) // есть биты ошибок
      {
      MkoBufB[0][0] = REPLY_732_VM_STATE_ERROR;
      memmove(&MkoBufB[0][1], &Config732[5], 4 * sizeof(WORD));
      if (MsgMask & IRQ2_MSG_ERROR) MkoXchgBn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_KPI, MKO_TX, 5), 0);
      return;      
      }
    }  
  }

// предварительный анализ ПУПС каналов ВЧМ (сообщения 5, 16, 18, 31)
for (Index = 0; Index < 4; Index++)
  {
  PupsList[Index] = 0;
  VmState = Config732[Index + 5];
  if (VmState & 0x0100) // канал ВЧМ включен
    {
    PupsState = VmState & 0x001F;
    if ((PupsState == 5) || (PupsState == 16) || (PupsState == 18) || (PupsState == 31)) 
      {
      PupsList[Index] = ((PupsState == 5) || (PupsState == 16)) ? (2) : (7);
      MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(2, 25 - Index, MKO_RX, PupsList[Index]), 0);
      memmove(&PupsData[Index], &MkoBufB[0][0], 8 * sizeof(WORD));
      SleepMSec(10);
      }
    }
  }

for (Index = 0; Index < 4; Index++)
  {
  DataSize = PupsList[Index];
  if (DataSize)
    { 
    // буфер данных КПИ 732
    memmove(&MkoBufB[0][0], &PupsData[Index], DataSize * sizeof(WORD));
    if (MsgMask & IRQ2_MSG_PROCESS) MkoXchgBn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_KPI + 1, MKO_TX, DataSize), 0);

    MkoBufB[0][0] = REPLY_732_EXT_INFO;
    MkoBufB[0][1] = DataSize * sizeof(WORD);
    MkoBufB[0][2] = CalcCrc16(&PupsData[Index], DataSize * sizeof(WORD));
    if (MsgMask & IRQ2_MSG_PROCESS) MkoXchgBn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_KPI, MKO_TX, 3), 0);
    }
  }

// сравнение состояния ПУПС ВЧМ 732
FirstState = 1;
PupsState = 0;

for (Index = 0; Index < 4; Index++)
  {
  VmState = Config732[Index + 5];
  if (VmState & 0x0100) // канал ВЧМ включен
    {
    if (FirstState)
      {
      FirstState = 0;
      PupsState = VmState & 0x001F;
      }
    if (PupsState != (VmState & 0x001F))
      {
      // сообщение о ошибке мажорирования ПУПС-ов 
      MkoBufB[0][0] = REPLY_732_PUPS_STATE_ERROR;
      memmove(&MkoBufB[0][1], &Config732[5], 4 * sizeof(WORD));
      if (MsgMask & IRQ2_MSG_ERROR) MkoXchgBn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_KPI, MKO_TX, 5), 0);
      return;      
      }
    }
  }

// контроль слова состояния ВЧМ 732
if (Config732[10] & 0x0F00)
  {
  MkoBufB[0][0] = REPLY_732_VM_STATE_ERROR;
  MkoBufB[0][1] = Config732[10];
  if (MsgMask & IRQ2_MSG_ERROR) MkoXchgBn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_KPI, MKO_TX, 2), 0);
  return;      
  }

// сообщение о неверном состоянии ПУПС-а
if (((Config732[10] & 0x0080) == 0) || ((Config732[10] & 0x001F) != PupsState))
  {
  MkoBufB[0][0] = REPLY_732_VM_STATE_ERROR;
  MkoBufB[0][1] = Config732[10];
  if (MsgMask & IRQ2_MSG_ERROR) MkoXchgBn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_KPI, MKO_TX, 2), 0);
  return;      
  }

// проверяем необходимость завершения прерывания
if ((PupsState == 19) && (!SpoConfig.Irq02Pups19Enabled)) return;

// выдаем в КПА достоверный ПУПС ВЧМ 732
MkoBufB[0][0] = REPLY_732_PUPS_OK;
MkoBufB[0][1] = PupsState;
if (MsgMask & IRQ2_MSG_PROCESS) MkoXchgBn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_KPI, MKO_TX, 2), 0);

// накапливаем список достоверных ПУПС-ов
if (B732PupsListSize < B732_PUPS_LIST_SIZE)
  {
  B732PupsList[B732PupsListSize++] = PupsState;
  }

// выдача СЧБК
if (PupsState == 1)
  {
  // передаем квитанцию "Да СЧЗК" в МБК04
  MkoBufB[0][0] = 0x00F0;
  Result = (Mbk04ReplyEnabled) ? (MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(4, 5, MKO_TX, 1), 0)) : (0);
  if (Result)
    {
    MkoBufB[0][0] = REPLY_MBK04_SHZK_WR_ERROR;
    if (MsgMask & IRQ2_MSG_ERROR) MkoXchgBn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_KPI, MKO_TX, 1), 0);
    return;
    }
  SleepMSec(10);

  // передаем СЧБК в МБК04 (если разрешено)
  if (SpoConfig.SendShbkTo732InIrq02)
    {
    // считываем код СЧБК (32-бит)
    MkoXchgBn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_SHBK, MKO_RX, 2), 0);
    memmove(&ShbkMask, &MkoBufB[0][0], sizeof(ShbkMask));

    // передаем СЧБК в 732
    Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(2, 21, MKO_TX, 2), 0);
    if (Result)
      {
      MkoBufB[0][0] = REPLY_732_SHBK_WR_ERROR;
      if (MsgMask & IRQ2_MSG_ERROR) MkoXchgBn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_KPI, MKO_TX, 1), 0);
      return;
      }
    SleepMSec(10);
    }

  if (SpoConfig.SendShbkToMbk04InIrq02)
    {
    // формирование буфера данных СЧБК
    MakeShbkBuffer(ShbkMask, (BYTE*) &ShbkBuffer);

    // передача буфера данных СЧБК в МБК4
    memmove(&MkoBufB[0][0], &ShbkBuffer, sizeof(ShbkBuffer));
    Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(4, 6, MKO_TX, 16), 0);
    if (Result)
      {
      MkoBufB[0][0] = REPLY_732_SHBK_WR_ERROR;
      if (MsgMask & IRQ2_MSG_ERROR) MkoXchgBn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_KPI, MKO_TX, 1), 0);
      return;
      }
    }

  // сообщение о выданном СЧБК
  MkoBufB[0][0] = REPLY_732_SHBK_OUT;
  if (MsgMask & IRQ2_MSG_PROCESS) MkoXchgBn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_KPI, MKO_TX, 1), 0);

  // сообщение "прерывание обработано"
  MkoBufB[0][0] = REPLY_IRQ_02_DONE;
  if (MsgMask & IRQ2_MSG_DONE) MkoXchgBn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_KPI, MKO_TX, 1), 0);
  return;
  }

// выдача квитанции "Да СЧЗК" в МБК04
if (PupsState == 2)
  {
  MkoBufB[0][0] = 0x00F0;
  Result = (Mbk04ReplyEnabled) ? (MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(4, 5, MKO_TX, 1), 0)) : (0);
  if (Result)
    {
    MkoBufB[0][0] = REPLY_MBK04_SHZK_WR_ERROR;
    if (MsgMask & IRQ2_MSG_ERROR) MkoXchgBn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_KPI, MKO_TX, 1), 0);
    return;
    }
  SleepMSec(10);
  }

// выдача квитанции "Да КПИ" в МБК04
if ((PupsState == 3) || (PupsState == 17) || (PupsState == 21) || (PupsState == 22))
  {
  MkoBufB[0][0] = 0x00F0;
  Result = (Mbk04ReplyEnabled) ? (MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(4, 7, MKO_TX, 1), 0)) : (0);
  if (Result)
    {
    MkoBufB[0][0] = REPLY_MBK04_KPI_WR_ERROR;
    if (MsgMask & IRQ2_MSG_ERROR) MkoXchgBn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_KPI, MKO_TX, 1), 0);
    return;
    }
  SleepMSec(10);
  }

// выдача КПИ
if ((PupsState == 4) || (PupsState == 15))
  {
  // пауза перед обработкой (управляемая)
  if (SpoConfig.ErtiIrq2Pause2) SleepMSec(SpoConfig.ErtiIrq2Pause2);

  MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(2, 21, MKO_RX, 2), 0);
  SleepMSec(10);

  // буфер данных КПИ 732
  memmove(&Kpi732, &MkoBufB[0][0], 2 * sizeof(WORD));
  if (MsgMask & IRQ2_MSG_PROCESS) MkoXchgBn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_KPI + 1, MKO_TX, 2), 0);

  MkoBufB[0][0] = REPLY_732_KPI2_OK;
  MkoBufB[0][1] = 2 * sizeof(WORD);
  MkoBufB[0][2] = CalcCrc16(&Kpi732, 2 * sizeof(WORD));
  if (MsgMask & IRQ2_MSG_PROCESS) MkoXchgBn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_KPI, MKO_TX, 3), 0);

  // передаем квитанцию "Да КПИ" на МБК04
  MkoBufB[0][0] = 0x00F0;
  Result = (Mbk04ReplyEnabled) ? (MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(4, 7, MKO_TX, 1), 0)) : (0);
  if (Result)
    {
    MkoBufB[0][0] = REPLY_MBK04_KPI_WR_ERROR;
    if (MsgMask & IRQ2_MSG_ERROR) MkoXchgBn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_KPI, MKO_TX, 1), 0);
    return;
    }
  SleepMSec(10);

  // сообщение "прерывание обработано"
  MkoBufB[0][0] = REPLY_IRQ_02_DONE;
  if (MsgMask & IRQ2_MSG_DONE) MkoXchgBn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_KPI, MKO_TX, 1), 0);
  return;      
  }

// сброс всех ПУПС 732 широковещательной командой
if (!SpoConfig.LkaOpoVersion)
  {
  MkoBufB[0][0] = 0xA83F;
  Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(2, 17, MKO_TX, 1), 0);
  }
else
  {
  MkoBufB[0][0] = 0x0D00;
  Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(2, 19, MKO_TX, 1), 0);
  }
if (Result)
  {
  MkoBufB[0][0] = REPLY_732_PUPS_WR_ERROR;
  if (MsgMask & IRQ2_MSG_ERROR) MkoXchgBn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_KPI, MKO_TX, 1), 0);
  return;
  }
SleepMSec(10);

// выдача квитанции "Нет СЧЗК" в МБК04
if (((PupsState >= 5) && (PupsState <= 9)) || (PupsState == 13) || (PupsState == 18) || (PupsState == 29) || (PupsState == 30))
  {
  MkoBufB[0][0] = 0x000F;
  Result = (Mbk04ReplyEnabled) ? (MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(4, 5, MKO_TX, 1), 0)) : (0);
  if (Result)
    {
    MkoBufB[0][0] = REPLY_MBK04_SHZK_WR_ERROR;
    if (MsgMask & IRQ2_MSG_ERROR) MkoXchgBn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_KPI, MKO_TX, 1), 0);
    return;
    }
  SleepMSec(10);
  }

// выдача квитанции "Нет КПИ" в МБК04
if ((PupsState == 10) || (PupsState == 11) || (PupsState == 12) || (PupsState == 14) || (PupsState == 16) || ((PupsState >= 23) && (PupsState <= 28)))
  {
  MkoBufB[0][0] = 0x000F;
  Result = (Mbk04ReplyEnabled) ? (MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(4, 7, MKO_TX, 1), 0)) : (0);
  if (Result)
    {
    MkoBufB[0][0] = REPLY_MBK04_SHZK_WR_ERROR;
    if (MsgMask & IRQ2_MSG_ERROR) MkoXchgBn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_KPI, MKO_TX, 1), 0);
    return;
    }
  SleepMSec(10);
  }

// прочие состояния ПУПС не обрабатываем

// сообщение "прерывание обработано"
MkoBufB[0][0] = REPLY_IRQ_02_DONE;
if (MsgMask & IRQ2_MSG_DONE) MkoXchgBn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_KPI, MKO_TX, 1), 0);
}
//================================================================================

#endif

