#include <socket_rpc.h>
#include <unmkprm.h>
#include <windows.h>
#include "mkprm_rpc.h"
#include "rpc_ports.h"
#include "unmkprm_h.h"

#if defined(__cplusplus) || defined(__cplusplus__)
extern "C" {
#endif

ViStatus _VI_FUNC unmkprm_init (
							ViRsrc Addr, 
							ViBoolean doInd, 
							ViBoolean doReset, 
							ViPSession mvi
							){ return 0; }

ViStatus _VI_FUNC unmkprm_connect (
							ViSession mvi, 
							ViSession basevi, 
							ViUInt16 meznum,
							ViBoolean doIdn, 
							ViBoolean doReset
							){ return 0; }

ViStatus _VI_FUNC unmkprm_alloc (
							ViSession mvi, 
							ViInt32 memsize
							){ return 0; }

ViStatus _VI_FUNC unmkprm_mode_block (
							ViSession mvi, 
							ViInt32 blockSize
							){ return 0; }

ViStatus _VI_FUNC unmkprm_mode_cycle (
							ViSession mvi, 
							ViInt32 packetSize
							){ return 0; }

ViStatus _VI_FUNC unmkprm_start (
							ViSession mvi
							)
{ 
	Srpc_buffer_class::Instance().mkprm_slot_thr.get_mkprm_obj()->unmkprm_start();
	return 0; 
}

ViStatus _VI_FUNC unmkprm_stop (
							ViSession mvi
							)
{
	Srpc_buffer_class::Instance().mkprm_slot_thr.get_mkprm_obj()->unmkprm_stop();
	return 0; 
}

ViStatus _VI_FUNC unmkprm_inp_type (
							ViSession mvi, 
							ViInt32 type
							){ return 0; }

ViStatus _VI_FUNC unmkprm_config_psp (
							ViSession mvi, 
							ViInt32 maskLen, 
							ViUInt16 feedback, 
							ViUInt16 invState, 
							ViUInt16 syncState,
							ViInt32 *strlen){ return 0; }

ViStatus _VI_FUNC unmkprm_config_sync_level (
							ViSession mvi, 
							ViInt32 level
							){ return 0; }

ViStatus _VI_FUNC unmkprm_config_adjust_level (
							ViSession mvi, 
							ViInt32 level
							){ return 0; }

ViStatus _VI_FUNC unmkprm_config_adjust_level2 (
							ViSession mvi, 
							ViInt32 level
							){ return 0; }

ViStatus _VI_FUNC unmkprm_num_ready_data (
							ViSession mvi, 
							ViPInt32 nready,
							ViPInt32 nreadystr
							){ return 0; }

ViStatus _VI_FUNC unmkprm_read_str_block (
							ViSession mvi, 
							ViInt32 strIdx, 
							ViInt32 needStr, 
							ViInt8 _VI_FAR data[], 
							ViInt8 _VI_FAR mask[], 
							ViPInt32 nreadstr
							){ return 0; }

ViStatus _VI_FUNC unmkprm_read_packet (
							ViSession mvi, 
							ViInt32 bsize, 
							ViInt8 _VI_FAR data[], 
							ViInt8 _VI_FAR mask[], 
							ViPInt32 nread,
							ViPInt32 nreadstr
							){ return 0; }
ViStatus _VI_FUNC unmkprm_read_str_packet (
							ViSession mvi, 
							ViInt32 needStr,
							ViInt8 _VI_FAR data[], 
							ViInt8 _VI_FAR mask[], 
							ViPInt32 nreadstr
							)
{
	QVariantList string_data;
	int tmp_begin = 0;
	int tmp_string_size;
	Srpc_buffer_class::Instance().mkprm_slot_thr.get_mkprm_obj()->unmkprm_get_strings(needStr, string_data);
	for (int i = 0; i < string_data.size(); i++)
	{
		QByteArray& tmp_string = string_data[i].toByteArray();
		tmp_string_size = tmp_string.size();
		std::copy(tmp_string.data(), tmp_string.data() + tmp_string_size, data + tmp_begin);
		tmp_begin += tmp_string_size;
	}
	*nreadstr = string_data.size();
	return 0; 
}

ViStatus _VI_FUNC unmkprm_state_q (
							ViSession mvi, 
							ViPInt32 state
							){ return 0; }

ViStatus _VI_FUNC unmkprm_install_handler (
							ViSession mvi, 
							Unmkprm_EventHandler handler, 
							ViAddr userData
							){ return 0; }

ViStatus _VI_FUNC unmkprm_reset (
							ViSession mvi
							){ return 0; }

ViStatus _VI_FUNC unmkprm_self_test (
							ViSession mvi, 
							ViPInt16 result,
							ViChar _VI_FAR message[]
							){ return 0; }

ViStatus _VI_FUNC unmkprm_test_ok2 (
							ViSession mvi, 
							ViInt32 inpType, 
							ViPInt16 result, 
							ViChar _VI_FAR message[]
							){ return 0; }

ViStatus _VI_FUNC unmkprm_revision_query (
							ViSession mvi, 
							ViChar _VI_FAR sw_rev[], 
							ViChar _VI_FAR hw_rev[]
							){ return 0; }

ViStatus _VI_FUNC unmkprm_error_query (
							ViSession mvi, 
							ViPInt32 error,
							ViChar _VI_FAR message[]
							){ return 0; }

ViStatus _VI_FUNC unmkprm_error_message (
							ViSession mvi, 
							ViStatus error,
							ViChar _VI_FAR message[]
							){ return 0; }

ViStatus _VI_FUNC unmkprm_close (
							ViSession mvi
							){ return 0; }

#if defined(__cplusplus) || defined(__cplusplus__)
}
#endif