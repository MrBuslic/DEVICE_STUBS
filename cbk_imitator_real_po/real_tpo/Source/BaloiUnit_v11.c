
/*! \file BaloiUnit.c
 * \author Гапонов Р.В.
 * \date 2018.12.21
 * \version 11.5.0
 * \brief Модуль содержит функции обработки прерывания 13 от Балои
 * Алгоритм работы с БАЛОИ версии 11.5
 */

#include "BaloiUnit_v11.h"
//================================================================================

#ifdef BALOI_V11
//================================================================================

// [номера слов] (option: [номера битов])

// данные на подадресе 1:
// [0][7..0] - код команды, = 0x62
// [0][15..8] - код операции, = 1..N

// код операции = 0x01, отключение обработки прерывания 13
// возврат, результат: OK
// возврат, данные: нет
// пример:
// ВЫДАТЬ_ДИР([0x0162], []);

// код операции = 0x02, включение обработки прерывания 13
// возврат, результат: OK
// возврат, данные: нет
// пример:
// ВЫДАТЬ_ДИР([0x0262], []);

// код операции = 0x03, установка кода СЕВ
// [1][15..0] - 1 слово кода СЕВ
// [2][15..0] - 2 слово кода СЕВ
// [3][15..0] - 3 слово кода СЕВ
// возврат, результат: OK
// возврат, данные: нет
// пример:
// ВЫДАТЬ_ДИР([0x0362, 0x1234, 0x1234, 0x1234], []);

// код операции = 0x04, чтение кода последнего сообщения
// возврат, результат: OK
// возврат, данные: 1 слово = код последнего сообщения от БАЛОИ
// пример:
// ВЫДАТЬ_ДИР([0x0462], []);
// if (БУФЕР_МКО[0] == 0x1234) ...

// код операции = 0x05, передача сообщения в БАЛОИ со словами данных
// [1][15..0] - код сообщения
// [2][15..0] - число слов данных для передачи (от 0 до 32 слов)
// возврат, результат: OK/ошибка
// возврат, данные: нет
// пример:
// ВЫДАТЬ_ДИР([0x0562, 0x1234, 0x0004], [0x0001, 0x0002, 0x0003, 0x0004]);

// код операции = 0x06, вкл/откл обработки сообщения от БАЛОИ (по умолчанию - обрабатываются все)
// [1][15..0] - код сообщения
// [2][15..0] - флаг режима (1 - вкл обработку, 0 - откл обработку)
// возврат, результат: OK/ошибка
// возврат, данные: нет
// пример:
// ВЫДАТЬ_ДИР([0x0662, 0x1234, 1], []); // сообщение 0x1234 будет обработано
// ВЫДАТЬ_ДИР([0x0662, 0x4321, 0], []); // сообщение 0x4321 будет игнорироваться

// код операции = 0x07, загрузка переменных и таблиц СПО для БАЛОИ
// [1][15..0] - номер переменной или таблицы
// [2][15..0] - индекс блока (для таблиц)
// [3][15..0] - размер блока, байт
// возврат, результат: OK/ошибка
// возврат, данные: нет
// номера переменных:
// номер "Komplekt_ALPS"           =  1, индексы блока = 0..0,  размер данных = 1 слово
// номер "Komplekt_MFS"            =  2, индексы блока = 0..0,  размер данных = 1 слово
// номер "BUFAR"                   =  3, индексы блока = 0..0,  размер данных = 1 слово
// номер "BUFAR_Sbros"             =  4, индексы блока = 0..0,  размер данных = 1 слово
// номер "Rejim_3_Rasch"           =  5, индексы блока = 0..0,  размер данных = 1 слово
// номер "Nomer_Shetverti"         =  6, индексы блока = 0..0,  размер данных = 1 слово
// номер "Otkl_AFAR"               =  7, индексы блока = 0..0,  размер данных = 1 слово
// номер "Pause_MFS"               =  8, индексы блока = 0..0,  размер данных = 1 слово
// номер "Pause_BUM"               =  9, индексы блока = 0..0,  размер данных = 3 слова
// номер "Pause_PRD"               = 10, индексы блока = 0..0,  размер данных = 1 слово
// номер "Pause_Rasch_3"           = 11, индексы блока = 0..0,  размер данных = 1 слово
// номер "Pause_LPH"               = 12, индексы блока = 0..0,  размер данных = 1 слово
// номер "Vkl_AFAR_BUM[5][2]"      = 13, индексы блока = 0..0,  размер данных = 10 слов
// номер "Tm_AFAR_BUM[5][2]"       = 14, индексы блока = 0..0,  размер данных = 10 слов
// номер "Vkl_AFAR_PRD[5][4][28]"  = 15, индексы блока = 0..19, размер данных = 28 слов
// номер "Tm_AFAR_PRD[5][15]"      = 16, индексы блока = 0..4,  размер данных = 15 слов
// номер "Control_Tm_AFAR"         = 17, индексы блока = 0..0,  размер данных = 1 слово
// номер "Chislo_Proverok_TM_AFAR" = 18, индексы блока = 0..0,  размер данных = 1 слово
// номер "Pause_Proverki_TM_AFAR"  = 19, индексы блока = 0..0,  размер данных = 1 слово
// номер "Pause_OTKL_AFAR"         = 20, индексы блока = 0..0,  размер данных = 1 слово
// номер "Data_OTKL_AFAR"          = 21, индексы блока = 0..0,  размер данных = 8 слов
// номер "Control_BUM"             = 22, индексы блока = 0..0,  размер данных = 1 слово
// номер "LPH"                     = 23, индексы блока = 0..0,  размер данных = 1 слово
// номер "Rejim_BRTK"              = 24, индексы блока = 0..0,  размер данных = 1 слово
// номер "Irq13SendNackEnabled"    = 25, индексы блока = 0..0,  размер данных = 1 слово
// номер "Irq13CmdFifoEnabled"     = 26, индексы блока = 0..0,  размер данных = 1 слово
// номер "Pause_MkoTx"             = 27, индексы блока = 0..0,  размер данных = 1 слово
// номер "Pause_AOS"               = 31, индексы блока = 0..0,  размер данных = 2 слова
// номер "Otkl_AT"                 = 32, индексы блока = 0..0,  размер данных = 1 слово
// номер "Code_DB"                 = 33, индексы блока = 0..0,  размер данных = 1 слово
// номер "KPI_Etalon"              = 34, индексы блока = 0..25, размер данных = 32 слова
// номер "KPI_Etalon_Len"          = 35, индексы блока = 0..0,  размер данных = 1 слово
// номер "KPI_Kvit"                = 36, индексы блока = 0..0,  размер данных = 2 слова
// номер "Irq13UserMsgData"        = 38, индексы блока = 0..31, размер данных = 32 слова
// номер "Pause_AFAR"              = 39, индексы блока = 0..0,  размер данных = 1 слово
// номер "Index_AFAR"              = 40, индексы блока = 0..0,  размер данных = 1 слово
// номер "Otkl_BPA_AT"             = 41, индексы блока = 0..0,  размер данных = 1 слово
// номер "AFAR_AT_M"               = 42, индексы блока = 0..0,  размер данных = 1 слово
// номер "Control_ZTM_BUFAR"       = 43, индексы блока = 0..0,  размер данных = 1 слово
// номер "Otriz_Kvit"              = 44, индексы блока = 0..0,  размер данных = 1 слово

// пример:
// ВЫДАТЬ_ДИР([0x0762, 1, 0, 2], [0x0003]); // переустановить Komplekt_ALPS = 3
// ВЫДАТЬ_ДИР([0x0762, 6, 0, 8], [1,2,3,4]); // загрузить таблицу VKL_AFAR_BUM[]
// ВЫДАТЬ_ДИР([0x0762, 8, 7, 56], [1, ..., 28]); // загрузить старшую страницу таблицы VKL_AFAR_PRD[]

// код операции = 0x08, установка кода сообщения от БАЛОИ для информирования КПА (по умолчанию отключено)
// [1][15..0] - код сообщения (= 0xFFFF - отключение выдачи сообщений)
// возврат, результат: OK
// возврат, данные: нет
// пример:
// ВЫДАТЬ_ДИР([0x0862, 0x1234], []);
// ЖДАТЬ_СООБЩЕНИЕ_ЦБК(0x0070, "", 4, 30000);

// код операции = 0x09, ручная имитация получения сообщения "Ввести ДН" (код 0xDB3A)
// возврат, результат: OK/ошибка
// возврат, данные: нет
// пример:
// ВЫДАТЬ_ДИР([0x0962], []);

// код операции = 0x0A, -

// код операции = 0x0B, -

// код операции = 0x0C, включение/отключение обработки набора сообщений для режима 2/3 БКРК
// [1][15..0] - код сообщения
// [2][15..0] - флаг режима (1 - вкл, 0 - откл)
// возврат, результат: OK/ошибка
// возврат, данные: нет
// пример:
// ВЫДАТЬ_ДИР([0x0С62, 0x0001], []); // включить режим 2/3
// ВЫДАТЬ_ДИР([0x0С62, 0x0000], []); // отключить режим 2/3

// код операции = 0x0D, -

// код операции = 0x0E, -

// код операции = 0x0F, вкл/откл циклического считывания ОК БАЛОИ
// [1][15..0] - флаг вкл/откл, 0..1 (0 - откл/по умолчанию)
// возврат, результат: OK
// возврат, данные: нет
// пример:
// ВЫДАТЬ_ДИР([0x0F62, 1], []);

// код операции = 0x10, вкл/откл циклического контроля исправности БАЛОИ
// [1][15..0] - флаг вкл/откл, 0..1 (0 - откл/по умолчанию)
// возврат, результат: OK
// возврат, данные: нет
// пример:
// ВЫДАТЬ_ДИР([0x1062, 1], []);

// код операции = 0x11, вкл/откл циклического контроля исправности ЦБК
// [1][15..0] - флаг вкл/откл, 0..1 (0 - откл/по умолчанию)
// возврат, результат: OK
// возврат, данные: нет
// пример:
// ВЫДАТЬ_ДИР([0x1162, 1], []);

// код операции = 0x12, -

// код операции = 0x13, чтение последнего РНС от БАЛОИ
// возврат, результат: OK
// возврат, данные: 1 слово = последний РНС от БАЛОИ
// пример:
// Result = ВЫДАТЬ_ДИР([0x1362, 1], []);
// if (!Result) RxRNS = БУФЕР_МКО[0];

// код операции = 0x14, вкл/откл передачи квитанции на сообщения БАЛОИ (по умолчанию - передается для всех)
// [1][15..0] - код сообщения
// [2][15..0] - флаг режима (1 - вкл передачу, 0 - откл передачу)
// возврат, результат: OK/ошибка
// возврат, данные: нет
// пример:
// ВЫДАТЬ_ДИР([0x1462, 0x1234, 1], []); // в ответ на сообщение 0x1234 будет передача квитанция
// ВЫДАТЬ_ДИР([0x1462, 0x4321, 0], []); // в ответ на сообщение 0x4321 не будет передача квитанция

// код операции = 0x15, вкл/откл расчета CRC для передаваемых в БАЛОИ сообщений (по умолчанию - правильный CRC для всех)
// [1][15..0] - код сообщения
// [2][15..0] - флаг режима (1 - вкл расчет, 0 - откл расчет)
// возврат, результат: OK/ошибка
// возврат, данные: нет
// пример:
// ВЫДАТЬ_ДИР([0x1562, 0x1234, 1], []); // для сообщения 0x1234 будет передан правильный код CRC
// ВЫДАТЬ_ДИР([0x1562, 0x4321, 0], []); // для сообщения 0x4321 будет передан ошибочный код CRC

// код операции = 0x16, установка типа передаваемой квитанции для указанных сообщений от БАЛОИ (по умолчанию - передается реальная квитанция)
// [1][15..0] - код сообщения
// [2][15..0] - флаг режима (1 - передавать указанную квитанцию, 0 - передавать реальную квитанцию)
// [3][15..0] - тип квитанции (1 - квитанция о успешном завершении, 0 - квитанция о ошибке)
// возврат, результат: OK/ошибка
// возврат, данные: нет
// пример:
// ВЫДАТЬ_ДИР([0x1662, 0x1234, 1, 1], []); // для сообщения 0x1234 будет передана квитанция "успешно"
// ВЫДАТЬ_ДИР([0x1662, 0x4321, 1, 0], []); // для сообщения 0x4321 будет передана квитанция "ошибка"
// ВЫДАТЬ_ДИР([0x1662, 0x2468, 0, 0], []); // для сообщения 0x2468 будет передана реальная квитанция

// код операции = 0x17, чтение текущего РНС передаваемого в БАЛОИ
// возврат, результат: OK
// возврат, данные: 1 слово с текущим номером РНС
// пример:
// Result = ВЫДАТЬ_ДИР([0x1762], []);
// if (!Result) TxRNS = БУФЕР_МКО[0];

// код операции = 0x18, переназначение номера РНС передаваемого в БАЛОИ
// [1][15..0] - режим (1 - установить РНС, 0 - вернуть РНС)
// [2][15..0] - номер РНС (при переустановке)
// возврат, результат: OK/ошибка
// возврат, данные: нет
// пример:
// ВЫДАТЬ_ДИР([0x1862, 1, 2468], []); // установить новое значение РНС = 2468
// ВЫДАТЬ_ДИР([0x1862, 0, 0], []); // вернуть старое значение РНС

// код операции = 0x19, установка сообщений, после обработки которых считываются ОК БАЛОИ
// [1][15..0] - код сообщения
// [2][15..0] - флаг режима (1 - добавить сообщение в список, 0 - убрать сообщение из списка)
// возврат, результат: OK/ошибка
// возврат, данные: нет
// пример:
// ВЫДАТЬ_ДИР([0x1962, 0x1234, 1], []); // после сообщения 0x1234 выполнять считывание ОК БАЛОИ
// ВЫДАТЬ_ДИР([0x1962, 0x1234, 0], []); // после сообщения 0x1234 ничего не делать

// код операции = 0x1A, чтение/сброс статуса БАЛОИ (статус после чтения 13/1/31)
// [1][15..0] - флаг режима (1 - сбросить флаг статуса сбоя, 0 - считать флаг статуса сбоя)
// возврат, результат: OK
// возврат, данные: 1 слово при считывании флага
// пример:
// ВЫДАТЬ_ДИР([0x1A62, 1], []); // очистить флаг сбоя БАЛОИ
// ВЫДАТЬ_ДИР([0x1A62, 0], []); // считать в МКО_БУФЕР[0] флаг сбоя (1 - сбой был, 0 - сбоя не было)

// код операции = 0x1B, сброс переменных БАЛОИ к исходным
// возврат, результат: OK
// ВЫДАТЬ_ДИР([0x1B62], []);

// код операции = 0x1C, сброс пакетной передачи
// [1][15..0] = код операции
//   0x0000 = очистить все пакеты
//     если пакеты запущены, они останавливаются
//     удаляются все сообщения
//     удаляется вся статистика
//   0x0001 = очистить указанный пакет
//     если пакет запущен, он останавливается
//     удаляются все сообщения из пакета
//     удаляется вся статистика по пакету
//   0x0002 = только сброс статистики пакета
//     состояние пакета при сбросе не важно
// [2][7..0] = номер пакета, 0..31 (если требуется)

// код операции = 0x1D, настройка данных пакетной передачи
// [1][15..0] = код операции
//   0x0000 = добавить сообщение в пакет
//     если пакет запущен = ошибка
//   0x0001 = добавить начальную паузу для пакета
//     если пакет запущен = ошибка
//   0x0002 = добавить интервал для пакета
//     если пакет запущен = ошибка
//   0x0003 = добавить линию/канал МКО для пакета
//     если пакет запущен = ошибка
// если добавляется сообщение в пакет (код опер. 0x0000):
//   [2][7..0]  = номер пакета, 0..31
//   [2][15..8] = флаг режима (0 – запись, 1 – чтение)
//   [3][7..0]  = адрес, от 0 до 30
//   [3][15..8] = подадрес, от 1 до 30
//   [4][7..0]  = число слов данных, 1..32
//   если режим сообщения = передача, считываются данные с п/а 2 для передачи
// если добавляется начальная пауза для пакета (код опер. 0x0001):
//   [2][7..0]  = номер пакета, 0..31
//   [3][15..0] = начальная однократная пауза для пакета, мсек, 10..65535
// если добавляется интервал для пакета (код опер. 0x0002):
//   [2][7..0]  = номер пакета, 0..31
//   [3][15..0] = интервал для пакета, мсек, 500..60000
// если добавляется линия/канал МКО для пакета (код опер. 0x0003):
//   [2][7..0]  = номер пакета, 0..31
//   [3][7..0]  = линия МКО для пакета, 0..1
//   [3][15..8] = канал МКО для пакета, 0..1

// код операции = 0x1E, запуск/останов пакетной передачи
// [1][15..0] = код операции
//   0x0000 = запуск всех непустых пакетов
//     если пакет уже запущен, ничего не делается
//     если нет пакетов для запуска, ничего не делается
//   0x0001 = запуск указанного пакета
//     если пакет пуст = ошибка
//     если пакет уже запущен, ничего не делается
//   0x0002 = стоп всех пакетов
//     если нет запущеных пакетов, ничего не делается
//   0x0003 = стоп указанного пакета
//     если пакет не запущен, ничего не делается
// [2][7..0] = номер пакета, 0..31 (если требуется)

// код операции = 0x1F, чтение данных пакетной передачи
// [1][15..0] = код операции
//   0x0000 = чтение статистики для пакета
// [2][7..0] = номер пакета, 0..31 (если требуется)

//================================================================================

int Irq13TxMsgLen, Irq13PrevTxMsgLen;
WORD Irq13TxBuffer[32 * 32], Irq13PrevTxBuffer[32 * 32];
WORD Irq13RxBuffer[32 * 32];
WORD Irq13UserMsgData[32 * 32];         // 0..FFFF, def 0                                               // +

volatile int Irq13CmdFifoEnabled, Irq13CmdFifoHead, Irq13CmdFifoTail;
TIrq13CmdBuffer Irq13CmdFifo[IRQ13_CMD_FIFO_SIZE];

WORD Irq13TimeSEV[3];                   // 0..FFFF, def 0                                               // +
WORD Irq13DevState[30];                 // 0..FFFF, def 0                                               // +
WORD Irq13RxIndex, Irq13TxIndex, Irq13PrevTxIndex;
WORD Irq13LastMsg, Irq13LastSum, Irq13LastRns;
int Irq13SingleEvent;
// extern UINT Irq13Semaphore;
int Irq13BaloiFail;
int Irq13BrtkMode2Enabled;
int Irq13SendNackEnabled;

volatile int Irq13WaitReplyFlag;
sysTime_t Irq13WaitReplyTimer;

int Rejim_BRTK;                         // 1..3, def 1                                                  // +
int Komplekt_AOS;                       // 0..FFFF, def 0                                               // +
int Komplekt_APD_MT;                    // 0..FFFF, def 0                                               // +
int DN_Vvedena;                         // 0..3, def 0                                                  // +
int LPH_Vkluchena;                      // 0..1, def 0                                                  // +
int Control_ZTM_BUFAR;                  // 0..1, def 0                                                  // +
int Otriz_Kvit;                         // 0..1, def 1

int Ctrl_Ispravnosti_Enabled;           // флаг циклического контроля исправности БАЛОИ (раз в 12 сек, def = 0)
sysTime_t Ctrl_Ispravnosti_Time;
int Ctrl_Ispravnosti;                   // 0..3, def 0                                                  // +

int Code_DB;                            // 0..FFFF, def 0                                               // +
int AFAR_AT_M;                          // 0..1, def 1                                                  // +

int Komplekt_ALPS;                      // 1..3, def 1                                                  // +
int Komplekt_MFS;                       // 1..3, def 1                                                  // +
int BUFAR;                              // 0..3, def 1                                                  // +
int BUFAR_Sbros;                        // 0..1, def 1                                                          // ???

int Rejim_3_Rasch;                      // 0..1, def 1                                                          // ???
int Nomer_Shetverti;                    // 1..4, def 1                                                  // +
int Otkl_AFAR;                          // 0..1, def 1                                                          // ???

int Pause_MFS[2];                       // 0..10000, def 3000,1500                                      // +
WORD Pause_BUM[3];                      // 0..10000, def 400,2500,300                                   // +
int Pause_PRD;                          // 0..10000, def 3000                                           // +
int Pause_Rasch_3;                      // 0..10000, def 100                                                    // ???
int Pause_LPH;                          // 0..10000, def 100                                            // +
int Pause_OTKL_AFAR;                    // 0..3000, def 300                                                     // ???
WORD Data_OTKL_AFAR[8];                 // 0..FFFF, def 000E,FFFE,0000,1FFE,0000,001E,0000,0000         // +

int Control_Tm_AFAR;                    // 0..1, def 1                                                          // ???
int Chislo_Proverok_TM_AFAR;            // 1..1000, def 1                                                       // ???
int Pause_Proverki_TM_AFAR;             // 0..10000, def 0                                                      // ???
int Control_BUM;                        // 0..1, def 1                                                  // +
int Pause_MkoTx;                        // 0..1000, def 50                                                      // ???
int Pause_AFAR;                         // 0..10000, def 100                                            // +

int Index_AFAR;                        	// 0..4, def 0                                                  // +
int SAA_Ustanovlena;                    // 0..3, def 0                                                  // +
int LPH;                                // 0..1, def 0                                                          // ???

int Pause_AOS[2];                       // 0..300, def 10,60                                            // +
int Otkl_AT;                            // 0..1, def 1                                                          // ???
int Otkl_BPA_AT;                        // 0..1, def 1                                                  // +
WORD KPI_Etalon[26 * 32];               // 0..FFFF, def 0, реально используется 810 слов                // +
int KPI_Etalon_Len;                     // 0...810, def 0, число слов в KPI_Etalon                      // +
WORD KPI_Kvit[2];                       // 0...FFFF, def 0, [0] - положит, [1] - отриц                  // +

WORD Vkl_AFAR_BUM[5][2];                // 0..FFFF, def 0                                               // +
WORD Tm_AFAR_BUM[5][2];                 // 0..FFFF, def 0                                               // +
WORD Vkl_AFAR_PRD[5][4][28];            // 0..FFFF, def 0                                               // +
WORD Tm_AFAR_PRD[5][15];                // 0..FFFF, def 0                                               // +

WORD Vkl_PRD_0[4][28];                  // tmp
//================================================================================

// периодическая пакетная выдача сообщений
UINT Baloi_MkoPackSem;                          // семафор для доступа к данным пакетов

mkoPack_t Baloi_MkoPack;                        // переменные для обмена по МКО
mkoPackRes_t Baloi_MkoPackRes;
mkoPackMsg_t Baloi_MkoPackMsg[32];
mkoMsgRes_t Baloi_MkoPackMsgRes[32];
mkoPackMsg_t* Baloi_MkoPackMsgList[32];

u8 Baloi_PackMsgCount[32];                      // число сообщений в пакете, 0..32
u16 Baloi_PackPrepPause[32];                    // однократная пауза при начальном запуске пакета, 0..FFFF
u16 Baloi_PackInterval[32];                     // интервал передачи пакета, мсек, 500..60000
u8 Baloi_PackMkoLine[32];                       // линия МКО для передачи пакета, 0..1
u8 Baloi_PackMkoChan[32];                       // канал МКО для передачи пакета, 0..1
u16 Baloi_PackList[32][32][33];                 // данные пакетов, список = 32 пакета, пакет = 32 сообщения, сообщение = CMD+32 слова данных
u32 Baloi_PackSendCount[32];                    // число передач пакета, 0..X
u32 Baloi_PackOkSendCount[32];                  // число успешных передач пакета, 0..X
u32 Baloi_PackErrSendCount[32];                 // число неудачных передач пакета, 0..X
//================================================================================

int Baloi_Read_OK;	                                        // флаг циклического считывания ИОК БАЛОИ (раз в 4 сек, def = 0)
sysTime_t Baloi_Read_OK_Time;

int Baloi_Send_Bau_OK;                                          // флаг контроля исправности ЦБК (раз в 1 мин, def = 0)
sysTime_t Baloi_Send_Bau_OK_Time;

WORD Baloi_DisabledMsgList[BALOI_MAX_DISABLED_MSG_COUNT];       // список сообщений от БАЛОИ, игнорируемых СПО
int Baloi_DisabledMsgListCount;

WORD Baloi_DisabledReplyList[BALOI_MAX_DISABLED_MSG_COUNT];     // список сообщений от БАЛОИ, в ответ на которые не передаются квитанции
int Baloi_DisabledReplyListCount;

WORD Baloi_DisabledCrcList[BALOI_MAX_DISABLED_MSG_COUNT];       // список сообщений в БАЛОИ, для которых не расчитывается CRC
int Baloi_DisabledCrcListCount;

WORD Baloi_ReplyTypeList[BALOI_MAX_DISABLED_MSG_COUNT][2];      // список сообщений от БАЛОИ, в ответ на которые передаются указанные квитанции
int Baloi_ReplyTypeListCount;

WORD Baloi_DebugReadList[BALOI_MAX_DISABLED_MSG_COUNT];         // список сообщений от БАЛОИ, после обработки которых считываются ОК БАЛОИ
int Baloi_DebugReadListCount;

int RedefinedTxRns;                     // флаг переназначения номера РНС передачи
WORD RedefinedTxRnsValue;
//================================================================================

// настройка среды
void Irq13InitSys (void)
{
// создаем семафор для доступа к данным пакетов
Baloi_MkoPackSem = semCreate();

// очистка данных для пакетной выдачи
memset(&Baloi_PackMsgCount, 0, sizeof(Baloi_PackMsgCount));
memset(&Baloi_PackPrepPause, 0, sizeof(Baloi_PackPrepPause));
memset(&Baloi_PackInterval, 0, sizeof(Baloi_PackInterval));
memset(&Baloi_PackMkoLine, 0, sizeof(Baloi_PackMkoLine));
memset(&Baloi_PackMkoChan, 0, sizeof(Baloi_PackMkoChan));
memset(&Baloi_PackList, 0, sizeof(Baloi_PackList));
memset(&Baloi_PackSendCount, 0, sizeof(Baloi_PackSendCount));
memset(&Baloi_PackOkSendCount, 0, sizeof(Baloi_PackOkSendCount));
memset(&Baloi_PackErrSendCount, 0, sizeof(Baloi_PackErrSendCount));
}
//================================================================================

// сброс переменных в состояние по умолчанию
void Irq13InitData (void)
{
Irq13CmdFifoEnabled = 1;
Irq13CmdFifoHead = 0;
Irq13CmdFifoTail = 0;

Irq13BrtkMode2Enabled = 1;

memset(&Irq13TimeSEV, 0, sizeof(Irq13TimeSEV));
memset(&Irq13DevState, 0, sizeof(Irq13DevState));
memset(&Irq13UserMsgData, 0, sizeof(Irq13UserMsgData));

Irq13RxIndex = 1;
Irq13TxIndex = 1;
Irq13LastMsg = Irq13LastSum = 0;
Irq13SingleEvent = -1;

Irq13LastRns = 0;
Irq13SendNackEnabled = 1;
Irq13WaitReplyFlag = 0;

// ********

Rejim_BRTK = 1;
Komplekt_AOS = 0;
Komplekt_APD_MT = 0;
DN_Vvedena = 0;
LPH_Vkluchena = 0;
Control_ZTM_BUFAR = 1;
Otriz_Kvit = 1;

Ctrl_Ispravnosti_Enabled = 0;
Ctrl_Ispravnosti = 0;

Code_DB = 0;
AFAR_AT_M = 1;

Komplekt_ALPS = 1;
Komplekt_MFS = 1;
BUFAR = 1;
BUFAR_Sbros = 1;

Rejim_3_Rasch = 1;
Nomer_Shetverti = 1;
Otkl_AFAR = 1;

Pause_MFS[0] = 3000; Pause_MFS[1] = 1500;
Pause_BUM[0] = 400; Pause_BUM[1] = 2500; Pause_BUM[2] = 300;
Pause_PRD = 3000;
Pause_Rasch_3 = 100;
Pause_LPH = 100;
Pause_OTKL_AFAR = 300;

Data_OTKL_AFAR[0] = 0x000E;
Data_OTKL_AFAR[1] = 0xFFFE;
Data_OTKL_AFAR[2] = 0x0000;
Data_OTKL_AFAR[3] = 0x1FFE;
Data_OTKL_AFAR[4] = 0x0000;
Data_OTKL_AFAR[5] = 0x001E;
Data_OTKL_AFAR[6] = 0x0000;
Data_OTKL_AFAR[7] = 0x0000;

Control_Tm_AFAR = 1;
Chislo_Proverok_TM_AFAR = 1;
Pause_Proverki_TM_AFAR = 0;
Control_BUM = 1;
Pause_MkoTx = 50;
Pause_AFAR = 100;

Index_AFAR = 0;
SAA_Ustanovlena = 0;
LPH = 0;

Pause_AOS[0] = 10; Pause_AOS[1] = 60;
Otkl_AT = 1;
Otkl_BPA_AT = 1;
memset(&KPI_Etalon, 0, sizeof(KPI_Etalon));
KPI_Etalon_Len = 0;
KPI_Kvit[0] = 0; KPI_Kvit[1] = 0;

memset(&Vkl_AFAR_BUM, 0, sizeof(Vkl_AFAR_BUM));
memset(&Tm_AFAR_BUM, 0, sizeof(Tm_AFAR_BUM));
memset(&Vkl_AFAR_PRD, 0, sizeof(Vkl_AFAR_PRD));
memset(&Tm_AFAR_PRD, 0, sizeof(Tm_AFAR_PRD));

// ********

Baloi_Read_OK = 0;

Baloi_DisabledMsgListCount = 0;
Baloi_DisabledReplyListCount = 0;
Baloi_DisabledCrcListCount = 0;
Baloi_ReplyTypeListCount = 0;
Baloi_DebugReadListCount = 0;

RedefinedTxRns = 0;
Irq13BaloiFail = 0;
}
//================================================================================

void KpaCommand62 (int BaseMkoLine, int BaseMkoChan)
{
int Result, Msg, Opcode;

// читаем код операции
Opcode = KpaCommand[0] >> 8;

// отключение обработки прерывания 13
if (Opcode == 0x01)
  {
  // отключение прерываний от устройства
  SpoConfig.EnabledIrqMask &= ~(1 << 13);
  sprIMaskVec(~SpoConfig.EnabledIrqMask);

  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ);
  }
// включение обработки прерывания 13
else if (Opcode == 0x02)
  {
  // сброс переменных (кроме кода СЕВ)
  memset(&Irq13RxBuffer, 0, sizeof(Irq13RxBuffer));
  memset(&Irq13TxBuffer, 0, sizeof(Irq13TxBuffer));
  memset(&Irq13DevState, 0, sizeof(Irq13DevState));

  Irq13RxIndex = 1;
  Irq13TxIndex = 1;
  Irq13LastMsg = Irq13LastSum = 0;

  // включение прерываний от устройства
  SpoConfig.EnabledIrqMask |= (1 << 13);
  sprIMaskVec(~SpoConfig.EnabledIrqMask);

  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ);
  }
// установка кода СЕВ
else if (Opcode == 0x03)
  {
  Irq13TimeSEV[0] = KpaCommand[1];
  Irq13TimeSEV[1] = KpaCommand[2];
  Irq13TimeSEV[2] = KpaCommand[3];

  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ);
  }
// чтение кода последнего сообщения
else if (Opcode == 0x04)
  {
  ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ, &Irq13LastMsg, sizeof(Irq13LastMsg));
  }
// передача сообщения в БАЛОИ со словами данных
else if (Opcode == 0x05)
  {
  Result = Irq13SendUserMsg(KpaCommand[1], KpaCommand[2]);
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, Result, KPA_SUBADDR_IRQ);
  }
// вкл/откл обработки сообщения от БАЛОИ
else if (Opcode == 0x06)
  {
  Result = Irq13ChangeDisabledMsgList(KpaCommand[1], KpaCommand[2]);
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, Result, KPA_SUBADDR_IRQ);
  }
// загрузка переменных и таблиц СПО для БАЛОИ
else if (Opcode == 0x07)
  {
  Result = Irq13LoadTableData(BaseMkoLine, BaseMkoChan, KpaCommand[1], KpaCommand[2], KpaCommand[3]);
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, Result, KPA_SUBADDR_IRQ);
  }
// установка кода сообщения от БАЛОИ для информирования КПА
else if (Opcode == 0x08)
  {
  Irq13SingleEvent = (KpaCommand[1] != 0xFFFF) ? (KpaCommand[1]) : (-1);
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ);
  }
// ручная имитация получения сообщения "Ввести ДН" (код 0xDB3A)
else if (Opcode == 0x09)
  {
  Result = Oper_0962(); // MsgProc0xDB3AExt();
  Msg = (!Result) ? (REPLY_CMD_DONE) : (REPLY_CMD_ERROR);
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, Msg, KPA_SUBADDR_IRQ);
  }
// включение/отключение обработки набора сообщений для режима 2/3 БКРК
else if (Opcode == 0x0C)
  {
  Irq13BrtkMode2Enabled = (KpaCommand[1]) ? (1) : (0);
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ);
  }
// вкл/откл циклического считывания ОК БАЛОИ
else if (Opcode == 0x0F)
  {
  Baloi_Read_OK = KpaCommand[1];
  if (Baloi_Read_OK) timeRead(&Baloi_Read_OK_Time);
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ);
  }
// вкл/откл циклического контроля исправности БАЛОИ
else if (Opcode == 0x10)
  {
  Ctrl_Ispravnosti_Enabled = KpaCommand[1];
  if (Ctrl_Ispravnosti_Enabled) timeRead(&Ctrl_Ispravnosti_Time);
  Ctrl_Ispravnosti = 0;
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ);
  }
// вкл/откл циклического контроля исправности ЦБК
else if (Opcode == 0x11)
  {
  Baloi_Send_Bau_OK = KpaCommand[1];
  if (Baloi_Send_Bau_OK) timeRead(&Baloi_Send_Bau_OK_Time);
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ);
  }
// чтение последнего РНС от БАЛОИ
else if (Opcode == 0x13)
  {
  ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ, &Irq13LastRns, sizeof(Irq13LastRns));
  }
// вкл/откл передачи квитанции на сообщения БАЛОИ
else if (Opcode == 0x14)
  {
  Result = Irq13ChangeDisabledReplyList(KpaCommand[1], KpaCommand[2]);
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, Result, KPA_SUBADDR_IRQ);
  }
// вкл/откл расчета CRC для передаваемых в БАЛОИ сообщений
else if (Opcode == 0x15)
  {
  Result = Irq13ChangeDisabledCrcList(KpaCommand[1], KpaCommand[2]);
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, Result, KPA_SUBADDR_IRQ);
  }
// установка типа передаваемой квитанции для указанных сообщений от БАЛОИ
else if (Opcode == 0x16)
  {
  Result = Irq13ChangeReplyTypeList(KpaCommand[1], KpaCommand[2], KpaCommand[3]);
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, Result, KPA_SUBADDR_IRQ);
  }
// чтение текущего РНС передаваемого в БАЛОИ
else if (Opcode == 0x17)
  {
  ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ, &Irq13TxIndex, sizeof(Irq13TxIndex));
  }
// переназначение номера РНС передаваемого в БАЛОИ
else if (Opcode == 0x18)
  {
  Result = Irq13ChangeCurrentTxRns(KpaCommand[1], KpaCommand[2]);
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, Result, KPA_SUBADDR_IRQ);
  }
// установка сообщений, после обработки которых считываются ОК БАЛОИ
else if (Opcode == 0x19)
  {
  Result = Irq13ChangeDebugReadList(KpaCommand[1], KpaCommand[2]);
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, Result, KPA_SUBADDR_IRQ);
  }
// чтение/сброс статуса БАЛОИ (по чтению 13/1/31)
else if (Opcode == 0x1A)
  {
  if (KpaCommand[1]) { Irq13BaloiFail = 0; ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ); }
  else ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ, &Irq13BaloiFail, sizeof(Irq13BaloiFail));
  }
// сброс переменных БАЛОИ к исходным
else if (Opcode == 0x1B)
  {
  Irq13InitData();
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_CMD_DONE, KPA_SUBADDR_IRQ);
  }
// сброс данных пакетной передачи
else if (Opcode == 0x1C)
  {
  Result = Irq13PackReset(BaseMkoLine, BaseMkoChan);
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, Result, KPA_SUBADDR_IRQ);
  }
// настройка данных пакетной передачи
else if (Opcode == 0x1D)
  {
  Result = Irq13PackSetup(BaseMkoLine, BaseMkoChan);
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, Result, KPA_SUBADDR_IRQ);
  }
// запуск/останов пакетной передачи
else if (Opcode == 0x1E)
  {
  Result = Irq13PackRun(BaseMkoLine, BaseMkoChan);
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, Result, KPA_SUBADDR_IRQ);
  }
// чтение данных пакетной передачи
else if (Opcode == 0x1F)
  {
  Result = Irq13PackReadInfo(BaseMkoLine, BaseMkoChan);
  // ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, Result, KPA_SUBADDR_IRQ);
  }
// неизвестная операция
else
  {
  Result = REPLY_CMD_DATA_ERROR;
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, Result, KPA_SUBADDR_IRQ);
  }
}
//================================================================================

void Irq13Handler (void)
{
int Result, Size, Size29, Size02, WantSize, Disabled, Redefined;
int Result02, Result28, Result29;
WORD OrigSum, RealSum, Reply02, Reply29, ReplyType;
char MkoLine, MkoChan;

// параметры линий обмена
MkoLine = SpoConfig.DevMkoLine & 1;
MkoChan = SpoConfig.DevMkoChan & 1;

// ****************************************

do
  {
  // читаем абонента 13/29/1
  Result29 = Irq13MkoXchgBn(MkoLine, MkoChan, 13, 29, MKO_RX, 1, 31, &Size29, &Reply29);

  // читаем основное сообщение 13/2/31
  Result02 = Irq13MkoXchgBn(MkoLine, MkoChan, 13, 2, MKO_RX, 31, 31, &Size02, &Irq13RxBuffer);

  // если нет ОС или в ОС есть ошибки (кроме занятости)
  if ((Result29 == -1) || (Result29 == -2) || (Result02 == -1) || (Result02 == -2))
    {
    if (Irq13WaitReplyFlag)
      {
      // повтор предыдущего сообщения
      Irq13TxIndex = Irq13PrevTxIndex;
      Irq13TxMsgLen = Irq13PrevTxMsgLen;
      memmove(&Irq13TxBuffer, &Irq13PrevTxBuffer, sizeof(Irq13TxBuffer));
      Irq13SendMsgBn();
      break;
      }
    if (Otriz_Kvit)
      {
      // передаем отриц квитанцию
      Reply02 = 0x00FF;
      Result = Irq13MkoXchgBn(MkoLine, MkoChan, 13, 29, MKO_TX, 1, 31, &Size, &Reply02);
      if (Result) { Irq13Error(2, 8, 29); }
      break;
      }
    // ничего не делаем
    break;
    }
  else
    {
    // старый алгоритм
    }

  Size = Size29;
  if (!Result29) Irq13WaitReplyFlag = 0; // успешный обмен = получили квитанцию на предыдущее сообщение
  if ((!Result29) && (Size < 1)) { Irq13Error(3, 11, 29); break; }
  if ((Result29 == -1) || (Result29 == -2)) // нет ответного слова или есть биты ошибок (кроме занятости)
    {
    if ((Irq13SendNackEnabled) && (!Irq13WaitReplyFlag))
      {
      Irq13TxIndex = Irq13PrevTxIndex;
      Irq13TxMsgLen = Irq13PrevTxMsgLen;
      memmove(&Irq13TxBuffer, &Irq13PrevTxBuffer, sizeof(Irq13TxBuffer));
      Irq13SendMsgBn();
      break;
      }
    }

  Size = Size02;
  if (Result02 != -3)
    {
    if ((Result02 == -1) || (Result02 == -2))
      {
      if ((Irq13SendNackEnabled) && (!Irq13WaitReplyFlag))
        {
        // передаем отриц квитанцию
        Reply02 = 0x00FF;
        Result = Irq13MkoXchgBn(MkoLine, MkoChan, 13, 29, MKO_TX, 1, 31, &Size, &Reply02);
        if (Result) { Irq13Error(2, 8, 29); }
        break;
        }
      }
    else
      {
      if (Result02) { Irq13Error(2, 2, 2); break; }
      if (Size < 3) { Irq13Error(3, 3, 2); break; }
      // остаток сообщения дочитываем после считывания контрольной суммы

      // читаем контрольную сумму сообщения
      Result28 = Irq13MkoXchgBn(MkoLine, MkoChan, 13, 28, MKO_RX, 1, 31, &Size, &OrigSum);
      if (Result28 == -3)
        {
        SleepMSec(100);
        Result28 = Irq13MkoXchgBn(MkoLine, MkoChan, 13, 28, MKO_RX, 1, 31, &Size, &OrigSum);
        }
      if ((Result28 == -1) || (Result28 == -2))
        {
        if ((Irq13SendNackEnabled) && (!Irq13WaitReplyFlag))
          {
          // передаем отриц квитанцию
          Reply02 = 0x00FF;
          Result = Irq13MkoXchgBn(MkoLine, MkoChan, 13, 29, MKO_TX, 1, 31, &Size, &Reply02);
          if (Result) { Irq13Error(2, 8, 29); }
          break;
          }
        }
      if (Result28) { Irq13Error(2, 6, 28); break; }
      if (Size < 1) { Irq13Error(3, 7, 28); break; }

      // дочитываем остаток сообщения
      if (Irq13RxBuffer[1] > 56)
        {
        WantSize = ((Irq13RxBuffer[1] - 56) + 1) >> 1;
        Result02 = Irq13MkoXchgBn(MkoLine, MkoChan, 13, 3, MKO_RX, WantSize, 31, &Size, &Irq13RxBuffer[31]);
        if ((Result02 == -1) || (Result02 == -2))
          {
          if ((Irq13SendNackEnabled) && (!Irq13WaitReplyFlag))
            {
            // передаем отриц квитанцию
            Reply02 = 0x00FF;
            Result = Irq13MkoXchgBn(MkoLine, MkoChan, 13, 29, MKO_TX, 1, 31, &Size, &Reply02);
            if (Result) { Irq13Error(2, 8, 29); }
            break;
            }
          }
        if (Result02) { Irq13Error(2, 4, 3); break; }
        if (WantSize != Size) { Irq13Error(3, 5, 3); break; }
        }
      }
    }

  // ****************************************

  if ((!Result02) && (!Irq13WaitReplyFlag))
    {
    // проверяем допустимость передачи квитанции
    Disabled = Irq13ReplyIsDisabled(Irq13RxBuffer[0]);
    if (!Disabled)
      {
      // перерасчитываем контрольную сумму сообщения
      Size = 3 + ((Irq13RxBuffer[1] + 1) >> 1);
      RealSum = Irq13CalcSum(&Irq13RxBuffer, Size);
      Reply02 = (RealSum == OrigSum) ? (0x0011) : (0x00FF);

      // проверяем переопределение типа квитанции
      Redefined = Irq13ReplyTypeRedefined(Irq13RxBuffer[0], &ReplyType);
      if (Redefined) Reply02 = (ReplyType) ? (0x0011) : (0x00FF);

      // передаем квитанцию на принятое сообщение
      Result = Irq13MkoXchgBn(MkoLine, MkoChan, 13, 29, MKO_TX, 1, 31, &Size, &Reply02);
      if (Result) { SleepMSec(10); Result = Irq13MkoXchgBn(MkoLine, MkoChan, 13, 29, MKO_TX, 1, 31, &Size, &Reply02); }
      if (Result) { Irq13Error(2, 8, 29); break; }

      if (Reply02 == 0x0011)
        {
        // проверяем номер сообщения
        if (Irq13RxBuffer[2] != Irq13RxIndex) Irq13Error(0, 0, 0);
        Irq13RxIndex++;

        // выполняем принятую в сообщении инструкцию
        Result = Irq13MsgHandler();
        if (Result) { Irq13Error(4, 9, 0); break; }
        }
      }
    }

  if (!Result29)
    {
    // проверяем принятую квитанцию
    if (Reply29 != 0x0011)
      {
      if (!RedefinedTxRns) Irq13TxIndex--;
      Irq13Error(5, 12, 29);
      break;
      }

    // проверяем на занятость абонента 13/2
    // Result = Irq13MkoXchgBn(MkoLine, MkoChan, 13, 2, MKO_RX, 31, 31, &Size, &Irq13RxBuffer);
    // if (Result != -3) { Irq13Error(1, 13, 2); break; }

    // проверяем на занятость абонента 13/28
    // Result = Irq13MkoXchgBn(MkoLine, MkoChan, 13, 28, MKO_RX, 31, 31, &Size, &Irq13RxBuffer);
    // if (Result != -3) Irq13Error();
    }
  }
while (0);
}
//================================================================================

void Irq13IdleTask (void)
{
int DevMkoLine, DevMkoChan, DeltaTime, Head, Tail;
sysTime_t CurrTime;

// параметры линий обмена
DevMkoLine = SpoConfig.DevMkoLine & 1;
DevMkoChan = SpoConfig.DevMkoChan & 1;

timeRead(&CurrTime);

// циклическое считывание ИОК БАЛОИ (раз в 4 сек)
if (Baloi_Read_OK)
  {
  DeltaTime = DeltaTimeMSec(&Baloi_Read_OK_Time, &CurrTime);
  if (DeltaTime >= 4000)
    {
    Baloi_Read_OK_Time = CurrTime;
    Irq13DoDebugReadAn();
    }
  }

// циклической контроль исправности БАЛОИ (раз в 12 сек)
if (Ctrl_Ispravnosti_Enabled)
  {
  DeltaTime = DeltaTimeMSec(&Ctrl_Ispravnosti_Time, &CurrTime);
  if (DeltaTime >= 12000)
    {
    Ctrl_Ispravnosti_Time = CurrTime;
    if (Ctrl_Ispravnosti >= 3)
      {
      Ctrl_Ispravnosti_Enabled = 0;
      Irq13Error(30, 0, 0); // ???
      }
    else
      {
      sprIMaskVec(0xFFFF);

      // сообщение для БАЛОИ "контроль исправности"
      Irq13InitMsg(0x71BE, 0);
      if (!Irq13WaitReplyFlag)
        {
        Irq13SendMsgAn();
        Ctrl_Ispravnosti++;
        }
      else
        {
        Irq13MsgToFifo();
        }

      sprIMaskVec(~SpoConfig.EnabledIrqMask);
      }
    }
  }

// отправка флага исправности ЦБК в БАУ (раз в 1 мин)
if (Baloi_Send_Bau_OK)
  {
  DeltaTime = DeltaTimeMSec(&Baloi_Send_Bau_OK_Time, &CurrTime);
  if (DeltaTime >= 60000)
    {
    Baloi_Send_Bau_OK_Time = CurrTime;

    MkoBufA[0][0] = 0x0000;
    MkoXchgAn(DevMkoLine, DevMkoChan, MKO_CMD(12, 13, MKO_TX, 1), 0);
    }
  }

// ожидание квитанции от БАЛОИ
if (Irq13WaitReplyFlag)
  {
  DeltaTime = DeltaTimeMSec(&Irq13WaitReplyTimer, &CurrTime);
  if (DeltaTime >= 1000)
    {
    sprIMaskVec(0xFFFF);

    Irq13TxIndex = Irq13PrevTxIndex;
    Irq13TxMsgLen = Irq13PrevTxMsgLen;
    memmove(&Irq13TxBuffer, &Irq13PrevTxBuffer, sizeof(Irq13TxBuffer));
    Irq13SendMsgAn();

    sprIMaskVec(~SpoConfig.EnabledIrqMask);
    }
  }

// передача отложенных команд для БАЛОИ
sprIMaskVec(0xFFFF);

Head = Irq13CmdFifoHead;
Tail = Irq13CmdFifoTail;
if ((Head != Tail) && (!Irq13WaitReplyFlag))
  {
  Irq13TxMsgLen = Irq13CmdFifo[Tail].MsgLen;
  memmove(&Irq13TxBuffer, &Irq13CmdFifo[Tail].MsgBuf, sizeof(Irq13TxBuffer));
  Irq13SendMsgAn();
  Irq13CmdFifoTail = (Tail + 1) & IRQ13_CMD_FIFO_MASK;
  }

sprIMaskVec(~SpoConfig.EnabledIrqMask);
}
//================================================================================

int Irq13SendUserMsg (WORD Msg, WORD Count)
{
int Result, Index;

if (Count > 803) return REPLY_CMD_DATA_ERROR;

Irq13InitMsg(Msg, Count * sizeof(WORD));
for (Index = 0; Index < Count; Index++) Irq13AddToMsg(Irq13UserMsgData[Index]);

if (!Irq13WaitReplyFlag)
  {
  Result = Irq13SendMsgBn();
  }
else
  {
  Irq13MsgToFifo();
  Result = 0;
  }

return ((Result) ? (DECODE_XCHG_ERROR(Result)) : (REPLY_CMD_DONE));
}
//================================================================================

int Irq13SendMsgBnOverFifo (void)
{
int Result;

if (!Irq13WaitReplyFlag)
  {
  Result = Irq13SendMsgBn();
  }
else
  {
  Irq13MsgToFifo();
  Result = 0;
  }

return ((Result) ? (DECODE_XCHG_ERROR(Result)) : (REPLY_CMD_DONE));
}
//================================================================================

void Irq13MsgToFifo (void)
{
int BaseMkoLine, BaseMkoChan;
int Head, Tail, NextHead, Count;

if (!Irq13CmdFifoEnabled) return;
sprIMaskVec(0xFFFF);

Head = Irq13CmdFifoHead;
Tail = Irq13CmdFifoTail;
NextHead = (Head + 1) & IRQ13_CMD_FIFO_MASK;
Count = NextHead - Tail;

if (Count)
  {
  Irq13CmdFifo[Head].MsgLen = Irq13TxMsgLen;
  memmove(&Irq13CmdFifo[Head].MsgBuf, &Irq13TxBuffer, sizeof(Irq13TxBuffer));
  Irq13CmdFifoHead = NextHead;
  }

// параметры линий обмена с КПА
BaseMkoLine = SpoConfig.KpaMkoLine & 1;
BaseMkoChan = SpoConfig.KpaMkoChan & 1;

MkoBufA[0][0] = 0xFEF0;
MkoBufA[0][1] = (Irq13CmdFifoHead << 8) | Tail;
MkoBufA[0][2] = (Count) ? (0x0001) : (0x00FF);
MkoBufA[0][3] = 0;
MkoXchgAn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_DEBUG, MKO_TX, 4), 0);

sprIMaskVec(~SpoConfig.EnabledIrqMask);
}
//================================================================================

void Irq13InitMsg (WORD Msg, WORD DataSize)
{
Irq13TxMsgLen = 0;
Irq13TxBuffer[Irq13TxMsgLen++] = Msg;
Irq13TxBuffer[Irq13TxMsgLen++] = DataSize;
Irq13TxBuffer[Irq13TxMsgLen++] = 0;
}
//================================================================================

void Irq13AddToMsg (WORD Data)
{
Irq13TxBuffer[Irq13TxMsgLen++] = Data;
}
//================================================================================

int Irq13SendMsgBn (void)
{
int Result, Size, Try, MkoChan;
int DevMkoLine, DevMkoChan;
WORD Sum;

// параметры линий обмена
DevMkoLine = SpoConfig.DevMkoLine & 1;
DevMkoChan = SpoConfig.DevMkoChan & 1;

Irq13PrevTxIndex = Irq13TxIndex;
Irq13PrevTxMsgLen = Irq13TxMsgLen;
memmove(&Irq13PrevTxBuffer, &Irq13TxBuffer, sizeof(Irq13TxBuffer));

// ставим реальный номер РНС
Irq13TxBuffer[2] = (!RedefinedTxRns) ? (Irq13TxIndex++) : (Irq13TxIndex);

// считаем контрольную сумму пакета
Sum = Irq13CalcSum(&Irq13TxBuffer, Irq13TxMsgLen);

// проверяем необходимость внесения ошибки в CRC
if (Irq13CrcIsDisabled(Irq13TxBuffer[0])) Sum = ~Sum;

// передаем сообщение
for (Try = 0, MkoChan = DevMkoChan; Try < 8; Try++)
  {
  Result = Irq13MkoXchgBn(DevMkoLine, MkoChan, 13, 2, MKO_TX, Irq13TxMsgLen, 31, &Size, &Irq13TxBuffer);
  if (!Result) break;
  if (Pause_MkoTx) SleepMSec(Pause_MkoTx);
  if (Try == 3) MkoChan ^= 1;
  }
if (Result) { Irq13TxIndex = Irq13PrevTxIndex; return Result; }

// Result = Irq13MkoXchgBn(DevMkoLine, DevMkoChan, 13, 2, MKO_TX, Irq13TxMsgLen, 31, &Size, &Irq13TxBuffer);
// if (Result) { SleepMSec(10); Result = Irq13MkoXchgBn(DevMkoLine, DevMkoChan, 13, 2, MKO_TX, Irq13TxMsgLen, 31, &Size, &Irq13TxBuffer); }
// if (Result) return Result;

// передаем контрольную сумму
for (Try = 0, MkoChan = DevMkoChan; Try < 8; Try++)
  {
  Result = Irq13MkoXchgBn(DevMkoLine, MkoChan, 13, 28, MKO_TX, 1, 31, &Size, &Sum);
  if (!Result) break;
  if (Pause_MkoTx) SleepMSec(Pause_MkoTx);
  if (Try == 3) MkoChan ^= 1;
  }
if (Result) { Irq13TxIndex = Irq13PrevTxIndex; return Result; }

// Result = Irq13MkoXchgBn(DevMkoLine, DevMkoChan, 13, 28, MKO_TX, 1, 31, &Size, &Sum);
// if (Result) { SleepMSec(10); Result = Irq13MkoXchgBn(DevMkoLine, DevMkoChan, 13, 28, MKO_TX, 1, 31, &Size, &Sum); }
// if (Result) return Result;

Irq13WaitReplyFlag = 1;
timeRead(&Irq13WaitReplyTimer);

return 0;
}
//================================================================================

int Irq13SendMsgAn (void)
{
int Result, Size, Disabled, Try, MkoChan;
int DevMkoLine, DevMkoChan;
WORD Sum;

// параметры линий обмена
DevMkoLine = SpoConfig.DevMkoLine & 1;
DevMkoChan = SpoConfig.DevMkoChan & 1;

Irq13PrevTxIndex = Irq13TxIndex;
Irq13PrevTxMsgLen = Irq13TxMsgLen;
memmove(&Irq13PrevTxBuffer, &Irq13TxBuffer, sizeof(Irq13TxBuffer));

// ставим реальный номер РНС
Irq13TxBuffer[2] = (!RedefinedTxRns) ? (Irq13TxIndex++) : (Irq13TxIndex);

// считаем контрольную сумму пакета
Sum = Irq13CalcSum(&Irq13TxBuffer, Irq13TxMsgLen);

// проверяем необходимость внесения ошибки в CRC
Disabled = Irq13CrcIsDisabled(Irq13TxBuffer[0]);
if (Disabled) Sum = ~Sum;

// передаем сообщение
for (Try = 0, MkoChan = DevMkoChan; Try < 8; Try++)
  {
  Result = Irq13MkoXchgAn(DevMkoLine, MkoChan, 13, 2, MKO_TX, Irq13TxMsgLen, 31, &Size, &Irq13TxBuffer);
  if (!Result) break;
  if (Pause_MkoTx) SleepMSec(Pause_MkoTx);
  if (Try == 3) MkoChan ^= 1;
  }
if (Result) { Irq13TxIndex = Irq13PrevTxIndex; return Result; }

// Result = Irq13MkoXchgAn(DevMkoLine, DevMkoChan, 13, 2, MKO_TX, Irq13TxMsgLen, 31, &Size, &Irq13TxBuffer);
// if (Result != 0) return Result;

// передаем контрольную сумму
for (Try = 0, MkoChan = DevMkoChan; Try < 8; Try++)
  {
  Result = Irq13MkoXchgAn(DevMkoLine, MkoChan, 13, 28, MKO_TX, 1, 31, &Size, &Sum);
  if (!Result) break;
  if (Pause_MkoTx) SleepMSec(Pause_MkoTx);
  if (Try == 3) MkoChan ^= 1;
  }
if (Result) { Irq13TxIndex = Irq13PrevTxIndex; return Result; }

// Result = Irq13MkoXchgAn(DevMkoLine, DevMkoChan, 13, 28, MKO_TX, 1, 31, &Size, &Sum);
// if (Result != 0) return Result;

Irq13WaitReplyFlag = 1;
timeRead(&Irq13WaitReplyTimer);

return 0;
}
//================================================================================

int Irq13MsgHandler (void)
{
int Result, Disabled, Enabled;

Result = 0;
Irq13LastMsg = Irq13RxBuffer[0];
Irq13LastRns = Irq13RxBuffer[2];

// проверяем допустимость обработки сообщения
Disabled = Irq13MsgIsDisabled(Irq13LastMsg);
if (!Disabled)
  {
  switch (Irq13LastMsg)
    {
    // БВМ включена (info)
    case 0xC011: { break; }
    // сообщение "выдать код СЕВ" (req)
    case 0xC312: { Result = Irq13Msg0xC312(); break; }
    // сообщение "изделие 14Р735 исправно/неисправно" (info)
    case 0xA524: { Result = Irq13Msg0xA524(); break; }
    // сообщение "запрос предыдущего состояния изделия 15Э1827" (req)
    case 0xC514: { Result = Irq13Msg0xC514(); break; }
    // сообщение "запрос состояния АОС" (req)
    case 0xA021: { Result = Irq13Msg0xA021(); break; }
    // сообщение "текущее состояние изделия 15Э1827" (info)
    case 0xC918: { Result = Irq13Msg0xC918(); break; }
    // сообщение "АПД МТ включена, синхронизация установлена"
    case 0xB474: { Result = Irq13Msg0xB474(); break; }
    // сообщение "исправно"
    case 0xBE71: { Result = Irq13Msg0xBE71(); break; }
    // принимаем МКИ
    // case 0xCF1E: { Result = Irq13Msg0xCF1E(); break; }
    // сообщение "КПИ"
    case 0x8756: { Result = Irq13Msg0x8756(); break; }
    // сообщение "подключить канал 9.6МТ к изделию 15Э1827"
    case 0xB171: { Result = Irq13Msg0xB171(); break; }
    // сообщение "отключить канал 9.6МТ от изделия 15Э1827"
    case 0xB272: { Result = Irq13Msg0xB272(); break; }
    }

  if (Irq13BrtkMode2Enabled)
    {
    switch (Irq13LastMsg)
      {
      // сообщение "вкл. БПА АТ (2)"
      case 0xD233: { Result = Irq13Msg0xD233(); break; }
      // сообщение "вкл. БПА АТ (3)"
      case 0xD435: { Result = Irq13Msg0xD435(); break; }
      // сообщение "НРО"
      case 0xD736: { Result = Irq13Msg0xD736(); break; }
      // сообщение "код Fck"
      case 0xD839: { Result = Irq13Msg0xD839(); break; }
      // выдать продолж. ожидания РЕС.СЭС
      case 0xA322: { Result = Irq13Msg0xA322(); break; }
      // сообщение "запрос кода ДБ"
      case 0x9041: { Result = Irq13Msg0x9041(); break; }
      // сообщение "ввести ДН"
      case 0xDB3A: { Result = Irq13Msg0xDB3A(1); break; }
      // сообщение "включить ЛПЧ"
      case 0xDE3F: { Result = Irq13Msg0xDE3F(); break; }
      // сообщение "передача закончена"
      case 0x9647: { Result = Irq13Msg0x9647(); break; }
      // сообщение "выключить БПА АТ"
      case 0xDD3C: { Result = Irq13Msg0xDD3C(); break; }
      // сообщение "аппаратура АТ авария"
      // case 0x9C4D: { Result = Irq13Msg0x9C4D(); break; }
      // сообщение "нет обмена информацией"
      case 0x8455: { Result = Irq13Msg0x8455(); break; }
      // сообщение "сбой"
      case 0x9A4B:
      // сообщение "аппаратура АТ авария"
      case 0x9C4D:
      // сообщение "отказ БФС"
      case 0x9948:
      // сообщение "сменить коэф. БФС"
      case 0x9342: { Result = Irq13Msg0x9xxx(); break; }
      }
    }
  }

Enabled = Irq13DebugReadIsEnabled(Irq13LastMsg);
if (Enabled) Irq13DoDebugReadBn();

if (Irq13SingleEvent == ((int) Irq13LastMsg)) Irq13SingleEventMsg();

return Result;
}
//================================================================================

// сообщение "КПИ"
int Irq13Msg0x8756 (void)
{
int DevMkoLine, DevMkoChan;
int Index, KpiIndex, Len;

// параметры линий обмена
DevMkoLine = SpoConfig.DevMkoLine & 1;
DevMkoChan = SpoConfig.DevMkoChan & 1;

Len = Irq13RxBuffer[1] / 2;
if (KPI_Etalon_Len == Len)
  {
  KpiIndex = 0;
  for (Index = 0; Index < KPI_Etalon_Len; Index++)
    {
    if (KPI_Etalon[Index] != Irq13RxBuffer[Index + 3])
      {
      // сообщение для МКПА "Неверная КПИ", передать индекс СД
      Irq13Error(40, 2, Index);
      KpiIndex = 1;
      break;
      }
    }
  }
else
  {
  // сообщение для МКПА "Неверный размер КПИ"
  Irq13Error(40, 1, 0);
  KpiIndex = 1;
  }

// сообщение для БАЛОИ "квитанция на КПИ"
return Send_KPI_Kvit(KPI_Kvit[KpiIndex]);
}
//================================================================================

// сообщение "подключить канал 9.6МТ к изделию 15Э1827"
int Irq13Msg0xB171 (void)
{
int DevMkoLine, DevMkoChan, Result;

// параметры линий обмена
DevMkoLine = SpoConfig.DevMkoLine & 1;
DevMkoChan = SpoConfig.DevMkoChan & 1;

MkoBufB[0][0] = 0x8100;
Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(21, 3, MKO_TX, 1), 0);
if (Result) return 0; // Result;

// пауза перед проверкой телеметрии
if (Pause_AOS[0]) SleepMSec(Pause_AOS[0] * 1000);

Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(21, 14, MKO_RX, 7), 0);
if (Result) return 0; // DECODE_XCHG_ERROR(Result);

if ((MkoBufB[0][3] & 0x3800) == 0x2800)
  {
  // пауза для установки синхронизма с ЗС
  if (Pause_AOS[1]) SleepMSec(Pause_AOS[1] * 1000);

  // сообщение для БАЛОИ "канал 9.6МТ к изделию 15Э1827 подключен"
  Irq13InitMsg(0x77B7, 0);
  Irq13SendMsgBnOverFifo();
  }
else
  {
  // сообщение для МКПА "Ошибка при подключении канала 9.6МТ к изделию 15Э1827"
  Irq13Error(40, 3, 0);
  }

return 0;
}
//================================================================================

// сообщение "отключить канал 9.6МТ от изделия 15Э1827"
int Irq13Msg0xB272 (void)
{
int Result;
int DevMkoLine, DevMkoChan;

// параметры линий обмена
DevMkoLine = SpoConfig.DevMkoLine & 1;
DevMkoChan = SpoConfig.DevMkoChan & 1;

MkoBufB[0][0] = 0x8000;
Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(21, 3, MKO_TX, 1), 0);
if (Result) return 0; // Result;

// пауза перед проверкой телеметрии
if (Pause_AOS[0]) SleepMSec(Pause_AOS[0] * 1000);

Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(21, 14, MKO_RX, 7), 0);
if (Result) return 0; // DECODE_XCHG_ERROR(Result);

if ((MkoBufB[0][3] & 0x2000) != 0x2000)
  {
  // пауза для установки синхронизма с ЗС
  if (Pause_AOS[1]) SleepMSec(Pause_AOS[1] * 1000);

  // сообщение для БАЛОИ "канал 9.6МТ от изделия 15Э1827 отключен"
  Irq13InitMsg(0x78B8, 0);
  Irq13SendMsgBnOverFifo();
  }
else
  {
  // сообщение для КПА "Ошибка при подключении канала 9.6 от 1827"
  Irq13Error(40, 4, 0);
  }

return 0;
}
//================================================================================

int Send_KPI_Kvit (WORD Kvit)
{
Irq13InitMsg(0x3CDD, 2);
Irq13AddToMsg(Kvit);
return Irq13SendMsgBnOverFifo();
}
//================================================================================

// сообщение "нет обмена информацией"
int Irq13Msg0x8455 (void)
{
// сообщение для БАЛОИ "готов к работе с БПА АТ"
Irq13InitMsg(0x36D7, 0);
return Irq13SendMsgBnOverFifo();
}
//================================================================================

int Irq13Msg0xCF1E (void)
{
if ((Irq13RxBuffer[3] == 0x4C0B) &&
    (Irq13RxBuffer[4] == 0x312A) &&
    (Irq13RxBuffer[23] == 0x6B81))
  {
  Irq13TxIndex--;
  }

return 0;
}
//================================================================================

// сообщение "выдать код СЕВ" (req)
int Irq13Msg0xC312 (void)
{
int Index;
Irq13InitMsg(0x17C6, 6);
for (Index = 0; Index < 3; Index++) Irq13AddToMsg(Irq13TimeSEV[Index]);
return Irq13SendMsgBnOverFifo();
}
//================================================================================

// сообщение "запрос предыдущего состояния изделия 15Э1827" (req)
int Irq13Msg0xC514 (void)
{
int Index;
Irq13InitMsg(0x11C0, 60);
for (Index = 0; Index < 30; Index++) Irq13AddToMsg(Irq13DevState[Index]);
return Irq13SendMsgBnOverFifo();
}
//================================================================================

// сообщение "запрос состояния АОС" (req)
int Irq13Msg0xA021 (void)
{
int Result, State1, State2;
char DevMkoLine, DevMkoChan;

// параметры линий обмена
DevMkoLine = SpoConfig.DevMkoLine & 1;
DevMkoChan = SpoConfig.DevMkoChan & 1;

Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(12, 17, MKO_RX, 5), 0);
if (Result) return DECODE_XCHG_ERROR(Result);

State1 = (MkoBufB[0][4] & 0x3800) >> 11;
State2 = (MkoBufB[0][4] & 0x0600);
if ((State1) && (State2 == 0x0600))
  {
  switch (State1)
    {
    case 1: { Komplekt_AOS = 0xFFFF; break; }
    case 2: { Komplekt_AOS = 0xFF00; break; }
    case 4: { Komplekt_AOS = 0x00FF; break; }
    default: { goto OnError1; }
    }

  Irq13InitMsg(0x4796, 0);
  return Irq13SendMsgBnOverFifo();
  }
else
  {
  OnError1:
  Irq13InitMsg(0x5988, 0);
  Irq13SendMsgBnOverFifo();

  Irq13Error(21, 1, 0);
  return 0;
  }
}
//================================================================================

// сообщение "исправно"
int Irq13Msg0xBE71 (void)
{
Ctrl_Ispravnosti = 0;
return 0;
}
//================================================================================

// сообщение "АПД МТ включена, синхронизация установлена"
int Irq13Msg0xB474 (void)
{
int Result;
char DevMkoLine, DevMkoChan;

// параметры линий обмена
DevMkoLine = SpoConfig.DevMkoLine & 1;
DevMkoChan = SpoConfig.DevMkoChan & 1;

Komplekt_APD_MT = Irq13RxBuffer[3];

Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(21, 14, MKO_RX, 7), 0);
if (Result) return DECODE_XCHG_ERROR(Result);

if (MkoBufB[0][3] & 0x0800)
  {
  Irq13InitMsg(0x71B1, 4);
  Irq13AddToMsg(Komplekt_AOS);
  Irq13AddToMsg(Komplekt_APD_MT);
  Irq13SendMsgBnOverFifo();
  }
else
  {
  Irq13InitMsg(0x72B2, 4);
  Irq13AddToMsg(Komplekt_AOS);
  Irq13AddToMsg(Komplekt_APD_MT);
  Irq13SendMsgBnOverFifo();

  Irq13Error(22, 1, 0); // ???
  }

return 0;
}
//================================================================================

// сообщение "текущее состояние изделия 15Э1827" (info)
int Irq13Msg0xC918 (void)
{
WORD State;

State = Irq13RxBuffer[3];
if ((State & 0x000F) == 2) { Rejim_BRTK = 1; }

memcpy(&Irq13DevState, &Irq13RxBuffer[3], sizeof(Irq13DevState));

return 0;
}
//================================================================================

// сообщение "изделие 14Р735 исправно/неисправно" (info)
int Irq13Msg0xA524 (void)
{
if (Irq13RxBuffer[3] != 0x00FF) { Irq13Error(20, 1, 0); return 0; }
return 0;
}
//================================================================================

// сообщение "вкл. БПА АТ (2)"
int Irq13Msg0xD233 (void)
{
Rejim_BRTK = 2;
return 0;
}
//================================================================================

// сообщение "вкл. БПА АТ (3)"
int Irq13Msg0xD435 (void)
{
Rejim_BRTK = 3;
return 0;
}
//================================================================================

// сообщение "НРО"
int Irq13Msg0xD736 (void)
{
if ((Rejim_BRTK != 2) && (Rejim_BRTK != 3)) { Irq13Error(11, 1, 0); return 0; }
return 0;
}
//================================================================================

// сообщение "код Fck"
int Irq13Msg0xD839 (void)
{
int Result;
char DevMkoLine, DevMkoChan;
WORD Msg;

// параметры линий обмена
DevMkoLine = SpoConfig.DevMkoLine & 1;
DevMkoChan = SpoConfig.DevMkoChan & 1;

if (!SAA_Ustanovlena)
  {
  Result = Vkluchenie_BPA_AT(0, Pause_MFS[0]);
  if (Result) return Result;
  }

Result = Control_ZTM(LPH_Vkluchena);
if (!Result)
  {
  if ((BUFAR) && (AFAR_AT_M) && (!SAA_Ustanovlena))
    {
    Result = Control_BUFAR();
    if (Result) goto OtklBpaAt;
    }

  // сообщение для БАЛОИ "САА установлена (2)" или "САА установлена (3)"
  Msg = (Rejim_BRTK == 2) ? (0x27A6) : (0x28A9);
  Irq13InitMsg(Msg, 0);
  Result = Irq13SendMsgBnOverFifo();
  }
else
  {
  OtklBpaAt:
  if (Otkl_BPA_AT)
    {
    // отключение БПА АТ
    MkoBufB[0][0] = (BUFAR) ? (0x80 >> BUFAR) : (0);
    MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(12, 17, MKO_TX, 1), 0);
    }

  // сообщение для МКПА "ошибка ЗТМ при включении БПА АТ"
  Irq13Error(11, 2, 0);

  Sbros_Priznakov();

  // сообщение для БАЛОИ "САА не установлена"
  Irq13InitMsg(0x2BAA, 0);
  Result = Irq13SendMsgBnOverFifo();
  }

return Result;
}
//================================================================================

int Vkluchenie_BPA_AT (int Vkl_LPH, int Pause)
{
int Result;
int DevMkoLine, DevMkoChan;

// параметры линий обмена
DevMkoLine = SpoConfig.DevMkoLine & 1;
DevMkoChan = SpoConfig.DevMkoChan & 1;

MkoBufB[0][0] = K_BRTK_A(Vkl_LPH);
Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(12, 17, MKO_TX, 1), 0);
if (Result) return Result;

SleepMSec(40);

Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(12, 16, MKO_RX, 1), 0);
if (Result) return Result;

// проверка ЗОБ
if (MkoBufB[0][0] == 0)
  {
  SleepMSec(Pause);
  }
else
  {
  // сообщение для КПА "Ошибка ЗОБ при включении БПА АТ"
  Irq13Error(40, 5, 0);

  SAA_Ustanovlena = 0;
  LPH_Vkluchena = 0;
  }

return Result;
}
//================================================================================

int Control_ZTM (int Vkl_LPH)
{
int Result;
int DevMkoLine, DevMkoChan;
int Ztm1, Ztm2, Mask;

// параметры линий обмена
DevMkoLine = SpoConfig.DevMkoLine & 1;
DevMkoChan = SpoConfig.DevMkoChan & 1;

Ztm1 = (((Vkl_LPH) ? (0x10020) : (0x10000)) >> Komplekt_ALPS) | (0x2000 >> Komplekt_MFS) | (0x03E1);
Ztm2 = (0x0200 >> ((Control_ZTM_BUFAR) ? (BUFAR) : (0))) | (0xFE00);
Mask = (Vkl_LPH) ? (0xFFFD) : (0xFFE1);

Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(12, 17, MKO_RX, 14), 0);
if (Result)
  {
  SAA_Ustanovlena = 0;
  LPH_Vkluchena = 0;
  return 1;
  }

// проверка ЗТМ
if (((MkoBufB[0][0] & Mask) == Ztm1) && ((MkoBufB[0][1] & 0xFFC0) == Ztm2))
  {
  SAA_Ustanovlena = Rejim_BRTK;
  LPH_Vkluchena = Vkl_LPH;
  return 0;
  }
else
  {
  // сообщение для МКПА "Ошибка ЗТМ при включении БПА АТ"
  Irq13Error(41, 0, 0);

  SAA_Ustanovlena = 0;
  LPH_Vkluchena = 0;
  return 1;
  }
}
//================================================================================

// выдать продолж. ожидания РЕС.СЭС
int Irq13Msg0xA322 (void)
{
// передача сообщения "продолж. ожидания РЕС.СЭС"
Irq13InitMsg(0x14C5, 4);
Irq13AddToMsg(0x0000);
Irq13AddToMsg(0x0000);
return Irq13SendMsgBnOverFifo();
}
//================================================================================

// сообщение "запрос кода ДБ"
int Irq13Msg0x9041 (void)
{
// сообщение для БАЛОИ "код ДБ"
Irq13InitMsg(0x2EAF, 2);
Irq13AddToMsg(Code_DB);
return Irq13SendMsgBnOverFifo();
}
//================================================================================

// сообщение "ввести ДН"
int Irq13Msg0xDB3A (int MsgFlag)
{
int Result;

if (BUFAR)
  {
  if ((AFAR_AT_M) && (!LPH_Vkluchena))
    {
    Result = Vkluchenie_BPA_AT(1, Pause_LPH);
    if (Result) return Result;

    Result = Control_ZTM(1);
    if (Result) return Result;
    }

  if (DN_Vvedena != Rejim_BRTK)
    {
    Result = Vkluchenie_AFAR();
    if (Result) return Result;
    }

  if (LPH_Vkluchena)
    {
    Result = Control_AFAR();
    if (Result) return Result;
    }
  }

// сообщение для БАЛОИ "ДН установлена"
Irq13InitMsg(0x2DAC, 0);
return Irq13SendMsgBnOverFifo();
}
//================================================================================

int Control_AFAR (void)
{
int Result;
int DevMkoLine, DevMkoChan;
int i, Etalon, Mask;

// параметры линий обмена
DevMkoLine = SpoConfig.DevMkoLine & 1;
DevMkoChan = SpoConfig.DevMkoChan & 1;

if (AFAR_AT_M)
  {
  Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(18, 2, MKO_RX, 15), 0);
  if (Result) goto OnError2;

  Etalon = (Rejim_BRTK == 2) ? (Nomer_Shetverti) : (5);
  if ((MkoBufB[0][1] >> 12) != Etalon) goto OnError2;
  }
else
  {
  Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(18, 29, MKO_RX, 9), 0);
  if (Result) goto OnError2;
  }

for (i = 0; i < ((AFAR_AT_M) ? (15) : (9)); i++)
  {
  if (i < 2)
    {
    Mask = (i == AFAR_AT_M) ? (0x0FFF) : (0xFFFF);
    if ((MkoBufB[0][i] & Mask) != Tm_AFAR_BUM[Index_AFAR][i]) goto OnError2;
    }
  else
    {
    Mask = ((i == 8) && (AFAR_AT_M == 0)) ? (0xF000) : (0xFFFF);
    if ((MkoBufB[0][i] & Mask) != Tm_AFAR_PRD[Index_AFAR][i]) goto OnError2;
    }
  }

DN_Vvedena = Rejim_BRTK;
return Result;

OnError2:
DN_Vvedena = 0;
Result = 1;

// сообщение для МКПА "ошибка при включении АФАР"
Irq13Error(21, 5, 0);

// сообщение для БАЛОИ "Отказ БУФАР"
Irq13InitMsg(0x35D4, 0);
Irq13SendMsgBnOverFifo();

return Result;
}
//================================================================================

int Vkluchenie_AFAR (void)
{
int Result;
int DevMkoLine, DevMkoChan;
int i, j, m, State, W0, W1;

// параметры линий обмена
DevMkoLine = SpoConfig.DevMkoLine & 1;
DevMkoChan = SpoConfig.DevMkoChan & 1;

Index_AFAR = (Rejim_BRTK == 2) ? (Nomer_Shetverti) : (0);

if (AFAR_AT_M)
  {
  MkoBufB[0][0] = (Rejim_BRTK == 2) ? (Nomer_Shetverti) : (5);
  MkoBufB[0][1] = 0;
  MkoBufB[0][2] = 0;

  Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(18, 1, MKO_TX, 3), 0);
  if (Result) return Result;

  goto OnDone1;
  }

if (DN_Vvedena)
  {
  Otkl_kanalov_AFAR();
  SleepMSec(100);
  }

for (i = 0; i < 4; i++)
  for (j = 0; j < 28; j++)
    Vkl_PRD_0[i][j] = 0x3F00;

memmove(&MkoBufB[0][0], &Vkl_AFAR_BUM[Index_AFAR], 2 * sizeof(WORD));
Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(18, 9, MKO_TX, 2), 0);
if (Result) return Result;

if (Pause_BUM[Control_BUM]) SleepMSec(Pause_BUM[Control_BUM]);

if (Control_BUM)
  {
  Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(18, 29, MKO_RX, 2), 0);
  if (Result) return Result;

  State = 0;
  W0 = MkoBufB[0][0] & 0x0FFF;
  W1 = MkoBufB[0][1];
  if ((W0 == Tm_AFAR_BUM[Index_AFAR][0]) && (W1 == Tm_AFAR_BUM[Index_AFAR][1])) State = 1;

  if (!State)
    {
    DN_Vvedena = 0;
    Result = 1;

    // сообщение для МКПА "Ошибка при включении БУМ"
    Irq13Error(43, 0, 0);

    // сообщение для БАЛОИ "Отказ БУФАР"
    Irq13InitMsg(0x35D4, 0);
    Irq13SendMsgBnOverFifo();

    return Result;
    }
  }

for (i = 0; i < 5; i++)
  {
  for (j = 0; j < 4; j++)
    {
    for (m = 0; (m < ((j == 3) ? (4) : (7))) && (i < 4); m++)
      {
      Vkl_PRD_0[j][i + (m * 4)] = Vkl_AFAR_PRD[Index_AFAR][j][i + (m * 4)];
      }
    memmove(&MkoBufB[0][0], &Vkl_PRD_0[j], sizeof(WORD) * 28);
    Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(18, j + 1, MKO_TX, ((j == 3) ? (16) : (28))), 0);
    if (Result) return Result;
    }
  if (i < 3) { SleepMSec(100); }
  else if (i == 3) { SleepMSec(Pause_AFAR); }
  }

OnDone1:
if (Pause_PRD) SleepMSec(Pause_PRD);
DN_Vvedena = Rejim_BRTK;

return Result;
}
//================================================================================

int Otkl_kanalov_AFAR (void)
{
int Result;
int DevMkoLine, DevMkoChan;
int i;

// параметры линий обмена
DevMkoLine = SpoConfig.DevMkoLine & 1;
DevMkoChan = SpoConfig.DevMkoChan & 1;

for (i = 0; i < 28; i++) MkoBufB[0][i] = 0x3F00;

Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(18, 1, MKO_TX, 28), 0);
if (Result) { } // return Result;
Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(18, 2, MKO_TX, 28), 0);
if (Result) { } // return Result;
Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(18, 3, MKO_TX, 28), 0);
if (Result) { } // return Result;
Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(18, 4, MKO_TX, 16), 0);
if (Result) { } // return Result;

return 0;
}
//================================================================================

int Control_AFAR_Fail (void)
{
// сообщение для КПА "Ошибка при включении АФАР"
Irq13Error(44, 0, 0);

DN_Vvedena = 0;

// сообщение для БАЛОИ "Отказ БУФАР"
Irq13InitMsg(0x35D4, 0);
Irq13SendMsgBnOverFifo();
return 0;
}
//================================================================================

int Irq13InfoMsg0x35D4 (void)
{
int Result;

// ошибка "Отказ БУФАР"
Irq13InitMsg(0x35D4, 0);
Result = Irq13SendMsgBnOverFifo();
if (Result) return DECODE_XCHG_ERROR(Result);

// сообщение для КПА "Ошибка при включении АФАР"
// ...

return 0;
}
//================================================================================

// сообщение "включить ЛПЧ"
int Irq13Msg0xDE3F (void)
{
int Result;

if (!LPH_Vkluchena)
  {
  Result = Vkluchenie_BPA_AT(1, Pause_LPH);
  if (Result) return Result;
  }

Result = Control_ZTM(1);
if (!Result)
  {
  // сообщение для БАЛОИ "норма раскачки УТ"
  Irq13InitMsg(0x30D1, 0);
  Result = Irq13SendMsgBnOverFifo();
  }
else
  {
  // сообщение для МКПА "ошибка ЗТМ при включении ЛПЧ"
  Irq13Error(21, 3, 0);

  // сообщение для БАЛОИ "отказ раскачки УТ"
  Irq13InitMsg(0x33D2, 0);
  Irq13SendMsgBnOverFifo();
  }

return Result;
}
//================================================================================

int Irq13InfoMsg0x33D2 (void)
{
int Result;

// ошибка "Отказ раскачки УТ"
Irq13InitMsg(0x33D2, 0);
Result = Irq13SendMsgBnOverFifo();
if (Result) return DECODE_XCHG_ERROR(Result);

// сообщение для КПА "Ошибка ЗТМ при включении ЛПЧ"
// ...

return 1;
}
//================================================================================

// сообщение "передача закончена"
int Irq13Msg0x9647 (void)
{
return 0;
}
//================================================================================

// сообщение "выключить БПА АТ"
int Irq13Msg0xDD3C (void)
{
Otkluchenie_AT();

// сообщение для БАЛОИ "БПА АТ выключена"
Irq13InitMsg(0x4B9A, 0);
return Irq13SendMsgBnOverFifo();
}
//================================================================================

int Otkluchenie_AT (void)
{
int Result;
int DevMkoLine, DevMkoChan;
int i, W0, W1, Otkl[2];

// параметры линий обмена
DevMkoLine = SpoConfig.DevMkoLine & 1;
DevMkoChan = SpoConfig.DevMkoChan & 1;

if (BUFAR)
  {
  if (AFAR_AT_M)
    {
    // отключение АФАР АТ-М
    MkoBufB[0][0] = 0;
    MkoBufB[0][1] = 0;
    MkoBufB[0][2] = 0;
    Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(18, 1, MKO_TX, 3), 0);
    if (Result) { }

    SleepMSec(Pause_BUM[2]);
    }
  else
    {
    if (Rejim_BRTK != 2)
      {
      Otkl[0] = 0x0000; Otkl[1] = 0x0000;

      for (i = 0; i < 4; i++)
        {
        if (i == 0) { Otkl[0] = 0x07FE; Otkl[1] = 0xFF80; }
        if (i == 1) { Otkl[0] = 0x07FE; Otkl[1] = 0xE000; }
        if (i == 2) { Otkl[0] = 0x07F0; Otkl[1] = 0x0000; }
        if (i == 3) { Otkl[0] = 0x0000; Otkl[1] = 0x0000; }

        MkoBufB[0][0] = Otkl[0];
        MkoBufB[0][1] = Otkl[1];
        Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(18, 9, MKO_TX, 2), 0);
        if (Result) { }

        SleepMSec(Pause_BUM[2]);
        }
      }
    else
      {
      MkoBufB[0][0] = 0;
      MkoBufB[0][1] = 0;
      Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(18, 9, MKO_TX, 2), 0);
      if (Result) { }

      SleepMSec(Pause_BUM[2]);
      }
    }
  }

// отключение БПА АТ и БУФАР
MkoBufB[0][0] = 0;
Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(12, 17, MKO_TX, 1), 0);
if (Result) { }

SleepMSec(40);

Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(12, 16, MKO_RX, 1), 0);
if (Result) { }

// проверка ЗОБ
if (MkoBufB[0][0] == 0)
  {
  SleepMSec(Pause_MFS[1]);

  Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(12, 17, MKO_RX, 14), 0);
  if (Result) { }

  // проверка ЗТМ
  W0 = MkoBufB[0][0] & 0xFDFF;
  W1 = MkoBufB[0][1] & 0xFDC0;
  if (!((W0 == 0) && (W1 == 0)))
    {
    // сообщение для МКПА "ошибка ЗТМ при отключении БПА АТ"
    Irq13Error(21, 7, 0);

    // сообщение для БАЛОИ "БПА АТ не выключить"
    Irq13InitMsg(0x5382, 0);
    Irq13SendMsgBnOverFifo();
    }
  }
else
  {
  // сообщение для МКПА "ошибка ЗОБ при отключении БПА АТ"
  Irq13Error(21, 6, 0);

  // сообщение для БАЛОИ "БПА АТ не выключить"
  Irq13InitMsg(0x5382, 0);
  Irq13SendMsgBnOverFifo();
  }

Sbros_Priznakov();
return 0;
}
//================================================================================

int Oper_0962 (void)
{
int Result;
int DevMkoLine, DevMkoChan;

// параметры линий обмена
DevMkoLine = SpoConfig.DevMkoLine & 1;
DevMkoChan = SpoConfig.DevMkoChan & 1;

Result = Vkluchenie_BPA_AT(0, Pause_MFS[0]);
if (Result) return Result;

Result = Vkluchenie_BPA_AT(1, Pause_LPH);
if (Result) return Result;

Result = Control_ZTM(1);
if (Result) return Result;

Result = Vkluchenie_AFAR();
if (Result) return Result;

Result = Control_AFAR();
return Result;
}
//================================================================================

int Otkluchenie_AT_Fail (int Msg)
{
// сообщение об ошибке для КПА
Irq13Error(Msg, 0, 0);

// сообщение для БАЛОИ "БПА АТ не выключить"
Irq13InitMsg(0x5382, 0);
Irq13SendMsgBnOverFifo();
return 0;
}
//================================================================================

int Otkluchenie_BUM_AFAR (int Mode)
{
int DevMkoLine, DevMkoChan, i;

// параметры линий обмена
DevMkoLine = SpoConfig.DevMkoLine & 1;
DevMkoChan = SpoConfig.DevMkoChan & 1;

if (Mode != 2)
  {
  for (i = 0; i < 4; i++)
    {
    MkoBufB[0][0] = Data_OTKL_AFAR[i * 2 + 0];
    MkoBufB[0][1] = Data_OTKL_AFAR[i * 2 + 1];
    MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(18, 9, MKO_TX, 2), 0);
    if (Pause_BUM[2]) SleepMSec(Pause_BUM[2]);
    }
  }
else
  {
  MkoBufB[0][0] = 0;
  MkoBufB[0][1] = 0;
  MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(18, 9, MKO_TX, 2), 0);
  if (Pause_BUM[2]) SleepMSec(Pause_BUM[2]);
  }

return 0;
}
//================================================================================

int Irq13Msg0x9C4D (void)
{
return Otkluchenie_AT();
}
//================================================================================

// сообщения "сбой","аппаратура АТ авария","отказ БФС","сменить коэф. БФС"
int Irq13Msg0x9xxx (void)
{
int Result;
int DevMkoLine, DevMkoChan;

// параметры линий обмена
DevMkoLine = SpoConfig.DevMkoLine & 1;
DevMkoChan = SpoConfig.DevMkoChan & 1;

Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(12, 17, MKO_RX, 25), 0);
if (Result) return Result;

Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(13, 1, MKO_RX, 31), 0);
if (Result) return Result;

Otkluchenie_AT();

return Result;
}
//================================================================================

void Sbros_Priznakov (void)
{
SAA_Ustanovlena = 0;
DN_Vvedena = 0;
LPH_Vkluchena = 0;
}
//================================================================================

int Irq13InfoMsg0x5382 (void)
{
int Result;

// ошибка "БПА АТ НЕ ВЫКЛЮЧИТЬ"
Irq13InitMsg(0x5382, 0);
Result = Irq13SendMsgBnOverFifo();
if (Result) return DECODE_XCHG_ERROR(Result);

// сообщение для КПА "Ошибка ЗОБ при отключении БПА АТ"
// ...

return 1;
}
//================================================================================

void Irq13Error (int Error, int Point, int Info)
{
int BaseMkoLine, BaseMkoChan;
WORD Buffer[4];

if (!Error) return;

BaseMkoLine = SpoConfig.KpaMkoLine & 1;
BaseMkoChan = SpoConfig.KpaMkoChan & 1;

Buffer[0] = Error;
Buffer[1] = Point;
Buffer[2] = Info;
Buffer[3] = 0;

ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, REPLY_IRQ_0D_ERROR, KPA_SUBADDR_DEBUG, &Buffer, sizeof(Buffer));
}
//================================================================================

void Irq13SingleEventMsg (void)
{
int BaseMkoLine, BaseMkoChan;

BaseMkoLine = SpoConfig.KpaMkoLine & 1;
BaseMkoChan = SpoConfig.KpaMkoChan & 1;

ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, REPLY_WAITED_BALOI_MSG, KPA_SUBADDR_IRQ);
}
//================================================================================

WORD Irq13CalcSum (void *Buffer, int Size)
{
WORD Sum, *Src;
Sum = 0;
Src = (WORD*) Buffer;
while (Size--) Sum ^= *Src++;
return Sum;
}
//================================================================================

// передача данных из процедур прерывания, пакет B
// линия МКО, канал МКО, адрес ОУ, начальный подадрес ОУ, направление передачи, общее число слов,
//   макс число слов с/на подадрес, число считанных слов, буфер с/под данные
int Irq13MkoXchgBn (int MkoLine, int MkoChan, int MkoAddr, int MkoSubAddr, int MkoDir, int DataSize,
  int BlockSize, int *ReadSize, void *DataBuffer)
{
int Result, MkoMode, MsgCount, WordCount, Index, Len, TotalLen;
STATUS Status;
WORD *Buffer;

Result = MsgCount = 0;
MkoMode = (MkoDir == MKO_RX) ? (mcTM_RTtoBC) : (mcTM_BCtoRT);
Buffer = (WORD*) DataBuffer;
*ReadSize = 0;

while (DataSize)
  {
  WordCount = (DataSize > BlockSize) ? (BlockSize) : (DataSize);

  // настройка сообщения
  MkoMsgB[MsgCount].TM   = MkoMode;     // тип сообщения, RX/TX
  MkoMsgB[MsgCount].Chan = MkoChan;     // канал МКО, 0 - основной, 1 - резервный
  MkoMsgB[MsgCount].CW   = MKO_CMD(MkoAddr, MkoSubAddr, MkoDir, WordCount);     // первое командное слово
  MkoMsgB[MsgCount].MissSW   = 1;       // прервать передачу при отсутствии ОС
  MkoMsgB[MsgCount].FlagSW   = 1;       // прервать передачу при ошибках в ОС
  MkoMsgB[MsgCount].FmtError = 1;       // прервать передачу при ошибках передачи
  MkoMsgB[MsgCount].GapTime  = 0;       // передача без пауз

  // сброс флагов для сообщения
  memset(&MkoMsgResB[MsgCount], 0, sizeof(mkoMsgRes_t));

  // перемещение блока данных для передачи
  if (MkoDir == MKO_TX)
    {
    memmove(&MkoBufB[MsgCount], Buffer, WordCount << 1);
    Buffer += WordCount;
    }

  // контроль запрещенных сообщений
  // ...

  DataSize -= WordCount;
  MsgCount++;
  MkoSubAddr++;
  }

// настройка пакета сообщений
MkoPackB.nLast = (MsgCount - 1);

// сброс флагов для пакета
memset(&MkoPackResB, 0, sizeof(mkoPackRes_t));

do
  {
  // запуск обмена по МКО
  Status = mkoPackIniStart(MkoLine, &MkoPackB, MkoMsgPtrB, 0);
  if (Status != OK) { Result = 1; break; }

  // ожидание завершения обмена по МКО
  Status = mkoNumPackWait(MkoLine, &MkoPackB);
  if (Status != OK) { Result = 2; break; }

  // проверка результата обмена
  Result = CheckMkoXchg(&MkoPackResB, &MkoMsgB[0], &MkoMsgResB[0], MsgCount);
  if (Result) break;

  // проверяем принятые данные
  if (MkoDir == MKO_RX)
    {
    for (Index = 0, TotalLen = 0; Index < MsgCount; Index++)
      {
      Len = MkoMsgResB[Index].Len;
      TotalLen += Len;

      // перемещаем принятые данные
      memmove(Buffer, &MkoBufB[Index], Len << 1);
      Buffer += Len;
      }
    *ReadSize = TotalLen;
    }

  // обмен успешно завершен
  Result = 0;
  }
while (0);

// обмен завершен
return Result;
}
//================================================================================

// передача данных из основного потока, пакет A
// линия МКО, канал МКО, адрес ОУ, начальный подадрес ОУ, направление передачи, общее число слов, макс число слов с/на подадрес, число считанных слов, буфер с/под данные
int Irq13MkoXchgAn (int MkoLine, int MkoChan, int MkoAddr, int MkoSubAddr, int MkoDir, int DataSize, int BlockSize, int *ReadSize, void *DataBuffer)
{
int Result, MkoMode, MsgCount, WordCount, Index, Len, TotalLen;
STATUS Status;
WORD *Buffer;

Result = MsgCount = 0;
MkoMode = (MkoDir == MKO_RX) ? (mcTM_RTtoBC) : (mcTM_BCtoRT);
Buffer = (WORD*) DataBuffer;
*ReadSize = 0;

while (DataSize)
  {
  WordCount = (DataSize > BlockSize) ? (BlockSize) : (DataSize);

  // настройка сообщения
  MkoMsgA[MsgCount].TM   = MkoMode;     // тип сообщения, RX/TX
  MkoMsgA[MsgCount].Chan = MkoChan;     // канал МКО, 0 - основной, 1 - резервный
  MkoMsgA[MsgCount].CW   = MKO_CMD(MkoAddr, MkoSubAddr, MkoDir, WordCount);     // первое командное слово
  MkoMsgA[MsgCount].MissSW   = 1;       // прервать передачу при отсутствии ОС
  MkoMsgA[MsgCount].FlagSW   = 1;       // прервать передачу при ошибках в ОС
  MkoMsgA[MsgCount].FmtError = 1;       // прервать передачу при ошибках передачи
  MkoMsgA[MsgCount].GapTime  = 0;       // передача без пауз

  // сброс флагов для сообщения
  memset(&MkoMsgResA[MsgCount], 0, sizeof(mkoMsgRes_t));

  // перемещение блока данных для передачи
  if (MkoDir == MKO_TX)
    {
    memmove(&MkoBufA[MsgCount], Buffer, WordCount << 1);
    Buffer += WordCount;
    }

  // контроль запрещенных сообщений
  // ...

  DataSize -= WordCount;
  MsgCount++;
  MkoSubAddr++;
  }

// настройка пакета сообщений
MkoPackA.nLast = (MsgCount - 1);

// сброс флагов для пакета
memset(&MkoPackResA, 0, sizeof(mkoPackRes_t));

do
  {
  // запуск обмена по МКО
  Status = mkoPackIniStart(MkoLine, &MkoPackA, MkoMsgPtrA, 0);
  if (Status != OK) { Result = 1; break; }

  // ожидание завершения обмена по МКО
  Status = mkoNumPackWait(MkoLine, &MkoPackA);
  if (Status != OK) { Result = 2; break; }

  // проверка результата обмена
  Result = CheckMkoXchg(&MkoPackResA, &MkoMsgA[0], &MkoMsgResA[0], MsgCount);
  if (Result) break;

  // проверяем принятые данные
  if (MkoDir == MKO_RX)
    {
    for (Index = 0, TotalLen = 0; Index < MsgCount; Index++)
      {
      Len = MkoMsgResA[Index].Len;
      TotalLen += Len;

      // перемещаем принятые данные
      memmove(Buffer, &MkoBufA[Index], Len << 1);
      Buffer += Len;
      }
    *ReadSize = TotalLen;
    }

  // обмен успешно завершен
  Result = 0;
  }
while (0);

// обмен завершен
return Result;
}
//================================================================================

int Irq13MsgIsDisabled (WORD Msg)
{
int Index, Found;

// ищем сообщение в списке
for (Index = 0, Found = 0; Index < Baloi_DisabledMsgListCount; Index++)
  {
  if (Baloi_DisabledMsgList[Index] == Msg) { Found = 1; break; }
  }

return Found;
}
//================================================================================

int Irq13ChangeDisabledMsgList (WORD Msg, WORD Mode)
{
int Index, Found;

// ищем сообщение в списке
for (Index = 0, Found = 0; Index < Baloi_DisabledMsgListCount; Index++)
  {
  if (Baloi_DisabledMsgList[Index] == Msg) { Found = 1; break; }
  }

if (Found)
  {
  if (Mode)
    {
    // убрать сообщение из списка
    for (Index = Index + 1; Index < Baloi_DisabledMsgListCount; Index++) Baloi_DisabledMsgList[Index - 1] = Baloi_DisabledMsgList[Index];
    Baloi_DisabledMsgListCount--;
    }
  }
else
  {
  if (!Mode)
    {
    // добавить сообщение в список
    if (Baloi_DisabledMsgListCount >= BALOI_MAX_DISABLED_MSG_COUNT) return REPLY_CMD_OVERFLOW_ERROR;
    Baloi_DisabledMsgList[Baloi_DisabledMsgListCount++] = Msg;
    }
  }

return REPLY_CMD_DONE;
}
//================================================================================

int Irq13ReplyIsDisabled (WORD Msg)
{
int Index, Found;

// ищем сообщение в списке
for (Index = 0, Found = 0; Index < Baloi_DisabledReplyListCount; Index++)
  {
  if (Baloi_DisabledReplyList[Index] == Msg) { Found = 1; break; }
  }

return Found;
}
//================================================================================

int Irq13ChangeDisabledReplyList (WORD Msg, WORD Mode)
{
int Index, Found;

// ищем сообщение в списке
for (Index = 0, Found = 0; Index < Baloi_DisabledReplyListCount; Index++)
  {
  if (Baloi_DisabledReplyList[Index] == Msg) { Found = 1; break; }
  }

if (Found)
  {
  if (Mode)
    {
    // убрать сообщение из списка
    for (Index = Index + 1; Index < Baloi_DisabledReplyListCount; Index++) Baloi_DisabledReplyList[Index - 1] = Baloi_DisabledReplyList[Index];
    Baloi_DisabledReplyListCount--;
    }
  }
else
  {
  if (!Mode)
    {
    // добавить сообщение в список
    if (Baloi_DisabledReplyListCount >= BALOI_MAX_DISABLED_MSG_COUNT) return REPLY_CMD_OVERFLOW_ERROR;
    Baloi_DisabledReplyList[Baloi_DisabledReplyListCount++] = Msg;
    }
  }

return REPLY_CMD_DONE;
}
//================================================================================

int Irq13CrcIsDisabled (WORD Msg)
{
int Index, Found;

// ищем сообщение в списке
for (Index = 0, Found = 0; Index < Baloi_DisabledCrcListCount; Index++)
  {
  if (Baloi_DisabledCrcList[Index] == Msg) { Found = 1; break; }
  }

return Found;
}
//================================================================================

int Irq13ChangeDisabledCrcList (WORD Msg, WORD Mode)
{
int Index, Found;

// ищем сообщение в списке
for (Index = 0, Found = 0; Index < Baloi_DisabledCrcListCount; Index++)
  {
  if (Baloi_DisabledCrcList[Index] == Msg) { Found = 1; break; }
  }

if (Found)
  {
  if (Mode)
    {
    // убрать сообщение из списка
    for (Index = Index + 1; Index < Baloi_DisabledCrcListCount; Index++) Baloi_DisabledCrcList[Index - 1] = Baloi_DisabledCrcList[Index];
    Baloi_DisabledCrcListCount--;
    }
  }
else
  {
  if (!Mode)
    {
    // добавить сообщение в список
    if (Baloi_DisabledCrcListCount >= BALOI_MAX_DISABLED_MSG_COUNT) return REPLY_CMD_OVERFLOW_ERROR;
    Baloi_DisabledCrcList[Baloi_DisabledCrcListCount++] = Msg;
    }
  }

return REPLY_CMD_DONE;
}
//================================================================================

int Irq13ReplyTypeRedefined (WORD Msg, WORD *ReplyType)
{
int Index, Found;

// ищем сообщение в списке
for (Index = 0, Found = 0; Index < Baloi_ReplyTypeListCount; Index++)
  {
  if (Baloi_ReplyTypeList[Index][0] == Msg) { Found = 1; break; }
  }

if (Found) *ReplyType = Baloi_ReplyTypeList[Index][1];
return Found;
}
//================================================================================

int Irq13ChangeReplyTypeList (WORD Msg, WORD Mode, WORD ReplyType)
{
int Index, Found;

// ищем сообщение в списке
for (Index = 0, Found = 0; Index < Baloi_ReplyTypeListCount; Index++)
  {
  if (Baloi_ReplyTypeList[Index][0] == Msg) { Found = 1; break; }
  }

if (Found)
  {
  if (Mode)
    {
    // обновить режим
    Baloi_ReplyTypeList[Index][1] = ReplyType;
    }
  else
    {
    // убрать сообщение из списка
    for (Index = Index + 1; Index < Baloi_ReplyTypeListCount; Index++)
      {
      Baloi_ReplyTypeList[Index - 1][0] = Baloi_ReplyTypeList[Index][0];
      Baloi_ReplyTypeList[Index - 1][1] = Baloi_ReplyTypeList[Index][1];
      }
    Baloi_ReplyTypeListCount--;
    }
  }
else
  {
  if (Mode)
    {
    // добавить сообщение в список
    if (Baloi_ReplyTypeListCount >= BALOI_MAX_DISABLED_MSG_COUNT) return REPLY_CMD_OVERFLOW_ERROR;
    Baloi_ReplyTypeList[Baloi_ReplyTypeListCount][0] = Msg;
    Baloi_ReplyTypeList[Baloi_ReplyTypeListCount++][1] = ReplyType;
    }
  }

return REPLY_CMD_DONE;
}
//================================================================================

int Irq13ChangeCurrentTxRns (WORD Mode, WORD TxRns)
{
if (Mode)
  {
  if (!RedefinedTxRns)
    {
    if (TxRns == Irq13TxIndex) return REPLY_CMD_DATA_ERROR;
    RedefinedTxRns = 1;
    RedefinedTxRnsValue = Irq13TxIndex;
    Irq13TxIndex = TxRns;
    }
  else
    {
    if (TxRns == RedefinedTxRnsValue) return REPLY_CMD_DATA_ERROR;
    Irq13TxIndex = TxRns;
    }
  }
else
  {
  if (RedefinedTxRns)
    {
    RedefinedTxRns = 0;
    Irq13TxIndex = RedefinedTxRnsValue;
    }
  }

return REPLY_CMD_DONE;
}
//================================================================================

int Irq13DebugReadIsEnabled (WORD Msg)
{
int Index, Found;

// ищем сообщение в списке
for (Index = 0, Found = 0; Index < Baloi_DebugReadListCount; Index++)
  {
  if (Baloi_DebugReadList[Index] == Msg) { Found = 1; break; }
  }

return Found;
}
//================================================================================

int Irq13ChangeDebugReadList (WORD Msg, WORD Mode)
{
int Index, Found;

// ищем сообщение в списке
for (Index = 0, Found = 0; Index < Baloi_DebugReadListCount; Index++)
  {
  if (Baloi_DebugReadList[Index] == Msg) { Found = 1; break; }
  }

if (Found)
  {
  if (!Mode)
    {
    // убрать сообщение из списка
    for (Index = Index + 1; Index < Baloi_DebugReadListCount; Index++) Baloi_DebugReadList[Index - 1] = Baloi_DebugReadList[Index];
    Baloi_DebugReadListCount--;
    }
  }
else
  {
  if (Mode)
    {
    // добавить сообщение в список
    if (Baloi_DebugReadListCount >= BALOI_MAX_DISABLED_MSG_COUNT) return REPLY_CMD_OVERFLOW_ERROR;
    Baloi_DebugReadList[Baloi_DebugReadListCount++] = Msg;
    }
  }

return REPLY_CMD_DONE;
}
//================================================================================

void Irq13DoDebugReadBn (void)
{
int Status, Result, DevMkoLine, DevMkoChan, Chan, Index;
WORD Sum;

// параметры линий обмена
DevMkoLine = SpoConfig.DevMkoLine & 1;
DevMkoChan = SpoConfig.DevMkoChan & 1;

for (Chan = 0, Status = 0; (Chan < 2) && (!Status); Chan++)
  {
  for (Index = 0; (Index < 4) && (!Status); Index++)
    {
    Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(13, 1, MKO_RX, 31), 0);
    if (!Result)
      {
      Sum = Irq13CalcSum(&MkoBufB[0], 30);
      if (Sum == MkoBufB[0][30]) Status = 1;
      }
    }
  DevMkoChan ^= 1;
  }

if (!Status) Irq13BaloiFail = 1;
}
//================================================================================

void Irq13DoDebugReadAn (void)
{
int Status, Result, DevMkoLine, DevMkoChan, Chan, Index;
WORD Sum;

// параметры линий обмена
DevMkoLine = SpoConfig.DevMkoLine & 1;
DevMkoChan = SpoConfig.DevMkoChan & 1;

for (Chan = 0, Status = 0; (Chan < 2) && (!Status); Chan++)
  {
  for (Index = 0; (Index < 4) && (!Status); Index++)
    {
    Result = MkoXchgAn(DevMkoLine, DevMkoChan, MKO_CMD(13, 1, MKO_RX, 31), 0);
    if (!Result)
      {
      Sum = Irq13CalcSum(&MkoBufA[0], 30);
      if (Sum == MkoBufA[0][30]) Status = 1;
      }
    }
  DevMkoChan ^= 1;
  }

if (!Status) Irq13BaloiFail = 1;
}
//================================================================================

#define IRQ13_READ_DATA(MinIndex, MaxIndex, BlockSize) \
  if ((TableIndex < MinIndex) || (TableIndex > MaxIndex)) return REPLY_CMD_DATA_ERROR; \
  if (DataSize != BlockSize) return REPLY_CMD_DATA_ERROR; \
  Cmd = MKO_CMD(KPA_ADDR, KPA_SUBADDR_CMD + 1, MKO_RX, BlockSize / 2); \
  Result = ReadExtMsgData(BaseMkoLine, BaseMkoChan, Cmd, KpaCommand[7], (WORD*) &Buffer); \
  if (Result != REPLY_CMD_DONE) return Result;

#define IRQ13_ASSIGN_DATA(MinValue, MaxValue, Param) \
  Value = Buffer[0]; \
  if ((Value < MinValue) || (Value > MaxValue)) return REPLY_CMD_DATA_ERROR; \
  Param = Value; \
  return REPLY_CMD_DONE;

//================================================================================


int Irq13LoadTableData (int BaseMkoLine, int BaseMkoChan, int VarIndex, int TableIndex, int DataSize)
{
int Result, Value;
WORD Cmd, *Dst, Buffer[32];

if (VarIndex == 1)
  {
  IRQ13_READ_DATA(0, 0, 2);
  IRQ13_ASSIGN_DATA(1, 3, Komplekt_ALPS);
  }
else if (VarIndex == 2)
  {
  IRQ13_READ_DATA(0, 0, 2);
  IRQ13_ASSIGN_DATA(1, 3, Komplekt_MFS);
  }
else if (VarIndex == 3)
  {
  IRQ13_READ_DATA(0, 0, 2);
  IRQ13_ASSIGN_DATA(0, 3, BUFAR);
  }
else if (VarIndex == 4)
  {
  IRQ13_READ_DATA(0, 0, 2);
  IRQ13_ASSIGN_DATA(0, 1, BUFAR_Sbros);
  }
else if (VarIndex == 5)
  {
  IRQ13_READ_DATA(0, 0, 2);
  IRQ13_ASSIGN_DATA(0, 1, Rejim_3_Rasch);
  }
else if (VarIndex == 6)
  {
  IRQ13_READ_DATA(0, 0, 2);
  IRQ13_ASSIGN_DATA(1, 4, Nomer_Shetverti);
  }
else if (VarIndex == 7)
  {
  IRQ13_READ_DATA(0, 0, 2);
  IRQ13_ASSIGN_DATA(0, 1, Otkl_AFAR);
  }
else if (VarIndex == 8)
  {
  IRQ13_READ_DATA(0, 0, 4);
  if (Buffer[0] > 10000) return REPLY_CMD_DATA_ERROR;
  if (Buffer[1] > 10000) return REPLY_CMD_DATA_ERROR;
  Pause_MFS[0] = Buffer[0];
  Pause_MFS[1] = Buffer[1];
  return REPLY_CMD_DONE;
  }
else if (VarIndex == 9)
  {
  IRQ13_READ_DATA(0, 0, 6);
  if (Buffer[0] > 10000) return REPLY_CMD_DATA_ERROR;
  if (Buffer[1] > 10000) return REPLY_CMD_DATA_ERROR;
  if (Buffer[2] > 10000) return REPLY_CMD_DATA_ERROR;
  Pause_BUM[0] = Buffer[0];
  Pause_BUM[1] = Buffer[1];
  Pause_BUM[2] = Buffer[2];
  return REPLY_CMD_DONE;
  }
else if (VarIndex == 10)
  {
  IRQ13_READ_DATA(0, 0, 2);
  IRQ13_ASSIGN_DATA(0, 10000, Pause_PRD);
  }
else if (VarIndex == 11)
  {
  IRQ13_READ_DATA(0, 0, 2);
  IRQ13_ASSIGN_DATA(0, 10000, Pause_Rasch_3);
  }
else if (VarIndex == 12)
  {
  IRQ13_READ_DATA(0, 0, 2);
  IRQ13_ASSIGN_DATA(0, 10000, Pause_LPH);
  }
else if (VarIndex == 13)
  {
  IRQ13_READ_DATA(0, 0, 20);
  memmove(&Vkl_AFAR_BUM, &Buffer, sizeof(Vkl_AFAR_BUM));
  return REPLY_CMD_DONE;
  }
else if (VarIndex == 14)
  {
  IRQ13_READ_DATA(0, 0, 20);
  memmove(&Tm_AFAR_BUM, &Buffer, sizeof(Tm_AFAR_BUM));
  return REPLY_CMD_DONE;
  }
else if (VarIndex == 15)
  {
  IRQ13_READ_DATA(0, 19, 56);
  Dst = (WORD*) &Vkl_AFAR_PRD;
  memmove(&Dst[TableIndex * 28], &Buffer, sizeof(WORD) * 28);
  return REPLY_CMD_DONE;
  }
else if (VarIndex == 16)
  {
  IRQ13_READ_DATA(0, 4, 30);
  Dst = (WORD*) &Tm_AFAR_PRD;
  memmove(&Dst[TableIndex * 15], &Buffer, sizeof(WORD) * 15);
  return REPLY_CMD_DONE;
  }
else if (VarIndex == 17)
  {
  IRQ13_READ_DATA(0, 0, 2);
  IRQ13_ASSIGN_DATA(0, 1, Control_Tm_AFAR);
  }
else if (VarIndex == 18)
  {
  IRQ13_READ_DATA(0, 0, 2);
  IRQ13_ASSIGN_DATA(1, 1000, Chislo_Proverok_TM_AFAR);
  }
else if (VarIndex == 19)
  {
  IRQ13_READ_DATA(0, 0, 2);
  IRQ13_ASSIGN_DATA(0, 10000, Pause_Proverki_TM_AFAR);
  }
else if (VarIndex == 20)
  {
  IRQ13_READ_DATA(0, 0, 2);
  IRQ13_ASSIGN_DATA(0, 3000, Pause_OTKL_AFAR);
  }
else if (VarIndex == 21)
  {
  IRQ13_READ_DATA(0, 0, 16);
  memmove(&Data_OTKL_AFAR, &Buffer, sizeof(Data_OTKL_AFAR));
  return REPLY_CMD_DONE;
  }
else if (VarIndex == 22)
  {
  IRQ13_READ_DATA(0, 0, 2);
  IRQ13_ASSIGN_DATA(0, 1, Control_BUM);
  }
else if (VarIndex == 23)
  {
  IRQ13_READ_DATA(0, 0, 2);
  IRQ13_ASSIGN_DATA(0, 1, LPH);
  }
else if (VarIndex == 24)
  {
  IRQ13_READ_DATA(0, 0, 2);
  IRQ13_ASSIGN_DATA(1, 3, Rejim_BRTK);
  }
else if (VarIndex == 25)
  {
  IRQ13_READ_DATA(0, 0, 2);
  IRQ13_ASSIGN_DATA(0, 1, Irq13SendNackEnabled);
  }
else if (VarIndex == 26)
  {
  IRQ13_READ_DATA(0, 0, 2);
  IRQ13_ASSIGN_DATA(0, 1, Irq13CmdFifoEnabled);
  }
else if (VarIndex == 27)
  {
  IRQ13_READ_DATA(0, 0, 2);
  IRQ13_ASSIGN_DATA(0, 1000, Pause_MkoTx);
  }
else if (VarIndex == 31)
  {
  IRQ13_READ_DATA(0, 0, 4);
  if (Buffer[0] > 300) return REPLY_CMD_DATA_ERROR;
  if (Buffer[1] > 300) return REPLY_CMD_DATA_ERROR;
  Pause_AOS[0] = Buffer[0];
  Pause_AOS[1] = Buffer[1];
  return REPLY_CMD_DONE;
  }
else if (VarIndex == 32)
  {
  IRQ13_READ_DATA(0, 0, 2);
  IRQ13_ASSIGN_DATA(0, 1, Otkl_AT);
  }
else if (VarIndex == 33)
  {
  IRQ13_READ_DATA(0, 0, 2);
  Code_DB = Buffer[0];
  return REPLY_CMD_DONE;
  }
else if (VarIndex == 34)
  {
  IRQ13_READ_DATA(0, 25, 64);
  Dst = (WORD*) &KPI_Etalon;
  memmove(&Dst[TableIndex * 32], &Buffer, sizeof(WORD) * 32);
  return REPLY_CMD_DONE;
  }
else if (VarIndex == 35)
  {
  IRQ13_READ_DATA(0, 0, 2);
  IRQ13_ASSIGN_DATA(0, 810, KPI_Etalon_Len);
  }
else if (VarIndex == 36)
  {
  IRQ13_READ_DATA(0, 0, 4);
  KPI_Kvit[0] = Buffer[0];
  KPI_Kvit[1] = Buffer[1];
  return REPLY_CMD_DONE;
  }
else if (VarIndex == 38)
  {
  IRQ13_READ_DATA(0, 31, 64);
  Dst = (WORD*) &Irq13UserMsgData;
  memmove(&Dst[TableIndex * 32], &Buffer, sizeof(WORD) * 32);
  return REPLY_CMD_DONE;
  }
else if (VarIndex == 39)
  {
  IRQ13_READ_DATA(0, 0, 2);
  IRQ13_ASSIGN_DATA(0, 10000, Pause_AFAR);
  }
else if (VarIndex == 40)
  {
  IRQ13_READ_DATA(0, 0, 2);
  IRQ13_ASSIGN_DATA(0, 4, Index_AFAR);
  }
else if (VarIndex == 41)
  {
  IRQ13_READ_DATA(0, 0, 2);
  IRQ13_ASSIGN_DATA(0, 1, Otkl_BPA_AT);
  }
else if (VarIndex == 42)
  {
  IRQ13_READ_DATA(0, 0, 2);
  IRQ13_ASSIGN_DATA(0, 1, AFAR_AT_M);
  }
else if (VarIndex == 43)
  {
  IRQ13_READ_DATA(0, 0, 2);
  IRQ13_ASSIGN_DATA(0, 1, Control_ZTM_BUFAR);
  }
else if (VarIndex == 44)
  {
  IRQ13_READ_DATA(0, 0, 2);
  IRQ13_ASSIGN_DATA(0, 1, Otriz_Kvit);
  }
else
  {
  return REPLY_CMD_DATA_ERROR;
  }
}
//================================================================================

int MsgProc0xDB3AExt (void)
{
int Result;

Result = Vkluchenie_BPA_AT(0, Pause_MFS[0]);
if (Result) return Result;

Result = Vkluchenie_LPH();
if (Result) return Result;

Result = Control_LPH(1);
if (Result) return Result;

Result = Vkluchenie_AFAR();
if (Result) return Result;

Result = Control_AFAR();
if (Result) return Result;

return 0;
}
//================================================================================

int Vkluchenie_LPH (void)
{
int Result;
int DevMkoLine, DevMkoChan;
WORD Msg;

// параметры линий обмена
DevMkoLine = SpoConfig.DevMkoLine & 1;
DevMkoChan = SpoConfig.DevMkoChan & 1;

MkoBufB[0][0] = K_BRTK_A(1);
Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(12, 17, MKO_TX, 1), 0);
if (Result) return Result;

SleepMSec(40);

Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(12, 16, MKO_RX, 1), 0);
if (Result) return Result;

if (MkoBufB[0][0] != 0)
  {
  // сообщение для КПА "Ошибка ЗОБ при включении ЛПЧ"
  Irq13Error(42, 0, 0);

  SAA_Ustanovlena = 0;

  if (Otkl_AT)
    {
    Result = Otkluchenie_AT();
    if (Result) Irq13Error(41, 3, 0);
    }

  // сообщение для БАЛОИ: "САА не установлена" / "отказ раскачки УТ"
  Msg = (LPH) ? (0x2BAA) : (0x33D2);
  Irq13InitMsg(Msg, 0);
  Irq13SendMsgBnOverFifo();
  return 1;
  }

if (Pause_LPH) SleepMSec(Pause_LPH);
return 0;
}
//================================================================================

int Control_LPH (int MsgType)
{
int Result;
int DevMkoLine, DevMkoChan;
WORD Msg, ZTM1, ZTM2;

// параметры линий обмена
DevMkoLine = SpoConfig.DevMkoLine & 1;
DevMkoChan = SpoConfig.DevMkoChan & 1;

Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(12, 17, MKO_RX, 14), 0);
if (Result) return Result;

ZTM1 = (0x10020 >> Komplekt_ALPS) | (0x2000 >> Komplekt_MFS) | (0x03E1);
ZTM2 = (0x0200 >> BUFAR) | (0xFE00);

if (!(((MkoBufB[0][0] & 0xFFFD) == ZTM1) && ((MkoBufB[0][1] & 0xFFC0) == ZTM2)))
  {
  // сообщение для КПА "Ошибка ЗОБ при включении ЛПЧ"
  Irq13Error(42, 0, 0);

  SAA_Ustanovlena = 0;

  if (Otkl_AT)
    {
    Result = Otkluchenie_AT();
    if (Result) Irq13Error(41, 4, 0);
    }

  // сообщение для БАЛОИ: "САА не установлена" / "отказ раскачки УТ"
  Msg = (MsgType) ? (0x2BAA) : (0x33D2);
  Irq13InitMsg(Msg, 0);
  Irq13SendMsgBnOverFifo();

  return 1;
  }

return 0;
}
//================================================================================

int K_BRTK_A (int Vkl_LPH)
{
WORD Data;
Data = (0x10000 >> Komplekt_ALPS) | (0x2000 >> Komplekt_MFS);

if (BUFAR) Data |= (0x80 >> BUFAR);
if (Vkl_LPH) Data |= (0x400 >> Komplekt_ALPS);

return Data;
}
//================================================================================

int ReadExtMsgData (int BaseMkoLine, int BaseMkoChan, WORD Cmd, WORD DataCrc, WORD *Buffer)
{
int Result;
WORD Crc16;

// считываем данные
Result = MkoXchgBn(BaseMkoLine, BaseMkoChan, Cmd, 0);
if (Result) return DECODE_XCHG_ERROR(Result);

// проверяем размер принятых данных
if (MkoMsgResB[0].Len != MKO_CMD_SIZE(Cmd)) return REPLY_NO_DATA_ERROR;

// проверяем целостность данных
Crc16 = CalcCrc16(&MkoBufB[0], MKO_CMD_SIZE(Cmd) * sizeof(WORD));
if (Crc16 != DataCrc) return REPLY_DATA_CRC_ERROR;

// сохраняем данные
memcpy(Buffer, &MkoBufB[0], MKO_CMD_SIZE(Cmd) * sizeof(WORD));

return REPLY_CMD_DONE;
}
//================================================================================

// задача для периодической выдачи пакетов по МКО

//__asm__(".align 4");
void BaloiPeriodicTask (int AParam)
{
int Result;
STATUS Status;
int PackIndex, MsgIndex, PrepPause, MsgCount, MkoLine, MkoChan;
mkoPackMsg_t *MkoMsg;
u16 MkoCmd;

// проверяем индекс задачи = индекс пакета
PackIndex = AParam;
if ((PackIndex >= 0) && (PackIndex <= 31))
  {
  // захват семафора для передачи пакета
  Status = semLock(Baloi_MkoPackSem, -1);
  if (Status == OK)
    {
    // результат по умолчанию = ошибка
    Result = 1;

    // число сообщений в пакете
    MsgCount = Baloi_PackMsgCount[PackIndex];
    if ((MsgCount >= 1) && (MsgCount <= 32))
      {
      // проверяем, нужна ли пауза при начальном запуске
      PrepPause = Baloi_PackPrepPause[PackIndex];
      if (PrepPause)
        {
        taskDelay(PrepPause);
        Baloi_PackPrepPause[PackIndex] = 0;
        }

      // настраиваем пакет
      Baloi_MkoPack.nFirst = 0;                 // номер первого сообщения
      Baloi_MkoPack.nLast  = MsgCount - 1;      // номер последнего сообщения
      Baloi_MkoPack.nPage  = 5;                 // номер страницы БОЗУ
      Baloi_MkoPack.DescPackRes = (mkoPackRes_t*) &Baloi_MkoPackRes; // указатель на описатель результата

      // параметры линии МКО
      MkoLine = Baloi_PackMkoLine[PackIndex] & 1;
      MkoChan = Baloi_PackMkoChan[PackIndex] & 1;

      // настраиваем сообщения
      for (MsgIndex = 0; MsgIndex < MsgCount; MsgIndex++)
        {
        MkoCmd = Baloi_PackList[PackIndex][MsgIndex][0];
        MkoMsg = &Baloi_MkoPackMsg[MsgIndex];

        MkoMsg->DescPack   = &Baloi_MkoPack;
        MkoMsg->nMsg       = MsgIndex;
        MkoMsg->TM         = (!MKO_CMD_DIR(MkoCmd)) ? (mcTM_BCtoRT) : (mcTM_RTtoBC);
        MkoMsg->Chan       = MkoChan;

        MkoMsg->Eom        = 0;
        MkoMsg->MissSW     = 1;
        MkoMsg->FlagSW     = 1;
        MkoMsg->FmtError   = 1;

        MkoMsg->CW         = MkoCmd;
        MkoMsg->CW2        = 0;
        MkoMsg->GapTime    = 0;

        MkoMsg->adrData    = (WORD*) &Baloi_PackList[PackIndex][MsgIndex][1];
        MkoMsg->DescMsgRes = &Baloi_MkoPackMsgRes[MsgIndex];

        // настройка списка указателей на сообщения
        Baloi_MkoPackMsgList[MsgIndex] = MkoMsg;
        }

      // передаем пакет
      do
        {
        Status = mkoPackIniStart(MkoLine, (mkoPack_t*) &Baloi_MkoPack, (mkoPackMsg_t**) &Baloi_MkoPackMsgList, 0);
        if (Status != OK) { break; }

        Status = mkoNumPackWait(MkoLine, (mkoPack_t*) &Baloi_MkoPack);
        if (Status != OK) { break; }

        // проверка результата обмена
        Result = CheckMkoXchg(&Baloi_MkoPackRes, &Baloi_MkoPackMsg[0], &Baloi_MkoPackMsgRes[0], MsgCount);
        if (Result) break;

        // обмен успешно завершен
        Result = 0;
        }
      while (0);
      }

    // обновляем счетчики статистики
    Baloi_PackSendCount[PackIndex]++;
    if (!Result) { Baloi_PackOkSendCount[PackIndex]++; } else { Baloi_PackErrSendCount[PackIndex]++; }

    // отпускаем семафор
    semUnlock(Baloi_MkoPackSem);
    }
  }

taskExit(0);
}
//================================================================================

// код операции = 0x1C, сброс данных пакетной передачи
// [1][15..0] = код операции
//   0x0000 = очистить все пакеты
//     если пакеты запущены, они останавливается
//     удаляются все сообщения
//     удаляется вся статистика
//   0x0001 = очистить указанный пакет
//     если пакет запущен, он останавливается
//     удаляются все сообщения из пакета
//     удаляется вся статистика по пакету
//   0x0002 = только сброс статистики пакета
//     состояние пакета при сбросе не важно
// [2][7..0] = номер пакета, 0..31 (если требуется)

int Irq13PackReset (int BaseMkoLine, int BaseMkoChan)
{
int Result;
int Mode, TaskIndex;
STATUS Status;
UINT TaskId;

// всегда ошибка если нет семафора
if (!Baloi_MkoPackSem) { return REPLY_INTERNAL_DATA_ERROR; }

Result = 1;
Mode = KpaCommand[1];

if (Mode == 0) // очистить все пакеты
  {
  // захват семафора
  Status = semLock(Baloi_MkoPackSem, -1);
  if (Status == OK)
    {
    // удаляем все пакеты с передачи
    Result = 0;
    for (TaskIndex = 0; TaskIndex < 32; TaskIndex++)
      {
      TaskId = taskVerify(TaskIndex + 5);
      if (TaskId)
        {
        Status = taskDelete(TaskId);
        if (Status != OK) { Result = 1; break; }
        }
      }

    // удаляем все сообщения и статистику
    if (!Result)
      {
      memset(&Baloi_PackMsgCount, 0, sizeof(Baloi_PackMsgCount));
      memset(&Baloi_PackSendCount, 0, sizeof(Baloi_PackSendCount));
      memset(&Baloi_PackOkSendCount, 0, sizeof(Baloi_PackOkSendCount));
      memset(&Baloi_PackErrSendCount, 0, sizeof(Baloi_PackErrSendCount));
      }

    // отпускаем семафор
    semUnlock(Baloi_MkoPackSem);
    }

  Result = (!Result) ? (REPLY_CMD_DONE) : (REPLY_CMD_ERROR);
  return Result;
  }

if (Mode == 1) // очистить указанный пакет
  {
  // номер пакета для очистки
  TaskIndex = KpaCommand[2] & 0xFF;
  if ((TaskIndex < 0) || (TaskIndex > 31)) { return REPLY_CMD_DATA_ERROR; }

  // захват семафора
  Status = semLock(Baloi_MkoPackSem, -1);
  if (Status == OK)
    {
    // удаляем указанный пакет с передачи
    Result = 0;
    TaskId = taskVerify(TaskIndex + 5);
    if (TaskId)
      {
      Status = taskDelete(TaskId);
      if (Status != OK) { Result = 1; }
      }

    // удаляем сообщения и статистику
    if (!Result)
      {
      Baloi_PackMsgCount[TaskIndex] = 0;
      Baloi_PackSendCount[TaskIndex] = 0;
      Baloi_PackOkSendCount[TaskIndex] = 0;
      Baloi_PackErrSendCount[TaskIndex] = 0;
      }

    // отпускаем семафор
    semUnlock(Baloi_MkoPackSem);
    }

  Result = (!Result) ? (REPLY_CMD_DONE) : (REPLY_CMD_ERROR);
  return Result;
  }

if (Mode == 2) // только сброс статистики пакета
  {
  // номер пакета для сброса статистики
  TaskIndex = KpaCommand[2] & 0xFF;
  if ((TaskIndex < 0) || (TaskIndex > 31)) { return REPLY_CMD_DATA_ERROR; }

  // захват семафора
  Status = semLock(Baloi_MkoPackSem, -1);
  if (Status == OK)
    {
    // сбрасываем статистику для указанного пакета
    Result = 0;
    Baloi_PackSendCount[TaskIndex] = 0;
    Baloi_PackOkSendCount[TaskIndex] = 0;
    Baloi_PackErrSendCount[TaskIndex] = 0;

    // отпускаем семафор
    semUnlock(Baloi_MkoPackSem);
    }

  Result = (!Result) ? (REPLY_CMD_DONE) : (REPLY_CMD_ERROR);
  return Result;
  }

return REPLY_CMD_DATA_ERROR;
}
//================================================================================

// код операции = 0x1D, настройка данных пакетной передачи
// [1][15..0] = код операции
//   0x0000 = добавить сообщение в пакет
//     если пакет запущен = ошибка
//   0x0001 = добавить начальную паузу для пакета
//     если пакет запущен = ошибка
//   0x0002 = добавить интервал для пакета
//     если пакет запущен = ошибка
//   0x0003 = добавить линию/канал МКО для пакета
//     если пакет запущен = ошибка
// если добавляется сообщение в пакет (код опер. 0x0000):
//   [2][7..0]  = номер пакета, 0..31
//   [2][15..8] = флаг режима (0 – запись, 1 – чтение)
//   [3][7..0]  = адрес, от 0 до 30
//   [3][15..8] = подадрес, от 1 до 30
//   [4][7..0]  = число слов данных, 1..32
//   если режим сообщения = передача, считываются данные с п/а 2 для передачи
// если добавляется начальная пауза для пакета (код опер. 0x0001):
//   [2][7..0]  = номер пакета, 0..31
//   [3][15..0] = начальная однократная пауза для пакета, мсек, 10..65535
// если добавляется интервал для пакета (код опер. 0x0002):
//   [2][7..0]  = номер пакета, 0..31
//   [3][15..0] = интервал для пакета, мсек, 500..60000
// если добавляется линия/канал МКО для пакета (код опер. 0x0003):
//   [2][7..0]  = номер пакета, 0..31
//   [3][7..0]  = линия МКО для пакета, 0..1
//   [3][15..8] = канал МКО для пакета, 0..1

int Irq13PackSetup (int BaseMkoLine, int BaseMkoChan)
{
int Result;
int Mode, TaskIndex, Pause, MsgCount, MkoDir, MkoAddr, MkoSub, MkoSize;
STATUS Status;
UINT TaskId;

if (!Baloi_MkoPackSem) { return REPLY_INTERNAL_DATA_ERROR; }

Result = 1;
Mode = KpaCommand[1];

if (Mode == 0) // добавить сообщение в пакет
  {
  // номер пакета для добавления сообщения
  TaskIndex = KpaCommand[2] & 0xFF;
  if ((TaskIndex < 0) || (TaskIndex > 31)) { return REPLY_CMD_DATA_ERROR; }

  // захват семафора
  Status = semLock(Baloi_MkoPackSem, -1);
  if (Status == OK)
    {
    // ищем указанный пакет
    TaskId = taskVerify(TaskIndex + 5);
    if (!TaskId) { Result = 0; }

    // проверяем число существующих сообщений
    if (!Result)
      {
      MsgCount = Baloi_PackMsgCount[TaskIndex];
      if (MsgCount >= 32) { Result = 1; }
      }

    // проверяем параметры сообщения
    if (!Result)
      {
      MkoDir  = KpaCommand[2] >> 8;
      MkoAddr = KpaCommand[3] & 0xFF;
      MkoSub  = KpaCommand[3] >> 8;
      MkoSize = KpaCommand[4] & 0xFF;

      MkoDir = MkoDir & 1;
      if ((MkoAddr < 0) || (MkoAddr > 30)) { Result = 1; }
      if ((MkoSub  < 1) || (MkoSub  > 30)) { Result = 1; }
      if ((MkoSize < 1) || (MkoSize > 32)) { Result = 1; }
      }

    // считываем данные для передачи
    if ((!Result) && (!MkoDir))
      {
      Result = MkoXchgBn(BaseMkoLine, BaseMkoChan, MKO_CMD(KPA_ADDR, KPA_SUBADDR_CMD + 1, MKO_RX, MkoSize), 0);
      if (Result) { Result = 1; } else { memmove(&Baloi_PackList[TaskIndex][MsgCount][1], &MkoBufB[0][0], sizeof(u16) * MkoSize); }
      }

    // добавляем сообщение для пакета
    if (!Result)
      {
      Baloi_PackList[TaskIndex][MsgCount][0] = MKO_CMD(MkoAddr, MkoSub, MkoDir, MkoSize);
      Baloi_PackMsgCount[TaskIndex]++;
      }

    // отпускаем семафор
    semUnlock(Baloi_MkoPackSem);
    }

  Result = (!Result) ? (REPLY_CMD_DONE) : (REPLY_CMD_ERROR);
  return Result;
  }

if (Mode == 1) // добавить начальную паузу для пакета
  {
  // номер пакета для настройки
  TaskIndex = KpaCommand[2] & 0xFF;
  if ((TaskIndex < 0) || (TaskIndex > 31)) { return REPLY_CMD_DATA_ERROR; }

  // захват семафора
  Status = semLock(Baloi_MkoPackSem, -1);
  if (Status == OK)
    {
    // ищем указанный пакет
    TaskId = taskVerify(TaskIndex + 5);
    if (!TaskId) { Result = 0; }

    // проверяем диапазон начальной паузы
    if (!Result)
      {
      Pause = KpaCommand[3];
      if (Pause < 10) { Result = 1; }
      }

    // добавляем паузу для пакета
    if (!Result) Baloi_PackPrepPause[TaskIndex] = Pause;

    // отпускаем семафор
    semUnlock(Baloi_MkoPackSem);
    }

  Result = (!Result) ? (REPLY_CMD_DONE) : (REPLY_CMD_ERROR);
  return Result;
  }

if (Mode == 2) // добавить интервал для пакета
  {
  // номер пакета для настройки
  TaskIndex = KpaCommand[2] & 0xFF;
  if ((TaskIndex < 0) || (TaskIndex > 31)) { return REPLY_CMD_DATA_ERROR; }

  // захват семафора
  Status = semLock(Baloi_MkoPackSem, -1);
  if (Status == OK)
    {
    // ищем указанный пакет
    TaskId = taskVerify(TaskIndex + 5);
    if (!TaskId) { Result = 0; }

    // проверяем диапазон интервала
    if (!Result)
      {
      Pause = KpaCommand[3];
      if ((Pause < 500) || (Pause > 60000)) { Result = 1; }
      }

    // добавляем интервал для пакета
    if (!Result) Baloi_PackInterval[TaskIndex] = Pause;

    // отпускаем семафор
    semUnlock(Baloi_MkoPackSem);
    }

  Result = (!Result) ? (REPLY_CMD_DONE) : (REPLY_CMD_ERROR);
  return Result;
  }

if (Mode == 3) // добавить линию/канал МКО для пакета
  {
  // номер пакета для настройки
  TaskIndex = KpaCommand[2] & 0xFF;
  if ((TaskIndex < 0) || (TaskIndex > 31)) { return REPLY_CMD_DATA_ERROR; }

  // захват семафора
  Status = semLock(Baloi_MkoPackSem, -1);
  if (Status == OK)
    {
    // ищем указанный пакет
    TaskId = taskVerify(TaskIndex + 5);
    if (!TaskId) { Result = 0; }

    // добавляем линию/канал МКО для пакета
    if (!Result)
      {
      Baloi_PackMkoLine[TaskIndex] = (KpaCommand[3] & 0x00FF) ? (1) : (0);
      Baloi_PackMkoChan[TaskIndex] = (KpaCommand[3] & 0xFF00) ? (1) : (0);
      }

    // отпускаем семафор
    semUnlock(Baloi_MkoPackSem);
    }

  Result = (!Result) ? (REPLY_CMD_DONE) : (REPLY_CMD_ERROR);
  return Result;
  }

return REPLY_CMD_DATA_ERROR;
}
//================================================================================

// код операции = 0x1E, запуск/останов пакетной передачи
// [1][15..0] = код операции
//   0x0000 = запуск всех непустых пакетов
//     если пакет уже запущен, ничего не делается
//     если нет пакетов для запуска, ничего не делается
//   0x0001 = запуск указанного пакета
//     если пакет пуст = ошибка
//     если пакет уже запущен, ничего не делается
//   0x0002 = стоп всех пакетов
//     если нет запущеных пакетов, ничего не делается
//   0x0003 = стоп указанного пакета
//     если пакет не запущен, ничего не делается
// [2][7..0] = номер пакета, 0..31 (если требуется)

int Irq13PackRun (int BaseMkoLine, int BaseMkoChan)
{
int Result;
int Mode, MsgCount, Interval, TaskIndex;
STATUS Status;
UINT TaskId;

if (!Baloi_MkoPackSem) { return REPLY_INTERNAL_DATA_ERROR; }

Result = 1;
Mode = KpaCommand[1];

if (Mode == 0) // запуск всех непустых пакетов
  {
  // захват семафора
  Status = semLock(Baloi_MkoPackSem, -1);
  if (Status == OK)
    {
    // проверяем параметры всех задач
    Result = 0;
    for (TaskIndex = 0; TaskIndex < 32; TaskIndex++)
      {
      MsgCount = Baloi_PackMsgCount[TaskIndex];
      Interval = Baloi_PackInterval[TaskIndex];
      if ((MsgCount) && (!Interval)) { Result = 1; break; }
      }

    // запускаем задачи для пакетов
    if (!Result)
      {
      for (TaskIndex = 0; TaskIndex < 32; TaskIndex++)
        {
        MsgCount = Baloi_PackMsgCount[TaskIndex];
        if (MsgCount)
          {
          TaskId = taskVerify(TaskIndex + 5);
          if (!TaskId)
            {
            TaskId = taskPeriod(TaskIndex + 5, TaskIndex, Baloi_PackInterval[TaskIndex]);
            if (!TaskId) { Result = 1; break; }
            }
          }
        }
      }

    // отпускаем семафор
    semUnlock(Baloi_MkoPackSem);
    }

  Result = (!Result) ? (REPLY_CMD_DONE) : (REPLY_CMD_ERROR);
  return Result;
  }

if (Mode == 1) // запуск указанного пакета
  {
  // номер пакета для запуска
  TaskIndex = KpaCommand[2] & 0xFF;
  if ((TaskIndex < 0) || (TaskIndex > 31)) { return REPLY_CMD_DATA_ERROR; }

  // захват семафора
  Status = semLock(Baloi_MkoPackSem, -1);
  if (Status == OK)
    {
    // проверяем параметры задачи
    Result = 0;
    MsgCount = Baloi_PackMsgCount[TaskIndex];
    Interval = Baloi_PackInterval[TaskIndex];
    if ((!MsgCount) || (!Interval)) { Result = 1; }

    // запускаем задачу
    if (!Result)
      {
      TaskId = taskVerify(TaskIndex + 5);
      if (!TaskId)
        {
        TaskId = taskPeriod(TaskIndex + 5, TaskIndex, Baloi_PackInterval[TaskIndex]);
        if (!TaskId) { Result = 1; }
        }
      }

    // отпускаем семафор
    semUnlock(Baloi_MkoPackSem);
    }

  Result = (!Result) ? (REPLY_CMD_DONE) : (REPLY_CMD_ERROR);
  return Result;
  }

if (Mode == 2) // стоп всех пакетов
  {
  // захват семафора
  Status = semLock(Baloi_MkoPackSem, -1);
  if (Status == OK)
    {
    // удаляем все пакеты с передачи
    Result = 0;
    for (TaskIndex = 0; TaskIndex < 32; TaskIndex++)
      {
      TaskId = taskVerify(TaskIndex + 5);
      if (TaskId)
        {
        Status = taskDelete(TaskId);
        if (Status != OK) { Result = 1; break; }
        }
      }

    // отпускаем семафор
    semUnlock(Baloi_MkoPackSem);
    }

  Result = (!Result) ? (REPLY_CMD_DONE) : (REPLY_CMD_ERROR);
  return Result;
  }

if (Mode == 3) // стоп указанного пакета
  {
  // номер пакета для останова
  TaskIndex = KpaCommand[2] & 0xFF;
  if ((TaskIndex < 0) || (TaskIndex > 31)) { return REPLY_CMD_DATA_ERROR; }

  // захват семафора
  Status = semLock(Baloi_MkoPackSem, -1);
  if (Status == OK)
    {
    // удаляем пакет с передачи
    Result = 0;
    TaskId = taskVerify(TaskIndex + 5);
    if (TaskId)
      {
      Status = taskDelete(TaskId);
      if (Status != OK) { Result = 1; }
      }

    // отпускаем семафор
    semUnlock(Baloi_MkoPackSem);
    }

  Result = (!Result) ? (REPLY_CMD_DONE) : (REPLY_CMD_ERROR);
  return Result;
  }

return REPLY_CMD_DATA_ERROR;
}
//================================================================================

// код операции = 0x1F, чтение данных пакетной передачи
// [1][15..0] = код операции
//   0x0000 = чтение статистики для пакета
// [2][7..0] = номер пакета, 0..31 (если требуется)

int Irq13PackReadInfo (int BaseMkoLine, int BaseMkoChan)
{
int Result;
int Mode, TaskIndex;
STATUS Status;
UINT TaskId;
u16 Buffer[8];

if (!Baloi_MkoPackSem)
  {
  Result = REPLY_INTERNAL_DATA_ERROR;
  ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, Result, KPA_SUBADDR_IRQ);
  return 0;
  }

Result = 1;
Mode = KpaCommand[1];

if (Mode == 0) // чтение статистики для пакета
  {
  // номер пакета для считывания
  TaskIndex = KpaCommand[2] & 0xFF;
  if ((TaskIndex < 0) || (TaskIndex > 31))
    {
    Result = REPLY_CMD_DATA_ERROR;
    ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, Result, KPA_SUBADDR_IRQ);
    return 0;
    }

  // захват семафора
  Status = semLock(Baloi_MkoPackSem, -1);
  if (Status == OK)
    {
    // проверяем состояние пакета
    Result = 0;
    TaskId = taskVerify(TaskIndex + 5);

    Buffer[0] = (TaskId) ? (1) : (0);
    memmove(&Buffer[1], &Baloi_PackSendCount[TaskIndex], sizeof(u32));
    memmove(&Buffer[3], &Baloi_PackOkSendCount[TaskIndex], sizeof(u32));
    memmove(&Buffer[5], &Baloi_PackErrSendCount[TaskIndex], sizeof(u32));

    // отпускаем семафор
    semUnlock(Baloi_MkoPackSem);
    }

  // передаем результат
  if (!Result)
    {
    Result = REPLY_CMD_DONE;
    ReplyToKpaWithData(BaseMkoLine, BaseMkoChan, Result, KPA_SUBADDR_IRQ, &Buffer, sizeof(u16) * 7);
    }
  else
    {
    Result = REPLY_CMD_ERROR;
    ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, Result, KPA_SUBADDR_IRQ);
    }
  return 0;
  }

Result = REPLY_CMD_DATA_ERROR;
ReplyToKpaNoData(BaseMkoLine, BaseMkoChan, Result, KPA_SUBADDR_IRQ);
return 0;
}
//================================================================================

int Control_BUFAR (void)
{
int Result, D0, D1;
char DevMkoLine, DevMkoChan;

// параметры линий обмена
DevMkoLine = SpoConfig.DevMkoLine & 1;
DevMkoChan = SpoConfig.DevMkoChan & 1;

Result = MkoXchgBn(DevMkoLine, DevMkoChan, MKO_CMD(18, 2, MKO_RX, 15), 0);
if (Result)
  {
  return 1;
  }
else
  {
  D0 = MkoBufB[0][0];
  D1 = MkoBufB[0][1] & 0x0FFF;
  if ((D0 == Tm_AFAR_PRD[0][0]) && (D1 == Tm_AFAR_BUM[0][1]))
    {
    return 0;
    }
  else
    {
    return 1;
    }
  }
}
//================================================================================

#endif

/*

нет контроля обмена в "отключение_АТ" ?
что за функция "операция 0962" ?
опечатка БПАТ АТ ?

Result = Vkluchenie_BPA_AT(0, Pause_MFS[0]);
Irq13Error(21, 7, Index); // last = 7

*/



