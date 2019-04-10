#ifdef BTICardAPI
#undef BTICardAPI
#endif
#define BTICardAPI __declspec(dllexport)

#ifdef _WIN32
#undef _WIN32
#endif
#include <BTICARD.h>
#include <BTI1553.H>
#define _WIN32

#include "omnibus_rpc.h"

#include <windows.h>
#include "rpc_ports.h"

#include "BufferClass.h"
#include "instruments.h"
#include <QApplication>

// Объявляем функцию DllMain
BOOL APIENTRY DllMain(HINSTANCE hinstDLL,
DWORD fdwReason, LPVOID lpvReserved)
{
	RPC_omnibus_SLOT_Thread& slot_thr(Srpc_buffer_class::Instance().omnibus_slot_thr);
	RPC_omnibus_SIGNAL_Thread& signal_thr(Srpc_buffer_class::Instance().omnibus_signal_thr);

	//получение айпи
	/*QString tmp_srvr_ip;
	QString ipSettingsFile = QString(QCoreApplication::applicationDirPath() + "/" + "ipSettings.ini");
	QSettings *ipSettings = new QSettings(ipSettingsFile, QSettings::IniFormat, NULL);
	ipSettings->beginGroup("IP");*/
	QString ip_str = instr::GetIpFromSettings("rpc_omnibus") ;//ipSettings->value(QString("rpc_omnibus"), "").toString();
	//ipSettings->endGroup();

switch (fdwReason)      // Дерево разбора уведомлений
{
case DLL_PROCESS_ATTACH: // Подключение DLL
	if (!slot_thr.isRunning())
	{
		slot_thr.set_connection_params(ip_str, OMNIBUS_SLOT);
		slot_thr.start();
	}
	//if (!slot_thr.wait_connected(3))
	//	return false;
	if (!signal_thr.isRunning())
	{
		signal_thr.set_connection_params(ip_str, OMNIBUS_SIGNAL);
		signal_thr.start();
		signal_thr.wait_connected(3);
	}
	


break; // успешная инициализация

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

break;

}
return TRUE;    // Код возврата игнорируется
}


#ifdef __cplusplus
extern "C" {
#endif




/**
*
*  BTICard Driver functions.
*
**/

BTICardAPI VOID __stdcall BTICard_Add64(LPULONG resulth,LPULONG resultl,ULONG valah,ULONG valal,ULONG valbh,ULONG valbl){ }
BTICardAPI ULONG __stdcall BTICard_AddrDSP(ULONG addr,HCARD handleval){ return 0; }
BTICardAPI ULONG __stdcall BTICard_AddrHost(ULONG addr,HCARD handleval){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_AsciiToMant(LPCSTR str,LPULONG mant,LPINT exp){ return 0; }
BTICardAPI ULONG __stdcall BTICard_BCDToBin(ULONG bcdval,INT msb,INT lsb){ return 0; }
BTICardAPI ULONG __stdcall BTICard_BinToBCD(ULONG oldbcdval,ULONG binval,INT msb,INT lsb){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_BootAdd(LPCSTR filename,LPCSTR name,HCORE handleval){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_BootAddr(INT index,LPULONG addr,HCORE handleval){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_BootAddTiEx(LPCSTR filename,HCORE handleval){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_BootCount(LPUSHORT count,HCORE handleval){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_BootDelete(USHORT index,HCORE handleval){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_BootGet(USHORT index,LPCSTR filename,HCORE handleval){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_BootInfo(USHORT index,LPUSHORT base,LPUSHORT length,LPSTR name,USHORT namelen,LPULONG crc,HCORE handleval){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_BootPtrGet(LPUSHORT index,HCORE handleval){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_BootPtrGetTiEx(LPUSHORT value,HCORE handleval){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_BootPtrPut(USHORT index,HCORE handleval){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_BootPtrPutTiEx(USHORT value,HCORE handleval){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_BootRdW(LPUSHORT value,ULONG addr,HCORE handleval){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_BootStr(USHORT index,LPSTR keystr,LPSTR buf,USHORT bufcount,HCORE handleval){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_BootWipe(HCORE handleval){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_BootWipeTiEx(HCORE handleval){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_BootWrW(USHORT value,ULONG addr,HCORE handleval){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_CardClose(HCARD handleval){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_CardCloseAll(VOID){ return 0; }
BTICardAPI ULONG __stdcall BTICard_CardGetInfo(USHORT infotype,INT channum,HCORE handleval){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_CardGetInfoEx(LPUSHORT bufmodel,USHORT bufmodelcount,LPUSHORT buffeature,USHORT buffeaturecount,HCORE handleval){ return 0; }
BTICardAPI BOOL __stdcall BTICard_CardIsRunning(HCORE handleval){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_CardNop(HCARD handleval){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_CardOpen(LPHCARD lpHandle, INT cardnum){ *lpHandle = 0; return 0; }
BTICardAPI ERRVAL __stdcall BTICard_CardOpenStr(LPHCARD lpHandle,LPCSTR cardstr){ return 0; }
BTICardAPI LPCSTR __stdcall BTICard_CardProductStr(HCORE handleval){ return 0; }
BTICardAPI VOID __stdcall BTICard_CardReset(HCORE handleval){  }
BTICardAPI VOID __stdcall BTICard_CardResetEx(HCORE handleval){  }
BTICardAPI ERRVAL __stdcall BTICard_CardResume(HCORE handleval){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_CardShadow(BOOL shadowval,HCORE handleval){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_CardStart(HCORE handleval){ return 0; }
BTICardAPI BOOL __stdcall BTICard_CardStop(HCORE handleval){ return 0; }
BTICardAPI VOID __stdcall BTICard_CardSyncEnable(BOOL enableflag,USHORT syncmask,USHORT pinpolarity,HCORE handleval){ }
BTICardAPI USHORT __stdcall BTICard_CardSyncValid(HCORE handleval){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_CardTest(USHORT level,HCORE handleval){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_CardTest0(HCORE handleval){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_CardTest1(HCORE handleval){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_CardTest2(HCORE handleval){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_CardTest3(HCORE handleval){ return 0; }
BTICardAPI VOID __stdcall BTICard_CardTrigger(HCORE handleval){ }
BTICardAPI VOID __stdcall BTICard_CardTriggerEnable(BOOL enableflag,HCORE handleval){ }
BTICardAPI VOID __stdcall BTICard_CardTriggerEnableEx(BOOL enableflag,USHORT trigmask,USHORT pinpolarity,HCORE handleval){  }
BTICardAPI VOID __stdcall BTICard_CardTriggerEx(USHORT trigmask,HCORE handleval){  }
BTICardAPI USHORT __stdcall BTICard_CardTriggerValid(HCORE handleval){ return 0; }
BTICardAPI LPCSTR __stdcall BTICard_CardTypeStr(HCORE handleval){ return 0; }
BTICardAPI VOID __stdcall BTICard_ChDARClr(USHORT maskval,USHORT addrval,INT channum,HCORE handleval){ }
BTICardAPI BOOL __stdcall BTICard_ChDARGet(USHORT maskval,USHORT addrval,INT channum,HCORE handleval){ return 0; }
BTICardAPI ULONG __stdcall BTICard_ChDARRdL(USHORT addrval,INT channum,HCORE handleval){ return 0; }
BTICardAPI VOID __stdcall BTICard_ChDARRdsW(LPUSHORT valueptr,USHORT addrval,INT countval,INT channum,HCORE handleval){  }
BTICardAPI USHORT __stdcall BTICard_ChDARRdW(USHORT addrval,INT channum,HCORE handleval){ return 0; }
BTICardAPI VOID __stdcall BTICard_ChDARSet(USHORT maskval,USHORT addrval,INT channum,HCORE handleval){ }
BTICardAPI VOID __stdcall BTICard_ChDARWrL(ULONG value,USHORT addrval,INT channum,HCORE handleval){  }
BTICardAPI VOID __stdcall BTICard_ChDARWrsW(LPUSHORT valueptr,USHORT addrval,INT countval,INT channum,HCORE handleval){ }
BTICardAPI VOID __stdcall BTICard_ChDARWrW(USHORT value,USHORT addrval,INT channum,HCORE handleval){  }
BTICardAPI ERRVAL __stdcall BTICard_CISRd(LPUSHORT buf,USHORT bufcount,INT cistype,HCORE handleval){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_CISWr(LPUSHORT buf,USHORT bufcount,INT cistype,HCORE handleval){ return 0; }
BTICardAPI USHORT __stdcall BTICard_CommBufRd(USHORT offset,HCORE handleval){ return 0; }
BTICardAPI VOID __stdcall BTICard_CommBufWr(USHORT value,USHORT offset,HCORE handleval){ }
BTICardAPI ERRVAL __stdcall BTICard_CommCall(ULONG addrval,HCORE handleval){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_CommChannelReconfig(ULONG chmask,HCORE handleval){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_CommCheck(HCORE handleval){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_CommDisable(USHORT command,HCORE handleval){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_CommDisableEx(USHORT command,HCORE handleval){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_CommEnable(HCORE handleval){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_CommExternSRQ(ULONG chmask,HCORE handleval){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_CommFillW(USHORT value,ULONG addrval,USHORT count,HCORE handleval){ return 0; }
BTICardAPI BOOL __stdcall BTICard_CommProtocolFunc(USHORT opcode,USHORT argcount,LPUSHORT argbuf,HCORE handleval){ return 0; }
BTICardAPI ULONG __stdcall BTICard_CommRdL(ULONG addrval,HCORE handleval){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_CommRdsW(LPUSHORT valueptr,ULONG addrval,USHORT count,HCORE handleval){ return 0; }
BTICardAPI USHORT __stdcall BTICard_CommRdW(ULONG addrval,HCORE handleval){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_CommRun(LPUSHORT dataptr,USHORT datacount,LPUSHORT codeptr,USHORT codecount,HCORE handleval){ return 0; }
BTICardAPI VOID __stdcall BTICard_CommWrL(ULONG value,ULONG addrval,HCORE handleval){  }
BTICardAPI ERRVAL __stdcall BTICard_CommWrsW(LPUSHORT valueptr,ULONG addrval,USHORT count,HCORE handleval){ return 0; }
BTICardAPI VOID __stdcall BTICard_CommWrW(USHORT value,ULONG addrval,HCORE handleval){ }
BTICardAPI ERRVAL __stdcall BTICard_CoProcCheck(HCARD handleval){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_CoProcGetInfo(LPULONG valueptr,USHORT infotype,HCARD handleval){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_CoProcMemRdL(LPULONG valueptr,ULONG addrval,HCARD handleval){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_CoProcMemRdsL(LPULONG valueptr,ULONG addrval,ULONG count,HCARD handleval){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_CoProcMemRdsW(LPUSHORT valueptr,ULONG addrval,ULONG count,HCARD handleval){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_CoProcMemRdW(LPUSHORT valueptr,ULONG addrval,HCARD handleval){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_CoProcMemWrL(ULONG value,ULONG addrval,HCARD handleval){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_CoProcMemWrsL(LPULONG valueptr,ULONG addrval,ULONG count,HCARD handleval){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_CoProcMemWrsW(LPUSHORT valueptr,ULONG addrval,ULONG count,HCARD handleval){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_CoProcMemWrW(USHORT value,ULONG addrval,HCARD handleval){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_CoreOpen(LPHCORE lphCore, INT corenum, HCARD hCard){ *lphCore = corenum; return 0; }
BTICardAPI VOID __stdcall BTICard_DARClr(USHORT maskval,USHORT addrval,HCORE handleval){  }
BTICardAPI BOOL __stdcall BTICard_DARGet(USHORT maskval,USHORT addrval,HCORE handleval){ return 0; }
BTICardAPI ULONG __stdcall BTICard_DARRdL(USHORT addrval,HCORE handleval){ return 0; }
BTICardAPI VOID __stdcall BTICard_DARRdsW(LPUSHORT valueptr,USHORT addrval,INT countval,HCORE handleval){  }
BTICardAPI USHORT __stdcall BTICard_DARRdW(USHORT addrval,HCORE handleval){ return 0; }
BTICardAPI VOID __stdcall BTICard_DARSet(USHORT maskval,USHORT addrval,HCORE handleval){  }
BTICardAPI VOID __stdcall BTICard_DARWrL(ULONG value,USHORT addrval,HCORE handleval){  }
BTICardAPI VOID __stdcall BTICard_DARWrsW(LPUSHORT valueptr,USHORT addrval,INT countval,HCORE handleval){  }
BTICardAPI VOID __stdcall BTICard_DARWrW(USHORT value,USHORT addrval,HCORE handleval){  }
BTICardAPI ULONG __stdcall BTICard_Div(ULONG diva,ULONG divb){ return 0; }
BTICardAPI VOID __stdcall BTICard_Div64(LPULONG resulth,LPULONG resultl,ULONG valah,ULONG valal,ULONG valbh,ULONG valbl){  }
BTICardAPI INT __stdcall BTICard_DllUsageCount(VOID){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_DspBioClear(HCORE handleval){ return 0; }
BTICardAPI BOOL __stdcall BTICard_DspBioRd(HCORE handleval){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_DspBioSet(HCORE handleval){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_DspIntfClear(USHORT intmask,HCORE handleval){ return 0; }
BTICardAPI BOOL __stdcall BTICard_DspIntfRd(USHORT intmask,HCORE handleval){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_DspIntmClear(HCORE handleval){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_DspIntmSet(HCORE handleval){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_DspXfClear(HCORE handleval){ return 0; }
BTICardAPI BOOL __stdcall BTICard_DspXfRd(HCORE handleval){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_DspXfSet(HCORE handleval){ return 0; }
BTICardAPI LPCSTR __stdcall BTICard_ErrDesc(ERRVAL errval,HCARD handleval){ return 0; }
BTICardAPI LPCSTR __stdcall BTICard_ErrDescStr(ERRVAL errval,HCORE handleval){ return "omnibus - заглушка"; }
BTICardAPI LPCSTR __stdcall BTICard_ErrName(ERRVAL errval,HCARD handleval){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_EventLogClear(HCORE handleval){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_EventLogConfig(USHORT configval,USHORT count,HCORE handleval){ return 0; }
BTICardAPI ULONG __stdcall BTICard_EventLogRd(LPUSHORT typeval,LPULONG infoval,LPINT channel,HCORE handleval){ return 0; }
BTICardAPI INT __stdcall BTICard_EventLogStatus(HCORE handleval){ return 0; }
BTICardAPI VOID __stdcall BTICard_ExpandMant(LPULONG mant,LPINT exp){  }
BTICardAPI BOOL __stdcall BTICard_ExtDinRd(HCORE handleval){ return 0; }
BTICardAPI VOID __stdcall BTICard_ExtDinWr(BOOL dinval,HCORE handleval){ }
BTICardAPI VOID __stdcall BTICard_ExtDIODirSet(INT dionum,BOOL dirval,HCORE handleval){  }
BTICardAPI ERRVAL __stdcall BTICard_ExtDIOMonConfig(USHORT rise_edge,USHORT fall_edge,INT banknum,HCORE handleval){ return 0; }
BTICardAPI BOOL __stdcall BTICard_ExtDIORd(INT dionum,HCORE handleval){ return 0; }
BTICardAPI VOID __stdcall BTICard_ExtDIOWr(INT dionum,BOOL dioval,HCORE handleval){ }
BTICardAPI BOOL __stdcall BTICard_ExtLEDRd(HCORE handleval){ return 0; }
BTICardAPI VOID __stdcall BTICard_ExtLEDWr(BOOL ledval,HCORE handleval){  }
BTICardAPI VOID __stdcall BTICard_ExtStatusLEDRd(LPINT ledon,LPINT ledcolor,HCORE handleval){  }
BTICardAPI VOID __stdcall BTICard_ExtStatusLEDWr(BOOL ledon,BOOL ledcolor,HCORE handleval){  }
BTICardAPI VOID __stdcall BTICard_FileClose(LPVOID handle){ }
BTICardAPI BOOL __stdcall BTICard_FileErr(VOID){ return 0; }
BTICardAPI VOID __stdcall BTICard_FileErrClr(VOID){  }
BTICardAPI VOID __stdcall BTICard_FileErrSet(VOID){  }
BTICardAPI LPVOID __stdcall BTICard_FileOpenRead(LPCSTR filename){ return 0; }
BTICardAPI LPVOID __stdcall BTICard_FileOpenWrite(LPCSTR filename){ return 0; }
BTICardAPI BOOL __stdcall BTICard_FileRead(LPVOID handle,LPVOID buffer,USHORT count){ return 0; }
BTICardAPI BOOL __stdcall BTICard_FileSeek(LPVOID handle,ULONG offset){ return 0; }
BTICardAPI ULONG __stdcall BTICard_FileTell(LPVOID handle){ return 0; }
BTICardAPI BOOL __stdcall BTICard_FileWrite(LPVOID handle,LPVOID buffer,USHORT count){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_FPGAAdd(LPCSTR filename,HCORE handleval){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_FPGAInfoRd(LPUSHORT buf,HCORE handleval){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_FPGAWipe(HCORE handleval){ return 0; }
BTICardAPI USHORT __stdcall BTICard_GetHigh(ULONG val){ return 0; }
BTICardAPI USHORT __stdcall BTICard_GetLow(ULONG val){ return 0; }
BTICardAPI USHORT __stdcall BTICard_GlobalRdW(USHORT addrval,HCORE handleval){ return 0; }
BTICardAPI VOID __stdcall BTICard_GlobalWrW(USHORT value,USHORT addrval,HCORE handleval){ }
BTICardAPI ERRVAL __stdcall BTICard_HandleInfo(LPSTR cardstr,LPINT cardnum,LPULONG sizval,LPVOID *vxdptr,HCORE handleval){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_HandleInfoEx(LPULONG valueptr,ULONG type,HCORE handleval){ return 0; }
BTICardAPI BOOL __stdcall BTICard_HandleIsCard(HCARD handleval){ return 0; }
BTICardAPI BOOL __stdcall BTICard_HandleIsCore(HCORE handleval){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_HandleMakeCard(LPHCARD lphCard,LPINT lpcorenum,HCORE hCore){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_HandleMakeCore(LPHCORE lphCore,INT corenum,HCARD hCard){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_HandleMakeRPC(LPHCARD hCard_Remote,LPHRPC lphRPC,HCARD handleval){ return 0; }
BTICardAPI BOOL __stdcall BTICard_HandleOkay(HCARD handleval){ return 0; }
BTICardAPI ULONG __stdcall BTICard_HeapAlloc(INT section,ULONG wordcount,HCORE handleval){ return 0; }
BTICardAPI ULONG __stdcall BTICard_HeapAllocAll(INT section,LPULONG wordcount,HCORE handleval){ return 0; }
BTICardAPI ULONG __stdcall BTICard_HeapAllocEx(USHORT configval,INT section,ULONG wordcount,HCORE handleval){ return 0; }
BTICardAPI ULONG __stdcall BTICard_HeapWipe(INT section,HCORE handleval){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_HexRd(LPCSTR fname,ERRVAL (*proc)(USHORT value,USHORT section,ULONG addr,LPVOID lpParam),LPVOID lpParam){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_HexRun(LPCSTR fname,HCORE handleval){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_HexRunIO(LPCSTR fname,HCARD handleval){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_HexRunIO16(LPCSTR fname,HCARD handleval){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_HexRunSerial(LPCSTR fname,HCARD handleval){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_HexStat(LPULONG lowaddr,LPULONG highaddr,LPULONG count,LPCSTR fname){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_HexWrAddr(USHORT addr,HCARD handleval){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_HexWrByte(BYTE ch,HCARD handleval){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_HexWrClose(HCARD handleval){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_HexWrFlush(HCARD handleval){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_HexWrFlushByte(USHORT value,HCARD handleval){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_HexWrOpen(LPCSTR fname,HCARD handleval){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_HexWrPage(USHORT pageval,HCARD handleval){ return 0; }
BTICardAPI VOID __stdcall BTICard_HookProtocolFunc(INT index,ERRVAL (__stdcall * ptr)(INT msgval,LPVOID lpParam,HCARD handleval)){  }
BTICardAPI VOID __stdcall BTICard_HPIFill(USHORT value,USHORT addrval,INT countval,HCORE handleval){  }
BTICardAPI ULONG __stdcall BTICard_HPIRdL(USHORT addrval,HCORE handleval){ return 0; }
BTICardAPI VOID __stdcall BTICard_HPIRdsL(LPULONG valueptr,USHORT addrval,INT countval,HCORE handleval){ }
BTICardAPI VOID __stdcall BTICard_HPIRdsW(LPUSHORT valueptr,USHORT addrval,INT countval,HCORE handleval){  }
BTICardAPI USHORT __stdcall BTICard_HPIRdW(USHORT addrval,HCORE handleval){ return 0; }
BTICardAPI VOID __stdcall BTICard_HPIWrL(ULONG value,USHORT addrval,HCORE handleval){  }
BTICardAPI VOID __stdcall BTICard_HPIWrsL(LPULONG valueptr,USHORT addrval,INT countval,HCORE handleval){ }
BTICardAPI VOID __stdcall BTICard_HPIWrsW(LPUSHORT valueptr,USHORT addrval,INT countval,HCORE handleval){  }
BTICardAPI VOID __stdcall BTICard_HPIWrW(USHORT value,USHORT addrval,HCORE handleval){  }
BTICardAPI USHORT __stdcall BTICard_IDRegRd(INT gate_array_num,HCORE handleval){ return 0; }
BTICardAPI VOID __stdcall BTICard_IDRegWr(USHORT value,INT gate_array_num,HCORE handleval){  }
BTICardAPI VOID __stdcall BTICard_IntClear(HCORE handleval){  }
BTICardAPI ERRVAL __stdcall BTICard_IntDisable(HCORE handleval){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_IntEnable(HCORE handleval){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_IntEnableCond(HCORE handleval){ return 0; }
BTICardAPI LPVOID __stdcall BTICard_IntGet(HCORE handleval){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_IntInstall(LPVOID hEvent,HCORE handleval){ return 0; }
BTICardAPI VOID __stdcall BTICard_IntReset(HCORE handleval){ }
BTICardAPI ERRVAL __stdcall BTICard_IntUninstall(HCORE handleval){ return 0; }
BTICardAPI ULONG __stdcall BTICard_IORdL(INT addrval,HCORE handleval){ return 0; }
BTICardAPI USHORT __stdcall BTICard_IORdW(INT addrval,HCORE handleval){ return 0; }
BTICardAPI USHORT __stdcall BTICard_IOWINRdW(INT addrval,HCORE handleval){ return 0; }
BTICardAPI VOID __stdcall BTICard_IOWINWrW(USHORT value,INT addrval,HCORE handleval){ }
BTICardAPI VOID __stdcall BTICard_IOWrL(ULONG value,INT addrval,HCORE handleval){  }
BTICardAPI VOID __stdcall BTICard_IOWrW(USHORT value,INT addrval,HCORE handleval){  }
BTICardAPI ERRVAL __stdcall BTICard_IRIGConfig(ULONG configval,HCORE handleval){ return 0; }
BTICardAPI ULONG __stdcall BTICard_IRIGFieldGetDays(ULONG irigvalh,ULONG irigvall)
{
	unsigned long long tmp_timestamp;
	tmp_timestamp = irigvall + ((unsigned long long)irigvalh << 32);
	return QDateTime::fromMSecsSinceEpoch(tmp_timestamp / 1000).date().dayOfYear()-1;
}
BTICardAPI ULONG __stdcall BTICard_IRIGFieldGetHours(ULONG irigvalh,ULONG irigvall)
{
	unsigned long long tmp_timestamp;
	tmp_timestamp = irigvall + ((unsigned long long)irigvalh << 32);
	return QDateTime::fromMSecsSinceEpoch(tmp_timestamp / 1000).time().hour();
}
BTICardAPI ULONG __stdcall BTICard_IRIGFieldGetMicrosec(ULONG irigvalh,ULONG irigvall)
{
	unsigned long long tmp_timestamp;
	tmp_timestamp = irigvall + ((unsigned long long)irigvalh << 32);
	return tmp_timestamp % 1000;
}
BTICardAPI ULONG __stdcall BTICard_IRIGFieldGetMillisec(ULONG irigvalh,ULONG irigvall)
{
	unsigned long long tmp_timestamp;
	tmp_timestamp = irigvall + ((unsigned long long)irigvalh << 32);
	return QDateTime::fromMSecsSinceEpoch(tmp_timestamp / 1000).time().msec();
}
BTICardAPI ULONG __stdcall BTICard_IRIGFieldGetMin(ULONG irigvalh,ULONG irigvall)
{
	unsigned long long tmp_timestamp;
	tmp_timestamp = irigvall + ((unsigned long long)irigvalh << 32);
	return QDateTime::fromMSecsSinceEpoch(tmp_timestamp / 1000).time().minute();
}
BTICardAPI ULONG __stdcall BTICard_IRIGFieldGetSec(ULONG irigvalh,ULONG irigvall)
{
	unsigned long long tmp_timestamp;
	tmp_timestamp = irigvall + ((unsigned long long)irigvalh << 32);
	return QDateTime::fromMSecsSinceEpoch(tmp_timestamp / 1000).time().second();
}

BTICardAPI VOID __stdcall BTICard_IRIGFieldPutDays(ULONG value,LPULONG irigvalh,LPULONG irigvall){  }
BTICardAPI VOID __stdcall BTICard_IRIGFieldPutHours(ULONG value,LPULONG irigvalh,LPULONG irigvall){  }
BTICardAPI VOID __stdcall BTICard_IRIGFieldPutMicrosec(ULONG value,LPULONG irigvalh,LPULONG irigvall){ }
BTICardAPI VOID __stdcall BTICard_IRIGFieldPutMillisec(ULONG value,LPULONG irigvalh,LPULONG irigvall){  }
BTICardAPI VOID __stdcall BTICard_IRIGFieldPutMin(ULONG value,LPULONG irigvalh,LPULONG irigvall){ }
BTICardAPI VOID __stdcall BTICard_IRIGFieldPutSec(ULONG value,LPULONG irigvalh,LPULONG irigvall){  }
BTICardAPI ERRVAL __stdcall BTICard_IRIGInputThresholdGet(LPUSHORT dacval,HCORE handleval){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_IRIGInputThresholdSet(USHORT dacval,HCORE handleval){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_IRIGRd(LPBTIIRIGTIME irigtime,HCORE handleval)
{
	QTime tmp_time = QTime::currentTime();
	irigtime->hours = tmp_time.hour();
	irigtime->min = tmp_time.minute();
	irigtime->sec = tmp_time.second();
	irigtime->msec = tmp_time.msec();
	irigtime->usec = 0;
	return 0; 
}
BTICardAPI ERRVAL __stdcall BTICard_IRIGRdEx(LPUSHORT timebuf,HCORE handleval){ return 0; }
BTICardAPI BOOL __stdcall BTICard_IRIGSyncStatus(HCORE handleval){ return 0; }
BTICardAPI VOID __stdcall BTICard_IRIGTimeBCDToBin(LPULONG timevalh,LPULONG timevall,ULONG irigvalh,ULONG irigvall){ }
BTICardAPI VOID __stdcall BTICard_IRIGTimeBinToBCD(LPULONG irigvalh,LPULONG irigvall,ULONG timevalh,ULONG timevall){  }
BTICardAPI ERRVAL __stdcall BTICard_IRIGWr(LPBTIIRIGTIME irigtime,HCORE handleval){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_IRIGWrEx(LPUSHORT timebuf,HCORE handleval){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_KernIntStatus(LPINT valueptr,HCARD handleval){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_KernStatus(LPULONG valueptr,ULONG type,HCARD handleval){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_KernStatusEx(LPULONG valueptr,ULONG type,ULONG index,HCARD handleval){ return 0; }
BTICardAPI ULONG __stdcall BTICard_MakeLong(USHORT valh,USHORT vall){ return 0; }
BTICardAPI USHORT __stdcall BTICard_MakeWord(BYTE valh,BYTE vall){ return 0; }
BTICardAPI LPSTR __stdcall BTICard_MantToAscii(LPSTR buf,INT mant,INT exp){ return 0; }
BTICardAPI ULONG __stdcall BTICard_Mask(ULONG dataval,USHORT cntval){ return 0; }
BTICardAPI VOID __stdcall BTICard_MaxMant(LPULONG mant,LPINT exp){  }
BTICardAPI ULONG __stdcall BTICard_Mod(ULONG moda,ULONG modb){ return 0; }
BTICardAPI VOID __stdcall BTICard_Mod64(LPULONG resulth,LPULONG resultl,ULONG valah,ULONG valal,ULONG valbh,ULONG valbl){ }
BTICardAPI ULONG __stdcall BTICard_Mul(ULONG mula,ULONG mulb){ return 0; }
BTICardAPI VOID __stdcall BTICard_Mul64(LPULONG resulth,LPULONG resultl,ULONG valah,ULONG valal,ULONG valbh,ULONG valbl){  }
BTICardAPI VOID __stdcall BTICard_NormalMant(LPULONG mant,LPINT exp){ }
BTICardAPI USHORT __stdcall BTICard_PortRd(INT addrval,HCORE handleval){ return 0; }
BTICardAPI VOID __stdcall BTICard_PortWr(USHORT value,INT addrval,HCORE handleval){ }
BTICardAPI ERRVAL __stdcall BTICard_ProcCall(ULONG useraddr,HCORE handleval){ return 0; }
BTICardAPI ULONG __stdcall BTICard_ProcLoad(LPUSHORT userbuf,USHORT count,HCORE handleval){ return 0; }
BTICardAPI VOID __stdcall BTICard_ProcLoc(LPUSHORT inputcode,LPUSHORT outputcode,USHORT inputbase,USHORT outputbase,INT count){  }
BTICardAPI ERRVAL __stdcall BTICard_ProcRun(LPVOID userbuf,USHORT count,HCORE handleval){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_ProcSetFore(ULONG useraddr,HCORE handleval){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_ProcSetInt(USHORT useraddr,INT intnum,USHORT intmask,HCORE handleval){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_ProcSetPost(ULONG useraddr,ULONG msgaddr,INT channum,HCORE handleval){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_ProcSetPre(ULONG useraddr,ULONG msgaddr,INT channum,HCORE handleval){ return 0; }
BTICardAPI USHORT __stdcall BTICard_ProgRdW(ULONG addrval,HCORE handleval){ return 0; }
BTICardAPI VOID __stdcall BTICard_ProgWrW(USHORT value,ULONG addrval,HCORE handleval){  }
BTICardAPI VOID __stdcall BTICard_RAMFill(USHORT value,ULONG addrval,ULONG countval,HCORE handleval){  }
BTICardAPI USHORT __stdcall BTICard_RAMRdB(ULONG addrval,HCORE handleval){ return 0; }
BTICardAPI ULONG __stdcall BTICard_RAMRdL(ULONG addrval,HCORE handleval){ return 0; }
BTICardAPI VOID __stdcall BTICard_RAMRdmL(LPULONG valueptr,LPULONG addrptr,INT countval,HCORE handleval){ }
BTICardAPI VOID __stdcall BTICard_RAMRdmW(LPUSHORT valueptr,LPULONG addrptr,INT countval,HCORE handleval){  }
BTICardAPI VOID __stdcall BTICard_RAMRdsL(LPULONG valueptr,ULONG addrval,INT countval,HCORE handleval){  }
BTICardAPI VOID __stdcall BTICard_RAMRdsW(LPUSHORT valueptr,ULONG addrval,INT countval,HCORE handleval){  }
BTICardAPI USHORT __stdcall BTICard_RAMRdW(ULONG addrval,HCORE handleval){ return 0; }
BTICardAPI VOID __stdcall BTICard_RAMWipe(HCORE handleval){  }
BTICardAPI VOID __stdcall BTICard_RAMWipeEx(USHORT value,HCORE handleval){ }
BTICardAPI VOID __stdcall BTICard_RAMWrB(USHORT value,ULONG addrval,HCORE handleval){  }
BTICardAPI VOID __stdcall BTICard_RAMWrL(ULONG value,ULONG addrval,HCORE handleval){  }
BTICardAPI VOID __stdcall BTICard_RAMWrmL(LPULONG valueptr,LPULONG addrptr,INT countval,HCORE handleval){ }
BTICardAPI VOID __stdcall BTICard_RAMWrmW(LPUSHORT valueptr,LPULONG addrptr,INT countval,HCORE handleval){  }
BTICardAPI VOID __stdcall BTICard_RAMWrsL(LPULONG valueptr,ULONG addrval,INT countval,HCORE handleval){  }
BTICardAPI VOID __stdcall BTICard_RAMWrsW(LPUSHORT valueptr,ULONG addrval,INT countval,HCORE handleval){  }
BTICardAPI VOID __stdcall BTICard_RAMWrW(USHORT value,ULONG addrval,HCORE handleval){  }
BTICardAPI ULONG __stdcall BTICard_ReverseLong(ULONG value){ return 0; }
BTICardAPI USHORT __stdcall BTICard_ReverseWord(USHORT value){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_ROMClose(HCORE handleval){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_ROMFlush(HCORE handleval){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_ROMInfoRd(LPBYTE buf,HCORE handleval){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_ROMInfoWr(LPBYTE buf,HCORE handleval){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_ROMOpen(HCORE handleval){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_ROMRdsW(LPUSHORT valueptr,ULONG addrval,USHORT countval,HCORE handleval){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_ROMRdW(LPUSHORT valueptr,ULONG addrval,HCORE handleval){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_ROMWrsW(USHORT enableflag,LPUSHORT valueptr,ULONG addrval,USHORT countval,HCORE handleval){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_ROMWrW(USHORT value,ULONG addrval,HCORE handleval){ return 0; }
BTICardAPI ULONG __stdcall BTICard_SeqBlkRd(LPUSHORT buf, ULONG bufcount, LPULONG blkcnt, HCORE handleval){ return Srpc_buffer_class::Instance().get_msgs_size(); }
BTICardAPI ULONG __stdcall BTICard_SeqBlkRdEx(LPUSHORT buf,ULONG bufcount,ULONG maxblkcnt,LPULONG blkcnt,HCORE handleval){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_SeqClear(HCORE handleval){ return 0; }
BTICardAPI USHORT __stdcall BTICard_SeqCommRd(LPUSHORT buf, USHORT bufcount, HCORE handleval){ return Srpc_buffer_class::Instance().get_msgs_size(); }
BTICardAPI ERRVAL __stdcall BTICard_SeqConfig(ULONG configval,HCORE handleval)
{
	RPC_omnibus_SIGNAL_Thread& signal_thr(Srpc_buffer_class::Instance().omnibus_signal_thr);
	QObject::connect(signal_thr.get_obj().get(), SIGNAL(new_message(QVariant, int, int, int, QVariantList, int)), &Srpc_buffer_class::Instance(), SLOT(new_message(QVariant, int, int, int, QVariantList, int)));
	return 0;
}
BTICardAPI ERRVAL __stdcall BTICard_SeqConfigEx(ULONG configval,ULONG seqcount,USHORT cardnum,HCORE handleval){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_SeqConfigExx(ULONG configval,ULONG seqaddr,ULONG seqcount,USHORT cardnum,HCORE handleval){ return 0; }
BTICardAPI ULONG __stdcall BTICard_SeqDMARd(LPUSHORT buf, ULONG bufcount, HCORE handleval){ return Srpc_buffer_class::Instance().get_msgs_size(); }
BTICardAPI BOOL __stdcall BTICard_SeqFindCheckVersion(LPUSHORT pRecord,USHORT version){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_SeqFindInit(LPUSHORT seqbuf,ULONG seqbufsize,LPSEQFINDINFO sfinfo){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_SeqFindMore1553(LPSEQRECORDMORE1553 *pRecMore,LPSEQRECORD1553 pRecBase)
{
	*pRecMore = Srpc_buffer_class::Instance().get_more();
	return 0;
}
BTICardAPI ERRVAL __stdcall BTICard_SeqFindMore1553Ex(LPSEQRECORDMORE1553 pRecMore,USHORT recordsize,LPSEQRECORD1553 pRecBase){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_SeqFindNext(LPUSHORT *pRecord,LPUSHORT seqtype,LPSEQFINDINFO sfinfo){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_SeqFindNext1553(LPSEQRECORD1553 *pRecord,LPSEQFINDINFO sfinfo)
{
	if (Srpc_buffer_class::Instance().get_msgs_size() == 0)
		return -1;
	*pRecord = Srpc_buffer_class::Instance().get_msg();
	return 0; 
}
BTICardAPI ERRVAL __stdcall BTICard_SeqFindNext1553Ex(LPSEQRECORD1553 pRecord,USHORT recordsize,LPSEQFINDINFO sfinfo){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_SeqFindNext429(LPSEQRECORD429 *pRecord,LPSEQFINDINFO sfinfo){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_SeqFindNext429Ex(LPSEQRECORD429 pRecord,USHORT recordsize,LPSEQFINDINFO sfinfo){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_SeqFindNext708(LPSEQRECORD708 *pRecord,LPSEQFINDINFO sfinfo){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_SeqFindNext708Ex(LPSEQRECORD708 pRecord,USHORT recordsize,LPSEQFINDINFO sfinfo){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_SeqFindNext717(LPSEQRECORD717 *pRecord,LPSEQFINDINFO sfinfo){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_SeqFindNext717Ex(LPSEQRECORD717 pRecord,USHORT recordsize,LPSEQFINDINFO sfinfo){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_SeqFindNextCSDB(LPSEQRECORDCSDB *pRecord,LPSEQFINDINFO sfinfo){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_SeqFindNextCSDBEx(LPSEQRECORDCSDB pRecord,USHORT recordsize,LPSEQFINDINFO sfinfo){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_SeqFindNextDIO(LPSEQRECORDDIO *pRecord,LPSEQFINDINFO sfinfo){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_SeqFindNextDIOEx(LPSEQRECORDDIO pRecord,USHORT recordsize,LPSEQFINDINFO sfinfo){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_SeqFindNextEx(LPUSHORT pRecord,USHORT recordcount,LPUSHORT seqtype,LPSEQFINDINFO sfinfo){ return 0; }
BTICardAPI INT __stdcall BTICard_SeqInterval(INT interval,INT mode,HCORE handleval){ return 0; }
BTICardAPI USHORT __stdcall BTICard_SeqIntervalEx(USHORT shiftval,HCORE handleval){ return 0; }
BTICardAPI BOOL __stdcall BTICard_SeqIsRunning(HCORE handleval){ return 0; }
BTICardAPI USHORT __stdcall BTICard_SeqLogFrequency(USHORT logfreq,HCORE handleval){ return 0; }
BTICardAPI USHORT __stdcall BTICard_SeqRd(LPUSHORT buf,HCORE handleval){ return 0; }
BTICardAPI USHORT __stdcall BTICard_SeqRdEx(LPUSHORT buf,USHORT bufcount,HCORE handleval){ return 0; }
BTICardAPI BOOL __stdcall BTICard_SeqResume(HCORE handleval){ return 0; }
BTICardAPI BOOL __stdcall BTICard_SeqStart(HCORE handleval){ return 0; }
BTICardAPI BOOL __stdcall BTICard_SeqStatus(HCORE handleval){ return 0; }
BTICardAPI BOOL __stdcall BTICard_SeqStop(HCORE handleval){ return 0; }
BTICardAPI ULONG __stdcall BTICard_Shl(ULONG dataval,USHORT cntval){ return 0; }
BTICardAPI ULONG __stdcall BTICard_Shr(ULONG dataval,USHORT cntval){ return 0; }
BTICardAPI USHORT __stdcall BTICard_SignMant(LPCSTR str){ return 0; }
BTICardAPI VOID __stdcall BTICard_Sub64(LPULONG resulth,LPULONG resultl,ULONG valah,ULONG valal,ULONG valbh,ULONG valbl){  }
BTICardAPI VOID __stdcall BTICard_SwapEndianL(LPULONG value){  }
BTICardAPI VOID __stdcall BTICard_SwapEndianW(LPUSHORT valuea,LPUSHORT valueb){  }
BTICardAPI VOID __stdcall BTICard_SwapIfBigEndianL(LPULONG value){  }
BTICardAPI VOID __stdcall BTICard_SwapIfBigEndianW(LPUSHORT valuea,LPUSHORT valueb){  }
BTICardAPI VOID __stdcall BTICard_SwapIfLittleEndianL(LPULONG value){  }
BTICardAPI VOID __stdcall BTICard_SwapIfLittleEndianW(LPUSHORT valuea,LPUSHORT valueb){ }
BTICardAPI INT __stdcall BTICard_TickTimerStart(INT milliseconds){ return 0; }
BTICardAPI BOOL __stdcall BTICard_TickTimerValid(INT timer){ return 0; }
BTICardAPI ERRVAL __stdcall BTICard_Timer64Rd(LPULONG valueh,LPULONG valuel,HCORE handleval){ return 0; }
BTICardAPI VOID __stdcall BTICard_Timer64Wr(ULONG valueh,ULONG valuel,HCORE handleval){  }
BTICardAPI VOID __stdcall BTICard_TimerClear(HCORE handleval){  }
BTICardAPI ULONG __stdcall BTICard_TimerRd(HCORE handleval){ return 0; }
BTICardAPI INT __stdcall BTICard_TimerResolution(INT timerresol,HCORE handleval){ return 0; }
BTICardAPI USHORT __stdcall BTICard_TimerResolutionEx(USHORT timershift,HCORE handleval){ return 0; }
BTICardAPI INT __stdcall BTICard_TimerStatus(HCORE handleval){ return TIMETAG_FORMAT_BCD; }
BTICardAPI VOID __stdcall BTICard_TimerWr(ULONG value,HCORE handleval){ }
BTICardAPI INT __stdcall BTICard_ValAsciiCmpi(LPSTR str1,LPSTR str2){ return 0; }
BTICardAPI LPSTR __stdcall BTICard_ValAsciiCpy(LPSTR strdest,LPCSTR strsrc,INT count){ return 0; }
BTICardAPI VOID __stdcall BTICard_ValAsciiTrimLead(LPSTR buf){ }
BTICardAPI VOID __stdcall BTICard_ValAsciiTrimTrail(LPSTR buf){ }
BTICardAPI ULONG __stdcall BTICard_ValFromAscii(LPCSTR asciistr,INT radixval){ return 0; }
BTICardAPI ULONG __stdcall BTICard_ValGetBits(ULONG oldvalue,INT startbit,INT endbit){ return 0; }
BTICardAPI LPSTR __stdcall BTICard_ValIncAscii(LPSTR asciistr){ return 0; }
BTICardAPI LPSTR __stdcall BTICard_ValInccAscii(LPSTR asciistr){ return 0; }
BTICardAPI BOOL __stdcall BTICard_ValIsLower(INT value){ return 0; }
BTICardAPI INT __stdcall BTICard_ValLenAscii(INT numbits,INT radixval){ return 0; }
BTICardAPI ULONG __stdcall BTICard_ValPutBits(ULONG oldvalue,ULONG newfld,INT startbit,INT endbit){ return 0; }
BTICardAPI LPSTR __stdcall BTICard_ValToAscii(ULONG value,LPSTR asciistr,INT numbits,INT radixval){ return 0; }
BTICardAPI INT __stdcall BTICard_ValToUpper(INT value){ return 0; }
BTICardAPI ULONG __stdcall BTICard_VARRdL(USHORT addrval,HCORE handleval){ return 0; }
BTICardAPI VOID __stdcall BTICard_VARRdsW(LPUSHORT valueptr,USHORT addrval,INT countval,HCORE handleval){  }
BTICardAPI USHORT __stdcall BTICard_VARRdW(USHORT addrval,HCORE handleval){ return 0; }
BTICardAPI VOID __stdcall BTICard_VARWrL(ULONG value,USHORT addrval,HCORE handleval){ }
BTICardAPI VOID __stdcall BTICard_VARWrsW(LPUSHORT valueptr,USHORT addrval,INT countval,HCORE handleval){  }
BTICardAPI VOID __stdcall BTICard_VARWrW(USHORT value,USHORT addrval,HCORE handleval){  }
BTICardAPI ERRVAL __stdcall BTICard_XBitFileRd(LPCSTR fname,ERRVAL (*proc)(USHORT value,USHORT section,ULONG addr,LPVOID lpParam),LPVOID lpParam){ return 0; }

#ifdef __cplusplus
}
#endif
