
/*! \file MkoStressTestUnit.c
 * \author Гапонов Р.В.
 * \date 2014.01.17
 * \version 1.0.0
 * \brief Модуль содержит функции проверки работы МКО в максимально нагруженном режиме
 */

#include "MkoStressTestUnit.h"
#include "BaloiUnit_v11.h"
#include "StdLib.h"
//================================================================================

/*

// формирование командного слова МКО
function MKO_CMD (Addr, SubAddr, Dir, Size)
{
var Cmd = 0;
Cmd |= (Addr & 0x1F) << 11;
Cmd |= (Dir & 0x01) << 10;
Cmd |= (SubAddr & 0x1F) << 5;
Cmd |= (Size & 0x1F) << 0;
return Cmd;
}
//================================================================================

// вызов функции СПО для тестирования МКО
function МКО_НАГРУЗОЧНЫЙ_ТЕСТ (Mode, MkoLine, MkoChan, MkoAddr, MkoSubAddr, MkoCmdSize, MkoData, MsgCount, PackSize, Interval)
{
var Result;
var Link = ((MkoLine) ? (1) : (0)) | ((MkoChan) ? (2) : (0));
Mode = Mode.toLowerCase();

if (Mode == "запись")
  {
  if (MkoData.length != MkoCmdSize) return -1;
  var MkoCmd = MKO_CMD(MkoAddr, MkoSubAddr, 0, MkoCmdSize);
  Result = ВЫДАТЬ_ДИР([0x0165, Link, MkoCmd, MsgCount, PackSize, Interval], MkoData, 240000);
  if (Result) Result = 1;
  }
else if (Mode == "чтение")
  {
  if (MkoData.length != MkoCmdSize) return -1;
  var MkoCmd = MKO_CMD(MkoAddr, MkoSubAddr, 1, MkoCmdSize);
  Result = ВЫДАТЬ_ДИР([0x0265, Link, MkoCmd, MsgCount, PackSize, Interval], MkoData, 240000);
  if (Result) Result = 1;
  }
else if (Mode == "команда")
  {
  // выбираем направление команды F4 для указанного кода операции
  var CmdDir = (0x000DFFFF & (1 << (MkoCmdSize & 0x1F))) ? (1) : (0);
  var MkoCmd = MKO_CMD(MkoAddr, MkoSubAddr, CmdDir, MkoCmdSize);
  Result = ВЫДАТЬ_ДИР([0x0365, Link, MkoCmd], [], 5000);
  if (Result) Result = 1;
  }
else
  {
  Result = -1;
  }

return Result;
}
//================================================================================

// примеры
var Result = МКО_НАГРУЗОЧНЫЙ_ТЕСТ("запись", 1,1, 5,5,5, [1,2,3,4,5], 10000,32,0);
if (Result == 0)
  {
  if (МКО_БУФЕР[0]) ...
  if (МКО_БУФЕР[1]) ...
  if (МКО_БУФЕР[2]) ...
  if (МКО_БУФЕР[3]) ...
  }
else
  {
  ВЫВЕСТИ("Ошибка теста МКО (запись)");
  }

var Result = МКО_НАГРУЗОЧНЫЙ_ТЕСТ("чтение", 1,1, 5,5,5, [1,2,3,4,5], 10000,32,0);
if (Result == 0)
  {
  if (МКО_БУФЕР[0]) ...
  if (МКО_БУФЕР[1]) ...
  if (МКО_БУФЕР[2]) ...
  if (МКО_БУФЕР[3]) ...
  }
else
  {
  ВЫВЕСТИ("Ошибка теста МКО (чтение)");
  }

var Result = МКО_НАГРУЗОЧНЫЙ_ТЕСТ("команда", 1,1, 5,5,5, [], 0,0,0);
if (Result == 0)
  {
  if (МКО_БУФЕР[0]) ...
  }
else
  {
  ВЫВЕСТИ("Ошибка теста МКО (команда)");
  }

*/

// [номера слов] (option: [номера битов])

// данные на подадресе 1 МКПА:
// [0][7..0] - код команды, = 0x65
// [0][15..8] - код операции, = 1..3

// код операции = 0x01, тест абонента МКО на запись (тип обмена МКО Ф2)
// [0][7..0] - код команды, = 0x65
// [0][15..8] - код операции, = 1
// [1][0] - линия МКО для операции (0 = 1 линия МКО, 1 = 2 линия МКО)
// [1][1] - канал МКО для операции (0 = основной канал МКО, 1 = резервный канал МКО)
// [2] - команда МКО для операции (16 бит, [Адрес, Направление, Подадрес, Размер данных])
// [3] - число сообщений, которое будет передано абоненту МКО (1..65535)
// [4] - размер пакета МКО, сообщений (1..32, если параметр = 0, число сообщений по умолчанию = 32)
// [5] - интервал "начало сообщения - начало сообщения" в пакете МКО, мксек (0..65535)
// дополнительные данные на подадрсе 2: 
// 1..32 слова для передачи абоненту в сообщении
// возврат, результат: OK/Error
// возврат, данные: 8 слов со статистикой обмена
// [0] = общее число переданных сообщений
// [1] = общее число возникших ошибок
// [2] = общее число ошибок на линии
// [3] = общее число ошибок "нет ОС"
// [4] = общее число ошибок "неисправность абонента"
// [5] = общее число ошибок "неисправность ОУ"
// [6] = общее число ошибок "абонент занят"
// [7] = общее число ошибок "сбой в сообщении"
// пример: 
// ВЫДАТЬ_ДИР([0x0165, xxxx, xxxx, xxxx, 0], [1,2,3,4,5,6,7,8,...]);

// код операции = 0x02, тест абонента МКО на чтение (тип обмена МКО Ф2)
// [0][7..0] - код команды, = 0x65
// [0][15..8] - код операции, = 2
// [1][0] - линия МКО для операции (0 = 1 линия МКО, 1 = 2 линия МКО)
// [1][1] - канал МКО для операции (0 = основной канал МКО, 1 = резервный канал МКО)
// [2] - команда МКО для операции (16 бит, [Адрес, Направление, Подадрес, Размер данных])
// [3] - число сообщений, которое будет считано от абонента МКО (1..65535)
// [4] - размер пакета МКО, сообщений (1..32, если параметр = 0, число сообщений по умолчанию = 32)
// [5] - интервал "начало сообщения - начало сообщения" в пакете МКО, мксек (0..65535)
// дополнительные данные на подадрсе 2: 
// 1..32 слова для контроля принятых данных от абонента
// возврат, результат: OK/Error
// возврат, данные: 9 слов со статистикой обмена
// [0] = общее число переданных сообщений
// [1] = общее число возникших ошибок
// [2] = общее число ошибок на линии
// [3] = общее число ошибок "нет ОС"
// [4] = общее число ошибок "неисправность абонента"
// [5] = общее число ошибок "неисправность ОУ"
// [6] = общее число ошибок "абонент занят"
// [7] = общее число ошибок "сбой в сообщении"
// [8] = общее число ошибок "принятые данные несовпадают с ожидаемыми"
// пример: 
// ВЫДАТЬ_ДИР([0x0265, xxxx, xxxx, xxxx, 0], [1,2,3,4,5,6,7,8,...]);

// код операции = 0x03, передача обоненту МКО команды (тип обмена МКО Ф4)
// [0][7..0] - код команды, = 0x65
// [0][15..8] - код операции, = 3
// [1][0] - линия МКО для операции (0 = 1 линия МКО, 1 = 2 линия МКО)
// [1][1] - канал МКО для операции (0 = основной канал МКО, 1 = резервный канал МКО)
// [2] - команда МКО для операции (16 бит, [Адрес, Направление, Подадрес, Размер данных])
// возврат, результат: OK/Error
// возврат, данные: 1 слово
// [0] = принятое ОС от абонента (или 0xFFFF если была ошибка до обмена)
// пример: 
// ВЫДАТЬ_ДИР([0x0365, xxxx, xxxx], []);

// код операции = 0x04, управление передачей статистики обменов МКО (если вкл - после обработки команд 1..3 выдает полный результат обмена по последнему пакету МКО)
// [0][7..0] - код команды, = 0x65
// [0][15..8] - код операции, = 4
// [1] - флаг управления (0 = откл выдачу статистики, не 0 = вкл выдачу статистики)
// возврат, результат: OK
// возврат, данные: нет
// пример: 
// ВЫДАТЬ_ДИР([0x0465, 1], []);

//================================================================================

int MkoStressMsgCount;                              	// общее число переданных сообщений  
int MkoStressErrorCount;                            	// общее число возникших ошибок      

int MkoStressLinkErrorCount;                        	// ошибки при передаче               
int MkoStressNoOsErrorCount;                        	// ошибки "нет ОС"                   
int MkoStressAbErrorCount;                          	// абонент выставил биты ошибок      
int MkoStressTermErrorCount;                        	// ОУ выставил биты ошибок           
int MkoStressBusyErrorCount;                        	// абонент выставил занятость        
int MkoStressMsgErrorCount;                         	// ошибки в сообщении                

int MkoStressDataErrorCount;                        	// ошибки в данных                   
int MkoStressLastOS;                                	// последнее принятое ОС             

int MkoStressShowLastPackResult;                    	// флаг выдачи статистики по последнему пакету МКО
TMkoStressLastPackResult MkoStressLastPackResult;   	// кеш результатов обмена для последнего пакета

int MkoStressPackResCount;       				// число элементов в списке результатов для пакетов
mkoPackRes_t MkoStressPackResList[MKO_STRESS_PACK_LIST_SIZE];   // список результатов для пакетов
int MkoStressMsgResCount;					// число элементов в списке результатов для сообщений
mkoMsgRes_t MkoStressMsgResList[MKO_STRESS_MSG_LIST_SIZE];      // список результатов для сообщений
//================================================================================

void InitMkoStressTest (void)
{
MkoStressShowLastPackResult = 0;
MkoStressPackResCount = 0;
MkoStressMsgResCount = 0;
}
//================================================================================

void KpaCommand65 (int BaseMkoLine, int BaseMkoChan)
{
int Opcode, Index, Size, Result;
WORD Buffer[32];

// читаем код операции
Opcode = KpaCommand[0] >> 8;

if (Opcode == 0x01) // передача набора сообщений в абоненты на МКО
  {
  MkoStressPackResCount = 0;
  MkoStressMsgResCount = 0;
  Result = MkoStressSendTest(BaseMkoLine, BaseMkoChan);

  // выдача статистики по последнему пакету МКО
  if (MkoStressShowLastPackResult) MkoStressSendStatistic(BaseMkoLine, BaseMkoChan);

  // переносим результаты
  Buffer[0] = MkoStressMsgCount;
  Buffer[1] = MkoStressErrorCount;
  Buffer[2] = MkoStressLinkErrorCount;
  Buffer[3] = MkoStressNoOsErrorCount;
  Buffer[4] = MkoStressAbErrorCount;
  Buffer[5] = MkoStressTermErrorCount;
  Buffer[6] = MkoStressBusyErrorCount;
  Buffer[7] = MkoStressMsgErrorCount;
  ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, Result, KPA_SUBADDR_IRQ, &Buffer, sizeof(WORD) * 8);
  }
else if (Opcode == 0x02) // прием набора сообщений из абонентов на МКО
  {
  MkoStressPackResCount = 0;
  MkoStressMsgResCount = 0;
  Result = MkoStressRecvTest(BaseMkoLine, BaseMkoChan);

  // выдача статистики по последнему пакету МКО
  if (MkoStressShowLastPackResult) MkoStressSendStatistic(BaseMkoLine, BaseMkoChan);

  // переложить результаты
  Buffer[0] = MkoStressMsgCount;
  Buffer[1] = MkoStressErrorCount;
  Buffer[2] = MkoStressLinkErrorCount;
  Buffer[3] = MkoStressNoOsErrorCount;
  Buffer[4] = MkoStressAbErrorCount;
  Buffer[5] = MkoStressTermErrorCount;
  Buffer[6] = MkoStressBusyErrorCount;
  Buffer[7] = MkoStressMsgErrorCount;
  Buffer[8] = MkoStressDataErrorCount;
  ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, Result, KPA_SUBADDR_IRQ, &Buffer, sizeof(WORD) * 9);
  }
else if (Opcode == 0x03) // передача сообщения МКО в формате F4
  {
  MkoStressPackResCount = 0;
  MkoStressMsgResCount = 0;
  Result = MkoStressSendF4Test(BaseMkoLine, BaseMkoChan);

  // выдача статистики по пакету МКО
  if (MkoStressShowLastPackResult) MkoStressSendStatistic(BaseMkoLine, BaseMkoChan);

  // переложить результаты
  Buffer[0] = MkoStressLastOS;
  ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, Result, KPA_SUBADDR_IRQ, &Buffer, sizeof(WORD) * 1);
  }
else if (Opcode == 0x04) // управление передачей статистики
  {
  MkoStressShowLastPackResult = (KpaCommand[1]) ? (1) : (0);
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ);
  }
else if (Opcode == 0x80) // считывание размеров массивов статистики
  {
  Buffer[0] = MkoStressPackResCount;
  Buffer[1] = MkoStressMsgResCount;
  ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ, &Buffer, sizeof(WORD) * 2);
  }
else if (Opcode == 0x81) // считывание массива результатов для пакетов
  {
  Index = KpaCommand[1];
  Size = KpaCommand[2];
  if ((Index < 0) || (Index >= MKO_STRESS_PACK_LIST_SIZE))
    {
    ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DATA_ERROR, KPA_SUBADDR_IRQ);
    return;
    }
  if ((Size < 1) || (Size > 16)) 
    {
    ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DATA_ERROR, KPA_SUBADDR_IRQ);
    return;
    }
  if ((Index + Size) > MKO_STRESS_PACK_LIST_SIZE)
    {
    ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DATA_ERROR, KPA_SUBADDR_IRQ);
    return;
    }

  memmove(&Buffer, &MkoStressPackResList[Index], Size * sizeof(mkoPackRes_t));
  ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ, &Buffer, Size * sizeof(mkoPackRes_t));
  }
else if (Opcode == 0x82) // считывание массива результатов для сообщений
  {
  Index = KpaCommand[1];
  Size = KpaCommand[2];
  if ((Index < 0) || (Index >= MKO_STRESS_MSG_LIST_SIZE))
    {
    ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DATA_ERROR, KPA_SUBADDR_IRQ);
    return;
    }
  if ((Size < 1) || (Size > 8)) 
    {
    ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DATA_ERROR, KPA_SUBADDR_IRQ);
    return;
    }
  if ((Index + Size) > MKO_STRESS_MSG_LIST_SIZE)
    {
    ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DATA_ERROR, KPA_SUBADDR_IRQ);
    return;
    }

  memmove(&Buffer, &MkoStressMsgResList[Index], Size * sizeof(mkoMsgRes_t));
  ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ, &Buffer, Size * sizeof(mkoMsgRes_t));
  }
else // неизвестная операция
  {
  Result = REPLY_CMD_DATA_ERROR;
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, Result, KPA_SUBADDR_IRQ);
  }
}
//================================================================================

#define INC_PARAM(Param) { if (Param < 0xFFFF) Param++; }
//================================================================================

// передача набора сообщений МКО
int MkoStressSendTest (int BaseMkoLine, int BaseMkoChan)
{
int MkoLine, MkoChan, MkoCmd, MsgRemain, PackLen, Interval;
int Result, Status, Cmd, Count, State1, State2;
WORD OrigData[32];

// очистка результатов выдачи
MkoStressMsgCount       = 0;
MkoStressErrorCount     = 0;
MkoStressLinkErrorCount = 0;
MkoStressNoOsErrorCount = 0;
MkoStressAbErrorCount   = 0;
MkoStressTermErrorCount = 0;
MkoStressBusyErrorCount = 0;
MkoStressMsgErrorCount  = 0;

// считываем параметры передачи из команды
MkoLine   = (KpaCommand[1] & 0x01) ? (1) : (0);       // линия МКО для передачи, 0..1 (первая/вторая)
MkoChan   = (KpaCommand[1] & 0x02) ? (1) : (0);       // канал МКО для передачи, 0..1 (основной/резервный)
MkoCmd    = KpaCommand[2];                            // командное слово МКО для передачи
MsgRemain = KpaCommand[3];                            // число сообщений, 1..65535
PackLen   = (KpaCommand[4]) ? (KpaCommand[4]) : (32); // размер пакета, 1..32
Interval  = KpaCommand[5];

// проверяем параметры
if (MKO_CMD_DIR(MkoCmd) != MKO_TX) return REPLY_CMD_DATA_ERROR;
if ((MKO_CMD_SUBADDR(MkoCmd) == 0) || (MKO_CMD_SUBADDR(MkoCmd) == 31)) return REPLY_CMD_DATA_ERROR;
if (!MsgRemain) return REPLY_CMD_DATA_ERROR;
if ((PackLen < 1) || (PackLen > 32)) return REPLY_CMD_DATA_ERROR;

// считываем данные для передачи
Cmd = MKO_CMD(KPA_ADDR, KPA_SUBADDR_CMD + 1, MKO_RX, MKO_CMD_SIZE(MkoCmd));
Result = ReadExtMsgData(BaseMkoLine, BaseMkoChan, Cmd, KpaCommand[7], (WORD*) &OrigData);
if (Result != REPLY_CMD_DONE) return Result;

// начальное состояние 
State1 = STATE_IDLE;
State2 = STATE_IDLE;
MkoStressPrepareSet1(PackLen, &OrigData, MkoChan, MkoCmd, Interval);
MkoStressPrepareSet2(PackLen, &OrigData, MkoChan, MkoCmd, Interval);

while (1)
  {
  if (State1 == STATE_WORK)
    {
    // [начало интервала D]
    // ожидание завершения пакета 1
    Result = mkoNumPackWait(MkoLine, &MkoPackB);
    if (Result != OK) { Status = REPLY_ERROR_POINT_2; break; }
    // пакет 1 завершен
    State1 = STATE_IDLE;     
    // [начало интервала A]
    // проверка результатов пакета 1
    MkoStressCheckTxPacket1(MsgRemain);
    }

  if ((MsgRemain) && (State1 == STATE_IDLE))
    {
    // подготовка пакета 1
    Count = (MsgRemain > PackLen) ? (PackLen) : (MsgRemain);
    MkoStressPreparePacket1(Count);
    // отправка пакета 1
    Result = mkoPackIniStart(MkoLine, &MkoPackB, MkoMsgPtrB, 0);
    if (Result != OK) { Status = REPLY_ERROR_POINT_1; break; }
    // пакет 1 передается 
    State1 = STATE_WORK;
    // число оставшихся сообщений 
    MsgRemain -= Count; 
    }

  if (State2 == STATE_WORK)
    {
    // [начало интервала B]
    // ожидание завершения пакета 2
    Result = mkoNumPackWait(MkoLine, &MkoPackC);
    if (Result != OK) { Status = REPLY_ERROR_POINT_2; break; }
    // пакет 2 завершен
    State2 = STATE_IDLE; 
    // [начало интервала C]
    // проверка результатов пакета 2
    MkoStressCheckTxPacket2(MsgRemain);
    }

  if ((MsgRemain) && (State2 == STATE_IDLE))
    {
    // подготовка пакета 2
    Count = (MsgRemain > PackLen) ? (PackLen) : (MsgRemain);
    MkoStressPreparePacket2(Count);
    // отправка пакета 2
    Result = mkoPackIniStart(MkoLine, &MkoPackC, MkoMsgPtrC, 0);
    if (Result != OK) { Status = REPLY_ERROR_POINT_1; break; }
    // пакет 2 передается
    State2 = STATE_WORK;
    // число оставшихся сообщений
    MsgRemain -= Count; 
    }
    
  if ((State1 == STATE_IDLE) && (State2 == STATE_IDLE))
    {
    // передача завершена
    Status = REPLY_CMD_DONE; break;
    }
  }

return Status;
}
//================================================================================

// прием набора сообщений МКО
int MkoStressRecvTest (int BaseMkoLine, int BaseMkoChan)
{
int MkoLine, MkoChan, MkoCmd, MsgRemain, PackLen, Interval;
int Result, Status, Cmd, Count, State1, State2;
WORD OrigData[32];

// очистка результатов выдачи
MkoStressMsgCount       = 0;
MkoStressErrorCount     = 0;
MkoStressLinkErrorCount = 0;
MkoStressNoOsErrorCount = 0;
MkoStressAbErrorCount   = 0;
MkoStressTermErrorCount = 0;
MkoStressBusyErrorCount = 0;
MkoStressMsgErrorCount  = 0;
MkoStressDataErrorCount = 0;

// считываем параметры передачи из команды
MkoLine   = (KpaCommand[1] & 0x01) ? (1) : (0);       // линия МКО для приема, 0..1 (первая/вторая)
MkoChan   = (KpaCommand[1] & 0x02) ? (1) : (0);       // канал МКО для приема, 0..1 (основной/резервный)
MkoCmd    = KpaCommand[2];                            // командное слово МКО для приема
MsgRemain = KpaCommand[3];                            // число сообщений, 1..65535
PackLen   = (KpaCommand[4]) ? (KpaCommand[4]) : (32); // размер пакета, 1..32
Interval  = KpaCommand[5];

// проверяем параметры
if (MKO_CMD_DIR(MkoCmd) != MKO_RX) return REPLY_CMD_DATA_ERROR;
if ((MKO_CMD_SUBADDR(MkoCmd) == 0) || (MKO_CMD_SUBADDR(MkoCmd) == 31)) return REPLY_CMD_DATA_ERROR;
if (!MsgRemain) return REPLY_CMD_DATA_ERROR;
if ((PackLen < 1) || (PackLen > 32)) return REPLY_CMD_DATA_ERROR;

// считываем контрольные данные для проверки принятых от абонента
Cmd = MKO_CMD(KPA_ADDR, KPA_SUBADDR_CMD + 1, MKO_RX, MKO_CMD_SIZE(MkoCmd));
Result = ReadExtMsgData(BaseMkoLine, BaseMkoChan, Cmd, KpaCommand[7], (WORD*) &OrigData);
if (Result != REPLY_CMD_DONE) return Result;

// начальное состояние 
State1 = STATE_IDLE;
State2 = STATE_IDLE;
MkoStressPrepareSet1(PackLen, 0, MkoChan, MkoCmd, Interval);
MkoStressPrepareSet2(PackLen, 0, MkoChan, MkoCmd, Interval);

while (1)
  {
  if (State1 == STATE_WORK)
    {
    // [начало интервала D]
    // ожидание завершения пакета 1
    Result = mkoNumPackWait(MkoLine, &MkoPackB);
    if (Result != OK) { Status = REPLY_ERROR_POINT_2; break; }
    // пакет 1 завершен
    State1 = STATE_IDLE;     
    // [начало интервала A]
    // проверка результатов пакета 1
    MkoStressCheckRxPacket1(&OrigData, MsgRemain);
    }

  if ((MsgRemain) && (State1 == STATE_IDLE))
    {
    // подготовка пакета 1
    Count = (MsgRemain > PackLen) ? (PackLen) : (MsgRemain);
    MkoStressPreparePacket1(Count);
    // запуск пакета 1
    Result = mkoPackIniStart(MkoLine, &MkoPackB, MkoMsgPtrB, 0);
    if (Result != OK) { Status = REPLY_ERROR_POINT_1; break; }
    // пакет 1 в работе 
    State1 = STATE_WORK;
    // число оставшихся сообщений 
    MsgRemain -= Count; 
    }

  if (State2 == STATE_WORK)
    {
    // [начало интервала B]
    // ожидание завершения пакета 2
    Result = mkoNumPackWait(MkoLine, &MkoPackC);
    if (Result != OK) { Status = REPLY_ERROR_POINT_2; break; }
    // пакет 2 завершен
    State2 = STATE_IDLE; 
    // [начало интервала C]
    // проверка результатов пакета 2
    MkoStressCheckRxPacket2(&OrigData, MsgRemain);
    }

  if ((MsgRemain) && (State2 == STATE_IDLE))
    {
    // подготовка пакета 2
    Count = (MsgRemain > PackLen) ? (PackLen) : (MsgRemain);
    MkoStressPreparePacket2(Count);
    // запуск пакета 2
    Result = mkoPackIniStart(MkoLine, &MkoPackC, MkoMsgPtrC, 0);
    if (Result != OK) { Status = REPLY_ERROR_POINT_1; break; }
    // пакет 2 в работе
    State2 = STATE_WORK;
    // число оставшихся сообщений
    MsgRemain -= Count; 
    }

  if ((State1 == STATE_IDLE) && (State2 == STATE_IDLE))
    {
    // прием завершен
    Status = REPLY_CMD_DONE; break;
    }
  }

return Status;
}
//================================================================================

// передача сообщения МКО в формате F4
int MkoStressSendF4Test (int BaseMkoLine, int BaseMkoChan)
{
int MkoLine, MkoChan, MkoCmd;
int Result, Status, Reply;

// очистка результатов выдачи
MkoStressLastOS = 0xFFFF;

// считываем параметры передачи из команды
MkoLine = (KpaCommand[1] & 0x01) ? (1) : (0);       // линия МКО для команды, 0..1 (первая/вторая)
MkoChan = (KpaCommand[1] & 0x02) ? (1) : (0);       // канал МКО для команды, 0..1 (основной/резервный)
MkoCmd  = KpaCommand[2];                            // командное слово МКО для передачи команды F4

// проверяем параметры
if (MKO_CMD_DIR(MkoCmd) != MKO_TX) return REPLY_CMD_DATA_ERROR;
if ((MKO_CMD_SUBADDR(MkoCmd) != 0) && (MKO_CMD_SUBADDR(MkoCmd) != 31)) return REPLY_CMD_DATA_ERROR;

// инициализация описателя сообщения
MkoMsgB[0].TM   = mcTM_ModeNoData; // тип сообщения, TX/F4
MkoMsgB[0].Chan = MkoChan;         // канал МКО, 0 - основной, 1 - резервный
MkoMsgB[0].CW   = MkoCmd;          // командное слово
MkoMsgB[0].MissSW   = 0;           // не прерывать передачу при ошибках
MkoMsgB[0].FlagSW   = 0;
MkoMsgB[0].FmtError = 0;
MkoMsgB[0].GapTime  = 0;           // передача без пауз

memset(&MkoMsgResB[0], 0, sizeof(mkoMsgRes_t)); // сброс флагов для сообщения

// настройка пакета сообщений
MkoPackB.nLast = 0; // индекс последнего сообщения
memset(&MkoPackResB, 0, sizeof(mkoPackRes_t)); // сброс флагов для пакета

do
  {
  // запуск пакета 1
  Result = mkoPackIniStart(MkoLine, &MkoPackB, MkoMsgPtrB, 0);
  if (Result != OK) { Status = REPLY_ERROR_POINT_1; break; }

  // ожидание завершения пакета 1
  Result = mkoNumPackWait(MkoLine, &MkoPackB);
  if (Result != OK) { Status = REPLY_ERROR_POINT_2; break; }

  // [проверка результатов для сообщения]
  // ошибка формата передачи
  if (MkoMsgResB[0].FmtError) { Status = REPLY_MKO_LINK_ERROR; break; } 

  // нет ответного слова
  if (MkoMsgResB[0].MissSW) { Status = REPLY_MKO_NO_OS_ERROR; break; }

  // есть флаги в ОС
  if (MkoMsgResB[0].FlagSW)
    {
    Reply = MkoMsgResB[0].SW;
    if (MKO_OS_OU_ERROR(Reply))  { Status = REPLY_MKO_OU_ERROR; break; }
    if (MKO_OS_AB_ERROR(Reply))  { Status = REPLY_MKO_OU_ERROR; break; }
    if (MKO_OS_BUSY(Reply))      { Status = REPLY_DEVICE_BUSY_ERROR; break; }
    if (MKO_OS_MSG_ERROR(Reply)) { Status = REPLY_MKO_OU_ERROR; break; }
    }

  // сохраняем ответное слово
  MkoStressLastOS = MkoMsgResB[0].SW; 
  Status = REPLY_CMD_DONE; 
  }
while (0);

// сохраняем результат для пакета
if (MkoStressShowLastPackResult) MkoStressSaveStateB();

return Status;
}
//================================================================================

void MkoStressPrepareSet1 (int AMsgCount, void *ABuffer, int AMkoChan, WORD AMkoCmd, WORD Interval)
{ 
int Index, Mode;

// инициализация описателей сообщений в пакете
for (Index = 0; Index < AMsgCount; Index++)
  {
  Mode = (ABuffer) ? (mcTM_BCtoRT) : (mcTM_RTtoBC);

  MkoMsgB[Index].TM   = Mode;        // тип сообщения, RX/TX
  MkoMsgB[Index].Chan = AMkoChan;    // канал МКО, 0 - основной, 1 - резервный
  MkoMsgB[Index].CW   = AMkoCmd;     // командное слово
  MkoMsgB[Index].MissSW   = 0;       // не прерывать передачу при ошибках
  MkoMsgB[Index].FlagSW   = 0;
  MkoMsgB[Index].FmtError = 0;
  MkoMsgB[Index].GapTime  = Interval; // передача без пауз

  if (ABuffer) memmove(&MkoBufB[Index], ABuffer, 32 * sizeof(WORD)); // данные для передачи
  }
}
//================================================================================

void MkoStressPrepareSet2 (int AMsgCount, void *ABuffer, int AMkoChan, WORD AMkoCmd, WORD Interval)
{ 
int Index, Mode;

// инициализация описателей сообщений в пакете
for (Index = 0; Index < AMsgCount; Index++)
  {
  Mode = (ABuffer) ? (mcTM_BCtoRT) : (mcTM_RTtoBC);

  MkoMsgC[Index].TM   = Mode;        // тип сообщения, TX
  MkoMsgC[Index].Chan = AMkoChan;    // канал МКО, 0 - основной, 1 - резервный
  MkoMsgC[Index].CW   = AMkoCmd;     // командное слово
  MkoMsgC[Index].MissSW   = 0;       // не прерывать передачу при ошибках
  MkoMsgC[Index].FlagSW   = 0;
  MkoMsgC[Index].FmtError = 0;
  MkoMsgC[Index].GapTime  = Interval; // передача без пауз

  if (ABuffer) memmove(&MkoBufC[Index], ABuffer, 32 * sizeof(WORD)); // данные для передачи
  }
}
//================================================================================

void MkoStressPreparePacket1 (int AMsgCount)
{ 
int Index;

// настройка пакета сообщений
MkoPackB.nLast = (AMsgCount - 1); // индекс последнего сообщения
memset(&MkoPackResB, 0, sizeof(mkoPackRes_t)); // сброс флагов для пакета

// инициализация описателей сообщений в пакете
for (Index = 0; Index < AMsgCount; Index++)
  {
  memset(&MkoMsgResB[Index], 0, sizeof(mkoMsgRes_t)); // сброс флагов для сообщения
  }
}
//================================================================================

void MkoStressPreparePacket2 (int AMsgCount)
{ 
int Index;

// настройка пакета сообщений
MkoPackC.nLast = (AMsgCount - 1); // индекс последнего сообщения
memset(&MkoPackResC, 0, sizeof(mkoPackRes_t)); // сброс флагов для пакета

// инициализация описателей сообщений в пакете
for (Index = 0; Index < AMsgCount; Index++)
  {
  memset(&MkoMsgResC[Index], 0, sizeof(mkoMsgRes_t)); // сброс флагов для сообщения
  }
}
//================================================================================

void MkoStressCheckTxPacket1 (int MsgCount)
{
int Index, Reply, ErrorFlag;

// сохраняем результат для последнего пакета
if ((MkoStressShowLastPackResult) && (!MsgCount)) MkoStressSaveStateB();

// сохраняем результат для текущего пакета
if (MkoStressPackResCount < MKO_STRESS_PACK_LIST_SIZE) MkoStressPackResList[MkoStressPackResCount++] = MkoPackResB;

// перебираем все завершенные сообщения
for (Index = 0; Index <= MkoPackResB.nMsg; Index++)
  {
  INC_PARAM(MkoStressMsgCount);

  // сохраняем результат для сообщений
  if (MkoStressMsgResCount < MKO_STRESS_MSG_LIST_SIZE) MkoStressMsgResList[MkoStressMsgResCount++] = MkoMsgResB[Index];

  // ошибка формата передачи
  if (MkoMsgResB[Index].FmtError) 
    {
    INC_PARAM(MkoStressErrorCount);
    INC_PARAM(MkoStressLinkErrorCount);
    continue;
    }

  // нет ответного слова
  if (MkoMsgResB[Index].MissSW)
    {
    INC_PARAM(MkoStressErrorCount);
    INC_PARAM(MkoStressNoOsErrorCount);
    continue;
    }

  // есть флаги в ОС
  if (MkoMsgResB[Index].FlagSW)
    {
    ErrorFlag = 0;
    Reply = MkoMsgResB[Index].SW;
    if (MKO_OS_OU_ERROR(Reply))  { ErrorFlag = 1; INC_PARAM(MkoStressTermErrorCount); }
    if (MKO_OS_AB_ERROR(Reply))  { ErrorFlag = 1; INC_PARAM(MkoStressAbErrorCount); }
    if (MKO_OS_BUSY(Reply))      { ErrorFlag = 1; INC_PARAM(MkoStressBusyErrorCount); }
    if (MKO_OS_MSG_ERROR(Reply)) { ErrorFlag = 1; INC_PARAM(MkoStressMsgErrorCount); }
    if (ErrorFlag) INC_PARAM(MkoStressErrorCount);
    continue;
    }
  } 
}
//================================================================================

void MkoStressCheckTxPacket2 (int MsgCount)
{
int Index, Reply, ErrorFlag;

// сохраняем результат для последнего пакета
if ((MkoStressShowLastPackResult) && (!MsgCount)) MkoStressSaveStateC();

// сохраняем результат для текущего пакета
if (MkoStressPackResCount < MKO_STRESS_PACK_LIST_SIZE) MkoStressPackResList[MkoStressPackResCount++] = MkoPackResC;

// перебираем все завершенные сообщения
for (Index = 0; Index <= MkoPackResC.nMsg; Index++)
  {
  INC_PARAM(MkoStressMsgCount);

  // сохраняем результат для сообщений
  if (MkoStressMsgResCount < MKO_STRESS_MSG_LIST_SIZE) MkoStressMsgResList[MkoStressMsgResCount++] = MkoMsgResC[Index];

  // ошибка формата передачи
  if (MkoMsgResC[Index].FmtError) 
    {
    INC_PARAM(MkoStressErrorCount);
    INC_PARAM(MkoStressLinkErrorCount);
    continue;
    }

  // нет ответного слова
  if (MkoMsgResC[Index].MissSW)
    {
    INC_PARAM(MkoStressErrorCount);
    INC_PARAM(MkoStressNoOsErrorCount);
    continue;
    }

  // есть флаги ошибок в ОС
  if (MkoMsgResC[Index].FlagSW)
    {
    ErrorFlag = 0;
    Reply = MkoMsgResC[Index].SW;
    if (MKO_OS_OU_ERROR(Reply))  { ErrorFlag = 1; INC_PARAM(MkoStressTermErrorCount); }
    if (MKO_OS_AB_ERROR(Reply))  { ErrorFlag = 1; INC_PARAM(MkoStressAbErrorCount); }
    if (MKO_OS_BUSY(Reply))      { ErrorFlag = 1; INC_PARAM(MkoStressBusyErrorCount); }
    if (MKO_OS_MSG_ERROR(Reply)) { ErrorFlag = 1; INC_PARAM(MkoStressMsgErrorCount); }
    if (ErrorFlag) INC_PARAM(MkoStressErrorCount);
    continue;
    }
  } 
}
//================================================================================

void MkoStressCheckRxPacket1 (void *AOrigData, int MsgCount)
{
int Index, Reply, ErrorFlag;

// сохраняем результат для последнего пакета
if ((MkoStressShowLastPackResult) && (!MsgCount)) MkoStressSaveStateB();

// сохраняем результат для текущего пакета
if (MkoStressPackResCount < MKO_STRESS_PACK_LIST_SIZE) MkoStressPackResList[MkoStressPackResCount++] = MkoPackResB;

// перебираем все завершенные сообщения
for (Index = 0; Index <= MkoPackResB.nMsg; Index++)
  {
  INC_PARAM(MkoStressMsgCount);

  // сохраняем результат для сообщений
  if (MkoStressMsgResCount < MKO_STRESS_MSG_LIST_SIZE) MkoStressMsgResList[MkoStressMsgResCount++] = MkoMsgResB[Index];

  // ошибка формата передачи
  if (MkoMsgResB[Index].FmtError) 
    {
    INC_PARAM(MkoStressErrorCount);
    INC_PARAM(MkoStressLinkErrorCount);
    continue;
    }

  // нет ответного слова
  if (MkoMsgResB[Index].MissSW)
    {
    INC_PARAM(MkoStressErrorCount);
    INC_PARAM(MkoStressNoOsErrorCount);
    continue;
    }

  // есть флаги в ОС
  if (MkoMsgResB[Index].FlagSW)
    {
    ErrorFlag = 0;
    Reply = MkoMsgResB[Index].SW;
    if (MKO_OS_OU_ERROR(Reply))  { ErrorFlag = 1; INC_PARAM(MkoStressTermErrorCount); }
    if (MKO_OS_AB_ERROR(Reply))  { ErrorFlag = 1; INC_PARAM(MkoStressAbErrorCount); }
    if (MKO_OS_BUSY(Reply))      { ErrorFlag = 1; INC_PARAM(MkoStressBusyErrorCount); }
    if (MKO_OS_MSG_ERROR(Reply)) { ErrorFlag = 1; INC_PARAM(MkoStressMsgErrorCount); }
    if (ErrorFlag) INC_PARAM(MkoStressErrorCount);
    continue;
    }

  // проверяем размер принятых данных
  if (MKO_CMD_SIZE(MkoMsgB[Index].CW) != MkoMsgResB[Index].Len)
    {
    INC_PARAM(MkoStressErrorCount);
    INC_PARAM(MkoStressDataErrorCount);
    continue;
    }

  // проверяем целостность принятых данных
  if (memcmp(AOrigData, &MkoBufB[Index], MkoMsgResB[Index].Len * sizeof(WORD))) 
    {
    INC_PARAM(MkoStressErrorCount);
    INC_PARAM(MkoStressDataErrorCount);
    continue;
    }
  } 
}
//================================================================================

void MkoStressCheckRxPacket2 (void *AOrigData, int MsgCount)
{
int Index, Reply, ErrorFlag;

// сохраняем результат для последнего пакета
if ((MkoStressShowLastPackResult) && (!MsgCount)) MkoStressSaveStateC();

// сохраняем результат для текущего пакета
if (MkoStressPackResCount < MKO_STRESS_PACK_LIST_SIZE) MkoStressPackResList[MkoStressPackResCount++] = MkoPackResC;

// перебираем все завершенные сообщения
for (Index = 0; Index <= MkoPackResC.nMsg; Index++)
  {
  INC_PARAM(MkoStressMsgCount);

  // сохраняем результат для сообщений
  if (MkoStressMsgResCount < MKO_STRESS_MSG_LIST_SIZE) MkoStressMsgResList[MkoStressMsgResCount++] = MkoMsgResC[Index];

  // ошибка формата передачи
  if (MkoMsgResC[Index].FmtError) 
    {
    INC_PARAM(MkoStressErrorCount);
    INC_PARAM(MkoStressLinkErrorCount);
    continue;
    }

  // нет ответного слова
  if (MkoMsgResC[Index].MissSW)
    {
    INC_PARAM(MkoStressErrorCount);
    INC_PARAM(MkoStressNoOsErrorCount);
    continue;
    }

  // есть флаги в ОС
  if (MkoMsgResC[Index].FlagSW)
    {
    ErrorFlag = 0;
    Reply = MkoMsgResC[Index].SW;
    if (MKO_OS_OU_ERROR(Reply))  { ErrorFlag = 1; INC_PARAM(MkoStressTermErrorCount); }
    if (MKO_OS_AB_ERROR(Reply))  { ErrorFlag = 1; INC_PARAM(MkoStressAbErrorCount); }
    if (MKO_OS_BUSY(Reply))      { ErrorFlag = 1; INC_PARAM(MkoStressBusyErrorCount); }
    if (MKO_OS_MSG_ERROR(Reply)) { ErrorFlag = 1; INC_PARAM(MkoStressMsgErrorCount); }
    if (ErrorFlag) INC_PARAM(MkoStressErrorCount);
    continue;
    }

  // проверяем размер принятых данных
  if (MKO_CMD_SIZE(MkoMsgC[Index].CW) != MkoMsgResC[Index].Len)
    {
    INC_PARAM(MkoStressErrorCount);
    INC_PARAM(MkoStressDataErrorCount);
    continue;
    }

  // проверяем целостность принятых данных
  if (memcmp(AOrigData, &MkoBufC[Index], MkoMsgResC[Index].Len * sizeof(WORD))) 
    {
    INC_PARAM(MkoStressErrorCount);
    INC_PARAM(MkoStressDataErrorCount);
    continue;
    }
  } 
}
//================================================================================

void MkoStressSaveStateB (void)
{
int Index;

MkoStressLastPackResult.PackDsc = MkoPackB;
MkoStressLastPackResult.PackRes = MkoPackResB;

for (Index = 0; Index < 32; Index++)
  {
  MkoStressLastPackResult.MsgDsc[Index] = MkoMsgB[Index];
  MkoStressLastPackResult.MsgRes[Index] = MkoMsgResB[Index];
  }
}
//================================================================================

void MkoStressSaveStateC (void)
{
int Index;

MkoStressLastPackResult.PackDsc = MkoPackC;
MkoStressLastPackResult.PackRes = MkoPackResC;

for (Index = 0; Index < 32; Index++)
  {
  MkoStressLastPackResult.MsgDsc[Index] = MkoMsgC[Index];
  MkoStressLastPackResult.MsgRes[Index] = MkoMsgResC[Index];
  }
}
//================================================================================

void MkoStressSendStatistic (int BaseMkoLine, int BaseMkoChan)
{
int Index;
WORD Buffer[32];

// mkoPack_t    = 2 x 32 = 4 x 16
// mkoPackRes_t = 1 x 32 = 2 x 16
// mkoPackMsg_t = 6 x 32 = 12 x 16
// mkoMSGRes_t  = 2 x 32 = 4 x 16

// mkoPack_t PackDsc;
// mkoPackRes_t PackRes;
// mkoPackMsg_t MsgDsc[32];
// mkoMsgRes_t MsgRes[32];

memmove(&Buffer[0], &MkoStressLastPackResult.PackDsc, sizeof(mkoPack_t));
Buffer[4] = 0xFFFF;
memmove(&Buffer[5], &MkoStressLastPackResult.PackRes, sizeof(mkoPackRes_t));

ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_DEBUG, KPA_SUBADDR_DEBUG, &Buffer, sizeof(WORD) * 7);

for (Index = 0; Index < 32; Index++)
  {
  memmove(&Buffer[0], &MkoStressLastPackResult.MsgDsc[Index], sizeof(mkoPackMsg_t));
  Buffer[12] = 0xFFFF;
  memmove(&Buffer[13], &MkoStressLastPackResult.MsgRes[Index], sizeof(mkoMsgRes_t));

  ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_DEBUG, KPA_SUBADDR_DEBUG, &Buffer, sizeof(WORD) * 17);
  }
}
//================================================================================

