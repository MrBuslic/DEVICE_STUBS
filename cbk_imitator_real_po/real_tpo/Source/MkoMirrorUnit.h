
#ifndef MkoMirrorUnitH
#define MkoMirrorUnitH
//================================================================================

#include "MainUnit.h"
//================================================================================

// [параметры обмена с абонентом]
extern int MkoMirrorAddr;                               // 1..31
extern int MkoMirrorLine;                               // 0..1
extern int MkoMirrorWrChan, MkoMirrorRdChan;            // 0..1

extern int MkoMirrorCycles;                             // 1..10000
extern int MkoMirrorWrLen, MkoMirrorRdLen;              // 1..32
extern int MkoMirrorInterval;                           // 30..1000 mks

// [счетчики обмена]
extern WORD MkoMirrorXchgTotal;                         // общее число обменов
extern WORD MkoMirrorAbWrError, MkoMirrorAbRdError;     // число ошибок записи и чтения абонента
extern WORD MkoMirrorXchgWithError;                     // число обменов с ошибками
extern WORD MkoMirrorBadWords, MkoMirrorBadBits;        // число сбойных слов и бит в сообщениях

extern int MkoMirrorXchgMask;		                // маска режима обменов по МКО
extern int MkoMirrorDebug;                              // режим отладки (1 - вкл, 0 - выкл)
//================================================================================

int MkoMirrorDoXchg (int BaseMkoLine, int BaseMkoChan);
void MkoMirrorSendDebugInfo (int BaseMkoLine, int BaseMkoChan, int PointCode, mkoPackRes_t *PackRes, mkoMsgRes_t *MsgRes0, mkoMsgRes_t *MsgRes1);
//================================================================================

#endif

