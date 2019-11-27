#include <unmn3i.h>

//--------------------------------
// Definitions of driver functions
//--------------------------------

//--------------------- Initialize --------------------------------------------
ViStatus _VI_FUNC unmn3i_init (ViRsrc rsrcName, ViBoolean IDquery,
								 ViBoolean doReset, ViSession *mezvi){ return 0; }
ViStatus _VI_FUNC unmn3i_connect(ViSession mezvi, ViSession vi, ViUInt16 m_num, ViBoolean IDquery,
								 ViBoolean doReset){ return 0; }

//--------------------- Set signle mode -------------                               
ViStatus _VI_FUNC unmn3i_mode_once (ViSession mvi){ return 0; }

//--------------------- Set block mode -----------------------------
ViStatus _VI_FUNC unmn3i_mode_block (ViSession mvi, ViUInt32 size){ return 0; }

//--------------------- Set cycle mode ------------------------------
ViStatus _VI_FUNC unmn3i_mode_cycle (ViSession mvi, ViUInt32 size){ return 0; }

//---------------------- Query mode -----------------------------
ViStatus _VI_FUNC unmn3i_mode_q (ViSession mvi, ViUInt16 *mode, ViUInt32 *size){ return 0; }

//----------------------- Allocate carrier memory ------------------
ViStatus _VI_FUNC unmn3i_alloc (ViSession mvi, ViUInt32 size){ return 0; }

//----------------------- Query carrier memory --------------------
ViStatus _VI_FUNC unmn3i_alloc_q (ViSession mvi, ViUInt32 *size){ return 0; }

//---------------------- Query range ---------------------------
ViStatus _VI_FUNC unmn3i_range_q (ViSession mvi, ViUInt16 chan, ViReal64 *range){ return 0; }

//---------------------------------------------------------------------------
ViStatus _VI_FUNC unmn3i_config_chans (ViSession mvi, ViUInt16 mask){ return 0; }

//---------------------- Set sample period -----------------------
ViStatus _VI_FUNC unmn3i_sample_period (ViSession mvi, ViReal64 periodS){ return 0; }

//---------------------- Query sample period -----------------------
ViStatus _VI_FUNC unmn3i_sample_period_q (ViSession mvi, ViReal64 *periodS){ return 0; }
//-------------------------- Set
ViStatus _VI_FUNC unmn3i_input_trigger (ViSession mvi, ViUInt16 chan, ViBoolean state){ return 0; }

//-------------------------- Query
ViStatus _VI_FUNC unmn3i_input_trigger_q (ViSession mvi, ViUInt16 chan, ViBoolean *state){ return 0; }

//------------------------------------------------------------------------------------
ViStatus _VI_FUNC unmn3i_setXchgType (ViSession mvi, ViInt32 type){ return 0; }

//--------------------- Query sample width -----------------------------------------
ViStatus _VI_FUNC unmn3i_sample_width_q (ViSession mvi, ViUInt16 nChans,
	ViUInt16 *widthW, ViUInt16 *widthB) {
	*widthW = 2;
	return 0;
}

//---------------------- Set mask interrupt --------------------------------
ViStatus _VI_FUNC unmn3i_config_events (ViSession mvi, ViBoolean data,
                                        ViBoolean over){ return 0; }

//---------------------- Query interrupt --------------------------------
ViStatus _VI_FUNC unmn3i_config_events_q (ViSession mvi, ViPBoolean data,
                                          ViPBoolean over){ return 0; }


//---------------------- Set user interrupt handle -------------------------------

ViStatus _VI_FUNC unmn3i_installHandler (ViSession mvi, ViPAttrState handle, ViPAttrState userData){ return 0; }

//---------------------- Calibrate ------------------------
ViStatus _VI_FUNC unmn3i_displacement (ViSession mvi){ return 0; }

//--------------------- Set config trigger -------------------------------------
ViStatus _VI_FUNC unmn3i_config_trigger (ViSession mvi, ViUInt16 trig){ return 0; }
ViStatus _VI_FUNC unmn3i_config_trigger_q (ViSession mvi, ViPUInt16 group){ return 0; }

//----------------------------------------
ViStatus _VI_FUNC unmn3i_start (ViSession mvi){ return 0; }

//----------------------------------------
ViStatus _VI_FUNC unmn3i_state (ViSession mvi, ViInt16 *stateParam, ViInt16 *errState){ return 0; }

//----------------------------------------
ViStatus _VI_FUNC unmn3i_stop (ViSession mvi){ return 0; }

//--------------------- Read one sample --------------------------
ViStatus _VI_FUNC unmn3i_read_sample (ViSession mvi, ViPReal64 buf, 
					ViPUInt32 firstTime,ViPUInt32 thisTime){ return 0; }

//--------------------- Read data in block mode -----------------------------       
ViStatus _VI_FUNC unmn3i_read_block (ViSession mvi, ViUInt32 firstSample,
					ViUInt32 numSamples,ViReal64* buf){ return 0; }

//-------------------- Read packet of data in cycle mode ----------------------
ViStatus _VI_FUNC unmn3i_read_packet (ViSession mvi, ViBoolean isHot, ViUInt32 numSamples,
										ViReal64 *buf, ViUInt32 *realNumSamples){ return 0; }

ViStatus _VI_FUNC unmn3i_read_packet_float (ViSession mvi, ViBoolean hot,
                                            ViUInt32 n, ViReal32 *buf,
                                            ViUInt32 *nr){ return 0; }

ViStatus _VI_FUNC unmn3i_reset_flag (ViSession arg0){ return 0; }

//--------------------- Query how much data is ready for read --------
ViStatus _VI_FUNC unmn3i_num_ready_data (ViSession mvi, ViUInt32 *num){ return 0; }

//----------------------------------------
ViStatus _VI_FUNC unmn3i_reset (ViSession mvi){ return 0; }

//----------------------------------------
ViStatus _VI_FUNC unmn3i_self_test (ViSession mvi, ViInt16 *result, ViChar message[]){ return 0; }
ViStatus _VI_FUNC unmn3i_sft_interface (ViSession mvi, ViPAttrState sftInterface, ViPAttrState userData){ return 0; }

//----------------------------------------
ViStatus _VI_FUNC unmn3i_error_query (ViSession mvi, ViInt32 *err, ViChar message[]){ return 0; }

//----------------------------------------
ViStatus _VI_FUNC unmn3i_error_message (ViSession mvi, ViStatus status,
									   ViChar message[]){ return 0; }

//----------------------------------------
ViStatus _VI_FUNC unmn3i_revision_query (ViSession mvi, ViChar verSoft[],
										ViChar verHard[]){ return 0; }

//----------------------------------------
ViStatus _VI_FUNC unmn3i_close (ViSession mvi){ return 0; }


///ҫ禡 㮳殭饠騯//
ViStatus _VI_FUNC unmn3i_extcalibr (ViSession mvi, ViInt32 chan, ViInt32 size, ViReal64 stdU){ return 0; }

ViStatus _VI_FUNC unmn3i_readFlash(ViSession mvi){ return 0; }
ViStatus _VI_FUNC unmn3i_writeFlash(ViSession mvi){ return 0; }
ViStatus _VI_FUNC unmn3i_get_attribute (ViSession mvi, ViInt32 attr, ViPAttrState value){ return 0; }
ViStatus _VI_FUNC unmn3i_resetCoeff(ViSession mvi){ return 0; }
