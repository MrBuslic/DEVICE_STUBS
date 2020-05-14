
    // **************** ФАЙЛ   PROZ.CPP  ЦБК
     //        С новым   ПВР и ММО 
    #include "PKM.h"   // <----  А это нужно.
    //----------------------------------------------------------


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

extern short prz;
extern short prerr;
		//"Характеристическая" локальная память:
    QString soob;           //Для криминального сообщения
    static UINT kodkom;     //Для кода команды при ее чтении.	    
			                //в функции stkom( ).
	static UINT kkom;   // Для переписи кодов ком. в КЕШ ком. 
	static UINT adkom;      // Для их адресов.
	static UINT kdan;       //Для кода данных при чтении.
    static  UINT sislo;      //Для обменов с памятью.
    //-----Для компонент 2 -х веществ.чисел 1 и 2 /индексы их/:
	  //символ d для чисел двойной точн.
    static short so1,so2,sd1,sd2;    //Для знаков чисел.   
    static int p1,p2,pd1,pd2;//Для несмещенных порядков чисел.
    static int f1,f2;            //Для мантисс одинарных чисел.
    static LONGLONG fd1,fd2;     //Для мантисс двойных чисел.
    static DWORDLONG m1,m2,m3;   //Тара для работы с мантиссами.
    static DWORDLONG   m4;      //Тара для хвоста двойных чисел.
    static int *u1,*u2,*u3,*u4; //Для отражения частей тары.
   //**********************************************
	DWORDLONG ttek4;
    DWORDLONG tn;
	UINT delt,delt16,delt4;
	extern UINT wus[1600];
	extern DWORDLONG Tmin;
   //********************************************** 
		 //Общая рабочая память:
    int  ss1,ss2,ss3,ss4;
    UINT s1,s2,s3,s4;
	short sdw;          //Для байтовых частей ячейки.
	short xwost,netos;
	short slog; //Для различ. сложения и вычитания.
	UINT stopwr;
    //*********************************************
    extern int nomerWM;
	extern short kolwm;
	extern ntip *bazuk[7];
//    extern short prslot[7];
	int serwklus;
	short rfee;  //признак для правильной реакции при RFE в слоте:
	//не выполнять вторую команду (перехода) в zakr при k->predper=1, 
	//так как эта RFE в слоте сработает и за нее (обойдет переход с правильной установкой адреса перехода)
	//*********************************************
		//Об'явления функций:
   extern void  printkl(QString s);
   extern char* Disass(unsigned long int cod,
	   unsigned long int epc,unsigned short pr);
   extern void wnu(UINT adr,char ss,UINT sisl,UINT *uk,ntip *k);
   extern void sbrostimeb1(ntip *k);
   extern void sbrkpr(ntip *);
   extern void sbrosostkfu(ntip *k);
   extern void sbrospk(ntip *k);
   extern void sbrospwr(ntip *k);
   extern void sbroskmk(ntip *k);
   short proz(ntip *k); //Возвращает 0 при
	    //полном упехе, а также при исключ.сит. в CP1,
        //если она оказалась замаскированной :
	    //они не всегда дают прерывание (маскируются);
	    //или 1 - при ошибке в команде в k->kom при Reset;
	    //или 2 - при исключит.ситуациях CP0 кроме Reset;
	    //или 3 - при програмн.ошибках;
	    //или 4 - при непредусмотренной команде;
        //или 5 - при прерывании от CP1.
   void stca(int v,ntip *k,UINT adr,short cp); //Перетряска
	    // регистров Status и Cause при исключ.ситуации.
	    // Установ epc и pc.
	    // v - код ошибки, k - указатель карты.
	    // adr - адрес обращения, cp - номер
	    // сопроцессора при его недоступности.
   short stkom(UINT adr,ntip *k); //Чтение команды по 
				  //адресу adr из модели k->mp.
   short stkeskom(UINT adr,ntip *k);
   short stzukom(UINT adr,ntip *k,short p);
   void  zapkeskom(UINT adr,UINT kod,ntip *k);
   short stb(UINT adr,ntip *k);   //Чтение байта в sislo
				  //из модели k->mp.
   short stp(UINT adr,ntip *k);   //Чтение полуслова в sislo
				  //из модели k->mp.
   short sts(UINT adr,ntip *k);   //Чтение слова в sislo
   short stkesdan(UINT adr,ntip *k);
   short stzudan(UINT adr,ntip *k,short p); 
   short  zapkesdan(UINT adr,UINT kod,ntip *k);
   short zapkesdanosob(UINT adr,UINT kod,ntip *k);                              
				  
   short zab(UINT adr,ntip *k);   //Запись байта из sislo
				  //в модель k->mp.
   short zap(UINT adr,ntip *k);   //Запись полуслова из sislo
				  //в модель k->mp.
   short zas(UINT adr,ntip *k);   //Запись слова из sislo
				  //в модель k->mp.
   short zapzudan(UINT adr,ntip *k);

   void invold(UINT adr,ntip *k);  

   short rpcp1(char s,ntip *k); //Проверка возможности прерыв. 
				                // от CP1 по признаку s /искл./
   void zapros(char s,ntip *k); //Вписыв запросы в contrlstatus.
   void flagi(char s,ntip *k);  //Впис.флагов туда же.
   short kto(UINT nr,char t,short ns,ntip *k);
				  //Определитель типа числа.
   short okrugo(ntip *k);         //Округляет одинарное число.
   short okrugd(ntip *k);         //Округляет двойное число.
   void okrugz(ntip *k);          //Округляет целое число.- 'o' 
   void okrugzd(ntip *k);          //Округляет целое число.- 'd' 
   int  sraw(char s,short us,short nan,UINT r1,UINT r2,ntip *k);
				  //Сравнивает 2 числа.
   UINT * modzu(UINT * zag);      //Строит модель ЗУ.
   extern void wpis(UINT tip,UINT pornom,UINT podtip,UINT nblok,
		                 UINT it,UINT d,UINT sis,ntip *k);
extern  UINT xitpr(UINT star,UINT now,UINT zam,UINT soxr,UINT xit0,
	                            UINT xit1,UINT sistka,UINT n);




    extern UINT indpam(UINT adr,ntip *k); 
                                //Возвращает индекс по адресу.  
   //-----------------------------------------------------
   short op0(ntip *k);               //Исполнитель группы команд.				     
   short op1(ntip *k);               //Исполнитель группы команд.
   short op4do15(short op,ntip *k);  //Исполнитель группы команд.   
   short op16(ntip *k);              //Исполнитель группы команд.
   short op17(ntip *k);              //Исполнитель группы команд.
   short op32do57(short op,ntip *k); //Исполнитель группы команд.   
  //**********************************************
extern UINT PtoF(UINT adr);
   extern void fnomou(ntip *k);
   void zawis(ntip *k);
   void serega(ntip *k);
    void serwkl(int n);


   //========================== ГЛАВНАЯ ФУНКЦИЯ: =============

   short proz(ntip *k)
 {   UINT rab,rab1,otw,rom1,rom2;   //Рабочие.
     int i; ntip *uk; int ewm;
     k->zp[0]=0;   //zero нулевого региста CP0.
	 k->sledo[0]=0;//Операция CP0 - до коррекции в CP1.
     k->sledo[1]=1;//Длительность ее 1 такт, до коррекции.
     k->sleds[0]=0;//Начальное отсутствие обращ. к ОЗУ.
	 k->sleds[1]=0; k->sledk[1]=0;
     k->codiskl=50;    //Отсутствие.  
     k->codisklcp1=0;  //Отсутствие.
	 rfee=0;    //Пока не знаем, RFE ли предстоящая операция?
    //------------------------------
	 
    if(k->kom==2)  //Reset **********************
    {  
	    
	   //k->tpr=0;   #$#$
	   //k->tzu=0;           //Чистка.
	   //k->tsl=0; k->tum=0; k->tdl=0; //Чистка.
	  // tn=0; 
	   //delt=0;
	    //delt16=0;
	 
	   //k->tpred4=0;  delt4=0;  
	   k->sledk[4]=13; //break - для пропуска первой команды 
	                   //в Reset при счете времени wrema.
	  for(rab=0;rab<=32;rab++) k->reg[rab]=0; //Чистка.
      
			for(rab=0;rab<=20;rab++)
	{ rab1=rab*(k->mtakt); k->dlit[rab]=(DWORDLONG)rab1;}
			 //Вычислили длительности по числу тактов.
	   //Выходы:
      k->jadro=0;  k->halt=0;
      k->config=0;
	  k->zapcp1=02000;
       //-------------------------------
    //------- Чистка будильника :       --------------
                                                                                                                                                                        		   for(i=0;wus[i+2]!=3;i=i+8)
		   { if(i>=1600) 
			{ printkl("#F PROZ : Ошибка при чистке будильника.");
			 return(3);}
			   wus[i+2]=0;  //Снятие активности.
		   }
		   Tmin=0xffffffffffffffff;
    //------------------------------------------------

	   sbrostimeb1(k);      //Сброс таймеров типа 1
	   sbrkpr(k); //Сброс контроллера прерыв. типа 50
	   sbrosostkfu(k); //Сброс остатков KFU типа 51
	   sbrospk(k);     //Сброс - установ регистров ПК типа 52
	   sbrospwr(k);    //Сброс - установ регистров ПВР типа 60
	   sbroskmk(k);    //Сброс КМК 
	   k->cause=0;  k->status=0;                              
                                          //установ разрядов
       k->status=(030000000|(k->status)); //регистра Status.
       k->pc=0xbfc00000;  //установ адреса обраб. Reset.
       k->predper=0;  k->zapusl=0; k->axtung=0;
	   fnomou(k);  //Установ адресов ОУ
	     return(0); 
    }  //***************************************** конец Reset.




{//-------------- Это для Сергея. XOST  ! 
 UINT RHst, ind, *mm; 
ind=indpam(0x8000048,k); mm=k->mp; RHst=mm[ind];
 if(RHst==02000) k->cause=0100000|k->cause; 
}//------------------------------ end Времянка





		   //Теперь команда:

    if(k->kom!=1)    //При не Reset код команды должен быть = 1.
   {soob.sprintf("#F - PROZ - Ошибка: код команды = %d",k->kom);
	  printkl(soob);	return(100); }  //----фатальная ситуация.
   	      //Теперь исследуем внешние прерывания:

	if(serwklus!=0) {serega(k);                       //ДАННЫЕ ДЛЯ СЕРГЕЯ
	}

	  rom1=1&(k->status); rom2=(k->status)&(k->cause)&0177400;
	    if((rom1!=0)&&(rom2!=0)&&(k->predper==0))
     //Смотрели:  есть ли оно разрешенное?
	 //При переходе нельзя прерываться от внешних прерываний.
	 //Так определил Сережа.
	{ewm=-1; for(i=1;i<=kolwm;i++)
		{uk=bazuk[i]; if(uk==k) {ewm=i-1; break;}}
		        if(prerr!=0)
		{
	      soob.sprintf("#I - PROZ - Прерывание ВМ%d по CAUSE=%x",ewm,k->cause); printkl(soob);  
	      soob.sprintf("перед исполнением команды по адресу %x",k->pc); printkl(soob);              
		}

//--Сделаем запись на epc адреса продолжения и уст.признак BD:
	 if(k->predper==1)  //предшествовал переход.
      { k->epc=k->pc-4;     k->cause=(020000000000|k->cause); }
	 else {k->epc=k->pc; k->cause=(017777777777&k->cause); }
//------Теперь установ адреса программы обработ.прерыв.по BEV :
       if((020000000&(k->status))==0) k->pc=0x80000000+0x080;
	  else                 k->pc=0xbfc00100+0x080;

      //---------------- Теперь запись ExcCode  /0/ в Cause:
       k->cause=(037777777600&(k->cause));       
                                    //--------сделано.
	//--------Теперь опускание стека в Status :
     rab=k->status; rab=(077&rab); rab=(rab<<2); rab=(077&rab);
     k->status=(037777777700&(k->status));
     k->status=(rab|(k->status));             //--------сделано.
	//----------------Теперь информирование на выход:
      k->jadro=0;  
                            //--------сделано.
      //------------Перед прогр.прер.нет учитываемого перехода:
      k->predper=0;	  k->sledk[4]=13;
      return(0); //Прерывание учтено. Система выведена
      // на 1 -ю команду программы обработки прерывания.
     } //----------------------- Конец работы с прерыванием .
         
  if((02&(k->status))!=0) k->jadro=1; else k->jadro=0;

  
         otw=stkom(k->pc,k);	//Чтение команды
	if(otw==4) 
	{soob.sprintf("Искл.сит.при выборке команды в ВМ%d",nomerWM-1);printkl(soob);//#$#$
		stca(4,k,k->pc,0); return(2);
	} //Исключ. 4.
	if(otw==6)
	{soob.sprintf("Искл.сит.при выборке команды в ВМ%d",nomerWM-1);printkl(soob);//#$#$ 
		stca(6,k,k->pc,0); return(2);
	} //Исключ. 6.
    if(otw==3)  { return(3);  }
               k->codisklcp1=0;  //Отсутствие вначале.
     //-------------------
     // КОМАНДА ВЫБРАНА.   ЕЕ КОД в  в kodkom.
     //  АДРЕС ЕЕ НА k->pc и в k->sledk[0].
	 // Код команды в k->sledk[4].
     //         НАЧИНАЕМ ЕЕ ИНТЕРПРЕТИРОВАТЬ:
		//Перебираем группы команд по коду op:


	  if((037400000000&kodkom)==0)
			 { rab=op0(k); return(rab); }
     else if((037400000000&kodkom)==0400000000)     
			 { rab=op1(k); return(rab); }
     else if((037400000000&kodkom)==01000000000)
     { //Исполняет команду из kodkom, если op=2.
       //Это безусл.переход, при op=2 единств.ком.   -  J.
		if(k->predper==0) 
		{    
		   rab=kodkom;
		   rab=(0377777777&rab); rab=(rab<<2);
		   rab1=k->pc;  rab1=(036000000000&rab1); 
		   rab=(rab|rab1); 
			k->adrper=rab;
			k->pc=(k->pc+4);    //Запись адреса след.ком.
		   k->predper=1; return(0); }
		else {  k->predper=0;  k->pc=k->adrper;
		  return(0);
		 }
      }	//-------------------------------Конец  J.
     else if((037400000000&kodkom)==01400000000)  
   {	//Исполняет команду из kodkom, если op=3.
	//Это переход с возвратом, при op=3 единств.ком.  -  JAL.
		if(k->predper==0) 
		{  
		   rab=kodkom;
		   rab=(0377777777&rab); rab=(rab<<2);
		   rab1=k->pc;  rab1=(036000000000&rab1); 
		   rab=(rab|rab1);      //Адрес перехода.
           k->adrper=rab; 			
			k->pc=(k->pc+4);     //Запись адреса след.ком.
		   k->zp[31]=k->pc+4;  //Записали адрес возврата.
    //Работа для nexti:
      if(k->wzwr==0)
	  {k->wzwr=1; k->adrwzwr=k->pc+4;}
    //--------------------------------
		   k->predper=1; return(0); }
	    else { k->predper=0;
		    k->pc=k->adrper;  return(0);
		 }
    }	//------------------------------Конец  JAL.

	   rab1=kodkom; rab1=(rab1>>26); rab1=(077&rab1);
	   if((rab1>=4)&&(rab1<=15))
	   { rab=op4do15(rab1,k); return(rab); } //Коды op  4 - 15.
       //--------------------------------
	  if(rab1==16) { rab=op16(k);

	  return(rab); } //Код 16.
       //--------------------------------------------
	  if(rab1==17) { rab=op17(k); return(rab); } //Код 17.
       //--------------------------------------------	   
	  if((rab1>=32)&&(rab1<=57))
	  { rab=op32do57(rab1,k); return(rab); } //Коды op 32 - 57.
       //--------------------------------------------	  		 
	       //Если непредусмотренная:
	  soob.sprintf
("#E - PROZ - Непредусмотр.команда %o по адресу %X",kodkom,k->pc);
	    if(prz) printkl(soob);
      stca(10,k,0,0);   return(2);
 }//================ КОНЕЦ ГЛАВНОЙ ФУНКЦИИ .==================


   void stca(int v,ntip *k,UINT adr,short cp)
		       //Коррекция Status и Cause при исключ.
		       //ситуации, включая: опускание стека,
		       // установ epc, адреса обработчика прерыв.,
		       //запись ExcCode,
		       //признака BD - (предшествовал ли переход),
		       //CE -  номера сопроцессора cp при его
		       // недоступности в искл.ситуац.11,
		       //запись на badvaddr адреса adr обращения
		       //к памяти при ошибке адресации.
		       //Частично информирует - в карту.
		       //v - код искл.ситуации. k - указат.карты.
		       //adr - адрес обращения. cp - номер
		       //сопроцессора при его недоступности.
    { UINT rab;  
 //--Сделаем запись на epc адреса продолжения и уст.признак BD:
         if((v!=8)&&(v!=9))  //#$#$
{
//soob.sprintf("Искл.сит.№%d.  Адрес команды %x",v,k->sledk[0]);printkl(soob);//!@!@
//soob.sprintf("  Адрес обращения к памяти %x, CP%d",adr,cp);printkl(soob);//!@!@
}
	 if(k->predper==1)  //предшествовал переход.
       { k->epc=k->pc-4;     k->cause=(020000000000|k->cause); }
	 else {k->epc=k->pc; k->cause=(017777777777&k->cause); }
 //------Теперь установ адреса программы обработ.прерыв.по BEV :
       if((020000000&(k->status))==0) k->pc=0x80000000+0x080;
	  else                 k->pc=0xbfc00100+0x080;
      //---------------- Теперь запись ExcCode в Cause:
       k->cause=(037777777600&(k->cause));     rab=v<<2;  
       k->cause=(rab|(k->cause));             //--------сделано.
	//--------Теперь опускание стека в Status :
     rab=k->status; rab=(077&rab); rab=(rab<<2); rab=(077&rab);
     k->status=(037777777700&(k->status));
     k->status=(rab|(k->status));             //--------сделано.
	//-------Теперь, если ошибка адресации, запомним адрес:
      if((v==4)||(v==5))   k->badvaddr=adr;   //--------сделано.
	//-----Если была недоступность сопроцессора, укажем его
	//-----в CE регистра Cause:
      if(v==11)      { rab=cp; rab=(rab<<28);
     k->cause=(031777777777&k->cause); k->cause=(rab|k->cause);}
	//----------------Теперь информирование на выход:
      k->jadro=0;  
      k->codiskl=v;                //--------сделано.
      //------------Перед прогр.прер.нет учитываемого перехода:
      k->predper=0;	 
   }//========================== Конец stca .===================



   short rpcp1(char s,ntip *k)   //Возвращает 1 при возможном
   { UINT rab; int z;            //прерыв.от CP1, иначе - 0.
                                 // s - обозначение искл.сит.
                                 //в CP1: e,v,z,o,u,i.
            z=1;                 //Она же ВПИСЫВАЕТ В CAUSE
      rab=k->contrlstatus;       //ЗАПРОС НА ПРЕРЫВАНИЕ В IP 
		 if(s=='v')               
	{                              
	if((04000&rab)==0)  z=0; }      
	   else  if(s=='z')
  { 
   if((02000&rab)==0)  z=0; }
	   else  if(s=='o')
  {if((01000&rab)==0)  z=0; }
	   else  if(s=='u')
  { 
   if((0400&rab)==0)   z=0;}
	   else  if(s=='e')
	   {    z=1;  }                     
	    else if(s=='i')
  { 
   if((0200&rab)==0)   z=0; }
		else  { } //Обман трансл.

   if(z!=0) k->cause=(k->zapcp1|(k->cause));                         

           if((01&k->status)==0)  return(0);		

   if((z==0)||((k->zapcp1&(k->status))==0)) return(0);
   else {  return(1);}                                          
     //Учтена и маска IM  в status по позиц. номеру zapcp1.
 }//=========================================================



			      
   void zapros(char s,ntip *k)        
   {       //Вписывает  признаки искл.сит.'s' в contrlstatus.          
                                 // s - обозначение искл.сит.
                                 //в CP1: e,v,z,o,u,i.                
		 if(s=='v')           
		 { k->contrlstatus=(0200000|k->contrlstatus);}  
	   else  if(s=='z')
	   { k->contrlstatus=(0100000|k->contrlstatus);}
	   else  if(s=='o')
	   { k->contrlstatus=(040000|k->contrlstatus);}
	   else  if(s=='u')
	   { k->contrlstatus=(020000|k->contrlstatus);}
	   else  if(s=='e')
	   { k->contrlstatus=(0400000|k->contrlstatus);}
	    else if(s=='i')
		{ k->contrlstatus=(010000|k->contrlstatus);}
		else  { } //Обман трансл.

 }//======================================================
			                      				                  
   void flagi(char s,ntip *k)  //Вписывает  флаги   
   {                //искл.сит. 's' в contrlstatus.    
				    // s - обозначение искл.сит.
                    //в CP1: v,z,o,u,i. 
		 if(s=='v')           
   k->contrlstatus=(0100|k->contrlstatus); 
	   else  if(s=='z')
   k->contrlstatus=(040|k->contrlstatus);   
	   else  if(s=='o')
   k->contrlstatus=(020|k->contrlstatus);   
	   else  if(s=='u')
   k->contrlstatus=(010|k->contrlstatus);   
	    else if(s=='i')
   k->contrlstatus=(04|k->contrlstatus);   
		else  {}; //Обман трансл.
 }//======================================================

   short kto(UINT nr,char t,short ns,ntip *k)
//Компоненты числа, находящегося в регистре CP1, записывает
//во внешнюю область, не исправляя условный порядок ненорма-
// лизованного числа, но вписывая подразумеваемый старший
// разряд нормализованного числа, и возвращает тип числа:
//0 - нормализованное не равно 0, 1 - это +0, 2 - это -0,
//3 - ненормализованное с условным порядком,
//4 - плюс бесконечность, 5 - минус бесконечность,
//6 - тихое нечисло, 7 - сигнализир.нечисло. 
//nr - номер регистра CP1, t=o одинарное t=d двойное число.
//ns - присваиваемый мной индекс числа /1 или 2/.
//Внимание: порядок числа записан (в p.или pd.) несмещенным!
   { UINT rab,*uk; LONGLONG sd; int p,f; short s;
	     if(t=='o')   //одинарная точность числа.
       //**************************************************

 { rab=k->fgr[nr];
  if((020000000000&rab)!=0) s=1; else s=0;  //Это знак числа.
  p=017740000000&rab; p=(p>>23); p=(p-127); //Несмещ.порядок.
      f=037777777&rab;                        //Мантисса.
    if((p>=(-126))&&(p<=127)) f=(040000000|f);//Уточнили ее.
	    if(ns==1)   {so1=s; p1=p; f1=f;}
	     else       {so2=s; p2=p; f2=f;}
	 //--------------Передали компоненты во внешнюю область.
	     if((p==128)&&(f!=0))
	{ if(020000000&f) return(7); else return(6);} //Не число.
	     if((p==128)&&(f==0))
	{ if(s==1)        return(5); else return(4);} //Бесконеч.
	     if((p>=(-126))&&(p<=127))    return(0);  //Нормализ.
	     if((p==(-127))&&(f!=0))      return(3);//Ненормализ.
	     if((p==(-127))&&(f==0))
	{ if(s==1)        return(2); else return(1);} //Нули.
			  return(100);  //Обман трансл. 
    }//------------Закончили с одинарной точностью.
	     else if(t=='d')       //Двойная точность числа.
       //***************************************************
   { uk=(UINT *)(&sd);
     uk[0]=k->fgr[nr]; uk[1]=k->fgr[nr+1]; //Взяли число в sd.
	if((020000000000&uk[1])!=0) s=1; else s=0;  //Знак числа.
    p=017774000000&uk[1]; p=(p>>20); p=(p-1023);//Несм.порядок.
       uk[1]=(03777777&uk[1]);                    //Мантисса.
	    if((p>=(-1022))&&(p<=1023))
	    uk[1]=(04000000|uk[1]);               //Уточнили ее.
	 if(ns==1)  { sd1=s; pd1=p; fd1=sd; }
	  else      { sd2=s; pd2=p; fd2=sd; }
	 //--------------Передали компоненты во внешнюю область.
	     if((p==1024)&&(sd!=0))
	{ if(02000000&uk[1]) return(7); else return(6);}//Не число.
	     if((p==1024)&&(sd==0))
	{ if(s==1)        return(5); else return(4);} //Бесконеч.
	     if((p>=(-1022))&&(p<=1023))    return(0);//Нормализ.
	     if((p==(-1023))&&(sd!=0))      return(3);//Ненормализ.
	     if((p==(-1023))&&(sd==0))
	{ if(s==1)        return(2); else return(1);}   //Нули.
			  return(100);  //Обман трансл.
    }//------------Закончили с двойной точностью.
			  return(100);  //Обман трансл.
  }//=======================================================			


 short okrugo(ntip *k) 
	     //Округляет одинарн.нормализ.число по условиям 
	     //в contrlstatus. Исходн.данные в p1,m3,so1,
	     //порядок на входе несмещенный, неусловный.
	     //Тара m3 отражена u3[]. Возвращает при	   
		 //переп.8, при ненормализ.рез-те 9,
		 //при нормализованном  - 0. 		   
		 //Подставляет результаты обратно
	     //в p1,u3[1], обеспечивая беск.,
         //условный порядок ненормализ.числа. 
	     //В нормализ.числе 23 разр.стерт!
	     //На выходе порядок несмещенный.
         //После смещения порядка компоненты
         //числа можно вписывать в ответ
         // - при неразреш.прерывания.
   { if((u3[0]!=0)||(xwost!=0))  netos=1; else netos=0;

      if((03&k->contrlstatus)==0)
      { if((020000000000&u3[0])==0)  goto wixod;
        else    //Теперь 1 в старш разряде  u3[0].
       { if((017777777777&u3[0])!=0)
		{u3[1]=u3[1]+1; goto wixod;}
     //Теперь полухвост в u3[0] после старш.разряда нулевой.
         if((01&u3[1])==0) goto wixod;
         else { u3[1]=u3[1]+1; goto wixod;}
		}
      }
      else if((03&k->contrlstatus)==1)
	 { if(so1==0)
	    {if(u3[0]!=0)  goto wixod;
	     else if((u3[0]==0)&&(xwost>=0)) goto wixod;
		 else {u3[1]=(u3[1]-1); goto wixod;}
	 }  
       else
       {if(u3[0]!=0) goto wixod;
	      else if((u3[0]==0)&&(xwost<=0)) goto wixod;
		  else {u3[1]=(u3[1]-1); goto wixod; }
	 }
	 }
	 //------------------
     else if((03&k->contrlstatus)==2)
    { if(so1==0)
    { if(((037777777777&u3[0])==0)&&(xwost<=0)) goto wixod;
	 else {  u3[1]=( u3[1]+1); goto wixod; }
     }    
     else 
     {
        if(((037777777777&u3[0])==0)&&(xwost>0)) 
	  {  u3[1]=( u3[1]-1); goto wixod; } 
       else  goto wixod; 
     } 
	 }
	 //------------------------------
	 else
     {   if(so1==0)
	 {	if(((037777777777&u3[0])==0)&&(xwost<0)) 
	 {u3[1]=(u3[1]-1); goto wixod; }
	  else goto wixod;
	 }
	 else
     {if(((037777777777&u3[0])==0)&&(xwost>=0)) goto wixod;
	 else {u3[1]=(u3[1]+1); goto wixod; }
	 }  
     }
	 //----Теперь учет ухода мантиссы влево от округления:
    wixod:    xwost=0;
	 if(0100000000&u3[1])   //Выскочила в 24 разряд.
	   { u3[1]=( u3[1]>>1);  p1=(p1+1);  }
	   //----------------------------------------------
	  if((040000000&u3[1])==0) 
	  {u3[1]=(u3[1]<<1); p1=(p1-1);}
	  //-----------------------------------------------
	   if(p1>=128) //Переполнение. Делаются подстановки
				   //в зависимости от реж.округления.
       {      if((03&k->contrlstatus)==0)
	       { p1=128;  u3[1]=0; }      //Бесконечность.
	     else  if((03&k->contrlstatus)==1)
	   {p1=127;  u3[1]=037777777;}  //Максимально большое.
	     else  if((03&k->contrlstatus)==2)
		 { if(so1==1) { p1=127; u3[1]=037777777; }//И т.д.
		  else      { p1=128; u3[1]=0;         }
	       }
	     else
	       { if(so1==0) { p1=127; u3[1]=037777777; }
		   else     { p1=128; u3[1]=0; }
	       }	    
		 return(8);  
	}   //---------------------------			           
	  else if(p1<-126)         //Ненормализ.
	{   p1=-127;
        u3[1]=0x200;
		 return(9);      
	}  //---------------------------
	else {        //Нормализованное со штатным порядком.
	       u3[1]=(037737777777&u3[1]);
		                  //Сброс подразумев.23 разр.
			  return(0);
	     }           	 
  }//====================================================


   void okrugz(ntip *k) //Округляет целое число по условиям 
			       //в contrlstatus. Исходн.данные в m3,so1.
			    
   { if((u3[0]!=0)||(xwost!=0))  netos=1; else netos=0;
	   if((03&k->contrlstatus)==0) //Тара m3 отражена u3[]. 
	   {            //Подставляет результат обратно в u3[1].
	   if((020000000000&u3[0])==0)  goto wixod;
		else                         
	{ if(so1==0)                  
	  {if(((017777777777&u3[0])==0)&&(xwost<0)) goto wixod;
		    }
		   if(so1==1)
      { if(((017777777777&u3[0])==0)&&(xwost>0)) goto wixod;
		}
		   {u3[1]=(u3[1]+1); goto wixod; }		                            	                                
		}//----------------         
     }                         
     else if((03&k->contrlstatus)==1) 
	 { if(so1==0)
	    {if(u3[0]!=0)  goto wixod;
	     else if((u3[0]==0)&&(xwost>=0)) goto wixod;
		 else {u3[1]=(u3[1]-1); goto wixod;}
	 }  
       else
       {if(u3[0]!=0) goto wixod;
	      else if((u3[0]==0)&&(xwost<=0)) goto wixod;
		  else {u3[1]=(u3[1]-1); goto wixod; }
	 }
	 }
	 //------------------
     else if((03&k->contrlstatus)==2)
   { if(so1==0)
    { if(((037777777777&u3[0])==0)&&(xwost<=0)) goto wixod;
	 else {  u3[1]=( u3[1]+1); goto wixod; }
    } 
     else 
     {
        if(((037777777777&u3[0])==0)&&(xwost>0)) 
	  {  u3[1]=( u3[1]-1); goto wixod; }        
       else  goto wixod; 
     } 
	 }
	 //------------------------------
	 else
     {   if(so1==0)
	 {	if(((037777777777&u3[0])==0)&&(xwost<0)) 
	 {u3[1]=(u3[1]-1); goto wixod; }
	  else goto wixod;
	 }
	 else
    {if(((037777777777&u3[0])==0)&&(xwost>=0)) goto wixod;
	 else {u3[1]=(u3[1]+1); goto wixod; }
	}  
     }
    wixod:  xwost=0; //Число можно вписывать в ответ из u3[1].
  }//=======================================================


short okrugd(ntip *k) 
             //Округляет двойное нормализ.число по условиям 
			 //в contrlstatus. Исходн.данные в pd1,m3,m4,sd1,
			 //порядок на входе несмещенный, неусловный. Тара
             //m3,m4 отражена в u3[],u4[]. Возвращ.при
             //переп.8, при ненормализ.рез-те 9,
             //при нормализованном  - 0. 
             //Подставляет результаты обратно.
             //в pd1,m3, обеспечивая беск.,
             //условный порядок ненормализ.числа. 
             //В нормализ.числе 20 разр.стерт!
		     //На выходе порядок несмещенный.
		     //После смещения порядка компоненты
             //числа можно вписывать в ответ.
		     // - при неразреш.прерывания.
   {if((m4!=0)||(xwost!=0)) netos=1; else netos=0;
 
    if((03&k->contrlstatus)==0)
	{ if((020000000000&u4[1])==0)    goto wixodd;          
	   //Теперь - 1 в старш. разряде m4. 
        if(((017777777777&u4[1])!=0)||(u4[0]!=0))
 {m3=m3+1; if((010000000&u3[1])!=0) {m3=m3>>1; pd1=pd1+1;}
		  goto wixodd; 
	}	//Теперь полухвост в m4 после старш.разряда нулевой. 
        //xwost в этом случае =0.
         if((01&m3)==0) goto wixodd;
		 else { m3=m3+1;  goto wixodd;}
	}
     else if((03&k->contrlstatus)==1) 
	 {if(sd1==0)
	 { if(m4!=0)   goto wixodd;
	     else if((m4==0)&&(xwost>=0)) goto wixodd;
		 else {m3=(m3-1); goto wixodd; }
	 }
	 else
	 {if(m4!=0) goto wixodd;
	    else if((m4==0)&&(xwost<=0)) goto wixodd;
		else {m3=(m3-1); goto wixodd; }
	 }
	 }
	 //--------------------------------------
     else if((03&k->contrlstatus)==2)
     { if(sd1==0)
       { if((m4==0)&&(xwost<=0)) goto wixodd;
	     else {  m3=( m3+1);  goto wixodd; }
       } 
       else 
	   {if((m4==0)&&(xwost>0))
	   { m3=(m3-1); goto wixodd; }
		  else goto wixodd; 
	   }
     }   
	 //-------------------------------------
     else 
     { if(sd1==0)
	 { if((m4==0)&&(xwost<0)) {m3=(m3-1); goto wixodd;}
	     else goto wixodd;
	 }
	 else
	 { if((m4==0)&&(xwost>=0)) goto wixodd;
	   else {m3=(m3+1); goto wixodd;}
	 }
     } //--Теперь учет ухода мантиссы влево от округления:
    wixodd:    xwost=0;     
	 if(010000000&u3[1])   //Выскочила в 21 разряд.
	   { m3=( m3>>1);  pd1=(pd1+1);  }
	   //----------- Теперь учет ухода вправо на 1 разряд. 
	     if((04000000&u3[1])==0)
		 {m3=(m3<<1);  pd1=(pd1-1); }
       //-------------------------------
	   if(pd1>=1024) //Переполнение. Делаются подстановки
				//в зависимости от реж.округления.
       {      if((03&k->contrlstatus)==0)
	       { pd1=1024;  m3=0; }          //Бесконечность.
	     else  if((03&k->contrlstatus)==1)
	       { pd1=1023; u3[0]=037777777777;
			   u3[1]=03777777; }  //Максимально большое.
	     else  if((03&k->contrlstatus)==2)
	       { if(sd1==1) { pd1=1023; u3[0]=037777777777;
					u3[1]=03777777; }   
		  else      { pd1=1024; m3=0;         }
	       }
	     else
	       { if(sd1==0) { pd1=1023; u3[0]=037777777777;
					u3[1]=03777777; }
		   else     { pd1=1024; m3=0; }
	       }	    
	 return(8);  
	}   //---------------------------			           
	  else if(pd1<-1022)         //Ненормализ.           
	{  pd1=-1023; 	       
	   u3[1]=0x200; u3[0]=0;   
		  
		  return(9);
	}  //---------------------------
	else {  //Нормализованное со штатным порядком.
	u3[1]=(037773777777&u3[1]);//Сброс подразумев.20 разр.
			  return(0);
	     }           	 
  }//====================================================


void okrugzd(ntip *k) //Округляет целое число по условиям 
              //в contrlstatus. Исходн.данные в m3,m4,sd1.
	          // Тара m3,m4 отражена в u3[],u4[].                                 
{                                          
	if((m4!=0)||(xwost!=0)) netos=1; else netos=0;
	if((03&k->contrlstatus)==0)  
     { 
	  if((020000000000&u4[1])==0)    goto wixodd;          
	   else
	{ if(sd1==0)
	{if(((017777777777&u4[1])==0)&&(u4[0]==0)&&(xwost<0))
	               goto wixodd;
	 }
	    if(sd1==1)
	{if(((017777777777&u4[1])==0)&&(u4[0]==0)&&(xwost>0))
		           goto wixodd;
	}
		{m3=(m3+1); goto wixodd; }
	   }           
	   }
	//------------------------------------
     else if((03&k->contrlstatus)==1) 
	 {if(sd1==0)
	 { if(m4!=0)   goto wixodd;
	     else if((m4==0)&&(xwost>=0)) goto wixodd;
		 else {m3=(m3-1); goto wixodd; }
	 }
	 else
	 {if(m4!=0) goto wixodd;
	    else if((m4==0)&&(xwost<=0)) goto wixodd;
		else {m3=(m3-1); goto wixodd; }
	 }
	 }
	 //--------------------------------------
     else if((03&k->contrlstatus)==2)
     { if(sd1==0)
       { if((m4==0)&&(xwost<=0)) goto wixodd;
	 else {  m3=( m3+1); goto wixodd; }
       } 
       else 
	   {if((m4==0)&&(xwost>0))
	   { m3=(m3-1); goto wixodd; }
		  else goto wixodd; 
	   }
     }   
	 //-------------------------------------
     else 
     { if(sd1==0)
	 { if((m4==0)&&(xwost<0)) {m3=(m3-1); goto wixodd;}
	     else goto wixodd;
	 }
	 else
	 { if((m4==0)&&(xwost>=0)) goto wixodd;
	 else {m3=(m3+1); goto wixodd; }
	 }
	 }
    wixodd: xwost=0; //Число можно вписывать в ответ из m3.    
  }//====================================================


 int  sraw(char s,short us,short nan,UINT r1,UINT r2,ntip *k)
 //Сравнивает два числа. s - 'o' или 'd (одинарн.или двойное).
 //us  : 0 (нет условия), 1 (если < ), 2 (если =), 3 (если <= ).
 //nan : 0 (если должны быть только числа или бескон.),
 // 1 (если наличие нечисла уже обеспечивает выполн.условия).
 // r1 - номер рег.первого числа, r2 - второго.
 //Возвращает: -1 при невыпол. условия и отсутствии нечисел.
 //            -2 при невыполнении условия и наличии нечисел.
     //         1 при выполнении условия и отсутствии нечисел.        
     //         2 при выполнении условия и наличии нечисел.
    { short pn,b2; // UINT s1,s2;
		 if(s=='o')      //Одинарные числа.
     { s1=kto(r1,'o',1,k); s2=kto(r2,'o',2,k); //Взяли числа.
       if((s1>=6)||(s2>=6)) pn=1; else pn=0; //Признак нечисла.
       if((nan==0)&&(pn==1))   return(-2);
       if((nan==1)&&(pn==1))   return(2);
       if((nan==1)&&(pn==0)&&(us==0))   return(-1);
       //------------Теперь только сравнение чисел и бескон:
	 if(s1==2) so1=0; if(s2==2) so2=0;  //Исправили -0 на +0.
      if((so1==so2)&&(p1==p2)&&(f1==f2))    //Числа равны.
      { if((us==2)||(us==3))  return(1); else return(-1); }
	 //----------Теперь только неравные числа и бескон:
	 if((so1==1)&&(so2==0))      b2=1; //Т.е. 2-е число больше.
    else if((so1==0)&&(so2==1))      b2=0;   //Т.е. оно меньше.
    else if((so1==0)&&(so2==0))      //Оба положительны.
	{      if(p2>p1) b2=1;	  
	  else if(p2<p1) b2=0;
	  else     { if(f2>f1) b2=1; else b2=0; }
	}    //----Теперь оба отрицательны:
     else {      if(p2>p1)   b2=0;
	    else if(p2<p1)   b2=1;
	    else   { if(f2>f1) b2=0; else b2=1; }
	  } //------------Отношение чисел определили в b2.
   if((b2==0)&&((us==1)||(us==3))) return(1); else return(-1);
     } //--------Теперь с двойной точностью:
   else 
    {  s1=kto(r1,'d',1,k);   s2=kto(r2,'d',2,k); //Взяли числа.
       if((s1>=6)||(s2>=6)) pn=1; else pn=0; //Признак нечисла.
       if((nan==0)&&(pn==1))   return(-2);
       if((nan==1)&&(pn==1))   return(2);
       if((nan==1)&&(pn==0)&&(us==0))   return(-1);
       //------------Теперь только сравнение чисел и бескон:
	 if(s1==2) sd1=0; if(s2==2) sd2=0;  //Исправили -0 на +0.
      if((sd1==sd2)&&(pd1==pd2)&&(fd1==fd2))    //Числа равны.
      { if((us==2)||(us==3))  return(1); else return(-1); }
	 //----------Теперь только неравные числа и бескон:
	 if((sd1==1)&&(sd2==0))     b2=1; //Т.е. 2-е число больше.
    else if((sd1==0)&&(sd2==1))    b2=0;   //Т.е. оно меньше.
    else if((sd1==0)&&(sd2==0))      //Оба положительны.
	{      if(pd2>pd1) b2=1;	  
	  else if(pd2<pd1) b2=0;
	  else     { if(fd2>fd1) b2=1; else b2=0; }
	}    //----Теперь оба отрицательны:
     else {      if(pd2>pd1)   b2=0;
	    else if(pd2<pd1)   b2=1;
	    else   { if(fd2>fd1) b2=0; else b2=1; }
	  } //------------Отношение чисел определили в b2.
	if((b2==0)&&((us==1)||(us==3))) return(1); else return(-1);
     } 
    }//=====================================================


   short op0(ntip *k) //Исполняет команду из kodkom, если op=0.
   {  UINT rab,rab1,rab2,rab3,s1,s2,s3;  int i;
      UINT *uk; int *ukint;                                                 
      LONGLONG zn1,a,b; DWORDLONG bzn1,c,d;
			//Делает абс.все. Возвращает: 0-успех,
			//2-искл.ситуация, 3-моя ошибка.
      int r1,r2,rd,sa,x,y,*ui;
		  rab=kodkom;   //Взяли код команды.
    sa=rab&03700; sa=(sa>>6);  rd=rab&0174000; rd=(rd>>11);
   r2=rab&07600000; r2=(r2>>16); r1=rab&0370000000; r1=(r1>>21);
      //----------- Оформили номера регистров как индексы.
      //---------------ПОШЕЛ РАЗБОР:
   if(((077&rab)==0)&&(r1==0)&&(r2==0)&&(rd==0)&&(sa==0))//nop
		            goto kkk;
	   //---------------------------------------------------
        if((077&rab)==0)    //Это SLL
       { rab1=k->zp[r2]; rab1=(rab1<<sa);
		     k->zp[rd]=rab1; goto kkk;
       } //-----------------Исполнили SLL.
        if((077&rab)==2)    //Это SRL
       { rab1=k->zp[r2]; rab1=(rab1>>sa);
		     k->zp[rd]=rab1; goto kkk;
       } //-----------------Исполнили SRL
        if((077&rab)==3)    //Это SRA
       { rab1=k->zp[r2];
		if(sa==0) k->zp[rd]=rab1;       //Нет сдвига.
	     else { if((020000000000&rab1)==0)     
		  rab2=0; else rab2=020000000000; 
		rab1=(rab1>>sa); //Сдвиг самого кода.
		 rab3=rab2;   for(i=1;i<sa;i++)
		 { rab2=(rab2>>1); rab3=(rab3|rab2);}
			    //Размножили знаковый разряд.
		rab1=(rab1|rab3);  k->zp[rd]=rab1;
	      }                                  goto kkk;
       }//--------------------Исполнили SRA.
        if((077&rab)==4)    //Это SLLV
	{ rab1=k->zp[r2]; rab2=k->zp[r1]; rab2=(037&rab2);
	  rab1=(rab1<<rab2);   k->zp[rd]=rab1;   goto kkk;
	}//-------------------Исполнили SLLV
	 if((077&rab)==6)    //Это SRLV
	{ rab1=k->zp[r2]; rab2=k->zp[r1]; rab2=(037&rab2);
	  rab1=(rab1>>rab2);   k->zp[rd]=rab1;   goto kkk;
	}//-------------------Исполнили SRLV
	 if((077&rab)==7)    //Это SRAV
	{ rab1=k->zp[r2]; rab2=k->zp[r1]; sa=(037&rab2);
		if(sa==0) k->zp[rd]=rab1;    //Нет сдвига.
	 else { if((020000000000&rab1)==0)     
		  rab2=0; else rab2=020000000000; 
		rab1=(rab1>>sa); //Сдвиг самого кода.
		 rab3=rab2;   for(i=1;i<sa;i++)
		 { rab2=(rab2>>1); rab3=(rab3|rab2);}
			    //Размножили знаковый разряд.
		rab1=(rab1|rab3);  k->zp[rd]=rab1;
	      }                                     goto kkk;
       }//--------------------ИсполнилиSRLV
	 if((077&rab)==8)    //Это JR   -   переход безусл.
	{ if(k->predper==0) 
	 {
	     k->adrper=k->zp[r1];
         k->pc=(k->pc+4); k->predper=1;
         return(0);
	 }

	else
	{k->predper=0; 
     k->pc=k->adrper;
      return(0);
	}
	}//-------------------Исполнили JR

	 if((077&rab)==9)  //Это JALR - переход безусл с возвратом. 
	{		
		 if(k->predper==0) 
{ k->pc=(k->pc+4);
		 k->adrper=k->zp[r1];
	if(rd!=0)	 k->zp[rd]=k->pc+4; //Зап.адреса возврата.
	 else        k->zp[31]=k->pc+4; //Зап.адреса возврата.
    //Работа для nexti:
      if(k->wzwr==0)
	  {k->wzwr=1; k->adrwzwr=k->pc+4;}
    //--------------------------------
	 k->predper=1; return(0); 
}
else{
	  k->pc=k->adrper;
	   k->predper=0; return(0);}
	}//-------------------------------------------------Исполнили JALR


	 if((077&rab)==12)   //Это SYSCALL
	{
      if(k->wzwr==0)                     //Правка для nexti ! 6.04.07.
	  {k->wzwr=1; k->adrwzwr=k->pc+4;}		 
		 stca(8,k,0,0);  
		   return(2);
	}//-------------------Исполнили SYSCALL
	 if((077&rab)==13)   //Это BREAK
	 {   
	  stca(9,k,0,0); return(2);}
	 //-------------------Исполнили BREAK
    
	 if((077&rab)==16)   //Это MFHI
	{ k->zp[rd]=k->hi;                        goto kkk; }
	//--------------------Исполнили MFHI
	 if((077&rab)==17)   //Это MTHI
	{ k->hi=k->zp[r1];                        goto kkk; }
	//--------------------Исполнили MTHI
	 if((077&rab)==18)   //Это MFLO
	{ k->zp[rd]=k->lo;                        goto kkk; }
	//--------------------Исполнили MFLO
	 if((077&rab)==19)   //Это MTLO
	{ k->lo=k->zp[r1];                        goto kkk; }
	//--------------------Исполнили MTLO
	 if((077&rab)==24)   //Это MULT
	{  uk=&k->zp[r1]; ui=(int *)uk; x=ui[0];
       uk=&k->zp[r2]; ui=(int *)uk; y=ui[0];
		 a=(LONGLONG)x; b=(LONGLONG)y; 
	   zn1=a*b; uk=(UINT *)(&zn1);
	   k->lo=uk[0]; k->hi=uk[1];              goto kkk; }
	//--------------------Исполнили MULT
	 if((077&rab)==25)   //Это MULTU
	{  c=(DWORDLONG)k->zp[r1]; d=(DWORDLONG)k->zp[r2]; 
	   bzn1=c*d ; uk=(UINT *)(&bzn1);
	   k->lo=uk[0]; k->hi=uk[1];              goto kkk; }
	//--------------------Исполнили MULTU

      
      if((077&rab)==26)   //Это DIV
{uk=&(k->zp[r1]); ukint=(int *)uk;  ss1=ukint[0];
 uk=&(k->zp[r2]); ukint=(int *)uk;  ss2=ukint[0];
    if((ss2!=0)&&((ss1!=0x80000000)||(ss2!=-1) ))
	{ss3=ss1/ss2;     ss4=ss1%ss2;
     ukint=&ss3; uk=(UINT *)ukint; k->lo=uk[0];      
     ukint=&ss4; uk=(UINT *)ukint; k->hi=uk[0];
	}
   goto kkk;
}
//--------------------Исполнили DIV

	 if((077&rab)==27)   //Это DIVU
	{ s1=k->zp[r1]; s2=k->zp[r2];
	  if(s2!=0)
	  { s3=s1/s2;     s4=s1%s2;
	  k->lo=s3;    k->hi=s4; }                 goto kkk; }
	//--------------------Исполнили DIVU

	 if((077&rab)==32)   //Это ADD 
	{int i1,i2,i3; int *ui1;int *ui2;
	
	 ui1=(int *)&k->zp[r1];
	 ui2=(int *)&k->zp[r2];
	      i1=ui1[0]; i2=ui2[0];
                i3=i1+i2;
          k->zp[rd]=(UINT)i3;
     if(i1==(i3-i2))  goto kkk;
   
	      //Иначе переполнение.
	     { stca(12,k,0,0); return(2); }
	}//--------------------Исполнили ADD
	 if((077&rab)==34)   //Это SUB
	 {
	  int i1,i2,i3; int *ui1;int *ui2; 	
	 ui1=(int *)&k->zp[r1];
	 ui2=(int *)&k->zp[r2];
	 
	      i1=ui1[0]; i2=ui2[0];
                i3=i1-i2;
          k->zp[rd]=(UINT)i3;
     if(i1==(i2+i3))  goto kkk;
	      //Иначе переполнение.
	     { stca(12,k,0,0); return(2); }
	}//--------------------Исполнили SUB



	 if((077&rab)==33)   //Это ADDU
	{ s1=k->zp[r1]; s2=k->zp[r2]; s3=s1+s2; k->zp[rd]=s3;
					     goto kkk;
	}//--------------------Исполнили ADDU

	 if((077&rab)==35)   //Это SUBU
	{ s1=k->zp[r1]; s2=k->zp[r2]; s3=s1-s2; k->zp[rd]=s3;
					     goto kkk;
	}//--------------------Исполнили SUBU
	 if((077&rab)==36)   //Это AND
	{ s1=k->zp[r1]; s2=k->zp[r2]; s3=s1&s2; k->zp[rd]=s3;
					     goto kkk;
	}//--------------------Исполнили AND
	 if((077&rab)==37)   //Это OR
	{ s1=k->zp[r1]; s2=k->zp[r2]; s3=s1|s2; k->zp[rd]=s3;
					     goto kkk;
	}//--------------------Исполнили OR
	 if((077&rab)==38)   //Это XOR
	{ s1=k->zp[r1]; s2=k->zp[r2]; s3=s1^s2; k->zp[rd]=s3;
					     goto kkk;
	}//--------------------Исполнили XOR

	 if((077&rab)==39)   //Это NOR
	{ s1=k->zp[r1]; s2=k->zp[r2]; s3=s1|s2;
	         s3=(037777777777^s3);
	  k->zp[rd]=s3;
					     goto kkk;
	}//--------------------Исполнили NOR
	 if((077&rab)==42)   //Это SLT
	{
	  int i1,i2; int *ui1;int *ui2; 	
	 ui1=(int *)&k->zp[r1];
	 ui2=(int *)&k->zp[r2];
	 
	      i1=ui1[0]; i2=ui2[0];
        if(i1<i2) s3=1; else s3=0;        
          k->zp[rd]=(UINT)s3;		 
		 
		  goto kkk;
	}//---------------------Исполнили SLT
	 if((077&rab)==43)   //Это SLTU
	{ s1=k->zp[r1]; s2=k->zp[r2];
	  if(s1<s2) s3=1; else s3=0;
	  k->zp[rd]=s3;                         goto kkk;
	}//----------------------Исполнили SLTU

	 
	       //Если непредусмотренная:
	  soob.sprintf
 ("#E - PROZ - Непредусмотр.команда %o по адресу %X",kodkom,k->pc);
	    if(prz) printkl(soob);
      stca(10,k,0,0);   return(4);
   kkk:    if(k->predper==0)  k->pc=(k->pc+4);
		else {k->pc=(k->pc-4);  }
		//Установ pc в беспереходн.команде.
	 return(0);					       
   }//====================================================


   short op1(ntip *k) //Исполняет команду из kodkom, если op=1.
   {  	              //Делает абс.все. Возвращает: 0-успех,
			//2-искл.ситуация, 3-моя ошибка.
     UINT rab,rab1,rab2;
     int r1,r2,off;

	 rab=kodkom;   //Взяли код команды.
 r2=rab&07600000; r2=(r2>>16); r1=rab&0370000000; r1=(r1>>21);
      //----------- Оформили номера регистров как индексы.
       off=0177777&rab;     //Вычислили offset.

      //----------------------ПОШЕЛ РАЗБОР:
      if(r2==0)           //Это BLTZ  - усл. переход
       {    if(k->predper==0) 
		 { k->pc=(k->pc+4); //Зап.адреса след. ком.
		     if(0100000&off) 
		     rab1=037777600000|off; else rab1=off;
		     rab1=(rab1<<2);
		     rab2=k->pc+rab1;//Это адрес возм. перехода.
           if(020000000000&(k->zp[r1])) k->adrper=rab2;
	                 	         else k->adrper=k->pc+4;
		   k->predper=1; return(0); }
	    else { k->predper=0;
		       k->pc=k->adrper;
		     return(0);
		  } }//----------------Исполнили BLTZ
       if(r2==1)           //Это BGEZ  - усл. переход
       {    if(k->predper==0) 
		 { k->pc=(k->pc+4); //Зап.адреса след. ком.
		     if(0100000&off) 
		     rab1=037777600000|off; else rab1=off;
		     rab1=(rab1<<2);
		     rab2=k->pc+rab1;//Это адрес возм.перехода.
          if((020000000000&(k->zp[r1]))==0)  k->adrper=rab2;
		                             else k->adrper=k->pc+4;
		   k->predper=1; return(0); }
	    else { k->predper=0;
		       k->pc=k->adrper; 
		     return(0);
		  } }//----------------Исполнили BGEZ
       if(r2==16)     //Это BLTZAL  - усл. переход с возвратом
       {    if(k->predper==0) 
		{ k->pc=(k->pc+4); //Зап.адреса след. ком.
           k->zp[31]=k->pc+4;//Запись адр.второй ком (возврата).
    //Работа для nexti:
      if(k->wzwr==0)
	  {k->wzwr=1; k->adrwzwr=k->pc+4;}
    //--------------------------------
		     if(0100000&off) 
		     rab1=037777600000|off; else rab1=off;
		     rab1=(rab1<<2);
		     rab2=k->pc+rab1;//Это адрес возм.перехода.
           if(020000000000&(k->zp[r1]))  k->adrper=rab2;
		                         else k->adrper=k->pc+4;
		   k->predper=1; return(0); }
	    else { k->predper=0;
		     k->pc=k->adrper;
		     return(0);
		  } }//----------------Исполнили BLTZAL
       if(r2==17)   //Это BGEZAL  - усл. переход с возвратом
       {    if(k->predper==0) 
		{ k->pc=(k->pc+4); //Зап.адреса след. ком.
           k->zp[31]=k->pc+4; //Запись адр.второй ком (возврата).
    //Работа для nexti:
      if(k->wzwr==0)
	  {k->wzwr=1; k->adrwzwr=k->pc+4;
	  }
    //--------------------------------
		     if(0100000&off) 
		     rab1=037777600000|off; else rab1=off;
		     rab1=(rab1<<2);
		     rab2=k->pc+rab1;//Это адрес возм.перехода.
           if((020000000000&(k->zp[r1]))==0) k->adrper=rab2;
		                             else k->adrper=k->pc+4;
		   k->predper=1; return(0); }
	    else { k->predper=0;
		     k->pc=k->adrper;
		     return(0);
		        } }//----------------Исполнили BGEZAL

	       //Если непредусмотренная:
	  soob.sprintf
("#E - PROZ - Непредусмотр.команда %o по адресу %X",kodkom,k->pc);
	    if(prz) printkl(soob);
      stca(10,k,0,0);   return(4);
   }//=====================================================
       

     short op4do15(short op,ntip *k) //Выполняет команды
    {  UINT rab,rab1,rab2,s1,s2,s3;    //с кодами 4 - 15.
       int r1,r2,off,rr1,rr2,rr3;
		  rab=kodkom;   //Взяли код команды.
      r2=rab&07600000; r2=(r2>>16);
	  r1=rab&0370000000; r1=(r1>>21);
      //--------- Оформили номера регистров как индексы.
       off=0177777&rab;     //Вычислили offset.
      //------------------------- Компоненты определены.
      if(op==4)          //Это BEQ, усл. переход.
       {    if(k->predper==0) 
		{ k->pc=(k->pc+4); //Зап.адреса след. ком.
		     if(0100000&off) 
		     rab1=037777600000|off; else rab1=off;
		     rab1=(rab1<<2);
		     rab2=k->pc+rab1;//Это адрес возм.перехода.
           if(k->zp[r1]==k->zp[r2]) k->adrper=rab2;
		                    else k->adrper=k->pc+4;
		   k->predper=1; return(0); }
	    else { k->predper=0;
		       k->pc=k->adrper;
		     return(0);
		  } 
	}//---------------------------Исполнили BEQ
       if(op==5)          //Это BNE, усл. переход.
   {    if(k->predper==0) 
	{ k->pc=(k->pc+4); //Зап.адреса след. ком.
		     if(0100000&off) 
		     rab1=037777600000|off; else rab1=off;
		     rab1=(rab1<<2);
		     rab2=k->pc+rab1;//Это адрес возм.перехода.
           if(k->zp[r1]!=k->zp[r2]) k->adrper=rab2;
		                     else k->adrper=k->pc+4;
		   k->predper=1; return(0); }
	    else { k->predper=0;
		       k->pc=k->adrper; 
		     return(0);
		  } 
	}//---------------------------Исполнили BEQ
       if(op==6)          //Это BLEZ, усл. переход.
       {    if(k->predper==0) 
		{ k->pc=(k->pc+4); //Зап.адреса след. ком.
		     if(0100000&off) 
		     rab1=037777600000|off; else rab1=off;
		     rab1=(rab1<<2);
		     rab2=k->pc+rab1;//Это адрес возм.перехода.
       if((k->zp[r1]==0)||((020000000000&(k->zp[r1]))!=0))
			   k->adrper=rab2; else k->adrper=k->pc+4;
		   k->predper=1; return(0); }
	    else { k->predper=0;
	           k->pc=k->adrper; 
		     return(0);
		  } 
	}//---------------------------Исполнили BLEZ
       if(op==7)          //Это BGTZ, усл. переход.
       {    if(k->predper==0) 
		{ k->pc=(k->pc+4); //Зап.адреса след. ком.
		     if(0100000&off) 
		     rab1=037777600000|off; else rab1=off;
		     rab1=(rab1<<2);
		     rab2=k->pc+rab1;//Это адрес возм.перехода.
       if((k->zp[r1]!=0)&&((020000000000&(k->zp[r1]))==0))
            k->adrper=rab2; else k->adrper=k->pc+4;
		   k->predper=1; return(0); }
	    else { k->predper=0;
	           k->pc=k->adrper;
		     return(0);
		  } 
	}//---------------------------Исполнили BGTZ
       if(op==8)          //Это ADDI 
     { if(0100000&off) s1=(UINT)037777600000|off;
	   else s1=(UINT)off;
	         s2=k->zp[r1]; s3=s1+s2; 	
	  if((020000000000&s1)!=0) ss1=1; else ss1=0;
	  if((020000000000&s2)!=0) ss2=1; else ss2=0;
      if((020000000000&s3)!=0) ss3=1; else ss3=0;
	     if(ss1!=ss2)           {k->zp[r2]=s3;   goto kkk;}
		 if((ss1==0)&&(ss3==0)) {k->zp[r2]=s3;   goto kkk;}
		 if((ss1==1)&&(ss3==1)&&((017777777777&s3)!=0))
								{k->zp[r2]=s3;   goto kkk;}
	      //Иначе переполнение.
	     { stca(12,k,0,0); return(2); }

      }//------------------------------Исполнили ADDI

       if(op==9)          //Это ADDIU 
     { if(0100000&off) rr1=037777600000|off; else rr1=off;
	rr2=k->zp[r1]; rr3=rr1+rr2;  
	   k->zp[r2]=(UINT)rr3;
	   goto kkk; 	 
      }//------------------------------Исполнили ADDIU
       if(op==10)          //Это SLTI                             
     { if(0100000&off) rr1=037777600000|off; else rr1=off;
	rr2=k->zp[r1]; if(rr2<rr1) k->zp[r2]=1;                 
		    else k->zp[r2]=0;            goto kkk; 	 
      }//------------------------------Исполнили SLTI
       if(op==11)          //Это SLTIU 
     { if(0100000&off) rab=037777600000|off; else rab=off;
	rab1=k->zp[r1]; if(rab1<rab) k->zp[r2]=1;               
		    else k->zp[r2]=0;            goto kkk; 	 
      }//------------------------------Исполнили SLTIU
       if(op==12)          //Это ANDI
      { rab=k->zp[r1]; k->zp[r2]=rab&off;  goto kkk; }
      //-------------------------------Исполнили ANDI
       if(op==13)          //Это ORI
      { rab=k->zp[r1]; k->zp[r2]=rab|off;  goto kkk; }
      //-------------------------------Исполнили ORI
       if(op==14)          //Это XORI
      { rab=k->zp[r1]; k->zp[r2]=rab^off;  goto kkk; }
      //-------------------------------Исполнили XORI
       if(op==15)          //Это LUI
      { k->zp[r2]=(off<<16);              goto kkk; }
      //-------------------------------Исполнили LUI
	  

	       //Если непредусмотренная:
	  soob.sprintf
("#E - PROZ - Непредусмотр.команда %o по адресу %X",kodkom,k->pc);
	    if(prz) printkl(soob);
      stca(10,k,0,0);   return(4);

   kkk:

	  
	  if(k->predper==0)  k->pc=(k->pc+4);
		 else {k->pc=(k->pc-4);  } 
	  //Установ pc в беспереходн.команде.
	 return(0);					       
     } //=================================================

    short op16(ntip *k)             //Код команды 16.
    {  UINT rab,rab1;           
       int r1,r2,rd,func;
		  rab=kodkom;   //Взяли код команды.
      func=077&rab;	rd=rab&0174000; rd=(rd>>11);
 r2=rab&07600000; r2=(r2>>16); r1=rab&0370000000; r1=(r1>>21);
      //--------- Оформили 5-ти разрядные комп. как индексы.
      if(r1==0)      //Это MFC0   - чтение рег.CP0.
      { if(((02000000000&(k->status))==0)&&(k->jadro==1))
	{stca(11,k,0,0); return(2); } //Исключ.11.Недоступен CP0.
	     if(rd==3)   k->zp[r2]=k->config;
    else if(rd==0)   k->zp[r2]=k->index;
	else if(rd==4)   k->zp[r2]=k->context;
	else if(rd==1)   k->zp[r2]=k->random;
	else if(rd==15)  k->zp[r2]=k->prid;
    else if(rd==2)   k->zp[r2]=k->entrylo;
	else if(rd==8)   k->zp[r2]=k->badvaddr;
    else if(rd==10)  k->zp[r2]=k->entryhi;
	else if(rd==12)  k->zp[r2]=k->status;
	else if(rd==13)  k->zp[r2]=k->cause;
	else if(rd==14)  k->zp[r2]=k->epc;
	  else
	  {soob.sprintf
("#E - PROZ - Ошибка номера =%d ? рег.сопроцессора CPO",rd);  
	       printkl(soob); return(3);  }
	 goto kkk;
      }//--------------------Исполнили MFC0
       if(r1==4)      //Это MTC0 - запись в рег.CP0.                 
{  if(((02000000000&(k->status))==0)&&(k->jadro==1))
	   {soob.sprintf("# MTC0. Исключ.11.Недоступен CP0. Адрес команды %x",k->pc); printkl(soob);  
		   stca(11,k,0,0);
	   
	   return(2);
	} //Исключ.11.Недоступен CP0.
	     if(rd==3)       //Это config
	{if((020000000000&k->config)==0)
		 {k->config=k->zp[r2]; 
          rab=k->config; rab=(rab>>26); rab=(07&rab);
		    if(rab==0) k->zapcp1=02000;
            else if(rab==1) k->zapcp1=04000;
            else if(rab==2) k->zapcp1=010000;
            else if(rab==3) k->zapcp1=020000;
            else if(rab==4) k->zapcp1=040000;
            else if(rab==5) k->zapcp1=0100000;

            else
			{ printkl("#E - PROZ -Ошибка записи в confif.Отказ.");ExitThread(TRUE);}  
    //----------Определили поз.номер запроса от CP1 по config.		 
		 }
	     else 
		 {soob.sprintf("#E - PROZ - Попытка записи в config при запрете в 31 разряде");
		 printkl(soob);
		 }
	}
    else if(rd==0)   k->index=k->zp[r2];
	else if(rd==4)   k->context=k->zp[r2];
	else if(rd==1)   k->random=k->zp[r2];
	else if(rd==15)  k->prid=k->zp[r2];
	else if(rd==8)   k->badvaddr=k->zp[r2];
    else if(rd==2)   k->entrylo=k->zp[r2];
    else if(rd==10)  k->entryhi=k->zp[r2];
	else if(rd==12) 
{//k->status=k->zp[r2];  //было  20.08.2009.
 k->status=xitpr(k->status,k->zp[r2],0xf257ff3f,0x1a80000,0xc0000c0,0,0,957);
	    k->axtung=1|k->axtung;
/*                                 //Изъято 20.08.2009. как атавизм    
	     if(k->predper==1)
		 { k->predper=0;
	           k->pc=k->adrper;
		     return(0);
		 }
*/
	
}
	else if(rd==13)  //Запись только в разр.8 и 9 Cause:
					{ k->cause=(037777776377&k->cause); rab1=k->zp[r2];
	                 rab1=(01400&rab1); k->cause=(rab1|k->cause);
					 k->axtung=1|k->axtung;
					}
	else if(rd==14)  k->epc=k->zp[r2];
	  else  {  soob.sprintf
	         ("#E - PROZ - Ошибка номера =%d ? рег.сопроцессора CPO",rd);  
             printkl(soob);	       return(3);
			}
	 goto kkk;
      }//--------------------Исполнили MTC0
      if((r1==16)&&(func==16))    //Это RFE
      {rfee=1;   //Признак исполнения RFE. Он учтется в zakr и сбросится в начале следующей операции 
        if(((02000000000&(k->status))==0)&&((02&(k->status))!=0))
		{soob.sprintf("# RFE. Исключ.11.Недоступен CP0. Адрес команды %x",k->pc); printkl(soob);  //&&&&&&&&&&&&&&&&&
			stca(11,k,0,0);   
        return(2); } //Исключ.11.Недоступен CP0.

	rab=k->status; rab=(rab>>2); rab=(017&rab);
	rab1=060&(k->status); rab=(rab1|rab);
	k->status=(037777777700&(k->status));
	k->status=(rab|(k->status));
/*
	     if(k->predper==1)                                                          
		 { k->predper=0; 
	           k->pc=k->adrper;
		     return(0);
		 }
*/

  goto kkk; 
      } //---------------------Исполнили RFE

	       //Если непредусмотренная:
	  soob.sprintf
 ("#E - PROZ - Непредусмотр.команда %o по адресу %X",kodkom,k->pc);
	    if(prz) printkl(soob);
	   stca(10,k,0,0);   return(4);

	      //------------------------
   kkk:   

	   if(k->predper==0)  k->pc=(k->pc+4);
		 else {k->pc=(k->pc-4);  }
		 //Установ pc в беспереходн.команде.
		 return(0);					       
    }//======================================================


   short op32do57(short op,ntip *k)  //Коды 32 - 57.
    {  UINT rab,rab1,rab2,rab3,otw,ad,adr;       
       UINT r1,r2,off,offr;   short kesk,ot,ot1;


		  rab=kodkom;   //Взяли код команды.
  r2=rab&07600000; r2=(r2>>16); r1=rab&0370000000; r1=(r1>>21);
      //----------- Оформили номера регистров как индексы.
     off=0177777&rab;   if(0100000&off)
     offr=037777600000|off; else offr=off; //offr - расширен.
      //---------------------------- Компоненты определены.

      if(op==32)          //Это LB.
      { rab1=k->zp[r1]+offr;  //Это адрес.
	otw=stb(rab1,k);     //Взяли байт в sislo.
    if(otw==4) { stca(4,k,rab1,0); return(2); } //Исключ. 4.
//	if(otw==5) { stca(5,k,rab1,0); return(2); } //Исключ. 5.
	if(otw==7) { stca(7,k,0,0);    return(2); } //Исключ. 7.
	if(0200&sislo) rab1=037777777400|sislo;
	else rab1=0377&sislo; k->zp[r2]=rab1;      goto kkk;
      } //--------------------Исполнили LB.
       if(op==33)          //Это LH.
      { rab1=k->zp[r1]+offr;  //Это адрес.
	if((1&rab1)!=0) {

		stca(4,k,rab1,0); return(2);} //Исключ.4
	otw=stp(rab1,k);     //Взяли полуслово в sislo.
   if(otw==4) {stca(4,k,rab1,0);   return(2);} //Исключ. 4. 
//   if(otw==5) {stca(5,k,rab1,0);   return(2);} //Исключ. 5.
  if(otw==7) { stca(7,k,0,0);      return(2);} //Исключ. 7.
	if(0100000&sislo) rab1=037777600000|sislo;
	else rab1=0177777&sislo; k->zp[r2]=rab1;      goto kkk;
      } //--------------------Исполнили LH.
       if(op==34)         //Это LWL
      { rab1=k->zp[r1]+offr;       //Это исходный адрес.
	rab2=037777777774&rab1; rab1=(rab1-rab2); //дельта.
	otw=sts(rab2,k);     //Взяли слово в sislo.
    if(otw==4) { stca(4,k,rab2,0); return(2);} //Исключ. 4.
//	if(otw==5) { stca(5,k,rab2,0); return(2);} //Исключ. 5.
	if(otw==7) { stca(7,k,0,0);    return(2);} //Исключ. 7.
	      if(rab1==0)
	{ sislo=(sislo<<24);  rab2=077777777&(k->zp[r2]);
	  k->zp[r2]=rab2|sislo;   goto kkk;  }
	 else if(rab1==1)
	{ sislo=(sislo<<16);  rab2=0177777&(k->zp[r2]);
	  k->zp[r2]=rab2|sislo;   goto kkk;  }
	 else if(rab1==2)
	{ sislo=(sislo<<8);  rab2=0377&(k->zp[r2]);
	  k->zp[r2]=rab2|sislo;   goto kkk;  }
	  else { k->zp[r2]=sislo; goto kkk;  }
      }//----------------------Исполнили LWL
       if(op==35)           //Это LW
      {
		   rab1=k->zp[r1]+offr;       //Это исходный адрес.
 if((03&rab1)!=0){
	         /*   soob.sprintf("Исполнение LW в BM%d:",nomerWM-1); printkl(soob);
	              soob.sprintf("LW !!!!!!!!!!!! некорректный адрес=%x",rab1); printkl(soob); //***
				  soob.sprintf("   Смещение offset=%x",offr); printkl(soob);
   				  soob.sprintf("   Значение на рег.%d =%x",r1,k->zp[r1]); printkl(soob);
                  soob.sprintf("Повторяем код команды LW =%x",kodkom); printkl(soob);
				  soob.sprintf("Повторяем адрес команды LW =%x",k->pc); printkl(soob);
                  printkl("-----------------------------");
             */
	              stca(4,k,rab1,0); return(2);
				}//Исключ.4
          	otw=sts(rab1,k);     //Взяли слово в sislo.
	     
    if(otw==4) {
		        
		         stca(4,k,rab1,0); return(2);
			} //Исключ. 4.
	if(otw==7) {
		        
		         stca(7,k,0,0);    return(2);
			} //Исключ. 7.
	     k->zp[r2]=sislo;            goto kkk;
      }//-----------------------Исполнили LW
       if(op==36)          //Это LBU
      { rab1=k->zp[r1]+offr;       //Это исходный адрес.
	otw=stb(rab1,k);     //Взяли байт в sislo.
    if(otw==4) { stca(4,k,rab1,0); return(2);} //Исключ. 4.
//	if(otw==5) { stca(5,k,rab1,0); return(2);} //Исключ. 5.
	if(otw==7) { stca(7,k,0,0);    return(2);} //Исключ. 7.
	     k->zp[r2]=0377&sislo;            goto kkk;
      }//----------------------Исполнили LBU
       if(op==37)         //Это LHU
      { rab1=k->zp[r1]+offr;       //Это исходный адрес.
 if((1&rab1)!=0) {stca(4,k,rab1,0); return(2);} //Исключ.4
	otw=stp(rab1,k);     //Взяли полуслово в sislo.
    if(otw==4) { stca(4,k,rab1,0); return(2);} //Исключ. 4.
//	if(otw==5) { stca(5,k,rab1,0); return(2);} //Исключ. 5.
	if(otw==7) { stca(7,k,0,0);    return(2);} //Исключ. 7.
	     k->zp[r2]=0177777&sislo;            goto kkk;
      }//----------------------Исполнили LHU

       if(op==38)         //Это  LWR
      { rab1=k->zp[r1]+offr;       //Это исходный адрес.
	rab2=037777777774&rab1; rab1=(rab1-rab2); //дельта.
	otw=sts(rab2,k);     //Взяли слово в sislo.
    if(otw==4) { stca(4,k,rab2,0); return(2);} //Исключ. 4.
//	if(otw==5) { stca(5,k,rab2,0); return(2);} //Исключ. 5.
	if(otw==7) { stca(7,k,0,0);    return(2);} //Исключ. 7.
	      if(rab1==3)
	{ sislo=(sislo>>24);  rab2=037777777400&(k->zp[r2]);
	  k->zp[r2]=rab2|sislo;   goto kkk;  }
	 else if(rab1==2)
	{ sislo=(sislo>>16);  rab2=037777600000&(k->zp[r2]);
	  k->zp[r2]=rab2|sislo;   goto kkk;  }
	 else if(rab1==1)
	{ sislo=(sislo>>8);  rab2=037700000000&(k->zp[r2]);
	  k->zp[r2]=rab2|sislo;   goto kkk;  }
	  else { k->zp[r2]=sislo; goto kkk;  }
      }//------------------Исполнили LWR
       if(op==40)         //Это SB
      { rab1=k->zp[r1]+offr;       //Это исходный адрес.
	sislo=k->zp[r2]; otw=zab(rab1,k);
	if(otw==5) { stca(5,k,rab1,0); return(2);} //Исключ. 5.
	if(otw==7) { stca(7,k,0,0);    return(2);} //Исключ. 7.
	if(otw==20) zawis(k);
				  goto kkk;
      }//------------------Исполнили SB
       if(op==41)         //Это SH
      { rab1=k->zp[r1]+offr;       //Это исходный адрес.
	if((1&rab1)!=0) { stca(5,k,rab1,0); return(2); }
	sislo=k->zp[r2]; otw=zap(rab1,k);
	if(otw==5) { stca(5,k,rab1,0); return(2);} //Исключ. 5.
	if(otw==7) { stca(7,k,0,0);    return(2);} //Исключ. 7.
	if(otw==20) zawis(k);
				  goto kkk;
      }//------------------Исполнили SH
	   //*********************************************

       if(op==42)        //Это SWL
      { rab1=k->zp[r1]+offr;       //Это исходный адрес.

	rab2=037777777774&rab1; rab1=(rab1-rab2); //дельта.
     k->sleds[0]=2; k->sleds[5]=rab2; adr=rab2;
	 sdw=(short)rab1;  rab3=k->zp[r2];
		if(k->jadro==1)   
		{ if((020000000000&adr)!=0)
			{stca(5,k,adr,0); return(2); }	
		}                           //Пользов.в простр.ядра.
//--------------------------------------------------------------------------
		ad=PtoF(adr);
		if((0xa0000000<=adr)&&(0xbfffffff>=adr)) kesk=0; else kesk=1;
		kesk=0; rab=034000000000&adr; 
  //------------------------ ad - физич адрес, kesk - признак кешир.области.
                   k->sleds[6]=ad;
       //Но может режим изолированный? Отреагируем:
     if((0200000&k->status)!=0)   //Изолированный
	 {k->sleds[0]=012; ot=stkesdan(ad,k); kdan=sislo;
	 //-------------------------------------------
      if(sdw==0)
      {rab3=(rab3>>24); kdan=(037777777400&kdan);
	    kdan=(rab3|kdan);
	  }
	  else if(sdw==1)
      {rab3=(rab3>>16); kdan=(037777600000&kdan);
	    kdan=(rab3|kdan);
	  }
	  else if(sdw==2)
      {rab3=(rab3>>8); kdan=(037700000000&kdan);
	    kdan=(rab3|kdan);
	  }
	  else {kdan=rab3; }
     //---------В kdan то, что надо записать в КЕШ  по адресу ad
            k->sleds[4]=kdan;
     ot=zapkesdanosob(ad,kdan,k);
		  if(ot==1) k->status=(02000000|k->status);   else
	 k->status=(037775777777&k->status);// 19-й разряд status. 
            goto kkk;
     }//------------------Далее - режим неизоляции -------
          if(kesk==0)   //Не в КЕШ                                
	{k->sleds[1]=2; ot=stzudan(ad,k,0);
	                                           //	  if(ot!=0)
											//{stca(7,k,ad,0); return(2);}
       //Число прочли в kdan из ОЗУ
		    rab3=k->zp[r2];
      //-----------------------------------
      if(sdw==0)
      {rab3=(rab3>>24); kdan=(037777777400&kdan);
	    kdan=(rab3|kdan);
	  }
	  else if(sdw==1)
      {rab3=(rab3>>16); kdan=(037777600000&kdan);
	    kdan=(rab3|kdan);
	  }
	  else if(sdw==2)
      {rab3=(rab3>>8); kdan=(037700000000&kdan);
	    kdan=(rab3|kdan);
	  }
	  else {kdan=rab3; }
     //--------В kdan то, что надо записать в ОЗУ  по адресу ad

    k->sleds[4]=kdan; k->sleds[1]=2; sislo=kdan;
	  ot=zapzudan(ad,k);                                 
	     if(ot==1)
	{stca(7,k,ad,0); return(2);}
		 if(ot==2)  zawis(k);
	     goto kkk;
	}//---Разделались с НЕ КЕШ.
          //---- Теперь  КЕШ:
     ot=stkesdan(ad,k); if(ot==0)   //Слово есть в КЕШ
     {kdan=sislo; k->sleds[1]=0; rab3=k->zp[r2];
	 //---------------------------------
      if(sdw==0)
      {rab3=(rab3>>24); kdan=(037777777400&kdan);
	    kdan=(rab3|kdan);
	  }
	  else if(sdw==1)
      {rab3=(rab3>>16); kdan=(037777600000&kdan);
	    kdan=(rab3|kdan);
	  }
	  else if(sdw==2)
      {rab3=(rab3>>8); kdan=(037700000000&kdan);
	    kdan=(rab3|kdan);
	  }
	  else {kdan=rab3; }
  //------В kdan то, что надо записать в КЕЩ и ОЗУ по адресу ad
	               k->sleds[4]=kdan;
      ot=zapkesdan(ad,kdan,k);  //Записали в КЕШ
                 sislo=kdan;
      ot1=zapzudan(ad,k);           //Записали то же в ОЗУ
                
	     if(ot1==1)
	{stca(7,k,ad,0); return(2);}
		 if(ot1==2)  zawis(k);
  
             goto kkk;
     }//--- Конец при действительности ячейки КЕШ
         //Был промах. Теперь писать только в ОЗУ:
      k->sleds[1]=1;  //Был промах.
	  ot=stzudan(ad,k,0); //Число прочли из ОЗУ в kdan;
                                                   //if(ot!=0) {stca(7,k,ad,0); return(2);}
            rab3=k->zp[r2];
      //----------------------------------------
      if(sdw==0)
      {rab3=(rab3>>24); kdan=(037777777400&kdan);
	    kdan=(rab3|kdan);
	  }
	  else if(sdw==1)
      {rab3=(rab3>>16); kdan=(037777600000&kdan);
	    kdan=(rab3|kdan);
	  }
	  else if(sdw==2)
      {rab3=(rab3>>8); kdan=(037700000000&kdan);
	    kdan=(rab3|kdan);
	  }
	  else {kdan=rab3; }
     //-------В kdan то, что надо записать в ОЗУ по адресу ad
       sislo=kdan; k->sleds[4]=kdan; 
	   ot=zapzudan(ad,k);     //Записали
	                                               // if(ot!=0) {stca(7,k,ad,0); return(2);}
	     if(ot==1)
	{stca(7,k,ad,0); return(2);}
		 if(ot==2)  zawis(k);

             goto kkk;

	}//----------------------------Исполнили SWL
       if(op==43)         //Это SW
      { rab1=k->zp[r1]+offr;       //Это исходный адрес.
  if((03&rab1)!=0) {stca(5,k,rab1,0);return(2);}//Исключ.5
	sislo=k->zp[r2];
	otw=zas(rab1,k);
	if(otw==5) { stca(5,k,rab1,0); return(2);} //Исключ. 5.
	if(otw==7) { stca(7,k,0,0);    return(2);} //Исключ. 7.
	if(otw==20) zawis(k);
				  goto kkk;
      }//------------------Исполнили SW

       if(op==46)        //Это SWR
      { rab1=k->zp[r1]+offr;       //Это исходный адрес.

	rab2=037777777774&rab1; rab1=(rab1-rab2); //дельта.
     k->sleds[0]=2; k->sleds[5]=rab2; adr=rab2;
	 sdw=(short)rab1;  rab3=k->zp[r2];
		if(k->jadro==1)   
		{ if((020000000000&adr)!=0)
			{stca(5,k,adr,0); return(2); }	
		}                           //Пользов.в простр.ядра.
//-------------------------------------------------------------
		ad=PtoF(adr);
		if((0xa0000000<=adr)&&(0xbfffffff>=adr)) kesk=0; else kesk=1;
 //----------- ad - физич адрес, kesk - признак кешир.области.
                   k->sleds[6]=ad;
       //Но может режим изолированный? Отреагируем:
     if((0200000&k->status)!=0)   //Изолированный
	 {k->sleds[0]=012; ot=stkesdan(ad,k); kdan=sislo;
	    //------------------------------
	 if(sdw==0)   {kdan=rab3; }
	  else if(sdw==1)
      {rab3=(rab3<<8); kdan=(0377&kdan);
	    kdan=(rab3|kdan);
	  }
	  else if(sdw==2)
      {rab3=(rab3<<16); kdan=(0177777&kdan);
	    kdan=(rab3|kdan);
	  }
	  else 
	  {rab3=(rab3<<24); kdan=(077777777&kdan);
       kdan=(rab3|kdan);
	  }
     //---В kdan то, что надо записать в КЕШ по адресу ad
            k->sleds[4]=kdan;
     ot=zapkesdanosob(ad,kdan,k);
		  if(ot==1) k->status=(02000000|k->status);   else
	 k->status=(037775777777&k->status);// 19-й разряд status. 
            goto kkk;
     }//------------------Далее - режим неизоляции -------
          if(kesk==0)   //Не в КЕШ                            
	{k->sleds[1]=2; ot=stzudan(ad,k,0);
		                                                            //if(ot!=0)
															//{stca(7,k,ad,0); return(2);}
       //Число прочли в kdan из ОЗУ
		    rab3=k->zp[r2];
         //----------------------------------
	 if(sdw==0)   {kdan=rab3; }
	  else if(sdw==1)
      {rab3=(rab3<<8); kdan=(0377&kdan);
	    kdan=(rab3|kdan);
	  }
	  else if(sdw==2)
      {rab3=(rab3<<16); kdan=(0177777&kdan);
	    kdan=(rab3|kdan);
	  }
	  else 
	  {rab3=(rab3<<24); kdan=(077777777&kdan);
       kdan=(rab3|kdan);
	  }
     //------В kdan то, что надо записать в ОЗУ по адресу ad
    k->sleds[4]=kdan; k->sleds[1]=2; sislo=kdan;
	  ot=zapzudan(ad,k);      
	     if(ot==1)
	{stca(7,k,ad,0); return(2);}
		 if(ot==2)  zawis(k);

	     goto kkk;
	}//---Разделались с НЕ КЕШ.
          //---- Теперь  КЕШ:
     ot=stkesdan(ad,k); if(ot==0)   //Слово есть в КЕШ
     {kdan=sislo; k->sleds[1]=0; rab3=k->zp[r2];
	 //-----------------------------------
	 if(sdw==0)   {kdan=rab3; }
	  else if(sdw==1)
      {rab3=(rab3<<8); kdan=(0377&kdan);
	    kdan=(rab3|kdan);
	  }
	  else if(sdw==2)
      {rab3=(rab3<<16); kdan=(0177777&kdan);
	    kdan=(rab3|kdan);
	  }
	  else 
	  {rab3=(rab3<<24); kdan=(077777777&kdan);
       kdan=(rab3|kdan);
	  }
     //---В kdan то, что надо записать в КЕЩ и ОЗУ по адресу ad
	               k->sleds[4]=kdan;
      ot=zapkesdan(ad,kdan,k);  //Записали в КЕШ
                 sislo=kdan;
       ot1=zapzudan(ad,k);        //Записали то же в ОЗУ
	     if(ot1==1)
	{stca(7,k,ad,0); return(2);}
		 if(ot1==2)  zawis(k);
		 
             goto kkk;
     }//--- Конец при действительности ячейки КЕШ
         //Был промах. Теперь писать только в ОЗУ:
      k->sleds[1]=1;  //Был промах.
	  ot=stzudan(ad,k,0); //Число прочли из ОЗУ в kdan;
                                              //if(ot!=0) {stca(7,k,ad,0); return(2);}
            rab3=k->zp[r2];
         //--------------------------------------
	 if(sdw==0)   {kdan=rab3; }
	  else if(sdw==1)
      {rab3=(rab3<<8); kdan=(0377&kdan);
	    kdan=(rab3|kdan);
	  }
	  else if(sdw==2)
      {rab3=(rab3<<16); kdan=(0177777&kdan);
	    kdan=(rab3|kdan);
	  }
	  else 
	  {rab3=(rab3<<24); kdan=(077777777&kdan);
       kdan=(rab3|kdan);
	  }
     //----В kdan то, что надо записать в ОЗУ по адресу ad
       sislo=kdan; k->sleds[4]=kdan; 
	   ot=zapzudan(ad,k);   //Записали
	     if(ot==1)
	{stca(7,k,ad,0); return(2);}
		 if(ot==2)  zawis(k);

             goto kkk;

	}//----------------------------Исполнили SWR


	   //***********************************************
       if(op==49)      //Это LWC1
      { k->sledo[1]=2;
	    k->sledo[0]=1;  //Команда CP1.
       k->sledo[2]=100; k->sledo[3]=100; k->sledo[4]=100;		   
		   if((04000000000&(k->status))==0)
	{stca(11,k,0,1); return(2);} //CP1 недоступен. 
	rab1=k->zp[r1]+offr;       //Это исходный адрес.
  if((03&rab1)!=0){stca(4,k,rab1,0); return(2);}//Исключ.4
	otw=sts(rab1,k);     //Взяли слово в sislo.
    if(otw==4) { stca(4,k,rab1,0); return(2);} //Исключ. 4.
//	if(otw==5) { stca(5,k,rab1,0); return(2);} //Исключ. 5.
	if(otw==7) { stca(7,k,0,0); return(2);}    //Исключ. 7.
	     k->fgr[r2]=sislo;            goto kkk;
      }//-----------------Исполнили LWC1
       if(op==57)      //Это SWC1
      {
	   k->sledo[0]=1;  //Команда CP1.
       k->sledo[2]=100; k->sledo[3]=100; k->sledo[4]=100;		   
		   if((04000000000&(k->status))==0)
	{stca(11,k,0,1); return(2);} //CP1 недоступен. 
	rab1=k->zp[r1]+offr;       //Это исходный адрес.
  if((03&rab1)!=0){stca(5,k,rab1,0); return(2);}//Исключ.5
	sislo=k->fgr[r2];  otw=zas(rab1,k);
	if(otw==5) { stca(5,k,rab1,0); return(2);} //Исключ. 5.
	if(otw==7) { stca(7,k,0,0);    return(2);} //Исключ. 7.
	if(otw==20) zawis(k);
			k->sledo[2]=r2;		  goto kkk;
      }//-----------------Исполнили SWC1


	       //Если непредусмотренная:
	  soob.sprintf
("#E - PROZ - Непредусмотр.команда %o по адресу %X",kodkom,k->pc);
	    if(prz) printkl(soob);
		stca(10,k,0,0);   return(4);
	      //------------------------

   kkk:
		
		if(k->predper==0)  k->pc=(k->pc+4);
		 else {k->pc=(k->pc-4);  }
		 		 //Установ pc в беспереходн.команде.
	 return(0);					       
   }//====================================================   




    short op17(ntip *k)           //Код команды 17
    {  UINT rab,rab1;       
       int r1,r2,r3,r4,func,off,offr,i,*ui;
       LONGLONG a,b;   
       int per,delt; short zn; 
 //      UINT  tixoeo=017750000000,  tixoed=017775000000;
       UINT  plusbo=017740000000,  plusbd=017774000000;
       UINT minusbo=037740000000, minusbd=037774000000;
	   QString str,otl;

	    k->sledo[0]=1;  //Команда CP1.
       k->sledo[2]=100; k->sledo[3]=100; k->sledo[4]=100;
		  rab=kodkom;   //Взяли код команды.
 r1=rab&0370000000; r1=(r1>>21); r2=rab&07600000; r2=(r2>>16); 
 r3=0177777&rab; r3=(r3>>11); r4=03777&rab;
      r4=(r4>>6); func=077&rab;
      //----------- Оформили номера регистров как индексы.
//---Общая для всех команд с кодом 17 проверка доступности CP1:

       if((04000000000&(k->status))==0)
	{stca(11,k,0,1);
soob.sprintf("#I - PROZ - CP1 недоступен,операция не выполнена.");
	   printkl(soob);  return(2);} //CP1 недоступен. 
     netos=0;  xwost=0;     //Нач,сброс неточности и хвоста,
       if((r1==2)&&(r4==0)&&(func==0))
		          //------Это CFC1 - чтение в ZP упр.рег. CP1. 
      {k->sledo[1]=2;  if(r3==31)  rab1=k->contrlstatus;
	 else if(r3==0)   rab1=k->implrevision;
	    else   
   { soob.sprintf
("#E - PROZ -  Ошибка номера %d управл.рег.CP1 в ком.по адр. %x",r3,k->pc);
		  printkl(soob);  return(3);  }
	 k->zp[r2]=rab1;               goto kkk;
      }//-------------------------------------Исполнили CFC1

       if((r1==6)&&(r4==0)&&(func==0)) 
		             //--------Это CTC1 - передача из ZP в CP1.
     {  k->sledo[1]=2;    rab1=k->zp[r2];
	     if(r3==31) {  k->contrlstatus=rab1;
		//Теперь анализ - не дает ли запись прерывания?

		    rab=(UINT)0100000;
if((rab&rab1)!=0) {if(rpcp1('z',k)==1) return(5); else flagi('z',k);}
		    rab=(UINT)0200000;
if((rab&rab1)!=0) {if(rpcp1('v',k)==1) return(5); else flagi('v',k);}
		    rab=(UINT)040000;
if((rab&rab1)!=0) {if(rpcp1('o',k)==1) return(5); else flagi('o',k);}
		    rab=(UINT)020000;
if((rab&rab1)!=0) {if(rpcp1('u',k)==1) return(5); else flagi('u',k);}
		    rab=(UINT)0400000;
if((rab&rab1)!=0) {if(rpcp1('e',k)==1) return(5); }
		    rab=(UINT)010000;
if((rab&rab1)!=0) {if(rpcp1('i',k)==1) return(5); else flagi('i',k);}
	  //Теперь - не дает прерывания:
             rab=k->zapcp1; rab=(037777777777^rab);
          k->cause=(rab&(k->cause));
             //Сняли возможно старые запросы на CAUSE CP0.
		 }
       //------------------------------
		 else if(r3==0) { }
		  // k->implrevision=rab1; - нельзя писать в него.
	    else   
   { soob.sprintf
 ("#E - PROZ -  Ошибка номера %d управл.рег.CP1 в ком.по адр.",r3,k->pc);
     	  	 printkl(soob);   return(3);  }
		       goto kkk;
      }//-------------------------------------Исполнили CTC1

       if((r1==0)&&(r4==0)&&(func==0))   //--------Это MFC1              
	   { k->sledo[1]=2;  k->sledo[2]=r3;    
		   k->zp[r2]=k->fgr[r3];	       goto kkk;
      }//--------------------------Исполнили MFC1
       if((r1==4)&&(r4==0)&&(func==0))  //Это MTC1
      {  k->sledo[1]=2;  k->fgr[r3]=k->zp[r2];	   goto kkk;
      }//------------------------------------ Исполнили MTC1


       if((r1==8)&&(r2==0))    //Это BC1F - усл. переход.
      { k->zapusl=k->uslsop1;
		   
		   if(k->predper==0)
		 {k->predper=1;
             if(k->zapusl==1)  //Истина.
			{
		       k->adrper=k->pc+8;
               k->pc=k->pc+4;
               return(0);
			}
             else              //ложь   
			 {
   		      off=0177777&rab; if((0100000&off)!=0)
              offr=037777600000|off; else offr=off; offr=(offr<<2);     
	          rab1=(k->pc)+4+offr;
              k->adrper=rab1; 
              k->pc=k->pc+4;
			  return(0);
             }
          }//------------------- конец при k->predper==0 

         else    //при k->predper==1   !!!
         {k->predper=0;
           printkl("Фатальная ошибка в PROZ. Команда перехода в слоте!"); return(1);
         }
       }//---------------------------------Исполнили BC1F


       if((r1==8)&&(r2==1))    //Это BC1T - усл. переход.
      { k->zapusl=k->uslsop1;
		   
		   if(k->predper==0)
		 {k->predper=1;
             if(k->zapusl==0)  //ложь
			{
		       k->adrper=k->pc+8;
               k->pc=k->pc+4;
               return(0);
			}
             else              //Истина    
			 {
   		      off=0177777&rab; if((0100000&off)!=0)
              offr=037777600000|off; else offr=off; offr=(offr<<2);     
	          rab1=(k->pc)+4+offr;
              k->adrper=rab1; 
              k->pc=k->pc+4;
			  return(0);
             }
          }//------------------- конец при k->predper==0 

         else    //при k->predper==1   !!!
         {k->predper=0;
           printkl("Фатальная ошибка в PROZ. Команда перехода в слоте!"); return(1);
         }
       }//---------------------------------Исполнили BC1T



 if((r1==16)&&(func==0)) //Сложение одинарных чисел ADD.S
       { k->contrlstatus=(037777007777&k->contrlstatus);
 //Это была чистка битов Cause в рег, 
 //они отражают последн.опер.
 slog=1; s1=kto(r3,'o',1,k); s2=kto(r2,'o',2,k);
                //Взяли операнды.
   dlawis:  
	  k->sledo[0]=2; k->sledo[1]=2;
	  k->sledo[2]=r2; k->sledo[3]=r3; k->sledo[4]=r4;
	     u3=(int *)(&m3);
       if((s1==3)||(s1>=6)||(s2==3)||(s2>=6))
		                         //"Плохой" операнд.
		 { k->codisklcp1=26;
   zapros('e',k); //Вписали - "нереализованная операция"
			s3=rpcp1('e',k); if(s3==1) {  
			 return(5); }      // Если будет прерывание.			                                    
			else {
		if(prz) printkl("#E - PROZ -  Некорректный операнд в команде:");
	char* pch=Disass((unsigned long int)k->sledk[4],
		               (unsigned long int)k->sledk[0],0);
	otl.sprintf("# %08X\t",(unsigned long int)k->sledk[0]);
	str=(const char*)pch;
	str=otl+str;
	if(prz) printkl(str); 
				k->fgr[r4]=0x7ff80000; goto kkk; }
	}//----------- Разделались, если был плохой операнд.

 if(((s1==4)&&(s2==5))||((s2==4)&&(s1==5)))
	                    //Бескон.конфликтуют.
   {  
	 zapros('e',k);  s3=rpcp1('e',k);  if(s3==1)
	{ k->contrlstatus=(037777007777&k->contrlstatus); 
	 k->codisklcp1=25;	 zapros('v',k);  return(5);}
	if(prz) printkl("#I - PROZ -  Конфликт бесконечностей в команде:");
	char* pch=Disass((unsigned long int)k->sledk[4],
		               (unsigned long int)k->sledk[0],0);
	otl.sprintf("# %08X\t",(unsigned long int)k->sledk[0]);
	str=(const char*)pch;
	str=otl+str;
	if(prz) printkl(str); 
	 k->codisklcp1=26;  k->fgr[r4]=0x7ff80000; goto kkk;
	}//-----------------Разделались с конфликтн. бесконечн.
     
 //----Теперь, если бесконечности, то они не конфликтуют:
	 if(s1>=4)    
   //Первое слагаемое бесконечность. Сигналов не будет.
 {if(so1==0) k->fgr[r4]=plusbo;
	                    else k->fgr[r4]=minusbo; goto kkk;}
	 else if(s2>=4)
		//Второе слагаемое бесконечность. Сигналов не будет.
      { if(so2==0) k->fgr[r4]=plusbo;
	                    else k->fgr[r4]=minusbo; goto kkk;}
	//--------Бесконечный результат вписан. Ушли на выход !

    //----Теперь работа с нулевыми операндами:
    if((s1==2)&&(s2==2))   //-0 + (-0)
    {p1=-127; u3[1]=0; so1=1; goto wpis; }     //Будет -0.
  else if(((s1>0)&&(s1<3))&&((s2>0)&&(s2<3)))
	                                //Оба нули, но иные. 
	{p1=-127; u3[1]=0; so1=0; goto wpis; }     //Будет +0.
	else if((s1>0)&&(s1<3))       //Только первое слаг.=0.
	{k->fgr[r4]=k->fgr[r2]; if(slog==0) 
	  k->fgr[r4]=020000000000^(k->fgr[r4]);   goto kkk;}
	else if((s2>0)&&(s2<3))       //Только второе слаг.=0.
	{k->fgr[r4]=k->fgr[r3];   goto kkk;}
	else { }
	   //-------------- Конец  нулей ----------------      

	//----------Ура! Начинаем  готовить штатное сложение:
//Учтем подразумев.23 -й разряд, остались числа нормализ:
	 f1=(040000000|f1); 
	 f2=(040000000|f2);  //Учли.
	  //-------------------
       if(p1<p2)                //Тогда переставим слагаемые:
       { per=p1; p1=p2;   p2=per;  //Переставили порядки.
	 zn=so1; so1=so2; so2=zn;  //Переставили знаки.
	 per=f1; f1=f2;   f2=per;  //Переставили мантиссы.
       }    //---------Теперь порядок p1 не меньше p2.
       m1=m2=0;  //Чистка тары слагаемых.
       u1=(int *)(&m1); u2=(int *)(&m2); u3=(int *)(&m3);
 u1[1]=f1; u2[1]=f2; //Вставили мантиссы в тары m1 и m2 слева.
       delt=p1-p2; xwost=0;
	        for(i=1;i<=delt;i++)
		{ if((1&m2)!=0)
			{if(so2==0) xwost=1; else xwost=(-1); }
                 m2=(m2>>1);
		}	   //Выровнили порядки мантисс.
	if(so1==so2)  m3=m1+m2;  //Сложили их.
 else {u2[0]=(037777777777^u2[0]); u2[1]=(037777777777^u2[1]);
      m2=(m2+1);
      m3=m1+m2;              //Сложили их.
	}
	//--------------
	     if(020000000000&u3[1])   //Если минус у m3:
	{ u3[0]=(037777777777^u3[0]); u3[1]=(037777777777^u3[1]);
	  m3=(m3+1);  //Это было инвертирование m3 в прямой код.
	  if(so1==1) so1=0; else so1=1;
	                       //Инвертир.знака рез-та в so1.
	} //----Получили ненормализ.мантиссу в m3 в прямом коде,
     //порядок в p1, знак рез-та в so1.
     // xwost фиксирует милли-ошибку:
     //если >0 ,то недобрали, если <0 ,то перебрали в ответе.
	  //Если  =0, то хвоста не было.
 oformlen: if(m3==0) goto noll; //На оформление нуля и конца
				      //и для операций  - , * , /.
	 //---- Начнем нормализацию ненулевого результата:  
       if(0100000000&u3[1])      //Мантисса выскочила влево,
				  //но не может более чем на 1 разр.
	      { m3=(m3>>1); p1=(p1+1); } 
       else { while((040000000&u3[1])==0)
		           //Мантисса с правым уклоном.
		{ m3=(m3<<1); p1=(p1-1); } 
		  //Есть возможность нештатного порядка.
	      }
//*****************  Нормализовали. Нормализ. результат  	
//отличен от нуля и с 1 в 23-м разряде левой части тары m3,
// несмещ.порядок в p1, знак числа в so1.
   //***************************************************

   okr:   s1=okrugo(k);
		if(s1==9)                //Ненормализ.рез-тат.
		{ k->codisklcp1=26; zapros('e',k);		
          s3=rpcp1('e',k); 
		  if(s3==1)  {  return(5);}
		  else
 if(prz) printkl("#I - PROZ -  Ненормализованный результат в команде:");
	char* pch=Disass((unsigned long int)k->sledk[4],
		                (unsigned long int)k->sledk[0],0);
	otl.sprintf("# %08X\t",(unsigned long int)k->sledk[0]);
	str=(const char*)pch;
	str=otl+str;
	if(prz) printkl(str); 
			 /* k->fgr[r4]=0x7ff80000;*/ goto kkk;
        } //-------------Разделались с ненормализованным.

		 else if(s1==8)              //Переполнение.

      {  k->codisklcp1=23; zapros('o',k); zapros('i',k);
		 s3=rpcp1('o',k); s4=rpcp1('i',k); 
	     if((s3==1)||(s4==1)) 
		 {   return(5);}
		 else {flagi('o',k); flagi('i',k); 
if(prz) printkl("#I - PROZ -  Внимание! Было переполнение в команде:");
	char* pch=Disass((unsigned long int)k->sledk[4],
		               (unsigned long int)k->sledk[0],0);
	otl.sprintf("# %08X\t",(unsigned long int)k->sledk[0]);
	str=(const char*)pch;
	str=otl+str;
	if(prz) printkl(str); 		 
		 goto wpis; }  
      }	    //--------------Разделались с переполнением

		 else if(s1==0)      //Нормализованный рез-тат.
	{ if(netos==0) goto wpis; zapros('i',k);
		 s3=rpcp1('i',k); if(s3==1) 
		 {  k->codisklcp1=21;
		              return(5);} 
		 else 
		 {flagi('i',k);  k->codisklcp1=21; goto wpis;}   

    } //---------------Разделались с нормализованным.
		 else { } 

      wpis:  s4=0;  s4=(u3[1]|s4);     //Вписали мантиссу.
  p1=(p1+127); p1=(p1<<23); s4=(s4|p1); //Вписали порядок,
	                					//сместив его.
	  if(so1==1) s4=(020000000000|s4);  //Вписали минус.
	  k->fgr[r4]=s4;          //Вписали весь ответ.
	  //-----------------------------------------
	      goto kkk;                 //На концовку. 
		     //-----------------------
     noll:  k->fgr[r4]=0;    //В ответ +0 
	    goto kkk;   
     }//============================КОМАНДУ   ИСПОЛНИЛИ.

 if((r1==16)&&(func==1)) //Вычитание одинарных чисел SUB.S
      { k->contrlstatus=(037777007777&k->contrlstatus);
//Это была чистка битов Cause, они отражают последн.опер.
   slog=0; s1=kto(r3,'o',1,k); s2=kto(r2,'o',2,k);
                                     //Взяли операнды.
 if(so2==1) so2=0; else so2=1; goto dlawis;
                                     //На общий хвост.
      }//===========================КОМАНДУ   ИСПОЛНИЛИ. 		     


 if((r1==16)&&(func==2)) //Умножение одинарных чисел MUL.S
       { k->contrlstatus=(037777007777&k->contrlstatus);
            k->sledo[0]=3; k->sledo[1]=4;
			k->sledo[2]=r2; k->sledo[3]=r3; k->sledo[4]=r4;
       s1=kto(r3,'o',1,k); s2=kto(r2,'o',2,k);
	                                  //Взяли операнды.
         if((s1==3)||(s1>=6)||(s2==3)||(s2>=6)) 
			                          //"Плохой" операнд.
		 {  k->codisklcp1=26; zapros('e',k);
		    s3=rpcp1('e',k);    
	  if(s3==1) {   return(5); } // Если будет прер.
			else {
	if(prz) printkl("#I - PROZ -  Некорректный операнд в команде:");
	char* pch=Disass((unsigned long int)k->sledk[4],
		               (unsigned long int)k->sledk[0],0);
	otl.sprintf("# %08X\t",(unsigned long int)k->sledk[0]);
	str=(const char*)pch;
	str=otl+str;
	if(prz) printkl(str); 				
				k->fgr[r4]=0x7ff80000; goto kkk; }
	}//------------ Разделались, если был плохой операнд.

	 if((((s1==4)||(s1==5)) && ((s2==1)||(s2==2)) )||
		( ((s2==4)||(s2==5)) && ((s1==1)||(s1==2)) ) )     
		//Некорректн операция.Умнож.бескон.на ноль.  	                                                        
    { 
	   zapros('e',k); s3=rpcp1('e',k);    
           if(s3==1)
	{ k->contrlstatus=(037777007777&k->contrlstatus);
		 k->codisklcp1=25; zapros('v',k);  return(5);}
		  else {
if(prz) printkl("#I - PROZ -  Умножение бесконечн.на ноль в команде:");
	char* pch=Disass((unsigned long int)k->sledk[4],
		               (unsigned long int)k->sledk[0],0);
	otl.sprintf("# %08X\t",(unsigned long int)k->sledk[0]);
	str=(const char*)pch;
	str=otl+str;
	if(prz) printkl(str); 			  
	k->fgr[r4]=0x7ff80000; k->codisklcp1=26;  goto kkk;}
	}//----Разделались с конфликтн.бесконечн.нуля.

       //------------------ А бесконечность без нуля?
	   if((s1>=4)||(s2>=4))
		       //Хотя бы один сомножитель бесконечен.
     {if(so1==so2) k->fgr[r4]=plusbo;
	                else k->fgr[r4]=minusbo; goto kkk;}

	   //----А есть ли в сомножителях нули?
	   if(((s1==1)||(s1==2))||((s2==1)||(s2==2)))
		                     //Умножение на ноль.
   {if(so1==so2) k->fgr[r4]=0;
	          else k->fgr[r4]=020000000000; goto kkk;}

 //--Конец бескон.и нулей. Начинаем нормальное умножение:
 //Учтем подразумев. 23 -й разряд, числа нормализованы:
	 f1=(040000000|f1); 
	 f2=(040000000|f2);  //Учли.
  a=(LONGLONG)f1; b=(LONGLONG)f2; m3=a*b;//Перемножили.
	 u3=(int *)(&m3);        //Нужная ссылка на тару.
     if(so1==so2) so1=0; else so1=1;
	                         //Учли знаки сомножителей.
	 p1=(p1+p2); m3=(m3<<9); 
	                 //Вывели мантиссу в стандарт тары.
			goto oformlen;  //На общий хвост-концовку.

    }//==========================КОМАНДУ  ИСПОЛНИЛИ.


 if((r1==16)&&(func==3)) //Деление одинарных чисел DIV.S
     { k->contrlstatus=(037777007777&k->contrlstatus);
            k->sledo[0]=4; k->sledo[1]=12;
	k->sledo[2]=r2; k->sledo[3]=r3; k->sledo[4]=r4;

       s1=kto(r3,'o',1,k); s2=kto(r2,'o',2,k);
	                                //Взяли операнды.
         if((s1==3)||(s1>=6)||(s2==3)||(s2>=6))
			                        //"Плохой" операнд.
		 {   k->codisklcp1=26;
		    zapros('e',k); s3=rpcp1('e',k);
		    //Вписали - "нереализованная операция"
	if(s3==1) {   return(5); } //Если будет прерывание.
			else { 
	if(prz) printkl("#E - PROZ -  Некорректный операнд в команде:");
	char* pch=Disass((unsigned long int)k->sledk[4],
		               (unsigned long int)k->sledk[0],0);
	otl.sprintf("# %08X\t",(unsigned long int)k->sledk[0]);
	str=(const char*)pch;
	str=otl+str;
	if(prz) printkl(str); 				
				k->fgr[r4]=0x7ff80000; goto kkk; }
	}//---------- Разделались, если был плохой операнд.


     if((((s1==1)||(s1==2)) && ((s2==1)||(s2==2)) )||
		( ((s2==4)||(s2==5)) && ((s1==4)||(s1==5)) ) ) 
	//Некорректн операция: бескон / бескон, ноль / ноль.	                                            
	{   zapros('e',k);
	   s3=rpcp1('e',k);  
           if(s3==1)
	{ k->codisklcp1=25; 
	  k->contrlstatus=(037777007777&k->contrlstatus);
		   zapros('v',k);  return(5);}
		  else {
if(prz) printkl("#E - PROZ -  Некорректное деление 0/0 или беск/беск.:");
	char* pch=Disass((unsigned long int)k->sledk[4],
		               (unsigned long int)k->sledk[0],0);
	otl.sprintf("# %08X\t",(unsigned long int)k->sledk[0]);
	str=(const char*)pch;
	str=otl+str;
	if(prz) printkl(str); 			  
	k->fgr[r4]=0x7ff80000; k->codisklcp1=26;  goto kkk;}

	 }//--Разделались с конфликтами бесконечностей и нулей.

       //-----------------Деление  обычного числа на 0:
	   if(((s2==1)||(s2==2))&&(s1==0))  
     {   k->codisklcp1=24;
	       zapros('z',k); s3=rpcp1('z',k);
      if(s3==1) { return(5); }
      else
 {if(so1==so2) k->fgr[r4]=plusbo; else k->fgr[r4]=minusbo;
	   flagi('z',k); 
if(prz) printkl("#I - PROZ -  Внимание! Было деление на 0  в команде:");
	char* pch=Disass((unsigned long int)k->sledk[4],
		               (unsigned long int)k->sledk[0],0);
	otl.sprintf("# %08X\t",(unsigned long int)k->sledk[0]);
	str=(const char*)pch;
	str=otl+str;
	if(prz) printkl(str); 	   
	   goto kkk;}

     }  //-----------------------------------------

       //-----------------Деление  бесконечности на 0:
	   if(((s2==1)||(s2==2))&&(s1>=4))  
     {       
if(prz) printkl("#I - PROZ -  Внимание! Было деление бескон. на 0 :");
	char* pch=Disass((unsigned long int)k->sledk[4],
		               (unsigned long int)k->sledk[0],0);
	otl.sprintf("# %08X\t",(unsigned long int)k->sledk[0]);
	str=(const char*)pch;
	str=otl+str;
	if(prz) printkl(str); 
      if(so1==so2) k->fgr[r4]=plusbo;
	        else k->fgr[r4]=minusbo; goto kkk;
     }  //-----------------------------------------

	   if((s2==4)||(s2==5)) 
		           //Деление не бесконечности на бескон. 
     {if(so1==so2) k->fgr[r4]=0;
	          else k->fgr[r4]=020000000000; goto kkk;}
        //--------------------------------------------
	   if((s1==1)||(s1==2)) //Деление 0 на не ноль.
     {if(so1==so2) k->fgr[r4]=0; 
	         else k->fgr[r4]=020000000000; goto kkk;}

//---Конец бесконечн и нулей. Начинаем нормальное деление:
//Учтем подразумев. 23 -й разряд,  числа нормализованы:
	 f1=(040000000|f1); 
	 f2=(040000000|f2);  //Учли.
	 a=(LONGLONG)f1; b=(LONGLONG)f2; 
	 u3=(int *)(&m3);        //Нужная ссылка на тару.
	 if(so1==so2) so1=0; else so1=1; 
	                       //Учли знаки сомножителей.
	 p1=(p1-p2);  a=(a<<39); m3=a/b; rab1=(UINT)(a%b); 
	 m3=(m3<<16); if(rab1!=0) m3=(0100000|m3);
		   //Вывели мантиссу в стандарт тары.
			goto oformlen;    //На общий хвост-концовку.
    }//==============================КОМАНДУ  ИСПОЛНИЛИ.

     if((r1==16)&&(r2==0)&&(func==5))        //  ABS.S
     { k->contrlstatus=(037777007777&k->contrlstatus);
	   k->sledo[0]=5; k->sledo[2]=r3; k->sledo[4]=r4;
      s1=kto(r3,'o',1,k);     

         if((s1==3)||(s1>=6))   //"Плохой" операнд.
		 {   k->codisklcp1=26;
 zapros('e',k);  //Вписали - "нереализованная операция"
		   s3=rpcp1('e',k); 
	if(s3==1) {   return(5); } //Если будет прерывание.
			else { 
	if(prz) printkl("#E - PROZ -  Некорректный операнд в команде:");
	char* pch=Disass((unsigned long int)k->sledk[4],
		               (unsigned long int)k->sledk[0],0);
	otl.sprintf("# %08X\t",(unsigned long int)k->sledk[0]);
	str=(const char*)pch;
	str=otl+str;
	if(prz) printkl(str); 				
				k->fgr[r4]=0x7ff80000; goto kkk; }
		 }//----- Разделались, если был плохой операнд.

       rab1=k->fgr[r3]; rab1=(017777777777&rab1);
	                              //Абс.величина.
   k->fgr[r4]=rab1; goto kkk; //Вписали ответ и на конец.
    }//----------------------------КОМАНДУ  ИСПОЛНИЛИ.

     if((r1==16)&&(r2==0)&&(func==6))        //  MOV.S
    {
	    k->sledo[2]=r3; k->sledo[4]=r4;		 
	 //k->contrlstatus=(037777007777&k->contrlstatus);
		k->fgr[r4]=k->fgr[r3];  goto kkk;       
    }//----------------------------КОМАНДУ  ИСПОЛНИЛИ.

     if((r1==17)&&(r2==0)&&(func==32))
		         //  CVT.S.D. Преобр.двойного в одинарн.
     { k->contrlstatus=(037777007777&k->contrlstatus);
	    k->sledo[0]=5; k->sledo[1]=2;
	    k->sledo[2]=r3; k->sledo[4]=r4;
       s1=kto(r3,'d',1,k);   

         if((s1==3)||(s1>=6)) 
			     //"Плохой" операнд, ненормализ.
		 {   k->codisklcp1=26;
		    zapros('e',k);
			     //Вписали - "нереализованная операция"
		   s3=rpcp1('e',k); 
	if(s3==1) {  return(5); } //Если будет прерывание.
			else {
	if(prz) printkl("#E - PROZ -  Некорректный операнд в команде:");
	char* pch=Disass((unsigned long int)k->sledk[4],
		               (unsigned long int)k->sledk[0],0);
	otl.sprintf("# %08X\t",(unsigned long int)k->sledk[0]);
	str=(const char*)pch;
	str=otl+str;
	if(prz) printkl(str); 				
	k->fgr[r4]=0x7ff80000; k->fgr[r4+1]=0; goto kkk; }
		 }//-- Разделались, если был плохой операнд.


       if(s1==4) { k->fgr[r4]=plusbo;
	          k->fgr[r4+1]=0;   goto kkk; }  //+беск.
       if(s1==5) { k->fgr[r4]=minusbo; 
	          k->fgr[r4+1]=0;   goto kkk; }  //-беск.
       if(s1==1) { k->fgr[r4]=0;     
	          k->fgr[r4+1]=0;      goto kkk; }  //+0
       if(s1==2) { k->fgr[r4]=020000000000;
	          k->fgr[r4+1]=0; goto kkk; }  //-0
	  //--------------

//Теперь перепишем компоненты двойного числа 
//в тару одинарного, по которой работает функция okrugo( ) :
       p1=pd1; m3=fd1<<3; u3=(int *)(&m3); so1=sd1; 
//И на округление,на оконечную часть сложения,
//т.к. запись ответа тоже по [r4] :
		  k->fgr[r4+1]=0;   goto okr;
     }//----------------------------КОМАНДУ  ИСПОЛНИЛИ.

     if((r1==16)&&(r2==0)&&(func==33)) 
		         // CVT.D.S. Преобр.одинарн.в двойное.
     { k->contrlstatus=(037777007777&k->contrlstatus);
	    k->sledo[0]=5; 
	    k->sledo[2]=r3; k->sledo[4]=r4;
       s1=kto(r3,'o',1,k);   

         if((s1==3)||(s1>=6))   //"Плохой" операнд.
		 {   k->codisklcp1=26;
           zapros('e',k);
		         //Вписали - "нереализованная операция"
		   s3=rpcp1('e',k); 
      if(s3==1) {   return(5); } //Если будет прерывание.
			else {
	if(prz) printkl("#E - PROZ -  Некорректный операнд в команде:");
	char* pch=Disass((unsigned long int)k->sledk[4],
		               (unsigned long int)k->sledk[0],0);
	otl.sprintf("# %08X\t",(unsigned long int)k->sledk[0]);
	str=(const char*)pch;
	str=otl+str;
	if(prz) printkl(str); 								
	k->fgr[r4+1]=0x7ff80000; k->fgr[r4]=0; goto kkk; }
	}//-------- Разделались, если был плохой операнд.
       if(s1==4) { k->fgr[r4]=0;
	          k->fgr[r4+1]=plusbd;  goto kkk; } //+беск.
       if(s1==5) { k->fgr[r4]=0;
	          k->fgr[r4+1]=minusbd; goto kkk; } //-беск.
       if(s1==1) { k->fgr[r4]=0;
	           k->fgr[r4+1]=0;       goto kkk; }  //+0
       if(s1==2) { k->fgr[r4]=0;
	         k->fgr[r4+1]=020000000000; goto kkk;}//-0
	  //--------------
 //Теперь перепишем компоненты одинарного числа 
 //в тару двойного.
  pd1=p1+1023; u3=(int *)(&m3); m3=0; u3[1]=f1;
                 m3=(m3>>3); sd1=so1; 
  if(sd1==1) u3[1]=(020000000000|u3[1]); //Учли знак
  u3[1]=(037773777777&u3[1]); //Стерли подразумев.разряд.
  pd1=(pd1<<20);   u3[1]=(pd1|u3[1]);//Вписали порядок.
       //--------Теперь запись ответа:
       k->fgr[r4]=u3[0];  k->fgr[r4+1]=u3[1]; 
		 goto kkk;
     }//-----------------------------КОМАНДУ  ИСПОЛНИЛИ.

      if((r1==20)&&(r2==0)&&(func==32))    //CVT.S   W
   {k->sledo[1]=2;                   //Целое в одинарное.
	  k->contrlstatus=(037777007777&k->contrlstatus); 	                                              
//Здесь не будет потери значимости в исповедуемом смысле,
//нет вообще переполнения, некорректности, бесконечности.
	    k->sledo[0]=5; k->sledo[1]=3;
	    k->sledo[2]=r3; k->sledo[4]=r4;
   rab1=k->fgr[r3]; if(rab1==0) {k->fgr[r4]=0; goto kkk;}
   u3=(int *)(&m3); m3=0; ui=(int *)(&rab1); if(ui[0]<0)
 {so1=1; rab1=(037777777777^rab1); rab1=rab1+1; u3[1]=rab1;}
       else { so1=0; u3[1]=rab1;}
       //----------------------
       p1=31; m3=(m3>>8);  while((040000000&u3[1])==0)
		     { m3=(m3<<1); p1=(p1-1); }
	  //Нормализовали с 1  в  23 разряде мантиссы  m3.
	    goto okr; //На конец сложения, т.к. ответ - по [r4].
     }//--------------------------------КОМАНДУ  ИСПОЛНИЛИ.
			
      if((r1==20)&&(r2==0)&&(func==33))      //CVT.D   W
{k->contrlstatus=(037777007777&k->contrlstatus); 
	                                     //Целое в двойное.     
  //Здесь не будет потери значимости в исповедуемом смысле,
  //нет вообще переполнения, некорректности, бесконечности.
	    k->sledo[0]=5; k->sledo[1]=3;
	    k->sledo[2]=r3; k->sledo[4]=r4;
       rab1=k->fgr[r3];  if(rab1==0)
	    { k->fgr[r4]=0; k->fgr[r4+1]=0; goto kkk; }
  u3=(int *)(&m3);   m3=0;  ui=(int *)(&rab1);  if(ui[0]<0)   
 {sd1=1; rab1=(037777777777^rab1); rab1=rab1+1; u3[1]=rab1;}
       else { sd1=0; u3[1]=rab1; }
       //----------------------
       pd1=31; m3=(m3>>11);  while((04000000&u3[1])==0)
		     { m3=(m3<<1); pd1=(pd1-1); }
  //Нормализовали с 1  в подразумев. разряде мантиссы  m3.
  u3[1]=(037773777777&u3[1]);  //Стерли 1 подразумеваемую.
  pd1=(pd1+1023); pd1=(pd1<<20); u3[1]=(pd1|u3[1]);
                                       //Вписали порядок.
  if(sd1==1) u3[1]=(020000000000|u3[1]);//Вписали знак.
k->fgr[r4]=u3[0];  k->fgr[r4+1]=u3[1]; //Вписали ответ.	
			    goto kkk;
     }//-------------------------------КОМАНДУ  ИСПОЛНИЛИ.


  if((r1==16)&&(r2==0)&&(func==36))          //CVT.W   S
                                        //одинарн.в целое.
   { k->contrlstatus=(037777007777&k->contrlstatus); 
	    k->sledo[0]=5; k->sledo[1]=2;
	    k->sledo[2]=r3; k->sledo[4]=r4;
       s1=kto(r3,'o',1,k); if((s1==1)||(s1==2))
	      {k->fgr[r4]=0; goto kkk; }
         if((s1==3)||(s1>=6)||(p1>=31))
			                //"Плохой" операнд или большой.
		 {  k->codisklcp1=26;
            zapros('e',k); 
			         //Вписали - "нереализованная операция"
		   s3=rpcp1('e',k); 
		  if(s3==1) {  return(5);} //Если будет прерывание.
		  else { 
	if(prz) printkl("#E - PROZ -  Некорректный операнд в команде:");
	char* pch=Disass((unsigned long int)k->sledk[4],
		               (unsigned long int)k->sledk[0],0);
	otl.sprintf("# %08X\t",(unsigned long int)k->sledk[0]);
	str=(const char*)pch;
	str=otl+str;
	if(prz) printkl(str); 							  
			  k->fgr[r4]=0; goto kkk; }     // обнуляю 
	}//-- Разделались, если был плохой или большой операнд.

//-Теперь только нормализ.с 1 в 23 разр.и без переполнений.
//Расположим целую часть в левой половине m3, т.е.в u3[1]:
       m3=0; u3=(int *)(&m3); u3[1]=f1; m3=(m3>>23);
	    if(p1==0)  { }
		else if(p1<0) 
		{for(i=1;i<=(-p1);i++)   
		{ if((01&m3)!=0) 
		{if(so1==0) xwost=1; else xwost=-1; }
			m3=(m3>>1);}
		}
       else            m3=(m3<<p1);
//--Получили в u3[1] неокругленный модуль ответа,знак в so1.
	   okrugz(k);
	     if(netos==1)
		 {   k->codisklcp1=21; 
		       zapros('i',k); s3=rpcp1('i',k);
		 if(s3==1) { return(5); }
           else flagi('i',k);
		 }
	   if(u3[1]==0) {k->fgr[r4]=0; goto kkk;}
	   if(so1==0) {k->fgr[r4]=u3[1]; goto kkk;} 
	   else {u3[1]=(037777777777^u3[1]); u3[1]=(u3[1]+1); 
	    k->fgr[r4]=u3[1]; goto kkk;} 

     }//----------------------------КОМАНДУ  ИСПОЛНИЛИ.

      if((r1==17)&&(r2==0)&&(func==36))       //CVT.W   D
                                       //двойное в целое.
     { k->contrlstatus=(037777007777&k->contrlstatus);
	    k->sledo[0]=5; k->sledo[1]=2;
	    k->sledo[2]=r3; k->sledo[4]=r4;
       s1=kto(r3,'d',1,k); if((s1==1)||(s1==2))
	      {k->fgr[r4]=0; goto kkk; }

      if((s1==3)||(s1>=6)||(pd1>=31)) 
		                 //"Плохой" операнд или большой.
		 {   k->codisklcp1=26;
          zapros('e',k); 
		          //Вписали - "нереализованная операция"
		   s3=rpcp1('e',k); 
	 if(s3==1) {   return(5); } //Если будет прерывание.
		  else { 
		if(prz) printkl("#E - PROZ -  Некорректный операнд в команде:");
	char* pch=Disass((unsigned long int)k->sledk[4],
		               (unsigned long int)k->sledk[0],0);
	otl.sprintf("# %08X\t",(unsigned long int)k->sledk[0]);
	str=(const char*)pch;
	str=otl+str;
	if(prz) printkl(str); 							  
			  k->fgr[r4]=0; goto kkk; } // обнуляю 
 }//- Разделались, если был плохой или большой операнд.
//--Теперь только нормализ.с 1 в подразум.разр.и не чрезмерн.

       u3=(int *)(&m3); m3=fd1; m4=0; u4=(int *)(&m4);
           for(i=1;i<=52;i++)
	{m4=(m4>>1); if((01&m3)!=0)
	             u4[1]=(020000000000|u4[1]); m3=(m3>>1);}
	    if(pd1==0)  { }
		else if(pd1<0) 
	{for(i=1;i<=(-pd1);i++)   
		{ if((01&m4)!=0) 
		 {if(sd1==0) xwost=1; else xwost=-1; } m4=(m4>>1);
	if((01&m3)!=0) u4[1]=(020000000000|u4[1]); m3=(m3>>1);
		}
	}
       else 
	   {     for(i=1;i<=pd1;i++) 
	    {m3=(m3<<1); if((020000000000&u4[1])!=0) m3=(01|m3);
                    m4=(m4<<1);
	           }
	   }
//-Получили в m3 неокругленный модуль ответа, знак в sd1.
// Т.к. ответ ограничен по величине, то надо брать u[0].
	   okrugzd(k);
	     if(netos==1)
		 {  k->codisklcp1=21; 
		      zapros('i',k); s3=rpcp1('i',k);
		 if(s3==1) { return(5); }
           else flagi('i',k);
		 }	   
	   k->fgr[r4+1]=0;
	   if(u3[0]==0) {k->fgr[r4]=0; goto kkk;}
	   if(sd1==0) {k->fgr[r4]=u3[0]; goto kkk;} 
	   else {u3[0]=(037777777777^u3[0]); u3[0]=(u3[0]+1); 
	    k->fgr[r4]=u3[0]; goto kkk;} 
	  }//----------------------------КОМАНДУ  ИСПОЛНИЛИ.

     if((r1==16)&&(r4==0)&&(func==48))	      //C.F.S
    { k->contrlstatus=(037737777777&k->contrlstatus);
      k->uslsop1=0;        goto kkk;
    }//------------------------------КОМАНДУ  ИСПОЛНИЛИ.

     if((r1==16)&&(r4==0)&&(func==49))         //C.UN.S                    
    { ss1=sraw('o',0,1,r2,r3,k);
	    k->sledo[0]=5; k->sledo[1]=2;
	    k->sledo[2]=r2; k->sledo[3]=r3;
         if((s1==3)||(s2==3))   //"Плохие" операнды.
		 {   k->codisklcp1=26;
    zapros('e',k); //Вписали - "нереализованная операция"
			s3=rpcp1('e',k); if(s3==1) {  
			 return(5); }  // Если будет прерывание.
if(prz) printkl("#I - PROZ -  Ненормализованный операнд в команде:");
	char* pch=Disass((unsigned long int)k->sledk[4],
		               (unsigned long int)k->sledk[0],0);
	otl.sprintf("# %08X\t",(unsigned long int)k->sledk[0]);
	str=(const char*)pch;
	str=otl+str;
	if(prz) printkl(str); 					                                    
		 }//------ Разделались, если был плохой операнд.

     srawn:
       if(ss1>0)           //Условие выполнено.
       { k->contrlstatus=(040000000|k->contrlstatus);
	 k->uslsop1=1;
       }   else            //Условие не выполнено.
       { k->contrlstatus=(037737777777&k->contrlstatus);
	 k->uslsop1=0;
       }                   goto kkk;
    }//------------------------------КОМАНДУ  ИСПОЛНИЛИ.

      if((r1==16)&&(r4==0)&&(func==50))       //C.ED.S
    { ss1=sraw('o',2,0,r2,r3,k);
	    k->sledo[0]=5; k->sledo[1]=2;
	    k->sledo[2]=r2; k->sledo[3]=r3;	  
         if((s1==3)||(s2==3))   //"Плохие" операнды.
		 {  k->codisklcp1=26;
  zapros('e',k); //Вписали - "нереализованная операция"
			s3=rpcp1('e',k); if(s3==1) {  
			 return(5); }  // Если будет прерывание.
if(prz) printkl("#I - PROZ -  Ненормализованный операнд в команде:");
	char* pch=Disass((unsigned long int)k->sledk[4],
		               (unsigned long int)k->sledk[0],0);
	otl.sprintf("# %08X\t",(unsigned long int)k->sledk[0]);
	str=(const char*)pch;
	str=otl+str;
	if(prz) printkl(str); 							                                    
		 }//----- Разделались, если был плохой операнд.
	  
	  goto srawn;
    }//------------------------------КОМАНДУ  ИСПОЛНИЛИ.

      if((r1==16)&&(r4==0)&&(func==51))      //C.UEQ. S
    { ss1=sraw('o',2,1,r2,r3,k);
	    k->sledo[0]=5; k->sledo[1]=2;
	    k->sledo[2]=r2; k->sledo[3]=r3;
         if((s1==3)||(s2==3))   //"Плохие" операнды.
		 {   k->codisklcp1=26;
  zapros('e',k); //Вписали - "нереализованная операция"
			s3=rpcp1('e',k); if(s3==1) {  
			 return(5); }   // Если будет прерывание.
if(prz) printkl("#I - PROZ -  Ненормализованный операнд в команде:");
	char* pch=Disass((unsigned long int)k->sledk[4],
		               (unsigned long int)k->sledk[0],0);
	otl.sprintf("# %08X\t",(unsigned long int)k->sledk[0]);
	str=(const char*)pch;
	str=otl+str;
	if(prz) printkl(str); 							                                    
		 }//---- Разделались, если был плохой операнд.
	  goto srawn;
    }//------------------------------КОМАНДУ  ИСПОЛНИЛИ.

      if((r1==16)&&(r4==0)&&(func==52))     //C.OLT. S
    { ss1=sraw('o',1,0,r2,r3,k);
	    k->sledo[0]=5; k->sledo[1]=2;
	    k->sledo[2]=r2; k->sledo[3]=r3;
         if((s1==3)||(s2==3))   //"Плохие" операнды.
		 {   k->codisklcp1=26;
 zapros('e',k); //Вписали - "нереализованная операция"
			s3=rpcp1('e',k); if(s3==1) {  
			 return(5); }  // Если будет прерывание.
if(prz) printkl("#I - PROZ -  Ненормализованный операнд в команде:");
	char* pch=Disass((unsigned long int)k->sledk[4],
		               (unsigned long int)k->sledk[0],0);
	otl.sprintf("# %08X\t",(unsigned long int)k->sledk[0]);
	str=(const char*)pch;
	str=otl+str;
	if(prz) printkl(str); 				                             
		 }//------- Разделались, если был плохой операнд.
	  goto srawn;
    }//------------------------------КОМАНДУ  ИСПОЛНИЛИ.

      if((r1==16)&&(r4==0)&&(func==53))     //C.ULT. S
    { ss1=sraw('o',1,1,r2,r3,k);
	    k->sledo[0]=5; k->sledo[1]=2;
	    k->sledo[2]=r2; k->sledo[3]=r3;
         if((s1==3)||(s2==3))   //"Плохие" операнды.
		 {  k->codisklcp1=26;
 zapros('e',k); //Вписали - "нереализованная операция"
			s3=rpcp1('e',k); if(s3==1) {  
			 return(5); }    // Если будет прерывание.
if(prz) printkl("#I - PROZ -  Ненормализованный операнд в команде:");
	char* pch=Disass((unsigned long int)k->sledk[4],
		               (unsigned long int)k->sledk[0],0);
	otl.sprintf("# %08X\t",(unsigned long int)k->sledk[0]);
	str=(const char*)pch;
	str=otl+str;
	if(prz) printkl(str); 							                                    
		 }//-------- Разделались, если был плохой операнд.
	  goto srawn;
    }//------------------------------КОМАНДУ  ИСПОЛНИЛИ.

      if((r1==16)&&(r4==0)&&(func==54))       //C.OLE. S
    { ss1=sraw('o',3,0,r2,r3,k);
	    k->sledo[0]=5; k->sledo[1]=2;
	    k->sledo[2]=r2; k->sledo[3]=r3;
         if((s1==3)||(s2==3))   //"Плохие" операнды.
		 {   k->codisklcp1=26;
 zapros('e',k); //Вписали - "нереализованная операция"
			s3=rpcp1('e',k); if(s3==1) {  
			 return(5); }      // Если будет прерывание.
	if(prz) printkl("#I - PROZ -  Ненормализованный операнд в команде:");
	char* pch=Disass((unsigned long int)k->sledk[4],
		               (unsigned long int)k->sledk[0],0);
	otl.sprintf("# %08X\t",(unsigned long int)k->sledk[0]);
	str=(const char*)pch;
	str=otl+str;
	if(prz) printkl(str); 							                                    
		 }//------ Разделались, если был плохой операнд.
	  goto srawn;
    }//------------------------------КОМАНДУ  ИСПОЛНИЛИ.

      if((r1==16)&&(r4==0)&&(func==55))      //C.ULE. S
    { ss1=sraw('o',3,1,r2,r3,k);
	    k->sledo[0]=5; k->sledo[1]=2;
	    k->sledo[2]=r2; k->sledo[3]=r3;
         if((s1==3)||(s2==3))   //"Плохие" операнды.
		 {   k->codisklcp1=26;
 zapros('e',k); //Вписали - "нереализованная операция"
			s3=rpcp1('e',k); if(s3==1) {  
			 return(5); }   // Если будет прерывание.
	if(prz) printkl("#I - PROZ -  Ненормализованный операнд в команде:");
	char* pch=Disass((unsigned long int)k->sledk[4],
		               (unsigned long int)k->sledk[0],0);
	otl.sprintf("# %08X\t",(unsigned long int)k->sledk[0]);
	str=(const char*)pch;
	str=otl+str;
	if(prz) printkl(str); 							                                    
		 }//-----Разделались, если был плохой операнд.
	  goto srawn;
    }//------------------------------КОМАНДУ  ИСПОЛНИЛИ.
//****************************************************
      if((r1==16)&&(r4==0)&&(func==56))	     //C.SF.S
    { k->contrlstatus=(037737777777&k->contrlstatus);
	    k->sledo[0]=5; k->sledo[1]=2;
	    k->sledo[2]=r2; k->sledo[3]=r3;
      k->uslsop1=0;  ss1=sraw('o',0,1,r2,r3,k);
         if((s1==3)||(s2==3))   //"Плохие" операнды.
		 {   k->codisklcp1=26;
 zapros('e',k); //Вписали - "нереализованная операция"
			s3=rpcp1('e',k); if(s3==1) {  
			 return(5); }     // Если будет прерывание.
	if(prz) printkl("#I - PROZ -  Ненормализованный операнд в команде:");
	char* pch=Disass((unsigned long int)k->sledk[4],
		               (unsigned long int)k->sledk[0],0);
	otl.sprintf("# %08X\t",(unsigned long int)k->sledk[0]);
	str=(const char*)pch;
	str=otl+str;
	if(prz) printkl(str); 							                                    
		 }//----- Разделались, если был плохой операнд.

		    if(ss1>1)
      {   k->codisklcp1=26; 
			       zapros('e',k); s3=rpcp1('e',k);
			if(s3==1) {   return(5); }
			     //Выдали реализуемый запрос на прер.
		if(prz) printkl("#E - PROZ -  Некорректная операция в команде:");
	char* pch=Disass((unsigned long int)k->sledk[4],
		               (unsigned long int)k->sledk[0],0);
	otl.sprintf("# %08X\t",(unsigned long int)k->sledk[0]);
	str=(const char*)pch;
	str=otl+str;
	if(prz) printkl(str); 				
   } //Оформили исключит.ситуацию - некорректная операция.
		  goto kkk;
    }//------------------------------КОМАНДУ  ИСПОЛНИЛИ.

      if((r1==16)&&(r4==0)&&(func==57))     //C.NGLE.S
    { ss1=sraw('o',0,1,r2,r3,k);
	    k->sledo[0]=5; k->sledo[1]=2;
	    k->sledo[2]=r2; k->sledo[3]=r3;
         if((s1==3)||(s2==3))   //"Плохие" операнды.
		 {   k->codisklcp1=26;
 zapros('e',k); //Вписали - "нереализованная операция"
			s3=rpcp1('e',k); if(s3==1) {  
			 return(5); }    // Если будет прерывание.
	if(prz) printkl("#I - PROZ -  Ненормализованный операнд в команде:");
	char* pch=Disass((unsigned long int)k->sledk[4],
		               (unsigned long int)k->sledk[0],0);
	otl.sprintf("# %08X\t",(unsigned long int)k->sledk[0]);
	str=(const char*)pch;
	str=otl+str;
	if(prz) printkl(str); 				                         
		 }//---Разделались, если был плохой операнд.

     srawn1:
       if(ss1>0)           //Условие выполнено.
       { k->contrlstatus=(040000000|k->contrlstatus);
	 k->uslsop1=1;
       }   else            //Условие не выполнено.
       { k->contrlstatus=(037737777777&k->contrlstatus);
	 k->uslsop1=0;
       }          if((ss1==2)||(ss1==-2))
      {   k->codisklcp1=26;
	          zapros('e',k); s3=rpcp1('e',k);
			if(s3==1) {   return(5); }
	if(prz) printkl("#E - PROZ -  Некорректная операция в команде:");
	char* pch=Disass((unsigned long int)k->sledk[4],
		               (unsigned long int)k->sledk[0],0);
	otl.sprintf("# %08X\t",(unsigned long int)k->sledk[0]);
	str=(const char*)pch;
	str=otl+str;
	if(prz) printkl(str); 				
			        //Выдали реализуемый запрос на прер.
  }//Оформили исключит.ситуацию - некорректная операция.
		       goto kkk;
    }//------------------------------КОМАНДУ  ИСПОЛНИЛИ.

      if((r1==16)&&(r4==0)&&(func==58))       //C.SEQ.S
    { ss1=sraw('o',2,0,r2,r3,k);
	    k->sledo[0]=5; k->sledo[1]=2;
	    k->sledo[2]=r2; k->sledo[3]=r3;
         if((s1==3)||(s2==3))   //"Плохие" операнды.
		 {  k->codisklcp1=26;
 zapros('e',k); //Вписали - "нереализованная операция"
			s3=rpcp1('e',k); if(s3==1) {  
			 return(5); }  // Если будет прерывание.
	if(prz) printkl("#I - PROZ -  Ненормализованный операнд в команде:");
	char* pch=Disass((unsigned long int)k->sledk[4],
		               (unsigned long int)k->sledk[0],0);
	otl.sprintf("# %08X\t",(unsigned long int)k->sledk[0]);
	str=(const char*)pch;
	str=otl+str;
	if(prz) printkl(str); 							                                    
		 }//------ Разделались, если был плохой операнд.
	  
	  goto srawn1;
    }//------------------------------КОМАНДУ  ИСПОЛНИЛИ.

      if((r1==16)&&(r4==0)&&(func==59))     //C.NGL. S
    { ss1=sraw('o',2,1,r2,r3,k);
	    k->sledo[0]=5; k->sledo[1]=2;
	    k->sledo[2]=r2; k->sledo[3]=r3;
         if((s1==3)||(s2==3))   //"Плохие" операнды.
		 {   k->codisklcp1=26;
 zapros('e',k); //Вписали - "нереализованная операция"
			s3=rpcp1('e',k); if(s3==1) {  
			 return(5); }     // Если будет прерывание.
	if(prz) printkl("#I - PROZ -  Ненормализованный операнд в команде:");
	char* pch=Disass((unsigned long int)k->sledk[4],
		               (unsigned long int)k->sledk[0],0);
	otl.sprintf("# %08X\t",(unsigned long int)k->sledk[0]);
	str=(const char*)pch;
	str=otl+str;
	if(prz) printkl(str); 							                                    
		 }//----- Разделались, если был плохой операнд.
	  
	  goto srawn1;
    }//------------------------------КОМАНДУ  ИСПОЛНИЛИ.


      if((r1==16)&&(r4==0)&&(func==60))      //C.LT. S
    { ss1=sraw('o',1,0,r2,r3,k);
	    k->sledo[0]=5; k->sledo[1]=2;
	    k->sledo[2]=r2; k->sledo[3]=r3;
         if((s1==3)||(s2==3))   //"Плохие" операнды.
		 {   k->codisklcp1=26;
 zapros('e',k); //Вписали - "нереализованная операция"
			s3=rpcp1('e',k); if(s3==1) {  
			 return(5); }    // Если будет прерывание.
	if(prz) printkl("#I - PROZ -  Ненормализованный операнд в команде:");
	char* pch=Disass((unsigned long int)k->sledk[4],
		               (unsigned long int)k->sledk[0],0);
	otl.sprintf("# %08X\t",(unsigned long int)k->sledk[0]);
	str=(const char*)pch;
	str=otl+str;
	if(prz) printkl(str); 							                                    
		 }//---- Разделались, если был плохой операнд.
	  
	  goto srawn1;
    }//------------------------------КОМАНДУ  ИСПОЛНИЛИ.

      if((r1==16)&&(r4==0)&&(func==61))      //C.NGE. S
    { ss1=sraw('o',1,1,r2,r3,k);
	    k->sledo[0]=5; k->sledo[1]=2;
	    k->sledo[2]=r2; k->sledo[3]=r3;
         if((s1==3)||(s2==3))   //"Плохие" операнды.
		 {   k->codisklcp1=26;
 zapros('e',k); //Вписали - "нереализованная операция"
			s3=rpcp1('e',k); if(s3==1) {  
		 return(5); }     // Если будет прерывание.
	if(prz) printkl("#I - PROZ -  Ненормализованный операнд в команде:");
	char* pch=Disass((unsigned long int)k->sledk[4],
		               (unsigned long int)k->sledk[0],0);
	otl.sprintf("# %08X\t",(unsigned long int)k->sledk[0]);
	str=(const char*)pch;
	str=otl+str;
	if(prz) printkl(str); 							                                    
		 }//------ Разделались, если был плохой операнд.
	  
	  goto srawn1;
    }//------------------------------КОМАНДУ  ИСПОЛНИЛИ.

      if((r1==16)&&(r4==0)&&(func==62))       //C.LE. S
    { ss1=sraw('o',3,0,r2,r3,k);
	    k->sledo[0]=5; k->sledo[1]=2;
	    k->sledo[2]=r2; k->sledo[3]=r3;
         if((s1==3)||(s2==3))   //"Плохие" операнды.
		 {   k->codisklcp1=26;
 zapros('e',k); //Вписали - "нереализованная операция"
			s3=rpcp1('e',k); if(s3==1) {  
			 return(5); }     // Если будет прерывание.
	if(prz) printkl("#I - PROZ -  Ненормализованный операнд в команде:");
	char* pch=Disass((unsigned long int)k->sledk[4],
		               (unsigned long int)k->sledk[0],0);
	otl.sprintf("# %08X\t",(unsigned long int)k->sledk[0]);
	str=(const char*)pch;
	str=otl+str;
	if(prz) printkl(str); 							                                   
		 }//------ Разделались, если был плохой операнд.
	  
	  goto srawn1;
    }//------------------------------КОМАНДУ  ИСПОЛНИЛИ.

      if((r1==16)&&(r4==0)&&(func==63))      //C.NGT. S
    { ss1=sraw('o',3,1,r2,r3,k);
	    k->sledo[0]=5; k->sledo[1]=2;
	    k->sledo[2]=r2; k->sledo[3]=r3;
         if((s1==3)||(s2==3))   //"Плохие" операнды.
		 {   k->codisklcp1=26;
 zapros('e',k); //Вписали - "нереализованная операция"
			s3=rpcp1('e',k); if(s3==1) { 
			 return(5); }    // Если будет прерывание.
	if(prz) printkl("#I - PROZ -  Ненормализованный операнд в команде:");
	char* pch=Disass((unsigned long int)k->sledk[4],
		               (unsigned long int)k->sledk[0],0);
	otl.sprintf("# %08X\t",(unsigned long int)k->sledk[0]);
	str=(const char*)pch;
	str=otl+str;
	if(prz) printkl(str); 							                                    
		 }//----- Разделались, если был плохой операнд.
	  
	  goto srawn1;
    }//------------------------------КОМАНДУ  ИСПОЛНИЛИ.
//****************************************************


      if((r1==17)&&(r4==0)&&(func==48))	      //C.F.D
    { k->contrlstatus=(037737777777&k->contrlstatus);
      k->uslsop1=0;        goto kkk;
    }//------------------------------КОМАНДУ  ИСПОЛНИЛИ.

      if((r1==17)&&(r4==0)&&(func==49))       //C.UN.D
    { ss1=sraw('d',0,1,r2,r3,k);
	    k->sledo[0]=5; k->sledo[1]=2;
	    k->sledo[2]=r2; k->sledo[3]=r3;
         if((s1==3)||(s2==3))   //"Плохие" операнды.
		 {   k->codisklcp1=26;
 zapros('e',k); //Вписали - "нереализованная операция"
			s3=rpcp1('e',k); if(s3==1) {  
			 return(5); }    // Если будет прерывание.
	if(prz) printkl("#I - PROZ -  Ненормализованный операнд в команде:");
	char* pch=Disass((unsigned long int)k->sledk[4],
		               (unsigned long int)k->sledk[0],0);
	otl.sprintf("# %08X\t",(unsigned long int)k->sledk[0]);
	str=(const char*)pch;
	str=otl+str;
	if(prz) printkl(str); 							                                    
		 }//----- Разделались, если был плохой операнд.

     srawn2:
       if(ss1>0)           //Условие выполнено.
       { k->contrlstatus=(040000000|k->contrlstatus);
	 k->uslsop1=1;
       }   else            //Условие не выполнено.
       { k->contrlstatus=(037737777777&k->contrlstatus);
	 k->uslsop1=0;
       }                   goto kkk;
    }//------------------------------КОМАНДУ  ИСПОЛНИЛИ.

      if((r1==17)&&(r4==0)&&(func==50))         //C.ED.D
    { ss1=sraw('d',2,0,r2,r3,k);
	    k->sledo[0]=5; k->sledo[1]=2;
	    k->sledo[2]=r2; k->sledo[3]=r3;
         if((s1==3)||(s2==3))   //"Плохие" операнды.
		 {   k->codisklcp1=26;
 zapros('e',k); //Вписали - "нереализованная операция"
			s3=rpcp1('e',k); if(s3==1) {  
			 return(5); }     // Если будет прерывание.
	if(prz) printkl("#I - PROZ -  Ненормализованный операнд в команде:");
	char* pch=Disass((unsigned long int)k->sledk[4],
		               (unsigned long int)k->sledk[0],0);
	otl.sprintf("# %08X\t",(unsigned long int)k->sledk[0]);
	str=(const char*)pch;
	str=otl+str;
	if(prz) printkl(str); 							                                    
		 }//------ Разделались, если был плохой операнд.
	  
	  goto srawn2;
    }//------------------------------КОМАНДУ  ИСПОЛНИЛИ.

      if((r1==17)&&(r4==0)&&(func==51))        //C.UEQ. D
    { ss1=sraw('d',2,1,r2,r3,k);
	    k->sledo[0]=5; k->sledo[1]=2;
	    k->sledo[2]=r2; k->sledo[3]=r3;
         if((s1==3)||(s2==3))   //"Плохие" операнды.
		 {   k->codisklcp1=26;
 zapros('e',k); //Вписали - "нереализованная операция"
			s3=rpcp1('e',k); if(s3==1) {  
			 return(5); }   // Если будет прерывание.
	if(prz) printkl("#I - PROZ -  Ненормализованный операнд в команде:");
	char* pch=Disass((unsigned long int)k->sledk[4],
		               (unsigned long int)k->sledk[0],0);
	otl.sprintf("# %08X\t",(unsigned long int)k->sledk[0]);
	str=(const char*)pch;
	str=otl+str;
	if(prz) printkl(str); 							                                    
		 }//------ Разделались, если был плохой операнд.
	  
	  goto srawn2;
    }//------------------------------КОМАНДУ  ИСПОЛНИЛИ.

      if((r1==17)&&(r4==0)&&(func==52))       //C.OLT. D
    { ss1=sraw('d',1,0,r2,r3,k);
	    k->sledo[0]=5; k->sledo[1]=2;
	    k->sledo[2]=r2; k->sledo[3]=r3;
         if((s1==3)||(s2==3))   //"Плохие" операнды.
		 {  k->codisklcp1=26;
 zapros('e',k); //Вписали - "нереализованная операция"
			s3=rpcp1('e',k); if(s3==1) {  
			 return(5); }   // Если будет прерывание.
	if(prz) printkl("#I - PROZ -  Ненормализованный операнд в команде:");
	char* pch=Disass((unsigned long int)k->sledk[4],
		               (unsigned long int)k->sledk[0],0);
	otl.sprintf("# %08X\t",(unsigned long int)k->sledk[0]);
	str=(const char*)pch;
	str=otl+str;
	if(prz) printkl(str); 							                                    
		 }//------- Разделались, если был плохой операнд.
	  
	  goto srawn2;
    }//------------------------------КОМАНДУ  ИСПОЛНИЛИ.

      if((r1==17)&&(r4==0)&&(func==53))       //C.ULT. D
    { ss1=sraw('d',1,1,r2,r3,k);
	    k->sledo[0]=5; k->sledo[1]=2;
	    k->sledo[2]=r2; k->sledo[3]=r3;
         if((s1==3)||(s2==3))   //"Плохие" операнды.
		 {   k->codisklcp1=26;
 zapros('e',k); //Вписали - "нереализованная операция"
			s3=rpcp1('e',k); if(s3==1) {  
			 return(5); }   // Если будет прерывание.
	if(prz) printkl("#I - PROZ -  Ненормализованный операнд в команде:");
	char* pch=Disass((unsigned long int)k->sledk[4],
		               (unsigned long int)k->sledk[0],0);
	otl.sprintf("# %08X\t",(unsigned long int)k->sledk[0]);
	str=(const char*)pch;
	str=otl+str;
	if(prz) printkl(str); 							                                    
		 }//----- Разделались, если был плохой операнд.
	  
	  goto srawn2;
    }//------------------------------КОМАНДУ  ИСПОЛНИЛИ.

      if((r1==17)&&(r4==0)&&(func==54))      //C.OLE. D
    { ss1=sraw('d',3,0,r2,r3,k);
	    k->sledo[0]=5; k->sledo[1]=2;
	    k->sledo[2]=r2; k->sledo[3]=r3;
         if((s1==3)||(s2==3))   //"Плохие" операнды.
		 {   k->codisklcp1=26;
 zapros('e',k); //Вписали - "нереализованная операция"
			s3=rpcp1('e',k); if(s3==1) {  
			 return(5); }    // Если будет прерывание.
	if(prz) printkl("#I - PROZ -  Ненормализованный операнд в команде:");
	char* pch=Disass((unsigned long int)k->sledk[4],
		               (unsigned long int)k->sledk[0],0);
	otl.sprintf("# %08X\t",(unsigned long int)k->sledk[0]);
	str=(const char*)pch;
	str=otl+str;
	if(prz) printkl(str); 				                
		 }//----- Разделались, если был плохой операнд.
	  
	  goto srawn2;
    }//------------------------------КОМАНДУ  ИСПОЛНИЛИ.

      if((r1==17)&&(r4==0)&&(func==55))      //C.ULE. D
    { ss1=sraw('d',3,1,r2,r3,k);
	    k->sledo[0]=5; k->sledo[1]=2;
	    k->sledo[2]=r2; k->sledo[3]=r3;
         if((s1==3)||(s2==3))   //"Плохие" операнды.
		 {   k->codisklcp1=26;
 zapros('e',k); //Вписали - "нереализованная операция"
			s3=rpcp1('e',k); if(s3==1) {  
			 return(5); }   // Если будет прерывание.
	if(prz) printkl("#I - PROZ -  Ненормализованный операнд в команде:");
	char* pch=Disass((unsigned long int)k->sledk[4],
		               (unsigned long int)k->sledk[0],0);
	otl.sprintf("# %08X\t",(unsigned long int)k->sledk[0]);
	str=(const char*)pch;
	str=otl+str;
	if(prz) printkl(str); 							                                    
		 }//------ Разделались, если был плохой операнд.
	  
	  goto srawn2;
    }//------------------------------КОМАНДУ  ИСПОЛНИЛИ.
//******************************************************
      if((r1==17)&&(r4==0)&&(func==56))	      //C.SF.D
    { k->contrlstatus=(037737777777&k->contrlstatus);
	    k->sledo[0]=5; k->sledo[1]=2;
	    k->sledo[2]=r2; k->sledo[3]=r3;
      k->uslsop1=0;  ss1=sraw('d',0,1,r2,r3,k);
         if((s1==3)||(s2==3))   //"Плохие" операнды.
		 {   k->codisklcp1=26;
 zapros('e',k); //Вписали - "нереализованная операция"
			s3=rpcp1('e',k); if(s3==1) { 
			 return(5); }  // Если будет прерывание.
	if(prz) printkl("#I - PROZ -  Ненормализованный операнд в команде:");
	char* pch=Disass((unsigned long int)k->sledk[4],
		               (unsigned long int)k->sledk[0],0);
	otl.sprintf("# %08X\t",(unsigned long int)k->sledk[0]);
	str=(const char*)pch;
	str=otl+str;
	if(prz) printkl(str); 				                
		 }//------ Разделались, если был плохой операнд.

		    if(ss1>1)
      {   k->codisklcp1=26;
			      zapros('e',k); s3=rpcp1('e',k);
			if(s3==1) { return(5); }
		        //Выдали реализуемый запрос на прер.
		if(prz) printkl("#E - PROZ -  Некорректная операция в команде:");
	char* pch=Disass((unsigned long int)k->sledk[4],
		               (unsigned long int)k->sledk[0],0);
	otl.sprintf("# %08X\t",(unsigned long int)k->sledk[0]);
	str=(const char*)pch;
	str=otl+str;
	if(prz) printkl(str); 				                       
   } //Оформили исключит.ситуацию - некорректная операция.
		  goto kkk;
    }//------------------------------КОМАНДУ  ИСПОЛНИЛИ.


      if((r1==17)&&(r4==0)&&(func==57))       //C.NGLE.D
    { ss1=sraw('d',0,1,r2,r3,k);
	    k->sledo[0]=5; k->sledo[1]=2;
	    k->sledo[2]=r2; k->sledo[3]=r3;
         if((s1==3)||(s2==3))   //"Плохие" операнды.
		 {  k->codisklcp1=26;
 zapros('e',k); //Вписали - "нереализованная операция"
			s3=rpcp1('e',k); if(s3==1) {  
			 return(5); }  // Если будет прерывание.
	if(prz) printkl("#I - PROZ -  Ненормализованный операнд в команде:");
	char* pch=Disass((unsigned long int)k->sledk[4],
		               (unsigned long int)k->sledk[0],0);
	otl.sprintf("# %08X\t",(unsigned long int)k->sledk[0]);
	str=(const char*)pch;
	str=otl+str;
	if(prz) printkl(str); 				                			                                    
		 }//------- Разделались, если был плохой операнд.

     srawn3:
       if(ss1>0)           //Условие выполнено.
       { k->contrlstatus=(040000000|k->contrlstatus);
	 k->uslsop1=1;
       }   else            //Условие не выполнено.
       { k->contrlstatus=(037737777777&k->contrlstatus);
	 k->uslsop1=0;
       }          if((ss1==2)||(ss1==-2))
      {   k->codisklcp1=26;
	               zapros('e',k); s3=rpcp1('e',k);
			if(s3==1) {  return(5); }
			        //Выдали реализуемый запрос на прер.
		if(prz) printkl("#E - PROZ -  Некорректная операция в команде:");
	char* pch=Disass((unsigned long int)k->sledk[4],
		               (unsigned long int)k->sledk[0],0);
	otl.sprintf("# %08X\t",(unsigned long int)k->sledk[0]);
	str=(const char*)pch;
	str=otl+str;
	if(prz) printkl(str); 				                
  } //Оформили исключит.ситуацию - некорректная операция.
		       goto kkk;
    }//------------------------------КОМАНДУ  ИСПОЛНИЛИ.

      if((r1==17)&&(r4==0)&&(func==58))      //C.SEQ.D
    { ss1=sraw('d',2,0,r2,r3,k);
	    k->sledo[0]=5; k->sledo[1]=2;
	    k->sledo[2]=r2; k->sledo[3]=r3;
         if((s1==3)||(s2==3))   //"Плохие" операнды.
		 {   k->codisklcp1=26;
 zapros('e',k); //Вписали - "нереализованная операция"
			s3=rpcp1('e',k); if(s3==1) {  
			 return(5); }  // Если будет прерывание.
	if(prz) printkl("#I - PROZ -  Ненормализованный операнд в команде:");
	char* pch=Disass((unsigned long int)k->sledk[4],
		               (unsigned long int)k->sledk[0],0);
	otl.sprintf("# %08X\t",(unsigned long int)k->sledk[0]);
	str=(const char*)pch;
	str=otl+str;
	if(prz) printkl(str); 				                			                                    
		 }//----- Разделались, если был плохой операнд.
	  
	  goto srawn3;
    }//------------------------------КОМАНДУ  ИСПОЛНИЛИ.

      if((r1==17)&&(r4==0)&&(func==59))      //C.NGL. D
    { ss1=sraw('d',2,1,r2,r3,k);
	    k->sledo[0]=5; k->sledo[1]=2;
	    k->sledo[2]=r2; k->sledo[3]=r3;
         if((s1==3)||(s2==3))   //"Плохие" операнды.
		 {   k->codisklcp1=26;
 zapros('e',k); //Вписали - "нереализованная операция"
			s3=rpcp1('e',k); if(s3==1) {  
			 return(5); }  // Если будет прерывание.
	if(prz) printkl("#I - PROZ -  Ненормализованный операнд в команде:");
	char* pch=Disass((unsigned long int)k->sledk[4],
		               (unsigned long int)k->sledk[0],0);
	otl.sprintf("# %08X\t",(unsigned long int)k->sledk[0]);
	str=(const char*)pch;
	str=otl+str;
	if(prz) printkl(str); 				                			                                    
		 }//---- Разделались, если был плохой операнд.
	  
	  goto srawn3;
    }//------------------------------КОМАНДУ  ИСПОЛНИЛИ.

      if((r1==17)&&(r4==0)&&(func==60))       //C.LT. D
    { ss1=sraw('d',1,0,r2,r3,k);
	    k->sledo[0]=5; k->sledo[1]=2;
	    k->sledo[2]=r2; k->sledo[3]=r3;
         if((s1==3)||(s2==3))   //"Плохие" операнды.
		 {   k->codisklcp1=26;
 zapros('e',k); //Вписали - "нереализованная операция"
			s3=rpcp1('e',k); if(s3==1) {  
			 return(5); }  // Если будет прерывание.
	if(prz) printkl("#I - PROZ -  Ненормализованный операнд в команде:");
	char* pch=Disass((unsigned long int)k->sledk[4],
		               (unsigned long int)k->sledk[0],0);
	otl.sprintf("# %08X\t",(unsigned long int)k->sledk[0]);
	str=(const char*)pch;
	str=otl+str;
	if(prz) printkl(str); 				                			                                    
		 }//----- Разделались, если был плохой операнд.
	  
	  goto srawn3;
    }//------------------------------КОМАНДУ  ИСПОЛНИЛИ.

      if((r1==17)&&(r4==0)&&(func==61))      //C.NGE. D
    { ss1=sraw('d',1,1,r2,r3,k);
	    k->sledo[0]=5; k->sledo[1]=2;
	    k->sledo[2]=r2; k->sledo[3]=r3;
         if((s1==3)||(s2==3))   //"Плохие" операнды.
		 {   k->codisklcp1=26;
 zapros('e',k); //Вписали - "нереализованная операция"
			s3=rpcp1('e',k); if(s3==1) {  
			 return(5); }   // Если будет прерывание.
	if(prz) printkl("#I - PROZ -  Ненормализованный операнд в команде:");
	char* pch=Disass((unsigned long int)k->sledk[4],
		               (unsigned long int)k->sledk[0],0);
	otl.sprintf("# %08X\t",(unsigned long int)k->sledk[0]);
	str=(const char*)pch;
	str=otl+str;
	if(prz) printkl(str); 				                			                                    
		 }//----- Разделались, если был плохой операнд.
	  
	  goto srawn3;
    }//------------------------------КОМАНДУ  ИСПОЛНИЛИ.

      if((r1==17)&&(r4==0)&&(func==62))      //C.LE. D
    { ss1=sraw('d',3,0,r2,r3,k);
	    k->sledo[0]=5; k->sledo[1]=2;
	    k->sledo[2]=r2; k->sledo[3]=r3;
         if((s1==3)||(s2==3))   //"Плохие" операнды.
		 {   k->codisklcp1=26;
 zapros('e',k); //Вписали - "нереализованная операция"
			s3=rpcp1('e',k); if(s3==1) {  
			 return(5); }    // Если будет прерывание.
	if(prz) printkl("#I - PROZ -  Ненормализованный операнд в команде:");
	char* pch=Disass((unsigned long int)k->sledk[4],
		               (unsigned long int)k->sledk[0],0);
	otl.sprintf("# %08X\t",(unsigned long int)k->sledk[0]);
	str=(const char*)pch;
	str=otl+str;
	if(prz) printkl(str); 				                			                                    
		 }//----- Разделались, если был плохой операнд.
	  
	  goto srawn3;
    }//------------------------------КОМАНДУ  ИСПОЛНИЛИ.

      if((r1==17)&&(r4==0)&&(func==63))      //C.NGT. D
    { ss1=sraw('d',3,1,r2,r3,k); 
	    k->sledo[0]=5; k->sledo[1]=2;
	    k->sledo[2]=r2; k->sledo[3]=r3;
         if((s1==3)||(s2==3))   //"Плохие" операнды.
		 {   k->codisklcp1=26;
 zapros('e',k); //Вписали - "нереализованная операция"
			s3=rpcp1('e',k); if(s3==1) {  
			 return(5); }   // Если будет прерывание.
	if(prz) printkl("#I - PROZ -  Ненормализованный операнд в команде:");
	char* pch=Disass((unsigned long int)k->sledk[4],
		               (unsigned long int)k->sledk[0],0);
	otl.sprintf("# %08X\t",(unsigned long int)k->sledk[0]);
	str=(const char*)pch;
	str=otl+str;
	if(prz) printkl(str); 				                			                                    
		 }//----Разделались, если был плохой операнд.
	  
	  goto srawn3;
    }//------------------------------КОМАНДУ  ИСПОЛНИЛИ.


  if((r1==17)&&(func==0)) //Сложение двойных чисел ADD.D
       { k->contrlstatus=(037777007777&k->contrlstatus);
   slog=1; s1=kto(r3,'d',1,k); s2=kto(r2,'d',2,k); 
	                         //Взяли операнды.
   dlawisd:
       k->sledo[0]=2; k->sledo[1]=2;
	   k->sledo[2]=r2; k->sledo[3]=r3; k->sledo[4]=r4;
         u3=(int *)(&m3);
  if((s1==3)||(s1>=6)||(s2==3)||(s2>=6)) 
	                            //"Плохой" операнд.
		 {   k->codisklcp1=26;
            zapros('e',k); 
			     //Вписали - "нереализованная операция"  
		   s3=rpcp1('e',k); 
	if(s3==1) {   return(5); } //Если будет прерывание.			                               
		  else { 
		if(prz) printkl("#E - PROZ -  Некорректный операнд в команде:");
	char* pch=Disass((unsigned long int)k->sledk[4],
		               (unsigned long int)k->sledk[0],0);
	otl.sprintf("# %08X\t",(unsigned long int)k->sledk[0]);
	str=(const char*)pch;
	str=otl+str;
	if(prz) printkl(str); 				                			  
			  k->fgr[r4+1]=0x7ff80000; goto kkk; }
		 }//--- Разделались, если был плохой операнд.

	if(((s1==4)&&(s2==5))||((s2==4)&&(s1==5)))
		                           //Бескон.конфликтуют.
    {  zapros('e',k);
	   s3=rpcp1('e',k); 
	   if(s3==1) 
	   {k->codisklcp1=25;
	    k->contrlstatus=(037777007777&k->contrlstatus); 
		 zapros('v',k);   return(5); 
	   } else
		{
		 if(prz) printkl("#E - PROZ -  Конфликт бесконечностей в команде:");
	char* pch=Disass((unsigned long int)k->sledk[4],
		               (unsigned long int)k->sledk[0],0);
	otl.sprintf("# %08X\t",(unsigned long int)k->sledk[0]);
	str=(const char*)pch;
	str=otl+str;
	if(prz) printkl(str); 				                			  		   
		  k->codisklcp1=26; zapros('e',k); 
		  k->fgr[r4+1]=0x7ff80000; goto kkk;
		}
	}//------------Разделались с конфликтн. бесконечн.
     
//----Теперь, если бесконечности, то они не конфликтуют:
	 if(s1>=4) 
	//Первое слагаемое бесконечность. Сигналов не будет.
	 { k->fgr[r4]=0;
		 if(sd1==0) 
 k->fgr[r4+1]=plusbd; else k->fgr[r4+1]=minusbd; goto kkk;}
	else if(s2>=4)
	//Второе слагаемое бесконечность. Сигналов не будет.
       { k->fgr[r4]=0;
 if(sd2==0) k->fgr[r4+1]=plusbd;
                     else k->fgr[r4+1]=minusbd; goto kkk;}
	//------Бесконечный результат вписан. Ушли на выход !

    //----Теперь работа с нулевыми операндами:
    if((s1==2)&&(s2==2))   //-0 + (-0)
 {pd1=-1023; u3[1]=0; u3[0]=0; sd1=1; goto wpisd;}//Будет -0.
    else if(((s1>0)&&(s1<3))&&((s2>0)&&(s2<3))) 
		                                //Оба нули, но иные. 
{pd1=-1023; u3[1]=0; u3[0]=0; sd1=0; goto wpisd;} //Будет +0.
	else if((s1>0)&&(s1<3))        //Только первое слаг.=0.
	{k->fgr[r4]=k->fgr[r2]; k->fgr[r4+1]=k->fgr[r2+1];
	 if(slog==0) k->fgr[r4+1]=020000000000^(k->fgr[r4+1]); 
	                    goto kkk;}
	else if((s2>0)&&(s2<3))        //Только второе слаг.=0.
{k->fgr[r4]=k->fgr[r3]; k->fgr[r4+1]=k->fgr[r3+1];goto kkk;}
	else { }
	   //-------------- Конец  нулей ----------------      
			 			 	   
	//Ура! Начинаем готовить штатное сложение:
	 //Учтем подразумев. разряд: числа нормализованы:
	 u1=(int *)(&fd1);  u2=(int *)(&fd2);
	 u1[1]=(04000000|u1[1]); 
	 u2[1]=(04000000|u2[1]); //Учли.
	  //-------------------
       if(pd1<pd2)          //Тогда переставим слагаемые:
       {per=pd1; pd1=pd2; pd2=per; //Переставили порядки.
	 zn=sd1; sd1=sd2; sd2=zn;  //Переставили знаки.
	 a=fd1; fd1=fd2;   fd2=a;  //Переставили мантиссы.
       }    //---------Теперь порядок pd1 не меньше pd2.

      delt=pd1-pd2; m1=fd1; m2=fd2; m4=0; u4=(int *)(&m4);
       xwost=0;	    for(i=1;i<=delt;i++)
	  { if((1&m4)!=0)  {if(sd2==0) xwost=1; else xwost=(-1);}
		 m4=m4>>1; if((01&m2)!=0)  u4[1]=020000000000|u4[1];
		 m2=m2>>1;
		}
	 //--------- В  m4  64-х разрядный хвост от сдвига  fd2
	 //при выравнивании порядков.	 Теперь сложение мантисс:
		       
	if(sd1==sd2)  m3=m1+m2;   //Хвост m4 не корректируется.
 else { u4[0]=(037777777777^u4[0]); u4[1]=(037777777777^u4[1]);
		  m4=(m4+1); u2=(int *)(&m2);
		u2[0]=(037777777777^u2[0]);
		u2[1]=(037777777777^u2[1]);
	if(m4==0) m2=(m2+1);//Учет возможн.переноса от хвоста m4.
		m3=m1+m2;       
	      }
//---------Сложили их. Мантисса-результат в m3.(Но может < 0). 
//----Общий знак результата в sd1.  Хвост в m4. Порядок в pd1.
                  u3=(int *)(&m3);
                                                 
	     if((020000000000&u3[1])!=0)   //Если минус у m3:
	{ u3[0]=(037777777777^u3[0]); u3[1]=(037777777777^u3[1]);
	   u4[0]=(037777777777^u4[0]); u4[1]=(037777777777^u4[1]);
		 m4=(m4+1); if(m4==0) m3=(m3+1);
	    //Это было инвертирование m3 в прямой код.
 if(sd1==1) sd1=0; else sd1=1; //Инвертир.знака рез-та в sd1.
}//----Получили ненормализ.мантиссу в m3 и m4 в прямом коде,
 //----порядок в pd1, знак рез-та в sd1. ------------------
   oformlend:
      if((m3==0)&&(m4==0)) goto nolld;  //На оформление нуля 
			       //и конца и для операций  - , * , /.
	 //---- Начнем нормализацию ненулевого результата:           
  u4=(int *)(&m4); if((010000000&u3[1])!=0) 
 //Мантисса выскочила влево,но не может более чем на 1 разр.
       { m4=(m4>>1); if((1&m3)!=0) u4[1]=(020000000000|u4[1]);
	       m3=(m3>>1); pd1=(pd1+1); 
       } else { while((04000000&u3[1])==0) 
		                    //Мантисса с правым уклоном.
		{ m3=(m3<<1); if((020000000000&u4[1])!=0) m3=(01|m3);
		  m4=(m4<<1); pd1=(pd1-1);  
		}  //Есть возможность нештатного порядка.
	      }
   //*************  Нормализовали. Нормализованный результат - 	
   //отличен от нуля и с 1 в 20-м разряде левой части  тары m3,
   //хвост в m4,  несмещ.порядок в pd1, знак числа в sd1.
   // u3[] и u4[] отражают тару m3 и m4.
   //******************************************************
          s1=okrugd(k);
		if(s1==9)                //Ненормализ.рез-тат.
		{  k->codisklcp1=26; zapros('e',k);		
          s3=rpcp1('e',k); 
		  if(s3==1)  {  return(5);}
		  else { 
if(prz) printkl("#I - PROZ -  Ненормализованный результат в команде:");
	char* pch=Disass((unsigned long int)k->sledk[4],
		               (unsigned long int)k->sledk[0],0);
	otl.sprintf("# %08X\t",(unsigned long int)k->sledk[0]);
	str=(const char*)pch;
	str=otl+str;
	if(prz) printkl(str); 				                			  			  
			 /* k->fgr[r4+1]=0x7ff80000;*/ goto kkk;} 
        } //------------Разделались с ненормализованным.
		 else if(s1==8)              //Переполнение.

      {   k->codisklcp1=23;
		           zapros('o',k); zapros('i',k);
		 s3=rpcp1('o',k); s4=rpcp1('i',k); 
	     if((s3==1)||(s4==1)) 
		 {  return(5);}
		 else {
if(prz) printkl("#I - PROZ -  Внимание! Было переполнение в команде:");
	char* pch=Disass((unsigned long int)k->sledk[4],
		               (unsigned long int)k->sledk[0],0);
	otl.sprintf("# %08X\t",(unsigned long int)k->sledk[0]);
	str=(const char*)pch;
	str=otl+str;
	if(prz) printkl(str); 				                			  			 
			 flagi('o',k); flagi('i',k); goto wpisd; }  
	           
      }	    //--------------Разделались с переполнением

		 else if(s1==0)     //Нормализованный рез-тат.
	{ if(netos==0) goto wpisd;   zapros('i',k);
		 s3=rpcp1('i',k); if(s3==1) 
		 {  k->codisklcp1=21;  return(5);} 
		 else 
		 {flagi('i',k);  k->codisklcp1=21; goto wpisd; }   

    } //---------------Разделались с нормализованным.
		 else { } 
      wpisd: u4=(int *)(&m4); m4=m3; //Вписали мантиссу.
  pd1=(pd1+1023); pd1=(pd1<<20);
       u4[1]=(u4[1]|pd1); //Вписали порядок,сместив его.
 if(sd1==1)  u4[1]=(020000000000|u4[1]); //Вписали минус.
 k->fgr[r4]=u4[0]; k->fgr[r4+1]=u4[1];//Вписали весь ответ.
	  //-----------------------------------------
	      goto kkk;                 //На концовку. 
		     //-----------------------
     nolld: { k->fgr[r4]=0; k->fgr[r4+1]=0;} //В ответ +0.
	  
		      goto kkk;   
     }//--------------------------КОМАНДУ   ИСПОЛНИЛИ.


  if((r1==17)&&(func==1)) //Вычитание двойных чисел  SUB.D
      { k->contrlstatus=(037777007777&k->contrlstatus);
     slog=0;  s1=kto(r3,'d',1,k); s2=kto(r2,'d',2,k);
	                                      //Взяли операнды.
       if(sd2==1) sd2=0; else sd2=1; goto dlawisd;
	                                      //На общий хвост.
      }//-------------------------------КОМАНДУ   ИСПОЛНИЛИ. 		     

     if((r1==17)&&(r2==0)&&(func==5))            //  ABS.D
     { k->contrlstatus=(037777007777&k->contrlstatus);
	   k->sledo[2]=r3; k->sledo[4]=r4;
      s1=kto(r3,'d',1,k);  if((s1==3)||(s1>=6)) 
		                     //Не число, некорректная опер.
       {   k->codisklcp1=26;         
	 zapros('e',k); s3=rpcp1('e',k);                    
	if(s3==1) {   return(5); } //Выдача запроса на прерыв.
	else { 
	if(prz) printkl("#E - PROZ -  Некорректный операнд в команде:");
	char* pch=Disass((unsigned long int)k->sledk[4],
		               (unsigned long int)k->sledk[0],0);
	otl.sprintf("# %08X\t",(unsigned long int)k->sledk[0]);
	str=(const char*)pch;
	str=otl+str;
	if(prz) printkl(str); 				                			  		
		k->fgr[r4]=0;  k->fgr[r4+1]=0x7ff80000;             
	 goto kkk;}  
       }//------Закончили с некорректностью. 
       rab1=k->fgr[r3+1]; rab1=(017777777777&rab1);
	                                    //Абс.величина.
       k->fgr[r4+1]=rab1; k->fgr[r4]=k->fgr[r3];
	 goto kkk;  //Вписали ответ и на конец.
    }//---------------------------------КОМАНДУ  ИСПОЛНИЛИ.

      if((r1==17)&&(r2==0)&&(func==6))     //  MOV.D
    { // k->contrlstatus=(037777007777&k->contrlstatus);    
		                             //     Это пересылка.
	   k->sledo[2]=r3; k->sledo[4]=r4;
		 k->fgr[r4]=k->fgr[r3]; k->fgr[r4+1]=k->fgr[r3+1];
		                  goto kkk;       
    }//---------------------------------КОМАНДУ  ИСПОЛНИЛИ.


 if((r1==17)&&(func==2)) //Умножение двойных чисел  MUL.D
       { k->contrlstatus=(037777007777&k->contrlstatus);
       k->sledo[0]=3; k->sledo[1]=5;
	   k->sledo[2]=r2; k->sledo[3]=r3; k->sledo[4]=r4;
       s1=kto(r3,'d',1,k); s2=kto(r2,'d',2,k); 
	                                     //Взяли операнды.
         if((s1==3)||(s1>=6)||(s2==3)||(s2>=6))
			                           //"Плохой" операнд.
	 {   k->codisklcp1=26; zapros('e',k);
		    s3=rpcp1('e',k);    
		if(s3==1) {  return(5); }  //Если будет прерывание.		                    
			else { 
	if(prz) printkl("#E - PROZ -  Некорректный операнд в команде:");
	char* pch=Disass((unsigned long int)k->sledk[4],
		               (unsigned long int)k->sledk[0],0);
	otl.sprintf("# %08X\t",(unsigned long int)k->sledk[0]);
	str=(const char*)pch;
	str=otl+str;
	if(prz) printkl(str); 				                			  				
	k->fgr[r4]=0; k->fgr[r4+1]=0x7ff80000; goto kkk; }
		 }//--- Разделались, если был плохой операнд.

	     if((((s1==4)||(s1==5)) && ((s2==1)||(s2==2)) )||
		( ((s2==4)||(s2==5)) && ((s1==1)||(s1==2)) ) )      
	       //Некорректн операция. Умнож.бескон.на ноль.  
    {  zapros('e',k);
	   s3=rpcp1('e',k);    
           if(s3==1)
		  { k->codisklcp1=25;
            k->contrlstatus=(037777007777&k->contrlstatus);
		   zapros('v',k);
		                          return(5); }
		  else {
 if(prz) printkl("#E - PROZ -  Некорректн.умножение бескон.на 0 в команде:");
	char* pch=Disass((unsigned long int)k->sledk[4],
		               (unsigned long int)k->sledk[0],0);
	otl.sprintf("# %08X\t",(unsigned long int)k->sledk[0]);
	str=(const char*)pch;
	str=otl+str;
	if(prz) printkl(str); 				                			  			  
			  k->fgr[r4]=0; k->fgr[r4+1]=0x7ff80000;
		       zapros('e',k); k->codisklcp1=26; goto kkk;}
	}//----------Разделались с конфликтом бесконечн.и нуля.

       //------------------------ А бесконечность без нуля?
 if((s1>=4)||(s2>=4))  //Хотя бы один сомножитель бесконечен.
	   { k->fgr[r4]=0; if(sd1==sd2) k->fgr[r4+1]=plusbd;
	     else k->fgr[r4+1]=minusbd; goto kkk;}
	   //----А есть ли в сомножителях нули?
	   if(((s1==1)||(s1==2))||((s2==1)||(s2==2))) 
		                             //Умножение на ноль.
	   { k->fgr[r4]=0; if(sd1==sd2) k->fgr[r4+1]=0;
		    else k->fgr[r4+1]=020000000000; goto kkk;}

 //------Конец бескон.и нулей. Начинаем нормальное умножение:
	 //Учтем подразумев. 20 -й разряд, числа нормализованы:
	    u1=(int *)(&fd1);  u2=(int *)(&fd2);
	 u1[1]=(04000000|u1[1]); 
	 u2[1]=(04000000|u2[1]);  //Учли.
	if(sd1==sd2) sd1=0; else sd1=1; //Учли знаки сомножителей.
	 pd1=(pd1+pd2);   //Знак рез-тата в sd1 , порядок в pd1.
//---Но еще надо перемножить нормализ. мантиссы в fd1 и fd2: 
	    m3=0; m4=0;	u4=(int *)(&m4); u3=(int *)(&m3); 
	      for(rab1=1;rab1<=53;rab1=rab1+1)
{m4=(m4>>1); if(1&m3) u4[1]=(020000000000|u4[1]); m3=(m3>>1);
	     if(1&fd1) m3=(m3+fd2);    fd1=(fd1>>1);
}//Перемножили, ответ в m3 (хвост в m4), запятая не сдвинута.       
			goto oformlend;    //На общий хвост-концовку.

    }//================================КОМАНДУ  ИСПОЛНИЛИ.


  if((r1==17)&&(func==3))     //Деление двойных чисел DIV.D
     { k->contrlstatus=(037777007777&k->contrlstatus);
       k->sledo[0]=4; k->sledo[1]=19;
	   k->sledo[2]=r2; k->sledo[3]=r3; k->sledo[4]=r4;
  s1=kto(r3,'d',1,k); s2=kto(r2,'d',2,k); //Взяли операнды.
         if((s1==3)||(s1>=6)||(s2==3)||(s2>=6))  
			                            //"Плохой" операнд.
		 {   k->codisklcp1=26;
		    s3=rpcp1('e',k); zapros('e',k);
			      //Вписали - "нереализованная операция"
			if(s3==1) {   return(5);} //Если будет прерывание.
			else {
	if(prz) printkl("#E - PROZ -  Некорректный операнд в команде:");
	char* pch=Disass((unsigned long int)k->sledk[4],
		               (unsigned long int)k->sledk[0],0);
	otl.sprintf("# %08X\t",(unsigned long int)k->sledk[0]);
	str=(const char*)pch;
	str=otl+str;
	if(prz) printkl(str); 				                			  				
	k->fgr[r4]=0; k->fgr[r4+1]=0x7ff80000; goto kkk; }
		 }//--------- Разделались, если был плохой операнд.


	     if((((s1==1)||(s1==2)) && ((s2==1)||(s2==2)) )||
	( ((s2==4)||(s2==5)) && ((s1==4)||(s1==5)) ) ) 
	  //Некорректн операция: бескон / бескон, ноль / ноль.
    { 
	   s3=rpcp1('e',k);    
           if(s3==1)
		  { k->codisklcp1=25;
		     k->contrlstatus=(037777007777&k->contrlstatus);
		   zapros('v',k);
		                          return(5); }
		  else {
if(prz) printkl("#E - PROZ -  Некорректное деление беск/беск или 0/0 :");
	char* pch=Disass((unsigned long int)k->sledk[4],
		               (unsigned long int)k->sledk[0],0);
	otl.sprintf("# %08X\t",(unsigned long int)k->sledk[0]);
	str=(const char*)pch;
	str=otl+str;
	if(prz) printkl(str); 				                			  			  
			  k->fgr[r4]=0; k->fgr[r4+1]=0x7ff80000;
		    zapros('e',k); k->codisklcp1=26; goto kkk;}
	}//-------Разделались с конфликтом бесконечн.и нуля.
	
       //-----------------Деление обычного числа  на 0 :
	   if(((s2==1)||(s2==2))&&(s1==0))  
     {   k->codisklcp1=24;
	   zapros('z',k); s3=rpcp1('z',k); 
	   if(s3==1) {   return(5); } 	
      else
	  { 
if(prz) printkl("#I - PROZ -  Внимание! было деление на 0 в команде:");
	char* pch=Disass((unsigned long int)k->sledk[4],
		               (unsigned long int)k->sledk[0],0);
	otl.sprintf("# %08X\t",(unsigned long int)k->sledk[0]);
	str=(const char*)pch;
	str=otl+str;
	if(prz) printkl(str); 				                			  		  
		  k->fgr[r4]=0; if(sd1==sd2) k->fgr[r4+1]=plusbd;
	else k->fgr[r4+1]=minusbd; flagi('z',k); goto kkk;}

     }  //-----------------------------------------

       //-----------------Деление  бесконечности на 0:
	   if(((s2==1)||(s2==2))&&(s1>=4))  
     {       
      k->fgr[r4]=0;  if(sd1==sd2) k->fgr[r4+1]=plusbd;
	   else k->fgr[r4+1]=minusbd; goto kkk;
     }  //-----------------------------------------

 if((s2==4)||(s2==5)) //Деление не бесконечности на бескон. 
	   { k->fgr[r4]=0;   if(sd1==sd2) k->fgr[r4+1]=0;
	    else k->fgr[r4+1]=020000000000; goto kkk;}
        //--------------------------------------------
	   if((s1==1)||(s1==2)) //Деление 0 на не ноль.
	   {k->fgr[r4]=0;  if(sd1==sd2) k->fgr[r4+1]=0;
	    else k->fgr[r4+1]=020000000000; goto kkk;}

//---Конец бесконечн и нулей. Начинаем нормальное деление:
//Учтем подразумев. 20 -й разряд,  числа нормализованы:

	    u1=(int *)(&fd1);  u2=(int *)(&fd2);
	 u1[1]=(04000000|u1[1]); 
	 u2[1]=(04000000|u2[1]);  //Учли.
 if(sd1==sd2) sd1=0; else sd1=1;  //Учли знаки сомножителей.

	 pd1=(pd1-pd2);   //Знак рез-тата в sd1 , порядок в pd1.
//---Но еще надо поделить нормализ. мантиссы в fd1 и fd2: 
	    m3=0; m4=0;	u4=(int *)(&m4); u3=(int *)(&m3); 
	      for(rab1=1;rab1<=117;rab1=rab1+1)
{m3=(m3<<1); if(020000000000&u4[1]) m3=(1|m3); m4=(m4<<1);
if((fd1-fd2)>=0) { m4=(1|m4); fd1=(fd1-fd2); fd1=fd1<<1;}
		     else fd1=fd1<<1;
}//Разделили, ответ в m3 (хвост в m4), запятая не сдвинута.       
	 if((04000000&u3[1])==0)   //Первый разр.результата - 0.
{ m3=(m3<<1); if(020000000000&u4[1]) m3=(1|m3); m4=(m4<<1);
	    if((fd1-fd2)>=0)  m4=(1|m4);   pd1=(pd1-1);
	}
		  goto oformlend;    //На общий хвост-концовку.
    }//---------------------------КОМАНДУ  ИСПОЛНИЛИ.

     if((r1==17)&&(r2==0)&&(func==7))        //  NEG.D
     {// k->contrlstatus=(037777007777&k->contrlstatus);
	   k->sledo[0]=5; k->sledo[2]=r3; k->sledo[4]=r4;
      s1=kto(r3,'d',1,k);    
	   rab1=k->fgr[r3+1]; 
	   if(020000000000&rab1) rab1=(017777777777&rab1);
	       else              rab1=(020000000000|rab1);
	   k->fgr[r4]=k->fgr[r3]; k->fgr[r4+1]=rab1;

	                if((s1>=6)||(s2==3)) 
      {  k->codisklcp1=26; zapros('e',k); s3=rpcp1('e',k);
			if(s3==1) {   return(5); }
			        //Выдали реализуемый запрос на прер.
	if(prz) printkl("#E - PROZ -  Некорректный операнд в команде:");
	char* pch=Disass((unsigned long int)k->sledk[4],
		               (unsigned long int)k->sledk[0],0);
	otl.sprintf("# %08X\t",(unsigned long int)k->sledk[0]);
	str=(const char*)pch;
	str=otl+str;
	if(prz) printkl(str); 				                			  
 } //Оформили исключит.ситуацию - нереализованная операция.

           goto kkk;
	 }//----------------------------КОМАНДУ ИСПОЛНИЛИ


     if((r1==16)&&(r2==0)&&(func==7))        //  NEG.S
     { k->contrlstatus=(037777007777&k->contrlstatus);
	   k->sledo[0]=5; k->sledo[2]=r3; k->sledo[4]=r4;
      s1=kto(r3,'o',1,k);    
	   rab1=k->fgr[r3]; 
	   if(020000000000&rab1) rab1=(017777777777&rab1);
	       else              rab1=(020000000000|rab1);
	   k->fgr[r4]=rab1; 	     
                    if((s1>=6)||(s1==3)) 
      {   k->codisklcp1=26;
					 zapros('e',k); s3=rpcp1('e',k);
			if(s3==1) {  return(5); }
			        //Выдали реализуемый запрос на прер.
	if(prz) printkl("#E - PROZ -  Некорректный операнд в команде:");
	char* pch=Disass((unsigned long int)k->sledk[4],
		               (unsigned long int)k->sledk[0],0);
	otl.sprintf("# %08X\t",(unsigned long int)k->sledk[0]);
	str=(const char*)pch;
	str=otl+str;
	if(prz) printkl(str); 				                			  
} //Оформили исключит.ситуацию - нереализованная операция.

           goto kkk;
	 }//-----------------------------КОМАНДУ ИСПОЛНИЛИ
	       
	       //Если непредусмотренная:
	  soob.sprintf
("#E - PROZ - Непредусмотр.команда %o по адресу %X",kodkom,k->pc);
	    if(prz) printkl(soob);
		stca(10,k,0,0);   return(4);
	      
       //-------------- Общая концовка команд:
   kkk:
		
		if(k->predper==0)  k->pc=(k->pc+4);
		 else {k->pc=(k->pc-4);  }
		 		 //Установ pc в беспереходн.команде.
	 return(0);					       
    }//======================================================


   short stkom(UINT adr,ntip *k)
	//Считывет в  kodkom 
	//код команды по адресу adr из модели k->mp,
	// обеспечивая все по КЭШ-памяти, 
	// возвращает при удаче  0,
	// иначе - код неудачи 6  (ошибка шины с командой).
	// или 4 - (ошибка адресации).
	//или 3 - ошибка с КЕШ при чтении команды.
   {  UINT ad;  short kesk,ot;

		 k->sledk[0]=adr;
		if(k->jadro==1)   
		{if((020000000000&adr)!=0) 
		return(4); }                  //Пользов.в простр.ядра.

//----------------------------------------------------------------
		ad=PtoF(adr);
		if((0xa0000000<=adr)&&(0xbfffffff>=adr)) kesk=0; else kesk=1;
//-------------------------- ad - физич адрес, kesk - признак кешир.области.
              if(kesk==0)                                  
 {k->sledk[1]=2; k->sledk[3]=0; ot=stzukom(ad,k,1); 
		  if(ot==1) return(6); else return(0);
 }
 //--- Теперь осталось с кешированием. От изоляции не зависим.
       ot=stkeskom(ad,k); if(ot==0)
	   { k->sledk[1]=0; k->sledk[3]=0;
ot=stzukom(ad,k,0); if(ot==1) return(6);                                
	    return(0);
	}  //Все прочитано, сделано КЕШ-ом.
//Теперь: был промах, надо читать по ad ЗУ и перепис.в КЕШ ком.
	   k->sledk[1]=1; k->sledk[3]=2;
 ot=stzukom(ad,k,1); if(ot!=0) return(6);
                             //Ошибка шины, иначе все о-кей:
	           // --------- Делаем 4 переписи в КЕШ-ком:
       adkom=ad; adkom=(037777777760&adkom); 
	                        //Очистили 4 младших разр.адреса.
	   ot=stzukom(adkom,k,0); if(ot!=0)  
	   {   return(6); }
	   zapkeskom(adkom,kkom,k); adkom=(adkom+4);
	   ot=stzukom(adkom,k,0); if(ot!=0)
	   {   return(6); }		   
	   zapkeskom(adkom,kkom,k); adkom=(adkom+4);
	   ot=stzukom(adkom,k,0); if(ot!=0)
	   {   return(6); }
	   zapkeskom(adkom,kkom,k); adkom=(adkom+4);
	   ot=stzukom(adkom,k,0); if(ot!=0)
	   {   return(6); }		   
	   zapkeskom(adkom,kkom,k); //Переписали в КЕШ.
                                 return(0);

   }//=======================================================

    short stkeskom(UINT adr,ntip *k)
 //Читает команду из установленной КЕШ-команд. Если нет промаха и
 //v=1, все пишет в карту и др, возвращая 0 /тип памяти - 0 ! /. 
 //Если отказ, возвращает 1.  /Высшая функция обратится к ЗУ.
    { DWORDLONG *mk,*md; short rk,rd,r;
	    UINT ml,st,*u;
          // Установ размеров блоков КЕШ:
                       if((040000000&k->config)==0)
				{ rd=1; rk=4; }	    else { rd=2; rk=2; }
		  // Учет возможного переключения блоков КЕШ:
	    if((0400000&k->status)==0)  { mk=k->kespk; md=k->kespd;}
		   else { mk=k->kespd; md=k->kespk; r=rd; rd=rk; rk=r; }
//Теперь КЕШ-ком.это mk[], его размер в тыс.адресов=rk /1,2 или4/
		   ml=adr>>2; if(rk==1) { st=ml>>10; ml=(01777&ml); }
                 else if(rk==2) { st=ml>>11; ml=(03777&ml); }		   
                        else    { st=ml>>12; ml=(07777&ml); }
 //--- Получили: st прообраз тега, ml адрес обращения в КЕШ-ком.
           u=(UINT *)(&mk[ml]); //Вышли на нужную ячейку КЕШ.
	      if(((020000000000&u[1])==0)||((077777777&u[1])!=st)) 
        return(1); //Отказ по недействительности или не тот тег.
                     //---Теперь все о-кей, читаем и все пишем.
	    kodkom=u[0];
       k->sledk[4]=u[0];  return(0);
	} //========================================================
	 

	short stzukom(UINT adr,ntip *k,short p)
 //Читает из модели ЗУ k->mp  команду. При p=1 пишет в карту и др,
 //возвращая 0. Возвращает 1 при неудаче /ошибка шины/. Если p=0,
 //  то пишет только код команды в kkom, в карту не пишет!
	{	UINT *m,in,rab; QString s;
	       m=k->mp;   //Массив модели ЗУ.
		          for(in=1; ;in=(in+8))
			{ if((adr>=m[in])&&(adr<m[in+1])) 
				  {   //Нашли кусок памяти.
				  rab=adr-m[in]; rab=(rab>>2); 
				  rab=(rab+m[in+3]); 
				  rab=(m[rab]); //Взяли саму ячейку. 
				  if(p==0) { kkom=rab; return(0); }
                  //Теперь пишем в карту и др:
				   kodkom=rab;
				  k->sledk[2]=m[in+5]; k->sledk[4]=rab; 
			if((01&m[in+4])==0) return(1);//Чтение запрещено.
				     return(0);
				 }
               if(m[in+2]==1)
			   {  
			                               //ком. непрочитана.
			      kkom=037777777777; return(1);
			   }
			}

	}//=======================================================

     void zapkeskom(UINT adr,UINT kod,ntip *k)
 //Пишет в КЕШ-ком команду kod по адресу adr, вычленяя усеченный
 //адрес, формируя тег и делая v=1.
    { DWORDLONG *mk,*md; short rk,rd,r;
	    UINT ml,st,*u;
          // Установ размеров блоков КЕШ:
                       if((040000000&k->config)==0)
				{ rd=1; rk=4; }	    else { rd=2; rk=2; }
		  // Учет возможного переключения блоков КЕШ:
		 if((0400000&k->status)==0) { mk=k->kespk; md=k->kespd;}
         else { mk=k->kespd; md=k->kespk; r=rd; rd=rk; rk=r; }
 //Теперь КЕШ-ком.это mk[], его размер в тыс.адресов=rk /1,2 или4/
		   ml=adr>>2; if(rk==1) { st=ml>>10; ml=(01777&ml); }
                 else if(rk==2) { st=ml>>11; ml=(03777&ml); }		   
                        else    { st=ml>>12; ml=(07777&ml); }
 //--- Получили: st прообраз тега, ml адрес обращения в КЕШ-ком.
            u=(UINT *)(&mk[ml]); //Вышли на нужную ячейку КЕШ.
      u[0]=kod; u[1]=st; u[1]=(020000000000|u[1]); //Все вписали.
	 }//========================================================


   short stb(UINT adr,ntip *k) 
	//Читает байт по adr из модели k->mp,
	//пишет его в мл.байт очищенной sislo,   
	//время выборки в twibd.
	//Возвращает: 0 - при успехе,
	// 7 - при ошибке шины с данными /исключ. ситуация/,
	// 5 - при ошибке адресации
   {  UINT ad;  short kesk,ot;
        k->sleds[0]=1; k->sleds[5]=adr;
		 //Записали программный адрес
		if(k->jadro==1)   
	{if((020000000000&adr)!=0) return(4);}//Пользов.в простр.ядра.
//------------------------------------------------------------------
		ad=PtoF(adr);
		if((0xa0000000<=adr)&&(0xbfffffff>=adr)) kesk=0; else kesk=1;

                 sdw=03&ad; //расположение байта
        ad=037777777774&ad;
	 //---------------------- ad - физич адрес, кратный 4, sdw указывает мл.байт.
              k->sleds[6]=ad;
	 //  kesk - признак кешир.области /=1/.---------
       //Но может режим - изолированный ?  Отреагируем:
               if((0200000&k->status)!=0)     //Режим изоляции
		{  k->sleds[0]=011; ot=stkesdan(ad,k); 
		  if(ot==1) k->status=(02000000|k->status);   else
  k->status=(037775777777&k->status);// 19-й разряд status.
    if(sdw==1) sislo=(sislo>>8);
	else if(sdw==2) sislo=(sislo>>16);
	else if(sdw==3) sislo=(sislo>>24);
	else { }  return(0);  
		}//------- Далее  режим неизоляции --------
	    if(kesk==0)                                     
	{ k->sleds[1]=2; ot=stzudan(ad,k,1); if(ot==1) return(7);
		        if(sdw==1) sislo=(sislo>>8);
		  else if(sdw==2) sislo=(sislo>>16);
		  else if(sdw==3) sislo=(sislo>>24);
	else { }  k->sleds[4]=sislo;

	
	return(0);
	}// Теперь осталось с кешированием. От изол.не зависим.
		ot=stkesdan(ad,k); if(ot==0) 
		{ k->sleds[1]=0;     
		if(sdw==1) sislo=(sislo>>8);
		  else if(sdw==2) sislo=(sislo>>16);
		  else if(sdw==3) sislo=(sislo>>24);
		  else { }  k->sleds[4]=sislo;  
		     return(0);  
		} //Все прочитано.
 //Теперь: был промах, надо читать по ad ЗУ и перепис.в КЕШ дан.
       k->sleds[1]=1; ot=stzudan(ad,k,1); if(ot!=0) return(7); 
	                          //Ошибка шины, иначе все о-кей:
	   if(sdw==1) sislo=(sislo>>8);
	   else if(sdw==2) sislo=(sislo>>16);
     else if(sdw==3) sislo=(sislo>>24);
	 else { }   k->sleds[4]=sislo;
	           // --------- Делаем  переписи в КЕШ-данных:      
           if((04000000000&k->config)==0)
		   { adkom=ad; ot=stzudan(adkom,k,0); if(ot!=0) return(7); 
		                      //adkom использую и для чисел.
		ot=zapkesdan(adkom,kdan,k);  k->sleds[3]=1;

		
		return(0); } 
		                      //Конец при одной перезаписи.
               //Теперь переписываем 4 числа:
       adkom=ad; adkom=(037777777760&adkom);
	                       //Очистили 4 младших разр.адреса.
	   ot=stzudan(adkom,k,0); if(ot!=0) return(7); 
	                         //adkom использую и для чисел.
	   ot=zapkesdan(adkom,kdan,k); adkom=(adkom+4);   
	   ot=stzudan(adkom,k,0); if(ot!=0) return(7);
	   ot=zapkesdan(adkom,kdan,k); adkom=(adkom+4);
	   ot=stzudan(adkom,k,0); if(ot!=0) return(7);
	   ot=zapkesdan(adkom,kdan,k); adkom=(adkom+4);
	   ot=stzudan(adkom,k,0); if(ot!=0) return(7);
       ot=zapkesdan(adkom,kdan,k); //Переписали в КЕШ.
                      k->sleds[3]=2; 

					  return(0);

   }//=======================================================


   short stp(UINT adr,ntip *k) 
	//Читает полуслово по adr из модели k->mp,
	//пишет его в мл.половину очищенной sislo, записывает:  
	//время выборки в twibd.
	//Возвращает: 0 - при успехе,
	// 7 - при ошибке шины с данными /исключ. ситуация/,
	// 5 - при ошибке адресации
   {  UINT ad;  short kesk,ot;
         k->sleds[0]=1; k->sleds[5]=adr; 
		 //Записали программный адрес
        if((01&adr)!=0)   return(4);  //Адрес не кратен 2.
		if(k->jadro==1)   
		{ if((020000000000&adr)!=0) return(4); }
		                           //Пользов.в простр.ядра.
//--------------------------------------------------------
		ad=PtoF(adr);
		if((0xa0000000<=adr)&&(0xbfffffff>=adr)) kesk=0; else kesk=1;

	          if((02&ad)==0) 
		sdw=0; else sdw=2; 
		//sdw - расположение мл.байта в ячейке/0,1,2,3/.
			  ad=(037777777774&ad);
			   
	 //- ad - физич адрес, кратный 4, sdw указывает мл.байт.
               k->sleds[6]=ad;
	 //  kesk - признак кешир.области /=1/.---------
     //Но может режим - изолированный ?  Отреагируем:
           if((0200000&k->status)!=0)     //Режим изоляции
		   { k->sleds[0]=011; ot=stkesdan(ad,k); 
			if(ot==1) k->status=(02000000|k->status);   else
	 k->status=(037775777777&k->status);// 19-й разряд status.  
	 if(sdw==2) sislo=(sislo>>16); 
	   return(0);  
					}//------- Далее  режим неизоляции 
	    if(kesk==0)                                          
	{ k->sleds[1]=2; ot=stzudan(ad,k,1); if(ot==1) return(7); 
		  if(sdw==2) sislo=(sislo>>16);
	        k->sleds[4]=sislo;
			
			return(0);
 }// Теперь осталось с кешированием. От изоляции не зависим 
		ot=stkesdan(ad,k); if(ot==0) 
		{ k->sleds[1]=0;    
		  if(sdw==2) sislo=(sislo>>16);
		   k->sleds[4]=sislo; return(0);  
		} //Все прочитано.
//--Теперь: был промах, надо читать по ad ЗУ и перепис.в КЕШ дан.
       k->sleds[1]=1;  ot=stzudan(ad,k,1); if(ot!=0) return(7); 
	                         //Ошибка шины, иначе все о-кей:
   if(sdw==2) sislo=(sislo>>16); 
         k->sleds[4]=sislo;  
	   // --------- Делаем  переписи в КЕШ-данных:
           if((04000000000&k->config)==0)
	{ adkom=ad; ot=stzudan(adkom,k,0); if(ot!=0) return(7);
	 ot=zapkesdan(adkom,kdan,k); k->sleds[3]=1;

	 
	 return(0); } 
		                     //Конец при одной перезаписи.
               //Теперь переписываем 4 числа:
       adkom=ad; adkom=(037777777760&adkom);
	                         //Очистили 4 младших разр.адреса.
	   ot=stzudan(adkom,k,0); if(ot!=0) return(7);
	   ot=zapkesdan(adkom,kdan,k); adkom=(adkom+4);   
	   ot=stzudan(adkom,k,0); if(ot!=0) return(7);
	   ot=zapkesdan(adkom,kdan,k); adkom=(adkom+4);
	   ot=stzudan(adkom,k,0); if(ot!=0) return(7);
	   ot=zapkesdan(adkom,kdan,k); adkom=(adkom+4);
	   ot=stzudan(adkom,k,0); if(ot!=0) return(7);
       ot=zapkesdan(adkom,kdan,k); //Переписали в КЕШ.
               k->sleds[3]=2; 

			   
			   return(0);

   }//=======================================================


   short sts(UINT adr,ntip *k) 
	//Читает слово по adr из модели k->mp,
	//пишет его в  sislo, записывает:  
	//Возвращает: 0 - при успехе,
	// 7 - при ошибке шины с данными /исключ. ситуация/,
	// 5 - при ошибке адресации
   {  UINT ad;  short kesk,ot;
         k->sleds[0]=1;  k->sleds[5]=adr;
		   //Записали программный адрес
        if((03&adr)!=0)   return(4);  //Адрес не кратен 4.
		if(k->jadro==1)   
		{ if((020000000000&adr)!=0) {
						return(4);} }
		                           //Пользов.в простр.ядра.
//-----------------------------------------------------------
		ad=PtoF(adr);
		if((0xa0000000<=adr)&&(0xbfffffff>=adr)) kesk=0; else kesk=1;

	 //----------------- ad - физич адрес, kesk - признак кешир.области.
	            k->sleds[6]=ad;
          //Но может режим - изолированный ?  Отреагируем:
                if((0200000&k->status)!=0) //Режим изоляции
				{ k->sleds[0]=011; ot=stkesdan(ad,k); 
				  if(ot==1) k->status=(02000000|k->status); 
				  else k->status=(037775777777&k->status); 
				                      // 19-й разряд status. 
			 return(0);  
					}//------- Далее  режим неизоляции 
	    if(kesk==0)                                       
	{ k->sleds[1]=2; ot=stzudan(ad,k,1); k->sleds[4]=sislo;
		if(ot==1) return(7);

		
		return(0);                     
	}// Теперь осталось с кешированием. От изоляции не зависим.
		ot=stkesdan(ad,k); if(ot==0) 
		{ k->sleds[1]=0;   
		    k->sleds[4]=sislo;  return(0);  
		} //Все прочитано.
 //Теперь: был промах, надо читать по ad ЗУ и перепис.в КЕШ дан.
       k->sleds[1]=1; ot=stzudan(ad,k,1); k->sleds[4]=sislo; 
	   if(ot!=0) return(7); 
	                           //Ошибка шины, иначе все о-кей:
	           // --------- Делаем  переписи в КЕШ-данных:
           if((04000000000&k->config)==0)
	{ adkom=ad; ot=stzudan(adkom,k,0); if(ot!=0) return(7);
	 ot=zapkesdan(adkom,kdan,k); k->sleds[3]=1;

	 
	 return(0); } 
		                         //Конец при одной перезаписи.
               //Теперь переписываем 4 числа:
       adkom=ad; adkom=(037777777760&adkom);
	                        //Очистили 4 младших разр.адреса.
	   ot=stzudan(adkom,k,0); if(ot!=0) return(7);
	   ot=zapkesdan(adkom,kdan,k); adkom=(adkom+4);   
	   ot=stzudan(adkom,k,0); if(ot!=0) return(7);
	   ot=zapkesdan(adkom,kdan,k); adkom=(adkom+4);
	   ot=stzudan(adkom,k,0); if(ot!=0) return(7);
	   ot=zapkesdan(adkom,kdan,k); adkom=(adkom+4);
	   ot=stzudan(adkom,k,0); if(ot!=0) return(7);
       ot=zapkesdan(adkom,kdan,k); //Переписали в КЕШ.
          k->sleds[3]=2;     

		  
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
		         if((k->mdp[rab]!=0)&&(p==1))
				 {k->sleds[2]=m[in+5];					 
				 wnu(adr,'s',sislo,&sislo,k); return(0); }
				  rab=(m[rab]); //Взяли ячейку. 
				  if(p==0) { kdan=rab; return(0); }
                  //Теперь пишем в карту и др:
		  sislo=rab; 
		if((01&m[in+4])==0) return(1);   //Чтение запрещено !  
	    k->sleds[2]=m[in+5]; //Запись в СЛЕД цикла чтения.
				   return(0);
				 }
               if(m[in+2]==1)
			   {     kdan=037777777777; return(1);
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


     short zapkesdanosob(UINT adr,UINT kod,ntip *k)                              
 //Пишет в КЕШ-дан число kod по адресу adr, вычленяя усеченный
 //адрес, формируя тег и делая v=0. //Для изолир.режима.
 //Возвр.1 при промахе, иначе - 0. Это нужно для изолир.режима.
    { DWORDLONG *mk,*md; short rk,rd,r,pr;
	    UINT ml,st,*u;
          // Установ размеров блоков КЕШ:
                       if((040000000&k->config)==0)
				{ rd=1; rk=4; }	    else { rd=2; rk=2; }
		  // Учет возможного переключения блоков КЕШ:
		if((0400000&k->status)==0)  { mk=k->kespk; md=k->kespd;}
		   else { mk=k->kespd; md=k->kespk; r=rd; rd=rk; rk=r; }
 //Теперь КЕШ-дан.это md[], его размер в тыс.адресов=rd /1,2 или4/.
		   ml=adr>>2; if(rd==1) { st=ml>>10; ml=(01777&ml); }
                 else if(rd==2) { st=ml>>11; ml=(03777&ml); }		   
                        else    { st=ml>>12; ml=(07777&ml); }
  //--- Получили: st прообраз тега, ml адрес обращения в КЕШ-дан.
         u=(UINT *)(&md[ml]); //Вышли на нужную ячейку КЕШ.
           if((077777777&u[1])!=st) pr=1; else pr=0;
             u[0]=kod; u[1]=st;  //Все вписали.
			             return(pr);
	 }//===================================================



   short zab(UINT adr,ntip *k)                                                     
	//Пишет байт по adr модели k->mp,
	//берет его из мл.байта sislo. Записывает:  
	//время записи в twibs.
	//Возвращает: 0 - при успехе,
	// 7 - при ошибке шины с данными /исключ. ситуация/,
	// 5 - при ошибке адресации
	// 20 - при записи в несуществующую ячейку.
   {  UINT rab,rab1,ad;  short kesk,ot,ot1;
      k->sleds[0]=2; k->sleds[5]=adr; 
	   //Записали программный адрес

		if(k->jadro==1)   
		{ if((020000000000&adr)!=0) return(5); }
		                             //Пользов.в простр.ядра.
//----------------------------------------------------------
		ad=PtoF(adr);
		if((0xa0000000<=adr)&&(0xbfffffff>=adr)) kesk=0; else kesk=1;

         sdw=03&ad; ad=(037777777774&ad);
		  
 //------------------ ad - физич адрес,кратный 4.kesk - признак кешир.области.
		        k->sleds[6]=ad;
		 //  sdw определяет байт записи.
               rab=0377&sislo;
          //Но может режим - изолированный ?  Отреагируем:
        if((0200000&k->status)!=0)     //Режим изоляции
	{k->sleds[0]=012; ot=stkesdan(ad,k); kdan=sislo;
       if(sdw==0)                                                
	   {kdan=(037777777400&kdan); kdan=(kdan|rab); }
	    else if(sdw==1)
	{kdan=(037777600377&kdan); rab1=(rab<<8); kdan=(kdan|rab1);}
	    else if(sdw==2)
	{kdan=(037700177777&kdan); rab1=(rab<<16); kdan=(kdan|rab1);}
	    else 
	{kdan=(077777777&kdan);    rab1=(rab<<24); kdan=(kdan|rab1);}
    //В kdan - то, что надо записать в КЕШ по адресу ad
	k->sleds[4]=kdan; ot=zapkesdanosob(ad,kdan,k);
	                            //Со стиранием v
		  if(ot==1) k->status=(02000000|k->status);   else
		 k->status=(037775777777&k->status);// 19-й разряд status. 
                             return(0);
	}//-----------Далее - режим неизоляции ---------------
         if(kesk==0)     //Не КЕШ                          
	{k->sleds[1]=2; ot=stzudan(ad,k,0);
//	 if(ot!=0) return(7); //Число прочли в kdan из ОЗУ 		  
       if(sdw==0)                                                
	   {kdan=(037777777400&kdan); kdan=(kdan|rab); }
	    else if(sdw==1)
	{kdan=(037777600377&kdan); rab1=(rab<<8); kdan=(kdan|rab1);}
	    else if(sdw==2)
	{kdan=(037700177777&kdan); rab1=(rab<<16); kdan=(kdan|rab1);}
	    else 
	{kdan=(077777777&kdan);    rab1=(rab<<24); kdan=(kdan|rab1);}
    //В kdan - то, что надо записать в ОЗУ по адресу ad
    k->sleds[4]=kdan; k->sleds[1]=2; sislo=kdan;
	 ot=zapzudan(ad,k);  //Записываем в ОЗУ из  sislo
	if(ot==1) return(7); else if(ot==2) return(20); else  return(0);
	}//---- Разделались с НЕ - КЕШ
		   //Теперь - КЕШ :
      ot=stkesdan(ad,k); if(ot==0) //Слово есть в КЕШ
	{kdan=sislo; k->sleds[1]=0;
       if(sdw==0)                                                
	   {kdan=(037777777400&kdan); kdan=(kdan|rab); }
	    else if(sdw==1)
	{kdan=(037777600377&kdan); rab1=(rab<<8); kdan=(kdan|rab1);}
	    else if(sdw==2)
	{kdan=(037700177777&kdan); rab1=(rab<<16); kdan=(kdan|rab1);}
	    else 
	{kdan=(077777777&kdan);    rab1=(rab<<24); kdan=(kdan|rab1);}
    //В kdan - то, что надо записать в КЕШ и ОЗУ по адресу ad
      k->sleds[4]=kdan; 
     ot=zapkesdan(ad,kdan,k); //Записали в КЕШ
	           sislo=kdan;
      ot1=zapzudan(ad,k);      //Записали то же в ОЗУ
     if(ot1==1) return(7); else if(ot1==2) return(20); else  return(0);  
	 }//----Конец при действительности ячейки КЕШ
   //Теперь - был промах при чтении из КЕШ, писать только в ОЗУ
    k->sleds[1]=1; //был промах при чтении
	ot=stzudan(ad,k,0); //число прочли в kdan из ОЗУ
//	if(ot!=0) return(7);
       if(sdw==0)                                                
	   {kdan=(037777777400&kdan); kdan=(kdan|rab); }
	    else if(sdw==1)
	{kdan=(037777600377&kdan); rab1=(rab<<8); kdan=(kdan|rab1);}
	    else if(sdw==2)
	{kdan=(037700177777&kdan); rab1=(rab<<16); kdan=(kdan|rab1);}
	    else 
	{kdan=(077777777&kdan);    rab1=(rab<<24); kdan=(kdan|rab1);}
    //В kdan - то, что надо записать в ОЗУ по адресу ad
     sislo=kdan; k->sleds[4]=kdan; 
	 ot=zapzudan(ad,k);  //Записали
     if(ot==1) return(7); else if(ot==2) return(20); else  return(0);

   }//========== Конец zab( ) ==============================


   short zap(UINT adr,ntip *k) 
	//Пишет полуслово по adr  модели k->mp,
	//берет его из мл.половины sislo. Записывает:  
	//время выборки в twibd.
	//Возвращает: 0 - при успехе,
	// 7 - при ошибке шины с данными /исключ. ситуация/,
	// 5 - при ошибке адресации
	//20 - при записи в несуществующую ячейку.
   {  UINT rab,rab1,ad;  short kesk,ot,ot1;
        k->sleds[0]=2; k->sleds[5]=adr;
		 //Записали программный адрес


        if((01&adr)!=0)   return(5);    //Адрес не кратен 2.
		if(k->jadro==1)   
		{ if((020000000000&adr)!=0) return(5); }
		                                //Пользов.в простр.ядра.
//--------------------------------------------------------
		ad=PtoF(adr);
		if((0xa0000000<=adr)&&(0xbfffffff>=adr)) kesk=0; else kesk=1;

         sdw=03&ad; ad=(037777777774&ad);
		  
 //----------- ad - физич адрес,кратный 4.kesk - признак кешир.области.
		           k->sleds[6]=ad;
                   rab=0177777&sislo; //Полуслово
       //Но может режим - изолированный ?  Отреагируем:
              if((0200000&k->status)!=0)   //Режим изоляции
	{k->sleds[0]=012; ot=stkesdan(ad,k); kdan=sislo; 
            if(sdw==0)
		{kdan=(037777600000&kdan); kdan=(kdan|rab);}
			else
		{kdan=(0177777&kdan); rab1=rab<<16; kdan=(kdan|rab1);}
        ot=zapkesdanosob(ad,kdan,k); //Со стиранием v
		k->sleds[4]=kdan;      
	if(ot==1) k->status=(02000000|k->status);   else
	k->status=(037775777777&k->status);// 19-й разряд status. 
                             return(0);
	}//------- Далее  режим неизоляции --------                 
             if(kesk==0)    //Не в КЕШ                           
	{k->sleds[1]=2; ot=stzudan(ad,k,0);
//     if(ot!=0) return(7); // Число прочли в kdan из ОЗУ
            if(sdw==0)
		{kdan=(037777600000&kdan); kdan=(kdan|rab);}
			else
		{kdan=(0177777&kdan); rab1=rab<<16; kdan=(kdan|rab1);}
         //В kdan - то, что надо записать в  ОЗУ по адресу ad
         k->sleds[4]=kdan; k->sleds[1]=2; sislo=kdan;
		 ot=zapzudan(ad,k);   //Записываем в ОЗУ из sislo
		 if(ot==1) return(7); else if(ot==2) return(20); else  return(0);
	}//--- Разделались с НЕ - КЕШ.
			//--Теперь   КЕШ:
    ot=stkesdan(ad,k);
	if(ot==0)     //Слово есть в КЕШ
    {kdan=sislo; k->sleds[1]=0;
            if(sdw==0)
		{kdan=(037777600000&kdan); kdan=(kdan|rab);}
			else
		{kdan=(0177777&kdan); rab1=rab<<16; kdan=(kdan|rab1);}
    //В kdan - то, что надо записать в КЕШ и ОЗУ по адресу ad
              k->sleds[4]=kdan;
     ot=zapkesdan(ad,kdan,k); //Записали в КЕШ
	         sislo=kdan;
      ot1=zapzudan(ad,k);  //Записали то же в ОЗУ
      if(ot1==1) return(7); else if(ot1==2) return(20); else return(0);
	 }//----Конец при действительности ячейки КЕШ
   //Теперь - был промах при чтении из КЕШ, писать только в ОЗУ
    k->sleds[1]=1; //был промах при чтении
	ot=stzudan(ad,k,0); //число прочли в kdan из ОЗУ
//	if(ot!=0) return(7);
            if(sdw==0)
		{kdan=(037777600000&kdan); kdan=(kdan|rab);}
			else
		{kdan=(0177777&kdan); rab1=rab<<16; kdan=(kdan|rab1);}
    //В kdan - то, что надо записать в ОЗУ по адресу ad
        sislo=kdan; k->sleds[4]=kdan;
      ot=zapzudan(ad,k);    //Записали
	if(ot==1) return(7); else if(ot==2) return(20); else return(0);

   }//================ Конец zap( )=================================


   short zas(UINT adr,ntip *k)                                          
	//Пишет слово по adr модели k->mp,
	//беря его из  sislo. Записывает:  
	//Возвращает: 0 - при успехе,
	// 7 - при ошибке шины с данными /исключ. ситуация/,
	// 5 - при ошибке адресации
   {  UINT ad;  short kesk,ot;  
        k->sleds[0]=2; k->sleds[5]=adr;
		 //Записали программный адрес
                
        if((03&adr)!=0)   return(5);  //Адрес не кратен 4.
		if(k->jadro==1)   
		{ if((020000000000&adr)!=0) 
			{
			 return(5);
			}
		}
		                           //Пользов.в простр.ядра.
//-------------------------------------------------------
		ad=PtoF(adr);
		if((0xa0000000<=adr)&&(0xbfffffff>=adr)) kesk=0; else kesk=1;

	   
 //----------- ad - физич адрес, kesk - признак кешир.области.
                   k->sleds[6]=ad;
         //Но может режим - изолированный ?  Отреагируем:
         if((0200000&k->status)!=0)     //Режим изоляции
		 { k->sleds[0]=012; ot=zapkesdan(ad,sislo,k);
		  if(ot==1) k->status=(02000000|k->status);   else
    k->status=(037775777777&k->status);// 19-й разряд status. 
                             return(0);
			}//------- Далее  режим неизоляции --------                 
     
	    if(kesk==0)                                     
	{ k->sleds[1]=2;   ot=zapzudan(ad,k);   
	 k->sleds[4]=sislo; if(ot==1) return(7); 
	   else if(ot==2) return(20); else return(0);
	} 
  //---- Теперь осталось с кешированием. От изоляции не зависим.   
       ot=zapzudan(ad,k); k->sleds[4]=sislo;  
	 if(ot==1) return(7); if(ot==2) return(20);                                              
  //---Теперь надо записать sislo в КЕШ-данных /адрес ad:
         k->sleds[1]=0;   ot=zapkesdan(ad,sislo,k); return(0);

   }//=======================================================


	short zapzudan(UINT adr,ntip *k)
		//Записывает в модель ЗУ k->mp  sislo,
		//возвращая 0. Возвращает 1 при неудаче /ошибка шины/.
		// или 2 - при записи в несуществующую ячейку. 
	{	UINT *m,in,rab;  int p;
	                      p=0;
	       m=k->mp;   //Массив модели ЗУ.
		          for(in=1; ;in=(in+8))
		{ if((adr>=m[in])&&(adr<m[in+1])) //Нашли кусок памяти.
				{ rab=adr-m[in]; rab=(rab>>2); 
				  rab=(rab+m[in+3]); //Это индекс в модели.           
                  //Теперь пишем в карту и др:
	  if((02&m[in+4])==0) 
	 {   return(0);} //Запись в ПЗУ запрещена 
   if(k->mdp[rab]!=0) { k->sleds[2]=m[in+6];
	   wnu(adr,'z',sislo,&sislo,k); return(0);}
     m[rab]=sislo;
	
	 k->sleds[2]=m[in+6];   //Запись в СЛЕД цикла записи.
				    return(0);
				 }
               if(m[in+2]==1)
			   {  wpis(50,0,0,0,11,2,adr,k);  //Запись адреса записи в RBEr
				   //данные непрочитаны.
			      kdan=037777777777; return(2);
			   }
			}

	}//==========================================================


   UINT * modzu(UINT * zag)
   //Строит модель ЗУ по ее заготовке в массиве zag.
   //Возвращает указатель на готовую модель или 0 при неудаче.
   // Заготовка модели (UINT) состоит из шестерок (по числу кусков):
   //нач.адрес куска,конечн.адрес куска,призн.конца/==1/,
   //тип памяти,первый параметр,второй параметр.
   //Модель (UINT) имеет в начале ее длину в байтах,затем идут
   //восьмерки (по числу кусков) и после них - зона модели ЗУ.
				    //Восьмерка в модели содержит:
         //нач.адрес куска,конечн,адрес куска (адрес ст.байта),
		 //тот же признак конца,индекс начала этого куска в зоне
		 //модели ЗУ,тот же тип памяти,первый и второй параметры
		 //(те же),обнуленную резервную ячейку.
		           //Ячейка типа памяти:
         // 0 разр.- разрешение чтения, 1 разр. - разрешение записи,
		 // 3 разр. - инициация ВУ при чтении, 4 разр. - инициация
		 // ВУ при записи. Разрешение - 1, запрет - 0. 
   { UINT *z,*m;     int i,r,w,j;  QString s;
       z=zag; w=1; r=4; //w - для счета ячеек, r - для счета байтов.
                    for(i=0; ;i=(i+6))
		{ if(i>600) return(0);      //Очень большая заготовка.
					if((z[i+2]!=0)&&(z[i+2]!=1)) 
					{if(prz) printkl("1");return(0);} //? в признаке конца.
		  if((03&z[i])!=0)            
		  { if(prz) printkl("2"); return(0); }
          if((03&z[i+1])!=0)          
		  {if(prz) printkl("3"); return(0);} //Адреса не кратны 4.
		  if(z[i+1]<z[i])           
		  {if(prz) printkl("4"); return(0);} //Неупорядочены.
		  if((037777777744&z[i+3])!=0) 
		  {if(prz) printkl("5"); return(0);} //Бяки в типе памяти.
          if(i>0) { if(z[i]<=z[i-6]) { if(prz) printkl("6"); return(0);}}
		                                         //Неупорядочено.
		  //----------------------------------------------
		  r=(r+(z[i+1]-z[i])+36);  //Добавка от данной восьмерки.
		  w=(w+8);  //Будет индексом первой ячейки зоны ЗУ.
		          if(z[i+2]==1) break;
		}//-- Параметры длин зон и всех восьмерок - знаем. -----
					if((r<=0)||(r>077777777)) 
					{if(prz) printkl("8"); if(prz) printkl(s); return(0);}
			m=(UINT *)malloc(r);
			if(m==NULL) {if(prz) printkl("7");if(prz) printkl(s); return(0);}
     //---------------------------------------------------------
                    //Начинаем формировать модель:
			  m[0]=r;      //Записали длину модели в первую яч.
                 j=1;      //Начало восьмерок в модели. 
                        for(i=0; ;i=(i+6))
		{ m[j]=z[i]; m[j+1]=z[i+1]; m[j+1]=(03|m[j+1]);
						                     //Адрес ст.байта.
		  m[j+2]=z[i+2];   //Перенос признака конца.
		  m[j+4]=z[i+3];   //Перенос типа памяти.
		  m[j+5]=z[i+4]; m[j+6]=z[i+5]; //Перенос параметров.
		  m[j+7]=0;
		         //Чистка резерва. Теперь - запись индекса куска:
		  m[j+3]=w;
		         //Сделали, но надо подготовить w для след.куска.
		r=z[i+1]-z[i]; r=(r>>2); r=(r+1); w=(w+r); //Подготовили.
		          j=(j+8);   if(z[i+2]==1)  break;
		}
                           return(m);

   }//========================================================


     void invold(UINT adr,ntip *k)  
		 //Инволюдизирует КЕШ данн.по адресу adr.

    { DWORDLONG *mk,*md; short rk,rd,r;
	    UINT ml,*u;
          // Установ размеров блоков КЕШ:
                       if((040000000&k->config)==0)
				{ rd=1; rk=4; }	    else { rd=2; rk=2; }
		  // Учет возможного переключения блоков КЕШ:
		if((0400000&k->status)==0)  { mk=k->kespk; md=k->kespd; }
		else { mk=k->kespd; md=k->kespk; r=rd; rd=rk; rk=r; }
 //Теперь КЕШ-дан.это md[], его размер в тыс.адресов=rd /1,2 или4/.
 //Теперь КЕШ-ком.это mk[], его размер в тыс.адресов=rk /1,2 или4/.
		   // ---Работаем только с данными:
		   ml=adr>>2; if(rd==1) {  ml=(01777&ml); }
                 else if(rd==2) {  ml=(03777&ml); }		   
                        else    {  ml=(07777&ml); }
           //--- Получили:  ml адрес обращения в КЕШ-дан.
            u=(UINT *)(&md[ml]); //Вышли на нужную ячейку КЕШ.
   u[1]=0; //Все вписали. 0 тега и 0 действительности для КЕШ-дан.
			  
			             return;
	 }//==========================================================


    void zawis(ntip *k)    //Устанавливает в CAUSE int4.
	{UINT i;
      i=k->cause; i=(040000|i);  k->cause=i;

    }//===================================================


     void serega(ntip *k)
//Готовит данные по времени запрета прерывания
{UINT rom1,rom3; DWORDLONG rrab; QString s;


      rom1=1&(k->status);  rom3=0177400&(k->status);
	       //---------------------------

  if((rom1!=0)&&((0400&rom3)!=0))  //Есть разрешение прерывания по данному запросу

  { if(k->sernas[0]==0)       //не было раньше запрета  
	{ }  
    else                      //был, но теперь разрешен
    {rrab=(4000*ttek4)-(k->sernas[0]);
	    k->sernas[0]=0; //снят запрет
      if(rrab>(k->sermax[0])) 
	  {  k->sermax[0]=rrab; //большее значение
         // s.sprintf("# status-8 разряд   %lu мкс",(rrab/1000)); printkl(s);
	  }
	}
  }//------------------ отработали при разрешении

  else                              //Есть запрет прерывания по данному запросу

  { if((k->sernas[0])==0)      //не было раньше запрета
     k->sernas[0]=(4000*ttek4);      //время начала запрета
    else                 //уже был запрет   
	{ }     //пустое подтверждение запрета 
  }//------------------ отработали при запрете 
  //-************************************************************ конец с данным запросом. 
  if((rom1!=0)&&((01000&rom3)!=0))  //Есть разрешение прерывания по данному запросу

  { if(k->sernas[1]==0)       //не было раньше запрета  
	{ }  
    else                      //был, но теперь разрешен
    {rrab=(4000*ttek4)-(k->sernas[1]);
	    k->sernas[1]=0; //снят запрет
      if(rrab>(k->sermax[1])) 
	  {k->sermax[1]=rrab; //большее значение
        //s.sprintf("# status-9 разряд   %lu мкс",(rrab/1000)); printkl(s);
	  }
	}
  }//------------------ отработали при разрешении

  else                              //Есть запрет прерывания по данному запросу

  { if((k->sernas[1])==0)      //не было раньше запрета
     k->sernas[1]=(4000*ttek4);      //время начала запрета
    else                 //уже был запрет   
	{ }     //пустое подтверждение запрета 
  }//------------------ отработали при запрете 
  //-************************************************************ конец с данным запросом. 
  if((rom1!=0)&&((02000&rom3)!=0))  //Есть разрешение прерывания по данному запросу

  { if(k->sernas[2]==0)       //не было раньше запрета  
	{ }  
    else                      //был, но теперь разрешен
    {rrab=(4000*ttek4)-(k->sernas[2]);
	    k->sernas[2]=0; //снят запрет
      if(rrab>(k->sermax[2]))
	  {k->sermax[2]=rrab; //большее значение
	    //s.sprintf("# status-10 разряд   %lu мкс",(rrab/1000)); printkl(s);
	  }
	}
  }//------------------ отработали при разрешении

  else                              //Есть запрет прерывания по данному запросу

  { if((k->sernas[2])==0)      //не было раньше запрета
     k->sernas[2]=(4000*ttek4);      //время начала запрета
    else                 //уже был запрет   
	{ }     //пустое подтверждение запрета 
  }//------------------ отработали при запрете 
  //-************************************************************ конец с данным запросом. 
  if((rom1!=0)&&((04000&rom3)!=0))  //Есть разрешение прерывания по данному запросу

  { if(k->sernas[3]==0)       //не было раньше запрета  
	{ }  
    else                      //был, но теперь разрешен
    {rrab=(4000*ttek4)-(k->sernas[3]);
	    k->sernas[3]=0; //снят запрет
      if(rrab>(k->sermax[3]))
	  {k->sermax[3]=rrab; //большее значение
	    //s.sprintf("# status-11 разряд   %lu мкс",(rrab/1000)); printkl(s);
	  }
	}
  }//------------------ отработали при разрешении

  else                              //Есть запрет прерывания по данному запросу

  { if((k->sernas[3])==0)      //не было раньше запрета
     k->sernas[3]=(4000*ttek4);      //время начала запрета
    else                 //уже был запрет   
	{ }     //пустое подтверждение запрета 
  }//------------------ отработали при запрете 
  //-************************************************************ конец с данным запросом. 
  if((rom1!=0)&&((010000&rom3)!=0))  //Есть разрешение прерывания по данному запросу

  { if(k->sernas[4]==0)       //не было раньше запрета  
	{ }  
    else                      //был, но теперь разрешен
    {rrab=(4000*ttek4)-(k->sernas[4]);
	    k->sernas[4]=0; //снят запрет
		if(rrab>(k->sermax[4])) 
		{k->sermax[4]=rrab; //большее значение 
          //s.sprintf("# status-12 разряд   %lu мкс",(rrab/1000)); printkl(s);
        }
	}
  }//------------------ отработали при разрешении

  else                              //Есть запрет прерывания по данному запросу

  { if((k->sernas[4])==0)      //не было раньше запрета
     k->sernas[4]=(4000*ttek4);      //время начала запрета
    else                 //уже был запрет   
	{ }     //пустое подтверждение запрета 
  }//------------------ отработали при запрете 
  //-************************************************************ конец с данным запросом. 
  if((rom1!=0)&&((020000&rom3)!=0))  //Есть разрешение прерывания по данному запросу

  { if(k->sernas[5]==0)       //не было раньше запрета  
	{ }  
    else                      //был, но теперь разрешен
    {rrab=(4000*ttek4)-(k->sernas[5]);
	    k->sernas[5]=0; //снят запрет
      if(rrab>(k->sermax[5]))
	  { k->sermax[5]=rrab; //большее значение
	    //s.sprintf("# status-13 разряд   %lu мкс",(rrab/1000)); printkl(s);
	  }
	}
  }//------------------ отработали при разрешении

  else                              //Есть запрет прерывания по данному запросу

  { if((k->sernas[5])==0)      //не было раньше запрета
     k->sernas[5]=(4000*ttek4);      //время начала запрета
    else                 //уже был запрет   
	{ }     //пустое подтверждение запрета 
  }//------------------ отработали при запрете 
  //-************************************************************ конец с данным запросом. 
  if((rom1!=0)&&((040000&rom3)!=0))  //Есть разрешение прерывания по данному запросу

  { if(k->sernas[6]==0)       //не было раньше запрета  
	{ }  
    else                      //был, но теперь разрешен
    {rrab=(4000*ttek4)-(k->sernas[6]);
	    k->sernas[6]=0; //снят запрет
      if(rrab>(k->sermax[6]))
	  {k->sermax[6]=rrab; //большее значение
	    //s.sprintf("# status-14 разряд   %lu мкс",(rrab/1000)); printkl(s);
	  }
	}
  }//------------------ отработали при разрешении

  else                              //Есть запрет прерывания по данному запросу

  { if((k->sernas[6])==0)      //не было раньше запрета
     k->sernas[6]=(4000*ttek4);      //время начала запрета
    else                 //уже был запрет   
	{ }     //пустое подтверждение запрета 
  }//------------------ отработали при запрете 
  //-************************************************************ конец с данным запросом. 
  if((rom1!=0)&&((0100000&rom3)!=0))  //Есть разрешение прерывания по данному запросу

  { if(k->sernas[7]==0)       //не было раньше запрета  
	{ }  
    else                      //был, но теперь разрешен
    {rrab=(4000*ttek4)-(k->sernas[7]);
	    k->sernas[7]=0; //снят запрет
      if(rrab>(k->sermax[7])) 
	  {k->sermax[7]=rrab; //большее значение
	   //s.sprintf("# status-15 разряд   %lu мкс",(rrab/1000)); printkl(s);
	  }
	}
  }//------------------ отработали при разрешении

  else                              //Есть запрет прерывания по данному запросу

  { if((k->sernas[7])==0)      //не было раньше запрета
     k->sernas[7]=(4000*ttek4);      //время начала запрета
    else                 //уже был запрет   
	{ }     //пустое подтверждение запрета 
  }//------------------ отработали при запрете 
  //-************************************************************ конец с данным запросом. 

}//================ конец serega =================== 


     void serwkl(int n)
     //устанавливает включение-выключение подготовки данных по временам запретов прерыв.
	 {serwklus=n;}
//============================= конец serwkl ===========



 //=================== ИДЕОЛОГИЯ =============================    
 // 1.  Очередная команда всегда берется по pc. 
 //           При выполнении беспереходной команды в ее конце:
 //          а/  при predper=0 на pc заносится адрес следующей
 //  по порядку команды/+4/.
 //          б/  при predper=1 на pc заносится адрес предшеств.
 //  команды/-4/.
 //                 ---------------------------
 //              При условном переходе - два вида его выполнения:
 //   Если predper=0, то на pc устанавливается адрес последующей
 //   команды/+4/, predper делается = 1, условие исследуется
 //   и исполнит.адрес запоминается в adrper. 
 //       Если predper=1, то последующая команда уже не выполняется,
 //   predpr обнуляется, исполнит.адрес adrper записывается на pc.
 //                  --------------------------
 //      Но при прерыв.по исключ.ситуации CP0 /команда фактически
 //   не завершается/ - надо в stca( ) по признаку predper /если
 //   он=1/ выполнять epc=pc-4, чтобы вернуться на ком.перехода.
 //   При прерыв.по исключ.сист.CP1 pc устанавливается в концовочных,
 //   завершающих частях команд аналогично/см. метки kkk в функциях/.
	      //******************************
 // 2.   Запросы на прерывание от настоящих, настойчивых  внешних
 //   устройств должны быть сброшены в ip10-ip15 их же моделями,
 //   когда они будут системой удовлетворены.
	      //******************************
 // 3. Если прерывание по исключ.ситуации в ЦП 
 //   обеспечивает stca( ), делая все как при немаскирумом прерывании, 
 //   то в прерывании от CP1 работает индивидуальная схема реакции, 
 //   которая может оказаться замаскированной. 
 //   Никому не передается тип исключ.ситуации в CP1, как в stca( ),
 //   тип определяет регистр contlstatus CP1. Читай его сам.
 //     Но я пишу его тип для себя в  codisklcp1.
	      //******************************
 // 4.   Исключительные ситуации в CP1 в нем же и маскируются, кроме
 //  "нереализованной операции", которая сразу делает запрос на преры-
 //  вание. Все запросы от CP1 "фильтруются" общим разрешением в ЦП,
 // здесь явное отличие от запросов по исключ.ситуациям в ЦП, которые
 //  прут беспрепятственно и прерывают даже программу ядра при общем
 //  запрете прерывания! Еще: Исключ.сит. 'i' есть следствие и 'o' .
	      //*******************************
 // 5.   Биты Cause  регистра contrlstatus отражают результат
 //   только последней операции CP1 /кроме загрузок, пересылок/.Биты
 //   Flag того же регистра устанавливаются всегда, не сбрасываются
 //   аппаратно, являются накопительной информацией.
	      //********************************
 // 6.   В CP1 исключительная ситуация! Варианты действий:
 //      a/ Прерывание разрешено.
 //        Исходные операнды и приемник результата не меняются.
 //      б/ Прерывание заблокировано.
 //        В результат записывается "подразумеваемое" значение /NAN/, 
 //        если операция  некорректная . При некорректной операции
 //        в результат пишется  нечисло.
	   //************************************
	      //ОСОБЕННОСТИ  С  ОДИНАРНЫМИ  ЧИСЛАМИ:
 // 7.   В операциях с плавающей запятой нештатно обрабатываются случаи
 //   с нулевыми и бесконечными операндами.  Если этого нет, вы- 
 //   полняется операция с обычными ненулевыми числами. Только после 
 //   операции используется округление okrugo( ). Оно не только округ-
 //   ляет, но и вписывает компоненты результата в тару мантиссы m3, 
 //   в порядок p1 (знак результата в so1).  Перед сложением, умножением 
 //   и делением операнды нормализованы(они ненулевые). Перед округлением
 //   ненулевой результат также нормализуется. Здесь используется общий
 //   хвост программы - с меткой oformlen  в блоке сложения одинарных
 //   чисел. 
 //       ОССОБЕННОСТИ С ДВОЙНЫМИ ЧИСЛАМИ - АНАЛОГИЧНЫ.
//  **********************************************************
//  8.       О ПРЕРЫВАНИИ  ОТ  CP1. 
//              /сделано так:   /       
//   Функция rpcp1 выясняет - может ли быть реализовано прерывание от CP1.
//  Если - да, то она вписывает в CAUSE в соответствующий (по config)
//  разряд 1. Это вписывание - только в случае реализации прерывания!
//	Данный разряд в CAUSE сбрасывается ТОЛЬКО при программной записи в
//  controlstatus, не дающей прерывания. Поэтому в обработке прерывания
//  можно по CAUSE определить заказчика прерывания, а затем записью в
//  controlstatus косвенно сбросить данный разряд - указатель на CP1.
//  ***********************************************************


