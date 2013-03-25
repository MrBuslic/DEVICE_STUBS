extern "C"
{
#include <unmds32.h>
}
#include <windows.h>
/*
// Объявляем функцию DllMain
BOOL APIENTRY DllMain(HINSTANCE hinstDLL,
      DWORD fdwReason, LPVOID lpvReserved)
{

switch (fdwReason)      // Дерево разбора уведомлений
{
  case DLL_PROCESS_ATTACH: // Подключение DLL
    MessageBox(NULL,"Подключение Заглушки UNMDS32 для Мезонина МДС-32","Использование заглушек!", MB_ICONINFORMATION);

    if (lpvReserved)  // Определение способа загрузки
      MessageBox(NULL,"DLL загружена с неявной компоновкой","Использование заглушек!", MB_ICONINFORMATION);
    else
      MessageBox(NULL,"DLL загружена с явной компоновкой","Использование заглушек!", MB_ICONINFORMATION);
    return 1; // успешная инициализация

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
    MessageBox(NULL,"Использование заглушек!","Завершение потока", MB_ICONINFORMATION);
    break;

  }
return TRUE;    // Код возврата игнорируется
}
*/
//--------------------- Initialize --------------------------------------------
#ifdef UNMDS32_OLD_INIT	
ViStatus _VI_FUNC unmds32_init (ViSession arg0, ViUInt16 arg1, ViBoolean arg2,
                              ViBoolean arg3, ViSession *arg4){ return 0; }
#else
ViStatus _VI_FUNC unmds32_init (ViRsrc rsrcName, ViBoolean IDquery,
                                 ViBoolean doReset, ViSession *mezvi){ return 0; }
ViStatus _VI_FUNC unmds32_connect (ViSession mezvi, ViSession vi, ViUInt16 m_num, ViBoolean IDquery,
                                 ViBoolean doReset){ return 0; }
#endif

//--------------------- Set signle mode -------------                               
ViStatus _VI_FUNC unmds32_mode_1 (ViSession arg0){ return 0; }

//--------------------- Set block mode -----------------------------
ViStatus _VI_FUNC unmds32_mode_block (ViSession arg0, ViUInt32 arg1){ return 0; }

//--------------------- Set cycle mode ------------------------------
ViStatus _VI_FUNC unmds32_mode_cycle (ViSession arg0, ViUInt32 arg1){ return 0; }

//---------------------- Query mode -------------------------------------------------
ViStatus _VI_FUNC unmds32_mode_q (ViSession mvi, ViUInt16 *mode, ViUInt32 *blockSize){ return 0; }

//----------------------- Allocate carrier memory ------------------
ViStatus _VI_FUNC unmds32_alloc (ViSession mvi, ViUInt32 nBytes){ return 0; }

//----------------------- Query carrier memory --------------------
ViStatus _VI_FUNC unmds32_alloc_q (ViSession mvi, ViUInt32 *nBytes){ return 0; }

//----------------------- Set comparator threshold -----------------
ViStatus _VI_FUNC unmds32_set_porog (ViSession mvi, ViReal64 porogC){ return 0; }
ViStatus _VI_FUNC unmds32_set_porog_group (ViSession mvi, ViReal64 porogC, ViUInt16 group){ return 0; }

//----------------------- Query comparator threshold -----------------
ViStatus _VI_FUNC unmds32_set_porog_q (ViSession mvi, ViReal64 *porog){ return 0; }
ViStatus _VI_FUNC unmds32_set_porog_group_q (ViSession mvi, ViUInt16 group, ViReal64 *porog){ return 0; }

//---------------------- Set sample period ----------------------------
ViStatus _VI_FUNC unmds32_sample_period (ViSession mvi, ViReal64 periodS){ return 0; }

//---------------------- Query sample period ----------------------------
ViStatus _VI_FUNC unmds32_sample_period_q (ViSession arg0, ViReal64 *arg1){ return 0; }

//---------------------- Set input trigger ------------------------------
ViStatus _VI_FUNC unmds32_input_trigger (ViSession mvi, ViBoolean state){ return 0; }
ViStatus _VI_FUNC unmds32_input_trigger_group (ViSession mvi, ViBoolean state, ViUInt16 group){ return 0; }

//---------------------- Query input trigger ------------------------------
ViStatus _VI_FUNC unmds32_input_trigger_q (ViSession mvi, ViPBoolean state){ return 0; }
ViStatus _VI_FUNC unmds32_input_trigger_group_q (ViSession mvi, ViUInt16 group, ViPBoolean state){ return 0; }

//---------------------- Set mask interrupt --------------------------------
ViStatus _VI_FUNC unmds32_mask_interrupt (ViSession arg0, ViBoolean FIFO){ return 0; }

//---------------------- Query mask interrupt --------------------------------
ViStatus _VI_FUNC unmds32_mask_interrupt_q (ViSession mvi, ViBoolean * enable){ return 0; }

//---------------------- Set user interrupt handle -------------------------------
 
ViStatus _VI_FUNC unmds32_set_user_ihandle (ViSession mvi, ViPAttrState handle){ return 0; }

//--------------------- Query sample width -----------------------------------------
ViStatus _VI_FUNC unmds32_sample_width_q (ViSession mvi,ViPUInt16 n,ViPUInt16 nBytes){ return 0; }

//--------------------- Set config trigger -------------------------------------
ViStatus _VI_FUNC unmds32_config_trigger (ViSession arg0, ViUInt16 arg1){ return 0; }

//---------------------
ViStatus _VI_FUNC unmds32_start (ViSession mvi){ return 0; }

//---------------------
ViStatus _VI_FUNC unmds32_state (ViSession arg0, ViBoolean *arg1){ return 0; }

//---------------------
ViStatus _VI_FUNC unmds32_stop (ViSession mvi){ return 0; }

//--------------------- Query how much data is ready for read --------
ViStatus _VI_FUNC unmds32_numReadyData (ViSession arg0, ViUInt32 *arg1){ return 0; }

//--------------------- Read one sample --------------------------
ViStatus _VI_FUNC unmds32_read_sample (ViSession mvi, ViPUInt32 buf,
		ViPUInt32 firstTime,ViPUInt32 lastTime){ return 0; }
//--------------------- Read data in block mode -----------------------------       
ViStatus _VI_FUNC unmds32_read_block (ViSession mvi, ViUInt32 firstSample,
					ViUInt32 numSamples,ViPUInt32 buf){ return 0; }
//-------------------- Read packet of data in cycle mode ----------------------
ViStatus _VI_FUNC unmds32_read_packet (ViSession mvi, ViUInt32 numSamples,
									   ViPUInt32 buf, ViPUInt32 realNumSamples){ return 0; }
//-----------------------------                                    
ViStatus _VI_FUNC unmds32_reset (ViSession mvi){ return 0; }

//-----------------------------                                    
ViStatus _VI_FUNC unmds32_self_test (ViSession mvi, ViPInt16 result,
									 ViChar _VI_FAR message[]){ return 0; }
//-----------------------------                                    
ViStatus _VI_FUNC unmds32_error_query (ViSession mvi, ViPInt32 error,
									   ViChar _VI_FAR errorMessage[]){ return 0; }
//-----------------------------                                    
ViStatus _VI_FUNC unmds32_error_message (ViSession mvi, ViStatus arg1,
										 ViChar _VI_FAR errorMessage[]){ return 0; }
//-----------------------------                                    
ViStatus _VI_FUNC unmds32_revision_query (ViSession mvi, ViChar _VI_FAR driverVer[],
										  ViChar _VI_FAR instrumentVer[]){ return 0; }
//-----------------------------                                    
ViStatus _VI_FUNC unmds32_close (ViSession mvi){ return 0; }


