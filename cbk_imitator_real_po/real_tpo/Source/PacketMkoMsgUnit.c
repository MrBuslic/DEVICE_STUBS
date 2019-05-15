
/*! \file PacketMkoMsgUnit.c
 * \author Гапонов Р.В.
 * \date 2014.05.13
 * \version 1.0.0
 * \brief Модуль содержит функции выдачи пакетов разных сообщений по МКО
 */

#include "MainUnit.h"
//================================================================================

int PacketMkoMsgCount;                                                          // число сообщений в списке
TPacketMkoMsg PacketMkoMsgList[PACKET_MKO_MSG_COUNT];                           // список описателей сообщений МКО

int PacketMkoIntCount;							        // число интервалов в списке (интервал - секунды, режим обмена - пинг-понг)
TPacketMkoInt PacketMkoIntList[PACKET_MKO_INT_COUNT];                           // список описателей интервалов 

int PacketMkoIntCntCount;				        	        // число интервалов в списке (интервал - единицы пакетов, режим обмена - пинг-понг)
TPacketMkoIntCnt PacketMkoIntCntList[PACKET_MKO_INT_COUNT];                     // список описателей интервалов 

int PacketMkoIntQueueCount;	                          	                // число интервалов в списке (интервал - секунды, режим обмена - очередь) 
TPacketMkoInt PacketMkoIntQueue0List[PACKET_MKO_INT_COUNT];                     // список описателей интервалов 
TPacketMkoInt PacketMkoIntQueue1List[PACKET_MKO_INT_COUNT];                     // список описателей интервалов 

TPacketMkoMsgResult PacketMkoMsgResultList[PACKET_MKO_MSG_COUNT];               // результат обмена для сообщений МКО, если MsgNum = 0xFFFF - обмен не проводился
TPacketMkoMsgResult PacketMkoMsgResultBuf[PACKET_MKO_MSG_COUNT + 2];            // буфер для передачи результата
int LastResult;                                                                 // результат последнего обмена

sysTime_t RunPacketBeginTime, RunPacketCurrTime;                         	// время начала прогона/текущее время
//================================================================================

int GRES_DBG;
UINT TrackPack, TrackPackCount;
UINT PacketRdDataInfo[2];
//================================================================================

// [номера слов] (option: [номера битов])

// данные на подадресе 1:
// [0][7..0] - код команды, = 0x63
// [0][15..8] - код операции, = 1..N

// код операции = 0x01, добавление сообщения в пакет сообщений МКО
// [1][15..0] - команда абоненту МКО
// [2][0] - линия МКО для обмена, 0/1
// [2][1] - канал МКО для обмена, 0/1
// [2][2] - 0 = обычный обмен, 1 = команда в формате Ф4, 
// [3] - задержка до следующего сообщения в пакете, мксек (мл слово) (0..200000)
// [4] - задержка до следующего сообщения в пакете, мксек (ст слово) (0..200000)
// если добавляется сообщение на запись абонента:
// данные на подадресе 2:
// [n..0] - данные для записи в абонент
// возврат, результат: OK/ошибка
// возврат, данные: нет

// код операции = 0x02, удаление сообщения из пакета сообщений МКО (параметр = сообщение) 
// [1][15..0] - команда абоненту МКО, удаляемая из списка
// [2][0] - линия МКО для обмена, 0/1
// [2][1] - канал МКО для обмена, 0/1
// [2][2] - 0 = обычный обмен, 1 = команда в формате Ф4, 
// [3] - задержка до следующего сообщения в пакете, мксек (мл слово) (0..200000)
// [4] - задержка до следующего сообщения в пакете, мксек (ст слово) (0..200000)
// если удаляется сообщение на запись абонента:
// данные на подадресе 2:
// [n..0] - данные для записи в абонент
// возврат, результат: OK/ошибка
// возврат, данные: нет

// код операции = 0x03, удаление сообщения из пакета сообщений МКО (параметр = индекс сообщения) 
// [1][15..0] - индекс сообщения, удаляемого из списка (0..n)
// возврат, результат: OK/ошибка
// возврат, данные: нет

// код операции = 0x04, получить количество сообщений в пакете сообщений МКО
// возврат, результат: OK
// возврат, данные: 1 слово, = число сообщений в списке

// код операции = 0x05, очистить список пакета сообщений МКО
// возврат, результат: OK/ошибка

// код операции = 0x06, выполнить обмен для пакета сообщений МКО
// возврат, результат: OK/ошибка
// возврат, данные: при ошибках выдает набор записей TPacketMkoMsgResult по числу сбойных обменов

// код операции = 0x07, заменить адреса МКО для сообщений из списка
// возврат, результат: OK/ошибка

// код операции = 0x08, заменить подадреса МКО для сообщений из списка
// возврат, результат: OK/ошибка

// код операции = 0x09, сброс буфера накопленных данных при чтении
// возврат, результат: OK

// код операции = 0x0A, проверка буфера на синхронность в прочими ВМ
// возврат, результат: OK/Ошибка+Код сбойных ВМ

// код операции = 0x10, добавление нескольких сообщений в пакет сообщений МКО

// код операции = 0x16, выполнить обмен для пакета сообщений МКО (multi msg packet)

// код операции = 0x17, передать результат последнего обмена
                       
// код операции = 0x20, добавление интервала в список интервалов (секунды, пинг-понг)
// возврат, результат: OK/ошибка
// код операции = 0x21, добавление указанного сообщения к указанному интервалу
// возврат, результат: OK/ошибка
// код операции = 0x22, запуск прогона сообщений по интервалам
// возврат, результат: OK/ошибка

// код операции = 0x28, добавление интервала в список интервалов (пакеты, пинг-понг)
// возврат, результат: OK/ошибка
// код операции = 0x29, добавление указанного сообщения к указанному интервалу
// возврат, результат: OK/ошибка
// код операции = 0x2A, запуск прогона сообщений по интервалам
// возврат, результат: OK/ошибка

// код операции = 0x30, добавление интервала в список интервалов (секунды, очередь)
// возврат, результат: OK/ошибка
// код операции = 0x31, добавление указанного сообщения к указанному интервалу
// возврат, результат: OK/ошибка
// код операции = 0x32, запуск прогона сообщений по интервалам
// возврат, результат: OK/ошибка

// код операции = 0x38, добавление интервала в список интервалов (пакеты, очередь)
// возврат, результат: OK/ошибка
// код операции = 0x39, добавление указанного сообщения к указанному интервалу
// возврат, результат: OK/ошибка
// код операции = 0x3A, запуск прогона сообщений по интервалам
// возврат, результат: OK/ошибка
//================================================================================

void InitPacketMkoMsg (void)
{
ClrPacketMkoMsgList();
}
//================================================================================

void KpaCommand63 (int BaseMkoLine, int BaseMkoChan)
{
int Opcode, Result;
UINT Buffer[2];

// читаем код операции
Opcode = KpaCommand[0] >> 8;

// добавление сообщения в пакет сообщений МКО
if (Opcode == 0x01)
  {
  Result = AddPacketMkoSingleMsg(BaseMkoLine, BaseMkoChan);
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, Result, KPA_SUBADDR_IRQ);
  }
// удаление сообщения из пакета сообщений МКО (параметр = сообщение)
else if (Opcode == 0x02)
  {
  Result = DelPacketMkoMsgByMsg(BaseMkoLine, BaseMkoChan);
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, Result, KPA_SUBADDR_IRQ);
  }
// удаление сообщения из пакета сообщений МКО (параметр = индекс сообщения)
else if (Opcode == 0x03)
  {
  Result = DelPacketMkoMsgByIndex(BaseMkoLine, BaseMkoChan);
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, Result, KPA_SUBADDR_IRQ);
  }
// получить количество сообщений в пакете сообщений МКО
else if (Opcode == 0x04)
  {
  Result = GetPacketMkoMsgCount();
  ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ, &Result, 2);
  }
// очистить список пакета сообщений МКО
else if (Opcode == 0x05)
  {
  Result = ClrPacketMkoMsgList();
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, Result, KPA_SUBADDR_IRQ);
  }
// выполнить обмен для пакета сообщений МКО (single msg packet)
else if (Opcode == 0x06)
  {
  LastResult = RunPacketMkoSingleMsg(BaseMkoLine, BaseMkoChan);
  RunPacketMkoSingleMsgResult(BaseMkoLine, BaseMkoChan, LastResult);
  }
// заменить адреса МКО для сообщений из списка
else if (Opcode == 0x07)
  {
  Result = XchgAddrPacketMkoMsgList();
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, Result, KPA_SUBADDR_IRQ);
  }
// заменить подадреса МКО для сообщений из списка
else if (Opcode == 0x08)
  {
  Result = XchgSubAddrPacketMkoMsgList();
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, Result, KPA_SUBADDR_IRQ);
  }
// сброс буфера накопленных данных при чтении
else if (Opcode == 0x09)
  {
  PacketRdDataInfo[0] = 0; // data size
  PacketRdDataInfo[1] = CalcCrc32Init(); // crc init
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ); 
  }
// проверка буфера на синхронность в прочими ВМ
else if (Opcode == 0x0A)
  {
  Buffer[0] = PacketRdDataInfo[0];
  Buffer[1] = synchInf(1, (UINT*) &PacketRdDataInfo, (USHORT)(sizeof(PacketRdDataInfo) / sizeof(UINT)));
  ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ, &Buffer, sizeof(Buffer)); 
  }
// добавление нескольких сообщений в пакет сообщений МКО
else if (Opcode == 0x10)
  {
  Result = AddPacketMkoMultiMsg(BaseMkoLine, BaseMkoChan);
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, Result, KPA_SUBADDR_IRQ);
  }
// выполнить обмен для пакета сообщений МКО (multi msg packet)
else if (Opcode == 0x16)
  {
  LastResult = RunPacketMkoMultiMsg(BaseMkoLine, BaseMkoChan);
  RunPacketMkoMultiMsgResult(BaseMkoLine, BaseMkoChan, LastResult);
  }
// передать результат последнего обмена
else if (Opcode == 0x17)
  {
  RunPacketMkoMultiMsgResult(BaseMkoLine, BaseMkoChan, LastResult);
  }

// добавление интервала в список интервалов (секунды, пинг-понг)
else if (Opcode == 0x20)
  {
  Result = AddPacketMko_SecPiPo_IntToList(BaseMkoLine, BaseMkoChan);
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, Result, KPA_SUBADDR_IRQ);
  }
// добавление указанного сообщения к указанному интервалу
else if (Opcode == 0x21)
  {
  Result = AddPacketMko_SecPiPo_MsgToInt(BaseMkoLine, BaseMkoChan);
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, Result, KPA_SUBADDR_IRQ);
  }
// запуск прогона сообщений по интервалам
else if (Opcode == 0x22)
  {
  Result = RunPacketMko_SecPiPo_IntList(BaseMkoLine, BaseMkoChan);
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, Result, KPA_SUBADDR_IRQ);
  }

// добавление интервала в список интервалов (пакеты, пинг-понг)
else if (Opcode == 0x28)
  {
  Result = AddPacketMko_CntPiPo_IntToList(BaseMkoLine, BaseMkoChan);
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, Result, KPA_SUBADDR_IRQ);
  }
// добавление указанного сообщения к указанному интервалу
else if (Opcode == 0x29)
  {
  Result = AddPacketMko_CntPiPo_MsgToInt(BaseMkoLine, BaseMkoChan);
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, Result, KPA_SUBADDR_IRQ);
  }
// запуск прогона сообщений по интервалам
else if (Opcode == 0x2A)
  {
  Result = RunPacketMko_CntPiPo_IntList(BaseMkoLine, BaseMkoChan);
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, Result, KPA_SUBADDR_IRQ);
  }

// добавление интервала в список интервалов (секунды, очередь)
else if (Opcode == 0x30)
  {
  Result = AddPacketMko_SecQue_IntToList(BaseMkoLine, BaseMkoChan);
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, Result, KPA_SUBADDR_IRQ);
  }
// добавление указанного сообщения к указанному интервалу
else if (Opcode == 0x31)
  {
  Result = AddPacketMko_SecQue_MsgToInt(BaseMkoLine, BaseMkoChan);
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, Result, KPA_SUBADDR_IRQ);
  }
// запуск прогона сообщений по интервалам
else if (Opcode == 0x32)
  {
  Result = RunPacketMko_SecQue_IntList(BaseMkoLine, BaseMkoChan);
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, Result, KPA_SUBADDR_IRQ);
  }

// добавление интервала в список интервалов (пакеты, очередь)
else if (Opcode == 0x38)
  {
  Result = AddPacketMko_CntQue_IntToList(BaseMkoLine, BaseMkoChan);
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, Result, KPA_SUBADDR_IRQ);
  }
// добавление указанного сообщения к указанному интервалу
else if (Opcode == 0x39)
  {
  Result = AddPacketMko_CntQue_MsgToInt(BaseMkoLine, BaseMkoChan);
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, Result, KPA_SUBADDR_IRQ);
  }
// запуск прогона сообщений по интервалам
else if (Opcode == 0x3A)
  {
  Result = RunPacketMko_CntQue_IntList(BaseMkoLine, BaseMkoChan);
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

// добавление сообщения в пакет сообщений МКО
int AddPacketMkoSingleMsg (int BaseMkoLine, int BaseMkoChan)
{
int Result, Index;
WORD MkoCmd, Cmd, F4Mode, Data[32];
DWORD Interval;
 
// описание добавляемого сообщения пакета
MkoCmd   = KpaCommand[1];
F4Mode   = (KpaCommand[2] & 0x01) ? (1) : (0);
Interval = (((DWORD) KpaCommand[4]) << 16) | (KpaCommand[3]);

// проверка параметров на допустимость
if (PacketMkoMsgCount >= PACKET_MKO_MSG_COUNT) return REPLY_CMD_OVERFLOW_ERROR;
if (Interval > 250000) return REPLY_CMD_DATA_ERROR;

// читаем данные для режима передачи
if ((MKO_CMD_DIR(MkoCmd) == MKO_TX) && (!F4Mode))
  {     
  Cmd = MKO_CMD(KPA_ADDR, KPA_SUBADDR_CMD + 1, MKO_RX, MKO_CMD_SIZE(MkoCmd));
  Result = ReadPacketMkoMsgData(BaseMkoLine, BaseMkoChan, Cmd, KpaCommand[7], (WORD*) &Data);
  if (Result != REPLY_CMD_DONE) return Result;
  }

// добавление сообщения в список
Index = PacketMkoMsgCount++;
PacketMkoMsgList[Index].MkoCmd   = MkoCmd;
PacketMkoMsgList[Index].F4Mode   = F4Mode;
PacketMkoMsgList[Index].Interval = Interval;
memcpy(&PacketMkoMsgList[Index].Data, &Data, sizeof(Data));

return REPLY_CMD_DONE;
}
//================================================================================

int AddPacketMkoMultiMsg (int BaseMkoLine, int BaseMkoChan)
{
int Result, Index, Count, StartIndex, MsgCount, Interleave;
WORD MkoCmd, Cmd, F4Mode, Interval, Data[32];

// описание добавляемого сообщения пакета
MkoCmd     = KpaCommand[1];
F4Mode     = (KpaCommand[2] & 0x8000) ? (1) : (0);
Interval   = KpaCommand[2] & 0x7FFF;
StartIndex = KpaCommand[3];
MsgCount   = KpaCommand[4];
Interleave = KpaCommand[5];

// проверка параметров на допустимость
if (StartIndex >= PACKET_MKO_MSG_COUNT) return REPLY_CMD_OVERFLOW_ERROR;
if (MsgCount == 0) return REPLY_CMD_DATA_ERROR;
if (Interval > 30000) return REPLY_CMD_DATA_ERROR;

for (Index = StartIndex, Count = MsgCount; Count; Index += Interleave, Count--)
  {
  if (Index >= PACKET_MKO_MSG_COUNT) return REPLY_CMD_OVERFLOW_ERROR;
  }  

// читаем данные для режима передачи
if ((MKO_CMD_DIR(MkoCmd) == MKO_TX) && (!F4Mode))
  {     
  Cmd = MKO_CMD(KPA_ADDR, KPA_SUBADDR_CMD + 1, MKO_RX, MKO_CMD_SIZE(MkoCmd));
  Result = ReadPacketMkoMsgData(BaseMkoLine, BaseMkoChan, Cmd, KpaCommand[7], (WORD*) &Data);
  if (Result != REPLY_CMD_DONE) return Result;
  }

for (Index = StartIndex, Count = MsgCount; Count; Index += Interleave, Count--)
  {
  PacketMkoMsgList[Index].MkoCmd   = MkoCmd;
  PacketMkoMsgList[Index].F4Mode   = F4Mode;
  PacketMkoMsgList[Index].Interval = Interval;
  memcpy(&PacketMkoMsgList[Index].Data, &Data, sizeof(Data));

  if (PacketMkoMsgCount <= Index) PacketMkoMsgCount = Index + 1;
  }

return REPLY_CMD_DONE;
}
//================================================================================

// считывание дополнительных данных для сообщения
int ReadPacketMkoMsgData (int BaseMkoLine, int BaseMkoChan, WORD Cmd, WORD DataCrc, WORD *Buffer)
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

// удаление сообщения из пакета сообщений МКО (параметр = сообщение)
int DelPacketMkoMsgByMsg (int BaseMkoLine, int BaseMkoChan)
{
int Result, Index, F4Mode;
WORD MkoCmd, Cmd, Data[32];
DWORD Interval;

// описание удаляемого сообщения пакета
MkoCmd   = KpaCommand[1];
F4Mode   = (KpaCommand[2] & 0x0001) ? (1) : (0);
Interval = KpaCommand[4];
Interval = (Interval << 16) | KpaCommand[3];

// проверка параметров на допустимость
if (Interval > 250000) return REPLY_CMD_DATA_ERROR;

// читаем данные для режима передачи
if ((MKO_CMD_DIR(MkoCmd) == MKO_TX) && (!F4Mode))
  {    
  Cmd = MKO_CMD(KPA_ADDR, KPA_SUBADDR_CMD + 1, MKO_RX, MKO_CMD_SIZE(MkoCmd));
  Result = ReadPacketMkoMsgData(BaseMkoLine, BaseMkoChan, Cmd, KpaCommand[7], (WORD*) &Data);
  if (Result != REPLY_CMD_DONE) return Result;
  }

// ищем сообщение в списке
if (!FindPacketMkoMsg(MkoCmd, F4Mode, Interval, (WORD*) &Data, &Index)) return REPLY_ITEM_NOT_FOUND;

// удаляем сообщение из списка
for ( ; Index < (PacketMkoMsgCount - 1); Index++) PacketMkoMsgList[Index] = PacketMkoMsgList[Index + 1];

PacketMkoMsgCount--;
return REPLY_CMD_DONE;
}
//================================================================================

// поиск сообщения по параметрам
int FindPacketMkoMsg (WORD MkoCmd, int F4Mode, DWORD Interval, WORD *Data, int *ItemIndex)
{
int Index, Found;

for (Index = 0, Found = 0; Index < PacketMkoMsgCount; Index++)
  {
  if (PacketMkoMsgList[Index].MkoCmd != MkoCmd) continue;
  if (PacketMkoMsgList[Index].F4Mode != F4Mode) continue;
  if (PacketMkoMsgList[Index].Interval != Interval) continue;

  if ((!F4Mode) && (MKO_CMD_DIR(MkoCmd) == MKO_TX))
    {
    if (PacketMkoMsgDataCmp(Data, (WORD*) &PacketMkoMsgList[Index].Data, MKO_CMD_SIZE(MkoCmd))) continue;
    }

  *ItemIndex = Index; 
  Found = 1; 
  break;
  }

return Found;
}
//================================================================================

// сравнение данных двух сообщений
int PacketMkoMsgDataCmp (WORD *Data1, WORD *Data2, int Count)
{
while (Count--) { if (*Data1++ != *Data2++) return 1; }
return 0;
}
//================================================================================

// удаление сообщения из пакета сообщений МКО (параметр = индекс сообщения)
int DelPacketMkoMsgByIndex (int BaseMkoLine, int BaseMkoChan)
{
int Index;

// индекс удаляемого сообщения пакета
Index = KpaCommand[1];

if (Index >= PacketMkoMsgCount) return REPLY_ITEM_NOT_FOUND;

// удаляем сообщение из списка
for ( ; Index < (PacketMkoMsgCount - 1); Index++) PacketMkoMsgList[Index] = PacketMkoMsgList[Index + 1];

PacketMkoMsgCount--;
return REPLY_CMD_DONE;
}
//================================================================================

// получить количество сообщений в пакете сообщений МКО
int GetPacketMkoMsgCount (void)
{
return PacketMkoMsgCount;
}
//================================================================================

// очистить список пакета сообщений МКО
int ClrPacketMkoMsgList (void)
{
int Index;

PacketMkoMsgCount = 0;
memset(&PacketMkoMsgList, 0, sizeof(PacketMkoMsgList));

PacketMkoIntCount = 0;
memset(&PacketMkoIntList, 0xFF, sizeof(PacketMkoIntList));

PacketMkoIntCntCount = 0;		
memset(&PacketMkoIntCntList, 0xFF, sizeof(PacketMkoIntCntList));

PacketMkoIntQueueCount = 0;
memset(&PacketMkoIntQueue0List, 0xFF, sizeof(PacketMkoIntQueue0List));
memset(&PacketMkoIntQueue1List, 0xFF, sizeof(PacketMkoIntQueue1List));

memset(&PacketMkoMsgResultList, 0xFF, sizeof(PacketMkoMsgResultList));
for (Index = 0; Index < PACKET_MKO_MSG_COUNT; Index++) PacketMkoMsgList[Index].MkoCmd = MKO_CMD(1, 1, MKO_RX, 16);

PacketRdDataInfo[0] = 0; // data size
PacketRdDataInfo[1] = CalcCrc32Init(); // crc init

return REPLY_CMD_DONE;
}
//================================================================================

// выполнить обмен для пакета сообщений МКО
int RunPacketMkoSingleMsg (int BaseMkoLine, int BaseMkoChan)
{
int DevMkoLine, DevMkoChan;
int TotalResult, Result, Index, F4Mode, XchgDone;
int BaseMsgIndex, RemainMsgCount, MsgBlockSize;
WORD MkoCmd, *MkoData;

// сброс результатов обмена
memset(&PacketMkoMsgResultList, 0xFF, sizeof(PacketMkoMsgResultList));

// проверка параметров на допустимость
if (!PacketMkoMsgCount) return REPLY_ITEM_NOT_FOUND;

// проверка сообщений на допустимость
for (Index = 0; Index < PacketMkoMsgCount; Index++)
  {
  MkoCmd  = PacketMkoMsgList[Index].MkoCmd;
  MkoData = (WORD*) &PacketMkoMsgList[Index].Data;
  F4Mode  = PacketMkoMsgList[Index].F4Mode;
  if ((!F4Mode) && (IsProhibitedMkoMsg(MkoCmd, MkoData))) return REPLY_PROHIB_MSG_ERROR;
  }

// параметры линий обмена
DevMkoLine = SpoConfig.DevMkoLine & 1;
DevMkoChan = SpoConfig.DevMkoChan & 1;

// передаем сообщения пакетами по 32 сообщения на пакет
RemainMsgCount = PacketMkoMsgCount;
BaseMsgIndex = 0;
TotalResult = 0;

// отладка
TrackPack = 0x00000000;
TrackPackCount = 0;

while (RemainMsgCount)
  {
  TrackPackCount++;
  MsgBlockSize = (RemainMsgCount > 32) ? (32) : (RemainMsgCount);

  // подготовка пакета сообщений
  PacketMkoMsgPrepareBlock(BaseMsgIndex, MsgBlockSize, DevMkoChan);

  // инициализация описателя пакета МКО
  MkoPackB.nLast = (MsgBlockSize - 1);                  // номер последнего сообщения
  memset(&MkoPackResB, 0, sizeof(MkoPackResB));         // очистка флагов ошибок пакета
  XchgDone = 0;                                         // флаг завершения обмена

  do
    {
    // запуск обмена по МКО
    Result = mkoPackIniStart(DevMkoLine, &MkoPackB, MkoMsgPtrB, 0);
    if (Result != OK) { TrackPack |= 0x00000001; TotalResult |= 0x01; GRES_DBG = TotalResult; /*__asm__("nop;nop;nop;nop;nop;nop;nop;nop;");*/ break; } 
    TrackPack |= 0x00000020;

    // ожидание завершения обмена по МКО
    Result = mkoNumPackWait(DevMkoLine, &MkoPackB);  
    if (Result != OK) { TrackPack |= 0x00000300; TotalResult |= 0x02; GRES_DBG = TotalResult; /*__asm__("nop;nop;nop;nop;nop;nop;nop;nop;");*/ break; } 
    TrackPack |= 0x00004000;
    
    // все обмены для пакета завершены
    XchgDone = 1;
    
    // [проверка результата для пакета]
    Result = PacketMkoMsgCheckPacket(BaseMsgIndex, MsgBlockSize);
    if (Result) TotalResult |= 0x04;

    // [проверка результата для сообщений]
    Result = PacketMkoMsgCheckMessages(BaseMsgIndex, MsgBlockSize);
    if (Result) TotalResult |= 0x08;
    }
  while (0);

  if (TotalResult & 0x03) break;
  if (XchgDone) PacketMkoMsgStoreRxData(BaseMsgIndex, MsgBlockSize);

  RemainMsgCount -= MsgBlockSize;
  BaseMsgIndex += MsgBlockSize;
  }

if (TotalResult) ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE | REPLY_FLAG_ERROR, KPA_SUBADDR_DEBUG, &TotalResult, sizeof(TotalResult)); 

// передача результата обмена
if (!TotalResult) return REPLY_CMD_DONE;
else if (TotalResult & 0x01) return REPLY_ERROR_POINT_1;
else if (TotalResult & 0x02) return REPLY_ERROR_POINT_2;
else if (TotalResult & 0x04) return REPLY_ERROR_POINT_3;
else return REPLY_ERROR_POINT_4;
}
//================================================================================

// подготовка набора сообщений МКО
void PacketMkoMsgPrepareBlock (int BaseIndex, int BlockSize, int DevMkoChan)
{
int PackIndex, ListIndex, F4Mode;
WORD MkoCmd, MkoMode;
DWORD Interval;

for (PackIndex = 0; PackIndex < BlockSize; PackIndex++)
  {
  // индекс сообщения в списке
  ListIndex = BaseIndex + PackIndex;
 
  // подготовка данных для сообщения
  MkoCmd   = PacketMkoMsgList[ListIndex].MkoCmd;
  F4Mode   = PacketMkoMsgList[ListIndex].F4Mode;
  Interval = PacketMkoMsgList[ListIndex].Interval;

  if ((MKO_CMD_DIR(MkoCmd) == MKO_TX) && (!F4Mode))
    {
    memmove(&MkoBufB[PackIndex], &PacketMkoMsgList[ListIndex].Data, 32 * sizeof(WORD));
    }

  MkoMode = (MKO_CMD_DIR(MkoCmd) == MKO_TX) ? (mcTM_BCtoRT) : (mcTM_RTtoBC);
  if (F4Mode) MkoMode = mcTM_ModeNoData;

  // инициализация описателя сообщения
  MkoMsgB[PackIndex].TM   = MkoMode;            // тип сообщения
  MkoMsgB[PackIndex].Chan = DevMkoChan;	        // канал МКО, 0 - основной, 1 - резервный
  MkoMsgB[PackIndex].CW   = MkoCmd;	        // первое командное слово
  MkoMsgB[PackIndex].MissSW   = 0;	        // не останавливать пакет при ошибках
  MkoMsgB[PackIndex].FlagSW   = 0;
  MkoMsgB[PackIndex].FmtError = 0;
  MkoMsgB[PackIndex].GapTime  = Interval;	// пауза до следующего сообщения

  MkoMsgResB[PackIndex].SW = 0;		        // обнуление ответных слов
  // конец инициализации сообщения
  }
}
//================================================================================

// анализ результата обмена для пакета
int PacketMkoMsgCheckPacket (int BaseIndex, int BlockSize)
{
int Status;

// сбрасываем возможную ошибку таймера
MkoPackResB.eOvTmr = 0;
MkoPackResB.Error = MkoPackResB.eFmtError | MkoPackResB.eTrans | MkoPackResB.eOvStack;

Status = 0;

if ((MkoPackResB.MissSW) ||             // нет ответных слов
    (MkoPackResB.FlagSW) ||             // флаги ошибок в ответных словах
    (MkoPackResB.Error))                // ошибки формата, длительности и т.д. при передаче
  {
  Status = 1;
  }

return Status;
}
//================================================================================

// анализ результата обмена для сообщений
int PacketMkoMsgCheckMessages (int BaseIndex, int BlockSize)
{
int Result, PackIndex, ListIndex, F4Mode;
WORD MkoCmd, Reply;

Result = 0;
for (PackIndex = 0; PackIndex < BlockSize; PackIndex++)
  {
  ListIndex = BaseIndex + PackIndex;

  MkoCmd = PacketMkoMsgList[ListIndex].MkoCmd;
  F4Mode = PacketMkoMsgList[ListIndex].F4Mode;
  Reply  = MkoMsgResB[PackIndex].SW;

  PacketMkoMsgResultList[ListIndex].MsgNum    = ListIndex;
  PacketMkoMsgResultList[ListIndex].MkoCmd    = MkoCmd;
  PacketMkoMsgResultList[ListIndex].MkoReply  = Reply;
  PacketMkoMsgResultList[ListIndex].SpoResult = REPLY_CMD_DONE;

  // нет ответного слова от абонента
  if (MkoMsgResB[PackIndex].MissSW)
    {
    PacketMkoMsgResultList[ListIndex].SpoResult = REPLY_MKO_NO_OS_ERROR;
    Result++;
    continue;
    }

  // ошибка формата/длительности обмена
  if (MkoMsgResB[PackIndex].FmtError)
    {
    PacketMkoMsgResultList[ListIndex].SpoResult = REPLY_MKO_LINK_ERROR;
    Result++;
    continue;
    }

  // абонент выставил биты ошибок в ответном слове
  if (MkoMsgResB[PackIndex].FlagSW)
    {
    PacketMkoMsgResultList[ListIndex].SpoResult = REPLY_MKO_OU_ERROR;
    Result++;
    continue;
    }
  }

return Result;
}
//================================================================================

// добавление принятых данных в буфер CRC
void PacketMkoMsgStoreRxData (int BaseIndex, int BlockSize)
{
int PackIndex, ListIndex, DataSize;
WORD MkoCmd;

for (PackIndex = 0; PackIndex < BlockSize; PackIndex++)
  {
  ListIndex = BaseIndex + PackIndex;
  MkoCmd = PacketMkoMsgList[ListIndex].MkoCmd;

  if (!MkoMsgResB[PackIndex].Eom) continue; 	        // обмен по данному сообщению не был завершен
  if (MkoMsgResB[PackIndex].MissSW) continue; 	        // нет ответного слова от абонента
  if (MkoMsgResB[PackIndex].FlagSW) continue; 	        // абонент выставил биты ошибок в ответном слове
  if (MkoMsgResB[PackIndex].FmtError) continue;         // ошибка формата/длительности обмена

  if (MKO_CMD_DIR(MkoCmd) == MKO_RX)
    {
    DataSize = MKO_CMD_SIZE(MkoCmd) * 2;
    PacketRdDataInfo[0] += DataSize;
    PacketRdDataInfo[1] = CalcCrc32Block(PacketRdDataInfo[1], &MkoBufB[PackIndex], DataSize);
    }
  }
}
//================================================================================

// заменить адреса МКО для сообщений из списка
int XchgAddrPacketMkoMsgList (void)
{
unsigned I, Addr, Index1, Index2, Tmp;
WORD Cmd;

Addr   = KpaCommand[1] & 0x1F;
Index1 = KpaCommand[2];
Index2 = KpaCommand[3];

if ((Index1 >= PacketMkoMsgCount) || (Index2 >= PacketMkoMsgCount)) return REPLY_CMD_DATA_ERROR;
if (Index1 > Index2) { Tmp = Index1; Index1 = Index2; Index2 = Tmp; }

for (I = Index1; I <= Index2; I++)
  {
  Cmd = PacketMkoMsgList[I].MkoCmd;
  Cmd = MKO_CMD(Addr, MKO_CMD_SUBADDR(Cmd), MKO_CMD_DIR(Cmd), MKO_CMD_SIZE(Cmd));
  PacketMkoMsgList[I].MkoCmd = Cmd;
  }

return REPLY_CMD_DONE;
}
//================================================================================

// заменить подадреса МКО для сообщений из списка
int XchgSubAddrPacketMkoMsgList (void)
{
unsigned I, SubAddr, Index1, Index2, Tmp;
WORD Cmd;

SubAddr = KpaCommand[1] & 0x1F;
Index1  = KpaCommand[2];
Index2  = KpaCommand[3];

if ((Index1 >= PacketMkoMsgCount) || (Index2 >= PacketMkoMsgCount)) return REPLY_CMD_DATA_ERROR;
if (Index1 > Index2) { Tmp = Index1; Index1 = Index2; Index2 = Tmp; }

for (I = Index1; I <= Index2; I++)
  {
  Cmd = PacketMkoMsgList[I].MkoCmd;
  Cmd = MKO_CMD(MKO_CMD_ADDR(Cmd), SubAddr, MKO_CMD_DIR(Cmd), MKO_CMD_SIZE(Cmd));
  PacketMkoMsgList[I].MkoCmd = Cmd;
  }

return REPLY_CMD_DONE;
}
//================================================================================

void RunPacketMkoSingleMsgResult (int BaseMkoLine, int BaseMkoChan, int Result)
{
int Index, SrcIndex, DstIndex;
WORD Buffer[8];
memset(&Buffer, 0, sizeof(Buffer));

/*
[0] = общий результат обмена
[1] = общее число запланированных сообщений в списке
[2] = число сообщений, передача по которым была завершена
[3] = число сообщений, передача по которым была завершена успешно
[4] = число сообщений к кодом ошибки "нет ОС" 
[5] = число сообщений к кодом ошибки "сбой на линии" 
[6] = число сообщений к кодом ошибки "признаки ошибок в ОС" 
*/

Buffer[0] = Result;
Buffer[1] = PacketMkoMsgCount;

for (Index = 0; Index < PacketMkoMsgCount; Index++)
  {
  if (PacketMkoMsgResultList[Index].MsgNum != 0xFFFF) { Buffer[2]++; } else { continue; }
  if (PacketMkoMsgResultList[Index].SpoResult == REPLY_CMD_DONE) Buffer[3]++;
  if (PacketMkoMsgResultList[Index].SpoResult == REPLY_MKO_NO_OS_ERROR) Buffer[4]++;
  if (PacketMkoMsgResultList[Index].SpoResult == REPLY_MKO_LINK_ERROR) Buffer[5]++;
  if (PacketMkoMsgResultList[Index].SpoResult == REPLY_MKO_OU_ERROR) Buffer[6]++;
  }

if (Result == REPLY_CMD_DONE)
  {
  ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ, &Buffer, 8 * sizeof(WORD)); 
  }
else
  {
  memmove(&PacketMkoMsgResultBuf[0], &Buffer, 8 * sizeof(WORD));

  for (SrcIndex = 0, DstIndex = 2; SrcIndex < PacketMkoMsgCount; SrcIndex++)
    {
    if (PacketMkoMsgResultList[SrcIndex].MsgNum == 0xFFFF) { continue; }
    if (PacketMkoMsgResultList[SrcIndex].SpoResult == REPLY_CMD_DONE) { continue; }
    memmove(&PacketMkoMsgResultBuf[DstIndex++], &PacketMkoMsgResultList[SrcIndex], sizeof(TPacketMkoMsgResult));
    }

  ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ, &PacketMkoMsgResultBuf, DstIndex * sizeof(TPacketMkoMsgResult)); 
  }

/*
void Output (unsigned nMKO, unsigned ADR, unsigned pADR, unsigned nCD, unsigned short *INF)     
TPacketMkoMsg PacketMkoMsgList[PACKET_MKO_MSG_COUNT];                           // список описателей сообщений МКО
TPacketMkoMsgResult PacketMkoMsgResultList[PACKET_MKO_MSG_COUNT];               // результат обмена для сообщений МКО, если MsgNum = 0xFFFF - обмен не проводился

WORD MsgNum;            // номер сообщения в списке (0..n)
WORD MkoCmd;            // командное слово МКО
WORD MkoReply;          // ответное слово МКО
WORD SpoResult;         // код результата СПО

  if (Result == REPLY_CMD_DONE)
    {
    ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, Result, KPA_SUBADDR_IRQ);
    }
  else 
    {
    Size = PacketMkoMsgResultCount * sizeof(TPacketMkoMsgResult);
    if (!Size) ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, Result, KPA_SUBADDR_IRQ);
    else ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, Result, KPA_SUBADDR_IRQ, &PacketMkoMsgResultList, Size);
    }
*/
}
//================================================================================

int RunPacketMkoMultiMsg (int BaseMkoLine, int BaseMkoChan)
{
return RunPacketMkoSingleMsg(BaseMkoLine, BaseMkoChan);
}
//================================================================================

void RunPacketMkoMultiMsgResult (int BaseMkoLine, int BaseMkoChan, int Result)
{
int Index;
WORD Buffer[8];
memset(&Buffer, 0, sizeof(Buffer));

/*
[0] = общий результат обмена
[1] = общее число запланированных сообщений в списке
[2] = число сообщений, передача по которым была завершена
[3] = число сообщений, передача по которым была завершена успешно
[4] = число сообщений к кодом ошибки "нет ОС" 
[5] = число сообщений к кодом ошибки "сбой на линии" 
[6] = число сообщений к кодом ошибки "признаки ошибок в ОС" 
*/

Buffer[0] = Result;
Buffer[1] = PacketMkoMsgCount;

for (Index = 0; Index < PacketMkoMsgCount; Index++)
  {
  if (PacketMkoMsgResultList[Index].MsgNum != 0xFFFF) { Buffer[2]++; } else { continue; }
  if (PacketMkoMsgResultList[Index].SpoResult == REPLY_CMD_DONE) Buffer[3]++;
  if (PacketMkoMsgResultList[Index].SpoResult == REPLY_MKO_NO_OS_ERROR) Buffer[4]++;
  if (PacketMkoMsgResultList[Index].SpoResult == REPLY_MKO_LINK_ERROR) Buffer[5]++;
  if (PacketMkoMsgResultList[Index].SpoResult == REPLY_MKO_OU_ERROR) Buffer[6]++;
  }

ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ, &Buffer, 8 * sizeof(WORD)); 
}
//================================================================================

// код операции = 0x20, добавление интервала в список интервалов
int AddPacketMko_SecPiPo_IntToList (int ABaseMkoLine, int ABaseMkoChan)
{
int Interval;
Interval = KpaCommand[1];

// ошибка если число интервалов равно максимальному
if (PacketMkoIntCount >= PACKET_MKO_INT_COUNT) return REPLY_CMD_DATA_ERROR;

// пределы для интервала = 1...3600 сек
if ((Interval < 1) || (Interval > 3600)) return REPLY_CMD_DATA_ERROR;

// если интервал не начальный - удлинняем время на указанную величину
if (PacketMkoIntCount) Interval += PacketMkoIntList[PacketMkoIntCount - 1].Interval;
PacketMkoIntList[PacketMkoIntCount++].Interval = Interval;

return REPLY_CMD_DONE;
}
//================================================================================

// код операции = 0x21, добавление указанного сообщения к указанному интервалу
int AddPacketMko_SecPiPo_MsgToInt (int ABaseMkoLine, int ABaseMkoChan)
{
int IntIndex, MsgIndex, Index, State;

IntIndex = KpaCommand[1];
MsgIndex = KpaCommand[2];

// ошибка если индекс интервала больше макс возможного
if (IntIndex >= PacketMkoIntCount) return REPLY_CMD_DATA_ERROR;

// ошибка если индекс сообщения больше макс возможного
if (MsgIndex >= PacketMkoMsgCount) return REPLY_CMD_DATA_ERROR;

for (Index = 0, State = 0; Index < 32; Index++)
  {
  if (PacketMkoIntList[IntIndex].MsgList[Index] == -1) 
    { 
    PacketMkoIntList[IntIndex].MsgList[Index] = MsgIndex; 
    State = 1; break; 
    }  
  }

// если данное сообщение не было добавлено к интервалу (список номеров сообщений уже заполнен)
if (!State) return REPLY_CMD_DATA_ERROR;

return REPLY_CMD_DONE;
}
//================================================================================

// код операции = 0x22, запуск прогона сообщений по интервалам
int RunPacketMko_SecPiPo_IntList (int ABaseMkoLine, int ABaseMkoChan)
{
int Status, Result, DeltaTime;
int DevMkoLine, DevMkoChan;
int State1, State2;

// проверяем списки интервалов и сообщений
Result = RunPacketMko_SecPiPo_CheckList();
if (Result) return REPLY_CMD_DATA_ERROR;

// параметры линий обмена
DevMkoLine = SpoConfig.DevMkoLine & 1;
DevMkoChan = SpoConfig.DevMkoChan & 1;

State1 = STATE_IDLE;
State2 = STATE_IDLE;

timeRead(&RunPacketBeginTime);
                            
while (1)
  {
  if (State1 == STATE_WORK)
    {
    // ожидание завершения пакета 1
    Result = mkoNumPackWait(DevMkoLine, &MkoPackB);
    if (Result != OK) { Status = REPLY_ERROR_POINT_2; break; }
    // пакет 1 завершен
    State1 = STATE_IDLE;     
    }

 if (State1 == STATE_IDLE)
    {
    // подготовка пакета 1
    timeRead(&RunPacketCurrTime);
    DeltaTime = DeltaTimeMSec(&RunPacketBeginTime, &RunPacketCurrTime);
    Result = RunPacketMko_SecPiPo_PreparePacket1(DeltaTime, DevMkoChan);
    if (!Result)
      {
      // отправка пакета 1
      Result = mkoPackIniStart(DevMkoLine, &MkoPackB, MkoMsgPtrB, 0);
      if (Result != OK) { Status = REPLY_ERROR_POINT_1; break; }
      // пакет 1 передается 
      State1 = STATE_WORK;
      }
    }

  if (State2 == STATE_WORK)
    {
    // ожидание завершения пакета 2
    Result = mkoNumPackWait(DevMkoLine, &MkoPackC);
    if (Result != OK) { Status = REPLY_ERROR_POINT_4; break; }
    // пакет 2 завершен
    State2 = STATE_IDLE; 
    }

  if (State2 == STATE_IDLE)
    {
    // подготовка пакета 2
    timeRead(&RunPacketCurrTime);
    DeltaTime = DeltaTimeMSec(&RunPacketBeginTime, &RunPacketCurrTime);
    Result = RunPacketMko_SecPiPo_PreparePacket2(DeltaTime, DevMkoChan);
    if (!Result)
      {
      // отправка пакета 2
      Result = mkoPackIniStart(DevMkoLine, &MkoPackC, MkoMsgPtrC, 0);
      if (Result != OK) { Status = REPLY_ERROR_POINT_3; break; }
      // пакет 2 передается
      State2 = STATE_WORK;
      }
    }

 if ((State1 == STATE_IDLE) && (State2 == STATE_IDLE))
    {
    // передача завершена
    Status = REPLY_CMD_DONE; 
    break;
    }
  }

return Status;
}
//================================================================================

int RunPacketMko_SecPiPo_CheckList (void)
{
int IntIndex;

// ошибка если список интервалов пустой
if (!PacketMkoIntCount) return 1;

for (IntIndex = 0; IntIndex < PacketMkoIntCount; IntIndex++)
  {
  // ошибка если список сообщений в интервале пустой
  if (PacketMkoIntList[IntIndex].MsgList[0] == -1) return 1;
  }

// список интервалов в норме
return 0;
}
//================================================================================

int RunPacketMko_SecPiPo_PreparePacket1 (int ADeltaTime, int AMkoChan)
{
int IntIndex, MsgIndex, Int, Count, Pass, MsgCount;
int Index, Mode, MsgListIndex;
TPacketMkoMsg *Msg;

ADeltaTime /= 1000;

// поиск временного интервала
for (IntIndex = 0, Int = -1; IntIndex < PacketMkoIntCount; IntIndex++)
  {
  if (ADeltaTime < PacketMkoIntList[IntIndex].Interval) { Int = IntIndex; break; }
  }

// указанное время лежит вне объявленных интервалов
if (Int == -1) return 1;

// расчет числа сообщений в интервале
for (MsgIndex = 0, Count = 0; MsgIndex < 32; MsgIndex++)
  {
  if (PacketMkoIntList[Int].MsgList[MsgIndex] == -1) break;
  Count++;
  }  

// в данном интервале нет сообщений
if (Count == 0) return 1;

// общее число сообщений собираемое в пакет
Pass = 32 / Count;
MsgCount = Pass * Count;

// настройка пакета сообщений
MkoPackB.nLast = (MsgCount - 1); // индекс последнего сообщения
memset(&MkoPackResB, 0, sizeof(mkoPackRes_t)); // сброс флагов для пакета

// инициализация описателей сообщений в пакете
for (Index = 0, MsgIndex = 0; Index < MsgCount; Index++)
  {
  // получаем сообщение из списка по индексу из интервала
  MsgListIndex = PacketMkoIntList[Int].MsgList[MsgIndex];
  Msg = &PacketMkoMsgList[MsgListIndex];
  if (++MsgIndex >= Count) MsgIndex = 0;

  // режим передачи (F1/F2/F4)
  Mode = (MKO_CMD_DIR(Msg->MkoCmd) == MKO_TX) ? (mcTM_BCtoRT) : (mcTM_RTtoBC);
  if (Msg->F4Mode) Mode = mcTM_ModeNoData;

  MkoMsgB[Index].TM   = Mode;        	// тип сообщения, RX/TX
  MkoMsgB[Index].Chan = AMkoChan;    	// канал МКО, 0 - основной, 1 - резервный
  MkoMsgB[Index].CW   = Msg->MkoCmd;    // командное слово
  MkoMsgB[Index].MissSW   = 0;       	// не прерывать передачу при ошибках
  MkoMsgB[Index].FlagSW   = 0;
  MkoMsgB[Index].FmtError = 0;
  MkoMsgB[Index].GapTime  = Msg->Interval; 

  if (Mode == mcTM_BCtoRT) memmove(&MkoBufB[Index], &Msg->Data, 32 * sizeof(WORD)); // данные для передачи

  memset(&MkoMsgResB[Index], 0, sizeof(mkoMsgRes_t)); // сброс флагов для сообщения
  }

return 0;
}
//================================================================================

int RunPacketMko_SecPiPo_PreparePacket2 (int ADeltaTime, int AMkoChan)
{
int IntIndex, MsgIndex, Int, Count, Pass, MsgCount;
int Index, Mode, MsgListIndex;
TPacketMkoMsg *Msg;

ADeltaTime /= 1000;

// поиск временного интервала
for (IntIndex = 0, Int = -1; IntIndex < PacketMkoIntCount; IntIndex++)
  {
  if (ADeltaTime < PacketMkoIntList[IntIndex].Interval) { Int = IntIndex; break; }
  }

// указанное время лежит вне объявленных интервалов
if (Int == -1) return 1;

// расчет числа сообщений в интервале
for (MsgIndex = 0, Count = 0; MsgIndex < 32; MsgIndex++)
  {
  if (PacketMkoIntList[Int].MsgList[MsgIndex] == -1) break;
  Count++;
  }  

// в данном интервале нет сообщений
if (Count == 0) return 1;

// общее число сообщений собираемое в пакет
Pass = 32 / Count;
MsgCount = Pass * Count;

// настройка пакета сообщений
MkoPackC.nLast = (MsgCount - 1); // индекс последнего сообщения
memset(&MkoPackResC, 0, sizeof(mkoPackRes_t)); // сброс флагов для пакета

// инициализация описателей сообщений в пакете
for (Index = 0, MsgIndex = 0; Index < MsgCount; Index++)
  {
  // получаем сообщение из списка по индексу из интервала
  MsgListIndex = PacketMkoIntList[Int].MsgList[MsgIndex];
  Msg = &PacketMkoMsgList[MsgListIndex];
  if (++MsgIndex >= Count) MsgIndex = 0;

  // режим передачи (F1/F2/F4)
  Mode = (MKO_CMD_DIR(Msg->MkoCmd) == MKO_TX) ? (mcTM_BCtoRT) : (mcTM_RTtoBC);
  if (Msg->F4Mode) Mode = mcTM_ModeNoData;

  MkoMsgC[Index].TM   = Mode;        	// тип сообщения, RX/TX
  MkoMsgC[Index].Chan = AMkoChan;    	// канал МКО, 0 - основной, 1 - резервный
  MkoMsgC[Index].CW   = Msg->MkoCmd;    // командное слово
  MkoMsgC[Index].MissSW   = 0;       	// не прерывать передачу при ошибках
  MkoMsgC[Index].FlagSW   = 0;
  MkoMsgC[Index].FmtError = 0;
  MkoMsgC[Index].GapTime  = Msg->Interval; 

  if (Mode == mcTM_BCtoRT) memmove(&MkoBufC[Index], &Msg->Data, 32 * sizeof(WORD)); // данные для передачи

  memset(&MkoMsgResC[Index], 0, sizeof(mkoMsgRes_t)); // сброс флагов для сообщения
  }

return 0;
}
//================================================================================

int AddPacketMko_CntPiPo_IntToList (int ABaseMkoLine, int ABaseMkoChan)
{
int Count;
Count = KpaCommand[1];

// ошибка если число интервалов равно максимальному
if (PacketMkoIntCntCount >= PACKET_MKO_INT_COUNT) return REPLY_CMD_DATA_ERROR;

// пределы для счетчика = 1...10000 шт
if ((Count < 1) || (Count > 10000)) return REPLY_CMD_DATA_ERROR;

// если интервал не начальный - удлинняем счетчик на указанную величину
if (PacketMkoIntCntCount) Count += PacketMkoIntCntList[PacketMkoIntCntCount - 1].Count;
PacketMkoIntCntList[PacketMkoIntCntCount++].Count = Count;

return REPLY_CMD_DONE;
}
//================================================================================

int AddPacketMko_CntPiPo_MsgToInt (int ABaseMkoLine, int ABaseMkoChan)
{
int IntIndex, MsgIndex, Index, State;

IntIndex = KpaCommand[1];
MsgIndex = KpaCommand[2];

// ошибка если индекс интервала больше макс возможного
if (IntIndex >= PacketMkoIntCntCount) return REPLY_CMD_DATA_ERROR;

// ошибка если индекс сообщения больше макс возможного
if (MsgIndex >= PacketMkoMsgCount) return REPLY_CMD_DATA_ERROR;

for (Index = 0, State = 0; Index < 32; Index++)
  {
  if (PacketMkoIntCntList[IntIndex].MsgList[Index] == -1) 
    { 
    PacketMkoIntCntList[IntIndex].MsgList[Index] = MsgIndex; 
    State = 1; break; 
    }  
  }

// если данное сообщение не было добавлено к интервалу (список номеров сообщений уже заполнен)
if (!State) return REPLY_CMD_DATA_ERROR;

return REPLY_CMD_DONE;
}
//================================================================================

int RunPacketMko_CntPiPo_IntList (int ABaseMkoLine, int ABaseMkoChan)
{
int Status, Result, TotalCount;
int DevMkoLine, DevMkoChan;
int State1, State2;

// проверяем списки интервалов и сообщений
Result = RunPacketMko_CntPiPo_CheckList();
if (Result) return REPLY_CMD_DATA_ERROR;

// параметры линий обмена
DevMkoLine = SpoConfig.DevMkoLine & 1;
DevMkoChan = SpoConfig.DevMkoChan & 1;

State1 = STATE_IDLE;
State2 = STATE_IDLE;

TotalCount = 0;
                            
while (1)
  {
  if (State1 == STATE_WORK)
    {
    // ожидание завершения пакета 1
    Result = mkoNumPackWait(DevMkoLine, &MkoPackB);
    if (Result != OK) { Status = REPLY_ERROR_POINT_2; break; }
    // пакет 1 завершен
    State1 = STATE_IDLE;     
    }

 if (State1 == STATE_IDLE)
    {
    // подготовка пакета 1
    Result = RunPacketMko_CntPiPo_PreparePacket1(TotalCount, DevMkoChan);
    if (!Result)
      {
      // отправка пакета 1
      Result = mkoPackIniStart(DevMkoLine, &MkoPackB, MkoMsgPtrB, 0);
      if (Result != OK) { Status = REPLY_ERROR_POINT_1; break; }
      // пакет 1 передается 
      State1 = STATE_WORK;
      TotalCount++;
      }
    }

  if (State2 == STATE_WORK)
    {
    // ожидание завершения пакета 2
    Result = mkoNumPackWait(DevMkoLine, &MkoPackC);
    if (Result != OK) { Status = REPLY_ERROR_POINT_4; break; }
    // пакет 2 завершен
    State2 = STATE_IDLE; 
    }

  if (State2 == STATE_IDLE)
    {
    // подготовка пакета 2
    Result = RunPacketMko_CntPiPo_PreparePacket2(TotalCount, DevMkoChan);
    if (!Result)
      {
      // отправка пакета 2
      Result = mkoPackIniStart(DevMkoLine, &MkoPackC, MkoMsgPtrC, 0);
      if (Result != OK) { Status = REPLY_ERROR_POINT_3; break; }
      // пакет 2 передается
      State2 = STATE_WORK;
      TotalCount++;
      }
    }

 if ((State1 == STATE_IDLE) && (State2 == STATE_IDLE))
    {
    // передача завершена
    Status = REPLY_CMD_DONE; 
    break;
    }
  }

return Status;
}
//================================================================================

int RunPacketMko_CntPiPo_CheckList (void)
{
int IntIndex;

// ошибка если список интервалов пустой
if (!PacketMkoIntCntCount) return 1;

for (IntIndex = 0; IntIndex < PacketMkoIntCntCount; IntIndex++)
  {
  // ошибка если список сообщений в интервале пустой
  if (PacketMkoIntCntList[IntIndex].MsgList[0] == -1) return 1;
  }

// список интервалов в норме
return 0;
}
//================================================================================

int RunPacketMko_CntPiPo_PreparePacket1 (int ATotalCount, int AMkoChan)
{
int IntIndex, MsgIndex, Int, Count, Pass, MsgCount;
int Index, Mode, MsgListIndex;
TPacketMkoMsg *Msg;

// поиск временного интервала
for (IntIndex = 0, Int = -1; IntIndex < PacketMkoIntCntCount; IntIndex++)
  {
  if (ATotalCount < PacketMkoIntCntList[IntIndex].Count) { Int = IntIndex; break; }
  }

// указанное время лежит вне объявленных интервалов
if (Int == -1) return 1;

// расчет числа сообщений в интервале
for (MsgIndex = 0, Count = 0; MsgIndex < 32; MsgIndex++)
  {
  if (PacketMkoIntCntList[Int].MsgList[MsgIndex] == -1) break;
  Count++;
  }  

// в данном интервале нет сообщений
if (Count == 0) return 1;

// общее число сообщений собираемое в пакет
Pass = 32 / Count;
MsgCount = Pass * Count;

// настройка пакета сообщений
MkoPackB.nLast = (MsgCount - 1); // индекс последнего сообщения
memset(&MkoPackResB, 0, sizeof(mkoPackRes_t)); // сброс флагов для пакета

// инициализация описателей сообщений в пакете
for (Index = 0, MsgIndex = 0; Index < MsgCount; Index++)
  {
  // получаем сообщение из списка по индексу из интервала
  MsgListIndex = PacketMkoIntCntList[Int].MsgList[MsgIndex];
  Msg = &PacketMkoMsgList[MsgListIndex];
  if (++MsgIndex >= Count) MsgIndex = 0;

  // режим передачи (F1/F2/F4)
  Mode = (MKO_CMD_DIR(Msg->MkoCmd) == MKO_TX) ? (mcTM_BCtoRT) : (mcTM_RTtoBC);
  if (Msg->F4Mode) Mode = mcTM_ModeNoData;

  MkoMsgB[Index].TM   = Mode;        	// тип сообщения, RX/TX
  MkoMsgB[Index].Chan = AMkoChan;    	// канал МКО, 0 - основной, 1 - резервный
  MkoMsgB[Index].CW   = Msg->MkoCmd;    // командное слово
  MkoMsgB[Index].MissSW   = 0;       	// не прерывать передачу при ошибках
  MkoMsgB[Index].FlagSW   = 0;
  MkoMsgB[Index].FmtError = 0;
  MkoMsgB[Index].GapTime  = Msg->Interval; 

  if (Mode == mcTM_BCtoRT) memmove(&MkoBufB[Index], &Msg->Data, 32 * sizeof(WORD)); // данные для передачи

  memset(&MkoMsgResB[Index], 0, sizeof(mkoMsgRes_t)); // сброс флагов для сообщения
  }

return 0;
}
//================================================================================

int RunPacketMko_CntPiPo_PreparePacket2 (int ATotalCount, int AMkoChan)
{
int IntIndex, MsgIndex, Int, Count, Pass, MsgCount;
int Index, Mode, MsgListIndex;
TPacketMkoMsg *Msg;

// поиск временного интервала
for (IntIndex = 0, Int = -1; IntIndex < PacketMkoIntCntCount; IntIndex++)
  {
  if (ATotalCount < PacketMkoIntCntList[IntIndex].Count) { Int = IntIndex; break; }
  }

// указанное время лежит вне объявленных интервалов
if (Int == -1) return 1;

// расчет числа сообщений в интервале
for (MsgIndex = 0, Count = 0; MsgIndex < 32; MsgIndex++)
  {
  if (PacketMkoIntCntList[Int].MsgList[MsgIndex] == -1) break;
  Count++;
  }  

// в данном интервале нет сообщений
if (Count == 0) return 1;

// общее число сообщений собираемое в пакет
Pass = 32 / Count;
MsgCount = Pass * Count;

// настройка пакета сообщений
MkoPackC.nLast = (MsgCount - 1); // индекс последнего сообщения
memset(&MkoPackResC, 0, sizeof(mkoPackRes_t)); // сброс флагов для пакета

// инициализация описателей сообщений в пакете
for (Index = 0, MsgIndex = 0; Index < MsgCount; Index++)
  {
  // получаем сообщение из списка по индексу из интервала
  MsgListIndex = PacketMkoIntCntList[Int].MsgList[MsgIndex];
  Msg = &PacketMkoMsgList[MsgListIndex];
  if (++MsgIndex >= Count) MsgIndex = 0;

  // режим передачи (F1/F2/F4)
  Mode = (MKO_CMD_DIR(Msg->MkoCmd) == MKO_TX) ? (mcTM_BCtoRT) : (mcTM_RTtoBC);
  if (Msg->F4Mode) Mode = mcTM_ModeNoData;

  MkoMsgC[Index].TM   = Mode;        	// тип сообщения, RX/TX
  MkoMsgC[Index].Chan = AMkoChan;    	// канал МКО, 0 - основной, 1 - резервный
  MkoMsgC[Index].CW   = Msg->MkoCmd;    // командное слово
  MkoMsgC[Index].MissSW   = 0;       	// не прерывать передачу при ошибках
  MkoMsgC[Index].FlagSW   = 0;
  MkoMsgC[Index].FmtError = 0;
  MkoMsgC[Index].GapTime  = Msg->Interval; 

  if (Mode == mcTM_BCtoRT) memmove(&MkoBufC[Index], &Msg->Data, 32 * sizeof(WORD)); // данные для передачи

  memset(&MkoMsgResC[Index], 0, sizeof(mkoMsgRes_t)); // сброс флагов для сообщения
  }

return 0;
}
//================================================================================

int AddPacketMko_SecQue_IntToList (int ABaseMkoLine, int ABaseMkoChan)
{
int Interval;
Interval = KpaCommand[1];

// ошибка если число интервалов равно максимальному
if (PacketMkoIntQueueCount >= PACKET_MKO_INT_COUNT) return REPLY_CMD_DATA_ERROR;

// пределы для интервала = 1...3600 сек
if ((Interval < 1) || (Interval > 3600)) return REPLY_CMD_DATA_ERROR;

// если интервал не начальный - удлинняем время на указанную величину
if (PacketMkoIntQueueCount) Interval += PacketMkoIntQueue0List[PacketMkoIntQueueCount - 1].Interval;
PacketMkoIntQueue0List[PacketMkoIntQueueCount].Interval = Interval;
PacketMkoIntQueue1List[PacketMkoIntQueueCount].Interval = Interval;
PacketMkoIntQueueCount++;

return REPLY_CMD_DONE;
}
//================================================================================

int AddPacketMko_SecQue_MsgToInt (int ABaseMkoLine, int ABaseMkoChan)
{
int IntIndex, MsgIndex, MkoLine, Index, State;

IntIndex = KpaCommand[1];
MsgIndex = KpaCommand[2];
MkoLine  = KpaCommand[3];

// ошибка если номер МКО вне допустимы
if (MkoLine > 1) return REPLY_CMD_DATA_ERROR;

// ошибка если индекс интервала больше макс возможного
if (IntIndex >= PacketMkoIntQueueCount) return REPLY_CMD_DATA_ERROR;

// ошибка если индекс сообщения больше макс возможного
if (MsgIndex >= PacketMkoMsgCount) return REPLY_CMD_DATA_ERROR;

for (Index = 0, State = 0; Index < 32; Index++)
  {
  if (MkoLine == 0)
    {
    if (PacketMkoIntQueue0List[IntIndex].MsgList[Index] == -1) 
      { 
      PacketMkoIntQueue0List[IntIndex].MsgList[Index] = MsgIndex; 
      State = 1; break; 
      }  
    }
  else
    {
    if (PacketMkoIntQueue1List[IntIndex].MsgList[Index] == -1) 
      { 
      PacketMkoIntQueue1List[IntIndex].MsgList[Index] = MsgIndex; 
      State = 1; break; 
      }  
    }
  }

// если данное сообщение не было добавлено к интервалу (список номеров сообщений уже заполнен)
if (!State) return REPLY_CMD_DATA_ERROR;

return REPLY_CMD_DONE;
}
//================================================================================

int RunPacketMko_SecQue_IntList (int ABaseMkoLine, int ABaseMkoChan)
{
int Status, MkoFirst, Result, Result1, Result2, DeltaTime;
int DevMkoLine, DevMkoChan;

MkoFirst = KpaCommand[1];

// ошибка если номер МКО вне допустимы
if (MkoFirst > 1) return REPLY_CMD_DATA_ERROR;

// проверяем списки интервалов и сообщений
Result = RunPacketMko_SecQue_CheckList();
if (Result) return REPLY_CMD_DATA_ERROR;

// параметры линий обмена
DevMkoLine = SpoConfig.DevMkoLine & 1;
DevMkoChan = SpoConfig.DevMkoChan & 1;

timeRead(&RunPacketBeginTime);
                            
while (1)
  {
  // подготовка пакетов
  timeRead(&RunPacketCurrTime);
  DeltaTime = DeltaTimeMSec(&RunPacketBeginTime, &RunPacketCurrTime);

  Result1 = RunPacketMko_SecQue_PreparePacket1(DeltaTime, DevMkoChan);
  Result2 = RunPacketMko_SecQue_PreparePacket2(DeltaTime, DevMkoChan);

  if ((!Result1) && (!Result2))
    {
    if (MkoFirst == 0)
      {
      // отправка пакета по МКО-0
      Result = mkoPackIniStart(0, &MkoPackB, MkoMsgPtrB, 0);
      if (Result != OK) { Status = REPLY_ERROR_POINT_1; break; }

      // отправка пакета по МКО-1
      Result = mkoPackIniStart(1, &MkoPackC, MkoMsgPtrC, 0);
      if (Result != OK) { Status = REPLY_ERROR_POINT_3; break; }

      // ожидание завершения пакета по МКО-0
      Result = mkoNumPackWait(0, &MkoPackB);
      if (Result != OK) { Status = REPLY_ERROR_POINT_2; break; }
  
      // ожидание завершения пакета по МКО-1
      Result = mkoNumPackWait(1, &MkoPackC);
      if (Result != OK) { Status = REPLY_ERROR_POINT_4; break; }
      } 
    else
      {
      // отправка пакета по МКО-1
      Result = mkoPackIniStart(1, &MkoPackC, MkoMsgPtrC, 0);
      if (Result != OK) { Status = REPLY_ERROR_POINT_3; break; }

      // отправка пакета по МКО-0
      Result = mkoPackIniStart(0, &MkoPackB, MkoMsgPtrB, 0);
      if (Result != OK) { Status = REPLY_ERROR_POINT_1; break; }

      // ожидание завершения пакета по МКО-1
      Result = mkoNumPackWait(1, &MkoPackC);
      if (Result != OK) { Status = REPLY_ERROR_POINT_4; break; }

      // ожидание завершения пакета по МКО-0
      Result = mkoNumPackWait(0, &MkoPackB);
      if (Result != OK) { Status = REPLY_ERROR_POINT_2; break; }
      }
    }
  else
    {
    // передача завершена
    Status = REPLY_CMD_DONE; 
    break;
    }
  }

return Status;
}
//================================================================================

int RunPacketMko_SecQue_CheckList (void)
{
int IntIndex;

// ошибка если список интервалов пустой
if (!PacketMkoIntQueueCount) return 1;

for (IntIndex = 0; IntIndex < PacketMkoIntQueueCount; IntIndex++)
  {
  // ошибка если список сообщений в интервале пустой
  if (PacketMkoIntQueue0List[IntIndex].MsgList[0] == -1) return 1;
  if (PacketMkoIntQueue1List[IntIndex].MsgList[0] == -1) return 1;
  }

// список интервалов в норме
return 0;
}
//================================================================================

int RunPacketMko_SecQue_PreparePacket1 (int ADeltaTime, int AMkoChan)
{
int IntIndex, MsgIndex, Int, Count, Pass, MsgCount;
int Index, Mode, MsgListIndex;
TPacketMkoMsg *Msg;

ADeltaTime /= 1000;

// поиск временного интервала
for (IntIndex = 0, Int = -1; IntIndex < PacketMkoIntQueueCount; IntIndex++)
  {
  if (ADeltaTime < PacketMkoIntQueue0List[IntIndex].Interval) { Int = IntIndex; break; }
  }

// указанное время лежит вне объявленных интервалов
if (Int == -1) return 1;

// расчет числа сообщений в интервале
for (MsgIndex = 0, Count = 0; MsgIndex < 32; MsgIndex++)
  {
  if (PacketMkoIntQueue0List[Int].MsgList[MsgIndex] == -1) break;
  Count++;
  }  

// в данном интервале нет сообщений
if (Count == 0) return 1;

// общее число сообщений собираемое в пакет
Pass = 32 / Count;
MsgCount = Pass * Count;

// настройка пакета сообщений
MkoPackB.nLast = (MsgCount - 1); // индекс последнего сообщения
memset(&MkoPackResB, 0, sizeof(mkoPackRes_t)); // сброс флагов для пакета

// инициализация описателей сообщений в пакете
for (Index = 0, MsgIndex = 0; Index < MsgCount; Index++)
  {
  // получаем сообщение из списка по индексу из интервала
  MsgListIndex = PacketMkoIntQueue0List[Int].MsgList[MsgIndex];
  Msg = &PacketMkoMsgList[MsgListIndex];
  if (++MsgIndex >= Count) MsgIndex = 0;

  // режим передачи (F1/F2/F4)
  Mode = (MKO_CMD_DIR(Msg->MkoCmd) == MKO_TX) ? (mcTM_BCtoRT) : (mcTM_RTtoBC);
  if (Msg->F4Mode) Mode = mcTM_ModeNoData;

  MkoMsgB[Index].TM   = Mode;        	// тип сообщения, RX/TX
  MkoMsgB[Index].Chan = AMkoChan;    	// канал МКО, 0 - основной, 1 - резервный
  MkoMsgB[Index].CW   = Msg->MkoCmd;    // командное слово
  MkoMsgB[Index].MissSW   = 0;       	// не прерывать передачу при ошибках
  MkoMsgB[Index].FlagSW   = 0;
  MkoMsgB[Index].FmtError = 0;
  MkoMsgB[Index].GapTime  = Msg->Interval; 

  if (Mode == mcTM_BCtoRT) memmove(&MkoBufB[Index], &Msg->Data, 32 * sizeof(WORD)); // данные для передачи

  memset(&MkoMsgResB[Index], 0, sizeof(mkoMsgRes_t)); // сброс флагов для сообщения
  }

return 0;
}
//================================================================================

int RunPacketMko_SecQue_PreparePacket2 (int ADeltaTime, int AMkoChan)
{
int IntIndex, MsgIndex, Int, Count, Pass, MsgCount;
int Index, Mode, MsgListIndex;
TPacketMkoMsg *Msg;

ADeltaTime /= 1000;

// поиск временного интервала
for (IntIndex = 0, Int = -1; IntIndex < PacketMkoIntQueueCount; IntIndex++)
  {
  if (ADeltaTime < PacketMkoIntQueue1List[IntIndex].Interval) { Int = IntIndex; break; }
  }

// указанное время лежит вне объявленных интервалов
if (Int == -1) return 1;

// расчет числа сообщений в интервале
for (MsgIndex = 0, Count = 0; MsgIndex < 32; MsgIndex++)
  {
  if (PacketMkoIntQueue1List[Int].MsgList[MsgIndex] == -1) break;
  Count++;
  }  

// в данном интервале нет сообщений
if (Count == 0) return 1;

// общее число сообщений собираемое в пакет
Pass = 32 / Count;
MsgCount = Pass * Count;

// настройка пакета сообщений
MkoPackC.nLast = (MsgCount - 1); // индекс последнего сообщения
memset(&MkoPackResC, 0, sizeof(mkoPackRes_t)); // сброс флагов для пакета

// инициализация описателей сообщений в пакете
for (Index = 0, MsgIndex = 0; Index < MsgCount; Index++)
  {
  // получаем сообщение из списка по индексу из интервала
  MsgListIndex = PacketMkoIntQueue1List[Int].MsgList[MsgIndex];
  Msg = &PacketMkoMsgList[MsgListIndex];
  if (++MsgIndex >= Count) MsgIndex = 0;

  // режим передачи (F1/F2/F4)
  Mode = (MKO_CMD_DIR(Msg->MkoCmd) == MKO_TX) ? (mcTM_BCtoRT) : (mcTM_RTtoBC);
  if (Msg->F4Mode) Mode = mcTM_ModeNoData;

  MkoMsgC[Index].TM   = Mode;        	// тип сообщения, RX/TX
  MkoMsgC[Index].Chan = AMkoChan;    	// канал МКО, 0 - основной, 1 - резервный
  MkoMsgC[Index].CW   = Msg->MkoCmd;    // командное слово
  MkoMsgC[Index].MissSW   = 0;       	// не прерывать передачу при ошибках
  MkoMsgC[Index].FlagSW   = 0;
  MkoMsgC[Index].FmtError = 0;
  MkoMsgC[Index].GapTime  = Msg->Interval; 

  if (Mode == mcTM_BCtoRT) memmove(&MkoBufC[Index], &Msg->Data, 32 * sizeof(WORD)); // данные для передачи

  memset(&MkoMsgResC[Index], 0, sizeof(mkoMsgRes_t)); // сброс флагов для сообщения
  }

return 0;
}
//================================================================================

int AddPacketMko_CntQue_IntToList (int ABaseMkoLine, int ABaseMkoChan)
{
int Count;
Count = KpaCommand[1];

// ошибка если число интервалов равно максимальному
if (PacketMkoIntCntCount >= PACKET_MKO_INT_COUNT) return REPLY_CMD_DATA_ERROR;

// пределы для счетчика = 1...10000 шт
if ((Count < 1) || (Count > 10000)) return REPLY_CMD_DATA_ERROR;

// если интервал не начальный - удлинняем счетчик на указанную величину
if (PacketMkoIntCntCount) Count += PacketMkoIntCntList[PacketMkoIntCntCount - 1].Count;
PacketMkoIntCntList[PacketMkoIntCntCount++].Count = Count;

return REPLY_CMD_DONE;
}
//================================================================================

int AddPacketMko_CntQue_MsgToInt (int ABaseMkoLine, int ABaseMkoChan)
{
int IntIndex, MsgIndex, MkoLine, Index, State;

IntIndex = KpaCommand[1];
MsgIndex = KpaCommand[2];
MkoLine  = KpaCommand[3];

// ошибка если номер МКО вне допустимы
if (MkoLine > 1) return REPLY_CMD_DATA_ERROR;

// ошибка если индекс интервала больше макс возможного
if (IntIndex >= PacketMkoIntCntCount) return REPLY_CMD_DATA_ERROR;

// ошибка если индекс сообщения больше макс возможного
if (MsgIndex >= PacketMkoMsgCount) return REPLY_CMD_DATA_ERROR;

for (Index = 0, State = 0; Index < 32; Index++)
  {
  if (MkoLine == 0)
    {
    if (PacketMkoIntCntList[IntIndex].Msg0List[Index] == -1) 
      { 
      PacketMkoIntCntList[IntIndex].Msg0List[Index] = MsgIndex; 
      State = 1; break; 
      }  
    }
  else
    {
    if (PacketMkoIntCntList[IntIndex].Msg1List[Index] == -1) 
      { 
      PacketMkoIntCntList[IntIndex].Msg1List[Index] = MsgIndex; 
      State = 1; break; 
      }  
    }
  }

// если данное сообщение не было добавлено к интервалу (список номеров сообщений уже заполнен)
if (!State) return REPLY_CMD_DATA_ERROR;

return REPLY_CMD_DONE;
}
//================================================================================

int RunPacketMko_CntQue_IntList (int ABaseMkoLine, int ABaseMkoChan)
{
int Status, MkoFirst, Result, Result1, Result2, TotalCount;
int DevMkoLine, DevMkoChan;

MkoFirst = KpaCommand[1];

// ошибка если номер МКО вне допустимы
if (MkoFirst > 1) return REPLY_CMD_DATA_ERROR;

// проверяем списки интервалов и сообщений
Result = RunPacketMko_CntQue_CheckList();
if (Result) return REPLY_CMD_DATA_ERROR;

// параметры линий обмена
DevMkoLine = SpoConfig.DevMkoLine & 1;
DevMkoChan = SpoConfig.DevMkoChan & 1;

TotalCount = 0;

while (1)
  {
  // подготовка пакетов
  Result1 = RunPacketMko_CntQue_PreparePacket1(TotalCount, DevMkoChan);
  Result2 = RunPacketMko_CntQue_PreparePacket2(TotalCount, DevMkoChan);
  TotalCount++;

  if ((!Result1) || (!Result2))
    {
    if (MkoFirst == 0)
      {
      if (!Result1) 
        {
        // отправка пакета по МКО-0
        Result = mkoPackIniStart(0, &MkoPackB, MkoMsgPtrB, 0);
        if (Result != OK) { Status = REPLY_ERROR_POINT_1; break; }
        }

      if (!Result2) 
        {
        // отправка пакета по МКО-1
        Result = mkoPackIniStart(1, &MkoPackC, MkoMsgPtrC, 0);
        if (Result != OK) { Status = REPLY_ERROR_POINT_3; break; }
        } 

      if (!Result1) 
        {
        // ожидание завершения пакета по МКО-0
        Result = mkoNumPackWait(0, &MkoPackB);
        if (Result != OK) { Status = REPLY_ERROR_POINT_2; break; }
        }
  
      if (!Result2) 
        {
        // ожидание завершения пакета по МКО-1
        Result = mkoNumPackWait(1, &MkoPackC);
        if (Result != OK) { Status = REPLY_ERROR_POINT_4; break; }
        }
      } 
    else
      {
      if (!Result2) 
        {
        // отправка пакета по МКО-1
        Result = mkoPackIniStart(1, &MkoPackC, MkoMsgPtrC, 0);
        if (Result != OK) { Status = REPLY_ERROR_POINT_3; break; }
        }

      if (!Result1) 
        {
        // отправка пакета по МКО-0
        Result = mkoPackIniStart(0, &MkoPackB, MkoMsgPtrB, 0);
        if (Result != OK) { Status = REPLY_ERROR_POINT_1; break; }
        }

      if (!Result2) 
        {
        // ожидание завершения пакета по МКО-1
        Result = mkoNumPackWait(1, &MkoPackC);
        if (Result != OK) { Status = REPLY_ERROR_POINT_4; break; }
        }

      if (!Result1) 
        {
        // ожидание завершения пакета по МКО-0
        Result = mkoNumPackWait(0, &MkoPackB);
        if (Result != OK) { Status = REPLY_ERROR_POINT_2; break; }
        }
      }
    }
  else
    {
    // передача завершена
    Status = REPLY_CMD_DONE; 
    break;
    }
  }

return Status;
}
//================================================================================

int RunPacketMko_CntQue_CheckList (void)
{
int IntIndex;

// ошибка если список интервалов пустой
if (!PacketMkoIntQueueCount) return 1;
                               
for (IntIndex = 0; IntIndex < PacketMkoIntCntCount; IntIndex++)
  {
  // ошибка если список сообщений в интервале пустой
  if (PacketMkoIntCntList[IntIndex].Msg0List[0] != -1) continue;
  if (PacketMkoIntCntList[IntIndex].Msg1List[0] != -1) continue;
  return 1;
  }

// список интервалов в норме
return 0;
}
//================================================================================

int RunPacketMko_CntQue_PreparePacket1 (int ATotalCount, int AMkoChan)
{
int IntIndex, MsgIndex, Int, Count, Pass, MsgCount;
int Index, Mode, MsgListIndex;
TPacketMkoMsg *Msg;

// поиск временного интервала
for (IntIndex = 0, Int = -1; IntIndex < PacketMkoIntCntCount; IntIndex++)
  {
  if (ATotalCount < PacketMkoIntCntList[IntIndex].Count) { Int = IntIndex; break; }
  }

// указанное время лежит вне объявленных интервалов
if (Int == -1) return 1;

// расчет числа сообщений в интервале
for (MsgIndex = 0, Count = 0; MsgIndex < 32; MsgIndex++)
  {
  if (PacketMkoIntCntList[Int].Msg0List[MsgIndex] == -1) break;
  Count++;
  }  

// в данном интервале нет сообщений
if (Count == 0) return 1;

// общее число сообщений собираемое в пакет
Pass = 32 / Count;
MsgCount = Pass * Count;

// настройка пакета сообщений
MkoPackB.nLast = (MsgCount - 1); // индекс последнего сообщения
memset(&MkoPackResB, 0, sizeof(mkoPackRes_t)); // сброс флагов для пакета

// инициализация описателей сообщений в пакете
for (Index = 0, MsgIndex = 0; Index < MsgCount; Index++)
  {
  // получаем сообщение из списка по индексу из интервала
  MsgListIndex = PacketMkoIntCntList[Int].Msg0List[MsgIndex];
  Msg = &PacketMkoMsgList[MsgListIndex];
  if (++MsgIndex >= Count) MsgIndex = 0;

  // режим передачи (F1/F2/F4)
  Mode = (MKO_CMD_DIR(Msg->MkoCmd) == MKO_TX) ? (mcTM_BCtoRT) : (mcTM_RTtoBC);
  if (Msg->F4Mode) Mode = mcTM_ModeNoData;

  MkoMsgB[Index].TM   = Mode;        	// тип сообщения, RX/TX
  MkoMsgB[Index].Chan = AMkoChan;    	// канал МКО, 0 - основной, 1 - резервный
  MkoMsgB[Index].CW   = Msg->MkoCmd;    // командное слово
  MkoMsgB[Index].MissSW   = 0;       	// не прерывать передачу при ошибках
  MkoMsgB[Index].FlagSW   = 0;
  MkoMsgB[Index].FmtError = 0;
  MkoMsgB[Index].GapTime  = Msg->Interval; 

  if (Mode == mcTM_BCtoRT) memmove(&MkoBufB[Index], &Msg->Data, 32 * sizeof(WORD)); // данные для передачи

  memset(&MkoMsgResB[Index], 0, sizeof(mkoMsgRes_t)); // сброс флагов для сообщения
  }

return 0;
}
//================================================================================

int RunPacketMko_CntQue_PreparePacket2 (int ATotalCount, int AMkoChan)
{
int IntIndex, MsgIndex, Int, Count, Pass, MsgCount;
int Index, Mode, MsgListIndex;
TPacketMkoMsg *Msg;

// поиск временного интервала
for (IntIndex = 0, Int = -1; IntIndex < PacketMkoIntCntCount; IntIndex++)
  {
  if (ATotalCount < PacketMkoIntCntList[IntIndex].Count) { Int = IntIndex; break; }
  }

// указанное время лежит вне объявленных интервалов
if (Int == -1) return 1;

// расчет числа сообщений в интервале
for (MsgIndex = 0, Count = 0; MsgIndex < 32; MsgIndex++)
  {
  if (PacketMkoIntCntList[Int].Msg1List[MsgIndex] == -1) break;
  Count++;
  }  

// в данном интервале нет сообщений
if (Count == 0) return 1;

// общее число сообщений собираемое в пакет
Pass = 32 / Count;
MsgCount = Pass * Count;

// настройка пакета сообщений
MkoPackB.nLast = (MsgCount - 1); // индекс последнего сообщения
memset(&MkoPackResB, 0, sizeof(mkoPackRes_t)); // сброс флагов для пакета

// инициализация описателей сообщений в пакете
for (Index = 0, MsgIndex = 0; Index < MsgCount; Index++)
  {
  // получаем сообщение из списка по индексу из интервала
  MsgListIndex = PacketMkoIntCntList[Int].Msg1List[MsgIndex];
  Msg = &PacketMkoMsgList[MsgListIndex];
  if (++MsgIndex >= Count) MsgIndex = 0;

  // режим передачи (F1/F2/F4)
  Mode = (MKO_CMD_DIR(Msg->MkoCmd) == MKO_TX) ? (mcTM_BCtoRT) : (mcTM_RTtoBC);
  if (Msg->F4Mode) Mode = mcTM_ModeNoData;

  MkoMsgB[Index].TM   = Mode;        	// тип сообщения, RX/TX
  MkoMsgB[Index].Chan = AMkoChan;    	// канал МКО, 0 - основной, 1 - резервный
  MkoMsgB[Index].CW   = Msg->MkoCmd;    // командное слово
  MkoMsgB[Index].MissSW   = 0;       	// не прерывать передачу при ошибках
  MkoMsgB[Index].FlagSW   = 0;
  MkoMsgB[Index].FmtError = 0;
  MkoMsgB[Index].GapTime  = Msg->Interval; 

  if (Mode == mcTM_BCtoRT) memmove(&MkoBufB[Index], &Msg->Data, 32 * sizeof(WORD)); // данные для передачи

  memset(&MkoMsgResB[Index], 0, sizeof(mkoMsgRes_t)); // сброс флагов для сообщения
  }

return 0;
}
//================================================================================

