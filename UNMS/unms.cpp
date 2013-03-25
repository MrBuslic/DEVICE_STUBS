#include <unms.h>

#if defined(__cplusplus) || defined(__cplusplus__)
extern "C" {
#endif

//-----------------------------------------------------------------------------------
// Инициализация
//------------------------------------------------------------------------------------
ViStatus _VI_FUNC unms_init (ViRsrc rsrcName, ViBoolean IDquery,
								 ViBoolean doReset, ViSession *mezvi){ return 0; }
ViStatus _VI_FUNC unms_connect (ViSession mezvi, ViSession vi, ViUInt16 m_num, ViBoolean IDquery,
								 ViBoolean doReset){ return 0; }
//-----------------------------------------------------------------------------------
// Конфигурация
//------------------------------------------------------------------------------------
ViStatus _VI_FUNC unms_installHandler (ViSession mvi, ViPAttrState handler, ViPAttrState userData){ return 0; }

ViStatus _VI_FUNC unms_chan_cfg (ViSession mvi, ViUInt16 chan, ViBoolean slope,
								 ViBoolean tmr_start, ViBoolean out_trg){ return 0; }

ViStatus _VI_FUNC unms_irq_cfg (ViSession mvi, ViUInt16 chan, ViBoolean enable){ return 0; }

ViStatus _VI_FUNC unms_timer_cfg (ViSession mvi, ViUInt16 tick_len){ return 0; }

ViStatus _VI_FUNC unms_out_cfg (ViSession mvi, ViBoolean ttl_en, ViBoolean lvds_en){ return 0; }
//--------------------------------------------------------------------------------
// управление/состояние
//--------------------------------------------------------------------------------

ViStatus _VI_FUNC unms_start (ViSession mvi, ViBoolean auto_rest){ return 0; }

ViStatus _VI_FUNC unms_reset_timer (ViSession mvi){ return 0; }

ViStatus _VI_FUNC unms_timer_irq (ViSession mvi, ViUInt16 Ntick_len, ViBoolean enable){ return 0; }

ViStatus _VI_FUNC unms_out_pulse (ViSession mvi){ return 0; }

ViStatus _VI_FUNC unms_time_q (ViSession mvi, ViUInt16 chan, ViPUInt32 time_h,
							   ViPUInt32 time_l){ return 0; }
ViStatus _VI_FUNC unms_input_trigger (ViSession mvi, ViBoolean on){ return 0; }

ViStatus _VI_FUNC unms_irq_q (ViSession mvi, ViInt32 *irq){ return 0; }

ViStatus _VI_FUNC unms_start_count (ViSession mvi){ return 0; }
ViStatus _VI_FUNC unms_get_count (ViSession mvi, 
								ViPUInt32 cnt1_h, ViPUInt32 cnt1_l,
								ViPUInt32 cnt2_h, ViPUInt32 cnt2_l,
								ViPUInt32 cnt3_h, ViPUInt32 cnt3_l){ return 0; }
ViStatus _VI_FUNC unms_state_q (ViSession mvi, ViInt32 *irq, ViInt32 *state){ return 0; }


//--------------------------------------------------------------------------------
// служебные функции
//--------------------------------------------------------------------------------
ViStatus _VI_FUNC unms_irq_reset (ViSession mvi){ return 0; }
ViStatus _VI_FUNC unms_reset (ViSession mvi){ return 0; }
ViStatus _VI_FUNC unms_self_test (ViSession mvi, ViPInt16 result,
									ViChar _VI_FAR message[]){ return 0; }


ViStatus _VI_FUNCH unms_sft_interface (ViSession mvi,  ViPAttrState handler, ViPAttrState userData){ return 0; }
ViStatus _VI_FUNC unms_error_query (ViSession mvi, ViPInt32 error,
									  ViChar _VI_FAR message[]){ return 0; }
ViStatus _VI_FUNC unms_error_message (ViSession mvi, ViStatus status,
										ViChar _VI_FAR message[]){ return 0; }
ViStatus _VI_FUNC unms_revision_query (ViSession mvi, ViChar _VI_FAR drvVer[],
										 ViChar _VI_FAR instrVer[]){ return 0; }


ViStatus _VI_FUNC unms_close (ViSession mvi){ return 0; }



#if defined(__cplusplus) || defined(__cplusplus__)
}
#endif