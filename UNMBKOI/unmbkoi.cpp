#include <unmbkoi.h>

#if defined(__cplusplus) || defined(__cplusplus__)
extern "C" {
#endif

ViStatus _VI_FUNC unmbkoi_init (
							ViRsrc Addr, 
							ViBoolean doInd, 
							ViBoolean doReset, 
							ViPSession mvi
							){ return 0; }

ViStatus _VI_FUNC unmbkoi_connect (
							ViSession mvi, 
							ViSession basevi, 
							ViUInt16 meznum,
							ViBoolean doIdn, 
							ViBoolean doReset){ return 0; }

ViStatus _VI_FUNC unmbkoi_alloc (
									ViSession mvi, 
									ViInt32 memsize
									){ return 0; }

ViStatus _VI_FUNC unmbkoi_chan_select (
									ViSession mvi, 
									ViInt32 on1, 
									ViInt32 on2, 
									ViInt32 on3
									){ return 0; }

ViStatus _VI_FUNC unmbkoi_config_trans_ctrl (
									ViSession mvi, 
									ViInt32 arg1
									){ return 0; }

ViStatus _VI_FUNC unmbkoi_config_sync (
									ViSession mvi, 
									ViInt32 src1,
									ViInt32 src2,
									ViInt32 src3
									){ return 0; }

ViStatus _VI_FUNC unmbkoi_invers (
								ViSession mvi, 
								ViInt32 inv1, 
								ViInt32 inv2, 
								ViInt32 inv3
								){ return 0; }

ViStatus _VI_FUNC unmbkoi_ext_connect (
									ViSession mvi, 
									ViInt32 out1, 
									ViInt32 out2
									){ return 0; }

ViStatus _VI_FUNC unmbkoi_data_len (
									ViSession mvi, 
									ViInt32 length){ return 0; }

ViStatus _VI_FUNC unmbkoi_load_data (
									ViSession mvi, 
									ViInt32 offset, 
									ViInt32 length, 
									ViInt8 _VI_FAR data[]
									){ return 0; }

ViStatus _VI_FUNC unmbkoi_start (
								ViSession mvi, 
								ViInt32 cycle){ return 0; }

ViStatus _VI_FUNC unmbkoi_stop (
							ViSession mvi
							){ return 0; }

ViStatus _VI_FUNC unmbkoi_state_q (
								ViSession mvi, 
								ViPInt32 state,
								ViPInt32 sent1,
								ViPInt32 sent2,
								ViPInt32 sent3
								){ return 0; }

ViStatus _VI_FUNC unmbkoi_reset (
								ViSession mvi
								){ return 0; }

ViStatus _VI_FUNC unmbkoi_self_test (
									ViSession mvi, 
									ViPInt16 result, 
									ViChar _VI_FAR message[]
									){ return 0; }

ViStatus _VI_FUNC unmbkoi_test_ok2 (
									ViSession mvi, 
									ViInt32 chan,
									ViInt32 F, 
									ViPInt16 result, 
									ViChar _VI_FAR message[]
									){ return 0; }

ViStatus _VI_FUNC unmbkoi_revision_query (
										ViSession mvi, 
										ViChar _VI_FAR sw_rev[], 
										ViChar _VI_FAR hw_rev[]
										){ return 0; }

ViStatus _VI_FUNC unmbkoi_error_query (
									ViSession mvi, 
									ViPInt32 errstatus, 
									ViChar _VI_FAR message[]
									){ return 0; }

ViStatus _VI_FUNC unmbkoi_error_message (
										ViSession mvi, 
										ViStatus errstatus, 
										ViChar _VI_FAR message[]
										){ return 0; }

ViStatus _VI_FUNC unmbkoi_close (
								ViSession mvi
								){ return 0; }

#if defined(__cplusplus) || defined(__cplusplus__)
}
#endif
