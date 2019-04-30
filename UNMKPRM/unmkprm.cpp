#include <socket_rpc.h>
#include <unmkprm.h>
#include <windows.h>
#include "mkprm_rpc.h"
#include "rpc_ports.h"

class rpc_buffer_class
{
public:
	RPC_mkprm_SLOT_Thread mkprm_slot_thr;
	RPC_mkprm_SIGNAL_Thread mkprm_signal_thr;
};

SINGLETON_DEF(rpc_buffer_class);

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

#if defined(__cplusplus) || defined(__cplusplus__)
extern "C" {
#endif

ViStatus _VI_FUNC unmkprm_init (
							ViRsrc Addr, 
							ViBoolean doInd, 
							ViBoolean doReset, 
							ViPSession mvi
							){ return 0; }

ViStatus _VI_FUNC unmkprm_connect (
							ViSession mvi, 
							ViSession basevi, 
							ViUInt16 meznum,
							ViBoolean doIdn, 
							ViBoolean doReset
							){ return 0; }

ViStatus _VI_FUNC unmkprm_alloc (
							ViSession mvi, 
							ViInt32 memsize
							){ return 0; }

ViStatus _VI_FUNC unmkprm_mode_block (
							ViSession mvi, 
							ViInt32 blockSize
							){ return 0; }

ViStatus _VI_FUNC unmkprm_mode_cycle (
							ViSession mvi, 
							ViInt32 packetSize
							){ return 0; }

ViStatus _VI_FUNC unmkprm_start (
							ViSession mvi
							)
{ 
	Srpc_buffer_class::Instance().mkprm_slot_thr.get_mkprm_obj()->unmkprm_start();
	return 0; 
}

ViStatus _VI_FUNC unmkprm_stop (
							ViSession mvi
							)
{
	Srpc_buffer_class::Instance().mkprm_slot_thr.get_mkprm_obj()->unmkprm_stop();
	return 0; 
}

ViStatus _VI_FUNC unmkprm_inp_type (
							ViSession mvi, 
							ViInt32 type
							){ return 0; }

ViStatus _VI_FUNC unmkprm_config_psp (
							ViSession mvi, 
							ViInt32 maskLen, 
							ViUInt16 feedback, 
							ViUInt16 invState, 
							ViUInt16 syncState,
							ViInt32 *strlen){ return 0; }

ViStatus _VI_FUNC unmkprm_config_sync_level (
							ViSession mvi, 
							ViInt32 level
							){ return 0; }

ViStatus _VI_FUNC unmkprm_config_adjust_level (
							ViSession mvi, 
							ViInt32 level
							){ return 0; }

ViStatus _VI_FUNC unmkprm_config_adjust_level2 (
							ViSession mvi, 
							ViInt32 level
							){ return 0; }

ViStatus _VI_FUNC unmkprm_num_ready_data (
							ViSession mvi, 
							ViPInt32 nready,
							ViPInt32 nreadystr
							){ return 0; }

ViStatus _VI_FUNC unmkprm_read_str_block (
							ViSession mvi, 
							ViInt32 strIdx, 
							ViInt32 needStr, 
							ViInt8 _VI_FAR data[], 
							ViInt8 _VI_FAR mask[], 
							ViPInt32 nreadstr
							){ return 0; }

ViStatus _VI_FUNC unmkprm_read_packet (
							ViSession mvi, 
							ViInt32 bsize, 
							ViInt8 _VI_FAR data[], 
							ViInt8 _VI_FAR mask[], 
							ViPInt32 nread,
							ViPInt32 nreadstr
							){ return 0; }
ViStatus _VI_FUNC unmkprm_read_str_packet (
							ViSession mvi, 
							ViInt32 needStr,
							ViInt8 _VI_FAR data[], 
							ViInt8 _VI_FAR mask[], 
							ViPInt32 nreadstr
							)
{
	QVariantList string_data;
	int tmp_begin = 0;
	int tmp_string_size;
	Srpc_buffer_class::Instance().mkprm_slot_thr.get_mkprm_obj()->unmkprm_get_strings(needStr, string_data);
	for (int i = 0; i < string_data.size(); i++)
	{
		QByteArray& tmp_string = string_data[i].toByteArray();
		tmp_string_size = tmp_string.size();
		std::copy(tmp_string.data(), tmp_string.data() + tmp_string_size, data + tmp_begin);
		tmp_begin += tmp_string_size;
	}
	*nreadstr = string_data.size();
	return 0; 
}

ViStatus _VI_FUNC unmkprm_state_q (
							ViSession mvi, 
							ViPInt32 state
							){ return 0; }

ViStatus _VI_FUNC unmkprm_install_handler (
							ViSession mvi, 
							Unmkprm_EventHandler handler, 
							ViAddr userData
							){ return 0; }

ViStatus _VI_FUNC unmkprm_reset (
							ViSession mvi
							){ return 0; }

ViStatus _VI_FUNC unmkprm_self_test (
							ViSession mvi, 
							ViPInt16 result,
							ViChar _VI_FAR message[]
							){ return 0; }

ViStatus _VI_FUNC unmkprm_test_ok2 (
							ViSession mvi, 
							ViInt32 inpType, 
							ViPInt16 result, 
							ViChar _VI_FAR message[]
							){ return 0; }

ViStatus _VI_FUNC unmkprm_revision_query (
							ViSession mvi, 
							ViChar _VI_FAR sw_rev[], 
							ViChar _VI_FAR hw_rev[]
							){ return 0; }

ViStatus _VI_FUNC unmkprm_error_query (
							ViSession mvi, 
							ViPInt32 error,
							ViChar _VI_FAR message[]
							){ return 0; }

ViStatus _VI_FUNC unmkprm_error_message (
							ViSession mvi, 
							ViStatus error,
							ViChar _VI_FAR message[]
							){ return 0; }

ViStatus _VI_FUNC unmkprm_close (
							ViSession mvi
							){ return 0; }

#if defined(__cplusplus) || defined(__cplusplus__)
}
#endif