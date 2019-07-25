#include <unmfsk24.h>
#include <socket_rpc.h>
#include <windows.h>
#include "mfsk24_rpc.h"
#include <rpc_ports.h>
#include "unmfsk24_h.h"

int mfsk_count = 0;

#if defined(__cplusplus) || defined(__cplusplus__)
extern "C" {
#endif

/*----------------------------------------------------------------------*/
/*  Функции                                                       */
/*----------------------------------------------------------------------*/
#ifdef UNMFSK24_OLD_INIT	
ViStatus _VI_FUNC unmfsk24_init (ViSession arg0, ViUInt16 arg1, ViBoolean arg2,
                              ViBoolean arg3, ViSession *arg4){ return 0; }
#else
ViStatus _VI_FUNC unmfsk24_init (ViRsrc rsrcName, ViBoolean IDquery,
                                 ViBoolean doReset, ViSession *mezvi){ 
	mfsk_count++;
	*mezvi = mfsk_count; 
	return 0;
}
ViStatus _VI_FUNC unmfsk24_connect (ViSession mezvi, ViSession vi, ViUInt16 m_num, ViBoolean IDquery,
                                 ViBoolean doReset){ return 0;}
#endif
ViStatus _VI_FUNC unmfsk24_set_cmd_time (ViSession line, ViInt16 chan,
										 ViReal64 time){
	return Srpc_buffer_class::Instance().mfsk24_slot_thr[line-1]->get_mfsk24_obj()->unmfsk24_set_cmd_time(chan, time);
}
ViStatus _VI_FUNC unmfsk24_config_trigger (ViSession arg0, ViUInt16 arg1){ return 0; }

ViStatus _VI_FUNC unmfsk24_cmd_time_q (ViSession arg0, ViInt16 arg1,
										   ViReal64 *arg2){ return 0; }

ViStatus _VI_FUNC unmfsk24_config_trigger_q (ViSession arg0, ViUInt16 *arg1){ return 0; }

ViStatus _VI_FUNC unmfsk24_start (ViSession line, ViInt16 _VI_FAR state[]){ 
	
	QVariantList tmp_state;
	if (state != NULL)
	{
		for (int i = 0; i < 24; i++)
			tmp_state << state[i];
	}
	return Srpc_buffer_class::Instance().mfsk24_slot_thr[line-1]->get_mfsk24_obj()->unmfsk24_start(tmp_state);
}

ViStatus _VI_FUNC unmfsk24_gstart_q (ViSession mvi, ViInt16 *state){ return 0; }

ViStatus _VI_FUNC unmfsk24_stop (ViSession arg0){ return 0; }

ViStatus _VI_FUNC unmfsk24_state (ViSession line, ViBoolean state[]){

	QVariantList tmp_state;
	Srpc_buffer_class::Instance().mfsk24_slot_thr[line-1]->get_mfsk24_obj()->unmfsk24_state(tmp_state);
	for (int i = 0; i < 24; i++)
		state[i] = tmp_state[i].toInt();
	return 0;
}

ViStatus _VI_FUNC unmfsk24_command(ViSession arg0, ViInt16 arg1, ViReal64 arg2){
	return 0;
}

ViStatus _VI_FUNC unmfsk24_group_command (ViSession arg0, ViReal64 arg1,
										  ViInt16 _VI_FAR arg2[]){ 
	return 0;
}
ViStatus _VI_FUNC unmfsk24_manual_cmd (ViSession line, ViInt16 chan, ViInt16 state_chan){ 
	return Srpc_buffer_class::Instance().mfsk24_slot_thr[line-1]->get_mfsk24_obj()->unmfsk24_manual_cmd(chan, state_chan);
}
ViStatus _VI_FUNC unmfsk24_manual_group_cmd (ViSession line, ViInt16 state_chan,
											 ViInt16 _VI_FAR mass[]){ 
	QVariantList tmp_mass;
	if (mass != NULL)
	{
		for (int i = 0; i < 24; i++)
			tmp_mass << mass[i];
	}
	return Srpc_buffer_class::Instance().mfsk24_slot_thr[line-1]->get_mfsk24_obj()->unmfsk24_manual_group_cmd(state_chan, tmp_mass);
}
ViStatus _VI_FUNC unmfsk24_reset (ViSession arg0){ return 0; }
ViStatus _VI_FUNC unmfsk24_self_test (ViSession arg0, ViPInt16 arg1, ViChar _VI_FAR arg2[]){ return 0; }
ViStatus _VI_FUNC unmfsk24_zmfsk_test (ViSession mvi, ViInt16 *result, ViChar testMessage[]){ return 0; }
ViStatus _VI_FUNC unmfsk24_sft_interface (ViSession arg0,  UNMEZONIN_SFT0_INTERFACE){ return 0; }

ViStatus _VI_FUNC unmfsk24_error_query (ViSession arg0, ViPInt32 arg1,
										ViChar _VI_FAR arg2[]){ return 0; }
ViStatus _VI_FUNC unmfsk24_error_message (ViSession arg0, ViStatus arg1,
										  ViChar _VI_FAR arg2[]){ return 0; }
ViStatus _VI_FUNC unmfsk24_revision_query (ViSession arg0, ViChar _VI_FAR arg1[],
										   ViChar _VI_FAR arg2[]){ return 0; }
ViStatus _VI_FUNC unmfsk24_close (ViSession arg0){ return 0; }

#if defined(__cplusplus) || defined(__cplusplus__)

}

#endif