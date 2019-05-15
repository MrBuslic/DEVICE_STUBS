
/*! \file IrqHandlerUnit.c
 * \author Гапонов Р.В.
 * \date 2012.10.28
 * \version 1.0.0
 * \brief Модуль содержит функции обработки прерываний от аппаратуры
 */

#include "MainUnit.h"
#include "BaloiUnit_v11.h"
//================================================================================

/*! \fn void HandleIrqBA (int AChanMask);
 * \brief Функция вызывается при возникновении разрешенного прерывания от БА
 * \param [in] AChanMask - маска прерываний от аппаратуры (установленные биты соответствуют наличию прерывания)
 * \details Функция проверяет состояние прерываний на корректность, затем вызывает функции обработки прерываний от конкретных устройств.
 * Вызывается из задачи, запускаемой ОПО при возникновении прерываний от БА.
 */

void HandleIrqBA (int AChanMask)
{
int BaseMkoLine, BaseMkoChan;
int Count, ChanMask, Mask;

#if (1)
if (SpoConfig.EnableMultiIrqControl)
  {
  // вычисляем число пришедших прерываний
  ChanMask = AChanMask & (~KPA_IRQ_MASK);
  Mask = 0x8000;
  Count = 0;

  while (Mask)
    {
    if (ChanMask & Mask) Count++;
    Mask >>= 1;
    }

  // если прерываний > 1 = ошибка
  if (Count > 1)
    {
    // параметры линий обмена с КПА
    BaseMkoLine = SpoConfig.KpaMkoLine & 1;
    BaseMkoChan = SpoConfig.KpaMkoChan & 1;

    ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_IRQ_ERROR, KPA_SUBADDR_IRQ, &AChanMask, 2);
    return;
    }
  }
#endif

if (AChanMask & 0x0002) IrqHandlerTable[0x01](); // макс приоритет обработки - прерываниям от КПА

if (AChanMask & 0x2000) IrqHandlerTable[0x0D]();

if (AChanMask & 0x0001) IrqHandlerTable[0x00]();
if (AChanMask & 0x0004) IrqHandlerTable[0x02]();
if (AChanMask & 0x0008) IrqHandlerTable[0x03]();

if (AChanMask & 0x0010) IrqHandlerTable[0x04]();
if (AChanMask & 0x0020) IrqHandlerTable[0x05]();
if (AChanMask & 0x0040) IrqHandlerTable[0x06]();
if (AChanMask & 0x0080) IrqHandlerTable[0x07]();

if (AChanMask & 0x0100) IrqHandlerTable[0x08]();
if (AChanMask & 0x0200) IrqHandlerTable[0x09]();
if (AChanMask & 0x0400) IrqHandlerTable[0x0A]();
if (AChanMask & 0x0800) IrqHandlerTable[0x0B]();

if (AChanMask & 0x1000) IrqHandlerTable[0x0C]();
if (AChanMask & 0x4000) IrqHandlerTable[0x0E]();
if (AChanMask & 0x8000) IrqHandlerTable[0x0F]();
}
//================================================================================

/*! \fn void IrqHandler00 (void);
 * \brief Функция обработки прерываний от АСН (маска прерывания 0x0001)
 * \details Передает по МКО сообщение о прерывании (если разрешено в конфигурации СПО), затем производит
 * расчет числа нормальных, лишних и потерянных прерываний от АСН (всегда пока разрешено прерывание)
 */

void IrqHandler00 (void)
{
int Result, Time1, Time2, AsnTime;
int BaseMkoLine, BaseMkoChan;
int DevMkoLine, DevMkoChan;
int PrevSec, PrevMin, PrevHrs, PrevDay, PrevMnt, PrevYrs;
int CurrSec, CurrMin, CurrHrs, CurrDay, CurrMnt, CurrYrs;

MSGS("IrqHandler00()");
SendIrqMessageToKPA(0x0001);

if (AsnStartTimer)
  {
  // пропускаем первое прерывание (вне сетки времени АСН)
  AsnStartTimer--;
  }
else
  {
  // общая сумма прерываний от АСН
  AsnIrqCount++;

  // параметры линий обмена с АСН
  DevMkoLine = SpoConfig.DevMkoLine & 1;
  DevMkoChan = SpoConfig.DevMkoChan & 1;

  BaseMkoLine = SpoConfig.KpaMkoLine & 1;
  BaseMkoChan = SpoConfig.KpaMkoChan & 1;

  // накапливаем список интервалов времени между прерываниями
  if (AsnTimeListSize < ASN_TIME_LIST_SIZE)
    {
    if (!AsnTimeListSize)
      {
      Result = 0xFFFF;
      }
    else
      {
      Result = DeltaTimeMSec((sysTime_t*) &AsnTimeIrqPrev, (sysTime_t*) &IrqTime);
      if (Result > 0xFFFF) Result = 0xFFFF;
      }
    AsnTimeList[AsnTimeListSize++] = Result;
    AsnTimeIrqPrev = IrqTime;
    }

  // считываем данные АСН
  Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(3, 9, MKO_RX, 24), 0);

  // размещаем данные по времени в нарастающем порядке
  MkoBufB[0][0] = MkoBufB[0][2];
  MkoBufB[0][1] = MkoBufB[0][1];
  memmove(&AsnTime, &MkoBufB[0][0], 4);

  // проверяем полученное время АСН
  if (!Result)
    {
    CurrSec = (AsnTime >> 0) & 0x3F;
    CurrMin = (AsnTime >> 6) & 0x3F;
    CurrHrs = (AsnTime >> 12) & 0x1F;
    CurrDay = (AsnTime >> 17) & 0x1F;
    CurrMnt = (AsnTime >> 22) & 0x0F;
    CurrYrs = (AsnTime >> 26) & 0x3F;

    if ((CurrSec > 59) || (CurrMin > 59) || (CurrHrs > 23) ||
        (CurrDay < 1) || (CurrDay > 31) || (CurrMnt < 1) || (CurrMnt > 12)) AsnParamFailCount++;
    }

  if (AsnWaitForFirstIrq)
    {
    // первое прерывание АСН (исходное)
    AsnWaitForFirstIrq = 0;

    AsnEnabledZone = IrqTime;
    AsnEnabledZone.time_sec++;
    AsnIrqAlreadySet = 0;

    // сохраняем начальное время АСН (если считано без сбоев)
    if (!Result) { AsnStatusBeginTime = AsnTime; AsnStatusPrevTime = AsnTime; }
    else AsnMkoFailCount++;
    }
  else
    {
    if (AsnStatusBeginTime)
      {
      if (!Result)
        {
        PrevSec = (AsnStatusPrevTime >> 0) & 0x3F;
        PrevMin = (AsnStatusPrevTime >> 6) & 0x3F;
        PrevHrs = (AsnStatusPrevTime >> 12) & 0x1F;
        PrevDay = (AsnStatusPrevTime >> 17) & 0x1F;
        PrevMnt = (AsnStatusPrevTime >> 22) & 0x0F;
        PrevYrs = (AsnStatusPrevTime >> 26) & 0x3F;

        if ((++PrevSec) >= 60)
          {
          PrevSec = 0;
          if ((++PrevMin) >= 60)
            {
            PrevMin = 0;
            if ((++PrevHrs) >= 24)
              {
              PrevHrs = 0;
              }
            }
          }

        CurrSec = (AsnTime >> 0) & 0x3F;
        CurrMin = (AsnTime >> 6) & 0x3F;
        CurrHrs = (AsnTime >> 12) & 0x1F;

        if ((PrevSec != CurrSec) || (PrevMin != CurrMin) || (PrevHrs != CurrHrs)) AsnTimerFailCount++;

        AsnStatusPrevTime = AsnTime;
        }
      else AsnMkoFailCount++;
      }
    else
      {
      // еще раз пробуем сохранить начальное время АСН (если считано без сбоев)
      if (!Result) { AsnStatusBeginTime = AsnTime; AsnStatusPrevTime = AsnTime; }
      else AsnMkoFailCount++;
      }

    Time1 = IrqTime.time_sec * 1000 + IrqTime.time_msec;
    Time2 = AsnEnabledZone.time_sec * 1000 + AsnEnabledZone.time_msec - AsnPreDeltaTime;
    if (Time1 < Time2) AsnExcIrqCount++;
    else
      {
      Time2 = AsnEnabledZone.time_sec * 1000 + AsnEnabledZone.time_msec + AsnPostDeltaTime;
      if (Time1 > Time2) AsnExcIrqCount++;
      else
        {
        if (!AsnIrqAlreadySet) AsnIrqAlreadySet = 1;
        else AsnExcIrqCount++;
        }
      }
    }
  }
}
//================================================================================

void IrqHandler00TaskDeltaTime (void)
{
AsnSecondIrqTime = IrqTime;
}
//================================================================================

/*! \fn void IrqHandler01 (void);
 * \brief Функция обработки прерываний от КПА (маска прерывания 0x0002)
 * \details Передает по МКО сообщение о прерывании (если разрешено в конфигурации СПО), затем производит
 * считывание, проверку целостности и выполнение команд от КПА. Принятая от КПА команда помещается в буфер
 * KpaCommand для передачи функциям обработки команд.
 */

void IrqHandler01 (void)
{
int Result;
int BaseMkoLine, BaseMkoChan;
BYTE Command;
KPA_CMD_HANDLER Handler;
WORD Crc16, Buffer[8];

MSGS("IrqHandler01()");
SendIrqMessageToKPA(0x0002);

// параметры линий обмена с КПА
BaseMkoLine = SpoConfig.KpaMkoLine & 1;
BaseMkoChan = SpoConfig.KpaMkoChan & 1;

Result = MkoXchgBn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_CMD, MKO_RX, KPA_CMD_LEN), 0);
if (Result)
  {
  MSGS("IrqHandler01(): Сбой при считывании команды КПА: %d", Result);

  MkoBufB[0][0] = DECODE_XCHG_ERROR(Result);
  MkoXchgBn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_IRQ, MKO_TX, 1), 0);
  return;
  }

if (MkoMsgResB[0].Len != KPA_CMD_LEN)
  {
  MSGS("IrqHandler01(): Ошибка при считывании команды КПА: размер данных < KPA_CMD_LEN, = %d", MkoMsgResB00.Len);

  // передаем результат обмена с КПА при сбоях передачи
  memmove(&Buffer[0], &MkoPackResB, 4);
  memmove(&Buffer[2], &MkoMsgResB[0], 8);

  memmove(&MkoBufB[0][0], &Buffer[0], 12);
  MkoXchgBn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_IRQ + 1, MKO_TX, 6), 0);

  MkoBufB[0][0] = REPLY_NO_DATA_ERROR;
  MkoBufB[0][1] = 12;
  MkoBufB[0][2] = CalcCrc16(&Buffer[0], 12);
  MkoXchgBn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_IRQ, MKO_TX, 3), 0);
  return;
  }

memmove(&KpaCommand, &MkoBufB[0][0], sizeof(KpaCommand));
Crc16 = CalcCrc16(&KpaCommand, 6 * sizeof(WORD));

if (Crc16 != KpaCommand[6])
  {
  MSGS("IrqHandler01(): Ошибка при считывании команды КПА: код CRC не совпадает: получено = 0x%04x, реально = 0x%04x", KpaCommand[6], Crc16);

  MkoBufB[0][0] = REPLY_CMD_CRC_ERROR;
  MkoXchgBn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_IRQ, MKO_TX, 1), 0);
  return;
  }

Command = KpaCommand[0] & 0xFF;
MSGS("IrqHandler01(): Получена команда от КПА = %d (= 0x%04x)", Command, KpaCommand[0]);

Handler = CmdHandlerTable[Command];
Handler(BaseMkoLine, BaseMkoChan);
}
//================================================================================

/*! \fn void IrqHandler03 (void);
 * \brief Функция-заглушка для прерывания #3 (маска прерывания 0x0008)
 * \details Передает по МКО сообщение о прерывании (если разрешено в конфигурации СПО)
 */

// функция-заглушка для прерывания #3 (маска прерывания 0x0008)
// передает по МКО сообщение о прерывании (если разрешено в конфигурации СПО)
// входные параметры:
// (глобальная переменная) структура конфигурации СПО
// возвращаемые значения:
// нет

void IrqHandler03 (void)
{
MSGS("IrqHandler03()");
SendIrqMessageToKPA(0x0008);
}
//================================================================================

/*! \fn void IrqHandler04 (void);
 * \brief Функция обработки прерываний от блока МБК04 (маска прерывания 0x0010)
 * \details Передает по МКО сообщение о прерывании (если разрешено в конфигурации СПО), затем производит
 * обработку прерывания в зависимости от состояния блока МБК04 (для детальной информации см. ТЗ на СПО)
 */

void IrqHandler04 (void)
{
int Result, Pos, MsgSize, EvenMode, FlagMode;
int BaseMkoLine, BaseMkoChan;
int DevMkoLine, DevMkoChan;
WORD Cmd1, Cmd2, Reply;

MSGS("IrqHandler04()");
SendIrqMessageToKPA(0x0010);

// параметры линий обмена с КПА
BaseMkoLine = SpoConfig.KpaMkoLine & 1;
BaseMkoChan = SpoConfig.KpaMkoChan & 1;

// параметры линий обмена с БА
DevMkoLine = SpoConfig.DevMkoLine & 1;
DevMkoChan = SpoConfig.DevMkoChan & 1;

// параметры передачи
EvenMode = SpoConfig.Irq4EvenMode & 1;
MsgSize = SpoConfig.Mbk04MessageSize;

if ((SpoConfig.Mbk04DoubleBuffered) && (SpoConfig.Mbk04DataFlagAfterIrq))
  {
  // передаем данные в буфер МКО, флаг данных идет первым
  memmove(&MkoBufB[0], &CbkDataImit[32], 1 * sizeof(MKO_BUFFER));

  // параметры передачи для режимов чет/нечет
  CbkDataImit[32][0] = (EvenMode == 0) ? (0x01) : (0x80); // пакет с флагом данных
  Cmd1 = (EvenMode == 0) ? (MKO_CMD(4, 3, MKO_TX, MsgSize)) : (MKO_CMD(4, 4, MKO_TX, MsgSize));  // команда на передачу сообщения в МБК04
  Cmd2 = MKO_CMD(4, 8, MKO_TX, 1);  // команда на передачу флага в МБК04
  Reply = (EvenMode == 0) ? (REPLY_BUF1_WR_ERROR_MBK04) : (REPLY_BUF2_WR_ERROR_MBK04);  // маркер на случай ошибок

  // ставим маркеры типа и номера пакета
  Pos = SpoConfig.Mbk04MarkerPosition;
  CbkDataImit[0][Pos + 0] = (SpoConfig.Irq4EvenMode & 1) + 1;  // маркер чет/нечет номера пакета
  CbkDataImit[0][Pos + 1]++;  // текущий номер пакета

  // передаем данные в буфер МКО
  memmove(&MkoBufB[1], &CbkDataImit[0], 1 * sizeof(MKO_BUFFER));
  memmove(&MkoBufB[32], &CbkDataImit[1], 1 * sizeof(MKO_BUFFER));

  // режим передачи
  FlagMode = MBK04_FLAG_BEFORE_DATA;
  }
else
  {
  // параметры передачи для режимов чет/нечет
  CbkDataImit[32][0] = (EvenMode == 0) ? (0x01) : (0x80); // пакет с флагом данных
  Cmd1 = (EvenMode == 0) ? (MKO_CMD(4, 3, MKO_TX, MsgSize)) : (MKO_CMD(4, 4, MKO_TX, MsgSize));  // команда на передачу сообщения в МБК04
  Cmd2 = MKO_CMD(4, 8, MKO_TX, 1);  // команда на передачу флага в МБК04
  Reply = (EvenMode == 0) ? (REPLY_BUF1_WR_ERROR_MBK04) : (REPLY_BUF2_WR_ERROR_MBK04);  // маркер на случай ошибок

  // ставим маркеры типа и номера пакета
  Pos = SpoConfig.Mbk04MarkerPosition;
  CbkDataImit[0][Pos + 0] = (SpoConfig.Irq4EvenMode & 1) + 1;  // маркер чет/нечет номера пакета
  CbkDataImit[0][Pos + 1]++;  // текущий номер пакета

  // передаем данные в буфер МКО, флаг данных идет последним
  memmove(&MkoBufB[0], &CbkDataImit[0], 1 * sizeof(MKO_BUFFER));
  memmove(&MkoBufB[32], &CbkDataImit[32], 1 * sizeof(MKO_BUFFER));

  // режим передачи
  FlagMode = MBK04_FLAG_AFTER_DATA;
  }

Result = MkoXchgBnBlock(DevMkoLine, DevMkoChan, Cmd1, Cmd2, SpoConfig.Mbk04PacketSize, FlagMode);
if (Result)
  {
  MSGS("IrqHandler04(): Сбой при передаче пакета в МБК04, code = %d", Result);

  // отключение прерываний МБК04
  SpoConfig.EnabledIrqMask &= (~0x0010);
  sprIMaskVec(~SpoConfig.EnabledIrqMask);

  // сбрасываем флаг "функция имитации активна"
  SpoConfig.EnableCbkDataImit = 0;

  // смена текущего статуса режима
  SpoConfig.CbkDataImitStatus = STATE_ABORT;

  // сообщение о сбое передачи в МБК04
  MkoBufB[0][0] = Reply;
  MkoXchgBn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_IRQ, MKO_TX, 1), 0);
  return;
  }

if (SpoConfig.Mbk04SendExtMessage)
  {
  // выдаем дополнительное сообщение на МБК04 (которое должно получить отбойник "абонент занят")
  MkoXchgBnBlock(DevMkoLine, DevMkoChan, Cmd1, Cmd2, 1, MBK04_FLAG_NONE);
  }

// переключение чет/нечет для номера пакета
SpoConfig.Irq4EvenMode++;
// смена размера сообщения, если разрешено
if (SpoConfig.Mbk04IncMessageSize)
  {
  if (++SpoConfig.Mbk04MessageSize > 32) SpoConfig.Mbk04MessageSize = 1;
  }
}
//================================================================================

/*! \fn void IrqHandler05 (void);
 * \brief Функция обработки прерываний от блока БЭЧ (маска прерывания 0x0020)
 * \details Передает по МКО сообщение о прерывании (если разрешено в конфигурации СПО), затем производит
 * обработку прерывания в зависимости от состояния блока БЭЧ (для детальной информации см. ТЗ на СПО)
 */

void IrqHandler05 (void)
{
int Result, Time1, Time2;
int BaseMkoLine, BaseMkoChan;
int DevMkoLine, DevMkoChan;
WORD Config[10];

MSGS("IrqHandler05()");
SendIrqMessageToKPA(0x0020);

// параметры линий обмена с КПА
BaseMkoLine = SpoConfig.KpaMkoLine & 1;
BaseMkoChan = SpoConfig.KpaMkoChan & 1;

DevMkoLine = SpoConfig.DevMkoLine & 1;
DevMkoChan = SpoConfig.DevMkoChan & 1;

// чтение текущей конфигурации БЭЧ
Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(5, 1, MKO_RX, 10), 0);

// текущая конфигурация блоков БЭЧ
memmove(&Config, &MkoBufB[0], sizeof(Config));

if (!BehControlEnabled)
  {
  // при ошибках обмена выдаем сообщение о сбое
  if (Result)
    {
    MkoBufB[0][0] = REPLY_IRQ_05_ERROR;
    MkoXchgBn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_IRQ, MKO_TX, 1), 0);
    return;
    }
  SleepMSec(10);

  if (Config[1] & 0x03)
    {
    // если есть признаки ФИНИК или ФШВ - выдаем массив конфигурации
    MkoXchgBn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_IRQ + 1, MKO_TX, 10), 0);

    MkoBufB[0][0] = REPLY_IRQ_05_DONE;
    MkoBufB[0][1] = sizeof(Config);
    MkoBufB[0][2] = CalcCrc16(&Config, sizeof(Config));
    MkoXchgBn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_IRQ, MKO_TX, 3), 0);
    }
  else
    {
    // если прерывание было по другой причине - выдаем пустое сообщение
    MkoBufB[0][0] = REPLY_IRQ_05_DONE;
    MkoBufB[0][1] = 0;
    MkoBufB[0][2] = CalcCrc16(&Config, 0);
    MkoXchgBn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_IRQ, MKO_TX, 3), 0);
    }
  }
else
  {
  if (BehStartTimer)
    {
    // пропускаем первое прерывание (вне сетки времени БЭЧ)
    BehStartTimer--;
    }
  else
    {
    // общая сумма прерываний от БЭЧ
    BehIrqCount++;

    if (BehWaitForFirstIrq)
      {
      // первое прерывание БЭЧ (исходное)
      BehWaitForFirstIrq = 0;

      BehEnabledZone = IrqTime;
      BehEnabledZone.time_sec++;
      BehIrqAlreadySet = 0;

      if (Result) BehMkoFailCount++;
      }
    else
      {
      Time1 = IrqTime.time_sec * 1000 + IrqTime.time_msec;
      Time2 = BehEnabledZone.time_sec * 1000 + BehEnabledZone.time_msec - BehPreDeltaTime;
      if (Time1 < Time2) BehExcIrqCount++;
      else
        {
        Time2 = BehEnabledZone.time_sec * 1000 + BehEnabledZone.time_msec + BehPostDeltaTime;
        if (Time1 > Time2) BehExcIrqCount++;
        else
          {
          if (!BehIrqAlreadySet) BehIrqAlreadySet = 1;
          else BehExcIrqCount++;
          }
        }

      if (BehFhvFlag) BehFhvTmpCount++;
      if (BehFinikFlag) BehFinikTmpCount++;
      if (Result) BehMkoFailCount++;

      if (!Result)
        {
        // если в конфигурации БЭЧ установлен флаг ФШВ
        if (Config[1] & 0x01)
          {
          if (!BehFhvFlag)
            {
            BehFhvFlag = 1;
            }
          else
            {
            if (BehFhvTmpCount < BehFhvPeriod) BehExcFhvIrqCount++;
            if (BehFhvTmpCount > BehFhvPeriod) BehMissFhvIrqCount++;
            }
          BehFhvIrqCount++;
          BehFhvTmpCount = 0;
          }

        // если в конфигурации БЭЧ установлен флаг ФИНИК
        if (Config[1] & 0x02)
          {
          if (!BehFinikFlag)
            {
            BehFinikFlag = 1;
            }
          else
            {
            if (BehFinikTmpCount < BehFinikPeriod) BehExcFinikIrqCount++;
            if (BehFinikTmpCount > BehFinikPeriod) BehMissFinikIrqCount++;
            }
          BehFinikIrqCount++;
          BehFinikTmpCount = 0;
          }
        }
      }
    }
  }
}
//================================================================================

/*! \fn void IrqHandler06 (void);
 * \brief Функция обработки прерываний от блока 733 (маска прерывания 0x0040)
 * \details Передает по МКО сообщение о прерывании (если разрешено в конфигурации СПО), затем производит
 * обработку прерывания в зависимости от состояния блока 733 (для детальной информации см. ТЗ на СПО)
 */

void IrqHandler06 (void)
{
int Result, Mode, VmMask, VmCount, Index;
int BaseMkoLine, BaseMkoChan;
int DevMkoLine, DevMkoChan;
WORD Vm733[4], PUPS;
char State;

MSGS("IrqHandler06()");
SendIrqMessageToKPA(0x0040);

// параметры линий обмена с КПА
BaseMkoLine = SpoConfig.KpaMkoLine & 1;
BaseMkoChan = SpoConfig.KpaMkoChan & 1;

DevMkoLine = SpoConfig.DevMkoLine & 1;
DevMkoChan = SpoConfig.DevMkoChan & 1;

// режим обработки прерывания
Mode = SpoConfig.Irq6Mode;

// режим #1 - только сообщение
if (Mode == 1)
  {
  MkoBufB[0][0] = REPLY_IRQ_STATE;
  MkoBufB[0][1] = 0x0040;
  MkoXchgBn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_IRQ, MKO_TX, 2), 0);
  return;
  }

// режим #2 - обработка ПУПСов ВЧМ 733
if (Mode == 2)
  {
  // пауза на установку ПУПС
  SleepMSec(500);

  // чтение конфигурации 733
  Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(6, 17, MKO_RX, 6), 0);
  if (Result)
    {
    MkoBufB[0][0] = REPLY_733_INFO_ERROR;
    MkoXchgBn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_IRQ, MKO_TX, 1), 0);
    return;
    }

  // сохраняем состояние ВЧМ
  memmove(&Vm733, &MkoBufB[0][2], sizeof(Vm733));

  // команда на сброс всех ПУПС
  MkoBufB[0][0] = 0xA83F;
  Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(6, 17, MKO_TX, 1), 0);
  if (Result)
    {
    MkoBufB[0][0] = REPLY_733_PUPS_CLR_ERROR;
    MkoXchgBn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_IRQ, MKO_TX, 1), 0);
    return;
    }

  // вычисляем число/маску ВЧМ текущей конфигурации
  VmMask = VmCount = 0;
  for (Index = 0; Index < 4; Index++)
    {
    if (Vm733[Index] & 0x0100) { VmMask |= (1 << Index); VmCount++; }
    }

  // если рабочих ВЧМ меньше 2 - ошибка
  if (VmCount < 2)
    {
    MkoBufB[0][0] = REPLY_733_WRONG_CONFIG;
    MkoBufB[0][1] = VmMask;
    MkoXchgBn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_IRQ, MKO_TX, 2), 0);
    return;
    }

  // сообщаем маску текущей конфигурации ВЧМ
  MkoBufB[0][0] = REPLY_733_VM_CONFIG;
  MkoBufB[0][1] = VmMask;
  MkoXchgBn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_IRQ, MKO_TX, 2), 0);

  // ищем сбойные ВЧМ
  State = 0;
  for (Index = 0; Index < 4; Index++)
    {
    // проверяем биты ошибок только для включенных ВЧМ
    if (VmMask & (1 << Index))
      {
      if (Vm733[Index] & 0x3880) { State = 1; break; }
      }
    }

  // если нашли - сообщаем и выдаем их конфигурацию
  if (State)
    {
    MkoBufB[0][0] = REPLY_733_WRONG_VM_STATE;
    MkoBufB[0][1] = Vm733[0];
    MkoBufB[0][2] = Vm733[1];
    MkoBufB[0][3] = Vm733[2];
    MkoBufB[0][4] = Vm733[3];
    MkoXchgBn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_IRQ, MKO_TX, 5), 0);
    return;
    }

  // сравниваем ПУПС рабочих ВЧМ
  State = PUPS = 0;
  for (Index = 0; Index < 4; Index++)
    {
    if (Vm733[Index] & 0x0100)
      {
      if (!State) { PUPS = Vm733[Index] & 0x003F; State = 1; }
      if (PUPS != (Vm733[Index] & 0x003F))
        {
        // нашли несовпадение ПУПС
        MkoBufB[0][0] = REPLY_733_WRONG_PUPS_STATE;
        MkoBufB[0][1] = Vm733[0];
        MkoBufB[0][2] = Vm733[1];
        MkoBufB[0][3] = Vm733[2];
        MkoBufB[0][4] = Vm733[3];
        MkoXchgBn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_IRQ, MKO_TX, 5), 0);
        return;
        }
      }
    }

  // сообщаем о достоверном ПУПС
  MkoBufB[0][0] = REPLY_733_GOOD_PUPS;
  MkoBufB[0][1] = PUPS;
  MkoXchgBn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_IRQ, MKO_TX, 2), 0);
  if (Result) ;

  // завершили обработку
  MkoBufB[0][0] = REPLY_IRQ_06_DONE;
  MkoXchgBn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_IRQ, MKO_TX, 1), 0);
  return;
  }

if (Mode == 3)
  {
  // считываем состояние УПИ 733
  Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(6, 1, MKO_RX, 4), 0);
  if (Result)
    {
    MkoBufB[0][0] = REPLY_733_UPI_READ_ERROR;
    MkoXchgBn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_IRQ, MKO_TX, 1), 0);
    return;
    }

  // сохраняем состояние УПИ 733
  memmove(&Vm733, &MkoBufB[0], sizeof(Vm733));

  // передаем состояние УПИ 733
  MkoBufB[0][0] = REPLY_733_UPI_INFO;
  memmove(&MkoBufB[0][1], &Vm733, sizeof(Vm733));
  MkoXchgBn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_IRQ, MKO_TX, 5), 0);

  // завершили обработку
  MkoBufB[0][0] = REPLY_IRQ_06_DONE;
  MkoXchgBn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_IRQ, MKO_TX, 1), 0);
  return;
  }
}
//================================================================================

/*! \fn void IrqHandler07 (void);
 * \brief Функция-заглушка для прерывания #7 (маска прерывания 0x0080)
 * \details Передает по МКО сообщение о прерывании (если разрешено в конфигурации СПО)
 */

void IrqHandler07 (void)
{
MSGS("IrqHandler07()");
SendIrqMessageToKPA(0x0080);
}
//================================================================================

/*! \fn void IrqHandler08 (void);
 * \brief Функция-заглушка для прерывания #8 (маска прерывания 0x0100)
 * \details Передает по МКО сообщение о прерывании (если разрешено в конфигурации СПО)
 */

void IrqHandler08 (void)
{
MSGS("IrqHandler08()");
SendIrqMessageToKPA(0x0100);
}
//================================================================================

/*! \fn void IrqHandler09 (void);
 * \brief Функция-заглушка для прерывания #9 (маска прерывания 0x0200)
 * \details Передает по МКО сообщение о прерывании (если разрешено в конфигурации СПО)
 */

void IrqHandler09 (void)
{
MSGS("IrqHandler09()");
SendIrqMessageToKPA(0x0200);
}
//================================================================================

/*! \fn void IrqHandler0A (void);
 * \brief Функция-заглушка для прерывания #10 (маска прерывания 0x0400)
 * \details Передает по МКО сообщение о прерывании (если разрешено в конфигурации СПО)
 */

mkoPack_t Irq10Pack;
mkoPackRes_t Irq10PackRes;

mkoPackMsg_t Irq10PackMsg[18];
mkoMsgRes_t Irq10PackMsgRes[18];
mkoPackMsg_t* Irq10PackMsgList[18];

WORD Irq10PackMsgBuf[18][32];
WORD Irq10Data[18][32];
WORD Irq10Result[2];
//================================================================================

void Irq10PackInit (void)
{
int Index;
mkoPackMsg_t *Msg;

// настройка параметров пакета 
Irq10Pack.nFirst      = 0;
Irq10Pack.nLast       = 17;
Irq10Pack.nPage       = 4;
Irq10Pack.DescPackRes = &Irq10PackRes;

// настройка набора сообщений 
memset(&Irq10PackMsg, 0, sizeof(Irq10PackMsg));

for (Index = 0; Index < 18; Index++)
  {
  Msg = &Irq10PackMsg[Index];

  Msg->DescPack   = &Irq10Pack;
  Msg->nMsg       = Index;
  Msg->TM         = mcTM_RTtoBC;
  Msg->Chan       = 0;
  
  Msg->Eom        = 0;
  Msg->MissSW     = 1;
  Msg->FlagSW     = 1;
  Msg->FmtError   = 1;

  Msg->CW         = (Index <= 2) ? (MKO_CMD(10, 5, MKO_RX, 28)) : (MKO_CMD(10, 6, MKO_RX, 22));
  Msg->CW2        = 0;
  Msg->GapTime    = 0;
  
  Msg->adrData    = (WORD*) &Irq10PackMsgBuf[Index];
  Msg->DescMsgRes = &Irq10PackMsgRes[Index];

  // настройка списка указателей на сообщения
  Irq10PackMsgList[Index] = Msg;
  }
}
//================================================================================

#if (0)
const mkoPackMsg_t Irq10PackMsg01 = { (mkoPack_t*) &Irq10Pack,  0, mcTM_RTtoBC, 0, 0, 1, 1, 1, MKO_CMD(10, 5, MKO_RX, 28), 0, 0, (WORD*) &Irq10PackMsgBuf[ 0], &Irq10PackMsgRes[ 0] };
const mkoPackMsg_t Irq10PackMsg02 = { (mkoPack_t*) &Irq10Pack,  1, mcTM_RTtoBC, 0, 0, 1, 1, 1, MKO_CMD(10, 5, MKO_RX, 28), 0, 0, (WORD*) &Irq10PackMsgBuf[ 1], &Irq10PackMsgRes[ 1] };
const mkoPackMsg_t Irq10PackMsg03 = { (mkoPack_t*) &Irq10Pack,  2, mcTM_RTtoBC, 0, 0, 1, 1, 1, MKO_CMD(10, 5, MKO_RX, 28), 0, 0, (WORD*) &Irq10PackMsgBuf[ 2], &Irq10PackMsgRes[ 2] };
const mkoPackMsg_t Irq10PackMsg04 = { (mkoPack_t*) &Irq10Pack,  3, mcTM_RTtoBC, 0, 0, 1, 1, 1, MKO_CMD(10, 6, MKO_RX, 22), 0, 0, (WORD*) &Irq10PackMsgBuf[ 3], &Irq10PackMsgRes[ 3] };
const mkoPackMsg_t Irq10PackMsg05 = { (mkoPack_t*) &Irq10Pack,  4, mcTM_RTtoBC, 0, 0, 1, 1, 1, MKO_CMD(10, 6, MKO_RX, 22), 0, 0, (WORD*) &Irq10PackMsgBuf[ 4], &Irq10PackMsgRes[ 4] };
const mkoPackMsg_t Irq10PackMsg06 = { (mkoPack_t*) &Irq10Pack,  5, mcTM_RTtoBC, 0, 0, 1, 1, 1, MKO_CMD(10, 6, MKO_RX, 22), 0, 0, (WORD*) &Irq10PackMsgBuf[ 5], &Irq10PackMsgRes[ 5] };
const mkoPackMsg_t Irq10PackMsg07 = { (mkoPack_t*) &Irq10Pack,  6, mcTM_RTtoBC, 0, 0, 1, 1, 1, MKO_CMD(10, 6, MKO_RX, 22), 0, 0, (WORD*) &Irq10PackMsgBuf[ 6], &Irq10PackMsgRes[ 6] };
const mkoPackMsg_t Irq10PackMsg08 = { (mkoPack_t*) &Irq10Pack,  7, mcTM_RTtoBC, 0, 0, 1, 1, 1, MKO_CMD(10, 6, MKO_RX, 22), 0, 0, (WORD*) &Irq10PackMsgBuf[ 7], &Irq10PackMsgRes[ 7] };
const mkoPackMsg_t Irq10PackMsg09 = { (mkoPack_t*) &Irq10Pack,  8, mcTM_RTtoBC, 0, 0, 1, 1, 1, MKO_CMD(10, 6, MKO_RX, 22), 0, 0, (WORD*) &Irq10PackMsgBuf[ 8], &Irq10PackMsgRes[ 8] };
const mkoPackMsg_t Irq10PackMsg10 = { (mkoPack_t*) &Irq10Pack,  9, mcTM_RTtoBC, 0, 0, 1, 1, 1, MKO_CMD(10, 6, MKO_RX, 22), 0, 0, (WORD*) &Irq10PackMsgBuf[ 9], &Irq10PackMsgRes[ 9] };
const mkoPackMsg_t Irq10PackMsg11 = { (mkoPack_t*) &Irq10Pack, 10, mcTM_RTtoBC, 0, 0, 1, 1, 1, MKO_CMD(10, 6, MKO_RX, 22), 0, 0, (WORD*) &Irq10PackMsgBuf[10], &Irq10PackMsgRes[10] };
const mkoPackMsg_t Irq10PackMsg12 = { (mkoPack_t*) &Irq10Pack, 11, mcTM_RTtoBC, 0, 0, 1, 1, 1, MKO_CMD(10, 6, MKO_RX, 22), 0, 0, (WORD*) &Irq10PackMsgBuf[11], &Irq10PackMsgRes[11] };
const mkoPackMsg_t Irq10PackMsg13 = { (mkoPack_t*) &Irq10Pack, 12, mcTM_RTtoBC, 0, 0, 1, 1, 1, MKO_CMD(10, 6, MKO_RX, 22), 0, 0, (WORD*) &Irq10PackMsgBuf[12], &Irq10PackMsgRes[12] };
const mkoPackMsg_t Irq10PackMsg14 = { (mkoPack_t*) &Irq10Pack, 13, mcTM_RTtoBC, 0, 0, 1, 1, 1, MKO_CMD(10, 6, MKO_RX, 22), 0, 0, (WORD*) &Irq10PackMsgBuf[13], &Irq10PackMsgRes[13] };
const mkoPackMsg_t Irq10PackMsg15 = { (mkoPack_t*) &Irq10Pack, 14, mcTM_RTtoBC, 0, 0, 1, 1, 1, MKO_CMD(10, 6, MKO_RX, 22), 0, 0, (WORD*) &Irq10PackMsgBuf[14], &Irq10PackMsgRes[14] };
const mkoPackMsg_t Irq10PackMsg16 = { (mkoPack_t*) &Irq10Pack, 15, mcTM_RTtoBC, 0, 0, 1, 1, 1, MKO_CMD(10, 6, MKO_RX, 22), 0, 0, (WORD*) &Irq10PackMsgBuf[15], &Irq10PackMsgRes[15] };
const mkoPackMsg_t Irq10PackMsg17 = { (mkoPack_t*) &Irq10Pack, 16, mcTM_RTtoBC, 0, 0, 1, 1, 1, MKO_CMD(10, 6, MKO_RX, 22), 0, 0, (WORD*) &Irq10PackMsgBuf[16], &Irq10PackMsgRes[16] };
const mkoPackMsg_t Irq10PackMsg18 = { (mkoPack_t*) &Irq10Pack, 17, mcTM_RTtoBC, 0, 0, 1, 1, 1, MKO_CMD(10, 6, MKO_RX, 22), 0, 0, (WORD*) &Irq10PackMsgBuf[17], &Irq10PackMsgRes[17] };

const mkoPackMsg_t* Irq10PackMsgList[18] =
 {
  (mkoPackMsg_t*) &Irq10PackMsg01, (mkoPackMsg_t*) &Irq10PackMsg02, (mkoPackMsg_t*) &Irq10PackMsg03, (mkoPackMsg_t*) &Irq10PackMsg04,
  (mkoPackMsg_t*) &Irq10PackMsg05, (mkoPackMsg_t*) &Irq10PackMsg06, (mkoPackMsg_t*) &Irq10PackMsg07, (mkoPackMsg_t*) &Irq10PackMsg08,
  (mkoPackMsg_t*) &Irq10PackMsg09, (mkoPackMsg_t*) &Irq10PackMsg10, (mkoPackMsg_t*) &Irq10PackMsg11, (mkoPackMsg_t*) &Irq10PackMsg12,
  (mkoPackMsg_t*) &Irq10PackMsg13, (mkoPackMsg_t*) &Irq10PackMsg14, (mkoPackMsg_t*) &Irq10PackMsg15, (mkoPackMsg_t*) &Irq10PackMsg16,
  (mkoPackMsg_t*) &Irq10PackMsg17, (mkoPackMsg_t*) &Irq10PackMsg18
 };
#endif
//================================================================================

void IrqHandler0A (void)
{
STATUS Status;
int Index, BaseMkoChan;

MSGS("IrqHandler0A()");
SendIrqMessageToKPA(0x0400);

Irq10Result[0] = 0xFFFF;
Irq10Result[1] = 0;

// настраиваем канал МКО для пакета
BaseMkoChan = SpoConfig.DevMkoChan & 1;
for (Index = 0; Index < 18; Index++) { Irq10PackMsg[Index].Chan = BaseMkoChan; }

do
  {
  Status = mkoPackIniStart(0, (mkoPack_t*) &Irq10Pack, (mkoPackMsg_t**) &Irq10PackMsgList, 0);
  if (Status != OK) { Irq10Result[0] = 1; break; }

  Status = mkoNumPackWait(0, (mkoPack_t*) &Irq10Pack);
  if (Status != OK) { Irq10Result[0] = 2; break; }

  // нет ответного слова
  if (Irq10PackRes.MissSW) { Irq10Result[0] = 3; break; }

  // флаги ошибок в ответном слове
  if (Irq10PackRes.FlagSW) { Irq10Result[0] = 4; break; }

  // ошибки формата, длительности и т.д. при передаче
  if (Irq10PackRes.Error) { Irq10Result[0] = 5; break; }

  // переносим блок данных в выходной буфер
  memmove(&Irq10Data, &Irq10PackMsgBuf, sizeof(Irq10Data));

  Irq10Result[0] = 0;
  Irq10Result[1] = 1;
  }
while (0);
}
//================================================================================

/*! \fn void IrqHandler0B (void);
 * \brief Функция-заглушка для прерывания #11 (маска прерывания 0x0800)
 * \details Передает по МКО сообщение о прерывании (если разрешено в конфигурации СПО)
 */

void IrqHandler0B (void)
{
MSGS("IrqHandler0B()");
SendIrqMessageToKPA(0x0800);
}
//================================================================================

/*! \fn void IrqHandler0C (void);
 * \brief Функция-заглушка для прерывания #12 (маска прерывания 0x1000)
 * \details Передает по МКО сообщение о прерывании (если разрешено в конфигурации СПО)
 */

void IrqHandler0C (void)
{
MSGS("IrqHandler0C()");
SendIrqMessageToKPA(0x1000);
}
//================================================================================

/*! \fn void IrqHandler0D (void);
 * \brief Функция-заглушка для прерывания #13 (маска прерывания 0x2000)
 * \details Передает по МКО сообщение о прерывании (если разрешено в конфигурации СПО)
 */

void IrqHandler0D (void)
{
MSGS("IrqHandler0D()");
SendIrqMessageToKPA(0x2000);
Irq13Handler();
}
//================================================================================

/*! \fn void IrqHandler0E (void);
 * \brief Функция-заглушка для прерывания #14 (маска прерывания 0x4000)
 * \details Передает по МКО сообщение о прерывании (если разрешено в конфигурации СПО)
 */

void IrqHandler0E (void)
{
MSGS("IrqHandler0E()");
SendIrqMessageToKPA(0x4000);
}
//================================================================================

/*! \fn void IrqHandler0F (void);
 * \brief Функция-заглушка для прерывания #15 (маска прерывания 0x8000)
 * \details Передает по МКО сообщение о прерывании (если разрешено в конфигурации СПО)
 */

void IrqHandler0F (void)
{
MSGS("IrqHandler0F()");
SendIrqMessageToKPA(0x8000);
}
//================================================================================

/*! \fn void SendIrqMessageToKPA (int AMask);
 * \brief Функция передает на КПА сообщение о возникшем прерывании
 * \param [in] AMask - маска прерывания
 * \details Функция проверяет, разрешено ли данному прерыванию выдавать сообщение на КПА,
 * и, если разрешено, передает сообщение.
 */

void SendIrqMessageToKPA (int AMask)
{
int BaseMkoLine, BaseMkoChan;

if (SpoConfig.IrqMsgMask & AMask)
  {
  // параметры линий обмена с КПА
  BaseMkoLine = SpoConfig.KpaMkoLine & 1;
  BaseMkoChan = SpoConfig.KpaMkoChan & 1;

  ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_IRQ_MESSAGE, KPA_SUBADDR_IRQ, &AMask, 2);
  }
}
//================================================================================

