#include <unmbkupi.h>

#if defined(__cplusplus) || defined(__cplusplus__)
extern "C" {
#endif


ViStatus _VI_FUNC unmbkupi_init (
							ViRsrc Addr, 
							ViBoolean doInd, 
							ViBoolean doReset, 
							ViPSession mvi){ return 0; }
//=========================================================================
ViStatus _VI_FUNC unmbkupi_connect (
							ViSession mvi, 
							ViSession basevi, 
							ViUInt16 meznum,
							ViBoolean doIdn, 
							ViBoolean doReset){ return 0; }

//=========================================================================
ViStatus _VI_FUNC unmbkupi_alloc (
									ViSession mvi, 
									ViInt32 memsize){ return 0; }

//=========================================================================
ViStatus _VI_FUNC unmbkupi_config_sync (
									ViSession mvi, 
									ViInt32 src1,
									ViInt32 src2,
									ViInt32 src3){ return 0; }

//=========================================================================
ViStatus _VI_FUNC unmbkupi_config_recv_ctrl (
									ViSession mvi, 
									ViInt32 src){ return 0; }

//=========================================================================
ViStatus _VI_FUNC unmbkupi_mode_block (
							ViSession mvi, 
							ViInt32 blockSize){ return 0; }

//=========================================================================
ViStatus _VI_FUNC unmbkupi_mode_cycle (
							ViSession mvi, 
							ViInt32 packetSize){ return 0; }

//=========================================================================
ViStatus _VI_FUNC unmbkupi_start (
							ViSession mvi){ return 0; }

//=========================================================================
ViStatus _VI_FUNC unmbkupi_stop (
							ViSession mvi){ return 0; }

//=========================================================================
ViStatus _VI_FUNC unmbkupi_state_q (
							ViSession mvi, 
							ViPInt32 state){ return 0; }

//=========================================================================
ViStatus _VI_FUNC unmbkupi_num_ready_data (
							ViSession mvi, 
							ViPInt32 nreadybytes){ return 0; }

//=========================================================================
ViStatus _VI_FUNC unmbkupi_read_block (
							ViSession mvi, 
							ViInt32 offset, 
							ViInt32 needbytes, 
							ViInt8 _VI_FAR chan1buf[],
							ViInt8 _VI_FAR chan2buf[],
							ViInt8 _VI_FAR chan3buf[]){ return 0; }

//=========================================================================
ViStatus _VI_FUNC unmbkupi_read_packet (
							ViSession mvi, 
							ViInt32 needbytes,
							ViInt8 _VI_FAR chan1buf[], 
							ViInt8 _VI_FAR chan2buf[], 
							ViInt8 _VI_FAR chan3buf[], 
							ViPInt32 nread){ return 0; }

//=========================================================================
ViStatus _VI_FUNC unmbkupi_reset (
								ViSession mvi){ return 0; }

//=====================================================================
ViStatus _VI_FUNC unmbkupi_self_test (
									ViSession mvi, 
									ViPInt16 result, 
									ViChar _VI_FAR message[]){ return 0; }

//=========================================================================
ViStatus _VI_FUNC unmbkupi_test_ok2 (
									ViSession mvi, 
									ViPInt16 result, 
									ViChar _VI_FAR message[]){ return 0; }

//=========================================================================
ViStatus _VI_FUNC unmbkupi_revision_query (
										ViSession mvi, 
										ViChar _VI_FAR sw_rev[], 
										ViChar _VI_FAR hw_rev[]){ return 0; }

//=========================================================================
ViStatus _VI_FUNC unmbkupi_error_query (
									ViSession mvi, 
									ViPInt32 errstatus, 
									ViChar _VI_FAR message[]){ return 0; }

//=========================================================================
ViStatus _VI_FUNC unmbkupi_error_message (
										ViSession mvi, 
										ViStatus errstatus, 
										ViChar _VI_FAR message[]){ return 0; }

//=========================================================================
ViStatus _VI_FUNC unmbkupi_close (
								ViSession mvi){ return 0; }

#if defined(__cplusplus) || defined(__cplusplus__)
}
#endif