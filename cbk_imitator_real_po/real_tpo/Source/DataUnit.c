
/*! \file DataUnit.c
 * \author Гапонов Р.В.
 * \date 2012.10.28
 * \version 1.0.0
 * \brief Модуль содержит глобальные переменные СПО и функции инициализации данных
 */

#include "DataUnit.h"
//================================================================================

int PostParam;					// параметр, переданный стартовой задаче из ОПО
char OpoVersionString[64];			// строка с номером версии ОПО
DWORD FlashSectorCSumTable[16];			// таблица контрольных сумм секторов флеша (8 по CRC32, 8 по CSumLong)
char KPADetected;				// флаг обнаружения КПА на шине МКО (1 = обнаружен)

UINT PostMkiDataBuf[6 * 4];			// буферы под результаты самотестирования ЦБК
UINT PostMkiDataSize;
UINT PostMdiDataBuf[(32 * 4) + (256 * 4) + 256];
UINT PostMdiDataSize;

char LastError[256];				// строка описания последней ошибки
TSpoConfig SpoConfig;				// конфигурация СПО
sysTime_t IrqTime;				// время прихода прерывания (запуска задачи обработки прерываний)
sysTime_t PassBeginTime;			// время начала очередного цикла проверки OK
KPA_CMD_HANDLER CmdHandlerTable[256];		// таблица обработчиков команд от КПА
IRQ_HANDLER IrqHandlerTable[16];		// таблица обработчиков прерываний от БА
WORD KpaCommand[KPA_CMD_LEN];			// командный пакет от КПА
UINT MkiBuffer[512];				// буфер под данные ОПО (заглушка)
cfgSYS_t CbkConfig;				// текущая конфигурация ЦБК
UINT VmStateData[24];				// карта ошибок ЦБК (запрашивается при старте СПО)
sysTime_t MainTaskTime0, MainTaskTime1;		// время в основном цикле

WORD SubsAddrBuffer[32];			// буфер под адреса (при блочном чтении абонентов на МКО)
WORD SubsDataBuffer[32 * 32];			// буфер под данные (при блочном чтении абонентов на МКО)

bppJob_t FlashJob;				// описатель задания на коррекцию флеша
BYTE FlashDataBuffer[256 * 1024];		// буфер под записываемые во флеш данные (образ сектора)
unsigned FlashDataIndex;			// размер данныз в образе сектора
unsigned FlashDataAddr;				// адрес для записи данных  во флеше

// пакет A - используется для обменов в основном потоке СПО
mkoPack_t MkoPackA;				// описатель пакета МКО
mkoPackRes_t MkoPackResA;                       // описатель результата для пакета
mkoPackMsg_t MkoMsgA[33];                       // описатели сообщений для пакета МКО
mkoMsgRes_t MkoMsgResA[33];			// описатели результата для сообщений МКО
mkoPackMsg_t* MkoMsgPtrA[33];                   // набор указателей на описатели сообщений МКО
MKO_BUFFER MkoBufA[33];                         // набор буферов под данные сообщений МКО
//================================================================================

// пакет B - используется для обменов в функциях прерываний СПО
mkoPack_t MkoPackB;                             // описатель пакета МКО
mkoPackRes_t MkoPackResB;                       // описатель результата для пакета
mkoPackMsg_t MkoMsgB[33];			// описатели сообщений для пакета МКО
mkoMsgRes_t MkoMsgResB[33];                     // описатели результата для сообщений МКО
mkoPackMsg_t* MkoMsgPtrB[33];			// набор указателей на описатели сообщений МКО
MKO_BUFFER MkoBufB[33];                         // набор буферов под данные сообщений МКО
//================================================================================

// пакет C - используется для обменов в функциях прерываний СПО (в режиме DoubleBuffer совместно с накетом B)
mkoPack_t MkoPackC;				// описатель пакета МКО
mkoPackRes_t MkoPackResC;			// описатель результата для пакета
mkoPackMsg_t MkoMsgC[33];                       // описатели сообщений для пакета МКО
mkoMsgRes_t MkoMsgResC[33];			// описатели результата для сообщений МКО
mkoPackMsg_t* MkoMsgPtrC[33];			// набор указателей на описатели сообщений МКО
MKO_BUFFER MkoBufC[33];				// набор буферов под данные сообщений МКО
//================================================================================

// пакет X - используется для передачи инфо об ощибках СПО
mkoPack_t MkoPackX;				// описатель пакета МКО
mkoPackRes_t MkoPackResX;                       // описатель результата для пакета
mkoPackMsg_t MkoMsgX;                           // описатели сообщений для пакета МКО
mkoMsgRes_t MkoMsgResX;				// описатели результата для сообщений МКО
mkoPackMsg_t* MkoMsgPtrX[1];			// набор указателей на описатели сообщений МКО
MKO_BUFFER MkoBufX[1];				// набор буферов под данные сообщений МКО
//================================================================================

// отладка для обменов
int DebugFlag;
mkoPackRes_t DebugPackRes;

// пакет Stub - используется в качестке заглушки
MKO_BUFFER MkoBufStub;

// буфер под расширенные данные ошибок на МКО
DWORD DebugMki[1024];

// набор переменных для процедур тестирования АСН
volatile int AsnIrqCount, AsnExcIrqCount, AsnMissIrqCount;
volatile int AsnStatusBeginTime, AsnStatusPrevTime, AsnTimerFailCount, AsnMkoFailCount, AsnParamFailCount;
volatile sysTime_t AsnControlStopTime, AsnEnabledZone;
volatile int AsnStartTimer, AsnWaitForFirstIrq, AsnIrqAlreadySet;
volatile int AsnPreDeltaTime, AsnPostDeltaTime;
volatile int AsnControlEnabled;

UINT AsnTaskDeltaTimeId;
volatile int AsnTaskDeltaTimeInterval;
volatile int AsnTaskDeltaTimeTimer;
volatile int AsnTaskDeltaTimeFlag;
volatile sysTime_t AsnSecondIrqTime;
volatile sysTime_t AsnPeriodicTaskTime;

volatile WORD AsnTimeList[ASN_TIME_LIST_SIZE];
volatile int AsnTimeListSize;
volatile sysTime_t AsnTimeIrqPrev;

// набор переменных для процедур тестирования 732
volatile BYTE B732PupsList[B732_PUPS_LIST_SIZE];
volatile int B732PupsListSize;

// набор переменных для процедур тестирования БЭЧ
volatile int BehIrqCount, BehExcIrqCount, BehMissIrqCount;
volatile int BehFhvIrqCount, BehExcFhvIrqCount, BehMissFhvIrqCount;
volatile int BehFinikIrqCount, BehExcFinikIrqCount, BehMissFinikIrqCount;
volatile int BehFhvFlag, BehFinikFlag;
volatile int BehFhvTmpCount, BehFinikTmpCount;
volatile int BehMkoFailCount;
volatile sysTime_t BehControlStopTime, BehEnabledZone;
volatile int BehStartTimer, BehWaitForFirstIrq, BehIrqAlreadySet;
volatile int BehPreDeltaTime, BehPostDeltaTime;
volatile int BehFhvPeriod, BehFinikPeriod;
volatile int BehControlEnabled;

// специальные переменные для отслеживания задач СПО
volatile UINT32 MainTaskIn;        // переменная увеличивается на 1 при входе в задачу MainTask СПО, инициализация не производится

volatile UINT32 ErrorTaskIn;       // переменная увеличивается на 1 при входе в задачу ErrorTask СПО, инициализация 0 при старте СПО
volatile UINT32 ErrorTaskOut;      // переменная увеличивается на 1 при выходе из задачи ErrorTask СПО, инициализация 0 при старте СПО

volatile UINT32 IrqTaskIn;         // переменная увеличивается на 1 при входе в задачу IrqTask СПО, инициализация 0 при старте СПО
volatile UINT32 IrqTaskOut;        // переменная увеличивается на 1 при выходе из задачи IrqTask СПО, инициализация 0 при старте СПО

volatile UINT32 TestTaskIn;        // переменная увеличивается на 1 при входе в задачу TestTask СПО, инициализация 0 при старте СПО
volatile UINT32 TestTaskOut;       // переменная увеличивается на 1 при выходе из задачи TestTask СПО, инициализация 0 при старте СПО

volatile UINT32 AsnTaskIn;         // переменная увеличивается на 1 при входе в задачу AsnTask СПО, инициализация 0 при старте СПО
volatile UINT32 AsnTaskOut;        // переменная увеличивается на 1 при выходе из задачи AsnTask СПО, инициализация 0 при старте СПО

volatile UINT32 MainTaskPdoCount;  // переменная увеличивается на 1 при каждом проходе цикла ПДО в задаче MainTask СПО, инициализация 0 при старте СПО
volatile UINT32 MainTaskCmdFail;   // переменная увеличивается на 1 при имитации сбоев СПО, инициализация 0 при старте СПО
//================================================================================

// массив для реконфигурации ЦБК
dataArr_t ReConfigBuffer[2];
//================================================================================

// буфер данных для имитации массива ЦБК
MKO_BUFFER CbkDataImit[33];
//================================================================================

// буфер под данные МДИ
UINT MdiBuffer[4][32];
//================================================================================

unsigned CallBackCount, ExtTaskCount, CbkErrCtrlCallBackType;
//================================================================================

/*! \fn void DataInit (void);
 * \brief Функция производит настройку переменных СПО и таблиц для поддержки обменов по МКО.
 * \details Вызывается однократно при старте СПО.
 */

void DataInit (void)
{
int Index;

// сброс флага обнаружения КПА
KPADetected = 0;

// сброс инфо об ошибках
memset(&LastError, 0, sizeof(LastError));

// [настройка пакета A для обмена по МКО]
// инициализация описателя пакета МКО
MkoPackA.nFirst = 0;  // номер первого сообщения
MkoPackA.nLast  = 0;  // номер последнего сообщения
MkoPackA.nPage  = 0;  // номер страницы БОЗУ
MkoPackA.DescPackRes = (mkoPackRes_t*) &MkoPackResA;  // указатель на описатель результата
// инициализация описателей сообщений
for (Index = 0; Index <= 32; Index++) InitMkoMsgA(&MkoMsgA[Index], Index, &MkoBufA[Index], &MkoMsgResA[Index]);

// [настройка пакета B для обмена по МКО]
// инициализация описателя пакета МКО
MkoPackB.nFirst = 0;  // номер первого сообщения
MkoPackB.nLast  = 0;  // номер последнего сообщения
MkoPackB.nPage  = 1;  // номер страницы БОЗУ
MkoPackB.DescPackRes = (mkoPackRes_t*) &MkoPackResB;  // указатель на описатель результата
// инициализация описателей сообщений
for (Index = 0; Index <= 32; Index++) InitMkoMsgB(&MkoMsgB[Index], Index, &MkoBufB[Index], &MkoMsgResB[Index]);

// [настройка пакета C для обмена по МКО]
// инициализация описателя пакета МКО
MkoPackC.nFirst = 0;  // номер первого сообщения
MkoPackC.nLast  = 0;  // номер последнего сообщения
MkoPackC.nPage  = 2;  // номер страницы БОЗУ
MkoPackC.DescPackRes = (mkoPackRes_t*) &MkoPackResC;  // указатель на описатель результата
// инициализация описателей сообщений
for (Index = 0; Index <= 32; Index++) InitMkoMsgC(&MkoMsgC[Index], Index, &MkoBufC[Index], &MkoMsgResC[Index]);

// [настройка пакета X для обмена по МКО]
// инициализация описателя пакета МКО
MkoPackX.nFirst = 0;  // номер первого сообщения
MkoPackX.nLast  = 0;  // номер последнего сообщения
MkoPackX.nPage  = 3;  // номер страницы БОЗУ
MkoPackX.DescPackRes = (mkoPackRes_t*) &MkoPackResX;  // указатель на описатель результата
// инициализация описателей сообщений
InitMkoMsgX(&MkoMsgX, 0, &MkoBufX[0], &MkoMsgResX);
}
//================================================================================

/*! \fn void InitMkoMsgA (mkoPackMsg_t *Msg, int Num, void *Buffer, mkoMsgRes_t *Result);
 * \brief Служебная функция настройки элемента из таблицы описателей сообщений МКО (сообщения для основного потока СПО)
 * \param [in] Msg - указатель на описатель сообщения МКО
 * \param [in] Num - номер описателя сообщения МКО
 * \param [in] Buffer - указатель на буфер данных для сообщения МКО
 * \param [in] Result - указатель на буфер результата обмена для сообщения МКО
 * \details Функция настраивает одну запись из таблицы сообщений МКО, затем 
 * помечает запись как соответствующую пакету A и добавляет указатель на запись в 
 * массив указателей для пакета A.
 */

void InitMkoMsgA (mkoPackMsg_t *Msg, int Num, void *Buffer, mkoMsgRes_t *Result)
{
memset(Msg, 0, sizeof(mkoPackMsg_t));

Msg->DescPack = &MkoPackA;      // указатель на пакет
Msg->nMsg = Num;                // порядковый момер сообщения в пакете
Msg->adrData = (WORD*) Buffer;  // адрес области ОЗУ куда помещаются данные
Msg->DescMsgRes = Result;       // указатель на результат

MkoMsgPtrA[Num] = Msg;          // указатель на сообщение
}
//================================================================================

/*! \fn void InitMkoMsgB (mkoPackMsg_t *Msg, int Num, void *Buffer, mkoMsgRes_t *Result);
 * \brief Служебная функция настройки элемента из таблицы описателей сообщений МКО (сообщения для функций обработки прерываний СПО)
 * \param [in] Msg - указатель на описатель сообщения МКО
 * \param [in] Num - номер описателя сообщения МКО
 * \param [in] Buffer - указатель на буфер данных для сообщения МКО
 * \param [in] Result - указатель на буфер результата обмена для сообщения МКО
 * \details Функция настраивает одну запись из таблицы сообщений МКО, затем 
 * помечает запись как соответствующую пакету B и добавляет указатель на запись в 
 * массив указателей для пакета B.
 */

void InitMkoMsgB (mkoPackMsg_t *Msg, int Num, void *Buffer, mkoMsgRes_t *Result)
{
memset(Msg, 0, sizeof(mkoPackMsg_t));

Msg->DescPack = &MkoPackB;      // указатель на пакет
Msg->nMsg = Num;                // порядковый момер сообщения в пакете
Msg->adrData = (WORD*) Buffer;  // адрес области ОЗУ куда помещаются данные
Msg->DescMsgRes = Result;       // указатель на результат

MkoMsgPtrB[Num] = Msg;          // указатель на сообщение
}
//================================================================================

/*! \fn void InitMkoMsgC (mkoPackMsg_t *Msg, int Num, void *Buffer, mkoMsgRes_t *Result);
 * \brief Служебная функция настройки элемента из таблицы описателей сообщений МКО (сообщения для функций обработки прерываний СПО)
 * \param [in] Msg - указатель на описатель сообщения МКО
 * \param [in] Num - номер описателя сообщения МКО
 * \param [in] Buffer - указатель на буфер данных для сообщения МКО
 * \param [in] Result - указатель на буфер результата обмена для сообщения МКО
 * \details Функция настраивает одну запись из таблицы сообщений МКО, затем 
 * помечает запись как соответствующую пакету C и добавляет указатель на запись в 
 * массив указателей для пакета C.
 */

void InitMkoMsgC (mkoPackMsg_t *Msg, int Num, void *Buffer, mkoMsgRes_t *Result)
{
memset(Msg, 0, sizeof(mkoPackMsg_t));

Msg->DescPack = &MkoPackC;      // указатель на пакет
Msg->nMsg = Num;                // порядковый момер сообщения в пакете
Msg->adrData = (WORD*) Buffer;  // адрес области ОЗУ куда помещаются данные
Msg->DescMsgRes = Result;       // указатель на результат

MkoMsgPtrC[Num] = Msg;          // указатель на сообщение
}
//================================================================================

/*! \fn void InitMkoMsgX (mkoPackMsg_t *Msg, int Num, void *Buffer, mkoMsgRes_t *Result);
 * \brief Служебная функция настройки элемента из таблицы описателей сообщений МКО (сообщения для функций обработки ошибок СПО)
 * \param [in] Msg - указатель на описатель сообщения МКО
 * \param [in] Num - номер описателя сообщения МКО
 * \param [in] Buffer - указатель на буфер данных для сообщения МКО
 * \param [in] Result - указатель на буфер результата обмена для сообщения МКО
 * \details Функция настраивает одну запись из таблицы сообщений МКО, затем 
 * помечает запись как соответствующую пакету X и добавляет указатель на запись в 
 * массив указателей для пакета X.
 */

void InitMkoMsgX (mkoPackMsg_t *Msg, int Num, void *Buffer, mkoMsgRes_t *Result)
{
memset(Msg, 0, sizeof(mkoPackMsg_t));

Msg->DescPack = &MkoPackX;      // указатель на пакет
Msg->nMsg = Num;                // порядковый момер сообщения в пакете
Msg->adrData = (WORD*) Buffer;  // адрес области ОЗУ куда помещаются данные
Msg->DescMsgRes = Result;       // указатель на результат

MkoMsgPtrX[Num] = Msg;          // указатель на сообщение
}
//================================================================================

