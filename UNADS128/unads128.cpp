extern "C" {
#include <unads128.h>
}
ViStatus _VI_FUNC unads128_sft_interface (ViSession vi,  UN_SFT_INTERFACE sftInterface){ return 0; }

ViStatus _VI_FUNC unads128_init ( 
								ViRsrc rsrcName,
								ViBoolean do_ID, 
								ViBoolean do_RESET, 
								ViPSession vi 
								){ return 0; }

ViStatus _VI_FUNC unads128_input ( 
								ViSession vi, 
								ViUInt16  _VI_FAR buff[] 
								){ return 0; }

ViStatus _VI_FUNC unads128_start ( 
								ViSession vi 
								){ return 0; }

ViStatus _VI_FUNC unads128_start_check ( 
								ViSession vi 
								){ return 0; }

ViStatus _VI_FUNC unads128_stop ( 
								ViSession vi 
								){ return 0; }

ViStatus _VI_FUNC unads128_state ( 
								ViSession vi, 
								ViPInt16 state 
								){ return 0; }

ViStatus _VI_FUNC unads128_conf_analog ( 
								ViSession vi, 
								ViInt16 group, 
								ViReal64 level_0, 
								ViReal64 level_1 
								){ return 0; }

ViStatus _VI_FUNC unads128_analog_q ( 
								ViSession vi, 
								ViInt16 group, 
								ViPReal64 level_0, 
								ViPReal64 level_1 
								){ return 0; }

ViStatus _VI_FUNC unads128_install_handler ( 
								ViSession vi, 
								ViAddr user_handler 
								){ return 0; }

ViStatus _VI_FUNC unads128_read_data ( 
								ViSession vi, 
								ViUInt16 _VI_FAR thisBuiff[], 
								ViUInt16 _VI_FAR firstBuff[] 
								){ return 0; }

ViStatus _VI_FUNC unads128_reset ( 
								ViSession vi 
								){ return 0; }


ViStatus _VI_FUNC unads128_self_test ( 
								ViSession vi, 
								ViPInt16 test_result, 
								ViChar _VI_FAR test_message[] 
								){ return 0; }

ViStatus _VI_FUNC unads128_test1 (
								ViSession vi,
								ViInt16 *TestResult,
								ViChar _VI_FAR TestMessage[]){ return 0; }
								  
ViStatus _VI_FUNC unads128_error_query ( 
								ViSession vi, 
								ViPInt32 error, 
								ViChar _VI_FAR error_message[] 
								){ return 0; }

ViStatus _VI_FUNC unads128_error_message ( 
								ViSession vi, 
								ViStatus status_code, 
								ViChar _VI_FAR status_descr[] 
								){ return 0; }

ViStatus _VI_FUNC unads128_revision_query ( 
								ViSession vi, 
								ViChar _VI_FAR driver_ver[], 
								ViChar _VI_FAR instr_ver[] 
								){ return 0; }

ViStatus _VI_FUNC unads128_close ( 
								ViSession vi 
								){ return 0; }

/*
ViStatus _VI_FUNC unads128_conf_time ( 
								ViSession vi, 
								ViUInt16 dT 
								){ return 0; }

ViStatus _VI_FUNC unads128_start_sourse ( 
								ViSession vi, 
								ViInt16 source, 
								ViInt16 TTLline 
								){ return 0; }

ViStatus _VI_FUNC unads128_start_sourse_q ( 
								ViSession vi, 
								ViPInt16 source, 
								ViPUInt16 TTLline 
								){ return 0; }
ViStatus _VI_FUNC unads128_calibrate ( 
								ViSession vi 
								){ return 0; }
*/

