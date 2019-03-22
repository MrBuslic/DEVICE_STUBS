#include <unols.h>
#include "unols_h.h"
#include <windows.h>

rpc_buffer_class::rpc_buffer_class()
{
	for (int i = 0; i < 1; i++)
	{
		RPC_ols_SLOT_Thread* slot_thr = new RPC_ols_SLOT_Thread;
		slot_thr->set_connection_params("127.0.0.1", 31070 + i);
		slot_thr->start();
		//if (!slot_thr.wait_connected(3))
		//	return false;
		RPC_ols_SIGNAL_Thread* signal_thr = new RPC_ols_SIGNAL_Thread;
		signal_thr->set_connection_params("127.0.0.1", 31075 + i);
		signal_thr->start();
		//bool res = signal_thr->wait_connected(5);


		ols_slot_thr.push_back(slot_thr);
		ols_signal_thr.push_back(signal_thr);
	}
}


// Объявляем функцию DllMain

BOOL APIENTRY DllMain(HINSTANCE hinstDLL,
	DWORD fdwReason, LPVOID lpvReserved)
{
	Srpc_buffer_class::Instance();
	switch (fdwReason)      // Дерево разбора уведомлений
	{
	case DLL_PROCESS_ATTACH: // Подключение DLL
							 //MessageBox(NULL,"Подключение Заглушки UNMN8I для Мезонина МН8И","Использование заглушек!", MB_ICONINFORMATION);

							 //if (lpvReserved)  // Определение способа загрузки
							 // MessageBox(NULL,"DLL загружена с неявной компоновкой","Использование заглушек!", MB_ICONINFORMATION);
							 //else
							 //MessageBox(NULL,"DLL загружена с явной компоновкой","Использование заглушек!", MB_ICONINFORMATION);
							 //return 1; // успешная инициализация

		break;

	case DLL_PROCESS_DETACH: // Отключение DLL
							 // Здесь – освобождаем память, закрываем
							 // файлы и т.д.
		break;

	case DLL_THREAD_ATTACH: // Уведомление о новом потоке 
							// Здесь – если надо переходим на
							// многопоточный режим работы с
							// использованием средств синхронизации
							// таких как критическая секция, мутанты,
							// семафоры и т.д.
		break;

	case DLL_THREAD_DETACH:
		//Уведомление о завершении потока
		// Здесь – если надо освобождаем все ресурсы, 
		// вязанные с завершившимся потоком. Какой именно
		// поток завершился можно узнать просмотром списка
		// потоков средствами TOOLHELP32
		//MessageBox(NULL,"Использование заглушек!","Завершение потока", MB_ICONINFORMATION);
		break;

	}
	return TRUE;    // Код возврата игнорируется
}

#if defined(__cplusplus) || defined(__cplusplus__)
extern "C" {
#endif




/****************************************************************************
		Функция инициализация сеанса с инструментом
*****************************************************************************/
ViStatus _VI_FUNC unols_init (	ViRsrc 		rsrcName, 
								ViBoolean 	id_query,
								ViBoolean 	reset, 
								ViPSession 	vi){ return 0; }
/****************************************************************************
		Функции конфигурации
*****************************************************************************/
ViStatus _VI_FUNC unols_set_synhro_mode (ViSession vi, ViInt16 mode){ return 0; } 

ViStatus _VI_FUNC unols_synhro_gener (ViSession vi, ViInt16 devise,  ViUInt16 sourse,
                                      ViBoolean front, ViUInt16 diT, ViReal64 T){ return 0; }

ViStatus _VI_FUNC unols_synhro_trigger (ViSession vi, ViInt16 devise, ViUInt16 sourse){ return 0; }

ViStatus _VI_FUNC unols_synhro_stop (ViSession vi, ViInt16 devise, ViUInt16 sourse){ return 0; }

ViStatus _VI_FUNC unols_config_trigger (ViSession vi, ViInt16 devise, 
                                        ViUInt32 _VI_FAR event[],
                                        ViUInt32 _VI_FAR chanEvent[]){ return 0; }

ViStatus _VI_FUNC unols_config_stop (ViSession vi, ViInt16 devise, 
                                     ViUInt32 _VI_FAR event[],
                                     ViUInt32 _VI_FAR chanEvent[]){ return 0; }

ViStatus _VI_FUNC unols_set_mode (ViSession vi, ViInt16 devise, ViInt16 mode, ViBoolean endless, ViBoolean stepMode){ return 0; }

ViStatus _VI_FUNC unols_config_mode (ViSession vi, ViUInt16 devise,  
                                     ViUInt16 packs, ViUInt32 _VI_FAR periodPack[], 
                                     ViUInt32 _VI_FAR seriesPack[]){ return 0; }
ViStatus _VI_FUNC unols_config_preTrig (ViSession vi, ViUInt16 statePreTrig){ return 0; }

ViStatus _VI_FUNC unols_config_channel (ViSession vi, ViInt16 devise,
                                        ViUInt32 _VI_FAR stateChans[],
                                        ViUInt32 _VI_FAR chans[],
										ViUInt16 defState){ return 0; }

ViStatus _VI_FUNC unols_config_Uout (ViSession vi, ViReal64 _VI_FAR U[]){ return 0; } 

ViStatus _VI_FUNC unols_config_inpExt (ViSession vi, ViInt16 sourse,  ViInt16 st){ return 0; }  

ViStatus _VI_FUNC unols_config_handler (ViSession vi,  ViAddr userCallback, ViUInt16 regEvent,
                                        ViUInt16 genEvent, ViUInt16 errorEvent){ return 0; }

ViStatus _VI_FUNC unols_set_synhro_mode_Q (ViSession vi, ViPInt16 mode){ return 0; }

ViStatus _VI_FUNC unols_synhro_gener_Q (ViSession vi, ViInt16 devise, ViPUInt16 sourse,
                                        ViPBoolean front, ViPUInt16 diT, ViPReal64 T){ return 0; }


ViStatus _VI_FUNC unols_synhro_trigger_Q (ViSession vi, ViInt16 devise, ViPUInt16 sourse){ return 0; }

ViStatus _VI_FUNC unols_synhro_stop_Q (ViSession vi, ViInt16 devise, ViPUInt16 sourse){ return 0; }

ViStatus _VI_FUNC unols_config_trigger_Q (ViSession vi, ViInt16 devise,
                                       	  ViUInt32 _VI_FAR event[],
                                          ViUInt32 _VI_FAR chanEvent[]){ return 0; }

ViStatus _VI_FUNC unols_config_stop_Q (ViSession vi, ViInt16 devise,
                                       	  ViUInt32 _VI_FAR event[],
                                          ViUInt32 _VI_FAR chanEvent[]){ return 0; }

ViStatus _VI_FUNC unols_set_mode_Q (ViSession vi, ViInt16 devise, ViPInt16 mode, ViPBoolean endless, ViPBoolean stepMode){ return 0; }

ViStatus _VI_FUNC unols_config_handler_Q (ViSession vi,  ViPAddr userCallback, ViPUInt16 regEvent,
                                        ViPUInt16 genEvent, ViPUInt16 errorEvent){ return 0; }
ViStatus _VI_FUNC unols_config_channel_Q (ViSession vi, ViInt16 devise,
                                        ViUInt32 _VI_FAR stateChans[],
										ViPUInt16 defState){ return 0; }
ViStatus _VI_FUNC unols_config_Uout_Q (ViSession vi, ViReal64 _VI_FAR U[]){ return 0; }
ViStatus _VI_FUNC unols_config_mode_Q (ViSession vi, ViUInt16 devise, ViPUInt16 packs,
                                       ViUInt32 _VI_FAR periodPacks[], 
									   ViUInt32 _VI_FAR seriesPacks[]){ return 0; }
ViStatus _VI_FUNC unols_config_preTrig_Q (ViSession vi, ViPUInt16 statePreTrig){ return 0; }   
ViStatus _VI_FUNC unols_config_inpExt_Q (ViSession vi,  ViPInt16 st){ return 0; }     


/****************************************************************************
		Функции управления/состояния
*****************************************************************************/
ViStatus _VI_FUNC unols_trigger (ViSession vi, ViInt16 devise){
	if (devise == 2) {
		Srpc_buffer_class::Instance().ols_slot_thr[vi - 1]->get_ols_obj()->unols_write_data_kf(Srpc_buffer_class::Instance().ols_buffer, Srpc_buffer_class::Instance().mask_buffer);
		
	}
	return 0; 
}  //todo (передача 2 буферов в rpc)

ViStatus _VI_FUNC unols_trigger_imm (ViSession vi, ViInt16 devise){
	if (devise == 2) {//Generator
		Srpc_buffer_class::Instance().ols_slot_thr[vi - 1]->get_ols_obj()->unols_trigger_imm();
	}
	return 0;
}     //todo

ViStatus _VI_FUNC unols_stop (ViSession vi, ViInt16 devise){ return 0; }   
ViStatus _VI_FUNC unols_reset_status (ViSession vi, ViInt16 devise, ViUInt16 maskEvent, ViUInt16 errorEvent){ return 0; } 
ViStatus _VI_FUNC unols_reset_DRAM (ViSession vi, ViInt16 devise, ViUInt32 period, ViUInt32 offsetData){ return 0; }
ViStatus _VI_FUNC unols_status_Q (ViSession vi, ViInt16 devise,  
                                  ViPUInt16  stateDev, ViPUInt16 eventDev, ViPUInt16 errDev){ 
	//*stateDev = unols_state;
	*stateDev = 0;
	return 0; } // todo
/****************************************************************************
		 Данные
*****************************************************************************/
ViStatus _VI_FUNC unols_countData (ViSession vi, ViInt16 devise, ViPUInt32 count){ return 0; }  

ViStatus _VI_FUNC unols_write_allData (ViSession vi, ViInt16 devise, ViUInt32 period,
                                       ViUInt32 offsetData, void* data){ return 0; }   
ViStatus _VI_FUNC unols_read_allData (ViSession vi, ViInt16 devise, ViUInt32 period,
                                    ViUInt32 offsetData, void* data){ return 0; }	 
ViStatus _VI_FUNC unols_read_dataKF (ViSession vi, ViUInt32 period,    
                                    ViUInt32 offsetData, void* data, void* maska){ return 0; }									
ViStatus _VI_FUNC unols_write_dataKF (ViSession vi, ViUInt32 period,   
                                    ViUInt32 offsetData, void* data, void* maska){
	//как определить число каналов?
	Srpc_buffer_class::Instance().put_data(period, data, maska);
//	kprd_list.put_data(period, data, maska);
	return 0; }
ViStatus _VI_FUNC unols_write_DataOZU (ViSession vi, ViInt16 devise, ViUInt32 period,
                                       ViUInt32 offsetData, void* data){ return 0; }   
ViStatus _VI_FUNC unols_read_DataOZU (ViSession vi, ViInt16 devise, ViUInt32 period,
                                    ViUInt32 offsetData, void* data){ return 0; }	 
/****************************************************************************
		Сервисные функции 
*****************************************************************************/
ViStatus _VI_FUNC unols_reset (ViSession vi){return 0;}
ViStatus _VI_FUNC unols_self_test (ViSession 	vi, 
									ViPInt16 	test_result, 
									ViChar 		_VI_FAR test_message[]){return 0;}
ViStatus _VI_FUNC unols_testOK_off_1 (ViSession 	vi, 
									ViPInt16 	test_result, 
									ViChar 		_VI_FAR test_message[]){return 0;}
ViStatus _VI_FUNC unols_testOK_off_2 (ViSession 	vi, 
									ViPInt16 	test_result, 
									ViChar 		_VI_FAR test_message[]){return 0;}
ViStatus _VI_FUNC unols_mTestOK_off_3 (ViSession 	vi, 
									  ViPInt16 	test_result, 
									  ViChar 		_VI_FAR test_message[]){return 0;}
ViStatus _VI_FUNC unols_mTestOK_off_4 (ViSession 	vi, 
									  ViPInt16 	test_result, 
									  ViChar 		_VI_FAR test_message[]){return 0;}
ViStatus _VI_FUNC unols_error_query(ViSession vi, 
									 ViPInt32 error, 
									 ViChar _VI_FAR error_message[]){return 0;}
ViStatus _VI_FUNC unols_error_message (ViSession vi, 
										ViStatus error, 
										ViChar _VI_FAR message[]){return 0;}
ViStatus _VI_FUNC unols_revision_query (ViSession vi,
										ViChar _VI_FAR driver_revision[],
										ViChar _VI_FAR instrument_revision[]){return 0;}



/*Внутренняя  функция*/
ViStatus _VI_FUNC unols_mConfig_mode_KF2 (ViSession vi, ViUInt16 packs, ViUInt16 packsEnd,
	                                        ViUInt32 _VI_FAR periodPacks){return 0;}	
// режим работы -3 
ViStatus _VI_FUNC unols_mSet_modeKF   (ViSession vi,  ViInt32 mode){return 0;}   //    статусы ++
ViStatus _VI_FUNC unols_mSet_modeKF_Q (ViSession vi,  ViPInt32 mode){return 0;} //++   статусы ++   
// параметры обмена (количество кадров)
ViStatus _VI_FUNC unols_mConfig_modeKF   (ViSession vi,  ViInt32 packs){return 0;}	   // статусы ++ 
ViStatus _VI_FUNC unols_mConfig_modeKF_Q (ViSession vi,  ViPInt32 packs){return 0;}// ++  // статусы ++
// источник  запуска
ViStatus _VI_FUNC unols_mSynhro_triggerKF  (ViSession vi,  ViInt32 sourse){return 0;}
ViStatus _VI_FUNC unols_mSynhro_triggerKF_Q (ViSession vi,  ViPInt32 sourse){return 0;}
// прерывание
ViStatus _VI_FUNC unols_mConfig_handlerKF (ViSession vi,  ViAddr userCallback, ViUInt32 genEvent){return 0;}     // статусы ++ 
ViStatus _VI_FUNC unols_mConfig_handlerKF_Q (ViSession vi,  ViPAddr userCallback, ViPUInt32 genEvent){return 0;}//++   // статусы ++ 

/****************************************************************************

	Функции конфигурации режима регистрации
	
*****************************************************************************/   
// установка частоты
ViStatus _VI_FUNC unols_mSet_freqKR   (ViSession vi,  ViInt32 freq){return 0;}   //    статусы ++  
ViStatus _VI_FUNC unols_mSet_freqKR_Q (ViSession vi,  ViPInt32 freq){return 0;}//++   //    статусы ++  
// параметры обмена (количество кадров)
ViStatus _VI_FUNC unols_mConfig_modeKR   (ViSession vi,  ViInt32 packs){return 0;} 	 //    статусы ++  
ViStatus _VI_FUNC unols_mConfig_modeKR_Q (ViSession vi,  ViPInt32 packs){return 0;}    //    статусы ++  
// источник  запуска
ViStatus _VI_FUNC unols_mSynhro_triggerKR  (ViSession vi,  ViInt32 sourse){return 0;}  
ViStatus _VI_FUNC unols_mSynhro_triggerKR_Q (ViSession vi,  ViPInt32 sourse){return 0;}//++   
// прерывание
ViStatus _VI_FUNC unols_mConfig_handlerKR (ViSession vi,  ViAddr userCallback, ViUInt32 regEvent){return 0;} //    статусы ++  
ViStatus _VI_FUNC unols_mConfig_handlerKR_Q (ViSession vi, ViPAddr userCallback, ViPUInt32 regEvent){return 0;}//++		//    статусы ++  
 
// Запрос количества принятых (зарегистрированных) пакетов
ViStatus _VI_FUNC unols_mPacksKR_Q (ViSession vi, ViPUInt32 packs){return 0;} 
/****************************************************************************
		Функции управления/состояния
*****************************************************************************/
ViStatus _VI_FUNC unols_mStart (ViSession vi, ViInt32 devise){return 0;}  
ViStatus _VI_FUNC unols_mStop (ViSession vi, ViInt32 devise){return 0;}   
ViStatus _VI_FUNC unols_mReset_status (ViSession vi, ViInt32 devise, ViUInt32 maskEvent){return 0;} 
ViStatus _VI_FUNC unols_mReset_DDR (ViSession vi, ViInt32 devise){return 0;} 
ViStatus _VI_FUNC unols_mStatus_Q (ViSession vi, ViInt32 devise,  
								  ViPUInt32  stateDev, ViPUInt32 eventDev, ViPUInt32 errDev){return 0;}
ViStatus _VI_FUNC unols_mStateDDR_KF_Q (ViSession vi, ViPUInt32 packs, ViPUInt32 adr){return 0;} 							
							
ViStatus _VI_FUNC unols_mPacksKF_Q (ViSession vi, ViPUInt32 packs){return 0;}

// Запрос типа ПО загруженного в модуль
ViStatus _VI_FUNC unols_mModuleType_Q (ViSession vi, ViPUInt32 type){return 0;}
	
/****************************************************************************
		 Данные
*****************************************************************************/
ViStatus _VI_FUNC unols_mWrite_dataKF (ViSession vi,ViChar _VI_FAR file_name[], ViPUInt32 adr){return 0;} 
// Чтение данных из ОЗУ ПРИЕМА 
ViStatus _VI_FUNC unols_mRead_dataKR (ViSession vi, ViUInt32 packs, ViChar _VI_FAR directory_name[]){return 0;}


/****************************************************************************
		Функция закрытия сеанса с инструментом
*****************************************************************************/
ViStatus _VI_FUNC unols_close (ViSession vi){return 0;}

/****************************************************************************          
		Функции загрузки контроллеров
*****************************************************************************/ 
ViStatus _VI_FUNC unols_readyPLD (ViSession vi){return 0;}
ViStatus _VI_FUNC unols_loadPLD (ViSession vi){return 0;}
ViStatus _VI_FUNC unols_mPrgPLD (ViSession vi, ViInt32 type){return 0;}  
ViStatus _VI_FUNC unols_prgPLD (ViSession vi){return 0;}
ViStatus _VI_FUNC unols_mVrfPLD (ViSession vi, ViInt32 type){return 0;} 
ViStatus _VI_FUNC unols_vrfPLD (ViSession vi){return 0;}
ViStatus _VI_FUNC unols_cmpVersions (ViSession vi){return 0;}
ViStatus _VI_FUNC unols_readFlashVersion (ViSession vi, ViChar _VI_FAR flash_rev[]){return 0;}
ViStatus _VI_FUNC unols_mRAM_KR_Q(ViSession vi, ViPInt32 state){return 0;}
ViStatus _VI_FUNC unols_mRAM_KF_Q(ViSession vi, ViPInt32 state){return 0;} 

/****************************************************************************

	Технологические функции 

*****************************************************************************/ 
ViStatus _VI_FUNC unols_synhro_gener_ext (ViSession vi, ViInt16 state, ViUInt16 diT, ViReal64 T){return 0;}   // Установка  внешней частоты      
ViStatus _VI_FUNC unols_synhro_gener_ext_Q (ViSession vi, ViPInt16 state,ViPUInt16 diT, ViPReal64 T){return 0;}

ViStatus _VI_FUNC unols_trigger_ext (ViSession vi){return 0;}    // Запуск от внешнего источника 

ViStatus _VI_FUNC unols_mWrite_allData_3 (ViSession vi, ViUInt16 packs, ViChar _VI_FAR file_name[], 
	                                     ViPUInt32 adr){return 0;} // нескольких файлов
ViStatus _VI_FUNC unols_mRead_allData_3 (ViSession vi, ViUInt16 packs,  ViChar _VI_FAR file_name[],
	                                    ViUInt32 period){return 0;} 
ViStatus _VI_FUNC unols_mConfig_channel (ViSession vi, ViInt16 devise,
										ViUInt32 _VI_FAR stateChans[],
										ViUInt32 _VI_FAR chans[],
										ViUInt16 defState){return 0;}
ViStatus _VI_FUNC unols_mConfig_Uout (ViSession vi, ViReal64 _VI_FAR U){return 0;} 
ViStatus _VI_FUNC unols_mWrite_DataOZU (ViSession vi, ViInt16 devise, ViUInt32 period,
									   ViUInt32 offsetData, void* data){return 0;}
ViStatus _VI_FUNC unols_mRead_DataOZU (ViSession vi, ViInt16 devise, ViUInt32 period,
									ViUInt32 offsetData, void* data){return 0;}

// Технологическая функция Включение/отключение модуляции
ViStatus _VI_FUNC unols_mModulation  (ViSession vi,   ViInt32 state){return 0;}
ViStatus _VI_FUNC unols_mModulation_Q  (ViSession vi, ViPInt32 state){return 0;}

ViStatus _VI_FUNC unols_TPO_KR(ViSession vi, ViInt32 inpOut, ViInt32 state){return 0;}  // ПРИЕМ (КR) Поддержка проверки  ВЫВОДА/ВВОДА    
ViStatus _VI_FUNC unols_TPO_KF(ViSession vi, ViInt32 inpOut, ViInt32 state){return 0;}  // ВЫДАЧА (КФ) Проверка ВЫВОДА/ВВОДА  
ViStatus _VI_FUNC unols_TPO_KR_Q(ViSession vi, ViInt32 inpOut, ViPInt32 state){return 0;}
ViStatus _VI_FUNC unols_TPO_KF_Q(ViSession vi, ViInt32 inpOut, ViPInt32 state){return 0;}

ViStatus _VI_FUNC unols_mGener_ext_code (ViSession vi,  ViUInt32 code, ViUInt32 state){return 0;} //  Запись, чтение кодов генератора   ++
ViStatus _VI_FUNC unols_mGener_ext_code_Q (ViSession vi, ViPUInt32 code, ViPUInt32 state){return 0;} 

// Контроль частот 
ViStatus _VI_FUNC unols_mClockFrequInpOut (ViSession vi,  ViPUInt32 state){return 0;}
ViStatus _VI_FUNC unols_mTestInpFreq (ViSession vi,  ViInt32 state){return 0;} // Прием на удвоенной частоте
//ViStatus _VI_FUNC unols_mTestInpFreq_Q (ViSession vi,  ViPInt32 state){return 0;}   
ViStatus unols_setNameFileKF(ViChar _VI_FAR file_name[]){return 0;}	
ViStatus unols_mWrRdData(ViSession vi, ViInt32 ofset, ViInt32 j, ViInt32 data[]){return 0;} // функция для тестов





#if defined(__cplusplus) || defined(__cplusplus__)
}
#endif
