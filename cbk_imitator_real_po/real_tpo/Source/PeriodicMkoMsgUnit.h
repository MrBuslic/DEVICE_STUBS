
#ifndef PeriodicMkoMsgUnitH
#define PeriodicMkoMsgUnitH
//================================================================================

#define PERIODIC_MKO_MSG_COUNT 256
//================================================================================

typedef struct
{
WORD MkoCmd;		// команда абоненту МКО
char MkoLine;		// линия МКО для обмена
char MkoChan;		// канал МКО для обмена
char F4Mode;            // команда абоненту в формате Ф4
WORD Interval;          // интервал передачи сообщения, сек
WORD Count;		// сколько раз передать сообщение (-1 = непрерывно)
WORD Data[32];		// буфер под данные приема/передачи для абонента
UINT Pass;              // сколько раз было передано сообщение (0..n)
UINT Timer;             // время следующей передачи сообщения (сек, время ЦБК)
} TPeriodicMkoMsg;
//================================================================================

extern int PeriodicMkoMsgCount, PeriodicMkoMsgLock;
extern TPeriodicMkoMsg PeriodicMkoMsgList[PERIODIC_MKO_MSG_COUNT];
//================================================================================

// заголовки функций периодической передачи сообщений по МКО
int GetPeriodicMkoMsgLock (void);
void SetPeriodicMkoMsgLock (int State);
int AddPeriodicMkoMsg (int BaseMkoLine, int BaseMkoChan);
int DelPeriodicMkoMsg (int BaseMkoLine, int BaseMkoChan);
int ReadPeriodicMkoMsgData (int BaseMkoLine, int BaseMkoChan, WORD Cmd, WORD DataCrc, WORD *Buffer);
int FindPeriodicMkoMsg (WORD MkoCmd, char MkoLine, char MkoChan, char F4Mode, WORD *Data, int *ItemIndex);
int PeriodicMkoMsgDataCmp (WORD *Data1, WORD *Data2, int Count);
int GetPeriodicMkoMsgCount (void);
int ClrPeriodicMkoMsgList (void);
int AbonentToAbonentMkoMsg (int BaseMkoLine, int BaseMkoChan);
//================================================================================

void InitPeriodicMkoMsg (void);
void DoPeriodicMkoMsg (void);
void ProcessPeriodicMkoMsg (void);
//================================================================================

#endif

