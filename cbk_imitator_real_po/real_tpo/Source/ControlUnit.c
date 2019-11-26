
/*! \file ControlUnit.c
 * \author Гапонов Р.В.
 * \date 2012.10.28
 * \version 1.0.0
 * \brief Модуль содержит функции для поддержки непрерывного контроля устройств БА
 */

#include "MainUnit.h"
//================================================================================

/*

[таблицы данных для контроля OK]
таблица расположения OK: содержит расположение и размеры битовых полей OK
задается до компиляции, неизменяема

таблица номеров OK: содержит номера битовых полей OK
каждая запись в таблице относится к записи в таблице распроложения с тем же индексом.
задается до компиляции, неизменяема

таблица 1: список устройств, поставленных на контроль
содержимое: 0/1 - признак установки устройств на контроль
контроль: содержимое меняется по запросу КПА
char Control_DeviceList[32];

таблица 2: список указателей на параметры контроля
содержимое: указатель на список, содержащий данные по расположению битов OK для конкретного устройства
контроль: содержимое заполняется при инициализации
unsigned *Control_AddrList[32];

таблица 3: список указателей на номера OK
содержимое: указатель на список, содержащий номера OK
контроль: содержимое заполняется при инициализации
unsigned *Control_NumList[32];

таблица 4: список состояния OK
содержимое: текущее состояние контролируемых OK
контроль: содержимое меняется СПО при контроле ОК от устройств
char Control_StateList[1024];

таблица 5: список наличия данных в буфере OK
содержимое: 0 = данных в буфере нет, считывание еще не было произведено, 1 - данные по состоянию OK находятся в буфере
контроль: содержимое меняется СПО при установке устройств на контроль и считывании ОК от устройств
char Control_DataAvail[1024];

список, на который указывают элементы таблицы 2:
список unsigned, описывающий положение OK. если поля "адрес/подадрес/размер данных" не пусты (<>0), то требуется провести обмен с устройством.
если поля пусты, использовать данные, полученные в предыдущем обмене. если элемент ==0, список завершен.

списки требуют инициализации до начала работы:
таблица 1 - обнулить
таблица 2 - присвоить адреса списков положения OK
таблица 3 - присвоить адреса списков номеров OK
таблица 4 - обнулить
таблица 5 - неважно (сбрасывается при установке устройств на контроль)

адрес       5 (<< 0)  0..31
подадрес    5 (<< 5)  0..31
число слов  6 (<< 10) 1..32
номер слова 5 (<< 16) 0..31
номер бита  4 (<< 21) 0..15
размер поля 7 (<< 25) 1..16

#define OK_ADDR             0       // размер поля 5 бит, маска 0x1F
#define OK_ADDR_MASK        0x1F
#define OK_SUBADDR          5       // размер поля 5 бит, маска 0x1F
#define OK_SUBADDR_MASK     0x1F
#define OK_WORD_NUM         10      // размер поля 6 бит, маска 0x3F
#define OK_WORD_NUM_MASK    0x3F
#define OK_WORD_INDEX       16      // размер поля 5 бит, маска 0x1F
#define OK_WORD_INDEX_MASK  0x1F
#define OK_BIT_INDEX        21      // размер поля 4 бит, маска 0x0F
#define OK_BIT_INDEX_MASK   0x0F
#define OK_FIELD_LEN        25      // размер поля 7 бит, маска 0x7F
#define OK_FIELD_LEN_MASK   0x7F

#define OK_PLACE(Addr, SubAddr, DataSize, WordIndex, BitIndex, FieldLen) \
  ((((Addr) & OK_ADDR_MASK) << OK_ADDR) | \
   (((SubAddr) & OK_SUBADDR_MASK) << OK_SUBADDR) | \
   (((DataSize) & OK_WORD_NUM_MASK) << OK_WORD_NUM) | \
   (((WordIndex) & OK_WORD_INDEX_MASK) << OK_WORD_INDEX) | \
   (((BitIndex) & OK_BIT_INDEX_MASK) << OK_BIT_INDEX) | \
   (((FieldLen) & OK_FIELD_LEN_MASK) << OK_FIELD_LEN))

#define OK_END 0 

[вопросы]
считывать данные OK немедленно или по метке "1s" ? = по метке
скидивать в КПА все измененные OK или первый на контроле (возможно, для каждого класса отдельно) ? = все
снимать ли с контроля при изменении OK ? = нет
что делать если ошибка при считывании OK по метке "1s" ? = слать сообщение об ошибке для класса, перейти к следующему OK

*/
//================================================================================

char Control_DeviceList[DEVICES_COUNT];
unsigned *Control_AddrList[DEVICES_COUNT];
unsigned *Control_NumList[DEVICES_COUNT];
WORD Control_StateList[OK_COUNT];
char Control_DataAvail[OK_COUNT];

WORD Control_ChangedList[2 * OK_COUNT];
int Control_ChangedCount;
//================================================================================

unsigned Control_BitMap[] = 
{ 0, 
  0x0001, 0x0003, 0x0007, 0x000F, 
  0x001F, 0x003F, 0x007F, 0x00FF, 
  0x01FF, 0x03FF, 0x07FF, 0x0FFF, 
  0x1FFF, 0x3FFF, 0x7FFF, 0xFFFF };
//================================================================================

#ifdef SPO_KOI
#include "OKTable-Koi.c"
#endif

#ifdef SPO_ERTI
#include "OKTable-Erti.c"
#endif

unsigned Control_AddrList09[] = { OK_END };
unsigned Control_AddrList0A[] = { OK_END };
unsigned Control_AddrList0B[] = { OK_END };
unsigned Control_AddrList0C[] = { OK_END };
unsigned Control_AddrList0D[] = { OK_END };
unsigned Control_AddrList0E[] = { OK_END };
unsigned Control_AddrList0F[] = { OK_END };

unsigned Control_AddrList10[] = { OK_END };
unsigned Control_AddrList11[] = { OK_END };
unsigned Control_AddrList12[] = { OK_END };
unsigned Control_AddrList13[] = { OK_END };
unsigned Control_AddrList14[] = { OK_END };
unsigned Control_AddrList15[] = { OK_END };
unsigned Control_AddrList16[] = { OK_END };
unsigned Control_AddrList17[] = { OK_END };
unsigned Control_AddrList18[] = { OK_END };
unsigned Control_AddrList19[] = { OK_END };
unsigned Control_AddrList1A[] = { OK_END };
unsigned Control_AddrList1B[] = { OK_END };
unsigned Control_AddrList1C[] = { OK_END };
unsigned Control_AddrList1D[] = { OK_END };
unsigned Control_AddrList1E[] = { OK_END };
unsigned Control_AddrList1F[] = { OK_END };
//================================================================================

#ifdef SPO_KOI
#include "OKTableDesc-Koi.c"
#endif

#ifdef SPO_ERTI
#include "OKTableDesc-Erti.c"
#endif

unsigned Control_NumList09[] = { 0 };
unsigned Control_NumList0A[] = { 0 };
unsigned Control_NumList0B[] = { 0 };
unsigned Control_NumList0C[] = { 0 };
unsigned Control_NumList0D[] = { 0 };
unsigned Control_NumList0E[] = { 0 };
unsigned Control_NumList0F[] = { 0 };

unsigned Control_NumList10[] = { 0 };
unsigned Control_NumList11[] = { 0 };
unsigned Control_NumList12[] = { 0 };
unsigned Control_NumList13[] = { 0 };
unsigned Control_NumList14[] = { 0 };
unsigned Control_NumList15[] = { 0 };
unsigned Control_NumList16[] = { 0 };
unsigned Control_NumList17[] = { 0 };
unsigned Control_NumList18[] = { 0 };
unsigned Control_NumList19[] = { 0 };
unsigned Control_NumList1A[] = { 0 };
unsigned Control_NumList1B[] = { 0 };
unsigned Control_NumList1C[] = { 0 };
unsigned Control_NumList1D[] = { 0 };
unsigned Control_NumList1E[] = { 0 };
unsigned Control_NumList1F[] = { 0 };
//================================================================================

void ControlInitTables (void)
{
memset(&Control_DeviceList, 0, sizeof(Control_DeviceList));

Control_AddrList[0x00] = (unsigned*) &Control_AddrList00;
Control_AddrList[0x01] = (unsigned*) &Control_AddrList01;
Control_AddrList[0x02] = (unsigned*) &Control_AddrList02;
Control_AddrList[0x03] = (unsigned*) &Control_AddrList03;
Control_AddrList[0x04] = (unsigned*) &Control_AddrList04;
Control_AddrList[0x05] = (unsigned*) &Control_AddrList05;
Control_AddrList[0x06] = (unsigned*) &Control_AddrList06;
Control_AddrList[0x07] = (unsigned*) &Control_AddrList07;

Control_AddrList[0x08] = (unsigned*) &Control_AddrList08;
Control_AddrList[0x09] = (unsigned*) &Control_AddrList09;
Control_AddrList[0x0A] = (unsigned*) &Control_AddrList0A;
Control_AddrList[0x0B] = (unsigned*) &Control_AddrList0B;
Control_AddrList[0x0C] = (unsigned*) &Control_AddrList0C;
Control_AddrList[0x0D] = (unsigned*) &Control_AddrList0D;
Control_AddrList[0x0E] = (unsigned*) &Control_AddrList0E;
Control_AddrList[0x0F] = (unsigned*) &Control_AddrList0F;

Control_AddrList[0x10] = (unsigned*) &Control_AddrList10;
Control_AddrList[0x11] = (unsigned*) &Control_AddrList11;
Control_AddrList[0x12] = (unsigned*) &Control_AddrList12;
Control_AddrList[0x13] = (unsigned*) &Control_AddrList13;
Control_AddrList[0x14] = (unsigned*) &Control_AddrList14;
Control_AddrList[0x15] = (unsigned*) &Control_AddrList15;
Control_AddrList[0x16] = (unsigned*) &Control_AddrList16;
Control_AddrList[0x17] = (unsigned*) &Control_AddrList17;

Control_AddrList[0x18] = (unsigned*) &Control_AddrList18;
Control_AddrList[0x19] = (unsigned*) &Control_AddrList19;
Control_AddrList[0x1A] = (unsigned*) &Control_AddrList1A;
Control_AddrList[0x1B] = (unsigned*) &Control_AddrList1B;
Control_AddrList[0x1C] = (unsigned*) &Control_AddrList1C;
Control_AddrList[0x1D] = (unsigned*) &Control_AddrList1D;
Control_AddrList[0x1E] = (unsigned*) &Control_AddrList1E;
Control_AddrList[0x1F] = (unsigned*) &Control_AddrList1F;

Control_NumList[0x00] = (unsigned*) &Control_NumList00;
Control_NumList[0x01] = (unsigned*) &Control_NumList01;
Control_NumList[0x02] = (unsigned*) &Control_NumList02;
Control_NumList[0x03] = (unsigned*) &Control_NumList03;
Control_NumList[0x04] = (unsigned*) &Control_NumList04;
Control_NumList[0x05] = (unsigned*) &Control_NumList05;
Control_NumList[0x06] = (unsigned*) &Control_NumList06;
Control_NumList[0x07] = (unsigned*) &Control_NumList07;

Control_NumList[0x08] = (unsigned*) &Control_NumList08;
Control_NumList[0x09] = (unsigned*) &Control_NumList09;
Control_NumList[0x0A] = (unsigned*) &Control_NumList0A;
Control_NumList[0x0B] = (unsigned*) &Control_NumList0B;
Control_NumList[0x0C] = (unsigned*) &Control_NumList0C;
Control_NumList[0x0D] = (unsigned*) &Control_NumList0D;
Control_NumList[0x0E] = (unsigned*) &Control_NumList0E;
Control_NumList[0x0F] = (unsigned*) &Control_NumList0F;

Control_NumList[0x10] = (unsigned*) &Control_NumList10;
Control_NumList[0x11] = (unsigned*) &Control_NumList11;
Control_NumList[0x12] = (unsigned*) &Control_NumList12;
Control_NumList[0x13] = (unsigned*) &Control_NumList13;
Control_NumList[0x14] = (unsigned*) &Control_NumList14;
Control_NumList[0x15] = (unsigned*) &Control_NumList15;
Control_NumList[0x16] = (unsigned*) &Control_NumList16;
Control_NumList[0x17] = (unsigned*) &Control_NumList17;

Control_NumList[0x18] = (unsigned*) &Control_NumList18;
Control_NumList[0x19] = (unsigned*) &Control_NumList19;
Control_NumList[0x1A] = (unsigned*) &Control_NumList1A;
Control_NumList[0x1B] = (unsigned*) &Control_NumList1B;
Control_NumList[0x1C] = (unsigned*) &Control_NumList1C;
Control_NumList[0x1D] = (unsigned*) &Control_NumList1D;
Control_NumList[0x1E] = (unsigned*) &Control_NumList1E;
Control_NumList[0x1F] = (unsigned*) &Control_NumList1F;

memset(&Control_StateList, 0, sizeof(Control_StateList));
memset(&Control_DataAvail, 0, sizeof(Control_DataAvail));
}
//================================================================================

void ControlCheckList (void)
{
int Index;

for (Index = 0; Index < DEVICES_COUNT; Index++)
  {
  if (Control_DeviceList[Index])
    {
    ControlCheckDevice(Index);
    }
  }
}
//================================================================================

void ControlCheckDevice (int ADeviceIndex)
{
int Result, Index, TotalSize, RemainSize, DataSize;
int BaseMkoLine, BaseMkoChan, DevMkoLine, DevMkoChan;
unsigned *AddrList, *NumList, OKPlace, OKNum;
int AddrOU, SubOU, WNum, WIndex, BIndex, FLen, State;
WORD *DataBuffer, *DataSrc;
WORD Buffer[3];

// параметры линий обмена
BaseMkoLine = SpoConfig.KpaMkoLine & 1;
BaseMkoChan = SpoConfig.KpaMkoChan & 1;

DevMkoLine = SpoConfig.DevMkoLine & 1;
DevMkoChan = SpoConfig.DevMkoChan & 1;

// очищаем число измененных ОК
Control_ChangedCount = 0;

// получаем список параметров ОК
AddrList = Control_AddrList[ADeviceIndex];
NumList = Control_NumList[ADeviceIndex];

// цикл по всем ОК в классе
for (Index = 0; AddrList[Index]; Index++)
  {
  OKPlace = AddrList[Index];
  OKNum = NumList[Index];

  AddrOU = (OKPlace >> OK_ADDR) & OK_ADDR_MASK;
  SubOU  = (OKPlace >> OK_SUBADDR) & OK_SUBADDR_MASK;
  WNum   = (OKPlace >> OK_WORD_NUM) & OK_WORD_NUM_MASK;
  WIndex = (OKPlace >> OK_WORD_INDEX) & OK_WORD_INDEX_MASK;
  BIndex = (OKPlace >> OK_BIT_INDEX) & OK_BIT_INDEX_MASK;
  FLen   = (OKPlace >> OK_FIELD_LEN) & OK_FIELD_LEN_MASK;

  if ((FLen < 1) || (FLen > 16) || ((BIndex + FLen) > 16))
    {
    Buffer[0] = ADeviceIndex; // индекс устройства, в данных которого есть ошибки
    Buffer[1] = Index; // индекс OK, в данных которого есть ошибки

    memmove(&MkoBufA[0][0], &Buffer[0], 4);
    MkoXchgAn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_MAIN + 1, MKO_TX, 2), 0);

    MkoBufA[0][0] = REPLY_INTERNAL_DATA_ERROR; // код ошибки "ошибка в таблицах СПО"
    MkoBufA[0][1] = 4;
    MkoBufA[0][2] = CalcCrc16(&Buffer, 4);
    MkoXchgAn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_MAIN, MKO_TX, 3), 0);
    return;
    }

  // поля адреса непустые - требуется произвести обмен с аппаратурой
  if (AddrOU + SubOU + WNum)
    {
    // чтение OK с абонента
    Result = MkoXchgAn(DevMkoLine, DevMkoChan, MKO_CMD(AddrOU, SubOU, MKO_RX, WNum), 0);
    if (Result)
      {
      Buffer[0] = ADeviceIndex; // индекс устройства, при обмене с которым возникли ошибки
      Buffer[1] = Index; // индекс OK, при считывании которого возникли ошибки
      Buffer[2] = DECODE_XCHG_ERROR(Result); // информация об ошибке

      memmove(&MkoBufA[0][0], &Buffer[0], 6);
      MkoXchgAn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_MAIN + 1, MKO_TX, 3), 0);

      MkoBufA[0][0] = REPLY_OK_READ_ERROR; // код ошибки "сбой чтения ОК"
      MkoBufA[0][1] = 6;
      MkoBufA[0][2] = CalcCrc16(&Buffer, 6);
      MkoXchgAn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_MAIN, MKO_TX, 3), 0);
      return;
      }
    }

  // получаем из буфера обмена битовое поле ОК
  State = (MkoBufA[0][WIndex] >> BIndex) & Control_BitMap[FLen];

  if (!Control_DataAvail[OKNum])
    {
    // начальное считывание ОК, сохранение состояния
    Control_DataAvail[OKNum] = 1;
    Control_StateList[OKNum] = State;
    }
  else
    {
    // повторное считывание ОК, проверка состояния
    if (State != Control_StateList[OKNum])
      {
      // бит ОК изменен - сохраняем новое значение и добавляем номер в список измененных
      Control_StateList[OKNum] = State;
      Control_ChangedList[Control_ChangedCount++] = OKNum;
      Control_ChangedList[Control_ChangedCount++] = State;
      }
    }
  }

if (Control_ChangedCount)
  {
  // передача списка данных по изменившимся ОК

  // источник/размер данных
  DataBuffer = (WORD*) &Control_ChangedList;
  TotalSize = Control_ChangedCount * 2;

  RemainSize = TotalSize / 2;
  DataSrc = DataBuffer;

  while (RemainSize)
    {
    DataSize = 0;
    while (RemainSize)
      {
      RemainSize--;
      MkoBufA[0][DataSize++] = *DataSrc++;
      if (DataSize >= 32) break;
      }
  
    Result = MkoXchgAn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_MAIN + 1, MKO_TX, DataSize), 0);
    if (Result)
      {
      Buffer[0] = DECODE_XCHG_ERROR(Result); // информация об ошибке

      memmove(&MkoBufA[0][0], &Buffer[0], 2);
      MkoXchgAn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_MAIN + 1, MKO_TX, 1), 0);

      MkoBufA[0][0] = REPLY_OK_SEND_ERROR; // код ошибки "сбой передачи ОК"
      MkoBufA[0][1] = 2;
      MkoBufA[0][2] = CalcCrc16(&Buffer, 2);
      MkoXchgAn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_MAIN, MKO_TX, 3), 0);
      return;
      }
    }

  // отправка сообщения о выполнении команды + CRC + размер данных в байтах
  MkoBufA[0][0] = REPLY_OK_CHANGE; // код сообщения "список изменившихся ОК"
  MkoBufA[0][1] = TotalSize; // общий размер списка в байтах
  MkoBufA[0][2] = CalcCrc16(DataBuffer, TotalSize); // Crc16 для списка данных
  MkoXchgAn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_MAIN, MKO_TX, 3), 0);
  }
}
//================================================================================

// установка/снятие флагов "первоначальное считывание ОК"
// вход: индекс класса устройств, флаг установки/снятия

void ControlResetDeviceState (int ADeviceIndex)
{
int Index;
unsigned *AddrList, *NumList, OKNum;

// получаем список параметров ОК
AddrList = Control_AddrList[ADeviceIndex];
NumList = Control_NumList[ADeviceIndex];

for (Index = 0; AddrList[Index]; Index++)
  {
  OKNum = NumList[Index];
  Control_DataAvail[OKNum] = 0;
  }
}
//================================================================================

// расчитывает номер устройства и индекс в таблице данных устройства по указанному номеру ОК
// возвращает номер устройства (0..31), для неизвестного номера ОК возвращает -1
// если переданный как параметр указатель не пустой, записывает локальный индекс в таблице данных устройства

int ControlOkNumberToDevice (int ANumber, int *ALocalIndex)
{
int DeviceFound, DeviceIndex, Index;
unsigned *AddrList, *NumList;

DeviceFound = -1;

// перебираем все устройства
for (DeviceIndex = 0; DeviceIndex < DEVICES_COUNT; DeviceIndex++)
  {
  // получаем список параметров ОК
  AddrList = Control_AddrList[DeviceIndex];
  NumList = Control_NumList[DeviceIndex];

  for (Index = 0; AddrList[Index]; Index++)
    {
    if (NumList[Index] == ANumber) 
      { 
      DeviceFound = DeviceIndex; 
      if (ALocalIndex) *ALocalIndex = Index;
      break; 
      }
    }

  if (DeviceFound != -1) break;
  }

return DeviceFound;
}
//================================================================================

