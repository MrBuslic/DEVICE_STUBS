#include "kp50_rpc.h"
#include "unkp50_h.h"
#include <windows.h>
#include "rpc_ports.h"

// Объявляем функцию DllMain
BOOL APIENTRY DllMain(HINSTANCE hinstDLL,
	DWORD fdwReason, LPVOID lpvReserved)
{
	RPC_kp50_SLOT_Thread& slot_thr(Srpc_buffer_class::Instance().kp50_slot_thr);
	RPC_kp50_SIGNAL_Thread& signal_thr(Srpc_buffer_class::Instance().kp50_signal_thr);
	switch (fdwReason)      // Дерево разбора уведомлений
	{
	case DLL_PROCESS_ATTACH: // Подключение DLL
		if (!slot_thr.isRunning())
		{
			slot_thr.set_connection_params("127.0.0.1", KP50_SLOT);
			slot_thr.start();
		}
		//if (!slot_thr.wait_connected(3))
		//	return false;
		if (!signal_thr.isRunning())
		{
			signal_thr.set_connection_params("127.0.0.1", KP50_SIGNAL);
			signal_thr.start();
		}

		return 1;
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
		break;

	}
	return TRUE;    // Код возврата игнорируется
}