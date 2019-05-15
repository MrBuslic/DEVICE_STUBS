
#ifndef PacketMkoMsgUnitH
#define PacketMkoMsgUnitH
//================================================================================

#define PACKET_MKO_MSG_COUNT 	1024		// макс число сообщений
#define PACKET_MKO_INT_COUNT	64		// макс число интервалов
//================================================================================

typedef struct
{
WORD MkoCmd;		// команда абоненту МКО
char F4Mode;            // команда абоненту в формате Ф4
DWORD Interval;         // пауза сообщение-сообщение
WORD Data[32];		// буфер под данные приема/передачи для абонента
} TPacketMkoMsg;
//================================================================================

typedef struct
{
int Interval;		// временной интервал (верхний предел), сек
short MsgList[32];	// список индексов сообщений (из списка PacketMkoMsgList[]) для данного интервала
} TPacketMkoInt;
//================================================================================

typedef struct
{
int Count;		// счетчик пакетов (верхний предел), шт
short MsgList[32];	// список индексов сообщений (из списка PacketMkoMsgList[]) для данного интервала
short Msg0List[32];	// список индексов сообщений для МКО-0 (из списка PacketMkoMsgList[]) для данного интервала
short Msg1List[32];	// список индексов сообщений для МКО-1 (из списка PacketMkoMsgList[]) для данного интервала
} TPacketMkoIntCnt;
//================================================================================

typedef struct
{
WORD MsgNum;            // номер сообщения в списке (0..n)
WORD MkoCmd;            // командное слово МКО
WORD MkoReply;          // ответное слово МКО
WORD SpoResult;         // код результата СПО
} TPacketMkoMsgResult;
//================================================================================

extern int PacketMkoMsgCount;                                                   // число сообщений в списке
extern TPacketMkoMsg PacketMkoMsgList[PACKET_MKO_MSG_COUNT];                    // список описателей сообщений МКО

extern int PacketMkoIntCount;							// число интервалов в списке (интервал - секунды, режим обмена - пинг-понг)
extern TPacketMkoInt PacketMkoIntList[PACKET_MKO_INT_COUNT];                    // список описателей интервалов 

extern int PacketMkoIntCntCount;				        	// число интервалов в списке (интервал - единицы пакетов, режим обмена - пинг-понг)
extern TPacketMkoIntCnt PacketMkoIntCntList[PACKET_MKO_INT_COUNT];              // список описателей интервалов 

extern int PacketMkoIntQueueCount;				       	        // число интервалов в списке (интервал - секунды, режим обмена - очередь) 
extern TPacketMkoInt PacketMkoIntQueue0List[PACKET_MKO_INT_COUNT];              // список описателей интервалов 
extern TPacketMkoInt PacketMkoIntQueue1List[PACKET_MKO_INT_COUNT];              // список описателей интервалов 

extern TPacketMkoMsgResult PacketMkoMsgResultList[PACKET_MKO_MSG_COUNT];        // результат обмена для сообщений МКО, если MsgNum = 0xFFFF - обмен не проводился
extern TPacketMkoMsgResult PacketMkoMsgResultBuf[PACKET_MKO_MSG_COUNT + 2];     // буфер для передачи результата
extern int LastResult;                                                          // результат последнего обмена

extern sysTime_t RunPacketBeginTime, RunPacketCurrTime;                         // время начала прогона/текущее время
//================================================================================

void InitPacketMkoMsg (void);

int AddPacketMkoSingleMsg (int BaseMkoLine, int BaseMkoChan);
int AddPacketMkoMultiMsg (int BaseMkoLine, int BaseMkoChan);

int ReadPacketMkoMsgData (int BaseMkoLine, int BaseMkoChan, WORD Cmd, WORD DataCrc, WORD *Buffer);
int DelPacketMkoMsgByMsg (int BaseMkoLine, int BaseMkoChan);
int FindPacketMkoMsg (WORD MkoCmd, int F4Mode, DWORD Interval, WORD *Data, int *ItemIndex);
int PacketMkoMsgDataCmp (WORD *Data1, WORD *Data2, int Count);
int DelPacketMkoMsgByIndex (int BaseMkoLine, int BaseMkoChan);
int GetPacketMkoMsgCount (void);
int ClrPacketMkoMsgList (void);

int XchgAddrPacketMkoMsgList (void);
int XchgSubAddrPacketMkoMsgList (void);

int RunPacketMkoSingleMsg (int BaseMkoLine, int sBaseMkoChan);
void RunPacketMkoSingleMsgResult (int BaseMkoLine, int BaseMkoChan, int Result);

void PacketMkoMsgPrepareBlock (int BaseIndex, int BlockSize, int DevMkoChan);
int PacketMkoMsgCheckPacket (int BaseIndex, int BlockSize);
int PacketMkoMsgCheckMessages (int BaseIndex, int BlockSize);
void PacketMkoMsgStoreRxData (int BaseIndex, int BlockSize);

int RunPacketMkoMultiMsg (int BaseMkoLine, int sBaseMkoChan);
void RunPacketMkoMultiMsgResult (int BaseMkoLine, int BaseMkoChan, int Result);

// ********

int AddPacketMko_SecPiPo_IntToList (int ABaseMkoLine, int ABaseMkoChan);
int AddPacketMko_SecPiPo_MsgToInt (int ABaseMkoLine, int ABaseMkoChan);
int RunPacketMko_SecPiPo_IntList (int ABaseMkoLine, int ABaseMkoChan);

int RunPacketMko_SecPiPo_CheckList (void);
int RunPacketMko_SecPiPo_PreparePacket1 (int ADeltaTime, int AMkoChan);
int RunPacketMko_SecPiPo_PreparePacket2 (int ADeltaTime, int AMkoChan);

// ********

int AddPacketMko_CntPiPo_IntToList (int ABaseMkoLine, int ABaseMkoChan);
int AddPacketMko_CntPiPo_MsgToInt (int ABaseMkoLine, int ABaseMkoChan);
int RunPacketMko_CntPiPo_IntList (int ABaseMkoLine, int ABaseMkoChan);

int RunPacketMko_CntPiPo_CheckList (void);
int RunPacketMko_CntPiPo_PreparePacket1 (int ATotalCount, int AMkoChan);
int RunPacketMko_CntPiPo_PreparePacket2 (int ATotalCount, int AMkoChan);

// ********

int AddPacketMko_SecQue_IntToList (int ABaseMkoLine, int ABaseMkoChan);
int AddPacketMko_SecQue_MsgToInt (int ABaseMkoLine, int ABaseMkoChan);
int RunPacketMko_SecQue_IntList (int ABaseMkoLine, int ABaseMkoChan);

int RunPacketMko_SecQue_CheckList (void);
int RunPacketMko_SecQue_PreparePacket1 (int ADeltaTime, int AMkoChan);
int RunPacketMko_SecQue_PreparePacket2 (int ADeltaTime, int AMkoChan);

// ********

int AddPacketMko_CntQue_IntToList (int ABaseMkoLine, int ABaseMkoChan);
int AddPacketMko_CntQue_MsgToInt (int ABaseMkoLine, int ABaseMkoChan);
int RunPacketMko_CntQue_IntList (int ABaseMkoLine, int ABaseMkoChan);

int RunPacketMko_CntQue_CheckList (void);
int RunPacketMko_CntQue_PreparePacket1 (int ATotalCount, int AMkoChan);
int RunPacketMko_CntQue_PreparePacket2 (int ATotalCount, int AMkoChan);
//================================================================================

#endif

