#include <unfoi.h>
#include <windows.h>

#if defined(__cplusplus) || defined(__cplusplus__)
extern "C" {
#endif

/*
// Объявляем функцию DllMain
BOOL APIENTRY DllMain(HINSTANCE hinstDLL,
      DWORD fdwReason, LPVOID lpvReserved)
{

switch (fdwReason)      // Дерево разбора уведомлений
{
  case DLL_PROCESS_ATTACH: // Подключение DLL
    MessageBox(NULL,"Подключение Заглушки UNFOI для ФОИ","Использование заглушек!", MB_ICONINFORMATION);

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
/*----------------------------------------------------------------------*/
/*  Инициализация                                                       */
/*----------------------------------------------------------------------*/
ViStatus _VI_FUNC unfoi_init (ViRsrc rsrcName, ViBoolean id_query,
								ViBoolean reset, ViPSession vi){ return 0; }
/*----------------------------------------------------------------------*/
/*  Заготовки для функций-обработчиков событий                         */
/*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*/
/*  Зависимые от инструмента функции                                    */
/*----------------------------------------------------------------------*/
ViStatus _VI_FUNCC unfoi_chans_setup (ViSession vi, ViInt16 chans[], ViInt16 line[],
							ViReal64 U[], ViReal64 time){ return 0; }
ViStatus _VI_FUNCC unfoi_chan_setup (ViSession vi, ViInt16 chan, ViInt16 line, /*+*/
						   ViReal64 U, ViReal64 time){ return 0; }
ViStatus _VI_FUNCC unfoi_run (ViSession vi){ return 0; }
ViStatus _VI_FUNCC unfoi_start (ViSession vi){ return 0; }	   
ViStatus _VI_FUNCC unfoi_conf_chans (ViSession vi, ViInt16 chans[], ViInt16 lines[],
						   ViReal64 U, ViReal64 time){ return 0; }
ViStatus _VI_FUNCC unfoi_conf_chan (ViSession vi, ViInt16 chan, ViInt16 line,
						   ViReal64 U, ViReal64 time){ return 0; }
ViStatus _VI_FUNCC unfoi_chans_route (ViSession vi, ViInt16 chans[], ViInt16 state){ return 0; } 
ViStatus _VI_FUNCC unfoi_chan_route (ViSession vi, ViInt16 state, ViInt16 chan){ return 0; } /*+*/           
ViStatus _VI_FUNCC unfoi_off (ViSession vi){ return 0; }
ViStatus _VI_FUNCC unfoi_time (ViSession vi, ViReal64 time){ return 0; }
ViStatus _VI_FUNCC unfoi_time_query (ViSession vi, ViReal64 *time){ return 0; }
ViStatus _VI_FUNCC unfoi_chans_voltage (ViSession vi, ViInt16 chans[], ViInt16 lines[],
							  ViReal64 U){ return 0; }
ViStatus _VI_FUNCC unfoi_chan_voltage (ViSession vi, ViInt16 chan, ViInt16 line,
							  ViReal64 U){ return 0; }
ViStatus _VI_FUNCC unfoi_chan_query (ViSession vi, ViInt16 chan, ViInt16 *state_chan,
						   ViReal64 *U_Main, ViReal64 *U_Reserv){ return 0; }
ViStatus _VI_FUNCC unfoi_chans_query (ViSession vi, ViInt16 state_chan[], ViReal64 U_Main[],
							ViReal64 U_Reserv[], ViReal64 *time){ return 0; }

/*----------------------------------------------------------------------*/
/*  Функции-приложения (общие)                                          */
/*----------------------------------------------------------------------*/
ViStatus _VI_FUNCC unfoi_state (ViSession vi, ViInt16 *state){ return 0; }        /*+*/    
ViStatus _VI_FUNCC unfoi_enable_event (ViSession vi, ViBoolean event){ return 0; }        
ViStatus _VI_FUNCC unfoi_instal_handler (ViSession vi, ViAddr handler){ return 0; }                  
ViStatus _VI_FUNCC unfoi_config_hierarch (ViSession vi,
								ViInt16 master,
								ViInt16 TTLTRG){ return 0; }
/*----------------------------------------------------------------------*/
/*  Служебные функции                                                   */
/*----------------------------------------------------------------------*/
ViStatus _VI_FUNC unfoi_reset (ViSession vi){ return 0; }
ViStatus _VI_FUNC unfoi_self_test (ViSession vi,
									ViPInt16 test_result,
									ViChar _VI_FAR test_message[]){ return 0; }
ViStatus _VI_FUNC unfoi_error_query (ViSession vi,
									  ViPInt32 error,
									  ViChar _VI_FAR error_message[]){ return 0; }
ViStatus _VI_FUNC unfoi_error_message (ViSession vi,
										ViStatus error,
										ViChar _VI_FAR message[]){ return 0; }
ViStatus _VI_FUNC unfoi_revision_query (ViSession vi,
										 ViChar _VI_FAR driver_revision[],
										 ViChar _VI_FAR instrument_revision[]){ return 0; }
ViStatus _VI_FUNC unfoi_getDeviceList (ViPInt16 slotList,
										ViPInt16 laList,
										ViInt16 listLength,
										ViPInt16 numFound){ return 0; }
/*----  Служебные функции   -------------------------------------------*/                                       
 ViStatus _VI_FUNCC unfoi_default_constant (ViSession vi){ return 0; }            
/*----------------------------------------------------------------------*/
/*  Закрытие сеанса инструмента                                        */
/*----------------------------------------------------------------------*/
ViStatus _VI_FUNC unfoi_close (ViSession vi){ return 0; }
/*----------------------------------------------------------------------*/
/*  Определения                                                         */
/*----------------------------------------------------------------------*/


#if defined(__cplusplus) || defined(__cplusplus__)
}
#endif
