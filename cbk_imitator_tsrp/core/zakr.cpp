//#include "stdafx.h"   
#include "pkm.h"   
//#include "LDBG.h"   
extern QMutex fbreak;
//extern CLdbgApp theApp;
extern QString DefaultDir,SystemDir,StartBin;
extern void prmdier(QString);
//----------------------------------------------------
//	
//	( Zakrutka )
//      void prozessor(void) 
//	author: Klyamko E.I.
//                    ФАЙЛ    ZAKR.CPP   -   ЦБК
//                 С новым ПВР и  ММО 
//                  *******************************
//-----------------------------------------------------

extern int ekran[5];
int buza;
      //============== Закрутка ========================

   //-------  Шаблон карты процесссора -----------:

   typedef struct  
		      //------------ ВХОДЫ:
   { short kom;    //команда интерпретатору: 1-шаг, 2-Reset.
		   //далее идут сигналы внешних прерываний:
		       //------------ВЫХОДЫ:
     //------- Общесистемный статус:
     short jadro;     //я в ядре (=0)
     short halt;      //я в halt (=1)
     short codiskl;   //код исключит. ситуации по ExcCode в ЦП.
		      //нет ее - 50.
     short codisklcp1;//а в CP1: неточ.операция - 21, потеря
		      //значимости - 22, переполнение -23, деление
		      //на ноль - 24, некорректная операция - 25,
		      //нереализованная операция - 26, нет ее -0.
     //------------Регистры ЦП:
     UINT zp[32];
     UINT hi;
     UINT lo;
     UINT pc;
     //------------Регистры CP0:
     UINT cause;
     UINT status;
     UINT config;
     UINT epc;
     UINT badvaddr;
	 UINT entryhi;
     UINT entrylo;
	 UINT index;
	 UINT context;
	 UINT random;
	 UINT prid;
     //------------Регистры CP1:
     UINT fgr[32];
     UINT contrlstatus;
     UINT implrevision;
     //Характеристическая память:
     short predper;     //Признак перех. в предшеств.kom.
     short uslsop1;     //Выставляет линию условия CP1.
     short zapusl; //Для запомин. его в усл.переходе - для CP1.
	 UINT  adrper;
     //----------------------------------------
     UINT *mp;     //Для записи модели памяти.
	 char *mdp;    //Для дополнения этой модели.
	 UINT *rw;      //Регистры внешних устройств.
     DWORDLONG kespk[4096]; // Для КЕШ-памяти.
     DWORDLONG kespd[2048];
	 UINT zapcp1;        //Будет позиц. номер запроса от CP1
	 UINT sledk[5];
	 UINT sleds[7];
	 UINT sledo[5];
	 UINT mtakt;        //Время малого такта.
     DWORDLONG reg[33]; //Время готовности рез-та на рег. CP1.
     DWORDLONG tzu;  //Время осбождения ОЗУ для послед. работы.
     DWORDLONG tsl;     //Время осбождения устройства СЛОЖ.
     DWORDLONG tum;     //Время осбождения устройства УМНОЖ.
	 DWORDLONG tdl;     //Время осбождения устройства ДЕЛЕН.
	 DWORDLONG tpr;     //Время работы процессора.
     DWORDLONG dlit[21];//Длительность по числу малых тактов.
	 UINT axtung;  //Возможное изменение внешн.запросов.
	 UINT ao[6];  //Для адресов описателей обменов
	 UINT ab[6];  //Для адресов буферов сообщений
	 UINT rtt[6]; //Индексы RTT MKO, или 0 /если MKO нет/
	 UINT wklt[6]; //Признаки включения таймеров МКО по RR.
	 DWORDLONG tf;   //Фактическое время работы.
	 DWORDLONG tpred;   //Предыд.время в мкс.
	 DWORDLONG tpred4;  //Предыд.время в 4мкс.
     DWORDLONG tpred16;  //Предыд.время в 16мкс.
	 short wzwr;   //Признак появления перехода с возвратом.
	 UINT adrwzwr; //адрес возврата.
	 UINT ipwr[30];
	 UINT iuzwid[4]; //инд.регистров узла выдачи (по n) в модели памяти
	 UINT iuzpr1[4]; //инд.регистров узла приема 1 (по n) в модели памяти
     UINT iuzpr2[4]; //инд.регистров узла приема 2 (по n) в модели памяти
	 UINT iuzpr3[4]; //инд.регистров узла приема 3 (по n) в модели памяти
	 UINT its[3];    //инд.регистров таймера синхр.ММО (по n) в модели памяти
     DWORDLONG sernas[8]; //Время появления запрета на прерывание 
	 DWORDLONG sermax[8]; //Макс.время действия запрета на прерывание
	 UINT bufrt0;
     UINT bufrf0;
	 UINT bufrt1;
	 UINT bufrf1;
	 UINT bufrp0;
	 UINT bufrp1;
	 short wkldp;
     short wklblk;
	 short wklmgr;
	 short pdgdp;
	 short wklop;
	 short pskwm;
	 short swkl;
	 short swikl;
	 //----- Выходные :
     short wds[7];
	 short imp0;
     short imp1;
	 short imp2;
	 short imp3;
	 short imp4;
	 short imp5;
	 short imp6;
	 short imp7;
	 short imp8;
	 short imp9;
	 short imp10;
	 short imp11;
	 short imp12;
     short imp13;
	 short imp14;
	 short imp15;
     DWORDLONG timp0;
     DWORDLONG timp1;
	 DWORDLONG timp2;
	 DWORDLONG timp3;
	 DWORDLONG timp4;
	 DWORDLONG timp5;
	 DWORDLONG timp6;
	 DWORDLONG timp7;
	 DWORDLONG timp8;
	 DWORDLONG timp9;
	 DWORDLONG timp10;
	 DWORDLONG timp11;
	 DWORDLONG timp12;
	 DWORDLONG timp13;
	 DWORDLONG timp14;
	 DWORDLONG timp15;
	 //--------------
     DWORDLONG tst;        //время срабат.сторож.таймера КУК
	 DWORDLONG tgot;       //время сброса признаков готовности команд КУК
	 //---------------
	 USHORT kk0;           //коды команд в КУК
     USHORT kk1;
	 USHORT kk2;
	 USHORT kk3;
     DWORDLONG tq;     //Время появления обобщ.признака синхрониз. в таймере синхр.на RT.
	 UINT adrupr[6]; //Адреса управл.слов в таблице для каждого КМК
	 UINT bufou[220];
	 UINT pak0,pak1,pak2,pak3,pak4,pak5;
	 DWORDLONG tnasper[6]; //Времена начала передач по МКО
	 USHORT gt0;  //готовности команд КУК
     USHORT gt1;
	 USHORT gt2;
     USHORT gt3;		 
   } ntip;
     //***************************** конец карты процесссора
 

   //************** Установочная информация ****************


 // МОДЕЛИ РЕАЛЬНОЙ ПАМЯТИ. ОПИСЫВАЮТСЯ ПО ФИЗИЧЕСКИМ АДРЕСАМ:
                //Учитываются в sasa6.

   UINT zagn[]={0,		0x001FFFFC,0,3,400,400,     //Временно !
	                               //ЭТО  БЦВМ
		0x08000000,		0x0801FFFC,0,3,400,400,
		                           //Q-BUS
		0x0FF00000,		0x0FF5FFFC,0,3,400,400,
		0x0FFD0000,		0x0FFFFFFC,0,3,400,400,
        0x1f600000,     0x1f60001c,0,3,400,400,
				         //Это регистровая зона.
		0x1FC00000,		0x1FDFFFFC,0,1,400,400,
		0x40000000,		0x401FFFFC,0,3,400,400,
		0x5FC80000,		0x5FDFFFFC,1,1,400,400};

   //*******************************  конец модели памяти.

   UINT zagn1[]={0,		0x001FFFFC,0,3,400,400,
	                               //ЭТО  БЦВМ
		0x08000000,		0x0801FFFC,0,3,400,400,
		                           //Q-BUS
		0x0FF00000,		0x0FF5FFFC,0,3,400,400,
		0x0FFD0000,		0x0FFFFFFC,0,3,400,400,
        0x1f600000,     0x1f60001c,0,3,400,400,
				         //Это регистровая зона.
		0x1FC00000,		0x1FDFFFFC,0,1,400,400,
		0x40000000,		0x401FFFFC,0,3,400,400,
		0x5FC80000,		0x5FDFFFFC,1,1,400,400};

   //*******************************  конец 1 модели памяти.

   UINT zagn2[]={0,		0x001FFFFC,0,3,400,400,
	                               //ЭТО  БЦВМ
		0x08000000,		0x0801FFFC,0,3,400,400,
		                           //Q-BUS
		0x0FF00000,		0x0FF5FFFC,0,3,400,400,
		0x0FFD0000,		0x0FFFFFFC,0,3,400,400,
        0x1f600000,     0x1f60001c,0,3,400,400,
				         //Это регистровая зона.
		0x1FC00000,		0x1FDFFFFC,0,1,400,400,
		0x40000000,		0x401FFFFC,0,3,400,400,
		0x5FC80000,		0x5FDFFFFC,1,1,400,400};

   //*******************************  конец 2 модели памяти.

   UINT zagn3[]={0,		0x001FFFFC,0,3,400,400,
	                               //ЭТО  БЦВМ
		0x08000000,		0x0801FFFC,0,3,400,400,
		                           //Q-BUS
		0x0FF00000,		0x0FF5FFFC,0,3,400,400,
		0x0FFD0000,		0x0FFFFFFC,0,3,400,400,
        0x1f600000,     0x1f60001c,0,3,400,400,
				         //Это регистровая зона.
		0x1FC00000,		0x1FDFFFFC,0,1,400,400,
		0x40000000,		0x401FFFFC,0,3,400,400,
		0x5FC80000,		0x5FDFFFFC,1,1,400,400};

   //*******************************  конец 3 модели памяти.

   UINT zagn4[]={0,		0x001FFFFC,0,3,400,400,
	                               //ЭТО  БЦВМ
		0x08000000,		0x0801FFFC,0,3,400,400,
		                           //Q-BUS
		0x0FF00000,		0x0FF5FFFC,0,3,400,400,
		0x0FFD0000,		0x0FFFFFFC,0,3,400,400,
        0x1f600000,     0x1f60001c,0,3,400,400,
				         //Это регистровая зона.
		0x1FC00000,		0x1FDFFFFC,0,1,400,400,
		0x40000000,		0x401FFFFC,0,3,400,400,
		0x5FC80000,		0x5FDFFFFC,1,1,400,400};

   //*******************************  конец 4 модели памяти.

   UINT zagn5[]={0,		0x001FFFFC,0,3,400,400,
	                               //ЭТО  БЦВМ
		0x08000000,		0x0801FFFC,0,3,400,400,
		                           //Q-BUS
		0x0FF00000,		0x0FF5FFFC,0,3,400,400,
		0x0FFD0000,		0x0FFFFFFC,0,3,400,400,
        0x1f600000,     0x1f60001c,0,3,400,400,
				         //Это регистровая зона.
		0x1FC00000,		0x1FDFFFFC,0,1,400,400,
		0x40080000,		0x401FFFFC,0,3,400,400,
		0x5FC80000,		0x5FDFFFFC,1,1,400,400};

   //*******************************  конец 5 модели памяти.

   UINT zagn6[]={0,		0x001FFFFC,0,3,400,400,
	                               //ЭТО  БЦВМ
		0x08000000,		0x0801FFFC,0,3,400,400,
		                           //Q-BUS
		0x0FF00000,		0x0FF5FFFC,0,3,400,400,
		0x0FFD0000,		0x0FFFFFFC,0,3,400,400,
        0x1f600000,     0x1f60001c,0,3,400,400,
				         //Это регистровая зона.
		0x1FC00000,		0x1FDFFFFC,0,1,400,400,
		0x40080000,		0x401FFFFC,0,3,400,400,
		0x5FC80000,		0x5FDFFFFC,1,1,400,400};

   //*******************************  конец 6 модели памяти.

   //--------- Значения такта процессора в н/с ------ :

 UINT mtakt[]={0,40,40,40,40,40,40}; //Учитываются в sasa6 занесением в карты проц.
     //-------------------------     // 0 - чтобы индекс был равен номеру машины.

  // РЕГИСТРЫ ВУ В ФИЗИЧЕСКОМ АДРЕСНОМ ПРОСТРАНСТВЕ ЭВМ:
  UINT rewu[]={
//50 - центральный контроллер прерываний, - один комплект:	  
	  0xfff,50,0,0,0,0x8000040,0x8000044,0x8000048,
      0x800004c,0x8000050,0x8000054,0x8000058,0x800005c,
            //------------------------------------------

	       //1 - беликовские таймеры - 4 комплекта : 
 0xfff, 1,0,0,0,0x8000000,0x8000004,0x8000008,0x800000c,
 0xfff, 1,1,0,0,0x8000010,0x8000014,0x8000018,0x800001c,
 0xfff, 1,2,0,0,0x8000020,0x8000024,0x8000028,0x800002c,
 0xfff, 1,3,0,0,0x8000030,0x8000034,0x8000038,0x800003c,
			//-------------------------------------------
		    //51 - остаток КФУ  - один комплект:
 0xfff,51,0,0,0,0x8000060,0x8000064,0x8000068,
 0x800006c,0x8000070,0x8000074,0x8000078,0x800007c,0x8000048,
			//-------------------------------------------
			    //52 - регистры ПК - 3 комплекта :
 0xfff,52,0,0,0,0xfffcfc0,0xfffcfc4,0xfffcfc8,0xfffcfcc,
 0xfffcfd0,0xfffcfd4,0xfffcfd8,0xfffcfdc,0xfffcfe0,0xfffcfe4,
 0xfff,52,1,0,0,0xfffdfc0,0xfffdfc4,0xfffdfc8,0xfffdfcc,
 0xfffdfd0,0xfffdfd4,0xfffdfd8,0xfffdfdc,0xfffdfe0,0xfffdfe4,
 0xfff,52,2,0,0,0xfffefc0,0xfffefc4,0xfffefc8,0xfffefcc,
 0xfffefd0,0xfffefd4,0xfffefd8,0xfffefdc,0xfffefe0,0xfffefe4,
                //---------------------------------------
				//60 - регистры ПВР - один комплект:

	0xfff,60,0,0,0,
 0xfffff00, 0xfffff10, 0xfffff04, 0xfffff14,
 0xfffff08, 0xfffff0c, 0xfffff1c, 0xfffffd4,
 0xfffff20, 0xfffff30, 0xfffff24, 0xfffff34,
 0xfffff28, 0xfffff2c, 0xfffff3c, 0xfffff40,
 0xfffff44, 0xfffff48, 0xfffffd0, 0xfffff50,
 0xfffff54, 0xfffff58, 0xfffff5c, 0xfffffc0,
 0xfffffd8, 0xfffffc4, 0xfffffc8, 0xfffffcc,
 0xfffffdc,

				//----------------------------------------
0xfff, 100, 0, 0, 0,	//Регистры KMK
 0xfffc000, 0xfffc004, 0xfffc008, 0xfffc00c,
 0xfffc014, 0xfffc010,  0xfffc018, 0xfffc01c,
 0xfffc020, 0xfffc024, 0xfffc028, 0xfffc02c,
 0xfffc030, 0xfffc034, 0xfffc038, 0xfffc03c,
//-----
0xfff, 100, 1, 0, 0,
 0xfffc040, 0xfffc044, 0xfffc048, 0xfffc04c,
 0xfffc054, 0xfffc050,  0xfffc058, 0xfffc05c,
 0xfffc060, 0xfffc064, 0xfffc068, 0xfffc06c,
 0xfffc070, 0xfffc074, 0xfffc078, 0xfffc07c,
//-----

0xfff, 100, 2, 0, 0,
 0xfffd000, 0xfffd004, 0xfffd008, 0xfffd00c,
 0xfffd014, 0xfffd010,  0xfffd018, 0xfffd01c,
 0xfffd020, 0xfffd024, 0xfffd028, 0xfffd02c,
 0xfffd030, 0xfffd034, 0xfffd038, 0xfffd03c,
//-----
0xfff, 100, 3, 0, 0,
 0xfffd040, 0xfffd044, 0xfffd048, 0xfffd04c,
 0xfffd054, 0xfffd050,  0xfffd058, 0xfffd05c,
 0xfffd060, 0xfffd064, 0xfffd068, 0xfffd06c,
 0xfffd070, 0xfffd074, 0xfffd078, 0xfffd07c,
//-----
0xfff, 100, 4, 0, 0,
 0xfffe000, 0xfffe004, 0xfffe008, 0xfffe00c,
 0xfffe014, 0xfffe010,  0xfffe018, 0xfffe01c,
 0xfffe020, 0xfffe024, 0xfffe028, 0xfffe02c,
 0xfffe030, 0xfffe034, 0xfffe038, 0xfffe03c,
//------

0xfff, 100, 5, 0, 0,
 0xfffe040, 0xfffe044, 0xfffe048, 0xfffe04c,
 0xfffe054, 0xfffe050,  0xfffe058, 0xfffe05c,
 0xfffe060, 0xfffe064, 0xfffe068, 0xfffe06c,
 0xfffe070, 0xfffe074, 0xfffe078, 0xfffe07c,

//----------	КОНЕЦ рег. КМК -------------
//Узел выдачи ММО:
 0xfff,149,0,0,0,0xfffc080,0xfffc084,0xfffc088,0xfffc08c,
//Узлы приема:
 0xfff,150,0,0,0,0xfffc090,0xfffc094,0xfffc098,0xfffc09c,
 0xfff,150,1,0,0,0xfffc0a0,0xfffc0a4,0xfffc0a8,0xfffc0ac,
 0xfff,150,2,0,0,0xfffc0b0,0xfffc0b4,0xfffc0b8,0xfffc0bc,
// Таймер синхронизайии ММО:
 0xfff,153,0,0,0,0xfffc0c0,0xfffc0c4,0xfffc0c8,
     //---конец ММО

      //  БОЗУ:
0xfff, 200, 0, 0, 0,
 0xff00000, 0xff1fffc,
0xfff, 200, 1, 0, 0,
 0xff20000, 0xff3fffc,
0xfff, 200, 2, 0, 0,
 0xff40000, 0xff5fffc,
 //----------------------
			0};   //Этот 0 - признак конца массива   
       //Конец массива rewu **************************
// Последовательность описания адресов регистров в устройстве соответствует 
// их внутренним индексам в ЭТОМ УСТРОЙСТВЕ - 0,1,2,  и.т.д. Индексы регистров   
// в каждом устройстве начинаются с нуля.
                   //Внимание!
// Адреса регистров KMK (тип 100) в rewu[] описываются в след порядке:
//  RQN,RR,RTT,RSP,RSW,RSC,RM,RN,RCW,6 нулей,RTech.
//-------------------------------------
//Адреса регистров ПK (тип 52) в rewu[] описываются в след порядке:
// RFM,RPN,RTX,RRst,REQB,RDA,RDB,RDC,REIQ,ноль.
//-------------------------------------
//Адреса регистров центр.контр.прер.(тип 50) в rewu[] описываются в след порядке:
// RQM, ноль,ноль,ноль, RExc,ноль,ноль,ноль.
//--------------------------------------
// Адреса (остатка КФУ, тип 51) в rewu[] описываются в след порядке:
// RRstF,ноль,RRst,ноль,RERB,ноль,ноль,ноль.
//---------------------------------------
//Адреса регистров таймера процессора(тип 1) в rewu[] описываются в след порядке:
// RT,RP,RC,ноль.
//---------------------------------------
//Адреса регистров ПВР (тип 60) описываются в след порядке:
//RT0,RTL0,RP0,RPL0,RC0,RFX0,RFXL0,RWD,
//RT1,RTL1,RP1,RPL1,RC1,RFX1,RFXL1,
//RQI,RMI,RNI,REr0,RQP,RMP,RNP,RFP,RFM,ROff,RR,RTlm,RRel,RBlk.
//----------------------------------------
// БОЗУ (тип 200) представляется тремя зонами. В описании каждой зоны - два адреса.  
// Первый адрес - начало зоны, второй адрес - конец зоны (отличие от регистров). 
// Зоны имеют порядковые номера - 0,1,2.
//---------------------------------------
// Адреса регистров узла выдачи ММО описываются в след.порядке:
// RCM0,RCW0,RAc0,RAf0.
// Адреса регистров 1 узла приема ММО описываются в след.порядке:
// RCM1,RCW1,RAc1,RAf1. Здесь pornom=0.
// Адреса регистров 2 узла приема ММО описываются в след.порядке:
// RCM2,RCW2,RAc2,RAf2. Здесь pornom=1.
// Адреса регистров 3 узла приема ММО описываются в след.порядке:
// RCM3,RCW3,RAc3,RAf3. Здесь pornom=2.
// Адреса регистров таймера синхронизации ММО описываются в след.порядке:
// RT,RP,RC.

 //-----------------------------------
extern UINT NAXAEW;
extern void impulsNAXAEW(void);

  DWORDLONG plaw[]={0,1000000,1000000,1000000,1000000,1000000,1000000};
   //Это массив неточностей генераторов процессоров.
      //0 - чтобы индекс был равен номеру машины.

  int kuda[]={0,0,0,0,0,0,0};
    //Это массив направлений неточностей: 
    //  1  - генератор частит, надо вычитать,
    //(-1) - генератор медлит, надо прибавлять,
    //  0  - работает точно.
   //0 в начале - чтобы индекс был равен номеру машины.

   UINT nomou[ ]={1,1,1,1,1,1}; //Адреса ОУ
   //**************** Конец установочной информации *****************

               // ОБ'ЯВЛЕНИЯ   ФУНКЦИЙ  :
//-------------------------------------

extern void prmdi(QString);
extern void printklp();
	extern char* Disass(unsigned long int cod,
		          unsigned long int epc,unsigned short pr);
    extern short zapkesdanosob(UINT adr,UINT kod,ntip *k);                              
    extern    short proz(ntip *k); 
	                 //Главная функция - модель процессора.
	extern    UINT * modzu(UINT * zag);//Строит модель ЗУ.
    void prozessor(void);              //Крутит ПРЦ.
	short stzuda(UINT adr,ntip *k,short p);
	short zapzuda(UINT adr,ntip *k);
	void invol(UINT adr,ntip *k); 
	         //Инволюдизирует КЕШ ком.и данн.по адресу adr. 
    extern void printkl(QString s);       
	short writes(UINT adr,UINT a,ntip *k);
	DWORDLONG wrema(ntip *k);      //Считает время.
    DWORDLONG wrrom(void); //Выдает время Романову.
   extern char * mdop(UINT *rwu,ntip *k);
                            //Строит дополнение модели ЗУ.
   extern void  stbelt(ntip *k);
   extern	void sttmsi(ntip *k);
   extern	void wnzapkpr(ntip *k);
   extern UINT indpam(UINT adr,ntip *k);
	int sasa6(void);                                          
	UINT *ukaz(UINT adr,short nom);
	void jmpkl(short nom);
	short readb6(UINT adr,short nom,UCHAR &b);                
    short writeb6(UINT adr,short nom,UCHAR b);
//	short writeb(UINT adr,UCHAR b);
	extern void rtt(ntip *k);
	extern void dispet(void);
extern UINT streg(UINT tip,UINT pornom,UINT podtip,UINT nblok,
		                                    UINT it,ntip *k);
extern UINT nreg(UINT tip,UINT pornom,UINT podtip,
	                              UINT nblok,UINT it,ntip *k);
extern void wpis(UINT tip,UINT pornom,UINT podtip,UINT nblok,
		                 UINT it,UINT d,UINT sis,ntip *k);

	extern void zakaz(UINT rbl);
extern	void raskrutka(ntip *k);
extern void razbmodel(UINT iden,short w);
static long int load_pkm_dll(void);
static void End (void);
int oper(int nom);
void nasreset(int nm);
extern  void sbrkpr(ntip *k);  
extern  void sbrosostkfu(ntip *k);
extern  void sbrospk(ntip *k);
extern  void sbrospwr(ntip *k);
extern  void sbroskmk(ntip *k);
extern  void sbroskww(UINT pornom,ntip *k);
extern void sbrostimeb1(ntip *k);
extern void fnomou(ntip *k);
extern void sbroskuk(ntip *k);
extern void sbrosmmo(ntip *k);
extern void anmag(void);
extern void srabtsmmo(void);
extern void skuk(void);
void son(void);
int sag(void);
short readCP0(int nr,short nom,UCHAR *a);
short writeCP0(int nr,short nom,UCHAR *a);
short readCP1(int nr,short nom,UCHAR *a);
short writeCP1(int nr,short nom,UCHAR *a);
void widser(void);
 void sernull(void);
extern short sts2(UINT adr,ntip *k);
extern short zas2(UINT adr,ntip *k);
extern void komkuk(int nom);
extern UINT PtoF(UINT adr);
       //          КОНЕЦ ОБ'ЯВЛЕНИЙ ФУНКЦИЙ


      //************** ПЕРЕМЕННЫЕ ************************* :
       //Массив структур Романова, описывающий остановы:
 extern   M_BP *pBP[];

HANDLE eProz ;

//--------------------------------
static HINSTANCE hInstDll;
struct reg_table *pRegTable, RegTable;
struct mod_table *pModTable;// Указатели на таблицу моделей
extern	void (*ukf)(void);
extern int serwklus;
//---------------------------------
   //QFile myf;
   QString ima;
   UINT *mpp;
   ntip *k,karta;
   ntip *bazuk[7]={0}; //Указатели на карты по номеру ВМ.             
   ntip karta1,karta2,karta3,karta4,karta5,karta6;       
   static  UINT sdw,sisl,kdan;   
	 UINT pes;  //Для регулирования печати.
    extern QString soob;
    extern UINT indstreg;
	extern UINT *vxod,*vixod;
	extern UINT ident;
	extern int ipen0;
	extern int maxpr;
	int next;       //номер ВМ - выполняющей nexti. Иначе - 0.
	int aktwm[7]={0};;    //Для признака ВМ, сработавшей в sag().
	UINT adrsisla;
	UINT zakkl[2]={0};  //Копирую сюда заказ Романова zakrom.
    extern UINT sislo;
 //==================================================
    short isp;  //Признак использования очередной шестерки в otwrom.
   
	//------- ниже данные для работы со временем:
    extern DWORDLONG tn;
    extern UINT stopwr;
	DWORDLONG tp,trab;
    extern DWORDLONG ttek4; 
                // ttek4 системное время. Ед.измерения - 4 мкс.   
	extern UINT delt,delt16,delt4;
	DWORDLONG zwonok;
	UINT wkltakt;
	UINT takt;
	UINT wus[1600]={0};
	DWORDLONG Tmin; //Минимальное время событий

    DWORDLONG tsist;   //Системное время в н/с
    DWORDLONG tsis;    //Рабочая 
	//------------------------------------
extern int stopkomfile;

	//--------- Системные   параметры ------------ :

     int nomerWM;   //Эта ВМ выполняет сейчас операцию.
	 //Устанавливается в oper() перед обращением к proz(). Нужна при обменах МКО
	 extern DWORDLONG mag[60];
	 extern UINT prizqc[4];
	 extern DWORDLONG tsmmot[4];
    //------ Конец системных параметров ------------
       
  //---- Связующие системные данные -----------
	short dlln;  //Признак наличия DLL
    short ostanow; //Признак останова по адресу.
	UINT adrost; //Для адреса останова от Романова
	UINT zapsten;//Для фиксации призначной ячейки чтен/зап Романова
    short isklCP0[7]={0};//Признаки исключит.ситуации CP0.
	extern short rfee;   //Для признака исполненной RFE. Устанавливается в RFE. 
	//Используется в zakr  при k->predper=1 в oper(). Сбрасывается в начале каждой операции.
  //--------------------------------------------
  
   //-- ДАННЫЕ, ОБЩИЕ С РОМАНОВЫМ  :

short kolwm=1;     //Количество ВМ
   UINT Komplex[7]={1,1,1,1,1,1,1};   //Включена-выключена i-я ВМ (по индексу ном ВМ).
                      //1 - включена,  0 - выключена
   UINT zakrom[2]={0};    //Заказ Романова. Пока нет заказа Романова

   UINT otwrom[37]={0};   //Ответ Романову / шестерки.
   short potkl=1;       //Признак, что я сплю(0)-работаю(1).
   UINT swrem;     //=0 - приказ не считать время.
   UINT nulwrem; //=0 - приказ обнулить время и установить =1.
   int Reskl;               //Для романовского Reset.
   extern UINT TargetVM;
   extern UINT VektVM[7];
   UINT JUMP;
   UINT PJUMP=0;
     //====================================================


     //--------------------- ФУНКЦИИ ------------------ :

    void prozessor(void)        //Сверх-главная функция.
    {    int i,ii,jn,otw;
		 int j;      QString str,otl;
		 short estbrek; //Признак останова в заказе (stepi или nexti).
		 printkl("prozessor started");
          for(i=1;i<=kolwm;i++)
{   
	 bazuk[i]->tf=0;
 bazuk[i]->tpr=0; bazuk[i]->tzu=0; bazuk[i]->tsl=0;
 bazuk[i]->tum=0; bazuk[i]->tdl=0;
 bazuk[i]->tpred=0; bazuk[i]->tpred4=0;
 bazuk[i]->tpred16=0;
}//--- Чистка времен ВМ

         tsist=0; ttek4=0; 

		 

		 otwrom[0]=0; //Для Романова.
Reset:		 potkl=1;

	ekran[0]=0; ekran[1]=0; ekran[2]=0; ekran[3]=0; ekran[4]=0;
           stopkomfile=0;

     dlln=0; //Пока нет DLL
      nomerWM=0;
	 wkltakt=0; //Тактовое разбуж.не включено.
    //--- Определение наличия DLL/тогда dlln=1/
	//--- или отсутствия/dlln=0/ DLL.          Олег !
//------------------------------------------------------
            jn=0;
	pRegTable = &RegTable;
	pRegTable->pTextToFile = &printklp; 
	pRegTable->pTextToWindow = &printklp; 
	if ((dlln = (short)load_pkm_dll()) != 0)
		                           // Загрузка PKM.DLL
	{		
	//-- Определение максимального идентификационного
	//--   номера модели./jn/                  Олег !
	             jn=0;      
		for (i = 0; pModTable[i].ID != 0; i++)
		{
			 if ((UINT)pModTable[i].ID >(UINT) jn)  
				 jn = pModTable[i].ID;
		}
	}
	
     if(jn>150)
	 {printkl("# Большое кол-во моделей. Отказ."); goto Son;}
		
  //-------------------------------------------------
 //-- Формирование структуры будильника под состав моделей:
    
   for(i=0;i<1600;i++)      wus[i]=0;  //Чистка
       
          for(i=1;i<=kolwm;i++)
{   
	 bazuk[i]->pak0=0; bazuk[i]->pak1=0; bazuk[i]->pak2=0;
	 bazuk[i]->pak3=0; bazuk[i]->pak4=0; bazuk[i]->pak5=0;
}

      //--------- Учредим 6 КМК и 6 ВМ в будильнике ---------                    
	        for(i=0;i<1600;i++)  wus[i]=0;   //Общая его чистка
        //----------------------------------------------------
			for(i=1;i<=6;i++)      //Цикл по ВМ,  i - мой номер ВМ
		{     for(j=0;j<=5;j++)    //Цикл по КМК, j - беликовский номер
			{ii=(i-1)*48; ii=ii+j*8;    //Начальный индекс восьмерки в wus[].
			 wus[ii]=1;   //признак KMK
			 wus[ii+1]=j; //номер КМК
			 wus[ii+2]=0; //пока не активен
			 wus[ii+5]=i; //мой номер ВМ
			}
		}// -------с ВМ и КМК разделались
      //---------------------  Теперь модели с 288 индекса в wus[].
             if(jn>0)   //Есть модели
		{j=0;
				   for(i=288;  ;i=i+8)
				{wus[i]=2;    //признак модели
                 wus[i+1]=j;  //номер этой модели
				 j=j+1;    if(j>jn)
						{ii=i; break;}
				}
		       wus[ii+10]=3;  //Конец будильника с моделями.
		}
		  else     //нет моделей
		wus[290]=3;	         //Конец будильника без моделей
	
	                          
		   Tmin=0xfffffffffffffff;  //Минимальное время в пустом будильнике
    
               // ********************************* Будильник готов !

	      tsist=0;   //Системное время
          ttek4=0;    
  //********** Определим реальный состав КМК (по RТТ) и др. для всех ВМ:

	          for(j=1;j<=kolwm;j=j+1)       //Цикл по ВМ.
{	             k=bazuk[j];
		      for(i=0;i<6;i++)
	{jn=nreg(100, i, 0, 0, 7, k);
      if(jn==1) k->rtt[i]=indstreg; else k->rtt[i]=0;
	}//-------------------------------------------
     for(i=0;i<6;i++)
	 { k->wklt[i]=0;} //Чистка призн.включ.
    //--------------------------------------------
           //Формируем ipwr[]  :      
      for(i=0;i<=28;i++)
	  {jn=nreg(60,0,0,0,i+5,k); k->ipwr[(int)i]=indstreg;}
 //------- Это были индексы регистров ПВР (начиная с нуля)

	       //Формируем iuzwid[]
	  for(i=0;i<=3;i++)
      {jn=nreg(149,0,0,0,i+5,k); k->iuzwid[i]=indstreg;}
        //-------------------------------
	      //Формируем iuzpr1[]
	  for(i=0;i<=3;i++)
      {jn=nreg(150,0,0,0,i+5,k); k->iuzpr1[i]=indstreg;}
        //-------------------------------
	      //Формируем iuzpr2[]
	  for(i=0;i<=3;i++)
      {jn=nreg(150,1,0,0,i+5,k); k->iuzpr2[i]=indstreg;}
        //-------------------------------
	      //Формируем iuzpr3
	  for(i=0;i<=3;i++)
      {jn=nreg(150,2,0,0,i+5,k); k->iuzpr3[i]=indstreg;}
        //-------------------------------
           //Формируем its[]
	  for(i=0;i<=2;i++)
      {jn=nreg(153,0,0,0,i+5,k); k->its[i]=indstreg;}
        //-------------------------------
           //--- Вспомогат.массивы сформированы.----
            
    for(i=0;i<4096;i++) k->kespk[i]=0;
    for(i=0;i<2048;i++) k->kespd[i]=0; //Инволюдизировали КЭШ.
	   //---------------------------------
}                                         //конец цикла по ВМ.
//********************************************************
                      
         
 
		   //Пока нет ведущей ВМ на МКО.
              //Начальный мой RESET:
	for(i=1;i<=kolwm;i=i+1) 
	{
		nasreset(i); 		
//	   Komplex[i]=1;
	}
	 //------------------------------

         //------- Чистка магистралей:
	        for(i=0;i<60;i++)   mag[i]=0;

	for(i=(kolwm+1);i<=6;i=i+1)	  
           Komplex[i]=0;  //Отсутствуют.
        
	 //***************************************************
			  if(dlln!=0)
		          //НАЧАЛЬНЫЙ установ моделей
	{   for(int i=0; pModTable[i].ID!=0;i++)
	  { 
	    if(pModTable[i].Reason==REASON_INIT)
		{
         vxod= (UINT *)pModTable[i].pInputBuffer;
         vixod=(UINT *)pModTable[i].pOutputBuffer;
		 ident= (UINT)pModTable[i].ID;
		 ukf=pModTable[i].Subrut;
		 vxod[0]=0; 
                    (*ukf)( ); 
                  pes=0;
				  zakaz(0);                        
				  pes=1;
		}		  
      }
	}//---- КОНЕЦ начальной установки моделей ---------------
                   next=0;   //Сброс nexti
//********************************************************

	
         sernull( );  //чистка массивов для Сергея

         serwklus=0;  //выключение подготовки этих данных
//*******************************************************
//*******************************************************

Son:	son( );      //сплю - не сплю.

	   potkl=1;
	   printkl("# potkl=1   Я проснулся");                             
 //*******************************************************
Sag:    if(PJUMP!=0)     //--------------Учет романовского jump.
		{ntip *k;
			PJUMP=0;
               for(i=1;i<=kolwm;i++)
			   {if((Komplex[i]==1)&&(VektVM[i]==1))
					{k=bazuk[i];
                     k->predper=0;
					 k->pc=JUMP;
					 next=0;
					}
			   }
		}//---------------------------------  конец этого учета
	   
	   //fbreak.lock();          //Начало критической секции

	   zakkl[0]=zakrom[0]; zakkl[1]=zakrom[1];
	   //fbreak.unlock();          //Конец критической секции
         //-------------------------------
	   if((040&zakkl[0])!=0)  //Reset
	   {otwrom[0]=040; otwrom[6]=0;             
	   goto Reset;}                      
      //-------------------------------	   
	   if((014&zakkl[0])!=0)  //Остановы внешние
	   {otwrom[0]=014&zakkl[0]; otwrom[6]=0;  goto Son;}    
      //-------------------------------
	   if(zakkl[0]==0)        //Нет задания
		{otwrom[0]=0; otwrom[6]=0; 
	      printkl("# Нет задания !"); goto Son; 
		}
	  //-------------------------------
       if(((03&zakkl[0])!=0)&&(Komplex[(int)zakkl[1]]==0))        
		                         //команда невключенной ВМ.

	{UINT nzam;int ii;  nzam=0;
                             printkl("# Команда stepi или nexti невключенной ВМ !");
		   if(zakkl[1]!=TargetVM) 
		{
	soob.sprintf("Попытка stepi или nexti по отключенной ВМ%d - не по отлаживаемой ВМ%d TargetVM",zakkl[1]-1,TargetVM-1);
		   printkl(soob);   if(Komplex[TargetVM]!=0) 
		   {nzam=TargetVM; soob.sprintf("# Переход на включенную отлаживаемую ВМ");printkl(soob);}
		}       //-------------------------------------------
		   

		   
		              if(nzam==0)  //Теперь надо смотреть только информационные ВМ
			{       for(ii=1;ii<=kolwm;ii++)
				{if((Komplex[ii]==0)||(VektVM[ii]==0)) continue;
		              nzam=(UINT)ii; break;
				}//  нашли среди информационных nzam !=0, если был break
					     //------------------------
		              if(nzam==0)  //Теперь надо смотреть только включенные ВМ, т.к.информационных нет
			       for(ii=1;ii<=kolwm;ii++)
				{if(Komplex[ii]==0) continue;
		              nzam=(UINT)ii; break;
				}//  нашли среди включенных nzam !=0, если был break
			}//---------------------------- все исследовали ----
					  if(nzam==0)  //ничего не нашли
					  {printkl("# Команда stepi или nexti, но все ВМ отключены");
					   otwrom[0]=02000|otwrom[0]; otwrom[6]=0;
		                       goto Son;
					  }
                      else
					  {zakrom[1]=nzam; zakkl[1]=zakrom[1];TargetVM=nzam;
					   soob.sprintf("# Назначена отлаживаемой ВМ%d",nzam-1);printkl(soob);
                      }
	 }                        
      //------------------------------- 
	  
	  if((03&zakkl[0])!=0)   //Есть stepi или nexti           
	  {
		  if(((1&zakkl[0])!=0)||((1&zakrom[0])!=0))   
		{
			if((1&zakkl[0])!=(1&zakrom[0]))
				{soob.sprintf("Не совпаден заказа по stepi: у Романова %d, у Клямко %d",zakrom[0],zakkl[0]);printkl(soob);}
            
			if(zakkl[1]!=zakrom[1])
				{soob.sprintf("Не совпадение ВМ по stepi: у Романова %d, у Клямко %d",zakrom[1],zakkl[1]);printkl(soob);}
            if(potkl!=1) printkl("В stepi  potkl=0 !");
			if((2&zakrom[0])!=0) printkl("В заказе Романова stepi и nexti  одновременно!");
       
	
		              	 
		}//------------------ конец моего анализа
             estbrek=1;                                
	  }
	
	    else  
	   { estbrek=0;} 
	   //------------------------------
 //====================== НАЧИНАЕМ   ШАГАТЬ =====================


	   otw=sag(); //Один шаг - одна операция 
                       //одновременно отставших ВМ.
   if(otw==1)
   {printkl("#  ВСЕ  ВМ  ОТКЛЮЧЕНЫ !");     goto Son;}
   if(otw==100) 
   {printkl("# Фатальная ошибка в oper().");goto Son;}
 //*******************************************************
 //--- Теперь есть продвинутые ВМ - по aktwm[]. Можно смотреть остановы и zakkl.
          //-- Контроль заказа Романова - в zakkl  и его исполнение:

	   otwrom[0]=0; //Предварительная чистка нач.ячейки ответа Романову
	      //------------------------------------
if(((020&zakkl[0])!=0)&&((017&zakkl[0])!=0)) //contin c другими командами
{otwrom[0]=(02000|otwrom[0]);
  printkl("# CONTIN вместе с другими командами !");
  goto Son;
}
 //*******************************************************
     
   //**************** ГОТОВИМ ОТВЕТ РОМАНОВУ  ************

          if(estbrek==0)   //Нет остановов по stepi или nexti
{                          //Будут учитываться только остановы по адресу 
//**** Начинается АНАЛИЗ списка остановов и исключ.ситуаций CP0. 
//***************** Этот анализ - по номерам ВМ. ****************
	//fbreak.lock();
                               next=0;     //Сброс nexti
  jn=0;  //Пойдет по otwrom[].
  otwrom[6]=0;
  isp=0;
  ostanow=0;    
                for(j=1;j<=kolwm;j++) //Цикл по продвинутым ВМ.         
	{if(aktwm[j]==0) continue; 
          for(i=0; ;i++)   //цикл по  i  - просмотр остановов j  ВМ.
	{  zapsten=(pBP[j]+i)->pr; //ячейка запись-чтение у Романова в i-й структуре
		  if(zapsten==0)
				break;  //поиск до 0.

      adrost=(pBP[j]+i)->adr;  //программный адрес у Романова в i-й структуре
	       if((02&zapsten)!=0)  //есть заказ по записи
		{if(bazuk[j]->sleds[0]==2)  //и у меня была запись числа
		 {adrsisla=bazuk[j]->sleds[5]; //мой адрес числа
		         if(adrost==adrsisla)  //будут записи в otwrom.
			{otwrom[jn]=(0400|otwrom[jn]);
             otwrom[jn+3]=bazuk[j]->sledk[0]; // Программный адрес команды / было =adrost;
			 otwrom[jn+4]=i;
             otwrom[jn+5]=j;
			 ostanow=1; //был останов по записи.
			 isp=1;
			}
		 }
		}//конец обработки элемента структуры по записи
             
		   if((1&zapsten)!=0)   //есть заказ по чтению
		{//-- сначала проверим на адрес исполненной команды:
			adrsisla=bazuk[j]->sledk[0]; //мой адрес команды
			if(adrsisla==adrost)   //это останов по адресу команды
            {otwrom[jn]=(0100|otwrom[jn]);
             otwrom[jn+1]=adrost;
             otwrom[jn+2]=i;
             otwrom[jn+5]=j; 
			 ostanow=1; //был останов.
			 isp=1;
            }//разобрались с остановом по адресу команды
          //-- Теперь проверим на адрес прочитанного числа:
            //-- Но сначала - было ли у меня такое чтение:
			 if(bazuk[j]->sleds[0]==1)  //было
			{adrsisla=bazuk[j]->sleds[5];
			 if(adrsisla==adrost)
			 {otwrom[jn]=(0200|otwrom[jn]);
              otwrom[jn+3]=bazuk[j]->sledk[0]; // Программный адрес команды / было =adrost;
              otwrom[jn+4]=i;
              otwrom[jn+5]=j; 
			  ostanow=1; //был останов.
			  isp=1;
			 }
            }//разобрались с остановом по адресу чтения числа

		}//конец обработки элемента структуры по чтению

	}//********* конец просмотра по i  остановов j  ВМ. ********
/*
	 //------ Теперь - учет исключит.ситуаций CP0 для j ВМ:
               if(isklCP0[j]==1)
	{otwrom[jn]=(01000|otwrom[jn]); otwrom[jn+1]=bazuk[j]->sledk[0];
			   //Дали программный адрес команды с исключ.ситуацией.
     otwrom[jn+5]=j;   
	 isp=1;
	}//************ Учли исключ.ситуацию. *********************
*/              
	              if(isp==1) {jn=(jn+6); otwrom[jn]=0; isp=0;}

	}//-------- Конец цикла по продвинутым ВМ. -------------------

           //--- Теперь  учет contin:
   if((020&zakkl[0])!=0)   //contin
   {otwrom[0]=(020|otwrom[0]); }
   //*****************************************************
                  
   if(ostanow==1) {
	  	//fbreak.unlock();

	   goto Son;}
   else {
	  	//fbreak.unlock();

	   goto Sag;}
   
} //---- Конец при estbrek==0 ----------


         else  //estbrek==1.  Есть остановы по stepi или nexti
{               //Остановы по адресу - игнорируются.           
//**** Начинается АНАЛИЗ  и исключ.ситуаций CP0. 
//***************** Этот анализ - по номерам ВМ. ****************************
                        isp=0;
      //---- Сначала - учет  отсутствия nexti по заказу Романова
               if((2&zakkl[0])==0) next=0; //сброс nexti
      //  --- Не вводится ли новое nexti ?
     if(((2&zakkl[0])!=0)&&(zakkl[1]!=(UINT)next))   //вводится
     {next=(int)zakkl[1]; bazuk[next]->wzwr=0; }     //next установлено, wzwr обновлен
	                                                 //для ВМ-next.
                 //--------------------------

  jn=0;  //Пойдет по otwrom[]. 
  otwrom[6]=0;
  ostanow=0;    
                for(j=1;j<=kolwm;j++) //Цикл по продвинутым ВМ.         
	{if(aktwm[j]==0) continue;
				
/*
	 //------ Теперь - учет исключит.ситуаций CP0 для j ВМ:
               if(isklCP0[j]==1)
	{otwrom[jn]=(01000|otwrom[jn]); otwrom[jn+1]=bazuk[j]->sledk[0];
			   //Дали программный адрес команды с исключ.ситуацией.
     otwrom[jn+5]=j;
	 isp=1;
	}//************ Учли исключ.ситуацию. *********************
       
*/
       //---- Теперь - учет stepi по заказу Романова
   if(((1&zakkl[0])!=0)&&(zakkl[1]==(UINT)j))    //stepi для этой ВМ - j
   {
	 ostanow=1;  otwrom[jn]=(1|otwrom[jn]); otwrom[jn+5]=(UINT)j;
	
           isp=1;          	
   }//******************************** учли stepi
//==============================================================================================Новое nexti

                //ОСНОВНАЯ  РАБОТА  ПО   nexti:

          if((2&zakkl[0])!=0)    //nexti

	{     if(j==next)            //ВМ j
         
		{ if((bazuk[j]->wzwr)==0) //находимся вне функции - wzwr=0.
			{
			 ostanow=1; otwrom[jn]=(2|otwrom[jn]);
             otwrom[jn+5]=next;
			 isp=1;
			}
           else if(bazuk[j]->wzwr==1)      //Мы перед слотом команды перехода с возвратом
		   {bazuk[j]->wzwr=2;
			 ostanow=1;  //т.е. высветится адрес слота
			 otwrom[jn]=(2|otwrom[jn]);
             otwrom[jn+5]=next;
			 isp=1;
		   }
           else if(((bazuk[j]->wzwr)==2)&&
		  (bazuk[j]->pc==bazuk[j]->adrwzwr)) //находимся в конце функции.
			{ ostanow=1;   //т.е.высветится адрес команды, в которую происходит возврат.
			 otwrom[jn]=(2|otwrom[jn]);
             otwrom[jn+5]=next;
			 isp=1;              
			  bazuk[j]->wzwr=0; //Окончание проскакивания функции.
			}
           else {}
		 }


		  else {}  // j!=next

    }//******************************* учли nexti


//==============================================================================================
		  if(isp==1) { jn=(jn+6); otwrom[jn]=0; isp=0; }
	}//-------- Конец цикла по продвинутым ВМ. -----------------------------
           //--- Теперь  учет contin:
   if((020&zakkl[0])!=0)   //contin
   {otwrom[0]=(020|otwrom[0]); }
   //*****************************************************
   if(ostanow==1) {goto Son;}
	   
	   
   else           {goto Sag;}
	   
	    
} //---- Конец при estbrek==1 ----------



 }//============  Конец  prozessor  =======================

    
   int sag(void)
      //Ищет одноврем. опаздывающие ВМ и выполняет для них 1 операцию.
	  //Возращает 1, если все ВМ выключены,
	  //Возращает 100, если  oper() дает фатальный ответ 1.
	  //Возращает 0 при удаче.
	  //В aktwm[] фиксирует по номеру ВМ признак (=1) одновр.опазд.ВМ.
	  //           Запускается в prozessor().
	           //---------------------------------
   {int i,j,nom,otw;  DWORDLONG t;
    // for(i=0;i<4;i++) prizqc[i]=0; //Обнуление признаков появления Qc  НЕ ИСПОЛЬЗУЕТСЯ
      //Ищем ВМ с минимальным tf  (фактического времени работы) - t:
     t=0xFFFFFFFFFFFFFFF; nom=0;     
                 for(i=1;i<=kolwm;i++)
		{if(Komplex[i]==0) continue;
         if((bazuk[i]->tf)<t)
		 {nom=i; t=bazuk[i]->tf;}
		}//----------------- нашли --

	if(nom==0) {/*printkl("# В sag() не найдена включенная ВМ.");*/ return(1);}
		         //-----------------
	for(i=1;i<=kolwm;i++) aktwm[i]=0;     //Обнулили признаки одновр.опазд.
	   //Теперь ищем одновр.опаздывающие ВМ и исполняем для них 1 операцию oper():
	           for(i=1;i<=kolwm;i++)
	{if(Komplex[i]==0) continue;
            if(bazuk[i]->tf==t)
			{aktwm[i]=1;
			   otw=oper(i);
			if(otw==1) return(100);
			}
	}  //------- Все одновр.опазд.ВМ выполнили 1 операцию.


             //-- Определяем системное время tsist:
	 j=0; t=0;  for(i=1;i<=kolwm;i++)
	 { if(Komplex[i]==0) continue;                   // if(aktwm[i]==0) continue;
	  j=(j+1); t=(t+bazuk[i]->tf);
     } //--------
       tsis=(t/j)+j; if(tsis>tsist)      tsist=tsis;       ttek4=tsist/4000;
//------------- Теперь продвинем время невключенных ВМ
            for(i=1;i<=kolwm;i++)
	{if(Komplex[i]!=0) continue;
      bazuk[i]->tf=tsist; bazuk[i]->tpr=tsist; bazuk[i]->tzu=tsist; bazuk[i]->tsl=tsist;
	                                           bazuk[i]->tum=tsist; bazuk[i]->tdl=tsist;
      bazuk[i]->tpred=((tsist/1000)+0)*1000; bazuk[i]->tpred4=((tsist/4000)+0)*4000;
	                bazuk[i]->tpred16=((tsist/16000)+0)*16000;
	}
                       //---- продвинули
			//--- Теперь ммо и КУК:
	              anmag( );
				  for(i=1;i<=kolwm;i++)  { komkuk(i);} 
	   srabtsmmo( );        //Устанавливает Q во всех ТС, если этот факт происходит
	   skuk();      //Реализует события по КУК-у по его  таймеру.
                     return(0);

   }//======= конец sag =======================================

  int oper(int nom)  //Выполняет одну операцию для одной ВМ. nom - номер ВМ. (мой)
	  //возвращает 1 при фатальной ситуации, иначе - 0.
	  //Фиксирует в isklCP0[nom] признак исключ.сит.CP0 (=1).
	  //               Запускается в sag().     
	        //-------------------------------------------
  {   short otwet; int i; QString str,otl; ntip *k; 
           DWORDLONG trab; 
//           UINT adrslot;         //Для запом.адреса слота. 		           
    k=bazuk[nom]; isklCP0[nom]=0;
	nomerWM=nom; //Зафиксировали номер ВМ, выполняющей операцию.
	  ipen0=0; maxpr=-1;  //Пенал перед операцией должен быть пуст. 
                                             		
        		 k->kom=1;

	             if(k->predper==1)                         
	{
		ipen0=0; maxpr=-1;
	  otwet=proz(k);
				if(otwet!=0)    
	{  if(otwet==3)
		{ printkl("#F - ZAKR - Ошибка в программе"); printkl(soob);
	char* pch=Disass((unsigned long int)k->sledk[4], 
		               (unsigned long int)k->sledk[0],0);
	otl.sprintf("# %08X\t",(unsigned long int)k->sledk[0]);
	str=(const char*)pch;
	str=otl+str;
	printkl(str); 
                   	return(1);
		}//--------------------------
			  else if(otwet==4)
			{ 
			//printkl("#E - ZAKR - Непредусмотренная команда"); //!@!@    
	char* pch=Disass((unsigned long int)k->sledk[4], 
		               (unsigned long int)k->sledk[0],0);
	otl.sprintf("# %08X\t",(unsigned long int)k->sledk[0]);
	str=(const char*)pch;
	str=otl+str;
	//printkl(str); 
                    
			}//------------------------
   else if(otwet==2)
   { isklCP0[nom]=1;     // printkl("#I Исключительная ситуация CP0"); //!@!@                 

   }

   else if(otwet==1)
   {printkl("#FFF Ошибка с ответом =1  в proz."); return(1);}
			  else { }
	
}//--- конец при условии otwет!=0

	                  tn=wrema(k);
     
    
   //Сначала вычислим фактическое время работы данной ВМ ном:
                if(kuda[nom]==1)     //генератор частит
           k->tf=tn-(tn/plaw[nom]);
           else if(kuda[nom]==(-1))  //генератор замедляет
           k->tf=tn+(tn/plaw[nom]);
		     else k->tf=tn;          //генератор идеален
    //****** вычислили и записали в карту ********
  //**********************************************
       if(tn!=0)
{           delt=0;
	   if(tn<(k->tpred))
	   {printkl("#  K m11: Фатальная ошибка в oper(). !");ExitThread(TRUE);}
m11:   
	   trab=tn-(k->tpred);  if(trab<1000) goto m21;
     k->tpred=((k->tpred)+1000); delt=(delt+1);
	  
	           goto m11;
     //---------- delt определено -----------    
m21:   		   delt16=0;
m31:  		   trab=tn-(k->tpred16);  if(trab<16000) goto m41;
     k->tpred16=((k->tpred16)+16000); delt16=(delt16+1);
	           goto m31;
     //---------- delt16 определено -----------    
m41:   		   delt4=0;
m51:   		   trab=tn-(k->tpred4);  if(trab<4000) goto m61;
     k->tpred4=((k->tpred4)+4000); delt4=(delt4+1);
	           goto m51;
     //---------- delt4 определено -----------    
m61:    { }       
}
                                
    else
{delt=0; delt4=0; delt16=0; }
//***********************************************
 //--- ИНИЦИАЦИЯ КОМПЛЕКСА МОДЕЛИРОВАНИЯ ВНЕШНИХ УСТРОЙСТВ ----------  
         //-------------- Для обработки беликовских таймеров:
                    if((delt>0)&&((017&k->axtung)!=0))
			{stbelt(k); }  //Ведет счет на таймерах процессора.
             //----------------------------------------
        
     	if(delt4>0)    rtt(k);
		//----------------------------------------

  if(delt>0) sttmsi(k); 
                      //Ведет счет на таймере синхронизации                 
	         //----------------------------------------
         //---------------------
   if(ttek4>=Tmin) dispet(); //Диспетчер моделей !!!
			//-----------------------------------------------
   if((ttek4>=zwonok)&&(wkltakt!=0 )) //Тактовое разбуживание
	{   if((dlln)!=0)   
		{               
					 for( i=0;pModTable[i].ID!=0;i++)               
			{if(pModTable[i].Reason!=REASON_TACT) continue;					 
             ident=(UINT)pModTable[i].ID;

//------------------------------------------------------------
                     razbmodel(ident,1);
			}
		}
      zwonok=(ttek4+takt);
	}//---------------------------------------------------
            //----  Теперь полное прочесывание пенала:
                  raskrutka(k);  //конец работы с пеналом.
			//-----------------------------------------
    //--- Вставка для г.Нахаева:
				  if(NAXAEW!=0) //Заказано ли импульсное прерывание в ком.файле?
	{
        impulsNAXAEW();
	    	NAXAEW=0;
	}//----------------------- 

 //---- Обработка запросов по центральному контроллеру прер kpr:
				   if((020&k->axtung)!=0)
			{  wnzapkpr(k);
			    k->axtung=(020^(k->axtung));	
			}
	  	//----------- Конец обработки выполнения команды. *****
  //--- КОНЕЦ ИНИЦИАЦИИ КОМПЛЕКСА МОДЕЛИРОВАНИЯ ВНЕШНИХ УСТРОЙСТВ ----


       if(Komplex[nom]==0)                              
{
  //soob.sprintf("# BM%d отключилась в слоте",nom-1);printkl(soob);
  return(0);
}
	   
      ipen0=0; maxpr=-1;
	  //if(rfee==0)    //В слоте при k->predper=1 была не rfe, поэтому надо исполнять proz(). 
		             //Если rfee=1, то этот proz не нужен, т.к. RFE в слоте сработала и за него. 
	  //otwet=proz(k);

      k->predper=0;   k->pc=k->adrper;                                                      //********************

      return(0);                                                                          //***************
	}                                        
	    //--------------- конец при k->predper==1 -----------------

				 else                  //k->predper==0
{        
		otwet=proz(k);                                                                       //**************
				if(otwet!=0)    
	{  if(otwet==3)
		{ printkl("#F - ZAKR - Ошибка в программе"); printkl(soob);
	char* pch=Disass((unsigned long int)k->sledk[4], 
		               (unsigned long int)k->sledk[0],0);
	otl.sprintf("# %08X\t",(unsigned long int)k->sledk[0]);
	str=(const char*)pch;
	str=otl+str;
	printkl(str); 
                   	return(1);
		}//--------------------------
			  else if(otwet==4)
			{ 
			//printkl("#E - ZAKR - Непредусмотренная команда"); //!@!@   
	char* pch=Disass((unsigned long int)k->sledk[4], 
		               (unsigned long int)k->sledk[0],0);
	otl.sprintf("# %08X\t",(unsigned long int)k->sledk[0]);
	str=(const char*)pch;
	str=otl+str;
	printkl(str);           //!@!@  // 
                    
			}//------------------------
   else if(otwet==2)
   { isklCP0[nom]=1;      //printkl("#I Исключительная ситуация CP0");  //!@!@  

   }

   else if(otwet==1)
   {printkl("#FFF Ошибка с ответом =1  в proz."); return(1);}
			  else { }
	
}//--- конец при условии otwет!=0
	                  tn=wrema(k);
	
   //Сначала вычислим фактическое время работы данной ВМ ном:
                if(kuda[nom]==1)     //генератор частит
           k->tf=tn-(tn/plaw[nom]);
           else if(kuda[nom]==(-1))  //генератор замедляет
           k->tf=tn+(tn/plaw[nom]);
		     else k->tf=tn;          //генератор идеален

    //****** вычислили и записали в карту ********

  //**********************************************
       if(tn!=0)
{           delt=0;
	        
	   if(tn<(k->tpred))
	  {printkl("#  K m1: Фатальная ошибка в oper(). !");ExitThread(TRUE);}
m1:       
	   trab=tn-(k->tpred);  if(trab<1000) goto m2;
     k->tpred=((k->tpred)+(DWORDLONG)1000); delt=(delt+1);
	       
	           goto m1;
     //---------- delt определено -----------    
m2:   		   delt16=0;
m3:  		   trab=tn-(k->tpred16);  if(trab<16000) goto m4;
     k->tpred16=((k->tpred16)+16000); delt16=(delt16+1);
	           goto m3;
     //---------- delt16 определено -----------    
m4:   		   delt4=0;
m5:   		   trab=tn-(k->tpred4);  if(trab<4000) goto m6;
     k->tpred4=((k->tpred4)+4000); delt4=(delt4+1);
	           goto m5;
     //---------- delt14 определено -----------    
m6:    { }       
}
                                
    else
{delt=0; delt4=0; delt16=0; }
//***********************************************
 //--- ИНИЦИАЦИЯ КОМПЛЕКСА МОДЕЛИРОВАНИЯ ВНЕШНИХ УСТРОЙСТВ ----------
       
         //-------------- Для обработки беликовских таймеров:
                    if((delt>0)&&((017&k->axtung)!=0))
			{stbelt(k); }  //Ведет счет на таймерах процессора.
             //----------------------------------------
        
     	if(delt4>0)    rtt(k);
		//----------------------------------------

  if(delt>0) sttmsi(k); 
                      //Ведет счет на таймере синхронизации                 
	         //----------------------------------------
         //---------------------
   if(ttek4>=Tmin) dispet(); //Диспетчер моделей !!!
			//-----------------------------------------------
   if((ttek4>=zwonok)&&(wkltakt!=0 )) //Тактовое разбуживание
	{   if((dlln)!=0)   
		{               
					 for( i=0;pModTable[i].ID!=0;i++)               
			{if(pModTable[i].Reason!=REASON_TACT) continue;					 
             ident=(UINT)pModTable[i].ID;

//------------------------------------------------------------
                     razbmodel(ident,1);
			}
		}
      zwonok=(ttek4+takt);
	}//---------------------------------------------------
            //----  Теперь полное прочесывание пенала:
                  raskrutka(k);  //конец работы с пеналом.
			//-----------------------------------------
    //--- Вставка для г.Нахаева:
				  if(NAXAEW!=0)  //Заказано ли импульсное прерывание в ком.файле?
	{
        impulsNAXAEW();
	    	NAXAEW=0;
	}//----------------------- 


 //---- Обработка запросов по центральному контроллеру прер kpr:
				   if((020&k->axtung)!=0)
			{  wnzapkpr(k);
			    k->axtung=(020^(k->axtung));	
			}
	  	//----------- Конец обработки выполнения команды. *****
  //--- КОНЕЦ ИНИЦИАЦИИ КОМПЛЕКСА МОДЕЛИРОВАНИЯ ВНЕШНИХ УСТРОЙСТВ ----

                     return(0);          
}                                                                          //*********************************
	}//============  Конец  oper   ================================   


   void nasreset(int nm)
	   //Начальный мой Reset. nm - номер ВМ (мой).
   { ntip *k;            
   UINT rab,rab1,rcm,r; int i;
          
            tsmmot[nm-1]=0;    //Обнуление времени запуска ТС ММО.
    		
               k=bazuk[nm];
			   for(i=0;i<7;i++)  k->wds[i]=0;   //Чистка ВДЩi
	  //Выходы:
      k->jadro=0;  k->halt=0;
      k->config=0;                                                     
       //-------------------------------
	   k->cause=0;  k->status=0;                              
                                          //установ разрядов
       k->status=(030000000|(k->status)); //регистра Status.
       k->pc=0xbfc00000;  //установ адреса первой будущей команды.
       k->predper=0;  k->zapusl=0; k->axtung=0;
	  
	        k->tf=tsist; k->tpr=tsist; k->tzu=tsist; k->tsl=tsist;
	                                           k->tum=tsist; k->tdl=tsist;
         k->tpred=((tsist/1000)+0)*1000; k->tpred4=((tsist/4000)+0)*4000;
          k->tpred16=((tsist/16000)+0)*16000;  

	   k->sledk[0]=0; //Не было никакой команды, адреса 0 не будет.
	   //k->sledk[4]=13; //break - для пропуска первой команды 
	                   //в Reset при счете времени wrema.
	   k->sleds[0]=0; //не было обращения к ЗУ.

	  for(rab=0;rab<=32;rab++) k->reg[rab]=0; //Чистка.
      
			for(rab=0;rab<=20;rab++)
	{ rab1=rab*(k->mtakt); k->dlit[rab]=(DWORDLONG)rab1;}
			 //Вычислили длительности по числу тактов.
	  k->kom=1;  //Будет далее только выполнение операций.

	   sbrostimeb1(k);      //Сброс таймеров типа 1
	   sbrkpr(k); //Сброс контроллера прерыв. типа 50
	   sbrosostkfu(k); //Сброс остатков KFU типа 51
	   sbrospk(k);     //Сброс - установ регистров ПК типа 52
	   sbrospwr(k);    //Сброс - установ регистров ПВР типа 60
	   sbroskmk(k);    //Сброс КМК
	   sbroskuk(k);    //Сброс КУК
	   sbrosmmo(k);  //Надо еще ввести номер ВМ.
       rcm=streg(149,0,0,0,5,k);  //RCM0
       r=nm-1; //Беликовский номер ВМ
	   r=(03&r); //Только 2 разряда   у  ММО  в регистре  RCMO
	   rcm=(r|rcm); //Приформировали номер ВМ в RCM0
       wpis(149,0,0,0,5,2,rcm,k); //Вписали в регистр 
	   fnomou(k);  //Установ адресов ОУ

       next=0;    //Пока нет nexti    //Обязательно ли?	      
        
   }//=============== Конец nasreset ==========================

   void son(void)
   {
	   potkl=0;
     printkl("# potkl=0   Я сплю");                             
		WaitForSingleObject(eProz, INFINITE); //Ожидание события eProz 
	  	 ResetEvent(eProz);   //Сброс события разбуживания eProz
	   potkl=1;

   }//=======================================================


short readCP0(int nr,short nom,UCHAR *a)
	   //Читает по его номеру -nr регистр CP0 из ВМ nom и пишет в память c "a".
	   // При удаче - возвращает 0.
   {UINT w,*uk; ntip *k;
    if((nom<1)||(nom>kolwm)) return(100);
	if((nr<0)||(nr>39))   return(nom);
	//---------------------------
         k=bazuk[(int)nom];
		 if(nr<32) w=k->zp[nr];
		 else
		 {     if(nr==32) w=k->epc;
		  else if(nr==33) w=k->status;
          else if(nr==34) w=k->lo;
          else if(nr==35) w=k->hi;
          else if(nr==36) w=k->badvaddr;
          else if(nr==37) w=k->cause;
		  else if(nr==38) w=k->config;
		     else         w=k->pc;
         }
		 uk=(UINT *)a; uk[0]=w;
		       return(0);

   }//========== Конец readCP0 ============================== 
short writeCP0(int nr,short nom,UCHAR *a)
      //Пишет  в регистр nr CP0 ВМ nom из памяти c "a". При удаче - возвращает 0.
   {ntip *k; UINT *uk,r;
    if((nom<1)||(nom>kolwm)) return(100);
	if((nr<0)||(nr>39))   return(nom);
	//---------------------------
      uk=(UINT *)a; r=uk[0];
         k=bazuk[(int)nom];
		 if(nr<32) k->zp[nr]=r;
         else
		 {     if(nr==32) k->epc=r;
		  else if(nr==33) k->status=r;
          else if(nr==34) k->lo=r;
          else if(nr==35) k->hi=r;
          else if(nr==36) k->badvaddr=r;
          else if(nr==37) k->cause=r;
		  else if(nr==38) k->config=r;
		     else         k->pc=r;
         }
                 return(0);
   }//================= Конец writeCP0 ======================== 

short readCP1(int nr,short nom,UCHAR *a)
	   //Читает по его номеру -nr регистр CP1 из ВМ nom и пишет в память c "a".
	   // При удаче - возвращает 0.
   {UINT w,*uk; ntip *k;
    if((nom<1)||(nom>kolwm)) return(100);
	if((nr<0)||(nr>32))   return(nom);
	//---------------------------
         k=bazuk[(int)nom];
		 if(nr<32) w=k->fgr[nr];
		 else      w=k->contrlstatus;
		 uk=(UINT *)a; uk[0]=w;
		       return(0);

   }//========== Конец readCP1 ============================== 


   short writeCP1(int nr,short nom,UCHAR *a)
	   //Пишет из памяти c "a" в регистр nr CP1 ВМ nom. При удаче - возвращает 0.
   { ntip *k; UINT *uk,r;
    if((nom<1)||(nom>kolwm)) return(100);
	if((nr<0)||(nr>32))   return(nom);
	//---------------------------
	uk=(UINT *)a; r=uk[0];
         k=bazuk[(int)nom];
		 if(nr<32) k->fgr[nr]=r;
		 else      k->contrlstatus=r;
		       return(0);

   }//========== Конец writeCP1 ============================== 


   short readb6(UINT adr,short nom,UCHAR &b)      
	//Читает байт по программному адресу adr из модели ЗУ
	//ВМ nom, пишет его в b. 
	//Возвращает: 0 - при успехе, иначе nom или 100.
 {  UINT ad,sisl,sdw,*ud,ind,adf; char *uc; //short ot;
     ntip *k;
	       if((nom==1)&&(kolwm>=1 )) k=bazuk[1];
	  else if((nom==2)&&(kolwm>=2 )) k=bazuk[2];
	  else if((nom==3)&&(kolwm>=3 )) k=bazuk[3];
	  else if((nom==4)&&(kolwm>=4 )) k=bazuk[4];
      else if((nom==5)&&(kolwm>=5 )) k=bazuk[5];
	  else if((nom==6)&&(kolwm>=6 )) k=bazuk[6];
	  else  {printkl("#F - writeb6 - Ошибка выбора ВМ."); 
	         if(nom!=0) return(nom); else return(100);
			}
			  ad=(037777777774&adr);  //Прогр.адрес всей ячейки
	          sdw=03&adr;     //sdw - расположение мл.байта в ячейке/0,1,2,3/. 
	    
     adf=PtoF(ad); ind=indpam(adf,k); if(ind==1)
	 {printkl("#  readb6. Ошибка индекса"); return(100);}
                  sisl=k->mp[ind];


/*
          ot=sts2(ad,k);  //Прочитали всю ячейку в sislo - в переменную файлов zakr и mwu.      
                    if(ot!=0)    //Бяка при чтении.
			  {soob.sprintf("# readb6. Ошибка чтения по адресу %x в ВМ %d ot=%d. Отказ.",adr,(nom-1),ot);
			    printkl(soob);

				return(100);
			  }//---------------
                            sisl=sislo;	  

*/
   if(sdw==1) sisl=(sisl>>8);
   else if(sdw==2) sisl=(sisl>>16);
   else if(sdw==3) sisl=(sisl>>24);
		  else { }
			   ud=&sisl; uc=(char *)ud; b=(*uc);
			   return(0);  	 

 }//=============== конец readb6 ========================


   short writeb6(UINT adr,short nom,UCHAR b)    //СКОРРЕКТИРОВАНА 03.06,05
		//Пишет байт b по программному адресу adr модели ЗУ ВМ nom.
	//Возвращает: 0 - при успехе, иначе - nom.
   {  UINT rab,ad,sdw,kdan,*ud; char *us; short ot;
      ntip *k;
	       if((nom==1)&&(kolwm>=1 )) k=bazuk[1];
	  else if((nom==2)&&(kolwm>=2 )) k=bazuk[2];
	  else if((nom==3)&&(kolwm>=3 )) k=bazuk[3];
	  else if((nom==4)&&(kolwm>=4 )) k=bazuk[4];
      else if((nom==5)&&(kolwm>=5 )) k=bazuk[5];
	  else if((nom==6)&&(kolwm>=6 )) k=bazuk[6];
	  else  {printkl("#F - writeb6 - Ошибка выбора ВМ."); 
	         if(nom!=0) return(nom); else return(100);
			}
      ad=(037777777774&adr); sdw=03&adr;
                //  sdw определяет байт записи.
          ot=sts2(ad,k);  //Прочитали всю ячейку в sislo - в переменную файлов zakr и mwu.      
                    if(ot!=0)    //Бяка при чтении.
			  {soob.sprintf("# 1. writeb6. Ошибка чтения по адресу %x в ВМ %d. Отказ.",adr,(nom-1));
			    printkl(soob); return(100);
			  }//---------------
                   kdan=sislo;  //Число переписали в kdan

                rab=0; ud=&rab; us=(char *)ud; (*us)=b; //Перенесли байт в rab. 
		if(sdw==0)                                            
			{ kdan=(037777777400&kdan);    kdan=(kdan|rab);}
	   else if(sdw==1){ kdan=(037777600377&kdan);
	                        rab=(rab<<8);  kdan=(kdan|rab);}
	   else if(sdw==2){ kdan=(037700177777&kdan);
	                        rab=(rab<<16); kdan=(kdan|rab);}

	   else { kdan=(077777777&kdan);
				            rab=(rab<<24); kdan=(kdan|rab);}

 //---- В kdan то, что надо перезаписать в ячейке по адресу ad.
                      sislo=kdan;
			ot=zas2(ad,k);		  
                    if(ot!=0)    //Бяка при записи.
			  {
				soob.sprintf("# 2. writeb6. Ошибка записи по адресу %x в ВМ %d. Отказ.",adr,(nom-1));
			    printkl(soob); return(100);
			  }//---------------
                    k->axtung=0xFFFFFFFF;
                  			return(0);  

   }//=============== конец writeb6 ========================== 


   int sasa6(void)   //Строит модели ЗУ и доп.памяти, пишет
      //в карты их указатели. Возвращает 0 при удаче, иначе             
      // - номер машины или 100
 {UINT ii,otwet,*mpp;  char *mm;

    bazuk[0]=(ntip *)0;  //Для несуществующей ВМ.
       // --------------------------------------------------- Для ВМ1 :
   mpp=modzu(zagn1);  if(mpp==NULL)
   { printkl("#F - ZAKR - Модель ЗУ ВМ0 не сформирована.Отказ.");
	     return(1);     
   }//-------------
   bazuk[1]=&karta1; bazuk[1]->mp=mpp; 
  // Теперь чистка регистровой зоны,
  //соответствующей данной модели памяти:
	 //Эта чистка выполняется по программным адресам.		   
              for(ii=0x9f600000;ii<=0x9f60001c;ii=ii+4)
	{ otwet=writes(ii,0,bazuk[1]);
				        if(otwet!=1)
		{printkl("#F - ZAKR - Отказ чистки регистровой зоны ВМ0.");
						return(1); 
		}
    }//Почистили регистровую зону.
       mm=mdop(rewu,bazuk[1]);  if(mm==NULL)
   { printkl("#F - ZAKR - Доп.модель ЗУ ВМ0 не сформирована.Отказ.");
	     return(1);     
   }//-------------
    bazuk[1]->mdp=mm; bazuk[1]->rw=rewu; 
	bazuk[1]->mtakt=mtakt[1]; //Внесли в карту
   //------ конец с одной ВМ .
          if(kolwm==1) return(0);
       // --------------------------------------------------- Для ВМ2 :
   mpp=modzu(zagn2);  if(mpp==NULL)
   { printkl("#F - ZAKR - Модель ЗУ ВМ1 не сформирована.Отказ.");
	     return(2);     
   }//-------------
   bazuk[2]=&karta2; bazuk[2]->mp=mpp;
  // Теперь чистка регистровой зоны,
  //соответствующей данной модели памяти:
	 //Эта чистка выполняется по программным адресам.		   
              for(ii=0x9f600000;ii<=0x9f60001c;ii=ii+4)
	{ otwet=writes(ii,0,bazuk[2]);
				        if(otwet!=1)
		{printkl("#F - ZAKR - Отказ чистки регистровой зоны ВМ1.");
						return(2); 
		}
    }//Почистили регистровую зону.
       mm=mdop(rewu,bazuk[2]);  if(mm==NULL)
   { printkl("#F - ZAKR - Доп.модель ЗУ ВМ1 не сформирована.Отказ.");
	     return(2);     
   }//-------------
    bazuk[2]->mdp=mm; bazuk[2]->rw=rewu;
	bazuk[2]->mtakt=mtakt[2]; //Внесли в карту
   //------ конец с одной ВМ .
	      if(kolwm==2) return(0);
       // --------------------------------------------------- Для ВМ3 :
   mpp=modzu(zagn3);  if(mpp==NULL)
   { printkl("#F - ZAKR - Модель ЗУ ВМ2 не сформирована.Отказ.");
	     return(3);     
   }//-------------
   bazuk[3]=&karta3; bazuk[3]->mp=mpp;
  // Теперь чистка регистровой зоны,
  //соответствующей данной модели памяти:
	 //Эта чистка выполняется по программным адресам.		   
              for(ii=0x9f600000;ii<=0x9f60001c;ii=ii+4)
	{ otwet=writes(ii,0,bazuk[3]);
				        if(otwet!=1)
		{printkl("#F - ZAKR - Отказ чистки регистровой зоны ВМ2.");
						return(3); 
		}
    }//Почистили регистровую зону.
       mm=mdop(rewu,bazuk[3]);  if(mm==NULL)
   { printkl("#F - ZAKR - Доп.модель ЗУ ВМ2 не сформирована.Отказ.");
	     return(3);     
   }//-------------
    bazuk[3]->mdp=mm; bazuk[3]->rw=rewu; 
	bazuk[3]->mtakt=mtakt[3]; //Внесли в карту
   //------ конец с одной ВМ .
	    if(kolwm==3) return(0);
       // --------------------------------------------------- Для ВМ4 :
   mpp=modzu(zagn4);  if(mpp==NULL)
   { printkl("#F - ZAKR - Модель ЗУ ВМ3 не сформирована.Отказ.");
	     return(4);     
   }//-------------
   bazuk[4]=&karta4; bazuk[4]->mp=mpp;
  // Теперь чистка регистровой зоны,
  //соответствующей данной модели памяти:
	 //Эта чистка выполняется по программным адресам.		   
              for(ii=0x9f600000;ii<=0x9f60001c;ii=ii+4)
	{ otwet=writes(ii,0,bazuk[4]);
				        if(otwet!=1)
		{printkl("#F - ZAKR - Отказ чистки регистровой зоны ВМ3.");
						return(4); 
		}
    }//Почистили регистровую зону.
       mm=mdop(rewu,bazuk[4]);  if(mm==NULL)
   { printkl("#F - ZAKR - Доп.модель ЗУ ВМ3 не сформирована.Отказ.");
	     return(4);     
   }//-------------
    bazuk[4]->mdp=mm; bazuk[4]->rw=rewu; 
	bazuk[4]->mtakt=mtakt[4]; //Внесли в карту
   //------ конец с одной ВМ .
         if(kolwm==4) return(0);
      // --------------------------------------------------- Для ВМ5 :
   mpp=modzu(zagn5);  if(mpp==NULL)
   { printkl("#F - ZAKR - Модель ЗУ ВМ4 не сформирована.Отказ.");
	     return(5);     
   }//-------------
   bazuk[5]=&karta5; bazuk[5]->mp=mpp;
  // Теперь чистка регистровой зоны,
  //соответствующей данной модели памяти:
	 //Эта чистка выполняется по программным адресам.		   
              for(ii=0x9f600000;ii<=0x9f60001c;ii=ii+4)
	{ otwet=writes(ii,0,bazuk[5]);
				        if(otwet!=1)
		{printkl("#F - ZAKR - Отказ чистки регистровой зоны ВМ4.");
						return(5); 
		}
    }//Почистили регистровую зону.
       mm=mdop(rewu,bazuk[5]);  if(mm==NULL)
   { printkl("#F - ZAKR - Доп.модель ЗУ ВМ4 не сформирована.Отказ.");
	     return(5);     
   }//-------------
    bazuk[5]->mdp=mm; bazuk[5]->rw=rewu;
	bazuk[5]->mtakt=mtakt[5]; //Внесли в карту
   //------ конец с одной ВМ .
         if(kolwm==5) return(0);
      // --------------------------------------------------- Для ВМ6 :
   mpp=modzu(zagn6);  if(mpp==NULL)
   { printkl("#F - ZAKR - Модель ЗУ ВМ5 не сформирована.Отказ.");
	     return(6);     
   }//-------------
   bazuk[6]=&karta6; bazuk[6]->mp=mpp;
  // Теперь чистка регистровой зоны,
  //соответствующей данной модели памяти:
	 //Эта чистка выполняется по программным адресам.		   
              for(ii=0x9f600000;ii<=0x9f60001c;ii=ii+4)
	{ otwet=writes(ii,0,bazuk[6]);
				        if(otwet!=1)
		{printkl("#F - ZAKR - Отказ чистки регистровой зоны ВМ5.");
						return(6); 
		}
    }//Почистили регистровую зону.
       mm=mdop(rewu,bazuk[6]);  if(mm==NULL)
   { printkl("#F - ZAKR - Доп.модель ЗУ ВМ5 не сформирована.Отказ.");
	     return(6);     
   }//-------------
    bazuk[6]->mdp=mm; bazuk[6]->rw=rewu;
	bazuk[6]->mtakt=mtakt[6]; //Внесли в карту
   //------ конец с одной ВМ .
         if(kolwm==6) return(0);
    //----------------------------------------------------------------
                          return(100);
 }//================== КОНЕЦ sasa6 ===================================


    UINT *ukaz(UINT adr,short nom)  //Возвращает указатель на ячейку
		//с физическим адресом  adr  в модели ВМ  nom.
		//При неудаче возвращает 0xFFFFFFFF.
{UINT ind,*uk;
             if((nom==1)&&(kolwm>=1))
	{ind=indpam(adr,bazuk[1]); if(ind==1) 
	 {printkl("#F Ошибка адреса в ukaz для ВМ1. Отказ."); uk=(UINT *)0xFFFFFFFF;
			 return(uk);}
     uk=&(bazuk[1]->mp[ind]);
	}//---------------------------------
        else if((nom==2)&&(kolwm>=2))
	{ind=indpam(adr,bazuk[2]); if(ind==1) 
	 {printkl("#F Ошибка адреса в ukaz для ВМ2. Отказ."); uk=(UINT *)0xFFFFFFFF;
		return(uk);}
     uk=&(bazuk[2]->mp[ind]);
	}//--------------------------------
        else if((nom==3)&&(kolwm>=3))
	{ind=indpam(adr,bazuk[3]); if(ind==1) 
	 {printkl("#F Ошибка адреса в ukaz для ВМ3. Отказ."); uk=(UINT *)0xFFFFFFFF;
		return(uk);}
     uk=&(bazuk[3]->mp[ind]);
	}//---------------------------------
       else if((nom==4)&&(kolwm>=4))
	{ind=indpam(adr,bazuk[4]); if(ind==1) 
	 {printkl("#F Ошибка адреса в ukaz для ВМ4. Отказ."); uk=(UINT *)0xFFFFFFFF;
	   return(uk);}
     uk=&(bazuk[4]->mp[ind]);
	}//---------------------------------
       else if((nom==5)&&(kolwm>=5))
	{ind=indpam(adr,bazuk[5]); if(ind==1) 
	 {printkl("#F Ошибка адреса в ukaz для ВМ5. Отказ."); uk=(UINT *)0xFFFFFFFF;
	   return(uk);}
     uk=&(bazuk[5]->mp[ind]);
	}//---------------------------------
       else if((nom==6)&&(kolwm>=6))
	{ind=indpam(adr,bazuk[6]); if(ind==1) 
	 {printkl("#F Ошибка адреса в ukaz для ВМ6. Отказ."); uk=(UINT *)0xFFFFFFFF;
	   return(uk);}
     uk=&(bazuk[6]->mp[ind]);
	}//---------------------------------
//-------------------------------------------------------------------------------

      else {printkl("#F Ошибка номера ВМ в ukaz.Отказ."); uk=(UINT *)0xFFFFFFFF;
	  return(uk);}

                       return(uk);
}//============= конец ukaz =======================================


  void jmpkl(short nom)         //Для романовского jump
  {
      bazuk[nom]->predper=0;
    //  prslot[nom]=0;          //Порвали с прошлым ВМ - nom.
      next=0;
  }//====================================================



    DWORDLONG wrrom(void) //Выдает время Романову в м/с.
	{   
      return(tsist/1000);
	}
  //==================================================


	DWORDLONG wrema(ntip *k)      //Считает время.
{DWORDLONG t1,t2,t3,ts,tb;//Возвращ. 0, если команда вне времени.
	 UINT n;              //Иначе возвр. время текущее в нанах.
	 
//	 if(stopwr==0) return(0);
	 //--------------------------------------------------
	 // Теперь - только целевая команда.

// Определим моменты оконч.чтения команды /t1/ и освобожд.ОЗУ /tzu/:       
        if(k->sledk[1]==0)            t1=(k->tpr)+(k->mtakt);
		else if(k->sledk[1]==2) //некешир.   
        {t3=(DWORDLONG)(k->sledk[2]); t1=(k->tpr)+t3+(k->mtakt);
		 if(k->tzu>k->tpr) k->tzu=k->tzu+(t3+k->mtakt);
		 else k->tzu=t1;
		} 
		else    //был промах при чтении команды из кешир.зоны.
        {t2=(DWORDLONG)(k->sledk[2]); t3=t2+t2; t1=k->tpr+t2;
         if(k->tzu>k->tpr) k->tzu=k->tzu+t3;
         else k->tzu=k->tpr+t3; 
		}
	     //********** Определили ****************************
                 //Операция CP0  без обращения к памяти:
           if((k->sledo[0]==0)&&(k->sleds[0]==0)) 
			          
		   {k->tpr=t1;  return(k->tpr);}
		   //**************************************************
		         //Операция с чтением из КЕШ без промаха:
            if((k->sleds[0]==1)&&(k->sleds[1]==0))
            {k->tpr=t1;  return(k->tpr);}
			//*************************************************
			if(k->sleds[0]==1)     //Операция с чтением числа.
		{//Определим длительность ts чтения :    
	 if(k->sleds[1]==0) ts=0; //из КЕШ без промаха,-избыточно!
		 else if(k->sleds[1]==2) ts=(DWORDLONG)(k->sleds[2]);
		                                        //из НЕКЕШ.
         else //был промах.
         {t2=(DWORDLONG)(k->sleds[2]);
		 if(k->sleds[1]!=1) printkl("# Ошибка Клямко - 1"); 
		   if(k->sleds[3]==1) ts=t2; else ts=t2+t2+t2;                 
		 } //------ Определили ---------------------  
                 if(k->tzu>=t1)
				 {k->tzu=k->tzu+ts; k->tpr=k->tzu; }
				 else
                 {k->tzu=t1+ts; k->tpr=k->tzu; }
                   return(k->tpr);
	}//********** Конец всех возможных чтений.*****************
			if(k->sleds[0]==2)   //Операция с записью числа.
	{ts=(DWORDLONG)(k->sleds[2]);     //длительность записи.
      tb=4*ts;       if(k->sledo[0]==0)   //Операция CP0
	 {if(k->tzu>=t1) k->tzu=k->tzu+ts; else k->tzu=t1+ts;
       if((t1+tb)>k->tzu) k->tpr=t1; //буфер не переполнен.
	   else k->tpr=k->tzu-tb;        //буфер был переполнен.
                  return(k->tpr);
      }//-- Теперь запись с регистра CP1:
          n=k->sledo[2]; if(n==100)   //n - номер регистра CP1
	{printkl("# Ошибка номера регистра при записи с него в ОЗУ.");
         soob.sprintf("# Код команды 16-тиричн. - %x",k->sledk[4]);
         printkl(soob);
		  }
       t2=k->reg[n];//Момент готовности числа от предыд.операции.
         if(t2>t1) t3=t2; else t3=t1;//t3 - момент начала записи.
		      if(k->tzu<t3)          
{k->tzu=t3+ts; k->tpr=t3; }//буфер пуст в момент начала записи. 
               else
{k->tzu=k->tzu+ts; if((t3+tb)>k->tzu) k->tpr=t3;//не переполнен.
			      else k->tpr=k->tzu-tb; //мог быть переполнен.
	}
			         return(k->tpr);
	}//********** Конец всех записей. **************************

    //****** Осталась только операция  CP1  без обращения к ОЗУ.

				 if(k->sledo[0]==1)    //Не требует ресурсов АУ. 
{//Определим момент начала операции по готовности операндов:
   t3=t1; n=k->sledo[2]; if(n!=100) //t3 - будет моментом начала.
       {t2=k->reg[n]; if(t2>t3) t3=t2; }
              n=k->sledo[3]; if(n!=100)
       {t2=k->reg[n]; if(t2>t3) t3=t2; }
          //В t3 теперь момент начала операции.
        ts=k->dlit[k->sledo[1]];  //Длительность операции.
		 n=k->sledo[4]; if(n!=100)  
		k->reg[n]=t3+ts;  //Момент готовности выходного регистра.
          k->tpr=t3; return(k->tpr);
	}//*******************************Конец безресурсной операции.


				 if(k->sledo[0]==5)    //требует ресурса сложителя. 
	{//Определим момент начала операции по готовности операндов:
     t3=t1; n=k->sledo[2]; if(n!=100) //t3 - будет моментом начала.
       {t2=k->reg[n]; if(t2>t3) t3=t2; }
              n=k->sledo[3]; if(n!=100)
       {t2=k->reg[n]; if(t2>t3) t3=t2; }
			  if(k->tsl>t3) t3=k->tsl;  //Учли занятость сложителя.
          //В t3 теперь момент начала операции.
        ts=k->dlit[k->sledo[1]];  //Длительность операции.
		 n=k->sledo[4]; if(n!=100)  
		  k->reg[n]=t3+ts;  //Момент готовности выходного регистра.
          k->tsl=t3+ts;     //Момент освобождения сложителя. 
          k->tpr=t3;
		  return(k->tpr);
	}//Конец операции, не сложения, но требующей ресурса сложителя.
     //************************************************************      

                 if(k->sledo[0]==2)    //сложение.
	{//Определим момент начала операции по готовности операндов:
       t3=t1; n=k->sledo[2];  //t3 - будет моментом начала.
       t2=k->reg[n]; if(t2>t3) t3=t2; 
              n=k->sledo[3]; 
       t2=k->reg[n]; if(t2>t3) t3=t2; 
			  if(k->tsl>t3) t3=k->tsl;  //Учли занятость сложителя.
          //В t3 теперь момент начала операции.
        ts=k->dlit[k->sledo[1]];  //Длительность операции.
		 n=k->sledo[4];   
		  k->reg[n]=t3+ts;  //Момент готовности выходного регистра.
          k->tsl=t3+ts;     //Момент освобождения сложителя. 
          k->tpr=t3;
		  return(k->tpr);
	}//****************************************** Конец сложения.

                 if(k->sledo[0]==3)    //умножение.
	{//Определим момент начала операции по готовности операндов:
       t3=t1; n=k->sledo[2];  //t3 - будет моментом начала.
       t2=k->reg[n]; if(t2>t3) t3=t2; 
              n=k->sledo[3]; 
       t2=k->reg[n]; if(t2>t3) t3=t2; 
			  if(k->tum>t3) t3=k->tum;
			                        //Учли занятость умножителя.
          //В t3 теперь момент начала операции.
        ts=k->dlit[k->sledo[1]];  //Длительность операции.
		 n=k->sledo[4];   
		  k->reg[n]=t3+ts;//Момент готовности выходного регистра.
          k->tum=t3+ts;   //Момент освобождения умножителя. 
          k->tpr=t3;
		  return(k->tpr);
	}//*************************************** Конец умножения.


                 if(k->sledo[0]==4)    //деление.
	{//Определим момент начала операции по готовности операндов:
       t3=t1; n=k->sledo[2];  //t3 - будет моментом начала.
       t2=k->reg[n]; if(t2>t3) t3=t2; 
              n=k->sledo[3]; 
       t2=k->reg[n]; if(t2>t3) t3=t2; 
			  if(k->tdl>t3) t3=k->tdl;//Учли занятость делителя.
          //В t3 теперь момент начала операции.
        ts=k->dlit[k->sledo[1]];  //Длительность операции.
		 n=k->sledo[4];   
		  k->reg[n]=t3+ts;//Момент готовности выходного регистра.
          k->tdl=t3+ts;     //Момент освобождения делителя. 
          k->tpr=t3;
		  return(k->tpr);
	}//*********************************** Конец деления.

     return(0);  //Обман транслятора.
	} //Конец wrema.
  //==================================================

 
	short stzuda(UINT adr,ntip *k,short p)
//Читает из модели ЗУ k->mp число по физическ.адресу.
//При p=1 пишет в sisl, возвращая 0. 
//Возвращает 1 при неудаче /ошибка шины/.
// Если p=0, то пишет только код  в kdan, в sisl не пишет!
	{	UINT *m,in,rab;
	       m=k->mp;   //Массив модели ЗУ.
		          for(in=1; ;in=(in+8))
		{ if((adr>=m[in])&&(adr<m[in+1])) //Нашли кусок памяти.
		{ rab=adr-m[in]; rab=(rab>>2); 
		 rab=(rab+m[in+3]); rab=(m[rab]); //Взяли саму ячейку. 
				  if(p==0) { kdan=rab; return(0); }
				   sisl=rab; 
				   return(0);
				 }
               if(m[in+2]==1)
			   { //данные. непрочитаны.
			      return(1);
			   }
			}
	}//======================================================

	short zapzuda(UINT adr,ntip *k)
//Записывает в модель ЗУ  sisl по физическому адресу adr,
//возвращая 0. Возвращает 1 при неудаче /ошибка шины/.
	{	UINT *m,in,rab;
	       m=k->mp;   //Массив модели ЗУ.
		          for(in=1; ;in=(in+8))
	{ if((adr>=m[in])&&(adr<m[in+1]))    //Нашли кусок памяти.
				{ rab=adr-m[in]; rab=(rab>>2); 
				  rab=(rab+m[in+3]); //Это индекс в модели.           

                  //Теперь пишем:
				  m[rab]=sisl; 
				   return(0);
				 }
               if(m[in+2]==1)
			   { //данные. непрочитаны.
			      return(1);
			   }
		}
	}//======================================================


   short writes(UINT adr,UINT a,ntip *k)                                                               
	//Пишет слово по программному адресу adr модели ЗУ,
	//беря его из  a.  k- указатель на карту процессора
	//Возвращает 1 при успехе, иначе - 0.
   {  UINT ad;  short kesk,ot;
        if((03&adr)!=0)   return(0);    //Адрес не кратен 4.
//-----------------------------------------------------
		ad=PtoF(adr);
		if((0xa0000000<=adr)&&(0xbfffffff>=adr)) kesk=0; else kesk=1;

	   
//----------- ad - физич адрес, kesk - признак кешир.области.
             if(kesk==0)                                              
{ sisl=a; ot=zapzuda(ad,k); if(ot==1) return(0); else return(1);}
// Теперь осталось инволюдизировать КЕШ. От изоляции не зависим.   
          if(kesk!=0) 
	{ sisl=a; ot=zapzuda(ad,k); if(ot==1) return(0);
		             else {invol(ad,k); return(1);}}
	  return(1); //Обман транслятора.		  
   }//=======================================================


     void invol(UINT adr,ntip *k)  
		 //Инволюдизирует КЕШ ком.и данн.по адресу adr.
    { DWORDLONG *mk,*md; short rk,rd,r;
	    UINT ml,*u;
          // Установ размеров блоков КЕШ:
                       if((040000000&k->config)==0)
				{ rd=1; rk=4; }	    else { rd=2; rk=2; }
		// Учет возможного переключения блоков КЕШ:
	if((0400000&k->status)==0)  { mk=k->kespk; md=k->kespd; }
		 else { mk=k->kespd; md=k->kespk; r=rd; rd=rk; rk=r; }
//--Теперь КЕШ-дан.это md[], его размер в тыс.адресов=rd /1,2 или4/.
//--Теперь КЕШ-ком.это mk[], его размер в тыс.адресов=rk /1,2 или4/.
		   // ---Работаем сначала с данными:
		   ml=adr>>2; if(rd==1) {  ml=(01777&ml); }
                 else if(rd==2) {  ml=(03777&ml); }		   
                        else    {  ml=(07777&ml); }
           //--- Получили:  ml адрес обращения в КЕШ-дан.
          u=(UINT *)(&md[ml]); //Вышли на нужную ячейку КЕШ.
  u[1]=0; //Все вписали. 0 тега и 0 действительности для КЕШ-дан.
      //-----Теперь то же для КЕШ-ком:
		   ml=adr>>2; if(rk==1) {  ml=(01777&ml); }
                 else if(rk==2) {  ml=(03777&ml); }		   
                        else    {  ml=(07777&ml); }
           //--- Получили:  ml адрес обращения в КЕШ-ком.
      u=(UINT *)(&mk[ml]); //Вышли на нужную ячейку КЕШ.
  u[1]=0; //Все вписали. 0 тега и 0 действительности для КЕШ-ком.
			  
			             return;
	 }//=======================================================


  void widser(void)
  //Выдача данных по мах.временам запретов прерывания
  {QString s; int i; DWORDLONG rab; short p;
     int j; ntip *k;


     printkl("# Максимальные времена запретов прерывания в дес.системе в мкс:");
                  for(j=1;j<=kolwm;j++)
{          if(Komplex[j]==0) continue; 
                k=bazuk[j];
   s.sprintf("# -----------------------  ЭВМ  %d :  ",(j-1)); printkl(s);				

	            for(i=0;i<8;i++)
	{  if(k->sernas[i]==0) {rab=k->sermax[i]; p=0;}
       else
	   {rab=(4000*ttek4)-(k->sernas[i]);
	      if(rab<(k->sermax[i])) {rab=k->sermax[i]; p=1;} //есть запрет меньшей длит.
		  else p=2; //продолжается мах.период
       }//--------------------------------
                      rab=(rab/1000); //в мкс
        s.sprintf("#  Status разряд %d  -  %lu",(i+8),rab); printkl(s);
if(p==1) printkl("#                      Продолжается новый запрет меньшей длительности.");           
if(p==2) printkl("#                      Указанный максимальный период еще продолжается.");


	}
}

    printkl("#---------------------------------------------------------- КОНЕЦ СООБЩЕНИЯ");

  }//==================== Конец widser ========================


  void sernull(void)
	  //Чистка массивов для выдачи Сергею мах.запретов прерыв.
  {	  int i,j; ntip *k; 
              
            for(j=1;j<=kolwm;j++)
{         if(Komplex[j]==0) continue;
                   k=bazuk[j];
                 for(i=0;i<8;i++)
		{k->sernas[i]=0;  k->sermax[i]=0;}           
}
	
  }//======================== Конец sernull ================== 



//********* Model's DLL Loading *********************************
static long int load_pkm_dll(void)
  {

  hInstDll = LoadLibrary(QString("pkm.dll").toStdWString().c_str());
  if (hInstDll == NULL)
	   {
printkl ("#E - ZAKR - ERROR - PKMMonitor- Can't load PKM.DLL");
// ::MessageBox(NULL,"Can't load PKM.DLL","DLL", MB_OK);
	  return 0;
      }

  typedef struct mod_table* (*ffp)(reg_table *);
  ffp InitPKM = (ffp)GetProcAddress(hInstDll,"init_pkm");
  if (InitPKM == NULL)
	  {
printkl ("#E - ZAKR - ERROR - PKMMonitor- Can't find init_pkm in pkm.dll");
//::MessageBox(NULL,"Can't find init_pkm","DLL", MB_OK);
       return 0;
     }
  pModTable =(*InitPKM)(pRegTable);

  return 1;
   }

//*********  End - free and close *******************************
//------------------------------------------------------------------



//ПРИМЕЧАНИЕ 1.
//Функции sts2 и zas2 в этом файле - собственные для zakr и mwu.
//Они служат для записи и чтения байта по readb6 и writeb6.
//Аналогично с функциями stzudan и zapzudan, используемыми этими
//функциями. В них вытравлены фиксация следов и блокировка записи в ПЗУ.
//Все - только для Романова.
//Их тезки sts, zas, stzudan, zapzudan в файле proz
// только для исполнения при работе моделируемой программы.

//ПРИМЕЧАНИЕ 2. При божественных чтениях и записях по
// readb6 и writeb6 разрешается запись пользователя (Романова)
//в область ядра. Для этого в STS2 и ZAS2 разрешаются  действия 
//пользователя в области ядра как "божественные" (снят
//запрещающий контроль).



