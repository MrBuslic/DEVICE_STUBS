#ifndef _DEFINES
#define _DEFINES

#define Ok      5
#define ZeroKey 4
#define Error   2

// АДРЕСА
#define Addr1_SI   0x050000
#define Addr2_SI   Addr1_SI+0x1000
#define Addr1_WORK Addr1_SI+0x80
#define Addr2_WORK Addr2_SI+0x80
#define inAdr      0x460000  // адрес входной информации

// ВИДЫ КОМАНДЫ (БИТЫ 89..81)
#define KPI1_U  0x148 // 1 0100 1000
#define KPI1_W  0x09c // 0 1001 1100
#define RKN1    0x136 // 1 0011 0110
#define RKN2    0x0d7 // 0 1101 0111
#define KPI2_IS 0x0b1 // 0 1011 0001
#define KPI2_KS 0x12d // 1 0010 1101

// ВРЕМЕНА
#define TimeMinZpr    700  // минимальное  время между ЗПР (1.4 мс)
#define TimeMaxZpr   1300  // максимальное время между ЗПР (2.6 мс)
#define TimeOutZPR     60  // максимальное время ожидания ЗПР (в у.е.)

// Время отсрочки запуска ветки "РЕАНИМАЦИЯ" (измеряется в количестве
// тиков таймера TIMER2, что происходит каждые 2 мкС)
// Скорректировано на 1 минуту вниз 
//(время между запросами вектора состояния ЛКА-06)
#define TK__020_HI                 1
#define TK__020_LO          10000000L // ~   20 секунд

#define TK____6_HI                 1
#define TK____6_LO         150000000L // ~    6 минут

#define TK___12_HI                 1
#define TK___12_LO         330000000L // ~   12 минут

#define TK___18_HI                 1 
#define TK___18_LO         510000000L // ~   18 минут

#define TK___24_HI                 1
#define TK___24_LO         690000000L // ~   24 минут

#define TK___30_HI                 1 
#define TK___30_LO         870000000L // ~   30 минут

#define TK___36_HI                 1 
#define TK___36_LO        1050000000L // ~   36 минут

#define TK___60_HI                 1 
#define TK___60_LO        1770000000L // ~   60 минут

#define TK__120_HI                 1 
#define TK__120_LO        3570000000L // ~  120 минут

#define TK__180_HI                 2 
#define TK__180_LO        1075032704L // ~  180 минут

#define TK__360_HI                 3 
#define TK__360_LO        2180065408L // ~  360 минут

#define TK__720_HI                 6 
#define TK__720_LO          95163520L // ~  720 минут

#define TK_1440_HI                11 
#define TK_1440_LO         220327040L // ~ 1440 минут

#define TAU_CBK              2500000L // Периодичность контроля обменов с ЦБК ~ 5 секунд

#endif
