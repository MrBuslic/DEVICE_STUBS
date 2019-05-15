
#ifndef DataUnitH
#define DataUnitH
//================================================================================

#include "Bos.h"     		// заголовки функций BOS
#include "String.h"
#include "Integer.h" 		// определения элементарных типов
#include "MainUnit.h"
//================================================================================

// указатель на функцию обработки прерывания от аппаратуры
typedef void (*IRQ_HANDLER) (void);

// указатель на функцию обработки команды от КПА
typedef void (*KPA_CMD_HANDLER) (int, int);

// указатель на функцию обмена по МКО
typedef int (*MKO_XCHG_FUNC) (int MkoLine, int MkoChan, WORD MkoCmd, int F4Mode);

// буфер данных для обменов на МКО
typedef WORD MKO_BUFFER[32];
//================================================================================

// конфигурация работы СПО
typedef struct
{
int KpaMkoLine; // линия МКО для обмена с КПА, 0 = первая, 1 = вторая
int KpaMkoChan; // канал МКО для обмена с КПА, 0 = основной, 1 = резервный

int DevMkoLine; // линия МКО для обмена с БА, 0 = первая, 1 = вторая
int DevMkoChan; // канал МКО для обмена с БА, 0 = основной, 1 = резервный

int EnabledIrqMask; // маска разрешенных прерываний от аппаратуры, размер 16 бит (бит = 1 - прерывание разрешено для обработки)
int IrqMsgMask; // маска разрешения выдачи сообщения о приходе прерывания, размер 16 бит (бит = 1 - выдача сообщения разрешена)

int ShbkImitEnabled; // флаг разрешения имитации СЧБК (1 = разрешено)
int ShbkImitStatus; // текущий статус режима имитации СЧБК (STATE_XXX)
int ShbkImitMode;
UINT ShbkImitMask;
int ShbkImitMaskReady;

int EnabledImitReply; // флаг разрешения имитации квитанций (1 = разрешено)
int ImitReplyStatus;  // текущий статус режима имитации квитанций (STATE_XXX)
int ImitReplyIndex;   // индекс текущей квитанции (7..0)

int Irq4EvenMode; // счетчик чет/нечет для прерывания 4
int Irq6Mode; // режим обработки прерывания 6

int EnableMultiIrqControl; // разрешить контроль числа одновременных прерываний (def = 0, запрещено)
int EnableMkoErrorExtendedInfo; // разрешить выдачу расширенной информации при сбоях обменов по МКО (1 = разрешено)

int EnableSecondMark; // разрешить выдачу секундной метки (1 = разрешено)
int EnableMkoExtXchg; // включение расширенного обмена по МКО (1 = разрешено)

int EnableCbkDataImit; // флаг работы функции "имитация массива ЦБК" (1 = работает)
int CbkDataImitStatus; // текущий статус режима имитации данных ЦБК (STATE_XXX)

int ShbkFailTimer, ShbkFailCount; // разрешение обработки ошибок и счетчик ошибок при имитации СЧБК
int ReplyFailTimer, ReplyFailCount; // разрешение обработки ошибок и счетчик ошибок при имитации квитанций

int Mbk04DoubleBuffered; // флаг режима двойной буферизации при записи в МБК04 (1 = двойная буферизация включена)
int Mbk04DataFlagAfterIrq; // (для режима двойной буферизации) передача флага готовности данных на МКБ04 после прерывания (1 = вкл, по умолчанию = откл)
int Mbk04SendExtMessage; // флаг выдачи дополнительного сообщения при обмене с МБК04 (1 = выдавать сообщение)
int Mbk04ResizePacket; // флаг управления размером пакета/сообщения при обмене с МБК04 (1 = менять размер)
int Mbk04PacketSize, Mbk04MessageSize; // размеры пакета/сообщения при обмене с МБК04 (1..32)
int Mbk04IncMessageSize; // флаг наращивания размера сообщения при обмене с МБК04 (1 = наращивать)
int Mbk04Irq2ReplyMode; // флаг управления передачей квитанций при обработке прерывания 2 ЕРТИ

int CbkTM; // счетчик выдачи телеметрии ЦБК

int TaskErrorSendInfo; // флаг выдачи сообщения об ошибках по МКО из задачи обработки ошибок
int TaskErrorInfoSize; // размер дополнительной информации об ошибках, слов, выдаваемой по МКО из задачи обработки ошибок

int SendShbkToMbk04InIrq02; // передача СЧБК в МБК04 при обработке прерывания 2 (def = 1)
int SendShbkTo732InIrq02; // передача СЧБК в 732 при обработке прерывания 2 (def = 1)
int Irq02Pups19Enabled; // разрешение обработки состояния ПУПС = 19 при обработке прерывания 2 (def = 1) 

int Mbk04MarkerPosition; // положение меток (чет/нечет и номер пакета) в передаваемом в МБК04 буфере (слово от 0 до 30)

int ErtiIrq2Pause1; // пауза перед запуском обработчика прерывания 2, мсек (def = 0)
int ErtiIrq2Pause2; // пауза перед обработкой состояния ПУПС 4/15 в обработчике прерывания 2, мсек (def = 0)

int ErtiIrq2MsgMask; // маска типов сообщений, передаваймых в процессе обработки прерывания 2 (def = все сообщения = 0x07)

int LkaOpoVersion; // тип ОПО модулей ЛКА, 0 = старый, 1 = новый (def = 0)

int EnableEomMkoFlag; // флаг выдачи прерывания по концу передачи сообщения МКО (для одиночных сообщений) (def = 0) 
int EnableCompMkoMsg; // флаг выдачи дополняющего отладочного сообщения по соседней линии МКО (для одиночных сообщений) (def = 0) 

int EnableMBK07CmdCheck; // флаг разрешения проверки команд вкл/откл аппаратуры передатчика
int EnableMBK07GenOn; // флаг разрешения включения генератора при отключении МБК07

int CheckLkaCmdSize; // вкл/откл контроля размера команд для ЛКА (4/28/TX) (def = 1)
int CheckBupState; // вкл/откл контроля состояния каналов БУП-Е (def = 1)

int Bup_09_State; // состояние каналов БУП-Е-09 (-1 = неизвестно, 0 = отключен, 1 = вкл 1 канал, 2 = вкл 2 канал) (def = -1)
int Bup_19_State; // состояние каналов БУП-Е-19 (-1 = неизвестно, 0 = отключен, 1 = вкл 1 канал, 2 = вкл 2 канал) (def = -1)
int Bup_20_State; // состояние каналов БУП-Е-20 (-1 = неизвестно, 0 = отключен, 1 = вкл 1 канал, 2 = вкл 2 канал) (def = -1)

// флаги управления повторными обменами
// (повтор при занятости приоритетнее)
int MkoReadAgainIfBusyFlag; // флаг повторного чтения при занятости абонента
int MkoReadAgainIfBusyInt; // интервал в мсек при повторном чтении
int MkoReadAgainIfErrorFlag; // флаг повторного чтения при ошибках обмена с абонентом
int MkoReadAgainIfErrorInt; // интервал в мсек при повторном чтении
int MkoWriteAgainIfBusyFlag; // флаг повторной записи при занятости абонента
int MkoWriteAgainIfBusyInt; // интервал в мсек при повторной записи
int MkoWriteAgainIfErrorFlag; // флаг повторной записи при ошибках обмена с абонентом
int MkoWriteAgainIfErrorInt; // интервал в мсек при повторной записи

} TSpoConfig;
//================================================================================

// параметр стартовой задачи, переданный из ОПО
extern int PostParam;

// строка с номером версии ОПО
extern char OpoVersionString[64];
// таблица контрольных сумм секторов флеша (8 по CRC32, 8 по CSumLong)
extern DWORD FlashSectorCSumTable[16];

// разрешение на обработку прерываний от КПА
extern char KPADetected;

// массивы под МКИ
extern UINT PostMkiDataBuf[6 * 4];			// 4 буфера по 6 слов
extern UINT PostMkiDataSize;
extern UINT PostMdiDataBuf[(32 * 4) + (256 * 4) + 256];	// 4 буфера для POST МКИ + запас
extern UINT PostMdiDataSize;

extern char LastError[256];				// строка с информацией о последней ошибке
extern TSpoConfig SpoConfig;				// текущая конфигурация работы СПО
extern sysTime_t IrqTime;				// время прихода прерывания
extern sysTime_t PassBeginTime;				// время отработки секундной метки
extern KPA_CMD_HANDLER CmdHandlerTable[256];		// таблица обработчиков каманд КПА
extern IRQ_HANDLER IrqHandlerTable[16];			// таблица обработчиков прерываний
extern WORD KpaCommand[KPA_CMD_LEN];			// буфер под команду КПА
extern UINT MkiBuffer[512];				// буфер для сброса КМИ в ОПО
extern cfgSYS_t CbkConfig;				// конфигурация машин ЦБК
extern UINT VmStateData[24];				// буфер под массив конфигурации ЦБК после запуска СПО
extern sysTime_t MainTaskTime0, MainTaskTime1;		// время в основном цикле

extern WORD SubsAddrBuffer[32];                        	// массив адресов абонентов для блочного считывания
extern WORD SubsDataBuffer[32 * 32];                   	// массив данных от абонентов при блочном считываним

extern bppJob_t FlashJob;				// описатель задания на коррекцию флеша
extern BYTE FlashDataBuffer[256 * 1024];		// буфер под записываемые во флеш данные (образ сектора)
extern unsigned FlashDataIndex;				// размер данныз в образе сектора
extern unsigned FlashDataAddr;				// адрес для записи данных  во флеше

// пакет A - используется для обменов в основном потоке СПО
extern mkoPack_t MkoPackA;           // описатель пакета по МКО
extern mkoPackRes_t MkoPackResA;     // описатель результата выдачи пакета по МКО
extern mkoPackMsg_t MkoMsgA[33];     // описатели сообщений МКО
extern mkoMsgRes_t MkoMsgResA[33];   // описатели результатов выдачи сообщения по МКО
extern mkoPackMsg_t* MkoMsgPtrA[33]; // массив указателей на сообщения по МКО
extern MKO_BUFFER MkoBufA[33];       // буферы под данные передаваемые по МКО
//================================================================================

// пакет B - используется для обменов в функциях прерываний СПО
extern mkoPack_t MkoPackB;           // описатель пакета по МКО
extern mkoPackRes_t MkoPackResB;     // описатель результата выдачи пакета по МКО
extern mkoPackMsg_t MkoMsgB[33];     // описатели сообщений МКО
extern mkoMsgRes_t MkoMsgResB[33];   // описатели результатов выдачи сообщения по МКО
extern mkoPackMsg_t* MkoMsgPtrB[33]; // массив указателей на сообщения по МКО
extern MKO_BUFFER MkoBufB[33];       // буферы под данные передаваемые по МКО
//================================================================================

// пакет C - используется для обменов в функциях прерываний СПО
extern mkoPack_t MkoPackC;           // описатель пакета по МКО
extern mkoPackRes_t MkoPackResC;     // описатель результата выдачи пакета по МКО
extern mkoPackMsg_t MkoMsgC[33];     // описатели сообщений МКО
extern mkoMsgRes_t MkoMsgResC[33];   // описатели результатов выдачи сообщения по МКО
extern mkoPackMsg_t* MkoMsgPtrC[33]; // массив указателей на сообщения по МКО
extern MKO_BUFFER MkoBufC[33];       // буферы под данные передаваемые по МКО
//================================================================================

// пакет X - используется для передачи информации об ошибках СПО
extern mkoPack_t MkoPackX;           // описатель пакета по МКО
extern mkoPackRes_t MkoPackResX;     // описатель результата выдачи пакета по МКО
extern mkoPackMsg_t MkoMsgX;         // описатели сообщений МКО
extern mkoMsgRes_t MkoMsgResX;       // описатели результатов выдачи сообщения по МКО
extern mkoPackMsg_t* MkoMsgPtrX[1];  // массив указателей на сообщения по МКО
extern MKO_BUFFER MkoBufX[1];        // буферы под данные передаваемые по МКО
//================================================================================

// данные для прерывания 10
extern WORD Irq10Result[2], Irq10Data[18][32];

// отладка для обменов
extern int DebugFlag;
extern mkoPackRes_t DebugPackRes;

// пакет Stub - используется в качестке заглушки
extern MKO_BUFFER MkoBufStub;

// буфер под расширенные данные ошибок на МКО
extern DWORD DebugMki[1024];

// набор переменных для процедур тестирования АСН
extern volatile int AsnIrqCount, AsnExcIrqCount, AsnMissIrqCount;
extern volatile int AsnStatusBeginTime, AsnStatusPrevTime, AsnTimerFailCount, AsnMkoFailCount, AsnParamFailCount;
extern volatile sysTime_t AsnControlStopTime, AsnEnabledZone;
extern volatile int AsnStartTimer, AsnWaitForFirstIrq, AsnIrqAlreadySet;
extern volatile int AsnPreDeltaTime, AsnPostDeltaTime;
extern volatile int AsnControlEnabled;

extern UINT AsnTaskDeltaTimeId;
extern volatile int AsnTaskDeltaTimeInterval;
extern volatile int AsnTaskDeltaTimeTimer;
extern volatile int AsnTaskDeltaTimeFlag;
extern volatile sysTime_t AsnSecondIrqTime;
extern volatile sysTime_t AsnPeriodicTaskTime;

#define ASN_TIME_LIST_SIZE 3600
extern volatile WORD AsnTimeList[ASN_TIME_LIST_SIZE];
extern volatile int AsnTimeListSize;
extern volatile sysTime_t AsnTimeIrqPrev;

// набор переменных для процедур тестирования 732
#define B732_PUPS_LIST_SIZE 50000
extern volatile BYTE B732PupsList[B732_PUPS_LIST_SIZE];
extern volatile int B732PupsListSize;

// набор переменных для процедур тестирования БЭЧ
extern volatile int BehIrqCount, BehExcIrqCount, BehMissIrqCount;
extern volatile int BehFhvIrqCount, BehExcFhvIrqCount, BehMissFhvIrqCount;
extern volatile int BehFinikIrqCount, BehExcFinikIrqCount, BehMissFinikIrqCount;
extern volatile int BehFhvFlag, BehFinikFlag;
extern volatile int BehFhvTmpCount, BehFinikTmpCount;
extern volatile int BehMkoFailCount;
extern volatile sysTime_t BehControlStopTime, BehEnabledZone;
extern volatile int BehStartTimer, BehWaitForFirstIrq, BehIrqAlreadySet;
extern volatile int BehPreDeltaTime, BehPostDeltaTime;
extern volatile int BehFhvPeriod, BehFinikPeriod;
extern volatile int BehControlEnabled;

// специальные переменные для отслеживания задач СПО
extern volatile UINT32 MainTaskIn;    		// переменная увеличивается на 1 при входе в задачу MainTask СПО, инициализация не производится

extern volatile UINT32 ErrorTaskIn;       	// переменная увеличивается на 1 при входе в задачу ErrorTask СПО, инициализация 0 при старте СПО
extern volatile UINT32 ErrorTaskOut;      	// переменная увеличивается на 1 при выходе из задачи ErrorTask СПО, инициализация 0 при старте СПО

extern volatile UINT32 IrqTaskIn;         	// переменная увеличивается на 1 при входе в задачу IrqTask СПО, инициализация 0 при старте СПО
extern volatile UINT32 IrqTaskOut;        	// переменная увеличивается на 1 при выходе из задачи IrqTask СПО, инициализация 0 при старте СПО

extern volatile UINT32 TestTaskIn;        	// переменная увеличивается на 1 при входе в задачу TestTask СПО, инициализация 0 при старте СПО
extern volatile UINT32 TestTaskOut;       	// переменная увеличивается на 1 при выходе из задачи TestTask СПО, инициализация 0 при старте СПО

extern volatile UINT32 AsnTaskIn;         	// переменная увеличивается на 1 при входе в задачу AsnTask СПО, инициализация 0 при старте СПО
extern volatile UINT32 AsnTaskOut;        	// переменная увеличивается на 1 при выходе из задачи AsnTask СПО, инициализация 0 при старте СПО

extern volatile UINT32 MainTaskPdoCount;  	// переменная увеличивается на 1 при каждом проходе цикла ПДО в задаче MainTask СПО, инициализация 0 при старте СПО
extern volatile UINT32 MainTaskCmdFail;   	// переменная увеличивается на 1 при имитации сбоев СПО, инициализация 0 при старте СПО
//================================================================================

// массив для реконфигурации ЦБК
extern dataArr_t ReConfigBuffer[2];
//================================================================================

// буфер данных для имитации массива ЦБК
extern MKO_BUFFER CbkDataImit[33];
//================================================================================

// буфер под данные МДИ
extern UINT MdiBuffer[4][32];
//================================================================================

extern unsigned CallBackCount, ExtTaskCount, CbkErrCtrlCallBackType;
//================================================================================

void DataInit (void);
void InitMkoMsgA (mkoPackMsg_t *Msg, int Num, void *Buffer, mkoMsgRes_t *Result);
void InitMkoMsgB (mkoPackMsg_t *Msg, int Num, void *Buffer, mkoMsgRes_t *Result);
void InitMkoMsgC (mkoPackMsg_t *Msg, int Num, void *Buffer, mkoMsgRes_t *Result);
void InitMkoMsgX (mkoPackMsg_t *Msg, int Num, void *Buffer, mkoMsgRes_t *Result);
//================================================================================

#endif

