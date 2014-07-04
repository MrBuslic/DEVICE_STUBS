#ifdef BTI1553API
#undef BTI1553API
#endif
#define BTI1553API __declspec(dllexport)

#ifdef _WIN32
#undef _WIN32
#endif

#include "OmniBus_interface.hpp"

#ifdef _TEST_FACILITY_
extern OmniDriver_interface* omni;
#else
	OmniDriver_interface* omni = new OmniDriver_interface();
#endif

#ifdef __cplusplus
extern "C" {
#endif
/*
// Объявляем функцию DllMain
BOOL APIENTRY DllMain(HINSTANCE hinstDLL,
      DWORD fdwReason, LPVOID lpvReserved)
{

switch (fdwReason)      // Дерево разбора уведомлений
{
  case DLL_PROCESS_ATTACH: // Подключение DLL
    MessageBox(NULL,"Подключение Заглушки BTI1553 для OmniBusBox","Использование заглушек!", MB_ICONINFORMATION);

    if (lpvReserved)  // Определение способа загрузки
      MessageBox(NULL,"DLL загружена с неявной компоновкой","Использование заглушек!", MB_ICONINFORMATION);
    else
      MessageBox(NULL,"DLL загружена с явной компоновкой","Использование заглушек!", MB_ICONINFORMATION);
    return 1; // успешная инициализация

  case DLL_PROCESS_DETACH: // Отключение DLL
    // Здесь – освобождаем память, закрываем
    // файлы и т.д.
    break;

  case DLL_THREAD_ATTACH: // Уведомление о новом потоке 
    // Здесь – если надо переходим на
    // многопоточный режим работы с
    // использованием средств синхронизации
    // таких как критическая секция, мутанты,
    // семафоры и т.д.
    break;

  case DLL_THREAD_DETACH:
      //Уведомление о завершении потока
    // Здесь – если надо освобождаем все ресурсы, 
    // вязанные с завершившимся потоком. Какой именно
    // поток завершился можно узнать просмотром списка
    // потоков средствами TOOLHELP32
    MessageBox(NULL,"Использование заглушек!","Завершение потока", MB_ICONINFORMATION);
    break;

  }
return TRUE;    // Код возврата игнорируется
}
*/


/**
*
*  BTI1553 Driver functions.
*
**/

BTI1553API ERRVAL __stdcall BTI1553_BCConfig(ULONG configval,INT channum,HCORE handleval){ return 0; }
BTI1553API ERRVAL __stdcall BTI1553_BCConfigEx(ULONG configval,USHORT count,INT channum,HCORE handleval){ return 0; }
BTI1553API MSGADDR __stdcall BTI1553_BCConfigMsg(ULONG configval,MSGADDR msgaddr,HCORE handleval){ return 0; }
BTI1553API LISTADDR __stdcall BTI1553_BCCreateList(ULONG listconfigval,INT count,ULONG msgconfigval,USHORT cwd1,USHORT cwd2,LPUSHORT data,HCORE handleval){ return 0; }
BTI1553API MSGADDR __stdcall BTI1553_BCCreateMsg(ULONG configval,USHORT cwd1,USHORT cwd2,LPUSHORT data,HCORE handleval){ return 0; }
BTI1553API VOID __stdcall BTI1553_BCPause(INT channum,HCORE handleval){}
BTI1553API INT __stdcall BTI1553_BCPauseCheck(INT channum,HCORE handleval){ return 0; }
BTI1553API VOID __stdcall BTI1553_BCResume(INT channum,HCORE handleval){ }
BTI1553API SCHNDX __stdcall BTI1553_BCSchedAgain(SCHNDX index,INT channum,HCORE handleval){ return 0; }
BTI1553API SCHNDX __stdcall BTI1553_BCSchedBranch(ULONG condition,SCHNDX destindex,INT channum,HCORE handleval){ return 0; }
BTI1553API ERRVAL __stdcall BTI1553_BCSchedBranchUpdate(SCHNDX destindex,SCHNDX opcodeindex,INT channum,HCORE handleval){ return 0; }
BTI1553API ERRVAL __stdcall BTI1553_BCSchedBuild(USHORT nummsgs,LPMSGADDR msgaddr,LPUSHORT freq,INT channum,HCORE handleval){ return 0; }
BTI1553API SCHNDX __stdcall BTI1553_BCSchedCall(ULONG condition,SCHNDX destindex,INT channum,HCORE handleval){ return 0; }
BTI1553API ERRVAL __stdcall BTI1553_BCSchedCallUpdate(SCHNDX destindex,SCHNDX opcodeindex,INT channum,HCORE handleval){ return 0; }
BTI1553API SCHNDX __stdcall BTI1553_BCSchedEntry(INT channum,HCORE handleval){ return 0; }
BTI1553API SCHNDX __stdcall BTI1553_BCSchedFrame(ULONG timeval,INT channum,HCORE handleval){ return 0; }
BTI1553API SCHNDX __stdcall BTI1553_BCSchedFrameEnd(INT channum,HCORE handleval){ return 0; }
BTI1553API SCHNDX __stdcall BTI1553_BCSchedFrameStart(ULONG timeval,INT channum,HCORE handleval){ return 0; }
BTI1553API SCHNDX __stdcall BTI1553_BCSchedGap(USHORT gapval,INT channum,HCORE handleval){ return 0; }
BTI1553API SCHNDX __stdcall BTI1553_BCSchedHalt(INT channum,HCORE handleval){ return 0; }
BTI1553API SCHNDX __stdcall BTI1553_BCSchedLog(ULONG condition,USHORT tagval,INT channum,HCORE handleval){ return 0; }
BTI1553API SCHNDX __stdcall BTI1553_BCSchedMsg(MSGADDR msgaddr,INT channum,HCORE handleval){ return 0; }
BTI1553API SCHNDX __stdcall BTI1553_BCSchedPause(INT channum,HCORE handleval){ return 0; }
BTI1553API SCHNDX __stdcall BTI1553_BCSchedPulse(INT dionum,INT channum,HCORE handleval){ return 0; }
BTI1553API SCHNDX __stdcall BTI1553_BCSchedPulse0(INT dionum,INT channum,HCORE handleval){ return 0; }
BTI1553API SCHNDX __stdcall BTI1553_BCSchedPulse1(INT dionum,INT channum,HCORE handleval){ return 0; }
BTI1553API SCHNDX __stdcall BTI1553_BCSchedRestart(INT channum,HCORE handleval){ return 0; }
BTI1553API SCHNDX __stdcall BTI1553_BCSchedRetry(ULONG condition,USHORT retries,INT channum,HCORE handleval){ return 0; }
BTI1553API SCHNDX __stdcall BTI1553_BCSchedReturn(INT channum,HCORE handleval){ return 0; }
BTI1553API ERRVAL __stdcall BTI1553_BCSetDefaultGap(INT gapval,INT channum,HCORE handleval){ return 0; }
BTI1553API ERRVAL __stdcall BTI1553_BCSetTimeout(USHORT timeoutval,INT channum,HCORE handleval){ return 0; }
BTI1553API ERRVAL __stdcall BTI1553_BCSyncDefine(BOOL enableflag,USHORT syncmask,USHORT pinpolarity,INT channum,HCORE handleval){ return 0; }
BTI1553API ERRVAL __stdcall BTI1553_BCTransmitMsg(LPXMITFIELDS1553 xmitfields,INT channum,HCORE handleval){ return 0; }
BTI1553API ERRVAL __stdcall BTI1553_BCTriggerDefine(BOOL enableflag,USHORT trigmask,USHORT trigval,USHORT pinpolarity,INT channum,HCORE handleval){ return 0; }
BTI1553API VOID __stdcall BTI1553_ChGetCount(LPINT a_count,LPINT b4_count,LPINT b32_count,LPINT c_count,HCORE handleval){ }
BTI1553API ULONG __stdcall BTI1553_ChGetInfo(USHORT infotype,INT channum,HCORE handleval){ return 0; }
BTI1553API BOOL __stdcall BTI1553_ChIs1553(INT channum,HCORE handleval){ return 0; }
BTI1553API BOOL __stdcall BTI1553_ChIsA(INT channum,HCORE handleval){ return 0; }
BTI1553API BOOL __stdcall BTI1553_ChIsB32(INT channum,HCORE handleval){ return 0; }
BTI1553API BOOL __stdcall BTI1553_ChIsB4(INT channum,HCORE handleval){ return 0; }
BTI1553API BOOL __stdcall BTI1553_ChIsBM(INT channum,HCORE handleval){ return 0; }
BTI1553API BOOL __stdcall BTI1553_ChIsC(INT channum,HCORE handleval){ return 0; }
BTI1553API BOOL __stdcall BTI1553_ChIsD(INT channum,HCORE handleval){ return 0; }
BTI1553API BOOL __stdcall BTI1553_ChIsM(INT channum,HCORE handleval){ return 0; }
BTI1553API BOOL __stdcall BTI1553_ChIsS(INT channum,HCORE handleval){ return 0; }
BTI1553API BOOL __stdcall BTI1553_ChIsX(INT channum,HCORE handleval){ return 0; }
BTI1553API BOOL __stdcall BTI1553_ChStart(INT channum,HCORE handleval){ return 0; }
BTI1553API BOOL __stdcall BTI1553_ChStop(INT channum,HCORE handleval){ return 0; }
BTI1553API USHORT __stdcall BTI1553_CmdMaxLoopRd(INT channum,HCORE handleval){ return 0; }
BTI1553API VOID __stdcall BTI1553_CmdMaxLoopWr(USHORT countval,INT channum,HCORE handleval){  }
BTI1553API BOOL __stdcall BTI1553_CmdShotRd(SCHNDX index,INT channum,HCORE handleval){ return 0; }
BTI1553API ERRVAL __stdcall BTI1553_CmdShotWr(BOOL value,SCHNDX index,INT channum,HCORE handleval){ return 0; }
BTI1553API BOOL __stdcall BTI1553_CmdSkipRd(SCHNDX index,INT channum,HCORE handleval){ return 0; }
BTI1553API ERRVAL __stdcall BTI1553_CmdSkipWr(BOOL value,SCHNDX index,INT channum,HCORE handleval){ return 0; }
BTI1553API BOOL __stdcall BTI1553_CmdStepRd(SCHNDX index,INT channum,HCORE handleval){ return 0; }
BTI1553API ERRVAL __stdcall BTI1553_CmdStepWr(BOOL value,SCHNDX index,INT channum,HCORE handleval){ return 0; }
BTI1553API ERRVAL __stdcall BTI1553_ErrorCtrl(ULONG ctrlval,INT channum,HCORE handleval){ return 0; }
BTI1553API ERRVAL __stdcall BTI1553_ErrorDefine(ULONG defineval,USHORT errvalue,INT countval,USHORT wordpos,USHORT bitpos,INT channum,HCORE handleval){ return 0; }
BTI1553API BOOL __stdcall BTI1553_ErrorSent(INT channum,HCORE handleval){ return 0; }
BTI1553API ERRVAL __stdcall BTI1553_ErrorTagBC(BOOL tagval,MSGADDR msgaddr,INT channum,HCORE handleval){ return 0; }
BTI1553API ERRVAL __stdcall BTI1553_ErrorTagRT(BOOL tagval,BOOL mcflag,INT taval,BOOL trflag,INT saval,INT channum,HCORE handleval){ return 0; }
BTI1553API ERRVAL __stdcall BTI1553_ErrorTriggerDefine(BOOL enableflag,USHORT trigmask,USHORT trigval,USHORT pinpolarity,INT channum,HCORE handleval){ return 0; }
BTI1553API MSGADDR __stdcall BTI1553_ListBlockRd(LPUSHORT buf,LISTADDR listaddr,HCORE handleval){ return 0; }
BTI1553API MSGADDR __stdcall BTI1553_ListBlockWr(LPUSHORT buf,LISTADDR listaddr,HCORE handleval){ return 0; }
BTI1553API INT __stdcall BTI1553_ListDataRd(LPUSHORT buf,INT count,LISTADDR listaddr,HCORE handleval){ return 0; }
BTI1553API INT __stdcall BTI1553_ListDataWr(LPUSHORT buf,INT count,LISTADDR listaddr,HCORE handleval){ return 0; }
BTI1553API BOOL __stdcall BTI1553_ListMultiBlockRd(LPUSHORT buf,LPINT blkcountptr,LISTADDR listaddr,HCORE handleval)
{ 
	return omni->ListMultiBlockRd(buf, blkcountptr, listaddr, handleval);
}
BTI1553API BOOL __stdcall BTI1553_ListMultiBlockWr(LPUSHORT buf,INT blkcount,LISTADDR listaddr,HCORE handleval){ return 0; }
BTI1553API ERRVAL __stdcall BTI1553_MonConfig(ULONG configval,INT channum,HCORE handleval){ return 0; }
BTI1553API ERRVAL __stdcall BTI1553_MonFilterSA(INT taval,ULONG rcvsamask,ULONG xmtsamask,ULONG rcvmcmask,ULONG xmtmcmask,INT channum,HCORE handleval){ return 0; }
BTI1553API ERRVAL __stdcall BTI1553_MonFilterTA(ULONG tamask,INT channum,HCORE handleval){ return 0; }
BTI1553API MSGADDR __stdcall BTI1553_MsgBlockRd(LPMSGFIELDS1553 buf,MSGADDR msgaddr,HCORE handleval){ return 0; }
BTI1553API MSGADDR __stdcall BTI1553_MsgBlockWr(LPMSGFIELDS1553 buf,MSGADDR msgaddr,HCORE handleval){ return 0; }
BTI1553API VOID __stdcall BTI1553_MsgDataRd(LPUSHORT buf,INT count,MSGADDR msgaddr,HCORE handleval){ }
BTI1553API VOID __stdcall BTI1553_MsgDataWr(LPUSHORT buf,INT count,MSGADDR msgaddr,HCORE handleval)
{
	omni->MsgDataWr(buf, count, msgaddr, handleval);
}
BTI1553API ULONG __stdcall BTI1553_MsgFieldRd(USHORT fieldtype,MSGADDR msgaddr,HCORE handleval){ return 0; }
BTI1553API ULONG __stdcall BTI1553_MsgFieldWr(ULONG fieldval,USHORT fieldtype,MSGADDR msgaddr,HCORE handleval){ return 0; }
BTI1553API VOID __stdcall BTI1553_MsgGroupDataRd(INT nummsgs,USHORT databufs[][32],LPMSGADDR msgaddrptr,HCORE handleval){  }
BTI1553API VOID __stdcall BTI1553_MsgGroupDataWr(INT nummsgs,USHORT databufs[][32],LPMSGADDR msgaddrptr,HCORE handleval){  }
BTI1553API VOID __stdcall BTI1553_MsgGroupRd(INT nummsgs,LPMSGFIELDS1553 msgflds,LPMSGADDR msgaddrptr,HCORE handleval){ }
BTI1553API VOID __stdcall BTI1553_MsgGroupWr(INT nummsgs,LPMSGFIELDS1553 msgflds,LPMSGADDR msgaddrptr,HCORE handleval){ }
BTI1553API BOOL __stdcall BTI1553_MsgSkipRd(MSGADDR msgaddr,HCORE handleval){ return 0; }
BTI1553API VOID __stdcall BTI1553_MsgSkipWr(BOOL skip,MSGADDR msgaddr,HCORE handleval){  }
BTI1553API ERRVAL __stdcall BTI1553_MsgSyncDefine(BOOL enableflag,USHORT syncmask,USHORT pinpolarity,MSGADDR msgaddr,HCORE handleval){ return 0; }
BTI1553API ERRVAL __stdcall BTI1553_MsgTriggerDefine(BOOL enableflag,USHORT trigmask,USHORT trigval,USHORT pinpolarity,MSGADDR msgaddr,HCORE handleval){ return 0; }
BTI1553API ERRVAL __stdcall BTI1553_ParamAmplitudeConfig(ULONG configval,USHORT dacval,INT channum,HCORE handleval){ return 0; }
BTI1553API ERRVAL __stdcall BTI1553_ParamAmplitudeGet(LPUSHORT dacval,INT channum,HCORE handleval){ return 0; }
BTI1553API ERRVAL __stdcall BTI1553_PlayConfig(ULONG configval,ULONG tamask,INT channum,HCORE handleval){ return 0; }
BTI1553API INT __stdcall BTI1553_PlayStatus(INT channum,HCORE handleval){ return 0; }
BTI1553API USHORT __stdcall BTI1553_PlayWr(LPUSHORT buf,USHORT bufcount,INT channum,HCORE handleval){ return 0; }
BTI1553API ERRVAL __stdcall BTI1553_RTConfig(ULONG configval,INT taval,INT channum,HCORE handleval){ return 0; }
BTI1553API LISTADDR __stdcall BTI1553_RTCreateList(ULONG listconfigval,INT count,ULONG msgconfigval,BOOL mcflag,INT taval,BOOL trflag,INT saval,INT channum,HCORE handleval){ return 0; }
BTI1553API MSGADDR __stdcall BTI1553_RTCreateMsg(ULONG configval,BOOL mcflag,INT taval,BOOL trflag,INT saval,INT channum,HCORE handleval){ return 0; }
BTI1553API MSGADDR __stdcall BTI1553_RTGetMsg(BOOL mcflag,INT taval,BOOL trflag,INT saval,INT channum,HCORE handleval){ return 0; }
BTI1553API ERRVAL __stdcall BTI1553_RTReset(INT taval,INT channum,HCORE handleval){ return 0; }
BTI1553API ERRVAL __stdcall BTI1553_RTResponseTimeSet(INT resptime,INT taval,INT channum,HCORE handleval){ return 0; }
BTI1553API ERRVAL __stdcall BTI1553_RTSetMode(ULONG configval,INT taval,INT channum,HCORE handleval){ return 0; }
BTI1553API USHORT __stdcall BTI1553_RTSWDRd(INT taval,INT channum,HCORE handleval){ return 0; }
BTI1553API ERRVAL __stdcall BTI1553_RTSWDWr(USHORT swdval,INT taval,INT channum,HCORE handleval){ return 0; }
BTI1553API ERRVAL __stdcall BTI1553_RTSyncDefine(BOOL enableflag,USHORT syncmask,USHORT pinpolarity,INT taval,ULONG rcvsamask,ULONG xmtsamask,ULONG rcvmcmask,ULONG xmtmcmask,INT channum,HCORE handleval){ return 0; }
BTI1553API USHORT __stdcall BTI1553_ValPackCWD(INT TAval,INT TRflag,INT SAval,INT WCval){ return 0; }
BTI1553API VOID __stdcall BTI1553_ValUnpackCWD(USHORT CWDval,LPINT TAval,LPINT TRflag,LPINT SAval,LPINT WCval){ }

/**
*
*  Internal functions used by the BTI1553 Driver.
*
**/

BTI1553API LISTADDR __stdcall BTI1553_BCCreateListEx(ULONG listconfigval,INT skipval,INT count,ULONG msgconfigval,USHORT cwd1,USHORT cwd2,LPUSHORT data,HCORE handleval){ return 0; }
BTI1553API LISTADDR __stdcall BTI1553_BCCreateListExx(ULONG listconfigval,INT skipval,INT count,ULONG msgconfigval,USHORT cwd1,USHORT cwd2,LPUSHORT data,HCORE handleval){ return 0; }
BTI1553API MSGADDR __stdcall BTI1553_BCGetMsg(INT index,INT channum,HCORE handleval){ return 0; }
BTI1553API ERRVAL __stdcall BTI1553_BCIMGapRd(LPUSHORT gapval,INT channum,HCORE handleval){ return 0; }
BTI1553API ERRVAL __stdcall BTI1553_BCIMGapWr(USHORT gapval,INT channum,HCORE handleval){ return 0; }
BTI1553API SCHNDX __stdcall BTI1553_BCSchedBranchEx(USHORT condition1,USHORT condition2,USHORT condition3,USHORT opcode,SCHNDX destindex,INT channum,HCORE handleval){ return 0; }
BTI1553API SCHNDX __stdcall BTI1553_BCSchedCallEx(USHORT condition1,USHORT condition2,USHORT condition3,USHORT opcode,SCHNDX destindex,INT channum,HCORE handleval){ return 0; }
BTI1553API SCHNDX __stdcall BTI1553_BCSchedLogEx(USHORT condition1,USHORT condition2,USHORT condition3,USHORT opcode,USHORT tagval,INT channum,HCORE handleval){ return 0; }
BTI1553API SCHNDX __stdcall BTI1553_BCSchedNop(INT channum,HCORE handleval){ return 0; }
BTI1553API SCHNDX __stdcall BTI1553_BCSchedRetryEx(USHORT condition1,USHORT condition2,USHORT condition3,USHORT retryflag,USHORT retries,INT channum,HCORE handleval){ return 0; }
BTI1553API SCHNDX __stdcall BTI1553_BCSchedUser(ULONG useraddr,INT channum,HCORE handleval){ return 0; }
BTI1553API SCHNDX __stdcall BTI1553_BCSetEntry(INT channum,HCORE handleval){ return 0; }
BTI1553API ERRVAL __stdcall BTI1553_BCTransmitMsgBlkEx(BOOL writeflag,INT count,LPXMITFIELDS1553 xmitfields,INT channum,HCORE handleval){ return 0; }
BTI1553API ERRVAL __stdcall BTI1553_BCTransmitMsgBlkInit(INT maxcount,INT channum,HCORE handleval){ return 0; }
BTI1553API ERRVAL __stdcall BTI1553_BCTransmitMsgBlkRd(INT count,LPXMITFIELDS1553 xmitfields,INT channum,HCORE handleval){ return 0; }
BTI1553API ERRVAL __stdcall BTI1553_BCTransmitMsgBlkWr(INT count,LPXMITFIELDS1553 xmitfields,INT channum,HCORE handleval){ return 0; }
BTI1553API USHORT __stdcall BTI1553_BlkAddr(INT taval,INT channum,HCORE handleval){ return 0; }
BTI1553API USHORT __stdcall BTI1553_BlkOptRd(INT taval,INT channum,HCORE handleval){ return 0; }
BTI1553API ERRVAL __stdcall BTI1553_BlkOptWr(USHORT rtopt,INT taval,INT channum,HCORE handleval){ return 0; }
BTI1553API ULONG __stdcall BTI1553_BlkPtrRd(INT taval,INT channum,HCORE handleval){ return 0; }
BTI1553API VOID __stdcall BTI1553_BlkPtrWr(ULONG descaddr,INT taval,INT channum,HCORE handleval){ }
BTI1553API ERRVAL __stdcall BTI1553_BlkRd(LPUSHORT block,INT taval,INT channum,HCORE handleval){ return 0; }
BTI1553API ERRVAL __stdcall BTI1553_BlkReset(INT taval,INT channum,HCORE handleval){ return 0; }
BTI1553API USHORT __stdcall BTI1553_BlkSWDRd(INT taval,INT channum,HCORE handleval){ return 0; }
BTI1553API ERRVAL __stdcall BTI1553_BlkSWDWr(USHORT swdval,INT taval,INT channum,HCORE handleval){ return 0; }
BTI1553API ERRVAL __stdcall BTI1553_BlkWipe(INT channum,HCORE handleval){ return 0; }
BTI1553API ERRVAL __stdcall BTI1553_BlkWr(LPUSHORT block,INT taval,INT channum,HCORE handleval){ return 0; }
BTI1553API ERRVAL __stdcall BTI1553_ChConfig(BOOL enableflag,INT type,USHORT taval,INT channum,HCORE handleval){ return 0; }
BTI1553API BOOL __stdcall BTI1553_ChIsRunning(INT channum,HCORE handleval){ return 0; }
BTI1553API ULONG __stdcall BTI1553_CmdAddr(SCHNDX index,INT channum,HCORE handleval){ return 0; }
BTI1553API ULONG __stdcall BTI1553_CmdAlloc(INT count,HCORE handleval){ return 0; }
BTI1553API ULONG __stdcall BTI1553_CmdAllocEx(ULONG configval,INT count,HCORE handleval){ return 0; }
BTI1553API ULONG __stdcall BTI1553_CmdBaseRd(INT channum,HCORE handleval){ return 0; }
BTI1553API VOID __stdcall BTI1553_CmdBaseWr(ULONG addrval,INT channum,HCORE handleval){  }
BTI1553API ERRVAL __stdcall BTI1553_CmdClear(INT channum,HCORE handleval){ return 0; }
BTI1553API USHORT __stdcall BTI1553_CmdCountRd(INT channum,HCORE handleval){ return 0; }
BTI1553API VOID __stdcall BTI1553_CmdCountWr(USHORT countval,INT channum,HCORE handleval){}
BTI1553API USHORT __stdcall BTI1553_CmdCtrlRd(SCHNDX index,INT channum,HCORE handleval){ return 0; }
BTI1553API VOID __stdcall BTI1553_CmdCtrlWr(USHORT ctrlval,SCHNDX index,INT channum,HCORE handleval){  }
BTI1553API USHORT __stdcall BTI1553_CmdCurrRd(INT channum,HCORE handleval){ return 0; }
BTI1553API VOID __stdcall BTI1553_CmdCurrWr(USHORT addrval,INT channum,HCORE handleval){ }
BTI1553API VOID __stdcall BTI1553_CmdInit(USHORT opcode,LPUSHORT block){ }
BTI1553API SCHNDX __stdcall BTI1553_CmdInsert(LPUSHORT block,INT channum,HCORE handleval){ return 0; }
BTI1553API ERRVAL __stdcall BTI1553_CmdRd(LPUSHORT block,ULONG addrval,HCORE handleval){ return 0; }
BTI1553API VOID __stdcall BTI1553_CmdRestart(INT channum,HCORE handleval){  }
BTI1553API VOID __stdcall BTI1553_CmdStackClr(INT channum,HCORE handleval){  }
BTI1553API USHORT __stdcall BTI1553_CmdStartRd(INT channum,HCORE handleval){ return 0; }
BTI1553API VOID __stdcall BTI1553_CmdStartWr(USHORT addrval,INT channum,HCORE handleval){  }
BTI1553API INT __stdcall BTI1553_CmdTotalRd(INT channum,HCORE handleval){ return 0; }
BTI1553API VOID __stdcall BTI1553_CmdTotalWr(INT countval,INT channum,HCORE handleval){  }
BTI1553API ERRVAL __stdcall BTI1553_CmdWr(LPUSHORT block,ULONG addrval,HCORE handleval){ return 0; }
BTI1553API ULONG __stdcall BTI1553_DescAddr(BOOL mcflag,INT taval,BOOL trflag,INT saval,INT channum,HCORE handleval){ return 0; }
BTI1553API ULONG __stdcall BTI1553_DescAlloc(HCORE handleval){ return 0; }
BTI1553API ERRVAL __stdcall BTI1553_DescConfig(ULONG configval,BOOL mcflag,INT taval,BOOL trflag,INT saval,INT channum,HCORE handleval){ return 0; }
BTI1553API ERRVAL __stdcall BTI1553_DescConfigAll(ULONG configval,INT taval,INT channum,HCORE handleval){ return 0; }
BTI1553API USHORT __stdcall BTI1553_DescOptRd(BOOL mcflag,INT taval,BOOL trflag,INT saval,INT channum,HCORE handleval){ return 0; }
BTI1553API VOID __stdcall BTI1553_DescOptWr(USHORT descopt,BOOL mcflag,INT taval,BOOL trflag,INT saval,INT channum,HCORE handleval){  }
BTI1553API MSGADDR __stdcall BTI1553_DescPtrRd(BOOL mcflag,INT taval,BOOL trflag,INT saval,INT channum,HCORE handleval){ return 0; }
BTI1553API ULONG __stdcall BTI1553_DescPtrWr(MSGADDR msgaddr,BOOL mcflag,INT taval,BOOL trflag,INT saval,INT channum,HCORE handleval){ return 0; }
BTI1553API ULONG __stdcall BTI1553_DescRd(BOOL mcflag,INT taval,BOOL trflag,INT saval,LPUSHORT desc,INT channum,HCORE handleval){ return 0; }
BTI1553API ULONG __stdcall BTI1553_DescWr(BOOL mcflag,INT taval,BOOL trflag,INT saval,LPUSHORT desc,INT channum,HCORE handleval){ return 0; }
BTI1553API INT __stdcall BTI1553_DllUsageCount(VOID){ return 0; }
BTI1553API BOOL __stdcall BTI1553_ErrorIsTaggedBC(MSGADDR msgaddr,INT channum,HCORE handleval){ return 0; }
BTI1553API BOOL __stdcall BTI1553_ErrorIsTaggedRT(BOOL mcflag,INT taval,BOOL trflag,INT saval,INT channum,HCORE handleval){ return 0; }
BTI1553API ERRVAL __stdcall BTI1553_ErrorResponse(USHORT responseval,INT channum,HCORE handleval){ return 0; }
BTI1553API VOID __stdcall BTI1553_ErrorSync(BOOL value,INT channum,HCORE handleval){  }
BTI1553API ERRVAL __stdcall BTI1553_ErrorTimeout(USHORT timeoutval,INT channum,HCORE handleval){ return 0; }
BTI1553API ERRVAL __stdcall BTI1553_HookMsgProc(INT msgval,LPVOID lpParam,HCORE handleval){ return 0; }
BTI1553API ERRVAL __stdcall BTI1553_HostComm(LPVOID lpParam,HCORE handleval){ return 0; }
BTI1553API BOOL __stdcall BTI1553_IsChan(INT channum,HCORE handleval){ return 0; }
BTI1553API MSGADDR __stdcall BTI1553_ListAddr(INT index,LISTADDR listaddr,HCORE handleval){ return 0; }
BTI1553API MSGADDR __stdcall BTI1553_ListAddrEx(INT index,LISTADDR listaddr,LPUSHORT header,HCORE handleval){ return 0; }
BTI1553API MSGADDR __stdcall BTI1553_ListBlockRdEx(LPUSHORT buf,INT index,LISTADDR listaddr,HCORE handleval){ return 0; }
BTI1553API MSGADDR __stdcall BTI1553_ListBlockWrEx(LPUSHORT buf,INT index,LISTADDR listaddr,HCORE handleval){ return 0; }
BTI1553API ERRVAL __stdcall BTI1553_ListClear(LISTADDR listaddr,HCORE handleval){ return 0; }
BTI1553API LISTADDR __stdcall BTI1553_ListConfig(ULONG configval,INT count,USHORT skipval,HCORE handleval){ return 0; }
BTI1553API LISTADDR __stdcall BTI1553_ListConfigEx(ULONG configval,INT count,USHORT skipval,HCORE handleval){ return 0; }
BTI1553API INT __stdcall BTI1553_ListDataRdEx(LPUSHORT buf,INT count,INT index,LISTADDR listaddr,HCORE handleval){ return 0; }
BTI1553API INT __stdcall BTI1553_ListDataWrEx(LPUSHORT buf,INT count,INT index,LISTADDR listaddr,HCORE handleval){ return 0; }
BTI1553API ERRVAL __stdcall BTI1553_ListInit(LISTADDR listaddr,MSGADDR msgaddr,HCORE handleval){ return 0; }
BTI1553API MSGADDR __stdcall BTI1553_ListNextRd(ULONG listaddr,LPUSHORT header,HCORE handleval){ return 0; }
BTI1553API MSGADDR __stdcall BTI1553_ListNextWr(ULONG listaddr,LPUSHORT header,HCORE handleval){ return 0; }
BTI1553API VOID __stdcall BTI1553_ListPostRd(LISTADDR listaddr,LPUSHORT header,HCORE handleval){ }
BTI1553API VOID __stdcall BTI1553_ListPostWr(LISTADDR listaddr,LPUSHORT header,HCORE handleval){  }
BTI1553API VOID __stdcall BTI1553_ListPreRd(LISTADDR listaddr,LPUSHORT header,HCORE handleval){  }
BTI1553API VOID __stdcall BTI1553_ListPreWr(LISTADDR listaddr,LPUSHORT header,HCORE handleval){  }
BTI1553API INT __stdcall BTI1553_ListStatus(LISTADDR listaddr,HCORE handleval){ return 0; }
BTI1553API BOOL __stdcall BTI1553_ModeIsBC(USHORT chanflag){ return 0; }
BTI1553API BOOL __stdcall BTI1553_ModeIsMon(USHORT chanflag){ return 0; }
BTI1553API BOOL __stdcall BTI1553_ModeIsNotBC(USHORT chanflag){ return 0; }
BTI1553API BOOL __stdcall BTI1553_ModeIsNotMon(USHORT chanflag){ return 0; }
BTI1553API BOOL __stdcall BTI1553_ModeIsNotPlay(USHORT chanflag){ return 0; }
BTI1553API BOOL __stdcall BTI1553_ModeIsNotRT(USHORT chanflag){ return 0; }
BTI1553API BOOL __stdcall BTI1553_ModeIsNotSerial(USHORT chanflag){ return 0; }
BTI1553API BOOL __stdcall BTI1553_ModeIsPlay(USHORT chanflag){ return 0; }
BTI1553API BOOL __stdcall BTI1553_ModeIsRT(USHORT chanflag){ return 0; }
BTI1553API BOOL __stdcall BTI1553_ModeIsSerial(USHORT chanflag){ return 0; }
BTI1553API ERRVAL __stdcall BTI1553_MonConfigEx(ULONG configval,ULONG moncount,USHORT cardnum,INT channum,HCORE handleval){ return 0; }
BTI1553API ERRVAL __stdcall BTI1553_MonFilterSAEx(INT taval,ULONG rcvsamask,ULONG xmtsamask,ULONG rcvmcmask,ULONG xmtmcmask,INT channum,HCORE handleval){ return 0; }
BTI1553API USHORT __stdcall BTI1553_MsgCheck(MSGADDR msgaddr,HCORE handleval){ return 0; }
BTI1553API MSGADDR __stdcall BTI1553_MsgCommRd(LPMSGFIELDS1553 buf,MSGADDR msgaddr,HCORE handleval){ return 0; }
BTI1553API MSGADDR __stdcall BTI1553_MsgCommWr(LPMSGFIELDS1553 buf,MSGADDR msgaddr,HCORE handleval){ return 0; }
BTI1553API MSGADDR __stdcall BTI1553_MsgConfig(ULONG configval,HCORE handleval){ return 0; }
BTI1553API MSGADDR __stdcall BTI1553_MsgConfigEx(ULONG configval,USHORT cwd,HCORE handleval){ return 0; }
BTI1553API USHORT __stdcall BTI1553_MsgCWD1Rd(MSGADDR msgaddr,HCORE handleval){ return 0; }
BTI1553API VOID __stdcall BTI1553_MsgCWD1Wr(USHORT cwd1,MSGADDR msgaddr,HCORE handleval){ }
BTI1553API USHORT __stdcall BTI1553_MsgCWD2Rd(MSGADDR msgaddr,HCORE handleval){ return 0; }
BTI1553API VOID __stdcall BTI1553_MsgCWD2Wr(USHORT cwd2,MSGADDR msgaddr,HCORE handleval){ }
BTI1553API USHORT __stdcall BTI1553_MsgErrorRd(MSGADDR msgaddr,HCORE handleval){ return 0; }
BTI1553API VOID __stdcall BTI1553_MsgErrorWr(USHORT value,MSGADDR msgaddr,HCORE handleval){  }
BTI1553API USHORT __stdcall BTI1553_MsgFlagRd(MSGADDR msgaddr,HCORE handleval){ return 0; }
BTI1553API VOID __stdcall BTI1553_MsgFlagWr(USHORT msgflag,MSGADDR msgaddr,HCORE handleval){  }
BTI1553API BOOL __stdcall BTI1553_MsgIsAccessed(MSGADDR msgaddr,HCORE handleval){ return 0; }
BTI1553API USHORT __stdcall BTI1553_MsgOptRd(MSGADDR msgaddr,HCORE handleval){ return 0; }
BTI1553API VOID __stdcall BTI1553_MsgOptWr(USHORT msgopt,MSGADDR msgaddr,HCORE handleval){  }
BTI1553API MSGADDR __stdcall BTI1553_MsgReset(MSGADDR msgaddr,HCORE handleval){ return 0; }
BTI1553API MSGADDR __stdcall BTI1553_MsgResetEx(MSGADDR msgaddr,HCORE handleval){ return 0; }
BTI1553API USHORT __stdcall BTI1553_MsgSWD1Rd(MSGADDR msgaddr,HCORE handleval){ return 0; }
BTI1553API USHORT __stdcall BTI1553_MsgSWD2Rd(MSGADDR msgaddr,HCORE handleval){ return 0; }
BTI1553API ERRVAL __stdcall BTI1553_PlayClear(INT channum,HCORE handleval){ return 0; }
BTI1553API ERRVAL __stdcall BTI1553_PlayConfigEx(ULONG configval,ULONG playcount,ULONG tamask,INT channum,HCORE handleval){ return 0; }
BTI1553API ULONG __stdcall BTI1553_PlayCounterWr(ULONG totalcount,INT channum,HCORE handleval){ return 0; }
BTI1553API INT __stdcall BTI1553_PlayStatusEx(LPULONG totalcount,INT channum,HCORE handleval){ return 0; }
BTI1553API USHORT __stdcall BTI1553_PlayWrEx(LPUSHORT buf,USHORT bufcount,INT channum,HCORE handleval){ return 0; }
BTI1553API ERRVAL __stdcall BTI1553_ProcSetPost(ULONG useraddr,ULONG msgaddr,INT channum,HCORE handleval){ return 0; }
BTI1553API ERRVAL __stdcall BTI1553_ProcSetPre(ULONG useraddr,ULONG msgaddr,INT channum,HCORE handleval){ return 0; }
BTI1553API ERRVAL __stdcall BTI1553_RTConfigEx(ULONG configval,INT taval,INT channum,HCORE handleval){ return 0; }
BTI1553API ERRVAL __stdcall BTI1553_RTConfigExx(ULONG configval,INT taval,INT channum,HCORE handleval){ return 0; }
BTI1553API LISTADDR __stdcall BTI1553_RTCreateListEx(ULONG listconfigval,INT skipval,INT count,ULONG msgconfigval,BOOL mcflag,INT taval,BOOL trflag,INT saval,INT channum,HCORE handleval){ return 0; }
BTI1553API LISTADDR __stdcall BTI1553_RTCreateListExx(ULONG listconfigval,INT skipval,INT count,ULONG msgconfigval,BOOL mcflag,INT taval,BOOL trflag,INT saval,INT channum,HCORE handleval){ return 0; }
BTI1553API ERRVAL __stdcall BTI1553_RTResponseTimeRd(LPUSHORT timeval,INT taval,INT channum,HCORE handleval){ return 0; }
BTI1553API ERRVAL __stdcall BTI1553_RTResponseTimeWr(USHORT timeval,INT taval,INT channum,HCORE handleval){ return 0; }
BTI1553API USHORT __stdcall BTI1553_RTStatus(INT taval,INT channum,HCORE handleval){ return 0; }
BTI1553API ERRVAL __stdcall BTI1553_SABlkWr(INT taval,INT saval,INT channum,HCORE handleval){ return 0; }
BTI1553API ERRVAL __stdcall BTI1553_SABlkWrEx(INT channum,HCORE handleval){ return 0; }
BTI1553API ERRVAL __stdcall BTI1553_SchedBuild(INT nummsgs,LPMSGADDR msgaddr,LPINT freq,INT channum,HCORE handleval){ return 0; }
BTI1553API ERRVAL __stdcall BTI1553_SchedBuildEx(INT nummsgs,LPMSGADDR msgaddr,LPINT freq,LPINT period,LPVOID buf,ULONG bufsize,INT channum,HCORE handleval){ return 0; }
BTI1553API ERRVAL __stdcall BTI1553_SchedBuildExx(ULONG frametime,INT nummsgs,LPMSGADDR msgaddr,LPINT period,LPVOID buf,ULONG bufsize,INT channum,HCORE handleval){ return 0; }
BTI1553API ERRVAL __stdcall BTI1553_SchedCalc(LPMSGADDR msgaddr,INT channum,HCORE handleval){ return 0; }
BTI1553API VOID __stdcall BTI1553_SchedDisplay(ULONG nummsgs,LPULONG count){  }
BTI1553API VOID __stdcall BTI1553_SchedHook(INT index,LPVOID ptr){  }
BTI1553API ERRVAL __stdcall BTI1553_SchedInsEnd(INT channum,HCORE handleval){ return 0; }
BTI1553API ERRVAL __stdcall BTI1553_SchedInsMsg(ULONG index,LPMSGADDR msgaddr,INT channum,HCORE handleval){ return 0; }
BTI1553API ERRVAL __stdcall BTI1553_SchedInsStart(ULONG frametime,ULONG count,INT channum,HCORE handleval){ return 0; }
BTI1553API ULONG __stdcall BTI1553_SchedMaxval(LPULONG array,ULONG arraycount){ return 0; }
BTI1553API ULONG __stdcall BTI1553_SchedMinval(LPULONG array,ULONG arraycount){ return 0; }
BTI1553API VOID __stdcall BTI1553_SchedPrint(LPSTR str,...){  }
BTI1553API ERRVAL __stdcall BTI1553_SerialClear(INT channum,HCORE handleval){ return 0; }
BTI1553API ERRVAL __stdcall BTI1553_SerialConfig(ULONG configval,USHORT count,INT channum,HCORE handleval){ return 0; }
BTI1553API ERRVAL __stdcall BTI1553_SerialConfigEx(ULONG configval,USHORT count,INT channum,HCORE handleval){ return 0; }
BTI1553API INT __stdcall BTI1553_SerialStatus(INT channum,HCORE handleval){ return 0; }
BTI1553API ULONG __stdcall BTI1553_SerialWr(LPUSHORT buf,INT channum,HCORE handleval){ return 0; }
BTI1553API ERRVAL __stdcall BTI1553_TestProtocol(HCORE handleval){ return 0; }
BTI1553API USHORT __stdcall BTI1553_ValPackRTCWD(INT MCflag,INT TAval,INT TRflag,INT SAval,INT count){ return 0; }
BTI1553API VOID __stdcall BTI1553_ValUnpackRTCWD(USHORT cwd,LPINT BCASTflag,LPINT MCflag,LPINT TAval,LPINT TRflag,LPINT SAval,LPINT count){  }

#ifdef __cplusplus
}
#endif

