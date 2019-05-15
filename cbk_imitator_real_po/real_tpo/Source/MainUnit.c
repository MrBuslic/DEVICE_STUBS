
/*! \file MainUnit.c
 * \author Гапонов Р.В.
 * \date 2012.10.28
 * \version 1.0.0
 * \brief Модуль содержит основные задачи СПО (функции, вызываемые по событиям из ОПО)
 */

#include "MainUnit.h"
//================================================================================

/*! \fn void MainTask (int AParam);
 * \brief Стартовая задача СПО (вектор = 0)
 * \param [in] AParam - режим запуска СПО (см. руководство программиста по ОПО БОС)
 * \return При нормальном функционировании ЦБК управление не возвращает
 * \details Стартовая задача запускается из ОПО после завершения самотестирования ЦБК,
 * сохраняет переданный из ОПО режим запуска, запрашивает у ОПО массивы с результатами
 * самотестирования ЦБК, производит инициализацию данных СПО и запуск основного цикла ПДО
 */

//__asm__(".align 4");
void MainTask (int AParam)
{
void *Src;

MSGS("\nMainTask(): Запущена базовая задача СПО\n");

// переносим полученную из ОПО строку с номером вермии
//Src = (void*)(RAM_SPO + 0x00100000 - sizeof(OpoVersionString)); // буфер памяти у границы 1Мб RAM СПО
//memmove(&OpoVersionString, Src, sizeof(OpoVersionString));
//
//// переносим контрольные суммы всех секторов флеша
//Src = (void*)(RAM_SPO + 0x00100000);
//memmove(&FlashSectorCSumTable, Src, sizeof(FlashSectorCSumTable));

// инициализация контрольных переменных активности СПО
ErrorTaskIn = ErrorTaskOut = 0;
IrqTaskIn = IrqTaskOut = 0;
TestTaskIn = TestTaskOut = 0;
AsnTaskIn = AsnTaskOut = 0;

MainTaskPdoCount = 0;
MainTaskCmdFail = 0;
MainTaskIn++;

// сохранение параметра, переданного из ОПО
PostParam = AParam;

// считывание массивов МКИ/МДИ
MainTaskGetMkiData();
MainTaskGetMdiData();

// коррекция конфигурации ЦБК (обход двойного запуска ЦБК, отладка)
// MainTaskDoReconfigCbk();

// инициализация структур базовой задачи
MainTaskInit();
// запуск основного цикла СПО
MainTaskRun();

// сюда дойти не должно
MSGS("\nMainTask(): Завершена базовая задача СПО\n");
taskExit(0);
}
//================================================================================

/*! \fn void ErrorTask (UINT *AdrBuf);
 * \brief Задача обработки ошибок (вектор = 1)
 * \param [in] AdrBuf - адрес буфера с детальной информацией о месте и причине возникшего сбоя (см. руководство программиста по ОПО БОС)
 * \return В данной версии СПО всегда возвращает 0 (ошибок нет)
 * \detail Задача запускается ОПО при возникновении в процессе работы ЦБК ошибок, требующих особого внимания СПО.
 * Данная версия СПО только информирует КПА о сбое.
 */

UINT FailVmDiag[1024];

//__asm__(".align 4");
void ErrorTask (UINT *AdrBuf)
{
int Result, BaseMkoLine, BaseMkoChan;
UINT MkiLen, MkiID, TaskID, *ErrInf, Len;
int DataSize, RemainSize, BlockSize;
char *Src;
UINT Buf[8];
// int primFunc, numFunc, numMod, typErr, numERR, lenMKI;

MSGS("================================================================================");
MSGS("ErrorTask(): Запущена задача обработки ошибок");

ErrorTaskIn++;

if (SpoConfig.TaskErrorSendInfo)
  {
  MkiLen = AdrBuf[0];
  MkiID  = AdrBuf[1]; // = ErrCod
  TaskID = AdrBuf[2];
  ErrInf = (UINT*) &AdrBuf[3]; // = начальный адрес внешнего буфера ErrInf

  Buf[0] = AdrBuf[0];
  Buf[1] = AdrBuf[1];
  Buf[2] = AdrBuf[2];

  // параметры линий обмена с КПА
  BaseMkoLine = SpoConfig.KpaMkoLine & 1;
  BaseMkoChan = SpoConfig.KpaMkoChan & 1;

  // передача в КПА заголовка ошибки
  memmove(&MkoBufX, &Buf, 3 * sizeof(UINT));
  Result = MkoXchgXn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, 30, MKO_TX, 6), 0);

  // передача в КПА данных по ошибке
  if (SpoConfig.TaskErrorInfoSize)
    {
    Len = SpoConfig.TaskErrorInfoSize;
    memmove(&MkoBufX, ErrInf, Len * sizeof(UINT));
    Result = MkoXchgXn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, 30, MKO_TX, Len * sizeof(WORD)), 0);
    }

  // запрашиваем данные из сбойной ВМ
  DataSize = ErrVMDIget((UINT*) &FailVmDiag, sizeof(FailVmDiag) / sizeof(UINT)); 
  if ((DataSize != 0) && (DataSize != ERROR))
    {
    DataSize *= sizeof(UINT);
    Src = (char*) &FailVmDiag;

    // флаг начала данных + размер данных
    MkoBufX[0][0] = 0xDB01;
    MkoBufX[0][1] = DataSize;
    MkoXchgXn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_DEBUG, MKO_TX, 2), 0);

    // передаем данные из сбойной ВМ
    RemainSize = DataSize;

    while (RemainSize)
      {
      BlockSize = (RemainSize > 64) ? (64) : (RemainSize);
      memmove(&MkoBufX, Src, BlockSize);
      RemainSize -= BlockSize;
      Src += BlockSize;
      MkoXchgXn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_DEBUG, MKO_TX, BlockSize / 2), 0);
      }

    // флаг конца данных + размер данных в словах
    MkoBufX[0][0] = 0xDB02;
    MkoBufX[0][1] = DataSize;
    MkoXchgXn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_DEBUG, MKO_TX, 2), 0);
    }
  }

#if (0)
primFunc = (MkiID >> 24) & 0x0F;
numFunc  = (MkiID >> 20) & 0x0F;
numMod   = (MkiID >> 16) & 0x0F;
typErr   = (MkiID >> 12) & 0x0F;
numERR   = (MkiID >> 8) & 0x0F;
lenMKI   = (MkiID & 0xFF);

MSGS("Размер данных блока МКИ = %d", MkiLen);
MSGS("МКИ ID = %x", MkiID);
MSGS("Task ID = %d", TaskID);

MSGS("размер МКИ в словах = %d", lenMKI);

MSGS("номер модуля БОС = %d, инфо о модуле:", numMod);
switch (numMod)
  {
  case  1: MSGS("  M_initBOS = 1: инициализация БОС"); break;
  case  2: MSGS("  M_sysBOS = 2: системные функции БОС"); break;
  case  3: MSGS("  M_taskLib = 3: управление задачами"); break;
  case  4: MSGS("  M_segLib = 4: управление сегментами"); break;
  case  5: MSGS("  M_timeLib = 5: управление временем"); break;
  case  6: MSGS("  M_timerLib = 6: управление таймерами"); break;
  case  7: MSGS("  M_semLib = 7: управление семафорами"); break;
  case  8: MSGS("  M_addLib = 8: дополнительные функции"); break;
  case  9: MSGS("  M_errLib = 9: отработка ошибок"); break;
  case 10: MSGS("  M_mkoLib = 10: организация обмена по МКО"); break;
  case 11: MSGS("  M_radSpr = 11: обслуживание радиальных прерываний"); break;
  case 12: MSGS("  M_Synch = 12: организация синхронизации ВМ"); break;
  default: MSGS("  Неизвестный тип модуля");
  }

MSGS("тип ошибки = %d, инфо об ошибке:", typErr);
switch (typErr)
  {
  case  1: MSGS("  ERR_PARAM = 1: ошибочные значения параметров функций"); break;
  case  2: MSGS("  ERR_MKO = 2: ошибки обмена по МКО"); break;
  case  4: MSGS("  ERR_SYS_SPO = 4: некорректные системные данные СПО или нарушение логики работы"); break;
  case  8: MSGS("  ERR_SYS_BOS = 8: некорректные системные данные БОС или нарушение логики работы"); break;
  case 12: MSGS("  ERR_TIMEOUT = 12: истекло контрольное время ожидания события: выполнения задачи, ожидания семафора"); break;
  case 14: MSGS("  ERR_SYNCH = 14: ошибки, обнаруженные при синхронизации ВМ"); break;
  case 15: MSGS("  ERR_LDR_BTPO = 15: ошибки, обнаруженные при загрузке ПО"); break;
  default: MSGS("  Неизвестный тип ошибки");
  }

MSGS("номер ошибки (типа typErr) = %d", numERR);
if (numMod == 1) // M_initBOS
  {
  switch (numERR)
    {
    case 1: MSGS("  "); break;
    }
  }
if (numMod == 2) // M_sysBOS
  {
  switch (numERR)
    {
    case 1: MSGS("  "); break;
    }
  }
if (numMod == 3) // M_taskLib
  {
  switch (numERR)
    {
    case  1: MSGS("  numERR = 1: недопустимое значение номера вектора запуска (номер=%d, мин=%d, макс=%d)", ErrInf[0], ErrInf[1], ErrInf[2]); break;
    case  2: MSGS("  numERR = 2: недопустимое значение времени запуска задачи (задано,с=%d, задано,мс=%d, текущее,с=%d, текущее,мс=%d)", ErrInf[0], ErrInf[1], ErrInf[2], ErrInf[3]); break;
    case  3: MSGS("  numERR = 3: недопустимое значение адреса области ОЗУ СПО для записи требуемых данных (адрес=%x, мин=%x, макс=%x)", ErrInf[0], ErrInf[1], ErrInf[2]); break;
    case  4: MSGS("  numERR = 4: запрос на создание задачи отработки ошибок (заданный вектор=%d, вектор ошибок=%d)", ErrInf[0], ErrInf[1]); break;
    case  5: MSGS("  numERR = 5: запрос на удаление задачи отработки ошибок (заданный вектор=%d, вектор ошибок=%d)", ErrInf[0], ErrInf[1]); break;
    case  6: MSGS("  numERR = 6: запрос на перезапуск задачи отработки ошибок (заданный вектор=%d, вектор ошибок=%d)", ErrInf[0], ErrInf[1]); break;
    case  7: MSGS("  numERR = 7: запрос на приостановку задачи отработки ошибок (заданный вектор=%d, вектор ошибок=%d)", ErrInf[0], ErrInf[1]); break;
    case  8: MSGS("  numERR = 8: запрос на удаление неудаляемой задачи (ID задачи=%d, вектор задачи=%d)", ErrInf[0], ErrInf[1]); break;
    case  9: MSGS("  numERR = 9: недопустимое значение периода запуска задачи (задано=%d, мин=%d, макс=%d)", ErrInf[0], ErrInf[1], ErrInf[2]); break;
    case 10: MSGS("  numERR = 10: недопустимый размер коррекции интервала времени, оставшегося до запуска задачи (значение=%d, размер=%d, знак=%d)", ErrInf[0], ErrInf[1], ErrInf[2]); break;
    default: MSGS("  Неизвестный тип ошибки"); break;
    }
  }
if (numMod == 4) // M_segLib
  {
  switch (numERR)
    {
    case 1: MSGS("  "); break;
    }
  }
if (numMod == 5) // M_timeLib
  {
  switch (numERR)
    {
    case 1: MSGS("  "); break;
    }
  }
if (numMod == 6) // M_timerLib
  {
  switch (numERR)
    {
    case 1: MSGS("  "); break;
    }
  }
if (numMod == 7) // M_semLib
  {
  switch (numERR)
    {
    case 1: MSGS("  "); break;
    }
  }
if (numMod == 8) // M_addLib
  {
  switch (numERR)
    {
    case 1: MSGS("  "); break;
    }
  }
if (numMod == 9) // M_errLib
  {
  switch (numERR)
    {
    case 1: MSGS("  "); break;
    }
  }
if (numMod == 10) // M_mkoLib
  {
  switch (numERR)
    {
    case 1: MSGS("  "); break;
    }
  }
if (numMod == 11) // M_radSpr
  {
  switch (numERR)
    {
    case 1: MSGS("  "); break;
    }
  }
if (numMod == 12) // M_Synch
  {
  switch (numERR)
    {
    case 1: MSGS("  "); break;
    }
  }

MSGS("номер функции (в модуле) = %d", numFunc);
MSGS("номер функции = %d", primFunc);
MSGS("================================================================================");
#endif

ErrorTaskOut++;
taskExit(0);
}
//================================================================================

/*! \fn void IrqTask (int AChanMask);
 * \brief Задача обработки прерываний от аппаратуры (вектор = 2)
 * \param [in] AChanMask - 16бит маска состояния каналов прерываний от БА (см. руководство программиста по ОПО БОС)
 * \return В данной версии СПО всегда возвращает 0 (ошибок нет)
 * \detail Задача запускается из ОПО если обнаружено разрешенное прерывание от БА.
 * Сохраняет в конфигурации СПО время возникновения прерывания, если при инициализации СПО 
 * был получен достоверный адрес КПА на шине МКО, вызывает соответствующий прерыванию обработчик.
 */

//__asm__(".align 4");
void IrqTask (int AChanMask)
{
MSGS("\nIrqTask(): Запущена задача обработки прерываний от аппаратуры, маска каналов: %16x\n", AChanMask);

// счетчик входов в обработчик
IrqTaskIn++;

// сохраняем время получения прерывания
timeRead(&IrqTime);

// если адрес КПА настроен
if (KPADetected)
  {
  // обработка принятого прерывания
  HandleIrqBA(AChanMask);
  }

IrqTaskOut++;
taskExit(0);
}
//================================================================================

/*! \fn void TestTask (int AParam);
 * \brief Тестовая задача для проверки функций БОС (вектор = 3)
 * \param [in] AParam - параметр задачи (см. руководство программиста по ОПО БОС)
 * \return Никаких данных не возвращает
 * \details Запускается по команде от КПА для проверки системы задач БОС
 */

//__asm__(".align 4");
void TestTask (int AParam)
{
int BaseMkoLine, BaseMkoChan;
char Buffer[64];
WORD Msg;

// счетчик входов в задачу
TestTaskIn++;

// параметры линий обмена с КПА
BaseMkoLine = SpoConfig.KpaMkoLine & 1;
BaseMkoChan = SpoConfig.KpaMkoChan & 1;

// пауза при запуске задачи для завершения внешних процессов
taskDelay(1000);

while (1)
  {
  #if (0) 
  // запрещаем все прерывания
  sprIMaskVec(0xFFFF);

  // передаем сообщение на отладочный подадрес
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_ERROR_POINT_4, KPA_SUBADDR_DEBUG);

  // разрешаем прерывания
  sprIMaskVec(~SpoConfig.EnabledIrqMask);

  // приостанавливаем задачу на 5 сек
  taskDelay(5000);
  #endif

  ExtTaskCount++; 

  Msg = 0xFA03;
  Output (1, 1, 25, 1, (WORD*) &Msg);
  
  memcpy(&Buffer, (void*) 0x1FC001C0, 64);
  ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_ERROR_POINT_4, KPA_SUBADDR_DEBUG, (void*) &Buffer, sizeof(Buffer));

  ExtTaskCount++; 

  taskDelay(5000);
  }

TestTaskOut++;
}
//================================================================================

/* тестовая задача для работы с АСН */

//__asm__(".align 4");
void AsnPeriodicTask (int AParam)
{
AsnTaskIn++;
timeRead((sysTime_t*) &AsnPeriodicTaskTime);
AsnTaskOut++;
taskExit(0);
}
//================================================================================

/*! \fn void OpoToSpoHookProc (unsigned Param);
 * \brief Перехват управления СПО из ОПО перед запуском СПО (выполняется в режиме SUPERVISOR)
 * \param [in] Param - код точки, из которой передано управление в функцию (см. руководство программиста по ОПО БОС)
 * \return В данной версии СПО всегда возвращает 0 (реально возвращаемое значение не используется)
 * \detail Функция вызывается в определнные моменты при инициализации БДО/ОПО, для выполнения
 * действий в СПО, которые могут быть совершены только в режиме процессора SUPERVISOR.
 * В данной версии функция копирует строку с версией ОПО в область памяти СПО.
 */

//__asm__(".align 4");
unsigned OpoToSpoHookProc (unsigned Param)
{
Arlosi_t *Arlosi;
DWORD Sector, Index, Sum, *Src, *Dst;

// ждем пока завершится тестирование RAM
Arlosi = (Arlosi_t*) ARLOSI_BASE_ADDR;
if (Arlosi->ntdsi < TABSYS_tdsi_SPO_ram) return 0;
if (Param != HOOK_ldrAll) return 0;

// копирование строки с версией ОПО
Src = (DWORD*) 0xBFC001C0; // адрес строки
Dst = (DWORD*) (RAM_SPO + 0x00100000 - sizeof(OpoVersionString)); // буфер памяти у границы 1Мб RAM СПО
memmove(Dst, Src, sizeof(OpoVersionString));
((char*) Dst)[63] = 0;

// расчет контрольных сумм всех секторов флеша
Dst = (DWORD*) (RAM_SPO + 0x00100000);
Sum = CalcCrc32(Dst, sizeof(DWORD) * 16);
if (Sum != Dst[16])
  {
  for (Sector = 0; Sector < 8; Sector++)
    {
    Src = (DWORD*) (0xBFC00000 + (Sector * 256 * 1024));
    Dst[Sector] = CalcCrc32(Src, 256 * 1024);
    for (Index = 0, Sum = 0; Index < ((256 * 1024) / 4); Index++) Sum = CSumLong(Sum, *Src++);
    Dst[Sector + 8] = Sum;
    }
  Dst[16] = CalcCrc32(Dst, sizeof(DWORD) * 16);
  }

return 0;
}
//================================================================================

