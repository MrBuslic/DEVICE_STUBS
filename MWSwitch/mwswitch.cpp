#if defined(__cplusplus) || defined(__cplusplus__)
extern "C" {
#endif
#include "AgMWSwitch.h"
#if defined(__cplusplus) || defined(__cplusplus__)
}
#endif
#include <socket_rpc.h>
#include <windows.h>
#include "mwswitch_rpc.h"
#include "rpc_ports.h"
#include "mwswitch_h.h"

int switch_count = 0;

#if defined(__cplusplus) || defined(__cplusplus__)
extern "C" {
#endif



////---------------------- Set input trigger ------------------------------
//ViStatus _VI_FUNC unmds32_input_trigger (ViSession mvi, ViBoolean state){
//	return Srpc_buffer_class::Instance().mds32_slot_thr[mvi-1]->get_mds32_obj()->unmds32_input_trigger(state);
//}

ViStatus _VI_FUNC AgMWSwitch_init(ViRsrc ResourceName, ViBoolean IdQuery, ViBoolean Reset, ViSession* Vi)
{
	Srpc_buffer_class::Instance();
	switch_count++;
	*Vi = switch_count;
	return 0;
}


ViStatus _VI_FUNC AgMWSwitch_close(ViSession Vi) { return 0; }
ViStatus _VI_FUNC AgMWSwitch_InitWithOptions(ViRsrc ResourceName, ViBoolean IdQuery, ViBoolean Reset, ViConstString OptionsString, ViSession* Vi) { return 0; }

/*- Utility */

ViStatus _VI_FUNC AgMWSwitch_revision_query(ViSession Vi, ViChar DriverRev[], ViChar InstrRev[]) { return 0; }
ViStatus _VI_FUNC AgMWSwitch_error_message(ViSession Vi, ViStatus ErrorCode, ViChar ErrorMessage[]) { return 0; }
ViStatus _VI_FUNC AgMWSwitch_GetError(ViSession Vi, ViStatus* ErrorCode, ViInt32 ErrorDescriptionBufferSize, ViChar ErrorDescription[]) { return 0; }
ViStatus _VI_FUNC AgMWSwitch_ClearError(ViSession Vi) { return 0; }
ViStatus _VI_FUNC AgMWSwitch_ClearInterchangeWarnings(ViSession Vi) { return 0; }
ViStatus _VI_FUNC AgMWSwitch_GetNextCoercionRecord(ViSession Vi, ViInt32 CoercionRecordBufferSize, ViChar CoercionRecord[]) { return 0; }
ViStatus _VI_FUNC AgMWSwitch_GetNextInterchangeWarning(ViSession Vi, ViInt32 InterchangeWarningBufferSize, ViChar InterchangeWarning[]) { return 0; }
ViStatus _VI_FUNC AgMWSwitch_InvalidateAllAttributes(ViSession Vi) { return 0; }
ViStatus _VI_FUNC AgMWSwitch_ResetInterchangeCheck(ViSession Vi) { return 0; }
ViStatus _VI_FUNC AgMWSwitch_Disable(ViSession Vi) { return 0; }
ViStatus _VI_FUNC AgMWSwitch_error_query(ViSession Vi, ViInt32* ErrorCode, ViChar ErrorMessage[]) { return 0; }
ViStatus _VI_FUNC AgMWSwitch_LockSession(ViSession Vi, ViBoolean* CallerHasLock) { return 0; }
ViStatus _VI_FUNC AgMWSwitch_reset(ViSession Vi) { return 0; }
ViStatus _VI_FUNC AgMWSwitch_ResetWithDefaults(ViSession Vi) { return 0; }
ViStatus _VI_FUNC AgMWSwitch_self_test(ViSession Vi, ViInt16* TestResult, ViChar TestMessage[]) { return 0; }
ViStatus _VI_FUNC AgMWSwitch_UnlockSession(ViSession Vi, ViBoolean* CallerHasLock) { return 0; }
ViStatus _VI_FUNC AgMWSwitch_GetChannelName(ViSession Vi, ViInt32 Index, ViInt32 NameBufferSize, ViChar Name[]) { return 0; }

/*- Attribute Accessors */

ViStatus _VI_FUNC AgMWSwitch_GetAttributeViInt32(ViSession Vi, ViConstString RepCapIdentifier, ViAttr AttributeID, ViInt32* AttributeValue) { return 0; }
ViStatus _VI_FUNC AgMWSwitch_GetAttributeViReal64(ViSession Vi, ViConstString RepCapIdentifier, ViAttr AttributeID, ViReal64* AttributeValue) { return 0; }
ViStatus _VI_FUNC AgMWSwitch_GetAttributeViBoolean(ViSession Vi, ViConstString RepCapIdentifier, ViAttr AttributeID, ViBoolean* AttributeValue) { return 0; }
ViStatus _VI_FUNC AgMWSwitch_GetAttributeViSession(ViSession Vi, ViConstString RepCapIdentifier, ViAttr AttributeID, ViSession* AttributeValue) { return 0; }
ViStatus _VI_FUNC AgMWSwitch_GetAttributeViString(ViSession Vi, ViConstString RepCapIdentifier, ViAttr AttributeID, ViInt32 AttributeValueBufferSize, ViChar AttributeValue[]) { return 0; }
ViStatus _VI_FUNC AgMWSwitch_SetAttributeViInt32(ViSession Vi, ViConstString RepCapIdentifier, ViAttr AttributeID, ViInt32 AttributeValue) { return 0; }
ViStatus _VI_FUNC AgMWSwitch_SetAttributeViReal64(ViSession Vi, ViConstString RepCapIdentifier, ViAttr AttributeID, ViReal64 AttributeValue) { return 0; }
ViStatus _VI_FUNC AgMWSwitch_SetAttributeViBoolean(ViSession Vi, ViConstString RepCapIdentifier, ViAttr AttributeID, ViBoolean AttributeValue) { return 0; }
ViStatus _VI_FUNC AgMWSwitch_SetAttributeViSession(ViSession Vi, ViConstString RepCapIdentifier, ViAttr AttributeID, ViSession AttributeValue) { return 0; }
ViStatus _VI_FUNC AgMWSwitch_SetAttributeViString(ViSession Vi, ViConstString RepCapIdentifier, ViAttr AttributeID, ViConstString AttributeValue) { return 0; }

/*- Route */

ViStatus _VI_FUNC AgMWSwitch_IsDebounced(ViSession Vi, ViBoolean* IsDebounced) { return 0; }
ViStatus _VI_FUNC AgMWSwitch_CanConnect(ViSession Vi, ViConstString Channel1, ViConstString Channel2, ViInt32* PathCapability) { return 0; }
ViStatus _VI_FUNC AgMWSwitch_Connect(ViSession Vi, ViConstString Channel1, ViConstString Channel2) { return 0; }
ViStatus _VI_FUNC AgMWSwitch_Disconnect(ViSession Vi, ViConstString Channel1, ViConstString Channel2) { return 0; }
ViStatus _VI_FUNC AgMWSwitch_DisconnectAll(ViSession Vi) { return 0; }

/*- Paths */

ViStatus _VI_FUNC AgMWSwitch_GetPath(ViSession Vi, ViConstString Channel1, ViConstString Channel2, ViInt32 PathListBufferSize, ViChar PathList[]) { return 0; }
ViStatus _VI_FUNC AgMWSwitch_SetPath(ViSession Vi, ViConstString PathList) { return 0; }

/*- Route */

ViStatus _VI_FUNC AgMWSwitch_RouteCloseChannel(ViSession Vi, ViConstString Channel) { return 0; }
ViStatus _VI_FUNC AgMWSwitch_RouteOpenAll(ViSession Vi) { return 0; }
ViStatus _VI_FUNC AgMWSwitch_RouteGetBankState(ViSession Vi, ViInt32 Bank, ViInt32 StateBufferSize, ViInt32 State[], ViInt32* StateActualSize) { return 0; }
ViStatus _VI_FUNC AgMWSwitch_RouteIsChannelClosed(ViSession Vi, ViConstString Channel, ViBoolean* Closed) { return 0; }
ViStatus _VI_FUNC AgMWSwitch_RouteOpenChannel(ViSession Vi, ViConstString Channel) { return 0; }

/*- Diagnostic */

ViStatus _VI_FUNC AgMWSwitch_DiagnosticGetRelayCount(ViSession Vi, ViConstString Channel, ViInt32* Count) { return 0; }
ViStatus _VI_FUNC AgMWSwitch_DiagnosticClearRelayCount(ViSession Vi, ViConstString Channel) { return 0; }
ViStatus _VI_FUNC AgMWSwitch_DiagnosticClearAllRelayCounts(ViSession Vi) { return 0; }

#if defined(__cplusplus) || defined(__cplusplus__)
}
#endif
