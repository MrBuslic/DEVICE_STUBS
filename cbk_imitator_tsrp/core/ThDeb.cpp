
#include "PKM.h" 


#define WM_MDI2                      WM_APP+2
#define WM_MDI3                      WM_APP+3
#define WM_MDI8                      WM_APP+8
#define WM_MDI10                      WM_APP+10
#define WM_MDI18                      WM_APP+18
#define WM_MDI191                      WM_APP+191
#define WM_MDI188                      WM_APP+188
#define WM_MDI38                      WM_APP+38
#define WM_MDI101                      WM_APP+101

extern int CSum_OPO(UINT adrs, UINT adre, UINT* sum, UINT start_sum, UINT crc32,UINT);
extern      UINT      gruppa[];   //Для чтения группы романовских чисел.
extern    short writeb6(UINT adr,short nom,UCHAR b); 
extern   short readb6(UINT adr,short nom,UCHAR &b);
extern   UINT      gruppa1[];   //Для чтения группы  чисел из ВМ1.
extern 	 UINT      gruppa2[];   //Для чтения группы  чисел из ВМ2.
extern 	 UINT      gruppa3[];   //Для чтения группы  чисел из ВМ3.
extern 	 UINT      gruppa4[];   //Для чтения группы  чисел из ВМ4.
extern 	 UINT      gruppa5[];   //Для чтения группы  чисел из ВМ5.
extern 	 UINT      gruppa6[];   //Для чтения группы  чисел из ВМ6.
extern int Stopli;
extern void serwkl(int n);
extern void widser(void);
extern void sernull(void);
extern 	void jmpkl(short nom);
extern HANDLE eProz ;
extern    UINT zakrom[];    //Заказ
extern    UINT otwrom[];   //Ответ  / шестерки.
extern    short potkl;       //Признак,  сплю(0)-работаю(1).
extern      UINT      adresost[];       //Адрес останова /есть всегда.
extern   UINT volatile widasa;
extern   int old_widasa;
extern void	OnTypeForGetadrT(UINT adr,Srom* p1,char* p2);
extern    UINT JUMP;
extern    UINT PJUMP;
extern void	OnTypeForGetadr(UINT adr,Srom* p1,char* p2);
extern      QString   mestost[];        //Текст, описывающий место останова.
extern void TrimBreak(UINT j);
extern int MetFromAdr(UINT epc,QString& sm);
extern QMutex fbreak;
//extern HWND hDmem;
//extern CNewmem* pMem;
//extern HWND hDg;
//extern int pDglob;
HANDLE hTrModel,hTrModel1,hTrMod;
extern UINT TargetVM;
extern UINT VektVM[];
extern int LineFromAdr(UINT adr, QString &str);
extern void SelStrBreak(QString str);
extern Srom lrom[];
extern void	OnType(UINT adr,Srom* p1,char*);
extern void	OnTypeSet(UINT adr,Srom* p1,char*,QString,UINT);
extern UINT mul_ind[];
extern QString fr,DefaultDir,SystemDir;
//extern Cdisloc* pDl;
//extern CDr* pDr;
extern int AdrFromIdent(QString im, UINT &adr,QString& e_im);
extern short readCP0(int nr,short nom,UCHAR *a);
extern short writeCP0(int nr,short nom,UCHAR *a);
extern short readCP1(int nr,short nom,UCHAR *a);
extern short writeCP1(int nr,short nom,UCHAR *a);
//extern BOOL m_ActivAc;
extern UINT nulwrem,swrem;
extern DWORDLONG wrrom(void);
extern void printkl(QString s);       
extern   short kolwm;      //Количество ВМ  
DWORDLONG mytime=0;
int	WriteCP1(int nom,UCHAR* dat);
int DelAll(UINT iVM);
int DelBreakB(QString str);
HANDLE eOpros,eReset ;
void	fZakaz(int z,int targ);	//contin
void printIskl(UINT iVM,UINT);
void OnTypeT(QString s);
void printOst(UINT otw,UINT iVM,UINT wibcom,int pr,UINT);
void Opros(void);
void MestOst(int targ);
int		OnPrintForGetadr(QString s2,QString s3);
int	WriteCP0(int nom,UCHAR* dat);
void DelRed(int dpr,UINT dadr,QString ds);
int AddBreakTarget(int prbp, QString s, QString str);
UINT NaturalStop=1,ForGo=1;
UINT mk_gop=0xD0005678;
UCHAR buf_print[1000]={0};
	int iVM,i,pr,rrn;
QString tip[7]={"","","","","","",""};
union 
{
	UINT word[500];
	UCHAR ch[2000];
}bufx;
void ForPrint(UINT);
void print_time(void);
UINT TargetVM=1;
UINT VektVM[7]={0,1,0,0,0,0,0};
extern UINT Komplex[];
//////////////////////////////////////////////////////

/////////////////////////
M_BP m_bp[1000]={0};	//для подтягивания массива
QString ms_bp[1000];
////////////////////////////
M_BP m_bp1[1000]={0};
QString ms_bp1[1000];
M_BP m_bp2[1000]={0};
QString ms_bp2[1000];
M_BP m_bp3[1000]={0};
QString ms_bp3[1000];
M_BP m_bp4[1000]={0};
QString ms_bp4[1000];
M_BP m_bp5[1000]={0};
QString ms_bp5[1000];
M_BP m_bp6[1000]={0};
QString ms_bp6[1000];
M_BP *pBP[]={0,
	m_bp1,m_bp2,m_bp3,m_bp4,m_bp5,m_bp6
};
QString* p_ms[]={ms_bp1,ms_bp1,ms_bp2,ms_bp3,ms_bp4,ms_bp5,ms_bp6};


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
extern ntip *k;
extern HWND hDr;
extern HWND hDl;
extern void KillAll(void);
extern HWND hDism;
extern HANDLE eDirective ;
extern UCHAR bufpr[], bufper[];
extern UINT fpere(QString str);
extern double dfpere(QString str);
//extern CComandBar *pCom;
//extern CBp *pBp;
extern UCHAR bufch[];
extern int WBufCom(UINT adr, int len, UCHAR *buf);
extern int KillRom(QString name);
extern int AttachRom(QString name);
extern int LoadRom(QString name);
extern void prmdier(QString str);
extern int NomToAdr( QString str, UINT& adr);
extern char* Disass(unsigned long int cod,unsigned long int epc,unsigned short pr);
extern int ReadCom(int col,UCHAR* buf);
extern void fnext(void);
extern void fstep(void);
extern void fnexti(int);
extern void fstepi(int);
extern int PrintRegs(UINT adr, QStringList p);
extern int PrintFRegs(UINT adr, QStringList p);
extern void prmdi(QString);
extern QString imakom;
extern QStringList name_regs;
extern QStringList name_regs_st;
extern QStringList name_fregs;
extern int prtab;
extern      void  intkom(LPDWORD aaa);
extern UINT adr_mail;
extern UINT adr_contact;
extern UINT mk_print;
extern UINT adr_maila;
extern UINT adr_maill;
///// значения в contact
extern UINT mk_gop;
extern HWND hCom;
extern HWND hDmod;
extern HWND hBp;
//extern HWND m_W;
extern UINT paus;
extern int VivRegs(QString s2,QString s3);
QString direct,Errs,strmdi,strForBr,strForDel;
extern QString out,out1;
void ThreaDeb();
UINT GetAir(QString str);
UINT ADRI=0;	// адрес следующей выполняемой команды 
UINT ADRB=0;	// адрес следующего байта в ОЗУ по Х
UCHAR FORMAT='X';	// формат выдачи по Х или по I (instruction)
int uu=4;  //единица измерения
int nn=4;  //
extern int kolvo(QString stroka);
extern int format(QString st);
extern int perevod(UINT adr,UINT mn);
int WdatAdr6(UINT adr, UCHAR* dat);
int OnWrite(QString s2,QString s3);
int OnDump(QString s2,QString s3);
void	OnDisplay(void);	// In windows Display registers and memory
int prT=0;
int		OnPrintR(QString s2, QString s3);
int OnSou(QString str);
UINT M_adr;
QString Me_im;
HANDLE kDirective;
int fjmp(QString adrs, int pr);
void	OnDisLoc(void);	// In windows Display locals
UINT    prkom=0;
HANDLE Globkolp_k;
DWORD  Treadidp_k;
int WdatAdr(UINT adr, UINT dat);
int prcommand=0;
UINT j_buf=0;
int fSet(QString str);
int fx(QString par, QString dat);
int RDatAdr(UINT n, UINT adr);
int NoWait=0;
int DelBreak(QString str);
int DelAll(void);
void bib(void);
int AddBreak(int prbp, QString s, QString str);
QStringList NameExcCode={
		"Прерывание",
			"TLB - ячейка закрыта от записи",
			"TLB - при загрузке или выборке команды",
			"TLB - при запоминании",
			"ошибка адресации - загрузка или выборка команды",
			"ошибка адресации - запоминание",
			"ошибка шины - выборка команды",

			"ошибка шины - обращение к данным",
			"Syscall",
			"Breakpoint",
			"зарезервированная команда",
			"сопроцессор недоступен",
			"арифметическое переполнение"
};

int mBreakCom=0;


   extern struct mod_table *pModTable;
   extern struct reg_table *pRegTable, RegTable;
   extern UINT *vxod,*vixod; //Для указателей входного и выходного буфера модели.
   extern UINT ident;       //Для идентификационного номера модели.
   extern void (*ukf)(void);  //Для указателя на функцию модели
   extern UINT pes;      //Атавизм
   extern short dlln;  //Признак наличия DLL
   extern void zakaz(UINT rbl);
   extern	UINT wkltakt;      //Признак включения тактового разбуживания
   extern void printklp();
   static long int load_pkm_dll_2(QString str);

   extern UINT wus[];
   extern ntip *bazuk[7];
   extern DWORDLONG Tmin; //Минимальное время событий

   extern int OnLoadMro(QString name);
   
/////////////////////////////////////////////////////

void bib(void)
{
	Beep(200,200);
}


/////////////////////////////////////////////////////
extern BOOL dirVis;
extern BOOL lineVis;
extern BOOL stopVis;
extern BOOL breakVis;
	UINT bpadr;

void ThreaDeb( )
{
	int l,ls,i,t,it=0;
	QString otl,otl1,ss,s1,s2,s3,s,longstr;
	for(;;)
	{
	 prcommand=0;
////////////////////////////////////////// если не ком. файл, открываем кнопки
//		if(!prkom)
	//	if((!prkom)||(prkom && mBreakCom))
	//	{
	//	HWND p=GetDlgItem(hCom,IDC_DIR);//->
	//	dirVis = TRUE;
	//	p=GetDlgItem(hCom,IDC_LINE);//->
	//	lineVis = TRUE;
	//	
	//	GetDlgItem(hCom,IDC_STOP);//->
	//	stopVis = FALSE;
	//	breakVis = FALSE;
	//::SendMessage(hCom,WM_MDI191,1,1);

	//if(pBp)
	//{
	//HWND p4=GetDlgItem(hBp,IDC_DELBP);//->
	//::EnableWindow(p4,TRUE);
	//HWND p5=GetDlgItem(hBp,IDC_DELALL);//->
	//::EnableWindow(p5,TRUE);

	//	
	//}
	//	}
	///////////////////////////////////////////////////
//		if((prkom)&&!prT)
		if(((prkom)&&!prT)&&(!mBreakCom))
//		{
		{
			prT=1;SetEvent(kDirective);// продолжение ком. файла;

		}
	//m_ActivAc=TRUE;		
		WaitForSingleObject(eDirective, INFINITE);// ждем команду из Intkom
	  	 ResetEvent(eDirective);
		 prcommand=1;
	prT=0;
	//if(pBp)
	//{
	//HWND p4=GetDlgItem(hBp,IDC_DELBP);//->
	//::EnableWindow(p4,FALSE);
	//HWND p5=GetDlgItem(hBp,IDC_DELALL);//->
	//::EnableWindow(p5,FALSE);
	//}
//		}
	longstr=direct;
	while(longstr!="")
	{

	l=longstr.indexOf(';');
	if(l==(-1))
	{
		direct=longstr; longstr="";
	}
	else
	{
		direct=longstr.left(l);
		longstr = longstr.right(longstr.count() - l - 1);

	}

	Errs="# "+direct+" - "+"Ошибка";
	s=direct.trimmed();
	s1=""; s2=""; s3="";

	if(s.isEmpty()) 
	{
		prmdier(Errs);  continue;
	}

	i=s.indexOf(' ');t=s.indexOf('\t');
	if((i==(-1)) && (t==(-1)))
	{
		s1=s; s="";
	}
	else
	{
				if(i==(-1)) it=t;
		else if(t==(-1)) it=i;
		else
		{
			if((i!=(-1)) && (t!=(-1)))
			{
				if(i>t) it=t;
				else it=i;
			}
		}
	s1=s.left(it);
	ls=s1.count();
	s = s.right(s.count() - ls);
	s = s.trimmed();

	}
	s1 = s1.toUpper();
	//////////////////////
	if(s1=="SHELL")
	{
		system(s.toLocal8Bit().data());
		prmdi(direct);  continue;
	}
	///////////////////////////////////////// s2
	if(!s.isEmpty() &&
		((s1!="LOAD") &&(s1!="KILL") &&(s1!="ATTACH") &&(s1!="COMMAND") ))
{
	i=s.indexOf(' ');t=s.indexOf('\t');
	if((i==(-1)) && (t==(-1)))
	{
		s2=s; s="";
	}
	else
	{
				if(i==(-1)) it=t;
		else if(t==(-1)) it=i;
		else
		{
			if((i!=(-1)) && (t!=(-1)))
			{
				if(i>t) it=t;
				else it=i;
			}
		}
	s2=s.left(it);
	ls=s2.count();
	s = s.right(s.count() - ls);
	s = s.trimmed();
	}
}
	if((s1=="LOAD") ||(s1=="KILL") ||(s1=="ATTACH") ||(s1=="COMMAND") )
	{ s2=s; s="";s2 = s2.trimmed();}
	////////////////////////////////////////// s3
	if(!s.isEmpty() && 
		((s1!="LOAD")&&(s1!="KILL")&&(s1!="ATTACH")&&(s1!="COMMAND")))
{
	i=s.indexOf(' ');t=s.indexOf('\t');

	if((i==(-1)) && (t==(-1)))
	{
		s3=s; s="";
	}
	else
	{
				if(i==(-1)) it=t;
		else if(t==(-1)) it=i;
		else
		{
			if((i!=(-1)) && (t!=(-1)))
			{
				if(i>t) it=t;
				else it=i;
			}
		}
	s3=s.left(it);
	ls=s3.count();

	s = s.right(s.count() - ls);
	s = s.trimmed();
	}
}
	///////////////////////////////////////// анализ директшвы

	s1 = s1.toUpper();
////////////////
	if(s1=="SET")
	{
		fSet(s2);
				OnDisplay();	// In windows Display registers and memory
				continue;};
//////////////////////////////////

		if(s1=="X") {
		if((!s2.isEmpty())&&(!s2.isEmpty()))
		{
			if(s2.at(0)!=QChar('/'))
			{
				prmdier(Errs); continue;
			}
		}
		fx(s2,s3); continue;}
/////////////////////////////////////////////////////////////

	if(s1=="ATTACH")
	{
		if(s2.isEmpty())
		{
			prmdier(Errs); continue;
		}
		
		if(AttachRom(s2))
	{
		prmdier("# Команда  не выполнена"); continue;
	}
	else
	{
	prmdi("# Команда выполнена"); continue;
	}
	}
////////////////////////////////////////////////////////////
	if(s1=="LOAD")
	{
		if(s2.isEmpty())
		{
			prmdier(Errs); continue;
		}
		
		if(LoadRom(s2))
	{
	OnDisplay();	// In windows Display registers and memory
		prmdier("# Загрузка не выполнена"); continue;
	}
	else
	{
	OnDisplay();	// In windows Display registers and memory
	prmdi("# Загрузка выполнена"); continue;
	}
	}
/////////////////////////////////////////////////////////////
	if(s1=="KILL")
	{
		if(s2.isEmpty())
		{
			prmdier(Errs); continue;
		}
		s3=s2;
		s3 = s3.toUpper();
		if(s3=="ALL")
		{
			KillAll(); continue;
		}
		else

		{prmdi(direct);KillRom(s2); continue;}
	}
////////////////////////////////////
	if(s1=="GETADR")
	{
	if(s3!="")
		{
			prmdier(Errs); continue;
		}
	OnPrintForGetadr(s2,s2);
	continue;
	}

///////////////////////////////////////////////////////////
	if((s1=="TRAPTIME")||(s1=="TT"))
	{
		prmdi(direct);
		s2 = s2.toUpper();
		if(s2=="")
		{
			widser();
		continue;
		}
		if(s2=="CLEAR")
		{
			sernull();
		continue;
		}
		if(s2=="OFF")
		{
			serwkl(0);
		continue;
		}
		if(s2=="ON")
		{
			serwkl(1);
		continue;
		}
	}
	/////////////////////////////////////////////////
	if(s1=="JUMP")
	{
		if(s2.isEmpty()||(s3!=""))
		{
			prmdier(Errs); continue;
		}

				for(i=1;i<=kolwm;i++)
		{
			mestost[i]="";
			adresost[i]=1;
		}
	fjmp(s2,0);
	continue;
	}
	if (s1 == "LOADROM")
	{
		if (potkl == 1)
		{
			prmdier(direct);
			prmdier("#Данное действие возможно только при остановленном процессоре");
			continue;
		}

		if ((s2.isEmpty()) || (!s3.isEmpty())) { prmdier(Errs); continue; }
		prmdi(direct);

		OnLoadMro(s2);
		continue;
	}

///////////////////////////////////
	if ((s1=="CONCOM")||(s1=="CC"))
	{
		mBreakCom=0;
		widasa=old_widasa;
		continue;
	}
///////////////////////////////////
	if ((s1=="BREAKCOM")||(s1=="BC"))
	{
		printkl("# Приостанов командного файла");

		mBreakCom=1;
		old_widasa=widasa;
		widasa=0;
		continue;
	}
	//////////////////////////////

	if(s1=="STOP")
	{
		prmdi("Stop");
		if(potkl==0) continue;
		fZakaz(04,TargetVM);
		for(;;)
		{
			Sleep(3);
			if(potkl==0) break;
			Sleep(3);
		}
		Sleep(30);
		continue;
	}
/////////////////
	if((s1=="STOPW")||(s1=="STOPR")||(s1=="STOPWR")||(s1=="STOPRW")||
		(s1=="TSTOPW")||(s1=="TSTOPR")||(s1=="TSTOPWR")||(s1=="TSTOPRW")
		||(s1=="TBREAK")||(s1=="BREAK"))
	{
		int pr=0;
		if(s1=="BREAK") s1="STOPR";
		if(s1=="TBREAK") s1="TSTOPR";

		if(s1=="STOPW") pr=02;
		if(s1=="STOPR") pr=01;
		if((s1=="STOPWR")||(s1=="STOPRW")) pr=03;
		
		if(s1=="TSTOPW") pr=06;
		if(s1=="TSTOPR") pr=05;
		if((s1=="TSTOPWR")||(s1=="TSTOPRW")) pr=07;
		

	if(s3!="")
		{
			prmdier(Errs); continue;
		}
		if(s2.at(0)==QChar('*'))
		{
			AddBreak(pr,"",s2);	// добавление новой breakpoint
			 continue;
		}
		if(s2.indexOf('/')==s2.indexOf('\\'))
		{
			if(AdrFromIdent( s2, M_adr, Me_im))
			{
			prmdier(Errs);
			prmdier("# невозможно определить адрес");
			continue;
			}
	ss.sprintf("*%08X",M_adr);
	direct=s1+" "+Me_im;
		AddBreak(pr,Me_im,ss);	// добавление новой breakpoint
			 continue;
	}
	if( NomToAdr( s2, bpadr))		//BpAdr(s2,&bpadr))
	{
		prmdier(Errs);
		prmdier("# невозможно определить адрес");
		continue;
	}
	ss.sprintf("*%08X",bpadr);
		AddBreak(pr,s2,ss);	// добавление новой breakpoint
			//::SendMessage(m_W,WM_MDI188,0,0);// color

			 continue;
	}
	
/////////////////////////////////////////////////////////////	
	/////////////////////////////////////////////////////////
	if((s1=="DELETE")||(s1=="DEL"))
	{
		if(s2.isEmpty()) 
		{
			DelAll(); continue;
		}
			DelBreak(s2);	// delete breakpoint


		continue;
	}
	/////////////////////////////////////////////////
	if(s1=="INFO")
	{
	s2 = s2.toUpper();

		if(s2=="LOCALS") 
		{
			
//			fLocInRes();
			continue;
		}
		else
		VivRegs(s2,s3);
		continue;
	}
////////////////////////////////////////////////
	if(s1=="STEPI")
	{
		fstepi(0);
		OnDisplay();OnDisLoc();	// In windows Display registers and memory
	continue;
	}

	if(s1=="WAITSTOP")
	{
		prmdi("WaitStop");
		for(;;)
		{
			Sleep(3);
			if(NaturalStop!=0) break;
			Sleep(3);
		}
		Sleep(320);
		NaturalStop=0;
		continue;
	}
	
////////////////////////////////////////////////
	if(s1=="NEXTI")
	{
		fnexti(0);
	OnDisplay();OnDisLoc();	// In windows Display registers and memory
		continue;
	}

////////////////////////////////////////////////
	if(s1=="STEP")
	{
		fstep();
		OnDisplay();OnDisLoc();	// In windows Display registers and memory
	continue;
	}

////////////////////////////////////////////////
	if(s1=="NEXT")
	{
		fnext();
	OnDisplay();OnDisLoc();	// In windows Display registers and memory
		continue;
	}

////////////////////////////////////////////////
	if((s1=="CONTINUE") || (s1=="CONTIN"))
	{
		prmdi("Continue");
		if((potkl==0)&&(ForGo==1))// continue;
		{
			for(i=1;i<=kolwm;i++)
			{
				mestost[i]="";
				adresost[i]=1;
			}

			NaturalStop=0;
			fZakaz(020,TargetVM);
			SetEvent(eProz); Sleep(2);
			SetEvent(eOpros);

			Sleep(9);
		}
		continue;
	}
////////////////////////////////////////////////
////////////////////////////////////////////////
	if((s1=="GO"))
	{
		prmdi("GO  ");
		if((potkl==0)&&(ForGo==1))// continue;
		{
			for(i=1;i<=kolwm;i++)
			{
				mestost[i]="";
				adresost[i]=1;
			}

			NaturalStop=0;
			fZakaz(020,TargetVM);
			SetEvent(eProz); Sleep(2);
			SetEvent(eOpros);

			Sleep(9);
//	OnDisplay();	OnDisLoc();// In windows Display registers and memory
		}
		continue;
	}
////////////////////////////////////////////////

	if(s1=="COMMAND")
	{
		if(mBreakCom)
		{
			prmdi("# При приостанове командного файла новый запускать нельзя");
			continue;
		}
		OnSou(s2);
		continue;
	}
//////////////////////////////////
	if(s1=="PRINT")
	{

	if(s2=="") 
	{prmdier(Errs); continue;}
		OnPrintR(s2,s2); continue;
	}
//////////////////////////////////
	if(s1=="TYPE")
	{

	if(s2=="") 
	{prmdier(Errs); continue;}
		OnTypeT(s2); continue;
	}
//////////////////////////////////
	if(s1=="WRITE")
	{

	if(s3=="") {prmdier(Errs); continue;}
	if(s2=="") {prmdier(Errs); continue;}
	if(s2.at(0)!=QChar('/')) {prmdier(Errs); continue;}
	
	if(OnWrite(s2,s3))
	{
		prmdier("# команда не выполнена");
	}
	OnDisplay();	// In windows Display registers and memory
	continue;
	}
	//////////////////////////////////////////////////////////
	if(s1=="SCD")
	{
	//	if(s2=="*")
	//	{
	//	SetCurrentDirectory( LPCTSTR (DefaultDir ));
	//		s1=s1+" * #" +DefaultDir;
	//prmdi(s1); continue;
	//	}
	//	if(!SetCurrentDirectory( LPCTSTR (s2 )))
	//	{
	//		prmdier(Errs); continue;
	//	}
		s1=s1+" "+s2;
			prmdi(s1); continue;
	}
/////////////////////////////////////////////////////
	if(s1=="SSD")
	{
		//if(!SetCurrentDirectory( LPCTSTR (SystemDir )))
		//{
		//	prmdier(Errs); continue;
		//}
			prmdi(s1); continue;
	}
///////////////////////////////////////////
	if(s1=="PCD")
	{wchar_t bufp[1000];

			 GetCurrentDirectory(1000, &bufp[0]);  // найти текущую директорию
	s2=QString::fromWCharArray(bufp);
	s2=s1+" # "+s2;

			prmdi(s2); continue;
	}
	//////////////////////////////////
		if(s1=="RESET")
	{
		prmdi("Reset");
			zakrom[1]=TargetVM;
		fZakaz(040,TargetVM);
		if(potkl==0)
		{
	SetEvent(eProz); Sleep(5);
		}
	SetEvent(eOpros);
				WaitForSingleObject(eReset, INFINITE);
	  	 ResetEvent(eReset);

  for(;;)
		 {
		 Sleep(40);
		if((potkl==0)&&(ForGo==1)) break;
		 }

	continue;
	}

	///////////////////////////////////////////
	if(s1=="DUMP")
	{

	if(s3=="") {prmdier(Errs); continue;}
	if(s2=="") {prmdier(Errs); continue;}
	if(s2.at(0)!=QChar('/')) {prmdier(Errs); continue;}
	
	if(OnDump(s2,s3))
	{
		prmdier("# команда не выполнена");
	}
	continue;
	}
	//////////////////////////////////////////////////////////
	if((s1=="SYSTIME")||(s1=="ST"))
	{
		s2 = s2.toUpper();
		if((s2!="CLEAR")&&(s2!="ON")&&(s2!="OFF")&&(s2!="INFO"))
		{
			prmdier(Errs);bib(); continue;
		}
	prmdi(direct);
		if(s2=="CLEAR")
		{
		mytime=wrrom();continue;
		}
		if(s2=="INFO")
		{
			print_time(); continue;
		}
		continue;
	}

	//////////////////////////////////
	if(s1=="TARGET")
	{
		union 
		{
			UCHAR c[4];
			UINT w;
		}d={0};
		QString s="";

		if(s2=="")
		{
			otl.sprintf("TARGET # BM %d",TargetVM-1);
			prmdi(otl);
///////////////
	readCP0(39,(USHORT)(TargetVM),&d.c[0]);
			if(!LineFromAdr(d.w,s))
		{
		SelStrBreak(s);
		}
			continue;
/////////////
		}
		otl=s1+" "+s2;
		prmdi(otl);
		UINT tar=fpere(s2);
		if(tar>=(UINT)kolwm)
		{
			prmdier("# Ошибка");
			continue;
		}
		if(Komplex[tar+1]==0)
		{
			prmdier("# Ошибка, канал выключен");
			continue;
		}
		TargetVM=tar+1;
	readCP0(39,(USHORT)(TargetVM),&d.c[0]);
			if(!LineFromAdr(d.w,s))
		{
		SelStrBreak(s);
		}


		continue;
	}
	///////////////////////////////////////////////////
	if(s1=="VM")
	{
		if(s2=="")
		{
			otl="VM # Информационный обмен с ";
			gruppa1[0]=0;
			gruppa2[0]=0;
			gruppa3[0]=0;
			gruppa4[0]=0;
			for(i=1;i<=kolwm;i++)
			{
				if(VektVM[i]==0) continue;
				
				if(	i==1) gruppa1[0]=1;
				if(	i==2) gruppa2[0]=1;
				if(	i==3) gruppa3[0]=1;
				if(	i==4) gruppa4[0]=1;
				otl1.sprintf(" BM%d",i-1);
				otl=otl+otl1;
			}
			prmdi(otl);
			continue;
		}
		otl=s1+" "+s2;
			prmdi(otl);

		for(i=1;i<=kolwm;i++)
		{
			otl.sprintf("%d",i-1);
			if(s2.indexOf(otl)==(-1))
				VektVM[i]=0;
			else VektVM[i]=1;
		}
		int pr=0;
		for(i=1;i<=kolwm;i++)
		{
			if(VektVM[i]!=0) pr=1;
		}
		if(pr==0)
			prmdier("# Ошибка, все заказанные VM отсутствуют");
		continue;
	}
	if(s1=="PWOFF")
	{
		if(s2=="")
		{
			otl="PWOFF # Выключены ";

			gruppa1[0]=0;
			gruppa2[0]=0;
			gruppa3[0]=0;
			gruppa4[0]=0;
				if(Komplex[1]!=0) 	gruppa1[0]=1;
				if(Komplex[2]!=0) 	gruppa2[0]=1;
				if(Komplex[3]!=0) 	gruppa3[0]=1;
				if(Komplex[4]!=0) 	gruppa4[0]=1;


	int pr_off=0;
	QString s_off;
			for(i=1;i<=kolwm;i++)
			{
				if(Komplex[i]!=0)
					continue;
				pr_off=1;
				
				if(	i==1) s_off=" BM0";
				if(	i==2) s_off=" BM1";
				if(	i==3) s_off=" BM2";
				if(	i==4) s_off=" BM3";
				otl=otl+s_off;
			}
			if(pr_off==0) prmdi ("PWOFF # Все включено ");
			else  
			prmdi(otl);
			continue;
		}
		otl=s1+" "+s2;
			prmdi(otl);

		for(i=1;i<=kolwm;i++)
		{
			otl.sprintf("%d",i-1);
			if(s2.indexOf(otl)!=(-1))
				Komplex[i]=0;
		}
		continue;
	}

//#include "Add_to_Thdeb.cpp"
	
	
	prmdier(Errs);bib(); continue;
	
	





	
	}


	}
	//ExitThread(TRUE);
}
	/////////////////////////////////////////////////////////
int fSet(QString str)
{
		QString str1,str2,name,base_name,base_path,otl;
		UINT adr,dat,a;
		int i,j;
		union {
			UCHAR ch[8];
			UINT d1[2];
			UINT dat;
			double ddat;
			float datf;
		}dat1;
		UCHAR tip=0;
		

		if(str.isEmpty())
	{
	prmdier(Errs); return(1);
	}

	i=str.indexOf('=');
	if(i==(-1))
	{	prmdier(Errs); return(1);	}
	str1=str.left(i);
	str1 = str1.trimmed();
	str2=str.mid(i+1);
	str2 = str2.trimmed();
	if(str2=="")
	{	prmdier(Errs); return(1);	}

	QChar ch=str2.at(0); 
	if((!ch.isDigit())&&(ch!='f')&&(ch!='g')&&(ch!='-')&&(ch!='+')
		&&(ch!='c')&&(ch!='h')&&(ch!='w') )

	{
	prmdier(Errs); return(1);	}
	UINT air_tip;
	air_tip=GetAir(str2);
	if((ch=='c')||(ch=='h')||(ch=='w')||(ch=='f'))
	{
		str2.remove(0, 1);
	}

	if((str1.at(0)=='*')||(str1.at(0)=='$'))
	{
	QChar ch1=str2.at(0); 
	
	if((!ch.isDigit())&&(ch1!='-')&&(ch1!='+'))
	{
		prmdier(Errs);
		prmdier("# Можно записывать только слово");
		return 1;
	}
	
	
	if(ch.isDigit()||(ch=='-')||(ch=='+'))
	{
	dat1.dat=(UINT)fpere(str2);tip=0;
	}
	if(ch=='f')
	{
		dat1.datf=(float)dfpere(str2);tip='f';
	}
	if(ch=='g')
	{
		str2.remove(0, 1);

		dat1.ddat=dfpere(str2);tip='g';
	}

	if(str1.at(0)=='*')
	{
		str1.remove(0, 1);
		str1 = str1.trimmed();
	str1="0x"+str1; 
	adr=(UINT)fpere(str1);
	prmdi(direct);	
			if(!tip)
		{
	i=WdatAdr6(adr,&dat1.ch[0]);
		}
		if(tip=='g')
		{
	i=WdatAdr6(adr,&dat1.ch[0]);
	i+=WdatAdr6(adr+4,&dat1.ch[4]);
		}
		if(tip=='f')
		{
		WdatAdr6(adr,&dat1.ch[0]);
		}
			   k->axtung=0xFFFFFFFF;	


	return(i);
	}
	else
	{
		//////////////////////////// set $

		if(((str1.at(1)=='f')&&(str1.at(2)!='p')) ||
			((str1.at(1)=='c')&&(str1.at(2)=='s')))
		{
			for(j=0;j<33;j++)
			{
				if(str1==name_fregs[j]) break;
			}
			if(j==33)
	{	prmdier(Errs); return(1);	}

	 dat=(UINT)fpere(str2);
	prmdi(direct);
				if(!tip)
		{
		WriteCP1(j,&dat1.ch[0]);
		}
		if(tip=='g')
		{
		WriteCP1(j,&dat1.ch[0]);
		WriteCP1(j+1,&dat1.ch[4]);
		}
		if(tip=='f')
		{
		i=WriteCP1(j,&dat1.ch[0]);
		}

			   k->axtung=0xFFFFFFFF;	

	return(i);
		}
		QString lo,lo1;
	lo1=str1; 
	lo1 = lo1.toLower();
		for(j=0;j<39;j++)
			{
			lo=name_regs[j];
			lo = lo.toLower();
				if(lo1==lo)	//name_regs[j])
				{
					prtab=0; break;
				}
			}
			if(j==39)
			{
				for(j=0;j<39;j++)
			{
			lo=name_regs_st[j]; 
			lo = lo.toLower();
				if(lo1==lo)	//name_regs_st[j])
				{
					prtab=1; break;
				}
			}
			if(j==39) {	prmdier(Errs); return(1);	}


			}
	 dat=(UINT)fpere(str2);
	prmdi(direct);	
			if(!tip)
		{
		WriteCP0(j+1,&dat1.ch[0]);
			}
		if(tip=='g')
		{
		i=WriteCP0(j+1,&dat1.ch[0]);
		i+=WriteCP0(j+2,&dat1.ch[4]);
		}
		if(tip=='f')
		{
		i+=WriteCP0(j+1,&dat1.ch[0]);
		}
			   k->axtung=0xFFFFFFFF;	

	return(i);
	}
	}
	else
	{
	QString s,progr_name,otl;
	char *p2;
	Srom *p1;

	p1=lrom;
	s=str1;
	int in=s.lastIndexOf(':');
	if(in==(-1)) {name=s;base_name=s;progr_name="";}
	else 
	{
		progr_name=s.left(in+1);
		base_name=s;

		base_name = base_name.right(base_name.count() - in -1);
		base_name = base_name.trimmed();
	}
	name=base_name;
	int tmp_in1 = base_name.indexOf('.');
	int tmp_in2 = base_name.indexOf('[');
	if ((tmp_in1 == (-1)) && (tmp_in2 == (-1)))
		in = -1;
	else
	{
		if (tmp_in1 == (-1))
			in = tmp_in2;
		else
			if (tmp_in2 == (-1))
				in = tmp_in1;
			else
			if (tmp_in2 < tmp_in1)
				in = tmp_in2;
			else
				in = tmp_in1;

	}
	//in=base_name.FindOneOf(".[");
	if(in==(-1)) base_name=name;
	else  
	base_name=base_name.left(in);
	progr_name=progr_name+base_name;
		if(str.indexOf('/')==str.indexOf('\\'))	//т.е. нет
		{
			if(AdrFromIdent( progr_name, M_adr, Me_im))
			{
			prmdier(Errs);
			prmdier("# невозможно определить адрес");
			return(1);
			}
			direct="set "+Me_im+"="+str2;
			in=direct.lastIndexOf(':');
			direct=direct.left(in+1)+name+"="+str2;
	a=	M_adr;
		}
		else
		{
	QChar ch=name.at(0);
	if(ch.isDigit())
		{prmdier(Errs); return(1);}



	if(NomToAdr( progr_name, a)) 
	{prmdier(Errs); return(1);}
		}
	prmdi(direct);
	//p2=(char*)name.GetBuffer(250);name.ReleaseBuffer(-1);
	p1=p1+mul_ind[0];
	OnTypeSet(a,p1,(char*)name.toStdString().c_str(),str2,air_tip);
	return(0);
}

}

/////////////////////////////////////////////////
int WdatAdr6(UINT adr, UCHAR* dat)
{
	int sost,i,j;
	UINT adrt;
	union
	{
		UINT datt;
		UCHAR ch[4];
	}d;
	adrt=adr; 
	for(i=0;i<4;i++)
		d.ch[i]=*(dat+i);
	sost=0;	
	for(j=1;j<=kolwm;j++)
	{
		if(VektVM[j]==0) continue;
		for(i=0;i<4;i++)
		{
			writeb6(adrt+i,(short)j,d.ch[i]);
		}
	}
	if(sost==0) return 0;
	return 0;

}	////////////////////////////
///////////////////////////////////////////////
int fx(QString pars, QString adrs)
{
		QString str1,str2;
		UINT adr=0,par=0;

		if((pars.isEmpty())&&(!adrs.isEmpty()))//
		{

			par=uu; //
			nn=uu;//

			if(adrs.at(0)=='*')//
			{
				adrs.remove(0, 1);
			}

			adrs="0x"+adrs;//
			adr=fpere(adrs);//

			prmdi(direct);//

			ADRB=adr+par;//
		}

		if((pars.isEmpty())&&(adrs.isEmpty()))
		{
			par=uu;  //par=4;	// 4 байта
			nn=uu;//
			adr=ADRB; ADRB=adr+par;
			prmdi(direct);
		}
		
		if((!pars.isEmpty())&&(!adrs.isEmpty()))
		{
			if(adrs.at(0)=='*')
			{
				adrs.remove(0, 1);
			}

			adrs="0x"+adrs;
			adr=fpere(adrs);


			if(pars.at(0)!='/')
			{
				prmdier(Errs); return(1);
			}
			format(pars);
			par=kolvo(pars);//

			prmdi(direct);

			if(par>128)
			{
			par=128;
			prmdier("# Разрешен вывод не более 32-х ячеек");
			}
	
			ADRB=adr+par;

		}

		if((!pars.isEmpty())&&(adrs.isEmpty()))
		{
			format(pars);
			par=kolvo(pars);//

			if(par>128)
				{
				par=128;
				prmdier("# Разрешен вывод не более 32-х ячеек");
				}

			adr=ADRB;
			ADRB=par+ADRB;
		}
		if(!pars.isEmpty())
		{
			format(pars);//

		}
		int a=RDatAdr(par,adr);
			return(a);
}
///////////////////////////////////////////////
//// Чтение по адресу, n - кол-во байт
int RDatAdr(UINT n, UINT adr)
{
	QString adrs,ns,str1,otl;
	int i,j,iVM;
	UINT icod,iadr;
	char* pch;
	UCHAR *pgr=0;

	for(iVM=1;iVM<=kolwm;iVM++)
	{
		if(VektVM[iVM]==0) continue;
		otl.sprintf("#-------------- BM %d ---------------",iVM-1);
		prmdi(otl);
		for(i=0;i<(int)n;i++)
		{
			readb6(adr+i,(short)iVM,bufx.ch[i]);
		}
	if(iVM==1) pgr=(UCHAR*)&gruppa1[0];
	if(iVM==2) pgr=(UCHAR*)&gruppa2[0];
	if(iVM==3) pgr=(UCHAR*)&gruppa3[0];
	if(iVM==4) pgr=(UCHAR*)&gruppa4[0];
	for(j=0;j<(int)n;j++)
	{
		*(pgr+j)=bufx.ch[j];
	}
	iadr=adr;

	perevod(iadr,n);//

	if(FORMAT=='I')
	{
		for(i=0;i<int(n/4);)
		{
		
		out.sprintf("# %08X: %08X ",iadr,bufx.word[i]);
		icod=bufx.word[i];
		if(prtab) pch=Disass((unsigned long int)icod,(unsigned long int)iadr,0);
	else pch=Disass((unsigned long int)icod,(unsigned long int)iadr,1);
	str1=(const char*)pch;
		out+=str1;
		prmdi(out);
		 i=i+1;
		 iadr+=4;
		}
	

	}}
	return(0);
}
//////////////////////////////////////////////////////////		
// Новая точка останова 0-bp, 1-tb
///////////////////////////////////////////
//////// переход по адресу adrs
int fjmp(QString adrs,int pr)	// pr=0 - JMP, CONTIN
								// pr=1 stepi, nexti, step, next 
{
	//CWnd wn;
	QString str,str1,prin,otl;
	UINT adr;
	union 
	{
		int w;
		UCHAR ch[4];
	}d;
	prin="# ";
	if(adrs.at(0)!='*')
	{
		if(adrs.indexOf('/')==adrs.indexOf('\\'))
		{
			if(AdrFromIdent( adrs, M_adr, Me_im))
			{
			prmdier(Errs);
			prmdier("# невозможно определить адрес");
			return(1);
			}
			direct="jump  "+Me_im;
		adr=M_adr;
		}
		else
		{
			if( NomToAdr( adrs, adr))	
			{
				prmdier(Errs);
				prmdier("# невозможно определить адрес");
				return(1);
			}
		}
	}
	// было *число
	else
	{
		adrs.remove(0, 1);
	out="0x"+adrs;
	adr=fpere(out);
	}

	if(adr&3)
	{
		prmdier(Errs); return(1);
	}
	if(!pr)	prmdi(direct);
		d.w=adr;
	JUMP=d.w;
	PJUMP=1;
	fZakaz(020,TargetVM);	//contin
	SetEvent(eProz); //Sleep(5);
	SetEvent(eOpros);
	
	return(0);
}
///////////////////////////////////////
// формирование zakrom[]
void	fZakaz(int z,int targ)	//contin
{
		fbreak.lock();
	
	zakrom[0]=(UINT)z;
	zakrom[1]=(UINT)targ;



					fbreak.unlock();
}

//////////////////////////////////////////////////////////		
// Новая точка останова 
//02-stopw 06-tstopw
//01-stopr 05-tstopr
//03-stopwr 07-tstopwr
// s="" if *
// str=adres

int AddBreak(int prbp, QString s, QString str)
{
	UINT adr;
	int i=0,is=0,iVM=1;
	QString sa;

	fbreak.lock();
	strForBr="";

	sa=str;
	sa.remove(0, 1);
	
	sa="0X"+sa;
	adr=fpere(sa);
	if(!adr)
	{
		prmdier(Errs);	fbreak.unlock();
 return(1);
	}
	prmdi(direct);
	for(iVM=1;iVM<=kolwm;iVM++)
	{
		if(VektVM[iVM]==0) continue;
	is=(-1);
	for(i=0;i<1000;i++)
	{
		if((((pBP[iVM]+i)->pr)!=0)&&(((pBP[iVM]+i)->adr)==adr)) break;
		if((pBP[iVM]+i)->pr==0)
		{
			is=i;break;
		}
	}
	if(s.isEmpty()) ((pBP[iVM]+i)->pr)=prbp;
	else
	{
		((pBP[iVM]+i)->pr)=(prbp|010);// есть текст
		*(p_ms[iVM]+i)=s;
	}
		((pBP[iVM]+i)->adr)=adr;

	}
		strForBr=s;

	//if(pBp)		::SendMessage(hBp,WM_MDI2,i,0);

	fbreak.unlock();

	return(0);
}
//////////////////////////////////////////
int AddBreakTarget(int prbp, QString s, QString str)
{
	UINT adr;
	int i=0,is=0,iVM=1;
	QString sa;

	fbreak.lock();
	strForBr="";

	sa=str;

	sa.remove(0, 1);
	sa="0X"+sa;
	adr=fpere(sa);
	if(!adr)
	{
		prmdier(Errs);	fbreak.unlock(); return(1);
	}
	prmdi(direct);
	iVM=TargetVM;
	for(i=0;i<1000;i++)
	{
		if((((pBP[iVM]+i)->pr)!=0)&&(((pBP[iVM]+i)->adr)==adr)) break;
		if((pBP[iVM]+i)->pr==0)
		{
			is=i;break;
		}
	}
	if(s.isEmpty()) ((pBP[iVM]+i)->pr)=prbp;
	else
	{
		((pBP[iVM]+i)->pr)=(prbp|010);// есть текст
		*(p_ms[iVM]+i)=s;
	}
		((pBP[iVM]+i)->adr)=adr;

		strForBr=s;

	//if(pBp)		::SendMessage(hBp,WM_MDI2,i,0);

	fbreak.unlock();

	return(0);
}
//////////////////////////////////////////
///////////////////////////////
// Удаление всех точек останова
int DelAll(void)
{
	int iVM;
	QString sa="";
	fbreak.lock();          //Начало критической секции

	prmdi(direct);
	for(iVM=1;iVM<=kolwm;iVM++)
	{
		if(VektVM[iVM]==0) continue;

		for(int i=0;i<1000;i++)
		{
						if((pBP[iVM]+i)->pr==0) break;
			(pBP[iVM]+i)->pr=0;

			if(((pBP[iVM]+i)->pr)&4) sa=*(p_ms[iVM]+i);else sa="";
			if((sa.indexOf('/')!=(-1))||(sa.indexOf('\\')!=(-1)))
			{
					strForDel=sa;
	//::SendMessage( m_W,WM_MDI38,0,0);
			}

		}

	}

	//if(pBp)		::SendMessage(hBp,WM_MDI3,0,0);
	fbreak.unlock();          //Начало критической секции
		return(0);
}
///////////////////////////////
// Удаление всех точек останова
int DelAll(UINT iVM)
{
	fbreak.lock();          //Начало критической секции


		for(int i=0;i<1000;i++)
		{
			if((pBP[iVM]+i)->pr==0) break;
			(pBP[iVM]+i)->pr=0;
		}

	//if(pBp)		::SendMessage(hBp,WM_MDI3,0,0);
	fbreak.unlock();          //Конец критической секции
		return(0);
}
/////////////////////////////////////////////
// Удаление точки останова
int DelBreak(QString str)
{
	UINT adr=0,bpadr=0;
	int i,iVM;
	QString sa,ss,ds;
	fbreak.lock();          //Начало критической секции

	sa=str;ss=str;

	prmdi(direct);
		if((sa.indexOf('/')==(-1))&&(sa.indexOf('\\')==(-1))&&(sa.at(0)!='*'))
		{
			if(AdrFromIdent( sa, M_adr, Me_im))
			{
			prmdier(Errs);
			prmdier("# невозможно определить адрес");
	fbreak.unlock();          //Начало критической секции
			return 1;
			}
	ss.sprintf("*%08X",M_adr);
		}
	else
	{
		if(sa.at(0)!='*')
	{

		if( NomToAdr( sa, bpadr))	
	{
		prmdier(Errs);
		prmdier("# невозможно определить адрес");	fbreak.unlock();
	return 1;
	}
	ss.sprintf("*%08X",bpadr);

	/////////////////////////////////////////
	}
}
	sa=ss;
		if(str.at(0)=='*')
		{
			sa=str;
		}
		sa.remove(0, 1);
	
	sa="0X"+sa;
	adr=fpere(sa);
	for(iVM=1;iVM<=kolwm;iVM++)
	{
		if(VektVM[iVM]==0) continue;
	for(i=0;i<1000;i++)
	{
		if((((pBP[iVM]+i)->pr)!=0)&&(((pBP[iVM]+i)->adr)==adr)) 
		{
			(pBP[iVM]+i)->pr=0; break;
		}
	}
		TrimBreak(iVM);
		}
	DelRed(0,0,str);
		//if(pBp)		::SendMessage(hBp,WM_MDI3,adr,0);
	fbreak.unlock();
		return(0);
}
////////////////////////////////////////////////
void DelRed(int ,UINT ,QString ds)
{
	int iVM,i,kol;
	QChar ch;
	kol=0;
	int in4=ds.lastIndexOf(':');
	if (in4==(-1)) return;
		ch=ds.at(in4+1);
		if(!ch.isDigit())
			
			return;
	fbreak.lock();          //Начало критической секции

		for (iVM=1;iVM<=kolwm;iVM++)
	{
	for(i=0;i<1000;i++)
	{
		if(	((pBP[iVM]+i)->pr)==0) break;
	if((*(p_ms[iVM]+i)==ds))
		{
			kol++;
		}
	}
	if(kol) break;
		
	}
		if(kol!=0)
		{
		fbreak.unlock(); return;
		}

						strForDel=ds;
	//::SendMessage( m_W,WM_MDI38,0,0);
		fbreak.unlock(); return;

}
/////////////////////////////////////////////
// Удаление точки останова
int DelBreakB(QString str)
{
	UINT adr=0;
	int i=0,iVM=1;
	QString sa;

	sa=str;

	prmdi(direct);
		if(str.at(0)=='*')
		{
			sa.remove(0, 1);
	sa="0X"+sa;
	adr=fpere(sa);
	for(iVM=1;iVM<=kolwm;iVM++)
	{
		if(VektVM[iVM]==0) continue;
	for(i=0;i<1000;i++)
	{
		if((((pBP[iVM]+i)->pr)!=0)&&(((pBP[iVM]+i)->adr)==adr)) 
		{
			(pBP[iVM]+i)->pr=0; break;
		}
	}
	for(i=0;i<1000;i++)
	{
		if(((m_bp[i].pr)!=0)&&(*(p_ms[iVM]+i)==str)) 
		{
			(pBP[iVM]+i)->pr=0; break;
		}
	}
		TrimBreak(iVM);
		}
		}

		//if(pBp)		::SendMessage(hBp,WM_MDI3,adr,0);

		return(0);
}
/////////////////////////////////////////////////
//////Запрос целевой машине
extern UCHAR bufVnutr[];

/////////////////////////////////////////////////
int OnSou(QString str)
{
	prmdi(direct);
	imakom=str;
	hTrModel1=	::CreateThread(NULL,
		0,(LPTHREAD_START_ROUTINE)intkom,&Globkolp_k,0,&Treadidp_k);
//	::SetThreadPriority(hTrModel1,THREAD_PRIORITY_ABOVE_NORMAL);
		Sleep(100);
		prT=1;
		return(0);
}
/////////////////////////////////////////////////////////////////////
int		OnPrintR(QString s2,QString s3)
{
	UINT a;
	QString s,name,progr_name,base_name,otl;
	char *p2;
	Srom *p1;

	p1=lrom;
	s=s3;
	int in=s.lastIndexOf(':');
	if(in==(-1)) {name=s;base_name=s;progr_name="";}
	else 
	{
		progr_name=s.left(in+1);
		base_name=s.right(s.count() - in - 1);
		base_name = base_name.trimmed();
	}
	name=base_name;
	int tmp_in1 = base_name.indexOf('.');
	int tmp_in2 = base_name.indexOf('[');
	if ((tmp_in1 == (-1)) && (tmp_in2 == (-1)))
		in = -1;
	else
	{
		if (tmp_in1 == (-1))
			in = tmp_in2;
		else
			if (tmp_in2 == (-1))
				in = tmp_in1;
			else
			if (tmp_in2 < tmp_in1)
				in = tmp_in2;
			else
				in = tmp_in1;

	}
	if(in==(-1)) base_name=name;
	else  
	base_name=base_name.left(in);
	progr_name=progr_name+base_name;
		if(s2.indexOf('/')==s2.indexOf('\\'))	//т.е. нет
		{
			if(AdrFromIdent( progr_name, M_adr, Me_im))
			{
			prmdier(Errs);
			prmdier("# невозможно определить адрес");
			return(1);
			}
			direct="print "+Me_im;
			in=direct.lastIndexOf(':');
			direct=direct.left(in+1)+name;
	a=	M_adr;
		}
		else
		{
	QChar ch=name.at(0);
	if(ch.isDigit())

	{prmdier(Errs); return(1);}



	if(NomToAdr( progr_name, a)) 
	{prmdier(Errs); return(1);}
		}
	prmdi(direct);

	p1=p1+mul_ind[0];
	OnType(a,p1,(char*)name.toStdString().c_str());	// это 
	return(0);
}

	/////////////////////////////////////////////////////////////////
void	OnDisplay(void)	// In windows Display registers and memory
{
	//if(pDr)
	//{
	//		::SendMessage(hDr,WM_MDI8,1,0);//registers
	//}
	//		if(pDglob)
	//		{
	//			::SendMessage(hDg,WM_MDI10,1,0);//globals
	//}
	//		
	//if(pMem)
	//{
	//		::SendMessage(hDmem,WM_MDI101,1,0);//memory
	//}

}
///////////////////////////////////////////////////////////////////
void	OnDisLoc(void)	// In windows Display locals
{
	//UINT adr=0;
	//if(pDl==NULL) return;
	//if(pDl)
	//		::SendMessage(hDl,WM_MDI18,adr,0);
}
///////////////////////////////////////////////////////////////////
//запись части ОЗУ целевой машины  в бинарный файл, s3-файл, s2-параметры:
// \<adr1>,<adr2> adr1,adr2- шестнадцатеричный,
// 
int OnDump(QString s2,QString s3)
{
	DWORD len=0;
	UINT adr=0,adr2=0;
	QFile file(s3);
	QString str1,otl,s2t;
	int in,i,it;
	union {
		UINT w;
		UCHAR c[4];
	}a;

	s2t=s2;
	if(!file.open(QIODevice::WriteOnly))
	{
		out1.sprintf("# Ошибка открытия файла %s",s3);
		prmdier(out1);
		  return(1);
	}
	
	s2.remove(0, 1);
	in=s2.indexOf(',');
	if(in==(-1))
	{
	file.close();
	prmdier("# Ошибка синтаксиса"); return(1);
	}
	if(in!=(-1))
	{
		str1=s2.left(in);
		str1="0X"+str1;
		adr=fpere(str1);
		s2 = s2.right(s2.count() - in - 1);
	}
	if(s2=="")
	{
	file.close();
	prmdier("# Ошибка синтаксиса"); return(1);
	}

	if(s2!="")
	{
			str1=s2;
			s2=""; str1="0X"+str1;
			adr2=fpere(str1);
			adr=adr&0xFFFFFFFC;adr2=adr2&0xFFFFFFFC;
			len=adr2-adr;
		}
			if(adr2<=adr)
			{
		file.close();
		prmdier("# Ошибка синтаксиса"); return(1);
		}

			for(;len>0;len-=4)
			{
			for(it=0;it<4;it++)
				readb6(adr+it,(short)TargetVM,a.c[it]);
			adr+=4;
				file.write((char*)&a.c,4);
			}
			file.close();
			otl="dump "+s2t+" "+s3; prmdi(otl);
			return(0);
	}
/////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
//запись файла в ОЗУ целевой машины, s3-файл, s2-параметры:
// \<adr>[,<offset>[,<col>]] adr,offset- шестнадцатеричные,
// col-любое
int OnWrite(QString s2,QString s3)
{
	LONG offset;
	DWORD len=0,lenf;
	UINT adr;
	QFile file(s3);
	QString str1,otl;
	int in,i;

	if(!file.open(QIODevice::ReadWrite))
	{
		out1.sprintf("# Ошибка открытия файла %s",s3);
		prmdier(out1);
		  return(1);
	}
		lenf=file.size();
	adr=0;  offset=0;
	s2.remove(0, 1);
	in=s2.indexOf(',');
	if(in==(-1))
	{
		str1=s2; s2="";
		str1="0X"+str1;
		adr=fpere(str1);
	len=file.size();
	}
	if(in!=(-1))
	{
		str1=s2.left(in);
		str1="0X"+str1;
		adr=fpere(str1);
		s2 = s2.right(s2.count() - in - 1);
		len=file.size();
	}
	if(s2!="")
	{
		in=s2.indexOf(',');
		if(in==(-1))
		{
			str1=s2;
			s2=""; str1="0X"+str1;
			offset=(LONG)fpere(str1); len=lenf-offset;
		}
		if(in!=(-1))
		{
			str1=s2.left(in);
			str1="0X"+str1; offset=fpere(str1);
			s2 = s2.right(s2.count() - in - 1);
			len=lenf-offset;
		}
	}
	if(s2!="")
	{
		len=fpere(s2);
		if((lenf-offset)<len) len=lenf-offset;
	}
	/////// чтение и запись

	file.seek(offset);
	UINT lt=len;
	if((int)len<=0)
	{
		prmdier("# ошибка в параметрах"); return(1);
	}
	prmdi(direct);
	for(;lt!=0;)
	{
		if(lt>512) { len=512; lt=lt-512;}
		else { len=lt; lt=0;}
		file.read((char*)bufch,len);
		
	int	b=WBufCom(adr,len,bufch);
		if(b)
		{
		file.close(); return(1);
	
		}
	adr=adr+len;
	}
		file.close(); return(0);
}
//////////////////////////////////////////////////////
void print_time(void)
{
	DWORDLONG t;
	UINT rab,hou,min,sek,ms,mks;
	QString s;

	hou=0; min=0;sek=0;ms=0; mks=0;
	t=wrrom();t=t-mytime;

	hou=(UINT)(t/3600000000);
	rab=(UINT)(t%3600000000);
	min=rab/60000000;
	rab=rab%60000000;
	sek=rab/1000000;
	rab=rab%1000000;
	ms=rab/1000;
	mks=rab%1000;

	s.sprintf("# Системное время %d ч %d мин %d с %d мс %d мкс",
		hou,min,sek,ms,mks);
	prmdi(s);
}
////////////////////////
UINT GetAir(QString str)
{
	QChar ch;

	ch=str.at(0);
	if(ch.isDigit()) return(4);
	if((ch=='c')||(ch=='b')) return(2);
	if(ch=='h') return(9);
	if(ch=='w') return(4);
	if(ch=='f') return(12);
	if(ch=='g') return(13);
	return(4);

}
int		WriteCP1(int nom,UCHAR* dat)
{
	int i;
	
	for(i=1;i<=kolwm;i++)
	{
		if(VektVM[i]==0) continue;
		writeCP1(nom,(short)i,dat);
	}
	return 0;
}
/////////////
int	WriteCP0(int nom,UCHAR* dat)
{
	int i;
	
	for(i=1;i<=kolwm;i++)
	{
		if(VektVM[i]==0) continue;
		writeCP0(nom,(short)i,dat);
	}
return 0;
}
////////////////////////////////////////
// поток опроса ВМ
void Opros(void)
{
	printkl("Opros started");
	UINT NatStop;
	int i,iVM;
	QString s,name,otl;
	UINT wibcom,wibdat;
	for(;;)
	{
		OnDisplay();
				WaitForSingleObject(eOpros, INFINITE);
	  	 ResetEvent(eOpros);
	NaturalStop=0;
	NatStop=0;
	ForGo=0;
		 for(;;)
		 {
 			 ForPrint(0);
			 if(potkl==0){Stopli=1; break;}
			 Stopli=0;
			 Sleep(3);
			 continue;
		 }
		 for(i=0;i<=36;i+=6)
		 {
	Sleep(1);
			 if(otwrom[i]==0) {  ForGo=1;NaturalStop=NatStop;break;}
			if(otwrom[i]&04)	// останов оператора
			{
				print_time();
				prmdi("# Операторский останов");
				MestOst(TargetVM);//!!!!!!!!!!!!!!!!!!!!
						NatStop=1;//continue;//break;
			}
			if(otwrom[i]&040)	// Reset
			{
				print_time();
				prmdi("# Выполнен Reset");
				SetEvent(eReset);//continue;//
	 ForGo=1;
			}
			if(otwrom[i]&0700)	// останов по записи или чтению
			{
				iVM=otwrom[i+5];
				wibcom=otwrom[i+2];wibdat=otwrom[i+4];
				if(otwrom[i]&0x40)
				{
					printOst(otwrom[i],iVM,wibcom,0,0);
					NatStop=1;
				}
				if(otwrom[i]&0600)
				{
					printOst(otwrom[i],iVM,wibdat,1,otwrom[i+3]);
					NatStop=1;
				}
			}
			if(otwrom[i]&01000)	// исключение
			{

				wibcom=otwrom[i+1];
				iVM=otwrom[i+5];
					printIskl(iVM,wibcom);	
					NatStop=1;

			}
			
			continue;
				
				break;
		}
	}//
}
//////////////////////////////////////////////////
void MestOst(int targ)
{
	UINT adr;
	union
	{
		UINT adr;
		UCHAR ch[4];
	}d;
	QString otl,str1,sm,s1,str;
	int i;
		char* pch;

	readCP0(39,(short)targ,&d.ch[0]);	//read PC
	adr=d.adr;
	for(i=0;i<4;i++)
	{
		readb6(adr+i,(short)targ,d.ch[i]);
	}

		if(prtab) pch=Disass((unsigned long int)d.adr,(unsigned long int)adr,0);
	else pch=Disass((unsigned long int)d.adr,(unsigned long int)adr,1);
	str1=(const char*)pch;
	MetFromAdr( adr, sm);
	if(sm!="") sm=sm+":";
	if(!LineFromAdr(adr,s1))
		{
		SelStrBreak(s1);
		}
	str.sprintf("# %08X %08X %s\t",adr,d.adr,sm); str=str+str1;
	prmdi(str);

}
//////////////////////
// pr=0 останов по выборке команды
// pr=1 останов по данным
void printOst(UINT otw,UINT iVM,UINT wibcom,int pr,UINT pc)
{
	int i;
	UINT adr;
	union
	{
		UINT adr;
		UCHAR ch[4];
	}d={0};
	QString nom,otl,str1,sm,s1,prost,str,name;
		char* pch;
		int dpr;
		UINT dadr;
		QString ds;
		nom.sprintf("#ВМ %d:",iVM-1);
			OnDisplay();	OnDisLoc();// In windows Display registers and memory


	if(pr==0)	//выборка команды
	{
	
		adr=(pBP[iVM]+wibcom)->adr;
		adresost[iVM]=adr;
		name="";
		if(((pBP[iVM]+wibcom)->pr)&010)	//есть строка
		{
			ds=*(p_ms[iVM]+wibcom);
			name=*(p_ms[iVM]+wibcom);
			mestost[iVM]=name;
		}
			str1.sprintf("Останов по выборке команды : адрес %08X ",adr);
		str1=nom+str1;
		if(((pBP[iVM]+wibcom)->pr)&010)	//есть строка
		str1=str1+name;
		prmdi(str1);
		if(((pBP[iVM]+wibcom)->pr)&04)	//tStop
		{
			dpr=(pBP[iVM]+wibcom)->pr;
			dadr=(pBP[iVM]+wibcom)->adr;

			(pBP[iVM]+wibcom)->pr=0;

			TrimBreak(iVM);
	DelRed(dpr,dadr,ds);
		}
	for(i=0;i<4;i++)
	{
		readb6(adr+i,(short)iVM,d.ch[i]);
	}

		
		if(prtab) pch=Disass((unsigned long int)d.adr,(unsigned long int)adr,0);
	else pch=Disass((unsigned long int)d.adr,(unsigned long int)adr,1);
	str1=(const char*)pch;
	MetFromAdr( adr, sm);
	if(sm!="") sm=sm+":";
	if(!LineFromAdr(adr,s1))
		{
		SelStrBreak(s1);//
		}
	str.sprintf("# %08X %08X %s\t",adr,d.adr,sm);
				print_time();
	str=str+str1;
	prmdi(str);
	prmdi("      ");
	}
	
	if(pr)	//работа с данными
	{
		if(otw&0400)	//останов по записи
			prost=" останов по записи ";
		else prost=" останов по чтению ";
	
		adr=(pBP[iVM]+wibcom)->adr;
		adresost[iVM]=adr;
		name="";
		if(((pBP[iVM]+wibcom)->pr)&010)	//есть строка
		{
			name=*(p_ms[iVM]+wibcom);
			mestost[iVM]=name;
		}
		str1.sprintf(": адрес %08X ",adr);
		str1=nom+prost+str1;
		if(((pBP[iVM]+wibcom)->pr)&010)	//есть строка
		str1=str1+name;
		prmdi(str1);

		for(i=0;i<4;i++)
		{
			readb6(adr+i,(short)iVM,d.ch[i]);
		}
		str1.sprintf("# Данные  0x%08X",d.adr);
		prmdi(str1);
		if(((pBP[iVM]+wibcom)->pr)&04)	//tStop
		{
			(pBP[iVM]+wibcom)->pr=0;
			TrimBreak(iVM);
		}
	adr=pc;
		for(i=0;i<4;i++)
	{
		readb6(adr+i,(short)iVM,d.ch[i]);
	}

	if(!LineFromAdr(adr,s1))
		{
		SelStrBreak(s1);//
		}
		
		if(prtab) pch=Disass((unsigned long int)d.adr,(unsigned long int)adr,0);
	else pch=Disass((unsigned long int)d.adr,(unsigned long int)adr,1);
	str1=(const char*)pch;
	MetFromAdr( adr, sm);
	if(sm!="") sm=sm+":";
	str.sprintf("# %08X %08X %s\t",adr,d.adr,sm);
	str=nom+str+str1;
				print_time();
	prmdi(str);
	prmdi("     ");
	}
}

//////////////////////////////////////////////////////
void printIskl(UINT iVM,UINT adri)
{
	int i;
	UINT cause,com;
	union
	{
		UINT adr;
		UCHAR ch[4];
	}d;
	QString nom,otl,str1,sm,s1,prost,str,name;
		char* pch;
		prmdi("#ИСКЛЮЧЕНИЕ");
	return;


		readCP0(37,(short)iVM,&d.ch[0]);
		cause=d.adr;
		for (i=0;i<4;i++)
			readb6(adri+i,(short)iVM,d.ch[i]);
		com=d.adr;
		nom.sprintf("# BM %d ",iVM);
			
	if((cause & 0174)!=0)
	{
		UINT a=cause; a=a & 0174; a=a>>2;
		str.sprintf("Останов, ExcCod=%02X",a);
			if(a<=12) str=str+" - "+NameExcCode[a];
	str=nom+str;
		prmdier(str);
	}


		
	if(prtab) pch=Disass((unsigned long int)com,(unsigned long int)adri,0);
	else pch=Disass((unsigned long int)com,(unsigned long int)adri,1);
	str1=(const char*)pch;
	MetFromAdr( adri, sm);
	if(sm!="") sm=sm+":";
	str.sprintf("# %08X %08X %s\t",adri,com,sm);
	str=nom+str+str1;
				print_time();
	prmdi(str);

}
/////////////////////////////////////
void ForPrint(UINT a)
{
	UINT priz[7]={0};

	QString otl,otl1;
	union
	{
		UINT w;
		UCHAR ch[4];
	}d={0};

	int iVM,i,pr,rrn;
	pr=0;

	for(iVM=01;iVM<=kolwm;iVM++)
	{
		for(i=0;i<=3;i++)
		readb6(adr_mail+adr_contact+i,(short)iVM,d.ch[i]);
		if(d.w==mk_print)
		{pr=1;
		break;}
	}

	if(pr==0) return;
	if(pr)	//это печать
		// stop
	{
	//дополнительный анализ всех ВМ

	for(iVM=01;iVM<=kolwm;iVM++)
	{
		for(i=0;i<=3;i++)
		readb6(adr_mail+adr_contact+i,(short)iVM,d.ch[i]);
		if(d.w==mk_print)
		{priz[iVM]=1;}else priz[iVM]=0;
	}
	//подготовка строк для печати
		UINT adrp=0,lp=0;
		QString ssr="";
	for(iVM=1;iVM<=kolwm;iVM++)
	{
		if (priz[iVM]==0) continue;
		for(i=0;i<=3;i++)
		readb6(adr_mail+adr_maila+i,(short)iVM,d.ch[i]);
		adrp=d.w;
		for(i=0;i<=3;i++)
		readb6(adr_mail+adr_maill+i,(short)iVM,d.ch[i]);
		lp=d.w;
		if(lp>=7000) prmdier("# Error in printf_tsn");
		//чтение строки

		for (i=0;i<(int)lp;i++)
		readb6(adrp+i,(short)iVM,buf_print[i]);
		buf_print[i]=0;
		ssr=(char*)buf_print;
		tip[iVM]+=ssr;
		rrn=0;
		QString forp;

		for(;;)
	{
		int rr=tip[iVM].indexOf('\n');
		if(rr==(-1))
		{
			forp=tip[iVM].right(7000);
				tip[iVM]="";
		otl.sprintf("# BM%d",iVM-1); 
//		forp.TrimLeft();
		forp = forp.trimmed();
		if(forp!="") 
		{
			forp=otl+": "+forp;
				prmdi(forp);
		}
				break;
		}
		if(rr!=(-1))
		{
						tip[iVM].replace(rr,1,' ');

			forp=tip[iVM].mid(rrn,rr-rrn);
						rrn=rr;
		otl=tip[iVM].mid(rr,7000);tip[iVM]=otl;
		otl.sprintf("# BM%d",iVM-1); 
//		forp.TrimLeft();
		forp = forp.trimmed();
		if(forp!="") 
		{
		forp=otl+": "+forp;
		prmdi(forp);
		}
		rrn=0;
		}
	}
		tip[iVM]="";
	}
		//write gop
		d.w=mk_gop;
		for(iVM=1;iVM<=kolwm;iVM++)
		{
			if(priz[iVM]==0) continue;
			for (i=0;i<=3;i++)
			writeb6(adr_mail+adr_contact+i,(short)iVM,d.ch[i]);
		}
		if(a==0)
		{
	return;
		}
	}
}
////////////////////////////////////
int		OnPrintForGetadr(QString s2,QString s3)
{
	UINT a;
	QString s,name,progr_name,base_name,otl;
	char *p2;
	Srom *p1;

	p1=lrom;
	s=s3;
	int in=s.lastIndexOf(':');
	if(in==(-1)) {name=s;base_name=s;progr_name="";}
	else 
	{
		progr_name=s.left(in+1);
		base_name=s;
		base_name = s.right(s.count() - in - 1);
		base_name = base_name.trimmed();
	}
	name=base_name;
	int tmp_in1 = base_name.indexOf('.');
	int tmp_in2 = base_name.indexOf('[');
	if ((tmp_in1 == (-1)) && (tmp_in2 == (-1)))
		in = -1;
	else
	{
		if (tmp_in1 == (-1))
			in = tmp_in2;
		else
			if (tmp_in2 == (-1))
				in = tmp_in1;
			else
			if (tmp_in2 < tmp_in1)
				in = tmp_in2;
			else
				in = tmp_in1;

	}
	if(in==(-1)) base_name=name;
	else  
	base_name=base_name.left(in);
	progr_name=progr_name+base_name;
		if(s2.indexOf('/')==s2.indexOf('\\'))	//т.е. нет
		{
			if(AdrFromIdent( progr_name, M_adr, Me_im))
			{
			prmdier(Errs);
			prmdier("# невозможно определить адрес");
			return(1);
			}
			direct="GetAdr "+Me_im;
			in=direct.lastIndexOf(':');
			direct=direct.left(in+1)+name;
	p1=p1+mul_ind[0];
	a=	M_adr;
	prmdi(direct);
	OnTypeForGetadr(a,p1,(char*)name.toStdString().c_str());	// это 
	return  (0);
		}
		else
		{
	QChar ch=name.at(0);
	if(ch.isDigit())

	{
	if( NomToAdr( s2, bpadr))	/// определение адреса по номеру строки

	{
		prmdier(Errs);
		prmdier("# невозможно определить адрес");
	return 1;
		//		continue;
	}
	otl.sprintf("%08X",bpadr);
	M_adr=bpadr;
	for(iVM=1;iVM<=kolwm;iVM++)
	{
		if(iVM==1) gruppa1[0]=M_adr;
		if(iVM==2) gruppa2[0]=M_adr;
		if(iVM==3) gruppa3[0]=M_adr;
		if(iVM==4) gruppa4[0]=M_adr;
		if(iVM==5) gruppa5[0]=M_adr;
		if(iVM==6) gruppa6[0]=M_adr;
	}
	prmdi(direct);
	otl="# Адрес="+otl;
	prmdi(otl);

	
	}

	else
	{

	if(NomToAdr( progr_name, a)) 

	{prmdier(Errs); return(1);}
		
	prmdi(direct);

	p1=p1+mul_ind[0];
	OnTypeForGetadr(a,p1,(char*)name.toStdString().c_str());	// это 
		
		}
		}
		return(0);
}
//////////////////////////////////////////////////////////////////
void OnTypeT(QString s)
{
QString s1,s3,s2,str,str1;
UINT adr=0,a;
	QString name,progr_name,base_name,otl;
	char *p2;
	Srom *p1;



	s1=s; 
	s1 = s1.trimmed();
			if(s1.at(0)=='*')//
			{
				s1.remove(0, 1);
			s1="0x"+s1;//
						adr=fpere(s1);//
	prmdi(direct);
			}
			else
			{
	p1=lrom;
	s3=s;
	s=s3;
	int in=s.lastIndexOf(':');
	if(in==(-1)) {name=s;base_name=s;progr_name="";}
	else 
	{
		progr_name=s.left(in+1);
		base_name=s;
		base_name = base_name.right(base_name.count() - in - 1);

		base_name = base_name.trimmed();
	}
	name=base_name;
	int tmp_in1 = base_name.indexOf('.');
	int tmp_in2 = base_name.indexOf('[');
	if ((tmp_in1 == (-1)) && (tmp_in2 == (-1)))
		in = -1;
	else
	{
		if (tmp_in1 == (-1))
			in = tmp_in2;
		else
			if (tmp_in2 == (-1))
				in = tmp_in1;
			else
			if (tmp_in2 < tmp_in1)
				in = tmp_in2;
			else
				in = tmp_in1;

	}
	if(in==(-1)) base_name=name;
	else  
	base_name=base_name.left(in);
	progr_name=progr_name+base_name;
		if(s1.indexOf('/')==s1.indexOf('\\'))	//т.е. нет
		{
			if(AdrFromIdent( progr_name, M_adr, Me_im))
			{
			prmdier(Errs);
			prmdier("# невозможно определить адрес");
			return;
			}
			direct="TYPE "+Me_im;
			in=direct.lastIndexOf(':');
			direct=direct.left(in+1)+name;
	
	p1=p1+mul_ind[0];
	a=	M_adr;
	OnTypeForGetadrT(a,p1,(char*)name.toStdString().c_str());	// это 
	prmdi(direct);
		}
		else
		{
	QChar ch=name.at(0);
	if(ch.isDigit())
	{
	if( NomToAdr( s1, bpadr))	/// определение адреса по номеру строки

	{
		prmdier(Errs);
		prmdier("# невозможно определить адрес");
	return;
	}
	otl.sprintf("%08X",bpadr);
	M_adr=bpadr;
	for(iVM=1;iVM<=kolwm;iVM++)
	{
		if(iVM==1) gruppa1[0]=M_adr;
		if(iVM==2) gruppa2[0]=M_adr;
		if(iVM==3) gruppa3[0]=M_adr;
		if(iVM==4) gruppa4[0]=M_adr;
	}
	}

	else
	{

	if(NomToAdr( progr_name, a)) 

	{prmdier(Errs); return;}
		
	prmdi(direct);

	
	p1=p1+mul_ind[0];
	OnTypeForGetadrT(a,p1,(char*)name.toStdString().c_str());	// это 
		
		}
			}
			for(iVM=1;iVM<=kolwm;iVM++)
	{
		if(iVM==1) adr=gruppa1[0];break;
		if(iVM==2) adr=gruppa2[0];break;
		if(iVM==3) adr=gruppa3[0];break;
		if(iVM==4) adr=gruppa4[0];break;
	}

			}
	UCHAR *pgr=0;
	int n=64,j;
	for(iVM=1;iVM<=kolwm;iVM++)
	{
		if(VektVM[iVM]==0) continue;
		for(i=0;i<(int)n;i++)
		{
			readb6(adr+i,(short)iVM,bufx.ch[i]);
		}
	if(iVM==1) pgr=(UCHAR*)&gruppa1[0];
	if(iVM==2) pgr=(UCHAR*)&gruppa2[0];
	if(iVM==3) pgr=(UCHAR*)&gruppa3[0];
	if(iVM==4) pgr=(UCHAR*)&gruppa4[0];
	for(j=0;j<(int)n;j++)
	{
		*(pgr+j)=bufx.ch[j];
	}

	bufx.ch[n-1]=0;
	
	
	str.sprintf("#ВМ%d: ", iVM - 1);
		str1=QString((char*)(&bufx.ch[0]));
	str=str+str1;
		printkl(str);
	
	}
}

//********* Model's DLL Loading *********************************
static long int load_pkm_dll_2(QString str)
{
	static HINSTANCE hInstDll2;

	hInstDll2 = LoadLibrary(str.toStdWString().c_str());
	if (hInstDll2 == NULL)
	{
printkl ("#E - ZAKR - ERROR - PKMMonitor- Can't load PKM.DLL");
// ::MessageBox(NULL,"Can't load PKM.DLL","DLL", MB_OK);
		return 0;
    }

	typedef struct mod_table* (*ffp)(reg_table *);
	ffp InitPKM = (ffp)GetProcAddress(hInstDll2,"init_pkm");
	if (InitPKM == NULL)
	{
printkl ("#E - ZAKR - ERROR - PKMMonitor- Can't find init_pkm in pkm.dll");
		//::MessageBox(NULL,"Can't find init_pkm","DLL", MB_OK);
		return 0;
    }
	pModTable =(*InitPKM)(pRegTable);

	return 1;
}
