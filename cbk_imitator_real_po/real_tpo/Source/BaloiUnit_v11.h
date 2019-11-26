
#ifndef BaloiUnit_V11H
#define BaloiUnit_V11H
//================================================================================

#include "MainUnit.h"
//================================================================================

#ifdef BALOI_V11
//================================================================================

#define IRQ13_CMD_FIFO_SIZE		(16)
#define IRQ13_CMD_FIFO_MASK		(IRQ13_CMD_FIFO_SIZE - 1)

typedef struct
{
int MsgLen;
WORD MsgBuf[32 * 32];
} TIrq13CmdBuffer;
//================================================================================

extern int Irq13TxMsgLen, Irq13PrevTxMsgLen;
extern WORD Irq13TxBuffer[32 * 32], Irq13PrevTxBuffer[32 * 32];
extern WORD Irq13RxBuffer[32 * 32];
extern WORD Irq13UserMsgData[32 * 32];

extern volatile int Irq13CmdFifoEnabled, Irq13CmdFifoHead, Irq13CmdFifoTail;
extern TIrq13CmdBuffer Irq13CmdFifo[IRQ13_CMD_FIFO_SIZE];

extern WORD Irq13TimeSEV[3];
extern WORD Irq13DevState[30];
extern WORD Irq13RxIndex, Irq13TxIndex, Irq13PrevTxIndex;
extern WORD Irq13LastMsg, Irq13LastSum, Irq13LastRns;
extern int Irq13SingleEvent;
extern UINT Irq13Semaphore;
extern int Irq13BaloiFail;
extern int Irq13BrtkMode2Enabled;
extern int Irq13SendNackEnabled;

extern volatile int Irq13WaitReplyFlag;
extern sysTime_t Irq13WaitReplyTimer;

extern int Rejim_BRTK;
extern int Komplekt_AOS;
extern int Komplekt_APD_MT;
extern int DN_Vvedena;
extern int LPH_Vkluchena;
extern int Control_ZTM_BUFAR;
extern int Otriz_Kvit;

extern int Ctrl_Ispravnosti_Enabled;    
extern sysTime_t Ctrl_Ispravnosti_Time;
extern int Ctrl_Ispravnosti;      

extern int Code_DB;
extern int AFAR_AT_M;

extern int Komplekt_ALPS;
extern int Komplekt_MFS;
extern int BUFAR;
extern int BUFAR_Sbros;

extern int Rejim_3_Rasch;
extern int Nomer_Shetverti;
extern int Otkl_AFAR;

extern int Pause_MFS[2];
extern WORD Pause_BUM[3];
extern int Pause_PRD;
extern int Pause_Rasch_3;
extern int Pause_LPH;
extern int Pause_OTKL_AFAR;
extern WORD Data_OTKL_AFAR[8];

extern int Control_Tm_AFAR;
extern int Chislo_Proverok_TM_AFAR;
extern int Pause_Proverki_TM_AFAR;
extern int Control_BUM;
extern int Pause_MkoTx;
extern int Pause_AFAR;

extern int Index_AFAR;
extern int SAA_Ustanovlena;
extern int LPH;

extern int Pause_AOS[2];
extern int Otkl_AT;
extern int Otkl_BPA_AT;
extern WORD KPI_Etalon[26 * 32];
extern int KPI_Etalon_Len;
extern WORD KPI_Kvit[2];

extern WORD Vkl_AFAR_BUM[5][2];
extern WORD Tm_AFAR_BUM[5][2];
extern WORD Vkl_AFAR_PRD[5][4][28];
extern WORD Tm_AFAR_PRD[5][15];

extern WORD Vkl_PRD_0[4][28];
//================================================================================

// периодическая пакетная выдача сообщений
extern UINT Baloi_MkoPackSem;                           // семафор для доступа к данным пакетов

extern mkoPack_t Baloi_MkoPack;                         // переменные для обмена по МКО
extern mkoPackRes_t Baloi_MkoPackRes;
extern mkoPackMsg_t Baloi_MkoPackMsg[32];
extern mkoMsgRes_t Baloi_MkoPackMsgRes[32];
extern mkoPackMsg_t* Baloi_MkoPackMsgList[32];

extern u8 Baloi_PackMsgCount[32];                       // число сообщений в пакете, 0..32
extern u16 Baloi_PackInterval[32];                      // интервал передачи пакета, мсек, 500..60000
extern u8 Baloi_PackMkoLine[32];                        // линия МКО для передачи пакета, 0..1
extern u8 Baloi_PackMkoChan[32];                        // канал МКО для передачи пакета, 0..1           
extern u16 Baloi_PackList[32][32][33];                  // данные пакетов, список = 32 пакета, пакет = 32 сообщения, сообщение = CMD+32 слова данных
extern u16 Baloi_PackPrepPause[32];                     // однократная пауза при начальном запуске пакета, 0..FFFF
extern u32 Baloi_PackSendCount[32];                     // число передач пакета, 0..X
extern u32 Baloi_PackOkSendCount[32];                   // число успешных передач пакета, 0..X
extern u32 Baloi_PackErrSendCount[32];                  // число неудачных передач пакета, 0..X
//================================================================================

extern int Baloi_Read_OK;	                                        // флаг циклического считывания ИОК БАЛОИ (раз в 4 сек, def = 0)
extern sysTime_t Baloi_Read_OK_Time;

extern int Baloi_Send_Bau_OK;                                           // флаг контроля исправности ЦБК (раз в 1 мин, def = 0)
extern sysTime_t Baloi_Send_Bau_OK_Time;

#define BALOI_MAX_DISABLED_MSG_COUNT    256

extern WORD Baloi_DisabledMsgList[BALOI_MAX_DISABLED_MSG_COUNT];        // список сообщений от БАЛОИ, игнорируемых СПО
extern int Baloi_DisabledMsgListCount;

extern WORD Baloi_DisabledReplyList[BALOI_MAX_DISABLED_MSG_COUNT];      // список сообщений от БАЛОИ, в ответ на которые не передаются квитанции
extern int Baloi_DisabledReplyListCount;

extern WORD Baloi_DisabledCrcList[BALOI_MAX_DISABLED_MSG_COUNT];        // список сообщений в БАЛОИ, для которых не расчитывается CRC
extern int Baloi_DisabledCrcListCount;

extern WORD Baloi_ReplyTypeList[BALOI_MAX_DISABLED_MSG_COUNT][2];       // список сообщений от БАЛОИ, в ответ на которые передаются указанные квитанции
extern int Baloi_ReplyTypeListCount;

extern WORD Baloi_DebugReadList[BALOI_MAX_DISABLED_MSG_COUNT];          // список сообщений от БАЛОИ, после обработки которых считываются ОК БАЛОИ
extern int Baloi_DebugReadListCount;

extern int RedefinedTxRns;                      // флаг переназначения номера РНС передачи
extern WORD RedefinedTxRnsValue;

//================================================================================

void Irq13InitSys (void);
void Irq13InitData (void);

void Irq13InitMsg (WORD Msg, WORD DataSize);
void Irq13AddToMsg (WORD Data);
int Irq13SendMsgBn (void);
int Irq13SendMsgAn (void);

int Irq13SendUserMsg (WORD Msg, WORD Count);
int Irq13SendMsgBnOverFifo (void);

void Irq13Handler (void);
void Irq13IdleTask (void);
int Irq13MsgHandler (void);
void Irq13Error (int Error, int Point, int Addr);
void Irq13SingleEventMsg (void);
WORD Irq13CalcSum (void *Buffer, int Size);
int Irq13MkoXchgBn (int MkoLine, int MkoChan, int MkoAddr, int MkoSubAddr, int MkoDir, int DataSize, int BlockSize, int *ReadSize, void *DataBuffer);
int Irq13MkoXchgAn (int MkoLine, int MkoChan, int MkoAddr, int MkoSubAddr, int MkoDir, int DataSize, int BlockSize, int *ReadSize, void *DataBuffer);

int Irq13MsgIsDisabled (WORD Msg);
int Irq13ChangeDisabledMsgList (WORD Msg, WORD Mode);

int Irq13ReplyIsDisabled (WORD Msg);
int Irq13ChangeDisabledReplyList (WORD Msg, WORD Mode);

int Irq13CrcIsDisabled (WORD Msg);
int Irq13ChangeDisabledCrcList (WORD Msg, WORD Mode);

int Irq13ReplyTypeRedefined (WORD Msg, WORD *ReplyType);
int Irq13ChangeReplyTypeList (WORD Msg, WORD Mode, WORD ReplyType);

int Irq13DebugReadIsEnabled (WORD Msg);
int Irq13ChangeDebugReadList (WORD Msg, WORD Mode);

int Irq13ChangeCurrentTxRns (WORD Mode, WORD TxRns);

void Irq13DoDebugReadBn (void);
void Irq13DoDebugReadAn (void);

int Irq13Msg0x8756 (void);
int Irq13Msg0xB171 (void);
int Irq13Msg0xB272 (void);
int Send_KPI_Kvit (WORD Kvit);

int Irq13Msg0x8455 (void);
int Irq13Msg0xCF1E (void);

int Irq13Msg0xC312 (void);
int Irq13Msg0xC514 (void);
int Irq13Msg0xA021 (void);
int Irq13Msg0xC918 (void);
int Irq13Msg0xA524 (void);
int Irq13Msg0xB474 (void);
int Irq13Msg0xBE71 (void);

int Irq13Msg0xD233 (void);
int Irq13Msg0xD435 (void);
int Irq13Msg0xD736 (void);
int Irq13Msg0xD839 (void);
int Irq13Msg0xA322 (void);
int Irq13Msg0x9041 (void);
int Irq13Msg0xDB3A (int MsgFlag);

int Irq13Msg0xDE3F (void);
int Irq13Msg0x9647 (void);
int Irq13Msg0xDD3C (void);
int Irq13Msg0x9C4D (void);
int Irq13Msg0x9xxx (void);

void Sbros_Priznakov (void);
int Control_ZTM (int Vkl_LPH);
int Oper_0962 (void);

int Control_AFAR (void);
int Vkluchenie_AFAR (void);
int Control_AFAR_Fail (void);

int Vkluchenie_ALPS_MFC (int Param);
int Otkluchenie_BUFAR (void);
int Otkluchenie_BUM_AFAR (int Mode);

int Otkluchenie_AT (void);
int Otkluchenie_AT_Fail (int Msg);
int K_BRTK_A (int Vkl_LPH);

int MsgProc0xDB3A (int *Status);
int MsgProc0xDB3AExt (void);

int Irq13LoadTableData (int BaseMkoLine, int BaseMkoChan, int VarIndex, int TableIndex, int DataSize);
int ReadExtMsgData (int BaseMkoLine, int BaseMkoChan, WORD Cmd, WORD DataCrc, WORD *Buffer);

int Vkluchenie_LPH (void);
int Control_LPH (int MsgType);
int Vkluchenie_BPA_AT (int Vkl_LPH, int Pause);
int Vkluchenie_LPH (void);
int Otkluchenie_BUM_AFAR (int Mode);
int Otkl_kanalov_AFAR (void);

int Irq13InfoMsg0x35D4 (void);
int Irq13InfoMsg0x5382 (void);
int Irq13InfoMsg0x33D2 (void);

void Irq13MsgToFifo (void);

void BaloiPeriodicTask (int AParam);
int Irq13PackReset (int BaseMkoLine, int BaseMkoChan);
int Irq13PackSetup (int BaseMkoLine, int BaseMkoChan);
int Irq13PackRun (int BaseMkoLine, int BaseMkoChan);
int Irq13PackReadInfo (int BaseMkoLine, int BaseMkoChan);

int Control_BUFAR (void);
//================================================================================

#endif
#endif

