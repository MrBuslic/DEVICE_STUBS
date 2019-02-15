/*---------------------------------------------------------------------------*/
/* Distributed by Informtest ltd.                                            */
/*                                                                           */
/* Do not modify the contents of this file.                                  */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/* Title   : VISA.H                                                          */
/* Date    : 09-12-2003                                                      */
/* Purpose : Include file for the VISA Library 3.0 specification             */
/*                                                                           */
/*---------------------------------------------------------------------------*/
#include <socket_rpc.h>
#include <Visa.h>
#include <windows.h>
#include <qcoreapplication.h>

#if defined(__cplusplus) || defined(__cplusplus__)
   extern "C" {
#endif

/*- Resource Manager Functions and Operations -------------------------------*/

ViStatus _VI_FUNC  viOpenDefaultRM (ViPSession vi)
{

	QString tt = QCoreApplication::applicationName();
	return 0;
}

ViStatus _VI_FUNC  viFindRsrc      (ViSession sesn, ViString expr, ViPFindList vi,
	ViPUInt32 retCnt, ViChar _VI_FAR desc[])
	{
		strcpy(desc, "VXI::1::INSTR");
		*retCnt = 1;
		return 0;
	}

ViStatus _VI_FUNC  viFindNext      (ViFindList vi, ViChar _VI_FAR desc[]){ return 0; }

ViStatus _VI_FUNC  viParseRsrc     (ViSession rmSesn, ViRsrc rsrcName,
                                    ViPUInt16 intfType, ViPUInt16 intfNum){ return 0; }

ViStatus _VI_FUNC  viParseRsrcEx   (ViSession rmSesn, ViRsrc rsrcName, ViPUInt16 intfType,
                                    ViPUInt16 intfNum, ViChar _VI_FAR rsrcClass[],
                                    ViChar _VI_FAR expandedUnaliasedName[],
                                    ViChar _VI_FAR aliasIfExists[]){ return 0; }

ViStatus _VI_FUNC  viOpen          (ViSession sesn, ViRsrc name, ViAccessMode mode,
	ViUInt32 timeout, ViPSession vi){
	*vi = 1; return 0;
}

/*- Resource Template Operations --------------------------------------------*/

ViStatus _VI_FUNC  viClose         (ViObject vi){ return 0; }
ViStatus _VI_FUNC  viSetAttribute  (ViObject vi, ViAttr attrName, ViAttrState attrValue){ return 0; }
ViStatus _VI_FUNC  viGetAttribute  (ViObject vi, ViAttr attrName, void _VI_PTR attrValue)
{
	switch (attrName)
	{
		case VI_ATTR_INTF_TYPE:
		{
			int* tmp_res = (int*)attrValue;
			*tmp_res = VI_INTF_VXI;
			break;
		}
		case VI_ATTR_MODEL_CODE:
		{
			int* tmp_res = (int*)attrValue;
			*tmp_res = 0x10B;// FOI
			//*tmp_res = 0x010D;//MBASE
			break;
		}
		case VI_ATTR_SLOT:
		{
			int* tmp_res = (int*)attrValue;
			*tmp_res = 2;
			break;
		}
	};
	return 0; 
}
ViStatus _VI_FUNC  viStatusDesc    (ViObject vi, ViStatus status, ViChar _VI_FAR desc[]){ return 0; }
ViStatus _VI_FUNC  viTerminate     (ViObject vi, ViUInt16 degree, ViJobId jobId){ return 0; }

ViStatus _VI_FUNC  viLock          (ViSession vi, ViAccessMode lockType, ViUInt32 timeout,
                                    ViKeyId requestedKey, ViChar _VI_FAR accessKey[]){ return 0; }
ViStatus _VI_FUNC  viUnlock        (ViSession vi){ return 0; }
ViStatus _VI_FUNC  viEnableEvent   (ViSession vi, ViEventType eventType, ViUInt16 mechanism,
                                    ViEventFilter context){ return 0; }
ViStatus _VI_FUNC  viDisableEvent  (ViSession vi, ViEventType eventType, ViUInt16 mechanism){ return 0; }
ViStatus _VI_FUNC  viDiscardEvents (ViSession vi, ViEventType eventType, ViUInt16 mechanism){ return 0; }
ViStatus _VI_FUNC  viWaitOnEvent   (ViSession vi, ViEventType inEventType, ViUInt32 timeout,
                                    ViPEventType outEventType, ViPEvent outContext){ return 0; }
ViStatus _VI_FUNC  viInstallHandler(ViSession vi, ViEventType eventType, ViHndlr handler,
                                    ViAddr userHandle){ return 0; }
ViStatus _VI_FUNC  viUninstallHandler(ViSession vi, ViEventType eventType, ViHndlr handler,
                                      ViAddr userHandle){ return 0; }

/*- Basic I/O Operations ----------------------------------------------------*/

ViStatus _VI_FUNC  viRead          (ViSession vi, ViPBuf buf, ViUInt32 cnt, ViPUInt32 retCnt){ return 0; }
ViStatus _VI_FUNC  viReadAsync     (ViSession vi, ViPBuf buf, ViUInt32 cnt, ViPJobId  jobId){ return 0; }
ViStatus _VI_FUNC  viReadToFile    (ViSession vi, ViConstString filename, ViUInt32 cnt,
                                    ViPUInt32 retCnt){ return 0; }
ViStatus _VI_FUNC  viWrite         (ViSession vi, ViBuf  buf, ViUInt32 cnt, ViPUInt32 retCnt){ return 0; }
ViStatus _VI_FUNC  viWriteAsync    (ViSession vi, ViBuf  buf, ViUInt32 cnt, ViPJobId  jobId){ return 0; }
ViStatus _VI_FUNC viWriteFromFile  (ViSession vi, ViConstString filename, ViUInt32 cnt,
                                    ViPUInt32 retCnt){ return 0; }
ViStatus _VI_FUNC  viAssertTrigger (ViSession vi, ViUInt16 protocol){ return 0; }
ViStatus _VI_FUNC  viReadSTB       (ViSession vi, ViPUInt16 status){ return 0; }
ViStatus _VI_FUNC  viClear         (ViSession vi){ return 0; }

/*- Formatted and Buffered I/O Operations -----------------------------------*/

ViStatus _VI_FUNC  viSetBuf        (ViSession vi, ViUInt16 mask, ViUInt32 size){ return 0; }
ViStatus _VI_FUNC  viFlush         (ViSession vi, ViUInt16 mask){ return 0; }

ViStatus _VI_FUNC  viBufWrite      (ViSession vi, ViBuf  buf, ViUInt32 cnt, ViPUInt32 retCnt){ return 0; }
ViStatus _VI_FUNC  viBufRead       (ViSession vi, ViPBuf buf, ViUInt32 cnt, ViPUInt32 retCnt){ return 0; }

ViStatus _VI_FUNCC viPrintf        (ViSession vi, ViString writeFmt, ...){ return 0; }
ViStatus _VI_FUNC  viVPrintf       (ViSession vi, ViString writeFmt, ViVAList params){ return 0; }
ViStatus _VI_FUNCC viSPrintf       (ViSession vi, ViPBuf buf, ViString writeFmt, ...){ return 0; }
ViStatus _VI_FUNC  viVSPrintf      (ViSession vi, ViPBuf buf, ViString writeFmt,
                                    ViVAList parms){ return 0; }
ViStatus _VI_FUNCC viScanf         (ViSession vi, ViString readFmt, ...){ return 0; }
ViStatus _VI_FUNC  viVScanf        (ViSession vi, ViString readFmt, ViVAList params){ return 0; }
ViStatus _VI_FUNCC viSScanf        (ViSession vi, ViBuf buf, ViString readFmt, ...){ return 0; }
ViStatus _VI_FUNC  viVSScanf       (ViSession vi, ViBuf buf, ViString readFmt,
                                    ViVAList parms){ return 0; }
ViStatus _VI_FUNCC viQueryf        (ViSession vi, ViString writeFmt, ViString readFmt, ...){ return 0; }
ViStatus _VI_FUNC  viVQueryf       (ViSession vi, ViString writeFmt, ViString readFmt, 
                                    ViVAList params){ return 0; }

/*- Memory I/O Operations ---------------------------------------------------*/

ViStatus _VI_FUNC  viIn8           (ViSession vi, ViUInt16 space,
                                    ViBusAddress offset, ViPUInt8  val8){ return 0; }
ViStatus _VI_FUNC  viOut8          (ViSession vi, ViUInt16 space,
                                    ViBusAddress offset, ViUInt8   val8){ return 0; }
ViStatus _VI_FUNC  viIn16          (ViSession vi, ViUInt16 space,
                                    ViBusAddress offset, ViPUInt16 val16){ return 0; }
ViStatus _VI_FUNC  viOut16         (ViSession vi, ViUInt16 space,
                                    ViBusAddress offset, ViUInt16  val16){ return 0; }
ViStatus _VI_FUNC  viIn32          (ViSession vi, ViUInt16 space,
                                    ViBusAddress offset, ViPUInt32 val32){ return 0; }
ViStatus _VI_FUNC  viOut32         (ViSession vi, ViUInt16 space,
                                    ViBusAddress offset, ViUInt32  val32){ return 0; }

ViStatus _VI_FUNC  viMoveIn8       (ViSession vi, ViUInt16 space, ViBusAddress offset,
                                    ViBusSize length, ViAUInt8  buf8){ return 0; }
ViStatus _VI_FUNC  viMoveOut8      (ViSession vi, ViUInt16 space, ViBusAddress offset,
                                    ViBusSize length, ViAUInt8  buf8){ return 0; }
ViStatus _VI_FUNC  viMoveIn16      (ViSession vi, ViUInt16 space, ViBusAddress offset,
                                    ViBusSize length, ViAUInt16 buf16){ return 0; }
ViStatus _VI_FUNC  viMoveOut16     (ViSession vi, ViUInt16 space, ViBusAddress offset,
                                    ViBusSize length, ViAUInt16 buf16){ return 0; }
ViStatus _VI_FUNC  viMoveIn32      (ViSession vi, ViUInt16 space, ViBusAddress offset,
                                    ViBusSize length, ViAUInt32 buf32){ return 0; }
ViStatus _VI_FUNC  viMoveOut32     (ViSession vi, ViUInt16 space, ViBusAddress offset,
                                    ViBusSize length, ViAUInt32 buf32){ return 0; }
ViStatus _VI_FUNC  viMove          (ViSession vi, ViUInt16 srcSpace, ViBusAddress srcOffset,
                                    ViUInt16 srcWidth, ViUInt16 destSpace, 
                                    ViBusAddress destOffset, ViUInt16 destWidth, 
                                    ViBusSize srcLength){ return 0; } 
ViStatus _VI_FUNC  viMoveAsync     (ViSession vi, ViUInt16 srcSpace, ViBusAddress srcOffset,
                                    ViUInt16 srcWidth, ViUInt16 destSpace, 
                                    ViBusAddress destOffset, ViUInt16 destWidth, 
                                    ViBusSize srcLength, ViPJobId jobId){ return 0; } 

ViStatus _VI_FUNC  viMapAddress    (ViSession vi, ViUInt16 mapSpace, ViBusAddress mapOffset,
                                    ViBusSize mapSize, ViBoolean access,
                                    ViAddr suggested, ViPAddr address){ return 0; }
ViStatus _VI_FUNC  viUnmapAddress  (ViSession vi){ return 0; }

void     _VI_FUNC  viPeek8         (ViSession vi, ViAddr address, ViPUInt8  val8){ }
void     _VI_FUNC  viPoke8         (ViSession vi, ViAddr address, ViUInt8   val8){}
void     _VI_FUNC  viPeek16        (ViSession vi, ViAddr address, ViPUInt16 val16){  }
void     _VI_FUNC  viPoke16        (ViSession vi, ViAddr address, ViUInt16  val16){  }
void     _VI_FUNC  viPeek32        (ViSession vi, ViAddr address, ViPUInt32 val32){  }
void     _VI_FUNC  viPoke32        (ViSession vi, ViAddr address, ViUInt32  val32){  }

/*- Shared Memory Operations ------------------------------------------------*/

ViStatus _VI_FUNC  viMemAlloc      (ViSession vi, ViBusSize size, ViPBusAddress offset){ return 0; }
ViStatus _VI_FUNC  viMemFree       (ViSession vi, ViBusAddress offset){ return 0; }

/*- Interface Specific Operations -------------------------------------------*/

ViStatus _VI_FUNC  viGpibControlREN  (ViSession vi, ViUInt16 mode){ return 0; }
ViStatus _VI_FUNC  viGpibControlATN  (ViSession vi, ViUInt16 mode){ return 0; }
ViStatus _VI_FUNC  viGpibSendIFC     (ViSession vi){ return 0; }
ViStatus _VI_FUNC  viGpibCommand     (ViSession vi, ViBuf cmd, ViUInt32 cnt, ViPUInt32 retCnt){ return 0; }
ViStatus _VI_FUNC  viGpibPassControl (ViSession vi, ViUInt16 primAddr, ViUInt16 secAddr){ return 0; }
ViStatus _VI_FUNC  viVxiCommandQuery (ViSession vi, ViUInt16 mode, ViUInt32 cmd,
                                      ViPUInt32 response){ return 0; }
ViStatus _VI_FUNC  viAssertUtilSignal(ViSession vi, ViUInt16 line){ return 0; }
ViStatus _VI_FUNC  viAssertIntrSignal(ViSession vi, ViInt16 mode, ViUInt32 statusID){ return 0; }
ViStatus _VI_FUNC  viMapTrigger      (ViSession vi, ViInt16 trigSrc, ViInt16 trigDest,
                                      ViUInt16 mode){ return 0; }
ViStatus _VI_FUNC  viUnmapTrigger    (ViSession vi, ViInt16 trigSrc, ViInt16 trigDest){ return 0; }
ViStatus _VI_FUNC  viUsbControlOut   (ViSession vi, ViInt16 bmRequestType, ViInt16 bRequest,
                                      ViUInt16 wValue, ViUInt16 wIndex, ViUInt16 wLength,
                                      ViBuf buf){ return 0; }
ViStatus _VI_FUNC  viUsbControlIn    (ViSession vi, ViInt16 bmRequestType, ViInt16 bRequest,
                                      ViUInt16 wValue, ViUInt16 wIndex, ViUInt16 wLength,
                                      ViPBuf buf, ViPUInt16 retCnt){ return 0; }

#if defined(__cplusplus) || defined(__cplusplus__)
   }
#endif


/*- The End -----------------------------------------------------------------*/
