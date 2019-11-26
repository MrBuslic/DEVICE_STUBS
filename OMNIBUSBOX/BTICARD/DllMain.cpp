#ifdef WIN32
#include <windows.h>
#endif
#include "omnibus_rpc.h"
#include "rpc_ports.h"
#include "BTICARD\BufferClass.h"
#include "instruments.h"

// Объявляем функцию DllMain
BOOL APIENTRY DllMain(HINSTANCE hinstDLL,
	DWORD fdwReason, LPVOID lpvReserved)
{
	RPC_omnibus_SLOT_Thread& slot_thr(Srpc_buffer_class::Instance().omnibus_slot_thr);
	RPC_omnibus_SIGNAL_Thread& signal_thr(Srpc_buffer_class::Instance().omnibus_signal_thr);

	//получение айпи
	/*QString tmp_srvr_ip;
	QString ipSettingsFile = QString(QCoreApplication::applicationDirPath() + "/" + "ipSettings.ini");
	QSettings *ipSettings = new QSettings(ipSettingsFile, QSettings::IniFormat, NULL);
	ipSettings->beginGroup("IP");*/
	QString ip_str = instr::GetIpFromSettings("rpc_omnibus");//ipSettings->value(QString("rpc_omnibus"), "").toString();
															 //ipSettings->endGroup();

	switch (fdwReason)      // Дерево разбора уведомлений
	{
	case DLL_PROCESS_ATTACH: // Подключение DLL
		if (!slot_thr.isRunning())
		{
			slot_thr.set_connection_params(ip_str, OMNIBUS_SLOT);
			slot_thr.start();
		}
		//if (!slot_thr.wait_connected(3))
		//	return false;
		if (!signal_thr.isRunning())
		{
			signal_thr.set_connection_params(ip_str, OMNIBUS_SIGNAL);
			signal_thr.start();
			signal_thr.wait_connected(3);
		}



		break; // успешная инициализация

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