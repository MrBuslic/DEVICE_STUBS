#include "PKM.h"

extern    short writeb6(UINT adr,short nom,UCHAR b); 
extern UINT mk_gop;
extern QString tip[];
extern UCHAR buf_print[];
extern UINT adr_maila;
extern UINT adr_maill;
extern HANDLE eOpros,eReset ;
extern UINT mk_print;
   extern short kolwm;    //количество машин от 1 до kolwm.
extern  void ForPrint(UINT);
extern  void print_time(void);
extern   short readb6(UINT adr,short nom,UCHAR &b);
extern short readCP0(int nr,short nom,UCHAR *a);
extern short writeCP0(int nr,short nom,UCHAR *a);
extern short readCP1(int nr,short nom,UCHAR *a);
extern short writeCP1(int nr,short nom,UCHAR *a);
extern HANDLE eProz ;
extern    UINT zakrom[];    //Заказ
extern    UINT otwrom[];   //Ответ  / шестерки.
extern    short potkl;       //Признак,  сплю(0)-работаю(1).
extern      UINT      adresost[];       //Адрес останова /есть всегда.
extern      QString   mestost[];        //Текст, описывающий место останова.
  extern UINT TargetVM;
 extern void	fZakaz(int z,int targ);	//contin
extern int MetFromAdr(UINT epc,QString& sm);
extern UINT adr_mail;
//extern UINT adr_regs;
//extern UINT adr_pc;
//extern UINT adr_cause;
//extern UINT adr_fregs;
extern UINT adr_contact;
extern int LineFromAdr(UINT adr, QString &str);
//extern UINT ReadWord(UINT adr);
extern void SelStrBreak(QString str);
extern int OnFindDiap(UINT adr,UINT &adr_ns,UINT& adr_ks);
extern int WdatAdr(UINT adr, UINT dat);
//extern UINT K_BREAK;
extern int fjmp(QString adrs, int pr);
extern char* Disass(unsigned long int cod,unsigned long int epc,unsigned short pr);
extern int prtab;
extern void prmdi(QString);
extern void prmdier(QString);
extern      UINT      konez;          //=7, если ЦМ завершила программу.

UINT st1[]={0x08000000, 0x0c000000,0};
UINT st2[]={0x00000008, 0x00000009,0};
UINT st3[]={0x10000000, 0x1c000000, 0x18000000, 0x14000000, 0};
UINT st4[]={0x04010000, 0x04110000, 0x04000000, 0x04100000, 0};
UINT st5[]={0x45000000, 0};

UINT next1[]={0x08000000, 0};
UINT next2[]={0x00000008, 0};
UINT next3[]={0x10000000, 0x1c000000, 0x18000000, 0x14000000, 0};
UINT next4[]={0x04010000, 0x04000000, 0};
UINT next5[]={0x45000000, 0};

void fnexti(int);
void fnext(void);
void fstep(void);
void fstepi(int);
int step_tag(UINT& nadr, UINT& ncom);
void ForPrintNext(UINT a);

///////////////////////////////////
// шаг и вывод в окно протокола
void fstepi(int pr)
{
	union
	{
		UINT adr;
		UCHAR ch[4];
	}d;
	int i;
	UINT adr;
	QString str,str1,sm,s1,otl;
	char* pch;

			fZakaz(01,TargetVM);
	SetEvent(eProz); //Sleep(100);
	for(;;)
	{
	Sleep(2);
		if(potkl==0) break;
	}

	readCP0(39,(short)TargetVM,&d.ch[0]);	//read PC
	adr=d.adr;


		for(i=0;i<4;i++)
	{
		readb6(adr+i,(short)TargetVM,d.ch[i]);
	}
		if(prtab) pch=Disass((unsigned long int)d.adr,(unsigned long int)adr,0);
	else pch=Disass((unsigned long int)d.adr,(unsigned long int)adr,1);
	str1=(const char*)pch;
	MetFromAdr( adr, sm);
	if(sm!="") sm=sm+":";
	if(pr==0)
	{
	if(!LineFromAdr(adr,s1))
		{
		SelStrBreak(s1);
		}
	str.sprintf("# %08X %08X %s\t",adr,d.adr,sm); str="stepi "+str+str1;
	prmdi(str);

	ForPrint(1);// печать
}
  }

///////////////////////////////////////////////////////////////////////////////
void fnexti(int pr)
{
	union
	{
		UINT adr;
		UCHAR ch[4];
	}d={0};
	int i,iVM,pr_print=0;
	UINT adr;
	QString str,str1,sm,s1,otl;
	char* pch;

			fZakaz(02,TargetVM);
	SetEvent(eProz);// Sleep(100);
	for(;;)
	{
		Sleep(1);
		if(potkl==0) break;
			for(iVM=01;iVM<=kolwm;iVM++)
	{
		for(i=0;i<=3;i++)
		readb6(adr_mail+adr_contact+i,(short)iVM,d.ch[i]);
		if(d.adr==mk_print)
		{
			pr_print=1;break;
		}
	}
			if(pr_print==1)
		{Sleep(6);
		break;}

	}

	if(potkl)
	{
	ForPrintNext(0);
		SetEvent(eOpros);
	return;
	}

	readCP0(39,(short)TargetVM,&d.ch[0]);	//read PC
	adr=d.adr;
		for(i=0;i<4;i++)
	{
		readb6(adr+i,(short)TargetVM,d.ch[i]);
	}

		if(prtab) pch=Disass((unsigned long int)d.adr,(unsigned long int)adr,0);
	else pch=Disass((unsigned long int)d.adr,(unsigned long int)adr,1);
	str1=(const char*)pch;
	MetFromAdr( adr, sm);
	if(sm!="") sm=sm+":";
	if(pr==0)
	{
			if(!LineFromAdr(adr,s1))
	{
		SelStrBreak(s1);
		}
	str.sprintf("# %08X %08X %s\t",adr,d.adr,sm); 
	str="nexti "+str+str1;
	prmdi(str);
	for(;;)
	{Sleep(1);
		if(potkl==0) 
			break;

	}
	Sleep(5);
	if(potkl==0)	ForPrint(1);
	}
}
////////////////////////////////////////////
void fstep(void)
{
	UINT adr,adrn,adrk,adrt;
	int a;
	union
	{
		UINT adr;
		UCHAR ch[4];
	}d;
	QString str,str1,sm,s1,otl;

	readCP0(39,(short)TargetVM,&d.ch[0]);	//read PC
	adr=d.adr;

	a=OnFindDiap(adr,adrn,adrk);
	if(a)
	{
		prmdier("step # нет отладочной информации");
		return;
	}
	for(;;)
	{
	fstepi(1);
	readCP0(39,(short)TargetVM,&d.ch[0]);	//read PC
	adrt=d.adr;
		if((adrt<adrn) || (adrt>=adrk)) break;
	}
		if(!LineFromAdr(adrt,s1))
		{
			otl.sprintf("step # адрес=%08X %s",adrt,s1);
			prmdi(otl);
			SelStrBreak(s1);
		}
		else
		{
			otl.sprintf("step # адрес=%08X",adrt);
			prmdi(otl);
			
		}

		return;
}
////////////////
void fnext(void)

{
	QString s1,otl;
	UINT adr,adrn,adrk,adrt;
	int a;

	union
	{
		UINT adr;
		UCHAR ch[4];
	}d;
	QString str,str1,sm;
	readCP0(39,(short)TargetVM,&d.ch[0]);	//read PC
	adr=d.adr;
	a=OnFindDiap(adr,adrn,adrk);
	if(a)
	{
		prmdier("next # нет отладочной информации");
		return;
	}
	for(;;)
	{
	fnexti(1);
	readCP0(39,(short)TargetVM,&d.ch[0]);	//read PC
	adrt=d.adr;
		if((adrt<adrn) || (adrt>=adrk)) break;
	}
		if(!LineFromAdr(adrt,s1))
		{
			otl.sprintf("next # адрес=%08X %s",adrt,s1);
			prmdi(otl);
			SelStrBreak(s1);
		}
		else
		{
			otl.sprintf("next # адрес=%08X",adrt);
			prmdi(otl);
			
		}
		return;
}
/////////////////////////////////////
void ForPrintNext(UINT )
{
	UINT priz[7]={0};

	QString otl,otl1;
	union
	{
		UINT w;
		UCHAR ch[4];
	}d={0};

	int iVM,i,pr;
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
	int	rrn=0;
		QString forp;
		for(;;)
	{
		int rr=tip[iVM].indexOf('\n');
		if(rr==(-1))
		{
			forp=tip[iVM].right(7000);

				tip[iVM]="";
		otl.sprintf("# BM%d",iVM-1); 
		forp=otl+": "+forp;prmdi(forp);
				break;
		}
		if(rr!=(-1))
		{

						tip[iVM].replace(rr,1,' ');
			forp=tip[iVM].mid(rrn,rr-rrn);
						rrn=rr;
				otl.sprintf("# BM%d",iVM-1); 
		forp=otl+": "+forp;prmdi(forp);
		otl=tip[iVM].mid(rr,7000);tip[iVM]=otl;
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
	}

}