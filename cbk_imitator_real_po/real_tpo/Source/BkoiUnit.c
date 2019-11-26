
#include "BkoiUnit.h"
//================================================================================

#ifdef SPO_BKOI
//================================================================================

unsigned Bkoi_I01_Imit_Enabled;
unsigned Bkoi_I01_Index, Bkoi_I01_Overflow;
unsigned Bkoi_I01_Count;
unsigned Bkoi_I01_Interval;
sysTime_t Bkoi_I01_Time;
//================================================================================

unsigned Bkoi_I12_Imit_Enabled;
unsigned Bkoi_I12_Index, Bkoi_I12_Overflow;
unsigned Bkoi_I12_Count;
unsigned Bkoi_I12_Interval;
sysTime_t Bkoi_I12_Time;
//================================================================================

unsigned Bkoi_I03_Imit_Enabled;
unsigned Bkoi_I03_Index, Bkoi_I03_Overflow;
unsigned Bkoi_I03_Count;
unsigned Bkoi_I03_Interval;
sysTime_t Bkoi_I03_Time;
//================================================================================

unsigned Bkoi_I04_Imit_Enabled;
unsigned Bkoi_I04_Index, Bkoi_I04_Overflow;
unsigned Bkoi_I04_Count;
unsigned Bkoi_I04_Interval;
sysTime_t Bkoi_I04_Time;
//================================================================================

unsigned Bkoi_I05_Imit_Enabled;
unsigned Bkoi_I05_Index, Bkoi_I05_Overflow;
unsigned Bkoi_I05_Count;
unsigned Bkoi_I05_Interval;
sysTime_t Bkoi_I05_Time;
//================================================================================

unsigned Bkoi_I06_Imit_Enabled;
unsigned Bkoi_I06_Index, Bkoi_I06_Overflow;
unsigned Bkoi_I06_Count;
unsigned Bkoi_I06_Interval;
sysTime_t Bkoi_I06_Time;
//================================================================================

unsigned Bkoi_I17_Imit_Enabled;
unsigned Bkoi_I17_Index, Bkoi_I17_Overflow;
unsigned Bkoi_I17_Count;
unsigned Bkoi_I17_Interval;
sysTime_t Bkoi_I17_Time;
TBkoi_I17_Array Bkoi_I17_Buffer[450];
//================================================================================

unsigned Bkoi_I18_Imit_Enabled;
unsigned Bkoi_I18_Index, Bkoi_I18_Overflow;
unsigned Bkoi_I18_Count;
unsigned Bkoi_I18_Interval;
sysTime_t Bkoi_I18_Time;
TBkoi_I18_Array Bkoi_I18_Buffer[450];
//================================================================================

void BkoiInit (void)                                                                                                              
{
Bkoi_I01_Imit_Enabled = 0;
Bkoi_I01_Count = 205;

Bkoi_I12_Imit_Enabled = 0;
Bkoi_I12_Count = 205;

// ********

Bkoi_I03_Imit_Enabled = 0;
Bkoi_I03_Count = 205;

Bkoi_I04_Imit_Enabled = 0;
Bkoi_I04_Count = 205;

Bkoi_I05_Imit_Enabled = 0;
Bkoi_I05_Count = 205;

Bkoi_I06_Imit_Enabled = 0;
Bkoi_I06_Count = 820;

// ********

Bkoi_I17_Imit_Enabled = 0;
Bkoi_I17_Count = 450;

Bkoi_I18_Imit_Enabled = 0;
Bkoi_I18_Count = 450;
}
//================================================================================

void KpaCommand66 (int BaseMkoLine, int BaseMkoChan)
{
int Result, Opcode, Param1, Param2;
WORD Buffer[8];

// читаем код операции
Opcode = KpaCommand[0] >> 8;
Param1 = KpaCommand[1];
Param2 = KpaCommand[2];

// включить имитацию массива И1
if (Opcode == 0x01)
  {
  if (Bkoi_I01_Imit_Enabled) { ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_STATE_ERROR, KPA_SUBADDR_IRQ); return; }
  Bkoi_I01_Imit_Enabled = 1;
  Bkoi_I01_Index = Bkoi_I01_Overflow = 0;
  Bkoi_I01_Interval = Param1;
  Bkoi_I01_Time = MainTaskTime1;
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ);
  }
// отключить имитацию массива И1
else if (Opcode == 0x02)
  {
  Bkoi_I01_Imit_Enabled = 0;
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ);
  }
// чтение параметров имитации
else if (Opcode == 0x03)
  {
  Buffer[0] = Bkoi_I01_Imit_Enabled;
  Buffer[1] = Bkoi_I01_Index;
  Buffer[2] = Bkoi_I01_Overflow;
  Buffer[3] = Bkoi_I01_Interval;
  ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ, &Buffer, sizeof(WORD) * 4);
  }

// включить имитацию массива И12
else if (Opcode == 0x11)
  {
  if (Bkoi_I12_Imit_Enabled) { ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_STATE_ERROR, KPA_SUBADDR_IRQ); return; }
  Bkoi_I12_Imit_Enabled = 1;
  Bkoi_I12_Index = Bkoi_I12_Overflow = 0;
  Bkoi_I12_Interval = Param1;
  Bkoi_I12_Time = MainTaskTime1;
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ);
  }
// отключить имитацию массива И12
else if (Opcode == 0x12)
  {
  Bkoi_I12_Imit_Enabled = 0;
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ);
  }
// чтение параметров имитации
else if (Opcode == 0x13)
  {
  Buffer[0] = Bkoi_I12_Imit_Enabled;
  Buffer[1] = Bkoi_I12_Index;
  Buffer[2] = Bkoi_I12_Overflow;
  Buffer[3] = Bkoi_I12_Interval;
  ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ, &Buffer, sizeof(WORD) * 4);
  }

// включить имитацию массива И03
else if (Opcode == 0x21)
  {
  if (Bkoi_I03_Imit_Enabled) { ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_STATE_ERROR, KPA_SUBADDR_IRQ); return; }
  Bkoi_I03_Imit_Enabled = 1;
  Bkoi_I03_Index = Bkoi_I03_Overflow = 0;
  Bkoi_I03_Interval = Param1;
  Bkoi_I03_Time = MainTaskTime1;
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ);
  }
// отключить имитацию массива И03
else if (Opcode == 0x22)
  {
  Bkoi_I03_Imit_Enabled = 0;
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ);
  }
// чтение параметров имитации
else if (Opcode == 0x23)
  {
  Buffer[0] = Bkoi_I03_Imit_Enabled;
  Buffer[1] = Bkoi_I03_Index;
  Buffer[2] = Bkoi_I03_Overflow;
  Buffer[3] = Bkoi_I03_Interval;
  ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ, &Buffer, sizeof(WORD) * 4);
  }

// включить имитацию массива И04
else if (Opcode == 0x31)
  {
  if (Bkoi_I04_Imit_Enabled) { ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_STATE_ERROR, KPA_SUBADDR_IRQ); return; }
  Bkoi_I04_Imit_Enabled = 1;
  Bkoi_I04_Index = Bkoi_I04_Overflow = 0;
  Bkoi_I04_Interval = Param1;
  Bkoi_I04_Time = MainTaskTime1;
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ);
  }
// отключить имитацию массива И04
else if (Opcode == 0x32)
  {
  Bkoi_I04_Imit_Enabled = 0;
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ);
  }
// чтение параметров имитации
else if (Opcode == 0x33)
  {
  Buffer[0] = Bkoi_I04_Imit_Enabled;
  Buffer[1] = Bkoi_I04_Index;
  Buffer[2] = Bkoi_I04_Overflow;
  Buffer[3] = Bkoi_I04_Interval;
  ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ, &Buffer, sizeof(WORD) * 4);
  }

// включить имитацию массива И05
else if (Opcode == 0x41)
  {
  if (Bkoi_I05_Imit_Enabled) { ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_STATE_ERROR, KPA_SUBADDR_IRQ); return; }
  Bkoi_I05_Imit_Enabled = 1;
  Bkoi_I05_Index = Bkoi_I05_Overflow = 0;
  Bkoi_I05_Interval = Param1;
  Bkoi_I05_Time = MainTaskTime1;
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ);
  }
// отключить имитацию массива И05
else if (Opcode == 0x42)
  {
  Bkoi_I05_Imit_Enabled = 0;
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ);
  }
// чтение параметров имитации
else if (Opcode == 0x43)
  {
  Buffer[0] = Bkoi_I05_Imit_Enabled;
  Buffer[1] = Bkoi_I05_Index;
  Buffer[2] = Bkoi_I05_Overflow;
  Buffer[3] = Bkoi_I05_Interval;
  ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ, &Buffer, sizeof(WORD) * 4);
  }

// включить имитацию массива И06
else if (Opcode == 0x51)
  {
  if (Bkoi_I06_Imit_Enabled) { ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_STATE_ERROR, KPA_SUBADDR_IRQ); return; }
  Bkoi_I06_Imit_Enabled = 1;
  Bkoi_I06_Index = Bkoi_I06_Overflow = 0;
  Bkoi_I06_Interval = Param1;
  Bkoi_I06_Time = MainTaskTime1;
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ);
  }
// отключить имитацию массива И06
else if (Opcode == 0x52)
  {
  Bkoi_I06_Imit_Enabled = 0;
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ);
  }
// чтение параметров имитации
else if (Opcode == 0x53)
  {
  Buffer[0] = Bkoi_I06_Imit_Enabled;
  Buffer[1] = Bkoi_I06_Index;
  Buffer[2] = Bkoi_I06_Overflow;
  Buffer[3] = Bkoi_I06_Interval;
  ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ, &Buffer, sizeof(WORD) * 4);
  }

// включить имитацию массива И17
else if (Opcode == 0x61)
  {
  if (Bkoi_I17_Imit_Enabled) { ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_STATE_ERROR, KPA_SUBADDR_IRQ); return; }
  Bkoi_I17_Imit_Enabled = 1;
  Bkoi_I17_Index = Bkoi_I17_Overflow = 0;
  Bkoi_I17_Interval = Param1;
  Bkoi_I17_Time = MainTaskTime1;
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ);
  }
// отключить имитацию массива И17
else if (Opcode == 0x62)
  {
  Bkoi_I17_Imit_Enabled = 0;
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ);
  }
// чтение параметров имитации
else if (Opcode == 0x63)
  {
  Buffer[0] = Bkoi_I17_Imit_Enabled;
  Buffer[1] = Bkoi_I17_Index;
  Buffer[2] = Bkoi_I17_Overflow;
  Buffer[3] = Bkoi_I17_Interval;
  ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ, &Buffer, sizeof(WORD) * 4);
  }
// чтение блока OK И17 по индексу
else if (Opcode == 0x64)
  {
  if ((Param1 < 0) || (Param1 >= Bkoi_I17_Count))
    {
    ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE | REPLY_FLAG_ERROR, KPA_SUBADDR_IRQ);
    return;
    }
  if ((Param2 < 0) || (Param2 > 1))
    {
    ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE | REPLY_FLAG_ERROR, KPA_SUBADDR_IRQ);
    return;
    }
  if (Param2 == 0) ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ, &Bkoi_I17_Buffer[Param1][0], sizeof(WORD) * 32);
  else ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ, &Bkoi_I17_Buffer[Param1][32], sizeof(WORD) * 24);
  }

// включить имитацию массива И18
else if (Opcode == 0x71)
  {
  if (Bkoi_I18_Imit_Enabled) { ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_STATE_ERROR, KPA_SUBADDR_IRQ); return; }
  Bkoi_I18_Imit_Enabled = 1;
  Bkoi_I18_Index = Bkoi_I18_Overflow = 0;
  Bkoi_I18_Interval = Param1;
  Bkoi_I18_Time = MainTaskTime1;
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ);
  }
// отключить имитацию массива И18
else if (Opcode == 0x72)
  {
  Bkoi_I18_Imit_Enabled = 0;
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ);
  }
// чтение параметров имитации
else if (Opcode == 0x73)
  {
  Buffer[0] = Bkoi_I18_Imit_Enabled;
  Buffer[1] = Bkoi_I18_Index;
  Buffer[2] = Bkoi_I18_Overflow;
  Buffer[3] = Bkoi_I18_Interval;
  ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ, &Buffer, sizeof(WORD) * 4);
  }
// чтение блока OK И18 по индексу
else if (Opcode == 0x74)
  {
  if ((Param1 < 0) || (Param1 >= Bkoi_I18_Count))
    {
    ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE | REPLY_FLAG_ERROR, KPA_SUBADDR_IRQ);
    return;
    }
  ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ, &Bkoi_I18_Buffer[Param1], sizeof(TBkoi_I18_Array));
  }

// неизвестная операция
else
  {
  Result = REPLY_CMD_DATA_ERROR;
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, Result, KPA_SUBADDR_IRQ);
  }
}  
//================================================================================

void Bkoi_I01_DoImit (sysTime_t *Time)
{
int DevMkoLine, DevMkoChan;

if (DeltaTimeMSec(&Bkoi_I01_Time, Time) >= (Bkoi_I01_Interval * 1000))
  {
  Bkoi_I01_Time.time_sec += Bkoi_I01_Interval;

  // параметры МКО для обмена
  DevMkoLine = SpoConfig.DevMkoLine & 1;
  DevMkoChan = SpoConfig.DevMkoChan & 1;

  if (Bkoi_I01_Index >= Bkoi_I01_Count) { Bkoi_I01_Index = 0; Bkoi_I01_Overflow = 1; }
  memmove(&MkoBufA[0], &Bkoi_I01_Buffer[Bkoi_I01_Index], sizeof(TBkoi_I01_Array));
  MkoXchgAn(DevMkoLine, DevMkoChan, MKO_CMD(23, 3, MKO_TX, 17), 0);
  Bkoi_I01_Index++;
  }
}
//================================================================================

void Bkoi_I12_DoImit (sysTime_t *Time)
{
int DevMkoLine, DevMkoChan;

if (DeltaTimeMSec(&Bkoi_I12_Time, Time) >= (Bkoi_I12_Interval * 1000))
  {
  Bkoi_I12_Time.time_sec += Bkoi_I12_Interval;

  // параметры МКО для обмена
  DevMkoLine = SpoConfig.DevMkoLine & 1;
  DevMkoChan = SpoConfig.DevMkoChan & 1;

  if (Bkoi_I12_Index >= Bkoi_I12_Count) { Bkoi_I12_Index = 0; Bkoi_I12_Overflow = 1; }
  memmove(&MkoBufA[0], &Bkoi_I12_Buffer[Bkoi_I12_Index], sizeof(TBkoi_I12_Array));
  MkoXchgAn(DevMkoLine, DevMkoChan, MKO_CMD(23, 13, MKO_TX, 2), 0);
  Bkoi_I12_Index++;
  }
}
//================================================================================

void Bkoi_I03_DoImit (sysTime_t *Time)
{
int DevMkoLine, DevMkoChan;

if (DeltaTimeMSec(&Bkoi_I03_Time, Time) >= (Bkoi_I03_Interval * 1000))
  {
  Bkoi_I03_Time.time_sec += Bkoi_I03_Interval;

  // параметры МКО для обмена
  DevMkoLine = SpoConfig.DevMkoLine & 1;
  DevMkoChan = SpoConfig.DevMkoChan & 1;

  if (Bkoi_I03_Index >= Bkoi_I03_Count) { Bkoi_I03_Index = 0; Bkoi_I03_Overflow = 1; }
  memmove(&MkoBufA[0], &Bkoi_I03_Buffer[Bkoi_I03_Index], sizeof(TBkoi_I03_Array));
  MkoXchgAn(DevMkoLine, DevMkoChan, MKO_CMD(23, 5, MKO_TX, 26), 0);
  Bkoi_I03_Index++;
  }
}
//================================================================================

void Bkoi_I04_DoImit (sysTime_t *Time)
{
int DevMkoLine, DevMkoChan;

if (DeltaTimeMSec(&Bkoi_I04_Time, Time) >= (Bkoi_I04_Interval * 1000))
  {
  Bkoi_I04_Time.time_sec += Bkoi_I04_Interval;

  // параметры МКО для обмена
  DevMkoLine = SpoConfig.DevMkoLine & 1;
  DevMkoChan = SpoConfig.DevMkoChan & 1;

  if (Bkoi_I04_Index >= Bkoi_I04_Count) { Bkoi_I04_Index = 0; Bkoi_I04_Overflow = 1; }
  memmove(&MkoBufA[0], &Bkoi_I04_Buffer[Bkoi_I04_Index], sizeof(TBkoi_I04_Array));
  MkoXchgAn(DevMkoLine, DevMkoChan, MKO_CMD(23, 6, MKO_TX, 28), 0);
  Bkoi_I04_Index++;
  }
}
//================================================================================

void Bkoi_I05_DoImit (sysTime_t *Time)
{
int DevMkoLine, DevMkoChan;

if (DeltaTimeMSec(&Bkoi_I05_Time, Time) >= (Bkoi_I05_Interval * 1000))
  {
  Bkoi_I05_Time.time_sec += Bkoi_I05_Interval;

  // параметры МКО для обмена
  DevMkoLine = SpoConfig.DevMkoLine & 1;
  DevMkoChan = SpoConfig.DevMkoChan & 1;

  if (Bkoi_I05_Index >= Bkoi_I05_Count) { Bkoi_I05_Index = 0; Bkoi_I05_Overflow = 1; }
  memmove(&MkoBufA[0], &Bkoi_I05_Buffer[Bkoi_I05_Index], sizeof(TBkoi_I05_Array));
  MkoXchgAn(DevMkoLine, DevMkoChan, MKO_CMD(23, 7, MKO_TX, 21), 0);
  Bkoi_I05_Index++;
  }
}
//================================================================================

void Bkoi_I06_DoImit (sysTime_t *Time)
{
int DevMkoLine, DevMkoChan, Index, Len;
TBkoi_I06_Array *Buffer;

if (DeltaTimeMSec(&Bkoi_I06_Time, Time) >= (Bkoi_I06_Interval * 1000))
  {
  Bkoi_I06_Time.time_sec += Bkoi_I06_Interval;

  // параметры МКО для обмена
  DevMkoLine = SpoConfig.DevMkoLine & 1;
  DevMkoChan = SpoConfig.DevMkoChan & 1;

  if (Bkoi_I06_Index >= Bkoi_I06_Count) { Bkoi_I06_Index = 0; Bkoi_I06_Overflow = 1; }
  Buffer = (TBkoi_I06_Array*) &Bkoi_I06_Buffer[Bkoi_I06_Index];

  for (Index = 0; Index < 8; Index++)
    {
    Len = (Index != 7) ? (32) : (28);
    memmove(&MkoBufA[0], &Buffer[Index], sizeof(WORD) * Len);
    MkoXchgAn(DevMkoLine, DevMkoChan, MKO_CMD(23, 19 + Index, MKO_TX, Len), 0);
    }
  Bkoi_I06_Index++;
  }
}
//================================================================================

void Bkoi_I17_DoImit (sysTime_t *Time)
{
int DevMkoLine, DevMkoChan;
TBkoi_I17_Array *Buffer;

if (DeltaTimeMSec(&Bkoi_I17_Time, Time) >= (Bkoi_I17_Interval * 1000))
  {
  Bkoi_I17_Time.time_sec += Bkoi_I17_Interval;

  // параметры МКО для обмена
  DevMkoLine = SpoConfig.DevMkoLine & 1;
  DevMkoChan = SpoConfig.DevMkoChan & 1;

  if (Bkoi_I17_Index >= Bkoi_I17_Count) { Bkoi_I17_Overflow = 1; return; }
  Buffer = &Bkoi_I17_Buffer[Bkoi_I17_Index];
  MkoXchgAn(DevMkoLine, DevMkoChan, MKO_CMD(11, 1, MKO_RX, 32), 0);
  memmove(&Buffer[0], &MkoBufA[0], sizeof(WORD) * 32);
  MkoXchgAn(DevMkoLine, DevMkoChan, MKO_CMD(11, 1, MKO_RX, 24), 0);
  memmove(&Buffer[32], &MkoBufA[0], sizeof(WORD) * 24);
  Bkoi_I17_Index++;
  }
}
//================================================================================

void Bkoi_I18_DoImit (sysTime_t *Time)
{
int DevMkoLine, DevMkoChan;

if (DeltaTimeMSec(&Bkoi_I18_Time, Time) >= (Bkoi_I18_Interval * 1000))
  {
  Bkoi_I18_Time.time_sec += Bkoi_I18_Interval;

  // параметры МКО для обмена
  DevMkoLine = SpoConfig.DevMkoLine & 1;
  DevMkoChan = SpoConfig.DevMkoChan & 1;

  if (Bkoi_I18_Index >= Bkoi_I18_Count) { Bkoi_I18_Overflow = 1; return; }
  MkoXchgAn(DevMkoLine, DevMkoChan, MKO_CMD(23, 16, MKO_RX, 31), 0);
  memmove(&Bkoi_I18_Buffer[Bkoi_I18_Index], &MkoBufA[0], sizeof(WORD) * 31);
  Bkoi_I18_Index++;
  }
}
//================================================================================

#endif

                                                                                       