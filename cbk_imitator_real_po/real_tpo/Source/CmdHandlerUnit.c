
/*! \file CmdHandlerUnit.c
 * \author Гапонов Р.В.
 * \date 2012.10.28
 * \version 1.0.0
 * \brief Модуль содержит функции обработки команд КПА
 */

#include "MainUnit.h"
#include "BaloiUnit_v11.h"
#include "MkoMirrorUnit.h"
#include "VersionUnit.h"
//================================================================================

#if !defined (OPO_V031) && !defined (OPO_V033)
  #error Не определена версия ОПО ЦБК!
#endif
//================================================================================

#define MAKE_LONG(Lo, Hi) ((((unsigned)(Hi)) << 16) | ((unsigned)(Lo)))
//================================================================================

#define SEND_RESULT(Result) \
  { \
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, Result, KPA_SUBADDR_IRQ); \
  return; \
  }
//================================================================================

/*! \fn void KpaCommandUnknown (int BaseMkoLine, int BaseMkoChan);
 * \brief Функция вызывается если СПО получает неизвестную команду (отсутствующую в таблице команд). Вызывается при обработке прерывания от КПА
 * \param [in] BaseMkoLine - номер линии МКО для обмена с КПА
 * \param [in] BaseMkoChan - номер канала МКО для обмена с КПА
 * \details Передает полученную команду обратно на КПА с кодом сообщения "неизвестная команда".
 */

void KpaCommandUnknown (int BaseMkoLine, int BaseMkoChan)
{
MSGS("KpaCommandUnknown(): Получена неизвестная команда = %d (= 0x%02x)", Command, Command);

ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_CMD_UNKNOWN_ERROR, KPA_SUBADDR_IRQ, &KpaCommand, sizeof(KpaCommand));
}
//================================================================================

/*! \fn void KpaCommand01 (int BaseMkoLine, int BaseMkoChan);
 * \brief Функция обработки команды 0x01: установка маски прерываний. Вызывается при обработке прерывания от КПА
 * \param [in] BaseMkoLine - номер линии МКО для обмена с КПА
 * \param [in] BaseMkoChan - номер канала МКО для обмена с КПА
 * \details <b> Формат данных команды: </b><br>
 * слово команды [0] биты [7..0] - код команды (0x01) <br>
 * слово команды [1] - 16бит маска разрешенных прерываний, где биты установленные в 1 разрешают обработку прерываний от БА <br>
 * <b> Работа команды: </b><br>
 * в полученной от КПА маске прерываний принудительно выставляется бит 1 (прерывания от КПА), затем
 * производится вызов функции ОПО для установки новой маски разрешенных прерываний от БА
 */

void KpaCommand01 (int BaseMkoLine, int BaseMkoChan)
{
WORD Mask;

// новая маска прерываний + разрешение прерываний от КПА
Mask = KpaCommand[1] | KPA_IRQ_MASK;

// установка новой маски прерываний
SpoConfig.EnabledIrqMask = Mask;
sprIMaskVec(~Mask);

// отправка сообщения о выполнении команды
ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ);
}
//================================================================================

/*! \fn void KpaCommand02 (int BaseMkoLine, int BaseMkoChan);
 * \brief Функция обработки команды 0x02: запустить/остановить циклический контроль для устройства. Вызывается при обработке прерывания от КПА
 * \param [in] BaseMkoLine - номер линии МКО для обмена с КПА
 * \param [in] BaseMkoChan - номер канала МКО для обмена с КПА
 * \details <b> Формат данных команды: </b><br>
 * слово команды [0] биты [7..0] - код команды (0x02) <br>
 * слово команды [1] биты [0..14] - номер класса устройств для контроля (0..31) <br>
 * слово команды [1] бит [15] - признак установить/снять контроль (1 = установить, 0 = снять) <br>
 * <b> Работа команды: </b><br>
 * производится проверка индекса устройства на допустимость <br>
 * производится проверка что запрошенный режим контроля устройства не равен текущему режиму <br>
 * производится смена режима контроля устройства <br>
 * если устройство ставится на контроль - производится установка начального состояния для контроля устройства <br>
 */

void KpaCommand02 (int BaseMkoLine, int BaseMkoChan)
{
int Mode, Index;

Mode = (KpaCommand[1] >> 15) & 0x01;
Index = KpaCommand[1] & 0x7FFF;

// проверяем индекс класса устройств для контроля
if ((Index < 0) || (Index > (DEVICES_COUNT - 1)))
  {
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DATA_ERROR, KPA_SUBADDR_IRQ);
  return;
  }

if (((Mode) && (Control_DeviceList[Index])) || 
    ((!Mode) && (!Control_DeviceList[Index])))
  {
  // если изменения не требуются - отправка сообщения о выполнении команды
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ);
  return;
  }

Control_DeviceList[Index] = Mode;
if (Mode) ControlResetDeviceState(Index);

// отправка сообщения о выполнении команды
ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ);
}
//================================================================================

/*! \fn void KpaCommand03 (int BaseMkoLine, int BaseMkoChan);
 * \brief Функция обработки команды 0x03: запись данных в абонент на МКО (в формате Ф1). Вызывается при обработке прерывания от КПА
 * \param [in] BaseMkoLine - номер линии МКО для обмена с КПА
 * \param [in] BaseMkoChan - номер канала МКО для обмена с КПА
 * \details <b> Формат данных команды: </b><br>
 * слово команды [0] биты [7..0] - код команды (0x03) <br>
 * слово команды [0] биты [13..8] - число слов данных для передачи в абонент (1..32) <br>
 * слово команды [1] биты [4..0] - адрес абонента на МКО (0..31) <br>
 * слово команды [1] биты [9..5] - подадрес абонента на МКО (0..31) <br>
 * слово команды [1] бит [10] - линия МКО для обмена с абонентом (0 = первая линия, 1 = вторая линия) <br>
 * слово команды [1] бит [11] - канал МКО для обмена с абонентом (0 = основной канал, 1 = резервный канал) <br>
 * <b> Работа команды: </b><br>
 * производится проверка параметров команды на допустимость <br>
 * производится считывание данных для записи в абонент с п/а 2 КПА и их проверка <br>
 * производится запись данных в указанный абонент на МКО <br>
 */

void KpaCommand03 (int BaseMkoLine, int BaseMkoChan)
{
int Result, AddrOU, SubAddrOU, WordCount, MkoLine, MkoChan;
WORD Crc16;

WordCount = (KpaCommand[0] >> 8) & 0x3F;   // размер данных - слово [0], биты 8..13
AddrOU    = (KpaCommand[1] & 0x1F);        // адрес ОУ - слово [1], биты 0..4
SubAddrOU = (KpaCommand[1] >> 5) & 0x1F;   // подадрес ОУ - слово [1], биты 5..9
MkoLine   = (KpaCommand[1] >> 10) & 0x01;  // линия МКО - слово [1], бит 10
MkoChan   = (KpaCommand[1] >> 11) & 0x01;  // канал МКО - слово [1], бит 11

// ошибка в параметрах команды
if ((WordCount < 1) || (WordCount > 32))
  {
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DATA_ERROR, KPA_SUBADDR_IRQ);
  return;
  }

// считываем дополнительные данные для записи в абонент
Result = MkoXchgBn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_CMD + 1, MKO_RX, WordCount), 0);
if (Result)
  {
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, DECODE_XCHG_ERROR(Result), KPA_SUBADDR_IRQ);
  return;
  }

// ошибка в размере принятых данных
if (MkoMsgResB[0].Len != WordCount)
  {
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_NO_DATA_ERROR, KPA_SUBADDR_IRQ);
  return;
  }

// проверка целостности данных
Crc16 = CalcCrc16(&MkoBufB[0][0], WordCount * sizeof(WORD));
if (Crc16 != KpaCommand[7])
  {
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_DATA_CRC_ERROR, KPA_SUBADDR_IRQ);
  return;
  }

// запись данных в абонент
Result = MkoXchgBn(MkoLine, MkoChan, MKO_CMD(AddrOU, SubAddrOU, MKO_TX, WordCount), 0);
if (Result)
  {
  if ((Result == -3) && (SpoConfig.MkoWriteAgainIfBusyFlag))
    {
    if (SpoConfig.MkoWriteAgainIfBusyInt) SleepMSec(SpoConfig.MkoWriteAgainIfBusyInt); 
    Result = MkoXchgBn(MkoLine, MkoChan, MKO_CMD(AddrOU, SubAddrOU, MKO_TX, WordCount), 0);
    }
  else if (SpoConfig.MkoWriteAgainIfErrorFlag)
    {
    if (SpoConfig.MkoWriteAgainIfErrorInt) SleepMSec(SpoConfig.MkoWriteAgainIfErrorInt); 
    Result = MkoXchgBn(MkoLine, MkoChan, MKO_CMD(AddrOU, SubAddrOU, MKO_TX, WordCount), 0);
    }
  }
SpoConfig.MkoWriteAgainIfBusyFlag = 0;
SpoConfig.MkoWriteAgainIfErrorFlag = 0;

if (Result)
  {
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, DECODE_XCHG_ERROR(Result), KPA_SUBADDR_IRQ);
  return;
  }

// отправка сообщения о выполнении команды
ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ);
}
//================================================================================

/*! \fn void KpaCommand04 (int BaseMkoLine, int BaseMkoChan);
 * \brief Функция обработки команды 0x04: чтение данных с абонента (в формате Ф2). Вызывается при обработке прерывания от КПА
 * \param [in] BaseMkoLine - номер линии МКО для обмена с КПА
 * \param [in] BaseMkoChan - номер канала МКО для обмена с КПА
 * \details <b> Формат данных команды: </b><br>
 * слово команды [0] биты [7..0] - код команды (0x04) <br>
 * слово команды [0] биты [13..8] - число слов данных для чтения с абонента (1..32) <br>
 * слово команды [1] биты [4..0] - адрес абонента на МКО (0..31) <br>
 * слово команды [1] биты [9..5] - подадрес абонента на МКО (0..31) <br>
 * слово команды [1] бит [10] - линия МКО для обмена с абонентом (0..1) <br>
 * слово команды [1] бит [11] - канал МКО для обмена с абонентом (0..1) <br>
 * <b> Работа команды: </b><br>
 * производится проверка параметров команды на допустимость <br>
 * производится считывание данных с абонента в буфер СПО <br>
 * производится передача результата выполнения и считанных с абонента данных на КПА <br>
 */

void KpaCommand04 (int BaseMkoLine, int BaseMkoChan)
{
int Result, AddrOU, SubAddrOU, WordCount, MkoLine, MkoChan;
WORD Buffer[32];

WordCount = (KpaCommand[0] >> 8) & 0x3F;   // размер данных - слово [0], биты 8..13
AddrOU    = (KpaCommand[1] & 0x1F);        // адрес ОУ - слово [1], биты 0..4
SubAddrOU = (KpaCommand[1] >> 5) & 0x1F;   // подадрес ОУ - слово [1], биты 5..9
MkoLine   = (KpaCommand[1] >> 10) & 0x01;  // линия МКО - слово [1], бит 10
MkoChan   = (KpaCommand[1] >> 11) & 0x01;  // канал МКО - слово [1], бит 11

// ошибка в параметрах команды
if ((WordCount < 1) || (WordCount > 32))
  {
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DATA_ERROR, KPA_SUBADDR_IRQ);
  return;
  }

// чтение данных с абонента
Result = MkoXchgBn(MkoLine, MkoChan, MKO_CMD(AddrOU, SubAddrOU, MKO_RX, WordCount), 0);
if (Result)
  {
  if ((Result == -3) && (SpoConfig.MkoReadAgainIfBusyFlag))
    {
    if (SpoConfig.MkoReadAgainIfBusyInt) SleepMSec(SpoConfig.MkoReadAgainIfBusyInt); 
    Result = MkoXchgBn(MkoLine, MkoChan, MKO_CMD(AddrOU, SubAddrOU, MKO_RX, WordCount), 0);
    }
  else if (SpoConfig.MkoReadAgainIfErrorFlag)
    {
    if (SpoConfig.MkoReadAgainIfErrorInt) SleepMSec(SpoConfig.MkoReadAgainIfErrorInt); 
    Result = MkoXchgBn(MkoLine, MkoChan, MKO_CMD(AddrOU, SubAddrOU, MKO_RX, WordCount), 0);
    }
  }
SpoConfig.MkoReadAgainIfBusyFlag = 0;
SpoConfig.MkoReadAgainIfErrorFlag = 0;

if (Result)
  {
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, DECODE_XCHG_ERROR(Result), KPA_SUBADDR_IRQ);
  return;
  }

// переносим данные в локальный буфер
memmove(&Buffer, &MkoBufB[0][0], sizeof(Buffer));

// отправка данных и сообщения о выполнении команды
ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ, &Buffer, WordCount * sizeof(WORD));
}
//================================================================================

/*! \fn void KpaCommand05 (int BaseMkoLine, int BaseMkoChan);
 * \brief Функция обработки команды 0x05: запись блока данных в ОЗУ СПО. Вызывается при обработке прерывания от КПА
 * \param [in] BaseMkoLine - номер линии МКО для обмена с КПА
 * \param [in] BaseMkoChan - номер канала МКО для обмена с КПА
 * \details <b> Формат данных команды: </b><br>
 * слово команды [0] биты [7..0] - код команды (0x05) <br>
 * слово команды [1] биты [7..0] - размер данных для записи (1..64), байт <br>
 * слово команды [1] бит [8] - проверять ли данные после записи (0 - нет, 1 - да) <br>
 * слово команды [2] - мл.16 бит адреса для записи <br>
 * слово команды [3] - ст.16 бит адреса для записи <br>
 * <b> Работа команды: </b><br>
 * производится проверка параметров команды на допустимость <br>
 * производится считывание данных для записи в ОЗУ с п/а 2 КПА и их проверка <br>
 * производится запись данных в ОЗУ СПО, если требуется проверка - производится обратное считывание данных и проверка <br>
 * производится передача результата выполнения на КПА <br>
 */

void KpaCommand05 (int BaseMkoLine, int BaseMkoChan)
{
int Result;
UINT DataSize, DataAddr, Verify, Index, WordCount;
WORD Crc16, Buffer[32];
BYTE *Src, *Dst;

DataSize = KpaCommand[1] & 0x00FF;
Verify = KpaCommand[1] & 0x0100;
memmove(&DataAddr, &KpaCommand[2], 4);

// ошибка в параметрах команды
if ((DataSize < 1) || (DataSize > 64) || (!DataAddr))
  {
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DATA_ERROR, KPA_SUBADDR_IRQ);
  return;
  }

// считываем данные для записи в ОЗУ
WordCount = (DataSize + 1) / 2;
Result = MkoXchgBn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_CMD + 1, MKO_RX, WordCount), 0);
if (Result)
  {
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, DECODE_XCHG_ERROR(Result), KPA_SUBADDR_IRQ);
  return;
  }

// ошибка в размере принятых данных
if (MkoMsgResB[0].Len != WordCount)
  {
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_NO_DATA_ERROR, KPA_SUBADDR_IRQ);
  return;
  }

// переносим данные в локальный буфер
memmove(&Buffer, &MkoBufB[0][0], sizeof(Buffer));

// проверка целостности данных
Crc16 = CalcCrc16(&Buffer, DataSize);
if (Crc16 != KpaCommand[7])
  {
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_DATA_CRC_ERROR, KPA_SUBADDR_IRQ);
  return;
  }

Src = (BYTE*) &Buffer;
Dst = (BYTE*) DataAddr;

// запись данных в ОЗУ
for (Index = 0; Index < DataSize; Index++)
  {
  Dst[Index] = Src[Index];
  }

if (Verify)
  {
  // проверка данных в ОЗУ
  for (Index = 0; Index < DataSize; Index++) 
    {
    if (Dst[Index] != Src[Index])
      {
      ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_RAM_WR_ERROR, KPA_SUBADDR_IRQ);
      return;
      }
    }
  }

// запись успешно завершена
ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ);
}
//================================================================================

/*! \fn void KpaCommand06 (int BaseMkoLine, int BaseMkoChan);
 * \brief Функция обработки команды 0x06: чтение блока данных из ОЗУ СПО. Вызывается при обработке прерывания от КПА
 * \param [in] BaseMkoLine - номер линии МКО для обмена с КПА
 * \param [in] BaseMkoChan - номер канала МКО для обмена с КПА
 * \details <b> Формат данных команды: </b><br>
 * слово команды [0] биты [7..0] - код команды (0x06) <br>
 * слово команды [1] биты [7..0] - размер данных для считывания (1..64), байт <br>
 * слово команды [2] - мл.16 бит адреса для считывания <br>
 * слово команды [3] - ст.16 бит адреса для считывания <br>
 * <b> Работа команды: </b><br>
 * производится проверка параметров команды на допустимость <br>
 * производится считывание данных из ОЗУ в буфер <br>
 * производится передача результата выполнения и считанных из ОЗУ СПО данных на КПА <br>
 */

void KpaCommand06 (int BaseMkoLine, int BaseMkoChan)
{
UINT DataSize, DataAddr, Index;
WORD Buffer[32];
BYTE *Src, *Dst;

DataSize = KpaCommand[1] & 0x00FF;
memmove(&DataAddr, &KpaCommand[2], 4);

// ошибка в параметрах команды
if ((DataSize < 1) || (DataSize > 64) || (!DataAddr))
  {
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DATA_ERROR, KPA_SUBADDR_IRQ);
  return;
  }

Src = (BYTE*) DataAddr;
Dst = (BYTE*) &Buffer;

// считывание данных из ОЗУ
for (Index = 0; Index < DataSize; Index++) 
  {
  Dst[Index] = Src[Index];
  }

// отправка данных и сообщения о выполнении команды
ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ, &Buffer, DataSize);
}
//================================================================================

/*! \fn void KpaCommand07 (int BaseMkoLine, int BaseMkoChan);
 * \brief Функция обработки команды 0x07: установить заданную конфигурацию МБК02. Вызывается при обработке прерывания от КПА
 * \param [in] BaseMkoLine - номер линии МКО для обмена с КПА
 * \param [in] BaseMkoChan - номер канала МКО для обмена с КПА
 * \details <b> Формат данных команды: </b><br>
 * слово команды [0] биты [7..0] - код команды (0x07) <br>
 * слово команды [1] биты [1..0] - код конфигурации каналов МБК02 (0 = выключены оба, 1 = включен 1-й, 2 = включен 2-й, 3 = ошибка в команде) <br>
 * <b> Работа команды: </b><br>
 * производится проверка параметров команды на допустимость <br>
 * производится считывание текущей конфигурации каналов МБК02 и сравнение с запрошенной <br>
 * если конфигурации не совпадают, производится выключение каналов и включение в требуемой конфигурации <br>
 * производится передача результата выполнения на КПА <br>
 */

#define ADDR_MBK02 2		// адрес МБК02 на шине МКО (= адресу 14Р732)

void KpaCommand07 (int BaseMkoLine, int BaseMkoChan)
{
int Result, MkoDevLine, MkoDevChan;
WORD Chan_Orig, Chan_Want, Chan_CurrState;
WORD Buffer[2];

// параметры линий обмена
MkoDevLine = SpoConfig.DevMkoLine & 1;
MkoDevChan = SpoConfig.DevMkoChan & 1;

// биты новой конфигурации блока
Chan_Orig = (KpaCommand[1] >> 0) & 0x03; Chan_Want = Chan_Orig;

// проверка корректности кода конфигурации
if (Chan_Want == 0x03) // ошибка кода состояния каналов
  {
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DATA_ERROR, KPA_SUBADDR_IRQ);
  return;
  }

// чтение текущей конфигурации МБК02
Result = MkoXchgBn(MkoDevLine, MkoDevChan, MKO_CMD(ADDR_MBK02, 1, MKO_RX, 1), 0);
if (Result)
  {
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, DECODE_XCHG_ERROR(Result), KPA_SUBADDR_IRQ);
  return;
  }
SleepMSec(10);

// текущая конфигурация каналов МБК02
Chan_CurrState = ((MkoBufB[0][0] >> 4) & 0x03) ^ 0x03;

// выход, если требуемая конфигурация уже установлена
if (Chan_CurrState == Chan_Want)
  {
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ);
  return;
  }

// отключаем все каналы МБК02
MkoBufB[0][0] = 0x0004;
Result = MkoXchgBn(MkoDevLine, MkoDevChan, MKO_CMD(ADDR_MBK02, 29, MKO_TX, 1), 0);
if (Result)
  {
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, DECODE_XCHG_ERROR(Result), KPA_SUBADDR_IRQ);
  return;
  }
SleepMSec(300);

// чтение текущей конфигурации МБК02
Result = MkoXchgBn(MkoDevLine, MkoDevChan, MKO_CMD(ADDR_MBK02, 1, MKO_RX, 1), 0);
if (Result)
  {
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, DECODE_XCHG_ERROR(Result), KPA_SUBADDR_IRQ);
  return;
  }
SleepMSec(10);

// текущая конфигурация каналов МБК02
Chan_CurrState = ((MkoBufB[0][0] >> 4) & 0x03) ^ 0x03;

// выход, если нет выключения каналов МБК02
if (Chan_CurrState != 0x00)
  {
  memmove(&Buffer, &MkoBufB[0][0], 2);
  ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_MBK02_OFF_ERROR, KPA_SUBADDR_IRQ, &Buffer, 2);
  return;
  }

// выход, если требуемое состояние МБК02 - "выключено"
if (Chan_Want == 0x00)
  {
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ);
  return;
  }

// включаем требуемый канал МБК04
if (Chan_Want & 0x01) MkoBufB[0][0] = 0x0001; 
else MkoBufB[0][0] = 0x0002;

Result = MkoXchgBn(MkoDevLine, MkoDevChan, MKO_CMD(ADDR_MBK02, 29, MKO_TX, 1), 0);
if (Result)
  {
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, DECODE_XCHG_ERROR(Result), KPA_SUBADDR_IRQ);
  return;
  }
SleepMSec(300);

// пауза на включение канала МБК02
SleepMSec(12000);

// чтение текущей конфигурации МБК02
Result = MkoXchgBn(MkoDevLine, MkoDevChan, MKO_CMD(ADDR_MBK02, 1, MKO_RX, 1), 0);
if (Result)
  {
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, DECODE_XCHG_ERROR(Result), KPA_SUBADDR_IRQ);
  return;
  }
SleepMSec(10);

// текущая конфигурация каналов МБК02
Chan_CurrState = ((MkoBufB[0][0] >> 4) & 0x03) ^ 0x03;

// выход, если нет установки требуемой конфигурации МБК02
if (Chan_CurrState != Chan_Want)
  {
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE | REPLY_FLAG_ERROR, KPA_SUBADDR_IRQ);
  return;
  }

// требуемая конфигурация МБК02 успешно установлена
ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ);
}
//================================================================================

/*! \fn void KpaCommand09 (int BaseMkoLine, int BaseMkoChan);
 * \brief Функция обработки команды 0x09: установить заданную конфигурацию МБК04. Вызывается при обработке прерывания от КПА
 * \param [in] BaseMkoLine - номер линии МКО для обмена с КПА
 * \param [in] BaseMkoChan - номер канала МКО для обмена с КПА
 * \details <b> Формат данных команды: </b><br>
 * слово команды [0] биты [7..0] - код команды (0x09) <br>
 * слово команды [1] биты [1..0] - код конфигурации каналов МБК04 (0 = выключены оба, 1 = включен 1-й, 2 = включен 2-й, 3 = ошибка в команде) <br>
 * <b> Работа команды: </b><br>
 * производится проверка параметров команды на допустимость <br>
 * производится считывание текущей конфигурации каналов МБК04 и сравнение с запрошенной <br>
 * если конфигурации не совпадают, производится выключение каналов и включение в требуемой конфигурации <br>
 * производится передача результата выполнения на КПА <br>
 */

#define ADDR_MBK04 4		// адрес МБК04 на шине МКО

void KpaCommand09 (int BaseMkoLine, int BaseMkoChan)
{
int Result, MkoDevLine, MkoDevChan;
WORD Chan_Orig, Chan_Want, Chan_CurrState;
WORD Buffer[2];

// параметры линий обмена
MkoDevLine = SpoConfig.DevMkoLine & 1;
MkoDevChan = SpoConfig.DevMkoChan & 1;

// биты новой конфигурации блока
Chan_Orig = (KpaCommand[1] >> 0) & 0x03; Chan_Want = Chan_Orig;

// проверка корректности кода конфигурации
if (Chan_Want == 0x03) // ошибка кода состояния каналов
  {
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DATA_ERROR, KPA_SUBADDR_IRQ);
  return;
  }

// чтение текущей конфигурации МБК04
Result = MkoXchgBn(MkoDevLine, MkoDevChan, MKO_CMD(ADDR_MBK04, 1, MKO_RX, 1), 0);
if (Result)
  {
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, DECODE_XCHG_ERROR(Result), KPA_SUBADDR_IRQ);
  return;
  }
SleepMSec(10);

// текущая конфигурация каналов МБК04
Chan_CurrState = (MkoBufB[0][0] & 0x03) ^ 0x03;

// выход, если требуемая конфигурация уже установлена
if (Chan_CurrState == Chan_Want)
  {
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ);
  return;
  }

// отключаем все каналы МБК04
MkoBufB[0][0] = 0x0202;
Result = MkoXchgBn(MkoDevLine, MkoDevChan, MKO_CMD(ADDR_MBK04, 29, MKO_TX, 1), 0);
if (Result)
  {
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, DECODE_XCHG_ERROR(Result), KPA_SUBADDR_IRQ);
  return;
  }
SleepMSec(300);

// чтение текущей конфигурации МБК04
Result = MkoXchgBn(MkoDevLine, MkoDevChan, MKO_CMD(ADDR_MBK04, 1, MKO_RX, 1), 0);
if (Result)
  {
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, DECODE_XCHG_ERROR(Result), KPA_SUBADDR_IRQ);
  return;
  }
SleepMSec(10);

// текущая конфигурация каналов МБК04
Chan_CurrState = (MkoBufB[0][0] & 0x03) ^ 0x03;

// выход, если нет выключения каналов МБК04
if (Chan_CurrState != 0x00)
  {
  memmove(&Buffer, &MkoBufB[0][0], 2);
  ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_MBK04_OFF_ERROR, KPA_SUBADDR_IRQ, &Buffer, 2);
  return;
  }

// выход, если требуемое состояние МБК04 - "выключено"
if (Chan_Want == 0x00)
  {
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ);
  return;
  }

// включаем требуемый канал МБК04
if (Chan_Want & 0x01) MkoBufB[0][0] = 0x0201; 
else MkoBufB[0][0] = 0x0204;

Result = MkoXchgBn(MkoDevLine, MkoDevChan, MKO_CMD(ADDR_MBK04, 29, MKO_TX, 1), 0);
if (Result)
  {
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, DECODE_XCHG_ERROR(Result), KPA_SUBADDR_IRQ);
  return;
  }

// пауза на включение канала МБК04
SleepMSec(2500);

// чтение текущей конфигурации МБК04
Result = MkoXchgBn(MkoDevLine, MkoDevChan, MKO_CMD(ADDR_MBK04, 1, MKO_RX, 1), 0);
if (Result)
  {
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, DECODE_XCHG_ERROR(Result), KPA_SUBADDR_IRQ);
  return;
  }
SleepMSec(10);

// текущая конфигурация каналов МБК04
Chan_CurrState = (MkoBufB[0][0] & 0x03) ^ 0x03;

// выход, если нет установки требуемой конфигурации МБК04
if (Chan_CurrState != Chan_Want)
  {
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE | REPLY_FLAG_ERROR, KPA_SUBADDR_IRQ);
  return;
  }

// требуемая конфигурация МБК04 успешно установлена
ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ);
}
//================================================================================

/*! \fn void KpaCommand0A_Erti (int BaseMkoLine, int BaseMkoChan);
 * \brief Функция обработки команды 0x0A: установить заданную конфигурацию БЭЧ. Вызывается при обработке прерывания от КПА
 * \param [in] BaseMkoLine - номер линии МКО для обмена с КПА
 * \param [in] BaseMkoChan - номер канала МКО для обмена с КПА
 * \details <b> Формат данных команды: </b><br>
 * слово команды [0] биты [7..0] - код команды (0x0A) <br>
 * слово команды [1] биты [1..0] - код конфигурации каналов ОУ БЭЧ (0 = не менять, 1 = включен 1-й, 2 = включен 2-й, 3 = ошибка в команде) <br>
 * слово команды [1] биты [4..2] - маска конфигурации каналов ОГ БЭЧ (0 = не менять, если в маске установлено более 1 бита = ошибка в команде) <br>
 * слово команды [1] биты [7..5] - маска конфигурации каналов ФИНИК БЭЧ (0 = не менять, если в маске установлено более 1 бита = ошибка в команде) <br>
 * слово команды [1] биты [9..8] - маска конфигурации каналов КП (0 = не менять, если в маске установлено более 1 бита = ошибка в команде) <br>
 * <b> Работа команды: </b><br>
 * производится проверка параметров команды на допустимость <br>
 * производится считывание текущей конфигурации БЭЧ <br>
 * если требуется смена конфигурации - переключение конфигурации блоков <br>
 * производится повторное считывание конфигурации БЭЧ <br>
 * производится проверка соответствия запрошенной и установленной конфигураций <br>
 * производится передача результата выполнения на КПА <br>
 */

void KpaCommand0A_Erti (int BaseMkoLine, int BaseMkoChan)
{
int Result, WrongState;
int DevMkoLine, DevMkoChan;
WORD MOU_Prev, MOU_Orig, MOU_Want, MOU_Curr;
WORD GEN_Prev, GEN_Orig, GEN_Want, GEN_Curr;
WORD FIN_Prev, FIN_Orig, FIN_Want, FIN_Curr;
WORD KP_Prev, KP_Orig, KP_Want, KP_Curr;
WORD PrevConfig[10], CurrConfig[10];

// параметры линий обмена
DevMkoLine = SpoConfig.DevMkoLine & 1;
DevMkoChan = SpoConfig.DevMkoChan & 1;

// биты новой конфигурации блоков
MOU_Orig = (KpaCommand[1] >> 0) & 0x03; MOU_Want = MOU_Orig;
GEN_Orig = (KpaCommand[1] >> 2) & 0x07; GEN_Want = GEN_Orig;
FIN_Orig = (KpaCommand[1] >> 5) & 0x07; FIN_Want = FIN_Orig;
KP_Orig  = (KpaCommand[1] >> 8) & 0x03; KP_Want  = KP_Orig;

// проверка корректности кода конфигурации
if ((MOU_Want == 0x03) || 		// ошибка кода конфигурации ОУ
    (BitCount(GEN_Want) > 1) || 	// ошибка кода конфигурации ОГ
    (BitCount(FIN_Want) > 1) || 	// ошибка кода конфигурации ФИНИК
    (KP_Want == 0x03)) 			// ошибка кода конфигурации КП
  {
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DATA_ERROR, KPA_SUBADDR_IRQ);
  return;
  }

// чтение текущей конфигурации БЭЧ
Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(5, 1, MKO_RX, 10), 0);
if (Result)
  {
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, DECODE_XCHG_ERROR(Result), KPA_SUBADDR_IRQ);
  return;
  }
SleepMSec(10);

// текущая конфигурация блоков БЭЧ
memmove(&PrevConfig, &MkoBufB[0], sizeof(PrevConfig));

MOU_Prev = ((PrevConfig[0] >> 6) & 0x01) ? (0x01) : (0x02);
GEN_Prev = (PrevConfig[0] >> 0) & 0x07;
FIN_Prev = (PrevConfig[0] >> 3) & 0x07;
KP_Prev  = ((PrevConfig[0] >> 7) & 0x01) ? (0x01) : (0x02);

// если запрашиваемая конфигурация уже установлена, сбрасываем запрос
if ((MOU_Want) && (MOU_Want == MOU_Prev)) MOU_Want = 0;
if ((GEN_Want) && (GEN_Want == GEN_Prev)) GEN_Want = 0;
if ((FIN_Want) && (FIN_Want == FIN_Prev)) FIN_Want = 0;
if ((KP_Want)  && (KP_Want == KP_Prev))   KP_Want = 0;

// если есть запрос на смену конфигурации блока КП
if (KP_Want)
  {
  // переключаем канал блока КП
  MkoBufB[0][0] = (KP_Want == 0x01) ? (0x0100) : (0x0200);
  Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(4, 28, MKO_TX, 1), 0);
  if (Result)
    {
    ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, DECODE_XCHG_ERROR(Result), KPA_SUBADDR_IRQ);
    return;
    }

  // пауза на переключение конфигурации блока КП
  SleepMSec(1500);

  // чтение новой конфигурации БЭЧ
  Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(5, 1, MKO_RX, 10), 0);
  if (Result)
    {
    ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, DECODE_XCHG_ERROR(Result), KPA_SUBADDR_IRQ);
    return;
    }
  SleepMSec(10);

  // новая конфигурация блоков БЭЧ
  memmove(&CurrConfig, &MkoBufB[0], sizeof(CurrConfig));

  MOU_Curr = ((CurrConfig[0] >> 6) & 0x01) ? (0x01) : (0x02);
  GEN_Curr = (CurrConfig[0] >> 0) & 0x07;
  FIN_Curr = (CurrConfig[0] >> 3) & 0x07;
  KP_Curr  = ((CurrConfig[0] >> 7) & 0x01) ? (0x01) : (0x02);

  // если есть незапланированые изменения в конфигурации блоков, восстанавливаем
  if ((!MOU_Want) && (MOU_Prev != MOU_Curr)) MOU_Want = MOU_Prev;
  if ((!GEN_Want) && (GEN_Prev != GEN_Curr)) GEN_Want = GEN_Prev;
  if ((!FIN_Want) && (FIN_Prev != FIN_Curr)) FIN_Want = FIN_Prev;

  // если запрашиваемая конфигурация уже установлена, сбрасываем запрос
  if ((MOU_Want) && (MOU_Want == MOU_Curr)) { MOU_Want = 0; MOU_Prev = MOU_Curr; }
  if ((GEN_Want) && (GEN_Want == GEN_Curr)) { GEN_Want = 0; GEN_Prev = GEN_Curr; }
  if ((FIN_Want) && (FIN_Want == FIN_Curr)) { FIN_Want = 0; FIN_Prev = FIN_Curr; }
  }

// если есть запрос на смену конфигурации блока ОУ
if (MOU_Want)
  {
  // переключаем канал блока ОУ
  MkoBufB[0][0] = (MOU_Want << 3);
  Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(4, 28, MKO_TX, 1), 0);
  if (Result)
    {
    ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, DECODE_XCHG_ERROR(Result), KPA_SUBADDR_IRQ);
    return;
    }

  // пауза на переключение конфигурации блока ОУ
  SleepMSec(1500);

  // чтение новой конфигурации БЭЧ
  Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(5, 1, MKO_RX, 10), 0);
  if (Result)
    {
    ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, DECODE_XCHG_ERROR(Result), KPA_SUBADDR_IRQ);
    return;
    }
  SleepMSec(10);

  // новая конфигурация блоков БЭЧ
  memmove(&CurrConfig, &MkoBufB[0], sizeof(CurrConfig));

  GEN_Curr = (CurrConfig[0] >> 0) & 0x07;
  FIN_Curr = (CurrConfig[0] >> 3) & 0x07;
  
  // если есть незапланированые изменения в конфигурации блоков, восстанавливаем
  if ((!GEN_Want) && (GEN_Prev != GEN_Curr)) GEN_Want = GEN_Prev;
  if ((!FIN_Want) && (FIN_Prev != FIN_Curr)) FIN_Want = FIN_Prev;

 // если запрашиваемая конфигурация уже установлена, сбрасываем запрос
  if ((GEN_Want) && (GEN_Want == GEN_Curr)) { GEN_Want = 0; GEN_Prev = GEN_Curr; }
  if ((FIN_Want) && (FIN_Want == FIN_Curr)) { FIN_Want = 0; FIN_Prev = FIN_Curr; }
  }

// переключаем конфигурацию ОГ
if (GEN_Want & 0x07)
  {
  if (GEN_Want & 0x01) MkoBufB[0][0] = 0x0001;
  else
    if (GEN_Want & 0x02) MkoBufB[0][0] = 0x0002;
    else MkoBufB[0][0] = 0x0003;

  Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(5, 2, MKO_TX, 1), 0);
  if (Result)
    {
    ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, DECODE_XCHG_ERROR(Result), KPA_SUBADDR_IRQ);
    return;
    }

  // пауза на переключение конфигурации ОГ
  SleepMSec(1000);
  }

// переключаем конфигурацию ФИНИК
if (FIN_Want & 0x07)
  {
  if (FIN_Want & 0x01) MkoBufB[0][0] = 0x0004;
  else
    if (FIN_Want & 0x02) MkoBufB[0][0] = 0x0008;
    else MkoBufB[0][0] = 0x000C;

  Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(5, 2, MKO_TX, 1), 0);
  if (Result)
    {
    ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, DECODE_XCHG_ERROR(Result), KPA_SUBADDR_IRQ);
    return;
    }

  // пауза на переключение конфигурации ФИНИК
  SleepMSec(1000);
  }

// чтение новой конфигурации БЭЧ
Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(5, 1, MKO_RX, 10), 0);
if (Result)
  {
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, DECODE_XCHG_ERROR(Result), KPA_SUBADDR_IRQ);
  return;
  }
SleepMSec(10);

// новая конфигурация блоков БЭЧ
memmove(&CurrConfig, &MkoBufB[0], sizeof(CurrConfig));

MOU_Curr = ((CurrConfig[0] >> 6) & 0x01) ? (0x01) : (0x02);
GEN_Curr = (CurrConfig[0] >> 0) & 0x07;
FIN_Curr = (CurrConfig[0] >> 3) & 0x07;
KP_Curr  = ((CurrConfig[0] >> 7) & 0x01) ? (0x01) : (0x02);

// сравниваем с запрошеной конфигурацией
WrongState = 0;

if ((KP_Orig)  && (KP_Orig != KP_Curr))   WrongState = 1;
if ((MOU_Orig) && (MOU_Orig != MOU_Curr)) WrongState = 2;
if ((GEN_Orig) && (GEN_Orig != GEN_Curr)) WrongState = 3;
if ((FIN_Orig) && (FIN_Orig != FIN_Curr)) WrongState = 4;

if (BitCount(GEN_Curr) > 1) WrongState = 5;
if (BitCount(FIN_Curr) > 1) WrongState = 6;

// один из модулей сконфигурирован неправильно
if (WrongState)
  {
  ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE | REPLY_FLAG_ERROR, KPA_SUBADDR_IRQ, &WrongState, 2);
  return;
  }

// выдаем код сообщения "конфигурация БЭЧ установлена"
ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ);
}
//================================================================================

/*! \fn void KpaCommand0A_Koi (int BaseMkoLine, int BaseMkoChan);
 * \brief Функция обработки команды 0x0A: установить заданную конфигурацию БЭЧ. Вызывается при обработке прерывания от КПА
 * \param [in] BaseMkoLine - номер линии МКО для обмена с КПА
 * \param [in] BaseMkoChan - номер канала МКО для обмена с КПА
 * \details <b> Формат данных команды: </b><br>
 * слово команды [0] биты [7..0] - код команды (0x0A) <br>
 * слово команды [1] биты [1..0] - код конфигурации каналов ОУ БЭЧ (0 = не менять, 1 = включен 1-й, 2 = включен 2-й, 3 = ошибка в команде) <br>
 * слово команды [1] биты [4..2] - маска конфигурации каналов ОГ БЭЧ (0 = не менять, если в маске установлено более 1 бита = ошибка в команде) <br>
 * слово команды [1] биты [7..5] - маска конфигурации каналов ФИНИК БЭЧ (0 = не менять, если в маске установлено более 1 бита = ошибка в команде) <br>
 * <b> Работа команды: </b><br>
 * производится проверка параметров команды на допустимость <br>
 * производится считывание текущей конфигурации БЭЧ <br>
 * если требуется смена конфигурации - переключение конфигурации блоков <br>
 * производится повторное считывание конфигурации БЭЧ <br>
 * производится проверка соответствия запрошенной и установленной конфигураций <br>
 * производится передача результата выполнения на КПА <br>
 */

void KpaCommand0A_Koi (int BaseMkoLine, int BaseMkoChan)
{
int Result, WrongState;
int DevMkoLine, DevMkoChan;
WORD MOU_Prev, MOU_Orig, MOU_Want, MOU_Curr;
WORD GEN_Prev, GEN_Orig, GEN_Want, GEN_Curr;
WORD FIN_Prev, FIN_Orig, FIN_Want, FIN_Curr;
WORD PrevConfig[10], CurrConfig[10];

// параметры линий обмена
DevMkoLine = SpoConfig.DevMkoLine & 1;
DevMkoChan = SpoConfig.DevMkoChan & 1;

// биты новой конфигурации блоков
MOU_Orig = (KpaCommand[1] >> 0) & 0x03; MOU_Want = MOU_Orig;
GEN_Orig = (KpaCommand[1] >> 2) & 0x07; GEN_Want = GEN_Orig;
FIN_Orig = (KpaCommand[1] >> 5) & 0x07; FIN_Want = FIN_Orig;

// проверка корректности кода конфигурации
if ((MOU_Want == 0x03) || 		// ошибка кода конфигурации ОУ
    (BitCount(GEN_Want) > 1) || 	// ошибка кода конфигурации ОГ
    (BitCount(FIN_Want) > 1)) 		// ошибка кода конфигурации ФИНИК
  {
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DATA_ERROR, KPA_SUBADDR_IRQ);
  return;
  }

// чтение текущей конфигурации БЭЧ
Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(5, 1, MKO_RX, 10), 0);
if (Result)
  {
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, DECODE_XCHG_ERROR(Result), KPA_SUBADDR_IRQ);
  return;
  }
SleepMSec(10);

// текущая конфигурация блоков БЭЧ
memmove(&PrevConfig, &MkoBufB[0], sizeof(PrevConfig));

MOU_Prev = (PrevConfig[0] >> 6) & 0x03;
GEN_Prev = (PrevConfig[0] >> 0) & 0x07;
FIN_Prev = (PrevConfig[0] >> 3) & 0x07;

// если запрашиваемая конфигурация уже установлена, сбрасываем запрос
if ((MOU_Want) && (MOU_Want == MOU_Prev)) MOU_Want = 0;
if ((GEN_Want) && (GEN_Want == GEN_Prev)) GEN_Want = 0;
if ((FIN_Want) && (FIN_Want == FIN_Prev)) FIN_Want = 0;

// если есть запрос на смену конфигурации блока ОУ
if (MOU_Want)
  {
  // переключаем канал блока ОУ
  MkoBufB[0][0] = (MOU_Want << 3);
  Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(4, 28, MKO_TX, 1), 0);
  if (Result)
    {
    ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, DECODE_XCHG_ERROR(Result), KPA_SUBADDR_IRQ);
    return;
    }
  SleepMSec(1500);
  
  // если нет запроса на установку новой конфигурации блоков, запрашиваем установку старой
  if (!GEN_Want) GEN_Want = GEN_Prev;
  if (!FIN_Want) FIN_Want = FIN_Prev;
  }

// переключаем конфигурацию ОГ
if (GEN_Want & 0x07)
  {
  if (GEN_Want & 0x01) MkoBufB[0][0] = 0x0001;
  else
    if (GEN_Want & 0x02) MkoBufB[0][0] = 0x0002;
    else MkoBufB[0][0] = 0x0003;

  Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(5, 2, MKO_TX, 1), 0);
  if (Result)
    {
    ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, DECODE_XCHG_ERROR(Result), KPA_SUBADDR_IRQ);
    return;
    }
  SleepMSec(1000);
  }

// переключаем конфигурацию ФИНИК
if (FIN_Want & 0x07)
  {
  if (FIN_Want & 0x01) MkoBufB[0][0] = 0x0004;
  else
    if (FIN_Want & 0x02) MkoBufB[0][0] = 0x0008;
    else MkoBufB[0][0] = 0x000C;

  Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(5, 2, MKO_TX, 1), 0);
  if (Result)
    {
    ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, DECODE_XCHG_ERROR(Result), KPA_SUBADDR_IRQ);
    return;
    }
  SleepMSec(1000);
  }

// чтение новой конфигурации БЭЧ
Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(5, 1, MKO_RX, 10), 0);
if (Result)
  {
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, DECODE_XCHG_ERROR(Result), KPA_SUBADDR_IRQ);
  return;
  }
SleepMSec(10);

// новая конфигурация блоков БЭЧ
memmove(&CurrConfig, &MkoBufB[0], sizeof(CurrConfig));

MOU_Curr = (CurrConfig[0] >> 6) & 0x03;
GEN_Curr = (CurrConfig[0] >> 0) & 0x07;
FIN_Curr = (CurrConfig[0] >> 3) & 0x07;

// сравниваем с запрошеной конфигурацией
WrongState = 0;

if ((MOU_Orig) && (MOU_Orig != MOU_Curr)) WrongState = 1;
if ((GEN_Orig) && (GEN_Orig != GEN_Curr)) WrongState = 2;
if ((FIN_Orig) && (FIN_Orig != FIN_Curr)) WrongState = 3;

if (BitCount(GEN_Curr) > 1) WrongState = 4;
if (BitCount(FIN_Curr) > 1) WrongState = 5;

// один из модулей сконфигурирован неправильно
if (WrongState)
  {
  ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE | REPLY_FLAG_ERROR, KPA_SUBADDR_IRQ, &WrongState, 2);
  return;
  }

// выдаем код сообщения "конфигурация БЭЧ установлена"
ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ);

/*
адрес 4/28, запись конфигурации ОУ
CD0/ОУ 0x0008/0x0010

адрес 5/2, запись конфигурации ОГ
CD0/ОГ 0x0001/0x0002/0x0003

адрес 5/2, запись конфигурации ФИНИК
CD0/ФИН 0x0004/0x0008/0x000C

адрес 5/1, чтение конфигурации, 10 слов
CD0 бит 0 - включен ОГ1
CD0 бит 1 - включен ОГ2
CD0 бит 2 - включен ОГ3
CD0 бит 3 - включен ФИНИК1
CD0 бит 4 - включен ФИНИК2
CD0 бит 5 - включен ФИНИК3
CD0 бит 6 - включен ОУ1
CD0 бит 7 - включен ОУ2
*/
}
//================================================================================

/*! \fn void KpaCommand0C (int BaseMkoLine, int BaseMkoChan);
 * \brief Функция обработки команды 0x0C: установить заданную конфигурацию МБК07. Вызывается при обработке прерывания от КПА
 * \param [in] BaseMkoLine - номер линии МКО для обмена с КПА
 * \param [in] BaseMkoChan - номер канала МКО для обмена с КПА
 * \details <b> Формат данных команды: </b><br>
 * слово команды [0] биты [7..0] - код команды (0x0C) <br>
 * слово команды [1] биты [2..0] - маска конфигурации каналов ФСМУ (0 = выключить, если в маске установлено более 1 бита = ошибка в команде) <br>
 * слово команды [1] биты [5..3] - маска конфигурации каналов ФСВУ (0 = выключить, если в маске установлено более 1 бита = ошибка в команде, если ФСМУ = 0 - выключить) <br>
 * слово команды [1] биты [7..6] - маска конфигурации генераторов (бит 6 = КГ1, бит 7 = КГ2, 0 = выключить, если в маске установлено более 1 бита = ошибка в команде, если = 0 - выдать команду "ВС") <br>
 * слово команды [1] биты [10..8] - маска конфигурации антенн (бит 8 = ОНА, бит 9 = МНА+У, бит 10 = МНА-У, 0 = не менять, если в маске установлено более 1 бита = ошибка в команде) <br>
 * <b> Работа команды: </b><br>
 * производится проверка параметров команды на допустимость <br>
 * производится считывание текущей конфигурации МБК07 <br>
 * если требуется смена конфигурации - переключение конфигурации блоков <br>
 * производится повторное считывание конфигурации МБК07 <br>
 * производится проверка соответствия запрошенной и установленной конфигураций <br>
 * производится передача результата выполнения на КПА <br>
 */

void KpaCommand0C (int BaseMkoLine, int BaseMkoChan);
void KpaCommand0CBase (int BaseMkoLine, int BaseMkoChan);
//================================================================================

void KpaCommand0C (int BaseMkoLine, int BaseMkoChan)
{
int PrevCtrlState;

// откл контроля команд для TX-07
PrevCtrlState = SpoConfig.EnableMBK07CmdCheck;
SpoConfig.EnableMBK07CmdCheck = 0;

KpaCommand0CBase(BaseMkoLine, BaseMkoChan);

SpoConfig.EnableMBK07CmdCheck = PrevCtrlState;
}
//================================================================================

void KpaCommand0CBase (int BaseMkoLine, int BaseMkoChan)
{
int Result, WrongState;
int DevMkoLine, DevMkoChan;
char CheckBuffer[8];
WORD FSMU_Orig, FSMU_Want; //, FSMU_Prev;
WORD FSVU_Orig, FSVU_Want; //, FSVU_Prev;
WORD GEN_Orig, GEN_Want; //, GEN_Prev;
WORD ANT_Orig, ANT_Want; //, ANT_Prev;
WORD State;
WORD Buffer[2];

// параметры линий обмена
DevMkoLine = SpoConfig.DevMkoLine & 1;
DevMkoChan = SpoConfig.DevMkoChan & 1;

// [маска ошибок]
// 000 001 010 011 100 101 110 111
//   0   0   0   1   0   1   1   1

CheckBuffer[0] = 0;
CheckBuffer[1] = 0;
CheckBuffer[2] = 0;
CheckBuffer[3] = 1;
CheckBuffer[4] = 0;
CheckBuffer[5] = 1;
CheckBuffer[6] = 1;
CheckBuffer[7] = 1;

// биты новой конфигурации блоков
FSMU_Orig = (KpaCommand[1] >> 0) & 0x07; FSMU_Want = FSMU_Orig;
FSVU_Orig = (KpaCommand[1] >> 3) & 0x07; FSVU_Want = FSVU_Orig;
GEN_Orig  = (KpaCommand[1] >> 6) & 0x03; GEN_Want  = GEN_Orig;
ANT_Orig  = (KpaCommand[1] >> 8) & 0x07; ANT_Want  = ANT_Orig;

// коррекция для случая выключенного ФСМУ
if (!FSMU_Orig) { FSVU_Want = FSVU_Orig = 0; }

// проверка корректности кода конфигурации
WrongState = 0;
if (CheckBuffer[FSMU_Orig]) { WrongState = 1; } // ошибка кода конфигурации ФСМУ
if (CheckBuffer[FSVU_Orig]) { WrongState = 2; } // ошибка кода конфигурации ФСВУ
if (CheckBuffer[GEN_Orig])  { WrongState = 3; } // ошибка кода конфигурации КГ
if (CheckBuffer[ANT_Orig])  { WrongState = 4; } // ошибка кода конфигурации антенн

if (WrongState)
  {
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DATA_ERROR, KPA_SUBADDR_IRQ);
  return;
  }

// сохраняем текущий конфиг МБК в логе обменов МКО
MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(7, 1, MKO_RX, 2), 0);

// отключаем МБК07
Result = DisableMBK07x(BaseMkoLine, BaseMkoChan, DevMkoLine, DevMkoChan, 1);
if (Result) return;

if (!FSMU_Want) // требуется выключение МБК07
  {
  // конфигурация МБК07 успешно установлена/изменена
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ);
  return;
  }
else // требуется включение/реконфигурация МБК07
  {
  // чтение текущей конфигурации МБК07
  Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(7, 1, MKO_RX, 2), 0);
  if (Result)
    {
    // МБК07 не ответил на запрос, вероятно блок выключен
    if (GEN_Want) // если требуется включить КГ
      {
      // выдаем команду на включение режима НС
      MkoBufB[0][0] = 0x8002;
      Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(4, 28, MKO_TX, 1), 0);
      if (Result)
        {
        ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, DECODE_XCHG_ERROR(Result), KPA_SUBADDR_IRQ);
        return;
        }
      // пауза на включение режима НС
      SleepMSec(MBK07_CMD_TIME);

      // включаем нужный канал КГ
      MkoBufB[0][0] = (GEN_Want & 0x01) ? (0xA002) : (0xB002);
      Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(4, 28, MKO_TX, 1), 0);
      if (Result)
        {
        ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, DECODE_XCHG_ERROR(Result), KPA_SUBADDR_IRQ);
        return;
        }
      SleepMSec(MBK07_CMD_TIME);
      }

    #if (1)
    if (ANT_Want)
      {
      // переключаем конфигурацию антенн
      MkoBufB[0][0] = 0;
      if (ANT_Want & 0x01) MkoBufB[0][0] = 0xA008;
      if (ANT_Want & 0x02) MkoBufB[0][0] = 0x9008;
      if (ANT_Want & 0x04) MkoBufB[0][0] = 0x8008;
    
      if (!MkoBufB[0][0])
        {
        ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_INTERNAL_DATA_ERROR, KPA_SUBADDR_IRQ);
        return;
        }
      
      Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(4, 28, MKO_TX, 1), 0);
      if (Result)
        {
        ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, DECODE_XCHG_ERROR(Result), KPA_SUBADDR_IRQ);
        return;
        }
      SleepMSec(MBK07_CMD_TIME);
      }
    #endif

    if (FSVU_Want)
      {
      // включаем нужный канал ФСВУ
      MkoBufB[0][0] = 0;
      if (FSVU_Want & 0x01) MkoBufB[0][0] = 0x8004;
      if (FSVU_Want & 0x02) MkoBufB[0][0] = 0x9004;
      if (FSVU_Want & 0x04) MkoBufB[0][0] = 0xA004;

      if (!MkoBufB[0][0])
        {
        ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_INTERNAL_DATA_ERROR, KPA_SUBADDR_IRQ);
        return;
        }

      Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(4, 28, MKO_TX, 1), 0);
      if (Result)
        {
        ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, DECODE_XCHG_ERROR(Result), KPA_SUBADDR_IRQ);
        return;
        }
      SleepMSec(MBK07_CMD_TIME);

      #if (0)
      // отключение ФСВУ
      MkoBufB[0][0] = 0xB004;
      Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(4, 28, MKO_TX, 1), 0);
      if (Result)
        {
        ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, DECODE_XCHG_ERROR(Result), KPA_SUBADDR_IRQ);
        return;
        }
      SleepMSec(MBK07_CMD_TIME); // пауза на отключение ФСВУ
      #endif
      }
    
    // включаем нужный канал ФСМУ
    MkoBufB[0][0] = 0;
    if (FSMU_Want & 0x01) MkoBufB[0][0] = 0x8001;
    if (FSMU_Want & 0x02) MkoBufB[0][0] = 0x9001;
    if (FSMU_Want & 0x04) MkoBufB[0][0] = 0xA001;

    if (!MkoBufB[0][0])
      {
      ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_INTERNAL_DATA_ERROR, KPA_SUBADDR_IRQ);
      return;
      }

    Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(4, 28, MKO_TX, 1), 0);
    if (Result)
      {
      ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, DECODE_XCHG_ERROR(Result), KPA_SUBADDR_IRQ);
      return;
      }
    SleepMSec(MBK07_CMD_TIME);

    // чтение текущей конфигурации МБК07
    Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(7, 1, MKO_RX, 2), 0);
    if (Result)
      {
      ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, DECODE_XCHG_ERROR(Result), KPA_SUBADDR_IRQ);
      return;
      }

    // анализируем ОК ФСМУ
    State = ((MkoBufB[0][0] >> 8) & 0x07) ^ 0x07;
    Result = CheckFsmuOK(BaseMkoLine, BaseMkoChan, DevMkoLine, DevMkoChan, FSMU_Want, State);
    if (Result) return;

    // анализируем состояние ОК КГ и режима стабильности
    State = ((MkoBufB[0][1] >> 5) & 0x07) ^ 0x07;
    Result = CheckGenOK(BaseMkoLine, BaseMkoChan, DevMkoLine, DevMkoChan, GEN_Want, State);
    if (Result) return;

    // проверяем состояние блока ФСМУ
    Result = CheckFsmuPower(DevMkoLine, DevMkoChan);
    if (Result != 1)
      {
      ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_MBK07_FSMU_STATE_ERROR, KPA_SUBADDR_IRQ);
      return;
      }
 
    #if (0)
    if (FSVU_Want)
      {
      // включаем нужный канал ФСВУ
      MkoBufB[0][0] = 0;
      if (FSVU_Want & 0x01) MkoBufB[0][0] = 0x8004;
      if (FSVU_Want & 0x02) MkoBufB[0][0] = 0x9004;
      if (FSVU_Want & 0x04) MkoBufB[0][0] = 0xA004;

      if (!MkoBufB[0][0])
        {
        ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_INTERNAL_DATA_ERROR, KPA_SUBADDR_IRQ);
        return;
        }

      Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(4, 28, MKO_TX, 1), 0);
      if (Result)
        {
        ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, DECODE_XCHG_ERROR(Result), KPA_SUBADDR_IRQ);
        return;
        }
      SleepMSec(MBK07_CMD_TIME);
      }
    #endif

    // чтение текущей конфигурации МБК07
    Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(7, 1, MKO_RX, 2), 0);
    if (Result)
      {
      ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, DECODE_XCHG_ERROR(Result), KPA_SUBADDR_IRQ);
      return;
      }

    // анализируем ОК ФСМУ
    State = ((MkoBufB[0][0] >> 8) & 0x07) ^ 0x07;
    Result = CheckFsmuOK(BaseMkoLine, BaseMkoChan, DevMkoLine, DevMkoChan, FSMU_Want, State);
    if (Result) return;

    // анализируем состояние КГ и режима стабильности
    State = ((MkoBufB[0][1] >> 5) & 0x07) ^ 0x07;
    Result = CheckGenOK(BaseMkoLine, BaseMkoChan, DevMkoLine, DevMkoChan, GEN_Want, State);
    if (Result) return;

    if (FSVU_Want)
      {
      // анализируем ОК ФСВУ
      State = ((MkoBufB[0][0] >> 11) & 0x07) ^ 0x07;
      Result = CheckFsvuOK(BaseMkoLine, BaseMkoChan, DevMkoLine, DevMkoChan, FSVU_Want, State);
      if (Result) return;

      // проверяем включенное состояние блока ФСВУ
      Result = CheckFsvuPower(DevMkoLine, DevMkoChan);
      if (Result == -2)
        {
        // нет ответа от ФСВУ - устройство ФСМУ неисправно
        ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_MBK07_FSMU_STATE_ERROR, KPA_SUBADDR_IRQ);
        return;
        }
      if (Result != 1)
        {
        // устройство ФСВУ выключено или неисправно
        ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_MBK07_FSVU_STATE_ERROR, KPA_SUBADDR_IRQ);
        return;
        }
      }

    // проверяем состояние блока ФСМУ
    Result = CheckFsmuPower(DevMkoLine, DevMkoChan);
    if (Result != 1)
      {
      ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_MBK07_FSMU_STATE_ERROR, KPA_SUBADDR_IRQ);
      return;
      }

    if (ANT_Want)
      {
      // чтение текущей конфигурации МБК07
      Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(7, 1, MKO_RX, 2), 0);
      if (Result)
        {
        ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, DECODE_XCHG_ERROR(Result), KPA_SUBADDR_IRQ);
        return;
        }

      // анализируем ОК состояния антенн
      State = ((MkoBufB[0][1] >> 2) & 0x07) ^ 0x07;
      if (CheckBuffer[State])
        {
        ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_MBK07_ANT_STATE_ERROR, KPA_SUBADDR_IRQ);
        return; 
        }

      // если текущая конфигурация антенн отличается от требуемой
      if (ANT_Want != State)
        {
        ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_MBK07_ANT_STATE_ERROR, KPA_SUBADDR_IRQ);
        return;
        }
      }

    ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ);
    }
  else
    {
    // МБК07 ответил на запрос - ошибка выключения блока
    Buffer[0] = MkoBufB[0][0];
    Buffer[1] = MkoBufB[0][1];
    ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_MBK07_FSMU_OFF_ERROR, KPA_SUBADDR_IRQ, &Buffer, 4);
    }
  }

/******************** DEBUG ********************/

/*
адрес 7/1, чтение конфигурации, 2 слова
CD0 бит 8  - включен 1-й канал ФСМУ
CD0 бит 9  - включен 2-й канал ФСМУ
CD0 бит 10 - включен 3-й канал ФСМУ
CD0 бит 11 - включен 1-й канал ФСВУ
CD0 бит 12 - включен 2-й канал ФСВУ
CD0 бит 13 - включен 3-й канал ФСВУ

CD1 бит 2 - подключена МНА-У
CD1 бит 3 - подключена МНА+У
CD1 бит 4 - подключена ОНА
CD1 бит 5 - включен кварцевый генератор 2
CD1 бит 6 - включен кварцевый генератор 1
CD1 бит 7 - включен режим высокой стабильности

адрес 4/28, установка конфигурации МБК07
0x8001 = Включить 1-й канал ФСМУ, отключить 2-й и 3-й каналы
0x9001 = Включить 2-й канал ФСМУ, отключить 1-й и 3-й каналы
0xA001 = Включить 3-й канал ФСМУ, отключить 1-й и 2-й каналы
0xB001 = Отключить ФСМУ

0x8004 = Включить 1-й канал ФСВУ, отключить 2-й и 3-й каналы
0x9004 = Включить 2-й канал ФСВУ, отключить 1-й и 3-й каналы
0xA004 = Включить 3-й канал ФСВУ, отключить 1-й и 2-й каналы
0xB004 = Выключить ФСВУ

0xA002 = Включить КГ1, отключить КГ2
0xB002 = Включить КГ2, отключить КГ1

0x8008 = Переключить МБК07 на ОНА
0x9008 = Переключить МБК07 на МНА+У
0xA008 = Переключить МБК07 на МНА-У

0x9002 = Включить режим ВС, отключить НС, КГ1 и КГ2
*/
}
//================================================================================

/*! \fn int DisableMBK07x (int BaseMkoLine, int BaseMkoChan, int DevMkoLine, int DevMkoChan, int DoFsvuOff);
 * \brief Служебная функция отключения МБК07
 * \param [in] BaseMkoLine - номер линии МКО для обмена с КПА
 * \param [in] BaseMkoChan - номер канала МКО для обмена с КПА
 * \param [in] DevMkoLine - номер линии МКО для обмена с БА
 * \param [in] DevMkoChan - номер канала МКО для обмена с БА
 * \param [in] DoFsvuOff - выполнить отключение блока ФСМУ (0 - нет, не 0 - да)
 * \return Возвращает: 0 - при успешном завершении, не 0 - при ошибках 
 * \details <b> Работа функции: </b><br>
 * производится отключение блоков ФСВУ и ФСМУ <br>
 * производится проверка отключения <br>
 * производится установка режима ВС <br>
 * при ошибках производится передача результата выполнения на КПА <br>
 */

int DisableMBK07x (int BaseMkoLine, int BaseMkoChan, int DevMkoLine, int DevMkoChan, int DoFsvuOff)
{
int Result;
WORD Buffer[2];

// отключение ФСМУ
MkoBufB[0][0] = 0xB001;
Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(4, 28, MKO_TX, 1), 0);
if (Result)
  {
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, DECODE_XCHG_ERROR(Result), KPA_SUBADDR_IRQ);
  return 1;
  }
SleepMSec(MBK07_CMD_TIME); // пауза на отключение ФСМУ

// чтение текущей конфигурации МБК07
Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(7, 1, MKO_RX, 2), 0);
if (!Result)
  {
  // МБК07 ответил на запрос - ошибка выключения блока
  Buffer[0] = MkoBufB[0][0];
  Buffer[1] = MkoBufB[0][1];
  ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_MBK07_FSMU_OFF_ERROR, KPA_SUBADDR_IRQ, &Buffer, 4);
  return 1;
  }

// отключение ФСВУ
MkoBufB[0][0] = 0xB004;
Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(4, 28, MKO_TX, 1), 0);
if (Result)
  {
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, DECODE_XCHG_ERROR(Result), KPA_SUBADDR_IRQ);
  return 1;
  }
SleepMSec(MBK07_CMD_TIME); // пауза на отключение ФСВУ

// чтение текущей конфигурации МБК07
Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(7, 1, MKO_RX, 2), 0);
if (!Result)
  {
  // МБК07 ответил на запрос - ошибка выключения блока
  Buffer[0] = MkoBufB[0][0];
  Buffer[1] = MkoBufB[0][1];
  ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_MBK07_FSMU_OFF_ERROR, KPA_SUBADDR_IRQ, &Buffer, 4);
  return 1;
  }

// включение режима высокой стабильности (режим ВС)
if (SpoConfig.EnableMBK07GenOn)
  {
  MkoBufB[0][0] = 0x9002;
  Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(4, 28, MKO_TX, 1), 0);
  if (Result)
    {
    ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, DECODE_XCHG_ERROR(Result), KPA_SUBADDR_IRQ);
    return 1;
    }
  SleepMSec(MBK07_CMD_TIME); // пауза на включение режима ВС
  }

// чтение текущей конфигурации МБК07
Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(7, 1, MKO_RX, 2), 0);
if (!Result)
  {
  // МБК07 ответил на запрос - ошибка выключения блока
  Buffer[0] = MkoBufB[0][0];
  Buffer[1] = MkoBufB[0][1];
  ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_MBK07_FSMU_OFF_ERROR, KPA_SUBADDR_IRQ, &Buffer, 4);
  return 1;
  }

return 0;
}
//================================================================================

/*! \fn int CheckFsmuPower (int DevMkoLine, int DevMkoChan);
 * \brief Служебная функция проверки состояния ФСМУ
 * \param [in] BaseMkoLine - номер линии МКО для обмена с КПА
 * \param [in] BaseMkoChan - номер канала МКО для обмена с КПА
 * \param [in] DevMkoLine - номер линии МКО для обмена с БА
 * \param [in] DevMkoChan - номер канала МКО для обмена с БА
 * \return Возвращает: 0 - устройство отключено, 1 - устройство включено, -1 - устройство неисправно
 * \details <b> Работа функции: </b><br>
 * производится считывание состояния блока ФСМУ <br>
 * производится анализ и возврат кода состояния устройства <br>
 */

int CheckFsmuPower (int DevMkoLine, int DevMkoChan)
{
int Result;

// считываем статус блока ФСМУ
Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(7, 26, MKO_RX, 2), 0);
if (Result)
  {
  // состояние ФСМУ - устройство выключено
  return 0;
  }

// уровень выше 125 - устройство включено
if (MkoBufB[0][0] >= 125)
  {
  // состояние ФСМУ - устройство включено
  return 1;
  }

// состояние ФСМУ - устройство неисправно
return -1;
}
//================================================================================

/*! \fn int CheckFsvuPower (int DevMkoLine, int DevMkoChan);
 * \brief Служебная функция проверки состояния ФСВУ
 * \param [in] BaseMkoLine - номер линии МКО для обмена с КПА
 * \param [in] BaseMkoChan - номер канала МКО для обмена с КПА
 * \param [in] DevMkoLine - номер линии МКО для обмена с БА
 * \param [in] DevMkoChan - номер канала МКО для обмена с БА
 * \return Возвращает: 0 - устройство отключено, 1 - устройство включено, -1 - устройство неисправно, -2 - выключен блок ФСМУ
 * \details <b> Работа функции: </b><br>
 * производится считывание состояния блока ФСВУ <br>
 * производится анализ и возврат кода состояния устройства <br>
 */

int CheckFsvuPower (int DevMkoLine, int DevMkoChan)
{
int Result;

// считываем статус блока ФСВУ
Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(7, 26, MKO_RX, 2), 0);
if (Result)
  {
  // блок не отвечает - ФСМУ отключен
  return -2;
  }

// уровень ниже 10 - устройство отключено
if (MkoBufB[0][1] < 125)
  {
  // состояние ФСВУ - устройство выключено
  return 0;
  }

// уровень выше 120 - устройство включено
if (MkoBufB[0][1] >= 125)
  {
  // состояние ФСВУ - устройство включено
  return 1;
  }

// уровень вне допустимых - устройство неисправно
return -1;
}
//================================================================================

/*! \fn int CheckFsmuOK (int BaseMkoLine, int BaseMkoChan, int DevMkoLine, int DevMkoChan, int WantState, int State);
 * \brief Служебная функция проверки OK ФСМУ
 * \param [in] BaseMkoLine - номер линии МКО для обмена с КПА
 * \param [in] BaseMkoChan - номер канала МКО для обмена с КПА
 * \param [in] DevMkoLine - номер линии МКО для обмена с БА
 * \param [in] DevMkoChan - номер канала МКО для обмена с БА
 * \param [in] WantState - требуемая конфигурация ФСМУ
 * \param [in] State - реальная конфигурация ФСМУ
 * \return Возвращает: 0 - конфигурация в норме, не 0 - ошибка в конфигурации
 * \details <b> Работа функции: </b><br>
 * производится анализ текущего и запрошенного состояния блока ФСМУ <br>
 * производится передача кодов ошибок на КПА при несоответствии требуемой конфигурации <br>
 */

int CheckFsmuOK (int BaseMkoLine, int BaseMkoChan, int DevMkoLine, int DevMkoChan, int WantState, int State)
{
WORD Buffer[2];

// анализируем ОК ФСМУ
if ((BitCount(State) != 1) ||
    ((WantState & 0x01) && ((State & 0x0001) == 0)) ||
    ((WantState & 0x02) && ((State & 0x0002) == 0)) ||
    ((WantState & 0x04) && ((State & 0x0004) == 0)))
  {
  Buffer[0] = MkoBufB[0][0];
  Buffer[1] = MkoBufB[0][1];
  ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_MBK07_FSMU_ON_ERROR, KPA_SUBADDR_IRQ, &Buffer, 4);
  return -1;
  }

return 0;
}
//================================================================================

/*! \fn int CheckFsvuOK (int BaseMkoLine, int BaseMkoChan, int DevMkoLine, int DevMkoChan, int WantState, int State);
 * \brief Служебная функция проверки OK ФСВУ
 * \param [in] BaseMkoLine - номер линии МКО для обмена с КПА
 * \param [in] BaseMkoChan - номер канала МКО для обмена с КПА
 * \param [in] DevMkoLine - номер линии МКО для обмена с БА
 * \param [in] DevMkoChan - номер канала МКО для обмена с БА
 * \param [in] WantState - требуемая конфигурация ФСВУ
 * \param [in] State - реальная конфигурация ФСВУ
 * \return Возвращает: 0 - конфигурация в норме, не 0 - ошибка в конфигурации
 * \details <b> Работа функции: </b><br>
 * производится анализ текущего и запрошенного состояния блока ФСВУ <br>
 * производится передача кодов ошибок на КПА при несоответствии требуемой конфигурации <br>
 */

int CheckFsvuOK (int BaseMkoLine, int BaseMkoChan, int DevMkoLine, int DevMko7Chan, int WantState, int State)
{
WORD Buffer[2];

// анализируем ОК ФСМУ
if ((BitCount(State) != 1) ||
    ((WantState & 0x01) && ((State & 0x0001) == 0)) ||
    ((WantState & 0x02) && ((State & 0x0002) == 0)) ||
    ((WantState & 0x04) && ((State & 0x0004) == 0)))
  {
  Buffer[0] = MkoBufB[0][0];
  Buffer[1] = MkoBufB[0][1];
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_MBK07_FSVU_ON_ERROR, KPA_SUBADDR_IRQ);
  return -1;
  }   

return 0;
}
//================================================================================

/*! \fn int CheckGenOK (int BaseMkoLine, int BaseMkoChan, int DevMkoLine, int DevMkoChan, int WantState, int State);
 * \brief Служебная функция проверки состояния генераторов и режима стабильности
 * \param [in] BaseMkoLine - номер линии МКО для обмена с КПА
 * \param [in] BaseMkoChan - номер канала МКО для обмена с КПА
 * \param [in] DevMkoLine - номер линии МКО для обмена с БА
 * \param [in] DevMkoChan - номер канала МКО для обмена с БА
 * \param [in] WantState - требуемая конфигурация генераторов
 * \param [in] State - реальная конфигурация генераторов
 * \return Возвращает: 0 - конфигурация в норме, не 0 - ошибка в конфигурации
 * \details <b> Работа функции: </b><br>
 * производится анализ текущего и запрошенного состояния генераторов <br>
 * производится передача кодов ошибок на КПА при несоответствии требуемой конфигурации <br>
 */

int CheckGenOK (int BaseMkoLine, int BaseMkoChan, int DevMkoLine, int DevMkoChan, int WantState, int State)
{
// проверяем состояние генераторов
if (WantState)
  {
  // проверяем наличие режима НС
  if ((State & 0x04) == 0)
    {
    ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_MBK07_LO_STAB_ON_ERROR, KPA_SUBADDR_IRQ);
    return -1;
    }

  // проверяем включение КГ1
  if ((WantState == 0x01) && ((State & 0x03) != 0x01))
    {
    ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_MBK07_KG1_ON_ERROR, KPA_SUBADDR_IRQ);
    return -1;
    }

  // проверяем включение КГ2
  if ((WantState == 0x02) && ((State & 0x03) != 0x02))
    {
    ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_MBK07_KG2_ON_ERROR, KPA_SUBADDR_IRQ);
    return -1;
    }
  }
else
  {
  // проверяем наличие режима ВС
  if ((State & 0x04) != 0)
    {
    ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_MBK07_HI_STAB_ON_ERROR, KPA_SUBADDR_IRQ);
    return -1;
    }

  // проверяем отключенное состояние генераторов
  if ((State & 0x03) != 0x03)
    {
    ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_MBK07_KG_OFF_ERROR, KPA_SUBADDR_IRQ);
    return -1;
    }
  }

return 0;
}
//================================================================================

/*! \fn void KpaCommand0D (int BaseMkoLine, int BaseMkoChan);
 * \brief Функция обработки команды 0x0D: установить заданную конфигурацию ЛКА5. Вызывается при обработке прерывания от КПА
 * \param [in] BaseMkoLine - номер линии МКО для обмена с КПА
 * \param [in] BaseMkoChan - номер канала МКО для обмена с КПА
 * \details <b> Формат данных команды: </b><br>
 * слово команды [0] биты [7..0] - код команды (0x0D) <br>
 * слово команды [1] биты [1..0] - код конфигурации блока МУ ЛКА5 (0 - не менять конфигурацию, 1 - вкл осн канал, 2 - вкл рез канал, 3 - ошибка в команде) <br>
 * слово команды [1] биты [3..2] - код конфигурации блока МВКУ0 ЛКА5 (0 - не менять конфигурацию, 1 - вкл осн канал, 2 - вкл рез канал, 3 - выключить оба) <br>
 * слово команды [1] биты [5..4] - код конфигурации блока МВКУ1 ЛКА5 (0 - не менять конфигурацию, 1 - вкл осн канал, 2 - вкл рез канал, 3 - выключить оба) <br>
 * слово команды [1] биты [7..6] - код конфигурации блока МВКУ2 ЛКА5 (0 - не менять конфигурацию, 1 - вкл осн канал, 2 - вкл рез канал, 3 - выключить оба) <br>
 * слово команды [1] биты [9..8] - код конфигурации блока МВМК0 ЛКА5 (0 - не менять конфигурацию, 1 - вкл осн канал, 2 - вкл рез канал, 3 - выключить оба) <br>
 * слово команды [1] биты [11..10] - код конфигурации блока МВМК1 ЛКА5 (0 - не менять конфигурацию, 1 - вкл осн канал, 2 - вкл рез канал, 3 - выключить оба) <br>
 * слово команды [1] биты [13..12] - код конфигурации блока МВМК2 ЛКА5 (0 - не менять конфигурацию, 1 - вкл осн канал, 2 - вкл рез канал, 3 - выключить оба) <br>
 * слово команды [1] биты [15..14] - код конфигурации блока МПВН ЛКА5 (0 - не менять конфигурацию, 1 - вкл осн канал, 2 - вкл рез канал, 3 - выключить оба) <br>
 * слово команды [2] - если не 0 = время на ожидание готовности блока МУ, мсек (если 0 = время на ожидание готовности = 300 мсек) <br>
 * <b> Работа команды: </b><br>
 * производится проверка параметров команды на допустимость <br>
 * производится считывание текущей конфигурации ЛКА5 <br>
 * производится анализ текущей и запрошенной конфигураций <br>
 * производится переключение конфигурации блоков <br>
 * производится повторное считывание конфигурации ЛКА5 <br>
 * производится проверка соответствия запрошенной и установленной конфигураций <br>
 * производится передача результата выполнения на КПА <br>
 */

void KpaCommand0D (int BaseMkoLine, int BaseMkoChan)
{
int Result, CmdCount, WrongState;
int DevMkoLine, DevMkoChan;
WORD MUPR_Prev, MUPR_Orig, MUPR_Want;
WORD MVKU0_Prev, MVKU0_Orig, MVKU0_Want;
WORD MVKU1_Prev, MVKU1_Orig, MVKU1_Want;
WORD MVKU2_Prev, MVKU2_Orig, MVKU2_Want;
WORD MVMK0_Prev, MVMK0_Orig, MVMK0_Want;
WORD MVMK1_Prev, MVMK1_Orig, MVMK1_Want;
WORD MVMK2_Prev, MVMK2_Orig, MVMK2_Want;
WORD MPVN1_Prev, MPVN1_Orig, MPVN1_Want, MPVN_Exist;
WORD State, Config[8], PrevConfig[8], CurrConfig[8];
WORD StateMVMK1, StateMVMK2;
WORD MuStartupTime;

// параметры линий обмена
DevMkoLine = SpoConfig.DevMkoLine & 1;
DevMkoChan = SpoConfig.DevMkoChan & 1;

// биты новой конфигурации блоков
MUPR_Orig  = (KpaCommand[1] >> 0) & 0x03;  MUPR_Want  = MUPR_Orig;
MVKU0_Orig = (KpaCommand[1] >> 2) & 0x03;  MVKU0_Want = MVKU0_Orig;
MVKU1_Orig = (KpaCommand[1] >> 4) & 0x03;  MVKU1_Want = MVKU1_Orig;
MVKU2_Orig = (KpaCommand[1] >> 6) & 0x03;  MVKU2_Want = MVKU2_Orig;
MVMK0_Orig = (KpaCommand[1] >> 8) & 0x03;  MVMK0_Want = MVMK0_Orig;
MVMK1_Orig = (KpaCommand[1] >> 10) & 0x03; MVMK1_Want = MVMK1_Orig;
MVMK2_Orig = (KpaCommand[1] >> 12) & 0x03; MVMK2_Want = MVMK2_Orig;
MPVN1_Orig = (KpaCommand[1] >> 14) & 0x03; MPVN1_Want = MPVN1_Orig;

MuStartupTime = (KpaCommand[2]) ? (KpaCommand[2]) : (300);

// проверка корректности кода конфигурации
if (MUPR_Want == 0x03) // ошибка кода состояния МУ
  {
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DATA_ERROR, KPA_SUBADDR_IRQ);
  return;
  }

// чтение текущей конфигурации ЛКА5
Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(4, 17, MKO_RX, 8), 0);
if (Result)
  {
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, DECODE_XCHG_ERROR(Result), KPA_SUBADDR_IRQ);
  return;
  }
SleepMSec(10);

// текущая конфигурация блоков ЛКА5
memmove(&PrevConfig, &MkoBufB[0], sizeof(PrevConfig));

MUPR_Prev  = (PrevConfig[0] >> 6) & 0x03;
MVKU0_Prev = (PrevConfig[1] >> 6) & 0x03;
MVKU1_Prev = (PrevConfig[2] >> 6) & 0x03;
MVKU2_Prev = (PrevConfig[3] >> 6) & 0x03;
MVMK0_Prev = (PrevConfig[4] >> 6) & 0x03;
MVMK1_Prev = (PrevConfig[5] >> 6) & 0x03;
MVMK2_Prev = (PrevConfig[6] >> 6) & 0x03;
MPVN1_Prev = (PrevConfig[7] >> 6) & 0x03;

MPVN_Exist = PrevConfig[7];
if (MPVN_Exist)
  {
  // для новых ЛКА вкл/откл модулей ВМК производится синхронно
  if (MVMK1_Orig != MVMK2_Orig)
    {
    ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DATA_ERROR, KPA_SUBADDR_IRQ);
    return;
    }
  }

// коррекция для состояния "выключено"
if (!MVKU0_Prev) MVKU0_Prev = 0x03;
if (!MVKU1_Prev) MVKU1_Prev = 0x03;
if (!MVKU2_Prev) MVKU2_Prev = 0x03;
if (!MVMK0_Prev) MVMK0_Prev = 0x03;
if (!MVMK1_Prev) MVMK1_Prev = 0x03;
if (!MVMK2_Prev) MVMK2_Prev = 0x03;
if (!MPVN1_Prev) MPVN1_Prev = 0x03;

// если запрашиваемая конфигурация уже установлена, сбрасываем запрос
if ((MUPR_Want)  && (MUPR_Want  == MUPR_Prev))  MUPR_Want = 0;
if ((MVKU0_Want) && (MVKU0_Want == MVKU0_Prev)) MVKU0_Want = 0;
if ((MVKU1_Want) && (MVKU1_Want == MVKU1_Prev)) MVKU1_Want = 0;
if ((MVKU2_Want) && (MVKU2_Want == MVKU2_Prev)) MVKU2_Want = 0;
if ((MVMK0_Want) && (MVMK0_Want == MVMK0_Prev)) MVMK0_Want = 0;
if ((MVMK1_Want) && (MVMK1_Want == MVMK1_Prev)) MVMK1_Want = 0;
if ((MVMK2_Want) && (MVMK2_Want == MVMK2_Prev)) MVMK2_Want = 0;
if ((MPVN1_Want) && (MPVN1_Want == MPVN1_Prev)) MPVN1_Want = 0;

// если есть запрос на смену конфигурации блока МУ
if (MUPR_Want)
  {
  // переключаем канал блока МУ
  MkoBufB[0][0] = (MUPR_Want == 0x01) ? (0x0008) : (0x0010);
  Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(2, 29, MKO_TX, 1), 0);
  if (Result)
    {
    ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, DECODE_XCHG_ERROR(Result), KPA_SUBADDR_IRQ);
    return;
    }
  SleepMSec(MuStartupTime);

  // если нет запроса на установку новой конфигурации блоков, запрашиваем установку старой
  if (!MVKU0_Want) MVKU0_Want = MVKU0_Prev;
  if (!MVKU1_Want) MVKU1_Want = MVKU1_Prev;
  if (!MVKU2_Want) MVKU2_Want = MVKU2_Prev;
  if (!MVMK0_Want) MVMK0_Want = MVMK0_Prev;
  if (!MVMK1_Want) MVMK1_Want = MVMK1_Prev;
  if (!MVMK2_Want) MVMK2_Want = MVMK2_Prev;
  if (!MPVN1_Want) MPVN1_Want = MPVN1_Prev;
  }

// собираем пакет команд для конфигурирования ЛКА5
CmdCount = 0; 
if (MVKU0_Want) { Config[CmdCount++] = 0x2000 | ((MVKU0_Want & 0x03) << 6); }
if (MVKU1_Want) { Config[CmdCount++] = 0x2100 | ((MVKU1_Want & 0x03) << 6); }
if (MVKU2_Want) { Config[CmdCount++] = 0x2200 | ((MVKU2_Want & 0x03) << 6); }
if (MVMK0_Want) { Config[CmdCount++] = 0x3000 | ((MVMK0_Want & 0x03) << 6); }
if (MVMK1_Want) { Config[CmdCount++] = 0x3100 | ((MVMK1_Want & 0x03) << 6); }
if (MVMK2_Want) { Config[CmdCount++] = 0x3200 | ((MVMK2_Want & 0x03) << 6); }
if (MPVN_Exist) { if (MPVN1_Want) { Config[CmdCount++] = 0x5000 | ((MPVN1_Want & 0x03) << 6); } }

// переключаем конфигурацию блоков ЛКА5
if (CmdCount)
  {
  memmove(&MkoBufB[0], &Config, CmdCount * sizeof(WORD));
  Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(4, 17, MKO_TX, CmdCount), 0);
  if (Result)
    {
    ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, DECODE_XCHG_ERROR(Result), KPA_SUBADDR_IRQ);
    return;
    }
  SleepMSec(100);
  }

// чтение новой конфигурации ЛКА5
Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(4, 17, MKO_RX, 8), 0);
if (Result)
  {
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, DECODE_XCHG_ERROR(Result), KPA_SUBADDR_IRQ);
  return;
  }
SleepMSec(10);

// новая конфигурация блоков ЛКА5
memmove(&CurrConfig, &MkoBufB[0], sizeof(CurrConfig));

// сравниваем с запрошеной конфигурацией
// (сравниваем все слово состояния, считаем что сбоев быть не должно)
WrongState = 0x0000;

if (MUPR_Orig)
  {
  State = CurrConfig[0];
  if ((MUPR_Orig == 0x01) && (State != 0x1040)) WrongState |= 0x0001;
  if ((MUPR_Orig == 0x02) && (State != 0x1080)) WrongState |= 0x0001;
  }
if (MVKU0_Orig)
  {
  State = CurrConfig[1] & 0xFFFC;
  if ((MVKU0_Orig == 0x01) && (State != 0x2040)) WrongState |= 0x0002;
  if ((MVKU0_Orig == 0x02) && (State != 0x2080)) WrongState |= 0x0002;
  if ((MVKU0_Orig == 0x03) && (State != 0x2010)) WrongState |= 0x0002;
  }
if (MVKU1_Orig)
  {
  State = CurrConfig[2] & 0xFFFC;
  if ((MVKU1_Orig == 0x01) && (State != 0x2140)) WrongState |= 0x0004;
  if ((MVKU1_Orig == 0x02) && (State != 0x2180)) WrongState |= 0x0004;
  if ((MVKU1_Orig == 0x03) && (State != 0x2110)) WrongState |= 0x0004;
  }
if (MVKU2_Orig)
  {
  State = CurrConfig[3] & 0xFFFC;
  if ((MVKU2_Orig == 0x01) && (State != 0x2240)) WrongState |= 0x0008;
  if ((MVKU2_Orig == 0x02) && (State != 0x2280)) WrongState |= 0x0008;
  if ((MVKU2_Orig == 0x03) && (State != 0x2210)) WrongState |= 0x0008;
  }
if (MVMK0_Orig)
  {
  State = CurrConfig[4] & 0xFFFC;
  if ((MVMK0_Orig == 0x01) && (State != 0x3040)) WrongState |= 0x0010;
  if ((MVMK0_Orig == 0x02) && (State != 0x3080)) WrongState |= 0x0010;
  if ((MVMK0_Orig == 0x03) && (State != 0x3010)) WrongState |= 0x0010;
  }
if (MVMK1_Orig)
  {
  State = CurrConfig[5] & 0xFFFC;
  if ((MVMK1_Orig == 0x01) && (State != 0x3140)) WrongState |= 0x0020;
  if ((MVMK1_Orig == 0x02) && (State != 0x3180)) WrongState |= 0x0020;
  if ((MVMK1_Orig == 0x03) && (State != 0x3110)) WrongState |= 0x0020;
  }
if (MVMK2_Orig)
  {
  State = CurrConfig[6] & 0xFFFC;
  if ((MVMK2_Orig == 0x01) && (State != 0x3240)) WrongState |= 0x0040;
  if ((MVMK2_Orig == 0x02) && (State != 0x3280)) WrongState |= 0x0040;
  if ((MVMK2_Orig == 0x03) && (State != 0x3210)) WrongState |= 0x0040;
  }

if (MPVN_Exist) 
  {
  StateMVMK1 = CurrConfig[5] & 0xF8FC;
  StateMVMK2 = CurrConfig[6] & 0xF8FC;
  if (StateMVMK1 != StateMVMK2)
    {
    WrongState |= 0x0020;
    WrongState |= 0x0040;
    }
  if (MPVN1_Orig)
    {
    State = CurrConfig[7] & 0xFFFC;
    if ((MPVN1_Orig == 0x01) && (State != 0x5050) && (State != 0x5040)) { WrongState |= 0x0080; }
    if ((MPVN1_Orig == 0x02) && (State != 0x5090) && (State != 0x5080)) { WrongState |= 0x0080; }
    if ((MPVN1_Orig == 0x03) && (State != 0x5010)) { WrongState |= 0x0080; }
    }
  }

// один из модулей сконфигурирован неправильно/выставил биты ошибок, выдаем маску ошибок модулей 
if (WrongState)
  {
  ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE | REPLY_FLAG_ERROR, KPA_SUBADDR_IRQ, &WrongState, 2);
  return;
  }

// выдаем код сообщения "конфигурация ЛКА5 установлена" (код REPLY_CMD_DONE, спец кода для ЛКА5 пока нет)
ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ);

/*
адрес 4/17, чтение конфигурации
CD0/МУ    0x1040/1080 (xxxx0N00 - бит неисправности)
CD1/МВКУ0 0x2040/2080 (ВРК-ВОК-ЗРК-ЗОК-НРК-НОК-x-x - биты статуса)
CD2/МВКУ1 0x2140/2180
CD3/МВКУ2 0x2240/2280
CD4/МВМК0 0x3040/3080
CD5/МВМК1 0x3140/3180
CD6/МВМК2 0x3240/3280
CD7/МПВН  0x5050/5090

адрес 4/17, запись конфигурации
CD0/МУ    0x1000
CD1/МВКУ0 0x2040/2080
CD2/МВКУ1 0x2140/2180
CD3/МВКУ2 0x2240/2280
CD4/МВМК0 0x3040/3080
CD5/МВМК1 0x3140/3180
CD6/МВМК2 0x3240/3280
CD7/МПВН  0x5040/5080

адрес 2/29, запись конфигурации МУ
CD0/МУ-O 0x0008
CD0/МУ-P 0x0010
*/
}
//================================================================================

/*! \fn void KpaCommand0E (int BaseMkoLine, int BaseMkoChan);
 * \brief Функция обработки команды 0x0E: имитация массива ЦБК. Вызывается при обработке прерывания от КПА
 * \param [in] BaseMkoLine - номер линии МКО для обмена с КПА
 * \param [in] BaseMkoChan - номер канала МКО для обмена с КПА
 * \details <b> Формат данных команды: </b><br>
 * слово команды [0] биты [7..0] - код команды (0x0E) <br>
 * слово команды [1] - размер пакета, сообщений (1..32, если смена размера пакета разрешена в конфигурации СПО, иначе по умолчанию = 32) <br>
 * слово команды [2] - размер одного сообщения в пакете, слов (1..32, если смена размера пакета разрешена в конфигурации СПО, иначе по умолчанию = 32) <br>
 * <b> Работа команды: </b><br>
 * разрешается обработка прерывания от МБК04 <br>
 * с КПА считываются 32 слова данных и передаются на МБК04 как начальные <br>
 * производится передача результата выполнения на КПА <br>
 */

void KpaCommand0E (int BaseMkoLine, int BaseMkoChan)
{
int Result, Index, Pos, FlagMode;
int DevMkoLine, DevMkoChan;
WORD Cmd1, Cmd2, PckSize, MsgSize;

// ставим параметры пакетов/сообщений
PckSize = (SpoConfig.Mbk04ResizePacket) ? (KpaCommand[1]) : (32);
MsgSize = (SpoConfig.Mbk04ResizePacket) ? (KpaCommand[2]) : (32);
if ((PckSize < 1) || (PckSize > 32) || (MsgSize < 1) || (MsgSize > 32))
  {
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DATA_ERROR, KPA_SUBADDR_IRQ);
  return;
  }
SpoConfig.Mbk04PacketSize = PckSize;
SpoConfig.Mbk04MessageSize = MsgSize;

// параметры линий обмена с аппаратурой
DevMkoLine = SpoConfig.DevMkoLine & 1;
DevMkoChan = SpoConfig.DevMkoChan & 1;

// считывание пакета данных для МБК04
Result = MkoXchgBn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_MBK4DATA, MKO_RX, 32), 0);
if (Result)
  {
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, DECODE_XCHG_ERROR(Result), KPA_SUBADDR_IRQ);
  return;
  }

// проверяем размер считанного пакета данных
if (MkoMsgResB[0].Len != 32)
  {
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_NO_DATA_ERROR, KPA_SUBADDR_IRQ);
  return;
  }

// запрет прерывания от МБК4
SpoConfig.EnabledIrqMask &= (~0x0010);
sprIMaskVec(~SpoConfig.EnabledIrqMask);

if (!SpoConfig.EnableCbkDataImit)
  {
  // начальный флаг чет/нечет номера пакета
  SpoConfig.Irq4EvenMode = 0;
  // готовим 32 копии пакета для передачи
  for (Index = 0; Index < 32; Index++) memmove(&CbkDataImit[Index], &MkoBufB[0], sizeof(MKO_BUFFER));
  // готовим пакет с флагом "четный"
  CbkDataImit[32][0] = 0x01;

  // ставим маркеры типа и номера пакета
  Pos = SpoConfig.Mbk04MarkerPosition; 
  CbkDataImit[0][Pos + 0] = (SpoConfig.Irq4EvenMode & 1) + 1;  // маркер чет/нечет номера пакета
  CbkDataImit[0][Pos + 1] = 0;  // текущий номер пакета

  // передаем данные в буфер МКО
  memmove(&MkoBufB[0], &CbkDataImit[0], 33 * sizeof(MKO_BUFFER));
  // команда МКО на передачу сообщения в МБК04
  Cmd1 = MKO_CMD(4, 3, MKO_TX, SpoConfig.Mbk04MessageSize);  
  // команда МКО на передачу флага в МБК04
  Cmd2 = MKO_CMD(4, 8, MKO_TX, 1);  

  // передаем пакет на МБК04
  Result = MkoXchgBnBlock(DevMkoLine, DevMkoChan, Cmd1, Cmd2, SpoConfig.Mbk04PacketSize, MBK04_FLAG_AFTER_DATA);
  if (Result)
    {
    ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, DECODE_XCHG_ERROR(Result), KPA_SUBADDR_IRQ);
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

  // если включен режим двойной буферизации - сразу заполняем второй буфер МБК04
  if (SpoConfig.Mbk04DoubleBuffered)
    {
    // готовим пакет с флагом "нечетный" 
    CbkDataImit[32][0] = 0x80;

    // ставим маркеры типа и номера пакета
    Pos = SpoConfig.Mbk04MarkerPosition;
    CbkDataImit[0][Pos + 0] = (SpoConfig.Irq4EvenMode & 1) + 1;  // маркер чет/нечет номера пакета
    CbkDataImit[0][Pos + 1]++;  // текущий номер пакета

    // передаем данные в буфер МКО
    memmove(&MkoBufB[0], &CbkDataImit[0], 1 * sizeof(MKO_BUFFER));
    memmove(&MkoBufB[32], &CbkDataImit[32], 1 * sizeof(MKO_BUFFER));
    // команда МКО на передачу сообщения в МБК04
    Cmd1 = MKO_CMD(4, 4, MKO_TX, SpoConfig.Mbk04MessageSize);  
    // команда МКО на передачу флага в МБК04
    Cmd2 = MKO_CMD(4, 8, MKO_TX, 1);  

    // режим передачи флага данных
    FlagMode = (SpoConfig.Mbk04DataFlagAfterIrq) ? (MBK04_FLAG_NONE) : (MBK04_FLAG_AFTER_DATA);

    // передаем пакет на МБК04
    Result = MkoXchgBnBlock(DevMkoLine, DevMkoChan, Cmd1, Cmd2, SpoConfig.Mbk04PacketSize, FlagMode);
    if (Result)
      {
      ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, DECODE_XCHG_ERROR(Result), KPA_SUBADDR_IRQ);
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

  // ставим флаг "функция имитации активна"
  SpoConfig.EnableCbkDataImit = 1;
  }
else
  {
  // сохраняем номер пакета
  Pos = SpoConfig.Mbk04MarkerPosition;
  Cmd1 = CbkDataImit[0][Pos + 1];

  // готовим 32 копии пакета для передачи
  for (Index = 0; Index < 32; Index++)
    {
    memmove(&CbkDataImit[Index], &MkoBufB[0], sizeof(MKO_BUFFER));
    }

  // восстанавливаем номер пакета
  CbkDataImit[0][Pos + 1] = Cmd1;
  }

// разрешение прерывания от МБК4
SpoConfig.EnabledIrqMask |= 0x0010;
sprIMaskVec(~SpoConfig.EnabledIrqMask);

// смена текущего статуса режима
SpoConfig.CbkDataImitStatus = STATE_WORK;

ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ);
}
//================================================================================

/*! \fn void KpaCommand0F (int BaseMkoLine, int BaseMkoChan);
 * \brief Функция обработки команды 0x0F: установить заданную конфигурацию АСН. Вызывается при обработке прерывания от КПА
 * \param [in] BaseMkoLine - номер линии МКО для обмена с КПА
 * \param [in] BaseMkoChan - номер канала МКО для обмена с КПА
 * \details <b> Формат данных команды: </b><br>
 * слово команды [0] биты [7..0] - код команды (0x0F) <br>
 * слово команды [1] биты [1..0] - маска конфигурации АСН (0 = выключить все, 1 = вкл канал 1, 2 = вкл канал 2, 3 = ошибка в команде) <br>
 * слово команды [1] биты [10..8] - номер банка НП (0..5) <br>
 * слово команды [1] биты [14..12] - номер банка СП (0..5) <br>
 * <b> Работа команды: </b><br>
 * производится проверка параметров команды на допустимость <br>
 * производится переключение конфигурации АСН <br>
 * производится проверка исправности конфигурации АСН <br>
 * производится передача результата выполнения на КПА <br>
 */

#define  ADDR_ASN  3		// адрес АСН на шине МКО

void KpaCommand0F (int BaseMkoLine, int BaseMkoChan)
{
int Result, DevMkoLine, DevMkoChan;
WORD Chan_Orig, Chan_Want;
WORD NP_Orig, SP_Orig;

// параметры линий обмена
DevMkoLine = SpoConfig.DevMkoLine & 1;
DevMkoChan = SpoConfig.DevMkoChan & 1;

// биты новой конфигурации блока
Chan_Orig = (KpaCommand[1] >> 0) & 0x03;  Chan_Want = Chan_Orig;
NP_Orig = (KpaCommand[1] >> 8) & 0x0F; 
SP_Orig = (KpaCommand[1] >> 12) & 0x0F;

// не требуется смена конфигурации
if ((Chan_Want == 0x03) || (NP_Orig > 5) || (SP_Orig > 5))
  {
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DATA_ERROR, KPA_SUBADDR_IRQ);
  return;
  }

if (Chan_Want == 0x01)
  {
  // включаем канал 1
  MkoBufB[0][0] = 0x1001;
  Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(4, 28, MKO_TX, 1), 0);
  if (Result)
    {
    ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, DECODE_XCHG_ERROR(Result), KPA_SUBADDR_IRQ);
    return;
    }
  SleepMSec(15000); // ждем переключения каналов АСН

  MkoBufB[0][0] = ((SP_Orig + 1) << 12) | ((SP_Orig + 1) << 8) | ((NP_Orig + 1) << 4) | ((NP_Orig + 1) << 0);
  Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(ADDR_ASN, 29, MKO_TX, 1), 0);
  if (Result)
    {
    ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, DECODE_XCHG_ERROR(Result), KPA_SUBADDR_IRQ);
    return;
    }
  SleepMSec(10000); // ждем загрузки банков АСН 

  // считываем состояние АСН  
  Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(ADDR_ASN, 8, MKO_RX, 30), 0);
  if (Result)
    {
    ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, DECODE_XCHG_ERROR(Result), KPA_SUBADDR_IRQ);
    return;
    }

  // проверка исправности АСН после переключения комплектов
  if ((MkoBufB[0][0] & 0x0001) == 0)
    {
    ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_ASN_RECONFIG_ERROR, KPA_SUBADDR_IRQ);
    return;
    }

  // канал успешно включен
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ);
  return;
  }

if (Chan_Want == 0x02)
  {
  // включаем канал 2
  MkoBufB[0][0] = 0x1002;
  Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(4, 28, MKO_TX, 1), 0);
  if (Result)
    {
    ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, DECODE_XCHG_ERROR(Result), KPA_SUBADDR_IRQ);
    return;
    }
  SleepMSec(15000); // ждем переключения каналов АСН

  MkoBufB[0][0] = ((SP_Orig + 1) << 12) | ((SP_Orig + 1) << 8) | ((NP_Orig + 1) << 4) | ((NP_Orig + 1) << 0);
  Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(ADDR_ASN, 29, MKO_TX, 1), 0);
  if (Result)
    {
    ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, DECODE_XCHG_ERROR(Result), KPA_SUBADDR_IRQ);
    return;
    }
  SleepMSec(10000); // ждем загрузки банков АСН 

  // считываем состояние АСН  
  Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(ADDR_ASN, 8, MKO_RX, 30), 0);
  if (Result)
    {
    ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, DECODE_XCHG_ERROR(Result), KPA_SUBADDR_IRQ);
    return;
    }

  // проверка исправности АСН после переключения комплектов
  if ((MkoBufB[0][0] & 0x0001) == 0)
    {
    ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_ASN_RECONFIG_ERROR, KPA_SUBADDR_IRQ);
    return;
    }

  // канал успешно включен
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ);
  return;
  }

if (Chan_Want == 0x00)
  {
  // выключаем все каналы
  MkoBufB[0][0] = 0x1004;
  Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(4, 28, MKO_TX, 1), 0);
  if (Result)
    {
    ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, DECODE_XCHG_ERROR(Result), KPA_SUBADDR_IRQ);
    return;
    }

  // ждем отключения АСН
  SleepMSec(300);

  // считываем состояние АСН  
  Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(ADDR_ASN, 8, MKO_RX, 30), 0);
  if (Result)
    {
    // АСН успешно отключен
    ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ);
    return;
    }

  // АСН не отключился
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE | REPLY_FLAG_ERROR, KPA_SUBADDR_IRQ);
  return;
  }

// на случай сбоя СПО
ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_INTERNAL_DATA_ERROR, KPA_SUBADDR_IRQ);
}
//================================================================================

/*! \fn void KpaCommand10 (int BaseMkoLine, int BaseMkoChan);
 * \brief Функция обработки команды 0x10: установить заданную конфигурацию БУП-Е. Вызывается при обработке прерывания от КПА
 * \param [in] BaseMkoLine - номер линии МКО для обмена с КПА
 * \param [in] BaseMkoChan - номер канала МКО для обмена с КПА
 * \details <b> Формат данных команды: </b><br>
 * слово команды [0] биты [7..0] - код команды (0x10) <br>
 * слово команды [1] биты [1..0] - маска конфигурации каналов БУП-Е (0 = выключить блок, 1 = вкл канал 1, 2 = вкл канал 2, 3 = ошибка в команде) <br>
 * <b> Работа команды: </b><br>
 * производится проверка параметров команды на допустимость <br>
 * производится считывание и анализ текущей конфигурации БУП-Е <br>
 * если требуется - производится переключение конфигурации БУП-Е <br>
 * производится считывание и анализ новой конфигурации БУП-Е <br>
 * производится передача результата выполнения на КПА <br>
 */

#define  ADDR_BUP_E  9		// адрес БУП-Е на шине МКО

void KpaCommand10 (int BaseMkoLine, int BaseMkoChan)
{
int Result, DevMkoLine, DevMkoChan;
WORD Chan_Orig, Chan_Want;

// параметры линий обмена
DevMkoLine = SpoConfig.DevMkoLine & 1;
DevMkoChan = SpoConfig.DevMkoChan & 1;

// биты новой конфигурации блока
Chan_Orig = (KpaCommand[1] >> 0) & 0x03;  Chan_Want = Chan_Orig;

// состояние запрещено
if (Chan_Want == 0x03)
  {
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DATA_ERROR, KPA_SUBADDR_IRQ);
  return;
  }

// считываем состояние БУП
Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(ADDR_BUP_E, 2, MKO_RX, 13), 0);
if (!Result)
  {
  // БУП отвечает по МКО - включен

  // командный массив на отключение ШД и ДУ (6 нулей)
  memset(&MkoBufB[0], 0, sizeof(MkoBufB[0]));

  // передача команды управления БУП-Е
  Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(ADDR_BUP_E, 3, MKO_TX, 6), 0);
  if (Result)
    {
    ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, DECODE_XCHG_ERROR(Result), KPA_SUBADDR_IRQ);
    return;
    }
  
  // пауза на отключение ШД и ДУ
  SleepMSec(100);

  // считываем состояние БУП
  Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(ADDR_BUP_E, 2, MKO_RX, 13), 0);
  if (Result)
    {
    ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, DECODE_XCHG_ERROR(Result), KPA_SUBADDR_IRQ);
    return;
    }  
  SleepMSec(50);
  
  if (((MkoBufB[0][0] & 0x000F) != 0x0000) || (MkoBufB[0][5] != 0) || (MkoBufB[0][6] != 0))
    {
    ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_BUPE_BUS_DISABLE_ERROR, KPA_SUBADDR_IRQ);
    return;
    }

  // передача команды выключения каналов БУП-Е
  MkoBufB[0][0] = 0x1080;
  Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(4, 28, MKO_TX, 1), 0);
  if (Result)
    {
    ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, DECODE_XCHG_ERROR(Result), KPA_SUBADDR_IRQ);
    return;
    }

  // пауза на отключение каналов
  SleepMSec(2500);

  // считываем состояние БУП
  Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(ADDR_BUP_E, 2, MKO_RX, 13), 0);
  if (!Result)
    {
    ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_BUPE_OFF_ERROR, KPA_SUBADDR_IRQ);
    return;
    }  
  }

// БУП не отвечает по МКО - выключен

if (Chan_Want == 0x00)
  {
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ);
  return;
  }

if (Chan_Want == 0x01)
  {
  // включение канала 1 БУП-Е
  BUPE_Chan1_Enable(BaseMkoLine, BaseMkoChan, DevMkoLine, DevMkoChan);
  return;
  }

if (Chan_Want == 0x02)
  {
  // включение канала 2 БУП-Е
  BUPE_Chan2_Enable(BaseMkoLine, BaseMkoChan, DevMkoLine, DevMkoChan);
  return;
  }

// на случай сбоя СПО
ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_INTERNAL_DATA_ERROR, KPA_SUBADDR_IRQ);

// БУП - +2 слова к размеру конфигурации:
// контр сумма теперь в 12 слове (сумма от слов с 0 по 11)
// проверять 0 слово 7 бит - 0 = 1 канал, 1 = 2 канал
}
//================================================================================

/*! \fn void BUPE_Chan1_Enable (int BaseMkoLine, int BaseMkoChan, int DevMkoLine, int DevMkoChan);
 * \brief Служебная функция включения 1-го канала БУП-Е
 * \param [in] BaseMkoLine - номер линии МКО для обмена с КПА
 * \param [in] BaseMkoChan - номер канала МКО для обмена с КПА
 * \param [in] DevMkoLine - номер линии МКО для обмена с БА
 * \param [in] DevMkoChan - номер канала МКО для обмена с БА
 * \details <b> Работа команды: </b><br>
 * производится включение 1-го канала БУП-Е <br>
 * производится считывание и проверка текущей конфигурации БУП-Е <br>
 * при ошибках производится передача результата выполнения на КПА <br>
 */

void BUPE_Chan1_Enable (int BaseMkoLine, int BaseMkoChan, int DevMkoLine, int DevMkoChan)
{
int Result, Index;
WORD Sum, Buffer[2];

// передача команды включения канала 1
MkoBufB[0][0] = 0x1020;
Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(4, 28, MKO_TX, 1), 0);
if (Result)
  {
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, DECODE_XCHG_ERROR(Result), KPA_SUBADDR_IRQ);
  return;
  }

// пауза на включение канала
SleepMSec(3500);

// считываем состояние БУП
Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(ADDR_BUP_E, 2, MKO_RX, 13), 0);
if (Result)
  {
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, DECODE_XCHG_ERROR(Result), KPA_SUBADDR_IRQ);
  return;
  }

// расчет контрольной суммы    
Sum = 0x0000;
for (Index = 0; Index <= 11; Index++) Sum += MkoBufB[0][Index];
    
if (Sum != MkoBufB[0][12]) 
  {
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_DEVICE_CRC_ERROR, KPA_SUBADDR_IRQ);
  return;
  }

// не установлены все биты исправности
if (MkoBufB[0][9] != 0x80FF) 
  {
  Buffer[0] = MkoBufB[0][0];
  Buffer[1] = MkoBufB[0][9];
  ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_BUPE_ON_ERROR, KPA_SUBADDR_IRQ, &Buffer, 2);
  return;
  }

// неправильно установлен бит текущей конфигурации
if ((MkoBufB[0][0] & 0x0080) != 0) 
  {
  Buffer[0] = MkoBufB[0][0];
  Buffer[1] = MkoBufB[0][9];
  ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_BUPE_ON_ERROR, KPA_SUBADDR_IRQ, &Buffer, 2);
  return;
  }

ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ);
}
//================================================================================

/*! \fn void BUPE_Chan2_Enable (int BaseMkoLine, int BaseMkoChan, int DevMkoLine, int DevMkoChan);
 * \brief Служебная функция включения 2-го канала БУП-Е
 * \param [in] BaseMkoLine - номер линии МКО для обмена с КПА
 * \param [in] BaseMkoChan - номер канала МКО для обмена с КПА
 * \param [in] DevMkoLine - номер линии МКО для обмена с БА
 * \param [in] DevMkoChan - номер канала МКО для обмена с БА
 * \details <b> Работа команды: </b><br>
 * производится включение 2-го канала БУП-Е <br>
 * производится считывание и проверка текущей конфигурации БУП-Е <br>
 * при ошибках производится передача результата выполнения на КПА <br>
 */

void BUPE_Chan2_Enable (int BaseMkoLine, int BaseMkoChan, int DevMkoLine, int DevMkoChan)
{
int Result, Index;
WORD Sum, Buffer[2];

// передача команды включения канала 2
MkoBufB[0][0] = 0x1040;
Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(4, 28, MKO_TX, 1), 0);
if (Result)
  {
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, DECODE_XCHG_ERROR(Result), KPA_SUBADDR_IRQ);
  return;
  }

// пауза на включение канала
SleepMSec(3500);

// считываем состояние БУП
Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(ADDR_BUP_E, 2, MKO_RX, 13), 0);
if (Result)
  {
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, DECODE_XCHG_ERROR(Result), KPA_SUBADDR_IRQ);
  return;
  }

// расчет контрольной суммы    
Sum = 0x0000;
for (Index = 0; Index <= 11; Index++) Sum += MkoBufB[0][Index];

if (Sum != MkoBufB[0][12]) 
  {
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_DEVICE_CRC_ERROR, KPA_SUBADDR_IRQ);
  return;
  }

// не установлены все биты исправности
if (MkoBufB[0][9] != 0x80FF) 
  {
  Buffer[0] = MkoBufB[0][0];
  Buffer[1] = MkoBufB[0][9];
  ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_BUPE_ON_ERROR, KPA_SUBADDR_IRQ, &Buffer, 2);
  return;
  }

// неправильно установлен бит текущей конфигурации
if ((MkoBufB[0][0] & 0x0080) == 0) 
  {
  Buffer[0] = MkoBufB[0][0];
  Buffer[1] = MkoBufB[0][9];
  ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_BUPE_ON_ERROR, KPA_SUBADDR_IRQ, &Buffer, 2);
  return;
  }

ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ);
}
//================================================================================

/*! \fn void KpaCommand11 (int BaseMkoLine, int BaseMkoChan);
 * \brief Функция обработки команды 0x11: запрет имитации массива ЦБК. Вызывается при обработке прерывания от КПА
 * \param [in] BaseMkoLine - номер линии МКО для обмена с КПА
 * \param [in] BaseMkoChan - номер канала МКО для обмена с КПА
 * \details <b> Формат данных команды: </b><br>
 * слово команды [0] биты [7..0] - код команды (0x11) <br>
 * <b> Работа команды: </b><br>
 * запрещается обработка прерывания от МБК04 <br>
 * производится передача результата выполнения на КПА <br>
 */

void KpaCommand11 (int BaseMkoLine, int BaseMkoChan)
{
// запрет прерывания от МБК4
SpoConfig.EnabledIrqMask &= (~0x0010);
sprIMaskVec(~SpoConfig.EnabledIrqMask);

// сбрасываем флаг "функция имитации активна"
SpoConfig.EnableCbkDataImit = 0;

// смена текущего статуса режима
SpoConfig.CbkDataImitStatus = STATE_STOP;

ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ);
}
//================================================================================

/*! \fn void KpaCommand13 (int BaseMkoLine, int BaseMkoChan);
 * \brief Функция обработки команды 0x13: включение имитации СЧБК. Вызывается при обработке прерывания от КПА
 * \param [in] BaseMkoLine - номер линии МКО для обмена с КПА
 * \param [in] BaseMkoChan - номер канала МКО для обмена с КПА
 * \details <b> Формат данных команды: </b><br>
 * слово команды [0] биты [7..0] - код команды (0x13) <br>
 * <b> Работа команды: </b><br>
 * устанавливается флаг СПО разрешающий имитацию СЧБК (флаг анализируется в цикле ПДО) <br>
 * производится передача результата выполнения на КПА <br>
 */

// включение имитации СЧБК
// (флаг анализируется в цикле ПДО)

void KpaCommand13 (int BaseMkoLine, int BaseMkoChan)
{
SpoConfig.ShbkImitEnabled = 1; // включение имитации СЧБК
SpoConfig.ShbkImitStatus = STATE_WORK; // смена текущего статуса режима
SpoConfig.ShbkImitMaskReady = 0; // флаг готовности значения СЧБК
SpoConfig.ShbkFailCount = 0; // начальное число ошибок имитации

ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ);
}
//================================================================================

/*! \fn void KpaCommand14 (int BaseMkoLine, int BaseMkoChan);
 * \brief Функция обработки команды 0x14: включение имитации квитанций. Вызывается при обработке прерывания от КПА
 * \param [in] BaseMkoLine - номер линии МКО для обмена с КПА
 * \param [in] BaseMkoChan - номер канала МКО для обмена с КПА
 * \details <b> Формат данных команды: </b><br>
 * слово команды [0] биты [7..0] - код команды (0x14) <br>
 * <b> Работа команды: </b><br>
 * устанавливается флаг СПО разрешающий имитацию квитанций (флаг анализируется в цикле ПДО) <br>
 * производится передача результата выполнения на КПА <br>
 */

void KpaCommand14 (int BaseMkoLine, int BaseMkoChan)
{
// включение имитации квитанций в цикле ПДО
SpoConfig.EnabledImitReply = 1;
SpoConfig.ImitReplyIndex = 7;

// смена текущего статуса режима
SpoConfig.ImitReplyStatus = STATE_WORK;

// начальное число ошибок имитации
SpoConfig.ReplyFailCount = 0;

ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ);
}
//================================================================================

/*! \fn void KpaCommand15 (int BaseMkoLine, int BaseMkoChan);
 * \brief Функция обработки команды 0x15: отключение имитации квитанций. Вызывается при обработке прерывания от КПА
 * \param [in] BaseMkoLine - номер линии МКО для обмена с КПА
 * \param [in] BaseMkoChan - номер канала МКО для обмена с КПА
 * \details <b> Формат данных команды: </b><br>
 * слово команды [0] биты [7..0] - код команды (0x15) <br>
 * <b> Работа команды: </b><br>
 * сбрасывает флаг СПО разрешающий имитацию квитанций (флаг анализируется в цикле ПДО) <br>
 * производится передача результата выполнения на КПА <br>
 */

void KpaCommand15 (int BaseMkoLine, int BaseMkoChan)
{
SpoConfig.EnabledImitReply = 0;

// смена текущего статуса режима
SpoConfig.ImitReplyStatus = STATE_STOP;

ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ);
}
//================================================================================

/*! \fn void KpaCommand16 (int BaseMkoLine, int BaseMkoChan);
 * \brief Функция обработки команды 0x16: отключение имитации СЧБК. Вызывается при обработке прерывания от КПА
 * \param [in] BaseMkoLine - номер линии МКО для обмена с КПА
 * \param [in] BaseMkoChan - номер канала МКО для обмена с КПА
 * \details <b> Формат данных команды: </b><br>
 * слово команды [0] биты [7..0] - код команды (0x16) <br>
 * <b> Работа команды: </b><br>
 * сбрасывает флаг СПО разрешающий имитацию СЧБК (флаг анализируется в цикле ПДО) <br>
 * производится передача результата выполнения на КПА <br>
 */

void KpaCommand16 (int BaseMkoLine, int BaseMkoChan)
{
SpoConfig.ShbkImitEnabled = 0; // отключение имитации СЧБК 
SpoConfig.ShbkImitStatus = STATE_STOP; // смена текущего статуса режима

ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ);
}
//================================================================================

/*! \fn void KpaCommand17 (int BaseMkoLine, int BaseMkoChan);
 * \brief Функция обработки команды 0x17: запуск теста БТПО ЦБК или считывание результатов тестов. Вызывается при обработке прерывания от КПА
 * \param [in] BaseMkoLine - номер линии МКО для обмена с КПА
 * \param [in] BaseMkoChan - номер канала МКО для обмена с КПА
 * \details <b> Формат данных команды: </b><br>
 * слово команды [0] биты [7..0] - код команды (0x17) <br>
 * слово команды [1] - режим команды (0 = запуск теста, 1 = считывание краткого МКИ, 2 = считывание полного МКИ) <br>
 * слово команды [2] - если режим = 0, содержит номер теста БТПО (1..6) <br>
 * <b> Работа команды: </b><br>
 * производится проверка параметров команды на допустимость <br>
 * производится вызов встроенного теста БТПО (режим 0) или передача результатов тестирования (режимы 1 и 2) <br>
 * производится ожидание перезапуска СПО (режим 0) <br>
 * производится передача результата выполнения на КПА (режимы 1 и 2) <br>
 */

void KpaCommand17 (int BaseMkoLine, int BaseMkoChan)
{
WORD Mode, TestNum;
STATUS Status;

Mode = KpaCommand[1];
TestNum = KpaCommand[2];

switch (Mode)
  {
  case 0: // запуск теста БТПО
    if ((TestNum < 1) || (TestNum > 6)) SEND_RESULT(REPLY_CMD_DATA_ERROR);
    Status = testStart(TestNum);
    if (Status == FATAL) SEND_RESULT(REPLY_CMD_DONE | REPLY_FLAG_ERROR);
    while (1) {} // после завершения теста ждем перезагрузку

  case 1: // считывание данных МКИ из буфера 
    ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ, &PostMkiDataBuf, PostMkiDataSize);
    return;

  case 2: // считывание данных МДИ из буфера 
    ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ, &PostMdiDataBuf, PostMdiDataSize);
    return;

  case 3: // обновление данных МКИ в буфере и считывание
    MainTaskGetMkiData();
    ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ, &PostMkiDataBuf, PostMkiDataSize);
    return;

  case 4: // обновление данных МДИ в буфере и считывание
    MainTaskGetMdiData();
    ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ, &PostMdiDataBuf, PostMdiDataSize);
    return;

  default: // ошибка в параметрах команды
    SEND_RESULT(REPLY_CMD_DATA_ERROR);
  }
}
//================================================================================

/*! \fn void KpaCommand18 (int BaseMkoLine, int BaseMkoChan);
 * \brief Функция обработки команды 0x18: запрос конфигурации ВМ ЦБК. Вызывается при обработке прерывания от КПА
 * \param [in] BaseMkoLine - номер линии МКО для обмена с КПА
 * \param [in] BaseMkoChan - номер канала МКО для обмена с КПА
 * \details <b> Формат данных команды: </b><br>
 * слово команды [0] биты [7..0] - код команды (0x18) <br>
 * <b> Работа команды: </b><br>
 * производится запрос к ОПО на выдачу текущей конфигурации ВМ ЦБК <br>
 * производится передача результата выполнения и данных конфигурации на КПА <br>
 */

void KpaCommand18 (int BaseMkoLine, int BaseMkoChan)
{
STATUS Status;
cfgSYS_t Config;

Status = cfgSysGet(&Config);
if (Status != OK)
  {
  // ошибка получения конфигурации ЦБК
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE | REPLY_FLAG_ERROR, KPA_SUBADDR_IRQ);
  return;
  }

// передача 32бит описателя конфигурации ВМ ЦБК на КПА
ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ, &Config, sizeof(Config));
}
//================================================================================

/*! \fn void KpaCommand19 (int BaseMkoLine, int BaseMkoChan);
 * \brief Функция обработки команды 0x19: запрос на коррекцию РПЗУ. Вызывается при обработке прерывания от КПА
 * \param [in] BaseMkoLine - номер линии МКО для обмена с КПА
 * \param [in] BaseMkoChan - номер канала МКО для обмена с КПА
 * \details <b> Формат данных команды: </b><br>
 * слово команды [0] биты [7..0] - код команды (0x19) <br>
 * <b> Работа команды: </b><br>
 * Алгоритм не определен в ТЗ, команда всегда возвращает ошибку <br>
 */

/*
1 - стереть сектор
2 - проверка сектора на чистоту
3 - сброс буфера сектора
4 - уст линейный адрес для записи
5 - счит линейный адрес
6 - добавить данные в буфер
7 - счит размер данных в буфере
8 - запись со стиранием сектора
9 - запись без стирания сектора
*/

void KpaCommand19 (int BaseMkoLine, int BaseMkoChan)
{
int OpCode, Sector, Result, Index, Size, Mode;
unsigned *Addr;
WORD Cmd, Buffer[32];

OpCode = KpaCommand[1];

if (OpCode == 1) // стереть сектор
  {
  Sector = KpaCommand[2];
  if ((Sector < 2) || (Sector > 7)) SEND_RESULT(REPLY_CMD_DATA_ERROR); 

  // формируем задание на стирание сектора
  memset(&FlashJob, 0, sizeof(FlashJob)); // сброс задания на запись

  FlashJob.sgn = 0xF15E;
  FlashJob.erase_map = 1 << Sector;
  FlashJob.typ_exit = BBP_RETURN;

  // стираем сектор с возвратом в СПО
  if (flashCorr(&FlashJob, 0x10) != OK) SEND_RESULT(REPLY_CMD_DONE | REPLY_FLAG_ERROR);

  SEND_RESULT(REPLY_CMD_DONE);
  }
else if (OpCode == 2) // проверка сектора на чистоту
  {
  Sector = KpaCommand[2];
  if ((Sector < 2) || (Sector > 7)) SEND_RESULT(REPLY_CMD_DATA_ERROR); 

  Addr = (unsigned*)(0x1FC00000 | (Sector << 18));
  Result = REPLY_CMD_DONE;

  for (Index = 0; Index < 65536; Index++, Addr++)
    {
    if (*Addr != 0xFFFFFFFF) { Result = REPLY_CMD_DONE | REPLY_FLAG_ERROR; break; }
    }

  SEND_RESULT(Result);
  }
else if (OpCode == 3) // сброс буфера сектора
  {
  FlashDataIndex = 0; // текущий индекс данных сектора флеша
  SEND_RESULT(REPLY_CMD_DONE);
  }
else if (OpCode == 4) // уст линейный адрес для записи
  {
  // извлекаем линейный адрес из команды
  FlashDataAddr = MAKE_LONG(KpaCommand[2], KpaCommand[3]);
  SEND_RESULT(REPLY_CMD_DONE);
  }
else if (OpCode == 5) // счит линейный адрес
  {
  ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ, &FlashDataAddr, sizeof(FlashDataAddr));
  }
else if (OpCode == 6) // добавить данные в буфер
  {
  Size = KpaCommand[2];
  if ((Size < 0) | (Size > 64) | (Size & 0x03)) SEND_RESULT(REPLY_CMD_DATA_ERROR); 
  if ((FlashDataIndex + Size) > sizeof(FlashDataBuffer)) SEND_RESULT(REPLY_CMD_DATA_ERROR);  

  Cmd = MKO_CMD(KPA_ADDR, KPA_SUBADDR_CMD + 1, MKO_RX, Size / sizeof(WORD));
  Result = ReadExtMsgData(BaseMkoLine, BaseMkoChan, Cmd, KpaCommand[7], (WORD*) &Buffer);
  if (Result != REPLY_CMD_DONE) SEND_RESULT(Result);

  memmove(&FlashDataBuffer[FlashDataIndex], &Buffer, Size);
  FlashDataIndex += Size;

  SEND_RESULT(REPLY_CMD_DONE);
  }
else if (OpCode == 7) // счит размер данных в буфере
  {
  ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ, &FlashDataIndex, sizeof(FlashDataIndex));
  }
else if (OpCode == 8) // запись со стиранием сектора
  {
  Sector = KpaCommand[2];
  Mode = KpaCommand[3];
  if ((Sector < 2) || (Sector > 7)) SEND_RESULT(REPLY_CMD_DATA_ERROR); 
  if ((Mode != 0x10) && (Mode != 0x12)) SEND_RESULT(REPLY_CMD_DATA_ERROR); 
  if (!FlashDataIndex) SEND_RESULT(REPLY_CMD_DATA_ERROR); 

  memset(&FlashJob, 0, sizeof(FlashJob)); // сброс задания на запись

  // формируем задание на запись сектора
  FlashJob.sgn = 0xF15E;
  FlashJob.erase_map = 1 << Sector;
  FlashJob.typ_exit = BBP_RETURN;

  FlashJob.dop[0].size = FlashDataIndex;
  FlashJob.dop[0].from = (void*) &FlashDataBuffer;
  FlashJob.dop[0].to   = (void*) FlashDataAddr;

  // пишем сектор с возвратом в СПО
  if (flashCorr(&FlashJob, Mode) != OK) SEND_RESULT(REPLY_ERROR_POINT_1);

  // сверяем записанные данные
  if (memcmp(FlashJob.dop[0].from, FlashJob.dop[0].to, FlashJob.dop[0].size)) SEND_RESULT(REPLY_ERROR_POINT_2);

  SEND_RESULT(REPLY_CMD_DONE);
  }
else if (OpCode == 9) // запись без стирания сектора
  {
  Mode = KpaCommand[2];
  if ((Mode != 0x10) && (Mode != 0x12)) SEND_RESULT(REPLY_CMD_DATA_ERROR); 
  if (!FlashDataIndex) SEND_RESULT(REPLY_CMD_DATA_ERROR); 

  memset(&FlashJob, 0, sizeof(FlashJob)); // сброс задания на запись

  // формируем задание на запись сектора
  FlashJob.sgn = 0xF15E;
  FlashJob.erase_map = 0;
  FlashJob.typ_exit = BBP_RETURN;

  FlashJob.dop[0].size = FlashDataIndex;
  FlashJob.dop[0].from = (void*) &FlashDataBuffer;
  FlashJob.dop[0].to   = (void*) FlashDataAddr;

  // пишем сектор с возвратом в СПО
  if (flashCorr(&FlashJob, Mode) != OK) SEND_RESULT(REPLY_ERROR_POINT_1);

  // сверяем записанные данные
  if (memcmp(FlashJob.dop[0].from, FlashJob.dop[0].to, FlashJob.dop[0].size)) SEND_RESULT(REPLY_ERROR_POINT_2);

  SEND_RESULT(REPLY_CMD_DONE);
  }
else 
  {
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DATA_ERROR, KPA_SUBADDR_IRQ);
  }

// FlashJob
// FlashDataBuffer;
// FlashDataIndex
// FlashDataAddr
}
//================================================================================

/*! \fn void KpaCommand20 (int BaseMkoLine, int BaseMkoChan);
 * \brief Функция обработки команды 0x20: режим обмена МКО-0 (установить 1-ю линию МКО для обмена с КПА). Вызывается при обработке прерывания от КПА
 * \param [in] BaseMkoLine - номер линии МКО для обмена с КПА
 * \param [in] BaseMkoChan - номер канала МКО для обмена с КПА
 * \details <b> Формат данных команды: </b><br>
 * слово команды [0] биты [7..0] - код команды (0x20) <br>
 * <b> Работа команды: </b><br>
 * в конфигурации СПО сохраняется новый номер линии МКО для обменов с КПА <br>
 * квитанция в ответ на команду выдается по предыдущей рабочей линии МКО <br>
 */

void KpaCommand20 (int BaseMkoLine, int BaseMkoChan)
{
SpoConfig.KpaMkoLine = 0;

ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ);
}
//================================================================================

/*! \fn void KpaCommand21 (int BaseMkoLine, int BaseMkoChan);
 * \brief Функция обработки команды 0x21: режим обмена МКО-1 (установить 2-ю линию МКО для обмена с КПА). Вызывается при обработке прерывания от КПА
 * \param [in] BaseMkoLine - номер линии МКО для обмена с КПА
 * \param [in] BaseMkoChan - номер канала МКО для обмена с КПА
 * \details <b> Формат данных команды: </b><br>
 * слово команды [0] биты [7..0] - код команды (0x21) <br>
 * <b> Работа команды: </b><br>
 * в конфигурации СПО сохраняется новый номер линии МКО для обменов с КПА <br>
 * квитанция в ответ на команду выдается по предыдущей рабочей линии МКО <br>
 */

void KpaCommand21 (int BaseMkoLine, int BaseMkoChan)
{
SpoConfig.KpaMkoLine = 1;

ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ);
}
//================================================================================

/*! \fn void KpaCommand22 (int BaseMkoLine, int BaseMkoChan);
 * \brief Функция обработки команды 0x22: запрос на выключение ЦБК. Вызывается при обработке прерывания от КПА
 * \param [in] BaseMkoLine - номер линии МКО для обмена с КПА
 * \param [in] BaseMkoChan - номер канала МКО для обмена с КПА
 * \details <b> Формат данных команды: </b><br>
 * слово команды [0] биты [7..0] - код команды (0x22) <br>
 * <b> Работа команды: </b><br>
 * Алгоритм не определен в ТЗ, команда всегда возвращает ошибку <br>
 */

void KpaCommand22 (int BaseMkoLine, int BaseMkoChan)
{
ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE | REPLY_FLAG_ERROR, KPA_SUBADDR_IRQ);
}
//================================================================================

/*! \fn void KpaCommand23 (int BaseMkoLine, int BaseMkoChan);
 * \brief Функция обработки команды 0x23: запрос на включение ВМ1 ЦБК. Вызывается при обработке прерывания от КПА
 * \param [in] BaseMkoLine - номер линии МКО для обмена с КПА
 * \param [in] BaseMkoChan - номер канала МКО для обмена с КПА
 * \details <b> Формат данных команды: </b><br>
 * слово команды [0] биты [7..0] - код команды (0x23) <br>
 * <b> Работа команды: </b><br>
 * производится включение канала ВМ1 ЦБК <br>
 * производится запрос новой конфигурации ВМ ЦБК и проверка включения канала <br>
 * производится передача результата выполнения на КПА <br>
 * <b> Внимание: </b><br>
 * команда не работает корректно с текущей версией ОПО, применять не рекомендуется <br>
 */

void KpaCommand23 (int BaseMkoLine, int BaseMkoChan)
{
CbkChanPowerOn(BaseMkoLine, BaseMkoChan, 0);
}
//================================================================================

/*! \fn void KpaCommand24 (int BaseMkoLine, int BaseMkoChan);
 * \brief Функция обработки команды 0x24: запрос на включение ВМ2 ЦБК. Вызывается при обработке прерывания от КПА
 * \param [in] BaseMkoLine - номер линии МКО для обмена с КПА
 * \param [in] BaseMkoChan - номер канала МКО для обмена с КПА
 * \details <b> Формат данных команды: </b><br>
 * слово команды [0] биты [7..0] - код команды (0x24) <br>
 * <b> Работа команды: </b><br>
 * производится включение канала ВМ2 ЦБК <br>
 * производится запрос новой конфигурации ВМ ЦБК и проверка включения канала <br>
 * производится передача результата выполнения на КПА <br>
 * <b> Внимание: </b><br>
 * команда не работает корректно с текущей версией ОПО, применять не рекомендуется <br>
 */

void KpaCommand24 (int BaseMkoLine, int BaseMkoChan)
{
CbkChanPowerOn(BaseMkoLine, BaseMkoChan, 1);
}
//================================================================================

/*! \fn void KpaCommand25 (int BaseMkoLine, int BaseMkoChan);
 * \brief Функция обработки команды 0x25: запрос на включение ВМ3 ЦБК. Вызывается при обработке прерывания от КПА
 * \param [in] BaseMkoLine - номер линии МКО для обмена с КПА
 * \param [in] BaseMkoChan - номер канала МКО для обмена с КПА
 * \details <b> Формат данных команды: </b><br>
 * слово команды [0] биты [7..0] - код команды (0x25) <br>
 * <b> Работа команды: </b><br>
 * производится включение канала ВМ3 ЦБК <br>
 * производится запрос новой конфигурации ВМ ЦБК и проверка включения канала <br>
 * производится передача результата выполнения на КПА <br>
 * <b> Внимание: </b><br>
 * команда не работает корректно с текущей версией ОПО, применять не рекомендуется <br>
 */

void KpaCommand25 (int BaseMkoLine, int BaseMkoChan)
{
CbkChanPowerOn(BaseMkoLine, BaseMkoChan, 2);
}
//================================================================================

/*! \fn void KpaCommand26 (int BaseMkoLine, int BaseMkoChan);
 * \brief Функция обработки команды 0x26: запрос на включение ВМ4 ЦБК. Вызывается при обработке прерывания от КПА
 * \param [in] BaseMkoLine - номер линии МКО для обмена с КПА
 * \param [in] BaseMkoChan - номер канала МКО для обмена с КПА
 * \details <b> Формат данных команды: </b><br>
 * слово команды [0] биты [7..0] - код команды (0x26) <br>
 * <b> Работа команды: </b><br>
 * производится включение канала ВМ4 ЦБК <br>
 * производится запрос новой конфигурации ВМ ЦБК и проверка включения канала <br>
 * производится передача результата выполнения на КПА <br>
 * <b> Внимание: </b><br>
 * команда не работает корректно с текущей версией ОПО, применять не рекомендуется <br>
 */

void KpaCommand26 (int BaseMkoLine, int BaseMkoChan)
{
CbkChanPowerOn(BaseMkoLine, BaseMkoChan, 3);
}
//================================================================================

/*! \fn void CbkChanPowerOn (int BaseMkoLine, int BaseMkoChan, int CbkChan);
 * \brief Служебная функция включения канала ВМ ЦБК
 * \param [in] BaseMkoLine - номер линии МКО для обмена с КПА
 * \param [in] BaseMkoChan - номер канала МКО для обмена с КПА
 * \param [in] CbkChan - номер канала ВМ ЦБК для включения (0..3)
 * \details <b> Работа команды: </b><br>
 * производится включение канала ВМ ЦБК <br>
 * производится запрос новой конфигурации ВМ ЦБК и проверка включения канала <br>
 * производится передача результата выполнения на КПА <br>
 * <b> Внимание: </b><br>
 * команда не работает корректно с текущей версией ОПО, применять не рекомендуется <br>
 */

void CbkChanPowerOn (int BaseMkoLine, int BaseMkoChan, int CbkChan)
{
UINT State, VmState;
STATUS Status;
cfgSYS_t Config, PrevConfig;
WORD Buffer[8];

State = synchPowerOn(CbkChan);
if (State)
  {
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_VM_ERROR, KPA_SUBADDR_IRQ);
  return;
  }

Status = cfgSysGet(&Config);
if (Status != OK)
  {
  // ошибка получения конфигурации ЦБК
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE | REPLY_FLAG_ERROR, KPA_SUBADDR_IRQ);
  return;
  }

PrevConfig = CbkConfig;
CbkConfig = Config;
VmState = Config.of & (1 << CbkChan); // если = 0, канал ЦБК включен (канала нет в списке выключенных)

if (!VmState)
  {
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ);
  }
else
  {
  memmove(&Buffer[0], &PrevConfig, sizeof(PrevConfig));
  memmove(&Buffer[2], &CbkConfig, sizeof(CbkConfig));
  ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ, &Buffer, 8);
  }
}
//================================================================================

/*! \fn void KpaCommand27 (int BaseMkoLine, int BaseMkoChan);
 * \brief Функция обработки команды 0x27: запрос на выключение ВМ1 ЦБК. Вызывается при обработке прерывания от КПА
 * \param [in] BaseMkoLine - номер линии МКО для обмена с КПА
 * \param [in] BaseMkoChan - номер канала МКО для обмена с КПА
 * \details <b> Формат данных команды: </b><br>
 * слово команды [0] биты [7..0] - код команды (0x27) <br>
 * <b> Работа команды: </b><br>
 * производится выключение канала ВМ1 ЦБК <br>
 * производится запрос новой конфигурации ВМ ЦБК и проверка выключения канала <br>
 * производится передача результата выполнения на КПА <br>
 */

void KpaCommand27 (int BaseMkoLine, int BaseMkoChan)
{
CbkChanPowerOff(BaseMkoLine, BaseMkoChan, 0, IRQ_THREAD);
}
//================================================================================

/*! \fn void KpaCommand28 (int BaseMkoLine, int BaseMkoChan);
 * \brief Функция обработки команды 0x28: запрос на выключение ВМ2 ЦБК. Вызывается при обработке прерывания от КПА
 * \param [in] BaseMkoLine - номер линии МКО для обмена с КПА
 * \param [in] BaseMkoChan - номер канала МКО для обмена с КПА
 * \details <b> Формат данных команды: </b><br>
 * слово команды [0] биты [7..0] - код команды (0x28) <br>
 * <b> Работа команды: </b><br>
 * производится выключение канала ВМ2 ЦБК <br>
 * производится запрос новой конфигурации ВМ ЦБК и проверка выключения канала <br>
 * производится передача результата выполнения на КПА <br>
 */

void KpaCommand28 (int BaseMkoLine, int BaseMkoChan)
{
CbkChanPowerOff(BaseMkoLine, BaseMkoChan, 1, IRQ_THREAD);
}
//================================================================================

/*! \fn void KpaCommand29 (int BaseMkoLine, int BaseMkoChan);
 * \brief Функция обработки команды 0x29: запрос на выключение ВМ3 ЦБК. Вызывается при обработке прерывания от КПА
 * \param [in] BaseMkoLine - номер линии МКО для обмена с КПА
 * \param [in] BaseMkoChan - номер канала МКО для обмена с КПА
 * \details <b> Формат данных команды: </b><br>
 * слово команды [0] биты [7..0] - код команды (0x29) <br>
 * <b> Работа команды: </b><br>
 * производится выключение канала ВМ3 ЦБК <br>
 * производится запрос новой конфигурации ВМ ЦБК и проверка выключения канала <br>
 * производится передача результата выполнения на КПА <br>
 */

void KpaCommand29 (int BaseMkoLine, int BaseMkoChan)
{
CbkChanPowerOff(BaseMkoLine, BaseMkoChan, 2, IRQ_THREAD);
}
//================================================================================

/*! \fn void KpaCommand2A (int BaseMkoLine, int BaseMkoChan);
 * \brief Функция обработки команды 0x2A: запрос на выключение ВМ4 ЦБК. Вызывается при обработке прерывания от КПА
 * \param [in] BaseMkoLine - номер линии МКО для обмена с КПА
 * \param [in] BaseMkoChan - номер канала МКО для обмена с КПА
 * \details <b> Формат данных команды: </b><br>
 * слово команды [0] биты [7..0] - код команды (0x2A) <br>
 * <b> Работа команды: </b><br>
 * производится выключение канала ВМ4 ЦБК <br>
 * производится запрос новой конфигурации ВМ ЦБК и проверка выключения канала <br>
 * производится передача результата выполнения на КПА <br>
 */

void KpaCommand2A (int BaseMkoLine, int BaseMkoChan)
{
CbkChanPowerOff(BaseMkoLine, BaseMkoChan, 3, IRQ_THREAD);
}
//================================================================================

/*! \fn void CbkChanPowerOff (int BaseMkoLine, int BaseMkoChan, int CbkChan, int Context);
 * \brief Служебная функция выключения канала ВМ ЦБК
 * \param [in] BaseMkoLine - номер линии МКО для обмена с КПА
 * \param [in] BaseMkoChan - номер канала МКО для обмена с КПА
 * \param [in] CbkChan - номер канала ВМ ЦБК для включения (0..3)
 * \param [in] Context - контекст вызова (NO_THREAD = пустой вызов, MAIN_THREAD = вызов из цикла ПДО, IRQ_THREAD = вызов из прерываний)
 * \details <b> Работа команды: </b><br>
 * производится сброс состояния ЦБК (подготовка к отключению канала ВМ) <br>
 * производится выключение канала ВМ ЦБК <br>
 * производится запрос новой конфигурации ВМ ЦБК и проверка выключения канала <br>
 * производится передача результата выполнения на КПА <br>
 */

void CbkChanPowerOff (int BaseMkoLine, int BaseMkoChan, int CbkChan, int Context)
{
int VmState, MkoSubAddr;
UINT State;
STATUS Status;
cfgSYS_t Config, PrevConfig;
MKO_XCHG_FUNC MkoXchgFunc;
WORD *MkoBuffer, Crc16;

// по умолчанию (Context == NO_THREAD)
MkoXchgFunc = &MkoXchgStub;
MkoBuffer = (WORD*) &MkoBufStub;
MkoSubAddr = KPA_SUBADDR_MAIN;

if (Context == MAIN_THREAD)
  {
  MkoXchgFunc = &MkoXchgAn;
  MkoBuffer = (WORD*) &MkoBufA[0];
  MkoSubAddr = KPA_SUBADDR_MAIN;
  }
if (Context == IRQ_THREAD)
  {
  MkoXchgFunc = &MkoXchgBn;
  MkoBuffer = (WORD*) &MkoBufB[0];
  MkoSubAddr = KPA_SUBADDR_IRQ;
  }

State = testMDIGet((UINT*) &MkiBuffer, 0xC0000000 | (sizeof(MkiBuffer) / sizeof(UINT)));
if (State == ERROR)
  {
  // ошибка получения данных МКИ
  MkoBuffer[0] = REPLY_VM_ERROR;
  MkoXchgFunc(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, MkoSubAddr, MKO_TX, 1), 0);
  return;
  }

State = synchPowerOff(CbkChan);
if (State == ERROR)
  {
  MkoBuffer[0] = REPLY_VM_ERROR;
  MkoXchgFunc(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, MkoSubAddr, MKO_TX, 1), 0);
  return;
  }

Status = cfgSysGet(&Config);
if (Status != OK)
  {
  // ошибка получения конфигурации ЦБК
  MkoBuffer[0] = REPLY_VM_ERROR;
  MkoXchgFunc(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, MkoSubAddr, MKO_TX, 1), 0);
  return;
  }

PrevConfig = CbkConfig;
CbkConfig = Config;
VmState = Config.of & (1 << CbkChan); // если = 1, канал ЦБК выключен (канал есть в списке выключенных)

if (VmState)
  {
  MkoBuffer[0] = REPLY_CMD_DONE;
  MkoXchgFunc(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, MkoSubAddr, MKO_TX, 1), 0);
  }
else
  {
  memmove(&MkoBuffer[0], &PrevConfig, sizeof(PrevConfig));
  memmove(&MkoBuffer[2], &CbkConfig, sizeof(CbkConfig));
  MkoXchgFunc(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, MkoSubAddr + 1, MKO_TX, 4), 0);

  Crc16 = CalcCrc16(MkoBuffer, 2 * sizeof(CbkConfig));

  MkoBuffer[0] = REPLY_VM_OFF_ERROR;
  MkoBuffer[1] = 2 * sizeof(CbkConfig);
  MkoBuffer[2] = Crc16;
  MkoXchgFunc(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, MkoSubAddr, MKO_TX, 1), 0);
  }
}
//================================================================================

/*! \fn void KpaCommand2B (int BaseMkoLine, int BaseMkoChan);
 * \brief Функция обработки команды 0x2B: запрос на замену ведущей ВМ на МКО-0. Вызывается при обработке прерывания от КПА
 * \param [in] BaseMkoLine - номер линии МКО для обмена с КПА
 * \param [in] BaseMkoChan - номер канала МКО для обмена с КПА
 * \details <b> Формат данных команды: </b><br>
 * слово команды [0] биты [7..0] - код команды (0x2B) <br>
 * <b> Работа команды: </b><br>
 * производится запрос и анализ конфигурации ВМ ЦБК <br>
 * производится переключение ведущей ВМ на МКО <br>
 * производится запрос и проверка новой конфигурации ВМ ЦБК <br>
 * производится передача результата выполнения на КПА <br>
 */

void KpaCommand2B (int BaseMkoLine, int BaseMkoChan)
{
int Result, VmIndex, Index;
STATUS Status;
cfgSYS_t Config;
WORD Buffer[4];

memset(&Buffer, 0, sizeof(Buffer));

// получение текущей конфигурации ЦБК
Status = cfgSysGet(&Config);
if (Status != OK)
  {
  // ошибка при получении конфигурации ЦБК
  Buffer[0] = 1;
  ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE | REPLY_FLAG_ERROR, KPA_SUBADDR_IRQ, &Buffer, sizeof(Buffer));
  return;
  }

// расчет индекса новой ведущей ВМ
for (Index = 0, VmIndex = Config.m0; Index < 4; Index++)
  {
  VmIndex = (VmIndex + 1) & 0x03;
  if (Config.vm & (1 << VmIndex)) break;
  }

// смена ведущей ВМ на МКО0
Result = synchMasterChVM(0);
if (Result != 0)
  {
  // ошибка при смене ведущей ВМ
  Buffer[0] = 2;
  ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE | REPLY_FLAG_ERROR, KPA_SUBADDR_IRQ, &Buffer, sizeof(Buffer));
  return;
  }

// получение новой конфигурации ЦБК
Status = cfgSysGet(&Config);
if (Status != OK)
  {
  // ошибка при повторном получении конфигурации ЦБК
  Buffer[0] = 3;
  ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE | REPLY_FLAG_ERROR, KPA_SUBADDR_IRQ, &Buffer, sizeof(Buffer));
  return;
  }

if (VmIndex != Config.m0)
  {
  // ошибка при сравнении прогнозируемой и реальной конфигурации ВМ
  Buffer[0] = 4;
  Buffer[1] = VmIndex;
  Buffer[2] = Config.m0;
  ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE | REPLY_FLAG_ERROR, KPA_SUBADDR_IRQ, &Buffer, sizeof(Buffer));
  return;
  }

ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ);
}
//================================================================================

/*! \fn void KpaCommand2C (int BaseMkoLine, int BaseMkoChan);
 * \brief Функция обработки команды 0x2C: запрос на замену ведущей ВМ на МКО-1. Вызывается при обработке прерывания от КПА
 * \param [in] BaseMkoLine - номер линии МКО для обмена с КПА
 * \param [in] BaseMkoChan - номер канала МКО для обмена с КПА
 * \details <b> Формат данных команды: </b><br>
 * слово команды [0] биты [7..0] - код команды (0x2C) <br>
 * <b> Работа команды: </b><br>
 * производится запрос и анализ конфигурации ВМ ЦБК <br>
 * производится переключение ведущей ВМ на МКО <br>
 * производится запрос и проверка новой конфигурации ВМ ЦБК <br>
 * производится передача результата выполнения на КПА <br>
 */

void KpaCommand2C (int BaseMkoLine, int BaseMkoChan)
{
int Result, VmIndex, Index;
STATUS Status;
cfgSYS_t Config;
WORD Buffer[4];

memset(&Buffer, 0, sizeof(Buffer));

// получение текущей конфигурации ЦБК
Status = cfgSysGet(&Config);
if (Status != OK)
  {
  // ошибка при получении конфигурации ЦБК
  Buffer[0] = 1;
  ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE | REPLY_FLAG_ERROR, KPA_SUBADDR_IRQ, &Buffer, sizeof(Buffer));
  return;
  }

// расчет индекса новой ведущей ВМ
for (Index = 0, VmIndex = Config.m1; Index < 4; Index++)
  {
  VmIndex = (VmIndex + 1) & 0x03;
  if (Config.vm & (1 << VmIndex)) break;
  }

// смена ведущей ВМ на МКО1
Result = synchMasterChVM(1);
if (Result != 0)
  {
  // ошибка при смене ведущей ВМ
  Buffer[0] = 2;
  ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE | REPLY_FLAG_ERROR, KPA_SUBADDR_IRQ, &Buffer, sizeof(Buffer));
  return;
  }

// получение новой конфигурации ЦБК
Status = cfgSysGet(&Config);
if (Status != OK)
  {
  // ошибка при повторном получении конфигурации ЦБК
  Buffer[0] = 3;
  ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE | REPLY_FLAG_ERROR, KPA_SUBADDR_IRQ, &Buffer, sizeof(Buffer));
  return;
  }

if (VmIndex != Config.m1)
  {
  // ошибка при сравнении прогнозируемой и реальной конфигурации ВМ
  Buffer[0] = 4;
  Buffer[1] = VmIndex;
  Buffer[2] = Config.m1;
  ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE | REPLY_FLAG_ERROR, KPA_SUBADDR_IRQ, &Buffer, sizeof(Buffer));
  return;
  }

ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ);
}
//================================================================================

/*! \fn void KpaCommand2D (int BaseMkoLine, int BaseMkoChan);
 * \brief Функция обработки команды 0x2D: передача готовой команды по МКО. Вызывается при обработке прерывания от КПА
 * \param [in] BaseMkoLine - номер линии МКО для обмена с КПА
 * \param [in] BaseMkoChan - номер канала МКО для обмена с КПА
 * \details <b> Формат данных команды: </b><br>
 * слово команды [0] биты [7..0] - код команды (0x2D) <br>
 * слово команды [1] - готовое командное слово для передачи по МКО <br>
 * слово команды [2] бит [0] - линия МКО для обмена с абонентом (0 = первая линия, 1 = вторая линия) <br>
 * слово команды [2] бит [1] - канал МКО для обмена с абонентом (0 = основной канал, 1 = резервный канал) <br>
 * <b> Работа команды: </b><br>
 * производится анализ команды <br>
 * для режима обмена Ф4 (управление) производится только запись команды <br>
 * для режима обмена Ф1 (запись в абонент) производится считывание дополнительных данных с КПА и передача их в абонент <br>
 * для режима обмена Ф2 (чтение с абонента) производится считывание данных с абонента и передача их в КПА <br>
 * производится передача результата выполнения и данных на КПА <br>
 */

void KpaCommand2D (int BaseMkoLine, int BaseMkoChan)
{
int Result, DataSize, AddrOU, SubAddrOU, Mode, RealDataSize;
WORD MkoCmd, MkoLine, MkoChan, Crc16, Buffer[32];

MkoCmd = KpaCommand[1];
MkoLine = (KpaCommand[2] & 0x01);
MkoChan = (KpaCommand[2] >> 1) & 0x01;

DataSize  = (MkoCmd & 0x1F);          		// число слов для передачи
SubAddrOU = (MkoCmd >> 5) & 0x1F;   		// подадрес ОУ
Mode      = (MkoCmd >> 10) & 0x01;  		// направление передачи (0 = запись в абонент, 1 = чтение с абонента)
AddrOU    = (MkoCmd >> 11) & 0x1F;  		// адрес ОУ
RealDataSize = (DataSize) ? (DataSize) : (32);	// реальный размер данных (для режимов Ф1 и Ф2)

if ((SubAddrOU == 0x00) || (SubAddrOU == 0x1F))
  {
  // обмен в режиме Ф4 (команда управления)
  Result = MkoXchgBn(MkoLine, MkoChan, MkoCmd, 1);
  if (Result)
    {
    ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, DECODE_XCHG_ERROR(Result), KPA_SUBADDR_IRQ);
    return;
    }

  // отправка сообщения о выполнении команды
  Buffer[0] = MkoMsgResB[0].SW;
  ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ, &Buffer, 2);
  return;
  }
else
  {
  if (Mode == 0)
    {
    // обмен в режиме Ф1 (запись в абонент) 

    // считываем дополнительные данные для записи в абонент
    Result = MkoXchgBn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_CMD + 1, MKO_RX, RealDataSize), 0);
    if (Result)
      {
      ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, DECODE_XCHG_ERROR(Result), KPA_SUBADDR_IRQ);
      return;
      }

    // ошибка в размере принятых данных
    if (MkoMsgResB[0].Len != RealDataSize)
      {
      ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_NO_DATA_ERROR, KPA_SUBADDR_IRQ);
      return;
      }

    // проверка целостности данных
    Crc16 = CalcCrc16(&MkoBufB[0], RealDataSize * sizeof(WORD));
    if (Crc16 != KpaCommand[7])
      {
      ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_DATA_CRC_ERROR, KPA_SUBADDR_IRQ);
      return;
      }

    // запись данных в абонент
    Result = MkoXchgBn(MkoLine, MkoChan, MkoCmd, 0);
    if (Result)
      {
      ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, DECODE_XCHG_ERROR(Result), KPA_SUBADDR_IRQ);
      return;
      }

    // отправка сообщения о выполнении команды
    Buffer[0] = MkoMsgResB[0].SW;
    ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ, &Buffer, 2);
    return;
    }
  else
    {
    // обмен в режиме Ф2 (чтение с абонента)

    // чтение данных с абонента
    Result = MkoXchgBn(MkoLine, MkoChan, MkoCmd, 0);
    if (Result)
      {
      ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, DECODE_XCHG_ERROR(Result), KPA_SUBADDR_IRQ);
      return;
      }

    // перенос данных в локальный буфер
    memmove(&Buffer, &MkoBufB[0], sizeof(Buffer));

    // передача данных в КПА
    ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ, &Buffer, RealDataSize * sizeof(WORD));
    return;
    }
  }
}
//================================================================================

/*! \fn void KpaCommand2E (int BaseMkoLine, int BaseMkoChan);
 * \brief Функция обработки команды 0x2E: передача маски разрешенных прерываний. Вызывается при обработке прерывания от КПА
 * \param [in] BaseMkoLine - номер линии МКО для обмена с КПА
 * \param [in] BaseMkoChan - номер канала МКО для обмена с КПА
 * \details <b> Формат данных команды: </b><br>
 * слово команды [0] биты [7..0] - код команды (0x2E) <br>
 * <b> Работа команды: </b><br>
 * на КПА передается текущая маска разрешенных прерываний из конфигурации СПО <br>
 * производится передача результата выполнения на КПА <br>
 */

void KpaCommand2E (int BaseMkoLine, int BaseMkoChan)
{
WORD Buffer[2];

Buffer[0] = SpoConfig.EnabledIrqMask;
ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ, &Buffer, 2);
}
//================================================================================

/*! \fn void KpaCommand2F (int BaseMkoLine, int BaseMkoChan);
 * \brief Функция обработки команды 0x2F: чтение списка номеров устройств, установленных на непрерывный контроль. Вызывается при обработке прерывания от КПА
 * \param [in] BaseMkoLine - номер линии МКО для обмена с КПА
 * \param [in] BaseMkoChan - номер канала МКО для обмена с КПА
 * \details <b> Формат данных команды: </b><br>
 * слово команды [0] биты [7..0] - код команды (0x2F) <br>
 * <b> Работа команды: </b><br>
 * формируется список с номерами устройств, находящихся на непрерывном контроле <br>
 * производится передача результата выполнения и данных на КПА <br>
 */

void KpaCommand2F (int BaseMkoLine, int BaseMkoChan)
{
WORD Index, DeviceCount, DeviceList[32];

DeviceCount = 0;
for (Index = 0; Index < DEVICES_COUNT; Index++)
  {
  if (Control_DeviceList[Index]) DeviceList[DeviceCount++] = Index;
  }

// передача данных в КПА
ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ, &DeviceList, DeviceCount * sizeof(WORD));
}
//================================================================================

/*! \fn void KpaCommand30 (int BaseMkoLine, int BaseMkoChan);
 * \brief Функция обработки команды 0x30: чтение состояния указанных ОК. Вызывается при обработке прерывания от КПА
 * \param [in] BaseMkoLine - номер линии МКО для обмена с КПА
 * \param [in] BaseMkoChan - номер канала МКО для обмена с КПА
 * \details <b> Формат данных команды: </b><br>
 * слово команды [0] биты [7..0] - код команды (0x30) <br>
 * слово команды [1] - число требуемых OK (1..32) <br>
 * <b> Работа команды: </b><br>
 * производится проверка параметров команды на допустимость <br>
 * с п/а 2 КПА считывается список номеров требуемых OK <br>
 * формируется список, содержащий состояние указанных номеров OK <br>
 * производится передача результата выполнения и данных на КПА <br>
 */

void KpaCommand30 (int BaseMkoLine, int BaseMkoChan)
{
Get_OK_OKK_Data(BaseMkoLine, BaseMkoChan, 0);
}
//================================================================================

/*! \fn void KpaCommand31 (int BaseMkoLine, int BaseMkoChan);
 * \brief Функция обработки команды 0x31: чтение состояния указанных ОКK. Вызывается при обработке прерывания от КПА
 * \param [in] BaseMkoLine - номер линии МКО для обмена с КПА
 * \param [in] BaseMkoChan - номер канала МКО для обмена с КПА
 * \details <b> Формат данных команды: </b><br>
 * слово команды [0] биты [7..0] - код команды (0x31) <br>
 * слово команды [1] - число требуемых OKK (1..32) <br>
 * <b> Работа команды: </b><br>
 * производится проверка параметров команды на допустимость <br>
 * с п/а 2 КПА считывается список номеров требуемых OKK <br>
 * формируется список, содержащий состояние для указанных номеров OKK <br>
 * производится передача результата выполнения и данных на КПА <br>
 */

void KpaCommand31 (int BaseMkoLine, int BaseMkoChan)
{
Get_OK_OKK_Data(BaseMkoLine, BaseMkoChan, 1);
}
//================================================================================

/*! \fn void Get_OK_OKK_Data (int BaseMkoLine, int BaseMkoChan, int AMode);
 * \brief Служебная функция чтения состояния указанных OK/ОКK
 * \param [in] BaseMkoLine - номер линии МКО для обмена с КПА
 * \param [in] BaseMkoChan - номер канала МКО для обмена с КПА
 * \param [in] AMode - тип запроса (0 - OK, 1 - OKK)
 * \details <b> Формат данных команды: </b><br>
 * слово команды [0] биты [7..0] - код команды (0x30/0x31) <br>
 * слово команды [1] - число требуемых OK/OKK (1..32) <br>
 * <b> Работа команды: </b><br>
 * производится проверка параметров команды на допустимость <br>
 * с п/а 2 КПА считывается список номеров требуемых OK/OKK <br>
 * формируется список, содержащий состояние указанных номеров OK/OKK <br>
 * производится передача результата выполнения и данных на КПА <br>
 */

void Get_OK_OKK_Data (int BaseMkoLine, int BaseMkoChan, int AMode)
{
int Result, OKCount, Index, DeviceIndex, OKIndex, I, OKFound;
int DevMkoLine, DevMkoChan;
WORD Crc16, Buffer[4], OKNumList[32], OKStateList[32];
BYTE OKDeviceList[32], OKOutIndexList[32];
unsigned *AddrList, OKPlaceList[32], OKAddrList[32];
unsigned OKPlace, Addr1, Addr2, Data;
int AddrOU, SubOU, WNum, WIndex, BIndex, FLen;

// число запрашиваемых ОК
OKCount = KpaCommand[1];

// параметры линий обмена
DevMkoLine = SpoConfig.DevMkoLine & 1;
DevMkoChan = SpoConfig.DevMkoChan & 1;

// ошибка в параметрах команды
if ((OKCount < 1) || (OKCount > 32))
  {
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DATA_ERROR, KPA_SUBADDR_IRQ);
  return;
  }
 
// считываем список ОК
Result = MkoXchgBn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_CMD + 1, MKO_RX, OKCount), 0);
if (Result)
  {
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, DECODE_XCHG_ERROR(Result), KPA_SUBADDR_IRQ);
  return;
  }

// ошибка в размере принятых данных
if (MkoMsgResB[0].Len != OKCount)
  {
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_NO_DATA_ERROR, KPA_SUBADDR_IRQ);
  return;
  }

// проверка целостности данных
Crc16 = CalcCrc16(&MkoBufB[0][0], OKCount * sizeof(WORD));
if (Crc16 != KpaCommand[7])
  {
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_DATA_CRC_ERROR, KPA_SUBADDR_IRQ);
  return;
  }

// сохраняем список затребованных OK
memmove(&OKNumList, &MkoBufB[0][0], sizeof(OKNumList));

// собираем данные по расположению затребованных ОК
for (Index = 0; Index < OKCount; Index++)
  {
  DeviceIndex = ControlOkNumberToDevice(OKNumList[Index], &OKIndex);
  if (DeviceIndex == -1)
    {
    // ошибка: данный ОК не найден в таблице данных
    ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_MISS_OK_NUMBER_ERROR, KPA_SUBADDR_IRQ);
    return;
    }

  // расположение поля ОК
  OKDeviceList[Index] = DeviceIndex; // номер устройства для данного OK
  OKOutIndexList[Index] = Index; // номер данных OK в выходной очереди
  AddrList = Control_AddrList[DeviceIndex];
  OKPlaceList[Index] = AddrList[OKIndex]; // расположение данных OK в буфере МКО
  
  // поиск адреса для обмена с аппаратурой
  for (I = OKIndex, OKFound = 0; I >= 0; I--)
    {
    OKPlace = AddrList[I];

    AddrOU = (OKPlace >> OK_ADDR) & OK_ADDR_MASK;
    SubOU  = (OKPlace >> OK_SUBADDR) & OK_SUBADDR_MASK;
    WNum   = (OKPlace >> OK_WORD_NUM) & OK_WORD_NUM_MASK;

    // сохраняем реальный адрес для считывания данных OK на МКО
    if (AddrOU + SubOU + WNum) { OKAddrList[Index] = OKPlace; OKFound = 1; break; }
    }

  if (!OKFound)
    {
    // ошибка: реальный адрес OK не найден
    ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_INTERNAL_DATA_ERROR, KPA_SUBADDR_IRQ);
    return;
    }
  }

// сортируем таблицы по возрастанию адресов устройств
if (OKCount > 1)
  {
  do
    {
    OKFound = 0;
    for (Index = 0; Index < (OKCount - 1); Index++)
      {
      OKPlace = OKAddrList[Index];
      AddrOU  = (OKPlace >> OK_ADDR) & OK_ADDR_MASK;
      SubOU   = (OKPlace >> OK_SUBADDR) & OK_SUBADDR_MASK;
      WNum    = (OKPlace >> OK_WORD_NUM) & OK_WORD_NUM_MASK;
      Addr1   = (AddrOU << 16) | (SubOU << 8) | (WNum);

      OKPlace = OKAddrList[Index + 1];
      AddrOU  = (OKPlace >> OK_ADDR) & OK_ADDR_MASK;
      SubOU   = (OKPlace >> OK_SUBADDR) & OK_SUBADDR_MASK;
      WNum    = (OKPlace >> OK_WORD_NUM) & OK_WORD_NUM_MASK;
      Addr2   = (AddrOU << 16) | (SubOU << 8) | (WNum);

      if (Addr1 > Addr2)
        {
        OKFound = 1;
        Data = OKAddrList[Index];     OKAddrList[Index]     = OKAddrList[Index + 1];     OKAddrList[Index + 1]     = Data;
        Data = OKPlaceList[Index];    OKPlaceList[Index]    = OKPlaceList[Index + 1];    OKPlaceList[Index + 1]    = Data;
        Data = OKDeviceList[Index];   OKDeviceList[Index]   = OKDeviceList[Index + 1];   OKDeviceList[Index + 1]   = Data;
        Data = OKOutIndexList[Index]; OKOutIndexList[Index] = OKOutIndexList[Index + 1]; OKOutIndexList[Index + 1] = Data;
        }
      }
    }
  while (OKFound);
  }

// читаем данные OK с устройств
Addr2 = 0;

for (Index = 0; Index < OKCount; Index++)
  {
  OKPlace = OKAddrList[Index];
  AddrOU  = (OKPlace >> OK_ADDR) & OK_ADDR_MASK;
  SubOU   = (OKPlace >> OK_SUBADDR) & OK_SUBADDR_MASK;
  WNum    = (OKPlace >> OK_WORD_NUM) & OK_WORD_NUM_MASK;
  Addr1   = (AddrOU << 16) | (SubOU << 8) | (WNum);

  // читаем данные только если адреса устройств различаются
  if (Addr1 != Addr2)
    {
    Addr2 = Addr1;

    // чтение данных OK с устройства
    Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(AddrOU, SubOU, MKO_RX, WNum), 0);
    if (Result)
      {
      Buffer[0] = OKDeviceList[Index]; // индекс устройства, при обмене с которым возникли ошибки
      Buffer[1] = Index; // индекс OK, при чтении которого возникли ошибки
      Buffer[2] = DECODE_XCHG_ERROR(Result); // информация об ошибке
      ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_OK_READ_ERROR, KPA_SUBADDR_IRQ, &Buffer, 6);
      return;
      }
    }

  OKPlace = OKPlaceList[Index];
  
  if (AMode == 0)
    {
    // текущий режим = чтение списка OK
    WIndex = (OKPlace >> OK_WORD_INDEX) & OK_WORD_INDEX_MASK;
    BIndex = (OKPlace >> OK_BIT_INDEX) & OK_BIT_INDEX_MASK;
    FLen   = (OKPlace >> OK_FIELD_LEN) & OK_FIELD_LEN_MASK;

    if ((FLen < 1) || (FLen > 16) || ((BIndex + FLen) > 16))
      {
      Buffer[0] = OKDeviceList[Index]; // индекс устройства, в данных которого есть ошибки
      Buffer[1] = Index; // индекс OK, в данных которого есть ошибки
      ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_INTERNAL_DATA_ERROR, KPA_SUBADDR_IRQ, &Buffer, 4);
      return;
      }

    // реальный индекс данных в выходном буфере
    I = OKOutIndexList[Index]; 
    // получаем из буфера обмена битовое поле ОК
    OKStateList[I] = (MkoBufB[0][WIndex] >> BIndex) & Control_BitMap[FLen];
    }
  else
    {
    // текущий режим = чтение списка OKK
    WIndex = (OKPlace >> OK_WORD_INDEX) & OK_WORD_INDEX_MASK;

    // реальный индекс данных в выходном буфере
    I = OKOutIndexList[Index]; 
    // получаем из буфера обмена битовое поле ОК
    OKStateList[I] = MkoBufB[0][WIndex];
    }
  }

// передача считанных OK на КПА
ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ, &OKStateList, OKCount * sizeof(WORD));
}
//================================================================================

/*! \fn void KpaCommand32 (int BaseMkoLine, int BaseMkoChan);
 * \brief Функция обработки команды 0x32: установка режима обработки прерывания 6. Вызывается при обработке прерывания от КПА
 * \param [in] BaseMkoLine - номер линии МКО для обмена с КПА
 * \param [in] BaseMkoChan - номер канала МКО для обмена с КПА
 * \details <b> Формат данных команды: </b><br>
 * слово команды [0] биты [7..0] - код команды (0x32) <br>
 * слово команды [1] биты [1..0] - режим обработки (1..3) <br>
 * <b> Работа команды: </b><br>
 * производится проверка параметров команды на допустимость <br>
 * производится установка режима в конфигурации СПО <br>
 * производится передача результата выполнения на КПА <br>
 */

void KpaCommand32 (int BaseMkoLine, int BaseMkoChan)
{
int Mode;

Mode = KpaCommand[1] & 0x03;

if (!Mode)
  {
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DATA_ERROR, KPA_SUBADDR_IRQ);
  return;
  }

SpoConfig.Irq6Mode = Mode;

ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ);
}
//================================================================================

/*! \fn void KpaCommand33 (int BaseMkoLine, int BaseMkoChan);
 * \brief Функция обработки команды 0x33: установка/чтение общей маски сообщений о приходе прерываний. Вызывается при обработке прерывания от КПА
 * \param [in] BaseMkoLine - номер линии МКО для обмена с КПА
 * \param [in] BaseMkoChan - номер канала МКО для обмена с КПА
 * \details <b> Формат данных команды: </b><br>
 * слово команды [0] биты [7..0] - код команды (0x33) <br>
 * слово команды [1] бит [0] - режим работы (0 = чтение 16-бит маски, 1 = установка маски) <br>
 * слово команды [2] - 16-бит маска каналов (если режим = установка маски) <br>
 * <b> Работа команды: </b><br>
 * если режим - чтение маски, производится передача маски в КПА <br>
 * если режим - запись маски, производится установка маски в конфигурации СПО <br>
 * производится передача результата выполнения на КПА <br>
 */

void KpaCommand33 (int BaseMkoLine, int BaseMkoChan)
{
int Mode;
WORD Mask, Buffer[2];

Mode = KpaCommand[1] & 0x01;
Mask = KpaCommand[2];

if (!Mode)
  {
  // считывание 16-бит маски сообщений
  Buffer[0] = SpoConfig.IrqMsgMask;
  ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ, &Buffer, 2);
  return;
  }
else
  {
  // установка 16-бит маски сообщений
  SpoConfig.IrqMsgMask = Mask;

  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ);
  return;
  }
}
//================================================================================

/*! \fn void KpaCommand34 (int BaseMkoLine, int BaseMkoChan);
 * \brief Функция обработки команды 0x34: установка/чтение бита маски сообщений о приходе прерываний. Вызывается при обработке прерывания от КПА
 * \param [in] BaseMkoLine - номер линии МКО для обмена с КПА
 * \param [in] BaseMkoChan - номер канала МКО для обмена с КПА
 * \details <b> Формат данных команды: </b><br>
 * слово команды [0] биты [7..0] - код команды (0x34) <br>
 * слово команды [1] бит [0] - режим работы (0 = чтение бита маски, 1 = изменение бита маски) <br>
 * слово команды [1] бит [1] - (если режим - изменение бита маски) новое состояние бита маски (1 = сообщения разрешены, 0 = запрещены) <br>
 * слово команды [2] - номер канала (0..15) <br>
 * <b> Работа команды: </b><br>
 * если режим - чтение бита маски, производится передача состояния бита маски в КПА <br>
 * если режим - запись бита маски, производится изменение бита маски в конфигурации СПО <br>
 * производится передача результата выполнения на КПА <br>
 */

void KpaCommand34 (int BaseMkoLine, int BaseMkoChan)
{
int Mode, State, ChanIndex;
WORD Buffer[2];

Mode = KpaCommand[1] & 0x01;
State = (KpaCommand[1] >> 1) & 0x01;
ChanIndex = KpaCommand[2] & 0x0F;

if (!Mode)
  {
  // считывание бита маски сообщений
  Buffer[0] = (SpoConfig.IrqMsgMask >> ChanIndex) & 0x01;
  ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ, &Buffer, 2);
  return;
  }
else
  {
  // изменение бита маски сообщений
  if (State) SpoConfig.IrqMsgMask |= (1 << ChanIndex);
  else SpoConfig.IrqMsgMask &= ~(1 << ChanIndex);

  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ);
  return;
  }
}
//================================================================================

/*! \fn void KpaCommand35 (int BaseMkoLine, int BaseMkoChan);
 * \brief Функция обработки команды 0x35: чтение группы абонентов. Вызывается при обработке прерывания от КПА
 * \param [in] BaseMkoLine - номер линии МКО для обмена с КПА
 * \param [in] BaseMkoChan - номер канала МКО для обмена с КПА
 * \details <b> Формат данных команды: </b><br>
 * слово команды [0] биты [7..0] - код команды (0x35) <br>
 * слово команды [1] - число описателей абонентов, переданных в п/а 2 КПА <br>
 * слово команды [2] бит [0] - линия МКО, по которой будет произведено чтение абонентов <br>
 * слово команды [2] бит [1] - канал МКО, по которому будет произведено чтение абонентов <br>
 * <b> Работа команды: </b><br>
 * производится проверка параметров команды на допустимость <br>
 * с п/а 2 КПА считывается и проверяется набор описателей для чтения абонентов на МКО <br>
 * производится чтение абонентов и накопление считанных данных <br>
 * производится передача результата выполнения на КПА <br>
 */

void KpaCommand35 (int BaseMkoLine, int BaseMkoChan)
{
int Result, SubsCount, MkoLine, MkoChan;
int Index, TotalWordCount, WordCount;
WORD MkoCmd, Crc16;

SubsCount = KpaCommand[1];
MkoLine  = (KpaCommand[2] >> 0) & 0x01;
MkoChan  = (KpaCommand[2] >> 1) & 0x01;

// ошибка в параметрах команды
if ((SubsCount < 1) || (SubsCount > 32))
  {
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DATA_ERROR, KPA_SUBADDR_IRQ);
  return;
  }

// считываем описатели абонентов
Result = MkoXchgBn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_CMD + 1, MKO_RX, SubsCount), 0);
if (Result)
  {
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, DECODE_XCHG_ERROR(Result), KPA_SUBADDR_IRQ);
  return;
  }

// ошибка в размере принятых данных
if (MkoMsgResB[0].Len != SubsCount)
  {
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_NO_DATA_ERROR, KPA_SUBADDR_IRQ);
  return;
  }

// проверка целостности данных
Crc16 = CalcCrc16(&MkoBufB[0], SubsCount * sizeof(WORD));
if (Crc16 != KpaCommand[7])
  {
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_DATA_CRC_ERROR, KPA_SUBADDR_IRQ);
  return;
  }

TotalWordCount = 0;
memmove(&SubsAddrBuffer, &MkoBufB[0], SubsCount * sizeof(WORD));

for (Index = 0; Index < SubsCount; Index++)
  {
  // принудительно выставляем флаг на чтение
  MkoCmd = SubsAddrBuffer[Index] | ((MKO_RX & 0x01) << SH_TR);
  // ожидаемый размер данных от абонента
  WordCount = SubsAddrBuffer[Index] & 0x1F;
  if (!WordCount) WordCount = 32;

  // чтение данных с абонента
  Result = MkoXchgBn(MkoLine, MkoChan, MkoCmd, 0);
  if (Result)
    {
    ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, DECODE_XCHG_ERROR(Result), KPA_SUBADDR_IRQ);
    return;
    }
  
  // ошибка в размере принятых данных
  if (MkoMsgResB[0].Len != WordCount)
    {
    ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_NO_DATA_ERROR, KPA_SUBADDR_IRQ);
    return;
    }
  
  // переносим принятые данные в выходной буфер
  memmove(&SubsDataBuffer[TotalWordCount], &MkoBufB[0], WordCount * sizeof(WORD));
  TotalWordCount += WordCount;
  }

// передача принятых от абонентов данных
ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ, &SubsDataBuffer, TotalWordCount * sizeof(WORD));
}
//================================================================================

/*! \fn void KpaCommand36 (int BaseMkoLine, int BaseMkoChan);
 * \brief Функция обработки команды 0x36: считывание состояния флага прерывания. Вызывается при обработке прерывания от КПА
 * \param [in] BaseMkoLine - номер линии МКО для обмена с КПА
 * \param [in] BaseMkoChan - номер канала МКО для обмена с КПА
 * \details <b> Формат данных команды: </b><br>
 * слово команды [0] биты [7..0] - код команды (0x36) <br>
 * слово команды [1] биты [3..0] - номер линии прерывания (0..15), состояние которой требуется считать <br>
 * <b> Работа команды: </b><br>
 * производится запрос к ОПО на считывание состояния указанной линии прерываний <br>
 * производится передача результата выполнения на КПА <br>
 */

void KpaCommand36 (int BaseMkoLine, int BaseMkoChan)
{
UINT Result, Line;
WORD Buffer[2];

Line = KpaCommand[1] & 0x0F;

Result = sprIGetF(Line);
if (Result == ERROR)
  {
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE | REPLY_FLAG_ERROR, KPA_SUBADDR_IRQ);
  return;
  }

Buffer[0] = (Result != 0) ? (1) : (0);
ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ, &Buffer, 2);
}
//================================================================================

/*! \fn void KpaCommand37 (int BaseMkoLine, int BaseMkoChan);
 * \brief Функция обработки команды 0x37: сброс флага прерывания. Вызывается при обработке прерывания от КПА
 * \param [in] BaseMkoLine - номер линии МКО для обмена с КПА
 * \param [in] BaseMkoChan - номер канала МКО для обмена с КПА
 * \details <b> Формат данных команды: </b><br>
 * слово команды [0] биты [7..0] - код команды (0x37) <br>
 * слово команды [1] биты [3..0] - номер линии прерывания (0..15), для которой надо сбросить запрос <br>
 * <b> Работа команды: </b><br>
 * производится запрос к ОПО на сброс указанной линии прерываний <br>
 * производится передача результата выполнения на КПА <br>
 */

void KpaCommand37 (int BaseMkoLine, int BaseMkoChan)
{
UINT Line;
STATUS Status;

Line = KpaCommand[1] & 0x0F;

Status = sprIQClear(Line);
if (Status == ERROR)
  {
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE | REPLY_FLAG_ERROR, KPA_SUBADDR_IRQ);
  return;
  }

ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ);
}
//================================================================================

/*! \fn void KpaCommand38 (int BaseMkoLine, int BaseMkoChan);
 * \brief Функция обработки команды 0x38: включение контроля "Запроса 0" (прерываний от АСН). Вызывается при обработке прерывания от КПА
 * \param [in] BaseMkoLine - номер линии МКО для обмена с КПА
 * \param [in] BaseMkoChan - номер канала МКО для обмена с КПА
 * \details <b> Формат данных команды: </b><br>
 * слово команды [0] биты [7..0] - код команды (0x38) <br>
 * слово команды [1] - длительность проверки, сек (10..36000) <br>
 * слово команды [2] биты [7..0] - порог опережения прерываний, мсек (0..255) <br>
 * слово команды [2] биты [15..8] - порог запаздывания прерываний, мсек (0..255) <br>
 * <b> Работа команды: </b><br>
 * производится проверка параметров команды на допустимость <br>
 * производится инициализация теста и разрешение прерываний АСН <br>
 * производится передача результата выполнения на КПА <br>
 * результат теста после его завершения передается на КПА в цикле ПДО <br>
 */

void KpaCommand38 (int BaseMkoLine, int BaseMkoChan)
{
STATUS Status;
sysTime_t CurrTime;
int ControlTime;

ControlTime = KpaCommand[1];
AsnPreDeltaTime = KpaCommand[2] & 0xFF;
AsnPostDeltaTime = (KpaCommand[2] >> 8) & 0xFF;

if ((ControlTime < 10) || (ControlTime > 36000))
  {
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DATA_ERROR, KPA_SUBADDR_IRQ);
  return;
  }

// отключение прерываний АСН
SpoConfig.EnabledIrqMask &= (~0x0001);
sprIMaskVec(~SpoConfig.EnabledIrqMask);

Status = timeRead(&CurrTime);
if (Status != OK)
  {
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE | REPLY_FLAG_ERROR, KPA_SUBADDR_IRQ);
  return;
  }

AsnControlStopTime = CurrTime;
AsnControlStopTime.time_sec += ControlTime;

AsnControlEnabled = 1;
AsnWaitForFirstIrq = 1;
AsnStartTimer = 1;
AsnIrqAlreadySet = 0;

AsnIrqCount = AsnExcIrqCount = AsnMissIrqCount = 0;

AsnStatusBeginTime = 0;
AsnTimerFailCount = 0;
AsnMkoFailCount = 0;
AsnParamFailCount = 0;

AsnTimeListSize = 0; // размер списка интервалов между прерываниями

// включение прерываний АСН
SpoConfig.EnabledIrqMask |= 0x0001;
sprIMaskVec(~SpoConfig.EnabledIrqMask);

ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ);
}
//================================================================================

/*! \fn void KpaCommand39 (int BaseMkoLine, int BaseMkoChan);
 * \brief Функция обработки команды 0x39: установить линию МКО для обменов с устройствами. Вызывается при обработке прерывания от КПА
 * \param [in] BaseMkoLine - номер линии МКО для обмена с КПА
 * \param [in] BaseMkoChan - номер канала МКО для обмена с КПА
 * \details <b> Формат данных команды: </b><br>
 * слово команды [0] биты [7..0] - код команды (0x39) <br>
 * слово команды [1] бит [0] - линия МКО для обменов с устройствами (0 = 1 линия, 1 = 2 линия) <br>
 * <b> Работа команды: </b><br>
 * производится сохранение номера линии МКО для обменов с устройствами в конфигурации СПО <br>
 * производится передача результата выполнения на КПА <br>
 */

void KpaCommand39 (int BaseMkoLine, int BaseMkoChan)
{
int Line;

Line = KpaCommand[1] & 0x01;
SpoConfig.DevMkoLine = Line;

ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ);
}
//================================================================================

/*! \fn void KpaCommand3A (int BaseMkoLine, int BaseMkoChan);
 * \brief Функция обработки команды 0x3A: включение контроля "Запроса 5" (прерываний от БЭЧ). Вызывается при обработке прерывания от КПА
 * \param [in] BaseMkoLine - номер линии МКО для обмена с КПА
 * \param [in] BaseMkoChan - номер канала МКО для обмена с КПА
 * \details <b> Формат данных команды: </b><br>
 * слово команды [0] биты [7..0] - код команды (0x3A) <br>
 * слово команды [1] - длительность проверки, сек (10..36000) <br>
 * слово команды [2] биты [7..0] - порог опережения прерываний, мсек (0..255) <br>
 * слово команды [2] биты [15..8] - порог запаздывания прерываний, мсек (0..255) <br>
 * слово команды [3] - период установки бита ФШВ (1..20) <br>
 * слово команды [4] - период установки бита ФИНИК (1..20) <br>
 * <b> Работа команды: </b><br>
 * производится проверка параметров команды на допустимость <br>
 * производится инициализация теста и разрешение прерываний БЭЧ <br>
 * производится передача результата выполнения на КПА <br>
 * результат теста после его завершения передается на КПА в цикле ПДО <br>
 */

void KpaCommand3A (int BaseMkoLine, int BaseMkoChan)
{
STATUS Status;
sysTime_t CurrTime;
int ControlTime;

ControlTime = KpaCommand[1];
BehPreDeltaTime = KpaCommand[2] & 0xFF;
BehPostDeltaTime = (KpaCommand[2] >> 8) & 0xFF;
BehFhvPeriod = KpaCommand[3];
BehFinikPeriod = KpaCommand[4];

if ((ControlTime < 10) || (ControlTime > 36000) ||
    (BehFhvPeriod < 1) || (BehFhvPeriod > 20) ||
    (BehFinikPeriod < 1) || (BehFinikPeriod > 20))
  {
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DATA_ERROR, KPA_SUBADDR_IRQ);
  return;
  }

// отключение прерываний БЭЧ
SpoConfig.EnabledIrqMask &= (~0x0020);
sprIMaskVec(~SpoConfig.EnabledIrqMask);

Status = timeRead(&CurrTime);
if (Status != OK)
  {
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE | REPLY_FLAG_ERROR, KPA_SUBADDR_IRQ);
  return;
  }

BehControlStopTime = CurrTime;
BehControlStopTime.time_sec += ControlTime;

BehControlEnabled = 1;
BehWaitForFirstIrq = 1;
BehStartTimer = 1;
BehIrqAlreadySet = 0;

BehIrqCount = BehExcIrqCount = BehMissIrqCount = 0;
BehFhvIrqCount = BehExcFhvIrqCount = BehMissFhvIrqCount = 0;
BehFinikIrqCount = BehExcFinikIrqCount = BehMissFinikIrqCount = 0;

BehFhvFlag = BehFinikFlag = 0;
BehFhvTmpCount = BehFinikTmpCount = 0;
BehMkoFailCount = 0;

// включение прерываний БЭЧ
SpoConfig.EnabledIrqMask |= 0x0020;
sprIMaskVec(~SpoConfig.EnabledIrqMask);

ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ);
}
//================================================================================

/*! \fn void KpaCommand40 (int BaseMkoLine, int BaseMkoChan);
 * \brief Функция обработки команды 0x40: установить канал МКО для обменов с устройствами. Вызывается при обработке прерывания от КПА
 * \param [in] BaseMkoLine - номер линии МКО для обмена с КПА
 * \param [in] BaseMkoChan - номер канала МКО для обмена с КПА
 * \details <b> Формат данных команды: </b><br>
 * слово команды [0] биты [7..0] - код команды (0x40) <br>
 * слово команды [1] бит [0] - канал МКО для обменов с устройствами (0 = основной канал, 1 = резервный канал) <br>
 * <b> Работа команды: </b><br>
 * производится сохранение номера канала МКО для обменов с устройствами в конфигурации СПО <br>
 * производится передача результата выполнения на КПА <br>
 */

void KpaCommand40 (int BaseMkoLine, int BaseMkoChan)
{
int Chan;

Chan = KpaCommand[1] & 0x01;
SpoConfig.DevMkoChan = Chan;

ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ);
}
//================================================================================

/*! \fn void KpaCommand41 (int BaseMkoLine, int BaseMkoChan);
 * \brief Функция обработки команды 0x41: запрос текущего статуса режимов работы
 * \param [in] BaseMkoLine - номер линии МКО для обмена с КПА
 * \param [in] BaseMkoChan - номер канала МКО для обмена с КПА
 * \details <b> Формат данных команды: </b><br>
 * слово команды [0] биты [7..0] - код команды (0x41) <br>
 * слово команды [1] - код режима <br>
 *                     1 - запрос статуса имитации данных ЦБК <br>
 *                     2 - запрос статуса имитации СЧБК <br>
 *                     3 - запрос статуса имитации квитанций <br>
 * <b> Работа команды: </b><br>
 * возвращает слово содержащее текущий статус режима (константу STATE_XXXX из MainUnit.h)
 */

void KpaCommand41 (int BaseMkoLine, int BaseMkoChan)
{
int Opcode;

// читаем код операции
Opcode = KpaCommand[1];

// запрос статуса имитации данных ЦБК
if (Opcode == 0x01)
  {
  ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ, &SpoConfig.CbkDataImitStatus, 2);
  }
// запрос статуса имитации СЧБК
else if (Opcode == 0x02)
  {
  ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ, &SpoConfig.ShbkImitStatus, 2);
  }
// запрос статуса имитации квитанций
else if (Opcode == 0x03)
  {
  ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ, &SpoConfig.ImitReplyStatus, 2);
  }
else
  {
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DATA_ERROR, KPA_SUBADDR_IRQ);
  }
}
//================================================================================


/*! \fn void KpaCommandFB (int BaseMkoLine, int BaseMkoChan);
 * \brief Функция обработки команды 0xFB: вызов функции реконфигурации ВМ с прямым указанием канала для втягивания. Вызывается при обработке прерывания от КПА
 * \param [in] BaseMkoLine - номер линии МКО для обмена с КПА
 * \param [in] BaseMkoChan - номер канала МКО для обмена с КПА
 * \details <b> Формат данных команды: </b><br>
 * слово команды [0] биты [7..0] - код команды (0xFB) <br>
 * слово команды [1] биты [1..0] - номер канала для втягивания (0..3) <br>
 * <b> Работа команды: </b><br>
 * производится запрос ОПО на выполнение втягивания указанного канала ВМ ЦБК <br>
 * если запрос на втягивание успешно принят ОПО, управление к СПО не возвращается (ОПО производит перезапуск СПО со стартовой задачи) <br>
 * иначе производится передача результата выполнения на КПА <br>
 */

void KpaCommandFB (int BaseMkoLine, int BaseMkoChan)
{
#ifdef OPO_V031
ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CBK_RECONFIG_PARAM_ERROR, KPA_SUBADDR_IRQ);
return;
#endif

#ifdef OPO_V033
int Mode, VmIndex;
STATUS Status;

VmIndex = KpaCommand[1] & 0x03;

ReConfigBuffer[0].adrArr = (void*) &MkoBufStub;
ReConfigBuffer[0].lenArr = sizeof(MkoBufStub);

// OPO v033:
// 0x00 - ВМ для втягивания выбирает БОС
// 0x10 - втянуть ВМ0
// 0x11 - втянуть ВМ1
// 0x12 - втянуть ВМ2
// 0x13 - втянуть ВМ3
// 0x20 - периодическая реконфигурация

Mode = 0x10 | VmIndex;

Status = synchRecEnable(100000, ReConfigBuffer, 1, Mode);
if (Status == ERROR)
  {
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CBK_RECONFIG_PARAM_ERROR, KPA_SUBADDR_IRQ);
  return;
  }
if (Status == OK)
  {
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CBK_RECONFIG_NO_VM, KPA_SUBADDR_IRQ);
  return;
  }

ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CBK_RECONFIG_UNK_ERROR, KPA_SUBADDR_IRQ);
#endif
}
//================================================================================

/*! \fn void KpaCommandFC (int BaseMkoLine, int BaseMkoChan);
 * \brief Функция обработки команды 0xFC: вызов функции периодической реконфигурации ВМ ЦБК. Вызывается при обработке прерывания от КПА
 * \param [in] BaseMkoLine - номер линии МКО для обмена с КПА
 * \param [in] BaseMkoChan - номер канала МКО для обмена с КПА
 * \details <b> Формат данных команды: </b><br>
 * слово команды [0] биты [7..0] - код команды (0xFC) <br>
 * <b> Работа команды: </b><br>
 * производится запрос ОПО на выполнение периодической реконфигурации ВМ ЦБК <br>
 * если запрос на реконфигурацию успешно принят ОПО, управление к СПО не возвращается (ОПО производит перезапуск СПО со стартовой задачи) <br>
 * иначе производится передача результата выполнения на КПА <br>
 */

void KpaCommandFC (int BaseMkoLine, int BaseMkoChan)
{
int Mode;
STATUS Status;

ReConfigBuffer[0].adrArr = (void*) &MkoBufStub;
ReConfigBuffer[0].lenArr = sizeof(MkoBufStub);

// OPO v031:
// 0 - восстановление конфигурации
// 1 - периодическая реконфигурация    

// OPO v033:
// 0x00 - ВМ для втягивания выбирает БОС
// 0x10 - втянуть ВМ0
// 0x11 - втянуть ВМ1
// 0x12 - втянуть ВМ2
// 0x13 - втянуть ВМ3
// 0x20 - периодическая реконфигурация

#ifdef OPO_V031
Mode = 1;
#endif

#ifdef OPO_V033
Mode = 0x20;
#endif

Status = synchRecEnable(100000, ReConfigBuffer, 1, Mode);
if (Status == ERROR)
  {
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CBK_RECONFIG_PARAM_ERROR, KPA_SUBADDR_IRQ);
  return;
  }
if (Status == OK)
  {
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CBK_RECONFIG_NO_VM, KPA_SUBADDR_IRQ);
  return;
  }

ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CBK_RECONFIG_UNK_ERROR, KPA_SUBADDR_IRQ);
}
//================================================================================

/*! \fn void KpaCommandFD (int BaseMkoLine, int BaseMkoChan);
 * \brief Функция обработки команды 0xFD: вызов программного сбоя в канале ВМ ЦБК. Вызывается при обработке прерывания от КПА
 * \param [in] BaseMkoLine - номер линии МКО для обмена с КПА
 * \param [in] BaseMkoChan - номер канала МКО для обмена с КПА
 * \details <b> Формат данных команды: </b><br>
 * слово команды [0] биты [7..0] - код команды (0xFD) <br>
 * слово команды [1] биты [1..0] - номер ВМ, в которой вызывается сбой (0..3) <br>
 * слово команды [2] - тип вызываемого сбоя в канале (0..8) <br>
 * <b> Работа команды: </b><br>
 * производится запрос ОПО на получение номера своей ВМ <br>
 * если номер своей ВМ равен указанному в команде, задача СПО вызывает сбой указанного типа <br>
 * иначе производится передача результата выполнения на КПА <br>
 */

void KpaCommandFD (int BaseMkoLine, int BaseMkoChan)
{
int ChanIndex, CurrChan, FailType, Value;
sysTime_t CurrTime;
WORD Mask;

ChanIndex = KpaCommand[1] & 0x03;
FailType = KpaCommand[2];

// индекс типа сбоя указан неверно
if ((FailType < 0) || (FailType > 8))
  {
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DATA_ERROR, KPA_SUBADDR_IRQ);
  return;
  }

CurrChan = numVMGet();

// если указанный номер канала соотв текущему, вызываем сбой в канале
if (ChanIndex == CurrChan)
  {
  if (FailType == 0) // сбой "вечный цикл"
    {
    while (1) {}
    }
  if (FailType == 1) // сбой "вечный цикл с измененем переменной"
    {
    while (1) { MainTaskCmdFail++; }
    }
  if (FailType == 2) // сбой "вечный цикл с мягким системным вызовом"
    {
    while (1) { timeRead(&CurrTime); }
    }
  if (FailType == 3) // сбой "вечный цикл с тяжелым системным вызовом"
    {
    Mask = SpoConfig.EnabledIrqMask;
    while (1) { sprIMaskVec(~Mask); }
    }
  if (FailType == 4) // сбой "неверная команда процессора #1"
    {
    //__asm__("break");
    }
  if (FailType == 5) // сбой "неверная команда процессора #2"
    {
    //__asm__(".word 0xFFFFFFFF");
    }
  if (FailType == 6) // сбой "невыровненный адрес при обращении к 32-бит ячейке"
    {
    __IO UINT32 *Ptr = (__IO UINT32*) 2;
    Value = *Ptr;
    }
  if (FailType == 7) // сбой "невыровненный адрес при обращении к 16-бит ячейке"
    {
    __IO UINT16 *Ptr = (__IO UINT16*) 1;
    Value = *Ptr;
    }
  if (FailType == 8) // сбой "обращение в запрещенную обрасть памяти"
    {
    *((BYTE*) 0x00000000) = 0;
    *((BYTE*) 0xFFFFFFFF) = 0;
    }
  }

ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ);
}
//================================================================================

/*! \fn void KpaCommandFE (int BaseMkoLine, int BaseMkoChan);
 * \brief Функция обработки команды 0xFE: вызов функции реконфигурации ВМ ЦБК. Вызывается при обработке прерывания от КПА
 * \param [in] BaseMkoLine - номер линии МКО для обмена с КПА
 * \param [in] BaseMkoChan - номер канала МКО для обмена с КПА
 * \details <b> Формат данных команды: </b><br>
 * слово команды [0] биты [7..0] - код команды (0xFE) <br>
 * <b> Работа команды: </b><br>
 * производится запрос ОПО на восстановление конфигурации ВМ ЦБК <br>
 * если запрос на реконфигурацию успешно принят ОПО, управление к СПО не возвращается (ОПО производит перезапуск СПО со стартовой задачи) <br>
 * иначе производится передача результата выполнения на КПА <br>
 */

void KpaCommandFE (int BaseMkoLine, int BaseMkoChan)
{
int Mode;
STATUS Status;

ReConfigBuffer[0].adrArr = (void*) &MkoBufStub;
ReConfigBuffer[0].lenArr = sizeof(MkoBufStub);

// OPO v031:
// 0 - восстановление конфигурации
// 1 - периодическая реконфигурация    

// OPO v033:
// 0x00 - ВМ для втягивания выбирает БОС
// 0x10 - втянуть ВМ0
// 0x11 - втянуть ВМ1
// 0x12 - втянуть ВМ2
// 0x13 - втянуть ВМ3
// 0x20 - периодическая реконфигурация

#ifdef OPO_V031
Mode = 0;
#endif

#ifdef OPO_V033
Mode = 0x00;
#endif

Status = synchRecEnable(100000, ReConfigBuffer, 1, Mode);
if (Status == ERROR)
  {
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CBK_RECONFIG_PARAM_ERROR, KPA_SUBADDR_IRQ);
  return;
  }
if (Status == OK)
  {
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CBK_RECONFIG_NO_VM, KPA_SUBADDR_IRQ);
  return;
  }

ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CBK_RECONFIG_UNK_ERROR, KPA_SUBADDR_IRQ);
}
//================================================================================

/*! \fn void KpaCommandFF (int BaseMkoLine, int BaseMkoChan);
 * \brief Функция обработки команды 0xFF: управление контрольными флагами СПО. Вызывается при обработке прерывания от КПА
 * \param [in] BaseMkoLine - номер линии МКО для обмена с КПА
 * \param [in] BaseMkoChan - номер канала МКО для обмена с КПА
 * \details <b> Формат данных команды: </b><br>
 * слово команды [0] биты [7..0] - код команды (0xFF) <br>
 * слово команды [1] биты [0..7] - индекс флага (0..255) <br>
 * слово команды [1] биты [8..11] - значение флага (0 или 1, имеет смысл только при записи) <br>
 * слово команды [1] биты [12..15] - тип операции над флагом (0 - считать, 1 - установить) <br>
 * <b> Работа команды: </b><br>
 * производится проверка параметров команды на допустимость <br>
 * производится считывание или установка переменных в конфигурации СПО <br>
 * производится передача результата выполнения на КПА <br>
 * допустимые индексы флагов: <br>
 * 0x01 - (только чтение) считать код версии СПО (4 байта в BCD формате: YYYYMMDD) <br>
 * 0x02 - (только чтение) считать строку версии СПО <br>
 * 0x03 - (чтение/запись) включение/отключение контроля нескольких одновременных прерываний (по умолчанию включено) <br>
 * 0x04 - (чтение/запись) включение/отключение секундной метки СПО (по умолчанию включено) <br>
 * 0x05 - (чтение/запись) включение/отключение расширенной статистики при ошибках обмена на МКО (по умолчанию отключено) <br>
 * 0x06 - (чтение/запись) включение/отключение расширенного режима обмена на МКО (по адресам 2 и 4) (по умолчанию отключено) <br>
 * 0x07 - (чтение/запись) включение/отключение контроля числа ошибок при имитации СЧБК (по умолчанию отключено) <br>
 * 0x08 - (чтение/запись) включение/отключение режима двойной буферизации при записи в МБК04 (по умолчанию отключено) <br>
 * 0x09 - (чтение/запись) включение/отключение выдачи дополнительного сообщения при пакетном обмене с МБК04 (по умолчанию отключено) <br>
 * 0x0A - (чтение/запись) включение/отключение управления размером пакета/сообщения при пакетном обмене с МБК04 (по умолчанию отключено) <br>
 * 0x0B - (чтение/запись) включение/отключение наращивания размера сообщения при пакетном обмене с МБК04 (по умолчанию отключено) <br>
 * 0x0C - (чтение/запись) включение/отключение контроля числа ошибок при имитации квитанций (по умолчанию отключено) <br>
 * 0x0D - (только чтение) передача строки с версией ОПО (в данной версии не поддерживается) <br>
 * 0x0E - (чтение/запись) включение/отключение передачи сообщений об ошибках из задачи обработки ошибок (по умолчанию включено) <br>
 * 0x0F - (чтение/запись) установка размера дополнительных данных, передаваемых из задачи обработки ошибок (по умолчанию 2 32бит слова) <br>
 * 0x10 - (только чтение) передача массива МДИ из всех каналов ЦБК <br>
 * 0x11 - (чтение/запись) изменить положение маркера пакета для МБК04 <br>
 * 0x12 - (чтение/запись) вкл/откл передачи кода СЧБК в МБК04 при обработке прерывания 2 <br>
 * 0x13 - (чтение/запись) вкл/откл передачи флага данных в МБК04 для режима двойной буферизации по прерыванию <br>
 * 0x14 - (чтение/запись) смена канала МКО для связи с КПА <br>
 * 0x15 - (чтение/запись) управление паузами в обработчиках прерываний <br>
 * 0x16 - (чтение/запись) управление сообщениями, выдаваемыми в МКПА при обработке прерывания 2
 * 0x17 - (только чтение) считывание контрольных сумм секторов флеша
 * 0x18 - (чтение/запись) установка функции конфигурирования БЭЧ
 * 0x19 - (чтение/запись) управление флагом выдачи квитанций в МБК04 (прерывание 2 ЕРТИ)
 * 0x1A - (чтение/запись) управление флагом режима имитации СЧБК 
 * 0x1B - считывание/сброс размера списка временных интервалов между преываниями АСН
 * 0x1C - считывание блока из списка временных интервалов между преываниями АСН
 * 0x1D - считывание/сброс размера списка достоверных ПУПС-ов 732
 * 0x1E - считывание блока из списка достоверных ПУПС-ов 732
 * 0x1F - (чтение/запись) управление флагом версии ОПО ЛКА
 * 0x20 - считывание параметра, переданного из ОПО при запуске СПО
 * 0x21 - создание и запуск дополнительной задачи СПО
 * 0x22 - вкл/откл установки бита EndOfMessage в функциях обмена по МКО
 * 0x23 - вкл/откл передачи дополняющего сообщения в функциях обмена по МКО
 * 0x24 - чтение абонента через функцию Input/SendPri()
 * 0x25 - вкл/откл передачи кода СЧБК в 732 при обработке прерывания 2
 * 0x26 - вкл/откл обработки состояния ПУПС = 19 в прерывании IRQ2
 * 0x27 - вкл/откл слежения за разницей времени прерываний от АСН и периодической задачи
 * 0x28 - чтение данных прерывания 10
 * 0x29 - управление флагом проверки команд вкл/откл аппаратуры передатчика
 * 0x2A - управление флагом проверки размера команд для ЛКА (4/28/TX)
 * 0x2B - управление флагом контроля состояния БУП-Е
 * 0x2C - перезапуск ЦБК в указанной конфигурации
 * 0x2D - включение/отключение call-back функции контроля ошибок
 * 0x2E - создание "мягкого" сбоя на конфигурации с 3 каналами ВМ
 * 0x2F - создание "мягкого" сбоя на конфигурации с 2 каналами ВМ
 * 0x30 - управление включением генератора при конфигурировании МБК07
 * 0x31 - чтение абонента с повтором при занятости
 * 0x32 - запись в абонент с повтором при занятости
 * 0x33 - контроль числа внешних секундных меток
 * 0x34 - перезадание параметров синхронизации ВМ
 * 0x35 - тест ВМ на число ошибок до реконфигурации
 */

#define LIMIT(Value, MinValue, MaxValue)                (((Value) < (MinValue)) ? (MinValue) : (((Value) > (MaxValue)) ? (MaxValue) : (Value)))

#define SET_CONFIG_FLAG(FlagName, FlagValue)            \
  FlagName = (FlagValue) ? (1) : (0);                   \
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ);

#define GET_CONFIG_FLAG(FlagName, FlagBuffer)           \
  FlagBuffer = (FlagName) ? (1) : (0);                  \
  ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ, &FlagBuffer, 2);
//================================================================================

void KpaCommandFF (int BaseMkoLine, int BaseMkoChan)
{
int Result, PrevIndex, NewIndex;
int Mode, Flag, Index, Param, Value;
WORD Buffer[32], PrevFlag, PrevCount;
int MkoLine, MkoChan, Addr, SubAddr, DataSize;

STATUS Status;
cfgSYS_t Config;
int VmCount, VmIndex;
UINT SyncRes, SyncBuf[4];

Mode  = KpaCommand[1] & 0xF000; // 0 - чтение флага, 1 - запись
Flag  = KpaCommand[1] & 0x0F00; // флаг, 0 или не-0
Index = KpaCommand[1] & 0x00FF; // номер флага
Param = KpaCommand[2];          // дополнительный параметр
Value = KpaCommand[3];          // дополнительный параметр

// выдать код версии
if (Index == 0x01)
  {
  ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ, (void*) &SpoVersionCode, sizeof(SpoVersionCode));
  return;
  }

// выдать строку версии
if (Index == 0x02)
  {
  ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ, (void*) &SpoVersionStr, strlen(SpoVersionStr) + 1);
  return;
  }

// вкл/выкл контроль множественных прерываний
if (Index == 0x03)
  {
  if (Mode) { SET_CONFIG_FLAG(SpoConfig.EnableMultiIrqControl, Flag); }
  else { GET_CONFIG_FLAG(SpoConfig.EnableMultiIrqControl, Buffer[0]); }
  return;
  }

// вкл/выкл выдачу секундной метки СПО
if (Index == 0x04)
  {
  if (Mode) { SET_CONFIG_FLAG(SpoConfig.EnableSecondMark, Flag); }
  else { GET_CONFIG_FLAG(SpoConfig.EnableSecondMark, Buffer[0]); }
  return;
  }

// вкл/выкл расширенную статистику ошибок на МКО
if (Index == 0x05)
  {
  if (Mode) { SET_CONFIG_FLAG(SpoConfig.EnableMkoErrorExtendedInfo, Flag); }
  else { GET_CONFIG_FLAG(SpoConfig.EnableMkoErrorExtendedInfo, Buffer[0]); }
  return;
  }

// вкл/выкл расширенный режим обмена на МКО (по адресам 2 и 4)
if (Index == 0x06)
  {
  if (Mode) { SET_CONFIG_FLAG(SpoConfig.EnableMkoExtXchg, Flag); }
  else { GET_CONFIG_FLAG(SpoConfig.EnableMkoExtXchg, Buffer[0]); }
  return;
  }

// вкл/выкл контроль числа ошибок при имитации СЧБК
if (Index == 0x07)
  {
  if (Mode) { SET_CONFIG_FLAG(SpoConfig.ShbkFailTimer, Flag); }
  else { GET_CONFIG_FLAG(SpoConfig.ShbkFailTimer, Buffer[0]); }
  return;
  }

// вкл/выкл режим двойной буферизации при записи в МБК04
if (Index == 0x08)
  {
  if (Mode) { SET_CONFIG_FLAG(SpoConfig.Mbk04DoubleBuffered, Flag); }
  else { GET_CONFIG_FLAG(SpoConfig.Mbk04DoubleBuffered, Buffer[0]); }
  return;
  }

// вкл/выкл выдачи дополнительного сообщения при пакетном обмене с МБК04
if (Index == 0x09)
  {
  if (Mode) { SET_CONFIG_FLAG(SpoConfig.Mbk04SendExtMessage, Flag); }
  else { GET_CONFIG_FLAG(SpoConfig.Mbk04SendExtMessage, Buffer[0]); }
  return;
  }

// вкл/выкл управления размером пакета/сообщения при пакетном обмене с МБК04
if (Index == 0x0A)
  {
  if (Mode) { SET_CONFIG_FLAG(SpoConfig.Mbk04ResizePacket, Flag); }
  else { GET_CONFIG_FLAG(SpoConfig.Mbk04ResizePacket, Buffer[0]); }
  return;
  }

// вкл/выкл наращивания размера сообщения при пакетном обмене с МБК04
if (Index == 0x0B)
  {
  if (Mode) { SET_CONFIG_FLAG(SpoConfig.Mbk04IncMessageSize, Flag); }
  else { GET_CONFIG_FLAG(SpoConfig.Mbk04IncMessageSize, Buffer[0]); }
  return;
  }

// вкл/выкл контроль числа ошибок при имитации квитанций
if (Index == 0x0C)
  {
  if (Mode) { SET_CONFIG_FLAG(SpoConfig.ReplyFailTimer, Flag); }
  else { GET_CONFIG_FLAG(SpoConfig.ReplyFailTimer, Buffer[0]); }
  return;
  }

// передаем строку с версией ОПО (Ansi/Win1251, ASCIIZ) 
if (Index == 0x0D)
  {
  ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ, &OpoVersionString, sizeof(OpoVersionString));
  return;
  }

// вкл/откл передачу сообщений об ошибках из задачи обработки ошибок
if (Index == 0x0E)
  {
  if (Mode) { SET_CONFIG_FLAG(SpoConfig.TaskErrorSendInfo, Flag); }
  else { GET_CONFIG_FLAG(SpoConfig.TaskErrorSendInfo, Buffer[0]); }
  return;
  }

// установить размер дополнительных данных, в словах, при передаче сообщений об ошибках из задачи обработки ошибок
// (размер данных из буфера ErrInf), от 0 до 8 слов
if (Index == 0x0F)
  {
  if (Mode)
    {
    SpoConfig.TaskErrorInfoSize = LIMIT(Flag, 0, 8);
    ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ);
    }
  else
    {
    Buffer[0] = SpoConfig.TaskErrorInfoSize;
    ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ, &Buffer, 2);
    }
  return;
  }

// передать массив МДИ из всех каналов ЦБК
if (Index == 0x10)
  {
  Result = testMDIGet((UINT*) &MdiBuffer, sizeof(MdiBuffer) / sizeof(UINT)); 
  if (Result != ERROR)
    {
    ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ, &MdiBuffer, sizeof(MdiBuffer));
    }
  else
    {
    ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE | REPLY_FLAG_ERROR, KPA_SUBADDR_IRQ);
    }
  return;
  }

// изменить положение маркера пакета для МБК04
if (Index == 0x11)
  {
  if (Mode)
    {
    PrevIndex = SpoConfig.Mbk04MarkerPosition;
    SpoConfig.Mbk04MarkerPosition = NewIndex = LIMIT(Param, 0, 30);

    PrevFlag  = CbkDataImit[0][PrevIndex + 0];
    PrevCount = CbkDataImit[0][PrevIndex + 1];
    CbkDataImit[0][PrevIndex + 0] = CbkDataImit[1][PrevIndex + 0];
    CbkDataImit[0][PrevIndex + 1] = CbkDataImit[1][PrevIndex + 1];
    CbkDataImit[0][NewIndex + 0] = PrevFlag;
    CbkDataImit[0][NewIndex + 1] = PrevCount;

    ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ);
    }
  else
    {
    Buffer[0] = SpoConfig.Mbk04MarkerPosition;
    ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ, &Buffer, 2);
    }
  return;
  }

// вкл/откл передачи кода СЧБК в МБК04 при обработке прерывания 2
if (Index == 0x12)
  {
  if (Mode) { SET_CONFIG_FLAG(SpoConfig.SendShbkToMbk04InIrq02, Flag); }
  else { GET_CONFIG_FLAG(SpoConfig.SendShbkToMbk04InIrq02, Buffer[0]); }
  return;
  }

// вкл/откл передачи флага данных в МБК04 для режима двойной буферизации по прерыванию <br>
if (Index == 0x13)
  {
  if (Mode) { SET_CONFIG_FLAG(SpoConfig.Mbk04DataFlagAfterIrq, Flag); }
  else { GET_CONFIG_FLAG(SpoConfig.Mbk04DataFlagAfterIrq, Buffer[0]); }
  return;
  }

// переключения канала МКО для связи с КПА
if (Index == 0x14)
  {
  if (Mode) 
    {
    SpoConfig.KpaMkoChan = (Flag) ? (1) : (0);
    ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ);
    } 
  else
    {
    Buffer[0] = (SpoConfig.KpaMkoChan) ? (1) : (0);
    ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ, &Buffer, 2);
    }
  return;
  }

// управление паузами в обработчиках прерываний
if (Index == 0x15)
  {
  if (Mode)
    {
    switch (Param)
      {
      case 1: { SpoConfig.ErtiIrq2Pause1 = Value; break; } // пауза при запуске обработчика IRQ2
      case 2: { SpoConfig.ErtiIrq2Pause2 = Value; break; } // пауза при обработке соcтояния ПУПС 4/15
      default: { ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DATA_ERROR, KPA_SUBADDR_IRQ); return; }
      }
    ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ);
    }
  else
    {
    switch (Param)
      {
      case 1: { Buffer[0] = SpoConfig.ErtiIrq2Pause1; break; } // пауза при запуске обработчика IRQ2
      case 2: { Buffer[0] = SpoConfig.ErtiIrq2Pause2; break; } // пауза при обработке соcтояния ПУПС 4/15
      default: { ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DATA_ERROR, KPA_SUBADDR_IRQ); return; }
      }
    ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ, &Buffer, 2);
    }
  return;
  }

// управление сообщениями, выдаваемыми в МКПА при обработке прерывания 2
if (Index == 0x16)
  {
  if (Mode)
    {
    SpoConfig.ErtiIrq2MsgMask = Param & (IRQ2_MSG_DONE | IRQ2_MSG_PROCESS | IRQ2_MSG_ERROR);
    ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ);
    }
  else
    {
    Buffer[0] = SpoConfig.ErtiIrq2MsgMask;
    ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ, &Buffer, 2);
    }
  return;
  }

// считывание контрольных сумм секторов флеша
if (Index == 0x17)
  {
  switch (Param)
    {
    // выдача контрольных сумм по алгоритму CRC32
    case 1: { ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ, &FlashSectorCSumTable[0], sizeof(DWORD) * 8); break; } 
    // выдача контрольных сумм по алгоритму CSumLong
    case 2: { ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ, &FlashSectorCSumTable[8], sizeof(DWORD) * 8); break; }
    default: { ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DATA_ERROR, KPA_SUBADDR_IRQ); break; }
    }
  return;
  }

// установка функции конфигурирования БЭЧ
if (Index == 0x18)
  {
  if (Mode)
    {
    switch (Param)
      {
      case 1: { CmdHandlerTable[0x0A] = KpaCommand0A_Koi; Result = REPLY_CMD_DONE; break; }
      case 2: { CmdHandlerTable[0x0A] = KpaCommand0A_Erti; Result = REPLY_CMD_DONE; break; }
      default: { Result = REPLY_CMD_DATA_ERROR; break; }
      }
    ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, Result, KPA_SUBADDR_IRQ);
    }
  else
    {
    Buffer[0] = (CmdHandlerTable[0x0A] == KpaCommand0A_Koi) ? (1) : (2);
    ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ, &Buffer, 2);
    }
  return;
  }

// управление флагом выдачи квитанций в МБК04 (прерывание 2 ЕРТИ)
if (Index == 0x19)
  {
  if (Mode) { SET_CONFIG_FLAG(SpoConfig.Mbk04Irq2ReplyMode, Flag); }
  else { GET_CONFIG_FLAG(SpoConfig.Mbk04Irq2ReplyMode, Buffer[0]); }
  return;
  }

// управление флагом режима имитации СЧБК 
if (Index == 0x1A)
  {
  if (Mode) { SET_CONFIG_FLAG(SpoConfig.ShbkImitMode, Flag); }
  else { GET_CONFIG_FLAG(SpoConfig.ShbkImitMode, Buffer[0]); }
  return;
  }

// считывание/сброс размера списка временных интервалов между преываниями АСН
if (Index == 0x1B)
  {
  if (!Mode)
    {
    Buffer[0] = AsnTimeListSize;
    ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ, &Buffer, 2);
    }
  else
    {
    AsnTimeListSize = 0;
    ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ);
    }
  return;
  }

// считывание блока из списка временных интервалов между преываниями АСН
if (Index == 0x1C)
  {                        
  // Param = Index      
  // Value = Size
  if ((Value >= 1) && (Value <= 32) && (Param < AsnTimeListSize) && ((Param + Value) <= AsnTimeListSize))
    {
    ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ, (void*) &AsnTimeList[Param], sizeof(WORD) * Value);
    }
  else
    {
    ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DATA_ERROR, KPA_SUBADDR_IRQ); 
    }
  return;
  }

// считывание/сброс размера списка достоверных ПУПС-ов 732
if (Index == 0x1D)
  {
  if (!Mode)
    {
    Buffer[0] = B732PupsListSize;
    ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ, &Buffer, 2);
    }
  else
    {
    B732PupsListSize = 0;
    ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ);
    }
  return;
  }

// считывание блока из списка достоверных ПУПС-ов 732
if (Index == 0x1E)
  {                        
  // Param = Index      
  // Value = Size
  if ((Value >= 1) && (Value <= 64) && (Param < B732PupsListSize) && ((Param + Value) <= B732PupsListSize))
    {
    ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ, (void*) &B732PupsList[Param], sizeof(BYTE) * Value);
    }
  else
    {
    ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DATA_ERROR, KPA_SUBADDR_IRQ); 
    }
  return;
  }

// управление флагом версии ОПО ЛКА
if (Index == 0x1F)
  {
  if (Mode) { SET_CONFIG_FLAG(SpoConfig.LkaOpoVersion, Flag); }
  else { GET_CONFIG_FLAG(SpoConfig.LkaOpoVersion, Buffer[0]); }
  return;
  }

// передача начального параметра стартовой задачи СПО
if (Index == 0x20)
  {
  ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ, (void*) &PostParam, sizeof(PostParam));
  return;
  }

// создание и запуск дополнительной задачи СПО
if (Index == 0x21)
  {
  Result = taskCreate(3, 0);
  if (Result) ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ);
  else ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE | REPLY_FLAG_ERROR, KPA_SUBADDR_IRQ);
  return;
  }

// вкл/откл установки бита EndOfMessage в функциях обмена по МКО
if (Index == 0x22)
  {
  if (Mode) { SET_CONFIG_FLAG(SpoConfig.EnableEomMkoFlag, Flag); }
  else { GET_CONFIG_FLAG(SpoConfig.EnableEomMkoFlag, Buffer[0]); }
  return;
  }

// вкл/откл передачи дополняющего сообщения в функциях обмена по МКО
if (Index == 0x23)
  {
  if (Mode) { SET_CONFIG_FLAG(SpoConfig.EnableCompMkoMsg, Flag); }
  else { GET_CONFIG_FLAG(SpoConfig.EnableCompMkoMsg, Buffer[0]); }
  return;
  }

// чтение абонента через функцию Input/SendPri()
if (Index == 0x24)
  {
  Input((Param) ? (1) : (0), 18, 29, 32, (USHORT*) &Buffer);     
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ);
  return;
  }

// вкл/откл передачи кода СЧБК в 732 при обработке прерывания 2
if (Index == 0x25)
  {
  if (Mode) { SET_CONFIG_FLAG(SpoConfig.SendShbkTo732InIrq02, Flag); }
  else { GET_CONFIG_FLAG(SpoConfig.SendShbkTo732InIrq02, Buffer[0]); }
  return;
  }

// вкл/откл обработки состояния ПУПС = 19 в прерывании IRQ2
if (Index == 0x26)
  {
  if (Mode) { SET_CONFIG_FLAG(SpoConfig.Irq02Pups19Enabled, Flag); }
  else { GET_CONFIG_FLAG(SpoConfig.Irq02Pups19Enabled, Buffer[0]); }
  return;
  }

// вкл/откл слежения за разницей времени прерываний от АСН и периодической задачи
if (Index == 0x27)
  {
  if (Param)  
    {
    // откл прерываний от АСН
    SpoConfig.EnabledIrqMask &= (~0x0001);
    sprIMaskVec(~SpoConfig.EnabledIrqMask);

    // подмена хендла IRQ АСН на отладочный
    IrqHandlerTable[0x00] = IrqHandler00TaskDeltaTime;

    // запуск периодической задачи (интервал 1 сек)
    AsnTaskDeltaTimeId = taskPeriodPlus(4, 0, 1, 0);

    // вкл прерываний от АСН
    SpoConfig.EnabledIrqMask |= 0x0001;
    sprIMaskVec(~SpoConfig.EnabledIrqMask);

    // установка интервала вывода данных
    if (Value < 1) Value = 1;
    if (Value > 60) Value = 60;
    AsnTaskDeltaTimeInterval = Value;
    AsnTaskDeltaTimeTimer = Value + 1;

    // установка флага слежения   
    AsnTaskDeltaTimeFlag = 1;
    }
  else
    {
    // откл прерываний от АСН
    SpoConfig.EnabledIrqMask &= (~0x0001);
    sprIMaskVec(~SpoConfig.EnabledIrqMask);

    // подмена хендла IRQ АСН на нормальный
    IrqHandlerTable[0x00] = IrqHandler00;

    // откл периодической задачи
    if (AsnTaskDeltaTimeId) { taskDelete(AsnTaskDeltaTimeId); AsnTaskDeltaTimeId = 0; }

    // сброс флага слежения   
    AsnTaskDeltaTimeFlag = 0;
    }
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ);
  return;
  }

// чтение данных прерывания 10
if (Index == 0x28)
  {
  if (Param == 0)
    {
    // сброс состояния
    Irq10Result[0] = 0xFFFF;
    Irq10Result[1] = 0;
    ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ);
    }
  else if (Param == 1)
    {
    // чтение текущего состояния
    ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ, (void*) &Irq10Result, sizeof(Irq10Result));
    }
  else if (Param == 2)
    {
    // чтение данных прерывания 10
    if (Value <= 17)
      {
      ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ, (void*) &Irq10Data[Value], 32 * sizeof(WORD));
      }
    else
      {
      ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DATA_ERROR, KPA_SUBADDR_IRQ);
      }
    }
  else
    {
    ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DATA_ERROR, KPA_SUBADDR_IRQ);
    }
  return;
  }

// управление флагом проверки команд вкл/откл аппаратуры передатчика
if (Index == 0x29)
  {
  if (Mode) { SET_CONFIG_FLAG(SpoConfig.EnableMBK07CmdCheck, Flag); }
  else { GET_CONFIG_FLAG(SpoConfig.EnableMBK07CmdCheck, Buffer[0]); }
  return;
  }

// управление флагом проверки размера команд для ЛКА (4/28/TX)
if (Index == 0x2A)
  {
  if (Mode) { SET_CONFIG_FLAG(SpoConfig.CheckLkaCmdSize, Flag); }
  else { GET_CONFIG_FLAG(SpoConfig.CheckLkaCmdSize, Buffer[0]); }
  return;
  }

// управление флагом контроля состояния БУП-Е
if (Index == 0x2B)
  {
  if (Mode) { SET_CONFIG_FLAG(SpoConfig.CheckBupState, Flag); }
  else { GET_CONFIG_FLAG(SpoConfig.CheckBupState, Buffer[0]); }
  return;
  }

// перезапуск ЦБК в указанной конфигурации
// только для ОПО v033
#ifdef OPO_V033
if (Index == 0x2C)
  {
  synchRestart(Param & 0x0F);
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE | REPLY_FLAG_ERROR, KPA_SUBADDR_IRQ);
  return;  
  }
#endif

// включение/отключение call-back функции контроля ошибок
// только для ОПО v033
#ifdef OPO_V033
if (Index == 0x2D)
  {
  if (Param)
    {
    CbkErrCtrlCallBackType = Value;
    Status = synchCtrl(0, 0, (void*) CbkErrCtrlCallBack);
    if (Status == OK) ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ);
    else ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE | REPLY_FLAG_ERROR, KPA_SUBADDR_IRQ);
    }
  else
    {
    Status = synchCtrl(0, 0, (void*) NULL);
    if (Status == OK) ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ);
    else ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE | REPLY_FLAG_ERROR, KPA_SUBADDR_IRQ);
    }
  return;  
  }
#endif

// создание "мягкого" сбоя на конфигурации с 3 каналами ВМ
if (Index == 0x2E)
  {
  Buffer[0] = 0xFF01; 
  Status = cfgSysGet(&Config);
  if (Status != OK) { ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE | REPLY_FLAG_ERROR, KPA_SUBADDR_IRQ, (void*) &Buffer, 2); return; }

  Buffer[0] = 0xFF02; 
  VmCount = 0;
  if (Config.vm & 0x01) VmCount++;
  if (Config.vm & 0x02) VmCount++;
  if (Config.vm & 0x04) VmCount++;
  if (Config.vm & 0x08) VmCount++;
  if (VmCount < 3) { ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE | REPLY_FLAG_ERROR, KPA_SUBADDR_IRQ, (void*) &Buffer, 2); return; } 

  VmIndex = numVMGet();
  memset(&SyncBuf, 0, sizeof(SyncBuf)); 
  if (Param == VmIndex) SyncBuf[0] = 1;

  SyncRes = synchInf(0, SyncBuf, 2);
  if (SyncRes == 0)          { Buffer[0] = 0xFF03; ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE | REPLY_FLAG_ERROR, KPA_SUBADDR_IRQ, (void*) &Buffer, 2); return; }
  else if (SyncRes == ERROR) { Buffer[0] = 0xFF04; ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE | REPLY_FLAG_ERROR, KPA_SUBADDR_IRQ, (void*) &Buffer, 2); }  
  else                       { Buffer[0] = 0xFF05; Buffer[1] = SyncRes; ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE | REPLY_FLAG_ERROR, KPA_SUBADDR_IRQ, (void*) &Buffer, 4); return; }  

  return;
  }

// создание "мягкого" сбоя на конфигурации с 2 каналами ВМ
if (Index == 0x2F)
  {
  Buffer[0] = 0xFF01; 
  Status = cfgSysGet(&Config);
  if (Status != OK) { ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE | REPLY_FLAG_ERROR, KPA_SUBADDR_IRQ, (void*) &Buffer, 2); return; }

  Buffer[0] = 0xFF02; 
  VmCount = 0;
  if (Config.vm & 0x01) VmCount++;
  if (Config.vm & 0x02) VmCount++;
  if (Config.vm & 0x04) VmCount++;
  if (Config.vm & 0x08) VmCount++;
  if (VmCount < 2) { ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE | REPLY_FLAG_ERROR, KPA_SUBADDR_IRQ, (void*) &Buffer, 2); return; } 

  VmIndex = numVMGet();
  memset(&SyncBuf, 0, sizeof(SyncBuf)); 
  if (Param == VmIndex) SyncBuf[0] = 1;

  SyncRes = synchInf(0, SyncBuf, 2);
  if (SyncRes == 0)          { Buffer[0] = 0xFF03; ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE | REPLY_FLAG_ERROR, KPA_SUBADDR_IRQ, (void*) &Buffer, 2); return; }
  else if (SyncRes == ERROR) { Buffer[0] = 0xFF04; ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE | REPLY_FLAG_ERROR, KPA_SUBADDR_IRQ, (void*) &Buffer, 2); }  
  else                       { Buffer[0] = 0xFF05; Buffer[1] = SyncRes; ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE | REPLY_FLAG_ERROR, KPA_SUBADDR_IRQ, (void*) &Buffer, 4); return; }  

  return;
  }

// управление включением генератора при конфигурировании МБК07
if (Index == 0x30)
  {
  if (Mode) { SET_CONFIG_FLAG(SpoConfig.EnableMBK07GenOn, Flag); }
  else { GET_CONFIG_FLAG(SpoConfig.EnableMBK07GenOn, Buffer[0]); }
  return;
  }

// чтение абонента с повтором при занятости
if (Index == 0x31)
  {
  // function MkoRead (MkoLine, MkoChan, Addr, SubAddr, WordCount, RetryPause);
  // W2 = (MkoLine & 1) | ((MkoChan & 1) << 1);
  // W3 = (Addr & 0x1F) | ((SubAddr & 0x1F) << 5) | ((WordCount & 0x3F) << 10);
  // ВЫДАТЬ_ДИР([0xFF, 0x31, W2, W3, RetryPause], []);

  // KpaCommand[0] = 0xFF
  // KpaCommand[1] = 0x31
  // KpaCommand[2] = (MkoLine & 1) | ((MkoChan & 1) << 1);
  // KpaCommand[3] = (Addr & 0x1F) | ((SubAddr & 0x1F) << 5) | ((WordCount & 0x3F) << 10);
  // KpaCommand[4] = (RetryPause);

  MkoLine  = (KpaCommand[2]) & 0x01;
  MkoChan  = (KpaCommand[2] >> 1) & 0x01;
  Addr     = (KpaCommand[3]) & 0x1F;
  SubAddr  = (KpaCommand[3] >> 5) & 0x1F;
  DataSize = (KpaCommand[3] >> 10) & 0x3F;

  SpoConfig.MkoReadAgainIfBusyFlag = 1;
  SpoConfig.MkoReadAgainIfBusyInt = KpaCommand[4]; 

  KpaCommand[0] = 0x04 | (DataSize << 8);
  KpaCommand[1] = (Addr) | (SubAddr << 5) | (MkoLine << 10) | (MkoChan << 11);
  KpaCommand04(BaseMkoLine, BaseMkoChan); 

  SpoConfig.MkoReadAgainIfBusyFlag = 0;
  return;
  }

// запись в абонент с повтором при занятости
if (Index == 0x32)
  {
  // function MkoWrite (MkoLine, MkoChan, Addr, SubAddr, WordCount, RetryPause, Data);
  // W2 = (MkoLine & 1) | ((MkoChan & 1) << 1);
  // W3 = (Addr & 0x1F) | ((SubAddr & 0x1F) << 5) | ((WordCount & 0x3F) << 10);
  // ВЫДАТЬ_ДИР([0xFF, 0x32, W2, W3, RetryPause], Data);

  // KpaCommand[0] = 0xFF
  // KpaCommand[1] = 0x32
  // KpaCommand[2] = (MkoLine & 1) | ((MkoChan & 1) << 1);
  // KpaCommand[3] = (Addr & 0x1F) | ((SubAddr & 0x1F) << 5) | ((WordCount & 0x3F) << 10);
  // KpaCommand[4] = (RetryPause);

  MkoLine  = (KpaCommand[2]) & 0x01;
  MkoChan  = (KpaCommand[2] >> 1) & 0x01;
  Addr     = (KpaCommand[3]) & 0x1F;
  SubAddr  = (KpaCommand[3] >> 5) & 0x1F;
  DataSize = (KpaCommand[3] >> 10) & 0x3F;

  SpoConfig.MkoWriteAgainIfBusyFlag = 1;
  SpoConfig.MkoWriteAgainIfBusyInt = KpaCommand[4]; 

  KpaCommand[0] = 0x03 | (DataSize << 8);
  KpaCommand[1] = (Addr) | (SubAddr << 5) | (MkoLine << 10) | (MkoChan << 11);
  KpaCommand03(BaseMkoLine, BaseMkoChan); 

  SpoConfig.MkoWriteAgainIfBusyFlag = 0;
  return;
  }

// контроль числа внешних секундных меток
// только для ОПО v033
#ifdef OPO_V033
if (Index == 0x33)
  {
  // интервал контроля от 1 до 60 сек
  if ((Param < 1) || (Param > 60))
    {
    ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DATA_ERROR, KPA_SUBADDR_IRQ);
    return;
    }
  // проверяем число поступивших сек меток за указанный интервал
  Value = CheckTimeMarkInInt(Param);
  ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ, (void*) &Value, 2); 
  return;
  }
#endif

// перезадание параметров синхронизации
// только для ОПО v033
#ifdef OPO_V033
if (Index == 0x34)
  {
  Status = synchCtrl(KpaCommand[2], KpaCommand[3], ((KpaCommand[4] == 1) ? ((void*) CbkErrCtrlCallBack) : (NULL)));
  Result = (Status == OK) ? (REPLY_CMD_DONE) : (REPLY_CMD_DONE | REPLY_FLAG_ERROR);
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, Result, KPA_SUBADDR_IRQ);
  return;
  }
#endif

// команда на создание рассогласований в 3-х машинной конфигурации
// только для ОПО v033
#ifdef OPO_V033
if (Index == 0x35)
  {
  // число попыток от 1 до 100
  if ((Value < 1) || (Value > 100))
    {
    ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DATA_ERROR, KPA_SUBADDR_IRQ);
    return;
    }
  // создаем последовательность рассогласований 
  Result = CheckVmDeSynch(Param, Value);
  Result = (!Result) ? (REPLY_CMD_DONE) : (REPLY_CMD_DONE | REPLY_FLAG_ERROR);
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, Result, KPA_SUBADDR_IRQ);
  return;
  }
#endif

// неизвестный код операции
ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE | REPLY_FLAG_ERROR, KPA_SUBADDR_IRQ);
}
//================================================================================

void CbkErrCtrlCallBack (void)
{
WORD Msg;
char Buffer[64];

CallBackCount++;

if (CbkErrCtrlCallBackType == 1)
  {
  Msg = 0xFA01;
  Output (1, 1, 25, 1, (WORD*) &Msg);
  memmove(&Buffer, &SpoVersionCode, sizeof(SpoVersionCode));  
  Output (1, 1, 25, 2, (WORD*) &Buffer);
  }

if (CbkErrCtrlCallBackType == 2)
  {
  Msg = 0xFA02;
  Output (1, 1, 25, 1, (WORD*) &Msg);
  memmove(&Buffer, (void*) 0x1FC001C0, 64);  
  Output (1, 1, 25, 32, (WORD*) &Buffer);
  }
}
//================================================================================

// проверяем число поступивших сек меток за указанный интервал
int CheckTimeMarkInInt (int TimeInt)
{
int MarkCount, i;

MarkCount = 0;
for (i = 0; i < TimeInt; i++)
  {
  taskDelay(1000);
  if (timeMarktst()) MarkCount++;
  }

return MarkCount;
}
//================================================================================

int CheckVmDeSynch (int VmNum, int PassCount)
{
STATUS Status;
int VmCount, VmIndex, i;
cfgSYS_t Config;
UINT SyncBuf[4];

Status = cfgSysGet(&Config);
if (Status != OK) { return 1; }

VmCount = 0;
if (Config.vm & 0x01) VmCount++;
if (Config.vm & 0x02) VmCount++;
if (Config.vm & 0x04) VmCount++;
if (Config.vm & 0x08) VmCount++;
if (VmCount < 3) { return 1; }

VmIndex = numVMGet();
for (i = 0; i < PassCount; i++)
  {
  memset(&SyncBuf, 0, sizeof(SyncBuf)); 
  if (VmIndex == VmNum) SyncBuf[0] = 1;
  synchInf(0, SyncBuf, 2);
  }

return 0;
}
//================================================================================

