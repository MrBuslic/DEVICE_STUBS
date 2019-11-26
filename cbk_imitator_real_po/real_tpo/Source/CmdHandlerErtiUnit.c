
/*! \file CmdHandlerUnit.c
 * \author Гапонов Р.В.
 * \date 2014.02.03
 * \version 1.1.0
 * \brief Модуль содержит функции обработки команд КПА, специфичных для комплекта аппаратуры ЕРТИ
 */

#include "MainUnit.h"
//================================================================================

#ifdef SPO_ERTI

/*! \fn void KpaCommand08 (int BaseMkoLine, int BaseMkoChan);
 * \brief Функция обработки команды 0x08: установить заданную конфигурацию 14Р732. Вызывается при обработке прерывания от КПА
 * \param [in] BaseMkoLine - номер линии МКО для обмена с КПА
 * \param [in] BaseMkoChan - номер канала МКО для обмена с КПА
 * \details <b> Формат данных команды: </b><br>
 * слово команды [0] биты [7..0] - код команды (0x08) <br>
 * слово команды [1] биты [1..0] - код конфигурации блока МУ (0 - не менять конфигурацию, 1 - вкл осн канал, 2 - вкл рез канал, 3 - ошибка в команде) <br>
 * слово команды [1] биты [3..2] - код конфигурации блока МВКУ (0 - не менять конфигурацию, 1 - вкл осн канал, 2 - вкл рез канал, 3 - выключить все) <br>
 * слово команды [1] биты [5..4] - код конфигурации блока МПВН (0 - не менять конфигурацию, 1 - вкл осн канал, 2 - вкл рез канал, 3 - выключить все) <br>
 * слово команды [1] биты [9..6] - маска конфигурации блока ВЧМ (если бит маски = 1 - вкл канал, если = 0 - выкл канал) <br>
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
WORD MVKU_Orig, MVKU_Want, MVKU_Prev; 
WORD MPVN_Orig, MPVN_Want, MPVN_Prev; 
WORD VCHM_Orig, VCHM_Want, VCHM_Prev;
WORD State, Config[15], PrevConfig[15], CurrConfig[15];
WORD MuStartupTime, VchmStartupTime;

// параметры линий обмена
DevMkoLine = SpoConfig.DevMkoLine & 1;
DevMkoChan = SpoConfig.DevMkoChan & 1;

// биты новой конфигурации блоков
MUPR_Orig = (KpaCommand[1] >> 0) & 0x03; MUPR_Want = MUPR_Orig;
MVKU_Orig = (KpaCommand[1] >> 2) & 0x03; MVKU_Want = MVKU_Orig;
MPVN_Orig = (KpaCommand[1] >> 4) & 0x03; MPVN_Want = MPVN_Orig;
VCHM_Orig = (KpaCommand[1] >> 6) & 0x0F; VCHM_Want = VCHM_Orig;

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
Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(2, 17, MKO_RX, 15), 0);
if (Result)
  {
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, DECODE_XCHG_ERROR(Result), KPA_SUBADDR_IRQ);
  return;
  }
SleepMSec(10);

// текущая конфигурация блоков 732
memmove(&PrevConfig, &MkoBufB[0], sizeof(PrevConfig));

MUPR_Prev = (PrevConfig[0] >> 6) & 0x03;
MVKU_Prev = (PrevConfig[1] >> 6) & 0x03;
MPVN_Prev = (PrevConfig[2] >> 6) & 0x03;

VCHM_Prev = 0;
if ((PrevConfig[3] & 0x8180) == 0x8180) VCHM_Prev |= 0x01;
if ((PrevConfig[4] & 0x8180) == 0x8180) VCHM_Prev |= 0x02;
if ((PrevConfig[5] & 0x8180) == 0x8180) VCHM_Prev |= 0x04;
if ((PrevConfig[6] & 0x8180) == 0x8180) VCHM_Prev |= 0x08;

// коррекция для состояния "выключено"
if (!MVKU_Prev) MVKU_Prev = 0x03;
if (!MPVN_Prev) MPVN_Prev = 0x03;

// если запрашиваемая конфигурация уже установлена, сбрасываем запрос
if ((MUPR_Want) && (MUPR_Want == MUPR_Prev)) MUPR_Want = 0;
if ((MVKU_Want) && (MVKU_Want == MVKU_Prev)) MVKU_Want = 0;
if ((MPVN_Want) && (MPVN_Want == MPVN_Prev)) MPVN_Want = 0;

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
  if (!MVKU_Want) MVKU_Want = MVKU_Prev;
  if (!MPVN_Want) MPVN_Want = MPVN_Prev;
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
if (MVKU_Want) { Config[CmdCount++] = 0x2000 | ((MVKU_Want & 0x03) << 6); }
if (MPVN_Want) { Config[CmdCount++] = 0x5000 | ((MPVN_Want & 0x03) << 6); }
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
Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(2, 17, MKO_RX, 15), 0);
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
if (MVKU_Orig)
  {
  State = CurrConfig[1] & 0xFFFC;
  if ((MVKU_Orig == 0x01) && (State != 0x2040)) WrongState = 2;
  if ((MVKU_Orig == 0x02) && (State != 0x2080)) WrongState = 2;
  if ((MVKU_Orig == 0x03) && (State != 0x2010)) WrongState = 2;
  }
if (MPVN_Orig)
  {
  State = CurrConfig[2] & 0xFFEC;
  if ((MPVN_Orig == 0x01) && (State != 0x5040)) WrongState = 3;
  if ((MPVN_Orig == 0x02) && (State != 0x5080)) WrongState = 3;
  if ((MPVN_Orig == 0x03) && (State != 0x5000)) WrongState = 3;
  }
if (1)
  {
  // проверка состояния ВЧМ (полная проверка статуса не производится, т.к. биты выставляются в течение 90с)
  for (Index = 0; Index < 4; Index++)
    {
    if (((VCHM_Orig & (1 << Index)) == 0) && ((CurrConfig[Index + 5] & 0x8100) != 0x8000)) WrongState = 4;
    if (((VCHM_Orig & (1 << Index)) != 0) && ((CurrConfig[Index + 5] & 0x8180) != 0x8180)) WrongState = 4;
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
int Status, Config, Result;

do
  {
  Status = 0;
  Config = REPLY_CMD_DONE;

  if (sprIQClear(6) != OK) break;                 // сброс запроса прерывания от 733
  Config = Reconfig733(BaseMkoLine, BaseMkoChan); // выполняем конфигурирование 733

  Status = 1;
  }
while (0);

// высылаем результат
if (Config != REPLY_CMD_DONE) Result = Config;
else Result = (Status) ? (REPLY_CMD_DONE) : (REPLY_CMD_DONE | REPLY_FLAG_ERROR);

ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, Result, KPA_SUBADDR_IRQ);
}
//================================================================================

// переключение конфигурации 733, возвращает код результата
int Reconfig733 (int BaseMkoLine, int BaseMkoChan)
{
int Result, Index;
int DevMkoLine, DevMkoChan;
WORD MUPR_Orig, MUPR_Want;
WORD MVKU_Orig, MVKU_Want, MVKU_Prev, MVKU_SpecialMode, MVKU_Final;
WORD VCHM_Orig, VCHM_Want;
WORD UPI_Orig, UPI_Change, UPI_Power;
WORD MuStartupTime, VchmStartupTime;
sysTime_t BeginTime, CurrTime;
WORD Config[15], UpiConfig[4];

// ****

// параметры линий обмена
DevMkoLine = SpoConfig.DevMkoLine & 1;
DevMkoChan = SpoConfig.DevMkoChan & 1;

// биты новой конфигурации блоков
MUPR_Orig = (KpaCommand[1] >> 0) & 0x03;  MUPR_Want = MUPR_Orig;
MVKU_Orig = (KpaCommand[1] >> 2) & 0x03;  MVKU_Want = MVKU_Orig;
VCHM_Orig = (KpaCommand[1] >> 6) & 0x0F;  VCHM_Want = VCHM_Orig;

MVKU_Final = 0; // чтобы не было предупреждений от компилятора

UPI_Orig   = KpaCommand[2] & 0x00FF;      // требуемая конфигурация УПИ
UPI_Change = (KpaCommand[2] >> 8) & 0x01; // 1 = требуется изменить конфигурацию УПИ
UPI_Power  = (KpaCommand[2] >> 9) & 0x01; // (при смене конфигурации) 0 = отключить УПИ, 1 = включить в указанной конфигурации

// пауза на включение/переключение блока МУ
MuStartupTime = (KpaCommand[3]) ? (KpaCommand[3]) : (300);

// пауза на включение ВЧМ
VchmStartupTime = (KpaCommand[4]) ? (KpaCommand[4]) : (100);

// проверка корректности кода конфигурации
if (MUPR_Want == 0x03) return REPLY_CMD_DATA_ERROR; // ошибка кода состояния МУ
if ((UPI_Change) && ((UPI_Orig & 0xC0) == 0xC0)) return REPLY_CMD_DATA_ERROR; // ошибка кода состояния МБК07 

// ****

// чтение текущей конфигурации
Result = Read733Config(DevMkoLine, DevMkoChan, &Config);
if (Result) return Result;

// текущая конфигурация блока МВКУ
MVKU_Prev = (Config[1] >> 6) & 0x03;
if (!MVKU_Prev) MVKU_Prev = 0x03; // коррекция для отключенного состояния

// если запрашиваемая конфигурация уже установлена, сбрасываем запрос
if ((MVKU_Want) && (MVKU_Want == MVKU_Prev)) MVKU_Want = 0x00;

// спец режим МВКУ при переключении конфигурации УПИ
// т.к. для переключения нужен включенный канал МВКУ
MVKU_SpecialMode = 0;

if (UPI_Change)
  {
  if (MVKU_Want == 0x00) // если нет запроса на смену конфигурации МВКУ
    {
    if (MVKU_Prev == 0x03)
      {
      MVKU_SpecialMode = 1;
      MVKU_Final = 0x03;
      MVKU_Orig = MVKU_Want = 0x01;
      }
    }

  if (MVKU_Want == 0x03) // если есть запрос на отключение МВКУ
    {
    MVKU_SpecialMode = 1;
    MVKU_Final = 0x03;
    MVKU_Orig = MVKU_Want = 0x01;
    }
  }

// ****

// проверяем конфигурацию блока МУ
if ((Config[0] != 0x1040) && (Config[0] != 0x1080)) return REPLY_733_MU_WRONG_CONFIG;

// если нет запроса на смену конфигурации МУ, ставим запрос на установку текущей конфигурации
if (!MUPR_Want) MUPR_Want = (Config[0] == 0x1040) ? (0x01) : (0x02);

// выдаем запрос на включение другого МУ (+ запуск ВЧМ)
MkoBufB[0][0] = (MUPR_Want == 0x01) ? (0x0040) : (0x0020);
Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(4, 28, MKO_TX, 1), 0);
if (Result) return DECODE_XCHG_ERROR(Result);

// ждем переключения блока МУ
SleepMSec(5000);

// выдаем запрос на включение требуемого МУ
MkoBufB[0][0] = (MUPR_Want == 0x01) ? (0x0020) : (0x0040);
Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(4, 28, MKO_TX, 1), 0);
if (Result) return DECODE_XCHG_ERROR(Result);

// ждем переключения блока МУ
SleepMSec(MuStartupTime);

// если нет запроса на установку новой конфигурации МВКУ, запрашиваем установку старой
if (!MVKU_Want) MVKU_Want = MVKU_Prev;

// ****

// чтение текущей конфигурации
Result = Read733Config(DevMkoLine, DevMkoChan, &Config);
if (Result) return Result;

// проверка конфигурации блоков
if ((MUPR_Want == 0x01) && (Config[0] != 0x1040)) return REPLY_733_MU_WRONG_CONFIG;
if ((MUPR_Want == 0x02) && (Config[0] != 0x1080)) return REPLY_733_MU_WRONG_CONFIG;
if (Config[1] != 0x2040) return REPLY_733_MVKU_WRONG_CONFIG;
if (Config[2] != 0x5050) return REPLY_733_MPVN_WRONG_CONFIG;
if ((Config[5] != 0x8100) || (Config[6] != 0x8300) || (Config[7] != 0x8500) || (Config[8] != 0x8700)) return REPLY_733_WRONG_VM_STATE;

// выключаем блок МПВН
MkoBufB[0][0] = 0x50C0;
Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(ADDR_733, 17, MKO_TX, 1), 0);
if (Result) return DECODE_XCHG_ERROR(Result);

// ждем отключения блока МПВН
SleepMSec(100);

// чтение текущей конфигурации
Result = Read733Config(DevMkoLine, DevMkoChan, &Config);
if (Result) return Result;

// проверка конфигурации блока МПВН
if (Config[2] != 0x5010) return REPLY_733_MPVN_WRONG_CONFIG;

// переключаем конфигурацию МВКУ
MkoBufB[0][0] = 0x2000 | (MVKU_Want << 6);
Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(ADDR_733, 17, MKO_TX, 1), 0);
if (Result) return DECODE_XCHG_ERROR(Result);

// ждем переключения блока МВКУ
SleepMSec(100);

// чтение текущей конфигурации
Result = Read733Config(DevMkoLine, DevMkoChan, &Config);
if (Result) return Result;

// проверяем новую конфигурацию МВКУ
if ((MVKU_Want == 0x01) && (Config[1] != 0x2040)) return REPLY_733_MVKU_WRONG_CONFIG;
if ((MVKU_Want == 0x02) && (Config[1] != 0x2080)) return REPLY_733_MVKU_WRONG_CONFIG;
if ((MVKU_Want == 0x03) && (Config[1] != 0x2010)) return REPLY_733_MVKU_WRONG_CONFIG;

// ****

// ждем прерывание от ВЧМ
timeRead(&BeginTime);
while (sprIGetF(6) == 0)
  {
  timeRead(&CurrTime);
  if (DeltaTimeMSec(&BeginTime, &CurrTime) > VchmStartupTime) return REPLY_733_IRQ_TIMEOUT;
  }

// сбрасываем запрос на прерывание от ВЧМ
if (sprIQClear(6) != OK) return REPLY_733_IRQ_CLR_ERROR;

// ****

// чтение текущей конфигурации
Result = Read733Config(DevMkoLine, DevMkoChan, &Config);
if (Result) return Result;

// проверка текущей конфигурации ВЧМ
if ((Config[5] != 0x8101) || (Config[6] != 0x8301) || (Config[7] != 0x8501) || (Config[8] != 0x8701)) return REPLY_733_WRONG_VM_STATE;

// сброс ПУПС ВЧМ
if (!SpoConfig.LkaOpoVersion)
  {
  MkoBufB[0][0] = 0xA83F;
  Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(ADDR_733, 17, MKO_TX, 1), 0);
  if (Result) return DECODE_XCHG_ERROR(Result);
  }
else
  {
  MkoBufB[0][0] = 0x0D00;
  Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(ADDR_733, 19, MKO_TX, 1), 0);
  if (Result) return DECODE_XCHG_ERROR(Result);
  }

// чтение текущей конфигурации
Result = Read733Config(DevMkoLine, DevMkoChan, &Config);
if (Result) return Result;

// проверка текущей конфигурации ВЧМ
if ((Config[5] != 0x8100) || (Config[6] != 0x8300) || (Config[7] != 0x8500) || (Config[8] != 0x8700)) return REPLY_733_PUPS_CLR_ERROR;

// ****

// изменение конфигурации модуля УПИ

if (UPI_Change)
  {
  // чтение конфигурации УПИ
  Result = Read733UpiConfig(DevMkoLine, DevMkoChan, &UpiConfig);
  if (Result) return Result;

  if (UPI_Power)
    {
    // требуется включение модуля УПИ в указанной конфигурации

    // проверка состояния каналов УПИ
    if (((UpiConfig[0] & 0x0101) != 0) || ((UpiConfig[1] & 0x0101) != 0))
      {
      // какие то каналы УПИ не включены - включаем все
      MkoBufB[0][0] = 0x0055;
      Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(ADDR_733, 29, MKO_TX, 1), 0);
      if (Result) return DECODE_XCHG_ERROR(Result);
  
      // ждем включения каналов УПИ
      SleepMSec(1000);

      // чтение новой конфигурации УПИ
      Result = Read733UpiConfig(DevMkoLine, DevMkoChan, &UpiConfig);
      if (Result) return Result;

      // проверка включения всех каналов УПИ
      if (((UpiConfig[0] & 0x0101) != 0) || ((UpiConfig[1] & 0x0101) != 0)) return REPLY_733_UPI_ON_ERROR;

      // установка блоков УПИ в исходное состояние (передаем команду "установить в исходное состояние" в формате Ф4)
      Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(ADDR_733, 31, MKO_TX, 0x08), 1); // возможно правка подадреса приемника
      if (Result) return DECODE_XCHG_ERROR(Result);

      // чтение новой конфигурации УПИ
      Result = Read733UpiConfig(DevMkoLine, DevMkoChan, &UpiConfig);
      if (Result) return Result;

      // проверка установки УПИ в исходное состояние
      if ((UpiConfig[2] != 0) || (UpiConfig[3] != 0)) return REPLY_733_UPI_RESET_ERROR;
      }

    // установка конфигурации УПИ
    MkoBufB[0][0] = (UPI_Orig << 8) | UPI_Orig;
    Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(ADDR_733, 4, MKO_TX, 1), 0);
    if (Result) return DECODE_XCHG_ERROR(Result);

    // ждем установки конфигурации УПИ
    SleepMSec(100);

    // проверка установленной конфигурации УПИ
    Result = Read733UpiConfig(DevMkoLine, DevMkoChan, &UpiConfig);
    if (Result) return Result;

    if (UpiConfig[2] != UpiConfig[3]) return REPLY_733_UPI_STATE_ERROR;
    if (UpiConfig[2] != ((UPI_Orig << 8) | UPI_Orig)) return REPLY_733_UPI_STATE_ERROR;
    }
  else
    {
    // требуется отключение модуля УПИ

    // проверка состояния каналов УПИ
    if (((UpiConfig[0] & 0x0101) != 0x0101) || ((UpiConfig[1] & 0x0101) != 0x0101))
      {      
      // какие то каналы УПИ не отключены - отключаем все
      MkoBufB[0][0] = 0x00AA;
      Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(ADDR_733, 29, MKO_TX, 1), 0);
      if (Result) return DECODE_XCHG_ERROR(Result);
  
      // ждем отключения каналов УПИ
      SleepMSec(100);

      // чтение конфигурации УПИ
      Result = Read733UpiConfig(DevMkoLine, DevMkoChan, &UpiConfig);
      if (Result) return Result;

      // проверка отключения всех каналов УПИ
      if (((UpiConfig[0] & 0x0101) != 0x0101) || ((UpiConfig[1] & 0x0101) != 0x0101)) return REPLY_733_UPI_OFF_ERROR;
      }
    }
  }

// ****

// спец режим МВКУ при переключении конфигурации УПИ
if (MVKU_SpecialMode)
  {
  // завершающая конфигурация МВКУ
  MVKU_Orig = MVKU_Want = MVKU_Final;

  // переключаем конфигурацию МВКУ
  MkoBufB[0][0] = 0x2000 | ((MVKU_Want & 0x03) << 6);
  Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(ADDR_733, 17, MKO_TX, 1), 0);
  if (Result) return DECODE_XCHG_ERROR(Result);

  // ждем переключения блока МВКУ
  SleepMSec(100);

  // чтение текущей конфигурации
  Result = Read733Config(DevMkoLine, DevMkoChan, &Config);
  if (Result) return Result;

  // проверяем новую конфигурацию МВКУ
  if ((MVKU_Want == 0x01) && (Config[1] != 0x2040)) return REPLY_733_MVKU_WRONG_CONFIG;
  if ((MVKU_Want == 0x02) && (Config[1] != 0x2080)) return REPLY_733_MVKU_WRONG_CONFIG;
  if ((MVKU_Want == 0x03) && (Config[1] != 0x2010)) return REPLY_733_MVKU_WRONG_CONFIG;
  }

// ****

// переключаем ВЧМ в требуемую конфигурацию

#if (0)
// отключаем все ВЧМ
MkoBufB[0][0] = 0x8000;
Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(ADDR_733, 17, MKO_TX, 1), 0);
if (Result) return DECODE_XCHG_ERROR(Result);

// ждем переключения конфигурации ВЧМ
SleepMSec(100);
#endif

if (VCHM_Want)
  {
  // включаем требуемые каналы ВЧМ
  MkoBufB[0][0] = 0x8000 | (VCHM_Want & 0x0F); 
  Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(ADDR_733, 17, MKO_TX, 1), 0);
  if (Result) return DECODE_XCHG_ERROR(Result);

  // ждем включения каналов ВЧМ
  SleepMSec(100);

  #if (0)
  // запускаем процессоры для включенных каналов ВЧМ
  Index = 0;
  if (VCHM_Want & 0x01) { MkoBufB[0][Index++] = 0x9030; } 
  if (VCHM_Want & 0x02) { MkoBufB[0][Index++] = 0x9230; } 
  if (VCHM_Want & 0x04) { MkoBufB[0][Index++] = 0x9430; } 
  if (VCHM_Want & 0x08) { MkoBufB[0][Index++] = 0x9630; } 
  Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(ADDR_733, 17, MKO_TX, Index), 0);
  if (Result) return DECODE_XCHG_ERROR(Result);

  // ждем запуска процессоров ВЧМ
  SleepMSec(VchmStartupTime);
  #endif
  }

// чтение текущей конфигурации
Result = Read733Config(DevMkoLine, DevMkoChan, &Config);
if (Result) return Result;

// проверка текущей конфигурации ВЧМ
// 8000 8200 8400 8600 = коды отключенных ВЧМ
// 8100 8300 8500 8700 = коды включенных ВЧМ, процессоры не запущены
// 8180 8380 8580 8780 = коды включенных ВЧМ, процессоры запущены

for (Index = 0; Index < 4; Index++)
  {
  if (((VCHM_Want & (1 << Index)) == 0) && ((Config[5 + Index] & 0xFFE0) != (0x8000 | (Index << 9)))) return REPLY_733_WRONG_VM_STATE;
  if (((VCHM_Want & (1 << Index)) != 0) && ((Config[5 + Index] & 0xFFE0) != (0x8100 | (Index << 9)))) return REPLY_733_WRONG_VM_STATE;
  } 

// ****

return REPLY_CMD_DONE;
}
//================================================================================

int Read733Config (int DevMkoLine, int DevMkoChan, void *Config)
{
int Result;
WORD *Cfg;

// чтение текущей конфигурации 733
Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(ADDR_733, 17, MKO_RX, 15), 0);
if (Result) return DECODE_XCHG_ERROR(Result);
SleepMSec(10);

// текущая конфигурация блоков 733
memmove(Config, &MkoBufB[0][0], 15 * sizeof(WORD));

// сброс бит "разрешения записи & чтения"
Cfg = (WORD*) Config;
Cfg[5] &= 0xFF3F;
Cfg[6] &= 0xFF3F;
Cfg[7] &= 0xFF3F;
Cfg[8] &= 0xFF3F;

return 0;
}
//================================================================================

int Read733UpiConfig (int DevMkoLine, int DevMkoChan, void *Config)
{
int Result;

// чтение текущей конфигурации УПИ
Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(ADDR_733, 1, MKO_RX, 4), 0);
if (Result) return DECODE_XCHG_ERROR(Result);
SleepMSec(10);

// текущая конфигурация УПИ
memmove(Config, &MkoBufB[0][0], 4 * sizeof(WORD));
return 0;
}
//================================================================================

#endif

