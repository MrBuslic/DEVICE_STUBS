#include "task_thread.h"

#ifdef __cplusplus
extern "C" {
#endif
#include "bos.h"


extern sysDescSPO_t sysDescSPO;

/* Создание, инициализация и активация задачи*/
UINT
taskCreate(
	USHORT numVecSt,	/* номер вектора запуска */
	int startArg		/* аргумент, передается задаче при запуске */
)
{
	sysDescSPO.pTabVecSt[numVecSt].taskStart();
	return 0;
}

UINT
taskErrCreate(void){return 0;}

 UINT
taskBAintCreate(void){return 0;}

 UINT
taskMKOouCreate(void){return 0;}

/*Создание задачи и запуск через интервал времени */

 UINT
taskInitDelay(
	USHORT numVecSt,	/* номер вектора запуска */
	int startArg,		/* аргумент, передается задаче при запуске */
	UINT delayTime	/* интервал времени */
){return 0;}

/* Создание задачи и запуск в заданное время */
UINT
taskInipTime(
	USHORT numVecSt,    /* номер вектора запуска */
	int    startArg,    /* аргумент, передается задаче при запуске */
	ULONG timeSec      /* значение времени в секундах */
){return 0;}

/* Создание задачи и запуск в заданное время */
UINT
taskIniFixTime(
	USHORT numVecSt,    /* номер вектора запуска */
	int    startArg,    /* аргумент, передается задаче при запуске */
	UINT sec,           /* значение секундной составляющей времени */
	UINT msec           /* значение миллисекундной составляющей времени @}@*/
){return 0;}

/* Создание задачи и периодический запуск */
UINT
taskPeriod(
	USHORT numVecSt, /* номер вектора запуска */
	int    startArg, /* аргумент, передается задаче при запуске */
	UINT   period    /* интервал времени в мс */
){return 0;}

UINT
taskPeriodMiss(
	USHORT numVecSt, /* номер вектора запуска */
	int    startArg, /* аргумент, передается задаче при запуске */
	UINT   period    /*= 20-0xFFFFFFFF -  интервал времени в мс @}@*/
){return 0;}

/* Создание задачи и периодический запуск */
UINT
taskPeriodPlus(
	USHORT numVecSt, /* номер вектора запуска */
	int    startArg, /* аргумент, передается задаче при запуске */
	UINT   period,   /* интервал времени в секундах*/
	UINT   offset    /* смещение в мс относительно секунды */
){return 0;}

/* Приостановка задачи */
 STATUS
taskSuspend(void){return 0;}

/* Возобновление приостановленной задачи*/
 STATUS
taskResume(
	UINT tid  /* идентификатор задачи */
){return 0;}

 STATUS
taskResumVec(
	USHORT numVecSt /* номер вектора запуска */
){return 0;}

/*	Приостановка задачи на заданный интервал времени */
 STATUS
taskDelay(
	UINT delayTime  /* интервал времени */
){
	 Sleep(delayTime);
	 return 0;}

/* Приостановка задачи до заданного времени */
STATUS
taskDelayTime(
	UINT sec, /* значение секундной составляющей времени */
	UINT msec /* значение миллисекундной составляющей времени */
){return 0;}

/* Приостановка задачи до заданного времени */
STATUS
taskDelayFixTime(
	UINT sec, /* значение секундной составляющей времени */
	UINT msec /* значение миллисекундной составляющей времени @}@*/
){return 0;}

/* Перезапуск задачи */

STATUS
taskReStart(
	UINT tid  /* идентификатор задачи */
){return 0;}

/*Получение id выполняющейся задачи*/
 UINT
taskIdSelf(void){return 0;}

/* Заполнение массива списком id активных задач*/
  int
taskIdListGet(
	UINT idList[],
	int maxTasks
){return 0;}

/* Чтение списка номеров векторов запуска задач */
int
taskVsListGet(
	USHORT VsList[],  /* адрес массива */
	int maxTasks     /* размер массива */
){return 0;}

 STATUS
taskInfoGet(
	taskInfo_t *taskInfo, /* адрес для записи информации */
	UINT 		  taskID   /* идентификатор задачи @}@*/
){return 0;}

 UINT
taskAllInfoGet(
	taskInfo_t taskInfo[], /* адрес массива */
	int maxTasks            /* количество элементов массива @}@*/
){return 0;}

/* Завершение задачи */
 void
taskExit(
	int exitCode /* код завершения задачи */
) {}

/* Удаление задачи*/
 STATUS
taskDelete(
	UINT tid    /* идентификатор задачи */
){return 0;}
 STATUS
taskDelVec(
	USHORT numVecSt /* номер вектора запуска @}@*/
){return 0;}

/* Удаление всех задач */
 STATUS
taskAllDelete(void){return 0;}

 STATUS
taskDelAllExc(
	USHORT VecStArr[], /* адрес массива векторов запуска неудаляемых задач */
	int lenArr          /* размер массива */
){return 0;}

/* Безусловное удаление всех задач */
 STATUS
taskAllDelAbs(void){return 0;}

/* Проверка существования задачи */
 UINT
taskVerify(
	USHORT  numVecSt /* номер вектора запуска */
){return 0;}

/*	проверка существования задачи с заданным идентификатором */
 UINT
taskIdVerify(
	UINT  taskId /* идентификатор задачи@}@*/
){return 0;}

/* Тестирование таймера задач */
 UINT
taskTimerMin(void){return 0;}

/*	коррекция интервала срабатывания таймера задачи */
 STATUS
taskTimerCorr(
	UINT taskId,    /* идентификатор задачи */
	UINT sizeCorr,  /* размер коррекции интервала в мс */
	UINT sigCorr){return 0;}   /* знак коррекции:
					 0 - интервал нужно увеличить на sizeCorr
					 -1 - интервал нужно уменьшить на sizeCorr */

#if __MACHINE__!=__BCVMC__
					 /*	обнуление интервала срабатывания таймера задачи */
 STATUS
taskTimerZero(
	UINT taskId    /* идентификатор задачи */
){return 0;}
#endif

/*	коррекция интервала срабатывания таймера периодической задачи */
STATUS
 taskPeriodRemCorr(
	UINT taskId,    /* идентификатор задачи */
	UINT sizeCorr,  /* размер коррекции интервала в мс: 1 - 998, но меньше периода задачи */
	UINT sigCorr){return 0;}  /* знак коррекции:
					0 - интервал нужно увеличить на sizeCorr
					-1 - интервал нужно уменьшить на sizeCorr */

					/* Проверка существования других задач */
 BOOL
taskReVerify(void){return 0;}

/* Создание нового вектора запуска и включение его в TabVecSt[] */
 USHORT  /* numVecSt - номер вектора запуска */
VecStCreate(
	VecSt_t	 *p_VecSt	/* указатель на вектор запуска */
){return 0;}

/* Чтение вектора запуска */
 STATUS
VecStRead(
	USHORT  numVecSt, /* номер вектора запуска */
	VecSt_t *p_VecSt /* указатель на вектор запуска */
){return 0;}

/* Модификация вектора запуска */
 STATUS
VecStModify(
	USHORT  numVecSt, /* номер вектора запуска */
	VecSt_t  *p_VecSt  /* указатель на вектор запуска */
){return 0;}


 TasksFactory::TasksFactory()
 {
	 for (int i = 0; i < 1; i++)
		 mutexes << new QMutex;
 }

#ifdef __cplusplus
}
#endif