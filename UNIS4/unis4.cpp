#include <unis4.h>
#include <socket_rpc.h>
#include <windows.h>
#include "is4_rpc.h"
#include "rpc_ports.h"

#ifndef SINGLETON_DEF
#define SINGLETON_DEF(x) typedef Loki::SingletonHolder<x,Loki::CreateUsingNew,Loki::NoDestroy> S##x;
#endif

int is4_count = 0;

class rpc_buffer_class
{
public:
	QList<RPC_is4_SLOT_Thread*> is4_slot_thr;
	QList<RPC_is4_SIGNAL_Thread*> is4_signal_thr;
	friend struct Loki::CreateUsingNew<rpc_buffer_class>;
private:
	rpc_buffer_class()
	{
			for (int i = 0; i < 1; i++)
		{
			RPC_is4_SLOT_Thread* slot_thr = new RPC_is4_SLOT_Thread;
			slot_thr->set_connection_params("127.0.0.1",  IS4_SLOT+ i);
			slot_thr->start();
			//if (!slot_thr.wait_connected(3))
			//	return false;
			RPC_is4_SIGNAL_Thread* signal_thr = new RPC_is4_SIGNAL_Thread;
			signal_thr->set_connection_params("127.0.0.1", IS4_SIGNAL + i);
			signal_thr->start();
			//if (!signal_thr.wait_connected(3))
			//	return false;
			is4_slot_thr.push_back(slot_thr);
			is4_signal_thr.push_back(signal_thr);
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
							 //MessageBox(NULL,"Подключение Заглушки UNIS4 для ИС4","Использование заглушек!", MB_ICONINFORMATION);

							 //if (lpvReserved)  // Определение способа загрузки
							 //  MessageBox(NULL,"DLL загружена с неявной компоновкой","Использование заглушек!", MB_ICONINFORMATION);
							 //else
							 //  MessageBox(NULL,"DLL загружена с явной компоновкой","Использование заглушек!", MB_ICONINFORMATION);
							 //return 1; // успешная инициализациZ


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
//	Функция установки текущего исполняемого процесса
ViStatus _VI_FUNC unis4_SetTypeProcess (ViSession vi, ViInt32 ExeProcess)
{ 
	Srpc_buffer_class::Instance().is4_slot_thr[vi - 1]->get_is4_obj()->unis4_SetTypeProcess(ExeProcess);
	return 0; 
}
//	Функция запроса текущего исполняемого процесса
ViStatus _VI_FUNC unis4_SetTypeProcess_Q (ViSession vi, ViPInt32 ExeProcess){ return 0; }
//	Функция установки разрешения или запрета завершения процессов по прерыванию
ViStatus _VI_FUNC unis4_InterruptEndProcess (ViSession vi, ViAddr addr_func){ return 0; }
//	Функция запроса разрешения или запрета завершения процессов по прерыванию
ViStatus _VI_FUNC unis4_InterruptEndProcess_Q (ViSession vi, ViPInt32 ModeEndMeasure){ return 0; }

//********************************************************************************************
//********************************************************************************************
//	ФУНКЦИИ УСТАНОВКИ ПАРАМЕТРОВ, ОБЩИХ ДЛЯ ВСЕХ ПРОЦЕССОВ ИЗМЕРЕНИЯ
//======================================================================
//	Функция установки диапазона установленного измерения 
ViStatus _VI_FUNC unis4_RangeMeas (ViSession vi, ViInt32 Range)
{


	return 0; 
}
//	Функция запроса диапазона установленного измерения
ViStatus _VI_FUNC unis4_RangeMeas_Q (ViSession vi, ViPInt32 Range){ return 0; }
//	Функция установки времени интегрирования
//	Коды времени интегрирования
//#define IS4_TimeAper16ms	4//код времени интегрирования 16,67 мс (для питающей сети с частотой 60 Гц)
ViStatus _VI_FUNC unis4_Aper (ViSession vi, ViInt32 NumbAper)
{ 
	
	return 0; 
}
//	Функция запроса номера времени интегрирования
ViStatus _VI_FUNC unis4_Aper_Q (ViSession vi, ViPInt32 NumbAper){ return 0; }
//	Функция установки количества измерений в одном запуске

ViStatus _VI_FUNC unis4_NumbMeasOneStart (ViSession vi, ViInt32 NumbMeas){ return 0; }
//	Функция запроса количества измерений в одном запуске
ViStatus _VI_FUNC unis4_NumbMeasOneStart_Q (ViSession vi, ViPInt32 NumbMeas){ return 0; }
//	Функция установки разрешения автокалибровки ноля при измерении
ViStatus _VI_FUNC unis4_AutoClbrNull (ViSession vi, ViInt32 AutoClbrNull)
{


	return 0; 
}
//	Функция запроса разрешения автокалибровки ноля при измерении
ViStatus _VI_FUNC unis4_AutoClbrNull_Q (ViSession vi, ViPInt32 AutoClbrNull){ return 0; }

//********************************************************************************************
//********************************************************************************************
//	ФУНКЦИИ УСТАНОВКИ ПАРАМЕТРОВ ИЗМЕРЕНИЯ p-n ПЕРЕХОДА
//=============================================================================
//	Функция установки тока проверки
ViStatus _VI_FUNC unis4_CurrTestPNmeas (ViSession vi, ViReal64 CurrTest){ return 0; }
//	Функция запроса тока проверки
ViStatus _VI_FUNC unis4_CurrTestPNmeas_Q (ViSession vi, ViPReal64 CurrTest){ return 0; }

//********************************************************************************************
//********************************************************************************************
//	ФУНКЦИИ УСТАНОВКИ ПАРАМЕТРОВ, ОБЩИХ ДЛЯ ВСЕХ ПРОЦЕССОВ ФОРМИРОВАНИЯ
//	НАПРЯЖЕНИЯ И ПРОЦЕССА ИЗМЕРЕНИЯ СОПРОТИВЛЕНИЯ ИЗОЛЯЦИИ
//======================================================================
//	Функция установки паузы:
//	1. Для измерения сопротивления изоляции - пауза между измерениями.
//	2. Для процессов формирования - пауза удержания испытательного напряжения.
ViStatus _VI_FUNC unis4_PauseBetweenProcess (ViSession vi, ViReal64 Time){ return 0; }
//	Функция запроса времени паузы
ViStatus _VI_FUNC unis4_PauseBetweenProcess_Q (ViSession vi, ViPReal64 Time){ return 0; }
//	Функция установки тока ограничения
//#define		IS4_maxCurrLimitRins	10.0e-3
ViStatus _VI_FUNC unis4_CurrLimit (ViSession vi, ViReal64 CurrLimit){ return 0; }
//	Функция запроса тока ограничения
ViStatus _VI_FUNC unis4_CurrLimit_Q (ViSession vi, ViPReal64 CurrLimit){ return 0; }

//********************************************************************************************
//********************************************************************************************
//	ФУНКЦИИ УСТАНОВКИ ПАРАМЕТРОВ, ОБЩИХ ДЛЯ
//	ФОРМИРОВАНИЯ НАПРЯЖЕНИЯ ПОСТОЯННОГО ТОКА И ИЗМЕРЕНИЯ СОПРОТ.ИЗОЛЯЦИИ
//======================================================================
//	Функция установки диапазона измерения, испытательного напряжения,
//	времени нарастания, времени снятия

ViStatus _VI_FUNC unis4_VoltT_FVoltDC (ViSession vi,
						ViInt32 RangeV, ViReal64 VoltT,
						ViReal64 TimGrow, ViReal64 TimDiscon)
{


	return 0; 
}
//	Функция запроса диапазона измерения, испытательного напряжения,
//	времени нарастания, времени снятия
ViStatus _VI_FUNC unis4_VoltT_FVoltDC_Q (ViSession vi,
						ViPInt32 RangeV, ViPReal64 VoltT,
						ViPReal64 TimGrow, ViPReal64 TimDiscon){ return 0; }

//********************************************************************************************
//********************************************************************************************
//	ФУНКЦИИ УСТАНОВКИ ПАРАМЕТРОВ ИЗМЕРЕНИЯ СОПРОТИВЛЕНИЯ ИЗОЛЯЦИИ
//======================================================================
//	Функция установки паузы задержки начала измерения
//	после завершения установки испытательного напряжения.
ViStatus _VI_FUNC unis4_PauseStartMeas_MResIns (ViSession vi, ViReal64 Time){ return 0; }
//	Функция запроса времени паузы
ViStatus _VI_FUNC unis4_PauseStartMeas_MResIns_Q (ViSession vi, ViPReal64 Time){ return 0; }
//	Функция подключения соединителя для цепи Ix в режиме измерения сопротивления изоляции
ViStatus _VI_FUNC unis4_ConnectCircIx (ViSession vi, ViInt32 ModeIncludeShassis){ return 0; }
//	Функция запроса подключения соединителя для цепи Ix в режиме измерения сопротивления изоляции
ViStatus _VI_FUNC unis4_ConnectCircIx_Q (ViSession vi, ViPInt32 ModeIncludeShassis){ return 0; }

//********************************************************************************************
//********************************************************************************************
//	ФУНКЦИИ УСТАНОВКИ ПАРАМЕТРОВ ФОРМИРОВАНИЯ НАПРЯЖЕНИЕ ПЕРЕМЕННОГО ТОКА
//======================================================================
//	Функция установки испытательного напряжения переменного тока
ViStatus _VI_FUNC unis4_VoltT_FVoltAC (ViSession vi, ViReal64 VoltT, ViReal64 TimGrow){ return 0; }
//	Функция запроса испытательного напряжения переменного тока
ViStatus _VI_FUNC unis4_VoltT_FVoltAC_Q (ViSession vi, ViPReal64 VoltT, ViPReal64 TimGrow){ return 0; }


//********************************************************************************************
//********************************************************************************************
//	ФУНКЦИИ УСТАНОВКИ ПАРАМЕТРОВ, ОБЩИХ ДЛЯ ИЗМЕРЕНИЯ СОПРОТ.ЦЕПИ
//======================================================================
//	Функция установки диапазона формирования силы тока и значения силы тока

ViStatus _VI_FUNC unis4_CurrT_FCurrDC (ViSession vi,
						ViInt32 RangeC, ViReal64 CurrT){ return 0; }
//	Функция запроса диапазона формирования силы тока и значения силы тока
ViStatus _VI_FUNC unis4_CurrT_FCurrDC_Q (ViSession vi,
						ViPInt32 RangeC, ViPReal64 CurrT){ return 0; }
//	Функция установки напряжения ограничения

ViStatus _VI_FUNC unis4_VoltLimit (ViSession vi, ViReal64 VoltLimit){ return 0; }
//	Функция запроса тока ограничения
ViStatus _VI_FUNC unis4_VoltLimit_Q (ViSession vi, ViPReal64 VoltLimit){ return 0; }

//********************************************************************************************
//********************************************************************************************
//	ФУНКЦИИ УСТАНОВКИ ПАРАМЕТРОВ, НЕ ЯВЛЯЮЩИХСЯ ПАРАМЕТРАМИ ПРОЦЕССОВ 
//======================================================================
//	Функция подключения резистора 100 МОм
ViStatus _VI_FUNC unis4_ConnectR100MOm (ViSession vi, ViInt32 ModeIncludeR100MOm){ return 0; }
//	Функция запроса состояния подключения резистора 100 МОм
ViStatus _VI_FUNC unis4_ConnectR100MOm_Q (ViSession vi, ViPInt32 ModeIncludeR100MOm){ return 0; }

//********************************************************************************************
//********************************************************************************************
//********************************************************************************************
//********************************************************************************************
//	ФУНКЦИИ УПРАВЛЕНИЯ
//********************************************************************************************
//********************************************************************************************
//	ФУНКЦИИ УПРАВЛЕНИЯ УСТАНОВЛЕННЫМИ ПРОЦЕССАМИ
//	ИЗМЕРЕНИЯ ИЛИ ФОРМИРОВАНИЯ
//======================================================================
//	Функция запуска процесса измерения или формирования
ViStatus _VI_FUNC unis4_StartProcess (ViSession vi){ return 0; }
//	Функция остановки процесса измерения или формирования
ViStatus _VI_FUNC unis4_StopProcess (ViSession vi){ return 0; }
//	Функция запроса данных измерения
ViStatus _VI_FUNC unis4_ResultMeas (ViSession vi, ViReal64 ResultMeas[], ViPInt32 NumbResult)
{ 
	double resmeas;
	uint numres;
 	Srpc_buffer_class::Instance().is4_slot_thr[vi - 1]->get_is4_obj()->unis4_ResultMeas(resmeas, numres);
	*NumbResult = numres;
	*ResultMeas = resmeas;
	return 0; 
}
//	Функция запроса частоты напряжения переменного тока
ViStatus _VI_FUNC unis4_ResultMeasFreq (ViSession vi, ViPReal64 ResultMeasFreq){ return 0; }
//	Функция запроса текущего состояния процесса в инструменте
//	константы состояния процесса
ViStatus _VI_FUNC unis4_StatusProcess (ViSession vi, ViPUInt16 StatusMeas){ return 0; }
//	Функция запроса текущего аппаратного состояния инструмента
//	константы общего состояния инструмента
ViStatus _VI_FUNC unis4_status_Q (ViSession vi, ViPInt32 StatusDev){ return 0; }
//	Функция запуска конфигурации парам.установленного процесса
ViStatus _VI_FUNC unis4_StartConfig (ViSession vi)
{

	return 0;
}
//	Функция калибровки аппаратуры
ViStatus _VI_FUNC unis4_StartCalibr (ViSession vi)
{ 


	return 0;
}

//********************************************************************************************
//	ДОПОЛНИТЕЛЬНЫЕ ФУНКЦИИ УПРАВЛЕНИЯ ПРОЦЕССАМИ
//	ИЗМЕРЕНИЯ ИЛИ ФОРМИРОВАНИЯ
//======================================================================
//	Запуск процесса включения напряжения
ViStatus _VI_FUNC unis4_IncludeVolt (ViSession vi)
{


	return 0; 
}
//	Запуск процесса включения источника тока
ViStatus _VI_FUNC unis4_IncludeCurr (ViSession vi){ return 0; }
//	Запуск процесса чтения АЦП
ViStatus _VI_FUNC unis4_StartACP (ViSession vi)
{ 
	Srpc_buffer_class::Instance().is4_slot_thr[vi - 1]->get_is4_obj()->unis4_StartACP();

	return 0;
}
//********************************************************************************************
//********************************************************************************************
//********************************************************************************************
//********************************************************************************************
//	СЛУЖЕБНЫЕ ФУНКЦИИ ИС4										
//********************************************************************************************
//********************************************************************************************
//	Функции тестирования ИС4
//----------------------------------------------------------------------
//	Функция тестирования в режиме "Внешний контроль (ОК отключен)"
ViStatus _VI_FUNC unis4_testOK_off (ViSession vi, ViPInt16 test_result,
									ViChar message [256]){ return 0; }
//----------------------------------------------------------------------
//	Функция тестирования в режиме "Внешний контроль (ОК2 отключен)"
ViStatus _VI_FUNC unis4_testOK_off2 (ViSession vi, ViPInt16 test_result,
									ViChar message [256]){ return 0; }
//**********************************************************************
//	Прототип пользовательской функции обработчика данных по прерываниям

//********************************************************************************************
//********************************************************************************************
//********************************************************************************************
//********************************************************************************************
//********************************************************************************************
//********************************************************************************************
/*	СТАНДАРТНЫЕ ФУНКЦИИ VPP												*/
//**********************************************************************
/*	ОСНОВНЫЕ СТАНДАРТНЫЕ ФУНКЦИИ VPP									*/
/*======================================================================*/
/*  Инициализация														*/
ViStatus _VI_FUNC unis4_init (ViRsrc rsrcName,
						ViBoolean id_query,ViBoolean reset,ViPSession vi)
{ 
	is4_count++;
	*vi = is4_count;
	return 0;
}
/*----------------------------------------------------------------------*/
/*  Закрытие сеанса инструмента											*/
ViStatus _VI_FUNC unis4_close (ViSession vi){ return 0; }

//**********************************************************************
/*	СЛУЖЕБНЫЕ СТАНДАРТНЫЕ ФУНКЦИИ									*/
/*======================================================================*/
/*	Программный "сброс" (Soft Reset)									*/
ViStatus _VI_FUNC unis4_reset (ViSession vi){ return 0; }			// 6.3
/*----------------------------------------------------------------------*/
/*	Самоконтроль														*/
ViStatus _VI_FUNC unis4_self_test (ViSession vi,
						ViPInt16 test_result,ViChar _VI_FAR test_message[]){ return 0; }
/*----------------------------------------------------------------------*/
/*	Запрос очередного кода ошибки из очереди ошибок						*/
ViStatus _VI_FUNC unis4_error_query (ViSession vi,
						ViPInt32 error,ViChar _VI_FAR error_message[]){ return 0; }
/*----------------------------------------------------------------------*/
/*	Запрос сообщения об ошибке по коду ошибки							*/
ViStatus _VI_FUNC unis4_error_message (ViSession vi,
						ViStatus error,ViChar _VI_FAR message[]){ return 0; }
/*----------------------------------------------------------------------*/
/*	Запрос версии драйвера и модификации инструмента					*/
ViStatus _VI_FUNC unis4_revision_query (ViSession vi,
						ViChar _VI_FAR driver_revision[],
						ViChar _VI_FAR instrument_revision[]){ return 0; }

//********************************************************************************************
//********************************************************************************************
//********************************************************************************************
//********************************************************************************************
//	ТЕХНОЛОГИЧЕСКИЕ ФУНКЦИИ, НЕПОСТАВЛЯЕМЫЕ В FP-ФАЙЛЕ ФУНКЦИИ ДРАЙВЕРА
//***************************************************************************
//	Функция измерения сопротивления 100 МОм при его внутреннем подключении
//	в режиме измерения сопротивления изоляции при заданном напряжении  
//  меряем N раз при апертуре 20 мс и усредняет
ViStatus _VI_FUNC unis4_ResultMeasInternConnect100MOm (ViSession vi, ViReal64 testVolt, ViInt32 N, ViPReal64 ResultMeas100MOm){ return 0; }
//--------------------------------------------------------------------------------------------
//	Функция запроса кодов данных измерения
ViStatus _VI_FUNC unis4_kodResultMeas (ViSession vi, ViInt32 kodResultMeas[], ViPInt32 NumbResult){ return 0; }
//--------------------------------------------------------------------------------------------
//	Функция запроса диапазона результата последнего измерения
ViStatus _VI_FUNC unis4_RangeResultMeas_Q (ViSession vi, ViPInt32 Range){ return 0; }
//--------------------------------------------------------------------------------------------
//	Функция запроса измеренного дифференциального напряжения при измерении сопротивления цепи
ViStatus _VI_FUNC unis4_MeasVoltResCirc (ViSession vi, ViReal64 ResultMeas[], ViPInt32 NumbResult){ return 0; }
//--------------------------------------------------------------------------------------------
//	ФУНКЦИИ УСТАНОВКИ НОМЕРА ТЕСТА САМОКОНТРОЛЯ 
ViStatus _VI_FUNC unis4_NumbTest_SelfTest (ViSession vi,ViUInt32 NumbTest){ return 0; }
//--------------------------------------------------------------------------------------------
//	Функция установки источника запуска исполняемого процесса (программа или линия TTL TRG)
ViStatus _VI_FUNC unis4_SourceStart (ViSession vi, ViInt32 SourceStart){ return 0; }
//--------------------------------------------------------------------------------------------
//	Функция запроса источника запуска исполняемого процесса (программа или линия TTL TRG)
ViStatus _VI_FUNC unis4_SourceStart_Q (ViSession vi, ViPInt32 SourceStart){ return 0; }
//--------------------------------------------------------------------------------------------
//	Функция установки разрешения/запрета формирования по выбранной линия TTL TRG
//	СИ по завершении процесса
ViStatus _VI_FUNC unis4_SiEndProcess (ViSession vi, ViInt32 SiEnd){ return 0; }
//--------------------------------------------------------------------------------------------
//	Функция запроса разрешения/запрета формирования по выбранной линия TTL TRG
//	СИ по завершении процесса
ViStatus _VI_FUNC unis4_SiEndProcess_Q (ViSession vi, ViPInt32 SiEnd){ return 0; }
//	Функция установки типа анализа перегрузки
//	TypeOverload=0 - анализ перегрузки по току
//	TypeOverload#0 - анализ перегрузки по напряжению
ViStatus _VI_FUNC unis4_SetTypeOverload (ViSession vi, ViInt32 TypeOverload){ return 0; }
//	Коды технологических процессов 

//------------------------------------------------------------------------
//	ФУНКЦИИ УПРАВЛЕНИЯ ПОПРАВОЧНЫМИ КОЭФФИЦИЕНТАМИ
//----------------------------------------------------------------------------
//	Размещение данных в РПЗУ
//	Длина данных

//	Функция записи калибровочных данных в инструмент и РПЗУ
ViStatus _VI_FUNC unis4_WriteCalibrKoeffRPZU (ViSession vi,ViReal64 buff[]){ return 0; }
//	Функция чтения калибровочных данных в инструмент и РПЗУ
ViStatus _VI_FUNC unis4_ReadCalibrKoeffRPZU (ViSession vi,ViReal64 buff[]){ return 0; }
//	Функция уст. работы в инструменте с калибровочными или данными по умолч.
ViStatus _VI_FUNC unis4_set_koeff_date (ViSession vi,ViUInt32 condK,ViReal64 buff[]){ return 0; }
//	Функция чтения калибровочных или статистических данных из инструмента
ViStatus _VI_FUNC unis4_get_koeff_date (ViSession vi,ViReal64 buff[]){ return 0; }
//======================================================================
//	Функции работы с РПЗУ и с кодами исполнительных контроллер
//----------------------------------------------------------------------
//	Функция опроса готовности PLD
ViStatus _VI_FUNC unis4_readyPLDtvj (ViSession vi, ViPUInt16 StatusPLD){ return 0; }
ViStatus _VI_FUNC unis4_readyPLD (ViSession vi){ return 0; }
//	Функция загрузки исполнительных контроллеров из РПЗУ и сброса инструмента
ViStatus _VI_FUNC unis4_loadPLD (ViSession vi){ return 0; }
//	Функция загрузки РПЗУ, загрузки исполнительных контроллеров и сброса инструмента 
ViStatus _VI_FUNC unis4_prgPLD (ViSession vi){ return 0; }
//	Функция сравнения кодов исполнительных контроллеров драйвера и кодов РПЗУ
ViStatus _VI_FUNC unis4_vrfPLD (ViSession vi){ return 0; }
						

#if defined(__cplusplus) || defined(__cplusplus__)

}

#endif
