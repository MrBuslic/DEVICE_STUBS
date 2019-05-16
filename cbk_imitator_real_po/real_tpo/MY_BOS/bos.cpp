#ifdef __cplusplus
extern "C" {
#endif
#include "bos.h"
#ifdef __cplusplus
}
#endif
#include "task_thread.h"
#include "thread_module.h"

void TasksFactory::lock_mutex(int ind, int timeout)
{
	mutexes[ind]->tryLock(timeout);
}
void TasksFactory::unlock_mutex(int ind)
{
	mutexes[ind]->unlock();
}
int TasksFactory::add_mutex()
{
	mutexes << new QMutex;
	return mutexes.count() - 1;
}
void TasksFactory::remove_mutex(int ind)
{
	
}

extern UINT 	semCreate(void) { return STasksFactory::Instance().add_mutex(); }
extern STATUS  semLock(UINT semId, int timeout) { STasksFactory::Instance().lock_mutex(semId, timeout); return 0; }
extern STATUS  semUnlock(UINT semId) { STasksFactory::Instance().unlock_mutex(semId); return 0; }
extern STATUS 	semDelete(UINT semId) { STasksFactory::Instance().remove_mutex(semId); return 0; }
extern UINT    semStateGet(UINT semId) { return 0; }

#ifdef __cplusplus
extern "C" {
#endif
	/*	Чтение значения времени */
	extern STATUS
		timeRead(
		sysTime_t  *time  /*- указатель на область ОЗУ для записи времени*/
		) 
		{
		int tmp_time = 0;
		int sec = 0;
		int msec = 0;
		tmp_time = STimeThread::Instance().getCurTime();
		msec = tmp_time % 1000;
		sec = tmp_time / 1000;
		time->time_sec = sec;
		time->time_msec = msec;
			return 0;
		}


	/* Установка системного времени */
	extern STATUS
		timeSet(
		UINT  timeSec  /*- значение времени в секундах*/
		) {
			return 0;
		}

	/*	Чтение значения времени */
	STATUS
		timeReadmics(
		micsTime_t  *time  /*- указатель на область ОЗУ для записи времени@}@ */
		) {
			return 0;
		}

	extern UINT  ErrMKIGet(UINT* adr, UINT len){ return 0; }

	/* получение информации об ошибках, обнаруженных тестами */
	extern UINT
		testMDIGet(
		UINT* adr, /* адрес области ОЗУ для записи МДИ */
		UINT len  /* размер области ОЗУ для записи МДИ */
		){
			return 0;
		}

	/* получение информации об ошибках, обнаруженных тестами */
	extern STATUS
		testErrGet(
		UINT* adr /* адрес области ОЗУ СПО для записи информации */
		){
			return 0;
		}


	/* очистка кэш-памяти */
	extern void
		cacheFlush(void){  }

	/* сброс запроса на прерывания */
	extern STATUS
		sprIQClear(
		UINT numChan    /* номер канала */
		){
			return 0;
		}

	extern STATUS
		sprIMaskVec(
		USHORT VecMask  /* вектор масок */
		){
			return 0;
		}

	STATUS
		cfgSysGet(
		cfgSYS_t * cfgCBK /* указатель на область для записи конфигурации ЦБК @}@*/
		){
		cfgCBK->vm = 7;
		cfgCBK->rs = 8;
		cfgCBK->er = 0;
		cfgCBK->of = 8;
		cfgCBK->m0 = 1;
		cfgCBK->b0 = 0;
		cfgCBK->m1 = 1;
		cfgCBK->b1 = 0;
		cfgCBK->no = 0;
		cfgCBK->dv = 1;
		cfgCBK->rg = CFGSYS_rgWork;

		return 0;
		}


	/* отключение отказавшей ВМ */
	UINT
		synchPowerOff(
		UINT  numVM  /* = 0-3 - номер отключаемой ВМ */
		){
			return 0;
		}

	UINT
		synchMasterChVM(
		UINT numMKO /* (= 0-1) номер MKO, для которого нужно произвести
					замену ведущей ВМ*/
					){
			return 0;
		}

	/* Разрешение проведения восстановления конфигурации */
	STATUS
		synchRecEnable(
		UINT interval, /*интервал времени в мс, в течение которого действует разрешение */
		dataArr_t dataArr[], /* указатель на массив диапазонов памяти,
							 восстанавливаемой в подключаемой ВМ */
							 UINT numArr,   /* количество восстанавливаемых диапазонов памяти */
							 UINT typRec    /* тип реконфигурации:
											0 - восстановление конфигурации
											1 - периодическая реконфигурация */

											){
			return 0;
		}

	/* выполнение задания на коррекцию ПЗУ */
	extern STATUS
		flashCorr(
		bppJob_t *bppJob,
		UINT typCtl
		){
			return 0;
		}


	/* запуск интегрального теста БТПО  */
	extern STATUS
		testStart(
		UINT num /*=1-6 - номер интегрального теста@}@*/
		){
			return 0;
		}


	/*получение состояния флага запроса на прерывания */
	extern UINT
		sprIGetF(
		UINT numChan    /* номер канала */
		){
			return 0;
		}


	UINT
		numVMGet(void){ return 0; }


	UINT
		synchPowerOn(
		UINT  numVM  /* = 0-3 - номер включаемой ВМ*/
		)
	{
			return 0;
		}


	UINT
		ErrVMDIget(
			UINT* adr, /* адрес области ОЗУ СПО для записи ДИ */
			UINT len   /* размер в словах области ОЗУ для записи ДИ@}@*/
		)
	{
		return 0;
	}


	/* получение информации о поступлении секундной метки времени*/
	extern UINT timeMarktst(void) { return 0; }

	/* Синхронизация и согласование массива информации */
	UINT
		synchInf(
			UCHAR synchId,  /* = 0 - 63 - идентификатор точки синхронизации */
			UINT  arrInf[], /* адрес массива согласуемой информации */
			USHORT lenArr   /* размер массива в словах */
		)

	{
		return 0;
	}



#ifdef __cplusplus
}
#endif