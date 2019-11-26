
#ifndef ProhibitedMkoMsgUnitH
#define ProhibitedMkoMsgUnitH
//================================================================================

#define PROHIBITED_MKO_MSG_COUNT 256
//================================================================================

typedef struct
{
WORD MkoCmd;		// команда абоненту МКО
WORD Data[32];		// буфер под данные абонента
WORD Mask[32];		// буфер под маску данных абонента
} TProhibitedMkoMsg;
//================================================================================

extern int ProhibitedMkoMsgCount;
extern TProhibitedMkoMsg ProhibitedMkoMsgList[PROHIBITED_MKO_MSG_COUNT];
//================================================================================

// заголовки функций блокировки передачи сообщений по МКО
int AddProhibitedMkoMsg (int BaseMkoLine, int BaseMkoChan);
int DelProhibitedMkoMsg (int BaseMkoLine, int BaseMkoChan);
int ReadProhibitedMkoMsgData (int BaseMkoLine, int BaseMkoChan, WORD Cmd, WORD DataCrc, WORD *Buffer);
int FindProhibitedMkoMsg (WORD MkoCmd, WORD *Data, WORD *Mask, int *ItemIndex);
int ProhibitedMkoMsgDataCmp (WORD *Data1, WORD *Data2, int Count);
int GetProhibitedMkoMsgCount (void);
int ClrProhibitedMkoMsgList (void);
void InitProhibitedMkoMsg (void);
int IsProhibitedMkoMsg (WORD MkoCmd, WORD *Data);
int CheckProhibitedMkoMsg (WORD MkoCmd, WORD *Data);
//================================================================================

#endif





