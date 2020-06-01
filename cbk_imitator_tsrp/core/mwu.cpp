//--------------------------------------------------------------------
//
//	mko, pwr Models for TSRP and PKM Call
//	mvu.
//	author: Klyamko E.I.
//          ****************   ФАЙЛ  MWU  ЦБК    с новым ПВР и с ММО 
//---------------------------------------------------------------------
#include "PKM.h"
extern short pesrazb;
extern short pesmko;
extern short pestsinxr; 
extern short pesretr;   
extern short pesmw;     
extern short pesrel;    
extern short pestelem;  
extern short pesdll;    
extern short pesouou;   
extern short pesosib;  
extern short nedop;    
extern short pesmmo;
extern short osibpwr;
extern short peskuk;      
//---------------------
    //REASON_HIM  не используется
#define rpenal 500                           //Размер пенала

int ekran[5];

UINT otl=0;                        //КАРТА ПРОЦЕССОРА:  

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
	 DWORDLONG tq;     //Время выработки обобщенного признака синхронизации Q.
	 UINT adrupr[6]; //Адреса управл.слов в таблице для каждого КМК
	 UINT bufou[220];
	 UINT pak0,pak1,pak2,pak3,pak4,pak5;
	 DWORDLONG tnasper[6]; //Времена начала передач по МКО
	 USHORT gt0;  //готовности команд КУК
     USHORT gt1;
	 USHORT gt2;
     USHORT gt3;		 
   } ntip;                            //КОНЕЦ КАРТЫ ПРОЦЕССОРА.
 

  //---   УСТАНОВОЧНАЯ     ИНФОРМАЦИЯ  ----------

   extern UINT rewu[];     //Массив адресов регистров внешних устройств.

   extern UINT nomou[ ];  //Начальный массив адресов КМК-ОУ на магистралях МКО.(Описан в zakr.cpp).

// Модель реальной памяти  zagn[]  описывается в zakr.cpp. 

 //--------- КОНЕЦ УСТАНОВОЧНОЙ ИНФОРМАЦИИ ---------



    //------------------- ПЕРЕМЕННЫЕ -------------------------   
	

   UINT indstreg; //Для индекса регистра-числа 
                  // при чтении его по streg( ).
   extern UINT stopwr;    //Для приказа  (от Тихонова) не считать время
   extern UINT delt,delt4; 
   extern UINT delt16;
   extern DWORDLONG ttek4; //Это системное время. Ед.измерения - 4 мкс.
   extern QString soob;
   extern struct mod_table *pModTable;
   extern struct reg_table *pRegTable, RegTable;
   UINT *vxod,*vixod; //Для указателей входного и выходного буфера модели.
   UINT ident;       //Для идентификационного номера модели.
   void (*ukf)(void);  //Для указателя на функцию модели
   extern UINT pes;      //Атавизм
   int penal[rpenal]={0};
   int ipen0;      //Индекс первой свободной ячейки пенала.
   int maxpr;      //Максим.приоритет в пенале.
   UINT indmod;    //Для индекса внешней модели в таблице моделей (Table).
   UINT grup;      //Признак группового обмена без ОС.
    
   extern	DWORDLONG zwonok;  //Время тактового разбуживания
   extern	UINT wkltakt;      //Признак включения тактового разбуживания
   extern	UINT takt;
   extern short dlln; //Признак наличия DLL
   extern UINT wus[1600];  // Это для восьмерок (объектов), т.е. сам будильник вместе с bud[] и zraz[].  
   DWORDLONG bud[200]={0}; //Для записи времен разбуживания объекта по номеру его восьмерки в wus[].
   UINT zraz[200]={0};     //дельта циклического разбуживания по индексу = номеру модели.
   extern DWORDLONG Tmin;  //Для минимального времени в будильнике
   
   
   extern DWORDLONG tsist; //системное время в н/с
   extern ntip *bazuk[7];  //Массив указателей на карты процессоров. Индекс - мой номер ВМ.
   extern UINT Komplex[];  //признаки включенной (1) ВМ. Индекс - мой номер ВМ.
   extern short kolwm;    //количество машин от 1 до kolwm.
   extern int nomerWM;  //Номер (мой) ВМ, выполняющей сейчас операцию в oper().

   DWORDLONG mag[60]={0};   //Состояния магистралей ММО
   //по 15 ячеек на магистраль. Первые 10 информационные. Остальные 5 резерв.
   //Десятка - из 2-х пятерок. Певая пятерка для информ.слова, вторая для команды.
   //В пятерке последовательно: мой номер ВМ, код на магистрали, время начала выдачи, 
   //время завершения выдачи, признак продолжения выдач (мой номер ВМ или 0, если нет).
   //Нумерация ВМ(магистралей) беликовская. Для ВМ-i начальный индекс в mag[] - 15*i. 
   int konwid;    //Признак конца выдачи информ.слов.
              //-- Ниже - для ТС ММО:
   DWORDLONG tsmmot[4]={0}; //Время срабатывания ТС ММО по номеру(беликовск) ВМ или 0.
   UINT prizqc[4]={0}; //Признак появления Qc в ВМ (по номеру(беликовск) ВМ). НЕ ИСПОЛЬЗУЕТСЯ
   UINT mmobozu;       //Для работы при межмашобмене.
   extern short phim;  //Для заказа моделью обмена с ОЗУ.
   UINT sislo;
   UINT adkom;
   UINT kdan;
   USHORT NETmag; //Признак нет мажоритирования (1 значит нет)

   UINT NAXAEW=0; //Признак реализации импульсного прерывания (тогда не равен 0)

    //-----------  ОБ'ЯВЛЕНИЯ ФУНКЦИЙ  ----------

	UINT indpam(UINT adr,ntip *k); //Возвращает индекс по адресу.
	char * mdop(UINT *rwu,ntip *k);//Строит дополнение модели ЗУ.
    extern void printkl(QString s);       
    void belt(UINT in,UINT it,char ss,UINT sisl,UINT *uk,
		                            UINT pornom,ntip *k);
    void wpis(UINT tip,UINT pornom,UINT podtip,UINT nblok,
		                 UINT it,UINT d,UINT sis,ntip *k);
	UINT streg(UINT tip,UINT pornom,UINT podtip,UINT nblok,
		                                  UINT it,ntip *k);
	void  stbelt(ntip *k);
	void sttmsi(ntip *k);
	void mwrem(UINT n);
	void sbrostimeb1(ntip *k);
 void kpr(UINT in,UINT it,char ss,UINT sisl,UINT *uk,ntip *k);    
 void pwr(UINT in,UINT it,char ss,UINT sisl,UINT *uk,ntip *k);
  UINT xitpr(UINT star,UINT now,UINT zam,UINT soxr,UINT xit0,
	                            UINT xit1,UINT sistka,UINT n);
  void ostkfu(UINT in,UINT it,char ss,UINT sisl,
	   UINT *uk,ntip *k); 
  void wnu(UINT adr,char ss,UINT sisl,UINT *uk,ntip *k); 
  void kmk(UINT in,UINT it,char ss,UINT sisl,UINT *uk,
	  UINT pornom,ntip *k);
  void pk(UINT pornom,UINT in,UINT it,char ss,UINT sisl,
	  UINT *uk,ntip *k);
  void progrper(ntip *k);
  void rtt(ntip *k);
  void dispet(void);
  void wnzapkpr(ntip *k);
  void sbrkpr(ntip *k);  
  void sbrosostkfu(ntip *k);
  void sbrospk(ntip *k);
  void sbrospwr(ntip *k);
  void sbroskmk(ntip *k);
  void sbroskww(UINT pornom,ntip *k);
  void zappak(UINT pornom, ntip *k);
  UINT adroo(UINT pornom,ntip *k);
  UINT adrbs(UINT adr,ntip *k);
  DWORDLONG dlit(UINT adr,ntip *k);
  void stoppak(UINT pornom, ntip *k);
  void pereust(UINT pornom,ntip * k);
  void kpr_rfm(ntip *k);
  void kpr_pwr(ntip *k);
  void rqn_rfm_pk(UINT pornom,ntip *k);
  void kmka(short pornom,short pr,ntip *k);
  void zakaz(UINT rbl);
  short konper(UINT *mm,UINT ind,UINT otw,UINT nou,
	                  UINT r0,short pornom,ntip *k);
  short konper2(UINT *mm,UINT ind,UINT nou,UINT r0,
	                           short pornom,ntip *k);
  UINT nreg(UINT tip,UINT pornom,UINT podtip,
	                         UINT nblok,UINT it,ntip *k);
  void bozu(UINT pornom,UINT adr,char ss,UINT sisl,UINT *uk,ntip *k);
  void razb(UINT z);
  void impuls(UINT z);
  void potenz(UINT z);
  void snatp(UINT z);
  void raskrutka(ntip *k);
  void widtsinxr(ntip *k);
  void razbmodel(UINT iden,short w);
  void retransl(int bit,ntip *k);
  void telemetr(ntip *k);
  void widrel(UINT sisl,ntip *k);
  void snatrel(UINT sisl,ntip *k);
  void fnomou(ntip *k);
  UINT uprslowo(UINT pornom,UINT komslowo,ntip *k);
  UINT dopkomsl(UINT pornom,UINT komslowo,ntip *k);
  void kontrmod(UINT pornom,UINT komslowo,UINT indm,UINT zz,ntip *k);
   short konperou(UINT *mm,UINT ind,UINT otw,UINT nou,
	   UINT r0,short pornom,ntip *k);
  void kmkaou(UINT pornom,UINT indm,ntip *k);
  void rabts(UINT n,UINT delt,ntip *k);
  void obrts(ntip *k);
  void obrrqp(ntip *k);
  void obrrqi(ntip *k);
  void tspwr(UINT in,UINT it,UINT sisl,ntip *k);
  void widtsinxr1(ntip *k);
  int reas(int wm,int ts);
  void mwrts(UINT t,UINT mw,ntip *k);
  void uzwid(UINT in,UINT it,char ss,UINT sisl,UINT *uk,ntip *k);
  void prermmo(int nm);
  void uzpriem(UINT in,UINT it,char ss,UINT sisl,UINT *uk,UINT pornom,ntip *k);
  void uztsinxr(UINT in,UINT it,char ss,UINT sisl,UINT *uk,ntip *k);
  void sbrosmmo(ntip *k);
  void sbroskuk(ntip *k);
  void widkom(int nm);
  void widslowo(int nm);
  void anmag(void);
  void srabtsmmo(void);
  void Qts(int nm);
  void polkom(int nm,USHORT sl);
  void polslowo(int nm,USHORT sl);
  void kuk(int nm,USHORT sl);
  void komkuk(int nom);
  void bik(int nom,USHORT sl);
  void skuk(void);
  extern void nasreset(int nm);
    void him(UINT z);
	short sts2(UINT adr,ntip *k);
static	short stkesdan(UINT adr,ntip *k);
static	short stzudan(UINT adr,ntip *k,short p);
static	short zapkesdan(UINT adr,UINT kod,ntip *k);
short zas2(UINT adr,ntip *k);
static	short zapzudan(UINT adr,ntip *k);
extern UINT PtoF(UINT adr);
void impulsNAXAEW(void);
  //------------------------------------------------------


             //--------- ФУНКЦИИ --------------------

    //----ФУНКЦИИ   ЧТЕНИЯ И ЗАПИСИ НА РЕГИСТРЫ  И  БОЗУ. --------
             //  Эти функции запускаются wnu().
//В этих функциях внутренний индекс регистра всегда обозначается через  n .
//Индекс его адреса в массиве  rw ( rewu) обозначен через - it.  
//Индекс адреса самого первого (начального) регистра в описании устройства (в rw) - in.
// (in , it   передаются этим функциям от  wnu). 
//Так что всегда в этих функциях  n=it-in.
//Индекс регистра в модели памяти обозначается здесь всегда как  ind.
  
                //---------------------------
  
  void belt(UINT in,UINT it,char ss,UINT sisl,UINT *uk,
		                              UINT pornom,ntip *k)
 //Пишет и читает регистр беликовского таймера, тип 1. 
 //in,it - индексы его адресов в k->rw:
 //in - начальный,it - текущий, собственный.
 //sisl - записываемое в регистр число.
 //pornom - порядковый номер таймера.
 //uk - указатель для записи прочитываемого значения.
 // k -   указатель карты процессора.
	{UINT ind, *m,*mm,adr,n,r0,r1,r2,r,ind0,ind1,dd;
      n=it-in; if(n>3)
	  {printkl("#F - MWU - Ошибка в номере регистра в belt."); 
	                                   ExitThread(TRUE); }
	   m=k->rw; adr=m[it]; ind=indpam(adr,k);
	 if(ind==1) 
{printkl("#F - MWU - Ошибка индекса в belt");ExitThread(TRUE);}
	   mm=k->mp;
	         if(ss=='s')    //Чтение регистра в uk[0].
		{r0=mm[ind]; if(n<2)
			{uk[0]=03777777&r0; return; }
			          else if(n==2)
			{uk[0]=034300000000&r0; return; }
		else {uk[0]=03777777&r0;
		if(pornom==0) {stopwr=1;
		/*printkl("# Время запущено чтением спецрегистра Беликова");*/ }		
		return;}
		}//---- Конец чтения.
			 else if(ss=='z')  //Запись в регистр.
		{
			if(n==0) return; //В RT нет записи
             else if(n==1)
			 {mm[ind]=03777777&sisl; return; } //Запись в RP
          else if(n==2)  //Запись в RC sisl через формируемый r.
			 { r=0; if((0100000000&sisl)!=0)   //off=1 в sisl.
				{r=0100000000|r;
		if(pornom==0) k->axtung=(037777777776&k->axtung);
        else if(pornom==1) k->axtung=(037777777775&k->axtung);
	    else if(pornom==2) k->axtung=(037777777773&k->axtung);
				 else  k->axtung=(037777777767&k->axtung);
				}
			      else    //off=0 в sisl.
			{
				 if(pornom==0) k->axtung=(1|k->axtung);
            else if(pornom==1) k->axtung=(2|k->axtung);
            else if(pornom==2) k->axtung=(4|k->axtung);
		           else  k->axtung=(010|k->axtung);
			}
			     //-----------------------------
			   if((0200000000&sisl)!=0) r=0200000000|r;
			                              //sys=1 в sisl.
                 //-----------------------------
			   if((02000000000&sisl)!=0)  //Start=1 в sisl.
			   {adr=m[in];   ind0=indpam(adr,k); //RT
	 if(ind0==1) 
{printkl("#F - MWU - Ошибка индекса ind0 в pwr");
	                                    ExitThread(TRUE);}
			    adr=m[in+1]; ind1=indpam(adr,k);
	 if(ind1==1) 
{printkl("#F - MWU - Ошибка индекса ind1 в pwr");
	                                    ExitThread(TRUE);}

				r1=mm[ind1]; //RP
			r1=03777777&r1; mm[ind0]=r1; //Переписали с RP в RT.
				//Восстановление активности axtung по Start:
				  if(pornom==0) k->axtung=(1|k->axtung);
             else if(pornom==1) k->axtung=(2|k->axtung);
             else if(pornom==2) k->axtung=(4|k->axtung);
				        else  k->axtung=(010|k->axtung);
			   }  //----------------------------
			   
			   if((010000000000&sisl)==0)    //Q=0 в sisl.
			   {     }    //Будет обнуление Q в RC через r.
			   else
	//1 в sisl в разряде Q,т.е.сохраняется старое Q в RC
			   {r2=mm[ind]; 
			   if((010000000000&r2)!=0) r=010000000000|r;} 
			                         //Сохран старое Q в r.	   .
                   //---------------------------
			   if((04000000000&sisl)!=0)    //M=1 в sisl.
			   { r=04000000000|r;
              if(pornom==0)      wpis(50,0,0,0,5,1,0200000,k);
         else if(pornom==1) wpis(50,0,0,0,5,1,01000000,k);
         else if(pornom==2) wpis(50,0,0,0,5,1,04000000,k);
         else if(pornom==3) wpis(50,0,0,0,5,1,020000000,k);
 else {printkl("#F - MWU - Ошибка 1 с номером таймера в belt.");
				                  ExitThread(TRUE);}
			   } //------------------------------
                  else   //M=0 в sisl.
	{if((010000000000&r)!=0) dd=0; else dd=1;				  
            if(pornom==0)      wpis(50,0,0,0,5,dd,0200000,k);
       else if(pornom==1) wpis(50,0,0,0,5,dd,01000000,k);
       else if(pornom==2) wpis(50,0,0,0,5,dd,04000000,k);
       else if(pornom==3) wpis(50,0,0,0,5,dd,020000000,k);
else {printkl("#F - MWU - Ошибка 2 с номером таймера в belt.");
		                            ExitThread(TRUE);}
				}//-------------------------------
                if((020000000000&sisl)!=0)   
		  //ErQ=1 в sisl.Сохраняется старое ErQ.
        {r2=mm[ind];
			if((020000000000&r2)!=0) r=020000000000|r;}
				                           //Сохранили.
		 else  //ErQ=0 в sisl.Оно сбрасыывается на регистре.
                {   }
				 //------------------------------- r готов.
                          mm[ind]=r; //Вписали RC.
                          return;
				}           
             //Конец записи  в RC
			 else   //n==3 
			 {if(pornom==0) {stopwr=0;	
	/*printkl("# Время остановлено записью на спецрегистр Беликова");*/}			 
			 return; } //Нет записи
		}	/* else {printkl("#F - MWU - Ошибка 3  в belt.");
			 ExitThread(TRUE);} */

	}
	//================================================




 void kpr(UINT in,UINT it,char ss,UINT sisl,UINT *uk,ntip *k)
    //Запись-чтение в регистры контроллера прерыв.Аналог belt.
	//Передаваемые параметры аналогичны belt/
   {UINT ind,*m,*mm,adr,n,r0,sis,mask,r1;
     n=it-in; if(n>7)
	{printkl("#F - MWU - Ошибка в порядк.номере рег.в kpr.");
	                                    ExitThread(TRUE);}
 	  m=k->rw; mm=k->mp; adr=m[it]; ind=indpam(adr,k);
	 if(ind==1) 
{printkl("#F - MWU - Ошибка индекса в kpr"); ExitThread(TRUE);}

	       if(ss=='s')   //Чтение с регистра в uk[0].
	{r0=mm[ind]; if(n==4)
		{uk[0]=037777777774&r0; return;}  //Rexc
		         else if(n==0)
		{uk[0]=0377601777&r0; return;}   //RQM
				 else if(n==6)
			{uk[0]=r0; return;}           //RBEr				 
			else {uk[0]=0; return;} //нулевые ячейки
	}//-----Конец чтения
         if(ss!='z')
	{printkl("#F - MWU - Некорректный код операции в kpr.");
		                                ExitThread(TRUE);}
              //Теперь - запись в регистр числа sisl.
	if(n==4) {mm[ind]=037777777774&sisl; return;} //Rexc
		 else if(n==0)          //Теперь запись в RQM :
		 {  
		  sis=0377601777&sisl; //Вычистили холостые разряды.
		  r0=mm[ind];  //RQM
		  r0=0377601777&r0;//Вычистили холостые разряды.
          mask=0252401252;       //Разряды масок.
		  r1=(0xffffffff)-mask;  //Ее инверсия
		  r0=r1&r0;    //Стерли все старые маски в RQM
		  r1=mask&sis; //Новые маски
		  r0=r1|r0;    //Вписали их в RQM
		     mm[ind]=r0; //Вписали результат - RQM.
              k->axtung=020|k->axtung;
			  return;
         }
		 else if(n==6)               //RBEr
         {if(sisl==0xa7f50000) 
             k->cause=(037777737777&(k->cause));
         }
		 else return; //Нет записи в нулевые ячейки
   }//===========================================

void pwr(UINT in,UINT it,char ss,UINT sisl,UINT *uk,ntip *k)      
	   //Запись - чтение с регистров ПВР
{UINT n,r0,r1,r2,r;
 // korwrkws(k);         //Коррекция в КВС по заказу Беликова       
  n=it-in; if(n>28)
  {printkl("# F pwr - Ошибка ПРОГРАММИСТА СУБМИКРОН в номере регистра"); return;}
    //-----------------------
  r0=streg(60,0,0,0,n+5,k);  //Прочитали регистр
  r0=(0177777&r0);           //Значение на регистре
    //-----------------------
          if(ss=='s')        //Чтение в uk[0]
{     if(n==0)                   //RT0
	{uk[0]=r0; r=streg(60,0,0,0,6,k); k->bufrt0=017&r;}
 else if(n==1)                   //RTL0
 uk[0]=k->bufrt0;
 else if(n==2)                   //RP0
 uk[0]=r0;
 else if(n==3)                   //RPL0
 uk[0]=017&r0;
 else if(n==4)                   //RC0
 uk[0]=0167770&r0;
 else if(n==5)                   //RFix0
 {uk[0]=r0; r=streg(60,0,0,0,11,k); k->bufrf0=017&r;}
 else if(n==6)                   //RFixL0
 uk[0]=k->bufrf0;
 else if(n==7)                   //RWD
 uk[0]=r0;
 //---------- конец с ТмрСн0 ------------
 else if(n==8)                   //RT1
 {uk[0]=r0; r=streg(60,0,0,0,14,k); k->bufrt1=017&r;}
 else if(n==9)                   //RTL1
 uk[0]=k->bufrt1;
 else if(n==10)                  //RP1
 uk[0]=r0;
 else if(n==11)                  //RPL1
 uk[0]=017&r0;
 else if(n==12)                  //RC1
 uk[0]=0167770&r0;
 else if(n==13)                  //RFix1
 {uk[0]=r0; r=streg(60,0,0,0,19,k); k->bufrf1=017&r;}
 else if(n==14)                  //RFixL1
 uk[0]=k->bufrf1;
 //---------- конец с ТмрСн1 ------------
 else if(n==15)                  //RQI     -    ЦВМ 101 !
 uk[0]=r0;
 else if(n==16)                  //RMI 
 uk[0]=r0;
 else if(n==17)                  //RNI
 uk[0]=r0;
 else if(n==18)                  //REr0
 uk[0]=0177600&r0;
 else if(n==19)                  //RQP
 uk[0]=r0;
 else if(n==20)                  //RMP
 uk[0]=r0;
 else if(n==21)                  //RNP
 uk[0]=r0;
 else if(n==22)                  //RFP
 uk[0]=r0;
 else if(n==23)                  //RFM
 uk[0]=r0;
 else if(n==24)                  //ROff
  uk[0]=017&r0;           
 else if(n==25)                  //RR
 uk[0]=r0;
 else if(n==26)                  //RTlm
 uk[0]=0377&r0;
 else if(n==27)                  //RRel
 {uk[0]=0;}
 else if(n==28)                  //RBlk
 uk[0]=01&r0;
 else { }   //Кланяюсь прихоти транслятора
              return;

}//--------       Конец чтения -----------------

      //--- Теперь Запись в регистр:

          r2=0177777&sisl;      //Это то, что записывается в регистр.
                                //А в r0  - то, что было раньше в регистре. 

      if(n==0)                   //RT0
{if(osibpwr) printkl("# Нет записи на RT0  . Ошибка пользователя.");} 
 else if(n==1)                   //RTL0
 {if(osibpwr) printkl("# Нет записи на RTL0  . Ошибка пользователя.");}
 else if(n==2)                   //RP0
 {wpis(60,0,0,0,7,2,r2,k); wpis(60,0,0,0,8,2,k->bufrp0,k);} 
 else if(n==3)                   //RPL0
 {r1=017&r2; k->bufrp0=r1;} 
 else if(n==4)                   //RC0
 tspwr(in,it,r2,k);
 else if(n==5)                   //RFX0
 {if(osibpwr){ printkl("# Нет записи на RFiX0  . Ошибка пользователя.");
  soob.sprintf("# Физич.адрес команды записи %x   16-тир",k->pc); printkl(soob);
 }} 
 else if(n==6)                   //RFXL0
 {if(osibpwr) {printkl("# Нет записи на RFiXL0  . Ошибка пользователя.");
   soob.sprintf("# Физич.адрес команды записи %x   16-тир",k->pc); printkl(soob);
 }}
 else if(n==7)                   //RWd 
 wpis(60,0,0,0,12,2,01777&r2,k);
 else if(n==8)                   //RT1 
 {if(osibpwr) {printkl("# Нет записи на RT1  . Ошибка пользователя.");
  soob.sprintf("# Физич.адрес команды записи %x   16-тир",k->pc); printkl(soob);
 }}
 else if(n==9)                   //RTL1
 {if(osibpwr) {printkl("# Нет записи на RTL1  . Ошибка пользователя.");
   soob.sprintf("# Физич.адрес команды записи %x   16-тир",k->pc); printkl(soob);
 }}
 else if(n==10)                  //RP1
 { wpis(60,0,0,0,15,2,r2,k); wpis(60,0,0,0,16,2,k->bufrp1,k);}
 else if(n==11)                  //RPL1
 {r1=017&r2; k->bufrp1=r1;}
 else if(n==12)                  //RC1
 tspwr(in,it,r2,k);
 else if(n==13)                  //RFX1
 {if(osibpwr) {printkl("# Нет записи на RFiX1  . Ошибка пользователя.");
  soob.sprintf("# Физич.адрес команды записи %x   16-тир",k->pc); printkl(soob);
 }}
 else if(n==14)                  //RFXL1 
 {if(osibpwr) {printkl("# Нет записи на RFiXL1  . Ошибка пользователя.");
  soob.sprintf("# Физич.адрес команды записи %x   16-тир",k->pc); printkl(soob);
 }}
 else if(n==15)                  //RQI
 {r1=xitpr(r0,r2,0,0,0177777,0,037777600000,100);
  wpis(60,0,0,0,20,2,r1,k); obrrqi(k);
 }
 else if(n==16)                  //RMI
 {wpis(60,0,0,0,21,2,r2,k); obrrqi(k);}	 
 else if(n==17)                  //RNI
 {wpis(60,0,0,0,22,2,r2,k); obrrqi(k);}	 
 else if(n==18)                  //RErO
 {r1=xitpr(r0,r2,0,0,0177600,0,037777600177,101);
   wpis(60,0,0,0,23,2,r1,k); 
 }
 else if(n==19)                  //RQP
 {r1=xitpr(r0,r2,0,0,0177777,0,037777600000,102);
  wpis(60,0,0,0,24,2,r1,k); obrrqp(k);
 }
 else if(n==20)                  //RMP
 {wpis(60,0,0,0,25,2,r2,k); obrrqp(k);}
 else if(n==21)                  //RNP
 {wpis(60,0,0,0,26,2,r2,k); obrrqp(k);}
 else if(n==22)                  //RFP
 {if(osibpwr) {printkl("# Нет записи на RFP  . Ошибка пользователя.");
  soob.sprintf("# Физич.адрес команды записи %x   16-тир",k->pc); printkl(soob);
 }}
 else if(n==23)                  //RFM
 {r1=xitpr(r0,r2,0125252,052525,0,0,037777600000,103);
  wpis(60,0,0,0,28,2,r1,k); kpr_pwr(k);
 }
 else if(n==24)                  //ROff
  wpis(60,0,0,0,29,2,017&r2,k);                            
 else if(n==25)                  //RR
 {r1=01&r2; wpis(60,0,0,0,30,2,r1,k);}
 else if(n==26)                  //RTlm
 {r1=0377&r2; wpis(60,0,0,0,31,2,r1,k);}
 else if(n==27)                  //RRel
 {r2=(0207&r2);
  if((0200&r2)!=0) widrel(r2,k);
    else           snatrel(r2,k);
   wpis(60,0,0,0,32,2,r2,k);
 }
 else if(n==28)                  //RBlk
 wpis(60,0,0,0,33,2,01&r2,k);
 
 else { }  //Кланяюсь прихоти транслятора
          return;

}//============ Конец pwr ===============================







  void tspwr(UINT in,UINT it,UINT sisl,ntip *k)
 //Запись sisl на рег.управления RC таймера синхрониз.(n=it-in=4 или 12)
 //В конце исполняет obrts, делающую RFM-ПВР и KPR.  .
{UINT n,rc,r,r1,r2;
                n=it-in; r1=n+5;
 rc=streg(60,0,0,0,r1,k);   //RC
 r2=xitpr(rc,sisl,027470,0,0140300,0,037777610007,105);
 wpis(60,0,0,0,r1,2,r2,k);  //Вписали в RC, Push уже сброшен в RC.
     //--- Теперь реакция на значение Push в sisl :
         if((010000&sisl)!=0)   //Будет перепись RP ->RT
	{      if(n==4)      //Таймер нулевой
	 {r=streg(60,0,0,0,5,k);   //RT0
      wpis(60,0,0,0,10,2,r,k); //Вписали в RFix0
      r=streg(60,0,0,0,6,k);   //RTL0
	  wpis(60,0,0,0,11,2,r,k); //Вписали в RFixL0
         //Переписали со счетчика в регистр фиксации
	  r=streg(60,0,0,0,8,k);  //RPL0
      r=(017&r);
	  wpis(60,0,0,0,6,2,r,k);  //Вписали младш.4 бита в RTL0
      r=streg(60,0,0,0,7,k);  //RP0
	  r=(0177777&r);
      wpis(60,0,0,0,5,2,r,k); //Вписали RP0 в RT0
	 }//---- Конец с нулевым таймером.
         else if(n==12)   //Первый таймер
	 {r=streg(60,0,0,0,13,k);  //RT1
      wpis(60,0,0,0,18,2,r,k); //Вписали в RFix1
	  r=streg(60,0,0,0,14,k);  //RTL1
	  wpis(60,0,0,0,19,2,r,k); //Вписали в RFixL1
	     //Переписали со счетчика в регистр фиксации
	  r=streg(60,0,0,0,16,k);  //RPL1
      r=(017&r);
	  wpis(60,0,0,0,14,2,r,k);  //Вписали младш.4 бита в RTL1
      r=streg(60,0,0,0,15,k);  //RP1
	  r=(0177777&r);
      wpis(60,0,0,0,13,2,r,k); //Вписали RP1 в RT1
	 }//---- Конец с первым таймером.
	  else  {printkl("# F - tspwr - Ошибка ПРОГРАММИСТА СУБМИКРОН с индексом.");
              return;
			}
	}//--- Реакция на Push  сделана ---
       //--- Осталось отразить ситуацию в RC    в RFM-ПВР и  KPR:
                     obrts(k);   

}//============ Конец tspwr ======================== 
  


  void obrrqi(ntip *k)
//Отражает все по импульсным сигналам, проводит через RFM до KPR.
{UINT rqi,rmi,rni,r0,rm;
  rqi=streg(60,0,0,0,20,k);  //RQI
  rmi=streg(60,0,0,0,21,k);  //RMI
  rni=streg(60,0,0,0,22,k);  //RNI

     r0=0177777^rni; r0=(0177777&r0); //Инверсия нулей в RNI.
	 rm=0177777^rmi; rm=(0177777&rm); //Инверсия масок
	   //----------------------------
	       if(((r0&rm)&rqi)!=0)    //Есть F0 в RFM
		wpis(60,0,0,0,28,0,1,k);   //Вписали F0=1
   else wpis(60,0,0,0,28,1,1,k);   //Вписали F0=0
       //-----------------------------
           if(((rni&rm)&rqi)!=0)   //Есть F1 в RFM
	    wpis(60,0,0,0,28,0,4,k);   //Вписали F1=1
   else wpis(60,0,0,0,28,1,4,k);   //Вписали F1=0
      //-------- RFM готов -----------

             kpr_pwr(k);   //Отражение в KPR

}//=========== Конец obrrqi ========================


  void obrrqp(ntip *k)
//Отражает все по потенциальным сигналам, проводит через RFM до KPR.
{UINT rqp,rmp,rnp,r0,rm;
  rqp=streg(60,0,0,0,24,k);  //RQP
  rmp=streg(60,0,0,0,25,k);  //RMP
  rnp=streg(60,0,0,0,26,k);  //RNP
     r0=0177777^rnp; r0=(0177777&r0); //Инверсия нулей в RNP.
	 rm=0177777^rmp; rm=(0177777&rm); //Инверсия масок
	   //----------------------------
	       if(((r0&rm)&rqp)!=0)      //Есть F2 в RFM
		wpis(60,0,0,0,28,0,020,k);   //Вписали F2=1
   else wpis(60,0,0,0,28,1,020,k);   //Вписали F2=0
       //-----------------------------
           if(((rnp&rm)&rqp)!=0)     //Есть F3 в RFM
	    wpis(60,0,0,0,28,0,0100,k);  //Вписали F3=1
   else wpis(60,0,0,0,28,1,0100,k);  //Вписали F3=0
      //-------- RFM готов -----------

             kpr_pwr(k);   //Отражение в KPR

}//=========== Конец obrrqp ========================


  void obrts(ntip *k)
 //Учитывает RC обоих таймеров и проводит все через RFM в KPR
{UINT rc;
             //Нулевой таймер:
  rc=streg(60,0,0,0,9,k);       //RC0
      if(((040000&rc)!=0)&&((020000&rc)==0))
      wpis(60,0,0,0,28,0,0400,k);      //F4=1 в RFM 
 else wpis(60,0,0,0,28,1,0400,k);      //F4=0 в RFM
         //------------------------
      if(((0100&rc)!=0)&&((040&rc)==0))
      wpis(60,0,0,0,28,0,02000,k);     //F5=1 в RFM 
 else wpis(60,0,0,0,28,1,02000,k);     //F5=0 в RFM
         //------------------------
            //Первый таймер:
  rc=streg(60,0,0,0,17,k);      //RC1
      if(((040000&rc)!=0)&&((020000&rc)==0))
      wpis(60,0,0,0,28,0,010000,k);      //F6=1 в RFM 
 else wpis(60,0,0,0,28,1,010000,k);      //F6=0 в RFM
         //------------------------
      if(((0100&rc)!=0)&&((040&rc)==0))
      wpis(60,0,0,0,28,0,040000,k);     //F7=1 в RFM 
 else wpis(60,0,0,0,28,1,040000,k);     //F7=0 в RFM
         //------------------------
              //-------- RFM готов -----

                kpr_pwr(k);   //Отражение в KPR

}//============= Конец obrts =======================



void ostkfu(UINT in,UINT it,char ss,UINT sisl,UINT *uk,ntip *k) 
	             //Запись-чтение регистров остатка KFU
  {UINT ind,*m,*mm,adr,n,r0,r1;
               
     n=it-in;  if(n>8)
{printkl("#F - MWU - Ошибка в порядковом номере регистра в ostkfu");
	                                    ExitThread(TRUE);}
   m=k->rw; mm=k->mp; adr=m[it]; ind=indpam(adr,k);
	 if(ind==1) 
{printkl("#F - MWU - Ошибка индекса в ostkfu"); ExitThread(TRUE);}
   r0=mm[ind];
	           if(ss=='s')   //чтение с регистра в uk[0]
			   {   uk[0]=r0; return;   
			   }    //конец чтений
                   if(ss!='z')
        {printkl("#F - MWU - Некорректный код операции в ostkfu");
			                          ExitThread(TRUE);}
                //Теперь - запись в регистр числа sisl
         if(n==0)
      {r1=xitpr(r0,sisl,070000,0,0100000,0,037777607777,7);
	    mm[ind]=r1; return;} //RRstF
    else if(n==2) //RRst
{mm[ind]=0; if((sisl&0xffff0000)==0xa7f50000) progrper(k);
	                                //прогр перезапуск
	   return; }   
        else if(n==4)     //RОRB
	{r1=xitpr(r0,sisl,0,0,01700000000,0,036077777777,8);
	  if((0400000000&sisl)==0)            //QSyn=0
	  wpis(50,0,0,0,5,1,0400,k);	   //Вписали его в RQM.
	          //--------------------
	  wpis(51,0,0,0,9,2,r1,k);          //Вписали RОRB
	         k->axtung=020|k->axtung;
	            return;
	}
	else if(n==8)    //RHst
	{r1=xitpr(r0,sisl,04000,0,02000,0,037777771777,53);	
//	soob.sprintf("!!!!!!!! r0=%x  sisl=%x r1=%x",r0,sisl,r1); printkl(soob);
        if((02000&sisl)==0)    //QHst=0
       wpis(50,0,0,0,5,1,0100000000,k); //Вписали его в RQM.
	  wpis(51,0,0,0,13,2,r1,k);          //Вписали RHst
	         k->axtung=020|k->axtung;
	            return;
    }
		else return;     //Нет записи в нули
  }//=======================================================
	                                                                     

   void pk(UINT pornom,UINT in,UINT it,char ss,UINT sisl,
	                                    UINT *uk,ntip *k)
	   //Чтение-запись в регистры ПК
   {UINT ind,*m,*mm,adr,n,r0,r1,sis;                          
        n=it-in; if(n>9)
{printkl("#F - MWU - Ошибка в порядковом номере регистра в PK");
		                                 ExitThread(TRUE);}
  m=k->rw; mm=k->mp; adr=m[it]; ind=indpam(adr,k);
	 if(ind==1) 
{printkl("#F - MWU - Ошибка индекса в pk"); ExitThread(TRUE);}
      r0=mm[ind];
	           if(ss=='s')   //чтение с регистра в uk[0]
	{	      if(n==0)      //RFM         
		{ uk[0]=r0;
			 wpis(52,pornom,0,0,10,2,r0,k);
             wpis(52,pornom,0,0,11,2,r0,k);
			 wpis(52,pornom,0,0,12,2,r0,k);			   
			   return; }
          else if(n==1)     //RPN         
		{ uk[0]=r0;
			 wpis(52,pornom,0,0,10,2,r0,k);
             wpis(52,pornom,0,0,11,2,r0,k);
			 wpis(52,pornom,0,0,12,2,r0,k);		  
		       return; }
          else if(n==3)     //RRst        
		{ uk[0]=r0; 
			 wpis(52,pornom,0,0,10,2,r0,k);
             wpis(52,pornom,0,0,11,2,r0,k);
			 wpis(52,pornom,0,0,12,2,r0,k);		  
		       return; }
          else if(n==2)     //RTX         
		{ uk[0]=r0;
			 wpis(52,pornom,0,0,10,2,r0,k);
             wpis(52,pornom,0,0,11,2,r0,k);
			 wpis(52,pornom,0,0,12,2,r0,k);		  
		       return; }
          else if(n==4)     //REQB        
		{ uk[0]=r0; 
			 wpis(52,pornom,0,0,10,2,r0,k);
             wpis(52,pornom,0,0,11,2,r0,k);
			 wpis(52,pornom,0,0,12,2,r0,k);
		       return; }
          else if(n==8)     //REIO        
		{ uk[0]=r0; 
			 wpis(52,pornom,0,0,10,2,r0,k);
             wpis(52,pornom,0,0,11,2,r0,k);
			 wpis(52,pornom,0,0,12,2,r0,k);
		       return; }
          else if((n>4)&&(n<8)) //RDA,RDB,RDC 
		 {uk[0]=r0; return;}
		 else                   //0       
        { uk[0]=r0;
			 wpis(52,pornom,0,0,10,2,r0,k);
             wpis(52,pornom,0,0,11,2,r0,k);
			 wpis(52,pornom,0,0,12,2,r0,k);		 
		       return; }
			
	}//--- Чтение завершено
                if(ss!='z')
		{printkl("#F - MWU - Ошибка типа операции в PK");
				                  ExitThread(TRUE);}
				//Теперь запись sisl в регистр:
              if(n==0)   //RFM  +
	{sis=sisl; 
	 r1=xitpr(r0,sis,0125252,052525,0,0,037777600000,9);
	 mm[ind]=r1; kpr_rfm(k); return;
	}
		else if(n==1)    //RPN  +            
	{r1=073567&sisl;
      mm[ind]=r1; return;
	}
		else if(n==3)    //RRst +            
	{ if((1&sisl)!=0)   sbroskww(2*pornom,k);
      if((020&sisl)!=0) sbroskww((2*pornom)+1,k);
      mm[ind]=0; return;
	}
        else if(n==2)    //RTX  +
	{r1=xitpr(r0,sisl,031463,0,0,0,037777746314,16);
     mm[ind]=r1; return;
	}
          else if(n==4)     //REQB        +
	{r1=xitpr(r0,sisl,0,0,07,0,037777777770,17);
      mm[ind]=r1; return;
	}
          else if(n==8)     //REIO        +
	{r1=xitpr(r0,sisl,0,0,0377,0,037777777400,18);
      mm[ind]=r1; return;
	}
      else if((n>4)&&(n<8)) //RDA,RDB,RDC +
	{ return; } //запись  игнорируется
       else                 //0           +
	{ return; } //запись в 0 игнорируется		   
   }//=================================================
  

 void kmk(UINT in, UINT it, char ss,
	UINT sisl, UINT *uk, UINT pornom, ntip *k)
 { 
	//Запись, чтение с регистров КМК
  UINT j, ind, ind0, n, adr, r0, rg, rsc,r,uww;
	UINT nitt,wt, ex, zam,xit0, sistka,p,rsw,rr,mod;
  UINT *m,*mm;
	n=it-in; if(n>15)
	{printkl("#F - MWU - ошибка номера регистра в kmk");
		ExitThread(TRUE);
	}
	m=k->rw; adr=m[it]; ind=indpam(adr,k);
		if(ind==1)
	{printkl("#F - MWU - ошибка индекса в кмк");
		ExitThread(TRUE);
	}
		mm=k->mp;
	if(ss=='s') //чтение с регистра в uk[0].
	{r0=mm[ind]; if(n==1) r0=(0160777&r0); uk[0]=0177777&r0;
	         uww=pornom/2;     //Вписывание в RDA,RDB,RDC:
	 wpis(52,uww,0,0,10,2,uk[0],k);
	 wpis(52,uww,0,0,11,2,uk[0],k);
	 wpis(52,uww,0,0,12,2,uk[0],k);
		return;
	}
//Конец чтения регистра ячейку памяти.
	if(ss!='z')
{printkl("#F - MWU - ошибка операции в kmk");
	                            ExitThread(TRUE);}
//теперь запись sisl в регистр mm[ind]:

 //Cначала проверка на запись в регистр КМК 
 //при его незаконченном обмене:
  p=0;	if((pornom==0) && (k->pak0!=0)) p=1;
  else	if((pornom==1) && (k->pak1!=0)) p=1;
  else	if((pornom==2) && (k->pak2!=0)) p=1;
  else	if((pornom==3) && (k->pak3!=0)) p=1;
  else	if((pornom==4) && (k->pak4!=0)) p=1;
  else	if(k->pak5!=0)                  p=1;
  else { }

     if((p!=0)&&(pesosib!=0))
{soob.sprintf("#W - MWU - Запись в регистр КМК-%d при незаверш.обмене !!!",
			                                       pornom);
	   printkl(soob);}      
        //Проверили
	sistka=037777600000;
	if(n==0) //RQN
{ rg=mm[ind];
	zam=0; 
	xit0=0377;
	r0=xitpr(rg, sisl, zam, 0, xit0, 0, 037777777400,12);
	mm[ind]=r0;
	rqn_rfm_pk(pornom,k);  kpr_rfm(k);
	//Учет воздействия на прерывания    
	return;
}
//---------------------
	else if(n==1) //RR
{ r0=0177177&sisl; r0=(0160777&r0);
	nitt=2; 
	if((nitt&r0)!=0)  
		                  //перезапуск таймера RTT:
	{j=in+2; adr=m[j]; ind0=indpam(adr,k);
		mm[ind0]=0; //сбросили таймер
		r0=nitt^r0; //сброс признака перезапуска.
	}//------------------------------
	 wt=4; //Признак включения таймера КМК по RR.
	 if((wt&r0)!=0)   k->wklt[pornom]=1;
	 else             k->wklt[pornom]=0;
	//-------------------------------
	            mod=r0>>13; mod=(07&mod);

	  if((mod==3)||(mod==7))     //Контроллер
	  {ex=1; if((ex&r0)!=0)   zappak(pornom,k);
	     //Был запуск пакета, если только контроллер.
	   wpis(100,pornom,0,0,6,2,r0,k); //Вписали готовый RR.
	      //--- Теперь его влияние на RSW - ответное слово.
abc:   rsw=streg(100,pornom,0,0,9,k);  //RSW
            rsw=(0177777&rsw);
        //-- Анализируем запрос на обслуживание по RR=r0:
       if((020&r0)!=0) rsw=(0400|rsw);
	   else rsw=(0177377&rsw);
	    //--Анализируем неисправность ОУ по RR=r0:
       if((040&r0)!=0)  rsw=(01|rsw);
	   else rsw=(0177776&rsw);
	    //--Анализируем неисправность абонента по RR=r0:
	   if((0100&r0)!=0) rsw=(04|rsw);
	   else rsw=(0177773&rsw);
	    //--Анализируем Абонент занят по RR=r0:
	   if((010&r0)!=0) rsw=(010|rsw);
	   else rsw=(0177767&rsw);
	   //----------------------------------------------
//          wpis(100,pornom,0,0,9,2,rsw,k); //Вписали RSW //RSW  здесь не меняется, только при команде ОУ
		          return;
	  }//---------- Конец с контроллером -------

    if((mod==0)||(mod==1)||(mod==4)||(mod==5)) // ОУ
      {rr=streg(100,pornom,0,0,6,k);  //RR
       rr=(01&rr); //оставляем только 0 разряд в RR.
	   r0=(0177776&r0);  r0=(r0|rr);
	       //Это новый RR с сохранением старого 0 разряда.
         wpis(100,pornom,0,0,6,2,r0,k); //Вписали готовый RR.
		    
        goto abc; //на формир.признаков в RSW - ответном слове
	  }//--------- Конец с ОУ -----------------
	
    if((mod==2)||(mod==6))         //MK  - /как и при ОУ/. 
      {rr=streg(100,pornom,0,0,6,k);  //RR
       rr=(01&rr); //оставляем только 0 разряд в RR.
	   r0=(0177776&r0);  r0=(r0|rr);
	       //Это новый RR с сохранением старого 0 разряда.
         wpis(100,pornom,0,0,6,2,r0,k); //Вписали готовый RR.
		    
        goto abc; //на формир.признаков в RSW - ответном слове		
    }//------  Конец МК -----------------------
} //------------------------------------------------------

	else if(n==2) //RTT
{ mm[ind]=0177777&sisl; return;}
//---------------------------
	else if(n==3) //RSP
{rr=streg(100,pornom,0,0,6,k);  //RR
  mod=rr>>13; mod=(07&mod);
	//- - - - - - - - - -
  if((mod!=2)&&(mod!=6))  //Т.е. не МК
  {wpis(100,pornom,0,0,8,2,(01770&sisl),k); return;}
  else          //MK
  {wpis(100,pornom,0,0,8,2,(017776&sisl),k); return;}
}
//---------------------------
	else if(n==4)               //RSW
{r0=mm[ind]; r=xitpr(r0,sisl,0174000,03777,0,0,sistka,19);
 mm[ind]=r;
return;
}
//--------------------------------
	else if(n==5) //RSC
{ rsc=mm[ind]; xit0=0177777;
 r0=xitpr(rsc,sisl, 0, 0, xit0, 0, sistka,13);
 mm[ind]=r0; return;
}
//----------------------------------
	else if(n==8) //RCW
	{ //Ничего не пишется. 
	return;
	}
//----------------------------------
    else if((n==6)||(n==7))     //RM или RN
    {r=0377&sisl; mm[ind]=r;
	 rqn_rfm_pk(pornom,k);  kpr_rfm(k);
	return;}
//----------------------------------
	else if((n>8)&&(n<15))
	{mm[ind]=0;   	
		return;

	}//---------------------------------
    else        //RTech
    {mm[ind]=0177777&sisl; return;
	}
}//========== КОНЕЦ KMK ======================



  void bozu(UINT pornom,UINT adr,char ss,UINT sisl,UINT *uk,ntip *k)
	  //Пишет - читает по адресу adr БОЗУ.
	         //Запускается wnu().
  {UINT ind,*mm;
    mm=k->mp; ind=indpam(adr,k);
	   if(ind==1)
{printkl("#F - MWU -Ошибка индекса в bozu"); ExitThread(TRUE);}
	             if(ss=='s') 
	   {uk[0]=0177777&mm[ind];
         wpis(52,pornom,0,0,10,2,uk[0],k); //Вписывание в RDA,RDB,RDC
         wpis(52,pornom,0,0,11,2,uk[0],k);
		 wpis(52,pornom,0,0,12,2,uk[0],k);
	   }
	   else     {mm[ind]=0177777&sisl;}
	
  }//======================================================



  //--- КОНЕЦ  ФУНКЦИЙ  ЧТЕНИЯ-ЗАПИСИ  НА  РЕГИСТРЫ  И  БОЗУ. ---------- 



   //---- ФУНКЦИИ, СВЯЗАННЫЕ  С  ТАЙМЕРАМИ  -------- 

  void  stbelt(ntip *k)
	       //Реакция таймеров процессора (тип 1) на delt>0
		  //Запускается в зоне инициации комплекса в zakr.cpp.
  {UINT *m,*mm,i,j,pornom,podtip,ind0,ind1,ind2,r0,r1,r2;
    m=k->rw; mm=k->mp; i=0;
                    while(m[i]!=0)
	{if(m[i]==0xfff)
		{if(m[i+1]==1)   //Тип 1.
			{pornom=m[i+2]; podtip=m[i+3];
             ind0=indpam(m[i+5],k); 
			 if(ind0==1)
              {printkl("#F - MWU - Ошибка с ind0 в stbelt."); 
			                            ExitThread(TRUE);}
			 r0=mm[ind0]; r0=(03777777&r0);  //RT
             ind1=indpam(m[i+6],k);
			 if(ind1==1)
              {printkl("#F - MWU - Ошибка с ind1 в stbelt.");
			                            ExitThread(TRUE);}
			 r1=mm[ind1]; r1=(03777777&r1);  //RP
             ind2=indpam(m[i+7],k);
			 if(ind2==1)
              {printkl("#F - MWU - Ошибка с ind2 в stbelt."); 
			                           ExitThread(TRUE);}
			 r2=mm[ind2];                    //RC
			       //Вытащили регистры. 
	if((0100000000&r2)!=0) //снять активность. Не включен. 
					{i=i+9;
		if(pornom==0) k->axtung=(037777777776&k->axtung);
   else if(pornom==1) k->axtung=(037777777775&k->axtung);
   else if(pornom==2) k->axtung=(037777777773&k->axtung);
		    else  k->axtung=(037777777767&k->axtung);
			 
			 continue;} 
          if(r0==0)  //снять активность. Счетчик обнулен.  
			 {i=i+9;
		if(pornom==0) k->axtung=(037777777776&k->axtung);
   else if(pornom==1) k->axtung=(037777777775&k->axtung);
   else if(pornom==2) k->axtung=(037777777773&k->axtung);
		      else  k->axtung=(037777777767&k->axtung);			 
			 continue;} 
	 //Теперь таймер включен и на счетчике не равно 0.
			  if(r0>delt) 
              {r0=r0-delt; mm[ind0]=r0; i=i+9; continue;}
			   //---------------------------------
			  else  //Таймер сработал.
              {mm[ind0]=0;     //RT
	 if((010000000000&r2)!=0) r2=(020000000000|r2); //ErQ
			  r2=010000000000|r2;  //Q
			  if((04000000000&r2)==0)
				  //Будет запись в контроллер прерыв.
          {     if(pornom==0) wpis(50,0,0,0,5,0,0200000,k);
           else if(pornom==1) wpis(50,0,0,0,5,0,01000000,k);
           else if(pornom==2) wpis(50,0,0,0,5,0,04000000,k);
           else if(pornom==3) wpis(50,0,0,0,5,0,020000000,k); 
              else {printkl("#F - MWU - Ошибка 1 в stbelt.");
			                           ExitThread(TRUE);}
			  }
		 r2=(034300000000&r2); mm[ind2]=r2; //Записали RC.
			  if((0200000000&r2)!=0)    //Циклический
              mm[ind0]=r1;   //Переписали на RT с RP.
			  else  //не циклический,снять активность axtung:
       {
			 if(pornom==0) k->axtung=(037777777776&k->axtung);
        else if(pornom==1) k->axtung=(037777777775&k->axtung);
		else if(pornom==2) k->axtung=(037777777773&k->axtung);
		         else  k->axtung=(037777777767&k->axtung);
				  
		}
			  i=i+9; continue;
			 }
			}//---------Конец типа 1
            else 
			{i=i+5; j=i;
			  while((m[j]!=0xfff)&&(m[j]!=0))  {j=j+1;}
              i=j;
			} //-----------
		}
	 else {printkl("#F - MWU - Ошибка 2 в stbelt"); 
	                                 ExitThread(TRUE);}
	}
  }//=========================================================


   void sttmsi(ntip *k)//Реакция 2-х таймеров синхр. только на опорн. частоту в 1 мкс.
{UINT rc,ind,*mm;
                    mm=k->mp;   //Указатель модели памяти
	      //---- Нулевой таймер ---- :
     ind=k->ipwr[4]; rc=mm[ind];  //RC0
      if(((04000&rc)!=0)||((0400&rc)!=0))   goto a1;
    else   rabts(0,delt,k);  //Считает 0 таймер.
	      //---- Первый таймер ------:
 a1: ind=k->ipwr[12]; rc=mm[ind];  //RC1
      if(((04000&rc)!=0)||((0400&rc)!=0))   return;
    else   rabts(1,delt,k);  //Считает 1 таймер.
   } //========== Конец sttmsi


  void rabts(UINT n,UINT delt,ntip *k)
 //Считает по таймеру синхр.ПВР номера n на приход delt.
 //Всегда исполняется, соблюдение условий исполнения выяснены
 //запускателем этой функции.
 //n - номер таймера.
{UINT *mm,rc,rt,rtl,rp,rpl,r,irc,irt,irtl,irp,irpl,irx,irxl; 
     mm=k->mp;
	    //--- Найдем значения на регистрах счетчика и индексы регистров:
              if(n==0)  //Нулевой таймер
	{irt=k->ipwr[0]; rt=0177777&mm[irt];  //RT0
     irtl=k->ipwr[1]; rtl=017&mm[irtl];    //RTL0
	  if((rt==0)&&(rtl==0)) return;     //От нуля счетчик не считает
	}
			  else      //Первый таймер
	{irt=k->ipwr[8]; rt=0177777&mm[irt];  //RT1
     irtl=k->ipwr[9]; rtl=017&mm[irtl];    //RTL1
	  if((rt==0)&&(rtl==0)) return;     //От нуля счетчик не считает
	}
      //---- Нашли. Теперь формируем r - 20-ти разрядный мой счетчик:
       r=rt<<4; r=(r|rtl);   //Сформировали.
	   //-------------------------------------------
	           if(r>delt)        //Не сработает таймер
 {r=(r-delt); rtl=017&r; rt=r>>4; mm[irt]=rt; mm[irtl]=rtl; //Откорректировали
                return;
 }    //-- конец при несрабатывании таймера

          //--- Теперь таймер СРАБОТАЕТ:
   if(pestsinxr!=0)
   {soob.sprintf("# Таймер синхр. %d сработал. Время срабатывания %lu",n,(4*ttek4));
         printkl(soob);
   }

  mm[irt]=0;  mm[irtl]=0;    //Обнулили счетчик
          //-- Установим признаки Q и ErQ:
                  //Найдем RC и его индекс.
           if(n==0)     //Нулевой таймер
		   {irc=k->ipwr[4]; rc=0177777&mm[irc];}
            else        //Первый таймер
           {irc=k->ipwr[12]; rc=0177777&mm[irc];}
                //--- Нашли --
 if((040000&rc)!=0)  rc=0100000|rc;  //ErQ
                     rc=(040000|rc); //Q
                     mm[irc]=rc;     //Вписали RC в модель памяти.
     //--- Признаки установлены -----

             // -- Анализ циклического режима :
                if((01000&rc)!=0)  //циклич.
	{     if(n==0)
		{irp=k->ipwr[2];  rp=0177777&mm[irp]; //RP0
         irpl=k->ipwr[3]; rpl=017&mm[irpl];   //RPL0
		 irx=k->ipwr[5];  
		 irxl=k->ipwr[6];
		}
          else
		{irp=k->ipwr[10];  rp=0177777&mm[irp]; //RP1
         irpl=k->ipwr[11]; rpl=017&mm[irpl];   //RPL1
		 irx=k->ipwr[13];  
		 irxl=k->ipwr[14];
		}
		  //-- Определили значения регистров. Терерь перепись:
          mm[irx]=0;  mm[irxl]=0;   //Перепись в RFX
          mm[irt]=rp; mm[irtl]=rpl; //Перепись в RT
         if(pestsinxr!=0) printkl("# в ЦИКлическом режиме");
	}//------ конец циклич.режима

			else {if(pestsinxr!=0) printkl("# в НЕциклическом режиме");}


             obrts(k);   //Влияние RC на KPR
    
     //-- В заключение выдача срабатывания таймера во вне:
       if((010&rc)!=0)   //0FF0
		   return;  //Нет выдачи.

	   //----- Теперь выдача:
	     if(n==0) widtsinxr(k);   
          else    widtsinxr1(k);

}//============= Конец rabts =========================


  void rtt(ntip *k)
//Обеспечивает работу счетчиков времени RTT для KMK
//Запускается в зоне инициации комплекса в zakr.cpp.
  { UINT i,ind, r0, r1, *mm, qrtt;
	mm=k->mp; 
	for(i=0; i<6; i++) //цикл по KMK       
	{ind=k->rtt[i];   if(ind==0) continue;
	 if((k->wklt[i])==0) continue; //Не включен по RR.
	         //------------------------
		r0=mm[ind];      r0=0177777&r0; //RTT
		r1=r0+delt4; if(r1<=0177777)
					{ mm[ind]=r1; continue;}
		else 
		{r1=r1-0177777; mm[ind]=r1;
		 qrtt=010; // Разряд переполнения RTT  в  RQN
		 r0=streg(100, i, 0, 0, 5, k); //RQN
		 r0=r0|qrtt; mm[indstreg]=r0;
		//Вписали в RQN признак переполнения RTT.
		  rqn_rfm_pk(i,k);   //Влияние RQN на RFM-ПК-
		 kpr_rfm(k);     //Влияние  RFM(ПК) на KPR.  
		}
	}
        
}//========== КОНЕЦ ФУНКЦИИ rtt.====================


  void mwrem(UINT n)
//Выдача метки времени с номером n. (n=0  MV, n=1  IP6)
  {UINT rc,r;  int i; ntip *k;

      for(i=1;i<=kolwm;i++)     //Переберем все ВМ.

	{ if(Komplex[i]==0) continue;
	  k=bazuk[i];


    rc=streg(60,0,0,0,9,k); //RC0
	r=rc>>10; r=(01&r);  //ExT 
       if(r==n)  mwrts(0,n,k);  //Метка идет на нулевой таймер
       //--------------------------
    rc=streg(60,0,0,0,17,k); //RC1
	r=rc>>10; r=(01&r);  //ExT 
       if(r==n)  mwrts(1,n,k);  //Метка идет на первый таймер
       //--------------------------
	}

  }//=============== Конец mwrem =========================


 void mwrts(UINT t,UINT mw,ntip *k)
//Приход метки времени номера mw (0,1) на таймер синхрониз. номер t (0,1).  
{UINT rc,rp,rpl,rt,rtl;
                if(pesmw!=0)
	{soob.sprintf("# Метка времени номер %d выдана на таймер %d",mw,t);
				printkl(soob);
     soob.sprintf("# Время выдачи %lu",(4*ttek4));
	            printkl(soob);
	}//------------------------------------------
              if(t==0)          //Нулевой таймер

	{rc=streg(60,0,0,0,9,k); rc=(0177777&rc); //RC
        //----- Установим QI и ErQI :
      if((0100&rc)!=0)    //Повторный приход метки
       rc=(0200|rc);   //ErQI
	   rc=(0100|rc);   //QI
	     wpis(60,0,0,0,9,2,rc,k);  //Вписали готовый RC
		 //------------------------------

		   //-- Будет ли перепись RP -> RT  ?

       if(((020&rc)!=0)&&((0400&rc)==0))         //Будет. ExP=1, Off=0.
	   {if(pesmw!=0) printkl("# Перепись RP на RT по метке времени"); 
		   rt=streg(60,0,0,0,5,k);  rt=(0177777&rt);
	        wpis(60,0,0,0,10,2,rt,k);            //Переписали в RFX
        rtl=streg(60,0,0,0,6,k); rtl=(017&rtl);  
		    wpis(60,0,0,0,11,2,rtl,k);           //Переписали  в RFXL
		    //-- сделана предварительная перепись на регистр фиксации

        rp=streg(60,0,0,0,7,k);      rp=(0177777&rp);
            wpis(60,0,0,0,5,2,rp,k);             //Переписали в RT
        rpl=streg(60,0,0,0,8,k);     rpl=(0177777&rpl);
            wpis(60,0,0,0,6,2,rpl,k);            //Переписали в RTL
       }//--- конец переписей ------------------------

	        //--- Будет ли прибавление +1 к счетчику ?
        
        if(((04000&rc)!=0)&&((0400&rc)==0))     //Будет. Tic=1, Off=0.
	     rabts(0,1,k);  //Завершение с работой счетчика.  

	}//--- конец с нулевым таймером

            else     //Первый таймер

	{rc=streg(60,0,0,0,17,k); rc=(0177777&rc); //RC
        //----- Установим QI и ErQI :
      if((0100&rc)!=0)    //Повторный приход метки
       rc=(0200|rc);   //ErQI
	   rc=(0100|rc);   //QI
	     wpis(60,0,0,0,17,2,rc,k);  //Вписали готовый RC
		 //------------------------------

		   //-- Будет ли перепись RP -> RT  ?

       if(((020&rc)!=0)&&((0400&rc)==0))         //Будет
	   {if(pesmw!=0) printkl("# Перепись RP на RT по метке времени");
		   rt=streg(60,0,0,0,13,k);  rt=(0177777&rt);
	        wpis(60,0,0,0,18,2,rt,k);            //Переписали в RFX
        rtl=streg(60,0,0,0,14,k); rtl=(017&rtl);  
		    wpis(60,0,0,0,19,2,rtl,k);           //Переписали  в RFXL
		    //-- сделана предварительная перепись на регистр фиксации

        rp=streg(60,0,0,0,15,k);      rp=(0177777&rp);
            wpis(60,0,0,0,13,2,rp,k);             //Переписали в RT
        rpl=streg(60,0,0,0,16,k);     rpl=(0177777&rpl);
            wpis(60,0,0,0,14,2,rpl,k);            //Переписали в RTL
       }//--- конец переписей ------------------------

	        //--- Будет ли прибавление +1 к счетчику ?
        
        if(((04000&rc)!=0)&&((0400&rc)==0))     //Будет
	     rabts(1,1,k);  //Завершение с работой счетчика.  

	}//--- конец с первым таймером

                obrts(k);   //Отражение в RFM и  KPR

}//============== Конец mwrts ========================


 // ФУНКЦИИ, ИНИЦИИРУЮЩИЕ  И   ОСТАНАВЛИВАЮЩИЕ  ПАКЕТЫ  ОБМЕНОВ   
 // ДЛЯ  КМК  КОНТРОЛЛЕРОВ 

void zappak(UINT pornom, ntip *k)
//Запуск пакета обменов для КМК контроллера с номером pornom;
//Запускается в kmk().
{ UINT r0,r3,r2, adr,adr1,ind, *mm,nom;
  DWORDLONG tb,dl; short p;  int i;

         nom=0; for(i=1;i<=4;i++)
		 {if(bazuk[i]==k) {nom=i; break;}
         }//--------------------------------
               if(nom==0)
    {printkl("#F - zappak. - Фатальная ошибка СУБМ в указателе карты процессора."); return;}
                     nom=nom-1;  //Номер по Беликову
           //------------------------------------
 
  
                    if(pesmko!=0)
 {
  if(k->wds[pornom]==0) 
  {soob.sprintf("#  Запуск пакета неведущей ВМ%d на  KMK-%d. ",(nom),pornom);
    printkl(soob); 
	//return;
  }
  if(k->wklblk==1) 
  {soob.sprintf("#  Запуска пакета ВМ%d при wklblk=1. на KMK-%d",nom,pornom);
    printkl(soob); 
	//return;
  }


 }//---------------------------------
//Cначала проверка на ошибочный повтор запуска;
  p=0;	if((pornom==0) && (k->pak0!=0)) p=1;
  else	if((pornom==1) && (k->pak1!=0)) p=1;
  else	if((pornom==2) && (k->pak2!=0)) p=1;
  else	if((pornom==3) && (k->pak3!=0)) p=1;
  else	if((pornom==4) && (k->pak4!=0)) p=1;
  else	if((pornom==5)&&(k->pak5!=0))   p=1;
  else { }
                   if(p!=0)   
{soob.sprintf("#W - MWU - Запуск пакета в ВМ%d на КМК-%d при незаверш.предыд.пакете",nom,
			                                       pornom);
          printkl(soob);
 soob.sprintf("# Запуск проигнорирован."); printkl(soob);
        return;
}

	   //--------------------------------------

    if(pesmko!=0)	   
{soob.sprintf("#I  Запуск пакета ВМ%d на КМК - %d ",(nom),pornom);
                    printkl(soob); 
soob.sprintf("#I Время запуска /дес/ t=%lu",4*ttek4);
       printkl(soob);}
  //----------------------------------------------

	mm=k->mp;	//модель памяти;
	if(pornom==0) k->pak0=1; else if(pornom==1) k->pak1=1;
    else if(pornom==2) k->pak2=1; else if(pornom==3) k->pak3=1;
	else if(pornom==4) k->pak4=1; else  k->pak5=1;
//Зафиксировали запуск пакета.
	// Далее читаем регистры;
	r2=streg(100, pornom, 0, 0, 7, k); //RTT из КМК;
//-----,-------.-------.-------;
   adr=adroo(pornom,k);//адрес описателя обмена
  ind=indpam(adr+8,k); //индекс ячейки времени начала обмена	
	//Впишем это время  в описатель;
	mm[ind]=0177777&r2;
//------,------,-----вписали----

   k->tnasper[pornom]=ttek4; //Учет времени начала передачи.
//------------------------------
//Определим длительность передачи;
	adr1=adrbs(adr,k); //адрес буфера сообщений
	dl=dlit(adr1,k);   //Определили эту длительность.
//Теперь определяем tb - момент завершения передачи;
	tb=ttek4+dl; //ед.измерения-4мкс;
	if(tb<Tmin) Tmin=tb;
//-----------------------------------------------------
//Формируем будильник;
      i=(nom)*48 + pornom*8;
	 wus[i+2]=1; //признак активности КМК0;
     wus[i+3]=1; //признак окoнчания передачи по будильнику;
     wus[i+4]=1; //ЭВМ - контроллер.
	 //wus[i+5]=(UINT)nom;      //избыточно
	 bud[i/8]=tb; //завели будильник для КМК;
		//на момент окончания передачи;


             if(pesmko!=0)
	{soob.sprintf("#  Начат первый обмен пакета BM%d с временем окончания t=%lu",nom,4*tb);
	         printkl(soob);
	}


//---- Сформируем слово состояния обмена в начале передачи:
      ind=indpam(adr,k); r0=mm[ind]; //Управляющее слово
      r3=040000; //С признаком начала передачи
	  if((0400&r0)!=0) r3=(020000|r3); 
	  //Взяли из упр. слова признак магистрали передачи 
      mm[ind+3]=r3; //Вписали слово состояния обмена.
      //Внимание: это последняя ячейка описателя обмена.
 //Она берется просто сдвигом индекса mm[ ] на 3 без indpam:
 //описатель может лежать только в одном куске модели памяти. 
	               return;     

}//============ КОНЕЦ ZAPPAK ================


     
 void stoppak(UINT pornom, ntip *k)
//Останавливает работу пакета передач для
//  КМК с номером pornom;
//Запускается функциями конца передачи, когда КМК контроллер.
 { UINT r0,*mm,p; int nom,i; 
     mm=k->mp;
	   nom=0;
	 for(i=1;i<=kolwm;i++) 
	 {
		 if(k==bazuk[i])
		{nom=i;break;}
	 }
	 if(nom==0) {printkl("# ФАТАЛЬНАЯ  ОШИБКА 1 ЭИК в stoppak !!!");  return;}
	 if(Komplex[nom]==0) 
	 {soob.sprintf("# Останов пакета в выключенной ВМ%d  на KMK-%d  !!!",nom-1,pornom);printkl(soob);}
	 nom=nom-1;  //Теперь nom  -  беликовский номер ВМ
	 
//Cначала проверка на ошибочный останов незапущенного пакета:
  p=0;	if((pornom==0) && (k->pak0==0)) p=1;
  else	if((pornom==1) && (k->pak1==0)) p=1;
  else	if((pornom==2) && (k->pak2==0)) p=1;
  else	if((pornom==3) && (k->pak3==0)) p=1;
  else	if((pornom==4) && (k->pak4==0)) p=1;
  else	if((pornom==5) && (k->pak5==0)) p=1;
  else { }
  
        if(p==1)
{soob.sprintf("#I Останов незапущенного пакета на BM%d  на КМК-%d ",nom,pornom);
                printkl(soob);
soob.sprintf("#I Время останова /дес/ t=%d",4*ttek4);
                      printkl(soob);
}

        if((pesmko!=0)&&(p==0))
{soob.sprintf("#I Останов ранее запущенного пакета на BM%d    КМК-%d",nom,pornom);
                printkl(soob);
soob.sprintf("#I Время останова /дес/ t=%d",4*ttek4);
                      printkl(soob);
}//-----------------------------------------------------------------------------  
         i=nom*48+pornom*8; 
		 wus[i+2]=0; //сделали КМК-pornom пассивным;

	if(pornom==0)
	{ 
	  k->pak0=0;      //снятие признака работы пакета.
	}
//---------
   else	if(pornom==1)
	{ 
	  k->pak1=0;
	}
//---------
  else	if(pornom==2)
	{ 
	  k->pak2=0;
	}
//---------
  else	if(pornom==3)
	{ 
	  k->pak3=0;
	}
//---------
  else	if(pornom==4)
	{ 
	  k->pak4=0;
	}
//---------
  else	
	{ 
	  k->pak5=0;
	}
//---------
        //Теперь сброс в RR разряда запуска пакета:
   r0=streg(100,pornom,0,0,6, k); //RR
   r0=(0177776&r0); //сброс его
   mm[indstreg]=r0; //запись в RR. 
   //-------------------------------
	return;
}
//========= КОНЕЦ STOPPAK ===============



 //ФУНКЦИИ, ВЛИЯЮЩИЕ  НА  ЦЕНТРАЛЬНЫЙ  КОНТРОЛЛЕР  ПРЕРЫВАНИЙ


void kpr_rfm(ntip *k) //Влияние RFM-ПК на KPR.
{ UINT r1,i,j,p;
	   for(i=0;i<3;i++)   //Три ПК.
{if(i==0)j=1; else if(i==1) j=4; else j=020;
	   //Разряды kpr в зависимости от номера i   RFM-ПК
	if((k->rtt[2*i])==0) continue;  //Нет этой пары КМК
	   //ПК опекает 2 КМК
r1=streg(52, i, 0, 0, 5, k); //RFM-i-ПК
        p=0;
   if(((1&r1)!=0)&&((2&r1)==0))       p=1;
   if(((4&r1)!=0)&&((010&r1)==0))     p=1;
   if(((020&r1)!=0)&&((040&r1)==0))   p=1;
   if(((0100&r1)!=0)&&((0200&r1)==0)) p=1;
   if(((0400&r1)!=0)&&((01000&r1)==0)) p=1;
   if(((02000&r1)!=0)&&((04000&r1)==0)) p=1;
             if(p!=0)
	wpis(50, 0, 0, 0, 5, 0, j, k);
			 else
	wpis(50, 0, 0, 0, 5, 1, j, k); 
}  
	   k->axtung=020|k->axtung;
}//========= Конец kpr_rfm() ==============


  
  void kpr_pwr(ntip *k) //Влияние RFM ПВР на KPR
  { UINT r0,zap;
    r0=streg(60,0,0,0,28,k); //Прочитали ПВР - RFM/
	if(
		(((1&r0)!=0)&&((2&r0)==0))||
        (((4&r0)!=0)&&((010&r0)==0))||
        (((020&r0)!=0)&&((040&r0)==0))|| 
        (((0100&r0)!=0)&&((0200&r0)==0))|| 
        (((0400&r0)!=0)&&((01000&r0)==0))||
        (((02000&r0)!=0)&&((04000&r0)==0))||
		(((010000&r0)!=0)&&((020000&r0)==0))||
		(((040000&r0)!=0)&&((0100000&r0)==0))
      )      zap=1;  else zap=0;
           

	 if(zap==1)   //передается запись 1 в kpr:		                               
		                               
           wpis(50,0,0,0,5,0,0100,k); //1 ->FU3 RQM
	          //иначе - вписывание 0:
	  else wpis(50,0,0,0,5,1,0100,k); //0 ->FU3 RQM         
	  
    k->axtung=020|k->axtung;   //Добавлено  15 сентября 09г. по аналогии с kpr_rfm()
	  
  }//===================================================

  

  void rqn_rfm_pk(UINT pornom,ntip *k)
  //Влияние  RQN КМК   на   RFM-ПК
  {UINT i,j,z,q,rqn,rfm,f0,f1,*mm,rn,rm;
             mm=k->mp;
   rqn=streg(100,pornom,0,0,5,k);
                          //RQN - запросы прерыв КМК
   rn=streg(100,pornom,0,0,12,k);
   rm=streg(100,pornom,0,0,11,k);
   //--------------------------------
   if(pornom<2) j=0; else if(pornom<4) j=1; else j=2;
                          //Это номер КО = номеру ПК

   rfm=streg(52,j,0,0,5,k);  //RFM - ПК
   if((pornom%2)==0) i=0; else i=1;
                               //номер КМК - 0 или 1.
   //--------------------------------------
   f0=0; f1=0; q=1; 
       for(z=0;z<8;z++)
	   {if(((q&rqn)!=0)&&((q&rm)==0)) //Есть немаскир.запрос
		{if((q&rn)!=0) f1=1; else f0=1;} 
             q=(q<<1); 
	   } 
	   //f0,f1 - признаки наличия хотя бы одной нулевой
	     // и первой линии запроса в RQN соттветственно
       //------------------------
	       if(i==0)    //КМК - 0
		{rfm=(037777777772&rfm);  //Стерли F00 , F01.
          if(f0==1) rfm=(01|rfm);
          if(f1==1) rfm=(04|rfm);
             mm[indstreg]=rfm;     return;
		}
		   else       //КМК - 1
		{rfm=(037777777657&rfm);  //Стерли F10 , F11.
          if(f0==1) rfm=(020|rfm);
          if(f1==1) rfm=(0100|rfm);
             mm[indstreg]=rfm;     return;
		}
  }//==================================================



  void wnzapkpr(ntip *k)   
	         //Выдает запрос в cause от контроллера прер.
			 //Запускается в зоне инициации комплекса в zakr.cpp.
  { UINT int0,int5,r0,*m,*mm,i,ind;
         m=k->rw; mm=k->mp; i=0;
		        while(m[i]!=0)
	{if(m[i]!=0xfff)
		{printkl("#F - MWU - Ошибка 1 в wnzapkpr.");
				                          ExitThread(TRUE);}
       if((m[i+1]==50)&&(m[i+2]==0)&&(m[i+3]==0))
		                                   //Нашли контроллер.
	   { ind=indpam(m[i+5],k); if(ind==1)
	       {printkl("#F - MWU - Ошибка с ind в wnzapkpr.");
	                                       ExitThread(TRUE);}
	      r0=mm[ind]; //Прочитали RQM.
		     if(((1&r0)!=0)&&((2&r0)==0))              int0=1;
        else if(((4&r0)!=0)&&((8&r0)==0))              int0=1;
        else if(((16&r0)!=0)&&((32&r0)==0))            int0=1;
        else if(((64&r0)!=0)&&((128&r0)==0))           int0=1;
        else if(((256&r0)!=0)&&((512&r0)==0))          int0=1;
        else if(((0200000&r0)!=0)&&((0400000&r0)==0))     int0=1;
        else if(((01000000&r0)!=0)&&((02000000&r0)==0))   int0=1;
        else if(((04000000&r0)!=0)&&((010000000&r0)==0))  int0=1;
        else if(((020000000&r0)!=0)&&((040000000&r0)==0)) int0=1;
                     else int0=0;
             //-----------------------------------
        if(((0100000000&r0)!=0)&&((0200000000&r0)==0))  int5=1;
		              else int5=0;
             //------------------------------------
        if(int0==0)   k->cause=037777775777&(k->cause);
		 else         k->cause=02000|(k->cause);
        if(int5==0)   k->cause=037777677777&(k->cause);
		 else         k->cause=0100000|(k->cause);
		              return;
       }      //----------------------
	   else
       {i=i+5; while((m[i]!=0xfff)&&(m[i]!=0))    {i=i+1;} }
	 }
  printkl("#F - MWU - Контроллер kpr  не найден в  wnzapkpr.");
	                                        ExitThread(TRUE);

  }//======= Конец wnzapkpr =====================


//------   ФУНКЦИИ ДИСПЕТЧЕРСКИЕ И ЗАКАЗЫВАЮЩИЕ  --------------

   void dispet(void)
   //Прочесывает будильник.
   //Запускается в зоне инициации комплекса в zakr.cpp 
   {int i,j,nom;  DWORDLONG t;
	   //Определим Tmin для неразбуживаемых:
	    t=0xffffffffffffffff;    
               for(i=0;wus[i+2]!=3;i=i+8)
		{if(i>=1600) 
{printkl("#F - MWU - Ошибка 1 в dispet"); return;}
		  if(wus[i+2]==0) continue; //Не активен
		  if(bud[i/8]>ttek4) //Активен но не будится
          {  if(bud[i/8]<t) t=bud[i/8];}
		 
		}//-----------------------------
                Tmin=t;
           //-------------------
	    j=0; for(i=0;wus[i+2]!=3;i=i+8)
	{ 
	 if(wus[i+2]==0)  continue; //Не активен
       if(bud[i/8]>ttek4)  continue;
	                    //Активен но не будится
	    //---------------------------------
	         //Нашли разбуживаемого:
	    j=1;  if((wus[i]==1)&&(wus[i+4]==1))  //КМК - контроллер
		{		nom=wus[i+5];                                                          
		                                                       
			kmka(wus[i+1],wus[i+3],bazuk[nom]);                                      
		   
		}
      else if((wus[i]==1)&&(wus[i+4]==2))     //КМК - оконечник
	  {
		       nom=wus[i+5];                                                              
	                                                        
		  kmkaou(wus[i+1],wus[i+7],bazuk[nom]);                                     
	  }
	  else if(wus[i]==2)                         //Модель
       razbmodel(wus[i+1],wus[i+3]);
      else {printkl("#F - MWU - Ошибка 2 в dispet");
                                  return;}
	}
   }//====================================================


  void zakaz(UINT rbl)                                                                       
   //Оформляет возможный заказ на разбуживание
   //и исполнение запросов
   // по уже готовым ident,vxod,vixod
   // после передачи данных по МКО,нач.установки и после
   //разбуживания.  rbl - размер предшествующего блока
   //в vixod.
   //rbl=0,	если заказ идет после разбуживания по razbmodel.
   // На пенал не влияет.
  { UINT z,pornom; int i,j;
                  z=rbl;
    mmm:
				  if(vixod[z]==0) return; //Конец всех заказов.  
    if((vixod[z]==5)&&((vixod[z+1]==REASON_TACT)||
(vixod[z+1]==REASON_TIMER)||(vixod[z+1]==REASON_TIMER_CYCLE)))	
	{
		razb(z); z=z+5;}     //Заказ на разбуживание.
    else if((vixod[z]==6)&&(vixod[z+1]==REASON_INTERRUPT_I))
	{impuls(z); z=z+6;}   //Импульсные запросы.
    else if((vixod[z]==6)&&(vixod[z+1]==REASON_INTERRUPT_P))
	{potenz(z); z=z+6;}   //Потенциальные запросы.
	else if((vixod[z]==6)&&(vixod[z+1]==REASON_INTERRUPT_UP))
	{snatp(z); z=z+6;}    //Снятие потенц. сигналов.
	else if((vixod[z]==5)&&(vixod[z+1]==REASON_TICK))
    {mwrem(0); z=z+5; }     //Выдача метки времени 0
	else if((vixod[z]==5)&&(vixod[z+1]==REASON_TICK1))
    {mwrem(1); z=z+5; }     //Выдача метки времени 1

//	else if((vixod[z]==5)&&(vixod[z+1]==REASON_TM_S_CVM))
//    {telemetr(k); z=z+5; } //Выдача программной телеметрии с RTlm.
//	else if((vixod[z]==5)&&(vixod[z+1]==REASON_TM_H_CVM))
//    { z=z+5; } //Выдача аппаратной телеметрии, - игнорируется.

	else if((vixod[z+1]>=REASON_OU0)&&(vixod[z+1]<=(REASON_OU0+5)))  //Заказ на обмен моделью-контроллером.
	{pornom=vixod[z+1]-REASON_OU0;   
//Номер МКО заказывается моделью с помощью REASON_OUI, где I - номер МКО(0-5), т.е. pornom.
	
	           //Ищем ВМ, активную на данном МКО:
	                          j=0; //будет моим номером ВМ, активной на МКО - pornom.
							  for(i=1;i<=kolwm;i++)   
					{if(bazuk[i]->wds[pornom]!=0) {j=i; break;}}  //Если j>=1, нашли
           //-----------------------------------------------------------
             if((pornom>5)||(j==0))  //бяка
			 {printkl("# Ошибка заказа номера МКО моделью-контроллером"); 
               printkl("# Заказ проигнорирован"); z=z+vixod[z]; 
			 }//----------
             else 
			 {kontrmod(vixod[z+1]-REASON_OU0,vixod[z+5],ident,z,bazuk[j]);
	                       //Здесь vixod[z+5] - ком.слово. 
	           z=z+vixod[z];
			 }
	}          

	
	else if(vixod[z+1]==REASON_HIM)  //Вписывание в память от модели в DLL 
    {him(z); z=z+vixod[z]; }  



	//--------------------------------
    else
{  if(pesosib!=0) 
{soob.sprintf("#I Непредусмотр.и проигнорир.заказ модели /дес/ %d",
			                                          ident);
              printkl(soob);
 soob.sprintf("# MWU    1 - %x",vixod[z]);  printkl(soob);
 soob.sprintf("# MWU    2 - %x",vixod[z+1]);printkl(soob);
 soob.sprintf("# MWU    3 - %x",vixod[z+2]);printkl(soob);
 soob.sprintf("# MWU    4 - %x",vixod[z+3]);printkl(soob);
 soob.sprintf("# MWU    5 - %x",vixod[z+4]);printkl(soob);
 soob.sprintf("# MWU    6 - %x",vixod[z+5]);printkl(soob);

	 return;
}
}  
       goto mmm;
  }//=============================================

  void him(UINT z)
  //Обработка заказа на изменение ячеек ОЗУ.
  // ident,vxod,vixod   уже определены.
         //Пишет информацию-ответ  в пенал.
  {int n,i,j,p,pp,ind,*uk;  short st;
   UINT kk,ks,ki,kr,ka,jj;     DWORDLONG *u;
   ntip *k;
                  pp=0;
         for(j=0;pModTable[j].ID!=0;j++)
	{if(pModTable[j].ID!=ident) continue;
     if(pModTable[j].Reason==vixod[z+1])
	 {pp=1; ind=j;                    //индекс модели
	  p=(int)pModTable[j].Prioritet; //приоритет заказа модели
      break;
     }
	} //-----------------------------
                if(pp==0)
	{      if(phim!=0)
		{soob.sprintf("# Модель %d  не имеет REASON_HIM.",ident);
		 printkl(soob); printkl("# Заказ не выполнен");
		}
       return;
	}//-------------------------------
         n=(int)vixod[z]; n=n-5;  if(((n%3)!=0)||(n<=0))	 
	{soob.sprintf("# Ошибка триад в заказе модели %d",ident);
		 if(phim!=0) {printkl(soob); printkl("# Заказ не выполнен");}
      return;
	}//-------------------------------
      if((ipen0+2+vixod[z])>(rpenal-1))
	  {printkl("#F -MWU-him  Буфер для очереди заданий мал. Задача снята.");
	     ExitThread(TRUE);
      }//------------------------------------------------------------------
                    k=bazuk[vixod[z+4]+1];   //Указатель на карту по заданному в vixod[z+4] номеру  ВМ
           //----- НАЧИНАЕМ  ВПИСЫВАТЬ  В  ПЕНАЛ -----------------
       penal[ipen0]=vixod[z];
       penal[ipen0+1]=(int)REASON_HIM;
	   penal[ipen0+2]=vixod[z+4];    //Номер ЭВМ
	       uk=&(penal[ipen0+3]);  
		   u=(DWORDLONG *)uk;
           u[0]=(DWORDLONG)ttek4;  //Время
       //---------  Шапка  вписана  в  информацию-ответ.-----

            for(i=5;i<(int)vixod[z];i=i+3)         // Цикл по триадам, по i
	{st=sts2(vixod[z+i+1],k);  //Прочитали число в sislo.
              if(st!=0)
		{     if(phim!=0)
			{soob.sprintf("# Ошибка адреса %x  чтения числа при REASON_HIM",vixod[z+i+1]);
			  printkl(soob); printkl("# Заказ не выполнен");
			}
          return;
		}//----------------------------------------------
             ks=sislo;           //Прочитанное старое значение
			 kk=vixod[z+i];      //Код команды модели
			 ka=vixod[z+i+1];    //Код адреса
			 ki=vixod[z+i+2];    //Код исполнения
			 //--------------------------------------
          
                  if(kk==0)          //ОБНУЛЕНИЕ
	{kr=(0xffffffff^ki);   //инвертировали код исполнения
      kr=(ks&kr);          //Это результат
	  //---------------------------------
              penal[ipen0+i]=ks;    //Внесли старое значение
              penal[ipen0+i+1]=ka;  //Переписали адрес
			  penal[ipen0+i+2]=kr;  //Вписали результат в ответ
			       sislo=kr;    //Для записи по zas2.
            st=zas2(ka,k);          //Записали в ОЗУ
              if(st!=0)
		{     if(phim!=0)
			{soob.sprintf("# Ошибка адреса %x  записи числа при REASON_HIM",vixod[z+i+1]);
			  printkl(soob); printkl("# Заказ не выполнен");
			}
          return;
		}//-----------------------------
	}//----------------- Конец обнуления --------------------
                  
             else if(kk==1)                    //Приформировывание 1
	{kr=ks|ki;                 //Это результат
	  //---------------------------------
              penal[ipen0+i]=ks;    //Внесли старое значение
              penal[ipen0+i+1]=ka;  //Переписали адрес
			  penal[ipen0+i+2]=kr;  //Вписали результат в ответ
			       sislo=kr;    //Для записи по zas2.
            st=zas2(ka,k);          //Записали в ОЗУ
              if(st!=0)
		{     if(phim!=0)
			{soob.sprintf("# Ошибка адреса %x  записи числа при REASON_HIM",vixod[z+i+1]);
			  printkl(soob); printkl("# Заказ не выполнен");
			}
          return;
		}//-----------------------------
	}//----------------- Конец приформировывания 1 ------------
			 
                else if(kk==2)                  //Полная замена ячейки
	{kr=ki;				 //Это результат
	  //---------------------------------
              penal[ipen0+i]=ks;    //Внесли старое значение
              penal[ipen0+i+1]=ka;  //Переписали адрес
			  penal[ipen0+i+2]=kr;  //Вписали результат в ответ
			       sislo=kr;    //Для записи по zas2.
            st=zas2(ka,k);          //Записали в ОЗУ
              if(st!=0)
		{     if(phim!=0)
			{soob.sprintf("# Ошибка адреса %x  записи числа при REASON_HIM",vixod[z+i+1]);
			  printkl(soob); printkl("# Заказ не выполнен");
			}
          return;
		}//-----------------------------
	}//----------------- Конец полной замены ячейки ------------

               else if(kk==3)                 //Чтение числа
	{
              penal[ipen0+i]=ks;    //Внесли старое значение
              penal[ipen0+i+1]=ka;  //Переписали адрес
			  penal[ipen0+i+2]=ks;  //Вписали результат в ответ
	}//--------------------- Конец чтения числа --------------

                  else
	{soob.sprintf("# Ошибка команды %d  при REASON_HIM",kk);
		if(phim!=0) {printkl(soob); printkl("# Заказ не выполнен");}		  
      return;
	}//---------------------------


	}//======== Конец цикла по триадам,  по i  . 

        penal[ipen0+i]=ind;          //Индекс модели в Table
        penal[ipen0+i+1]=p;          //Приоритет заказа модели
		   //-----------------------------
		ipen0=ipen0+i+2;        //Сдвинули для дальнейшего
		if(p>maxpr)   maxpr=p;
		//---------------------------------
		        //---- А возможные спровоцированные прерывания?
		 
		      k->axtung=(0637|k->axtung);
              //-------------------------
                  for(i=0;i<6;i++)
		{if(k->rtt==0) continue;
          jj=(UINT)i;
		  rqn_rfm_pk(jj,k);
		}//----------------------------
               kpr_rfm(k);
               kpr_pwr(k);      //Учли все воздействия на прерывания

       
  }//======= конец  him =====================



   void razb(UINT z)
   //Обработка заказа на разбуживание. Запускаетя в zakaz().	   
   // z - индекс начала заказа в vixod.
   //ident уже определен.
   //Пишет в будильник или оформляет дела с тактовым разбуживанием. 
   {UINT tt; short i; DWORDLONG t; 
               int j,p;
        if(dlln==0) return;
		          p=0;
          for(j=0;pModTable[j].ID!=0;j++)
	{
      if(pModTable[j].ID!=ident) continue;
      if(pModTable[j].Reason==vixod[z+1])  {p=1;
	  break;}
	}//----------------------------
	if(p==0) {
			  if((ttek4>0)&&(pesrazb==1))
			  {soob.sprintf("#I Модель ID %d не имеет Reason=%d",ident,vixod[z+1]);
			  printkl(soob); }
	  return;} //Не имеет право на такой заказ.
     //*********** Теперь модель имеет право на такой заказ.

    tt=(vixod[z+3]+2)/4;       //tt=vixod[z+3];
		
   t=(DWORDLONG)tt;
     //Проверка - не сброс ли заказа
     if(tt==0) //сброс
	 { 	 if(vixod[z+1]==REASON_TACT) //Сброс тактового разбуж.
		{ wkltakt=0;     
           if((ttek4>0)&&(pesrazb==1))
{soob.sprintf("#I  Модель номер %d",ident); printkl(soob);
 printkl("#I сбросила тактовое разбуживание "); 
}  
	      return;
		}
       //--- Теперь сброс индивидуального заказа
		 for(i=288;(wus[i+2])!=3;i=i+8)
	{if(i>=1600) {printkl("#F - MWU - Ошибка 3 в razb. Отказ"); 
		                                ExitThread(TRUE);}
		  if((wus[i+1])!=ident) continue; //Не тот номер	
			 wus[i+2]=0;  //Пассивность - заказ сброшен.
              break;
	}
           if((ttek4>0)&&(pesrazb==1))
{soob.sprintf("# Сброс индивидуального заказа на разбуживание модели номер %d",ident);
		   printkl(soob); }   
	        return;
     }
   //--------- Конец проверки сброса -----****************

   if(vixod[z+1]==REASON_TACT)  //Заказ такта разбуживания
   { wkltakt=1; takt=tt;  // tt  теперь отлично от 0.
     zwonok=ttek4+(DWORDLONG)takt;   
           if((ttek4>0)&&(pesrazb==1)&&(pes==1))
{soob.sprintf("#I  Модель номер %d",ident); printkl(soob);
 soob.sprintf("#I Установила период тактового разбуживания /дес/ =%d",(4*tt));
            printkl(soob);
}  
   return;
   }//----  Конец заказа такта разбуживания --*************

           if((ttek4>0)&&(pesrazb==1)&&(pes==1))                              
{soob.sprintf("#I  Модель номер %d",ident); printkl(soob);   

 soob.sprintf("#I Сделан заказ на разбуживание, Delta/дес/ =%d",(4*tt)); 
           printkl(soob);
		   }  
		  //-------------------------------
		        for(i=288;(wus[i+2])!=3;i=i+8)
		{if(i>=1600) {printkl("#F - MWU - Ошибка 1 в razb. Отказ"); 
		                                ExitThread(TRUE);}
		 if((wus[i+1])!=ident) continue; //Не тот номер	
			 wus[i+2]=1;  //Активность
             if(vixod[z+1]==REASON_TIMER)  //Разовый заказ
				{wus[i+3]=2; t=(t+ttek4); bud[i/8]=t;
           if((ttek4>0)&&(pesrazb==1)&&(pes==1))                                
{soob.sprintf("#I Время будущего разового разбуж./дес/ t=%lu ",(4*t));   
		   printkl(soob);}
		if(t<(Tmin))  Tmin=t;
           if((ttek4>=0)&&(pesrazb==1)&&(pes==1))                                     
{soob.sprintf("#I Время заказа текущее/дес/ =%lu",(4*ttek4));
		   printkl(soob);}
				   return; 
				 }
           //-----------------------------------
	else if((vixod[z+1])==REASON_TIMER_CYCLE)  //Циклический заказ
				 {wus[i+3]=3; t=(t+ttek4); bud[i/8]=t;
				   if(t<(Tmin))  Tmin=t;
				   zraz[ident]=tt;
           if((ttek4>0)&&(pesrazb==1)&&(pes==1))
{soob.sprintf("#I Время будущего циклического разбуж./дес/ t=%lu ",(4*t));   
		   printkl(soob);}
           if((ttek4>0)&&(pesrazb==1)&&(pes==1))
{soob.sprintf("#I Время  заказа текущее/дес/ =%lu",(4*ttek4));
		   printkl(soob);}				   
				   return;
				 }
                else {printkl("#F - MWU - Ошибка 2 в razb. Отказ");
                                         ExitThread(TRUE);}
			}
 }//============================================================


   void impuls(UINT z)
    //Воспринимает импульсные запросы. Запускается в zakaz().
    // z - индекс начала заказа в vixod.
	//Влияет на RQI   ПВР
{UINT rqi,r; int i; ntip * k;   int j,p; UINT roff,rer0;

        if(dlln==0) return;

		          p=0;
          for(j=0;pModTable[j].ID!=0;j++)
	{
      if(pModTable[j].ID!=ident) continue;
      if(pModTable[j].Reason==vixod[z+1])  {p=1;
	  break;}
	}//----------------------------
	if(p==0) {	  return;} //Не имеет право на такой заказ.
     //*********** Теперь модель имеет право на такой заказ.

      for(i=1;i<=kolwm;i++)     //Переберем все ВМ.

   { if(Komplex[i]==0) continue;
	  k=bazuk[i];
     rqi=streg(60,0,0,0,20,k); //RQI
                        r=rqi;
	 roff=streg(60,0,0,0,29,k);
		if((04&roff)==0)   //Не отключ.7-15
		  r=r|(0177600&vixod[z+5]);	
        if((02&roff)==0)   //Не отключ.0-6
          r=r|(0177&vixod[z+5]);
		wpis(60,0,0,0,20,2,r,k);   //Записали RQI
		//----------------------------------
        if((01&roff)==0)             //Не отключ.запись на REr0
		{rer0=streg(60,0,0,0,23,k);  //REr0
		  rer0=rer0|(0177600&vixod[z+5]);
         wpis(60,0,0,0,23,2,rer0,k);
		}
        //----------------------------
          
	             obrrqi(k);  //Отразили в RFM  и   KPR

	} //---конец по одной ВМ

}//==================================================



   void impulsNAXAEW(void)
    //Воспринимает импульсные запросы из командного файла. 
        //Воздействует на все машины
	//Влияет на RQI   ПВР
{UINT rqi,r; int i; ntip * k;    UINT roff,rer0;

      for(i=1;i<=kolwm;i++)     //Переберем все ВМ.

   { if(Komplex[i]==0) continue;
	  k=bazuk[i];
     rqi=streg(60,0,0,0,20,k); //RQI
                        r=rqi;
	 roff=streg(60,0,0,0,29,k);
		if((04&roff)==0)   //Не отключ.7-15
		  r=r|(0177600&NAXAEW);	
        if((02&roff)==0)   //Не отключ.0-6
          r=r|(0177&NAXAEW);
		wpis(60,0,0,0,20,2,r,k);   //Записали RQI
	
		//----------------------------------
        if((01&roff)==0)             //Не отключ.запись на REr0
		{rer0=streg(60,0,0,0,23,k);  //REr0
		  rer0=rer0|(0177600&NAXAEW);
         wpis(60,0,0,0,23,2,rer0,k);
		}
        //----------------------------
          
	             obrrqi(k);  //Отразили в RFM  и   KPR

	} //---конец по одной ВМ

}//================= end impulsNAXAEW =============================




   void potenz(UINT z)
    //Реализация потенциальных запросов. Запускаетя в zakaz().
	// z - индекс начала заказа в vixod.
   {UINT rfp,rqp,rfpstar,now;  ntip *k;   int i,j,p; UINT roff;
              
        if(dlln==0) return;

		          p=0;
          for(j=0;pModTable[j].ID!=0;j++)
	{
      if(pModTable[j].ID!=ident) continue;
      if(pModTable[j].Reason==vixod[z+1])  {p=1;
	  break;}
	}//----------------------------
	if(p==0) {	  return;} //Не имеет право на такой заказ.
     //*********** Теперь модель имеет право на такой заказ.

      for(i=1;i<=kolwm;i++)     //Переберем все ВМ.
   { if(Komplex[i]==0) continue;
	  k=bazuk[i];

roff=streg(60,0,0,0,29,k);  //ROff
if((010&roff)!=0)  continue;             //отключен прием

     rfp=streg(60,0,0,0,27,k); rfp=(0177777&rfp);   //RFP
	 rfpstar=rfp;   //Запомнили старое значение RFP
	       //---------------------------
	 rfp=(vixod[z+5]|rfp); rfp=(0177777&rfp);
	   wpis(60,0,0,0,27,2,rfp,k);     //Записали новое значение RFP.
	   //--------------------------------------
	   now=0177777^rfpstar;  //Отметили 1-ми нули в старом RFP
	   now=(now&vixod[z+5]); //Отметили в now  новые запросы
       //---------------------------------------
	   rqp=streg(60,0,0,0,24,k); rqp=(0177777&rqp); //RQP
              rqp=(rqp|now);  //Приписали новые запросы к RQP
			  rqp=(0177777&rqp);
         wpis(60,0,0,0,24,2,rqp,k); //Записано новое значение RQP.
       //----- RQP и RFP  уже зафиксированы ----------------
                   obrrqp(k);  //Отразили в RFM и в KPR 

	  }
   }//============ Конец potenz ========================

   void snatp(UINT z)
   //Снятие потенциальных сигналов. Запускаетя в zakaz().
   // z - индекс начала заказа в vixod.
   {UINT rfp,r; int i,j,p; ntip *k;
   
        if(dlln==0) return;

		          p=0;
          for(j=0;pModTable[j].ID!=0;j++)
	{
      if(pModTable[j].ID!=ident) continue;
      if(pModTable[j].Reason==vixod[z+1])  {p=1;
	  break;}
	}//----------------------------
	if(p==0) {	  return;} //Не имеет право на такой заказ.
     //*********** Теперь модель имеет право на такой заказ.

      for(i=1;i<=kolwm;i++)     //Переберем все ВМ.

   { if(Komplex[i]==0) continue;
	  k=bazuk[i];

     rfp=streg(60,0,0,0,27,k); rfp=(0177777&rfp); //RFP
     r=0177777^vixod[z+5]; r=(r&rfp); 
	                        //Новое значение RFP
          wpis(60,0,0,0,27,2,r,k);
		      obrrqp(k);   //Отражение в RFM и в KPR 

	}
  }//======== Конец snat ===============================

//+++++++++++++++++ Заготовка коррекции от 10.06.04   новая

 void kontrmod(UINT pornom,UINT komslowo,UINT indm,UINT zz,ntip *k)
//Оформляет заказ модели-контроллера на обмен, пишет в будильник.
//pornom - порядковый номер КМК, komslowo - командное слово.
// indm  - идентификационный номер модели в Table,
// zz    - индекс начала заказа в vixod. 
// В пенал ничего не пишет, пишет в будильник.
             //Запускаетя в zakaz().
 {UINT rsw,uprsl,adr,ind,*mm,rtt,msw,nou,nou1,r1,mod,pr1,pr2,r,kkom,i,j;
  DWORDLONG z,tb;  UINT nom; int ii;
              nom=0;
  for(ii=1;ii<=kolwm;ii++) {if(k==bazuk[ii]) {nom=(UINT)ii; break; }  }  //nom - мой номер ВМ
   if((nom>(UINT)kolwm)||(nom<1))
   {printkl("# Ошибка 1 в konrmod. Заказ модели контроллера проигнорирован!"); return;}
   //-------------------------------------------------------
    if(Komplex[nom]==0)
	{soob.sprintf("Модель-контроллер %d заказывает обмен с выключенной ВМ%d. Отказ.",indm,nom-1);
	 printkl(soob); return;
	}//------------------------------------

  if(k==0) {printkl("# Нет ведущей ВМ на МКО. В kontrmod. Ошибка!"); return;}
          //----------------------------
  pr1=0; pr2=0; //Признаки pr1 - недоп.ком.слово; pr2 - не передача последней ком.
       msw=0;    //Будущее слово состояния обмена.
                      mm=k->mp; //Модель памяти.
   mod=streg(100,pornom,0,0,6,k);  //RR
   mod=(0177777&mod);  mod=(mod>>13); //Тип-функция ЭВМ по RR.
                      //------------------
       if((mod==3)||(mod==7))   //ЭВМ контроллер
	{if(pesmko!=0)
     printkl("# ЭВМ и модель - контроллеры. Обмен игнорируется");
	   return;
	}//-----------------------
	        if(pesmko!=0)
{soob.sprintf("# Начало обмена модели-контроллера %d с КМК %d",pModTable[indm].ID,pornom);
                    printkl(soob);
 soob.sprintf("# Время начала обмена /дес/ t=%lu",4*ttek4);
                   printkl(soob);
}//----------------------------
    if(dopkomsl(pornom,komslowo,k)!=0) //Недопустимое ком. слово
	{	    pr1=1;
	rsw=streg(100,pornom,0,0,9,k);  //RSW - рег ответного слова
     rsw=(02000|rsw); //Вписали признак недопустимости ком.слова
	                  //Но пока еще не в регистр.
	 msw=(01000|msw); 
	  //Вписали этот же признак в будущее слово состояния обмена     
               if(nedop!=0)
		{soob.sprintf("# Недопустимое командное слово %x для КМК %d",komslowo,pornom);
		 printkl(soob); 
		}
			   uprsl=uprslowo(pornom,komslowo,k); //Взяли управляющее слово
               if((010000&uprsl)!=0)  rsw=0177777; //Означает - нет ответного слова
	}//-----------------
	else {rsw=streg(100,pornom,0,0,9,k); rsw=(0175777&rsw);}
       //------------------------------------------------------
	   uprsl=uprslowo(pornom,komslowo,k); //Взяли управляющее слово

	       if((0400&uprsl)!=0)    //Абонент занят по управл.слову
	{rsw=(010|rsw); //Отразили это в проекте ответн.слова
        
	}//-------------------------------------------------
       wpis(100,pornom,0,0,9,2,rsw,k); //Вписали в рег.ответн.слова
                  //----------------------
   adr=adroo(pornom,k);  //Адрес описателя обмена
   ind=indpam(adr,k);  //Индекс управляющего слова в модели памяти
   mm[ind]=uprsl;  //Вписали управл.слово на его место в описателе
   mm[ind+1]=0177777&komslowo; //Вписали на его место ком.слово
   rtt=streg(100,pornom,0,0,7,k); //RTT
   rtt=(0177777&rtt);
   mm[ind+2]=rtt;   //Вписали на место время обмена по RTT
       //----------------------------------------
   nou=komslowo>>11; nou=(037&nou);  //Адрес в ком.слове
   nou1=rsw>>11;   nou1=(037&nou1);  //Адрес в ответном слове
           if(nou!=nou1)
	{printkl("# Адреса в ответном и ком.словах не совпадают");
     printkl("# Такой режим не предусмотрен. Обмен некорректен.");
	      
	}//----------------------------
      msw=(040000|msw);      //Признак начала обмена
	  mm[ind+3]=0177777&msw; //Вписали его в слово состояния обмена
	     //----- Теперь определим длительность передачи:
	  r1=01740&komslowo;   
	     if((r1==0)||(r1==01740))  z=12;
		 else
	{r1=037&komslowo; if(r1==0) r1=32;
     r1=(r1+2); r1=((5*r1)+2);
	 z=(DWORDLONG)r1; //z - время передачи в 4-мкс единицах
	}//-----------------------------------------------
           tb=ttek4+z; 
	//tb- время разбуживания ЭВМ при конце передачи из модели
		    if(tb<Tmin)  Tmin=tb; 
   r1=vixod[zz]-6; //Кол-во данных после ком.слова в vixod.
      if(r1>32)
	  { if(pesosib!=0)
        printkl("#E Ошибка, число слов передачи в vixod >32. Отказ");
	    return;
      }//------------------------------
          i=35*pornom; //Начальный индекс зоны в bufou.
      k->bufou[i]=r1; //Записали кол-во данных в bufou.
           if(r1>0)  //Есть данные
{for(j=1;j<=r1;j++) k->bufou[i+j+2]=0177777&vixod[zz+j+5];}
 //Сформировали зону данных в bufou.
 //Теперь впишем ком.слово в рег.последней команды RCW ,		   
 //если оно допустимое и не передать последнюю команду:
   r=01740&komslowo; if((r==0)&&(r==01740)) //Ком.управления
   {kkom=037&komslowo; //Код команды
     if(kkom==022) //Передать последнюю команду
       pr2=1;
   }//-----------------------------------------
       if((pr1==0)&&(pr2==0)) //Надо вписывать
    wpis(100,pornom,0,0,13,2,komslowo,k); 
	   //--- Вписали --------------------------

              //------- Теперь заполняем будильник:

     ii=(nom-1)*48+pornom*8;
	   if(wus[ii+5]!=nom)
	   {printkl("Ошибка 2 в kontrmod. Заказ модели-контроллера проигнорирован."); return; }
       //--------------------------------	 

	   wus[ii+2]=1;  //Активность
       wus[ii+4]=2;  //ОУ
	   //wus[ii+5]=nom;       //избыточно
	   wus[ii+7]=indm;//индекс модели в Table
	   bud[ii/8]=tb;  //Время разбуживания
     //---------------------------------

        //и exe=1 в RR:
    mod=streg(100,pornom,0,0,6,k);  //RR
    mod=1|mod; // exe=1
	wpis(100,pornom,0,0,6,2,mod,k);





 }//==========   КОНЕЦ kontrmod  =================

  


//-------- ФУНКЦИИ, НАПОЛНЯЮЩИЕ  И ОБРАБАТЫВАЮЩИЕ  ПЕНАЛ ---------------


 void razbmodel(UINT iden,short w)
//Только набивает пенал приказом на разбуживание
//модели iden. w=1 - тактовое, w=2 - разовое, 
//w=3 - циклическое.
     //Запускается dispet().
 {DWORDLONG *u; int p,ind,i,*uk; UINT R;
        if(w==1) R=REASON_TACT;
   else if(w==2) R=REASON_TIMER;
   else if(w==3) R=REASON_TIMER_CYCLE;
   else {printkl("#F Ошибка 1 программы в razbmodel");
                ExitThread(TRUE);}
      //--- Найдем индекс ind и приоритет p модели: 
   ind=-1;   for(i=0;pModTable[i].ID!=0;i++)
   {if((pModTable[i].ID==iden)&&(pModTable[i].Reason==R))
	{ind=i; p=(int)pModTable[i].Prioritet; break;}
   }//------------------------ 
      if(ind==(-1))
	{printkl("#F Ошибка 2 программы в razbmodel");
                  ExitThread(TRUE);}
	  //----- Нашли ind  и  p. 
     if((ipen0+7)>(rpenal-1))
{printkl("#F - Буфер для очереди заданий мал - razbmodel");
              ExitThread(TRUE);}
      //------- Теперь вписывание приказа в пенал: ---- 
      penal[ipen0]=5; 
      penal[ipen0+1]=(int)R;
	  penal[ipen0+2]=0;
	  uk=&penal[ipen0+3]; u=(DWORDLONG *)uk;
	  u[0]=(DWORDLONG)(4*ttek4);  //Вписали время.
      penal[ipen0+5]=i;   //индекс модели в Table.
      penal[ipen0+6]=p; 
          //--- приказ вписан. ------
      ipen0=(ipen0+7);   //Сдвинули на следующую запись.
       if(p>maxpr)    maxpr=p;  //Коррекция мах.приор.

 }//============= КОНЕЦ razbmodel =====================


  void widtsinxr(ntip *k)
//Выдача сигнала во вне от срабатывания тайм.синхронизации:
//Поиск откликающихся и вписывние им приказов в пенал.
           //Запускается  mwrem() и sttmsi() через rabts.
 {int i,p,*uk,rea; DWORDLONG *u; UINT nom;
	 //-----------------------------------

              if(dlln==0) return;

         nom=0; for(i=1;i<=4;i++)
		 {if(bazuk[i]==k) {nom=i; break;}
         }//--------------------------------
               if(nom==0)
    {printkl("#F - widtsinxr - Фатальная ошибка СУБМ в указателе карты процессора."); return;}
                       //  nom -Номер по Романову
           //------------------------------------
			  
     rea=reas(nom,0);    //Определили Reason

     if(pestsinxr==1)
	 {soob.sprintf("#I Выдача из БЦВМ сигнала от тайм.синхронизации 0 при Т=%lu",(4*ttek4));
		printkl(soob);}
  //------------------------------------------


	       for(i=0;pModTable[i].ID!=0;i++)
	{if(pModTable[i].Reason!=(UINT)rea)
	          continue;  //Не откликается.
     if((ipen0+7)>(rpenal-1))
{printkl("#F - Буфер для очереди заданий мал - widtsinxr");
              ExitThread(TRUE);}
      //---------------------------------- 
      //----- Теперь вписываем приказ в пенал:
      penal[ipen0]=5; 
      penal[ipen0+1]=rea;
	  penal[ipen0+2]=0;
	  uk=&(penal[ipen0+3]); u=(DWORDLONG *)uk;
	  u[0]=(DWORDLONG)ttek4;  //Вписали время.
      penal[ipen0+5]=i;   //индекс модели в Table.
	  p=(int)pModTable[i].Prioritet;
      penal[ipen0+6]=p; 
          //--- приказ вписан. ------
      ipen0=(ipen0+7);   //Сдвинули на следующую запись.
       if(p>maxpr)    maxpr=p;  //Коррекция мах.приор.

	}//--------- конец          цикла --------
}//================ КОНЕЦ widtsinxr =======================
		   

  void widtsinxr1(ntip *k)
//Выдача сигнала во вне от срабатывания тайм.синхронизации:
//Поиск откликающихся и вписывние им приказов в пенал.
           //Запускается  mwrem() и sttmsi() через rabts.
 {int i,p,*uk,rea; DWORDLONG *u; UINT nom;
	 //-----------------------------------
              if(dlln==0) return;

         nom=0; for(i=1;i<=4;i++)
		 {if(bazuk[i]==k) {nom=i; break;}
         }//--------------------------------
               if(nom==0)
    {printkl("#F - widtsinxr1 - Фатальная ошибка СУБМ в указателе карты процессора."); return;}
                       //  nom -Номер по Романову
           //------------------------------------



    rea=reas(nom,1);    //Определили Reason

     if(pestsinxr==1)
	 {soob.sprintf("#I Выдача из БЦВМ сигнала от тайм.синхронизации 1 при Т=%lu",(4*ttek4));
		printkl(soob);}
  //------------------------------------------
	       for(i=0;pModTable[i].ID!=0;i++)
	{if(pModTable[i].Reason!=(UINT)rea)
	          continue;  //Не откликается.
     if((ipen0+7)>(rpenal-1))
{printkl("#F - Буфер для очереди заданий мал - widtsinxr1");
              ExitThread(TRUE);}
      //---------------------------------- 
      //----- Теперь вписываем приказ в пенал:
      penal[ipen0]=5; 
      penal[ipen0+1]=rea;
	  penal[ipen0+2]=0;
	  uk=&(penal[ipen0+3]); u=(DWORDLONG *)uk;
	  u[0]=(DWORDLONG)ttek4;  //Вписали время.
      penal[ipen0+5]=i;   //индекс модели в Table.
	  p=(int)pModTable[i].Prioritet;
      penal[ipen0+6]=p; 
          //--- приказ вписан. ------
      ipen0=(ipen0+7);   //Сдвинули на следующую запись.
       if(p>maxpr)    maxpr=p;  //Коррекция мах.приор.

	}//--------- конец          цикла --------
}//================ КОНЕЦ widtsinxr1 =======================
  
  
  int reas(int wm,int ts)
 // Опеределяет и возвращает REASON по номеру машины и номеру таймера синхр. 
  { 
	  if((wm<1)||(wm>kolwm))
	  {printkl("# F Ошибка с номером ВМ при срабат.таймера синхрониз."); return(100);}
	  if((ts<0)||(ts>1))
	  {printkl("# F Ошибка с номером таймера синхрониз."); return(101);}

    if((wm==1)&&(ts==0)) return((int)REASON_TACT_CVM00);
    if((wm==1)&&(ts==1)) return((int)REASON_TACT_CVM01); 
    if((wm==2)&&(ts==0)) return((int)REASON_TACT_CVM10);  
    if((wm==2)&&(ts==1)) return((int)REASON_TACT_CVM11);
	if((wm==3)&&(ts==0)) return((int)REASON_TACT_CVM20);
    if((wm==3)&&(ts==1)) return((int)REASON_TACT_CVM21);
    if((wm==4)&&(ts==0)) return((int)REASON_TACT_CVM30);
         return((int)REASON_TACT_CVM31);
  }//================= Конец reas =========================


 void retransl(int bit,ntip *k)  //Ретрансляция МВ. Только набивка пенала.
	                     //bit - направления ретрансляции из БЦВМ.
     //Запускается pwr()  и  mwrem().
 {int i,p,*uk; DWORDLONG *u;
	 //-----------------------------------

   return;   //***********  В ЦБК нет.

                if(dlln==0) return;
	       for(i=0;pModTable[i].ID!=0;i++)
	{if(pModTable[i].Reason!=REASON_TICK_CVM)
	          continue;  //Не откликается.
     if((ipen0+8)>(rpenal-1))
{printkl("#F - Буфер для очереди заданий мал - retransl");
              ExitThread(TRUE);}
      //---------------------------------- 
      //----- Теперь вписываем приказ в пенал:
      penal[ipen0]=6; 
      penal[ipen0+1]=(int)REASON_TICK_CVM;
	  penal[ipen0+2]=0;
	  uk=&(penal[ipen0+3]); u=(DWORDLONG *)uk;
	  u[0]=(DWORDLONG)ttek4;  //Вписали время.
      penal[ipen0+5]=bit;   //
      penal[ipen0+6]=i;   //индекс модели в Table.
	  p=(int)pModTable[i].Prioritet;
      penal[ipen0+7]=p; 
          //--- приказ вписан. ------
      ipen0=(ipen0+8);   //Сдвинули на следующую запись.
       if(p>maxpr)    maxpr=p;  //Коррекция мах.приор.

	}//--------- конец          цикла --------

 }//================ КОНЕЦ retransl ==================== 


  void telemetr(ntip *k)
//Выдача моделям с регистра RTlm
       //Запускается в zakaz().
 {int i,p,*uk,bit; DWORDLONG *u;
	 //-----------------------------------

       return;   //***********  В ЦБК нет.

              if(dlln==0) return;
    bit=(int)streg(60,0,0,0,15,k);  //RTlm
    bit=(0100377&bit);
     //-----------------------------------
          if(pestelem==1)
{soob.sprintf("#I Выдача моделям с регистра телеметрии RTlm. Kод=%x",bit);
      printkl(soob);
}//---------------------------------------------
	       for(i=0;pModTable[i].ID!=0;i++)
	{if(pModTable[i].Reason!=REASON_TM_S_CVM)
	          continue;  //Не откликается.
     if((ipen0+8)>(rpenal-1))
{printkl("#F - Буфер для очереди заданий мал - telemetr");
              ExitThread(TRUE);}
      //---------------------------------- 
      //----- Теперь вписываем приказ в пенал:
      penal[ipen0]=6; 
      penal[ipen0+1]=(int)REASON_TM_S_CVM;
	  penal[ipen0+2]=0;
	  uk=&(penal[ipen0+3]); u=(DWORDLONG *)uk;
	  u[0]=(DWORDLONG)ttek4;  //Вписали время.
      penal[ipen0+5]=bit;   //
      penal[ipen0+6]=i;   //индекс модели в Table.
	  p=(int)pModTable[i].Prioritet;
      penal[ipen0+7]=p; 
          //--- приказ вписан. ------
      ipen0=(ipen0+8);   //Сдвинули на следующую запись.
       if(p>maxpr)    maxpr=p;  //Коррекция мах.приор.

	}//--------- конец          цикла --------

 }//================ КОНЕЦ telemetr ==================== 


    void widrel(UINT sisl,ntip *k)   //Выдача релейного сигнала
		   //Запускается в  pwr().
 {int i,p,*uk,bit,bitt; DWORDLONG *u;
	 //-----------------------------------

      return;   //***********  В ЦБК нет.

	            if(dlln==0) return;
       bitt=(int)sisl; bitt=(7&bitt);  //Номер релейного сигнала.
	        if(bitt==0) bit=1;
       else if(bitt==1) bit=2;
       else if(bitt==2) bit=4;
       else if(bitt==3) bit=8;
       else if(bitt==4) bit=16;
       else if(bitt==5) bit=32;
       else if(bitt==6) bit=64;
              else      bit=128;
        //---------------------------------
          if(pesrel==1)
{soob.sprintf("#I Выдача релейного сигнала номер %d",bitt);
		  printkl(soob);
}//----------------------------------------------


	       for(i=0;pModTable[i].ID!=0;i++)
	{if(pModTable[i].Reason!=REASON_RELAY_P_CVM)
	          continue;  //Не откликается.
     if((ipen0+8)>(rpenal-1))
{printkl("#F - Буфер для очереди заданий мал - widrel");
              ExitThread(TRUE);}
      //---------------------------------- 
      //----- Теперь вписываем приказ в пенал:
      penal[ipen0]=6; 
      penal[ipen0+1]=(int)REASON_RELAY_P_CVM;
	  penal[ipen0+2]=0;
	  uk=&(penal[ipen0+3]); u=(DWORDLONG *)uk;
	  u[0]=(DWORDLONG)ttek4;  //Вписали время.
      penal[ipen0+5]=bit;   //
      penal[ipen0+6]=i;   //индекс модели в Table.



	  p=(int)pModTable[i].Prioritet;
      penal[ipen0+7]=p; 
          //--- приказ вписан. ------
      ipen0=(ipen0+8);   //Сдвинули на следующую запись.
       if(p>maxpr)    maxpr=p;  //Коррекция мах.приор.

	}//--------- конец          цикла --------

 }//================ КОНЕЦ widrel ==================== 

  void snatrel(UINT sisl,ntip *k)    //Снятие релейного сигнала.
	      //Запускается в  pwr().
 {int i,p,*uk,bit,bitt; DWORDLONG *u;
	 //-----------------------------------

     return;   //***********  В ЦБК нет.

                if(dlln==0) return;
       bitt=(int)sisl; bitt=(7&bitt);  //Номер релейного сигнала.
	        if(bitt==0) bit=1;
       else if(bitt==1) bit=2;
       else if(bitt==2) bit=4;
       else if(bitt==3) bit=8;
       else if(bitt==4) bit=16;
       else if(bitt==5) bit=32;
       else if(bitt==6) bit=64;
              else      bit=128;
        //---------------------------------
          if(pesrel==1)
{soob.sprintf("#I Снятие релейного сигнала номер %d",bitt);
		  printkl(soob);
}//----------------------------------------------


	       for(i=0;pModTable[i].ID!=0;i++)
	{if(pModTable[i].Reason!=REASON_RELAY_UP_CVM)
	          continue;  //Не откликается.
     if((ipen0+8)>(rpenal-1))
{printkl("#F - Буфер для очереди заданий мал - snatrel");
              ExitThread(TRUE);}
      //---------------------------------- 
      //----- Теперь вписываем приказ в пенал:
      penal[ipen0]=6; 
      penal[ipen0+1]=(int)REASON_RELAY_UP_CVM;
	  penal[ipen0+2]=0;
	  uk=&(penal[ipen0+3]); u=(DWORDLONG *)uk;
	  u[0]=(DWORDLONG)ttek4;  //Вписали время.
      penal[ipen0+5]=bit;   //
      penal[ipen0+6]=i;   //индекс модели в Table.
	  p=(int)pModTable[i].Prioritet;
      penal[ipen0+7]=p; 
          //--- приказ вписан. ------
      ipen0=(ipen0+8);   //Сдвинули на следующую запись.
       if(p>maxpr)    maxpr=p;  //Коррекция мах.приор.

	}//--------- конец          цикла --------

 }//================ КОНЕЦ snatrel ==================== 


  void raskrutka(ntip *k)
//Прочесывает пенал, исполняет приказы по приоритетности,
// пополняет - в хвост пенала - сам пенал новыми приказами
// из заказов моделей после исполнения ими приказа.
      //Запускается в в zakr.cpp.

{int i,j,jj,z,r;  UINT tt; DWORDLONG t;     
         if(ipen0==0) return;
       //Начинаем многократное прочесывание пенала:
 nasalo:
       //Определение начального maxpr в пенале:
  maxpr=(-1);  for(i=0;i<ipen0;i=(i+penal[i]+2))
  {j=i+penal[i]+1;    //Индекс приоритета приказа.
   if(penal[j]>maxpr)  maxpr=penal[j];
  }//--------  определили.
       if(maxpr==(-1))   
	   { ipen0=0; return;}  //Все приказы выполнены.    //********

      for(i=0;i<ipen0;i=(i+penal[i]+2))        //+2 - сдвиг индекса, учитывающий
                                               //две последние ячейки заказа-приказа: 
                                               //индекс модели и ее приоритет в Table.
	{j=i+penal[i];   //индекс в пенале индекса
		             //модели в их таблице.
      if(penal[j+1]<maxpr) continue; //меньший приоритет.
	  //--- Теперь работа с отобранным приказом:
      jj=penal[j];   //индекс модели в Table.
	    indmod=jj;  //Зафиксировали его глобально        Но может это не нужно ?
	  //---- Определим по jj  vxod,vixod,ukf,ident:
	   ident=(UINT)pModTable[jj].ID;
      vxod=(UINT *)pModTable[jj].pInputBuffer;
     vixod=(UINT *)pModTable[jj].pOutputBuffer;
          ukf=pModTable[jj].Subrut;
      //--------- Определили.    jj освободился.
             r=penal[i];   //размер приказа.
	 //-- Перепишем приказ из пенала в входной буфер модели:		 
      for(jj=0;jj<r;jj++)  vxod[jj]=penal[i+jj];//сделали.
	        vxod[r]=0;      //обозначение конца приказа.
		//--------------------------------------	

               (*ukf)( );    //обращение к модели.

       penal[j+1]=(-1);  //Приказ выполнен, приоритет =-1.

        //---- Было ли это разбуживание ? -----
        //Тогда надо учесть сброс разового и повтор циклич.заказа:
       //--------------------------------------------------------------
         if(penal[i+1]==REASON_TIMER)        //РАЗОВЫЙ
	{
			 if((pesrazb==1)&&(ttek4>0))
		{soob.sprintf("#I Разовое разбуживание модели %d при Т=%lu",ident,(4*ttek4));
			         printkl(soob);
		}//---------------------
              for(z=288;wus[z+2]!=3;z=z+8)
		{if(z>=1600)    {printkl("#F - MWU - Ошибка 1 в raskrutka. Отказ"); 
		                               return;;
						}
		  if(wus[z+1]!=ident) continue; //Не тот номер	
				wus[z+2]=0; break;  //Сделали пассивным
		}	
	 }//-------------- конец разового ----------------------------

         if(penal[i+1]==REASON_TIMER_CYCLE)  //ЦИКЛИЧЕСКИЙ
	{ 
			 if((pesrazb==1)&&(ttek4>0))
		{soob.sprintf("#I Циклическое разбуживание модели %d при Т=%lu",ident,(4*ttek4));
			         printkl(soob);
		}//---------------------

             for(z=288;wus[z+2]!=3;z=z+8)
		{if(z>=1600) 
			 {printkl("#F - MWU - Ошибка 2 в raskrutka. Отказ"); 
		                               return;
			 }
 		   if(wus[z+1]!=ident) continue; //Не тот номер	

           tt=zraz[ident]; //Дельта циклич.разбуживания этой модели.
           t=(DWORDLONG)tt; t=(t+ttek4); bud[z/8]=t; //В будильник!
      	               if(t<Tmin)  Tmin=t; break;
        }
	}//-------- конец циклического -------------------------------


          if(penal[i+1]==REASON_TACT)       //ТАКТОВЫЙ
		{
			 if((pesrazb==1)&&(ttek4>0))
			 {soob.sprintf("#I Тактовое разбуживание модели %d при Т=%lu",ident,(4*ttek4));
			         printkl(soob);
			 }//---------------------

		}
          //--- Осталось посмотреть заказы этой модели:

                     zakaz(0);
        //Обработка заказов этой модели ident в vixod с начальной
        //ячейки - vixod[0].
        //  ident, vixod - уже ОПРЕДЕЛЕНЫ.
					 
	}//---- конец однократного прочесывания пенала.
    
       goto nasalo;  
	        
}//============== КОНЕЦ raskrutka ===================



//ФУНКЦИИ, ОБЕСПЕЧИВАЮЩИЕ  ОБМЕНЫ   КМК - КОНТРОЛЛЕРА


   void kmka(short pornom,short pr,ntip *k)
   //Обрабатывает заявку на КМК контроллера с номером pornom,
   //pr - признак начала /0/ или конца /1/ обмена.
         //Запускается dispet().
{UINT *mm,adr,adr1,r,r0,r1,r3,ind,ind1,kol,nou,i,j,otw,*uk,kkom,rr;
 UINT r2,otw1,otw2,nou2,kol2; //Для ОУ2.
 UINT num_mod,Reason;
 char string[72];   UINT nom; int ii;
     DWORDLONG *u,t; short pp; 
         nom=0;
		 for(ii=1;ii<=kolwm;ii++) { if(k==bazuk[ii]) {nom=(UINT)ii; break; }  } //nom - мой номер 
   if((nom>(UINT)kolwm)||(nom<1))
   {printkl("# Информируйте ЭИК об ошибке 1 в kmka"); return;}
   if(Komplex[nom]==0) 
   {soob.sprintf("#  ВМ%d выключена, а она изволит быть активным контроллером!",nom-1);printkl(soob);}
   //-------------------------------------------------------



	 if(k==0) {printkl("#  Нет ведущей ВМ на МКО. В kmka(). Ошибка!"); return;}
	        //----------------------
	            grup=0;        //Пока неизвестно - групповой ли обмен без ОС ?
	           mm=k->mp;
	 if(pr==0)  goto nasalo;
 if(pr!=1) {printkl("#F - MWU - Ошибка 2 в kmka");
                                           return;}
         // Реализация окончания обмена:
        if(pesmko!=0)   
	{soob.sprintf("#I Окончился обмен ВМ-%d на КМК - %o ",(nom-1),pornom);
             printkl(soob);
       soob.sprintf("#I Время  окончания обмена /дес/ - t= %lu ",4*ttek4); 
                 printkl(soob);
	}
       adr=adroo(pornom,k);  //адрес описателя обмена
	   adr1=adrbs(adr,k);    //адрес буфера сообщений
	 ind=indpam(adr,k); r0=mm[ind]; //Управляющее слово
      ind1=indpam(adr1,k); r1=mm[ind1]; 
	   r1=(0177777&r1);  //Это командное слово
                if(pesmko!=0)
{soob.sprintf("#I Командное слово /16-тиричн/ =%x",r1); 
                 printkl(soob);} 

  nou=r1>>11; nou=(037&nou);  //Адрес ОУ в командном слове
          //----------------------------------------
    //-------  ТЕПЕРЬ - РЕАЛЬНАЯ РАБОТА : ------------------

if(((01000&r0)!=0)&&(nou!=037))
           //Формат 3. ОУ - ОУ негрупповой.                       ФОРМАТ 3
{
		            if(dlln==0)   //Нет DLL:
	{              otw=0200000;
		if(pesdll==1)	printkl("# Нет DLL  !!!");                    
           pp=konper(mm,ind,otw,nou,r0,pornom,k);
		   if(pp==1) return;  //Прекращение пакета                  
           else goto bbb;     //К подготовке следующего обмена.
	}//--------------------------------------------------------
		            if((k->wds[pornom]==0)||(k->wklblk==1))      // для пассивной ВМ                
	{              otw=0200000;
    soob.sprintf("# КУК отключил передатчики ВМ %d на МКО-%d",(nom-1),pornom);                                 
      printkl(soob);
		if(pesmko!=0)	printkl("# Не будет ответного слова  !!!");                    
           pp=konper(mm,ind,otw,nou,r0,pornom,k);
		   if(pp==1) return;  //Прекращение пакета                  
           else goto bbb;     //К подготовке следующего обмена.
	}//--------------------------------------------------------
					
	
	if(pesouou!=0)  printkl("#I Выполнялся обмен ОУ - ОУ негрупповой"); 
	//Берем командное слово r2 и nou2 для ОУ2
  r2=mm[ind1+1]; r2=(0177777&r2); //Второе командное слово.
                if(pesmko!=0)
{soob.sprintf("#I Второе командное слово /16-тиричн/ =%x",r2); 
                 printkl(soob);} 
  nou2=r2>>11; nou2=(037&nou2);   //Номер ОУ из второго ком.слова.
            if(((nou2==037)||(nou2==nou))&&(pesosib!=0))
  printkl("#E  В формате 3 ошибка адреса ОУ во 2-м ком.слове"); 
            if(((02000&r1)!=0)&&(pesosib!=0))
  printkl("#E  В формате 3 ошибка направления передачи в 1-м ком.слове");
            if(((02000&r2)==0)&&(pesosib!=0))
  printkl("#E  В формате 3 ошибка направления передачи в 2-м ком.слове");
			//---------------------------------
  kol=037&r1; if(kol==0) kol=32; kol2=037&r2; if(kol2==0) kol2=32;
                if((kol!=kol2)&&(pesosib!=0))
  printkl("#E  В формате 3 в командных словах несовпадение количеств слов");

      r=01740&r1;  if(((r==0)||(r==01740))&&(pesosib!=0))
  printkl("#E  В формате 3 ошибка подадреса в 1-м ком.слове");		  
      r=01740&r2;  if(((r==0)||(r==01740))&&(pesosib!=0))
  printkl("#E  В формате 3 ошибка подадреса в 2-м ком.слове");		  
          //---------------------------------------------

//---------- Определение  vxod,vixod,ident, ukf - Олег !
// по  pornom  и  nou2.
//----------------------- find model in ModTable -------
   num_mod = 0177777;
   Reason = pornom+REASON_MKO0;
  
  for ( i = 0; pModTable[i].ID != 0; i++)
	{
if ((pModTable[i].Reason == Reason) && (pModTable[i].Address == nou2)) 
		{num_mod = i; break;}
	}
  if (num_mod == 0177777)
    {
soob.sprintf("#E Формат 3:в РКМ нет модели Reason=%d-дес, номер ОУ=%o-восм \n",Reason,nou2);
 if(pesosib!=0) printkl(soob);
	              otw=0200000; //Нет модели !
      pp=konper(mm,ind,otw,nou2,r0,pornom,k);
      if(pp==1) return; else goto bbb;
    }

	       if(pesmko!=0)
{sprintf(string,"#I - Вызвана модель - ID = %d-дес Reason = %o , номер ОУ = %o-восм \n",
pModTable[num_mod].ID,pModTable[num_mod].Reason, pModTable[num_mod].Address);                                               
                 printkl(string);}
    
//------------------------- extract model data ------

 vxod   = (unsigned int *)pModTable[num_mod].pInputBuffer;
 vixod  = (unsigned int *)pModTable[num_mod].pOutputBuffer;
 ident  = (unsigned int )pModTable[num_mod].ID;
 ukf    =  pModTable[num_mod].Subrut;
 indmod=num_mod;
//---------------- end model finding ------
    
//------ Определили. Теперь выдача из ОУ2 в БОЗУ ----------
     if(pesmko!=0) printkl("#I  Выдача из ОУ2, формат 3");
  vxod[0]=6; vxod[1]=pornom+REASON_MKO0;	 
  vxod[2]=0; vxod[5]= r2; //Это ком.слово для ОУ2
  uk=vxod+3; u=(DWORDLONG *)uk; u[0]=4*ttek4; vxod[6]=0;
    //---------- Вписали во входной массив.-------
 if(pesmko!=0) printkl("#I Обращаемся к функции модели ОУ2.");
 (*ukf)( ); if(pesmko!=0) printkl("#I  Вышли из модели.");

		otw=vixod[5]; otw=(0177777&otw); 
		     if(vixod[0]!=(kol2+6))
{if(pesosib!=0)
         printkl("#E - Ошибка ОУ2 в формате 3: vixod[0]!=kol2+6.");
					     otw=(0400000|otw);}
         //--------------------
  rr=otw; rr=(rr>>11); rr=(037&rr); //Адрес ОУ2 в ответном слове
      if(rr!=nou2)  otw=(01000000|otw);  //Несовпадение адресов
	  //-------------------------------------------
 if(pesmko!=0) 
{soob.sprintf("#I Ответн.слово ОУ2 в 3-м формате 16-тиричн.=%x",vixod[5]);
 printkl(soob); } //---------------------------
   otw2=otw; //Зафиксировали ответное слово ОУ2 с доп.признаками.

     mm[ind1+2]=vixod[5]; //Вписали отв.слово ОУ2 в БОЗУ	 
	         for(i=1;i<=kol2;i++)   //Перепись данных в БОЗУ:
         mm[ind1+2+i]=0177777&vixod[i+5];      //Переписали.
			 //--------------------------------
               if((0400000&otw)==0)  
	 zakaz(kol2+6); //Только, если нет ошибки в вых.буфере ОУ2
 //-------- ОБРАБОТАЛИ ДАННЫЕ ОТ  ОУ2.-----------------------

     //----- Теперь работа с ОУ1:
//---------- Определение  vxod,vixod,ident, ukf - Олег !
// по  pornom  и  nou.
//----------------------- find model in ModTable -------

   num_mod = 0177777;
   Reason = pornom+REASON_MKO0;
  
  for ( i = 0; pModTable[i].ID != 0; i++)
	{
if ((pModTable[i].Reason == Reason) && (pModTable[i].Address == nou)) 
		{num_mod = i; break;}
	}
  if (num_mod == 0177777)
    {
sprintf(string,"#E - В РКМ нет модели с Reason = %d-дес, номер ОУ = %o-восм \n",Reason, nou);                                               
    if(pesosib!=0) printkl(string);
	              otw=0200000; //Нет модели !
      pp=konper(mm,ind,otw,nou,r0,pornom,k);
      if(pp==1) return; else goto bbb;
    }

	       if(pesmko!=0)
{sprintf(string,"#I - Вызвана модель - ID = %d-дес Reason = %o , номер ОУ = %o-восм \n",
pModTable[num_mod].ID,pModTable[num_mod].Reason, pModTable[num_mod].Address);                                               
                 printkl(string);}
    
//------------------------- extract model data ------

 vxod   = (unsigned int *)pModTable[num_mod].pInputBuffer;
 vixod  = (unsigned int *)pModTable[num_mod].pOutputBuffer;
 ident  = (unsigned int )pModTable[num_mod].ID;
 ukf    =  pModTable[num_mod].Subrut;
 indmod=num_mod;
//---------------- end model finding ------
       
//------ Определили. Теперь выдача из БОЗУ в ОУ1:   ------

	 if(pesmko!=0) printkl("#I  Передача данных в ОУ1, формат 3 ");
		vxod[0]=kol+6; vxod[1]=pornom+REASON_MKO0;
		           vxod[2]=0; vxod[5]=r1; //Это ком.слово
         uk=vxod+3;  u=(DWORDLONG *)uk; u[0]=4*ttek4;
	  //--------- Вписали заголовок --------
	         for(i=1;i<=kol;i++)
			 {vxod[i+5]=(0177777&mm[ind1+i+2]);
//soob.sprintf("#I ПЕРЕДАЕТСЯ  В ОУ %x",vxod[i+5]); 
//                            printkl(soob);  
			 }
			  vxod[kol+6]=0;
			 //----- Переписали данные в ОУ -------
  if(pesmko!=0) printkl("#I Обращаемся к функции модели ОУ1.");
   (*ukf)( ); //Обратились к модели
	if(pesmko!=0) printkl("#I  Вышли из модели.");
    otw=vixod[5]; otw=(0177777&otw);  
if(vixod[0]!=6) 
{if(pesosib!=0)
      printkl("#E - Ошибка ОУ1 в формате 3: vixod[0]!=6.");
				                      otw=(0400000|otw);}
				                   
             //------------------------------------
                        mm[ind1+kol+3]=vixod[5];
		                   //Вписыв ответного слова в БОЗУ
         if(pesmko!=0)
 {soob.sprintf("#I Ответное слово ОУ1 16-тирич. =%x",vixod[5]);
                             printkl(soob);}
             //------------------------------------
    if((0400000&otw)==0)  zakaz(6);      otw1=otw;

	//------Закончено все по передаче из ОУ2 в ОУ1 --------

 if((01000000&otw2)!=0) otw1=(01000000|otw1); //Учли несовп.адресов в ОУ2
 if((010&otw2)!=0)  otw1=(010|otw1);  //Перенесли занятость абонента в ОУ2
                   //-----------------------
if(pesouou!=0)  printkl("#I Завершение обмена ОУ - ОУ негруппового");
           pp=konper(mm,ind,otw1,nou,r0,pornom,k);
		   if(pp==1) return;  //Прекращение пакета
		   else goto bbb;

}//************* Конец ФОРМАТА 3, обмена ОУ - ОУ негруппового. *********


 else if(((01000&r0)!=0)&&(nou==037))
	       //Формат 8.  ОУ - ОУ групповой.                      ФОРМАТ 8
{
		            if(dlln==0)   //Нет DLL:
	{              otw=0200000;
		if(pesdll==1)	printkl("# Нет DLL  !!!");                    
           pp=konper(mm,ind,otw,nou,r0,pornom,k);
		   if(pp==1) return;  //Прекращение пакета                  
           else goto bbb;     //К подготовке следующего обмена.
	}//--------------------------------------------------------
		     if((k->wds[pornom]==0)||(k->wklblk==1))      // для пассивной ВМ   
	{              otw=0200000;
    soob.sprintf("# КУК отключил передатчики ВМ %d на МКО-%d",(nom-1),pornom);
      printkl(soob);
		if(pesmko!=0)	printkl("# Не будет ответного слова  !!!");                    
           pp=konper(mm,ind,otw,nou,r0,pornom,k);
		   if(pp==1) return;  //Прекращение пакета                  
           else goto bbb;     //К подготовке следующего обмена.
	}//--------------------------------------------------------
					
	 
	 if(pesouou!=0)  printkl("#I Выполнялся обмен ОУ - ОУ групповой"); 
	//Берем командное слово r2 и nou2 для ОУ2
  r2=mm[ind1+1]; r2=(0177777&r2); //Второе командное слово.
                if(pesmko!=0)
{soob.sprintf("#I Второе командное слово /16-тиричн/ =%x",r2); 
                 printkl(soob);} 

  nou2=r2>>11; nou2=(037&nou2);   //Номер ОУ из второго ком.слова.
            if(((nou2==037)||(nou2==nou))&&(pesosib!=0))
  printkl("#E  В формате 8 ошибка адреса ОУ во 2-м ком.слове"); 
            if(((02000&r1)!=0)&&(pesosib!=0))
  printkl("#E  В формате 8 ошибка направления передачи в 1-м ком.слове");
            if(((02000&r2)==0)&&(pesosib!=0))
  printkl("#E  В формате 8 ошибка направления передачи в 2-м ком.слове");
			//---------------------------------
  kol=037&r1; if(kol==0) kol=32; kol2=037&r2; if(kol2==0) kol2=32;
                if((kol!=kol2)&&(pesosib!=0))
  printkl("#E  В формате 8 в командных словах несовпадение количеств слов");

      r=01740&r1;  if(((r==0)||(r==01740))&&(pesosib!=0))
  printkl("#E  В формате 8 ошибка подадреса в 1-м ком.слове");		  
      r=01740&r2;  if(((r==0)||(r==01740))&&(pesosib!=0))
  printkl("#E  В формате 8 ошибка подадреса в 2-м ком.слове");		  
          //---------------------------------------------
//---------- Определение  vxod,vixod,ident, ukf - Олег !
// по  pornom  и  nou2.
//----------------------- find model in ModTable -------

   num_mod = 0177777;
   Reason = pornom+REASON_MKO0;
  
  for ( i = 0; pModTable[i].ID != 0; i++)
	{
if ((pModTable[i].Reason==Reason)&&(pModTable[i].Address==nou2)) 
		{num_mod = i; break;}
	}
  if (num_mod == 0177777)
    {
sprintf(string,"#E Формат 8:в РКМ нет модели Reason=%d-дес, номер ОУ=%o-восм \n",Reason,nou2);                                               
    if(pesosib!=0) printkl(string);
	              otw=0200000; //Нет модели !
      pp=konper(mm,ind,otw,nou2,r0,pornom,k);
      if(pp==1) return; else goto bbb;
    }
    
	       if(pesmko!=0)
{sprintf(string,"#I - Вызвана модель - ID = %d-дес Reason = %o , номер ОУ = %o-восм \n",
pModTable[num_mod].ID,pModTable[num_mod].Reason, pModTable[num_mod].Address);                                               
                 printkl(string);}
    
//------------------------- extract model data ------

 vxod   = (unsigned int *)pModTable[num_mod].pInputBuffer;
 vixod  = (unsigned int *)pModTable[num_mod].pOutputBuffer;
 ident  = (unsigned int )pModTable[num_mod].ID;
 ukf    =  pModTable[num_mod].Subrut;
 indmod=num_mod;
//---------------- end model finding ------
                                                                        
//------ Определили. Теперь выдача из ОУ2 в БОЗУ ----------
     if(pesmko!=0) printkl("#I  Выдача из ОУ2, формат 8");
  vxod[0]=6; vxod[1]=pornom+REASON_MKO0;	 
  vxod[2]=0; vxod[5]= r2; //Это ком.слово для ОУ2
  uk=vxod+3; u=(DWORDLONG *)uk; u[0]=4*ttek4; vxod[6]=0;
    //---------- Вписали во входной массив.-------
 if(pesmko!=0) printkl("#I Обращаемся к функции модели ОУ2.");
 (*ukf)( ); if(pesmko!=0) printkl("#I  Вышли из модели.");

		otw=vixod[5]; otw=(0177777&otw); 
		             if(vixod[0]!=(kol2+6))
{if(pesosib!=0) printkl("#E - Ошибка ОУ2 в формате 8: vixod[0]!=kol2+6.");
					     otw=(0400000|otw);}
         //--------------------
  rr=otw; rr=(rr>>11); rr=(037&rr); //Адрес ОУ2 в ответном слове
      if(rr!=nou2)  otw=(01000000|otw);  //Несовпадение адресов
	  //-------------------------------------------
 if(pesmko!=0) 
{soob.sprintf("#I Ответн.слово ОУ2 в 8-м формате 16-тиричн.=%x",vixod[5]);
 printkl(soob); } //---------------------------
      otw2=otw; //Зафиксировали ответное слово ОУ2 с доп.признаками.

     mm[ind1+2]=vixod[5]; //Вписали отв.слово ОУ2 в БОЗУ	 
	         for(i=1;i<=kol2;i++)   //Перепись данных в БОЗУ:
         mm[ind1+2+i]=0177777&vixod[i+5];      //Переписали.
			 //--------------------------------
               if((0400000&otw)==0)  
	 zakaz(kol2+6); //Только, если нет ошибки в вых.буфере ОУ2
 //-------- ОБРАБОТАЛИ ДАННЫЕ ОТ  ОУ2.-----------------------

     //----- Теперь работа с ОУ1:
//---------- Определение  vxod,vixod,ident, ukf - Олег !
// по  pornom  и  nou. (nou=037)
//----------------------- find model in ModTable -------

   num_mod = 0177777;
   Reason = pornom+REASON_MKO0;
  
  for(j=0;pModTable[j].ID != 0;j++)  //Просмотр моделей с nou=037
	{
if ((pModTable[j].Reason == Reason) && (pModTable[j].Address == nou)) 
		{//Работа с найденной моделью	
	     num_mod = j;
    
	       if(pesmko!=0)
{sprintf(string,"#I - Вызвана модель - ID = %d-дес Reason = %o , номер ОУ = %o-восм \n",
pModTable[num_mod].ID,pModTable[num_mod].Reason, pModTable[num_mod].Address);                                               
                 printkl(string);}

//------------------------- extract model data ------

 vxod   = (unsigned int *)pModTable[num_mod].pInputBuffer;
 vixod  = (unsigned int *)pModTable[num_mod].pOutputBuffer;
 ident  = (unsigned int )pModTable[num_mod].ID;
 ukf    =  pModTable[num_mod].Subrut;
 indmod=num_mod;
//---------------- end model finding ------
       
//------ Определили. Теперь выдача из БОЗУ в ОУ:   ------

	 if(pesmko!=0) printkl("#I  Передача данных в ОУ, формат 8");
		vxod[0]=kol+6; vxod[1]=pornom+REASON_MKO0;
		           vxod[2]=0; vxod[5]=r1; //Это ком.слово
         uk=vxod+3;  u=(DWORDLONG *)uk; u[0]=4*ttek4;
	  //--------- Вписали заголовок --------
	         for(i=1;i<=kol;i++)
			 {vxod[i+5]=(0177777&mm[ind1+i+2]);
//soob.sprintf("#I ПЕРЕДАЕТСЯ  В ОУ %x",vxod[i+5]); 
//                            printkl(soob);  
			 }
			  vxod[kol+6]=0;

			 //----- Переписали данные в ОУ -------
  if(pesmko!=0) printkl("#I Обращаемся к функции модели ОУ.");
   (*ukf)( ); //Обратились к модели
	if(pesmko!=0) printkl("#I  Вышли из модели.");
       //Ответное слово не берется, его нет.
	
    if(vixod[0]==6)  zakaz(6);     

		}//Конец работы с найденной моделью                               

  } //----------------------------- Конец просмотра всех моделей

if(pesouou!=0)  printkl("#I Завершение обмена ОУ - ОУ группового");
      pp=konper2(mm,ind,nou,r0,pornom,k);
      if(pp==1) return; else goto bbb;
    
}//************ Конец ФОРМАТА 8, обмена ОУ - ОУ группового. *********
 			   
else {  }        //Для транслятора и меня - конец ОУ - ОУ общий


      //--- Теперь - форматы 9 и 10:

  r=01740&r1; if(((r==0)||(r==01740))&&(nou==037))
	                             //Команда управления
	{kkom=037&r1;  //Код команды управления
     if(kkom<=017)  //Команда без слова данных,               9 ФОРМАТ.            
	 {       grup=1;   //Без ОС

	//-------- Анализ - отключен ли контроллер:  
		rr=streg(100,pornom,0,0,6,k);    //RR
	if(((0160000&rr)==060000)||((02000&r0)==0))  //Отключен.
	//Проверили это по Mod в rr и по 10-му разряду управл.слова - r0:
	                                          //- тестовый режим.
		{
		 otw=0200000; pp=konper(mm,ind,otw,nou,r0,pornom,k);			   
		   if(pp==1) return;  //Прекращение пакета                  
           else goto bbb;     //К подготовке следующего обмена.
		}//----------------------------------------			

		            if(dlln==0)   //Нет DLL:
	{              otw=0200000;
		if(pesdll==1)	printkl("# Нет DLL  !!!");                    
           pp=konper(mm,ind,otw,nou,r0,pornom,k);
		   if(pp==1) return;  //Прекращение пакета                  
           else goto bbb;     //К подготовке следующего обмена.
	}//--------------------------------------------------------			
		    if((k->wds[pornom]==0)||(k->wklblk==1))      // для пассивной ВМ   
	{              otw=0200000;
    soob.sprintf("# КУК отключил передатчики ВМ %d на МКО-%d",(nom-1),pornom);
      printkl(soob);

		if(pesmko!=0)	printkl("# Не будет ответного слова  !!!");                    
           pp=konper(mm,ind,otw,nou,r0,pornom,k);
		   if(pp==1) return;  //Прекращение пакета                  
           else goto bbb;     //К подготовке следующего обмена.
	}//--------------------------------------------------------
		 
		 if(pesmko!=0)
	 printkl("#I Групповая команда без слова данных 9 ФОРМАТ.");
		  if(((02000&r1)==0)&&(pesosib!=0))
{printkl("#W - MWU - Ошибка в разряде направления передачи в 9 формате.");}
 
//------------------------------------------------ 
                        
   num_mod = 0177777;
   Reason = pornom+REASON_MKO0;
  
  for(i=0;pModTable[i].ID != 0;i++)  //-----Просмотр моделей с nou=037
	{
if ((pModTable[i].Reason == Reason) && (pModTable[i].Address == nou)) 
		{//Работа с найденной моделью	
	     num_mod = i;
    

	       if(pesmko!=0)
{sprintf(string,"#I - Вызвана модель - ID = %d-дес Reason = %o , номер ОУ = %o-восм \n",
pModTable[num_mod].ID,pModTable[num_mod].Reason, pModTable[num_mod].Address);                                               
                 printkl(string);}

//------------------------- extract model data ------

 vxod   = (unsigned int *)pModTable[num_mod].pInputBuffer;
 vixod  = (unsigned int *)pModTable[num_mod].pOutputBuffer;
 ident  = (unsigned int )pModTable[num_mod].ID;
 ukf    =  pModTable[num_mod].Subrut;
 indmod=num_mod;
//---------------- end model finding ------

 vxod[0]=6; vxod[1]=pornom+REASON_MKO0;vxod[2]=0; vxod[5]=r1;
		uk=vxod+3; u=(DWORDLONG *)uk; u[0]=4*ttek4; 
		vxod[6]=0;
		//---------------------
 if(pesmko!=0) printkl("#I Обращаемся к функции модели.");
 (*ukf)( ); if(pesmko!=0) printkl("#I  Вышли из модели.");

		 if((vixod[0]!=6)&&(pesosib!=0))
{printkl("#E - MWU - Ошибка в kmka в формате 9:  vixod[0]!=6."); }
         //--------------------
        
	if(vixod[0]==6)  zakaz(6);
  
		}//Конец работы с найденной моделью                               

  } //----------------------------- Конец просмотра всех моделей
  if(pesmko!=0)
  printkl("#I Завершение обмена по формату 9");
   pp=konper2(mm,ind,nou,r0,pornom,k);
    if(pp==1) return; else goto bbb;

  }//--- Конец  ФОРМАТА 9.***********


	  else  //Команда управления со словом данных.        ФОРМАТ 10
      {     grup=1;      //Без ОС

	//-------- Анализ - отключен ли контроллер:  
		rr=streg(100,pornom,0,0,6,k);    //RR
	if(((0160000&rr)==060000)||((02000&r0)==0))  //Отключен.
	//Проверили это по Mod в rr и по 10-му разряду управл.слова - r0:
	                                          //- тестовый режим.
		{
		 otw=0200000; pp=konper(mm,ind,otw,nou,r0,pornom,k);			   
		   if(pp==1) return;  //Прекращение пакета                  
           else goto bbb;     //К подготовке следующего обмена.
		}//----------------------------------------			

		            if(dlln==0)   //Нет DLL:
	{              otw=0200000;
		if(pesdll==1)	printkl("# Нет DLL  !!!");                    
           pp=konper(mm,ind,otw,nou,r0,pornom,k);
		   if(pp==1) return;  //Прекращение пакета                  
           else goto bbb;     //К подготовке следующего обмена.
	}//--------------------------------------------------------
		    if((k->wds[pornom]==0)||(k->wklblk==1))      // для пассивной ВМ   
	{              otw=0200000;
    soob.sprintf("# КУК отключил передатчики ВМ %d на МКО-%d",(nom-1),pornom);
      printkl(soob);

		if(pesmko!=0)	printkl("# Не будет ответного слова  !!!");                    
           pp=konper(mm,ind,otw,nou,r0,pornom,k);
		   if(pp==1) return;  //Прекращение пакета                  
           else goto bbb;     //К подготовке следующего обмена.
	}//--------------------------------------------------------
					
		  
		  if((02000&r1)!=0)   
 printkl("#E Ошибка направления передачи в формате 10");

 if(pesmko!=0)
printkl("#I Групповая команда управления со словом данных, формат 10");
                      
   num_mod = 0177777;
   Reason = pornom+REASON_MKO0;
  
  for(i=0;pModTable[i].ID != 0;i++)  //------Просмотр моделей с nou=037
	{
if ((pModTable[i].Reason == Reason) && (pModTable[i].Address == nou)) 
		{//Работа с найденной моделью	
	     num_mod = i;
    
	       if(pesmko!=0)
{sprintf(string,"#I - Вызвана модель - ID = %d-дес Reason = %o , номер ОУ = %o-восм \n",
pModTable[num_mod].ID,pModTable[num_mod].Reason, pModTable[num_mod].Address);                                               
                 printkl(string);}

//------------------------- extract model data ------

 vxod   = (unsigned int *)pModTable[num_mod].pInputBuffer;
 vixod  = (unsigned int *)pModTable[num_mod].pOutputBuffer;
 ident  = (unsigned int )pModTable[num_mod].ID;
 ukf    =  pModTable[num_mod].Subrut;
 indmod=num_mod;
//---------------- end model finding ------

	vxod[0]=7; vxod[1]=pornom+REASON_MKO0;vxod[2]=0; vxod[5]=r1;
	    	uk=vxod+3; u=(DWORDLONG *)uk; u[0]=4*ttek4; 
		 vxod[6]=mm[ind1+1]; //Запись слова.
		 vxod[7]=0;
		//---------------
if(pesmko!=0) printkl("#I Обращаемся к функции модели.");
    (*ukf)( );if(pesmko!=0)  printkl("#I  Вышли из модели."); 
//	 if((vixod[0]!=6)&&(pesosib!=0))
//{printkl("#E - MWU - Ошибка  в kmka в формате 10: vixod[0]!=6.")}
         //--------------------
	 
	
               	zakaz(0);	//if(vixod[0]==6)	 zakaz(6);
		}//Конец работы с найденной моделью                               

  } //----------------------------- Конец просмотра всех моделей

  if(pesmko!=0)
  printkl("#I Завершение обмена по формату 10");
   pp=konper2(mm,ind,nou,r0,pornom,k);
    if(pp==1) return; else goto bbb;

		}//******** Конец ФОРМАТА 10. **********

}//************ Конец команды управления общий.***********************

          //---- Теперь 7 формат: 

 r=01740&r1; if(((r!=0)&&(r!=01740))&&(nou==037))          //  ФОРМАТ 7
	      // Передача слов из КК в группу ОУ, формат 7
 {             grup=1;     //Без ОС

	//-------- Анализ - отключен ли контроллер:  
		rr=streg(100,pornom,0,0,6,k);    //RR
	if(((0160000&rr)==060000)||((02000&r0)==0))  //Отключен.
	//Проверили это по Mod в rr и по 10-му разряду управл.слова - r0:
	                                          //- тестовый режим.
		{
		 otw=0200000; pp=konper(mm,ind,otw,nou,r0,pornom,k);			   
		   if(pp==1) return;  //Прекращение пакета                  
           else goto bbb;     //К подготовке следующего обмена.
		}//----------------------------------------			

		            if(dlln==0)   //Нет DLL:
	{              otw=0200000;
		if(pesdll==1)	printkl("# Нет DLL  !!!");                    
           pp=konper(mm,ind,otw,nou,r0,pornom,k);
		   if(pp==1) return;  //Прекращение пакета                  
           else goto bbb;     //К подготовке следующего обмена.
	}//--------------------------------------------------------			
		    if((k->wds[pornom]==0)||(k->wklblk==1))      // для пассивной ВМ   
	{              otw=0200000;
    soob.sprintf("# КУК отключил передатчики ВМ %d на МКО-%d",(nom-1),pornom);
      printkl(soob);

		if(pesmko!=0)	printkl("# Не будет ответного слова  !!!");                    
           pp=konper(mm,ind,otw,nou,r0,pornom,k);
		   if(pp==1) return;  //Прекращение пакета                  
           else goto bbb;     //К подготовке следующего обмена.
	}//--------------------------------------------------------
	 
	 if(((02000&r1)!=0)&&(pesosib!=0))
  printkl("#E Ошибка направления передачи в формате 7");
                       
   num_mod = 0177777;
   Reason = pornom+REASON_MKO0;
  
  for(j=0;pModTable[j].ID != 0;j++)  //------Просмотр моделей с nou=037
	{
if ((pModTable[j].Reason == Reason) && (pModTable[j].Address == nou)) 
		{//Работа с найденной моделью	
	     num_mod = j;
    
	       if(pesmko!=0)
{sprintf(string,"#I - Вызвана модель - ID = %d-дес Reason = %o , номер ОУ = %o-восм \n",
pModTable[num_mod].ID,pModTable[num_mod].Reason, pModTable[num_mod].Address);                                               
                 printkl(string);}

//------------------------- extract model data ------

 vxod   = (unsigned int *)pModTable[num_mod].pInputBuffer;
 vixod  = (unsigned int *)pModTable[num_mod].pOutputBuffer;
 ident  = (unsigned int )pModTable[num_mod].ID;
 ukf    =  pModTable[num_mod].Subrut;
 indmod=num_mod;
//---------------- end model finding ------

	 if(pesmko!=0) printkl("#I  Передача данных в ОУ, формат 7 ");
    kol=037&r1; if(kol==0) kol=32;     //Кол-во слов передачи
		vxod[0]=kol+6; vxod[1]=pornom+REASON_MKO0;
		           vxod[2]=0; vxod[5]=r1;
         uk=vxod+3;  u=(DWORDLONG *)uk; u[0]=4*ttek4;
	  //--------- Вписали заголовок --------
	

	         for(i=1;i<=kol;i++)
			 {vxod[i+5]=(0177777&mm[ind1+i]);
//soob.sprintf("#I ПЕРЕДАЕТСЯ  В ОУ %x",vxod[i+5]); 
//                            printkl(soob);  
			 }
			  vxod[kol+6]=0;
			 //----- Переписали данные в ОУ -------
  if(pesmko!=0) printkl("#I Обращаемся к функции модели.");
   (*ukf)( ); //Обратились к модели  
	if(pesmko!=0) printkl("#I  Вышли из модели.");
//if((vixod[0]!=6)&&(pesosib!=0)) 
//{printkl("#E - MWU - Ошибка в kmka в формате 7: vixod[0]!=6.");}
             //------------------------------------
//			if(vixod[0]==6) zakaz(6);
	   zakaz(0);  //Что она могла заказать
	} //--Конец работы с найденной моделью

  }//---- Конец просмотра моделей

  if(pesmko!=0)
  printkl("#I Завершение обмена по формату 7");
   pp=konper2(mm,ind,nou,r0,pornom,k);
    if(pp==1) return; else goto bbb;

}// *************** Конец ФОРМАТА 7 ******************************





    // ------ ОСТАЛИСЬ НЕГРУППОВЫЕ  И НЕ  ОУ - ОУ  ОБМЕНЫ.

//---------- Определение  vxod,vixod,ident, ukf - Олег !
// по  pornom  и  nou.
//----------------------- find model in ModTable -------
                   			

		            if(dlln==0)   //Нет DLL:
	{              otw=0200000;
		if(pesdll==1)	printkl("# Нет DLL  !!!");                    
           pp=konper(mm,ind,otw,nou,r0,pornom,k);
		   if(pp==1) return;  //Прекращение пакета                  
           else goto bbb;     //К подготовке следующего обмена.
	}//--------------------------------------------------------			
		   if((k->wds[pornom]==0)||(k->wklblk==1))      // для пассивной ВМ   
	{              otw=0200000;
    soob.sprintf("# КУК отключил передатчики ВМ %d на МКО-%d",(nom-1),pornom);
      printkl(soob);

		if(pesmko!=0)	printkl("# Не будет ответного слова  !!!");                    
           pp=konper(mm,ind,otw,nou,r0,pornom,k);
		   if(pp==1) return;  //Прекращение пакета                  
           else goto bbb;     //К подготовке следующего обмена.
	}//--------------------------------------------------------



   num_mod = 0177777;
   Reason = pornom+REASON_MKO0;
  
  for ( i = 0; pModTable[i].ID != 0; i++)
	{
if ((pModTable[i].Reason == Reason) && (pModTable[i].Address == nou)) 
		{num_mod = i; break;}
	}
  if (num_mod == 0177777)
    {
sprintf(string,"#E - В РКМ нет модели с Reason = %d-дес, номер ОУ = %o-восм \n",Reason, nou);                                               
    if(pesosib!=0) printkl(string);
	              otw=0200000; //Нет модели !
      pp=konper(mm,ind,otw,nou,r0,pornom,k);
      if(pp==1) return; else goto bbb;
    }
    
	       if(pesmko!=0)
{sprintf(string,"#I - Вызвана модель - ID = %d-дес Reason = %o , номер ОУ = %o-восм \n",
pModTable[num_mod].ID,pModTable[num_mod].Reason, pModTable[num_mod].Address);                                               
                 printkl(string);}
    
//------------------------- extract model data ------


 vxod   = (unsigned int *)pModTable[num_mod].pInputBuffer;
 vixod  = (unsigned int *)pModTable[num_mod].pOutputBuffer;
 ident  = (unsigned int )pModTable[num_mod].ID;
 ukf    =  pModTable[num_mod].Subrut;
 indmod=num_mod;

//---------------------------------------- end model finding ------
    
//-------------- Определили ----------------------------
  r=01740&r1; if((r==0)||(r==01740))
	                             //Команда управления
	{kkom=037&r1;  //Код команды управления
     if(kkom<=017)  //Команда без слова данных,                  4 ФОРМАТ.
	 { if(pesmko!=0)  printkl("#I  Команда без слова данных.");
		  if(((02000&r1)==0)&&(pesosib!=0))
{printkl("#W - MWU - Ошибка в разряде направления передачи в 4 формате.");
}
  vxod[0]=6; vxod[1]=pornom+REASON_MKO0;vxod[2]=0; vxod[5]=r1;
		uk=vxod+3; u=(DWORDLONG *)uk; u[0]=4*ttek4; 
		vxod[6]=0;
		//---------------------
 if(pesmko!=0) printkl("#I Обращаемся к функции модели.");
 (*ukf)( ); if(pesmko!=0) printkl("#I  Вышли из модели.");

		otw=vixod[5]; otw=(0177777&otw); if(vixod[0]!=6)
{if(pesosib!=0)
 printkl("#E - MWU - Ошибка 3 в kmka:  vixod[0]!=6."); otw=(0400000|otw);}
         //--------------------
        
		mm[ind1+1]=vixod[5];  //Ответ слово - в БОЗУ.
if(pesmko!=0) 
{soob.sprintf("#I Ответное слово 16-тирич. =%x",vixod[5]); printkl(soob);}
	
           pp=konper(mm,ind,otw,nou,r0,pornom,k);

	if((0400000&otw)==0)  zakaz(6);
		   if(pp==1) return;  //Прекращение пакета
		   else goto bbb;
  }//********** Конец ФОРМАТА 4.*************

	  else  //Команда управления со словом данных 5,6 форматы:
      {if((02000&r1)==0)   //Передача в ОУ,                      ФОРМАТ 6
{if(pesmko!=0)
    printkl("#I  Команда управления со словом данных, формат 6");
	vxod[0]=7; vxod[1]=pornom+REASON_MKO0;vxod[2]=0; vxod[5]=r1;
	    	uk=vxod+3; u=(DWORDLONG *)uk; u[0]=4*ttek4; 
		 vxod[6]=mm[ind1+1]; //Запись слова.
		 vxod[7]=0;
		//---------------
if(pesmko!=0) printkl("#I Обращаемся к функции модели.");
    (*ukf)( );if(pesmko!=0)  printkl("#I  Вышли из модели."); 
		otw=vixod[5]; otw=(0177777&otw); if(vixod[0]!=6)
{if(pesosib!=0)
  printkl("#E - MWU - Ошибка 4 в kmka:  vixod[0]!=6.");
				         otw=(0400000|otw);}
         //--------------------
	 
		mm[ind1+2]=vixod[5]; //Ответ слово - в БОЗУ.
if(pesmko!=0){soob.sprintf("#I Ответное слово 16-тирич. =%x",vixod[5]);
                             printkl(soob);}
	
           pp=konper(mm,ind,otw,nou,r0,pornom,k);
		if((0400000&otw)==0)	 zakaz(6);
		   if(pp==1) return;  //Прекращение пакета
		   else goto bbb;
		}//******** Конец ФОРМАТА 6. **********

	    else     //Прием из ОУ,                                 ФОРМАТ 5:
{if(pesmko!=0) 
  printkl("#I  Команда управления со словом данных, формат 5");
 vxod[0]=6; vxod[1]=pornom+REASON_MKO0;vxod[2]=0; vxod[5]=r1;
		uk=vxod+3; u=(DWORDLONG *)uk; u[0]=4*ttek4; 
		vxod[6]=0;
		//------------------------------------------
 if(pesmko!=0) printkl("#I Обращаемся к функции модели.");
  (*ukf)( ); if(pesmko!=0) printkl("#I  Вышли из модели."); 
		 otw=vixod[5]; otw=(0177777&otw); if(vixod[0]!=7)
{if(pesosib!=0)
 printkl("#E - MWU - Ошибка 5 в kmka:  vixod[0]!=7.");
				        otw=(0400000|otw);}
         //--------------------
        
		mm[ind1+1]=vixod[5]; //Ответ слово  - в БОЗУ.
if(pesmko!=0)
{soob.sprintf("#I Ответное слово 16-тирич. =%x",vixod[5]); printkl(soob);}
	
                 mm[ind1+2]=vixod[6];
	                               //Слово данных - в БОЗУ.
           pp=konper(mm,ind,otw,nou,r0,pornom,k);
			if((0400000&otw)==0) zakaz(7);
		   if(pp==1) return;  //Прекращение пакета
		   else goto bbb;
		}//*********** Конец ФОРМАТА 5. **************
      }//------- Конец команды управления со словом данных.
	}//************ Конец команды управления общий.****************

	 else  //-- Теперь - передача слов данных: 
{ kol=037&r1; if(kol==0) kol=32; //Кол-во слов

           if((02000&r1)==0)   //Передача данных в ОУ            ФОРМАТ 1
	{
	 if(pesmko!=0) printkl("#I  Передача данных в ОУ, формат 1 ");
		vxod[0]=kol+6; vxod[1]=pornom+REASON_MKO0;
		           vxod[2]=0; vxod[5]=r1;
         uk=vxod+3;  u=(DWORDLONG *)uk; u[0]=4*ttek4;
	  //--------- Вписали заголовок --------
	         for(i=1;i<=kol;i++)
			 {vxod[i+5]=(0177777&mm[ind1+i]);
//soob.sprintf("#I ПЕРЕДАЕТСЯ  В ОУ %x",vxod[i+5]); 
//                            printkl(soob);  
			 }
			  vxod[kol+6]=0;
			 //----- Переписали данные в ОУ -------
  if(pesmko!=0) printkl("#I Обращаемся к функции модели.");
   (*ukf)( ); //Обратились к модели
    otw=vixod[5]; otw=(0177777&otw);  
	if(pesmko!=0) printkl("#I  Вышли из модели.");
if(vixod[0]!=6) 
{if(pesosib!=0)
 printkl("#E - MWU - Ошибка 1 в kmka: vixod[0]!=6.");
				                      otw=(0400000|otw);}
				                   
             //------------------------------------
                        mm[ind1+kol+1]=vixod[5];
		                   //Вписыв ответного слова в БОЗУ
         if(pesmko!=0)
 {soob.sprintf("#I Ответное слово 16-тирич. =%x",vixod[5]);
                             printkl(soob);}
             //------------------------------------
           pp=konper(mm,ind,otw,nou,r0,pornom,k);
			if((0400000&otw)==0) zakaz(6);
				               //Что она могла заказать
		   if(pp==1) return;  //Прекращение пакета
		   else goto bbb;
	}//*********** Конец ФОРМАТА 1 ********************

		   else         //Прием от ОУ:                            ФОРМАТ 2
{if(pesmko!=0) printkl("#I Прием от ОУ, формат 2");                                
 vxod[0]=6; vxod[1]=pornom+REASON_MKO0; 
                   vxod[2]=0; vxod[5]=r1;
      uk=vxod+3; u=(DWORDLONG *)uk; u[0]=4*ttek4; vxod[6]=0;
	  //--------- Вписали заголовок --------
    if(pesmko!=0)  printkl("#I Обращаемся к функции модели.");   
		 (*ukf)( ); //Обратились к модели
		 otw=vixod[5]; otw=(0177777&otw);
    if(pesmko!=0) printkl("#I  Вышли из модели.");
	   
    //----------------------------
       
	if(vixod[0]!=(kol+6))
{if(pesosib!=0)
 printkl("#E - MWU - Ошибка 6 в kmka: vixod[0]!=kol+6.");
soob.sprintf("#I Размер выходного буфера /дес/ vixod[0]=%d",vixod[0]);  
		if(pesosib!=0) printkl(soob);
		 soob.sprintf("#I kol+6  /дес/=%d",kol+6);
		if(pesosib!=0) printkl(soob);
	                              otw=(0400000|otw);}      
		 //-----------------------------
	            
if(pesmko!=0) {soob.sprintf("#I Ответное слово 16-тирич. =%x",vixod[5]);
				                                 printkl(soob);}      
      mm[ind1+1]=vixod[5]; //Вписыв ответного слова в БОЗУ
	         for(i=1;i<=kol;i++)
			 {mm[ind1+i+1]= (0177777&vixod[i+5]);
//soob.sprintf("#I ПРИНЯТО ОТ ОУ %x",mm[ind1+i+1]); printkl(soob);  
			 }
			 //----- Переписали данные в БОЗУ -------
     if((0400000&otw)==0) zakaz(kol+6);//Что она могла заказать
	
             //------------------------------------
           pp=konper(mm,ind,otw,nou,r0,pornom,k);
		   if(pp==1) return;  //Прекращение пакета
		   else goto bbb;
	}//********** Конец ФОРМАТА 2 ***************

   }//********* Конец одного обмена по передаче слов данных.*****

//--- Теперь подготовка следующего обмена ------- :
bbb:    r=mm[ind+1];   //Интервал между обменами
  if((k->tnasper[pornom]+r)<=ttek4) t=ttek4+1;
  else t=k->tnasper[pornom]+r; 
 // t - время начала следующего обмена
	   if(t<Tmin)    Tmin=t;
	   //---------------------------------------
	   r=streg(100,pornom,0,0,8,k);  //RSP
	   r=(r+8);  //Сдвиг на следующий описатель

                 if(r>=0x400)            //переполнение
	{UINT iii; 
	 r=(r-0x400); 
     iii=streg(100,pornom,0,0,5,k);     //RQN
     iii=(04|iii);  
     wpis(100,pornom,0,0,5,2,iii,k);    //вписали 
	}//------------------
	   wpis(100,pornom,0,0,8,2,r,k); //Записали на RSP
        //-- Теперь формируем будильник на начало обмена:
               ii=(nom-1)*48+pornom*8;
      wus[ii+2]=1;  //Активность
	  wus[ii+3]=0;  //признак начала обмена
	  bud[ii/8]=t;

               return;
  //-- Конец обмена и подготовки следующего -----

            //-- Теперь обработка начала обмена:
    nasalo: adr=adroo(pornom,k); //Адрес описателя

if(pesmko!=0) 
{soob.sprintf("#I  Начало обмена ВМ%d на КМК - %d",(nom-1),pornom);
                         printkl(soob);
 soob.sprintf("#I Время  начала обмена /дес/  t=%lu",4*ttek4); 
                                      printkl(soob);}
 k->tnasper[pornom]=ttek4;   //В 4 - мкс
 //-----------------------------------------------
  ind=indpam(adr+8,k);  //Индекс ячейки времени начала обмена
	r=streg(100,pornom,0,0,7,k);  //RTT
	mm[ind]=0177777&r;
	//----- Вписали время начала обмена в описатель
      adr1=adrbs(adr,k); //Адрес буфера сообщений
      t=dlit(adr1,k); //Длительность передачи 
	  t=(t+ttek4);    //Время завершения передачи
       if(t<Tmin)   Tmin=t;
 //-------- Формируем будильник и слово состояния обмена:

                  ii=(nom-1)*48+pornom*8;
      wus[ii+2]=1;  //признак активности КМК;
	  wus[ii+3]=1;  //признак окoнчания передачи по будильнику;
	  bud[ii/8]=t;


//-- Сформируем слово состояния обмена в начале передачи:
      ind=indpam(adr,k); r0=mm[ind]; //Управляющее слово
      r3=040000; //С признаком начала передачи
	  if((0400&r0)!=0) r3=(020000|r3); 
	     //Взяли из упр. слова признак магистрали передачи 
      mm[ind+3]=r3; //Вписали слово состояния обмена.
      //Внимание: это последняя ячейка описателя обмена.
	//Она берется просто сдвигом индекса mm[ ] на 3 без indpam:
	//описатель может лежать только в одном куске модели памяти. 
  //------- Зафиксируем адреса описателя и буфера сообщений:
     return;
   }//=========================================


   short konper(UINT *mm,UINT ind,UINT otw,UINT nou,
	   UINT r0,short pornom,ntip *k)
	   //Запускается kmka().
   //Формирует слово состояния обмена,
   //анализ выделенного обмена, анализ прекращения
   //пакета - все это по окончанию обмена.
   //Возвращает 1 при прекращении пакета (любом),
   //иначе - 0.
   // mm - указатель на модель памяти
   // ind - индекс управляющего слова в модели памяти
   // otw - ответное слово
   // nou - номер-адрес оконечника по ком.слову
   // r0 -  управляющее слово
   // pornom - номер КМК контроллера.
   {short priz,prizo,konez; UINT r,rr;
       if((010&otw)!=0) //Абонент занят.
	{if(pesmko!=0) printkl("#I Абонент занят. Обмен не состоялся");}

      //-------------------------------------------
    if(otw<0200000)   //Есть ОС и нет ошибок в выходном буфере          
	{
		priz=0;  //Признак совпадения адресов ОУ
	    prizo=0; //Признак нормального ОС
		konez=0; //Пока нет конца пакета
		//-----------------------
		r=mm[ind+3];     //Слово состояния обмена		
        r=(0100000|r);   // Обмен закончен
         r=(0137777&r);  //Сброс начала обмена  
  rr=otw; rr=(rr>>11); rr=(037&rr); 
                         //Адрес ОУ в ответном слове
   if(nou!=rr)  //Несовпадение адресов ОУ
   {if(pesmko!=0) printkl("# I Несовпад.адресов ОУ !");
	   r=(020|r);  priz=1;
   }                      
   if((02417&otw)!=0) {r=(010000|r); prizo=1;} 
                               //Есть признаки ошибок
	 mm[ind+3]=r; //Записали слово состояния обмена.
        //-----------------------------------------------
            r=streg(100,pornom,0,0,5,k); //RQN

     if((0100000&r0)!=0)     //Выделенный обмен 
            r=(02|r);    //Запрос по выделенному обмену
			//--------------------
        if((04000&r0)!=0)  //разрешение останова пакета по
                               //плохому ОС
		{if(prizo==1)
			{konez=1; r=(1|r); //Запрос по концу пакета
			}
		}//--------------------

        if((020000&r0)!=0) //разрешение останова по несовп.адресов
        {   if(priz==1)
			{konez=1; r=(1|r); }
		}//--------------------

        if((040000&r0)!=0)   //Безусловный останов пакета
        {konez=1; r=(1|r);}  //Запрос по концу пакета
             //--------------------
   if(priz==1)  r=(040|r);
                  //Запрос при несовпадении адресов ОУ
   if(prizo==1) r=(0200|r); //Запрос при ошибках в ОС
   mm[indstreg]=r; //wpis(100,pornom,0,0,5,2,r,k); //Записали RQN.
            //---------------------------------
    r=streg(100,pornom,0,0,6,k);  //RR
	if((1&r)==0)  konez=1;    //Был сброс запуска пакета
            //--------------------------------
//               rqn_rfm_pk(pornom,k); kpr_rfm(k);
            //---------------------------------
 if(konez==1)
 {
                r=streg(100,pornom,0,0,8,k);  r=(r+8);

                 if(r>=0x400)            //переполнение
				 {UINT iii; 
	              r=(r-0x400); 
                  iii=streg(100,pornom,0,0,5,k);     //RQN
                  iii=(04|iii);  
                  wpis(100,pornom,0,0,5,2,iii,k);    //вписали 
				 }//------------------

                 wpis(100,pornom,0,0,8,2,r,k); //Увеличение RSP 
	             stoppak(pornom,k); //Стоп пакету со сбросом запуска в RR
                    rqn_rfm_pk(pornom,k); kpr_rfm(k);
			              return(1);
}//---------------------------------------------      
               else
			 {rqn_rfm_pk(pornom,k); kpr_rfm(k);			   
				   return(0);
			 }
	}//----- Конец otw<0200000 --------------- 
      
	   else if(otw==0200000)  //Нет  ОС

{ konez=0;if(pesmko!=0) printkl("#I Нет ОС. Обмен не состоялся");
		 r=mm[ind+3];     //Слово состояния обмена		
         r=(0100000|r);   // Обмен закончен
         r=(0137777&r);  //Сброс начала обмена
		     if(grup==0)     //Не групповая без ОС                     
		 r=(0100|r);    //Пометили - нет ОС
		    mm[ind+3]=r; //Записали слово состояния обмена
          //-----------------------
          r=streg(100,pornom,0,0,5,k); //RQN
	if((010000&r0)!=0)   //Разреш. останова по отсутствию ОС
          {konez=1; r=(1|r); } //Запрос по останову пакета
		  //------------------------
            if((040000&r0)!=0)   //Безусловный останов пакета
            {konez=1; r=(1|r);}  //Запрос по концу пакета
          //------------------------
  		if(grup==0)	r=(0100|r);       //Запрос по отсутствию ОС
          //------------------------
            if((0100000&r0)!=0)    //Выделенный обмен 
            r=(02|r);        //Запрос по выделенному обмену
			//--------------------
   mm[indstreg]=r;// wpis(100,pornom,0,0,5,2,r,k); //Записали RQN.     
            //--------------------
    r=streg(100,pornom,0,0,6,k);  //RR
	if((1&r)==0)  konez=1;    //Был сброс запуска пакета
    //---------------------------------
//               rqn_rfm_pk(pornom,k); kpr_rfm(k);
            //---------------------------------
 if(konez==1)
 {
        r=streg(100,pornom,0,0,8,k);  r=(r+8);

                 if(r>=0x400)            //переполнение
	{UINT iii; 
	 r=(r-0x400); 
     iii=streg(100,pornom,0,0,5,k);     //RQN
     iii=(04|iii);  
     wpis(100,pornom,0,0,5,2,iii,k);    //вписали 
	}//------------------

        wpis(100,pornom,0,0,8,2,r,k); //Увеличение RSP 	 
	 stoppak(pornom,k); //Стоп пакету со сбросом запуска в RR
            rqn_rfm_pk(pornom,k); kpr_rfm(k);
			             return(1);
 }//-----------------------------------------------      
        else
		{rqn_rfm_pk(pornom,k); kpr_rfm(k);	
			return(0);
       	}		  
   }//----- Конец otw==0200000 ----------------------------

	   else if((otw&0400000)!=0)  //Были ошибки в вых.буфере. Не равны кол-ва слов.
{  // не включался zakaz.
		priz=0;  //Признак совпадения адресов ОУ
	    prizo=0; //Признак нормального ОС
		konez=0; //Пока нет конца пакета
		//-----------------------
		r=mm[ind+3];     //Слово состояния обмена
		r=(1|r);     //Не равны кол-ва слов.
        r=(0100000|r);   // Обмен закончен
         r=(0137777&r);  //Сброс начала обмена  
  rr=otw; rr=(rr>>11); rr=(037&rr); 
                         //Адрес ОУ в ответном слове
   if(nou!=rr)  {r=(020|r);  priz=1; }
                         //Несовпадение адресов ОУ
   if((02417&otw)!=0) {r=(010000|r); prizo=1;} 
                               //Есть признаки ошибок
	 mm[ind+3]=r; //Записали слово состояния обмена.
        //-----------------------------------------------
            r=streg(100,pornom,0,0,5,k); //RQN

     if((0100000&r0)!=0)     //Выделенный обмен 
            r=(02|r);    //Запрос по выделенному обмену
			//--------------------
        if((04000&r0)!=0)  //разрешение останова пакета по
                               //плохому ОС
		{if(prizo==1)
			{konez=1; r=(1|r); //Запрос по концу пакета
			}
		}//--------------------

        if((020000&r0)!=0) //разрешение останова по несовп.адресов
        {   if(priz==1)
			{konez=1; r=(1|r); }
		}//--------------------

        if((040000&r0)!=0)   //Безусловный останов пакета
        {konez=1; r=(1|r);}  //Запрос по концу пакета
             //--------------------
		r=(040|r);  //Не равны кол-ва слов.
   if(priz==1)  r=(040|r);
                  //Запрос при несовпадении адресов ОУ. Уже избыточно.
   if(prizo==1) r=(0200|r); //Запрос при ошибках в ОС
   mm[indstreg]=r; //wpis(100,pornom,0,0,5,2,r,k); //Записали RQN.
            //---------------------------------
    r=streg(100,pornom,0,0,6,k);  //RR
	if((1&r)==0)  konez=1;    //Был сброс запуска пакета
            //--------------------------------
//               rqn_rfm_pk(pornom,k); kpr_rfm(k);
            //---------------------------------
 if(konez==1)
 {
        r=streg(100,pornom,0,0,8,k);  r=(r+8);

                 if(r>=0x400)            //переполнение
	{UINT iii; 
	 r=(r-0x400); 
     iii=streg(100,pornom,0,0,5,k);     //RQN
     iii=(04|iii);  
     wpis(100,pornom,0,0,5,2,iii,k);    //вписали 
	}//------------------

        wpis(100,pornom,0,0,8,2,r,k); //Увеличение RSP 	 
	 stoppak(pornom,k); //Стоп пакету со сбросом запуска в RR
           rqn_rfm_pk(pornom,k); kpr_rfm(k);
			              return(1);
 }//---------------------------------------------      
               else 
			{rqn_rfm_pk(pornom,k); kpr_rfm(k);	   
				   return(0);
			 }
 }//----- Конец (otw&0400000)!=0 -----------------------
    

  else if((otw&01000000)!=0)  //При ОУ2 -> ОУ1 несовп.адресов в ОУ2.
{ if(pesmko!=0) printkl("# I Несовпад.адресов ОУ2   .");
		priz=0;  //Признак совпадения адресов ОУ1
	    prizo=0; //Признак нормального ОС
		konez=0; //Пока нет конца пакета
		//-----------------------
		r=mm[ind+3];     //Слово состояния обмена		
        r=(0100000|r);   // Обмен закончен
         r=(0137777&r);  //Сброс начала обмена  
  rr=otw; rr=(rr>>11); rr=(037&rr); 
                         //Адрес ОУ в ответном слове
   if(nou!=rr)   
   {if(pesmko!=0) printkl("# I Несовпад.адресов ОУ1 ."); }
 	r=(020|r);  priz=1; //Несовпад.адресов здесь всегда, без условий.
                         
   if((02417&otw)!=0) {r=(010000|r); prizo=1;} 
                               //Есть признаки ошибок
	 mm[ind+3]=r; //Записали слово состояния обмена.
        //-----------------------------------------------
            r=streg(100,pornom,0,0,5,k); //RQN

     if((0100000&r0)!=0)     //Выделенный обмен 
            r=(02|r);    //Запрос по выделенному обмену
			//--------------------
        if((04000&r0)!=0)  //разрешение останова пакета по
                               //плохому ОС
		{if(prizo==1)
			{konez=1; r=(1|r); //Запрос по концу пакета
			}
		}//--------------------

        if((020000&r0)!=0) //разрешение останова по несовп.адресов
        {   if(priz==1)           //здесь priz=1 всегда
			{konez=1; r=(1|r); }
		}//--------------------

        if((040000&r0)!=0)   //Безусловный останов пакета
        {konez=1; r=(1|r);}  //Запрос по концу пакета
             //--------------------
   if(priz==1)  r=(040|r);
                  //Запрос при несовпадении адресов ОУ
   if(prizo==1) r=(0200|r); //Запрос при ошибках в ОС
    mm[indstreg]=r; //wpis(100,pornom,0,0,5,2,r,k); //Записали RQN.
            //---------------------------------
    r=streg(100,pornom,0,0,6,k);  //RR
	if((1&r)==0)  konez=1;    //Был сброс запуска пакета
            //--------------------------------
//               rqn_rfm_pk(pornom,k); kpr_rfm(k);
            //---------------------------------
	if(konez==1)
{ 
        r=streg(100,pornom,0,0,8,k);  r=(r+8);

                 if(r>=0x400)            //переполнение
	{UINT iii; 
	 r=(r-0x400); 
     iii=streg(100,pornom,0,0,5,k);     //RQN
     iii=(04|iii);  
     wpis(100,pornom,0,0,5,2,iii,k);    //вписали 
	}//------------------
        wpis(100,pornom,0,0,8,2,r,k); //Увеличение RSP 	 
	 stoppak(pornom,k); //Стоп пакету со сбросом запуска в RR
           rqn_rfm_pk(pornom,k); kpr_rfm(k);
			              return(1);
}//---------------------------------------      
               else 
			{rqn_rfm_pk(pornom,k); kpr_rfm(k);
				   return(0);
			}
  }//----  Конец (otw&01000000)!=0 ---------------------
	   else                
	   {                         
	   return(0);}
  }//===========================================


   short konper2(UINT *mm,UINT ind,UINT nou,UINT r0,
	                              short pornom,ntip *k)
       //Запускается kmka().
   //Формирует слово состояния обмена,
   //анализ выделенного обмена, анализ прекращения
   //пакета - все это по окончанию обмена.
   //Возвращает 1 при прекращении пакета (любом),
   //иначе - 0.
   // mm - указатель на модель памяти
   // ind - индекс управляющего слова в модели памяти
   // nou - номер-адрес оконечника
   // r0 -  управляющее слово
   // pornom - номер КМК
   // ИСПОЛЬЗУЕТСЯ ТОЛЬКО ПРИ ГРУППОВЫХ ОБМЕНАХ
   {short konez; UINT r;

		konez=0; //Пока нет конца пакета
		//-----------------------
		r=mm[ind+3];     //Слово состояния обмена		
        r=(0100000|r);   // Обмен закончен
         r=(0137777&r);  //Сброс начала обмена  
	 mm[ind+3]=r; //Записали слово состояния обмена.
        //-----------------------------------------------
            r=streg(100,pornom,0,0,5,k); //RQN

     if((0100000&r0)!=0)     //Выделенный обмен 
            r=(02|r);    //Запрос по выделенному обмену
			//--------------------
        if((040000&r0)!=0)   //Безусловный останов пакета
        {konez=1; r=(1|r);}  //Запрос по концу пакета
             //--------------------
   mm[indstreg]=r; //wpis(100,pornom,0,0,5,2,r,k); //Записали RQN.
            //---------------------------------
    r=streg(100,pornom,0,0,6,k);  //RR
	if((1&r)==0)  konez=1;    //Был сброс запуска пакета
            //--------------------------------
//               rqn_rfm_pk(pornom,k); kpr_rfm(k);
            //---------------------------------
 if(konez==1) 
 {
        r=streg(100,pornom,0,0,8,k);  r=(r+8);

                 if(r>=0x400)            //переполнение
	{UINT iii; 
	 r=(r-0x400); 
     iii=streg(100,pornom,0,0,5,k);     //RQN
     iii=(04|iii);  
     wpis(100,pornom,0,0,5,2,iii,k);    //вписали 
	}//------------------
        wpis(100,pornom,0,0,8,2,r,k); //Увеличение RSP 
       //---------------------------------------------	 
	 stoppak(pornom,k); //Стоп пакету со сбросом запуска в RR
              rqn_rfm_pk(pornom,k); kpr_rfm(k);
			              return(1);
 }//---------------------------------------      
               else
		{rqn_rfm_pk(pornom,k); kpr_rfm(k);
				   return(0);
		}
  }//===========================================




//----  ФУНКЦИИ ИСПОЛЬЗУЕМЫЕ В proz() и при начальном запуске в zakr.cpp.


	void wnu(UINT adr,char ss,UINT sisl,UINT *uk,ntip *k) 
		//Обращ.к внеш.рег.  по физич. адресу adr.
		   //Запускается в proz().
	{ UINT *m,i,j,tip,podtip,pornom,nblok,in,it,kon;        
	  m=k->rw; kon=0; i=0;        //ss=z запись, ss=s чтение.
	         while(m[i]!=0)
{if(m[i]==0xfff)
	{tip=m[i+1];pornom=m[i+2];podtip=m[i+3];nblok=m[i+4];
			    i=i+5; in=i;
	}
			 else
             {printkl("#F - MWU - Ошибка 1 в wnu.");
			  return;}

			  j=i;  while((m[j]!=0xfff)&&(m[j]!=0))
			{ if(tip==200)  //БОЗУ
				{if((m[i]<=adr)&&(m[i+1]>=adr))
					 {kon=1; break;}
				  j=i+2;
				}//-------
			    else  //Регистр
                {  if(m[j]==adr) {it=j; kon=1; break; }
			         j=j+1;
				}
			 }  if(kon==1) break;
			    i=j;
		}	 
	             if(kon!=1)
	{printkl("#F - MWU - Адрес ВУ не найден. Отказ wnu. ");
soob.sprintf("# Было обращение к ВУ - внешнему устройству по физ.адресу %x",adr);
                               printkl(soob);
			 return;;}
          if(tip==1)
		  {belt(in,it,ss,sisl,uk,pornom,k); return; }
      else if(tip==50)
	  {kpr(in,it,ss,sisl,uk,k); return;}
	  else if(tip==51) {ostkfu(in,it,ss,sisl,uk,k); return;}
      else if(tip==52) {pk(pornom,in,it,ss,sisl,uk,k); return;}
	  else if(tip==60) {pwr(in,it,ss,sisl,uk,k); return;}
	  else if(tip==100) {  kmk(in,it,ss,sisl,uk,pornom,k); return;}
      else if(tip==149) {uzwid(in,it,ss,sisl,uk,k); return;}          //Узел выдачи ММО
	  else if(tip==150) {uzpriem(in,it,ss,sisl,uk,pornom,k); return;} //Узел приема ММО
	  else if(tip==153) {uztsinxr(in,it,ss,sisl,uk,k); return;}       //Узел тайм.синхр.ММО
      else if(tip==200) {bozu(pornom,adr,ss,sisl,uk,k); return;}
		  else
          {printkl("#F - MWU - Непредусмотренное обращение в wnu.");
		                                     return;}
	} //===================================================


 void fnomou(ntip *k)
//Вписывает в регистр ответного слова адрес ОУ по nomou[] для каждого КМК.
             //Запускается в proz().
 {UINT i,r,n;
       for(i=0;i<6;i++)
	{if(k->rtt[i]==0) continue;
      r=streg(100,i,0,0,9,k);  //RSW
	  n=nomou[i]; n=(n<<11); //Адрес ОУ для RSW
	  r=(03777&r); r=(r|n);  //Приписали этот адрес
      wpis(100,i,0,0,9,2,r,k);  //Вписали в RSW
	}
 }//=======================


	char * mdop(UINT *rwu,ntip *k) //Строит дополнение модели ЗУ
          //Запускаетя в zakr.cpp при начальном запуске комплекса.		
{UINT *m,rab,i,j,ind,adr,adrk;//по массиву rwu внешних устройств,    
      char *mm;                //возвращает указатель дополнения. 
        m=k->mp; rab=m[0]; //Кол-во байт в самой модели ЗУ.
        rab=(rab/4)+4;    //С запасом.
		       mm=(char *)malloc(rab);  if(mm==NULL)
	{printkl("#E - MWU -  Нет дополнительной памяти в mdop."); 
			                                  return(NULL);}
            for(i=0;i<rab;i++)  mm[i]=0;  //Почистили дополнение.
			i=0; while(rwu[i]!=0) //Цикл по массиву RWU
	{
			  if(rwu[i]==0xfff) i=i+5;//На начало физич. адресов.
			  else 
{printkl("#F - MWU - Ошибка 1 в массиве регистров ВУ в mdop.");
          	                 return(NULL);    //ExitThread(TRUE);
}
                   if(rwu[i-4]<200) //Пометки - 1  в регистры.
			{j=i;  while((rwu[j]!=0xfff)&&(rwu[j]!=0))
			  {adr=rwu[j]; ind=indpam(adr,k);
				      if(ind>rab)
		{printkl("#F - MWU - Ошибка 2 в mdop"); return(NULL); //ExitThread(TRUE);
		}
			           if(ind==1)
		{printkl("#F - MWU - В mdop  не найден адрес регистра.");
					                     return(NULL);      //ExitThread(TRUE);
		}
                  mm[ind]=1; //Пометка  
				  j=j+1;
			  }
			     i=j;
			}//------------------------------------
             else    //Пометки - 1   в БОЗУ.     
             {adr=rwu[i]; adrk=rwu[i+1];
			         for(j=adr;j<=adrk;j=j+4)
					 {ind=indpam(j,k); 
				      if(ind>rab)
	{printkl("#F - MWU - Ошибка 3 в mdop"); return(NULL); //ExitThread(TRUE);
	}
			           if(ind==1)
					{printkl("# В mdop  не найден адрес БОЗУ.");
				              return(NULL);               //ExitThread(TRUE);
					}
                         mm[ind]=1;//Пометка
					 }
                i=i+2;
             }                                    
      }//Конец цикла по массиву RWU
		return(mm);	
  }//===================================================

  //-----  КОНЕЦ  ЭТОЙ  ГРУППЫ  ФУНКЦИЙ ----------



//---- ФУНКЦИИ, ОБСЛУЖИВАЮЩИЕ  КМК-ОКОНЕЧНИК ---------


 void kmkaou(UINT pornom,UINT indm,ntip *k)
//Реализует окончание обмена, когда КМК оконечник.
// Аналог kmka. indm - идентификационный номер модели в Table.
//pornom - номер КМК - ОУ.
            //Запускается dispet().
 {
  UINT *mm,*uk,adr,adr1,r,r0,r1,ind,ind1,kkom,otw,nou,rr;
  UINT kol,i,j; short pp;   UINT REASON,num_mod;
  DWORDLONG *u; int ii,nom; 
   //------------------------------
                 nom=0;
   for(ii=1;ii<=kolwm;ii++) {if(k==bazuk[ii]) {nom=ii; break; }  }
   if((nom>kolwm)||(nom<1))
    printkl("# Информируйте ЭИК об ошибке 1 в kmkaou");
   //-------------------------------------------------------
   if(Komplex[nom]==0)
   {soob.sprintf("# ВМ%d оконечник модели %d отключена! Отказ.",nom-1,indm); printkl(soob); return;}
                       //-----------------------------------------
           mm=k->mp;

	 if(k==0) {printkl("Нет ведущей ВМ на МКО. В kmkaou(). Ошибка!"); return;}

     
		   //Ищем модель:
num_mod=0177777; REASON=REASON_OU0+pornom;
        for(i=0;pModTable[i].ID!=0;i++)
	{if((pModTable[i].Reason==REASON)&&(pModTable[i].ID==indm))
        {num_mod=i; break;}
	}
          if(num_mod==0177777)
{soob.sprintf("# В PKM нет модели-контроллера %d (дес.) Reason=%d (дес.) Отказ.",indm,REASON); 
                     printkl(soob); return;
	}//---- Нашли --------------------------

   vxod= (UINT *)pModTable[num_mod].pInputBuffer;
   vixod=(UINT *)pModTable[num_mod].pOutputBuffer;
   ident=  (UINT)pModTable[num_mod].ID;
            ukf= pModTable[num_mod].Subrut;
        indmod=num_mod;
    //------------------------


         if(pesmko!=0)
   {soob.sprintf("#I  Окончание обмена на КМК %d.ЭВМ оконечник.",pornom);
     printkl(soob);    
    soob.sprintf("#I  Время окончания обмена /дес/ t=%lu",4*ttek4);
	 printkl(soob);
   }//------------------------


   adr=adroo(pornom,k); //Адрес описателя обмена.
   adr1=adrbs(adr,k);   //Адрес буфера сообщений.
   ind=indpam(adr,k);   
   r0=mm[ind];          //Управляющее слово.

 //------------------------------
     otw=streg(100,pornom,0,0,9,k);                    //Сброс предшеств.занятости
	 otw=(037777777767&otw);             //Сбросили
     wpis(100,pornom,0,0,9,2,otw,k);      //Вписали 
 //----------------------------

	              //И сбросить exe в RR:
	 rr=streg(100,pornom,0,0,6,k);
     rr=0177776&rr;  //Сбросили
     wpis(100,pornom,0,0,6,2,rr,k);      //Вписали
 //----------------------------

   //Учет LocBB в управл.слове на "абонент занят" :
               if((0400&r0)!=0)
	{otw=streg(100,pornom,0,0,9,k); otw=(010|otw);
     wpis(100,pornom,0,0,9,2,otw,k); //Вписали в рег.
	}//------------------------------------
   ind1=indpam(adr1,k); //Индекс буфера сообщен6ий в БОЗУ.
   r1=mm[ind+1];        //Командное слово.
             if(pesmko!=0)
   {soob.sprintf("#I Командное слово = %x",r1);
           printkl(soob);
   }//------------------------
     nou=r1>>11; nou=(037&nou); //Адрес ОУ в ком.слове.
   //-------------------------------------------------
         //Учет GlbBB(RR) на  "абонент занят" :
    rr=streg(100,pornom,0,0,6,k);  //RR
	if((010&rr)!=0)
    {otw=streg(100,pornom,0,0,9,k);
	 otw=(010|otw); //абонент занят
	 wpis(100,pornom,0,0,9,2,otw,k); //Вписали в рег.отв.слова
	}//-------------------------------------------------------

    otw=streg(100,pornom,0,0,9,k); //Ответное слово
	otw=(0177777&otw);
	if((02015&otw)!=0)  //Всякие бяки
	goto	baka;
   //-------------------------------------------------
    r=01740&r1; if((r==0)||(r==01740))  //Ком.управления.
 {
		kkom=037&r1;  //Код команды управления.
        if(kkom<=017)  //Без слова данных, 4-й формат.
   {if(pesmko!=0) 
     printkl("#I Команда без слова данных. 4-й формат.");
         if(((02000&r1)==0)&&(pesosib!=0))
     printkl("#W Ошибка в разряде направления передачи.");
 //----- Теперь проверка  vixod./учитывался ранее в kontrmod/  
        i=35*pornom;   
    if(k->bufou[i]!=0) //Д.быть нулевое кол-во данных.
   {  if(pesosib!=0)
        {soob.sprintf("#E Ошибка в размере вых.буфера: vixod[0]!=6 в модели %d",ident); 
   printkl(soob);
        }
   }//-------------------------
        if(kkom==0)        //Принять управление интерфейсом
{if((010000&r0)!=0)        //Анализ SpcOpt  в управляющем слове
       otw=(02|otw);      //будет принято
 else  otw=(0177775&otw); //не принято
      wpis(100,pornom,0,0,9,2,otw,k); //Вписали и в регистр отв.слово
}//-----------------------------

baka:	if(pesmko!=0)
{soob.sprintf("#I Ответное слово =  %x",otw); printkl(soob);}
	vxod[0]=6; vxod[1]=pornom+REASON_OU0; vxod[2]=0;
	vxod[5]=otw; uk=vxod+3; u=(DWORDLONG *)uk;
	u[0]=4*ttek4; vxod[6]=0;
	//-----------------------

            if((k->wds[pornom]==1)||(k->wklblk==0))      
{if(pesmko!=0) printkl("#I Обращаемся к функции модели, информируя ее"); 
    (*ukf)();
if(pesmko!=0) printkl("#I Вышли из модели");
   //------------------------
      pp=konperou(mm,ind,otw,nou,r0,pornom,k);
	   //-- Окончание передачи, когда КМК оконечник.
	          zakaz(0);   return;
}
             else                            //Коррекция для пассивной ВМ
{
    soob.sprintf("# КУК отключил передатчики ВМ %d на МКО-%d",(nom-1),pornom);
      printkl(soob);

 pp=konperou(mm,ind,otw,nou,r0,pornom,k);
 return;
}

  }//******** Конец команды без слова данных ********
      //-- Теперь команда управления со словом данных:
             else    //со словом данных
{if((02000&r1)==0)  //Формат 6. Передача в ОУ - ЭВМ.				 
		{if(pesmko!=0) 
	 printkl("#I Команда принять КМК слово данных.Формат 6.");
	 //-- Проверим правильность прежнего vixod[0]:
			 i=35*pornom;
     if(k->bufou[i]!=1)  //Д.быть одно слово
     {    if(pesosib!=0)
		{soob.sprintf("#I Ошибка в вых.буфере: vixod[0]!=7 модели %d",ident);
	        printkl(soob);
		}
	 }//------------------------------------------------------------
          if(kkom==021)  //Синхрониз.со словом данных
{ if((010000&r0)==0)
  mm[ind1]=k->bufou[i+3]; //Приняли слово в БОЗУ
  else
  wpis(100,pornom,0,0,7,2,k->bufou[i+3],k); //Вписали в RTT
}//----------------------------------
		  else
        mm[ind1]=k->bufou[i+3]; //Приняли слово в БОЗУ        
         //-----------------------------------------------------------
        otw=streg(100,pornom,0,0,9,k); //Ответное слово
		otw=(0177777&otw);
	if(pesmko!=0)
{soob.sprintf("#I Ответное слово =  %x",otw); printkl(soob);}
	vxod[0]=6; vxod[1]=pornom+REASON_OU0; vxod[2]=0;
	vxod[5]=otw; uk=vxod+3; u=(DWORDLONG *)uk;
	u[0]=4*ttek4; vxod[6]=0;
	//-----------------------

       if((k->wds[pornom]==1)||(k->wklblk==0))
{if(pesmko!=0) printkl("#I Обращаемся к функции модели, информируя ее"); 
    (*ukf)();
if(pesmko!=0) printkl("#I Вышли из модели");
   //------------------------
      pp=konperou(mm,ind,otw,nou,r0,pornom,k);
	   //-- Окончание передачи, когда КМК оконечник.
	          zakaz(0);   return;
}
      else                                                   //Коррекция для пассивной ВМ
{
    soob.sprintf("# КУК отключил передатчики ВМ %d на МКО-%d",(nom-1),pornom);
      printkl(soob);
		    
  pp=konperou(mm,ind,otw,nou,r0,pornom,k);
  return;
}

      
	}//******* Конец формата 6 ****************
       else    //Формат 5. Выдача из ЭВМ в модель слова данных
	{if(pesmko!=0) 
	 printkl("#I Формат 5. Выдача слова из ЭВМ в модель");
	 //-- Проверим правильность прежнего vixod[0]:
			 i=35*pornom;
     if(k->bufou[i]!=0)  //Д.быть 0 слов.
     {    if(pesosib!=0)
		{soob.sprintf("#I Ошибка в вых.буфере: vixod[0]!=6 модели %d",ident);
	        printkl(soob);
		}
	 }//------------------
       otw=streg(100,pornom,0,0,9,k);  //Ответное слово
		otw=(0177777&otw);
	if(pesmko!=0)
{soob.sprintf("#I Ответное слово =  %x",otw); printkl(soob);}
	vxod[0]=7; vxod[1]=pornom+REASON_OU0; vxod[2]=0;
	vxod[5]=otw; uk=vxod+3; u=(DWORDLONG *)uk;
	u[0]=4*ttek4;
	//------------   Анализ команды :       -----
       if(kkom==023)   //Выдать слово ВСК
{if((010000&r0)==0)
 vxod[6]=streg(100,pornom,0,0,10,k);  //Из RSC
 else
 vxod[6]=mm[ind1]; //Это слово данных из БОЗУ
}//------------------
   else if(kkom==022)  //Выдать последнюю команду
   vxod[6]=streg(100,pornom,0,0,13,k);  //Из RCW	   
   //-----------------
    else   //прочие
	vxod[6]=mm[ind1]; //Это слово данных из БОЗУ
    //-------------------------------------------
	vxod[7]=0;
	//-----------------------

      if((k->wds[pornom]==1)||(k->wklblk==0))
{if(pesmko!=0) printkl("#I Обращаемся к функции модели, информируя ее"); 
    (*ukf)();
if(pesmko!=0) printkl("#I Вышли из модели");
   //------------------------
      pp=konperou(mm,ind,otw,nou,r0,pornom,k);
	   //-- Окончание передачи, когда КМК оконечник.
	          zakaz(0);   return;
}
       else                                                      //Коррекция для пассивной ВМ
{
    soob.sprintf("# КУК отключил передатчики ВМ %d на МКО-%d",(nom-1),pornom);
      printkl(soob);
		   
 pp=konperou(mm,ind,otw,nou,r0,pornom,k);
  return;
}

      
	}//************* Конец формата 5 ***************
}//************ Конец ком. со словом данных
 }//******** Конец команды управления ****************
    else   //-- Теперь передача слов данных:
{kol=037&r1; if(kol==0) kol=32; //Кол-во слов
	if((02000&r1)==0)  //Формат 1. Передача в ЭВМ.
{if(pesmko!=0) printkl("#I Передача данных в ЭВМ - оконечник");
	 //-- Проверим правильность прежнего vixod[0]:
			 i=35*pornom;
     if(k->bufou[i]!=kol)  
     {    if(pesosib!=0)
		{soob.sprintf("#I Ошибка в вых.буфере: vixod[0] модели %d",ident);
	        printkl(soob);
		}
	 }//------------------
       otw=streg(100,pornom,0,0,9,k);  //Ответное слово
		otw=(0177777&otw);
	if(pesmko!=0)
{soob.sprintf("#I Ответное слово =  %x",otw); printkl(soob);}
	//----------------------
                for(j=1;j<=kol;j++)
  mm[ind1+(j-1)]=k->bufou[i+j+2]; //Передали данные в БОЗУ.
	//---------------------------------------------			
	vxod[0]=6; vxod[1]=pornom+REASON_OU0; vxod[2]=0;
	vxod[5]=otw; uk=vxod+3; u=(DWORDLONG *)uk;
	u[0]=4*ttek4; vxod[6]=0;
	//-----------------------

	if((k->wds[pornom]==1)||(k->wklblk==0))
{if(pesmko!=0) printkl("#I Обращаемся к функции модели, информируя ее"); 
    (*ukf)();
if(pesmko!=0) printkl("#I Вышли из модели");
   //------------------------
      pp=konperou(mm,ind,otw,nou,r0,pornom,k);
	   //-- Окончание передачи, когда КМК оконечник.
	          zakaz(0);   return;
}
       else                                   //Коррекция для пассивной ВМ
{
    soob.sprintf("# КУК отключил передатчики ВМ %d на МКО-%d",(nom-1),pornom);
      printkl(soob);
		   
  pp=konperou(mm,ind,otw,nou,r0,pornom,k);
  return;
}

}//********** Конец формата 1 *****************
     else   //---- Формат 2
{if(pesmko!=0) printkl("#I Формат 2. Выдача данных из ЭВМ в модель");
	 //-- Проверим правильность прежнего vixod[0]:
			 i=35*pornom;
     if(k->bufou[i]!=0)  
     {    if(pesosib!=0)
		{soob.sprintf("#I Ошибка в вых.буфере: vixod[0] модели %d",ident);
	        printkl(soob);
		}
	 }//------------------
       otw=streg(100,pornom,0,0,9,k);  //Ответное слово
		otw=(0177777&otw);
	if(pesmko!=0)
{soob.sprintf("#I Ответное слово =  %x",otw); printkl(soob);}
	vxod[0]=kol+6; vxod[1]=pornom+REASON_OU0; vxod[2]=0;
	vxod[5]=otw; uk=vxod+3; u=(DWORDLONG *)uk;
	u[0]=4*ttek4; vxod[kol+6]=0;
	         for(j=1;j<=kol;j++)
        vxod[j+5]=mm[ind1+(j-1)]; //Выдали данные в модель.
	//-----------------------

     if((k->wds[pornom]==1)||(k->wklblk==0))
{if(pesmko!=0) printkl("#I Обращаемся к функции модели, информируя ее"); 
    (*ukf)();
if(pesmko!=0) printkl("#I Вышли из модели");
   //------------------------
      pp=konperou(mm,ind,otw,nou,r0,pornom,k);
	   //-- Окончание передачи, когда КМК оконечник.
	          zakaz(0);   return;
}
      else                               //Коррекция для пассивной ВМ
{
    soob.sprintf("# КУК отключил передатчики ВМ %d на МКО-%d",(nom-1),pornom);
      printkl(soob);
		  
  pp=konperou(mm,ind,otw,nou,r0,pornom,k);
  return;
}


}//****** Конец формата 2 *****************

}//***** Конец передачи слов данных ****************

 }//=========== Конец kmkaou ==========================


//+++++++++++++++++ Заготовка  коррекции от 10.06.04  новая

   short konperou(UINT *mm,UINT ind,UINT otw,UINT nou,
	   UINT r0,short pornom,ntip *k)
	   //Запускается kmkaou().
   //Аналог  konper  для КМК оконечника.
   //Входы те же. Исключаются чтение RR  и реакция на
   //его нулевой разряд, а также использование
   //stoppak.
   {short priz,prizo,konez,p; UINT r,rr,RR,indupr; int i,nom,ii;
       nom=0; for(i=1;i<=kolwm;i++) 
	   {if(k==bazuk[i]) {nom=i; break;}
	   }//-------------

                     p=0;
     if((0177777&otw)==0177777)
	 {if(pesmko!=0)
       printkl("Нет ответного слова");
      p=1;
	 }//----------

       if((010&otw)!=0) //Абонент занят.
	{if((pesmko!=0)&&(p==0)) printkl("#I Абонент занят. Обмен некорректен");}

      //-------------------------------------------
	      //Учет EnbLBB в управляющем слове :
                  if((01000&r0)!=0)
		{indupr=indpam(k->adrupr[pornom],k);
          r=mm[indupr];  //управл.слово в таблице.
		   r=(0400|r);  //Вписали LocBB=1
          mm[indupr]=r; //Вписали в таблицу 
		}//----------------------------------------
		priz=0;  //Признак совпадения адресов ОУ
	    prizo=0; //Признак нормального ОС
		konez=0; //Пока нет конца пакета
		//-----------------------
		r=mm[ind+3];     //Слово состояния обмена		
        r=(0100000|r);   // Обмен закончен
         r=(0137777&r);  //Сброс начала обмена  
  rr=otw; rr=(rr>>11); rr=(037&rr); 
                         //Адрес ОУ в ответном слове
   if(nou!=rr)  
   {if(pesmko!=0) {if(p==0) printkl("# I Несовпад.адресов ОУ !");}
	     priz=1; }
                         //Несовпадение адресов ОУ
   if((02417&otw)!=0) { prizo=1;} 
                               //Есть признаки ошибок
	 mm[ind+3]=r; //Записали слово состояния обмена.
        //-----------------------------------------------
            r=streg(100,pornom,0,0,5,k); //RQN

     if((0100000&r0)!=0)     //Выделенный обмен 
            r=(02|r);    //Запрос по выделенному обмену
	  //-----------------------------------
        if((040000&r0)!=0)   //Безусловный останов пакета
        {konez=1; r=(1|r);}  //Запрос по концу пакета
             //--------------------
   if(priz==1)  r=(040|r);
                  //Запрос при несовпадении адресов ОУ
   if(prizo==1) r=(0200|r); //Запрос при ошибках в ОС
   mm[indstreg]=r; //wpis(100,pornom,0,0,5,2,r,k); //Записали RQN.
    //---------------------------------
          //Учет EnbGBB  в управл.слове для будущего :
              if((02000&r0)!=0)
	{RR=streg(100,pornom,0,0,6,k);  //RR
     rr=(010|rr);  //вписали GlbBB=1.
	 rr=(0177777&RR);
     wpis(100,pornom,0,0,6,2,RR,k); //Вписали в RR.
	}
   //------------------------------------------------------------
//               rqn_rfm_pk(pornom,k); kpr_rfm(k);
            //---------------------------------
                 ii=(nom-1)*48+pornom*8;
                      wus[ii+2]=0;  // сделали пассивным
                 //--------------------------
  

  //---------------------------------------------------
 if(konez==1)
 {
        r=streg(100,pornom,0,0,8,k);  r=(r+8);

                 if(r>=0x400)            //переполнение
	{UINT iii; 
	 r=(r-0x400); 
     iii=streg(100,pornom,0,0,5,k);     //RQN
     iii=(04|iii);  
     wpis(100,pornom,0,0,5,2,iii,k);    //вписали 
	}//------------------
        wpis(100,pornom,0,0,8,2,r,k); //Увеличение RSP
		   rqn_rfm_pk(pornom,k); kpr_rfm(k);
			              return(1);
 }      
 else   
 {
        r=streg(100,pornom,0,0,8,k);  r=(r+8);

                 if(r>=0x400)            //переполнение
	{UINT iii; 
	 r=(r-0x400); 
     iii=streg(100,pornom,0,0,5,k);     //RQN
     iii=(04|iii);  
     wpis(100,pornom,0,0,5,2,iii,k);    //вписали 
	}//------------------
        wpis(100,pornom,0,0,8,2,r,k); //Увеличение RSP 
	     rqn_rfm_pk(pornom,k); kpr_rfm(k);
	     return(0);
}
   }//====== Конец  konperou ==============

  


//+++++++++++++++++++  Заготовка  коррекции от 10.06.04  новая

  
 UINT uprslowo(UINT pornom,UINT komslowo,ntip *k)
	 //Возвращает управляющее слово для ОУ-КМК.
	 //pornom - номер КМК-ОУ,  komslowo - командное слово.
	           //Запускаетя в kontrmod().
 {UINT aou,ig,pw,sa,sd,na,j,r0,adr,rab,tab,ind,sis,*mm;
    aou=0174000&komslowo; aou=(aou>>11); //Адрес ОУ
if(aou==31) ig=1; else ig=0; //0 - индивид, 1 - групповой обмен.
if((02000&komslowo)==0) pw=0; else pw=1; //0 - прием, 1 - выдача.
  //---------------------------------------------
  sa=01740&komslowo; sa=(sa>>5); //Подадрес
  sd=037&komslowo;      //Число слов, код.ком.управления.
    //------ Определим нач.адрес  adr  таблиц в БОЗУ:
  if(pornom<2) j=0; else if(pornom<4) j=1; else j=2;
   r0=streg(52,j,0,0,6,k);  //RPN из ПК
                  adr=0;
 if((pornom%2)==0) {rab=07&r0;rab=(rab<<14);}
 else              {rab=0160&r0; rab=(rab<<10);}
  adr=(rab|adr); //Вписали номер страницы.
          //----------------------
  j=(j<<17); adr=(j|adr); //Вписали номер КО
  adr=(01774000000|adr); //Вписали старшую часть
     // -- adr - опеределен.
 //-- Теперь определим нач.адрес  tab  нужной таблицы:
               //Команды обмена:
      if((ig==0)&&(pw==0)&&(sa!=0)&&(sa!=31))
	  {tab=adr+04000; na=tab+(4*sa);}  //Адресный прием
 else if((ig==0)&&(pw==1)&&(sa!=0)&&(sa!=31))
       {tab=adr+04200; na=tab+(4*sa);}   //Адресная выдача
 else if((ig==1)&&(pw==0)&&(sa!=0)&&(sa!=31))
        {tab=adr+04400; na=tab+(4*sa);}   //Групповой прием
 else if((ig==1)&&(pw==1)&&(sa!=0)&&(sa!=31))
        {tab=adr+04600; na=tab+(4*sa);}   //Групповая выдача
            //Команды управления:

//----------- Коррекция 10.06.04.
 else if((ig==0)&&((sa==0)||(sa==31))&&(pw==0))   
	{tab=adr+05000; na=tab+(4*sd);}  //Адресная КУ
 else if((ig==0)&&((sa==31)||(sa==0))&&(pw==1))   
	{tab=adr+05200; na=tab+(4*sd);} //Адресная КУ
 else if((ig==1)&&((sa==0)||(sa==31))&&(pw==0))   
	{tab=adr+05400; na=tab+(4*sd);} //Групповая КУ
 else if((ig==1)&&((sa==31)||(sa==0))&&(pw==1))   
 {tab=adr+05600; na=tab+(4*sd);}  //Групповая КУ
 //---------- конец коррекции

 else {printkl("#F Ошибка в программе uprslowo."); }
    //---  tab определен. ---------------
          //na -  Это адрес управляющего слова
	      k->adrupr[pornom]=na;  //Запомнили этот адрес для konperou.
	        mm=k->mp;
   ind=indpam(na,k); sis=mm[ind];  //управляющее слово

            return(sis);
 }//============  КОНЕЦ uprslowo ===================




 UINT dopkomsl(UINT pornom,UINT komslowo,ntip *k)
	 //Возвращает 1 при недопустимом ком.слове для КМК-ОУ, иначе - 0.
	 //pornom - номер КМК-ОУ,  komslowo - командное слово.
          //Запускаетя в kontrmod().
 {UINT aou,ig,pw,sa,sd,na,j,r0,adr,rab,tab,ind,sis,*mm;
    aou=0174000&komslowo; aou=(aou>>11); //Адрес ОУ
if(aou==31) ig=1; else ig=0; //0 - индивид, 1 - групповой обмен.
if((02000&komslowo)==0) pw=0; else pw=1; //0 - прием, 1 - выдача.
  //---------------------------------------------
  sa=01740&komslowo; sa=(sa>>5); //Подадрес
  sd=037&komslowo;      //Число слов, код.ком.управления.
    //------ Определим нач.адрес  adr  таблиц в БОЗУ:
  if(pornom<2) j=0; else if(pornom<4) j=1; else j=2;
   r0=streg(52,j,0,0,6,k);  //RPN из ПК
                  adr=0;
 if((pornom%2)==0) {rab=07&r0;rab=(rab<<14);}
 else              {rab=0160&r0; rab=(rab<<10);}
  adr=(rab|adr); //Вписали номер страницы.
          //----------------------
  j=(j<<17); adr=(j|adr); //Вписали номер КО
  adr=(01774000000|adr); //Вписали старшую часть
     // -- adr - опеределен.
  //-- Теперь определим нач.адрес  tab  нужной таблицы:
      if((ig==0)&&(pw==0)) //Адресный прием
        tab=adr+06000;
 else if((ig==0)&&(pw==1)) //Адресная выдача
        tab=adr+06400;
 else if((ig==1)&&(pw==0)) //Групповой прием
        tab=adr+0x07000;
 else                      //Групповая выдача
        tab=adr+07400;
   //------------------------------
  na=tab+(8*sa); //Нач.адрес пары ячеек.
//-- Теперь смотрим по  sd  бит в паре ячеек на допустимость
//   командного слова:
            mm=k->mp;  //Модель памяти.
             if(sd<=15)   //В первом слове
	{ind=indpam(na,k); sis=mm[ind];
	  rab=1; rab=(rab<<sd);
	  if((rab&sis)!=0) return(1); else return(0);
	} //----
             else         //Во втором слове
	{ind=indpam(na+4,k); sis=mm[ind];
               sd=(sd-16);
	  rab=1; rab=(rab<<sd);
	  if((rab&sis)!=0) return(1); else return(0);
	} //----
 }//============== КОНЕЦ dopkomsl ============================


   //---- ВСПОМОГАТЕЛЬНЫЕ    ФУНКЦИИ -------------------


  void wpis(UINT tip,UINT pornom,UINT podtip,UINT nblok,
	                      UINT it,UINT d,UINT sis,ntip *k)
   //Меняет содержимое регистра с инд. it в k->rw.
   //(относит индекс - с начала устройства) = внутреннему индексу + 5.
   //d - действие:  0 - приформировывает 1 sis,
   //1 - пишет нули на место 1 в sis,
   //  2 - полная замена на sis. 
  {UINT *m,*mm,i,j,ind,r,a;
    if((tip==50)&&(it==5)) k->axtung=(020|k->axtung);
	                                  //запись на kpr!
     m=k->rw; mm=k->mp; i=0;
	          while(m[i]!=0)
	{if(m[i]!=0xfff) {printkl("#F - MWU - Ошибка 1 в wpis.");
			                           ExitThread(TRUE);}
 if((m[i+1]==tip)&&(m[i+2]==pornom)&&(m[i+3]==podtip)&&
			                             (m[i+4]==nblok))
          {    for(j=i+5;j<=(i+it);j++)
			{if((m[j]==0xfff)||(m[j]==0))
             {printkl("#F - MWU - Регистр не найден в wpis.");
		                               ExitThread(TRUE);}
			}  
		       ind=indpam(m[i+it],k);
	 if(ind==1) 
{printkl("#F - MWU - Ошибка индекса в wpis");
	                                 ExitThread(TRUE);}
			   
			   r=mm[ind];    /*indstreg=ind;*/
			                  //Прочитали регистр.
               if(d==0) {r=sis|r; mm[ind]=r; return;}
			   else if(d==1)
		{a=(0xffffffff)-sis; r=a&r; mm[ind]=r; return;}
			   else {mm[ind]=sis; return;}
			
		  }
		  i=i+5;  while((m[i]!=0xfff)&&(m[i]!=0))  {i=i+1;}
		}
printkl("#F - MWU - Устройство и регистр не найдены в wpis.");
soob.sprintf("#I tip=%d pornom=%d  it=%d d=%d sis=%d",
			tip,pornom,it,d,sis); printkl(soob);
                   ExitThread(TRUE);


  }//======================================================


  UINT xitpr(UINT star,UINT now,UINT zam,UINT soxr,UINT xit0,
	                            UINT xit1,UINT sistka,UINT n)
  // star - старое значение, now - записываемое в него, 
  // zam - те разряды, которые точно идут взамен из now,
  //soxr - сохраняемые.
  // xit0 - особые разряды в now: 
                  //1 - не влияют, 0 - вписывается нулем.
  // xit1 - особые разряды в now:
                     //0 - не влияют, 1 - вписывается  1.
  // sistka - указывыает очищаемые разряды результата.
  //  разряды указываются 1 в их местах.
  {UINT r0,r,r1,r2,r3,r4,r5,r6,r7,rt;
               r0=0xffffffff;
    if((zam+soxr+xit0+xit1+sistka)!=r0)
{printkl("#F - MWU - Ошибка  в параметрах в xitpr.");
	soob.sprintf("# N=%d ",n);printkl(soob);
	soob.sprintf("# zam=%x",zam);printkl(soob);
	soob.sprintf("# soxr=%x",soxr);printkl(soob);
	soob.sprintf("# xit0=%x",xit0);printkl(soob);
	soob.sprintf("# xit1=%x",xit1);printkl(soob);
	soob.sprintf("# sistka=%x",sistka);printkl(soob);

	ExitThread(TRUE);}
          //-------------------------------
	r=(now&zam)|(star&soxr);   //вписывается безусловно

	         //Работаем с xit0:
	r1=r0-now;   //отметили единицами нули в now
	r2=r1&xit0;  //Отметили единицами обнуляемые разряды
	             // в оставшейся части ответа
	r3=r0-r2;    // инвертировали
	r4=(star&xit0)&r3;   //часть от xit0  готова.

	        //Теперь работаем с xit1:
	r5=now&xit1;	 //отметили единицами вписываемые 1. 
    rt=(r0-r5)&xit1;
//отметили единицами не затрагиваемые в star/xit1 разряды.
	r1=star&rt;      //сохранили их для вписывания.
	          //Теперь вписываем все:
	r6=r|r4|r5|r1;    //неочищенный ответ
	r7=(r0-sistka)&r6;  //очищенный, избыточно.
	        return(r7);

  }//==================================================


  UINT streg(UINT tip,UINT pornom,UINT podtip,
	                            UINT nblok,UINT it,ntip *k) 
   //Читает заказанный регистр, кроме БОЗУ.   
   //it -относит.индекс от обознач.начала типа.
  {UINT *m,*mm,i,j,ind,r;
     m=k->rw; mm=k->mp; i=0;
	          while(m[i]!=0)
	{if(m[i]!=0xfff) {printkl("#F - MWU - Ошибка 1 в streg.");
			                            ExitThread(TRUE);}
          if((m[i+1]==tip)&&(m[i+2]==pornom)&&
			            (m[i+3]==podtip)&&(m[i+4]==nblok))
          {    for(j=i+5;j<=(i+it);j++)
			{if((m[j]==0xfff)||(m[j]==0))
             {printkl("#F - MWU - Регистр не найден в streg.");
		                               ExitThread(TRUE);}
			}  
		       ind=indpam(m[i+it],k);
	 if(ind==1) 
{printkl("#F - MWU - Ошибка индекса в streg");ExitThread(TRUE);}

			   r=mm[ind]; indstreg=ind;
			                   //Прочитали регистр.
			           return(r);
		  }
		 i=i+5;  while((m[i]!=0xfff)&&(m[i]!=0))  {i=i+1;}
		}
printkl("#F - MWU - Устройство и регистр не найдены в streg.");
	                                      ExitThread(TRUE);
              return(0); //Обман транслятора
  }//=========================================================


  UINT nreg(UINT tip,UINT pornom,UINT podtip,
	                           UINT nblok,UINT it,ntip *k) 
   //Ищет заказанный регистр, кроме БОЗУ. 
   //it -относит.индекс от обознач.начала типа.
   // Возвращает 1 если регистр есть. Иначе - 0.
   // Если есть, то в indstreg  его индекс в модели памяти.  
  {UINT *m,*mm,i,j,ind;
     m=k->rw; mm=k->mp; i=0;
	          while(m[i]!=0)
		{if(m[i]!=0xfff) {printkl("#F - MWU - Ошибка 1 в nreg.");
			                            ExitThread(TRUE);}
          if((m[i+1]==tip)&&(m[i+2]==pornom)&&
			            (m[i+3]==podtip)&&(m[i+4]==nblok))
          {    for(j=i+5;j<=(i+it);j++)
			{if((m[j]==0xfff)||(m[j]==0))
		  {  return(0);}
			}  
		       ind=indpam(m[i+it],k);
	 if(ind==1) 
{printkl("#F - MWU - Ошибка индекса в nreg");ExitThread(TRUE);}
			    indstreg=ind;       //Нашли регистр.
			           return(1);
		  }
		  i=i+5;  while((m[i]!=0xfff)&&(m[i]!=0))  {i=i+1;}
		}
          return(0);
  }//=========================================================


  UINT adroo(UINT pornom,ntip *k)
 //Определяет физический адрес описателя обмена по номеру  КМК
  {UINT r0,r1,j,adr,rab;
   //Определим j -  номер ПК = номеру КО:
    if(pornom<2) j=0; else if(pornom<4) j=1; else j=2;
	r0=streg(52,j,0,0,6,k);         //RPN из ПК
	r1=streg(100,pornom,0,0,8,k);   //RSP из КМК

      adr=0;  //Получим номер страницы:
               if((pornom%2)==0)
			  {rab=07&r0;   rab=(rab<<14); }
	    else  {rab=0160&r0; rab=(rab<<10); }
         adr=(rab|adr);     //Вписали номер страницы
    j=(j<<17); adr=(j|adr); //Вписали номер КО    
     adr=(01774000000|adr); //Вписали старшую часть
        // Теперь учтем  RSP:
     r1=(01776&r1); r1=(r1<<1); adr=(r1|adr); //Учли

	           return(adr);
  }//======================================


  UINT adrbs(UINT adr,ntip *k)
 //Вычисляет физический адрес буфера соообщений по адресу adr
 //описателя обмена
  {UINT *mm,r,r0,ind;
    mm=k->mp; ind=indpam(adr,k); r0=mm[ind]; 
	 //r0 - это содержимое по адресу опис. обмена
     // управляющее слово.
                r0=(0377&r0); 
	//Выделили 8 старших разрядов для адреса буфера 
    r0=(r0<<6); r=adr; r=(037777740000&r);
	                //Вычистили 12 разрядов
	   r=(r0|r);   return(r);
  }//==========================================


  DWORDLONG dlit(UINT adr,ntip *k)
  //Определяет длительность передачи по адресу adr 
  // буфера сообщений, т.е. где лежит командное слово.
  //Единица измерения времени - 4 мкс
  {UINT *mm,r0,r1,ind; DWORDLONG z;
    mm=k->mp; ind=indpam(adr,k); r0=mm[ind];
	              //Это командное слово.
	r1=01740&r0;  //Подадрес - режим управления
	if((r1==0)||(r1==01740)) return(12); 
	                   //Это команда управления
         //Теперь - передача данных: 
    r1=037&r0; //Количество слов
	if(r1==0) r1=32; 
	r1=r1+2;     //Учет ответного и командного слова
	r1=(5*r1)+2; //+2 - учет паузы
	  z=(DWORDLONG)r1;   return(z);
  }//==============================================


  UINT indpam(UINT adr,ntip *k)  //Возвращает индекс в модели
  { UINT rab,in,*m;              // по физическому адресу,
       if((adr%4)!=0)            //или 1 при неудаче.
	   {soob.sprintf("# adr=%x",adr); printkl(soob);
printkl("#E - MWU - Адрес  не кратен 4 в indpam. Отказ.");
        ExitThread(TRUE); }                                                   
                         //Взяли модель памяти.
                        m=k->mp;
		          for(in=1; ;in=(in+8))
		{ if((adr>=m[in])&&(adr<m[in+1])) //Нашли кусок памяти.
				{ rab=adr-m[in]; rab=(rab>>2); 
				  rab=(rab+m[in+3]); //Это индекс в модели.           

				    return(rab);
				 }
               if(m[in+2]==1)   //данные непрочитаны.
			   {         return(1);  }
		}
  }
  //==================================================


   //---- КОНЕЦ  ВСПОМОГАТЕЛЬНЫХ  ФУНКЦИЙ -----------------



  //------------СБРОСЫ   УСТАНОВЫ   РЕГИСТРОВ --------------

   void sbrostimeb1(ntip *k) //Сброс - установ таймеров типа 1.
   {UINT i;
        for(i=0;i<4;i++)
		{wpis(1,i,0,0,5,2,0,k);            //RT
         wpis(1,i,0,0,6,2,0,k);            //RP
         wpis(1,i,0,0,7,2,04100000000,k);  //RC 
         wpis(1,i,0,0,8,2,0,k);            //Нулевой регистр
        }

   }//==============================================



  void sbrkpr(ntip *k) //Сброс - установ контроллера прерываний.
  {                                      //центрального
    wpis(50,0,0,0,9,2,0,k);  //Вписали  0  в Rexc
    wpis(50,0,0,0,5,2,0252401252,k);  //Вписали маски в RQM.
	  //Далее - обнуление нулей:
    wpis(50,0,0,0,6,2,0,k);
	wpis(50,0,0,0,7,2,0,k);
	wpis(50,0,0,0,8,2,0,k);
	wpis(50,0,0,0,10,2,0,k);
	wpis(50,0,0,0,11,2,0,k);
	wpis(50,0,0,0,12,2,0,k);

  }//=================================================


  void sbrosostkfu(ntip *k)      //сброс остатков KFU
  { UINT i;
     for(i=5;i<13;i++)
     wpis(51,0,0,0,i,2,0,k);
  }//================================================


  void sbrospk(ntip *k)       //сброс - установ регистров ПК.
  {UINT i,j;
               for(i=0;i<3;i++)
	{wpis(52,i,0,0,5,2,0125252,k);
	             for(j=6;j<15;j++)
			{wpis(52,i,0,0,j,2,0,k); }		 

	}
  }//=================================================

  void sbrospwr(ntip *k)      //сброс - установ регистров ПВР. 
  { int i;
    for(i=0;i<29;i++)
    wpis(60,0,0,0,i+5,2,0,k);
	 //---------------
	wpis(60,0,0,0,28,2,0125252,k); //RFM
    wpis(60,0,0,0,9,2,020450,k);   //RC0
	wpis(60,0,0,0,17,2,020450,k);  //RC1	
   
	wpis(60,0,0,0,21,2,0xffff,k);  //RMI
    wpis(60,0,0,0,25,2,0xffff,k);  //RMP
	wpis(60,0,0,0,12,2,0,k);  //RWd  
  }//=================================================



   void sbroskmk(ntip *k)
   {UINT i,j;
      for(i=0;i<6;i++)
      {if(k->rtt[i]==0) continue;
	       for(j=5;j<=20;j++)
		   {wpis(100,i,0,0,j,2,0,k);}
          wpis(100,i,0,0,11,2,0377,k); //RM
		  rqn_rfm_pk(i,k);  kpr_rfm(k);
	  }
	  for(i=0;i<6;i++) k->wklt[i]=0; //Сброс призн.включения таймеров КМК.
   }//=================================================

   void sbroskww(UINT pornom,ntip *k) //pornom - номер KMK
   {UINT j;
     for(j=5;j<=20;j++) {wpis(100,pornom,0,0,j,2,0,k);}
                  wpis(100,pornom,0,0,11,2,0377,k); //RM
        k->wklt[pornom]=0; //Сброс призн.включения таймера КМК.
		rqn_rfm_pk(pornom,k);  kpr_rfm(k);
   }//=================================================



  void progrper(ntip *k)
  {                      // Начальный переустанов
               // только процессора.
	  //k->kom=0; //Выходы:
      k->jadro=0;  k->halt=0;
      k->config=0;                                                     
       //-------------------------------
	   k->cause=0;  k->status=0;                              
       k->status=(067377774&(k->status)); //установ разрядов
       k->status=(020000000|(k->status)); //регистра Status.
       k->pc=0xbfc00000-4;  //установ адреса обраб. Reset.
       k->predper=0;  k->zapusl=0; k->axtung=0;
	     	
  }//===========================================



          //Сбросы ММО и КУК

  void sbrosmmo(ntip *k)   //nm - мой номер ВМ.
{UINT i;
     //--- Узел выдачи:
       
     
  wpis(149,0,0,0,5,2,042020,k);       //RCM0
  wpis(149,0,0,0,6,2,0,k);         //RCW0
  wpis(149,0,0,0,7,2,0,k);         //RAc0
  wpis(149,0,0,0,8,2,0,k);         //RAf0
        //-----------------
    //---- Узлы приема:
             for(i=0;i<=2;i++)
	{wpis(150,i,0,0,5,2,042020,k);  //RCM
     wpis(150,i,0,0,6,2,0,k);       //RCW 
     wpis(150,i,0,0,7,2,0,k);       //RAc
	 wpis(150,i,0,0,8,2,0,k);       //RAf
	}//----------------
      //----- Таймер синхрониз.
	  wpis(153,0,0,0,5,2,0,k);       //RT
      wpis(153,0,0,0,6,2,0,k);       //RP
	  wpis(153,0,0,0,7,2,044252,k);  //RC
	  k->tq=0;
}//=========== Конец без вписывания номера ВМ  ========
  

  void sbroskuk(ntip *k)                  
 {int i;
	  k->kk0=0; k->kk1=0; k->kk2=0; k->kk3=0;
  k->tgot=0; k->tst=0; k->wklblk=0; k->wkldp=0;
  k->gt0=0; k->gt1=0; k->gt2=0; k->gt3=0;
   for(i=0;i<=5;i++)  k->wds[i]=0;

   k->wklmgr=1;

 }//==================================================


   //----- КОНЕЦ  ФУНКЦИЙ  -   СБРОСОВ --------



    //------------ ФУНКЦИИ      ММО ------------------ :


   void uzwid(UINT in,UINT it,char ss,UINT sisl,UINT *uk,ntip *k)
// Запись-чтение регистра узла выдачи ММО.
{UINT n,adr,*m,ind,*mm,r0,r1,rab; int nwm,i;   

   m=k->rw; adr=m[it];  ind=indpam(adr,k); if(ind==1)   
   {printkl("#F Фатальная ошибка СУБМ в uzwid."); return;}
       //---------------------------
    mm=k->mp; n=it-in; 

    nwm=0; for(i=1;i<=kolwm;i++) {if(bazuk[i]==k) {nwm=i; break;}}
    if(nwm==0) {printkl("uztsinxr. Сист.фатальная ошибка"); return;}
                 nwm=nwm-1;   //Номер по Беликову
       //-------------------------------------------
            if(n>3)
	{printkl("#F uswid - Фатальная ошибка СУБМ в номере регистра."); return;} 
	  //--------------------------------------------------
                 if(ss=='s')       //ЧТЕНИЕ с регистра в uk[0]
 {r0=mm[ind];    //Чисто прочитали регистр
      if(n==0)     //RCM0
	 {r1=(0167623&r0);
	           if(r1!=r0)
	  {printkl("# uswid - Ошибка в RCMO."); /*return;*/}
      uk[0]=r0;
     }//----------
  else if(n==1)   //RCW0
	{r1=0177777&r0;   
	           if(r1!=r0)
	  {printkl("# uswid - Ошибка в RCWO."); /*return;*/}
       uk[0]=r0;
	}//------------
  else if(n==2)   //RAc0  
	{r1=037774&r0;
	           if(r1!=r0)
	  {printkl("# uswid - Ошибка в RAc0."); /*return;*/}
       uk[0]=r0;
	}//------------
       else       //RAf0
	{r1=037774&r0;
	           if(r1!=r0)
	  {printkl("# uswid - Ошибка в RAf0."); /*return;*/}
       uk[0]=r0;
	}//------------

          return;

 }//------ конец чтения с регистра --------------------

     //----- Теперь запись sisl в регистр, в mm[ind].
				 
             i=15*nwm;      //Индекс начала 2-х пятерок в mag[].

        if((n==2)||(n==3))   //RAc0 или RAf0
	{rab=streg(149,0,0,0,5,k);  //RCM0
	   if(((0400&rab)!=0)&&(pesmmo!=0))       //Exe=1
	   {printkl("#I Запись на адресный регистр узла выдачи ММО при неоконченной выдаче");
	    soob.sprintf("# ВМ %d",nwm); printkl(soob);
	   }//--------------
       r1=037774&sisl; mm[ind]=r1; return;
    }//----------------------------------------

    else if(n==1)    //RCW0
    {mm[ind]=0177777&sisl; return;
	}//----------------------------------------

	else if((020&sisl)==0)   //RCM0 при off=0.   
    {
	//-------------------------------------------------------------------------- Коррекция от Сережи 11.11.04 :
	// off узла выдачи MMO переносится в RCM принимающих узлов в разряд SL.	
	{int i,j; UINT r,r1; ntip *kk; UINT nwid; //Белик.ном.выдающей ВМ
	 r=streg(149,0,0,0,5,k);  //RCMO узла выдачи
	 nwid=03&r;   //Белик.ном.выдающей ВМ
	          for(i=1;i<=kolwm;i++)
			  {if(Komplex[i]==0) continue;
		                 kk=bazuk[i];
                       for(j=0;j<3;j++)
					{r=streg(150,j,0,0,5,kk);  //RCM узла приема
                      r1=03&r;  //Куда подключен этот узел
					  if(r1!=nwid) continue;   //Этот узел не принимает
					  wpis(150,j,0,0,5,1,010,kk);  //Очистили SL=0
					}
              }
	}	
	//---------------------------------------------------------------------------- Конец этой коррекции	
		r0=mm[ind];   //Старое значение
	 r1=xitpr(r0,sisl,073420,3,0104200,0,037777600154,30); 
	   //r1 - новое значение, но помнит start.
	 mm[ind]=0167777&r1;   //Записали в регистр со стиранием start.

	      //-- Последствия по start:
      if((010000&r1)!=0)         //start=1
      {  if(pesmmo!=0)
		{soob.sprintf("#I Команда start  по ММО ВМ %d",nwm); printkl(soob);}

        if((mag[i]!=0)||(mag[i+5]!=0))  //Магистраль занята
        {mag[i+9]=nwm+1; //Будет позже выдана эта команда из ВМ  nwm+1 (мой номер) 
		if(pesmmo!=0) printkl("#I Магистраль занята.");
		}//----------------

        else   //Магистраль свободна
        { widkom(nwm);
		  if(pesmmo!=0) printkl("#I Выдача команды произведена.");
		}
	  }//------------------------------- Учли все по start при off=0.

	      //-- Последствия по Exe;

       if(((0400&r0)==0)&&((0400&r1)!=0))   //Exe=0 заменяется на Exe=1
	   {        if(pesmmo!=0)   
		{soob.sprintf("#I Команда Exe по ММО ВМ %d",nwm); printkl(soob);}
          //------------------------
	       if((mag[i]!=0)||(mag[i+5]!=0))     //Магистраль занята
        {mag[i+4]=nwm+1;  //Будет позже выдано информ.слово из ВМ  nwm+1 (мой номер)
          if(pesmmo!=0) printkl("#I Магистраль занята");
		}//-----------------------
           else                              //Магистраль свободна
		{widslowo(nwm); 
          if(konwid==0)  mag[i+4]=0;  //конец выдач
		  else           mag[i+4]=nwm+1;  //будет продолжение

		   if(pesmmo!=0) printkl("#I Выдача слова информации произведена");
		}//---------------------
	   }//---- конец Exe=0 заменяется на Exe=1 -----

      if(((0400&r0)!=0)&&((0400&r1)==0))   //Exe=1 заменяется на Exe=0
	  { if(pesmmo!=0)
		{soob.sprintf("#I Программный сброс Exe по ММО ВМ %d",nwm); printkl(soob);}
        mag[i+4]=0;   //Сбрасывание продолжения выдачи слов информации
	  }//--- конец  Exe=1 заменяется на Exe=0

     //---  Учет возможного изменения масок и линий регистра управления RCM0:
//                      prermmo(nwm);   //Учли

       //--- Влияние RCM0 на RC  таймера синхронизации ММО:
             if((0100000&r1)==0)       //Сброшено Qc  в  RCM0
              wpis(153,0,0,0,7,1,1,k);  //Очистили F10 в RC.
			    //-----------------------------
    }//---- конец записи на RCM0 при off=0.

	  else                    //Запись на RCM0 при off=1.
	  
    {
	//-------------------------------------------------------------------------- Коррекция от Сережи 11.11.04	
	{int i,j; UINT r,r1/*,rr*/; ntip *kk; /*CString s,ss;*/ UINT nwid; //Белик.ном.выдающей ВМ
	 r=streg(149,0,0,0,5,k);  //RCMO узла выдачи
	 nwid=03&r;   //Белик.ном.выдающей ВМ
	  if(nwid!=(UINT)nwm)
{printkl("#  uzwid. Алгоритмич.системная ошибка! ФАТАЛЬНО.");
 soob.sprintf("# Номер действующей ВМ = %d не равен NumVM =%d в узле выдачи",nwm,nwid);
                  printkl(soob);
}
	               //soob.sprintf("====================== Смотрим узел выдачи ВМ - %d",nwid); printkl(soob);
	          for(i=1;i<=kolwm;i++)
			  {if(Komplex[i]==0) {/*s.sprintf("BM%d обесточена",(i-1));printkl(s);*/ continue;}
		                 kk=bazuk[i];
                       for(j=0;j<3;j++)
					{r=streg(150,j,0,0,5,kk);  //RCM узла приема
					   //s.sprintf("--------- Исходный узел приема:   BM%d  RCM%d =%x",(i-1),(j+1),r); printkl(s);  
                      r1=03&r;  //Куда подключен этот узел
					  if(r1!=nwid) {/*printkl("Не принимает");*/   continue;}    //Этот узел не принимает
                         //-- Теперь этот  узел (i-1) -той ВМ принимает от ВМ nwid
					                  // printkl(" Принимает");
                               //   wpis(150,j,0,0,5,0,010,kk);
                      
                             if((i-1)!=(int)nwid)   
			{/*printkl("Номера ВМ различны, SL->1 в этом узле приема");*/	wpis(150,j,0,0,5,0,010,kk); } //Вписали SL=1
                             else
			{/*printkl("Номера ВМ равны, SL->0 в этом узле приема");*/ wpis(150,j,0,0,5,1,010,kk);} //Вписали SL=0

                       //rr=streg(150,j,0,0,5,kk);
						//ss.sprintf(" Результирующий узел приема:   BM%d  RCM%d =%x",(i-1),(j+1),rr); printkl(ss);	 
					}
              }
			    //printkl("************************** Конец с этим узлом выдачи");
	}	
	//---------------------------------------------------------------------------- Конец этой коррекции	
		  
		  
		  r0=mm[ind];   //Старое значение
	 r1=xitpr(r0,sisl,073420,3,0104200,0,037777600154,31); 
	   //r1 - новое значение, но помнит start.
	 mm[ind]=0167777&r1;   //Записали в регистр со стиранием start.

	      //-- Последствия по start:
      if((010000&r1)!=0)         //start=1
      {  if(pesmmo!=0)
		{soob.sprintf("#I Команда start  по ММО ВМ %d  при off=1",nwm); printkl(soob);}



	  }//------------------------------- Учли все по start при off=1.

	      //-- Последствия по Exe;

       if(((0400&r0)==0)&&((0400&r1)!=0))   //Exe=0 заменяется на Exe=1
	   {        if(pesmmo!=0)   
		{soob.sprintf("#I Команда Exe по ММО ВМ %d  при off=1.",nwm); printkl(soob);}
          //------------------------



	   }//---- конец Exe=0 заменяется на Exe=1 -----

      if(((0400&r0)!=0)&&((0400&r1)==0))   //Exe=1 заменяется на Exe=0
	  { if(pesmmo!=0)
	{soob.sprintf("#I Программный сброс Exe по ММО ВМ %d  при off=1.",nwm); printkl(soob);}



	  }//--- конец  Exe=1 заменяется на Exe=0

     //---  Учет возможного изменения масок и линий регистра управления RCM0:
  //                    prermmo(nwm);   //Учли

       //--- Влияние RCM0 на RC  таймера синхронизации ММО:
             if((0100000&r1)==0)       //Сброшено Qc  в  RCM0
              wpis(153,0,0,0,7,1,1,k);  //Очистили F10 в RC.
			    //-----------------------------
    //---- конец записи на RCM0 при off=1.

    //   printkl("#I Запись с последствиями на RCM0 при off=1 пока не отработана");
      }//-------------------------------


      // Учет возможного изменения масок и линий:

                      prermmo(nwm);   //Учли


                 return;   //Общее завершение

}//=============== Конец uzwid ================================


  void uzpriem(UINT in,UINT it,char ss,UINT sisl,UINT *uk,UINT pornom,ntip *k)
//Чтение-запись на регистр узла приема
{UINT n,adr,*m,ind,*mm,r0,r1; int i,nwm;   

   m=k->rw; adr=m[it];  ind=indpam(adr,k); if(ind==1)   
   {printkl("#F Фатальная ошибка СУБМ в uzpriem."); return;}
       //---------------------------
    mm=k->mp; n=it-in; 
            if(n>3)
	{printkl("#F - uzpriem - Фатальная ошибка СУБМ в номере регистра."); return;}
			
    nwm=0; for(i=1;i<=kolwm;i++) {if(bazuk[i]==k) {nwm=i; break;}}
    if(nwm==0) {printkl("uztsinxr. Сист.фатальная ошибка"); return;}
                 nwm=nwm-1;   //Номер по Беликову

	  //--------------------------------------------------
                 if(ss=='s')       //ЧТЕНИЕ с регистра в uk[0]
 {r0=mm[ind];    //Чисто прочитали регистр
      if(n==0)     //RCM-pornom
	 {r1=(0177633&r0);
	           if(r1!=r0)
	  {printkl("# uspriem - Ошибка в RCM."); /*return;*/}
      uk[0]=r0;
     }//----------
  else if(n==1)   //RCW-pornom
	{r1=0177777&r0;   
	           if(r1!=r0)
	  {printkl("# uspriem - Ошибка в RCW."); /*return;*/}
       uk[0]=r0;
	}//------------
  else if(n==2)   //RAc-pornom  
	{r1=037774&r0;
	           if(r1!=r0)
	  {printkl("# uspriem - Ошибка в RAc."); /*return;*/}
       uk[0]=r0;
	}//------------
       else       //RAf-pornom
	{r1=037774&r0;
	           if(r1!=r0)
	  {printkl("# uspriem - Ошибка в RAf."); /*return;*/}
       uk[0]=r0;
	}//------------

          return;

 }//------ конец чтения с регистра --------------------

     //----- Теперь запись sisl в регистр, в mm[ind].
				 
             r0=mm[ind];    //Старое значение регистра.

        if((n==2)||(n==3))   //RAc или RAf - pornom
	{
       r1=037774&sisl; mm[ind]=r1; return;
    }//----------------------------------------

    else if(n==1)    //RCW-pornom
    {/*mm[ind]=0177777&sisl;*/ return;        // КОРРЕКЦИЯ  - (БЕЗ ЗАПИСИ) ПО БЕЛИКОВУ ОТ 2.11.04г
	}//----------------------------------------

	else    //RCM-pornom 
    {
	 r1=xitpr(r0,sisl,063023,010,0114600,0,037777600144,32); 
	   //r1 - новое значение
	 mm[ind]=r1;   //Записали в регистр.


       //--- Влияние RCM-pornom на RC  таймера синхронизации ММО:
             if((0100000&r1)==0)       //Сброшено Qc  в  RCM-pornom
		{if(pornom==0)      wpis(153,0,0,0,7,1,4,k);     //Очистили F11 в RC.
         else if(pornom==1) wpis(153,0,0,0,7,1,020,k);   //Очистили F12 в RC.
		 else               wpis(153,0,0,0,7,1,0100,k);  //Очистили F13 в RC.		 
				 
		}	    //-----------------------------

    }//---- конец записи на RCM-pornom.

     //---  Учет возможного изменения масок и линий:
                      prermmo(nwm);   //Учли


                 return;   //Общее завершение

}//=============== Конец uzpriem ================================


   void uztsinxr(UINT in,UINT it,char ss,UINT sisl,UINT *uk,ntip *k)
//Чтение-запись на регистры таймера синхрониз.ММО
{UINT n,adr,*m,ind,*mm,r0,r1,rab; int nwm,i;   /*DWORDLONG t;*/  UINT rt;

   m=k->rw; adr=m[it];  ind=indpam(adr,k); if(ind==1)   
   {printkl("#F Фатальная ошибка СУБМ в uztsinxr."); return;}
       //---------------------------
    mm=k->mp; n=it-in;
	
    
    nwm=0; for(i=1;i<=kolwm;i++) {if(bazuk[i]==k) {nwm=i; break;}}
    if(nwm==0) {printkl("uztsinxr. Сист.фатальная ошибка"); return;}
                 nwm=nwm-1;   //Номер по Беликову

            if(n>2)
	{printkl("#F - uztsinxr - Фатальная ошибка СУБМ в номере регистра."); return;} 
      rab=streg(149,0,0,0,5,k);  //RCM0
      rab=(03&rab); i=(int)rab;
/*
	  if(i!=nwm)
	  {printkl("#F - uztsinxr - Фатальная ошибка СУБМ в номере ВМ"); return;}
	  //--------------------------------------------------
*/
                 if(ss=='s')       //ЧТЕНИЕ с регистра в uk[0]
	{r0=mm[ind];    //полное чтение
      if(n==0)            //RT
	  {        //чтение считающего счетчика RT
        rt=streg(153,0,0,0,5,k);

              uk[0]=rt;
	  }//-------------
	  else if(n==1)       //RP
	  {r1=0100377&r0;  if(r1!=r0)
		{printkl("# uztsinxr - Ошибка в RP"); /*return;*/}
        uk[0]=r0;
	  }//-------------
       else               //RC
	  {r1=0174377&r0;  if(r1!=r0)
        {printkl("# uztsinxr - Ошибка в RC"); /*return;*/}
        uk[0]=r0; 
      }//-------------
            return;
	}//---- конец чтения --------------------

        //---- Теперь запись sisl в mm[ind].

			r0=mm[ind];    //прямое прочтение
			
      if(n==0)       //RT
	  { }  //нет записи
	  //---------------------
	  else if(n==1)  //RP
      {r1=0100377&sisl;
       mm[ind]=r1; 
	  }//--------------------
	  else           //RC
      {r1=xitpr(r0,sisl,064252,0125,0110000,0,037777603400,33);
       mm[ind]=r1;
	      prermmo(nwm);
      }
           return;
}//=============== Конец uztsinxr =================================


   void anmag(void)  
//Анализ магистралей mag[]. Запускается в конце sag().
{int nm,i;  USHORT sl;
      for(nm=0;nm<kolwm;nm++)        //ЦИКЛ по магистралям    (не мои номера).
{//if (Komplex[nm+1]==0) continue;
		  i=15*nm;  //Начальный индекс в mag[].  nm - не мой номер
		  
 if((mag[i+5]!=0)&&(mag[i]!=0))
 {soob.sprintf("#F anmag - В магистрали %d две одновременн.выдачи. Фатально!",nm);
     printkl(soob);
per:    goto per;  
 }//-------------------------------------------------------------------------------


          if(mag[i+5]!=0)    //------------------------------- Команда в магистрали.
 {
  if(mag[i+8]>tsist)  continue;   //выдача команды не закончена
        //-- Теперь выдача закончена, надо раздать команду получателям.
    sl=(USHORT)mag[i+6]; //Код команды
	polkom(nm,sl);          //Раздали получателям в ВМ

	kuk(nm,sl);  //Раздали всем КУК-ам. 
    
    mag[i+5]=0;          //Освободили магистраль
	     //---------------------
	if(mag[i+9]!=0)        //есть продолжение по выдаче команды
	{widkom(nm);    //выдали в магистраль задержанную команду
     continue;
	}//---------------------------
           else if(mag[i+4]!=0)     //есть продолжение по выдаче слова
    {widslowo(nm); //выдали слово в магистраль
	 if(konwid==0)  mag[i+4]=0;
	 else           mag[i+4]=nm+1;
	   continue;
	}//-------------------------

 }//------------ Конец при наличии команды в магистрали -----------****************
         

		   if(mag[i]!=0)    //------------------------------ Слово в магистрали, 
 {
  if(mag[i+3]>tsist)    //выдача слова не закончена
     continue;
	//-------------------------------
	     //--- Теперь выдача слова закончена ---
	sl=(USHORT)mag[i+1];    //код слова
	polslowo(nm,sl);   //раздали получателям
	mag[i]=0;        //освободили магистраль
	//---------------------------------------
	       if(mag[i+9]!=0)     //есть продолжение по выдаче команды
    {widkom(nm);   //выдали команду в магистраль
	 continue;
	}//----------------------
      else if(mag[i+4]!=0)     //есть продолжение по выдаче слова
    {widslowo(nm); //выдали слово в магистраль
	 if(konwid==0)  mag[i+4]=0;
	 else           mag[i+4]=nm+1;
	 continue;
	}//-------------------------
 }//------------- Конец при наличии слова в магистрали -------------****************


       if((mag[i+5]==0)&&(mag[i]==0))  //-------- Нет ничего ---------------------------------
 {
         	if(mag[i+9]!=0)        //есть продолжение по выдаче команды
	{widkom(nm);    //выдали в магистраль задержанную команду
     continue;
	}//---------------------------

           else if(mag[i+4]!=0)     //есть продолжение по выдаче слова
    {widslowo(nm); //выдали слово в магистраль
	 if(konwid==0)  mag[i+4]=0;
	 else           mag[i+4]=nm+1;
	   continue;
	}//-------------------------
           else {}

 }//--------------- Конец нет  ничего ---------------------------------------******************

}  //-------------------------- конец  ЦИКЛА по магистралям   


}//================= конец anmag ================================



   void widkom(int nm)
//Выдает из ВМ-nm (беликовск) команду в магистраль
{ntip *k; int i; UINT r,*mm;
     k=bazuk[nm+1]; mm=k->mp;
	 r=mm[k->iuzwid[1]];    //RCW0 - nm, регистр выдаваемой команды
	 r=(0177777&r);
	 //-----------------------
	         i=15*nm;     //нач.индекс в mag[].
           if(Komplex[nm+1]==0)
   {soob.sprintf("# Попытка выдачи команды из отключенной ВМ%d",nm);
    printkl(soob); mag[i+9]=0; return;
   }//-----------------------------------------
     if((mag[i]!=0)||(mag[i+5]!=0))    //самоконтроль
     {soob.sprintf("#F Фатальная ошибка 1 в widkom в магистрали %d",nm);
	    printkl(soob); return;
	 }//----------------------------
         mag[i+5]=nm+1;
         mag[i+6]=(DWORDLONG)r;
		 mag[i+7]=tsist;
		 mag[i+8]=tsist+6000;
		 mag[i+9]=0;
		         if(pesmmo!=0)
		{soob.sprintf("#I Выдается команда %x - 16-тирич. в магистраль %d",r,nm);
				 printkl(soob);
         soob.sprintf("#I Время начала выдачи в мкс %lu",tsist/1000); printkl(soob);         
		}//------------------------

}//================ конец widkom ==========================

   
   void widslowo(int nm)
//Выдает из ВМ-nm (беликовск) слово в магистраль. Устанавливает konwid.
{ntip *k; int i; UINT r,*mm,adr,rac,raf,rcm,ind;
       k=bazuk[nm+1];  mm=k->mp;

   r=streg(52,0,0,0,6,k);           //RPN ПК     Вычисление mmobozu
   r=(03400&r); r=(r<<6); mmobozu=01774000000|r;

	   rcm=mm[k->iuzwid[0]];    //рег.управления RCM0 узла выдачи ВМ - nm.
	       if((0400&rcm)==0)   //Exe=0, нет выдачи информ. слова в магистраль.
			                   //Самоконтроль.
	{soob.sprintf("#I Фатальная ошибка 1 в магистрали %d  в widslowo",nm);
		   printkl(soob); return;
	}//------------------------------
        i=15*nm;
           if(Komplex[nm+1]==0)
   {soob.sprintf("# Попытка выдачи слова из отключенной ВМ%d",nm);
    printkl(soob); mag[i+4]=0; return;
   }//-----------------------------------------

      rac=mm[k->iuzwid[2]]; 
	  raf=mm[k->iuzwid[3]];
       adr=mmobozu|rac;       //физич.адрес слова в БОЗУ
	   rac=(rac+4);
	   mm[k->iuzwid[2]]=rac;   //инкрементировали
	    //--------------------------
	   if(rac>raf) konwid=0; else konwid=1;

	   //-- Теперь выдача слова:
	    ind=indpam(adr,k);  if(ind==1)
		{soob.sprintf("#I Фатальная ошибка 2 в магистрали %d  в widslowo",nm);
               return;
        }//-------------------------
             r=mm[ind]; r=(0177777&r);   //выдаваемое слово
                      
        mag[i]=nm+1;
		mag[i+1]=(DWORDLONG)r;
        mag[i+2]=tsist;
		mag[i+3]=tsist+6000;
        //--------------------
             if(pesmmo!=0)
		{soob.sprintf("#I Выдается слово %x  -16-тирич. в магистраль %d ",r,nm);

			 printkl(soob);
         soob.sprintf("#I Адрес слова 0x%x",adr);  printkl(soob);
         soob.sprintf("#I Время начала выдачи в мкс %lu",tsist/1000);                    
		              printkl(soob);
         if(konwid==0) printkl("# Выдача закончена");
		 else          printkl("# Выдача НЕ закончена");

		}//-----------

}//================ конец widslowo ==========================


  void polkom(int nm,USHORT sl)
//Передает команду sl всем получателям от ВМ nm (беликовский номер).
//Запускается в anmag.  
{ntip *k; int j; UINT rc,rct,*mm;
   int p;      p=0;                                                                  
       for(j=1;j<=kolwm;j++)         //Цикл по ВМ, j - мой номер
 {if(Komplex[j]==0) continue;
	   //-- Теперь ВМ включена.

     k=bazuk[j]; mm=k->mp;

	   //--- Смотрим 1 узел приема :
     rc=mm[k->iuzpr1[0]];           //RCM1
	 rc=(0177777&rc);
	     if(((03&rc)==(UINT)nm)&&((030&rc)==0))     //Учли и 0ff  и  SL
	         //Откликается.
	{p=1;                                                                             
			 if(pesmmo!=0)
	 {soob.sprintf("#I Принята команда в ВМ %d  в узле приема %d  ",j-1,1);
		 printkl(soob);
	  soob.sprintf("#I код принятой команды %x  - 16-тиричн. ",sl);
	     printkl(soob);
	 }//----------------------
     //--- Запись команды sl на приемный регистр узла приема
     mm[k->iuzpr1[1]]=(UINT)sl;
	      //----------------------
	 if((0100000&rc)!=0)    //Q уже есть
     {rc=(010000|rc);   //Erc
	  if(pesmmo!=0) printkl("#I Был прием команды при установленном Qc=1  !");
	 }//---------
	 rc=(0100000|rc);  //Qc

      rct=mm[k->its[2]];  //рег.упр. таймера синхр
	  if((0x4&rct)==0) prizqc[j-1]=1;   //Пометили в признаке наличие НОВОГО  Qc 
                                        //по нулевому F11 и Off1=0 (НЕ учитываемому!).
	                                    //Это нужно для послед.проверки реакции на Qc
//  --- Теперь надо отразить Qc в рег.упр. таймера синхрониз. ММО:

	  rct=(04|rct);      //F11  -  от 1 узла

	  mm[k->its[2]]=rct;  //вписали в рег.упр. тайм.синхрониз.
	        //----- Отразили ----.
	 //--- Теперь надо rc (с Qc) вписать в регистр управл.узла приема.
	    mm[k->iuzpr1[0]]=rc;  //вписали
	}//-------------------------------- конец по откликнувшемуся узлу приема 1


	   //--- Смотрим 2 узел приема :
     rc=mm[k->iuzpr2[0]];           //RCM2
	 rc=(0177777&rc);
	     if(((03&rc)==(UINT)nm)&&((030&rc)==0))     //Учли и 0ff  и  SL
	         //Откликается.
	{p=1;                                                 
			 if(pesmmo!=0)
	 {soob.sprintf("#I Принята команда в ВМ %d  в узле приема %d  ",j-1,2);
		 printkl(soob);
	  soob.sprintf("#I код принятой команды %x  - 16-тиричн. ",sl);
	     printkl(soob);
	 }//----------------------
     //--- Запись команды sl на приемный регистр узла приема
     mm[k->iuzpr2[1]]=(UINT)sl;
	      //----------------------
	 if((0100000&rc)!=0)    //Q уже есть
     {rc=(010000|rc);   //Erc
	  if(pesmmo!=0) printkl("#I Был прием команды при установленном Qc=1  !");
	 }//---------
	 rc=(0100000|rc);  //Qc

     rct=mm[k->its[2]];  //рег.упр.  таймера синхр
     if((0x10&rct)==0) prizqc[j-1]=1;   //Пометили в признаке наличие НОВОГО  Qc

	   //  --- Теперь надо отразить Qc в рег.упр. таймера синхрониз. ММО:
      
	  rct=(020|rct);      //F12  -  от 2 узла
	  mm[k->its[2]]=rct;  //вписали в рег.упр. тайм.синхрониз.
	        //----- Отразили ----.
	 //--- Теперь надо rc (с Qc) вписать в регистр управл.узла приема.
	    mm[k->iuzpr2[0]]=rc;  //вписали
	}//-------------------------------- конец по откликнувшемуся узлу приема 2


	   //--- Смотрим 3 узел приема :
     rc=mm[k->iuzpr3[0]];           //RCM3
	 rc=(0177777&rc);
	     if(((03&rc)==(UINT)nm)&&((030&rc)==0))     //Учли и 0ff  и  SL
	         //Откликается.
	{p=1;                                                              
			 if(pesmmo!=0)
	 {soob.sprintf("#I Принята команда в ВМ %d  в узле приема %d  ",j-1,3);
		 printkl(soob);
	  soob.sprintf("#I код принятой команды %x  - 16-тиричн. ",sl);
	     printkl(soob);
	 }//----------------------
     //--- Запись команды sl на приемный регистр узла приема
     mm[k->iuzpr3[1]]=(UINT)sl;
	      //----------------------
	 if((0100000&rc)!=0)    //Qc уже есть
     {rc=(010000|rc);   //Erc
	  if(pesmmo!=0) printkl("#I Был прием команды при установленном Qc=1  !");
	 }//---------
	 rc=(0100000|rc);  //Qc

     rct=mm[k->its[2]];  //рег.упр.  таймера синхр
     if((0x40&rct)==0) prizqc[j-1]=1;   //Пометили в признаке наличие НОВОГО  Qc

	   //  --- Теперь надо отразить Qc в рег.упр. таймера синхрониз. ММО:
	  rct=(0100|rct);      //F13  -  от 3 узла
	  mm[k->its[2]]=rct;  //вписали в рег.упр. тайм.синхрониз.
	        //----- Отразили ----.
	 //--- Теперь надо rc (с Qc) вписать в регистр управл.узла приема.
	    mm[k->iuzpr3[0]]=rc;  //вписали
	}//-------------------------------- конец по откликнувшемуся узлу приема 3

 }//--- Конец цикла по ВМ.    j - освободился.
  
	   if((p==0)&&(pesmmo!=0)) {soob.sprintf("# Никем не принята команда %x  от ВМ%d",sl,nm); printkl(soob);}             

   //--- Теперь работа с выдавшей команду ВМ - nm. Надо установить в ней Qc=1 в рег.RCM:
	   k=bazuk[nm+1]; mm=k->mp;
	   rc=mm[k->iuzwid[0]];  //RCM
	   rc=(0100000|rc);    //Qc
       mm[k->iuzwid[0]]=rc;     //установили

	  //--- Но надо еще отразить этот Qc в рег.упр.тайм.синхр.ММО: 
       rct=mm[k->its[2]];  //прочитали рег.упр.тайм.синхр.
       if((01&rct)==0) prizqc[nm]=1;  //Пометили в признаке наличие НОВОГО Qc
	           rct=(01|rct);
            mm[k->its[2]]=rct;     
			//---- отразили.

     //--- Теперь - учет прерываний по всем ВМ:
               for(j=1;j<=kolwm;j++)
	{if(Komplex[j]==0) continue;
			   prermmo(j-1);
	}//-------------------------------

}//============ Конец polkom ==========================


   void polslowo(int nm,USHORT sl)
//Передает слово информации sl получателям от ВМ nm (беликовский номер).
//Запускается в anmag.
{ntip *k; int i,j; UINT  *mm,r,rc,rac,raf,adr,ind,prizp; 
                                 //prizp - признак возможного прерывания (=1).
                prizp=0;
       for(j=1;j<=kolwm;j++)     //Цикл по ВМ,   j - мой номер.
 {if(Komplex[j]==0)  continue;
	   //-- Теперь ВМ включена.

	   k=bazuk[j]; mm=k->mp;

	   //---- Смотрим 1 узел:
         rc=mm[k->iuzpr1[0]]; rc=(0177777&rc);
     if(((03&rc)==(UINT)nm)&&((030&rc)==0))   //учли 0ff  и  SL
		                  //Откликается
	 {if(pesmmo!=0)
		{soob.sprintf("#I Поступило слово в ВМ %d  в узел приема %d ",j-1,1);
	             printkl(soob);
         soob.sprintf("#I Код слова %x 16-тиричн.",sl); printkl(soob);
		}//----------------------------------------
            if((04000&rc)!=0)    //Qi уже есть
		{if(pesmmo!=0) printkl("#I Qi=1. Отказ приема слова");
			rc=(0400|rc);  //Eri
		}//------ конец неприема слова.
			else      //слово принимается
		{rac=mm[k->iuzpr1[2]];
         raf=mm[k->iuzpr1[3]];
   r=streg(52,0,0,0,6,k);           //RPN ПК     Вычисление mmobozu
   r=(03400&r); r=(r<<6); mmobozu=01774000000|r;
         adr=mmobozu|rac;
		 rac=(rac+4);           //инкрементировали
         mm[k->iuzpr1[2]]=rac;
               if(rac>raf)
			   {rc=(04000|rc);    //Qi
                 prizp=1;
			   }//---------------------
          ind=indpam(adr,k);   if(ind==1)
		  {printkl("#F Фатальная ошибка 1 в polslowo"); return;}
		            //-------------
		  mm[ind]=(UINT)sl;  //приняли слово в БОЗУ
            if(pesmmo!=0) printkl("#I Слово принято");
		}//------- конец принятия слова --------------

			mm[k->iuzpr1[0]]=rc;   //вписали RCM узла приема

     }//----------------------- Конец по откликнувшемуся узлу приема 1


	   //---- Смотрим 2 узел:
         rc=mm[k->iuzpr2[0]]; rc=(0177777&rc);
     if(((03&rc)==(UINT)nm)&&((030&rc)==0))   //учли 0ff  и  SL
		                  //Откликается
	 {if(pesmmo!=0)
		{soob.sprintf("#I Поступило слово в ВМ %d  в узел приема %d ",j-1,2);
	             printkl(soob);
         soob.sprintf("#I Код слова %x 16-тиричн.",sl); printkl(soob);
		}//----------------------------------------
            if((04000&rc)!=0)    //Qi уже есть
		{if(pesmmo!=0) printkl("#I Qi=1. Отказ приема слова");
			rc=(0400|rc);  //Eri
		}//------ конец неприема слова.
			else      //слово принимается
		{rac=mm[k->iuzpr2[2]];
         raf=mm[k->iuzpr2[3]];
   r=streg(52,0,0,0,6,k);           //RPN ПК     Вычисление mmobozu
   r=(03400&r); r=(r<<6); mmobozu=01774000000|r;
         adr=mmobozu|rac;
		 rac=(rac+4);           //инкрементировали
         mm[k->iuzpr2[2]]=rac;
               if(rac>raf)
			   {rc=(04000|rc);    //Qi
                 prizp=1;
			   }//---------------------
          ind=indpam(adr,k);   if(ind==1)
		  {printkl("#F Фатальная ошибка 2 в polslowo"); return;}
		            //-------------
		  mm[ind]=(UINT)sl;  //приняли слово в БОЗУ
            if(pesmmo!=0) printkl("#I Слово принято");
		}//------- конец принятия слова --------------

			mm[k->iuzpr2[0]]=rc;   //вписали RCM узла приема

     }//----------------------- Конец по откликнувшемуся узлу приема 2


	   //---- Смотрим 3 узел:
         rc=mm[k->iuzpr3[0]]; rc=(0177777&rc);
     if(((03&rc)==(UINT)nm)&&((030&rc)==0))   //учли 0ff  и  SL
		                  //Откликается
	 {if(pesmmo!=0)
		{soob.sprintf("#I Поступило слово в ВМ %d  в узел приема %d ",j-1,3);
	             printkl(soob);
         soob.sprintf("#I Код слова %x 16-тиричн.",sl); printkl(soob);
		}//----------------------------------------
            if((04000&rc)!=0)    //Qi уже есть
		{if(pesmmo!=0) printkl("#I Qi=1. Отказ приема слова");
			rc=(0400|rc);  //Eri
		}//------ конец неприема слова.
			else      //слово принимается
		{rac=mm[k->iuzpr3[2]];
         raf=mm[k->iuzpr3[3]];
   r=streg(52,0,0,0,6,k);           //RPN ПК     Вычисление mmobozu
   r=(03400&r); r=(r<<6); mmobozu=01774000000|r;
         adr=mmobozu|rac;
		 rac=(rac+4);           //инкрементировали
         mm[k->iuzpr3[2]]=rac;
               if(rac>raf)
			   {rc=(04000|rc);    //Qi
                 prizp=1;
			   }//---------------------
          ind=indpam(adr,k);   if(ind==1)
		  {printkl("#F Фатальная ошибка 3 в polslowo"); return;}
		            //-------------
		  mm[ind]=(UINT)sl;  //приняли слово в БОЗУ
            if(pesmmo!=0) printkl("#I Слово принято");
		}//------- конец принятия слова --------------

			mm[k->iuzpr3[0]]=rc;   //вписали RCM узла приема

     }//----------------------- Конец по откликнувшемуся узлу приема 3

 }//-------- Конец цикла по ВМ.  j освободился. 

          //-------- Теперь работа с ВМ - nm,выдавшей слово: не конец ли выдачи,
		  //установление Exe и Qi в узле выдачи.  
                     i=15*nm;
    if(mag[i+4]==0)        //нет продолжения выдач слов
 {k=bazuk[nm+1];  mm=k->mp;
	rc=mm[k->iuzwid[0]];   //RCM0 ВМ - nm.

      if((0400&rc)!=0)    //есть Exe=1
	{rc=(04000|rc);    //Qi
     rc=(0177377&rc);  //сброс Exe
     mm[k->iuzwid[0]]=rc;   //вписали RCM0 - nm
	      prizp=1;
                     if(pesmmo!=0)
      {soob.sprintf("#I Узел выдачи ВМ %d полностью закончил выдачу слов.",nm);
	                    printkl(soob);
      }//------------------------------
	}//-- конец, когда  Exe=1

    else        // Exe=0
                if(pesmmo!=0)
    {soob.sprintf("#I Узел выдачи ВМ %d закончил выдачу слов по сбросу Exe.",nm);
                        printkl(soob);
    }//--- конец, когда Exe=0

 }//------ конец, когда нет продолжения выдачи слов

          //---- Теперь - учет прерываний:
	             if(prizp==0) return;
			for(j=1;j<=kolwm;j++)
	{if(Komplex[j]==0)   continue;
       prermmo(j-1);
	}//---------------

}//============= Конец polslowo =======================

   

	   void prermmo(int nm)
//Обеспечивает проведение прерываний от ММО полностью для ВМ - nm (беликовский номер).
//Смотрит все регистры управления, устанавливает F20 и F21 в ПК, далее - kpr_rfm(k).
{ntip *k; UINT rc,*mm; int f20,f21;
	   k=bazuk[nm+1];  mm=k->mp;
	     f20=0; f21=0;   //начальный установ
		     //-----------------------

    //---- Обработка регистра управления узла выдачи:
       rc=mm[k->iuzwid[0]];   //RCM0

  if(((0100000&rc)!=0)&&((040000&rc)==0))   //Есть прерывание по Qc
  { if((020000&rc)==0)   //Nc=0
     f20=1;
    else
     f21=1;
  }//---------- закончили с Qc

  if(((04000&rc)!=0)&&((02000&rc)==0))   //Есть прерывание по Qi
  { if((010000&rc)==0)   //Ni=0
     f20=1;
    else
     f21=1;
  }//---------- закончили с Qi
          //-------------------- Закончили с узлом выдачи ---------
  
     //---- Обработка регистра управления узла приема  1:
       rc=mm[k->iuzpr1[0]];   //RCM1

  if(((0100000&rc)!=0)&&((040000&rc)==0))   //Есть прерывание по Qc
  { if((020000&rc)==0)   //Nc=0
     f20=1;
    else
     f21=1;
  }//---------- закончили с Qc

  if(((04000&rc)!=0)&&((02000&rc)==0))   //Есть прерывание по Qi
  { if((010000&rc)==0)   //Ni=0
     f20=1;
    else
     f21=1;
  }//---------- закончили с Qi
          //-------------------- Закончили с узлом приема 1  ---------

     //---- Обработка регистра управления узла приема  2:
       rc=mm[k->iuzpr2[0]];   //RCM2

  if(((0100000&rc)!=0)&&((040000&rc)==0))   //Есть прерывание по Qc
  { if((020000&rc)==0)   //Nc=0
     f20=1;
    else
     f21=1;
  }//---------- закончили с Qc

  if(((04000&rc)!=0)&&((02000&rc)==0))   //Есть прерывание по Qi
  { if((010000&rc)==0)   //Ni=0
     f20=1;
    else
     f21=1;
  }//---------- закончили с Qi
          //-------------------- Закончили с узлом приема 2  ---------

     //---- Обработка регистра управления узла приема  3:
       rc=mm[k->iuzpr3[0]];   //RCM3

  if(((0100000&rc)!=0)&&((040000&rc)==0))   //Есть прерывание по Qc
  { if((020000&rc)==0)   //Nc=0
     f20=1;
    else
     f21=1;
  }//---------- закончили с Qc

  if(((04000&rc)!=0)&&((02000&rc)==0))   //Есть прерывание по Qi
  { if((010000&rc)==0)   //Ni=0
     f20=1;
    else
     f21=1;
  }//---------- закончили с Qi
          //-------------------- Закончили с узлом приема 3  ---------

   //-------------   Закончили с узлами приема -----------------------

          //---- Обработка рег.управления таймера синхр. ММО:
              rc=mm[k->its[2]];   //RC

  if(((0100000&rc)!=0)&&((040000&rc)==0))   //Есть прерывание по Qc
  { if((020000&rc)==0)   //Nc=0
     f20=1;
    else
     f21=1;
  }//---------- закончили с таймером синхрониз. -----------
    
        //----- Теперь вписывание готовых  F20 и F21   в   RFM-ПК :
   if(f20==0)    wpis(52,0,0,0,5,1,0400,k);
   else          wpis(52,0,0,0,5,0,0400,k); 

   if(f21==0)    wpis(52,0,0,0,5,1,02000,k);
   else          wpis(52,0,0,0,5,0,02000,k); 
              //----------------------- готово

            kpr_rfm(k);   //Отразили в  KPR, на самом верху.

}//============ Конец  prermmo ========================= 

     void srabtsmmo(void)
//Устанавливает Q во всех ТС ММО, если этот факт происходит.
//Запускается в конце sag().
{int i;
	 for(i=0;i<4;i++)  //Цикл по беликовским номерам ВМ
 {if(Komplex[i+1]==0) continue;
	  Qts(i);     
 }//--- Конец цикла -----

}//=========== Конец srabtsmmo ==========================
	 
    void Qts(int nm)   
//Устанавливает Q в ТС  ВМ-nm (беликовск), если этот факт происходит.
//Запускается в srabtsmmo.
// tsmmot[nm] - время будущего срабатывания ТС. k->tq - время возникновения обобщ. признака
//синхронизации   - Q
//Если tsmmot[nm]=0, то это значит, что таймер Тс не запущен.
{UINT rc,rp,*mm; int i,j,kolr,kolp; UINT razr,kp; DWORDLONG rab,rab1; ntip *k;
   int dd; int zapusk;

                  k=bazuk[nm+1]; mm=k->mp;
                 rc=mm[k->its[2]];   //вытащили RC ТC
             if((0x8000&rc)!=0)   return;  //Вышли при Q=1 При Q=1 ничего не делается

    if(tsmmot[nm]!=0) zapusk=1; else zapusk=0; //Признак запуска ТС ранее (RT уже действует).


    if((zapusk==1)&&(tsmmot[nm]<=tsist))  //Q не появился, запуск был,
		//но RT дал истекшее время.  Нужно формирование   Q  с  Er
	{
                     if(pesmmo!=0)
{soob.sprintf
  ("# .Срабат. ТС по истекшему времени  ЭВМ%d при t=%lu в мкс. 1 разреш.",nm,tsist/1000); 
	                    printkl(soob);
}//------------------------------

     rc=mm[k->its[2]];   //RC ТC
     rc=(0110000|rc);    //вписали  Q и Er 
     mm[k->its[2]]=rc;   //перезаписали RC
	 k->tq=tsist; mm[k->its[0]]=0;  // сбросили RT
     tsmmot[nm]=0;  //Снятие признака запуска Tc
	  //---------------
        prermmo(nm);  //отметили  прерывание
             return;
	}//-------------------------------------------- end срабатыв. ТС по истекшему времени 

    
   //********* Теперь .Qc есть  или могут быть  в RC  Tc в разрядах F10 - F13  ********

	         if((0x55&rc)==0)     return;  //Нет ничего в F10-F13,  ничего не произойдет		 

			 //--------------------- Теперь что-то там есть, а Q  нет.

                       razr=0;   //будет фиксировать разрешенные признаки синхронизации
                       kolr=0;  //колво разрешенных

          //пометим в razr разрешенные признаки позиционно на местах F10 - F13:
	 if((02&rc)==0)   {kolr=(kolr+1); razr=1|razr;}
     if((010&rc)==0)  {kolr=(kolr+1); razr=4|razr;}
     if((040&rc)==0)  {kolr=(kolr+1); razr=0x10|razr;}
     if((0200&rc)==0) {kolr=(kolr+1); razr=0x40|razr;}

          if(kolr==0)  return;   //ничего не может произойти, нет разрешенных  вообще.
          //-----------------------------------------------------------------------------------


     else if(kolr==1)                                    //ОДИН КАНАЛ РАЗРЕШЕН:
 {	 if((razr&rc)!=0)  //Пришел разрешенный Qc. Сразу срабатывание Q 
	{		 
                     if(pesmmo!=0)
		{soob.sprintf("# Штатн.срабат. ТС  ЭВМ%d при t=%lu в мкс. 1 разреш.",nm,tsist/1000); 
	                    printkl(soob);
		}//------------------------------
	                         
		 rc=(0100000|rc);  //   Q
         mm[k->its[0]]=0;  // сбросили RT
		 mm[k->its[2]]=rc; //перезаписали RC  
		  tsmmot[nm]=0; //сбросили активность  ТС
          k->tq=tsist; 
          //--------------------
                     if(pesmmo!=0)
		{soob.sprintf("# Штатн.срабат. ТС  Q=1 ЭВМ%d при t=%lu в мкс. 1 разр.",nm,tsist/1000); 
	                    printkl(soob);
		}//------------------------------

          prermmo(nm);
		               return;
	}//------------------------------- сразу выработался Q без запуска ТС.
	  else return;   //ничего не может произойти, пришел неразрешенный Qc.
 }//-------------------------------------------------------------------- КОНЕЦ С ОДНИМ РАЗРЕШЕННЫМ КАНАЛОМ
			  

     else if(kolr==2)                                    //ДВА КАНАЛА РАЗРЕШЕНЫ:
     {kolp=0; j=1;    //Найдем кол-во разрешенных пришедших:  
	   kp=razr&rc; for(i=1;i<5;i++) {if((j&kp)!=0)  kolp=kolp+1; j=j<<2;} 	 
			     //----------------------------
           if(kolp==0)  return; //не пришло ни одного разрешенного Qc
		    //---------------------------------
      else if(kolp==1)          //только один пришел разрешенный Qc. ТС может запуститься.
		{if(zapusk==1)  return;             //запуск уже был,не запускай повторно и глупо
		  rp=mm[k->its[1]]; //RP  ТС
           dd=0377&rp; dd=dd+1;                                
		  if((0100000&rp)==0)        //Tic=0
		  {rab=0; rab1=4000; for(i=1;i<=dd;i++) rab=rab+rab1;  }	
		  else  {rab=0; rab1=8192000; for(i=1;i<=dd;i++) rab=rab+rab1;  }  //Tic=1         
          tsmmot[nm]=rab+tsist;   //вписали  время срабатывания ТС
		     
                     if(pesmmo!=0)
					{soob.sprintf("#I Запуск ТС ВМ%d при t=%lu в мкс. 2 разрешенных. Один Qc",
			               nm,tsist/1000);          
	                    printkl(soob);
                     soob.sprintf("# Время ожидания срабатывания ТС =%lu в мкс",tsmmot[nm]/1000); printkl(soob);    
					}//------------------------------
		                             return;
		}//-------------------------- конец с одним пришедшим разрешенным
         else          //пришли сразу два разрешенных Qc
		{//Сразу срабатывание ТС:
	      rc=(0100000|rc);    //Q
		  mm[k->its[0]]=0;  // и сбросили RT
          mm[k->its[2]]=rc;   //перезаписали RC
		  tsmmot[nm]=0;    //сбросили активность ТС
          k->tq=tsist;
	      //---------------
                     if(pesmmo!=0)
		{soob.sprintf("# Штатн.срабат. ТС  ЭВМ%d при t=%lu в мкс. 2 разреш.",nm,tsist/1000); 
	                    printkl(soob);
		}//------------------------------

                prermmo(nm);  //отметили  прерывание
                   return;
		}//-- конец при двух пришедших разрешенных Qc
     }//------------------------------------------------------------------ КОНЕЦ ПРИ ДВУХ РАЗРЕШЕННЫХ КАНАЛАХ 


      else if(kolr==3)                                      //РАЗРЕШЕНЫ 3 КАНАЛА:
	{kolp=0; j=1;    //Найдем кол-во разрешенных пришедших:
	  kp=razr&rc; for(i=1;i<5;i++) {if((j&kp)!=0)  kolp=kolp+1; j=j<<2;}
			 //кол-во пришедших разрешенных
			     //----------------------------

           if(kolp<2)  return; //не пришло нужного числа разрешенных Qc
		   //--------------------------------------------------------------
      else if(kolp==2)          //пришло два разрешенных Qc. ТС запускается.    
		{if(zapusk==1)  return;           //запуск уже был,не запускай повторно и глупо
		  rp=mm[k->its[1]]; //RP  ТС
           dd=0377&rp; dd=dd+1;                                
		  if((0100000&rp)==0)        //Tic=0
		  {rab=0; rab1=4000; for(i=1;i<=dd;i++) rab=rab+rab1;  }	
		  else  {rab=0; rab1=8192000; for(i=1;i<=dd;i++) rab=rab+rab1;}  //Tic=1                 
          tsmmot[nm]=rab+tsist;   //вписали время срабатывания ТС
		     //Время запуска ТС

                     if(pesmmo!=0)
{soob.sprintf("#I Запуск ТС ВМ%d при t=%lu в мкс. 3 разрешенных. Два Qc",nm,tsist/1000);			                                                                    
	                               printkl(soob);
   soob.sprintf("# Время ожидания срабатывания ТС =%lu в мкс",tsmmot[nm]/1000); printkl(soob);     
}//------------------------------
		         return;
		}//-------------------- конец с двумя пришедшими разрешенными Qc

         else          //пришли сразу 3 или 4 разрешенных Qc
		{//Сразу срабатывание ТС:
	      rc=(0100000|rc);    //Q
		   mm[k->its[0]]=0;  //и сбросили RT
          mm[k->its[2]]=rc;   //перезаписали RC
		  tsmmot[nm]=0;    //сбросили активность ТС
		  k->tq=tsist;
	      //---------------
          prermmo(nm);  //отметили  прерывание

                     if(pesmmo!=0)
{soob.sprintf("#I Штатн.срабат. ТС ВМ%d при t=%lu в мкс. 3 разрешенных.",nm,tsist/1000);
	                    printkl(soob);      
}//------------------------------
             return;
		}//-- конец при 3  пришедших разрешенных Qc

	}//------------------------------------------------------------------ КОНЕЦ ПРИ 3-Х РАЗРЕШЕННЫХ КАНАЛАХ


      else                                       //РАЗРЕШЕНЫ 4 КАНАЛА
	{kolp=0; j=1;  //Найдем кол-во разрешенных пришедших:
	  kp=razr&rc; for(i=1;i<5;i++) {if((j&kp)!=0)  kolp=kolp+1; j=j<<2;}
			 //кол-во пришедших разрешенных
			     //----------------------------
           if(kolp<2)  return; //не пришло нужного числа разрешенных Qc
		   //--------------------------------------------------------------
      else if(kolp==2)          //пришло два разрешенных Qc. ТС запускается.    
		{if(zapusk==1)  return;             //запуск уже был,не запускай повторно и глупо
		  rp=mm[k->its[1]]; //RP  ТС
           dd=0377&rp; dd=dd+1;                                
		  if((0100000&rp)==0)        //Tic=0
		  {rab=0; rab1=4000; for(i=1;i<=dd;i++) rab=rab+rab1;  }	
		  else  {rab=0; rab1=8192000; for(i=1;i<=dd;i++) rab=rab+rab1;}  //Tic=1                  
          tsmmot[nm]=rab+tsist;   //вписали время срабатывания ТС
		  
                     if(pesmmo!=0)
{soob.sprintf("#I Запуск ТС ВМ%d при t=%lu в мкс. 4 разрешенных. Два Qc",			                                     nm,tsist/1000);           
	                    printkl(soob);
  soob.sprintf("# Время ожидания срабатывания ТС =%lu в мкс",tsmmot[nm]/1000); printkl(soob);    
}//------------------------------

		  return;
		}//-- конец с двумя пришедшими разрешенными Qc
         else          //пришли сразу  3 или 4 разрешенных Qc
		{//Сразу срабатывание ТС:
	      rc=(0100000|rc);    //Q
		   mm[k->its[0]]=0;  // и сбросили RT
          mm[k->its[2]]=rc;   //перезаписали RC
		  tsmmot[nm]=0;
		  k->tq=tsist;
	      //---------------
          prermmo(nm);  //отметили  прерывание
                     if(pesmmo!=0)
{soob.sprintf("#I Штатн.срабат. ТС ВМ%d при t=%lu в мкс. 4 разрешенных.",nm,tsist/1000);
	                    printkl(soob);      
}//------------------------------
             return;
		}//-- конец при  4 пришедших разрешенных Qc

	}//---------------------------------------- КОНЕЦ ПРИ 4-Х РАЗРЕШЕННЫХ КАНАЛА
}//============= Конец Qts ============================== 



   // ------------------------------ ФУНКЦИИ   КУК ----------------------:

     void kuk(int nm,USHORT sl)   
 //Раздает команду sl из магистрали nm всем КУК-ам в их БПК (nm - белик.ном.ВМ, ее магистр.)
 //и формирует признаки готовности команды.
 //nm  определяет номер БВК в КУК-е, куда шлепается информация.
 // kki - код принятой команды,  gti - признак ее готовности (i - здесь номер магистрали,
 //.т.е. белик.ном.ВМ, из которой поступила команда).  
 //Запускается в anmag().
 {int i; ntip *k;
	 
	 if((0140000&sl)!=0140000) return; //команда не для КУК-а.
	 if((nm+1)>kolwm)
     {printkl("# Фатально! Ошибка в номере магистрали в kuk().");     }
	 //---------------------------------------
	        for(i=1;i<=kolwm;i++)     //Цикл по всем ВМ - их КУКам
	{ k=bazuk[i];                        
                if(nm==0)
				{   k->kk0=sl;      
		            k->gt0=1; //признак готовности команды
				}//-----
           else if(nm==1)
				{   k->kk1=sl;
		            k->gt1=1;
				}//-----
           else if(nm==2)
				{   k->kk2=sl;
		            k->gt2=1;
				}//-----
           else if(nm==3)
				{   k->kk3=sl;
		            k->gt3=1;
				}//-----
      else {printkl("#  kuk. Ошибка номера магистрали!");}
	}//----------------------------------------------------- end for
            
 }//===================== конец kuk() =====================================



   void komkuk(int nom)   //nom - мой номер ВМ.
 //Оценивает готовности команд в БПКi и выдает из БВК команду на исполнение.
 //Учитывает наличие или отсутствие мажоритирования.
 //Запускается по всем ВМ после anmag(), в которой  kuk() уже раздал команды в БПКi.
 {ntip *k; int p,kol; UINT ts; USHORT zprmgr; QString ss; USHORT sl,est;  int pp;
    
                  k=bazuk[nom];  //Работаем только с этой ВМ

   if(k->tgot!=0)
   {   
	   return;  //Нельзя, не закончилось время задержки от мажорирования!
   }//-------------------------------------------------

   if((k->gt0==0)&&(k->gt1==0)&&(k->gt2==0)&&(k->gt3==0))
   {
	   return; //нет готовых команд.
   }//--------------------------------------------------

           
/* //----------------------------------------------------------------  //обдумать это изъятие!   
   if((Komplex[nom]==0)&&(k->wkldp==0)&&(k->tst==0))
   {if(peskuk!=0) {soob.sprintf("# Выдана команда на выключенный КУК ВМ-%d. Не выполняется!"
                  ,(nom-1));printkl(soob);}
	   return;
   } //нет никакого питания
   //Условие (k->tst==0) связано с поддержкой деж.питания при работе сторожевого таймера. 
           //------- Теперь этот КУК работает.
*///-------------------------------------------------------------------------------------------


               if(k->wklmgr==0)   //ОТСУТСТВИЕ  МАЖОРИТИРОВАНИЯ      //Берется 1 команда,
   {  pp=0;                                                          //первая при просмотре.
                 NETmag=1;	//нет мажоритир.		   
		 if((k->gt0==1)) {sl=k->kk0; bik(nom,sl); k->gt0=0; pp=1;}     
	else if((k->gt1==1)) {sl=k->kk1; bik(nom,sl); k->gt1=0; pp=1;} 
	else if((k->gt2==1)) {sl=k->kk2; bik(nom,sl); k->gt2=0; pp=1;}
    else if((k->gt3==1)) {sl=k->kk3; bik(nom,sl); k->gt3=0; pp=1;}
	else  {}     

                   if((pp==1)&&(peskuk!=0)) 
{soob.sprintf("# komkuk. Исполнение без мажоритир.команды %x в ВМ%d",sl,nom-1);printkl(soob);}
    //----------------- закончили с выдачей одной команды на исполнение.
  }//------------- Конец отсутствия мажоритирования ------------------------


             else  //МАЖОРИТИРОВАНИЕ   - k->wklmgr!=0  
 {
				 NETmag=0;    //есть мажоритир.

//	if((peskuk!=0) && (ekran[nom]==0))
//	{soob.sprintf("# Попытка мажоритирования в ВМ%d",nom-1); printkl(soob);}
	
	 //-- Сначала особый спецанализ выключения мажоритирования:
  //ts=streg(153,0,0,0,7,k);  //RC таймера синхронизации
  //         if(((04000&ts)==0)&&((010000&ts)!=0)) 
	 // zprmgr=1; else zprmgr=0; //для управл.выключением
  //
				 zprmgr = 1;

                     est=0;
                     kol=0;
	if(((k->kk0&0xf01f)==0xc005)&&(k->gt0==1))     
	 {est=1; sl=k->kk0; kol=kol+1;}
	
	if(((k->kk1&0xf01f)==0xc005)&&(k->gt1==1))
	{est=1; sl=k->kk1; kol=kol+1;}

	if(((k->kk2&0xf01f)==0xc005)&&(k->gt2==1))
	{est=1; sl=k->kk2; kol=kol+1;}
	
	if(((k->kk3&0xf01f)==0xc005)&&(k->gt3==1))
	{est=1; sl=k->kk3; kol=kol+1;}
	    //--- kol это количество команд сброса мажорит. для данной ВМ
	
               if(est==1)   //Команда сброса мажоритирования 
	{  
        if(zprmgr==1)          //******************
          {           if(peskuk!=0)             
			{soob.sprintf("# Выключение мажоритирования по особому условию в ВМ%d",nom-1);
	                   printkl(soob);
			}//----------------
			bik(nom,sl);
			k->tgot=tsist+240000;   //Для будущего сброса готовности.			
			        return;
		  }//*************************************** end   zprmgr==1
		else    //zprmgr==0
		{if(kol>=2)
			{         if(peskuk!=0)           
				{soob.sprintf("# Мажоритир. выключение мажоритирования  в ВМ%d",nom-1);
	                   printkl(soob);
				}//----------------
			  bik(nom,sl);
		      k->tgot=tsist+240000;   //Для будущего сброса готовности.
                    return;

			}//------------- end kol>=2 
              else return;
		}//----------------------------- end zprmgr==0
	}//------------------------------- end Команды сброса мажоритирования 


	//---- Теперь мажоритирование команды. Неособое, обычное.


            //--- Попарное сравнение команд:
                          p=0;

	      if(kolwm==2)                           //2 BM 
		  {if((k->kk0==k->kk1)&&(k->gt0==1)&&(k->gt1==1))
					{p=1; sl=k->kk0;}                                  
		  }//--------------------------------------     
		  
     else if(kolwm==3)                           //3 BM
	 {    if((k->kk0==k->kk1)&&(k->gt0==1)&&(k->gt1==1)) 
					{p=1; sl=k->kk0;}
	 else if((k->kk0==k->kk2)&&(k->gt0==1)&&(k->gt2==1)) 
				{p=1; sl=k->kk0;} 
	 else if((k->kk1==k->kk2)&&(k->gt1==1)&&(k->gt2==1))
				{p=1; sl=k->kk1;} 
	}//---------------------------------------------
	 
	 else if(kolwm==4)                            //4 BM
	 {    if((k->kk0==k->kk1)&&(k->gt0==1)&&(k->gt1==1))
					{p=1; sl=k->kk0;}
	 else if((k->kk0==k->kk2)&&(k->gt0==1)&&(k->gt2==1))
					{p=1; sl=k->kk0;}
	 else if((k->kk0==k->kk3)&&(k->gt0==1)&&(k->gt3==1))
					{p=1; sl=k->kk0;}
	 else if((k->kk1==k->kk2)&&(k->gt1==1)&&(k->gt2==1))
					{p=1; sl=k->kk1;}
     else if((k->kk1==k->kk3)&&(k->gt1==1)&&(k->gt3==1)) 
					{p=1; sl=k->kk1;}
	 else if((k->kk2==k->kk3)&&(k->gt2==1)&&(k->gt3==1))
					{p=1; sl=k->kk2;}
	}//--------------------------------------------

	 else {}

	 if(p==0) 
{if((peskuk!=0)&& (ekran[nom]==0))                
 {soob.sprintf("# Попытка мажоритирования неуспешна в ВМ%d",nom-1);printkl(soob);}
	 ekran[nom]=1;
	 return; 
}//--------------------------------------------------------- //Не совпадают команды
		 
     //---- Теперь 2 команды совпали. Надо команду исполнять.
        if(peskuk!=0)               
{soob.sprintf("# Попытка мажоритирования успешна в ВМ%d",nom-1); printkl(soob);
 soob.sprintf("Это команда %x",sl); printkl(soob);
}//-----------------------------------------------
                           bik(nom,sl);   //исполнение
           k->tgot=tsist+240000;   //Для будущего сброса готовности.
						   ekran[nom]=0;

	 }//-------------- Конец мажоритирования.

}//======================== конец komkuk() ================


   void bik(int nom,USHORT sl)  
 //исполняет команду sl в ВМ nom (мой номер)
 //запускается в komkuk().
 {USHORT nwm,ko,r78;  ntip *k; DWORDLONG rab,rab1; QString soob1,soob2, s; int i;
               
   if(peskuk!=0)
   {   if(NETmag==1)
	   
	{   
	 s.sprintf("# Исполняется НЕМАЖОРИТ. команда %x (16-тир) в КУК ВМ-%d ",sl,(nom-1));
     printkl(s);
	}//----------------
       else
	{   
	 s.sprintf("# Исполняется МАЖОРИТ. команда %x (16-тир) в КУК ВМ-%d ",sl,(nom-1));
     printkl(s);
	}//----------------
   }//-------------------------------------------

   
         ko=037&sl; nwm=01400&sl; nwm=(nwm>>8);
		    r78=030000&sl; r78=(r78>>12);
                  k=bazuk[nom];
   //---------------------------------------------

      if((r78==0)&&(ko==022)&&(nwm==(nom-1)))          //ВКЛДП0  уст.в 1
	{if(k->wklblk==1)
	  {k->wkldp=(0177776&(k->wkldp));
	    if(peskuk!=0) printkl("# ВКЛДП0=0,т.к.ВКЛБЛК=1");
	  }//-------------
      else
	  {
		int i; ntip *kk; for(i=1;i<=4;i++)
		{kk=bazuk[i];    
          //kk->wkldp=(01|(kk->wkldp)); //Селезнев: выбросить!
		}//-------------------------------- 
		  k->wkldp=(01|(k->wkldp));   //А у себя включить!
	   if(peskuk!=0) printkl("# ВКЛДП0=1");
	  }//--------------
          return;
	 }//------------------------------------------------

  else if((r78==0)&&(ko==02))                          //ВКЛДП0  уст.в 0
  {k->wkldp=(0177776&(k->wkldp)); 
    if(peskuk!=0) printkl("# ВКЛДП0=0");
    return;
  }//--------------------------------------------------

  else if((r78==0)&&(ko==022)&&(nwm!=(nom-1))) 
  {k->wkldp=(0177776&(k->wkldp)); 
    if(peskuk!=0) printkl("# ВКЛДП0=0");
    return;
  }
  //-------------------------------------------------

   else if((r78==0)&&(ko==023)&&(nwm==(nom-1)))      //ВКЛДП1   уст. в 1 
  {if(k->wklblk==1)
   {k->wkldp=(0177775&(k->wkldp));
           if(peskuk!=0) printkl("# ВКЛДП1=0,т.к.ВКЛБЛК=1");
   }//---------------------
      else
	  {
       int i; ntip *kk; for(i=1;i<=4;i++)
		{kk=bazuk[i];    
          //kk->wkldp=(02|(kk->wkldp));  //Селезнев: выбросить!
		}//----------------------------------- 
		  k->wkldp=(02|(k->wkldp));
           if(peskuk!=0) printkl("# ВКЛДП1=1");
	  }//-----------------
          return;
  }//------------------------------------------------

   else if((r78==0)&&(ko==03))                          //ВКЛДП1  уст.в 0
  {k->wkldp=(0177775&(k->wkldp));
    if(peskuk!=0) printkl("# ВКЛДП1=0");
    return;
  }//---------------------------------------

  else if((r78==0)&&(ko==023)&&(nwm!=(nom-1))) 
  {k->wkldp=(0177775&(k->wkldp)); 
    if(peskuk!=0) printkl("# ВКЛДП1=0");
    return;
  }
  //-------------------------------------------------

     else if((r78==0)&&(ko==024)&&(nwm==(nom-1)))          //ВКЛБЛК  уст.в 1
	 {k->wklblk=1;
	    if(peskuk!=0) printkl("# ВКЛБЛК=1");
		return;
	 }//------------------------------------------

     else if((r78==0)&&(ko==04)&&(nwm==(nom-1)))          //ВКЛБЛК  уст.в 0
	 {k->wklblk=0;
	    if(peskuk!=0) printkl("# ВКЛБЛК=0");
	  return;
	 }
  //---------------------------------------------------

	  //---- Теперь ответственное дело - ВКЛМЖР:

   else if((r78==0)&&(ko==05)&&(nwm==(nom-1)))     //сброс ВКЛМЖР
   {if(peskuk!=0) 
	{soob.sprintf("# Сброс режима мажоритирования в ВМ-%d",(nom-1));printkl(soob);}
                if(peskuk!=0)
   {
                   if(NETmag==1)
			{printkl("# Сброс мажоритирования при его отсутствия! Что бы это значило!");
				   return;
			}
   }//-------------------------------------------------
				k->wklmgr=0;
   }//----------------------------------- Конец сброва ВКЛМЖР
   

   else if((r78==0)&&(ko==025)&&(nwm==(nom-1)))   //установ в 1 ВКЛМЖР
   {if(peskuk!=0)                
	{soob.sprintf("# Включение режима мажоритирования в ВМ-%d",(nom-1));printkl(soob);}
	   k->wklmgr=1; return;
   }
   //--------------------- Конец с вкл.- выкл. мажоритирования -------------


   else if((r78==0)&&(ko==021)&&(nwm==(nom-1)))   //СВКЛ  - включение основного питания.
   {Komplex[nom]=1; //включили
    nasreset(nom);
	 if(peskuk!=0)                              
	{soob.sprintf("# Включение КУК-ом основного питания в ВМ-%d",(nom-1));printkl(soob);}
	return;
   }//--------- пока длит.включения =0  ----------

   else if((r78==0)&&(ko==1)&&(nwm==(nom-1)))   //СВЫКЛ  - выключение основного питания.
   {Komplex[nom]=0; //выключили 
      if(peskuk!=0)
	{soob.sprintf("# Выключение КУК-ом основного питания в ВМ-%d",(nom-1));printkl(soob);}
	return;
   }//--------- пока длит.выключения =0  ----------

   else if((r78==3)&&(nwm==(nom-1)))          //Сторож.таймер
   {
	   if (peskuk != 0)
		   printkl("# Игнорирование запуска сторожевого таймера");
	   /*
	   rab1=700000000; rab=0; for(i=1;i<=ko;i++) rab=(rab+rab1);
	   k->tst=tsist+rab;
    if(peskuk!=0) {soob.sprintf("# Запуск сторожевого таймера  в КУК ВМ-%d",(nom-1));
	     printkl(soob);
     soob1.sprintf("# Время запуска = %lu в мкс",tsist/1000); printkl(soob1);                        
	 soob2.sprintf("# Время его будущего срабатывания = %lu в мкс",k->tst/1000); printkl(soob2);      
	 
	}*/
      return;
   }//---------------------------------------

    else if((r78==2)&&(nwm==(nom-1)))          //Для МКО
	{ k->wds[ko]=1;                
	if(peskuk!=0) {soob.sprintf("# Назначена ведущей ВМ-%d на МКО-%d",(nom-1),ko);printkl(soob);}
      return;
    }//--------------------------------

    else if((r78==2)&&(nwm!=(nom-1)))          //Для МКО
	{ k->wds[ko]=0;                
	if(peskuk!=0) {soob.sprintf("# У неведущей ВМ-%d сброшен ВДЩ%d",(nom-1),ko);printkl(soob);}
      return;
    }//--------------------------------
      //Ведущей на каждой МКО может быть только одна ВМ !

  else {}

 }//================  конец  bik() ================



  void skuk(void)
//Реализует события по КУК-у: срабатывние стор.таймера,
// сброс признаков готовности и условный установ wklmgr=0, если было wklmgr=10(задержанное выключение).
//Кроме того, учитывает признак обобщенной синхронизации Q в работе с RT таймера синхрониз.ММО (в конце функции).
//Запускается в конце sag() после anmag() и srabtsmmo().
 {int i; ntip *k;   DWORDLONG rab; UINT *mm;
             for(i=1;i<=kolwm;i++)                            //Цикл по всем ВМ
	{k=bazuk[i];
	  if(k->tst!=0)          //Работает сторожевой таймер
		{   if(k->tst<=tsist)     //Сработал таймер
			{k->tst=0;
			 Komplex[i]=1;     //Учли включение ВМ-i
			 nasreset(i);
  if(peskuk!=0) {soob.sprintf("# Сработал таймер КУК.  ВМ-%d  включена",(i-1));printkl(soob);
                 soob.sprintf("# Время его срабатывания =%lu в мкс",tsist/1000);        printkl(soob);  
				}
			}
		}//---------------- закончили с этим сторожевым таймером
      if(k->tgot!=0)       //Есть ожидание сброса признака готовности.
	  { if(k->tgot<=tsist)      //Истекло время ожидания сброса признака готовности.
		{k->tgot=0;
	     k->gt0=0;
         k->gt1=0;   
         k->gt2=0;
		 k->gt3=0;
  if(peskuk!=0)
  {soob.sprintf("# Сброшены признаки готовности команд КУК в ВМ-%d",(i-1));printkl(soob);
  }
		}
	  }//-------- закончили с этим признаком готовности.
                 //Теперь учет работы RT таймера синхрониз. ММО:
                            mm=k->mp;
              if((0x80000&(k->its[2]))!=0)  //есть Q - признак обобщ.синхр
	       
		   {rab=tsist-(k->tq);  if(rab>=256000)
					{
                      mm[k->its[0]]=0100000;  //Вписали в RT Qv=1 и остальные нули.
					 
					}
		                      else
					{
                     rab=rab/1000;
                     mm[k->its[0]]=(UINT)rab;  
					}
           }//-------------------------------------
            //else  mm[k->its[0]]=0;		   
	}//----------------------------------------------------------//Конец цикла по ВМ  
 }//=============== Конец skuk() ======================


    //---- ПОКА  ПУСТЫЕ  ФУНКЦИИ ---------------------

     void pereust(UINT pornom,ntip * k)
  {



  }//==========================================
//========================================================================================================
   short sts2(UINT adr,ntip *k) 
	//Читает слово по adr из модели k->mp,
	//пишет его в  sislo, записывает:  
	//Возвращает: 0 - при успехе,
	// 7 - при ошибке шины с данными /исключ. ситуация/,
	// 5 - при ошибке адресации
	// Не устанавливаются никакие следы!
   {  UINT ad,adkom;  short kesk,ot;
        if((03&adr)!=0)   return(4);  //Адрес не кратен 4.
//		if(k->jadro==1)   
//		{ if((020000000000&adr)!=0) return(4); }
//		                           //Пользов.в простр.ядра.
//----------------------------------------------------------
		ad=PtoF(adr);
		if((0xa0000000<=adr)&&(0xbfffffff>=adr)) kesk=0; else kesk=1;

	   
	 //----------- ad - физич адрес, kesk - признак кешир.области.
                                                               
          //Но может режим - изолированный ?  Отреагируем:
                if((0200000&k->status)!=0) //Режим изоляции
				{ ot=stkesdan(ad,k); 
				  if(ot==1) k->status=(02000000|k->status); 
				  else k->status=(037775777777&k->status); 
				                      // 19-й разряд status. 
			 return(0);  
					}//------- Далее  режим неизоляции
				
	  if(kesk==0)  
	{ ot=stzudan(ad,k,1);
		if(ot==1) 
		{
			return(7);
		}
		return(0);                     
	}//=================================
	
	      
	// Теперь осталось с кешированием. От изоляции не зависим.
		ot=stkesdan(ad,k); if(ot==0) 
		{   
		     return(0);  
		} //Все прочитано.
 //Теперь: был промах, надо читать по ad ЗУ и перепис.в КЕШ дан.
        ot=stzudan(ad,k,1); 
		if(ot!=0)
		{ 
			return(7); 
	    }                  //Ошибка шины, иначе все о-кей:
	           // --------- Делаем  переписи в КЕШ-данных:
           if((04000000000&k->config)==0)
		   { adkom=ad; ot=stzudan(adkom,k,0); if(ot!=0) 
				{
			        return(7);
				}
	           ot=zapkesdan(adkom,kdan,k);            
	 
	              return(0);
		   } 
		                         //Конец при одной перезаписи.
               //Теперь переписываем 4 числа:
       adkom=ad; adkom=(037777777760&adkom);
	                        //Очистили 4 младших разр.адреса.
	   ot=stzudan(adkom,k,0); if(ot!=0)
	   {
		   return(7);
	   }
	   ot=zapkesdan(adkom,kdan,k); adkom=(adkom+4);   
	   ot=stzudan(adkom,k,0); if(ot!=0) return(7);
	   ot=zapkesdan(adkom,kdan,k); adkom=(adkom+4);
	   ot=stzudan(adkom,k,0); if(ot!=0) return(7);
	   ot=zapkesdan(adkom,kdan,k); adkom=(adkom+4);
	   ot=stzudan(adkom,k,0); if(ot!=0) return(7);
       ot=zapkesdan(adkom,kdan,k); //Переписали в КЕШ.
               
		  return(0);

   }//=======================================================
   

    short stkesdan(UINT adr,ntip *k)
   //Читает в sislo число из установленной КЕШ-данных.  
   //Если нет промаха и v=1,  возвращает 0. 
   //Если отказ, возвращает 1.  /Высшая функция обратится к ЗУ.
    { DWORDLONG *mk,*md; short rk,rd,r;
	    UINT ml,st,*u;
          // Установ размеров блоков КЕШ:
                       if((040000000&k->config)==0)
				{ rd=1; rk=4; }	    else { rd=2; rk=2; }
		  // Учет возможного переключения блоков КЕШ:
	if((0400000&k->status)==0)  { mk=k->kespk; md=k->kespd; }
		   else { mk=k->kespd; md=k->kespk; r=rd; rd=rk; rk=r; }
 //Теперь КЕШ-дан.это md[], его размер в тыс.адресов=rd /1,2 или4/.
		   ml=adr>>2; if(rd==1) { st=ml>>10; ml=(01777&ml); }
                 else if(rd==2) { st=ml>>11; ml=(03777&ml); }		   
                        else    { st=ml>>12; ml=(07777&ml); }
 //--- Получили: st прообраз тега, ml адрес обращения в КЕШ-ком.
             u=(UINT *)(&md[ml]); //Вышли на нужную ячейку КЕШ.
						 sislo=u[0];          //Взяли число
	      if(((020000000000&u[1])==0)||((077777777&u[1])!=st)) 
         return(1); //Отказ по недействительности или не тот тег.
         else  return(0);
	} //==========================================================

	short stzudan(UINT adr,ntip *k,short p)
 //Читает из модели ЗУ k->mp  число. При p=1 пишет в карту и 
 //в sislo, возвращая 0. Возвращает 1 при неудаче /ошибка шины/.
 // Если p=0, то пишет только код  в kdan, в карту не пишет!
	{	UINT *m,in,rab;
	       m=k->mp;   //Массив модели ЗУ.

		          for(in=1; ;in=(in+8))
			{ if((adr>=m[in])&&(adr<m[in+1])) //Нашли кусок памяти.
				{ rab=adr-m[in]; rab=(rab>>2); 
				  rab=(rab+m[in+3]);
//		         if((k->mdp[rab]!=0)&&(p==1))
//				 {
//				 wnu(adr,'s',sislo,&sislo,k); return(0); }
				  rab=(m[rab]); //Взяли ячейку. 
				  if(p==0) { kdan=rab; return(0); }
                  //Теперь пишем в карту и др:
		  sislo=rab; 
		//if((01&m[in+4])==0) return(1);   //Чтение запрещено !  
	    
				   return(0);
				 }
               if(m[in+2]==1)
			   {     kdan=037777777777; 

                 return(1);
			   }
			}

	}//=========================================================


     short zapkesdan(UINT adr,UINT kod,ntip *k)
 //Пишет в КЕШ-дан число kod по адресу adr, вычленяя усеченный
 //адрес, формируя тег и делая v=1.
 //Возвр.1 при промахе, иначе - 0. Это нужно для изолир.режима.
    { DWORDLONG *mk,*md; short rk,rd,r,pr;
	    UINT ml,st,*u;
          // Установ размеров блоков КЕШ:
                       if((040000000&k->config)==0)
				{ rd=1; rk=4; }	    else { rd=2; rk=2; }
		  // Учет возможного переключения блоков КЕШ:
		   if((0400000&k->status)==0) { mk=k->kespk; md=k->kespd;}
		   else { mk=k->kespd; md=k->kespk; r=rd; rd=rk; rk=r; }
//-Теперь КЕШ-дан.это md[], его размер в тыс.адресов=rd /1,2 или4/.
		   ml=adr>>2; if(rd==1) { st=ml>>10; ml=(01777&ml); }
                 else if(rd==2) { st=ml>>11; ml=(03777&ml); }		   
                        else    { st=ml>>12; ml=(07777&ml); }
  //--- Получили: st прообраз тега, ml адрес обращения в КЕШ-дан.
            u=(UINT *)(&md[ml]); //Вышли на нужную ячейку КЕШ.
           if((077777777&u[1])!=st) pr=1; else pr=0;
       u[0]=kod; u[1]=st; u[1]=(020000000000|u[1]); //Все вписали.
			             return(pr);
	 }//========================================================

   short zas2(UINT adr,ntip *k)                                          
	//Пишет слово по adr модели k->mp,
	//беря его из  sislo. Записывает:  
	//Возвращает: 0 - при успехе,
	// 7 - при ошибке шины с данными /исключ. ситуация/,
	// 5 - при ошибке адресации
	//20 - при записи в несуществующую ячейку.
	//Не устанавливает никакие следы !
   {  UINT ad;  short kesk,ot;
	 //if(stopwr!=0)                                                         
	 //{}	 
        if((03&adr)!=0)   return(5);  //Адрес не кратен 4.
//		if(k->jadro==1)   
//		{ if((020000000000&adr)!=0) return(5); }
//		                           //Пользов.в простр.ядра.
//------------------------------------------------------
		ad=PtoF(adr);
		if((0xa0000000<=adr)&&(0xbfffffff>=adr)) kesk=0; else kesk=1;

	   
 //----------- ad - физич адрес, kesk - признак кешир.области.
                  
         //Но может режим - изолированный ?  Отреагируем:
         if((0200000&k->status)!=0)     //Режим изоляции
		 {  ot=zapkesdan(ad,sislo,k);
		  if(ot==1) k->status=(02000000|k->status);   else
    k->status=(037775777777&k->status);// 19-й разряд status. 
                             return(0);
			}//------- Далее  режим неизоляции --------                 
     
	   if(kesk==0)                                         
	{  ot=zapzudan(ad,k);  
	  
	 if(ot==1) return(7); else if(ot==2) return(20); else return(0);
	} 
  //---- Теперь осталось с кешированием. От изоляции не зависим.   
     ot=zapzudan(ad,k);  
	 if(ot==1) return(7);  if(ot==2) return(20); 
	                                              
  //---Теперь надо записать sislo в КЕШ-данных /адрес ad:
           ot=zapkesdan(ad,sislo,k); return(0);

   }//=======================================================


	short zapzudan(UINT adr,ntip *k)
		//Записывает в модель ЗУ k->mp  sislo по физич.адресу adr. 
		//возвращая 0. Возвращает  1  при неудаче /ошибка шины/
		//или  2  при записи в несуществующую ячейку.
	{	UINT *m,in,rab;
	       m=k->mp;   //Массив модели ЗУ.
		          for(in=1; ;in=(in+8))
		{ if((adr>=m[in])&&(adr<m[in+1])) //Нашли кусок памяти.
				{ rab=adr-m[in]; rab=(rab>>2); 
				  rab=(rab+m[in+3]); //Это индекс в модели.           
                  //Теперь пишем в карту и др:
    
				  
//   if((02&m[in+4])==0) return(1);   //Запись запрещена !
//   if(k->mdp[rab]!=0) { 
//	   wnu(adr,'z',sislo,&sislo,k); return(0);}
     m[rab]=sislo;
	    
				    return(0);
				 }
               if(m[in+2]==1)    //Ячейка не существует. 
			   { wpis(50,0,0,0,11,2,adr,k);  //Запись адреса записи в RBEr
				   //данные непрочитаны.
			      kdan=037777777777; return(2);
			   }
			}

	}//==========================================================





//    1.     Массив wus[] и будильник bud[], комментарий;
      //-------------------------------
//UINT wus[1600] разбит по восьмеркам,каждая восьмерка
//соответствует сочетанию КМК - ВМ  или внешнему абоненту (модели его) по номеру модели;
//Вначале по 287 индекс КМК - ВМ,
//далее с 288 индекса идут модели в последовательности их номеров, начиная с номера 0.
//Для М-ой ВМ (мой номер) и Р -го КМК начальный индекс их восьмерки определяется формулой:
//     i=(M-1)*48+P*8;
//Эта формула используется в том числе в prozessor() при формировании будильника.
     //-------   В каждой восьмерке (обозначаем здесь индексы с нуля):
//wus[0]=1 означает КМК; wus[0]=2 -означает модель;
    //--------------------------------------
//wus[1]- содержит pornom КМК или цифровой идентификатор модели;
   //---------------------------------------
//wus[2]=0 -объект пассивен, wus[2]=1-объект активен;
//wus[2]=3  - вообще конец перечисления об'ектов /изолированный/.
   //---------------------------------------       
//wus[3]=0 - будильник заведен на начало обмена по КМК(активный объект);
//wus[3]=1 - будильник заведен на окончание обмена по КМК(активный объект);
//wus[3]=2 - будильник заведен на разовое разбуживание моделиактивный объект);
//wus[3]=3 - будильник заведен на циклическое разбуживание ее(активный объект);
   //---------------------------------------
//wus[4] =1, если КМК контроллер. =2, если КМК оконечник. =3, если монитор.
//wus[5]  - номер (мой) ВМ
//wus[7]  - индекс модели в  Table при работе с ней ЭВМ.
//---------------------------------------------------------------- 
 //wus[5], wus[6], wus[7] - резерв на будущее;
//----------------------------------------------------------------
//DWORDLONG bud[200] - будильник для 200
//разных объектов - КМК-ВМ или моделей;
//индекс i bud[] соответствует одной восьмерке в wus[];
//так что wus[8i] - начало восьмерки, т.е. признак КМК или модели.
//bud[i] - это значащее время будильника, если только:
//wus[8i+2]=1, т.е. объект активен, иначе;
//bud[i] - не содержит данных;
//Цена единицы времени в будильнике bud[]  - 4мкс.


       //   2.     ПРИКАЗ В ПЕНАЛЕ ДЛЯ МОДЕЛИ:

  // Каждый приказ - в собственном формате. В первой его ячейке
  // - число слов в приказе. Но после приказа следуют еще две
  // ячейки вне приказа, наличие которых надо учитывать при переходе 
  // к следующему приказу. Это: индекс модели в Table и затем
  // приоритет ее Prioritet при данном Reason.   
  //    Приказы в пенале идут слитно, на первую свободную ячейку      
  // в нем указывает индекс пенала  ipen0.


      //    3.    Когда модель контроллер, а ВМ оконечник:

//Номер МКО заказывается моделью с помощью REASON_OUI, где I - номер МКО(0-5), т.е. pornom.


  //                ДОБАВЛЕНИЕ :
    // UINT bufou[220] в карте процессора служит для запоминания
 //   vixod[] от модели-контроллера.Информацию эту принимает КМК-ОУ.
 //  По его pornom эти данные пишутся в bufou /в функции kontrmod( )     
 //  в начале передачи/ с индекса  35*pornom . Структура этой зоны
 //          - кол-во данных 
 //          - резерв
 //          - резерв 
 //          - сами ДАННЫЕ   .их количество в первой ячейке.  
 //  Эти данные используются в kmkaou( )  в конце передачи.

 //============== Конец  файла ====================================