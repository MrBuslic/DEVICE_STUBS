#if defined(__cplusplus) || defined(__cplusplus__)
extern "C" {
#endif
#include <unmds32.h>
#if defined(__cplusplus) || defined(__cplusplus__)
}
#endif
#include <socket_rpc.h>
#include <windows.h>
#include "mds32_rpc.h"
#include "rpc_ports.h"

#define SINGLETON_DEF(x) typedef Loki::SingletonHolder<x,Loki::CreateUsingNew,Loki::NoDestroy> S##x;
int mds_count = 0;
class rpc_buffer_class
{
public:
	QList<RPC_mds32_SLOT_Thread*> mds32_slot_thr;
	QList<RPC_mds32_SIGNAL_Thread*> mds32_signal_thr;
	friend struct Loki::CreateUsingNew<rpc_buffer_class>;
private:
	rpc_buffer_class()
	{
		for (int i = 0; i < 2; i++)
		{
			RPC_mds32_SLOT_Thread* slot_thr = new RPC_mds32_SLOT_Thread;
			slot_thr->set_connection_params("127.0.0.1", MDS_SLOT +i);
			slot_thr->start();
			//if (!slot_thr.wait_connected(3))
			//	return false;
			RPC_mds32_SIGNAL_Thread* signal_thr = new RPC_mds32_SIGNAL_Thread;
			signal_thr->set_connection_params("127.0.0.1", MDS_SIGNAL +i);
			signal_thr->start();
			//if (!signal_thr.wait_connected(3))
			//	return false;

			mds32_slot_thr.push_back(slot_thr);
			mds32_signal_thr.push_back(signal_thr);
		}
	}
};

SINGLETON_DEF(rpc_buffer_class);

// Объявляем функцию DllMain
BOOL APIENTRY DllMain(HINSTANCE hinstDLL,
      DWORD fdwReason, LPVOID lpvReserved)
{
	Srpc_buffer_class::Instance();
	switch (fdwReason)      // Дерево разбора уведомлений
	{
	case DLL_PROCESS_ATTACH: // Подключение DLL
	    //MessageBox(NULL,"Подключение Заглушки UNMDS32 для Мезонина МДС-32","Использование заглушек!", MB_ICONINFORMATION);

		//if (lpvReserved)  // Определение способа загрузки
		// MessageBox(NULL,"DLL загружена с неявной компоновкой","Использование заглушек!", MB_ICONINFORMATION);
		//else
		//MessageBox(NULL,"DLL загружена с явной компоновкой","Использование заглушек!", MB_ICONINFORMATION);
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

#if defined(__cplusplus) || defined(__cplusplus__)
extern "C" {
#endif

	//--------------------- Initialize --------------------------------------------
#ifdef UNMDS32_OLD_INIT	
ViStatus _VI_FUNC unmds32_init (ViSession arg0, ViUInt16 arg1, ViBoolean arg2,
                              ViBoolean arg3, ViSession *arg4){ return 0; }
#else
ViStatus _VI_FUNC unmds32_init (ViRsrc rsrcName, ViBoolean IDquery,
                                 ViBoolean doReset, ViSession *mezvi)
{ 
	mds_count++;
	*mezvi = mds_count;
	return 0; 
}
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
ViStatus _VI_FUNC unmds32_input_trigger (ViSession mvi, ViBoolean state){
	return Srpc_buffer_class::Instance().mds32_slot_thr[mvi-1]->get_mds32_obj()->unmds32_input_trigger(state);
}


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
ViStatus _VI_FUNC unmds32_start (ViSession mvi){ 
	return Srpc_buffer_class::Instance().mds32_slot_thr[mvi-1]->get_mds32_obj()->unmds32_start();
}

//---------------------
ViStatus _VI_FUNC unmds32_state (ViSession arg0, ViBoolean *arg1){ return 0; }

//---------------------
ViStatus _VI_FUNC unmds32_stop (ViSession mvi){ return 0; }

//--------------------- Query how much data is ready for read --------
ViStatus _VI_FUNC unmds32_numReadyData (ViSession arg0, ViUInt32 *arg1){ return 0; }

//--------------------- Read one sample --------------------------
ViStatus _VI_FUNC unmds32_read_sample (ViSession mvi, ViPUInt32 buf,ViPUInt32 firstTime,ViPUInt32 lastTime){
	uint tmp_buf;
	uint _firstTime;
	uint _lastTime;
	Srpc_buffer_class::Instance().mds32_slot_thr[mvi-1]->get_mds32_obj()->unmds32_read_sample(tmp_buf,_firstTime,_lastTime);
	*buf = tmp_buf;
	*lastTime = _lastTime;
	*firstTime = _firstTime;
	
	return 0;
}


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

#if defined(__cplusplus) || defined(__cplusplus__)
}
#endif
