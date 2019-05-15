
/*! \file VecUnit.c
 * \author Гапонов Р.В.
 * \date 2012.10.28
 * \version 1.0.0
 * \brief Системные данные СПО
 * \warning Добавить в Dependencies проекта *.e файл из основного проекта
 */

/*
[ЦБК = НОК, загрузка в RAM, с печатью (отладка)]
адреса в настройках проекта:
  data segment: 0x90000
  text segment: 0x90000
флаги в настройках проекта:
  препроцессор: -D__ALLOW_PRINT__ -D__MACHINE__=__CBK__
  компилятор:   -D__ALLOW_PRINT__ -D__MACHINE__=__CBK__

[ЦБК = НОК, загрузка в ROM, c печатью (отладка)]
адреса в настройках проекта:
  data segment: SPO_SYS_DATA_ADDR
  text segment: SPO_SYS_DATA_ADDR
флаги в настройках проекта:
  препроцессор: -D__ALLOW_PRINT__ -D__MACHINE__=__CBK__
  компилятор:   -D__ALLOW_PRINT__ -D__MACHINE__=__CBK__

[ЦБК = НОК, загрузка в ROM, без печати (работа)]
адреса в настройках проекта:
  data segment: SPO_SYS_DATA_ADDR
  text segment: SPO_SYS_DATA_ADDR
флаги в настройках проекта:
  препроцессор: -D__MACHINE__=__CBK__
  компилятор:   -D__MACHINE__=__CBK__

[ЦБК = цех, загрузка в RAM, с печатью (отладка)]
адреса в настройках проекта:
  data segment: 0x90000
  text segment: 0x90000
флаги в настройках проекта:
  препроцессор: -D__ALLOW_PRINT__ -D_SPO_IN_SYS_ -D__MACHINE__=__CBK__
  компилятор:   -D__ALLOW_PRINT__ -D_SPO_IN_SYS_ -D__MACHINE__=__CBK__

[ЦБК = цех, загрузка в ROM, c печатью (отладка)]
адреса в настройках проекта:
  data segment: SPO_SYS_DATA_ADDR
  text segment: SPO_SYS_DATA_ADDR
флаги в настройках проекта:
  препроцессор: -D__ALLOW_PRINT__ -D_SPO_IN_SYS_ -D__MACHINE__=__CBK__
  компилятор:   -D__ALLOW_PRINT__ -D_SPO_IN_SYS_ -D__MACHINE__=__CBK__

[ЦБК = цех, загрузка в ROM, без печати (работа)]
адреса в настройках проекта:
  data segment: SPO_SYS_DATA_ADDR
  text segment: SPO_SYS_DATA_ADDR
флаги в настройках проекта:
  препроцессор: -D_SPO_IN_SYS_ -D__MACHINE__=__CBK__
  компилятор:   -D_SPO_IN_SYS_ -D__MACHINE__=__CBK__
*/

#include "VecUnit.h"
#include "MainUnit.h"
#include "BaloiUnit_v11.h"
//================================================================================

#if !defined (OPO_V031) && !defined (OPO_V033)
  #error Не определена версия ОПО ЦБК!
#endif
//================================================================================

// Таблица векторов запуска
#ifdef OPO_V031
// для версии ОПО v031
VecSt_t TabVec[VEC_TABLE_SIZE] = 
{
  /* 00 */ { MainTask,          (void*) 0, 0, 3, 0, 0 },        // стартовая задача СПО 
  /* 01 */ { ErrorTask,         (void*) 0, 0, 2, 0, 0 },        // задача обработки ошибок СПО
  /* 02 */ { IrqTask,           (void*) 0, 0, 1, 0, 0 },        // задача обработки прерываний от БА
  /* 03 */ { TestTask,          (void*) 0, 0, 2, 0, 0 },        // тестовая задача
  /* 04 */ { AsnPeriodicTask,   (void*) 0, 0, 2, 0, 0 },        // задача расчета разбега времени с АСН
  
  /* 05 */ { BaloiPeriodicTask, (void*) 0, 0, 2, 0, 0 },        // 32 задачи для периодической выдачи пакетов по МКО
  /* 06 */ { BaloiPeriodicTask, (void*) 0, 0, 2, 0, 0 },
  /* 07 */ { BaloiPeriodicTask, (void*) 0, 0, 2, 0, 0 },
  /* 08 */ { BaloiPeriodicTask, (void*) 0, 0, 2, 0, 0 },
  /* 09 */ { BaloiPeriodicTask, (void*) 0, 0, 2, 0, 0 },
  /* 10 */ { BaloiPeriodicTask, (void*) 0, 0, 2, 0, 0 },
  /* 11 */ { BaloiPeriodicTask, (void*) 0, 0, 2, 0, 0 },
  /* 12 */ { BaloiPeriodicTask, (void*) 0, 0, 2, 0, 0 },
  
  /* 13 */ { BaloiPeriodicTask, (void*) 0, 0, 2, 0, 0 },
  /* 14 */ { BaloiPeriodicTask, (void*) 0, 0, 2, 0, 0 },
  /* 15 */ { BaloiPeriodicTask, (void*) 0, 0, 2, 0, 0 },
  /* 16 */ { BaloiPeriodicTask, (void*) 0, 0, 2, 0, 0 },
  /* 17 */ { BaloiPeriodicTask, (void*) 0, 0, 2, 0, 0 },
  /* 18 */ { BaloiPeriodicTask, (void*) 0, 0, 2, 0, 0 },
  /* 19 */ { BaloiPeriodicTask, (void*) 0, 0, 2, 0, 0 },
  /* 20 */ { BaloiPeriodicTask, (void*) 0, 0, 2, 0, 0 },

  /* 21 */ { BaloiPeriodicTask, (void*) 0, 0, 2, 0, 0 },
  /* 22 */ { BaloiPeriodicTask, (void*) 0, 0, 2, 0, 0 },
  /* 23 */ { BaloiPeriodicTask, (void*) 0, 0, 2, 0, 0 },
  /* 24 */ { BaloiPeriodicTask, (void*) 0, 0, 2, 0, 0 },
  /* 25 */ { BaloiPeriodicTask, (void*) 0, 0, 2, 0, 0 },
  /* 26 */ { BaloiPeriodicTask, (void*) 0, 0, 2, 0, 0 },
  /* 27 */ { BaloiPeriodicTask, (void*) 0, 0, 2, 0, 0 },
  /* 28 */ { BaloiPeriodicTask, (void*) 0, 0, 2, 0, 0 },

  /* 29 */ { BaloiPeriodicTask, (void*) 0, 0, 2, 0, 0 },
  /* 30 */ { BaloiPeriodicTask, (void*) 0, 0, 2, 0, 0 },
  /* 31 */ { BaloiPeriodicTask, (void*) 0, 0, 2, 0, 0 },
  /* 32 */ { BaloiPeriodicTask, (void*) 0, 0, 2, 0, 0 },
  /* 33 */ { BaloiPeriodicTask, (void*) 0, 0, 2, 0, 0 },
  /* 34 */ { BaloiPeriodicTask, (void*) 0, 0, 2, 0, 0 },
  /* 35 */ { BaloiPeriodicTask, (void*) 0, 0, 2, 0, 0 },
  /* 36 */ { BaloiPeriodicTask, (void*) 0, 0, 2, 0, 0 }
};
#endif
//================================================================================

#ifdef OPO_V033
// для версии ОПО v033
VecSt_t TabVec[VEC_TABLE_SIZE] = 
{
  /* 00 */ { MainTask,          (void*) 0, 0, 3, 0, _SYS_TASK_SPO_, 0 },        // стартовая задача СПО 
  /* 01 */ { ErrorTask,         (void*) 0, 0, 2, 0, _SYS_TASK_SPO_, 0 },        // задача обработки ошибок СПО
  /* 02 */ { IrqTask,           (void*) 0, 0, 1, 0, _SYS_TASK_SPO_, 0 },        // задача обработки прерываний от БА
  /* 03 */ { TestTask,          (void*) 0, 0, 2, 0, _SYS_TASK_SPO_, 0 },        // тестовая задача
  /* 04 */ { AsnPeriodicTask,   (void*) 0, 0, 2, 0, _SYS_TASK_SPO_, 0 },        // задача расчета разбега времени с АСН
  
  /* 05 */ { BaloiPeriodicTask, (void*) 0, 0, 2, 0, 0, 0 },                     // 32 задачи для периодической выдачи пакетов по МКО
  /* 06 */ { BaloiPeriodicTask, (void*) 0, 0, 2, 0, 0, 0 },
  /* 07 */ { BaloiPeriodicTask, (void*) 0, 0, 2, 0, 0, 0 },
  /* 08 */ { BaloiPeriodicTask, (void*) 0, 0, 2, 0, 0, 0 },
  /* 09 */ { BaloiPeriodicTask, (void*) 0, 0, 2, 0, 0, 0 },
  /* 10 */ { BaloiPeriodicTask, (void*) 0, 0, 2, 0, 0, 0 },
  /* 11 */ { BaloiPeriodicTask, (void*) 0, 0, 2, 0, 0, 0 },
  /* 12 */ { BaloiPeriodicTask, (void*) 0, 0, 2, 0, 0, 0 },
  
  /* 13 */ { BaloiPeriodicTask, (void*) 0, 0, 2, 0, 0, 0 },
  /* 14 */ { BaloiPeriodicTask, (void*) 0, 0, 2, 0, 0, 0 },
  /* 15 */ { BaloiPeriodicTask, (void*) 0, 0, 2, 0, 0, 0 },
  /* 16 */ { BaloiPeriodicTask, (void*) 0, 0, 2, 0, 0, 0 },
  /* 17 */ { BaloiPeriodicTask, (void*) 0, 0, 2, 0, 0, 0 },
  /* 18 */ { BaloiPeriodicTask, (void*) 0, 0, 2, 0, 0, 0 },
  /* 19 */ { BaloiPeriodicTask, (void*) 0, 0, 2, 0, 0, 0 },
  /* 20 */ { BaloiPeriodicTask, (void*) 0, 0, 2, 0, 0, 0 },

  /* 21 */ { BaloiPeriodicTask, (void*) 0, 0, 2, 0, 0, 0 },
  /* 22 */ { BaloiPeriodicTask, (void*) 0, 0, 2, 0, 0, 0 },
  /* 23 */ { BaloiPeriodicTask, (void*) 0, 0, 2, 0, 0, 0 },
  /* 24 */ { BaloiPeriodicTask, (void*) 0, 0, 2, 0, 0, 0 },
  /* 25 */ { BaloiPeriodicTask, (void*) 0, 0, 2, 0, 0, 0 },
  /* 26 */ { BaloiPeriodicTask, (void*) 0, 0, 2, 0, 0, 0 },
  /* 27 */ { BaloiPeriodicTask, (void*) 0, 0, 2, 0, 0, 0 },
  /* 28 */ { BaloiPeriodicTask, (void*) 0, 0, 2, 0, 0, 0 },

  /* 29 */ { BaloiPeriodicTask, (void*) 0, 0, 2, 0, 0, 0 },
  /* 30 */ { BaloiPeriodicTask, (void*) 0, 0, 2, 0, 0, 0 },
  /* 31 */ { BaloiPeriodicTask, (void*) 0, 0, 2, 0, 0, 0 },
  /* 32 */ { BaloiPeriodicTask, (void*) 0, 0, 2, 0, 0, 0 },
  /* 33 */ { BaloiPeriodicTask, (void*) 0, 0, 2, 0, 0, 0 },
  /* 34 */ { BaloiPeriodicTask, (void*) 0, 0, 2, 0, 0, 0 },
  /* 35 */ { BaloiPeriodicTask, (void*) 0, 0, 2, 0, 0, 0 },
  /* 36 */ { BaloiPeriodicTask, (void*) 0, 0, 2, 0, 0, 0 }
};
#endif
//================================================================================

/*
Параметры векторов:
1. точка старта
2. адрес стека (если 0 - автоматическое назначение с параметрами по умолчанию)
3. размер стека 
4. приоритет для планирования: от 1 до MAX_PRI (1 - наивысший)
5. необходимость использования сопроцессора
6. предельное время выполнения задачи в мс (если 0 - не контролировать)
*/
//================================================================================

// Дескриптор СПО
sysDescSPO_t sysDescSPO =
{
TabVec,  		/* адрес таблицы векторов запуска */
36,       		/* максимальный номер в таблице векторов (существующий реально) */
VEC_TABLE_SIZE,     	/* максимально допустимый номер в таблице векторов (на такое количество зарезервировано памяти) */
0,       		/* номер вектора стартовой задачи */
1,       		/* номер вектора задачи обработки ошибок */
2,       		/* номер вектора задачи обслуживания импульсных прерываний */
0xDEFC   		/* тип параметра на входе задачи обработки прерываний: 0x0 - номер вектора, 0xDEFC - маска векторов */
};
//================================================================================

