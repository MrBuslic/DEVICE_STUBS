#include <unmfsk24.h>
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
    MessageBox(NULL,"Подключение Заглушки UNMFSK24 для Мезонина МФСК-24","Использование заглушек!", MB_ICONINFORMATION);

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
/*  Функции                                                       */
/*----------------------------------------------------------------------*/
#ifdef UNMFSK24_OLD_INIT	
ViStatus _VI_FUNC unmfsk24_init (ViSession arg0, ViUInt16 arg1, ViBoolean arg2,
                              ViBoolean arg3, ViSession *arg4){ return 0; }
#else
ViStatus _VI_FUNC unmfsk24_init (ViRsrc rsrcName, ViBoolean IDquery,
                                 ViBoolean doReset, ViSession *mezvi){ return 0; }
ViStatus _VI_FUNC unmfsk24_connect (ViSession mezvi, ViSession vi, ViUInt16 m_num, ViBoolean IDquery,
                                 ViBoolean doReset){ return 0; }
#endif
ViStatus _VI_FUNC unmfsk24_set_cmd_time (ViSession arg0, ViInt16 arg1,
										 ViReal64 arg2){ return 0; }
ViStatus _VI_FUNC unmfsk24_config_trigger (ViSession arg0, ViUInt16 arg1){ return 0; }

ViStatus _VI_FUNC unmfsk24_cmd_time_q (ViSession arg0, ViInt16 arg1,
										   ViReal64 *arg2){ return 0; }

ViStatus _VI_FUNC unmfsk24_config_trigger_q (ViSession arg0, ViUInt16 *arg1){ return 0; }

ViStatus _VI_FUNC unmfsk24_start (ViSession arg0, ViInt16 _VI_FAR arg1[]){ return 0; }

ViStatus _VI_FUNC unmfsk24_gstart_q (ViSession mvi, ViInt16 *state){ return 0; }

ViStatus _VI_FUNC unmfsk24_stop (ViSession arg0){ return 0; }

ViStatus _VI_FUNC unmfsk24_state (ViSession arg0, ViBoolean arg1[]){ return 0; }

ViStatus _VI_FUNC unmfsk24_command (ViSession arg0, ViInt16 arg1, ViReal64 arg2){ return 0; }
ViStatus _VI_FUNC unmfsk24_group_command (ViSession arg0, ViReal64 arg1,
										  ViInt16 _VI_FAR arg2[]){ return 0; }
ViStatus _VI_FUNC unmfsk24_manual_cmd (ViSession arg0, ViInt16 arg1, ViInt16 arg2){ return 0; }
ViStatus _VI_FUNC unmfsk24_manual_group_cmd (ViSession arg0, ViInt16 arg1,
											 ViInt16 _VI_FAR arg2[]){ return 0; }
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