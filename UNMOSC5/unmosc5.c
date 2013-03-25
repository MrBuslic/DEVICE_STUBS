/************************************/
/* Copyright (C) 2012				*/
/* LLirik - kometa					*/
/* 									*/
/* All rights reserved.				*/
/************************************/
#include <unmosc5.h>

#if defined(__cplusplus) || defined(__cplusplus__)
extern "C" {
#endif
/****************************************************************************
		Функция инициализация сеанса с инструментом
*****************************************************************************/
ViStatus _VI_FUNC unmosc5_init (ViRsrc rsrcName, ViBoolean IDquery,
                                 ViBoolean doReset, ViSession *mvi){ return 0; }
ViStatus _VI_FUNC unmosc5_connect (ViSession mvi, ViSession vi, ViUInt16 m_num, ViBoolean IDquery,
                                 ViBoolean doReset){ return 0; }


/****************************************************************************
		Функции конфигурации
*****************************************************************************/
ViStatus _VI_FUNC unmosc5_ConfigInput  (ViSession 	mvi,	
								 		ViInt16   	Ch,	
								 		ViInt16 	state,
								 		ViInt16 	type,	
								 		ViInt16 	resist,
								 		ViInt16 	fnc){ return 0; }	
ViStatus _VI_FUNC unmosc5_ConfigInput_Q  (ViSession 	mvi,	
								 		ViInt16   	ch,	
								 		ViPInt16 	state,
								 		ViPInt16 	type,	
								 		ViPInt16 	resist,
								 		ViPInt16 	fnc){ return 0; }	
ViStatus _VI_FUNC unmosc5_ConfigAnalog  (ViSession 	mvi,	
								 		ViInt16   	Ch,	
								 		ViInt16   	mode,	
								 		ViReal64 	range,	
								 		ViReal64 	offset){ return 0; }
ViStatus _VI_FUNC unmosc5_ConfigAnalog_Q  (ViSession 	mvi,	
								 		ViInt16   	ch,	
								 		ViPReal64 	range,	
								 		ViPReal64 	offset){ return 0; }
ViStatus _VI_FUNC unmosc5_ConfigTrigger (ViSession 	vi,	
								 		ViInt16 	master, 
								 		ViInt16 	source, 
								 		ViInt16 	evtrg, 
								 		ViPReal64 	level1,
								 		ViPReal64 	level2){ return 0; }
ViStatus _VI_FUNC unmosc5_ConfigTrigger_Q (ViSession 	vi,	
								 		ViPInt16 	master, 
								 		ViPInt16 	source, 
								 		ViPInt16 	evtrg, 
								 		ViPReal64 	level1,
								 		ViPReal64 	level2){ return 0; }
ViStatus _VI_FUNC unmosc5_ConfigTime (ViSession 	mvi,	
		 							 ViPReal64 	dt,		
		 							 ViPReal64 	time,		
								 	 ViPReal64 	delay){ return 0; }  
ViStatus _VI_FUNC unmosc5_ConfigTime_Q (ViSession 	mvi,	
		 							 ViPReal64 	dt,		
		 							 ViPReal64 	time,		
								 	 ViPReal64 	delay){ return 0; }  

ViStatus _VI_FUNC unmosc5_ConfigGenerOC (ViSession vi, ViInt16 mode){ return 0; }
ViStatus _VI_FUNC unmosc5_ConfigGenerOC_Q (ViSession vi, ViPInt16 mode){ return 0; }
/****************************************************************************
		Функции чтения данных
*****************************************************************************/
ViStatus _VI_FUNC unmosc5_ReadWaveform_Real (ViSession arg0, ViInt16 arg1,
											ViPReal64 t0, ViPReal64 dT, ViPInt32 size, 
											ViPReal64 _VI_FAR buff){ return 0; }
ViStatus _VI_FUNC unmosc5_ReadWaveform_Real32 (ViSession arg0, ViInt16 arg1,
											ViPReal64 t0, ViPReal64 dT, ViPInt32 size, 
											ViPReal32 _VI_FAR buff){ return 0; }
ViStatus _VI_FUNC unmosc5_read(ViSession mvi, ViInt16 ch, ViPInt32 size, ViAddr buff){ return 0; }


/****************************************************************************
		Функции управления генератором
*****************************************************************************/
ViStatus _VI_FUNC unmosc5_setGenerator (ViSession mvi, 
								ViInt16 commut,
								ViInt16 mode,
								ViReal64 U0,
								ViReal64 Up,
								ViReal64 Un){ return 0; }
ViStatus _VI_FUNC unmosc5_offGenerator (ViSession mvi){ return 0; }
ViStatus _VI_FUNC unmosc5_setGenerator_Q (ViSession mvi, 
								ViPInt16 commut,
								ViPInt16 mode){ return 0; }
ViStatus _VI_FUNC unmosc5_setGeneratorCPU (ViSession mvi, 
								ViInt16 gd,
								ViInt16 commut,
								ViUInt16 U0,
								ViUInt16 Up,
								ViUInt16 Un){ return 0; }
/****************************************************************************
		Функции управления/состояния
*****************************************************************************/
ViStatus _VI_FUNC unmosc5_config(ViSession mvi){ return 0; } 
ViStatus _VI_FUNC unmosc5_trigger(ViSession mvi){ return 0; }
ViStatus _VI_FUNC unmosc5_stop(ViSession mvi){ return 0; }
ViStatus _VI_FUNC unmosc5_ready_Q(ViSession mvi, 
								 ViPUInt16	status){ return 0; }
ViStatus _VI_FUNC unmosc5_status_Q(ViSession mvi, 
								 ViPUInt16	state1,
								 ViPUInt16	state2,
								 ViPUInt16	state3){ return 0; }

/****************************************************************************
		Сервисные функции 
*****************************************************************************/
ViStatus _VI_FUNC unmosc5_reset (ViSession vi){ return 0; }
ViStatus _VI_FUNC unmosc5_self_test (ViSession 	vi, 
									ViPInt16 	test_result, 
									ViChar 		_VI_FAR test_message[]){ return 0; }
ViStatus _VI_FUNC unmosc5_testOK_off (ViSession 	vi, 
									ViInt16 	test_num, 
									ViPInt16 	test_result, 
									ViChar 		_VI_FAR test_message[]){ return 0; }
ViStatus _VI_FUNC unmosc5_error_query(ViSession vi, 
									ViPInt32 error, 
									ViChar _VI_FAR error_message[]){ return 0; }
ViStatus _VI_FUNC unmosc5_error_message (ViSession vi, 
									ViStatus error, 
									ViChar _VI_FAR message[]){ return 0; }
ViStatus _VI_FUNC unmosc5_revision_query (ViSession vi,
									ViChar _VI_FAR driver_revision[],
									ViChar _VI_FAR instrument_revision[]){ return 0; }


ViInt32 unmosc5_AnalysPulse (ViInt16 pulse, ViInt16 polar, ViReal64 *waveform, ViInt32 size,
							ViReal64 dt, ViReal64 tmin, ViReal64 porogT, 
							ViReal64 offset, ViReal64 porogU, 
                            ViAddr dataPulse, ViInt32 maxNumPulse){ return 0; }
/****************************************************************************
		Функция закрытия сеанса с инструментом
*****************************************************************************/
ViStatus _VI_FUNC unmosc5_close (ViSession vi){ return 0; }

//Ф-ии загрузки контроллеров
ViStatus _VI_FUNC unmosc5_readyPLD (ViSession vi){ return 0; }
ViStatus _VI_FUNC unmosc5_loadPLD (ViSession vi){ return 0; }
ViStatus _VI_FUNC unmosc5_prgPLD (ViSession vi){ return 0; }
ViStatus _VI_FUNC unmosc5_vrfPLD (ViSession vi){ return 0; }

//Технологические ф-ии чтения/записи калибровочных коэффициентов
ViStatus _VI_FUNC unmosc5_dataRPZU_Q (ViSession vi, ViAddr buff, ViPInt32 size){ return 0; }
ViStatus _VI_FUNC unmosc5_readDataRPZU (ViSession vi, ViAddr buff, ViInt32 size){ return 0; }
ViStatus _VI_FUNC unmosc5_writeDataRPZU (ViSession vi, ViAddr buff, ViInt32 size){ return 0; }
ViStatus _VI_FUNC unmosc5_testPLD (ViSession 	mvi, 
								ViChar 		_VI_FAR test_message[]){ return 0; }

ViStatus _VI_FUNC unmosc5_configACP (ViSession mvi, ViInt16 ch, ViInt16 kod){ return 0; }
ViStatus _VI_FUNC unmosc5_configPLL (ViSession mvi, ViInt16 full, ViInt16 kod1,  ViInt16 kod2,  ViInt16 kod3){ return 0; }
ViStatus _VI_FUNC unmosc5_setCPU  (ViSession mvi, ViInt16 ch, ViInt16 code1, ViInt16	code2){ return 0; }	
ViStatus _VI_FUNC unmosc5_setCPL  (ViSession mvi, ViUInt16 level1, ViUInt16 level2)	{ return 0; }
ViStatus _VI_FUNC unmosc5_ConfigAnalog_K  (ViSession 	mvi, ViInt16   	ch,	ViInt16 	cod,	ViInt16 	di)	{ return 0; }
ViStatus _VI_FUNC unmosc5_ConfigOffset  (ViSession 	mvi,	
								 		ViInt16   	Ch,	
								 		ViReal64 	range,	
								 		ViReal64 	offset){ return 0; }


#if defined(__cplusplus) || defined(__cplusplus__)
}
#endif