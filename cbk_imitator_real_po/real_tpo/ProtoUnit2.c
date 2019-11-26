
3. установка запрещенных сообщений на МКО

[Data]
#define PROHIBITED_MKO_MSG_COUNT 256

typedef struct
{
WORD MkoCmd;		// команда абоненту МКО
WORD Data[32];		// буфер под данные абонента
WORD Mask[32];		// буфер под маску данных абонента
} TProhibitedMkoMsg;

int ProhibitedMkoMsgCount;
TProhibitedMkoMsg ProhibitedMkoMsgList[PROHIBITED_MKO_MSG_COUNT];

[CmdHandler]
// [номера слов] (option: [номера битов])

// данные на подадресе 1:
// [0][7..0] - код команды, = 0x61
// [0][15..8] - код операции, = 1..4

// код операции = 1, добавление сообщения в список запрещенных сообщений для МКО
// [1][15..0] - добавляемая запрещенная команда для МКО
// если добавляется сообщение на запись абонента:
// данные на подадресе 2:
// [n..0] - запрещенные данные
// данные на подадресе 3:
// [n..0] - маска запрещенных данных (установленные биты отмечают используемые данные)
// возврат, результат: OK/ошибка
// возврат, данные: нет

// код операции = 2, удаление сообщения из списка запрещенных сообщений для МКО
// [1][15..0] - удаляемая запрещенная команда для МКО
// если удаляется сообщение на запись абонента:
// данные на подадресе 2:
// [n..0] - запрещенные данные
// данные на подадресе 3:
// [n..0] - маска запрещенных данных (установленные биты отмечают используемые данные)
// возврат, результат: OK/ошибка
// возврат, данные: нет

// код операции = 3, получить количество сообщений в списке запрещенных сообщений МКО
// возврат, результат: OK
// возврат, данные: 1 слово, = число сообщений в списке

// код операции = 4, очистить список запрещенных сообщений МКО
// возврат, результат: OK
// возврат, данные: нет

void KpaCommand61 (int BaseMkoLine, int BaseMkoChan)
{
int Opcode, Result;

// читаем код операции
Opcode = KpaCommand[0] >> 8;

// добавление сообщения в список запрещенных сообщений для МКО
if (Opcode == 0x01)
  {
  Result = AddProhibitedMkoMsg(BaseMkoLine, BaseMkoChan);
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, Result, KPA_SUBADDR_IRQ);
  }
// удаление сообщения из списка запрещенных сообщений для МКО
else if (Opcode == 0x02)
  {
  Result = DelProhibitedMkoMsg(BaseMkoLine, BaseMkoChan);
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, Result, KPA_SUBADDR_IRQ);
  }
// получить количество сообщений в списке запрещенных сообщений МКО
else if (Opcode == 0x03)
  {
  Result = GetProhibitedMkoMsgCount();
  ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ, &Result, 2);
  }
// очистить список запрещенных сообщений МКО
else if (Opcode == 0x04)
  {
  Result = ClrProhibitedMkoMsgList();
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

int AddProhibitedMkoMsg (int BaseMkoLine, int BaseMkoChan)
{
int Result, Index;
WORD MkoCmd, Cmd, Data[32], Mask[32];

// описание добавляемого запрещенного сообщения
MkoCmd = KpaCommand[1];

// проверка параметров на допустимость
if (ProhibitedMkoMsgCount >= PROHIBITED_MKO_MSG_COUNT) return REPLY_CMD_OVERFLOW_ERROR;
if (MKO_CMD_ADDR(MkoCmd) == KPA_ADDR) return REPLY_CMD_DATA_ERROR;

// читаем данные для режима передачи
if (MKO_CMD_DIR(MkoCmd) == MKO_TX)
  {
  Cmd = MKO_CMD(KPA_ADDR, KPA_SUBADDR_CMD + 1, MKO_RX, MKO_CMD_SIZE(MkoCmd));
  Result = ReadProhibitedMkoMsgData(BaseMkoLine, BaseMkoChan, Cmd, KpaCommand[7], &Data);
  if (Result != REPLY_CMD_DONE) return Result;

  Cmd = MKO_CMD(KPA_ADDR, KPA_SUBADDR_CMD + 2, MKO_RX, MKO_CMD_SIZE(MkoCmd));
  Result = ReadProhibitedMkoMsgData(BaseMkoLine, BaseMkoChan, Cmd, KpaCommand[5], &Mask);
  if (Result != REPLY_CMD_DONE) return Result;
  }

// ищем запрещенное сообщение в списке
if (FindProhibitedMkoMsg(MkoCmd, &Data, &Mask, &Index)) return REPLY_ALREADY_SET_ERROR;

// добавление запрещенного сообщения в список
Index = ProhibitedMkoMsgCount;
ProhibitedMkoMsgList[Index].MkoCmd = MkoCmd;

if (MKO_CMD_DIR(MkoCmd) == MKO_TX)
  {
  memcpy(&ProhibitedMkoMsgList[Index].Data, &Data, MKO_CMD_SIZE(MkoCmd) * sizeof(WORD));
  memcpy(&ProhibitedMkoMsgList[Index].Mask, &Mask, MKO_CMD_SIZE(MkoCmd) * sizeof(WORD));
  }

ProhibitedMkoMsgCount++;
return REPLY_CMD_DONE;
}
//================================================================================

int DelProhibitedMkoMsg (int BaseMkoLine, int BaseMkoChan)
{
int Result, Index;
WORD MkoCmd, Crc16, Data[32], Mask[32];

// описание удаляемого запрещенного сообщения
MkoCmd = KpaCommand[1];

// читаем данные для режима передачи
if (MKO_CMD_DIR(MkoCmd) == MKO_TX)
  {
  Cmd = MKO_CMD(KPA_ADDR, KPA_SUBADDR_CMD + 1, MKO_RX, MKO_CMD_SIZE(MkoCmd));
  Result = ReadProhibitedMkoMsgData(BaseMkoLine, BaseMkoChan, Cmd, KpaCommand[7], &Data);
  if (Result != REPLY_CMD_DONE) return Result;

  Cmd = MKO_CMD(KPA_ADDR, KPA_SUBADDR_CMD + 2, MKO_RX, MKO_CMD_SIZE(MkoCmd));
  Result = ReadProhibitedMkoMsgData(BaseMkoLine, BaseMkoChan, Cmd, KpaCommand[5], &Mask);
  if (Result != REPLY_CMD_DONE) return Result;
  }

// ищем сообщение в списке
if (!FindProhibitedMkoMsg(MkoCmd, &Data, &Mask, &Index)) return REPLY_ITEM_NOT_FOUND;

// удаляем сообщение из списка
for ( ; Index < (ProhibitedMkoMsgCount - 1); Index++)
  {
  ProhibitedMkoMsgList[Index] = ProhibitedMkoMsgList[Index + 1];
  }

ProhibitedMkoMsgCount--;
return REPLY_CMD_DONE;
}
//================================================================================

int ReadProhibitedMkoMsgData (int BaseMkoLine, int BaseMkoChan, WORD Cmd, WORD DataCrc, WORD *Buffer)
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

int FindProhibitedMkoMsg (WORD MkoCmd, WORD *Data, WORD *Mask, int *ItemIndex)
{
int Index, Found;

for (Index = 0, Found = 0; Index < ProhibitedMkoMsgCount; Index++)
  {
  if (ProhibitedMkoMsgList[Index].MkoCmd != MkoCmd) continue;

  if (MKO_CMD_DIR(MkoCmd) == MKO_TX)
    {
    if (ProhibitedMkoMsgDataCmp(Data, &ProhibitedMkoMsgList[Index].Data, MKO_CMD_SIZE(MkoCmd))) continue;
    if (ProhibitedMkoMsgDataCmp(Mask, &ProhibitedMkoMsgList[Index].Mask, MKO_CMD_SIZE(MkoCmd))) continue;
    }
                               
  *ItemIndex = Index;          
  Found = 1;                   
  break;
  }

return Found;
}
//================================================================================

int ProhibitedMkoMsgDataCmp (WORD *Data1, WORD *Data2, int Count)
{
while (Count--) { if (*Data1++ != *Data2++) return 1; }
return 0;
}
//================================================================================

int GetProhibitedMkoMsgCount (void)
{
return ProhibitedMkoMsgCount;
}
//================================================================================

int ClrProhibitedMkoMsgList (void)
{
ProhibitedMkoMsgCount = 0;
return REPLY_CMD_DONE;
}
//================================================================================

[Init]
void InitProhibitedMkoMsg (void)
{
ProhibitedMkoMsgCount = 0;
}
//================================================================================

[Main]
int MkoXchgAn (int MkoLine, int MkoChan, WORD MkoCmd, int F4Mode)
{
...                                             

if (!F4Mode) && (IsProhibitedMkoMsg(MkoCmd, &MkoBufA[0][0])) return -5;

...
}
//================================================================================

int IsProhibitedMkoMsg (WORD MkoCmd, WORD *Data)
{
int Result, Count1, Count2;
DWORD Crc1, Crc2;

if (!ProhibitedMkoMsgCount) return 0;

do
  {
  Count1 = ProhibitedMkoMsgCount;
  Crc1 = CalcCrc32(&ProhibitedMkoMsgList, Count1 * sizeof(TProhibitedMkoMsg));

  Result = CheckProhibitedMkoMsg(MkoCmd, Data);

  Count2 = ProhibitedMkoMsgCount;
  Crc2 = CalcCrc32(&ProhibitedMkoMsgList, Count2 * sizeof(TProhibitedMkoMsg));
  }
while ((Count1 != Count2) || (Crc1 != Crc2));

return Result;
}
//================================================================================

int CheckProhibitedMkoMsg (WORD MkoCmd, WORD *Data)
{
int ListIndex, DataIndex, Found, Count, Diff;
WORD *OrigData, *OrigMask, Mask;

for (ListIndex = 0, Found = 0; ListIndex < ProhibitedMkoMsgCount; ListIndex++)
  {
  if (ProhibitedMkoMsgList[ListIndex].MkoCmd != MkoCmd) continue;

  if (MKO_CMD_DIR(MkoCmd) == MKO_TX)
    {
    OrigData = &ProhibitedMkoMsgList[ListIndex].Data;
    OrigMask = &ProhibitedMkoMsgList[ListIndex].Mask;
    Count = MKO_CMD_SIZE(MkoCmd);

    for (DataIndex = 0, Diff = 0; DataIndex < Count; DataIndex++)
      {
      if ((Mask = OrigMask[DataIndex]) == 0) continue;
      if ((Data[DataIndex] & Mask) != (OrigData[DataIndex] & Mask)) { Diff = 1; break; }
      }
    if (Diff) continue;
    }

  Found = 1;                   
  break;
  }

return Found;
}
//================================================================================

