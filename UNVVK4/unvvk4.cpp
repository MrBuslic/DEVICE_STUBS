#include <unvvk4.h>
#include <socket_rpc.h>
#include <windows.h>
#include "vvk4_rpc.h"



#ifndef SINGLETON_DEF
#define SINGLETON_DEF(x) typedef Loki::SingletonHolder<x,Loki::CreateUsingNew,Loki::NoDestroy> S##x;
#endif

class rpc_buffer_class
{
public:
	RPC_vvk4_SLOT_Thread vvk4_slot_thr;
	RPC_vvk4_SIGNAL_Thread vvk4_signal_thr;
};

SINGLETON_DEF(rpc_buffer_class);

// Объявляем функцию DllMain
BOOL APIENTRY DllMain(HINSTANCE hinstDLL,
	DWORD fdwReason, LPVOID lpvReserved)
{
	RPC_vvk4_SLOT_Thread& slot_thr(Srpc_buffer_class::Instance().vvk4_slot_thr);
	RPC_vvk4_SIGNAL_Thread& signal_thr(Srpc_buffer_class::Instance().vvk4_signal_thr);
	switch (fdwReason)      // Дерево разбора уведомлений
	{
	case DLL_PROCESS_ATTACH: // Подключение DLL
							 //MessageBox(NULL,"Подключение Заглушки UNVVK4 для ВВК4","Использование заглушек!", MB_ICONINFORMATION);

							 //if (lpvReserved)  // Определение способа загрузки
							 //  MessageBox(NULL,"DLL загружена с неявной компоновкой","Использование заглушек!", MB_ICONINFORMATION);
							 //else
							 //  MessageBox(NULL,"DLL загружена с явной компоновкой","Использование заглушек!", MB_ICONINFORMATION);
							 //return 1; // успешная инициализация


		if (!slot_thr.isRunning())
		{
			slot_thr.set_connection_params("127.0.0.1", 70001);
			slot_thr.start();
		}
		//if (!slot_thr.wait_connected(3))
		//	return false;
		if (!signal_thr.isRunning())
		{
			signal_thr.set_connection_params("127.0.0.1", 70002);
			signal_thr.start();
		}
		//if (!signal_thr.wait_connected(3))
		//	return false;



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
ViStatus _VI_FUNC unvvk4_init (	ViRsrc 		rsrcName, 
								ViBoolean 	id_query,
								ViBoolean 	reset, 
								ViPSession 	vi){ return 0; }

/****************************************************************************
		Функции конфигурации
*****************************************************************************/


/****************************************************************************
		Сервисные функции 
*****************************************************************************/
ViStatus _VI_FUNC unvvk4_reset (ViSession vi){ return 0; }
ViStatus _VI_FUNC unvvk4_self_test (ViSession 	vi, 
									ViPInt16 	test_result, 
									ViChar 		_VI_FAR test_message[]){ return 0; }
//ViStatus _VI_FUNC unvvk4_testOK_off (ViSession 	vi, 
//									ViPInt16 	test_result, 
//									ViChar 		_VI_FAR test_message[]){ return 0; }
ViStatus _VI_FUNC  unvvk4_testOK_off (ViSession vi_vvk4, ViInt16 NumTest, ViPInt16 test_result, ViChar _VI_FAR test_message[]){ return 0; }

ViStatus _VI_FUNC unvvk4_error_query(ViSession vi, 
									 ViPInt32 error, 
									 ViChar _VI_FAR error_message[]){ return 0; }
ViStatus _VI_FUNC unvvk4_error_message (ViSession vi, 
										ViStatus error, 
										ViChar _VI_FAR message[]){ return 0; }
ViStatus _VI_FUNC unvvk4_revision_query (ViSession vi,
										ViChar _VI_FAR driver_revision[],
										ViChar _VI_FAR instrument_revision[]){ return 0; }
/****************************************************************************
		Функция закрытия сеанса с инструментом
*****************************************************************************/
ViStatus _VI_FUNC unvvk4_close (ViSession vi){ return 0; }

//Ф-ии загрузки контроллеров
////ViStatus _VI_FUNC unvvk4_readyPLD (ViSession vi){ return 0; }
//ViStatus _VI_FUNC unvvk4_loadPLD (ViSession vi){ return 0; }
//ViStatus _VI_FUNC unvvk4_prgPLD (ViSession vi){ return 0; }
//ViStatus _VI_FUNC unvvk4_vrfPLD (ViSession vi){ return 0; }

//Технологические ф-ии чтения/записи калибровочных коэффициентов
//ViStatus _VI_FUNC unvvk4_readDataRPZU (ViSession vi, void* buff, ViInt32 size){ return 0; }
//ViStatus _VI_FUNC unvvk4_writeDataRPZU (ViSession vi, void* buff, ViInt32 size){ return 0; }







// Функция "Исполнения коммутации"
ViStatus _VI_FUNC unvvk4_start_commut (ViSession vi_vvk4){ return 0; }

// Функция "Подключение выхода"	   //
ViStatus _VI_FUNC unvvk4_commut_Output (ViSession vi_vvk4, ViInt16 line, ViInt16 output, ViInt16 state)
{
	int res = -1;
	QString chanel = QString::number(output);
	if (state == 0)
		res = Srpc_buffer_class::Instance().vvk4_slot_thr.get_vvk4_obj()->unvvk4_commut_ListOutput(line, "", chanel);
	else if (state == 1)
		res = Srpc_buffer_class::Instance().vvk4_slot_thr.get_vvk4_obj()->unvvk4_commut_ListOutput(line, chanel, "");
	return res;
}				
// Функция "Коммутация группы выходов"	   0-снять конфигурацию, 1-сконфигурировать 
ViStatus _VI_FUNC unvvk4_commut_ListOutput (ViSession vi_vvk4, ViInt16 line, ViPChar MasOn, ViPChar MasOff)
{
	QString masOnStr = MasOn;
	QString masOffStr = MasOff;
	return Srpc_buffer_class::Instance().vvk4_slot_thr.get_vvk4_obj()->unvvk4_commut_ListOutput(line, masOnStr, masOffStr);
}
// Функция "Коммутация всех выходов"	 
ViStatus _VI_FUNC unvvk4_commut_AllOutput (ViSession vi_vvk4, ViInt16 line, ViInt16 *NN_output){ return 0; }
// Функция "Коммутация измерительных линий" :	 0-отключить, 1-подключить	  
ViStatus _VI_FUNC unvvk4_commut_MeasureLine (ViSession vi_vvk4, ViInt16 IzmLine, ViInt16 state){ return 0; }

// Функция "Конфигурация выхода"
ViStatus _VI_FUNC unvvk4_config_Output (ViSession vi_vvk4, ViInt16 line, ViInt16 output, ViInt16 state){ return 0; }
// Функция "Конфигурация группы выходов"	   0-снять конфигурацию, 1-сконфигурировать 
ViStatus _VI_FUNC unvvk4_config_ListOutput (ViSession vi_vvk4, ViInt16 line, ViPChar MasOn, ViPChar MasOff){	return 0; }
// Функция "Конфигурация всех выходов"	   0-снять конфигурацию, 1-сконфигурировать 
ViStatus _VI_FUNC unvvk4_config_AllOutput (ViSession vi_vvk4, ViInt16 line, ViInt16 *NN_output){ return 0; }
// Функция "Конфигурация измерительных линий" :	 0-отключить, 1-подключить	  
ViStatus _VI_FUNC unvvk4_config_MeasureLine (ViSession vi_vvk4, ViInt16 IzmLine, ViInt16 state){ return 0; }

// Функция "Запрос состояния конфигурации входных измерительных линий"  
ViStatus _VI_FUNC unvvk4_config_MeasureLine_Q (ViSession vi_vvk4, ViInt16 *Mas){ return 0; }
// Функция "Запрос состояния коммутации входных измерительных линий"  
ViStatus _VI_FUNC unvvk4_commut_MeasureLine_Q (ViSession vi_vvk4, ViInt16 *Mas){ return 0; }
// Функция "Запрос состояния конфигурации выхода" :	 0-сконфигурировать на отключение, 1-сконфигурировать на подключение	  
ViStatus _VI_FUNC unvvk4_config_Output_Q (ViSession vi_vvk4, ViInt16 line, ViInt16 output, ViPInt16 state){ return 0; }
// Функция "Запрос состояния коммутации выхода" :	 0-сконфигурировать на отключение, 1-сконфигурировать на подключение	  
ViStatus _VI_FUNC unvvk4_commut_Output_Q (ViSession vi_vvk4, ViInt16 line, ViInt16 output, ViPInt16 state){ return 0; }

							
// Функция "Отключение коммутации"	   
ViStatus _VI_FUNC unvvk4_Off_commut (ViSession vi_vvk4){ return 0; }


// Функция "Запрос состояния конфигурации выходов" :	 0-отключить, 1-подключить	  
ViStatus _VI_FUNC unvvk4_config_AllOutput_Q (ViSession vi_vvk4, ViInt16 line, ViInt16 *Mas){ return 0; }
// Функция "Запрос состояния коммутации выходов" :	 0-отключить, 1-подключить	  
ViStatus _VI_FUNC unvvk4_commut_AllOutput_Q (ViSession vi_vvk4, ViInt16 line, ViInt16 *Mas){ return 0; }

//		Функция "Коммутация реле"	   //
//*************************************************************/
ViStatus _VI_FUNC unvvk4_commut_Rele (
								ViSession 	vi_vvk4, 
								ViInt16 	rele, 
								ViInt16 	state){ return 0; }				
// Конфигурация группы выходов
ViStatus _VI_FUNC unvvk4_config_GroupOutput (ViSession vi_vvk4, ViInt16 line, ViInt16 *MasOutput, ViInt16 state){ return 0; }
// Коммутация группы выходов
ViStatus _VI_FUNC unvvk4_commut_GroupOutput (ViSession vi_vvk4, ViInt16 line, ViInt16 *MasOutput, ViInt16 state){ return 0; }


////////////////////////  для технологической панели  ////////////////////////////////////////////////////
ViStatus _VI_FUNC unvvk4_texnolog(ViSession vi_vvk4, ViInt16 x1, ViInt16 x2){ return 0; }
////////////////////////  для технологической панели  ////////////////////////////////////////////////////
ViStatus _VI_FUNC unvvk4_texnolog2(ViSession vi_vvk4, ViPInt16 x1, ViPInt16 x2){ return 0; }


#if defined(__cplusplus) || defined(__cplusplus__)
}
#endif