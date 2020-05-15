#include "PKM.h"
extern UCHAR FORMAT;
extern void prmdi(QString);

QString S;
extern UCHAR bufpr[];
unsigned char bufc[1000];
extern int nn,uu;
extern union 
{
	UINT word[500];
	UCHAR ch[2000];
}bufx;
extern UINT fpere(QString str);
int k; //global'nie peremennie, svyaz'dvuh funkciy
////////////////////////////////

int kolvo(QString stroka)
{
int n;
stroka.remove(0, 1);
stroka = stroka.trimmed();

//////////!!!!!!!!!!!!!!!!!!!!!!!		
			n=fpere(stroka); if(n==0) n=1;

			n=n*uu;
			nn=n;
return n;

}
///////////////////////////////////////
int format(QString st)
{
			if((st.indexOf('i')!=(-1)) || (st.indexOf('I')!=(-1)))
			{
				FORMAT='I';
				uu=4;
			}
			if((st.indexOf('x')!=(-1)) || (st.indexOf('X')!=(-1)))
				FORMAT='X';
			if((st.indexOf('d')!=(-1)) || (st.indexOf('D')!=(-1)))
				FORMAT='D';
			if((st.indexOf('u')!=(-1)) || (st.indexOf('U')!=(-1)))
				FORMAT='U';
			if((st.indexOf('f')!=(-1)) || (st.indexOf('F')!=(-1)))
				FORMAT='F';
			if((st.indexOf('g')!=(-1)) || (st.indexOf('G')!=(-1)))
			{
				FORMAT='G';
				uu=8;
			}

			if((st.indexOf('b')!=(-1)) || (st.indexOf('B')!=(-1)))
				uu=1;
			if((st.indexOf('h')!=(-1)) || (st.indexOf('H')!=(-1)))
				uu=2;
			if((st.indexOf('w')!=(-1)) || (st.indexOf('W')!=(-1)))
				uu=4;

return 1;
}
///////////////////////////////////////
int perevod(UINT adr,UINT mn)
{
 int kl,q;

kl=mn;
 for (int r=0;r<kl;r++)
      {
          bufc[r]=bufx.ch[r];
      }

///////////////////////////////////////////////////
  if ((FORMAT=='D')&&(uu==1))
       {
	///////////////////////////////
	union {  char simb;
		     unsigned char izbufc;
	      } dznak1;
	///////////////////////////////
      for( q=0;q<kl;q++)
	    {
	       dznak1.izbufc=bufc[q];
		   S.sprintf("#%08X\t%d ",adr+(q*uu),dznak1.simb);
		   prmdi(S);

	    }

       }

////////////////////////////////////////////////////
  if ((FORMAT=='U')&&(uu==1))
       {
	///////////////////////////////
	union {  unsigned char simb;
		     unsigned char izbufc;
	      } dbezznak1;
	///////////////////////////////
      for( q=0;q<kl;q++)
	    {
	       dbezznak1.izbufc=bufc[q];
		   S.sprintf("#%08X\t%u ",adr+(q*uu),dbezznak1.simb);
		   prmdi(S);

	    }

       }
////////////////////////////////////////////////////
    if ((FORMAT=='X')&&(uu==1))
       {
	///////////////////////////////
	union {  unsigned char simb;
		     unsigned char izbufc;
	      } dbezznak1;
	///////////////////////////////
      for( q=0;q<kl;q++)
	    {
	       dbezznak1.izbufc=bufc[q];
	      // cout<<(int)dbezznak1.simb<<' ';
		   S.sprintf("#%08X\t0x%02X ",adr+(q*uu),dbezznak1.simb);
		   prmdi(S);

	    }

       }
////////////////////////////////////////////////////

  if ((FORMAT=='D')&&(uu==2))
       {
	///////////////////////////////
	union {   unsigned char izbufc[2];
		      short simb;
	      } dznak2;
	///////////////////////////////
      for( q=0;q<(kl/uu);q++)
	    {
	       dznak2.izbufc[0]=bufc[2*q];
	       dznak2.izbufc[1]=bufc[2*q+1];
		   S.sprintf("#%08X\t%d ",adr+(q*uu),dznak2.simb);
		   prmdi(S);

	    }

       }
////////////////////////////////////////////////////
  if ((FORMAT=='U')&&(uu==2))
       {
	///////////////////////////////
	union {   unsigned char izbufc[2];
		      unsigned short simb;
	      } dbezznak2;
	///////////////////////////////
      for( q=0;q<(kl/uu);q++)
	    {
	       dbezznak2.izbufc[0]=bufc[2*q];
	       dbezznak2.izbufc[1]=bufc[2*q+1];
		   S.sprintf("#%08X\t%u ",adr+(q*uu),dbezznak2.simb);
		   prmdi(S);


	    }

       }

////////////////////////////////////////////////////
  if ((FORMAT=='X')&&(uu==2))
       {
	///////////////////////////////
	union {   unsigned char izbufc[2];
		      unsigned short simb;
	      } dbezznak2;
	///////////////////////////////
      for( q=0;q<(kl/uu);q++)
	    {
	       dbezznak2.izbufc[0]=bufc[2*q];
	       dbezznak2.izbufc[1]=bufc[2*q+1];
		   S.sprintf("#%08X\t0x%04X ",adr+(q*uu),dbezznak2.simb);
		   prmdi(S);


	    }

       }

////////////////////////////////////////////////////
  if ((FORMAT=='D')&&(uu==4))
       {
	///////////////////////////////
	union {   unsigned char izbufc[4];
    		  long simb;
	      } dznak4;
	///////////////////////////////
      for( q=0;q<(kl/uu);q++)
	    {
	       dznak4.izbufc[0]=bufc[4*q];
	       dznak4.izbufc[1]=bufc[4*q+1];
	       dznak4.izbufc[2]=bufc[4*q+2];
	       dznak4.izbufc[3]=bufc[4*q+3];

		   S.sprintf("#%08X\t%d ",adr+(q*uu),dznak4.simb);
		   prmdi(S);


	    }

       }
////////////////////////////////////////////////////
  if ((FORMAT=='U')&&(uu==4))
       {
	///////////////////////////////
	union {   unsigned char izbufc[4];
	    	  unsigned long simb;
	      } dbezznak4;
	///////////////////////////////
      for( q=0;q<(kl/uu);q++)
	    {
	       dbezznak4.izbufc[0]=bufc[4*q];
	       dbezznak4.izbufc[1]=bufc[4*q+1];
	       dbezznak4.izbufc[2]=bufc[4*q+2];
	       dbezznak4.izbufc[3]=bufc[4*q+3];
		   S.sprintf("#%08X\t%u ",adr+(q*uu),dbezznak4.simb);
		   prmdi(S);


	    }

       }
////////////////////////////////////////////////////
  if ((FORMAT=='F')&&(uu==4))
       {
	     double datd;
	///////////////////////////////
	union {   unsigned char izbufc[4];
		      float simb;
	      } float4;
	///////////////////////////////
      for( q=0;q<(kl/uu);q++)
	    {
	       float4.izbufc[0]=bufc[4*q];
	       float4.izbufc[1]=bufc[4*q+1];
	       float4.izbufc[2]=bufc[4*q+2];
	       float4.izbufc[3]=bufc[4*q+3];

		   datd=(double)float4.simb;
		   S.sprintf("#%08X\t%e ",adr+(q*uu),datd);
		   prmdi(S);


	    }

       }
////////////////////////////////////////////////////
  if ((FORMAT=='X')&&(uu==4))
       {
	///////////////////////////////
	union {   unsigned char izbufc[4];
		      UINT simb;
	      } float4;
	///////////////////////////////
      for( q=0;q<(kl/uu);q++)
	    {
	       float4.izbufc[0]=bufc[4*q];
	       float4.izbufc[1]=bufc[4*q+1];
	       float4.izbufc[2]=bufc[4*q+2];
	       float4.izbufc[3]=bufc[4*q+3];
		   S.sprintf("#%08X\t0x%08X ",adr+(q*uu),float4.simb);
		   prmdi(S);


	    }

       }
////////////////////////////////////////////////////
  if ((FORMAT=='G')&&(uu==8))
       {
	///////////////////////////////
	union {   unsigned char izbufc[8];
			  UINT aa[2];
		      double simb;
	      } double8;
	///////////////////////////////
      for( q=0;q<(kl/uu);q++)
	    {
	       double8.izbufc[0]=bufc[8*q];
	       double8.izbufc[1]=bufc[8*q+1];
	       double8.izbufc[2]=bufc[8*q+2];
	       double8.izbufc[3]=bufc[8*q+3];
	       double8.izbufc[4]=bufc[8*q+4];
	       double8.izbufc[5]=bufc[8*q+5];
	       double8.izbufc[6]=bufc[8*q+6];
	       double8.izbufc[7]=bufc[8*q+7];

		   S.sprintf("#%08X\t%.18g ",adr+(q*uu),double8.simb);
		   prmdi(S);
	    }

       }


 return kl;
}

///////////////////////////////////////


///////////////////////////////////////
