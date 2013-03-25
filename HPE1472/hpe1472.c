/* Copyright (C) 1997 Hewlett-Packard Company */
/* VXIplug&play Instrument Driver for the HPE1472*/

#include <hpe1472.h>

#if defined(__cplusplus) || defined(__cplusplus__)
extern "C" {
#endif

/* required plug and play functions from VPP-3.1*/
ViStatus _VI_FUNC hpe1472_init (ViRsrc InstrDesc, ViBoolean id_query,
                            ViBoolean do_reset, ViPSession vi ){ return 0; }

ViStatus _VI_FUNC hpe1472_close (ViSession vi){ return 0; }

ViStatus _VI_FUNC hpe1472_reset (ViSession vi){ return 0; }

ViStatus _VI_FUNC hpe1472_self_test (ViSession vi, ViPInt16 test_result,
                                 ViChar _VI_FAR test_message[]){ return 0; }

ViStatus _VI_FUNC hpe1472_error_query (ViSession vi, ViPInt32 error_number,
                                   ViChar _VI_FAR error_message[]){ return 0; }

ViStatus _VI_FUNC hpe1472_error_message (ViSession vi, ViStatus error_number,
                                     ViChar _VI_FAR message[]){ return 0; }

ViStatus _VI_FUNC hpe1472_revision_query (ViSession vi, ViChar _VI_FAR driver_rev[],
                                      ViChar _VI_FAR instr_rev[]){ return 0; }

/* HP other standard functions */

ViStatus _VI_FUNC hpe1472_dcl(ViSession vi){ return 0; }

ViStatus _VI_FUNC hpe1472_errorQueryDetect(ViSession vi, ViBoolean errDetect){ return 0; }

ViStatus _VI_FUNC hpe1472_errorQueryDetect_Q(ViSession vi, ViPBoolean pErrDetect){ return 0; }

ViStatus _VI_FUNC hpe1472_opc(ViSession vi){ return 0; }

ViStatus _VI_FUNC hpe1472_opc_Q(ViSession vi, ViPInt16 opc){ return 0; }

ViStatus _VI_FUNC hpe1472_readStatusByte_Q(ViSession vi, ViPInt16 statusByte){ return 0; }

ViStatus _VI_FUNC hpe1472_statEvenClr(ViSession vi){ return 0; }

ViStatus _VI_FUNC hpe1472_statEven_Q(ViSession vi, ViInt32 happening,
                                 ViPBoolean pEvent){ return 0; }
#ifdef INSTR_CALLBACKS	
ViStatus _VI_FUNC hpe1472_statEvenHdlr(ViSession vi, ViInt32 happening,       
                                   hpe1472_InstrEventHandler eventHandler,
                                   ViAddr userData){ return 0; }

ViStatus _VI_FUNC hpe1472_statEvenHdlr_Q(ViSession vi, ViInt32 happening,
                                     hpe1472_InstrPEventHandler pEventHandler,
                                     ViPAddr pUserData){ return 0; }

ViStatus _VI_FUNC hpe1472_statEvenHdlrDelAll(ViSession vi){ return 0; }
#endif /* INSTR_CALLBACKS */

ViStatus _VI_FUNC hpe1472_timeOut (ViSession vi, ViInt32 timeOut){ return 0; }

ViStatus _VI_FUNC hpe1472_timeOut_Q (ViSession vi, ViPInt32 timeOut){ return 0; }

ViStatus _VI_FUNC hpe1472_wai(ViSession vi){ return 0; }

/* Instr Specific Functions follow */

ViStatus _VI_FUNC hpe1472_routClos  (
  ViSession vi,
  ViInt32 channel){ return 0; }

ViStatus _VI_FUNC hpe1472_routClosList  (
  ViSession vi,
  ViString channelList){ return 0; }

ViStatus _VI_FUNC hpe1472_routClosList_Q  (
  ViSession vi,
  ViString channelList,
  ViChar _VI_FAR routClosList[]){ return 0; }

ViStatus _VI_FUNC hpe1472_routClos_Q  (
  ViSession vi,
  ViInt32 channel,
  ViPBoolean routClos){ return 0; }

ViStatus _VI_FUNC hpe1472_setAll  (
  ViSession vi,
  ViInt16 Card_Number,
  ViInt16 Module_Number,
  ViInt16 Bank0,
  ViInt16 Bank1,
  ViInt16 Bank2,
  ViInt16 Bank3,
  ViInt16 Bank4,
  ViInt16 Bank5){ return 0; }

ViStatus _VI_FUNC hpe1472_systCdes_Q  (
  ViSession vi,
  ViInt16 card,
  ViChar _VI_FAR systCdes[]){ return 0; }

ViStatus _VI_FUNC hpe1472_systCopt_Q  (
  ViSession vi,
  ViInt16 card,
  ViChar _VI_FAR systCopt[]){ return 0; }

ViStatus _VI_FUNC hpe1472_systCpon  (
  ViSession vi,
  ViInt16 card){ return 0; }

ViStatus _VI_FUNC hpe1472_systCtyp_Q  (
  ViSession vi,
  ViInt16 card,
  ViChar _VI_FAR systCtyp[]){ return 0; }

/* Used for "C" externs in C++ */
#if defined(__cplusplus) || defined(__cplusplus__)
}    /* end of "C" externs for C++ */
#endif 


