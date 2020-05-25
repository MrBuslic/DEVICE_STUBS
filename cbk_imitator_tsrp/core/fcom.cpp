
#include "PKM.h"
#include "rpc_loger.h"
extern reg_table RegTable;


#define WM_MDI1                      WM_APP+1
#define WM_MDI2                      WM_APP+2
#define WM_MDI3                      WM_APP+3
#define WM_MDI38                      WM_APP+38


 extern   M_BP *pBP[];
extern M_BP m_bp[];
//extern CBp *pBp;

extern HWND hBp;
extern    short writeb6(UINT adr,short nom,UCHAR b); 
extern QString direct,Errs,strmdi,strForBr,strForDel;
extern UINT VektVM[];
extern   short kolwm;      //Количество ВМ  
//extern CComandBar *pCom;
extern QString Errs,direct,ms_bp[],out,out1;
//extern void fchsum( UCHAR* buf);	// KS
extern int last_ind;
extern QString* p_ms[];
//extern HWND m_W;
extern HWND hDmod;
extern UINT paus;
extern   UINT volatile widasa;
//extern      CString   imakom;         //Имя исходного командного файла.
//extern      UINT      adresost;       //Адрес останова /есть всегда.
//extern      QString   mestost;        //Текст, описывающий место останова.
extern      UINT      konez;          //=7, если ЦМ завершила программу.
extern      UINT      avar;           //=7, если если авар.ситуация в ЦМ.
extern      UINT      snat;           //=7, если надо снять мою задачу.
extern      UINT      gruppa[];   //Для чтения группы романовских чисел.
extern      UINT      tiprom;         //Определяет тип романовской перем.
extern int AttachObj(int i, QString name);
int AttachRom(QString name);

//extern short readb(UINT adr, UCHAR &b);
//extern short writeb(UINT adr, UCHAR b);
void KillAll(void);
QMutex fmdi,fkl;
UINT fpere(QString str);
DCB dcb;
HANDLE hCom=NULL;
DWORD dwErr, dwMask, kol;
BOOL fSuccess;
OVERLAPPED o;
COMMTIMEOUTS tout;
UCHAR bufpr[2000], bufper[2000];
UCHAR bufch[600];

UINT baut=115200;
QString a="COM2";

extern QString direct;
extern QString strmdi;
void TrimBreak(UINT j);
QMutex fbreak;
QString fName;
extern int LoadObj(int i, QString name);
void prmdier(QString str);
QString out,out1;
void printkl(QString str);
void prmdi(QString str);
//extern CLdbgApp theApp;
//CEditView* ev,*evdoc;
//CLdbgDoc* doc,*docdoc;
QString priem;
void OnTa(void); 
extern void ThreaDeb(void);
int ReadCom(int col,UCHAR* buf);
void OnNastr(void); 
//int InitCom();
Srom lrom[100]={0};
QString from[100];

int LoadRom(QString name);
double dfpere(QString str);
int WBufCom(UINT adr, int len, UCHAR *buf);
int timer;
void	KillBp(UINT adrs,UINT adre);	// delete breakpoints
int KillRom(QString name);



//////////////////////////////////////////////////
UCHAR bufVnutr[2000];
//////////////////////////////////////////////////
UINT fpere(QString str)
{
	char  *ppa;
	long la = 1;
	la = strtoul(str.toStdString().c_str(), &ppa, 0);
	return((UINT)la);

}
double dfpere(QString str)
{	
	char  *ppa;
	double la;
	la = strtod(str.toStdString().c_str(), &ppa);
	return(la);
}
//////////////////////////////////////////////////////////
void prmdi(QString str)
{
	fkl.lock();
	if(widasa)
	{
		fkl.unlock();
		return;
	}
		strmdi=str;
		SRPCSignalClass::Instance().toLog(strmdi);
	//::SendMessage(m_W,WM_MDI1,0,0);
	fkl.unlock();
}
void printkl(QString str)
{
	fkl.lock();
	strmdi=str;
	SRPCSignalClass::Instance().toLog(strmdi);
	//::SendMessage(m_W,WM_MDI1,0,0);
	fkl.unlock();

}

void printklp()
{
	fkl.lock();
	strmdi = RegTable.BufferForText;
	SRPCSignalClass::Instance().toLog(strmdi);
	//::SendMessage(m_W,WM_MDI1,0,0);
	fkl.unlock();

}

void prmdier(QString str)
{
	fkl.lock();
	Beep(200,300);//snat=7;
	strmdi=str;
	SRPCSignalClass::Instance().toLog(strmdi);
	//::SendMessage(m_W,WM_MDI1,0,0);
	fkl.unlock();
}

///////////////////////////////////////////////////
//// загрузка программы в целевую машину
int LoadRom(QString name)
{
	QString otl,nam1,nam2;
	int i;

	prmdi(direct);
	out=name;
	nam1=name;
	nam1 = nam1.toLower();

	for(i=0;i<99;i++)
	{
		nam2=from[i];
		nam2 = nam2.toLower(); 
		if((nam2==nam1)&&(lrom[i].rom!=0)) break;
		if(lrom[i].rom==0)
		{
			lrom[i+1].rom="";
			from[i]=name;
			lrom[i].rom=from[i];
	lrom[i].pstab=0;
	lrom[i].lstab=0;
	lrom[i].pstabst=0;
	lrom[i].lstabst=0;
	lrom[i].psym=0;
	lrom[i].lsym=0;
	lrom[i].pstrtab=0;
	lrom[i].lstrtab=0;
			break;
		}
	}
	if(i==99)
	{
		prmdier("#  Слишком много программ - команда не выполнена");
		lrom[i].rom="";
		return(1);
	}
	if(lrom[i].pstab) 
		free(lrom[i].pstab);
	if(lrom[i].pstabst) free(lrom[i].pstabst);
	if(lrom[i].psym) free(lrom[i].psym);
	if(lrom[i].pstrtab) free(lrom[i].pstrtab);
	lrom[i].pstab=0;
	lrom[i].lstab=0;
	lrom[i].pstabst=0;
	lrom[i].lstabst=0;
	lrom[i].psym=0;
	lrom[i].lsym=0;
	lrom[i].pstrtab=0;
	lrom[i].lstrtab=0;

	if(LoadObj(i,name))
	{
	if(lrom[i].pstab) 
		free(lrom[i].pstab);
	if(lrom[i].pstabst) free(lrom[i].pstabst);
	if(lrom[i].psym) free(lrom[i].psym);
	if(lrom[i].pstrtab) free(lrom[i].pstrtab);
			from[i]="";
	lrom[i].pstab=0;
	lrom[i].lstab=0;
	lrom[i].pstabst=0;
	lrom[i].lstabst=0;
	lrom[i].psym=0;
	lrom[i].lsym=0;
	lrom[i].pstrtab=0;
	lrom[i].lstrtab=0;
			
			lrom[i].rom="";
		return(1);
	}
	return(0);
}
/////////////////////////////////////////////
///////////////////////////////////////////////////
//// подключение отладочной информации 
int AttachRom(QString name)
{
	QString otl,nam1,nam2;
	int i;

	prmdi(direct);
	out=name;
	nam1=name; 
	nam1 = nam1.toLower();

	for(i=0;i<99;i++)
	{
		nam2=from[i];
		nam2 = nam2.toLower();
		if((nam2==nam1)&&(lrom[i].rom!=0)) break;
		if(lrom[i].rom==0)
		{
			lrom[i+1].rom="";
			from[i]=name;
			lrom[i].rom=from[i];
	lrom[i].pstab=0;
	lrom[i].lstab=0;
	lrom[i].pstabst=0;
	lrom[i].lstabst=0;
	lrom[i].psym=0;
	lrom[i].lsym=0;
	lrom[i].pstrtab=0;
	lrom[i].lstrtab=0;
			break;
		}
	}
	if(i==99)
	{
		prmdier("#  Слишком много программ - команда не выполнена");
		lrom[i].rom="";
		//file.Close();
		return(1);
	}
	if(lrom[i].pstab) 
		free(lrom[i].pstab);
	if(lrom[i].pstabst) free(lrom[i].pstabst);
	if(lrom[i].psym) free(lrom[i].psym);
	if(lrom[i].pstrtab) free(lrom[i].pstrtab);
	lrom[i].pstab=0;
	lrom[i].lstab=0;
	lrom[i].pstabst=0;
	lrom[i].lstabst=0;
	lrom[i].psym=0;
	lrom[i].lsym=0;
	lrom[i].pstrtab=0;
	lrom[i].lstrtab=0;

	if(AttachObj(i,name))
	{
	if(lrom[i].pstab) 
		free(lrom[i].pstab);
	if(lrom[i].pstabst) free(lrom[i].pstabst);
	if(lrom[i].psym) free(lrom[i].psym);
	if(lrom[i].pstrtab) free(lrom[i].pstrtab);
			from[i]="";
	lrom[i].pstab=0;
	lrom[i].lstab=0;
	lrom[i].pstabst=0;
	lrom[i].lstabst=0;
	lrom[i].psym=0;
	lrom[i].lsym=0;
	lrom[i].pstrtab=0;
	lrom[i].lstrtab=0;
			
			lrom[i].rom="";
		return(1);
	}
	return(0);
}

/////////////////////////////////////////////
void KillAll(void)
{
	QString nam1,nam2;
	prmdi(direct);
	for(;;)
	{
	if(lrom[0].rom==0) {return;}
	nam1=lrom[0].rom;
	KillRom(nam1);
	}
}
///////////////////////////////////////////////////
//// уничтожение программы 
int KillRom(QString name)
{
	int i,j,ind_kill=0;
	UINT adrs, adre;
	QString nam1,nam2,otl;
	out=name;
	nam1=name; 
	nam1 = nam1.toLower();
	for(i=0;i<=99;i++)
	{
		if(lrom[i].rom==0) break;
		nam2=lrom[i].rom;
		nam2 = nam2.toLower();
		if((nam2==nam1)&&(lrom[i].rom!=0))
		{
	if(lrom[i].pstab) 
		free(lrom[i].pstab);
	if(lrom[i].pstabst) free(lrom[i].pstabst);
	if(lrom[i].psym) free(lrom[i].psym);
	if(lrom[i].pstrtab) free(lrom[i].pstrtab);
	lrom[i].pstab=0;
	lrom[i].lstab=0;
	lrom[i].pstabst=0;
	lrom[i].lstabst=0;
	lrom[i].psym=0;
	lrom[i].lsym=0;
	lrom[i].pstrtab=0;
	lrom[i].lstrtab=0;
			ind_kill=i;break;
		}
	}
		if((lrom[i].rom==0)||(i==99))
		{
			prmdier("# Нет такой программы");
			return(1);
		}
	

	adrs=lrom[ind_kill].ftext;adre=lrom[ind_kill].etext;
	KillBp(adrs,adre);	// delete breakpoints

	////////// подтянем таблицу
	for (i=ind_kill,j=ind_kill+1;lrom[j].rom!="";i++,j++)
	{
		lrom[i]=lrom[j]; from[i]=from[j]; lrom[i].rom=from[i];
	}
	lrom[i].rom="";
	last_ind=0; return(0);


	if(last_ind<ind_kill) return(0);
	if((last_ind==ind_kill)&&(last_ind==0)) return(0);
	if((last_ind==ind_kill)&&(last_ind!=0))
	{
		last_ind--; return(0);
	}
	if(last_ind>ind_kill)
	{
		last_ind--; return(0);
	}
	return(0);
}
//////////////////////////////////////////////
void	KillBp(UINT adrs,UINT adre)	// delete breakpoints
{
	int i,j;
	fbreak.lock();
	for(j=1;j<=kolwm;j++)
	{

		for(i=0;i<1000;i++)
	{
	if(((pBP[j]+i)->adr>=adrs)&&((pBP[j]+i)->adr<=adre))
	{
	if(((pBP[j]+i)->pr)&4)
	{
			strForDel=		*(p_ms[j]+i);

	//::SendMessage( m_W,WM_MDI38,0,0);
	}
		
		(pBP[j]+i)->pr=0;
	}
		}
	}




		//if(pBp)		::SendMessage(hBp,WM_MDI3,0,0); //FIX!!!
					fbreak.unlock();

}

/////////////////////////////////////////////
////////////////////////////////////////////////////////
int WBufCom(UINT adr, int len, UCHAR *buf)
{

	//CString adrs,lens,chs;
	int i,j;
	for(i=1;i<=kolwm;i++)
	{
		if(VektVM[i])
		{
			for(j=0;j<len;j++)
			{
				if(writeb6(adr+j,(short)i,buf[j]))
				{
					prmdier("# Ошибка от write6 в WBufCom");
					return 1;
				}
			}
		}
	}
	return 0;
}

///////////////////////////////////////////////////////////
////////////////////////////////////////////
// подтягивание точек останова к началу массива
// j- номер ВМ 
void TrimBreak(UINT j)
{
	int i,k;
	fbreak.lock();
	for(k=0;k<1000;k++)
	{
		m_bp[k].pr=0;
	}


	for(i=0,k=0;i<1000;i++)
	{
		if((pBP[j]+i)->pr==0) continue;
		m_bp[k].pr=(pBP[j]+i)->pr;
		m_bp[k].adr=(pBP[j]+i)->adr;
		ms_bp[k]=*(p_ms[j]+i);
		k++;
		m_bp[k].pr=0;
	}

	for(i=0,k=0;i<1000;k++,i++)
	{
		if(m_bp[k].pr==0) break;
		(pBP[j]+i)->pr=m_bp[k].pr;
		(pBP[j]+i)->adr=m_bp[k].adr;
		*(p_ms[j]+i)=ms_bp[k];
	}
		(pBP[j]+i)->pr=0;
				//if(pBp)	
				//{
				//	::SendMessage(hBp,WM_MDI3,0,0);Sleep(1); FIX!!! BP!!
				//}
	fbreak.unlock();
	
}

/////////////////////////////////////////////

