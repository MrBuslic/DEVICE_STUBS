
/*! \file IrqHandlerKoiUnit.c
 * \author Гапонов Р.В.
 * \date 2013.05.06
 * \version 1.0.0
 * \brief Модуль содержит функции обработки прерываний от аппаратуры, специфичных для комплекта аппаратуры КОИ
 */

#include "MainUnit.h"
//================================================================================

#ifdef SPO_KOI

/*! \fn void IrqHandler02 (void);
 * \brief Функция обработки прерываний от блока 732 (маска прерывания 0x0004)
 * \details Передает по МКО сообщение о прерывании (если разрешено в конфигурации СПО), затем производит
 * обработку прерывания в зависимости от состояния блока 732 (для детальной информации см. ТЗ на СПО).
 */

void IrqHandler02 (void)
{
int Result, Index, VmMask;
int DevMkoLine, DevMkoChan;
int BaseMkoLine, BaseMkoChan;
WORD Config732[9], Kpi732[8], VmState, FirstState, PupsState, KpiCurr[2], KpiGood[2];
UINT ShbkMask;
BYTE ShbkBuffer[32];

MSGS("IrqHandler02()");
SendIrqMessageToKPA(0x0004);

// параметры линий обмена
DevMkoLine = SpoConfig.DevMkoLine & 1;
DevMkoChan = SpoConfig.DevMkoChan & 1;

BaseMkoLine = SpoConfig.KpaMkoLine & 1;
BaseMkoChan = SpoConfig.KpaMkoChan & 1;

// пауза перед обработкой
SleepMSec(1);

// сброс запроса прерывания 732
if (sprIQClear(2) == ERROR)
  {
  MkoBufB[0][0] = REPLY_IRQ_02_DONE | REPLY_FLAG_ERROR;
  MkoBufB[0][1] = 1;
  MkoXchgBn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_KPI, MKO_TX, 2), 0);
  return;
  }

// чтение текущей конфигурации 732
Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(2, 17, MKO_RX, 9), 0);
if (Result)
  {
  MkoBufB[0][0] = REPLY_732_INFO_ERROR;
  MkoXchgBn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_KPI, MKO_TX, 1), 0);
  return;
  }
SleepMSec(10);

// буфер с конфигурацией 732
memmove(&Config732, &MkoBufB[0][0], sizeof(Config732));

// сброс всех ПУПС 732 широковещательной командой
if (!SpoConfig.LkaOpoVersion)
  {
  MkoBufB[0][0] = 0xA83F;
  Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(2, 17, MKO_TX, 1), 0);
  if (Result)
    {
    MkoBufB[0][0] = REPLY_732_PUPS_WR_ERROR;
    MkoXchgBn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_KPI, MKO_TX, 1), 0);
    return;
    }
  SleepMSec(10);
  }

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
  MkoXchgBn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_KPI, MKO_TX, 1), 0);
  return;
  }

// сообщение "Конфигурация ВЧМ 732"
MkoBufB[0][0] = REPLY_732_VM_STATE;
MkoBufB[0][1] = VmMask;
MkoXchgBn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_KPI, MKO_TX, 2), 0);

// проверка битов ошибок в ВЧМ 732
for (Index = 0; Index < 4; Index++)
  {
  VmState = Config732[Index + 5];
  if (VmState & 0x0100) // канал ВЧМ включен
    {
    if (VmState & 0x3880) // есть биты ошибок
      {
      MkoBufB[0][0] = REPLY_732_VM_STATE_ERROR;
      memmove(&MkoBufB[0][1], &Config732[5], 4 * sizeof(WORD));
      MkoXchgBn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_KPI, MKO_TX, 5), 0);
      return;      
      }
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
      PupsState = VmState & 0x003F;
      }
    if (PupsState != (VmState & 0x003F))
      {
      // сообщение о ошибке мажорирования ПУПС-ов 
      MkoBufB[0][0] = REPLY_732_PUPS_STATE_ERROR;
      memmove(&MkoBufB[0][1], &Config732[5], 4 * sizeof(WORD));
      MkoXchgBn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_KPI, MKO_TX, 5), 0);
      return;      
      }
    }
  }

// выдаем в КПА достоверный ПУПС ВЧМ 732
MkoBufB[0][0] = REPLY_732_PUPS_OK;
MkoBufB[0][1] = PupsState;
MkoXchgBn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_KPI, MKO_TX, 2), 0);

// накапливаем список достоверных ПУПС-ов
if (B732PupsListSize < B732_PUPS_LIST_SIZE)
  {
  B732PupsList[B732PupsListSize++] = PupsState;
  }

// выдача СЧБК
if (PupsState == 1)
  {
  // считываем код СЧБК (32-бит)
  MkoXchgBn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_SHBK, MKO_RX, 2), 0);
  memmove(&ShbkMask, &MkoBufB[0][0], sizeof(ShbkMask));

  // передаем СЧБК в 732
  Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(2, 21, MKO_TX, 2), 0);
  if (Result)
    {
    MkoBufB[0][0] = REPLY_732_SHBK_WR_ERROR;
    MkoXchgBn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_KPI, MKO_TX, 1), 0);
    return;
    }
  SleepMSec(10);
  
  // передаем квитанцию "Да СЧЗК" в МБК04
  MkoBufB[0][0] = 0x00F0;
  Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(4, 5, MKO_TX, 1), 0);
  if (Result)
    {
    MkoBufB[0][0] = REPLY_MBK04_SHZK_WR_ERROR;
    MkoXchgBn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_KPI, MKO_TX, 1), 0);
    return;
    }
  SleepMSec(10);

  // передаем СЧБК в МБК04
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
      MkoXchgBn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_KPI, MKO_TX, 1), 0);
      return;
      }
    }

  // сообщение о выданном СЧБК
  MkoBufB[0][0] = REPLY_732_SHBK_OUT;
  MkoXchgBn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_KPI, MKO_TX, 1), 0);

  // сообщение "прерывание обработано"
  MkoBufB[0][0] = REPLY_IRQ_02_DONE;
  MkoXchgBn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_KPI, MKO_TX, 1), 0);
  return;
  }

// выдача квитанции "Да СЧЗК" в МБК04
if (PupsState == 2)
  {
  MkoBufB[0][0] = 0x00F0;
  Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(4, 5, MKO_TX, 1), 0);
  if (Result)
    {
    MkoBufB[0][0] = REPLY_MBK04_SHZK_WR_ERROR;
    MkoXchgBn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_KPI, MKO_TX, 1), 0);
    return;
    }
  SleepMSec(10);
  }

// выдача квитанции "Да КПИ" в МБК04
if (PupsState == 3)
  {
  MkoBufB[0][0] = 0x00F0;
  Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(4, 7, MKO_TX, 1), 0);
  if (Result)
    {
    MkoBufB[0][0] = REPLY_MBK04_KPI_WR_ERROR;
    MkoXchgBn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_KPI, MKO_TX, 1), 0);
    return;
    }
  SleepMSec(10);
  }

// выдача КПИ
if ((PupsState == 4) || (PupsState == 15))
  {
  MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(2, 21, MKO_RX, 8), 0);
  SleepMSec(10);

  // буфер данных КПИ 732
  memmove(&Kpi732, &MkoBufB[0][0], sizeof(Kpi732));
  
  // сравнение КПИ 732
  FirstState = 1;
  KpiGood[0] = KpiGood[1] = 0;

  for (Index = 0; Index < 4; Index++)
    {
    VmState = Config732[Index + 5];
    KpiCurr[0] = Kpi732[Index];
    KpiCurr[1] = Kpi732[Index + 4];
    if (VmState & 0x0100) // канал ВЧМ включен
      {
      if (FirstState)
        {
        FirstState = 0;
        KpiGood[0] = KpiCurr[0];
        KpiGood[1] = KpiCurr[1];
        }
      if ((KpiGood[0] != KpiCurr[0]) || (KpiGood[1] != KpiCurr[1]))
        {
        // сообщение о ошибке мажорирования КПИ
        memmove(&MkoBufB[0][0], &Kpi732, sizeof(Kpi732));
        MkoXchgBn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_KPI+1, MKO_TX, 8), 0);

        MkoBufB[0][0] = REPLY_732_KPI2_SYNC_ERROR;
        MkoBufB[0][1] = sizeof(Kpi732);
        MkoBufB[0][2] = CalcCrc16(&Kpi732, sizeof(Kpi732));
        MkoXchgBn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_KPI, MKO_TX, 3), 0);
        return;      
        }
      }
    }

  // сравнение КПИ успешно
  MkoBufB[0][0] = KpiGood[0];
  MkoBufB[0][1] = KpiGood[1];
  MkoXchgBn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_KPI+1, MKO_TX, 2), 0);

  MkoBufB[0][0] = REPLY_732_KPI2_OK;
  MkoBufB[0][1] = sizeof(KpiGood);
  MkoBufB[0][2] = CalcCrc16(&KpiGood, sizeof(KpiGood));
  MkoXchgBn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_KPI, MKO_TX, 3), 0);

  // передаем квитанцию "Да КПИ" на МБК04
  MkoBufB[0][0] = 0x00F0;
  Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(4, 7, MKO_TX, 1), 0);
  if (Result)
    {
    MkoBufB[0][0] = REPLY_MBK04_KPI_WR_ERROR;
    MkoXchgBn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_KPI, MKO_TX, 1), 0);
    return;
    }
  SleepMSec(10);

  // сообщение "прерывание обработано"
  MkoBufB[0][0] = REPLY_IRQ_02_DONE;
  MkoXchgBn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_KPI, MKO_TX, 1), 0);
  return;      
  }

// выдача квитанции "Нет СЧЗК" в МБК04
if ((PupsState >= 5) && (PupsState <= 9))
  {
  MkoBufB[0][0] = 0x000F;
  Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(4, 5, MKO_TX, 1), 0);
  if (Result)
    {
    MkoBufB[0][0] = REPLY_MBK04_SHZK_WR_ERROR;
    MkoXchgBn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_KPI, MKO_TX, 1), 0);
    return;
    }
  SleepMSec(10);
  }

// выдача квитанции "Нет КПИ" в МБК04
if ((PupsState == 10) || (PupsState == 11) || (PupsState == 12) || (PupsState == 14) || (PupsState == 16))
  {
  MkoBufB[0][0] = 0x000F;
  Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(4, 7, MKO_TX, 1), 0);
  if (Result)
    {
    MkoBufB[0][0] = REPLY_MBK04_SHZK_WR_ERROR;
    MkoXchgBn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_KPI, MKO_TX, 1), 0);
    return;
    }
  SleepMSec(10);
  }

// прочие состояния ПУПС не обрабатываем

// сообщение "прерывание обработано"
MkoBufB[0][0] = REPLY_IRQ_02_DONE;
MkoXchgBn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_KPI, MKO_TX, 1), 0);
}
//================================================================================

#endif

