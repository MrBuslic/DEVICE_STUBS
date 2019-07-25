/*1. периодическая передача сообщений по МКО

List
  {
  Count - число элементов в списке сообщений
  Msg[] - список сообщений для передачи
  }

Msg
  {
  MkoLine - линия МКО для передачи сообщения (0/1)
  MkoChan - канал МКО для передачи сообщения (0/1)
  Cmd - команда МКО для сообщения (N)
  Interval - период передачи сообщения, в сек (1..3600)
  Count - сколько раз передать сообщение (-1 = бесконечно)
  Data[] - данные для приема/передачи в сообщении (WORD[32])
  Timer - время следующей передачи сообщения (N)
  }

[code=1 2 3 4 5 6 crc=7 crc=8]
*/

typedef struct
{
char MkoLine;		// линия МКО для обмена
char MkoChan;		// канал МКО для обмена
char F4Mode;            // команда абоненту в формате Ф4
WORD MkoCmd;		// команда абоненту МКО
WORD Interval;          // интервал передачи сообщения, сек
WORD Count;		// сколько раз передать сообщение (-1 = непрерывно)
WORD Data[32];		// буфер под данные приема/передачи для абонента
UINT Timer;             // время следующей передачи сообщения (сек, время ЦБК)
} TMkoPeriodicMsg;

#define PERIODIC_MKO_MSG_COUNT 256
int PeriodicMkoMsgCount;
TPeriodicMkoMsg PeriodicMkoMsgList[PERIODIC_MKO_MSG_COUNT]

// данные на подадресе 1
// [0][7..0] - код команды
// [0][8] - 1 = добавление сообщения в список, 0 = удаление сообщения из списка
// [0][9] - линия МКО для обмена, 0/1
// [0][10] - канал МКО для обмена, 0/1
// [0][11] - 0 = обычный обмен, 1 = команда в формате Ф4, 
// [1][15..0] - команда абоненту МКО
// [2][15..0] - интервал передачи сообщения, сек
// [3][15..0] - сколько раз передать сообщение (-1 = непрерывно)
// [6][15..0] - crc команды
// [7][15..0] - crc команды

// если сообщение на запись абонента
// данные на подадресе 2
// [31..0] - данные для записи в абонент

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

void MkoCommandXX (int BaseMkoLine, int BaseMkoChan)
{
int Mode = KpaCommand[0] & 0x0100;
int Result = (Mode) ? (SetPeriodicMkoMsg(BaseMkoLine, BaseMkoChan)) : (ClrPeriodicMkoMsg(BaseMkoLine, BaseMkoChan));
ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, Result, KPA_SUBADDR_IRQ);
}

int SetPeriodicMkoMsg (int BaseMkoLine, int BaseMkoChan)
{
int Result, Index;
char MkoLine, MkoChan, F4Mode;
WORD MkoCmd, Interval, Count, Crc16;
sysTime_t Time;

// описание периодического сообщения
MkoLine  = (KpaCommand[0] & 0x0200) ? (1) : (0);
MkoChan  = (KpaCommand[0] & 0x0400) ? (1) : (0);
F4Mode   = (KpaCommand[0] & 0x0800) ? (1) : (0);
WORD MkoCmd   = KpaCommand[1];
WORD Interval = KpaCommand[2];
WORD Count    = KpaCommand[3];

// проверка параметров на допустимость
if (PeriodicMkoMsgCount >= PERIODIC_MKO_MSG_COUNT) return REPLY_CMD_OVERFLOW_ERROR;

for (Index = 0; Index < PeriodicMkoMsgCount; Index++)
  {
  if (PeriodicMkoMsgList[Index].MkoCmd == MkoCmd) return REPLY_CMD_DATA_ERROR;
  }

if ((Interval < 1) || (Interval > 36000)) return REPLY_CMD_DATA_ERROR;

if (Count < 1) return REPLY_CMD_DATA_ERROR;

// установка сообщения в список
Index = PeriodicMkoMsgCount;
PeriodicMkoMsgList[Index].MkoLine = MkoLine;
PeriodicMkoMsgList[Index].MkoChan = MkoChan;
PeriodicMkoMsgList[Index].F4Mode = F4Mode;
PeriodicMkoMsgList[Index].MkoCmd = MkoCmd;
PeriodicMkoMsgList[Index].Interval = Interval;
PeriodicMkoMsgList[Index].Count = Count;

// назначаем время запуска 
if (timeRead(&Time) != OK) return REPLY_OPO_INT_ERROR;

PeriodicMkoMsgList[Index].Timer = Time.time_sec + Interval;
if (Time.time_msec >= 500) PeriodicMkoMsgList[Index].Timer++;

// если команда в формате Ф4 - закончить
if (F4Mode) 
  {
  PeriodicMkoMsgCount++;
  return REPLY_CMD_DONE;
  }

// если команда на прием - закончить
if (MKO_CMD_DIR(MkoCmd) == MKO_RX)
  {
  PeriodicMkoMsgCount++;
  return REPLY_CMD_DONE;
  }
                   
// если команда на передачу - считываем данные для передачи в абонент
Result = MkoXchgBn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_CMD + 1, MKO_RX, MKO_CMD_SIZE(MkoCmd)), 0);
if (Result) return DECODE_XCHG_ERROR(Result);

// ошибка в размере принятых данных
if (MkoMsgResB[0].Len != MKO_CMD_SIZE(MkoCmd)) return REPLY_NO_DATA_ERROR;

// проверка целостности данных
Crc16 = CalcCrc16(&MkoBufB[0][0], MKO_CMD_SIZE(MkoCmd) * sizeof(WORD));
if (Crc16 != KpaCommand[7]) return REPLY_DATA_CRC_ERROR;

// сохраняем данные для предачи в списке
memcpy(&PeriodicMkoMsgList[Index].Data, &MkoBufB[0][0], MKO_CMD_SIZE(MkoCmd) * sizeof(WORD));

PeriodicMkoMsgCount++;
return REPLY_CMD_DONE;
}

int ClrPeriodicMkoMsg (int BaseMkoLine, int BaseMkoChan)
{
int Index, Found;
WORD MkoCmd;

// описание периодического сообщения
MkoCmd = KpaCommand[1];

// проверка параметров на допустимость
for (Index = 0, Found = 0; Index < PeriodicMkoMsgCount; Index++)
  {
  if (PeriodicMkoMsgList[Index].MkoCmd == MkoCmd) { Found = 1; break; }
  }
if (!Found) return REPLY_CMD_DATA_ERROR;

for ( ; Index < (PeriodicMkoMsgCount - 1); Index++)
  {
  PeriodicMkoMsgList[Index] = PeriodicMkoMsgList[Index + 1];
  }

PeriodicMkoMsgCount--;
return REPLY_CMD_DONE;
}


2. передача сообщения абонент>>абонент

Cmd
  {
  SrcMkoLine - линия МКО для передачи сообщения (0/1)
  SrcMkoChan - канал МКО для передачи сообщения (0/1)
  SrcCmd

  DstMkoLine - линия МКО для передачи сообщения (0/1)
  DstMkoChan - канал МКО для передачи сообщения (0/1)
  DstCmd

  Data[]
  }

// данные на подадресе 1
// [0][7..0] - код команды
// [1][0] - линия МКО для чтения абонента, 0/1
// [1][1] - канал МКО для чтения абонента, 0/1
// [2][15..0] - команда на чтение абонента МКО
// [3][0] - линия МКО для записи абонента, 0/1
// [3][1] - канал МКО для записи абонента, 0/1
// [4][15..0] - команда на запись абонента МКО
// [6][15..0] - crc команды

void MkoCommandXX (int BaseMkoLine, int BaseMkoChan)
{
int Result;

Result = AbonentToAbonentMkoMsg(BaseMkoLine, BaseMkoChan);

ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, Result, KPA_SUBADDR_IRQ);
}

int AbonentToAbonentMkoMsg (int BaseMkoLine, int BaseMkoChan)
{
int Result;
char SrcMkoLine, SrcMkoChan, DstMkoLine, DstMkoChan;
WORD SrcMkoCmd, DstMkoCmd;

SrcMkoLine  = (KpaCommand[1] & 0x0001) ? (1) : (0);
SrcMkoChan  = (KpaCommand[1] & 0x0002) ? (1) : (0);
SrcMkoCmd   = KpaCommand[2];
DstMkoLine  = (KpaCommand[3] & 0x0001) ? (1) : (0);
DstMkoChan  = (KpaCommand[3] & 0x0002) ? (1) : (0);
DstMkoCmd   = KpaCommand[4];

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


3. установка запрещенных сообщений на МКО

List
  {
  Count - число элементов в списке сообщений
  Msg[] - список сообщений для передачи
  }

Msg
  {
  Cmd [Addr, SubAddr, Dir, Size]
  Data[32]
  Mask[32]
  }













bfdfefc0

1FC00000
AFC0 

0001 1111 1100 0000 
1011 1111 1101 1111


/*
[ЦБК = НОК, загрузка в RAM, с печатью (отладка)]
адреса в настройках проекта:
  data segment: RAM_SPO
  text segment: TEXT_SPO_RAM
флаги в настройках проекта:
  препроцессор: -D__ALLOW_PRINT__ -D__MACHINE__=__CBK__ -D_TOPSEG_OFF_ -D_NOK_2_MOD_
  компилятор:   -D__ALLOW_PRINT__ -D__MACHINE__=__CBK__ -D_TOPSEG_OFF_ -D_NOK_2_MOD_ -O2

[ЦБК = НОК, загрузка в ROM, c печатью (отладка)]
адреса в настройках проекта:
  data segment: RAM_SPO
  text segment: ROM_SPO
флаги в настройках проекта:
  препроцессор: -D__ALLOW_PRINT__ -D__MACHINE__=__CBK__ -D_TOPSEG_OFF_ -D_NOK_2_MOD_
  компилятор:   -D__ALLOW_PRINT__ -D__MACHINE__=__CBK__ -D_TOPSEG_OFF_ -D_NOK_2_MOD_ -O2

[ЦБК = НОК, загрузка в ROM, без печати (работа)]
адреса в настройках проекта:
  data segment: RAM_SPO
  text segment: ROM_SPO
флаги в настройках проекта:
  препроцессор: -D__MACHINE__=__CBK__ -D_TOPSEG_OFF_ -D_NOK_2_MOD_
  компилятор:   -D__MACHINE__=__CBK__ -D_TOPSEG_OFF_ -D_NOK_2_MOD_ -O2

[ЦБК = цех, загрузка в RAM, с печатью (отладка)]
адреса в настройках проекта:
  data segment: RAM_SPO
  text segment: TEXT_SPO_RAM
флаги в настройках проекта:
  препроцессор: -D__ALLOW_PRINT__ -D_SPO_IN_SYS_ -D__MACHINE__=__CBK__ -D_TOPSEG_OFF_ -D_NOK_2_MOD_
  компилятор:   -D__ALLOW_PRINT__ -D_SPO_IN_SYS_ -D__MACHINE__=__CBK__ -D_TOPSEG_OFF_ -D_NOK_2_MOD_ -O2

[ЦБК = цех, загрузка в ROM, c печатью (отладка)]
адреса в настройках проекта:
  data segment: RAM_SPO
  text segment: ROM_SPO
флаги в настройках проекта:
  препроцессор: -D__ALLOW_PRINT__ -D_SPO_IN_SYS_ -D__MACHINE__=__CBK__ -D_TOPSEG_OFF_ -D_NOK_2_MOD_
  компилятор:   -D__ALLOW_PRINT__ -D_SPO_IN_SYS_ -D__MACHINE__=__CBK__ -D_TOPSEG_OFF_ -D_NOK_2_MOD_ -O2

[ЦБК = цех, загрузка в ROM, без печати (работа)]
адреса в настройках проекта:
  data segment: RAM_SPO
  text segment: ROM_SPO
флаги в настройках проекта:
  препроцессор: -D_SPO_IN_SYS_ -D__MACHINE__=__CBK__ -D_TOPSEG_OFF_ -D_NOK_2_MOD_
  компилятор:   -D_SPO_IN_SYS_ -D__MACHINE__=__CBK__ -D_TOPSEG_OFF_ -D_NOK_2_MOD_ -O2
*/

/*
[ASN irq number control]

Msg(Req:Set)
{
IrqDisable(ASN)

AsnControlStopTime = GetTime() + Req:Param:ControlTime

AsnControlEnabled = true
AsnWaitForFirstIrq = true

AsnIrqCount = 0
AsnExcIrqCount = 0
AsnMissIrqCount = 0

IrqEnable(ASN)
Msg(Cmd:Done:Ok)
}
//****************************************
*/

/*
IrqHandlerAsn()
{
AsnIrqCount++;

if (AsnWaitForFirstIrq)
  {
  AsnWaitForFirstIrq = false

  AsnEnabledZone = IrqTime + 1000
  AsnIrqAlreadySet = false
  }
else
  {
  if (IrqTime < (AsnEnabledZone - AsnPreDeltaTime)) AsnExcIrqCount++
  else
    {
    if (IrqTime > (AsnEnabledZone + AsnPostDeltaTime)) AsnExcIrqCount++
    else
      {
      if (!AsnIrqAlreadySet) AsnIrqAlreadySet = true; 
      else AsnExcIrqCount++
      }
    }
  }
}
//****************************************
*/

/*
MainThreadHandler()
{
while (1)
  {
  Idle();
  
  if ((AsnControlEnabled) && (!AsnWaitForFirstIrq))
    {
    Time = GetTime()
    if (Time > (AsnEnabledZone + AsnPostDeltaTime))
      {
      if (!AsnIrqAlreadySet) AsnMissCount++;
      AsnIrqAlreadySet = false; 
      AsnEnabledZone += 1sec 
      }
    
    if (Time > AsnControlStopTime) 
      {
      AsnControlEnabled = false 
      IrqDisable(ASN)
      Msg(Result:Asn:AsnIrqCount, AsnPreCount, AsnPostCount) 
      }
    }
  }
}
//****************************************
*/


