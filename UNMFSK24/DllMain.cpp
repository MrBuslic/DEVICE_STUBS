#ifdef WIN32
#include <windows.h>
#endif
#include "unmfsk24_h.h"

// Объявляем функцию DllMain
BOOL APIENTRY DllMain(HINSTANCE hinstDLL,
	DWORD fdwReason, LPVOID lpvReserved)
{
	Srpc_buffer_class::Instance();
	switch (fdwReason)      // Дерево разбора уведомлений
	{
	case DLL_PROCESS_ATTACH: //  Подключение DLL
							 //MessageBox(NULL, "Подключение Заглушки UNMFSK24 для Мезонина МФСК-24", "Использование заглушек!", MB_ICONINFORMATION);

							 //if (lpvReserved)  // Определение способа загрузки
							 //  MessageBox(NULL,"DLL загружена с неявной компоновкой","Использование заглушек!", MB_ICONINFORMATION);
							 //else
							 //  MessageBox(NULL,"DLL загружена с явной компоновкой","Использование заглушек!", MB_ICONINFORMATION);
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