#include <unmn8i.h>


typedef ViStatus(_VI_FUNCH * UNMN8IIntHandle)(ViSession mvi,
	 ViInt32 reason, ViPBoolean levelUp,
	 ViPBoolean levelDown, ViAddr userdata);


UNMN8IIntHandle _interrupt_handle;

//--------------------------------
// Definitions of driver functions
//--------------------------------

//--------------------- Initialize --------------------------------------------
ViStatus _VI_FUNC unmn8i_init (ViRsrc rsrcName, ViBoolean IDquery,
                                 ViBoolean doReset, ViSession *mezvi){ return 0; }
ViStatus _VI_FUNC unmn8i_connect(ViSession mezvi, ViSession vi, ViUInt16 m_num, ViBoolean IDquery,
                                 ViBoolean doReset){ return 0; }

//--------------------- Set signle mode -------------                               
ViStatus _VI_FUNC unmn8i_mode_once (ViSession mvi){ return 0; }

//--------------------- Set block mode -----------------------------
ViStatus _VI_FUNC unmn8i_mode_block (ViSession mvi, ViUInt32 size){ return 0; }

//--------------------- Set cycle mode ------------------------------
ViStatus _VI_FUNC unmn8i_mode_cycle (ViSession mvi, ViUInt32 size){ return 0; }

//---------------------- Query mode -----------------------------
ViStatus _VI_FUNC unmn8i_mode_q (ViSession mvi, ViUInt16 *mode, ViUInt32 *size){ return 0; }

//----------------------- Allocate carrier memory ------------------
ViStatus _VI_FUNC unmn8i_alloc (ViSession mvi, ViUInt32 size){ return 0; }

//----------------------- Query carrier memory --------------------
ViStatus _VI_FUNC unmn8i_alloc_q (ViSession mvi, ViUInt32 *size){ return 0; }

//---------------------- level ---------------------------
ViStatus _VI_FUNC unmn8i_level (ViSession mvi, ViUInt16 chan, ViReal64 low, ViReal64 high){ return 0; }
ViStatus _VI_FUNC unmn8i_level_q (ViSession mvi, ViUInt16 chan, ViPReal64 low, ViPReal64 high){ return 0; }

//---------------------- range ---------------------------
ViStatus _VI_FUNC unmn8i_range (ViSession mvi, ViUInt16 chan, ViReal64 diap){ return 0; }
ViStatus _VI_FUNC unmn8i_range_q (ViSession mvi, ViUInt16 chan, ViReal64 *diap){ return 0; }
ViStatus _VI_FUNC unmn8i_mn6i_ranges_q (ViSession mvi, ViUInt16 chan, ViReal64 *diap){ return 0; }

//---------------------- Set sample period -----------------------
ViStatus _VI_FUNC unmn8i_sample_period (ViSession mvi, ViReal64 periodS){ return 0; }

//---------------------- Query sample period -----------------------
ViStatus _VI_FUNC unmn8i_sample_period_q (ViSession mvi, ViReal64 *periodS,
                                         ViUInt16 *nSum){ return 0; }
ViStatus _VI_FUNC unmn8i_num_sum (ViSession mvi, ViUInt16 nSums){ return 0; }

//-------------------------- Set
ViStatus _VI_FUNC unmn8i_input_trigger (ViSession mvi, ViBoolean on){ return 0; }

ViStatus _VI_FUNC unmn8i_mn6i_input_trigger (ViSession mvi, ViUInt16 chan,
                                             ViBoolean on){ return 0; }

//-------------------------- Query
ViStatus _VI_FUNC unmn8i_input_trigger_q (ViSession mvi, ViBoolean *on){ return 0; }


//--------------------- Query sample width -----------------------------------------
ViStatus _VI_FUNC unmn8i_sample_width_q (ViSession mvi, ViUInt16 *widthWord,
	ViUInt16 *widthByte){
	*widthWord = 8; return 0;
}

//---------------------- Set mask interrupt --------------------------------
ViStatus _VI_FUNC unmn8i_mask_interrupt (ViSession mvi, ViBoolean enableFIFO,
										ViBoolean enableOverLow,
										ViBoolean enableOverHigh, 
										ViBoolean enableBInterrupt){ return 0; }
ViStatus _VI_FUNC unmn8i_mask_interrupt_chan (ViSession mvi, ViUInt32 chan,
										ViBoolean enableOverLow, ViBoolean enableOverHigh){ return 0; }

//---------------------- Query interrupt --------------------------------
ViStatus _VI_FUNC unmn8i_mask_interrupt_q (ViSession mvi, ViBoolean *enableFIFO,
										  ViBoolean enableOverLow[],
										  ViBoolean enableOverHigh[],
										  ViBoolean *enableBInterrupt){ return 0; }

ViStatus _VI_FUNC unmn8i_overlevel_q(ViSession mvi, ViBoolean LevelUp[], ViBoolean LevelDown[]){ return 0; }




ViStatus _VI_FUNC unmn8i_installHandler (ViSession mvi, ViPAttrState handle, ViPAttrState userData)
{
	_interrupt_handle = reinterpret_cast<UNMN8IIntHandle>(handle);
	(*_interrupt_handle)(mvi, UNMN8I_IRQ_PACKET_READY, 0, 0, 0);
	return 0; 
}

ViStatus _VI_FUNC unmn8i_reset_flag (ViSession mvi){ return 0; }

//--------------------- Set config trigger -------------------------------------
ViStatus _VI_FUNC unmn8i_config_trigger (ViSession mvi, ViUInt16 trig){ return 0; }
ViStatus _VI_FUNC unmn8i_config_trigger_q (ViSession mvi, ViPUInt16 group){ return 0; }

//--------------------- Get attribute ------------------------------------------
ViStatus _VI_FUNC unmn8i_get_attribute (ViSession mvi, ViInt32 attr, ViPAttrState value){ return 0; }

//----------------------------------------
ViStatus _VI_FUNC unmn8i_start (ViSession mvi){ return 0; }

//----------------------------------------
ViStatus _VI_FUNC unmn8i_state (ViSession mvi, ViInt16 *stateParam, ViInt16 *errState){ return 0; }

//----------------------------------------
ViStatus _VI_FUNC unmn8i_stop (ViSession mvi){ return 0; }

//--------------------- Read one sample --------------------------
ViStatus _VI_FUNC unmn8i_read_sample (ViSession mvi, ViPReal64 buf, 
                    ViPUInt32 firstTime,ViPUInt32 thisTime){ return 0; }

//--------------------- Read data in block mode -----------------------------       
ViStatus _VI_FUNC unmn8i_read_block (ViSession mvi, ViUInt32 firstSample,
                    ViUInt32 numSamples,ViReal64* buf){ return 0; }

//-------------------- Read packet of data in cycle mode ----------------------
ViStatus _VI_FUNC unmn8i_read_packet (ViSession mvi, ViBoolean isHot, ViUInt32 numSamples,
										ViReal64 *buf, ViUInt32 *realNumSamples){ return 0; }
ViStatus _VI_FUNC unmn8i_read_packet_float (ViSession mvi, ViBoolean isHot, ViUInt32 numSamples,
										ViReal32 *buf, ViUInt32 *realNumSamples){ return 0; }
//--------------------- Query how much data is ready for read --------
ViStatus _VI_FUNC unmn8i_num_ready_data (ViSession mvi, ViUInt32 *num){ return 0; }

//---------------------- Calibrate ------------------------
ViStatus _VI_FUNC unmn8i_displacement (ViSession mvi){ return 0; }
ViStatus _VI_FUNC unmn8i_displacement_range (ViSession mvi){ return 0; }
//----------------------------------------
ViStatus _VI_FUNC unmn8i_reset (ViSession mvi){ return 0; }


ViStatus _VI_FUNC unmn8i_self_test (ViSession mvi, ViInt16 *result, ViChar message[]){ return 0; }
ViStatus _VI_FUNC unmn8i_sft_interface (ViSession mvi, ViPAttrState sftInterface, ViPAttrState userData){ return 0; }

//----------------------------------------
ViStatus _VI_FUNC unmn8i_error_query (ViSession mvi, ViInt32 *err, ViChar message[]){ return 0; }

//----------------------------------------
ViStatus _VI_FUNC unmn8i_error_message (ViSession mvi, ViStatus status,
                                       ViChar message[]){ return 0; }

//----------------------------------------
ViStatus _VI_FUNC unmn8i_revision_query (ViSession mvi, ViChar verSoft[],
                                        ViChar verHard[]){ return 0; }

//----------------------------------------
ViStatus _VI_FUNC unmn8i_close (ViSession mvi){ return 0; }



///ҫ禡 㮳殭饠騯//
ViStatus _VI_FUNC unmn8i_extcalibr (ViSession mvi, ViReal64 stdU){ return 0; }

ViStatus _VI_FUNC unmn8i_readFlash(ViSession mvi){ return 0; }
ViStatus _VI_FUNC unmn8i_writeFlash(ViSession mvi){ return 0; }

ViStatus _VI_FUNC unmn8i_resetCoeff(ViSession mvi){ return 0; }

