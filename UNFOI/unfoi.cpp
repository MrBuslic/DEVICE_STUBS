#include <socket_rpc.h>
#include <unfoi.h>
#include <windows.h>
#include "foi_rpc.h"
#include "rpc_ports.h"
#include "unfoi_h.h"

#if defined(__cplusplus) || defined(__cplusplus__)
extern "C" {
#endif

/*----------------------------------------------------------------------*/
/*  Инициализация                                                       */
/*----------------------------------------------------------------------*/
ViStatus _VI_FUNC unfoi_init (ViRsrc rsrcName, ViBoolean id_query,
								ViBoolean reset, ViPSession vi){ return 0; }
/*----------------------------------------------------------------------*/
/*  Заготовки для функций-обработчиков событий                          */
/*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*/
/*  Зависимые от инструмента функции                                    */
/*----------------------------------------------------------------------*/
ViStatus _VI_FUNCC unfoi_chans_setup (ViSession vi, ViInt16 chans[], ViInt16 line[],
							ViReal64 U[], ViReal64 time){ return 0; }
ViStatus _VI_FUNCC unfoi_chan_setup (ViSession vi, ViInt16 chan, ViInt16 line, /*+*/
	ViReal64 U, ViReal64 time){
	return Srpc_buffer_class::Instance().foi_slot_thr.get_foi_obj()->unfoi_chan_setup(chan, line, U, time);
}
ViStatus _VI_FUNCC unfoi_run(ViSession vi){ return Srpc_buffer_class::Instance().foi_slot_thr.get_foi_obj()->unfoi_run(); }
ViStatus _VI_FUNCC unfoi_start (ViSession vi){ return 0; }	   
ViStatus _VI_FUNCC unfoi_conf_chans (ViSession vi, ViInt16 chans[], ViInt16 lines[],
						   ViReal64 U, ViReal64 time){ return 0; }
ViStatus _VI_FUNCC unfoi_conf_chan (ViSession vi, ViInt16 chan, ViInt16 line,
						   ViReal64 U, ViReal64 time){ return 0; }
ViStatus _VI_FUNCC unfoi_chans_route (ViSession vi, ViInt16 chans[], ViInt16 state){ return 0; } 
ViStatus _VI_FUNCC unfoi_chan_route (ViSession vi, ViInt16 state, ViInt16 chan){ return 0; } /*+*/           
ViStatus _VI_FUNCC unfoi_off (ViSession vi){ return 0; }
ViStatus _VI_FUNCC unfoi_time (ViSession vi, ViReal64 time){ return 0; }
ViStatus _VI_FUNCC unfoi_time_query (ViSession vi, ViReal64 *time){ return 0; }
ViStatus _VI_FUNCC unfoi_chans_voltage (ViSession vi, ViInt16 chans[], ViInt16 lines[],
							  ViReal64 U){ return 0; }
ViStatus _VI_FUNCC unfoi_chan_voltage (ViSession vi, ViInt16 chan, ViInt16 line,
							  ViReal64 U){ return 0; }
ViStatus _VI_FUNCC unfoi_chan_query (ViSession vi, ViInt16 chan, ViInt16 *state_chan,
						   ViReal64 *U_Main, ViReal64 *U_Reserv){ return 0; }
ViStatus _VI_FUNCC unfoi_chans_query (ViSession vi, ViInt16 state_chan[], ViReal64 U_Main[],
							ViReal64 U_Reserv[], ViReal64 *time){ return 0; }

/*----------------------------------------------------------------------*/
/*  Функции-приложения (общие)                                          */
/*----------------------------------------------------------------------*/
ViStatus _VI_FUNCC unfoi_state (ViSession vi, ViInt16 *state){ return 0; }        /*+*/    
ViStatus _VI_FUNCC unfoi_enable_event (ViSession vi, ViBoolean event){ return 0; }        
ViStatus _VI_FUNCC unfoi_instal_handler (ViSession vi, ViAddr handler){ return 0; }                  
ViStatus _VI_FUNCC unfoi_config_hierarch (ViSession vi,
								ViInt16 master,
								ViInt16 TTLTRG){ return 0; }
/*----------------------------------------------------------------------*/
/*  Служебные функции                                                   */
/*----------------------------------------------------------------------*/
ViStatus _VI_FUNC unfoi_reset (ViSession vi){ return 0; }
ViStatus _VI_FUNC unfoi_self_test (ViSession vi,
									ViPInt16 test_result,
									ViChar _VI_FAR test_message[]){ return 0; }
ViStatus _VI_FUNC unfoi_error_query (ViSession vi,
									  ViPInt32 error,
									  ViChar _VI_FAR error_message[]){ return 0; }
ViStatus _VI_FUNC unfoi_error_message (ViSession vi,
										ViStatus error,
										ViChar _VI_FAR message[]){ return 0; }
ViStatus _VI_FUNC unfoi_revision_query (ViSession vi,
										 ViChar _VI_FAR driver_revision[],
										 ViChar _VI_FAR instrument_revision[]){ return 0; }
ViStatus _VI_FUNC unfoi_getDeviceList (ViPInt16 slotList,
										ViPInt16 laList,
										ViInt16 listLength,
										ViPInt16 numFound){ return 0; }
/*----  Служебные функции   -------------------------------------------*/                                       
 ViStatus _VI_FUNCC unfoi_default_constant (ViSession vi){ return 0; }            
/*----------------------------------------------------------------------*/
/*  Закрытие сеанса инструмента                                        */
/*----------------------------------------------------------------------*/
ViStatus _VI_FUNC unfoi_close (ViSession vi){ return 0; }
/*----------------------------------------------------------------------*/
/*  Определения                                                         */
/*----------------------------------------------------------------------*/


#if defined(__cplusplus) || defined(__cplusplus__)
}
#endif
