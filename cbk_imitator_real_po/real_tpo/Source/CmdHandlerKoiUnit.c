
/*! \file CmdHandlerKoiUnit.c
 * \author Гапонов Р.В.
 * \date 2014.02.03
 * \version 1.1.0
 * \brief Модуль содержит функции обработки команд КПА, специфичных для комплекта аппаратуры КОИ
 */

#include "MainUnit.h"
//================================================================================

#ifdef SPO_KOI

/*! \fn void KpaCommand08 (int BaseMkoLine, int BaseMkoChan);
 * \brief Функция обработки команды 0x08: установить заданную конфигурацию 14Р732. Вызывается при обработке прерывания от КПА
 * \param [in] BaseMkoLine - номер линии МКО для обмена с КПА
 * \param [in] BaseMkoChan - номер канала МКО для обмена с КПА
 * \details <b> Формат данных команды: </b><br>
 * слово команды [0] биты [7..0] - код команды (0x08) <br>
 * слово команды [1] биты [1..0] - код конфигурации блока МУ 732 (0 - не менять конфигурацию, 1 - вкл осн канал, 2 - вкл рез канал, 3 - ошибка в команде) <br>
 * слово команды [1] биты [3..2] - код конфигурации блока МВКУ0 732 (0 - не менять конфигурацию, 1 - вкл осн канал, 2 - вкл рез канал, 3 - выключить все) <br>
 * слово команды [1] биты [5..4] - код конфигурации блока МПВН 732 (0 - не менять конфигурацию, 1 - вкл осн канал, 2 - вкл рез канал, 3 - выключить все) <br>
 * слово команды [1] биты [9..6] - маска конфигурации блока ВЧМ 732 (если бит маски = 1 - вкл канал, если = 0 - выкл канал) <br>
 * слово команды [1] биты [11..10] - код конфигурации блока МВКУ1 732 (0 - не менять конфигурацию, 1 - вкл осн канал, 2 - вкл рез канал, 3 - выключить все) <br>
 * слово команды [1] биты [13..12] - код конфигурации блока МВКУ2 732 (0 - не менять конфигурацию, 1 - вкл осн канал, 2 - вкл рез канал, 3 - выключить все) <br>
 * слово команды [2] - если не 0 = время на ожидание готовности блока МУ, мсек (если 0 = время на ожидание готовности = 300 мсек) <br>
 * слово команды [3] - если не 0 = пауза при включении ВЧМ, мсек (если 0 = пауза со значением по умолчанию = 100 мс) <br>
 * <b> Работа команды: </b><br>
 * производится проверка параметров команды на допустимость <br>
 * производится считывание текущей конфигурации 14Р732 <br>
 * производится анализ текущей и запрошенной конфигураций <br>
 * производится переключение конфигураций блоков (блоки ВЧМ всегда выключаются перед установкой конфигурации) <br>
 * производится повторное считывание конфигурации 14Р732 <br>
 * производится проверка соответствия запрошенной и установленной конфигураций <br>
 * производится передача результата выполнения на КПА <br>
 */

#define ADDR_732 2		// адрес 732 на шине МКО

void KpaCommand08 (int BaseMkoLine, int BaseMkoChan)
{
int Result, Index, CmdCount, WrongState;
int DevMkoLine, DevMkoChan;
WORD MUPR_Orig, MUPR_Want, MUPR_Prev;
WORD MVKU0_Orig, MVKU0_Want, MVKU0_Prev;
WORD MPVN_Orig, MPVN_Want, MPVN_Prev;
WORD VCHM_Orig, VCHM_Want, VCHM_Prev;
WORD MVKU1_Orig, MVKU1_Want, MVKU1_Prev;
WORD MVKU2_Orig, MVKU2_Want, MVKU2_Prev;
WORD State, Config[9], PrevConfig[9], CurrConfig[9];
WORD MuStartupTime, VchmStartupTime;

// параметры линий обмена
DevMkoLine = SpoConfig.DevMkoLine & 1;
DevMkoChan = SpoConfig.DevMkoChan & 1;

// биты новой конфигурации блоков
MUPR_Orig  = (KpaCommand[1] >> 0) & 0x03;  MUPR_Want  = MUPR_Orig;
MVKU0_Orig = (KpaCommand[1] >> 2) & 0x03;  MVKU0_Want = MVKU0_Orig;
MPVN_Orig  = (KpaCommand[1] >> 4) & 0x03;  MPVN_Want  = MPVN_Orig;
VCHM_Orig  = (KpaCommand[1] >> 6) & 0x0F;  VCHM_Want  = VCHM_Orig;
MVKU1_Orig = (KpaCommand[1] >> 10) & 0x03; MVKU1_Want = MVKU1_Orig;
MVKU2_Orig = (KpaCommand[1] >> 12) & 0x03; MVKU2_Want = MVKU2_Orig;

// пауза на включение/переключение блока МУ
MuStartupTime = (KpaCommand[2]) ? (KpaCommand[2]) : (300);

// пауза на включение ВЧМ
VchmStartupTime = (KpaCommand[3]) ? (KpaCommand[3]) : (100);

// проверка корректности кода конфигурации
if (MUPR_Want == 0x03) // ошибка кода состояния МУ
  {
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DATA_ERROR, KPA_SUBADDR_IRQ);
  return;
  }

// чтение текущей конфигурации 732
Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(2, 17, MKO_RX, 9), 0);
if (Result)
  {
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, DECODE_XCHG_ERROR(Result), KPA_SUBADDR_IRQ);
  return;
  }
SleepMSec(10);

// текущая конфигурация блоков 732
memmove(&PrevConfig, &MkoBufB[0], sizeof(PrevConfig));

MUPR_Prev  = (PrevConfig[0] >> 6) & 0x03;
MVKU0_Prev = (PrevConfig[1] >> 6) & 0x03;
MVKU1_Prev = (PrevConfig[2] >> 6) & 0x03;
MVKU2_Prev = (PrevConfig[3] >> 6) & 0x03;
MPVN_Prev  = (PrevConfig[4] >> 6) & 0x03;

VCHM_Prev = 0;
if ((PrevConfig[5] & 0x8180) == 0x8100) VCHM_Prev |= 0x01;
if ((PrevConfig[6] & 0x8180) == 0x8100) VCHM_Prev |= 0x02;
if ((PrevConfig[7] & 0x8180) == 0x8100) VCHM_Prev |= 0x04;
if ((PrevConfig[8] & 0x8180) == 0x8100) VCHM_Prev |= 0x08;

// коррекция для состояния "выключено"
if (!MVKU0_Prev) MVKU0_Prev = 0x03;
if (!MVKU1_Prev) MVKU1_Prev = 0x03;
if (!MVKU2_Prev) MVKU2_Prev = 0x03;
if (!MPVN_Prev)  MPVN_Prev  = 0x03;

// если запрашиваемая конфигурация уже установлена, сбрасываем запрос
if ((MUPR_Want)  && (MUPR_Want == MUPR_Prev))   MUPR_Want  = 0;
if ((MVKU0_Want) && (MVKU0_Want == MVKU0_Prev)) MVKU0_Want = 0;
if ((MVKU1_Want) && (MVKU1_Want == MVKU1_Prev)) MVKU1_Want = 0;
if ((MVKU2_Want) && (MVKU2_Want == MVKU2_Prev)) MVKU2_Want = 0;
if ((MPVN_Want)  && (MPVN_Want == MPVN_Prev))   MPVN_Want  = 0;

// если есть запрос на смену конфигурации блока МУ
if (MUPR_Want)
  {
  // переключаем канал блока МУ
  MkoBufB[0][0] = (MUPR_Want == 0x01) ? (0x0002) : (0x0004);
  Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(4, 28, MKO_TX, 1), 0);
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
  if (!MPVN_Want)  MPVN_Want  = MPVN_Prev;
  }

// всегда отключаем все ВЧМ
MkoBufB[0][0] = 0x8000;
Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(2, 17, MKO_TX, 1), 0);
if (Result)
  {
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, DECODE_XCHG_ERROR(Result), KPA_SUBADDR_IRQ);
  return;
  }
SleepMSec(100);

// собираем пакет команд для 732
CmdCount = 0;
if (MVKU0_Want) { Config[CmdCount++] = 0x2000 | ((MVKU0_Want & 0x03) << 6); }
if (MVKU1_Want) { Config[CmdCount++] = 0x2100 | ((MVKU1_Want & 0x03) << 6); }
if (MVKU2_Want) { Config[CmdCount++] = 0x2200 | ((MVKU2_Want & 0x03) << 6); }
if (MPVN_Want)  { Config[CmdCount++] = 0x5000 | ((MPVN_Want & 0x03) << 6); }
Config[CmdCount++] = 0x8000 | (VCHM_Want & 0x0F);

// переключаем конфигурацию блоков 732
memmove(&MkoBufB[0], &Config, CmdCount * sizeof(WORD));
Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(2, 17, MKO_TX, CmdCount), 0);
if (Result)
  {
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, DECODE_XCHG_ERROR(Result), KPA_SUBADDR_IRQ);
  return;
  }
SleepMSec(100);

// если есть включенные каналы ВЧМ - запускаем процессоры
if (VCHM_Want)
  {
  if (!SpoConfig.LkaOpoVersion)
    {
    MkoBufB[0][0] = 0x9830;
    Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(2, 17, MKO_TX, 1), 0);
    if (Result)
      {
      ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, DECODE_XCHG_ERROR(Result), KPA_SUBADDR_IRQ);
      return;
      }
    }
  SleepMSec(VchmStartupTime);
  }

// чтение новой конфигурации 732
Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(2, 17, MKO_RX, 9), 0);
if (Result)
  {
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, DECODE_XCHG_ERROR(Result), KPA_SUBADDR_IRQ);
  return;
  }
SleepMSec(10);

// новая конфигурация блоков 732
memmove(&CurrConfig, &MkoBufB[0], sizeof(CurrConfig));

// сравниваем с запрошеной конфигурацией
// (сравниваем все слово состояния, считаем что сбоев быть не должно)
WrongState = 0;

if (MUPR_Orig)
  {
  State = CurrConfig[0];
  if ((MUPR_Orig == 0x01) && (State != 0x1040)) WrongState = 1;
  if ((MUPR_Orig == 0x02) && (State != 0x1080)) WrongState = 1;
  }
if (MVKU0_Orig)
  {
  State = CurrConfig[1] & 0xFFFC;
  if ((MVKU0_Orig == 0x01) && (State != 0x2060)) WrongState = 2;
  if ((MVKU0_Orig == 0x02) && (State != 0x2090)) WrongState = 2;
  if ((MVKU0_Orig == 0x03) && (State != 0x2030)) WrongState = 2;
  }
if (MVKU1_Orig)
  {
  State = CurrConfig[2] & 0xFFFC;
  if ((MVKU1_Orig == 0x01) && (State != 0x2160)) WrongState = 3;
  if ((MVKU1_Orig == 0x02) && (State != 0x2190)) WrongState = 3;
  if ((MVKU1_Orig == 0x03) && (State != 0x2130)) WrongState = 3;
  }
if (MVKU2_Orig)
  {
  State = CurrConfig[3] & 0xFFFC;
  if ((MVKU2_Orig == 0x01) && (State != 0x2260)) WrongState = 4;
  if ((MVKU2_Orig == 0x02) && (State != 0x2290)) WrongState = 4;
  if ((MVKU2_Orig == 0x03) && (State != 0x2230)) WrongState = 4;
  }
if (MPVN_Orig)
  {
  State = CurrConfig[4] & 0xFFFC;
  if ((MPVN_Orig == 0x01) && (State != 0x5060)) WrongState = 5;
  if ((MPVN_Orig == 0x02) && (State != 0x5090)) WrongState = 5;
  if ((MPVN_Orig == 0x03) && (State != 0x5030)) WrongState = 5;
  }
if (1)
  {
  // проверка состояния ВЧМ (полная проверка статуса не производится, т.к. биты выставляются в течение 90с)
  for (Index = 0; Index < 4; Index++)
    {
    if (((VCHM_Orig & (1 << Index)) == 0) && ((CurrConfig[Index + 5] & 0x8100) != 0x8000)) WrongState = 6;
    if (((VCHM_Orig & (1 << Index)) != 0) && ((CurrConfig[Index + 5] & 0x8180) != 0x8100)) WrongState = 6;
    } 
  }

if (WrongState)
  {
  ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE | REPLY_FLAG_ERROR, KPA_SUBADDR_IRQ, &WrongState, 2);
  return;
  }

// конфигурация 732 успешно установлена
ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ);
}
//================================================================================

/*! \fn void KpaCommand0B (int BaseMkoLine, int BaseMkoChan);
 * \brief Функция обработки команды 0x0B: установить заданную конфигурацию 14Р733 и заданный режим УПИ. Вызывается при обработке прерывания от КПА
 * \param [in] BaseMkoLine - номер линии МКО для обмена с КПА
 * \param [in] BaseMkoChan - номер канала МКО для обмена с КПА
 * \details <b> Формат данных команды: </b><br>
 * слово команды [0] биты [7..0] - код команды (0x0B) <br>
 * слово команды [1] биты [1..0] - код конфигурации блока МУ 733 (0 = не менять конфигурацию, 1 = вкл осн канал, 2 = вкл рез канал, 3 = ошибка в команде) <br>
 * слово команды [1] биты [3..2] - код конфигурации блока МВКУ 733 (0 = не менять конфигурацию, 1 = вкл осн канал, 2 = вкл рез канал, 3 = выключить все) <br>
 * слово команды [1] биты [5..4] - код конфигурации блока МПВН 733 (0 = не менять конфигурацию, 1 = вкл осн канал, 2 = вкл рез канал, 3 = выключить все) <br>
 * слово команды [1] биты [9..6] - маска конфигурации блока ВЧМ 733 (если бит маски = 1 - вкл канал, если = 0 - выкл канал) <br>
 * слово команды [2] биты [1..0] - номер рабочего канала УПИ (0..3) <br>
 * слово команды [2] биты [3..2] - номер контрольного канала УПИ (0..3) <br>
 * слово команды [2] биты [5..4] - номер канала ФСЧ (0..3) <br>
 * слово команды [2] биты [7..6] - номер канала МБК07 (0..2) <br>
 * слово команды [2] бит [8] - нужно ли менять конфигурацию УПИ (1 = изменить, 0 = не трогать) <br>
 * слово команды [2] бит [9] - как менять конфигурацию УПИ (1 = установить требуемую конфигурацию, 0 = отключить питание) <br>
 * слово команды [3] - если не 0 = время на ожидание готовности блока МУ, мсек (если 0 = время на ожидание готовности = 300 мсек) <br>
 * слово команды [4] - если не 0 = пауза при включении ВЧМ, мсек (если 0 = пауза со значением по умолчанию = 100 мс) <br>
 * <b> Работа команды: </b><br>
 * производится проверка параметров команды на допустимость <br>
 * производится считывание текущей конфигурации 14Р733 <br>
 * производится анализ текущей и запрошенной конфигураций <br>
 * производится переключение конфигураций блоков (блоки ВЧМ всегда выключаются перед установкой конфигурации) <br>
 * производится повторное считывание конфигурации 14Р733 <br>
 * производится проверка соответствия запрошенной и установленной конфигураций <br>
 * если требуется смена конфигурации УПИ - производится изменение и проверка новой конфигурации УПИ <br>
 * производится передача результата выполнения на КПА <br>
 */

#define ADDR_733 6		// адрес 733 на шине МКО

void KpaCommand0B (int BaseMkoLine, int BaseMkoChan)
{
int Result, CmdCount;
int DevMkoLine, DevMkoChan;
WORD MUPR_Orig, MUPR_Want, MUPR_Prev;
WORD MVKU_Orig, MVKU_Want, MVKU_Prev;
WORD MPVN_Orig, MPVN_Want; // , MPVN_Prev;
WORD VCHM_Orig, VCHM_Want, VCHM_Prev;
WORD UPI_Orig, UPI_Change, UPI_Power;
WORD Config[6], PrevConfig[6];
WORD UPIConfig[4];
WORD MuStartupTime, VchmStartupTime;
WORD MVKU_SpecialMode, MVKU_Final;

// параметры линий обмена
DevMkoLine = SpoConfig.DevMkoLine & 1;
DevMkoChan = SpoConfig.DevMkoChan & 1;

// биты новой конфигурации блоков
MUPR_Orig = (KpaCommand[1] >> 0) & 0x03;  MUPR_Want = MUPR_Orig;
MVKU_Orig = (KpaCommand[1] >> 2) & 0x03;  MVKU_Want = MVKU_Orig;
MPVN_Orig = (KpaCommand[1] >> 4) & 0x03;  MPVN_Want = MPVN_Orig; // в текущем комплекте БКУПИ не используется
VCHM_Orig = (KpaCommand[1] >> 6) & 0x0F;  VCHM_Want = VCHM_Orig;

UPI_Orig   = KpaCommand[2] & 0x00FF;   // требуемая конфигурация УПИ
UPI_Change = (KpaCommand[2] >> 8) & 1; // 1 = требуется изменить конфигурацию УПИ
UPI_Power  = (KpaCommand[2] >> 9) & 1; // (при смене конфигурации) 0 = отключить УПИ, 1 = включить в указанной конфигурации

MVKU_Final = 0; // чтобы не было предупреждений от компилятора

// пауза на включение/переключение блока МУ
MuStartupTime = (KpaCommand[3]) ? (KpaCommand[3]) : (300);

// пауза на включение ВЧМ
VchmStartupTime = (KpaCommand[4]) ? (KpaCommand[4]) : (100);

// проверка корректности кода конфигурации
if (MUPR_Want == 0x03) // ошибка кода состояния МУ
  {
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DATA_ERROR, KPA_SUBADDR_IRQ);
  return;
  }

if ((UPI_Change) && ((UPI_Orig & 0xC0) == 0xC0)) // ошибка кода состояния МБК07
  {
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DATA_ERROR, KPA_SUBADDR_IRQ);
  return;
  }

// чтение текущей конфигурации 733
Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(ADDR_733, 17, MKO_RX, 6), 0);
if (Result)
  {
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, DECODE_XCHG_ERROR(Result), KPA_SUBADDR_IRQ);
  return;
  }
SleepMSec(10);

// текущая конфигурация блоков 733
memmove(&PrevConfig, &MkoBufB[0], sizeof(PrevConfig));

MUPR_Prev = (PrevConfig[0] >> 6) & 0x03;
MVKU_Prev = (PrevConfig[1] >> 6) & 0x03;

VCHM_Prev = 0;
if ((PrevConfig[2] & 0x8180) == 0x8100) VCHM_Prev |= 0x01;
if ((PrevConfig[3] & 0x8180) == 0x8100) VCHM_Prev |= 0x02;
if ((PrevConfig[4] & 0x8180) == 0x8100) VCHM_Prev |= 0x04;
if ((PrevConfig[5] & 0x8180) == 0x8100) VCHM_Prev |= 0x08;

// коррекция для состояния "выключено"
if (!MVKU_Prev) MVKU_Prev = 0x03;

// если запрашиваемая конфигурация уже установлена, сбрасываем запрос
if ((MUPR_Want) && (MUPR_Want == MUPR_Prev)) MUPR_Want = 0;
if ((MVKU_Want) && (MVKU_Want == MVKU_Prev)) MVKU_Want = 0;

// спец режим МВКУ при переключении конфигурации УПИ
MVKU_SpecialMode = 0;
if (UPI_Change)
  {
  if (MVKU_Want == 0x00) // не менять конфиг МВКУ
    {
    if (MVKU_Prev == 0x03)
      {
      MVKU_SpecialMode = 1;
      MVKU_Final = 0x03;
      MVKU_Orig = MVKU_Want = 0x01;
      }
    }
  if (MVKU_Want == 0x03) // выкл оба канала МВКУ
    {
    MVKU_SpecialMode = 1;
    MVKU_Final = 0x03;
    MVKU_Orig = MVKU_Want = 0x01;
    }
  }

// всегда отключаем все ВЧМ
MkoBufB[0][0] = 0x8000;
Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(ADDR_733, 17, MKO_TX, 1), 0);
if (Result)
  {
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, DECODE_XCHG_ERROR(Result), KPA_SUBADDR_IRQ);
  return;
  }
SleepMSec(100);

// если есть запрос на смену конфигурации блока МУ
if (MUPR_Want)
  {
  // переключаем канал блока МУ
  MkoBufB[0][0] = (MUPR_Want == 0x01) ? (0x0020) : (0x0040);
  Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(4, 28, MKO_TX, 1), 0);
  if (Result)
    {
    ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, DECODE_XCHG_ERROR(Result), KPA_SUBADDR_IRQ);
    return;
    }
  SleepMSec(MuStartupTime);

  // если нет запроса на установку новой конфигурации блоков, запрашиваем установку старой
  if (!MVKU_Want) MVKU_Want = MVKU_Prev;
  }

// собираем пакет команд для 733
CmdCount = 0;
if (MVKU_Want) { Config[CmdCount++] = 0x2000 | ((MVKU_Want & 0x03) << 6); }
Config[CmdCount++] = 0x8000 | (VCHM_Want & 0x0F); 

// переключаем конфигурацию блоков 733
if (CmdCount)
  {
  memmove(&MkoBufB[0], &Config, CmdCount * sizeof(WORD));
  Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(ADDR_733, 17, MKO_TX, CmdCount), 0);
  if (Result)
    {
    ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, DECODE_XCHG_ERROR(Result), KPA_SUBADDR_IRQ);
    return;
    }
  SleepMSec(100);
  }

// если есть включенные каналы ВЧМ - запускаем процессоры
if (VCHM_Want)
  {
  CmdCount = 0;
  if (VCHM_Want & 0x01) { Config[CmdCount++] = 0x9030; } 
  if (VCHM_Want & 0x02) { Config[CmdCount++] = 0x9230; } 
  if (VCHM_Want & 0x04) { Config[CmdCount++] = 0x9430; } 
  if (VCHM_Want & 0x08) { Config[CmdCount++] = 0x9630; } 
  
  if (CmdCount)
    {
    memmove(&MkoBufB[0], &Config, CmdCount * sizeof(WORD));
    Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(ADDR_733, 17, MKO_TX, CmdCount), 0);
    if (Result)
      {
      ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, DECODE_XCHG_ERROR(Result), KPA_SUBADDR_IRQ);
      return;
      }
    SleepMSec(VchmStartupTime);
    }
  }

// проверяем текущую конфигурацию 733
Result = Check733Config(BaseMkoLine, BaseMkoChan, DevMkoLine, DevMkoChan, MUPR_Orig, MVKU_Orig, VCHM_Orig);
if (Result) return;

// [изменение конфигурации модуля УПИ]

if (UPI_Change)
  {
  // чтение конфигурации УПИ 733
  Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(ADDR_733, 1, MKO_RX, 4), 0);
  if (Result)
    {
    ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, DECODE_XCHG_ERROR(Result), KPA_SUBADDR_IRQ);
    return;
    }
  SleepMSec(10);

  // конфигурация блоков УПИ 733
  memmove(&UPIConfig, &MkoBufB[0], sizeof(UPIConfig));

  if (UPI_Power)
    {
    // требуется включение модуля УПИ в указанной конфигурации

    // проверка состояния каналов УПИ
    if (((UPIConfig[0] & 0x0001) != 0) || ((UPIConfig[0] & 0x0100) != 0) || ((UPIConfig[1] & 0x0001) != 0) || ((UPIConfig[1] & 0x0100) != 0))
      {
      // какие то каналы не включены - включаем все
      MkoBufB[0][0] = 0x0055;
      Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(ADDR_733, 29, MKO_TX, 1), 0);
      if (Result)
        {
        ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, DECODE_XCHG_ERROR(Result), KPA_SUBADDR_IRQ);
        return;
        }
      SleepMSec(10);
  
      // ждем включения каналов УПИ
      SleepMSec(1000);

      // чтение новой конфигурации УПИ 733
      Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(ADDR_733, 1, MKO_RX, 4), 0);
      if (Result)
        {
        ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, DECODE_XCHG_ERROR(Result), KPA_SUBADDR_IRQ);
        return;
        }
      SleepMSec(10);

      // новая конфигурация блоков УПИ 733
      memmove(&UPIConfig, &MkoBufB[0], sizeof(UPIConfig));

      // проверка включения всех каналов УПИ
      if (((UPIConfig[0] & 0x0001) != 0) || ((UPIConfig[0] & 0x0100) != 0) || ((UPIConfig[1] & 0x0001) != 0) || ((UPIConfig[1] & 0x0100) != 0))
        {
        ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_733_UPI_ON_ERROR, KPA_SUBADDR_IRQ, &UPIConfig, 4);
        return;
        }

      // установка блоков УПИ в исходное состояние (передаем команду "установить в исходное состояние" в формате Ф4)
      Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(ADDR_733, 31, MKO_TX, 0x08), 1); // возможно правка подадреса приемника
      if (Result)
        {
        ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, DECODE_XCHG_ERROR(Result), KPA_SUBADDR_IRQ);
        return;
        }
      }

    // установка конфигурации модуля УПИ
    MkoBufB[0][0] = (UPI_Orig << 8) | UPI_Orig;
    Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(ADDR_733, 4, MKO_TX, 1), 0);
    if (Result)
      {
      ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, DECODE_XCHG_ERROR(Result), KPA_SUBADDR_IRQ);
      return;
      }
    SleepMSec(100);

    // проверка установленной конфигурации
    Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(ADDR_733, 1, MKO_RX, 4), 0);
    if (Result)
      {
      ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, DECODE_XCHG_ERROR(Result), KPA_SUBADDR_IRQ);
      return;
      }

    if (MkoBufB[0][2] != MkoBufB[0][3])
      {
      ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_733_UPI_STATE_ERROR, KPA_SUBADDR_IRQ);
      return;
      }
    if (MkoBufB[0][2] != ((UPI_Orig << 8) | UPI_Orig))
      {
      ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_733_UPI_STATE_ERROR, KPA_SUBADDR_IRQ);
      return;
      }
    }
  else
    {
    // требуется отключение модуля УПИ

    // проверка состояния каналов УПИ
    if (((UPIConfig[0] & 0x0001) == 0) || ((UPIConfig[0] & 0x0100) == 0) || ((UPIConfig[1] & 0x0001) == 0) || ((UPIConfig[1] & 0x0100) == 0))
      {      
      // какие то каналы не отключены - отключаем все
      MkoBufB[0][0] = 0x00AA;
      Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(ADDR_733, 29, MKO_TX, 1), 0);
      if (Result)
        {
        ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, DECODE_XCHG_ERROR(Result), KPA_SUBADDR_IRQ);
        return;
        }
      SleepMSec(10);
  
      // ждем отключения каналов УПИ
      SleepMSec(100);

      // чтение новой конфигурации УПИ 733
      Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(ADDR_733, 1, MKO_RX, 4), 0);
      if (Result)
        {
        ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, DECODE_XCHG_ERROR(Result), KPA_SUBADDR_IRQ);
        return;
        }
      SleepMSec(10);

      // новая конфигурация блоков УПИ 733
      memmove(&UPIConfig, &MkoBufB[0], sizeof(UPIConfig));

      // проверка отключения всех каналов УПИ
      if (((UPIConfig[0] & 0x0001) == 0) || ((UPIConfig[0] & 0x0100) == 0) || ((UPIConfig[1] & 0x0001) == 0) || ((UPIConfig[1] & 0x0100) == 0))
        {
        ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_733_UPI_OFF_ERROR, KPA_SUBADDR_IRQ, &UPIConfig, 4);
        return;
        }
      }
    }
  }

// спец режим МВКУ при переключении конфигурации УПИ
if (MVKU_SpecialMode)
  {
  // завершающая конфигурация МВКУ
  MVKU_Orig = MVKU_Want = MVKU_Final;

  MkoBufB[0][0] = 0x2000 | ((MVKU_Want & 0x03) << 6);

  // переключаем конфигурацию блоков 733
  Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(ADDR_733, 17, MKO_TX, 1), 0);
  if (Result)
    {
    ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, DECODE_XCHG_ERROR(Result), KPA_SUBADDR_IRQ);
    return;
    }
  SleepMSec(100);

  // еще раз проверяем конфигурацию 733
  Result = Check733Config(BaseMkoLine, BaseMkoChan, DevMkoLine, DevMkoChan, MUPR_Orig, MVKU_Orig, VCHM_Orig);
  if (Result) return;
  }

// конфигурация 733 успешно установлена
ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ);
}
//================================================================================

/*! \fn int Check733Config (int BaseMkoLine, int BaseMkoChan, int DevMkoLine, int DevMkoChan, WORD MUPR_Orig, WORD MVKU_Orig, WORD VCHM_Orig);
 * \brief Служебная функция проверки конфигурации блока 733
 * \param [in] BaseMkoLine - номер линии МКО для обмена с КПА
 * \param [in] BaseMkoChan - номер канала МКО для обмена с КПА
 * \param [in] DevMkoLine - номер линии МКО для обмена с БА
 * \param [in] DevMkoChan - номер канала МКО для обмена с БА
 * \param [in] MUPR_Orig - конфигурация блока МУ для проверки
 * \param [in] MVKU_Orig - конфигурация блока МВКУ для проверки
 * \param [in] VCHM_Orig - конфигурация блока ВЧМ для проверки
 * \return Возвращает: 0 - при успешном завершении, не 0 - при ошибках
 * \details <b> Работа команды: </b><br>
 * считывается текущая конфигурация блока 733 <br>
 * производится проверка соответствия текущей конфигурации запрошенной <br>
 * при ошибках производится передача результата выполнения на КПА <br>
 */

int Check733Config (int BaseMkoLine, int BaseMkoChan, int DevMkoLine, int DevMkoChan, WORD MUPR_Orig, WORD MVKU_Orig, WORD VCHM_Orig)
{
int Result, Index, WrongState;
WORD State, CurrConfig[6];

// чтение новой конфигурации 733
Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(ADDR_733, 17, MKO_RX, 6), 0);
if (Result)
  {
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, DECODE_XCHG_ERROR(Result), KPA_SUBADDR_IRQ);
  return 1;
  }
SleepMSec(10);

// новая конфигурация блоков 733
memmove(&CurrConfig, &MkoBufB[0], sizeof(CurrConfig));

// сравниваем с запрошеной конфигурацией
WrongState = 0;
if (MUPR_Orig)
  {
  State = CurrConfig[0];
  if ((MUPR_Orig == 0x01) && (State != 0x1040)) WrongState = 1;
  if ((MUPR_Orig == 0x02) && (State != 0x1080)) WrongState = 1;
  }
if (MVKU_Orig)
  {
  State = CurrConfig[1] & 0xFFFC;
  if ((MVKU_Orig == 0x01) && (State != 0x2060)) WrongState = 2; // *
  if ((MVKU_Orig == 0x02) && (State != 0x2090)) WrongState = 2; // *
  if ((MVKU_Orig == 0x03) && (State != 0x2030)) WrongState = 2; // *
  }
if (1)
  {
  // проверка состояния ВЧМ (полная проверка статуса не производится, т.к. биты выставляются в течение 90с)
  for (Index = 0; Index <= 3; Index++)
    {
    if (((VCHM_Orig & (1 << Index)) == 0) && ((CurrConfig[Index + 2] & 0x8100) != 0x8000)) WrongState = 3;
    if (((VCHM_Orig & (1 << Index)) != 0) && ((CurrConfig[Index + 2] & 0x8180) != 0x8100)) WrongState = 3;
    } 
  }

if (WrongState)
  {
  ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE | REPLY_FLAG_ERROR, KPA_SUBADDR_IRQ, &WrongState, 2);
  return 1;
  }

return 0;
}
//================================================================================

#endif

