#include <unmn8i.h>
#include <socket_rpc.h>
#include <windows.h>
#include "mn8i_rpc.h"
#include "unmn8i_h.h"
int mn8i_count = 0;

UNMN8IIntHandle _interrupt_handle;




rpc_buffer_class::rpc_buffer_class()
{
	for (int i = 0; i < 1; i++)
	{
		RPC_mn8i_SLOT_Thread* slot_thr = new RPC_mn8i_SLOT_Thread;
		slot_thr->set_connection_params("127.0.0.1", 30040 + i);
		slot_thr->start();
		//if (!slot_thr.wait_connected(3))
		//	return false;
		RPC_mn8i_SIGNAL_Thread* signal_thr = new RPC_mn8i_SIGNAL_Thread;
		signal_thr->set_connection_params("127.0.0.1", 30045 + i);
		signal_thr->start();
		//bool res = signal_thr->wait_connected(5);

		
		mn8i_slot_thr.push_back(slot_thr);
		mn8i_signal_thr.push_back(signal_thr);
	}
}

void rpc_buffer_class::packet_ready()
{
	QObject* tmp_sender = sender();
	for (int i = 0; i < mn8i_signal_thr.count(); i++)
	 	if (mn8i_signal_thr[i]->get_obj().get() == tmp_sender)
			(*_interrupt_handle)(i, UNMN8I_IRQ_PACKET_READY, 0, 0, 0);
}

// Объявляем функцию DllMain

BOOL APIENTRY DllMain(HINSTANCE hinstDLL,
	DWORD fdwReason, LPVOID lpvReserved)
{
	Srpc_buffer_class::Instance();
	switch (fdwReason)      // Дерево разбора уведомлений
	{
	case DLL_PROCESS_ATTACH: // Подключение DLL
		//MessageBox(NULL,"Подключение Заглушки UNMN8I для Мезонина МН8И","Использование заглушек!", MB_ICONINFORMATION);

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
	//--------------------------------
// Definitions of driver functions
//--------------------------------

//--------------------- Initialize --------------------------------------------
ViStatus _VI_FUNC unmn8i_init (ViRsrc rsrcName, ViBoolean IDquery,
                                 ViBoolean doReset, ViSession *mezvi)
{ 
	mn8i_count++;
	*mezvi = mn8i_count; 
	return 0; 
}
ViStatus _VI_FUNC unmn8i_connect(ViSession mezvi, ViSession vi, ViUInt16 m_num, ViBoolean IDquery,
                                 ViBoolean doReset){ return 0; }

//--------------------- Set signle mode -------------                               
ViStatus _VI_FUNC unmn8i_mode_once (ViSession mvi){ return 0; }

//--------------------- Set block mode -----------------------------
ViStatus _VI_FUNC unmn8i_mode_block (ViSession mvi, ViUInt32 size){ return 0; }

//--------------------- Set cycle mode ------------------------------
ViStatus _VI_FUNC unmn8i_mode_cycle (ViSession mvi, ViUInt32 size)
{ 
	Srpc_buffer_class::Instance().mn8i_slot_thr[mvi - 1]->get_mn8i_obj()->unmn8i_mode_cycle(size);
	return 0;   
}  

//---------------------- Query mode -----------------------------
ViStatus _VI_FUNC unmn8i_mode_q (ViSession mvi, ViUInt16 *mode, ViUInt32 *size){ return 0; }

//----------------------- Allocate carrier memory ------------------
ViStatus _VI_FUNC unmn8i_alloc (ViSession mvi, ViUInt32 size){ return 0; }

//----------------------- Query carrier memory --------------------
ViStatus _VI_FUNC unmn8i_alloc_q (ViSession mvi, ViUInt32 *size){ return 0; }

//---------------------- level ---------------------------
ViStatus _VI_FUNC unmn8i_level (ViSession mvi, ViUInt16 chan, ViReal64 low, ViReal64 high){ return 0; }
ViStatus _VI_FUNC unmn8i_level_q (ViSession mvi, ViUInt16 chan, ViPReal64 low, ViPReal64 high){ return 0; }

//---------------------- range ---------------------------
ViStatus _VI_FUNC unmn8i_range (ViSession mvi, ViUInt16 chan, ViReal64 diap){ return 0; }
ViStatus _VI_FUNC unmn8i_range_q (ViSession mvi, ViUInt16 chan, ViReal64 *diap){ return 0; }
ViStatus _VI_FUNC unmn8i_mn6i_ranges_q (ViSession mvi, ViUInt16 chan, ViReal64 *diap){ return 0; }

//---------------------- Set sample period -----------------------
ViStatus _VI_FUNC unmn8i_sample_period (ViSession mvi, ViReal64 periodS)
{ 
	Srpc_buffer_class::Instance().mn8i_slot_thr[mvi - 1]->get_mn8i_obj()->unmn8i_sample_period(periodS);
	return 0;  
}  

//---------------------- Query sample period -----------------------
ViStatus _VI_FUNC unmn8i_sample_period_q (ViSession mvi, ViReal64 *periodS,
                                         ViUInt16 *nSum){ return 0; }
ViStatus _VI_FUNC unmn8i_num_sum (ViSession mvi, ViUInt16 nSums){ return 0; }

//-------------------------- Set
ViStatus _VI_FUNC unmn8i_input_trigger (ViSession mvi, ViBoolean on)
{
	return Srpc_buffer_class::Instance().mn8i_slot_thr[mvi - 1]->get_mn8i_obj()->unmn8i_input_trigger(on);
}

ViStatus _VI_FUNC unmn8i_mn6i_input_trigger (ViSession mvi, ViUInt16 chan,
                                             ViBoolean on){ return 0; }

//-------------------------- Query
ViStatus _VI_FUNC unmn8i_input_trigger_q (ViSession mvi, ViBoolean *on){ return 0; }


//--------------------- Query sample width -----------------------------------------
ViStatus _VI_FUNC unmn8i_sample_width_q (ViSession mvi, ViUInt16 *widthWord,
                                        ViUInt16 *widthByte){
	uint frame_width;
	uint width_in_bytes;
	Srpc_buffer_class::Instance().mn8i_slot_thr[mvi - 1]->get_mn8i_obj()->unmn8i_sample_width_q(frame_width, width_in_bytes);
	if (widthWord)
		*widthWord = frame_width;
	if (widthByte)
		*widthByte = width_in_bytes;
	return 0; }

//---------------------- Set mask interrupt --------------------------------
ViStatus _VI_FUNC unmn8i_mask_interrupt (ViSession mvi, ViBoolean enableFIFO,
										ViBoolean enableOverLow,
										ViBoolean enableOverHigh, 
										ViBoolean enableBInterrupt){ return 0; }
ViStatus _VI_FUNC unmn8i_mask_interrupt_chan (ViSession mvi, ViUInt32 chan,
										ViBoolean enableOverLow, ViBoolean enableOverHigh){ return 0; }

//---------------------- Query interrupt --------------------------------
ViStatus _VI_FUNC unmn8i_mask_interrupt_q (ViSession mvi, ViBoolean *enableFIFO,
										  ViBoolean enableOverLow[],
										  ViBoolean enableOverHigh[],
										  ViBoolean *enableBInterrupt){ return 0; }

ViStatus _VI_FUNC unmn8i_overlevel_q(ViSession mvi, ViBoolean LevelUp[], ViBoolean LevelDown[]){ return 0; }


ViStatus _VI_FUNC unmn8i_installHandler (ViSession mvi, ViPAttrState handle, ViPAttrState userData)
{ 
	_interrupt_handle = reinterpret_cast<UNMN8IIntHandle>(handle);
	QObject::connect(static_cast<RPC_mn8i_SIGNAL_Object*>(Srpc_buffer_class::Instance().mn8i_signal_thr[mvi-1]->get_obj().get()), &RPC_mn8i_SIGNAL_Object::packet_ready, &Srpc_buffer_class::Instance(), &rpc_buffer_class::packet_ready);
	return 0;
}

ViStatus _VI_FUNC unmn8i_reset_flag (ViSession mvi){ return 0; }

//--------------------- Set config trigger -------------------------------------
ViStatus _VI_FUNC unmn8i_config_trigger (ViSession mvi, ViUInt16 trig){ return 0; }
ViStatus _VI_FUNC unmn8i_config_trigger_q (ViSession mvi, ViPUInt16 group){ return 0; }

//--------------------- Get attribute ------------------------------------------
ViStatus _VI_FUNC unmn8i_get_attribute (ViSession mvi, ViInt32 attr, ViPAttrState value){ return 0; }

//----------------------------------------
ViStatus _VI_FUNC unmn8i_start (ViSession mvi){
	return Srpc_buffer_class::Instance().mn8i_slot_thr[mvi - 1]->get_mn8i_obj()->unmn8i_start();
}

//----------------------------------------
ViStatus _VI_FUNC unmn8i_state (ViSession mvi, ViInt16 *stateParam, ViInt16 *errState){ return 0; }

//----------------------------------------
ViStatus _VI_FUNC unmn8i_stop (ViSession mvi){ 

return Srpc_buffer_class::Instance().mn8i_slot_thr[mvi - 1]->get_mn8i_obj()->unmn8i_stop();

}

//--------------------- Read one sample --------------------------
ViStatus _VI_FUNC unmn8i_read_sample (ViSession mvi, ViPReal64 buf, 
                    ViPUInt32 firstTime,ViPUInt32 thisTime){
	uint  tmp_buf;
	uint _firstTime;
	uint _thisTime;
	Srpc_buffer_class::Instance().mn8i_slot_thr[mvi - 1]->get_mn8i_obj()->unmn8i_read_sample(tmp_buf, _firstTime, _thisTime);
	*buf = tmp_buf;
	*firstTime = _firstTime;
	*thisTime = _thisTime;
	return 0;  
}

//--------------------- Read data in block mode -----------------------------       
ViStatus _VI_FUNC unmn8i_read_block (ViSession mvi, ViUInt32 firstSample,
                    ViUInt32 numSamples,ViReal64* buf){ return 0; }

//-------------------- Read packet of data in cycle mode ----------------------
ViStatus _VI_FUNC unmn8i_read_packet (ViSession mvi, ViBoolean isHot, ViUInt32 numSamples,
										ViReal64 *buf, ViUInt32 *realNumSamples)
{
	uint _realNumSamples;
	QVariantList tmp_buffer;
	Srpc_buffer_class::Instance().mn8i_slot_thr[mvi - 1]->get_mn8i_obj()->unmn8i_read_packet(isHot, numSamples, tmp_buffer, _realNumSamples);
	*realNumSamples = _realNumSamples;
	for (int i = 0; i < _realNumSamples; i++)
			for (int j = 0; j < 8; j++)
				buf[i*8+j] = tmp_buffer[i].toList()[j].toDouble();
		return 0;
}
ViStatus _VI_FUNC unmn8i_read_packet_float (ViSession mvi, ViBoolean isHot, ViUInt32 numSamples,
										ViReal32 *buf, ViUInt32 *realNumSamples){ return 0; }
//--------------------- Query how much data is ready for read --------
ViStatus _VI_FUNC unmn8i_num_ready_data (ViSession mvi, ViUInt32 *num)
{	
	uint _num;
	Srpc_buffer_class::Instance().mn8i_slot_thr[mvi - 1]->get_mn8i_obj()->unmn8i_num_ready_data(_num);
	*num = _num;
	return 0;
}

//---------------------- Calibrate ------------------------
ViStatus _VI_FUNC unmn8i_displacement (ViSession mvi){ return 0; }
ViStatus _VI_FUNC unmn8i_displacement_range (ViSession mvi){ return 0; }
//----------------------------------------
ViStatus _VI_FUNC unmn8i_reset (ViSession mvi){	return 0;}


ViStatus _VI_FUNC unmn8i_self_test (ViSession mvi, ViInt16 *result, ViChar message[]){ return 0; }
ViStatus _VI_FUNC unmn8i_sft_interface (ViSession mvi, ViPAttrState sftInterface, ViPAttrState userData){ return 0; }

//----------------------------------------
ViStatus _VI_FUNC unmn8i_error_query (ViSession mvi, ViInt32 *err, ViChar message[]){ return 0; }

//----------------------------------------
ViStatus _VI_FUNC unmn8i_error_message (ViSession mvi, ViStatus status,
                                       ViChar message[]){ return 0; }

//----------------------------------------
ViStatus _VI_FUNC unmn8i_revision_query (ViSession mvi, ViChar verSoft[],
                                        ViChar verHard[]){ return 0; }

//----------------------------------------
ViStatus _VI_FUNC unmn8i_close (ViSession mvi){ return 0; }


///ҫ禡 㮳殭饠騯//
ViStatus _VI_FUNC unmn8i_extcalibr (ViSession mvi, ViReal64 stdU){ return 0; }

ViStatus _VI_FUNC unmn8i_readFlash(ViSession mvi){ return 0; }
ViStatus _VI_FUNC unmn8i_writeFlash(ViSession mvi){ return 0; }

ViStatus _VI_FUNC unmn8i_resetCoeff(ViSession mvi){ return 0; }

#if defined(__cplusplus) || defined(__cplusplus__)
}
#endif