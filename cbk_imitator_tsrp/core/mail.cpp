
#include "PKM.h"



#define WM_MDI2                      WM_APP+2
#define WM_MDI3                      WM_APP+3
#define WM_MDI5                      WM_APP+5
#define WM_MDI191                      WM_APP+191
#define WM_MDI38                      WM_APP+38



//extern      unsigned int      adresost;       //Адрес останова /есть всегда.
//extern      QString   mestost;        //Текст, описывающий место останова.
extern      UINT      konez;          //=7, если ЦМ завершила программу.
extern QString direct,Errs,strmdi,strForBr,strForDel;
extern DWORD OldFlag,NewFlag;
extern HWND hCom;
extern char* Disass(unsigned long int cod,unsigned long int epc,unsigned short pr);
 extern M_BP m_bp[];
extern  QStringList NameExcCode;
extern  QString a;
extern UINT baut;
extern int MetFromAdr(UINT epc,QString& sm);

extern void prmdi(QString);
//extern UINT ReadWord(UINT adr);
extern UCHAR bufpr[];
//extern void fchsum( UCHAR* buf);	// KS
//extern int WriteCom(int col,UCHAR* buf);
int ReadCom(int col,UCHAR* buf);
//extern      UINT      adresost;       //Адрес останова /есть всегда.
//extern      QString   mestost;        //Текст, описывающий место останова.
extern      UINT      konez;          //=7, если ЦМ завершила программу.
extern int LineFromAdr(UINT adr, QString &str);
extern   UINT      gruppa1[];   //Для чтения группы  чисел из ВМ1.
extern 	 UINT      gruppa2[];   //Для чтения группы  чисел из ВМ2.
extern 	 UINT      gruppa3[];   //Для чтения группы  чисел из ВМ3.
extern 	 UINT      gruppa4[];   //Для чтения группы  чисел из ВМ3.
extern 	 UINT      gruppa5[];   //Для чтения группы  чисел из ВМ3.
extern 	 UINT      gruppa6[];   //Для чтения группы  чисел из ВМ3.
extern short readCP0(int nr,short nom,UCHAR *a);
extern short readCP1(int nr,short nom,UCHAR *a);
extern UINT VektVM[];
extern short kolwm;    //количество машин от 1 до kolwm.
//extern HWND m_W;
extern  UCHAR bufper[];
extern      UINT      gruppa[];   //Для чтения группы чисел.
extern QString str_fors;
extern void prmdier(QString str);
extern QString Errs,direct,ms_bp[],out,out1;
//extern CBp *pBp;
//extern CLdbgApp theApp;
extern UINT ADRI;	// адрес следующей выполняемой команды 
UINT fpere(QString str);
extern HWND hBp;
extern BOOL m_ActivDMA;
extern BOOL m_ActivRESET;
UINT regs[40];
//UINT adr_mail=0xA0000100; //bc042000;
UINT adr_mail=0xA0000200; //bc042000;
//BOOL SOP=TRUE;
//UINT adr_regs=4;
//UINT adr_pc=128;
//UINT adr_cause=148;
//UINT adr_fregs=172;
UINT adr_contact=336;	//332;
UINT adr_maila=340;	//336;
UINT adr_maill=344;	//340;
int ReadRegs(UINT adr);
int VivRegs(QString s2,QString s3);
int PrintRegs(UINT adr, QStringList p);
int PrintFRegs(UINT adr, QStringList p);
void SelStrBreak(QString str);
QString str_fors;
//////////////////////
///// значения в contact
//UINT mk_ok=0x1234;
extern UINT mk_gop;
UINT mk_badexc=0x0BAD2345;
UINT mk_print=0x0BAD3452;
int prtab=0;
QStringList name_regs={
		"$at",		/* assembler temporary */
		"$v0",		/* return value 0 */
		"$v1",		/* return value 1 */
		"$a0",		/* argument 0 */
		"$a1",		/* argument 1 */
		"$a2",		/* argument 2 */
		"$a3",		/* argument 3 */
		"$t0",		/* caller saved 0 */
		"$t1",		/* caller saved 1 */
		"$t2",		/* caller saved 2 */
		"$t3",		/* caller saved 3 */
		"$t4",		/* caller saved 4 */
		"$t5",		/* caller saved 5 */
		"$t6",		/* caller saved 6 */
		"$t7",		/* caller saved 7 */
		"$s0",		/* callee saved 0 */
		"$s1",		/* callee saved 1 */
		"$s2",		/* callee saved 2 */
		"$s3",		/* callee saved 3 */
		"$s4",		/* callee saved 4 */
		"$s5",		/* callee saved 5 */
		"$s6",		/* callee saved 6 */
		"$s7",		/* callee saved 7 */
		"$t8",		/* code generator 0 */
		"$t9",		/* code generator 1 */
		"$k0",		/* kernel temporary 0 */
		"$k1",		/* kernel temporary 1 */
		"$gp",		/* global pointer */
		"$sp",		/* stack pointer */
		"$fp",		/* frame pointer */
		"$ra",		/* return address */
		"$EPC",		// EPC
		"$Status",		//Status
		"$lo",		//
		"$hi",
		"$BadVaddr",//BadVaddr
		"$Cause",	//Cause
		"$Config",	
		"$pc"		/* program counter */
};
QStringList name_regs_st={
		"$1",		/* assembler temporary */
		"$2",		/* return value 0 */
		"$3",		/* return value 1 */
		"$4",		/* argument 0 */
		"$5",		/* argument 1 */
		"$6",		/* argument 2 */
		"$7",		/* argument 3 */
		"$8",		/* caller saved 0 */
		"$9",		/* caller saved 1 */
		"$10",		/* caller saved 2 */
		"$11",		/* caller saved 3 */
		"$12",		/* caller saved 4 */
		"$13",		/* caller saved 5 */
		"$14",		/* caller saved 6 */
		"$15",		/* caller saved 7 */
		"$16",		/* callee saved 0 */
		"$17",		/* callee saved 1 */
		"$18",		/* callee saved 2 */
		"$19",		/* callee saved 3 */
		"$20",		/* callee saved 4 */
		"$21",		/* callee saved 5 */
		"$22",		/* callee saved 6 */
		"$23",		/* callee saved 7 */
		"$24",		/* code generator 0 */
		"$25",		/* code generator 1 */
		"$26",		/* kernel temporary 0 */
		"$27",		/* kernel temporary 1 */
		"$28",		/* global pointer */
		"$29",		/* stack pointer */
		"$30",		/* frame pointer */
		"$31",		/* return address */
		"$EPC",		// EPC
		"$Status",		//Status
		"$lo",		//
		"$hi",
		"$BadVaddr",//BadVaddr
		"$Cause",	//Cause
		"$Config",	
		"$pc"		/* program counter */

};
QStringList name_fregs={
		"$f0",		/* assembler temporary */
		"$f1",		/* assembler temporary */
		"$f2",		/* return value 0 */
		"$f3",		/* return value 1 */
		"$f4",		/* argument 0 */
		"$f5",		/* argument 1 */
		"$f6",		/* argument 2 */
		"$f7",		/* argument 3 */
		"$f8",		/* caller saved 0 */
		"$f9",		/* caller saved 1 */
		"$f10",		/* caller saved 2 */
		"$f11",		/* caller saved 3 */
		"$f12",		/* caller saved 4 */
		"$f13",		/* caller saved 5 */
		"$f14",		/* caller saved 6 */
		"$f15",		/* caller saved 7 */
		"$f16",		/* callee saved 0 */
		"$f17",		/* callee saved 1 */
		"$f18",		/* callee saved 2 */
		"$f19",		/* callee saved 3 */
		"$f20",		/* callee saved 4 */
		"$f21",		/* callee saved 5 */
		"$f22",		/* callee saved 6 */
		"$f23",		/* callee saved 7 */
		"$f24",		/* code generator 0 */
		"$f25",		/* code generator 1 */
		"$f26",		/* kernel temporary 0 */
		"$f27",		/* kernel temporary 1 */
		"$f28",		/* global pointer */
		"$f29",		/* stack pointer */
		"$f30",		/* frame pointer */
		"$f31",		/* return address */
		"$csr"
};
// int	AnalBreak(void);
int ReadRegsF(UINT iVM);
int ReadRegs(UINT adr);
int VivRegs(QString s2,QString s3);
int PrintRegs(UINT adr, QStringList p);
int PrintFRegs(UINT adr, QStringList p);
extern UINT j_buf; 
extern UINT nulwrem,swrem;
extern void print_time(void);

//extern UCHAR bufchar[]; 
////////////////////////////////
/////////////////////////////////////////////////////////////
///// печать регистров в Х-виде, adr в mail, таблица имен
int PrintRegs(UINT , QStringList p)
{
	QString s;
	int iVM;
	for (iVM=1;iVM<=kolwm;iVM++)
	{
		if(VektVM[iVM]==0) continue;
		out = QString("#------------- BM %1 --------------").arg(iVM-1);
		prmdi(out);
	if(ReadRegs((UINT)iVM)) return(1);

	for(int i=0;i<33;i=i+4)
	{
		out.sprintf("# %s=%08X, \t%s=%08X, \t%s=%08X, \t%s=%08X",
			p[i],regs[i],p[i+1],regs[i+1],p[i+2],regs[i+2],p[i+3],regs[i+3]);
		prmdi(out);
	}
	out.sprintf("# %s=%08X, \t%s=%08X \t%s=%08X",
			p[36],regs[36],p[37],regs[37],p[38],regs[38]);
		prmdi(out);
	}
return(0);
}
/////////////////////////////////////////////////////////////
///// печать f-регистров в Х-виде, adr в mail, таблица имен
int PrintFRegs(UINT , QStringList p)
{
	for (int iVM=1;iVM<=kolwm;iVM++)
	{
		if(VektVM[iVM]==0) continue;
		out.sprintf("#------------- BM %d --------------",iVM-1);
		prmdi(out);
	if(ReadRegsF(iVM)) return(1);

	for(int i=0;i<29;i=i+4)
	{
		out.sprintf("# %s=%08X, \t%s=%08X, \t%s=%08X, \t%s=%08X",
			p[i],regs[i],p[i+1],regs[i+1],p[i+2],regs[i+2],p[i+3],regs[i+3]);
		prmdi(out);
	}
	out.sprintf("# %s=%08X",
			p[32],regs[32]);
		prmdi(out);
	}	
return(0);
}
///////////////////////////////////////////
int ReadRegs(UINT iVM)
{
	QString adrs,ns;
	int i;

	for (i=0;i<=39;i++)
	{
		readCP0(i+1,(short)iVM,(UCHAR*)&regs[i]);
	}
	if(iVM==1)
	{
		for(i=0;i<=39;i++)
		{
			gruppa1[i]=regs[i];
		}
	}
	if(iVM==2)
	{
		for(i=0;i<=39;i++)
		{
			gruppa2[i]=regs[i];
		}
	}
	if(iVM==3)
	{
		for(i=0;i<=39;i++)
		{
			gruppa3[i]=regs[i];
		}
	}
	if(iVM==4)
	{
		for(i=0;i<=39;i++)
		{
			gruppa4[i]=regs[i];
		}
	}
	if(iVM==5)
	{
		for(i=0;i<=39;i++)
		{
			gruppa5[i]=regs[i];
		}
	}
	if(iVM==6)
	{
		for(i=0;i<=39;i++)
		{
			gruppa6[i]=regs[i];
		}
	}
  return(0);
}
///////////////////////////////////////////////////////////////
///////////////////////////////////////////
int ReadRegsF(UINT iVM)
{
	QString adrs,ns;
	int i;

	for (i=0;i<=32;i++)
	{
		readCP1(i,(short)iVM,(UCHAR*)&regs[i]);
	}
	if(iVM==1)
	{
		for(i=0;i<=32;i++)
		{
			gruppa1[i]=regs[i];
		}
	}
	if(iVM==2)
	{
		for(i=0;i<=32;i++)
		{
			gruppa2[i]=regs[i];
		}
	}
	if(iVM==3)
	{
		for(i=0;i<=32;i++)
		{
			gruppa3[i]=regs[i];
		}
	}
	if(iVM==4)
	{
		for(i=0;i<=32;i++)
		{
			gruppa4[i]=regs[i];
		}
	}
	if(iVM==5)
	{
		for(i=0;i<=32;i++)
		{
			gruppa5[i]=regs[i];
		}
	}
	if(iVM==6)
	{
		for(i=0;i<=32;i++)
		{
			gruppa6[i]=regs[i];
		}
	}
  return(0);
}
////////////////////////////////
int VivRegs(QString s2,QString s3)
{
	UINT adr=0,dat;
	int i,iVM;
	QString s;
	if(s3.isEmpty())	// Вывод всех регистров CP0
	{
		prmdi(direct);
		if(prtab)
		{
		PrintRegs(adr,name_regs_st); return(0);
		}
		else
		{
		PrintRegs(adr,name_regs); return(0);
		}
	}
	s2 = s2.toUpper();
	if(s2=="ALL")
	{
		prmdi(direct);
		if(prtab)
		{
		PrintRegs(adr,name_regs_st);
		}
		else
		{
		PrintRegs(adr,name_regs); 
		}
		PrintFRegs(adr,name_fregs);return(0); 
	}
	s3 = s3.toLower();
	QString lo;

	for(i=0;i<39;i++)
	{
		lo=name_regs[i];
		lo = lo.toLower();
		if(s3==lo)	//name_regs[i])
			break;
	}
	if(i!=39)
	{
		prmdi(direct);
		prtab=0;
		for(iVM=1;iVM<=kolwm;iVM++)
		{
			if(VektVM[iVM]==0) continue;
			s.sprintf("#BM %d: ",iVM-1);
			readCP0(i+1,(short)iVM,(UCHAR*)&dat);

		
		out.sprintf(" %s=%08X",name_regs[i], dat);
		out=s+out;
		prmdi(out);
		if(iVM==1) gruppa1[0]=dat;
		if(iVM==2) gruppa2[0]=dat;
		if(iVM==3) gruppa3[0]=dat;
		if(iVM==4) gruppa4[0]=dat;
//		if(iVM==5) gruppa5[0]=dat;
//		if(iVM==6) gruppa6[0]=dat;
		}
		return(0);
	}
	///////////////////////////////////////////
	for(i=0;i<39;i++)
	{
	lo=name_regs_st[i];
	lo = lo.toLower();
		if(s3==lo)	//name_regs_st[i])
			break;
	}
	if(i!=39)
	{
		prmdi(direct);
		prtab=1;
		for(iVM=1;iVM<=kolwm;iVM++)
		{
			if(VektVM[iVM]==0) continue;
			s.sprintf("#BM %d: ",iVM-1);
			readCP0(i+1,(short)iVM,(UCHAR*)&dat);
		out.sprintf(" %s=%08X",name_regs_st[i], dat);
		out=s+out;
		prmdi(out);
		if(iVM==1) gruppa1[0]=dat;
		if(iVM==2) gruppa2[0]=dat;
		if(iVM==3) gruppa3[0]=dat;
		if(iVM==4) gruppa4[0]=dat;
		if(iVM==5) gruppa5[0]=dat;
		if(iVM==6) gruppa6[0]=dat;
		}
		return(0);
	}
	///////////////////////////////////////////
	for(i=0;i<33;i++)
	{
		if(s3==name_fregs[i])
			break;
	}
	if(i!=33)
	{
		prmdi(direct);
		
		for(iVM=1;iVM<=kolwm;iVM++)
		{
			if(VektVM[iVM]==0) continue;
			s.sprintf("#BM %d: ",iVM-1);
			readCP1(i,(short)iVM,(UCHAR*)&dat);
		out.sprintf("# %s=%08X",name_fregs[i], dat);
		out=s+out;
		prmdi(out);
		if(iVM==1) gruppa1[0]=dat;
		if(iVM==2) gruppa2[0]=dat;
		if(iVM==3) gruppa3[0]=dat;
		if(iVM==4) gruppa4[0]=dat;
		if(iVM==5) gruppa5[0]=dat;
		if(iVM==6) gruppa6[0]=dat;
		}
		return(0);
	}
	///////////////////////////////////////////
	prmdier(Errs); return(1);
}
/////////////////////////////////////////////
void SelStrBreak(QString str)
{
	str_fors=str;				
	//::SendMessage(m_W,WM_MDI5,0,0);
	//::SendMessage(hCom,WM_MDI191,1,1);


}
