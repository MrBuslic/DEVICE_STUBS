#include <windows.h>
#include "mkprm_rpc.h"
#include "rpc_ports.h"
#include "unmkprm_h.h"

// Объявляем функцию DllMain
BOOL APIENTRY DllMain(HINSTANCE hinstDLL,
	DWORD fdwReason, LPVOID lpvReserved)
{
	RPC_mkprm_SLOT_Thread& slot_thr(Srpc_buffer_class::Instance().mkprm_slot_thr);
	RPC_mkprm_SIGNAL_Thread& signal_thr(Srpc_buffer_class::Instance().mkprm_signal_thr);
	switch (fdwReason)      // Дерево разбора уведомлений
	{
	case DLL_PROCESS_ATTACH: // Подключение DLL
							 //MessageBox(NULL,"Подключение Заглушки UNFOI для ФОИ","Использование заглушек!", MB_ICONINFORMATION);

							 //if (lpvReserved)  // Определение способа загрузки
							 //  MessageBox(NULL,"DLL загружена с неявной компоновкой","Использование заглушек!", MB_ICONINFORMATION);
							 //else
							 //  MessageBox(NULL,"DLL загружена с явной компоновкой","Использование заглушек!", MB_ICONINFORMATION);
							 //return 1; // успешная инициализация


		if (!slot_thr.isRunning())
		{
			slot_thr.set_connection_params("127.0.0.1", MKPRM_SLOT);
			slot_thr.start();
		}
		//if (!slot_thr.wait_connected(3))
		//	return false;
		if (!signal_thr.isRunning())
		{
			signal_thr.set_connection_params("127.0.0.1", MKPRM_SIGNAL);
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
