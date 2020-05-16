#include "PKM.h"
  

extern  int WdatAdr6(UINT adr, UCHAR* dat);
extern    short writeb6(UINT adr,short nom,UCHAR b); 
extern   UINT      gruppa1[];   //Для чтения группы  чисел из ВМ1.
extern 	 UINT      gruppa2[];   //Для чтения группы  чисел из ВМ2.
extern 	 UINT      gruppa3[];   //Для чтения группы  чисел из ВМ3.
extern 	 UINT      gruppa4[];   //Для чтения группы  чисел из ВМ3.
extern 	 UINT      gruppa5[];   //Для чтения группы  чисел из ВМ3.
extern 	 UINT      gruppa6[];   //Для чтения группы  чисел из ВМ3.
extern   short readb6(UINT adr,short nom,UCHAR &b);
extern UINT mul_ind[];
extern int multi_ind;
extern UINT VektVM[];
extern UINT mul_tip[];	//=1 global, =2 static
extern void	OnDisLoc(void);	// In windows Display locals
extern      UINT      gruppa[];   
extern double dfpere(QString str);
extern short kolwm;    //количество машин от 1 до kolwm.
extern UINT fpere(QString str);
extern void prmdi(QString str);
extern void prmdier(QString);

UINT SH[]=
	{0x0,
	0x1,		0x3,		0x7,		0xF,
	0x1F,		0x3F,		0x7F,		0xFF,
	0x1FF,		0x3FF,		0x7FF,		0xFFF,
	0x1FFF,		0x3FFF,		0x7FFF,		0xFFFF,
	0x1FFFF,	0x3FFFF,	0x7FFFF,	0xFFFFF,
	0x1FFFFF,	0x3FFFFF,	0x7FFFFF,	0xFFFFFF,
	0x1FFFFFF,	0x3FFFFFF,	0x7FFFFFF,	0xFFFFFFF,
	0x1FFFFFFF,	0x3FFFFFFF,	0x7FFFFFFF,	0xFFFFFFFF
};



typedef struct
{
	UINT offset;
	UINT type;
	UINT shift;
	UINT len;
}About_t;


About_t TabDat[10000];
QString TabName[10000];
extern int FormType(UINT ,
				Srom *p1,
				UCHAR *p2,
				About_t *p3,
				QString *p4,
				UCHAR* p5);
void	OnTypeForGetadr(UINT adr,Srom* p1,char* p2);
void	OnType(UINT adr,Srom* p1,char*);
void	OnTypeSet(UINT adr,Srom* p1,char*,QString,UINT);
void	OnTypeForGetadrT(UINT adr,Srom* p1,char* p2);

////////////////////////////////////////////////////
void	OnType(UINT adr,Srom* p1,char* p2)
{

	QString otl="",nn(p2),*p4,res,s1;
	int otv,it,itgr,i,iVM;
	About_t *p3;
	UINT dat,adrtype;
	UCHAR* p5;
	UCHAR mp5[2]={0};
	union
	{
		UCHAR ch[8];
		double d;
		USHORT h;
		UINT w[2];
		float f;
		LONGLONG g;
	}d={0};

	otl=otl+nn;
	p3=TabDat;
	p4=TabName;
	adrtype=adr;
	if(mul_tip[0]==1) adrtype=0;
	p5=mp5;
	otv=FormType( adrtype,	//
				p1,
				(UCHAR*)p2,
				p3,
				p4,
		(UCHAR*)p5);
	if(otv)
	{
		if((nn.indexOf(".")==(-1))&&(nn.indexOf("[")==(-1)))
		{
			TabDat[0].offset=0; TabDat[0].type=4;TabName[1]="";
			TabName[0]=nn;
		}
		else
		{
			s1 = QString("# Не определен тип переменной %1 0x%2").arg(otl).arg(adrtype, 8, 16, QChar('0'));
			prmdier(s1);
			return;
		}

	}
	for(iVM=1;iVM<=kolwm;iVM++)
	{
		if(VektVM[iVM]==0) continue;
	otl = QString("#--------------  BM %1 -------------").arg(iVM-1);
	prmdi(otl);


	UINT adrt=adr;
	itgr=0;

	for(it=0;it<10000;it++)
	{
		if(TabName[it]=="") break;
		switch (TabDat[it].type)
		{
		case 0:adrt=adr+TabDat[it].offset;
				for(i=0;i<4;i++)
					readb6(adrt+i,(short)iVM,d.ch[i]);
				dat=d.w[0];
			dat=dat>>TabDat[it].shift;
			dat=dat&SH[TabDat[it].len];
			otl = QString("#%1	0x%2	%3	%4").arg(adrt, 8, 16, QChar('0')).arg(dat, 8, 16, QChar('0')).arg(TabName[it]).arg(dat);
				prmdi(otl);
				if(itgr<rrom)
				{
			if(iVM==1) gruppa1[itgr]=dat;
			if(iVM==2) gruppa2[itgr]=dat;
			if(iVM==3) gruppa3[itgr]=dat;
			if(iVM==4) gruppa4[itgr]=dat;
			itgr++;
				}
				break;

		case 1:
		case 3:
			adrt=adr+TabDat[it].offset;
			for (i=0;i<4;i++)
			{
				readb6(adrt+i,(short)iVM,d.ch[i]);
			}
		otl=QString("#%1	0x%2	%3	%4").arg(adrt, 8, 16, QChar('0')).arg(d.w[0], 8, 16, QChar('0')).arg(TabName[it]).arg(d.w[0]);
		dat=d.w[0];
				if(itgr<rrom)
				{
			if(iVM==1) gruppa1[itgr]=dat;
			if(iVM==2) gruppa2[itgr]=dat;
			if(iVM==3) gruppa3[itgr]=dat;
			if(iVM==4) gruppa4[itgr]=dat;
			itgr++;
				}
			
			prmdi(otl); break;

		case 2:
		case 11:
			adrt=adr+TabDat[it].offset;
				readb6(adrt,(short)iVM,d.ch[0]);
	otl=QString("#%1	0x%2	%3	%4").arg(adrt, 8, 16, QChar('0')).arg(d.ch[0], 8, 16, QChar('0')).arg(TabName[it]).arg(d.ch[0]);
	dat=d.w[0]&(0xFF);
				if(itgr<rrom)
				{
			if(iVM==1) gruppa1[itgr]=dat;
			if(iVM==2) gruppa2[itgr]=dat;
			if(iVM==3) gruppa3[itgr]=dat;
			if(iVM==4) gruppa4[itgr]=dat;
			itgr++;
				}
			
			
			prmdi(otl); break;

		case 4:
		case 5:
			adrt=adr+TabDat[it].offset;
			for (i=0;i<4;i++)
			{
				readb6(adrt+i,(short)iVM,d.ch[i]);
			}
	otl=QString("#0x%1	0x%2	%3	%4").arg(adrt, 8, 16, QChar('0')).arg(d.w[0], 8, 16, QChar('0')).arg(TabName[it]).arg(d.w[0]);
	dat=d.w[0];
				if(itgr<rrom)
				{
			if(iVM==1) gruppa1[itgr]=dat;
			if(iVM==2) gruppa2[itgr]=dat;
			if(iVM==3) gruppa3[itgr]=dat;
			if(iVM==4) gruppa4[itgr]=dat;
			itgr++;
				}
			prmdi(otl); break;
	
		case 6:
			adrt=adr+TabDat[it].offset;
			for (i=0;i<8;i++)
			{
				readb6(adrt+i,(short)iVM,d.ch[i]);
			}
			otl	= QString("#%1	0x%2%3	%4	%5").arg(adrt, 8, 16, QChar('0')).arg(d.w[1], 8, 16, QChar('0')).arg(d.w[0], 8, 16, QChar('0')).arg(TabName[it]).arg(d.g);
			
			prmdi(otl);
			dat=d.w[0];
				if(itgr<rrom-1)
				{
			if(iVM==1) gruppa1[itgr]=dat;
			if(iVM==2) gruppa2[itgr]=dat;
			if(iVM==3) gruppa3[itgr]=dat;
			if(iVM==4) gruppa4[itgr]=dat;
			itgr++;
				}
	dat=d.w[1];
				if(itgr<rrom-1)
				{
			if(iVM==1) gruppa1[itgr]=dat;
			if(iVM==2) gruppa2[itgr]=dat;
			if(iVM==3) gruppa3[itgr]=dat;
			if(iVM==4) gruppa4[itgr]=dat;
			itgr++;
				}
			break;
		
		case 7:
			adrt=adr+TabDat[it].offset;
			for (i=0;i<8;i++)
			{
				readb6(adrt+i,(short)iVM,d.ch[i]);
			}
	otl = QString("#%1	0x%2%3	%4	%5").arg(adrt, 8, 16, QChar('0')).arg(d.w[1], 8, 16, QChar('0')).arg(d.w[0], 8, 16, QChar('0')).arg(TabName[it]).arg(d.g);
	dat=d.w[0];
				if(itgr<rrom-1)
				{
			if(iVM==1) gruppa1[itgr]=dat;
			if(iVM==2) gruppa2[itgr]=dat;
			if(iVM==3) gruppa3[itgr]=dat;
			if(iVM==4) gruppa4[itgr]=dat;
			itgr++;
				}
	dat=d.w[1];
				if(itgr<rrom-1)
				{
			if(iVM==1) gruppa1[itgr]=dat;
			if(iVM==2) gruppa2[itgr]=dat;
			if(iVM==3) gruppa3[itgr]=dat;
			if(iVM==4) gruppa4[itgr]=dat;
			itgr++;
				}
				prmdi(otl); break;
		
		case 8:
			adrt=adr+TabDat[it].offset;
			for (i=0;i<2;i++)
			{
				readb6(adrt+i,(short)iVM,d.ch[i]);
			}
	otl=QString("#%1	0x%2	%3	%4").arg(adrt, 8, 16, QChar('0')).arg((SHORT)d.h, 4, 16, QChar('0')).arg(TabName[it]).arg((SHORT)d.h);
			prmdi(otl);
	dat=d.w[0]&0xFFFF;
				if(itgr<rrom-1)
				{
			if(iVM==1) gruppa1[itgr]=dat;
			if(iVM==2) gruppa2[itgr]=dat;
			if(iVM==3) gruppa3[itgr]=dat;
			if(iVM==4) gruppa4[itgr]=dat;
			itgr++;
				}
			break;
		case 9:
			adrt=adr+TabDat[it].offset;
			for (i=0;i<2;i++)
			{
				readb6(adrt+i,(short)iVM,d.ch[i]);
			}
			otl.sprintf("#%08X	0x%08X	%s	%u"
				,adrt,d.h,TabName[it],d.h);
			prmdi(otl); 
	dat=d.w[0]&0xFFFF;
				if(itgr<rrom-1)
				{
			if(iVM==1) gruppa1[itgr]=dat;
			if(iVM==2) gruppa2[itgr]=dat;
			if(iVM==3) gruppa3[itgr]=dat;
			if(iVM==4) gruppa4[itgr]=dat;
			itgr++;
				}
			break;
		case 10:
			adrt=adr+TabDat[it].offset;
				readb6(adrt,(short)iVM,d.ch[0]);
	otl.sprintf("#%08X	0x%08X	%s	%d"
		,adrt,(char)d.ch[0],TabName[it],(char)d.ch[0]);
			prmdi(otl); 
	dat=d.w[0]&0xFF;
				if(itgr<rrom-1)
				{
			if(iVM==1) gruppa1[itgr]=dat;
			if(iVM==2) gruppa2[itgr]=dat;
			if(iVM==3) gruppa3[itgr]=dat;
			if(iVM==4) gruppa4[itgr]=dat;
			itgr++;
				}
			break;

		case 12:
		case 16:
			adrt=adr+TabDat[it].offset;
			for (i=0;i<4;i++)
			{
				readb6(adrt+i,(short)iVM,d.ch[i]);
			}
	otl.sprintf("#%08X	0x%08X	%s	%e",adrt,d.w[0],TabName[it],d.f);
			prmdi(otl);
	dat=d.w[0];
				if(itgr<rrom-1)
				{
			if(iVM==1) gruppa1[itgr]=dat;
			if(iVM==2) gruppa2[itgr]=dat;
			if(iVM==3) gruppa3[itgr]=dat;
			if(iVM==4) gruppa4[itgr]=dat;
			itgr++;
				}
			break;
		
		
		case 13:
		case 14:
		case 17:
		case 18:
			adrt=adr+TabDat[it].offset;
			for (i=0;i<8;i++)
			{
				readb6(adrt+i,(short)iVM,d.ch[i]);
			}
			otl.sprintf("#%08X	0x%X%08X	%s	%.18g"
				,adrt,d.w[1],d.w[0],TabName[it],d.g);
	dat=d.w[0];
				if(itgr<rrom-1)
				{
			if(iVM==1) gruppa1[itgr]=dat;
			if(iVM==2) gruppa2[itgr]=dat;
			if(iVM==3) gruppa3[itgr]=dat;
			if(iVM==4) gruppa4[itgr]=dat;
			itgr++;
				}
	dat=d.w[1];
				if(itgr<rrom-1)
				{
			if(iVM==1) gruppa1[itgr]=dat;
			if(iVM==2) gruppa2[itgr]=dat;
			if(iVM==3) gruppa3[itgr]=dat;
			if(iVM==4) gruppa4[itgr]=dat;
			itgr++;
				}
				prmdi(otl); break;
		
		
		case 19:
			adrt=adr+TabDat[it].offset;
			for (i=0;i<4;i++)
			{
				readb6(adrt+i,(short)iVM,d.ch[i]);
			}
			otl.sprintf("#%08X	0x%08X	%s",adrt,d.w,TabName[it]);
	dat=d.w[0];
				if(itgr<rrom-1)
				{
			if(iVM==1) gruppa1[itgr]=dat;
			if(iVM==2) gruppa2[itgr]=dat;
			if(iVM==3) gruppa3[itgr]=dat;
			if(iVM==4) gruppa4[itgr]=dat;
			itgr++;
				}
				prmdi(otl); break;
		case 15:
			int dat19;
			adrt=adr+TabDat[it].offset;
			for (i=0;i<4;i++)
			{
				readb6(adrt+i,(short)iVM,d.ch[i]);
			}
			dat19=d.w[0];
			adrt=adrt+4;
			for (i=0;i<4;i++)
			{
				readb6(adrt+i,(short)iVM,d.ch[i]);
			}

			otl.sprintf("#%08X	0x%08X	0x%08X	%s	%d	%d"
				,adrt,dat19,d.w[0],TabName[it],dat19,d.w[0]);
			prmdi(otl); 
	dat=dat19;
				if(itgr<rrom-1)
				{
			if(iVM==1) gruppa1[itgr]=dat;
			if(iVM==2) gruppa2[itgr]=dat;
			if(iVM==3) gruppa3[itgr]=dat;
			if(iVM==4) gruppa4[itgr]=dat;
			itgr++;
				}
	dat=d.w[0];
				if(it<rrom-1)
				{
			if(iVM==1) gruppa1[itgr]=dat;
			if(iVM==2) gruppa2[itgr]=dat;
			if(iVM==3) gruppa3[itgr]=dat;
			if(iVM==4) gruppa4[itgr]=dat;
			itgr++;
				}
			break;

		default:
	otl.sprintf("# Неизвестный тип переменной, тип=%d",TabDat[it].type);
		prmdier(otl);break;	
		}
	}
	}
}
////////////////////////////////////////////////////////
void	OnTypeSet(UINT adr,Srom* p1,char* p2,QString str,UINT air_tip)
{
	QString otl,nn(p2),*p4,res,s1;
	int otv,it,i,iVM;
	About_t *p3;
	UINT dat2,dat1,adrtype;
	UCHAR* p5;
	UCHAR mp5[2]={0};
	union
	{
		UCHAR ch[8];
		double d;
		USHORT h;
		UINT w;
		float f;
		LONGLONG g;
	}d;

	otl=otl+nn;
	p3=TabDat;
	p4=TabName;
	p5=mp5;
	adrtype=adr;
	if(mul_tip[0]==1) adrtype=0;

	otv=FormType( adrtype,	//
				p1,
				(UCHAR*)p2,
				p3,
				p4,
			(UCHAR*)	p5);
	if(otv)
	{
	if((nn.indexOf(".")==(-1))&&(nn.indexOf("[")==(-1)))
	{
		TabDat[0].offset=0; TabDat[0].type=air_tip;TabName[1]="";
		TabName[0]=nn;
	}
	else
	{
		s1.sprintf("# Не определен тип переменной %s %X",otl,adrtype);
		prmdier(s1);
		return;
	}

	}


	if(TabName[1]!="")
	{
		prmdier("#Ошибка, у этого символа нет значения");return;
	}
	UINT adrt=adr;

	if((str.at(0)==QChar('f'))||(str.at(0)==QChar('g')))
	{
		str.remove(0, 1);
	}
	for(it=0;it<10000;it++)
	{
		if(TabName[it]=="") break;
		switch (TabDat[it].type)
		{
		case 0:
			for(iVM=1;iVM<=kolwm;iVM++)
			{
			if(VektVM[iVM]==0) continue;
			adrt=adr+TabDat[it].offset;
			dat1=fpere(str);
		dat1=dat1&SH[TabDat[it].len];
			dat1=dat1<<TabDat[it].shift;
			 dat2=SH[TabDat[it].len]<<TabDat[it].shift;
			dat2=dat2^0xFFFFFFFF;
			for(i=0;i<4;i++) readb6(adrt+i,(short)iVM,d.ch[i]);

			
			d.w=d.w&dat2;d.w=d.w|dat1;
			for(i=0;i<4;i++) writeb6(adrt+i,(short)iVM,d.ch[i]);

			}

			break;

		case 1:
		case 3:
			adrt=adr+TabDat[it].offset;
			d.w=(int)fpere(str);
			WdatAdr6(adrt,&d.ch[0]);
			break;

		case 2:
		case 11:
			adrt=adr+TabDat[it].offset;
			d.ch[0]=(UCHAR)fpere(str);
			for(iVM=1;iVM<=kolwm;iVM++)
			{		if(VektVM[iVM]==0) continue;
			writeb6(adrt,(short)iVM,d.ch[0]);
			}		
			break;

		case 4:
		case 5:
			adrt=adr+TabDat[it].offset;
			d.w=(UINT)fpere(str);
			WdatAdr6(adrt,&d.ch[0]);
			break;


			
		case 6:
			adrt=adr+TabDat[it].offset;
			d.g=(LONGLONG)dfpere(str);
			for(iVM=1;iVM<=kolwm;iVM++)
			{
					if(VektVM[iVM]==0) continue;
			for (i=0;i<8;i++)
			{
				writeb6(adrt+i,(short)iVM,d.ch[i]);
			}
			}
			break;
		
		case 7:
			adrt=adr+TabDat[it].offset;
			d.g=(LONGLONG)dfpere(str);
			for(iVM=1;iVM<=kolwm;iVM++)
			{
				if(VektVM[iVM]==0) continue;
			for (i=0;i<8;i++)
			{
				writeb6(adrt+i,(short)iVM,d.ch[i]);
			}
			}
			break;
		
		case 8:
			adrt=adr+TabDat[it].offset;
			d.h=(short)fpere(str);
			for(iVM=1;iVM<=kolwm;iVM++)
			{
				if(VektVM[iVM]==0) continue;
			for (i=0;i<2;i++)
			{
				writeb6(adrt+i,(short)iVM,d.ch[i]);
			}
			}
			break;
		case 9:
			adrt=adr+TabDat[it].offset;
			d.h=(USHORT)fpere(str);
			for(iVM=1;iVM<=kolwm;iVM++)

			{
				if(VektVM[iVM]==0) continue;
			for (i=0;i<2;i++)
			{
				writeb6(adrt+i,(short)iVM,d.ch[i]);
			}
			}
			break;
		case 10:
			adrt=adr+TabDat[it].offset;
			d.ch[0]=(char)fpere(str);
			for(iVM=1;iVM<=kolwm;iVM++)
			{
				if(VektVM[iVM]==0) continue;
			writeb6(adrt,(short)iVM,d.ch[0]);
			}

			break;

		case 12:
		case 16:
			adrt=adr+TabDat[it].offset;
			d.f=str.toFloat();
			WdatAdr6(adrt,&d.ch[0]);
			break;
		
		case 13:
		case 14:
		case 17:
		case 18:
			adrt=adr+TabDat[it].offset;
			d.d=dfpere(str);
			WdatAdr6(adrt,&d.ch[0]);
			WdatAdr6(adrt+4,&d.ch[4]);
			break;
		
		case 19:
			adrt=adr+TabDat[it].offset;
			d.w=fpere(str);
			WdatAdr6(adrt,&d.ch[0]);
			break;

		default:
	otl.sprintf("# Неизвестный тип переменной, тип=%d",TabDat[it].type);
		prmdier(otl);break;	
		}
	}
OnDisLoc();
}
//////////////////////////////////////////
void	OnTypeForGetadr(UINT adr,Srom* p1,char* p2)
{

	QString otl="",nn(p2),*p4,res,s1;
	int otv,iVM;
	About_t *p3;
	UINT adrtype;
	UCHAR* p5;
	UCHAR mp5[2]={0};
	otl=otl+nn;
	p3=TabDat;
	p4=TabName;
	adrtype=adr;
	if(mul_tip[0]==1) adrtype=0;
	p5=mp5;
	otv=FormType( adrtype,	//
				p1,
				(UCHAR*)p2,
				p3,
				p4,
		(UCHAR*)p5);
	if(otv)
	{
	if((nn.indexOf(".")==(-1))&&(nn.indexOf("[")==(-1)))
	{
		TabDat[0].offset=0; TabDat[0].type=4;TabName[1]="";
		TabName[0]=nn;
	}
	else
	{
		s1.sprintf("# Не определен тип переменной %s %X",otl,adrtype);
		prmdier(s1);
		return;
	}

	}
	UINT adrt=adr;
	adrt=adrt+TabDat[0].offset;
	for(iVM=1;iVM<=kolwm;iVM++)
	{
		if(VektVM[iVM]==0) continue;
			if(iVM==1) gruppa1[0]=adrt;
			if(iVM==2) gruppa2[0]=adrt;
			if(iVM==3) gruppa3[0]=adrt;
			if(iVM==4) gruppa4[0]=adrt;
			if(iVM==5) gruppa5[0]=adrt;
			if(iVM==6) gruppa6[0]=adrt;
	}
	s1.sprintf("# Адрес=%08X",adrt); prmdi(s1);
	return;

}
//////////////////////////////////////////////////////////////////////////////
void	OnTypeForGetadrT(UINT adr,Srom* p1,char* p2)
{

	QString otl="",nn(p2),*p4,res,s1;
	int otv,iVM;
	About_t *p3;
	UINT adrtype;
	UCHAR* p5;
	UCHAR mp5[2]={0};
	otl=otl+nn;
	p3=TabDat;
	p4=TabName;
	adrtype=adr;
	if(mul_tip[0]==1) adrtype=0;
	p5=mp5;
	otv=FormType( adrtype,	//
				p1,
				(UCHAR*)p2,
				p3,
				p4,
		(UCHAR*)p5);
	if(otv)
	{
	if((nn.indexOf(".")==(-1))&&(nn.indexOf("[")==(-1)))
	{
		TabDat[0].offset=0; TabDat[0].type=4;TabName[1]="";
		TabName[0]=nn;
	}
	else
	{
		s1.sprintf("# Не определен тип переменной %s %X",otl,adrtype);
		prmdier(s1);
		return;
	}

	}
	UINT adrt=adr;
	adrt=adrt+TabDat[0].offset;
	for(iVM=1;iVM<=kolwm;iVM++)
	{
//		if(VektVM[iVM]==0) continue;
			if(iVM==1) gruppa1[0]=adrt;
			if(iVM==2) gruppa2[0]=adrt;
			if(iVM==3) gruppa3[0]=adrt;
			if(iVM==4) gruppa4[0]=adrt;
			if(iVM==5) gruppa5[0]=adrt;
			if(iVM==6) gruppa6[0]=adrt;
	}
//	s1.Format("# Адрес=%08X",adrt); prmdi(s1);
	return;

}
////////////////////////////////////