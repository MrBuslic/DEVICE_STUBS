#include "kp50_rpc.h"
#include "unkp50_h.h"
#include <unkp50.h>
#include <windows.h>
#include "rpc_ports.h"


kp50_rpc_buffer_class::kp50_rpc_buffer_class()
{
	kp50_slot_thr = new RPC_kp50_SLOT_Thread;
	kp50_slot_thr->set_connection_params("127.0.0.1", KP50_SLOT);
	kp50_slot_thr->start();

	kp50_signal_thr = new RPC_kp50_SIGNAL_Thread;
	kp50_signal_thr->set_connection_params("127.0.0.1", KP50_SIGNAL);
	kp50_signal_thr->start();

	kp50_slot_thr->wait_connected(3);
	kp50_signal_thr->wait_connected(3);
}

#if defined(__cplusplus) || defined(__cplusplus__)
extern "C" {
#endif
/****************************************************************************
		Функция инициализация сеанса с инструментом
*****************************************************************************/
ViStatus _VI_FUNC unkp50_init (	ViRsrc 		rsrcName,
								ViBoolean 	id_query,
								ViBoolean 	reset, 
								ViPSession 	vi)
{
	Skp50_rpc_buffer_class::Instance();
	return 0;
}

/****************************************************************************
		Сервисные функции 
*****************************************************************************/
ViStatus _VI_FUNC unkp50_reset (ViSession vi)
{
	return 0;
}
ViStatus _VI_FUNC unkp50_self_test (ViSession 	vi, 
									ViPInt16 	test_result, 
									ViChar 		_VI_FAR test_message[])
{
	return 0;
}
//ViStatus _VI_FUNC unkp50_testOK_off (ViSession vi, ViInt16 NumTest, ViPInt16 test_result, 
//									ViChar _VI_FAR test_message[]);
ViStatus _VI_FUNC unkp50_testOK_off (ViSession vi, ViInt16 chan, ViInt16 NumTest, ViPInt16 test_result, 
									ViChar _VI_FAR test_message[])
{
	return 0;
}

ViStatus _VI_FUNC unkp50_error_query(ViSession vi, 
									 ViPInt32 error, 
									 ViChar _VI_FAR error_message[])
{
	return 0;
}
ViStatus _VI_FUNC unkp50_error_message (ViSession vi, 
										ViStatus error, 
										ViChar _VI_FAR message[])
{
	return 0;
}
ViStatus _VI_FUNC unkp50_revision_query (ViSession vi,
										ViChar _VI_FAR driver_revision[],
										ViChar _VI_FAR instrument_revision[])
{
	return 0;
}

typedef ViStatus (_VI_FUNCH _VI_PTR p_unkp50_eventHandler)
					(ViSession vi, ViInt16 chan, ViUInt16 code1, ViUInt16 code2);
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////


// Установка пользовательской процедуры
ViStatus _VI_FUNC unkp50_install_handler (ViSession vi, p_unkp50_eventHandler callback)
{
	return 0;
}
// Функция "Разрешить прерывания"	   
ViStatus _VI_FUNC unkp50_config_event (ViSession vi, ViInt16 on)
{
	return 0;
}
// Функция-запрос "Состояние прерывания"	   
ViStatus _VI_FUNC unkp50_config_event_Q (ViSession vi, ViPInt16 on)
{
	return 0;
}

// Функция "Сброс канала"
ViStatus _VI_FUNC unkp50_reset_channel (ViSession vi, ViInt16 chan)
{
	return 0;
}

// Функция "Запрос состояния канала. ОШИБКИ канала"
ViStatus _VI_FUNC unkp50_channel_state_Q (ViSession vi, ViInt16 chan, ViUInt16 *code1, ViUInt16 *code2)
{
	bool on = Skp50_rpc_buffer_class::Instance().kp50_slot_thr->get_kp50_obj()->unkp50_channel_state_Q(chan);
	if (on)
		*code1 = 0x400;
	else
		*code1 = 0;
	return 0;
}
// Функция "Измерение тока канала"
ViStatus _VI_FUNC unkp50_meas_I (ViSession vi, ViInt16 chan, ViReal64 *I)
{
	*I = Skp50_rpc_buffer_class::Instance().kp50_slot_thr->get_kp50_obj()->unkp50_meas_I(chan);
	return 0;
}
// Функция "Измерение входного напряжения канала"
ViStatus _VI_FUNC unkp50_meas_Uin (ViSession vi, ViInt16 chan, ViReal64 *Uin)
{
	*Uin = Skp50_rpc_buffer_class::Instance().kp50_slot_thr->get_kp50_obj()->unkp50_meas_Uin(chan);
	return 0;
}
// Функция "Измерение выходного напряжения канала"
ViStatus _VI_FUNC unkp50_meas_Uout (ViSession vi, ViInt16 chan, ViReal64 *Uout)
{
	*Uout = Skp50_rpc_buffer_class::Instance().kp50_slot_thr->get_kp50_obj()->unkp50_meas_Uout(chan);
	return 0;
}
// Функция "Проверка готовности микроконтроллеров канала к работе"
ViStatus _VI_FUNC unkp50_check_channel (ViSession vi, ViInt16 chan)
{
	return 0;
}
// Функция "Включение реле"
ViStatus _VI_FUNC unkp50_switch_relay (ViSession vi, ViInt16 relay, ViInt16 state)
{
	return 0;
}
// Функция "Состояние канала"
//ViStatus _VI_FUNC unkp50_channel_state_Q (ViSession vi, ViInt16 chan, ViUInt16 *state);

// Функция "Включить канал"
ViStatus _VI_FUNC unkp50_switch_channel (ViSession vi, ViInt16 chan, ViInt16 on)
{
	return Skp50_rpc_buffer_class::Instance().kp50_slot_thr->get_kp50_obj()->unkp50_switch_channel(chan, on);
}

// Функция "Включить контроль КЗ входов канала на корпус"
ViStatus _VI_FUNC unkp50_config_shorts (ViSession vi, ViInt16 chan, ViInt16 on)
{
	return 0;
}

// Запрос состояния инструмента 
ViStatus _VI_FUNC unkp50_state_Q (ViSession vi, ViPUInt16	state)
{
	return 0;
}

/****************************************************************************
		Функция закрытия сеанса с инструментом
*****************************************************************************/
ViStatus _VI_FUNC unkp50_close (ViSession vi)
{
	return 0;
}

ViStatus _VI_FUNC unkp50_pld_reload (ViSession vi)
{
	return 0;
}
ViStatus _VI_FUNC unkp50_pld_write (ViSession vi)
{
	return 0;
}
ViStatus _VI_FUNC unkp50_pld_verify (ViSession vi)
{
	return 0;
}
ViStatus _VI_FUNC unkp50_pld_check (ViSession vi)
{
	return 0;
}


//технологические ф-ии
ViStatus _VI_FUNC unkp50_EEPROM_read(ViSession vi, ViInt16 offs, ViInt16* buff, ViInt16 size)
{
	return 0;
}
ViStatus _VI_FUNC unkp50_EEPROM_write(ViSession vi, ViInt16 offs, ViInt16* buff, ViInt16 size)
{
	return 0;
}
ViStatus _VI_FUNC unkp50_EEPROM_writeInfo(ViSession vi)
{
	return 0;
}
ViStatus _VI_FUNC unkp50_EEPROM_writeTime(ViSession vi)
{
	return 0;
}
ViStatus _VI_FUNC unkp50_EEPROM_readServ (ViSession vi)
{
	return 0;
}
ViStatus _VI_FUNC unkp50_EEPROM_readTime (ViSession vi)
{
	return 0;
}

ViStatus _VI_FUNC unkp50_reset33 (ViSession vi, ViInt16 on)
{
	return 0;
}

//		Функция "Проверка готовности"   (gt=0 - на "0", gt=1 - на "1")
//*************************************************************/
ViStatus _VI_FUNC unkp50_inf_bit (ViSession vi, ViInt16 chan, ViInt16 reg16, ViInt16 gt)
{
	return 0;
}

ViStatus _VI_FUNC unkp50_ErrorChan (ViSession vi, ViInt16 chan, ViUInt16 code1, char *tmpmsg)
{
	return 0;
}
ViStatus _VI_FUNC unkp50_WarnChan (ViInt16 chan, ViUInt16 code2, char *tmpmsg)
{
	return 0;
}
ViStatus _VI_FUNC unkp50_cmd_second (ViSession vi, ViInt16 chan, ViInt16 reg16)
{
	return 0;
}
ViStatus _VI_FUNC unkp50_cmd_main (ViSession vi, ViInt16 chan, ViInt16 reg16)
{
	return 0;
}
ViStatus _VI_FUNC unkp50_code_Uin (ViSession vi, ViInt16 chan, ViUInt16 *reg16)
{
	return 0;
}
ViStatus _VI_FUNC unkp50_code_Uout (ViSession vi, ViInt16 chan, ViUInt16 *reg16)
{
	return 0;
}
ViStatus _VI_FUNC unkp50_code_I (ViSession vi, ViInt16 chan, ViUInt16 *reg16)
{
	return 0;
}

//------------------------------------------------------------------------
#if defined(__cplusplus) || defined(__cplusplus__)
}
#endif



