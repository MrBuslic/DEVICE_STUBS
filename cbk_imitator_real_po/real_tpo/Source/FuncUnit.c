
/*! \file FuncUnit.c
 * \author Гапонов Р.В.
 * \date 2012.10.28
 * \version 1.0.0
 * \brief Модуль содержит функции инициализации СПО, поддержки секундного цикла ПДО
 * и набор функций тестирования и управления аппаратурой БА (выполняются по командам от КПА)
 */

#include "MainUnit.h"
#include "BaloiUnit_v11.h"
//================================================================================

/*! \fn void MainTaskInit (void);
 * \brief Функция производит начальную инициализацию окружения СПО, вызывается однократно при запуске СПО
 * \details функция производит инициализацию переменных СПО, производит установку начальной конфигурации СПО,
 * инициализирует таблицы обработчиков прерываний и команд от КПА, настраивает таблицы данных для 
 * циклического контроля устройств БА, производит поиск КПА на шине МКО, устанавливает исходную маску прерываний от БА,
 * передает сообщение на КПА о запуске СПО, передает данные о результатах тестирования и текущей концигурации ЦБК,
 * передает сообщения о причинах запуска/перезапуска СПО
 */

void MainTaskInit (void)
{
// инициализация СПО
DataInit();
CallBackCount = 0;
ExtTaskCount = 0;

SetDefaultConfig();
HandlerTableInit();
ControlInitTables();
InitPeriodicMkoMsg();
InitProhibitedMkoMsg();
DetectMkoLine();
Irq13InitSys();
Irq13InitData();
InitPacketMkoMsg();
InitMkoStressTest();
Irq10PackInit();

#ifdef SPO_BKOI
BkoiInit();
#endif

// маска прерываний, по умолчанию = только КПА
sprIMaskVec(~SpoConfig.EnabledIrqMask);

// возможно, проверка причины перезапуска СПО
if (1)
  {
  SendStartMsgToKPA();
  SendVmStateToKPA();
  SendMkiDataToKPA();
  SendMdiDataToKPA();
  }

// если СПО запущено после реконфигурации ЦБК
if (PostParam == 0xECF9)
  {
  SendCbkReConfigMsgToKPA();
  }

// возможно, проверка причины перезапуска СПО
if (1)
  {
  SendCbkConfigToKPA();
  }

// разрешение на обработку прерываний
KPADetected = 1;
}
//================================================================================

/*! \fn void MainTaskRun (void);
 * \brief Функция содержит основной цикл ПДО, вызывается после инициализации СПО, управление не возвращает
 * \details Функция производит контроль текущего времени ЦБК и передает на КПА сообщения о секундной метке,
 * контролирует текущую конфигурацию ЦБК и передает на КПА сообщения в случае ее изменений.
 * Если есть установленные на циклический контроль устройства БА, производится их проверка. 
 * Производится контроль прерываний от АСН (если разрешено командой от КПА).
 * По команде от КПА производится вызов дополнительных функций для контроля и управления устройствами БА.
 */

void MainTaskRun (void)
{
int DeltaTime;
timeRead(&MainTaskTime0);

while (1)
  {
  MainTaskPdoCount++;
  timeRead(&MainTaskTime1);

  // контроль прерываний от АСН (если разрешен)
  if ((AsnControlEnabled) && (!AsnWaitForFirstIrq)) AsnControlProc(&MainTaskTime1);

  // контроль прерываний от БЭЧ (если разрешен)
  if ((BehControlEnabled) && (!BehWaitForFirstIrq)) BehControlProc(&MainTaskTime1);

  // фоновый контроль за аппаратурой БАЛОИ
  Irq13IdleTask();

  #ifdef SPO_BKOI
  // функции имитации сообщений для БКОИ
  if (Bkoi_I01_Imit_Enabled) Bkoi_I01_DoImit(&MainTaskTime1);
  if (Bkoi_I12_Imit_Enabled) Bkoi_I12_DoImit(&MainTaskTime1);

  if (Bkoi_I03_Imit_Enabled) Bkoi_I03_DoImit(&MainTaskTime1);
  if (Bkoi_I04_Imit_Enabled) Bkoi_I04_DoImit(&MainTaskTime1);
  if (Bkoi_I05_Imit_Enabled) Bkoi_I05_DoImit(&MainTaskTime1);
  if (Bkoi_I06_Imit_Enabled) Bkoi_I06_DoImit(&MainTaskTime1);

  if (Bkoi_I17_Imit_Enabled) Bkoi_I17_DoImit(&MainTaskTime1);
  if (Bkoi_I18_Imit_Enabled) Bkoi_I18_DoImit(&MainTaskTime1);
  #endif

  // проверка текущей конфигурации ЦБК
  MainTaskCheckCbkConfig();

  DeltaTime = DeltaTimeMSec(&MainTaskTime0, &MainTaskTime1);
  if (DeltaTime >= 1000)
    {
    MainTaskTime0 = MainTaskTime1;
    MSGS("MainTaskRun(): 1000ms Mark"); // , Time = %d.%03dsec, DeltaTime = %d", MainTaskTime1.time_sec, MainTaskTime1.time_msec, DeltaTime);

    // очередной шаг проверки OK
    MainTaskRunCheck();
    // проверка текущей конфигурации ЦБК
    MainTaskCheckCbkConfig();
    // проверка разбега времени с АСН
    if (AsnTaskDeltaTimeFlag) CheckAsnDeltaTime();
 
    // обработка признака "имитация СЧБК"
    if (SpoConfig.ShbkImitEnabled) MainTaskDoShbkImit();
    // обработка признака "имитация квитанций"
    if (SpoConfig.EnabledImitReply) MainTaskDoReplyImit();

    // периодическая передача сообщений по МКО
    DoPeriodicMkoMsg();
    }
  else
    {
    MainTaskIdle();
    }
  }
}
//================================================================================

void CheckAsnDeltaTime (void)
{
int BaseMkoLine, BaseMkoChan;
sysTime_t AsnTime, TaskTime, PrevTime;
UINT DeltaMSec;

// ждем момента для передачи данных
if (--AsnTaskDeltaTimeTimer == 0)
  {
  AsnTaskDeltaTimeTimer = AsnTaskDeltaTimeInterval;

  // параметры линий обмена с КПА
  BaseMkoLine = SpoConfig.KpaMkoLine & 1;
  BaseMkoChan = SpoConfig.KpaMkoChan & 1;

  // ждем стабилизации переменных 
  AsnTime = AsnSecondIrqTime;
  do
    {
    PrevTime = AsnTime;
    AsnTime = AsnSecondIrqTime;
    }
  while ((AsnTime.time_sec != PrevTime.time_sec) || (AsnTime.time_msec != PrevTime.time_msec));

  TaskTime = AsnPeriodicTaskTime;
  do
    {
    PrevTime = TaskTime;
    TaskTime = AsnPeriodicTaskTime;
    }
  while ((TaskTime.time_sec != PrevTime.time_sec) || (TaskTime.time_msec != PrevTime.time_msec));

  // расчет дельты времени 
  // (если АСН раньше - значение = 0xxxxxxx, если задача раньше - значение = 8xxxxxxx)

  if ((AsnTime.time_sec < TaskTime.time_sec) || 
      ((AsnTime.time_sec == TaskTime.time_sec) && (AsnTime.time_msec <= TaskTime.time_msec)))
    {
    // АСН раньше задачи
    DeltaMSec = ((TaskTime.time_sec - AsnTime.time_sec) * 1000) + (TaskTime.time_msec - AsnTime.time_msec);
    DeltaMSec |= 0x00000000;    
    }
  else
    {
    // задача раньше АСН
    DeltaMSec = ((AsnTime.time_sec - TaskTime.time_sec) * 1000) + (AsnTime.time_msec - TaskTime.time_msec);
    DeltaMSec |= 0x80000000;    
    } 

  MkoBufA[0][0] = REPLY_ASN_DELTA_TIME;
  MkoBufA[0][1] = (WORD) DeltaMSec;
  MkoBufA[0][2] = (WORD)(DeltaMSec >> 16);
  MkoXchgAn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_MAIN, MKO_TX, 3), 0);
  }
}
//================================================================================

/*! \fn void AsnControlProc (void);
 * \brief Выполняет проход контроля прерываний от АСН
 * \details Функция выполняет проверку времени прерываний от АСН,
 * производит слежение за временем контроля и отправляет наработанную 
 * статистику на КПА при завершении контроля АСН.
 */

void AsnControlProc (sysTime_t *ACurrTime)
{
int BaseMkoLine, BaseMkoChan;
int Time1, Time2;
WORD Buffer[8];

// проверка на пропущенные прерывания от АСН
Time1 = ACurrTime->time_sec * 1000 + ACurrTime->time_msec;
Time2 = AsnEnabledZone.time_sec * 1000 + AsnEnabledZone.time_msec + AsnPostDeltaTime;
if (Time1 > Time2)
  {
  // не менять порядок операторов!!! AsnEnabledZone обновлять последней!!!
  if (!AsnIrqAlreadySet) AsnMissIrqCount++;
  AsnIrqAlreadySet = 0; 
  AsnEnabledZone.time_sec++;
  }
    
// проверка на завершение контроля АСН
Time2 = AsnControlStopTime.time_sec * 1000 + AsnControlStopTime.time_msec;
if (Time1 > Time2) 
  {
  AsnControlEnabled = 0;

  // отключение прерываний АСН
  SpoConfig.EnabledIrqMask &= (~0x0001);
  sprIMaskVec(~SpoConfig.EnabledIrqMask);

  // параметры линий обмена с КПА
  BaseMkoLine = SpoConfig.KpaMkoLine & 1;
  BaseMkoChan = SpoConfig.KpaMkoChan & 1;

  // выдаем замеренные данные о прерываниях АСН
  Buffer[0] = AsnIrqCount;
  Buffer[1] = AsnExcIrqCount;
  Buffer[2] = AsnMissIrqCount;
  memmove(&Buffer[3], (void*) &AsnStatusBeginTime, 4);
  Buffer[5] = (AsnTimerFailCount > 0xFFFF) ? (0xFFFF) : (AsnTimerFailCount);
  Buffer[6] = (AsnMkoFailCount > 0xFFFF) ? (0xFFFF) : (AsnMkoFailCount);
  Buffer[7] = (AsnParamFailCount > 0xFFFF) ? (0xFFFF) : (AsnParamFailCount);
  memmove(&MkoBufA[0][0], &Buffer, sizeof(WORD) * 8);

  MkoXchgAn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_MAIN + 1, MKO_TX, 8), 0);

  MkoBufA[0][0] = REPLY_ASN_IRQ_MESSAGE;
  MkoBufA[0][1] = sizeof(WORD) * 8;
  MkoBufA[0][2] = CalcCrc16(&Buffer, sizeof(WORD) * 8);
  MkoXchgAn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_MAIN, MKO_TX, 3), 0);
  }
}
//================================================================================

/*! \fn void BehControlProc (void);
 * \brief Выполняет проход контроля прерываний от БЭЧ
 * \details Функция выполняет проверку времени прерываний от БЭЧ,
 * производит слежение за временем контроля и отправляет наработанную 
 * статистику на КПА при завершении контроля БЭЧ.
 */

void BehControlProc (sysTime_t *ACurrTime)
{
int BaseMkoLine, BaseMkoChan;
int Time1, Time2;
WORD Buffer[10];

// проверка на пропущенные прерывания от БЭЧ
Time1 = ACurrTime->time_sec * 1000 + ACurrTime->time_msec;
Time2 = BehEnabledZone.time_sec * 1000 + BehEnabledZone.time_msec + BehPostDeltaTime;
if (Time1 > Time2)
  {
  // не менять порядок операторов!!! BehEnabledZone обновлять последней!!!
  if (!BehIrqAlreadySet) BehMissIrqCount++;
  BehIrqAlreadySet = 0; 
  BehEnabledZone.time_sec++;
  }

// проверка на завершение контроля БЭЧ
Time2 = BehControlStopTime.time_sec * 1000 + BehControlStopTime.time_msec;
if (Time1 > Time2) 
  {
  BehControlEnabled = 0;

  // отключение прерываний БЭЧ
  SpoConfig.EnabledIrqMask &= (~0x0020);
  sprIMaskVec(~SpoConfig.EnabledIrqMask);

  // параметры линий обмена с КПА
  BaseMkoLine = SpoConfig.KpaMkoLine & 1;
  BaseMkoChan = SpoConfig.KpaMkoChan & 1;

  // выдаем замеренные данные о прерываниях БЭЧ
  Buffer[0] = BehIrqCount;
  Buffer[1] = BehExcIrqCount;
  Buffer[2] = BehMissIrqCount;
  Buffer[3] = BehFhvIrqCount;
  Buffer[4] = BehExcFhvIrqCount;
  Buffer[5] = BehMissFhvIrqCount;
  Buffer[6] = BehFinikIrqCount;
  Buffer[7] = BehExcFinikIrqCount;
  Buffer[8] = BehMissFinikIrqCount;
  Buffer[9] = (BehMkoFailCount > 0xFFFF) ? (0xFFFF) : (BehMkoFailCount);
  memmove(&MkoBufA[0][0], &Buffer, sizeof(WORD) * 10);

  MkoXchgAn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_MAIN + 1, MKO_TX, 10), 0);

  MkoBufA[0][0] = REPLY_BEH_IRQ_MESSAGE;
  MkoBufA[0][1] = sizeof(WORD) * 10;
  MkoBufA[0][2] = CalcCrc16(&Buffer, sizeof(WORD) * 10);
  MkoXchgAn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_MAIN, MKO_TX, 3), 0);
  }
}
//================================================================================

/*! \fn void MainTaskRunCheck (void);
 * \brief Запуск проверки OK, находящихся на циклическом контроле
 * \details Функция передает сообщение на КПА о начале секундного цикла СПО,
 * и вызывает проверку устройств, находящихся на циклическом контроле.
 */

void MainTaskRunCheck (void)
{
timeRead(&PassBeginTime);
if (SpoConfig.EnableSecondMark) SendNextPassMsgToKPA();

// проверка устройств
ControlCheckList();
}
//================================================================================

/*! \fn void MainTaskIdle (void);
 * \brief Функция вызова фоновых задач СПО
 * \details Производит вызов фоновых задач в моменты бездействия СПО. В данной версии СПО не используется.
 */

void MainTaskIdle (void)
{
// фоновые процедуры
// ...
}
//================================================================================

/*! \fn void SetDefaultConfig (void);
 * \brief Функция установки начальной конфигурации СПО
 */

void SetDefaultConfig (void)
{
memset((void*) &SpoConfig, 0, sizeof(SpoConfig));
memset((void*) &CbkConfig, 0, sizeof(CbkConfig));

// начальная конфигурация линий обмена с БА
SpoConfig.DevMkoLine = 1;
SpoConfig.DevMkoChan = 0;

// разрешение на передачу квитанций в МБК04 в перывании 2 ЕРТИ
SpoConfig.Mbk04Irq2ReplyMode = 1;

// передача СЧБК на МБК04 в прерывании 2
SpoConfig.SendShbkToMbk04InIrq02 = 1;
SpoConfig.SendShbkTo732InIrq02 = 1;
SpoConfig.Irq02Pups19Enabled = 1;

// передавать все сообщения при обработке прерывания 2
SpoConfig.ErtiIrq2MsgMask = (IRQ2_MSG_DONE | IRQ2_MSG_PROCESS | IRQ2_MSG_ERROR);

// разрешение на контроль одновременных прерываний
SpoConfig.EnableMultiIrqControl = 0;

// разрешение на выдачу секундной метки
SpoConfig.EnableSecondMark = 1;

// статусы режимов имитации по умолчанию
SpoConfig.CbkDataImitStatus = STATE_IDLE;
SpoConfig.ShbkImitStatus    = STATE_IDLE;
SpoConfig.ImitReplyStatus   = STATE_IDLE;

// конфигурация по умолчанию задачи обработки ошибок
SpoConfig.TaskErrorSendInfo = 1; // выдавать сообщения по МКО об ошибках
SpoConfig.TaskErrorInfoSize = 4; // дополнительно передавать 4 слова

// проверка команд вкл/откл аппарутры передатчика
SpoConfig.EnableMBK07CmdCheck = 1;

SpoConfig.CheckLkaCmdSize = 1; // вкл/откл контроля размера команд для ЛКА (4/28/TX) (def = 1)
SpoConfig.CheckBupState = 1; // вкл/откл контроля состояния каналов БУП-Е (def = 1)

// состояние по умолчанию для каналов БУП = неизвестно
SpoConfig.Bup_09_State = -1; 
SpoConfig.Bup_19_State = -1; 
SpoConfig.Bup_20_State = -1; 

// для МБК07 разрешено включение генератора
SpoConfig.EnableMBK07GenOn = 1;

// разрешение приема прерываний КПА
SpoConfig.EnabledIrqMask = KPA_IRQ_MASK;
}
//================================================================================

/*! \fn void HandlerTableInit (void);
 * \brief Функция настройки таблиц обработчиков прерываний и команд от КПА
 * \details Настраивает таблицы вызова обработчиков, для несуществующих команд настраивается 
 * адрес функции-заглушки.
 */

void HandlerTableInit (void)
{
int i;

IrqHandlerTable[0x00] = IrqHandler00; 
IrqHandlerTable[0x01] = IrqHandler01;
IrqHandlerTable[0x02] = IrqHandler02;
IrqHandlerTable[0x03] = IrqHandler03;
IrqHandlerTable[0x04] = IrqHandler04;
IrqHandlerTable[0x05] = IrqHandler05;
IrqHandlerTable[0x06] = IrqHandler06;
IrqHandlerTable[0x07] = IrqHandler07;

IrqHandlerTable[0x08] = IrqHandler08;
IrqHandlerTable[0x09] = IrqHandler09;
IrqHandlerTable[0x0A] = IrqHandler0A;
IrqHandlerTable[0x0B] = IrqHandler0B;
IrqHandlerTable[0x0C] = IrqHandler0C;
IrqHandlerTable[0x0D] = IrqHandler0D;
IrqHandlerTable[0x0E] = IrqHandler0E;
IrqHandlerTable[0x0F] = IrqHandler0F;

for (i = 0; i < 256; i++) CmdHandlerTable[i] = KpaCommandUnknown;

CmdHandlerTable[0x01] = KpaCommand01;
CmdHandlerTable[0x02] = KpaCommand02;
CmdHandlerTable[0x03] = KpaCommand03;
CmdHandlerTable[0x04] = KpaCommand04;
CmdHandlerTable[0x05] = KpaCommand05;
CmdHandlerTable[0x06] = KpaCommand06;
CmdHandlerTable[0x07] = KpaCommand07;
CmdHandlerTable[0x08] = KpaCommand08;

CmdHandlerTable[0x09] = KpaCommand09;

#ifdef SPO_ERTI
CmdHandlerTable[0x0A] = KpaCommand0A_Erti;
#endif
#ifdef SPO_KOI
CmdHandlerTable[0x0A] = KpaCommand0A_Koi;
#endif

CmdHandlerTable[0x0B] = KpaCommand0B;
CmdHandlerTable[0x0C] = KpaCommand0C;
CmdHandlerTable[0x0D] = KpaCommand0D;
CmdHandlerTable[0x0E] = KpaCommand0E;
CmdHandlerTable[0x0F] = KpaCommand0F;
CmdHandlerTable[0x10] = KpaCommand10;

CmdHandlerTable[0x11] = KpaCommand11;
CmdHandlerTable[0x13] = KpaCommand13;
CmdHandlerTable[0x14] = KpaCommand14;
CmdHandlerTable[0x15] = KpaCommand15;
CmdHandlerTable[0x16] = KpaCommand16;
CmdHandlerTable[0x17] = KpaCommand17;
CmdHandlerTable[0x18] = KpaCommand18;
CmdHandlerTable[0x19] = KpaCommand19;

CmdHandlerTable[0x20] = KpaCommand20;
CmdHandlerTable[0x21] = KpaCommand21;
CmdHandlerTable[0x22] = KpaCommand22;
CmdHandlerTable[0x23] = KpaCommand23;
CmdHandlerTable[0x24] = KpaCommand24;
CmdHandlerTable[0x25] = KpaCommand25;
CmdHandlerTable[0x26] = KpaCommand26;
CmdHandlerTable[0x27] = KpaCommand27;

CmdHandlerTable[0x28] = KpaCommand28;
CmdHandlerTable[0x29] = KpaCommand29;
CmdHandlerTable[0x2A] = KpaCommand2A;
CmdHandlerTable[0x2B] = KpaCommand2B;
CmdHandlerTable[0x2C] = KpaCommand2C;
CmdHandlerTable[0x2D] = KpaCommand2D;
CmdHandlerTable[0x2E] = KpaCommand2E;
CmdHandlerTable[0x2F] = KpaCommand2F;

CmdHandlerTable[0x30] = KpaCommand30;
CmdHandlerTable[0x31] = KpaCommand31;
CmdHandlerTable[0x32] = KpaCommand32;
CmdHandlerTable[0x33] = KpaCommand33;
CmdHandlerTable[0x34] = KpaCommand34;
CmdHandlerTable[0x35] = KpaCommand35;
CmdHandlerTable[0x36] = KpaCommand36;
CmdHandlerTable[0x37] = KpaCommand37;

CmdHandlerTable[0x38] = KpaCommand38;
CmdHandlerTable[0x39] = KpaCommand39;
CmdHandlerTable[0x3A] = KpaCommand3A;
CmdHandlerTable[0x40] = KpaCommand40;
CmdHandlerTable[0x41] = KpaCommand41;

CmdHandlerTable[0x60] = KpaCommand60;
CmdHandlerTable[0x61] = KpaCommand61;
CmdHandlerTable[0x62] = KpaCommand62;
CmdHandlerTable[0x63] = KpaCommand63;
CmdHandlerTable[0x64] = KpaCommand64;
CmdHandlerTable[0x65] = KpaCommand65;

#ifdef SPO_BKOI
CmdHandlerTable[0x66] = KpaCommand66; 
#endif

CmdHandlerTable[0xFB] = KpaCommandFB;
CmdHandlerTable[0xFC] = KpaCommandFC;
CmdHandlerTable[0xFD] = KpaCommandFD;
CmdHandlerTable[0xFE] = KpaCommandFE;
CmdHandlerTable[0xFF] = KpaCommandFF;
}
//================================================================================

/*! \fn void DetectMkoLine (void);
 * \brief Функция поиска расположения КПА на шине МКО, вызывается однократно при запуске СПО
 * \details Производит циклический перебор линий и каналов МКО для указанного адреса КПА,
 * если КПА успешно ответит на запрос на некоторой линии/канале МКО более 5 раз, данные 
 * о расположении КПА будут сохранены в конфигурации и СПО продолжит работу.
 */

void DetectMkoLine (void)
{
int Result;
int MkoCmd, MkoIndex, MkoPass, MkoLine, MkoChan;

// команда обмена с КПА
MkoCmd = MKO_CMD(KPA_ADDR, KPA_SUBADDR_CMD, MKO_RX, KPA_CMD_LEN);  // первое командное слово (считать команду КПА)

MkoPass = 0; // число успешных попыток соединения
MkoIndex = 0; // начальное состояние: линия 0, канал 0 (основной)

while (1)
  {
  // линия/канал МКО для обмена
  MkoChan = (MkoIndex >> 0) & 1;
  MkoLine = (MkoIndex >> 1) & 1;

  // проверка наличия абонента
  Result = MkoXchgAn(MkoLine, MkoChan, MkoCmd, 0);
  if (Result)
    {
    MkoPass = 0;
    MkoIndex++;
    taskDelay(MKO_DETECT_DELAY);
    continue;
    }

  // выход, если линк стабилен (не менее 5 успешных попыток чтения подряд)
  if (++MkoPass >= MKO_DETECT_TRY) break;
  taskDelay(MKO_DETECT_DELAY);
  }

// сохраняем расположение КПА на шине
SpoConfig.KpaMkoLine = MkoLine;
SpoConfig.KpaMkoChan = MkoChan;
}
//================================================================================

/*! \fn void MainTaskGetMkiMdiData (void);
 * \brief Функция производит считывание массивов МКИ из ОПО при старте СПО
 * \details Данные о результатах самотестирования ЦБК сохраняются в буфере СПО и позже
 * передаются на КПА. 
 */

void MainTaskGetMkiData (void)
{
STATUS Status;

// получение массива МКИ
PostMkiDataSize = sizeof(PostMkiDataBuf);
Status = testErrGet((UINT*) &PostMkiDataBuf);
if (Status == ERROR) 
  {
  PostMkiDataSize = 1;
  memset(&PostMkiDataBuf, 0, sizeof(PostMkiDataBuf));
  }
}
//================================================================================

void MainTaskGetMdiData (void)
{
int Result;

// получение массива МДИ
PostMdiDataSize = sizeof(PostMdiDataBuf);
Result = testMDIGet((UINT*) &PostMdiDataBuf, (PostMdiDataSize / sizeof(UINT)) | 0xC0000000);

MSGS("MainTaskGetMdiData(): Размер данных МДИ = %d", Result * sizeof(UINT));

if (Result == ERROR)
  {
  PostMdiDataSize = 1;
  memset(&PostMdiDataBuf, 0, sizeof(PostMdiDataBuf));
  }
else
  {
  if (Result == (PostMdiDataSize / sizeof(UINT)))
    {
    MSGS("\nMainTaskGetMdiData(): Буфер под данные МДИ слишком мал\n");
    }
  PostMdiDataSize = Result * sizeof(UINT);
  }
}
//================================================================================

/*! \fn int DeltaTimeMSec (sysTime_t *BeginTime, sysTime_t *EndTime);
 * \brief Функция производит расчет интервала между двумя точками времени
 * \param [in] BeginTime - начальная точка времени
 * \param [in] EndTime - конечная точка времени
 * \return интервал времени, мсек, между указанными точками
 */

int DeltaTimeMSec (sysTime_t *BeginTime, sysTime_t *EndTime)
{
int DeltaTime;
DeltaTime = (EndTime->time_sec - BeginTime->time_sec) * 1000;
DeltaTime += (EndTime->time_msec - BeginTime->time_msec);
return DeltaTime;
}
//================================================================================

/*! \fn void SleepMSec (int ATime);
 * \brief Функция делает паузу в работе СПО
 * \param [in] ATime - пауза в работе СПО, мсек
 * \details Производится пауза в работе задачи, алгоритм задержки "не менее чем", т.е.
 * для SleepMSec(1) реальная задержка будет от 1 до 2 мсек
 */

void SleepMSec (int ATime)
{
sysTime_t BeginTime, CurrTime;
int DeltaTime;

timeRead(&BeginTime);

while (1)
  {
  timeRead(&CurrTime);
  DeltaTime = DeltaTimeMSec(&BeginTime, &CurrTime);
  if (DeltaTime > ATime) break;
  }
}
//================================================================================

/*! \fn void SendStartMsgToKPA (void);
 * \brief Функция передает сообщение о старте СПО на КПА
 */

void SendStartMsgToKPA (void)
{
int BaseMkoLine, BaseMkoChan;

MSGS("SendStartMsgToKPA()");

// параметры МКО для обмена с КПА
BaseMkoLine = SpoConfig.KpaMkoLine & 1;
BaseMkoChan = SpoConfig.KpaMkoChan & 1;

MkoBufA[0][0] = REPLY_SPO_START;
MkoXchgAn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_MAIN, MKO_TX, 1), 0);
}
//================================================================================

/*! \fn void SendVmStateToKPA (void);
 * \brief Функция отправляет сообщение о текущей конфигурации ЦБК на КПА
 */

void SendVmStateToKPA (void)
{
int BaseMkoLine, BaseMkoChan;
STATUS State;

MSGS("SendVmStateToKPA()");

// параметры МКО для обмена с КПА
BaseMkoLine = SpoConfig.KpaMkoLine & 1;
BaseMkoChan = SpoConfig.KpaMkoChan & 1;

// запрос карты ошибок
State = testErrGet((UINT*) &VmStateData);
if (State != OK)
  {
  MkoBufA[0][0] = REPLY_CBK_POST_RESULT | REPLY_FLAG_ERROR;
  MkoXchgAn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_MAIN, MKO_TX, 1), 0);
  return;
  }

VmStateData[1] = VmStateData[6];
VmStateData[2] = VmStateData[12];
VmStateData[3] = VmStateData[18];
memmove(&MkoBufA[0][0], &VmStateData, 4 * sizeof(UINT));
MkoXchgAn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_MAIN + 1, MKO_TX, 8), 0);

MkoBufA[0][0] = REPLY_CBK_POST_RESULT;
MkoBufA[0][1] = 4 * sizeof(UINT);
MkoBufA[0][2] = CalcCrc16(&VmStateData, 4 * sizeof(UINT));
MkoXchgAn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_MAIN, MKO_TX, 3), 0);
}
//================================================================================

/*! \fn void SendMkiDataToKPA (void);
 * \brief Функция отправляет на КПА результаты тестов БТПО (массивы МКИ)
 * \details Данные передаются одним или несколькими сообщениями МКО, если их размер
 * превысит размер одного сообщения.
 */

void SendMkiDataToKPA (void)
{
int BaseMkoLine, BaseMkoChan;
int TotalSize, RemainSize, DataSize;
WORD *DataBuffer, *DataSrc;

MSGS("SendMkiDataToKPA()");

BaseMkoLine = SpoConfig.KpaMkoLine & 1;
BaseMkoChan = SpoConfig.KpaMkoChan & 1;

if (1)
  {
  // источник/размер данных
  DataBuffer = (WORD*) &PostMkiDataBuf;
  TotalSize = PostMkiDataSize;

  // выравнивание размера на 16-бит слово, выравнивание вверх
  RemainSize = WORD_ALIGN(TotalSize) / 2;
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
  
    MkoXchgAn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_MAIN + 1, MKO_TX, DataSize), 0);
    }

  // отправка сообщения о выполнении команды + CRC + размер данных в байтах
  MkoBufA[0][0] = REPLY_CMD_DONE;
  MkoBufA[0][1] = TotalSize;
  MkoBufA[0][2] = CalcCrc16(DataBuffer, TotalSize);
  MkoXchgAn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_MAIN, MKO_TX, 3), 0);
  }
}
//================================================================================

/*! \fn void SendMdiDataToKPA (void);
 * \brief Функция отправляет на КПА результаты тестов БТПО (массивы МДИ)
 * \details Данные передаются одним или несколькими сообщениями МКО, если их размер
 * превысит размер одного сообщения.
 */

void SendMdiDataToKPA (void)
{
int BaseMkoLine, BaseMkoChan;
int TotalSize, RemainSize, DataSize;
WORD *DataBuffer, *DataSrc;

MSGS("SendMdiDataToKPA()");

BaseMkoLine = SpoConfig.KpaMkoLine & 1;
BaseMkoChan = SpoConfig.KpaMkoChan & 1;

if (1)
  {
  // источник/размер данных
  DataBuffer = (WORD*) &PostMdiDataBuf;
  TotalSize = PostMdiDataSize;

  // выравнивание размера на 16-бит слово, выравнивание вверх
  RemainSize = WORD_ALIGN(TotalSize) / 2;
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
  
    MkoXchgAn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_MAIN + 1, MKO_TX, DataSize), 0);
    }

  // отправка сообщения о выполнении команды + CRC + размер данных в байтах
  MkoBufA[0][0] = REPLY_MDI_DATA;
  MkoBufA[0][1] = TotalSize;
  MkoBufA[0][2] = CalcCrc16(DataBuffer, TotalSize);
  MkoXchgAn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_MAIN, MKO_TX, 3), 0);
  }
}
//================================================================================

/*! \fn void SendNextPassMsgToKPA (void);
 * \brief Функция передает на КПА сообщение о начале очередного цикла ПДО (секундная метка)
 */

void SendNextPassMsgToKPA (void)
{
int BaseMkoLine, BaseMkoChan;

MSGS("SendNextPassMsgToKPA()");

// параметры МКО для обмена с КПА
BaseMkoLine = SpoConfig.KpaMkoLine & 1;
BaseMkoChan = SpoConfig.KpaMkoChan & 1;

MkoBufA[0][0] = REPLY_PDO_PASS;
MkoBufA[0][1] = CallBackCount;
MkoBufA[0][2] = ExtTaskCount;
MkoXchgAn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_MAIN, MKO_TX, 3), 0);
}
//================================================================================

/*! \fn void SendCbkReConfigMsgToKPA (void);
 * \brief Функция передает на КПА сообщение о запуске СПО после процедуры реконфигурации ЦБК
 */

void SendCbkReConfigMsgToKPA (void)
{
int BaseMkoLine, BaseMkoChan;

MSGS("SendCbkReConfigMsgToKPA()");

// параметры МКО для обмена с КПА
BaseMkoLine = SpoConfig.KpaMkoLine & 1;
BaseMkoChan = SpoConfig.KpaMkoChan & 1;

MkoBufA[0][0] = REPLY_START_AFTER_CBK_RECONFIG;
MkoXchgAn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_MAIN, MKO_TX, 1), 0);
}
//================================================================================

/*! \fn void SendCbkConfigToKPA (void);
 * \brief Функция передает на КПА текущую конфигурацию ЦБК
 * \details Считывает и передает на КПА данные по текущей конфигурации ЦБК. Также конфигурация сохраняются
 * в данных СПО, для отслеживания изменений в конфигурации ЦБК.
 */

void SendCbkConfigToKPA (void)
{
int BaseMkoLine, BaseMkoChan;
STATUS Status;
cfgSYS_t Config;

MSGS("SendCbkConfigToKPA()");

// параметры МКО для обмена с КПА
BaseMkoLine = SpoConfig.KpaMkoLine & 1;
BaseMkoChan = SpoConfig.KpaMkoChan & 1;

Status = cfgSysGet(&Config);
if (Status != OK)
  {
  // ошибка получения конфигурации ЦБК
  MkoBufA[0][0] = REPLY_CBK_CONFIG | REPLY_FLAG_ERROR;
  MkoXchgAn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_MAIN, MKO_TX, 1), 0);
  return;
  }

// исходная конфигурация ЦБК
memmove(&CbkConfig, &Config, sizeof(Config));

// отсылаем конфигурацию на КПА
memmove(&MkoBufA[0][0], &Config, sizeof(Config));
MkoXchgAn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_MAIN + 1, MKO_TX, 2), 0);

MkoBufA[0][0] = REPLY_CBK_CONFIG;
MkoBufA[0][1] = sizeof(Config);
MkoBufA[0][2] = CalcCrc16(&Config, sizeof(Config));
MkoXchgAn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_MAIN, MKO_TX, 3), 0);
}
//================================================================================

/*! \fn void MainTaskCheckCbkConfig (void);
 * \brief Функция проверяет конфигурацию ЦБК и отправляет на КПА сообщение в случае ее изменений
 * \details Производит считывание текущей конфигурации и сравнение ее с ранее сохраненным эталоном.
 * При изменениях на КПА выдается сообщение с новой конфигурацией и сохранение ее как новый эталон.
 * Операции обменов по МКО при сравнении конфигураций не учитываются.
 */

void MainTaskCheckCbkConfig (void)
{
int BaseMkoLine, BaseMkoChan;
STATUS Status;
cfgSYS_t CurrConfig;
UINT CfgOld, CfgNew;

MSGS("MainTaskCheckCbkConfig()");

// параметры МКО для обмена с КПА
BaseMkoLine = SpoConfig.KpaMkoLine & 1;
BaseMkoChan = SpoConfig.KpaMkoChan & 1;

Status = cfgSysGet(&CurrConfig);
if (Status != OK)
  {
  // ошибка получения конфигурации ЦБК
  MkoBufA[0][0] = REPLY_CBK_CONFIG | REPLY_FLAG_ERROR;
  MkoXchgAn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_MAIN, MKO_TX, 1), 0);
  return;
  }

// копируем конфигурации в локальные переменные
memmove(&CfgOld, &CbkConfig, sizeof(CbkConfig));
memmove(&CfgNew, &CurrConfig, sizeof(CurrConfig));

// убираем флаги текущих обменов на МКО
CfgOld &= 0xFF77FFFF;
CfgNew &= 0xFF77FFFF;

if (CfgOld != CfgNew)
  {
  CbkConfig = CurrConfig;

  memmove(&MkoBufA[0][0], &CbkConfig, sizeof(CbkConfig));
  MkoXchgAn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_MAIN + 1, MKO_TX, 2), 0);

  MkoBufA[0][0] = REPLY_CBK_CONFIG;
  MkoBufA[0][1] = sizeof(CbkConfig);
  MkoBufA[0][2] = CalcCrc16(&CbkConfig, sizeof(CbkConfig));
  MkoXchgAn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_MAIN, MKO_TX, 3), 0);
  }
}
//================================================================================

/*! \fn void MainTaskDoSHBKImit (void);
 * \brief Функция производит имитацию данных СЧБК
 * \details Если от КПА поступила команда на начало имитации, в каждом секундном цикле производится 
 * считывание с КПА маски СЧБК, формирование данных имитации и передача данных на МБК04.
 */

void MainTaskDoShbkImit (void)
{
int Result;
int BaseMkoLine, BaseMkoChan;
int DevMkoLine, DevMkoChan;
BYTE ShbkBuffer[32];

MSGS("MainTaskDoSHBKImit()");

// параметры линий обмена с КПА
BaseMkoLine = SpoConfig.KpaMkoLine & 1;
BaseMkoChan = SpoConfig.KpaMkoChan & 1;

// параметры линий обмена с БА
DevMkoLine = SpoConfig.DevMkoLine & 1;
DevMkoChan = SpoConfig.DevMkoChan & 1;

// считывание маски СЧБК
if ((!SpoConfig.ShbkImitMode) || ((SpoConfig.ShbkImitMode) && (!SpoConfig.ShbkImitMaskReady)))
  {
  Result = MkoXchgAn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_SHBK, MKO_RX, 2), 0);
  if (Result)
    {
    SpoConfig.ShbkImitEnabled = 0; // отключаем имитацию при ошибках
    SpoConfig.ShbkImitStatus = STATE_ABORT; // смена текущего статуса режима

    MkoBufA[0][0] = REPLY_SHBK_WR_ERROR;
    MkoXchgAn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_MAIN, MKO_TX, 1), 0);
    return;
    }
  memmove(&SpoConfig.ShbkImitMask, &MkoBufA[0], 4);
  SpoConfig.ShbkImitMaskReady = 1;
  }

// формирование буфера данных СЧБК
MakeShbkBuffer(SpoConfig.ShbkImitMask, (BYTE*) &ShbkBuffer);

// передача буфера данных СЧБК в МБК4
memmove(&MkoBufA[0][0], &ShbkBuffer, sizeof(ShbkBuffer));
Result = MkoXchgAn(DevMkoLine, DevMkoChan, MKO_CMD(4, 6, MKO_TX, 16), 0);
if (Result)
  {
  if ((Result == -3) && (SpoConfig.ShbkFailTimer))
    {
    SpoConfig.ShbkFailCount++;
    if (SpoConfig.ShbkFailCount < 15) return;
    }

  SpoConfig.ShbkImitEnabled = 0; // отключаем имитацию при ошибках
  SpoConfig.ShbkImitStatus = STATE_ABORT; // смена текущего статуса режима

  MkoBufA[0][0] = REPLY_SHBK_WR_ERROR;
  MkoXchgAn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_MAIN, MKO_TX, 1), 0);
  return;
  }

// обнуляем счетчик ошибок при успешном обмене
SpoConfig.ShbkFailCount = 0;

// меняем число в режиме имитации 2
if (SpoConfig.ShbkImitMode) SpoConfig.ShbkImitMask++;
}
//================================================================================

/*! \fn void MakeShbkBuffer (UINT ShbkMask, BYTE *ShbkBuffer);
 * \brief Функция создает буфер с данными по указанной маске СЧБК
 */

void MakeShbkBuffer (UINT ShbkMask, BYTE *ShbkBuffer)
{
int Index;
UINT BitMask;
BYTE Data;

// формирование буфера данных СЧБК
for (Index = 0, BitMask = 0x80000000; Index < 32; Index++, BitMask >>= 1)
  {
  ShbkBuffer[Index] = ((ShbkMask & BitMask) ? 0xF0 : 0x0F);
  }
// меняем байты в буфере местами
for (Index = 0; Index < 32; Index += 2)
  {
  Data = ShbkBuffer[Index];
  ShbkBuffer[Index] = ShbkBuffer[Index + 1];
  ShbkBuffer[Index + 1] = Data;
  }
}
//================================================================================

/*! \fn void MainTaskDoReplyImit (void);
 * \brief Функция производит имитацию квитанций
 * \details Если от КПА поступила команда на начало имитации, в каждом секундном цикле производится 
 * считывание с КПА данных квитанции, формирование данных имитации и передача данных на МБК04.
 */

void MainTaskDoReplyImit (void)
{
int Result, Reply, DevSubAddr;
int BaseMkoLine, BaseMkoChan;
int DevMkoLine, DevMkoChan;
WORD Buffer[2];

MSGS("MainTaskDoReplyImit()");

// параметры линий обмена с КПА
BaseMkoLine = SpoConfig.KpaMkoLine & 1;
BaseMkoChan = SpoConfig.KpaMkoChan & 1;

// параметры линий обмена с БА
DevMkoLine = SpoConfig.DevMkoLine & 1;
DevMkoChan = SpoConfig.DevMkoChan & 1;

// считывание маски квитанций
Result = MkoXchgAn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_REPLY_IMIT, MKO_RX, 1), 0);
if (Result)
  {
  // отключаем имитацию при ошибках
  SpoConfig.EnabledImitReply = 0;

  // смена текущего статуса режима
  SpoConfig.ImitReplyStatus = STATE_ABORT;

  // при сбоях на подготовке данных передаем код ошибки + 0 (как данные об ошибке)
  Buffer[0] = 0;

  memmove(&MkoBufA[0][0], &Buffer[0], 2);
  MkoXchgAn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_MAIN + 1, MKO_TX, 1), 0);

  MkoBufA[0][0] = REPLY_WR_ERROR_MBK04;
  MkoBufA[0][1] = 2;
  MkoBufA[0][2] = CalcCrc16(&Buffer[0], 2);
  MkoXchgAn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_MAIN, MKO_TX, 3), 0);
  return;
  }

// код выдаваемой квитанции
Reply = (MkoBufA[0][0] >> (SpoConfig.ImitReplyIndex * 2)) & 0x03;

// квитанция "ДА СЧЗК"  = "11b" = 0x03
// квитанция "НЕТ СЧЗК" = "10b" = 0x02
// квитанция "ДА КПИ"   = "01b" = 0x01
// квитанция "НЕТ КПИ"  = "00b" = 0x00

MkoBufA[0][0] = (Reply & 0x01) ? (0x00F0) : (0x000F);
DevSubAddr = (Reply & 0x02) ? (5) : (7);

// передача слова данных квитанции в МБК4
Result = MkoXchgAn(DevMkoLine, DevMkoChan, MKO_CMD(4, DevSubAddr, MKO_TX, 1), 0);
if (Result)
  {
  if ((Result == -3) && (SpoConfig.ReplyFailTimer))
    {
    SpoConfig.ReplyFailCount++;
    if (SpoConfig.ReplyFailCount < 3) return;
    }

  // отключаем имитацию при ошибках
  SpoConfig.EnabledImitReply = 0;

  // смена текущего статуса режима
  SpoConfig.ImitReplyStatus = STATE_ABORT;

  // при сбоях на передаче квитанций передаем код ошибки + квитанцию (как данные об ошибке)
  Buffer[0] = MkoBufA[0][0];

  memmove(&MkoBufA[0][0], &Buffer[0], 2);
  MkoXchgAn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_MAIN + 1, MKO_TX, 1), 0);

  MkoBufA[0][0] = REPLY_WR_ERROR_MBK04;
  MkoBufA[0][1] = 2;
  MkoBufA[0][2] = CalcCrc16(&Buffer[0], 2);
  MkoXchgAn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_MAIN, MKO_TX, 3), 0);
  return;
  }

// переход на следующую квитанцию при успешном обмене
SpoConfig.ImitReplyIndex = (SpoConfig.ImitReplyIndex - 1) & 0x07;

// обнуляем счетчик ошибок при успешном обмене
SpoConfig.ReplyFailCount = 0;
}
//================================================================================

/*! \fn int BitCount (int Value);
 * \brief Функция возвращает число установленых бит
 * \param [in] Value - параметр для расчета
 * \return Функция возвращает число установленых бит
 */

int BitCount (int Value)
{
int Count, Index;

for (Index = 0, Count = 0; Index < 32; Index++)
  {
  if (Value & 0x01) Count++;
  Value >>= 1;
  }

return Count;
}
//================================================================================

