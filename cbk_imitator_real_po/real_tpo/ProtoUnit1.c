
1. периодическая передача сообщений по МКО

[Common]
typedef unsigned short          WORD
typedef unsigned long           DWORD
typedef unsigned long           UINT

// положение полей в командном слове МКО
#define SH_adrOU                11      // положение поля "адрес ОУ"
#define SH_TR                   10      // положение поля "направление передачи"
#define SH_SA                   5       // положение поля "подадрес ОУ"

// макрос формирования командного слова МКО
#define MKO_CMD(Addr, SubAddr, Dir, Size) \
  ((WORD) ( ((((UINT) Addr) & 0x1F) << SH_adrOU) | \
            ((((UINT) Dir) & 0x01) << SH_TR) | \
            ((((UINT) SubAddr) & 0x1F) << SH_SA) | \
            (((UINT) Size) & 0x1F) ) )

// макросы выделения полей из командного слова МКО
#define MKO_CMD_SIZE(Cmd) 	((((UINT) Cmd) & 0x1F) ? (((UINT) Cmd) & 0x1F) : (32))	// число слов для передачи
#define MKO_CMD_SUBADDR(Cmd)    ((((UINT) Cmd) >> SH_SA) & 0x1F)      			// подадрес ОУ
#define MKO_CMD_DIR(Cmd)        ((((UINT) Cmd) >> SH_TR) & 0x01)     			// направление передачи
#define MKO_CMD_ADDR(Cmd)       ((((UINT) Cmd) >> SH_adrOU) & 0x1F)     		// адрес ОУ
//================================================================================

[Data]
#define PERIODIC_MKO_MSG_COUNT 256

typedef struct
{
WORD MkoCmd;		// команда абоненту МКО
char MkoLine;		// линия МКО для обмена
char MkoChan;		// канал МКО для обмена
char F4Mode;            // команда абоненту в формате Ф4
WORD Interval;          // интервал передачи сообщения, сек
WORD Count;		// сколько раз передать сообщение (-1 = непрерывно)
WORD Data[32];		// буфер под данные приема/передачи для абонента
UINT Pass;              // сколько раз было передано сообщение (0..n)
UINT Timer;             // время следующей передачи сообщения (сек, время ЦБК)
} TPeriodicMkoMsg;

int PeriodicMkoMsgCount, PeriodicMkoMsgLock;
TPeriodicMkoMsg PeriodicMkoMsgList[PERIODIC_MKO_MSG_COUNT];
//================================================================================

[Init]
PeriodicMkoMsgCount = PeriodicMkoMsgLock = 0;
//================================================================================

[CmdHandler]
// [номера слов] (option: [номера битов])

// данные на подадресе 1:
// [0][7..0] - код команды, = 0x60
// [0][15..8] - код операции, = 1..7

// код операции = 1, чтение состояния блокировки периодических сообщений МКО
// возврат, результат: OK
// возврат, данные: 1 слово, = 0 - нет блокировки, = 1 - есть блокировка

// код операции = 2, изменить состояние блокировки периодических сообщений МКО
// [1] - 1 = установить, 0 = снять
// возврат, результат: OK
// возврат, данные: нет

// код операции = 3, добавление сообщения в список периодических сообщений МКО
// [1][15..0] - команда абоненту МКО
// [2][0] - линия МКО для обмена, 0/1
// [2][1] - канал МКО для обмена, 0/1
// [2][2] - 0 = обычный обмен, 1 = команда в формате Ф4, 
// [3][15..0] - интервал передачи сообщения, сек
// [4][15..0] - сколько раз передать сообщение (0xFFFF = непрерывно)
// если добавляется сообщение на запись абонента:
// данные на подадресе 2:
// [n..0] - данные для записи в абонент
// возврат, результат: OK/ошибка
// возврат, данные: нет
// инфо: требует блокировку списка

// код операции = 4, удаление сообщения из списка периодических сообщений МКО
// [1][15..0] - команда абоненту МКО, удаляемая из списка
// [2][0] - линия МКО для обмена, 0/1
// [2][1] - канал МКО для обмена, 0/1
// [2][2] - 0 = обычный обмен, 1 = команда в формате Ф4, 
// если удаляется сообщение на запись абонента:
// данные на подадресе 2:
// [n..0] - данные для записи в абонент
// возврат, результат: OK/ошибка
// возврат, данные: нет
// инфо: требует блокировку списка

// код операции = 5, получить количество сообщений в списке периодических сообщений МКО
// возврат, результат: OK
// возврат, данные: 1 слово, = число сообщений в списке

// код операции = 6, очистить список периодических сообщений МКО
// возврат, результат: OK/ошибка
// инфо: требует блокировку списка

// код операции = 7, выполнить обмен абонент/абонент по МКО
// [1][15..0] - команда на чтение абонента МКО
// [2][0] - линия МКО для чтения абонента, 0/1
// [2][1] - канал МКО для чтения абонента, 0/1
// [3][15..0] - команда на запись абонента МКО
// [4][0] - линия МКО для записи абонента, 0/1
// [4][1] - канал МКО для записи абонента, 0/1
// возврат, результат: OK/ошибка
// возврат, данные: нет

void KpaCommand60 (int BaseMkoLine, int BaseMkoChan)
{
int Opcode, Result;

// читаем код операции
Opcode = KpaCommand[0] >> 8;

// чтение состояния блокировки периодических сообщений МКО
if (Opcode == 0x01)
  {
  Result = GetPeriodicMkoMsgLock();
  ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ, &Result, 2);
  }
// изменить состояние блокировки периодических сообщений МКО
else if (Opcode == 0x02)
  {
  SetPeriodicMkoMsgLock(KpaCommand[1]);
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ);
  }
// добавление сообщения в список периодических сообщений МКО
else if (Opcode == 0x03)
  {
  Result = AddPeriodicMkoMsg(BaseMkoLine, BaseMkoChan);
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, Result, KPA_SUBADDR_IRQ);
  }
// удаление сообщения из списка периодических сообщений МКО
else if (Opcode == 0x04)
  {
  Result = DelPeriodicMkoMsg(BaseMkoLine, BaseMkoChan);
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, Result, KPA_SUBADDR_IRQ);
  }
// получить количество сообщений в списке периодических сообщений МКО
else if (Opcode == 0x05)
  {
  Result = GetPeriodicMkoMsgCount();
  ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ, &Result, 2);
  }
// очистить список периодических сообщений МКО
else if (Opcode == 0x06)
  {
  Result = ClrPeriodicMkoMsgList();
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, Result, KPA_SUBADDR_IRQ);
  }
// выполнить обмен абонент/абонент по МКО
else if (Opcode == 0x07)
  {
  Result = AbonentToAbonentMkoMsg(BaseMkoLine, BaseMkoChan);
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, Result, KPA_SUBADDR_IRQ);
  }
// неизвестная операция
else 
  {
  Result = REPLY_CMD_DATA_ERROR;
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, Result, KPA_SUBADDR_IRQ);
  }
}
//================================================================================

int GetPeriodicMkoMsgLock (void)
{
// выдаем запрошенную и подтвержденную блокировку
return ((PeriodicMkoMsgLock & 0x03) == 0x03) ? (1) : (0);
}
//================================================================================

void SetPeriodicMkoMsgLock (int State)
{
// меняем флаг запроса блокировки
if (State) PeriodicMkoMsgLock |= 0x01; else PeriodicMkoMsgLock &= (~0x01);
}
//================================================================================

int AddPeriodicMkoMsg (int BaseMkoLine, int BaseMkoChan)
{
int Result, Index;
char MkoLine, MkoChan, F4Mode;
WORD MkoCmd, Interval, Count, Cmd, Data[32];
sysTime_t Time;

// описание добавляемого периодического сообщения
MkoCmd   = KpaCommand[1];
MkoLine  = (KpaCommand[2] & 0x0001) ? (1) : (0);
MkoChan  = (KpaCommand[2] & 0x0002) ? (1) : (0);
F4Mode   = (KpaCommand[2] & 0x0004) ? (1) : (0);
Interval = KpaCommand[3];
Count    = KpaCommand[4];

// проверка параметров на допустимость
if ((PeriodicMkoMsgLock & 0x03) != 0x03) return REPLY_CMD_STATE_ERROR;
if (PeriodicMkoMsgCount >= PERIODIC_MKO_MSG_COUNT) return REPLY_CMD_OVERFLOW_ERROR;
if ((Interval < 1) || (Interval > 36000)) return REPLY_CMD_DATA_ERROR;
if (Count < 1) return REPLY_CMD_DATA_ERROR;

// читаем данные для режима передачи
if ((!F4Mode) && (MKO_CMD_DIR(MkoCmd) == MKO_TX))
  {     
  Cmd = MKO_CMD(KPA_ADDR, KPA_SUBADDR_CMD + 1, MKO_RX, MKO_CMD_SIZE(MkoCmd));
  Result = ReadPeriodicMkoMsgData(BaseMkoLine, BaseMkoChan, Cmd, KpaCommand[7], &Data);
  if (Result != REPLY_CMD_DONE) return Result;
  }

// ищем сообщение в списке
if (FindPeriodicMkoMsg(MkoCmd, MkoLine, MkoChan, F4Mode, &Data, &Index)) return REPLY_ALREADY_SET_ERROR;

// добавление сообщения в список
Index = PeriodicMkoMsgCount;
PeriodicMkoMsgList[Index].MkoCmd = MkoCmd;
PeriodicMkoMsgList[Index].MkoLine = MkoLine;
PeriodicMkoMsgList[Index].MkoChan = MkoChan;
PeriodicMkoMsgList[Index].F4Mode = F4Mode;
PeriodicMkoMsgList[Index].Interval = Interval;
PeriodicMkoMsgList[Index].Count = Count;
PeriodicMkoMsgList[Index].Pass = 0;

if ((!F4Mode) && (MKO_CMD_DIR(MkoCmd) == MKO_TX))
  {
  memcpy(&PeriodicMkoMsgList[Index].Data, &Data, MKO_CMD_SIZE(MkoCmd) * sizeof(WORD));
  }

// назначаем время запуска 
if (timeRead(&Time) != OK) return REPLY_OPO_INT_ERROR;
if (Time.time_msec >= 500) Time.time_sec++;

PeriodicMkoMsgList[Index].Timer = Time.time_sec + Interval;

PeriodicMkoMsgCount++;
return REPLY_CMD_DONE;
}
//================================================================================

int DelPeriodicMkoMsg (int BaseMkoLine, int BaseMkoChan)
{
int Index, Found;
char MkoLine, MkoChan, F4Mode;
WORD MkoCmd, Crc16, Data[32];

// описание удаляемого периодического сообщения
MkoCmd   = KpaCommand[1];
MkoLine  = (KpaCommand[2] & 0x0001) ? (1) : (0);
MkoChan  = (KpaCommand[2] & 0x0002) ? (1) : (0);
F4Mode   = (KpaCommand[2] & 0x0004) ? (1) : (0);

// проверка параметров на допустимость
if ((PeriodicMkoMsgLock & 0x03) != 0x03) return REPLY_CMD_STATE_ERROR;

// читаем данные для режима передачи
if ((!F4Mode) && (MKO_CMD_DIR(MkoCmd) == MKO_TX))
  {    
  Cmd = MKO_CMD(KPA_ADDR, KPA_SUBADDR_CMD + 1, MKO_RX, MKO_CMD_SIZE(MkoCmd));
  Result = ReadPeriodicMkoMsgData(BaseMkoLine, BaseMkoChan, Cmd, KpaCommand[7], &Data);
  if (Result != REPLY_CMD_DONE) return Result;
  }

// ищем сообщение в списке
if (!FindPeriodicMkoMsg(MkoCmd, MkoLine, MkoChan, F4Mode, &Data, &Index)) return REPLY_ITEM_NOT_FOUND;

// удаляем сообщение из списка
for ( ; Index < (PeriodicMkoMsgCount - 1); Index++)
  {
  PeriodicMkoMsgList[Index] = PeriodicMkoMsgList[Index + 1];
  }

PeriodicMkoMsgCount--;
return REPLY_CMD_DONE;
}
//================================================================================

int ReadPeriodicMkoMsgData (int BaseMkoLine, int BaseMkoChan, WORD Cmd, WORD DataCrc, WORD *Buffer)
{
int Result;
WORD Crc16;

// считываем данные
Result = MkoXchgBn(BaseMkoLine, BaseMkoChan, Cmd, 0);
if (Result) return DECODE_XCHG_ERROR(Result);

// проверяем размер принятых данных
if (MkoMsgResB[0].Len != MKO_CMD_SIZE(Cmd)) return REPLY_NO_DATA_ERROR;

// проверяем целостность данных
Crc16 = CalcCrc16(&MkoBufB[0][0], MKO_CMD_SIZE(Cmd) * sizeof(WORD));
if (Crc16 != DataCrc) return REPLY_DATA_CRC_ERROR;

// сохраняем данные 
memcpy(Buffer, &MkoBufB[0][0], MKO_CMD_SIZE(Cmd) * sizeof(WORD));

return REPLY_CMD_DONE;
}
//================================================================================

int FindPeriodicMkoMsg (WORD MkoCmd, char MkoLine, char MkoChan, char F4Mode, WORD *Data, int *ItemIndex)
{
int Index, Found;

for (Index = 0, Found = 0; Index < PeriodicMkoMsgCount; Index++)
  {
  if (PeriodicMkoMsgList[Index].MkoCmd != MkoCmd) continue;
  if (PeriodicMkoMsgList[Index].MkoLine != MkoLine) continue;
  if (PeriodicMkoMsgList[Index].MkoChan != MkoChan) continue;
  if (PeriodicMkoMsgList[Index].F4Mode != F4Mode) continue;

  if ((!F4Mode) && (MKO_CMD_DIR(MkoCmd) == MKO_TX))
    {
    if (PeriodicMkoMsgDataCmp(Data, &PeriodicMkoMsgList[Index].Data, MKO_CMD_SIZE(MkoCmd))) continue;
    }

  *ItemIndex = Index; 
  Found = 1; 
  break;
  }

return Found;
}
//================================================================================

int PeriodicMkoMsgDataCmp (WORD *Data1, WORD *Data2, int Count)
{
while (Count--) { if (*Data1++ != *Data2++) return 1; }
return 0;
}
//================================================================================

int GetPeriodicMkoMsgCount (void)
{
return PeriodicMkoMsgCount;
}
//================================================================================

int ClrPeriodicMkoMsgList (void)
{
// проверка параметров на допустимость
if ((PeriodicMkoMsgLock & 0x03) != 0x03) return REPLY_CMD_STATE_ERROR;

PeriodicMkoMsgCount = 0;
return REPLY_CMD_DONE;
}
//================================================================================

int AbonentToAbonentMkoMsg (int BaseMkoLine, int BaseMkoChan)
{
int Result;
char SrcMkoLine, SrcMkoChan, DstMkoLine, DstMkoChan;
WORD SrcMkoCmd, DstMkoCmd;

SrcMkoCmd   = KpaCommand[1];
SrcMkoLine  = (KpaCommand[2] & 0x0001) ? (1) : (0);
SrcMkoChan  = (KpaCommand[2] & 0x0002) ? (1) : (0);
DstMkoCmd   = KpaCommand[3];
DstMkoLine  = (KpaCommand[4] & 0x0001) ? (1) : (0);
DstMkoChan  = (KpaCommand[4] & 0x0002) ? (1) : (0);

// проверка параметров на допустимость
if (MKO_CMD_DIR(SrcMkoCmd) != MKO_RX) return REPLY_CMD_DATA_ERROR;
if (MKO_CMD_DIR(DstMkoCmd) != MKO_TX) return REPLY_CMD_DATA_ERROR;
if (MKO_CMD_SIZE(SrcMkoCmd) != MKO_CMD_SIZE(DstMkoCmd)) return REPLY_CMD_DATA_ERROR;

// читаем данные из абонента источника
Result = MkoXchgBn(SrcMkoLine, SrcMkoChan, SrcMkoCmd, 0);
if (Result) return DECODE_XCHG_ERROR(Result);

// ошибка в размере принятых данных
if (MkoMsgResB[0].Len != MKO_CMD_SIZE(SrcMkoCmd)) return REPLY_NO_DATA_ERROR;

// пишем данные в абонент приемник
Result = MkoXchgBn(DstMkoLine, DstMkoChan, DstMkoCmd, 0);
if (Result) return DECODE_XCHG_ERROR(Result);

return REPLY_CMD_DONE;
}
//================================================================================

[Main]
void DoPeriodicMkoMsg (void)
{                       
// если нет подтвержденной блокировки - выполняем обмены
if ((PeriodicMkoMsgLock & 0x02) == 0) ProcessPeriodicMkoMsg(); 

// если есть запрос на блокировку - подтверждаем, иначе - сбрасываем
if (PeriodicMkoMsgLock & 0x01) PeriodicMkoMsgLock |= 0x02; else PeriodicMkoMsgLock &= (~0x02);
}
//================================================================================

void ProcessPeriodicMkoMsg (void)
{
int Result, Index, MkoLine, MkoChan;
int BaseMkoLine, BaseMkoChan;
sysTime_t Time;
WORD MkoCmd, Buffer[8];

if (timeRead(&Time) != OK) return;
if (Time.time_msec >= 500) Time.time_sec++;

BaseMkoLine = SpoConfig.KpaMkoLine & 1;
BaseMkoChan = SpoConfig.KpaMkoChan & 1;

for (Index = 0; Index < PeriodicMkoMsgCount; Index++)
  {
  if (PeriodicMkoMsgList[Index].Count == 0) continue;

  if (Time.time_sec >= PeriodicMkoMsgList[Index].Timer)
    {
    PeriodicMkoMsgList[Index].Timer = Time.time_sec + PeriodicMkoMsgList[Index].Interval;
    PeriodicMkoMsgList[Index].Pass++;

    MkoCmd  = PeriodicMkoMsgList[Index].MkoCmd;
    MkoLine = PeriodicMkoMsgList[Index].MkoLine;
    MkoChan = PeriodicMkoMsgList[Index].MkoChan;

    if (PeriodicMkoMsgList[Index].F4Mode)
      {
      Result = MkoXchgBn(MkoLine, MkoChan, MkoCmd, 1);
      }
    else 
      {
      if (MKO_CMD_DIR(MkoCmd) == MKO_TX) memcpy(&MkoBufB[0][0], &PeriodicMkoMsgList[Index].Data, MKO_CMD_SIZE(MkoCmd) * sizeof(WORD));
      Result = MkoXchgBn(MkoLine, MkoChan, MkoCmd, 0);
      }

    if (Result)
      {
      Buffer[0] = MkoCmd;
      Buffer[1] = MkoMsgResB[0].SW;
      Buffer[2] = DECODE_XCHG_ERROR(Result);
      Buffer[3] = (WORD) PeriodicMkoMsgList[Index].Pass;
      Buffer[4] = (WORD)(PeriodicMkoMsgList[Index].Pass >> 16);
      ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_PERIODIC_MSG_ERROR, KPA_SUBADDR_IRQ, &Buffer, 5 * sizeof(WORD));
      }

    if (PeriodicMkoMsgList[Index].Count != 0xFFFF) PeriodicMkoMsgList[Index].Count--;
    }
  } 
}
//================================================================================

