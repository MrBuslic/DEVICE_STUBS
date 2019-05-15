
/*! \file MkoMirrorUnit.c
 * \author Гапонов Р.В.
 * \date 2013.10.30
 * \version 1.0.0
 * \brief Модуль содержит функции работы с подадресом 30 (зеркало) абонентов МКО
 */

#include "MkoMirrorUnit.h"
#include "StdLib.h"
//================================================================================

// [номера слов] (option: [номера битов])

// данные на подадресе 1:
// [0][7..0] - код команды, = 0x64
// [0][15..8] - код операции, = 1..4

// код операции = 1, сброс состояния (в неинициализированное)
// возврат, результат: OK
// возврат, данные: нет
// пример: ВЫДАТЬ_ДИР([0x0164], []);

// код операции = 2, установка параметров связи с абонентом
// возврат, результат: OK/Error
// возврат, данные: нет
// пример: ВЫДАТЬ_ДИР([0x0264, Адрес_Абонента, Линия_МКО, Канал_Записи_МКО, Канал_Чтения_МКО], []);

// код операции = 3, установка параметров обмена
// возврат, результат: OK/Error
// возврат, данные: нет
// пример: ВЫДАТЬ_ДИР([0x0364, Число_Циклов, Слов_Для_Записи, Слов_Для_Чтения, Интервал_в_мкСек], []);

// код операции = 4, запуск обмена с абонентом
// возврат, результат: OK/Error
// возврат, данные: код завершения и 6 слов данных:
// [0] - общее число обменов
// [1] - число ошибок записи абонента 
// [2] - число ошибок чтения абонента
// [3] - число обменов с ошибками
// [4] - число сбойных слов в сообщениях
// [5] - число сбойных бит в сообщениях
// пример: ВЫДАТЬ_ДИР([0x0464], []);

#if (0)
// код операции = 5, установка маски режимов обмена
// возврат, результат: OK
// возврат, данные: нет
// пример: ВЫДАТЬ_ДИР([0x0564, Маска], []);

// код операции = 6, вкл/откл режима отладки
// возврат, результат: OK
// возврат, данные: нет
// пример: ВЫДАТЬ_ДИР([0x0664, Режим], []);
#endif

//================================================================================

int MkoMirrorAddr;                               // 1..31
int MkoMirrorLine;                               // 0..1
int MkoMirrorWrChan, MkoMirrorRdChan;            // 0..1

int MkoMirrorCycles;                             // 1..10000
int MkoMirrorWrLen, MkoMirrorRdLen;              // 1..32
int MkoMirrorInterval;                           // 30..1000 mks

WORD MkoMirrorXchgTotal;                         // общее число обменов
WORD MkoMirrorAbWrError, MkoMirrorAbRdError;     // число ошибок записи и чтения абонента
WORD MkoMirrorXchgWithError;                     // число обменов с ошибками
WORD MkoMirrorBadWords, MkoMirrorBadBits;        // число сбойных слов и бит в сообщениях

int MkoMirrorXchgMask;				 // маска режима обменов по МКО
int MkoMirrorDebug;                              // режим отладки (1 - вкл, 0 - выкл)
//================================================================================

void KpaCommand64 (int BaseMkoLine, int BaseMkoChan)
{
int Opcode, Result;
int Value1, Value2, Value3, Value4;
WORD Buffer[6];

// читаем код операции
Opcode = (KpaCommand[0] >> 8) & 0xFF;

// сброс состояния
if (Opcode == 0x01)
  {
  MkoMirrorAddr = -1;
  MkoMirrorLine = -1;
  MkoMirrorWrChan = MkoMirrorRdChan = -1;

  MkoMirrorCycles = -1;
  MkoMirrorWrLen = MkoMirrorRdLen = -1;
  MkoMirrorInterval = -1;

  MkoMirrorXchgMask = 0x05;
  MkoMirrorDebug = 0;
  
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ);
  }
// настройка параметров связи с абонентом
else if (Opcode == 0x02)
  {
  Value1 = KpaCommand[1];
  Value2 = KpaCommand[2];
  Value3 = KpaCommand[3];
  Value4 = KpaCommand[4];

  do
    {
    Result = REPLY_CMD_DATA_ERROR;
    if ((Value1 < 1) || (Value1 > 31)) break;
    if ((Value2 < 0) || (Value2 > 1)) break;
    if ((Value3 < 0) || (Value3 > 1)) break;
    if ((Value4 < 0) || (Value4 > 1)) break;

    Result = REPLY_CMD_DONE;
    MkoMirrorAddr   = Value1;
    MkoMirrorLine   = Value2;
    MkoMirrorWrChan = Value3;
    MkoMirrorRdChan = Value4;
    }
  while (0);

  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, Result, KPA_SUBADDR_IRQ);
  }
// настройка параметров обмена с абонентом
else if (Opcode == 0x03)
  {
  Value1 = KpaCommand[1];
  Value2 = KpaCommand[2];
  Value3 = KpaCommand[3];
  Value4 = KpaCommand[4];

  do
    {
    Result = REPLY_CMD_DATA_ERROR;
    if ((Value1 < 1) || (Value1 > 10000)) break;
    if ((Value2 < 1) || (Value2 > 32)) break;
    if ((Value3 < 1) || (Value3 > 32)) break;
    if ((Value4 < 0) || (Value4 > 30000)) break;

    Result = REPLY_CMD_DONE;
    MkoMirrorCycles   = Value1;
    MkoMirrorWrLen    = Value2;
    MkoMirrorRdLen    = Value3;
    MkoMirrorInterval = Value4;
    }
  while (0);

  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, Result, KPA_SUBADDR_IRQ);
  }
// запуск обмена с абонентом
else if (Opcode == 0x04)
  {
  Result = MkoMirrorDoXchg(BaseMkoLine, BaseMkoChan);
  
  Buffer[0] = (MkoMirrorXchgTotal > 0xFFFF)     ? (0xFFFF) : (MkoMirrorXchgTotal);
  Buffer[1] = (MkoMirrorAbWrError > 0xFFFF)     ? (0xFFFF) : (MkoMirrorAbWrError);
  Buffer[2] = (MkoMirrorAbRdError > 0xFFFF)     ? (0xFFFF) : (MkoMirrorAbRdError);
  Buffer[3] = (MkoMirrorXchgWithError > 0xFFFF) ? (0xFFFF) : (MkoMirrorXchgWithError);
  Buffer[4] = (MkoMirrorBadWords > 0xFFFF)      ? (0xFFFF) : (MkoMirrorBadWords);
  Buffer[5] = (MkoMirrorBadBits > 0xFFFF)       ? (0xFFFF) : (MkoMirrorBadBits);

  ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, Result, KPA_SUBADDR_IRQ, &Buffer, sizeof(Buffer));
  }
#if (0)
// установка маски режима обмена по МКО
else if (Opcode == 0x05)
  {
  MkoMirrorXchgMask = KpaCommand[1] & 0x07;

  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ);
  }
// установка режима отладки
else if (Opcode == 0x06)
  {
  MkoMirrorDebug = KpaCommand[1] & 0x01;

  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ);
  }
#endif
// неизвестная операция
else
  {
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DATA_ERROR, KPA_SUBADDR_IRQ);
  }
}
//================================================================================

#define MKO_MIRROR_SAVE_RESULT()      \
        { \
        memmove(&PackRes, &MkoPackResB, sizeof(PackRes)); \
        memmove(&MsgRes0, &MkoMsgResB[0], sizeof(MsgRes0)); \
        memmove(&MsgRes1, &MkoMsgResB[1], sizeof(MsgRes1)); \
        memmove(&RxData, &MkoBufB[1], sizeof(RxData)); \
        }        
//================================================================================

int MkoMirrorDoXchg (int BaseMkoLine, int BaseMkoChan)
{
int Pass, Index, Timer, XchgResult, LastMsg, ErrorFlag;
WORD Reply, Value, TxData[32], RxData[32];
sysTime_t Time;
STATUS Status;
mkoPackRes_t PackRes;
mkoMsgRes_t MsgRes0, MsgRes1;

// сброс счетчиков обмена
MkoMirrorXchgTotal = 0;
MkoMirrorAbWrError = MkoMirrorAbRdError = 0;
MkoMirrorXchgWithError = 0;
MkoMirrorBadWords = MkoMirrorBadBits = 0;

// проверка параметров обмена
if (MkoMirrorAddr == -1) return REPLY_CMD_DATA_ERROR;
if (MkoMirrorLine == -1) return REPLY_CMD_DATA_ERROR;
if ((MkoMirrorWrChan == -1) || (MkoMirrorRdChan == -1)) return REPLY_CMD_DATA_ERROR;

if (MkoMirrorCycles == -1) return REPLY_CMD_DATA_ERROR;
if ((MkoMirrorWrLen == -1) || (MkoMirrorRdLen == -1)) return REPLY_CMD_DATA_ERROR;
if (MkoMirrorInterval == -1) return REPLY_CMD_DATA_ERROR;

// инициализация генератора случайных чисел
timeRead(&Time);
srand(Time.time_msec);

// цикл по числу обменов с абонентом
for (Pass = 0; Pass < MkoMirrorCycles; Pass++)
  {
  // создание набора псевдослучайных чисел (последнее число размазывается до конца буфера)
  for (Index = 0, Value = 0; Index < 32; Index++)
    {
    if (Index < MkoMirrorWrLen) Value = rand();
    TxData[Index] = Value;
    }

  // интервал обмен-обмен (грубо)
  Timer = (MkoMirrorInterval + 999) / 1000;
  if (Timer) SleepMSec(Timer);

  // обмен с абонентом
  do
    {
    XchgResult = 1;

    // инициализация описателя пакета МКО (на 2 сообщения)
    MkoPackB.nLast = 1;		// номер последнего сообщения
    memset(&MkoPackResB, 0, sizeof(MkoPackResB));  // очистка флагов ошибок пакета

    // настройка сообщения 0
    MkoMsgB[0].TM   = mcTM_BCtoRT;        // тип сообщения, TX
    MkoMsgB[0].Chan = MkoMirrorWrChan;    // канал МКО для записи
    MkoMsgB[0].CW   = MKO_CMD(MkoMirrorAddr, 30, MKO_TX, MkoMirrorWrLen);  // командное слово
    MkoMsgB[0].MissSW   = (MkoMirrorXchgMask & 0x01) ? (1) : (0);              // прервать передачу при отсутствии ОС
    MkoMsgB[0].FlagSW   = (MkoMirrorXchgMask & 0x02) ? (1) : (0);              // прервать передачу при ошибках в ОС
    MkoMsgB[0].FmtError = (MkoMirrorXchgMask & 0x04) ? (1) : (0);              // прервать передачу при ошибках передачи
    MkoMsgB[0].GapTime  = MkoMirrorInterval;  // пауза между сообщениями
    // сброс флагов для сообщения
    memset(&MkoMsgResB[0], 0, sizeof(mkoMsgRes_t));   
    // блок данных для передачи
    memmove(&MkoBufB[0], &TxData, sizeof(TxData));
    // конец настройки сообщения 0

    // настройка сообщения 1
    MkoMsgB[1].TM   = mcTM_RTtoBC;        // тип сообщения, RX
    MkoMsgB[1].Chan = MkoMirrorRdChan;    // канал МКО для чтения
    MkoMsgB[1].CW   = MKO_CMD(MkoMirrorAddr, 30, MKO_RX, MkoMirrorRdLen);  // командное слово
    MkoMsgB[1].MissSW   = (MkoMirrorXchgMask & 0x01) ? (1) : (0);              // прервать передачу при отсутствии ОС
    MkoMsgB[1].FlagSW   = (MkoMirrorXchgMask & 0x02) ? (1) : (0);              // прервать передачу при ошибках в ОС
    MkoMsgB[1].FmtError = (MkoMirrorXchgMask & 0x04) ? (1) : (0);              // прервать передачу при ошибках передачи
    MkoMsgB[1].GapTime  = MkoMirrorInterval;  // пауза между сообщениями
    // сброс флагов для сообщения
    memset(&MkoMsgResB[1], 0, sizeof(mkoMsgRes_t));   
    // конец настройки сообщения 1

    // проведение обмена по МКО
    Status = mkoPackIniStart(MkoMirrorLine, &MkoPackB, MkoMsgPtrB, 0);  // запуск обмена по МКО
    if (Status != OK) 
      {
      MKO_MIRROR_SAVE_RESULT();
      if (MkoMirrorDebug) MkoMirrorSendDebugInfo(BaseMkoLine, BaseMkoChan, REPLY_ERROR_POINT_1, &PackRes, &MsgRes0, &MsgRes1);
      return REPLY_ERROR_POINT_1;
      }
  
    Status = mkoNumPackWait(MkoMirrorLine, &MkoPackB);  // ожидание завершения обмена по МКО
    if (Status != OK) 
      {
      MKO_MIRROR_SAVE_RESULT();
      if (MkoMirrorDebug) MkoMirrorSendDebugInfo(BaseMkoLine, BaseMkoChan, REPLY_ERROR_POINT_2, &PackRes, &MsgRes0, &MsgRes1);
      return REPLY_ERROR_POINT_2;
      }

    MKO_MIRROR_SAVE_RESULT();
    if (MkoMirrorDebug) MkoMirrorSendDebugInfo(BaseMkoLine, BaseMkoChan, REPLY_ERROR_POINT_3, &PackRes, &MsgRes0, &MsgRes1);

    // [проверка результата для пакета]
    // нет ответного слова
    if (PackRes.MissSW) return REPLY_MKO_NO_OS_ERROR;

    // ошибки при передаче
    if (PackRes.Error) return REPLY_MKO_LINK_ERROR;

    // ошибки в ответном слове
    if ((PackRes.FlagSW) || (PackRes.ErrSW))
      {
      // [проверка результата для сообщений]
      LastMsg = PackRes.nMsg;
      if (LastMsg >= 0)
        {
        // проверка сообщения записи в абонент
        Reply = MsgRes0.SW;
        if (MKO_OS_OU_ERROR(Reply)) return REPLY_MKO_OU_ERROR;
        if (MKO_OS_AB_ERROR(Reply)) return REPLY_MKO_OU_ERROR;
        if ((MKO_OS_BUSY(Reply)) || (MKO_OS_MSG_ERROR(Reply))) { MkoMirrorAbWrError++; break; }
        }
      if (LastMsg >= 1)
        {
        // проверка сообщения чтения из абонента
        Reply = MsgRes1.SW;
        if (MKO_OS_OU_ERROR(Reply)) return REPLY_MKO_OU_ERROR;
        if (MKO_OS_AB_ERROR(Reply)) return REPLY_MKO_OU_ERROR;
        if ((MKO_OS_BUSY(Reply)) || (MKO_OS_MSG_ERROR(Reply))) { MkoMirrorAbRdError++; break; }
        }
      }

    XchgResult = 0;
    }
  while (0);

  // если передача-прием прошли без ошибок
  if (!XchgResult)
    {
    // проверяем целостность принятых данных
    for (Index = 0, ErrorFlag = 0; Index < MkoMirrorRdLen; Index++)
      {
      Value = RxData[Index];
      if (Value != TxData[Index])
        {
        ErrorFlag = 1;
        // увеличиваем счетчик сбойных слов
        MkoMirrorBadWords++;
        // увеличиваем счетчик сбойных бит
        MkoMirrorBadBits += BitCount(Value ^ TxData[Index]);
        }
      }

    // увеличиваем счетчик обменов со сбоями в данных
    if (ErrorFlag) MkoMirrorXchgWithError++;
    }

  // увеличиваем счетчик произведенных обменов
  MkoMirrorXchgTotal++;
  }

return REPLY_CMD_DONE;
}
//================================================================================

void MkoMirrorSendDebugInfo (int BaseMkoLine, int BaseMkoChan, int PointCode, mkoPackRes_t *PackRes, mkoMsgRes_t *MsgRes0, mkoMsgRes_t *MsgRes1)
{
MkoBufB[0][0] = PointCode;
MkoXchgBn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, 25, MKO_TX, 1), 0);

memset(&MkoBufB[0], 0, sizeof(MkoBufB[0]));
memmove(&MkoBufB[0], PackRes, sizeof(mkoPackRes_t));
MkoXchgBn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, 25, MKO_TX, 16), 0);

memset(&MkoBufB[0], 0, sizeof(MkoBufB[0]));
memmove(&MkoBufB[0], MsgRes0, sizeof(mkoMsgRes_t));
MkoXchgBn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, 25, MKO_TX, 16), 0);

memset(&MkoBufB[0], 0, sizeof(MkoBufB[0]));
memmove(&MkoBufB[0], MsgRes1, sizeof(mkoMsgRes_t));
MkoXchgBn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, 25, MKO_TX, 16), 0);
}
//================================================================================

