
#ifndef MkoStressTestUnitH
#define MkoStressTestUnitH
//================================================================================

#include "MainUnit.h"
//================================================================================

typedef struct
{
mkoPack_t PackDsc;
mkoPackRes_t PackRes;

mkoPackMsg_t MsgDsc[32];
mkoMsgRes_t MsgRes[32];
} TMkoStressLastPackResult;
//================================================================================

// макс размер для списка резульатов для пакетов
#define MKO_STRESS_PACK_LIST_SIZE	(16 * 1024)
// макс размер для списка резульатов для сообщений
#define MKO_STRESS_MSG_LIST_SIZE	(16 * 1024)
//================================================================================

// [счетчики обмена]
extern int MkoStressMsgCount;                              	// общее число переданных сообщений
extern int MkoStressErrorCount;                            	// общее число возникших ошибок

extern int MkoStressLinkErrorCount;                        	// ошибки при передаче
extern int MkoStressNoOsErrorCount;                        	// ошибки "нет ОС"
extern int MkoStressAbErrorCount;                          	// абонент выставил биты ошибок
extern int MkoStressTermErrorCount;                        	// ОУ выставил биты ошибок
extern int MkoStressBusyErrorCount;                        	// абонент выставил занятость
extern int MkoStressMsgErrorCount;                         	// ошибки в сообщении

extern int MkoStressDataErrorCount;                        	// ошибки в данных
extern int MkoStressLastOS;                                	// последнее принятое ОС

extern int MkoStressShowLastPackResult;                    	// флаг выдачи статистики по последнему пакету МКО
extern TMkoStressLastPackResult MkoStressLastPackResult;   	// кеш результатов обмена для последнего пакета

extern int MkoStressPackResCount;       				// число элементов в списке результатов для пакетов
extern mkoPackRes_t MkoStressPackResList[MKO_STRESS_PACK_LIST_SIZE];    // список результатов для пакетов
extern int MkoStressMsgResCount;					// число элементов в списке результатов для сообщений
extern mkoMsgRes_t MkoStressMsgResList[MKO_STRESS_MSG_LIST_SIZE];       // список результатов для сообщений
//================================================================================

void InitMkoStressTest (void);
void MkoStressSaveStateB (void);
void MkoStressSaveStateC (void);
void MkoStressSendStatistic (int BaseMkoLine, int BaseMkoChan);

// void KpaCommand65 (int BaseMkoLine, int BaseMkoChan);

int MkoStressSendTest (int BaseMkoLine, int BaseMkoChan);
int MkoStressRecvTest (int BaseMkoLine, int BaseMkoChan);
int MkoStressSendF4Test (int BaseMkoLine, int BaseMkoChan);

void MkoStressPrepareSet1 (int AMsgCount, void *ABuffer, int AMkoChan, WORD AMkoCmd, WORD Interval);
void MkoStressPrepareSet2 (int AMsgCount, void *ABuffer, int AMkoChan, WORD AMkoCmd, WORD Interval);
void MkoStressPreparePacket1 (int AMsgCount);
void MkoStressPreparePacket2 (int AMsgCount);

void MkoStressCheckTxPacket1 (int MsgCount);
void MkoStressCheckTxPacket2 (int MsgCount);
void MkoStressCheckRxPacket1 (void *AOrigData, int MsgCount);
void MkoStressCheckRxPacket2 (void *AOrigData, int MsgCount);
//================================================================================

#endif

