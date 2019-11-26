#include <unmt8k4l.h>
#include <socket_rpc.h>
#include <windows.h>
#include "mt8k4l_rpc.h"
#include "unmt8k4l_h.h"

#if defined(__cplusplus) || defined(__cplusplus__)
extern "C" {
#endif

int mt8k4l_count = 0;

/*----------------------------------------------------------------------*/
/*  Функции                                                       */
/*----------------------------------------------------------------------*/
ViStatus _VI_FUNC unmt8k4l_init (ViRsrc rsrcName, ViBoolean IDquery,
								 ViBoolean doReset, ViSession *mezvi){
	mt8k4l_count++;
	*mezvi = mt8k4l_count;
	return 0;
	}
ViStatus _VI_FUNC unmt8k4l_connect (ViSession mezvi, ViSession vi, ViUInt16 m_num, ViBoolean IDquery,
								 ViBoolean doReset){return 0;}
//--------------------- Set signle mode -------------                               
ViStatus _VI_FUNC unmt8k4l_mode_1 (ViSession arg0){return 0;}

//--------------------- Set block mode -----------------------------
ViStatus _VI_FUNC unmt8k4l_mode_block (ViSession arg0, ViUInt32 arg1){return 0;}

//--------------------- Set cycle mode ------------------------------
ViStatus _VI_FUNC unmt8k4l_mode_cycle (ViSession arg0, ViUInt32 arg1){return 0;}

//---------------------- Query mode -------------------------------------------------
ViStatus _VI_FUNC unmt8k4l_mode_q (ViSession arg0, ViUInt16 *arg1,
								   ViUInt32 *arg2){return 0;}

//---------------------- Set sample period -----------------------
ViStatus _VI_FUNC unmt8k4l_sample_period (ViSession mvi, ViReal64 periodS,ViUInt16 ns){return 0;}

//---------------------- Query sample period -----------------------
ViStatus _VI_FUNC unmt8k4l_sample_period_q (ViSession arg0, ViReal64 *arg1,ViUInt16 *ns){return 0;}

//--------------------------
ViStatus _VI_FUNC unmt8k4l_input_trigger (ViSession mvi, ViBoolean state){
	return Srpc_buffer_class::Instance().mt8k4l_slot_thr[mvi - 1]->get_mt8k4l_obj()->unmt8k4l_input_trigger(state);
}

//-------------------------- Query
ViStatus _VI_FUNC unmt8k4l_input_trigger_q (ViSession arg0, ViBoolean *arg1){return 0;}

//----------------------- Allocate carrier memory ------------------
ViStatus _VI_FUNC unmt8k4l_alloc (ViSession arg0, ViUInt32 arg1){return 0;}

//----------------------- Query carrier memory --------------------
ViStatus _VI_FUNC unmt8k4l_alloc_q (ViSession arg0, ViUInt32 *arg1){return 0;}

//--------------------- Query sample width -----------------------------------------
ViStatus _VI_FUNC unmt8k4l_sample_width_q (ViSession mvi, ViPUInt16 nWords, ViPUInt16 nBytes){
	uint frame_width;
	uint width_in_bytes;
	Srpc_buffer_class::Instance().mt8k4l_slot_thr[mvi - 1]->get_mt8k4l_obj()->unmt8k4l_sample_width_q(frame_width, width_in_bytes);
	*nWords = frame_width;
	*nBytes = width_in_bytes;
	return 0;
}

//---------------------- Set mask interrupt --------------------------------
ViStatus _VI_FUNC unmt8k4l_mask_interrupt (ViSession mvi, ViBoolean enableFIFOover,
										  ViBoolean enableLineBreak,
										  ViBoolean enableBInterrupt){return 0;}
//---------------------- Query mask interrupt --------------------------------
ViStatus _VI_FUNC unmt8k4l_mask_interrupt_q (ViSession mvi, ViBoolean *enableFIFOover,
										  ViBoolean *enableLineBreak,
										  ViBoolean *enableBInterrupt){return 0;}
//---------------------- Set user interrupt handle -------------------------------
ViStatus _VI_FUNC unmt8k4l_set_user_ihandle (ViSession mvi, ViPAttrState handle) {return 0;}

//---------------------- Calibrate ------------------------
ViStatus _VI_FUNC unmt8k4l_displacement (ViSession arg0){return 0;}

//---------------------- Calibrate I ------------------------
ViStatus _VI_FUNC unmt8k4l_calibrI (ViSession arg0){return 0;}

//--------------------- Set config trigger -------------------------------------
ViStatus _VI_FUNC unmt8k4l_config_trigger (ViSession arg0, ViUInt16 arg1){return 0;}

//----------------------------------------
ViStatus _VI_FUNC unmt8k4l_start (ViSession mvi){
	return Srpc_buffer_class::Instance().mt8k4l_slot_thr[mvi - 1]->get_mt8k4l_obj()->unmt8k4l_start();
}

//----------------------------------------
ViStatus _VI_FUNC unmt8k4l_state (ViSession arg0, ViInt16 *arg1){return 0;}

//----------------------------------------
ViStatus _VI_FUNC unmt8k4l_stop (ViSession arg0){return 0;}

//--------------------- Query how much data is ready for read --------
ViStatus _VI_FUNC unmt8k4l_numReadyData (ViSession arg0, ViUInt32 *arg1){return 0;}

//--------------------- Read one sample --------------------------
ViStatus _VI_FUNC unmt8k4l_read_sample (ViSession mvi, ViPReal64 buf, 
					ViPUInt32 firstTime,ViPUInt32 thisTime){
	QVariantList  tmp_buf;
	uint _firstTime;
	uint _thisTime;
	Srpc_buffer_class::Instance().mt8k4l_slot_thr[mvi - 1]->get_mt8k4l_obj()->unmt8k4l_read_sample(tmp_buf, _firstTime, _thisTime);
	for (int i = 0; i < 8; i++)
		buf[i] = tmp_buf.at(i).toDouble();
	*firstTime = _firstTime;
	*thisTime = _thisTime;
	return 0;
}

//--------------------- Read data in block mode -----------------------------       
ViStatus _VI_FUNC unmt8k4l_read_block (ViSession arg0, ViUInt32 arg1,
									   ViUInt32 arg2, ViReal64 *arg3){return 0;}

//-------------------- Read packet of data in cycle mode ----------------------
ViStatus _VI_FUNC unmt8k4l_read_packet (ViSession arg0, ViUInt32 arg1,
										ViReal64 *arg2, ViUInt32 *arg3){return 0;}

//----------------------------------------
ViStatus _VI_FUNC unmt8k4l_reset (ViSession arg0){return 0;}

//----------------------------------------
ViStatus _VI_FUNC unmt8k4l_self_test (ViSession arg0, ViInt16 *arg1,
									  ViChar arg2[]){return 0;}

//----------------------------------------

ViStatus _VI_FUNC unmt8k4l_error_query (ViSession arg0, ViInt32 *arg1,
										ViChar arg2[]){return 0;}

//----------------------------------------
ViStatus _VI_FUNC unmt8k4l_error_message (ViSession arg0, ViStatus arg1,
										  ViChar arg2[]){return 0;}

//----------------------------------------
ViStatus _VI_FUNC unmt8k4l_revision_query (ViSession arg0, ViChar arg1[],
										   ViChar arg2[]){return 0;}

//----------------------------------------
ViStatus _VI_FUNC unmt8k4l_close (ViSession arg0){return 0;}


#if defined(__cplusplus) || defined(__cplusplus__)
}
#endif

