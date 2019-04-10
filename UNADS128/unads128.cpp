#include <unads128.h>
#include <socket_rpc.h>
#include <windows.h>
#include "ads128_rpc.h"
#include "unads128_h.h"
#include "rpc_ports.h"

int ads128_count = 0;


rpc_buffer_class::rpc_buffer_class()
{
	for (int i = 0; i < 2; i++)
	{
		RPC_ads128_SLOT_Thread* slot_thr = new RPC_ads128_SLOT_Thread;
		slot_thr->set_connection_params("127.0.0.1", ADS_SLOT + i);
		slot_thr->start();
		//if (!slot_thr.wait_connected(3))
		//	return false;
		RPC_ads128_SIGNAL_Thread* signal_thr = new RPC_ads128_SIGNAL_Thread;
		signal_thr->set_connection_params("127.0.0.1", ADS_SIGNAL + i);
		signal_thr->start();
		//bool res = signal_thr->wait_connected(5);


		ads128_slot_thr.push_back(slot_thr);
		ads128_signal_thr.push_back(signal_thr);
	}
}

// Объявляем функцию DllMain  

BOOL APIENTRY DllMain(HINSTANCE hinstDLL,
	DWORD fdwReason, LPVOID lpvReserved)
{
	Srpc_buffer_class::Instance();
	switch (fdwReason)    
		// Дерево разбора уведомлений
	{
	case DLL_PROCESS_ATTACH: // Подключение DLL
		//MessageBox(NULL,"Подключение Заглушки UNADS128 для Мезонина АДС128","Использование заглушек!", MB_ICONINFORMATION);
		//if (lpvReserved)  // Определение способа загрузки
		// MessageBox(NULL,"DLL загружена с неявной компоновкой","Использование заглушек!", MB_ICONINFORMATION);
		//else
		//MessageBox(NULL,"DLL загружена с явной компоновкой","Использование заглушек!", MB_ICONINFORMATION);
		//return 1; // успешная инициализация
				break;

	case DLL_PROCESS_DETACH: 
		// Отключение DLL
		// Здесь – освобождаем память, закрываем
		// файлы и т.д.
		break;

	case DLL_THREAD_ATTACH: 
		// Уведомление о новом потоке
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


	ViStatus _VI_FUNC unads128_sft_interface(ViSession vi, UN_SFT_INTERFACE sftInterface) { return 0; }

	ViStatus _VI_FUNC unads128_init (ViRsrc rsrcName,
		ViBoolean do_ID,
		ViBoolean do_RESET,
		ViPSession vi) 
	{
		ads128_count++;
		*vi = ads128_count;
		return 0;
	}

	ViStatus _VI_FUNC unads128_input (ViSession vi,
		ViUInt16  _VI_FAR buff[])
	{
		return 0;
	}

	ViStatus _VI_FUNC unads128_start (ViSession vi)
	{
	Srpc_buffer_class::Instance().ads128_slot_thr[vi - 1]->get_ads128_obj()->ads128_start();
		return 0; 
	}

	ViStatus _VI_FUNC unads128_start_check (ViSession vi) 
	{
		return 0;
	}

	ViStatus _VI_FUNC unads128_stop (ViSession vi) 
	{
		Srpc_buffer_class::Instance().ads128_slot_thr[vi - 1]->get_ads128_obj()->ads128_stop();
		return 0;
	}

	ViStatus _VI_FUNC unads128_state (ViSession vi,
		ViPInt16 state)
	{
		return 0;
	}

	ViStatus _VI_FUNC unads128_conf_analog (ViSession vi,
		ViInt16 group,
		ViReal64 level_0,
		ViReal64 level_1)
	{
		Srpc_buffer_class::Instance().ads128_slot_thr[vi - 1]->get_ads128_obj()->ads128_conf_analog(group,level_0,level_1);
		return 0;
	}

	ViStatus _VI_FUNC unads128_analog_q (ViSession vi,
		ViInt16 group,
		ViPReal64 level_0,
		ViPReal64 level_1)
	{
		uint group_;
		double lev0;
		double lev1;
		Srpc_buffer_class::Instance().ads128_slot_thr[vi - 1]->get_ads128_obj()->ads128_analog_q(group_, lev0, lev1);
		*level_0 = lev0;
		*level_1 = lev1;
		group = group_;
		return 0;
	}

	ViStatus _VI_FUNC unads128_install_handler (ViSession vi,
		ViAddr user_handler)
	{


		return 0;
	}

	ViStatus _VI_FUNC unads128_read_data (ViSession vi,
		ViUInt16 _VI_FAR thisBuiff[],
		ViUInt16 _VI_FAR firstBuff[]) 
	{

		QVariantList thisbuf;

		Srpc_buffer_class::Instance().ads128_slot_thr[vi - 1]->get_ads128_obj()->ads128_read_data(thisbuf, QVariantList());

		for (int i = 0; i < 16; i++)
				thisBuiff[i] = thisbuf[i].toUInt();
			
		return 0;
	}

	ViStatus _VI_FUNC unads128_reset (ViSession vi)
	{
		return 0;
	}


	ViStatus _VI_FUNC unads128_self_test (ViSession vi,
		ViPInt16 test_result,
		ViChar _VI_FAR test_message[]) 
	{
		return 0;
	}

	ViStatus _VI_FUNC unads128_test1 (ViSession vi,
		ViInt16 *TestResult,
		ViChar _VI_FAR TestMessage[]) 
	{
		return 0;
	}

	ViStatus _VI_FUNC unads128_error_query (ViSession vi,
		ViPInt32 error,
		ViChar _VI_FAR error_message[])
	{
		return 0;
	}

	ViStatus _VI_FUNC unads128_error_message (ViSession vi,
		ViStatus status_code,
		ViChar _VI_FAR status_descr[])
	{
		return 0;
	}

	ViStatus _VI_FUNC unads128_revision_query (ViSession vi,
		ViChar _VI_FAR driver_ver[],
		ViChar _VI_FAR instr_ver[])
	{
		return 0;
	}

	ViStatus _VI_FUNC unads128_close (ViSession vi)
	{
		return 0;
	}




	/*
	ViStatus _VI_FUNC unads128_conf_time (
									ViSession vi,
									ViUInt16 dT
									){ return 0; }

	ViStatus _VI_FUNC unads128_start_sourse (
									ViSession vi,
									ViInt16 source,
									ViInt16 TTLline
									){ return 0; }

	ViStatus _VI_FUNC unads128_start_sourse_q (
									ViSession vi,
									ViPInt16 source,
									ViPUInt16 TTLline
									){ return 0; }
	ViStatus _VI_FUNC unads128_calibrate (
									ViSession vi
									){ return 0; }
	*/


