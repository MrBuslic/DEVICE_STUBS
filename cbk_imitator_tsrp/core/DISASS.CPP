#include "PKM.h"

struct Tabl { unsigned short int Code;   // kod operation
	      char Name[16];             // name operation
	      unsigned short int Format; // format command (func.Form1-Form22)
	    };
struct Regst { unsigned short int Number; // number register
	       char Name[16];             // name register
	    };
//------------- oper.[31-26] ----------------
static struct Tabl TabOper[] = {
{ 2, "j  ",   9},  { 3, "jal",    9},
{ 4, "beq",  10},  { 5, "bne",   10},
{ 6, "blez", 11},  { 7, "bgtz",  11},
{ 8, "addi", 12},  { 9, "addiu", 12},
{10, "slti", 12},  {11, "sltiu", 12},
{12, "andi", 13},  {13, "ori",   13},
{14, "xori", 13},  {15, "lui",   22},
{32, "lb ",  14},  {33, "lh ",   14},
{34, "lwl",  14},  {35, "lw ",   14},
{36, "lbu",  14},  {37, "lhu",   14},
{38, "lwr",  14},
{40, "sb ",  14},  {41, "sh ",   14},
{42, "swl",  14},  {43, "sw ",   14},
{46, "swr",  14},  {49, "lwc1",  15},
{57, "swc1", 15},
{999, "", 0 }};
//------------- oper.[31-26] = 0 ---------------
//              oper.[ 5-0 ]           
static struct Tabl TabFunc0[] = {
{ 0, "sll",  1}, { 2, "srl",   1},
{ 3, "sra",  1}, { 4, "sllv",  2},
{ 6, "srlv", 2}, { 7, "srav",  2},
{ 8, "jr ",  6}, { 9, "jalr",  4},
{12, "syscall", 5},{ 13, "break", 5},
{16, "mfhi", 3}, {17, "mthi",  6},
{18, "mflo", 3}, {19, "mtlo",  6},
{24, "mult", 7}, {25, "multu", 7},
{26, "div",  7}, {27, "divu",  7},
{32, "add", 24}, {33, "addu", 24},
{34, "sub", 24}, {35, "subu", 24},
{36, "and", 24}, {37, "or ",  24},
{38, "xor", 24}, {39, "nor",  24},
{42, "slt", 24}, {43, "sltu", 24},
{999, "", 0}};
//------------- oper.[31-26] = 1 ----------------
//              oper.[ 5-0 ]           
static struct Tabl TabFunc1[] = {
{ 0, "bltz",   8},
{ 1, "bgez",   8},
{16, "bltzal", 8},
{17, "bgezal", 8},
{999, "", 0}};
//------------- oper.[ 31-26 ] = 16 ---------------
//              oper.[ 25-21 ]           
static struct Tabl TabFunc16[] = {
{ 0, "mfc0", 16},
{ 4, "mtc0", 16},
{999, "", 0 }};
//----- oper.: [31-26]=16, [25-21]=8 --------------
//              oper.[ 20-16 ]           
static struct Tabl TabFunc16_8[] = {
{ 0, "bc0f", 18},
{ 1, "bc0t", 18},
{999, "", 0}};
//----- oper.: [31-26]=16, [25-21]=16 --------------
//              oper.[ 5-0 ]           
static struct Tabl TabFunc16_16[] = {
{ 1, "tlbr",  17},
{ 2, "tlbwi", 17},
{ 6, "tlbwr", 17},
{ 8, "tlbp",  17},
{16, "rfe",   17},
{999, "", 0}};
//------------- oper.[ 31-26 ] = 17 ---------------
//              oper.[ 25-21 ]           
static struct Tabl TabFunc17[] = {
{ 0, "mfc1", 23},
{ 4, "mtc1", 23},
{ 2, "cfc1", 23},
{ 6, "ctc1", 23},
{999, "", 0 }};
//----- oper.: [31-26]=17, [25-21]=8 --------------
//              oper.[ 20-16 ]           
static struct Tabl TabFunc17_8[] = {
{ 0, "bc1f", 18},
{ 1, "bc1t", 18},
{999, "", 0}};
//----- oper.: [31-26]=17, [25-21]=16 --------------
//              oper.[ 5-0 ]           
static struct Tabl TabFunc17_16[] = {
{  0, "add.s",   19},
{  1, "sub.s",   19},
{  2, "mul.s",   19},
{  3, "div.s",   19},
{  5, "abs.s",   20},
{  6, "mov.s",   20},
{  7, "neg.s",   20},
{ 33, "cvt.d.s", 20},
{ 36, "cvt.w.s", 20},
{ 48, "c.f.s",   21},
{ 49, "c.un.s",  21},
{ 50, "c.eq.s",  21},
{ 51, "c.ueq.s", 21},
{ 52, "c.olt.s", 21},
{ 53, "c.ult.s", 21},
{ 54, "c.ole.s", 21},
{ 55, "c.ule.s", 21},
{ 56, "c.sf.s",  21},
{ 57, "c.ngle.s",21},
{ 58, "c.seq.s", 21},
{ 59, "c.ngl.s", 21},
{ 60, "c.lt.s",  21},
{ 61, "c.nge.s", 21},
{ 62, "c.le.s",  21},
{ 63, "c.ngt.s", 21},
{999, "", 0}};
//----- oper.: [31-26]=17, [25-21]=17 --------------
//              oper.[ 5-0 ]           
static struct Tabl TabFunc17_17[] = {
{  0, "add.d",   19},
{  1, "sub.d",   19},
{  2, "mul.d",   19},
{  3, "div.d",   19},
{  5, "abs.d",   20},
{  6, "mov.d",   20},
{  7, "neg.d",   20},
{ 32, "cvt.s.d", 20},
{ 36, "cvt.w.d", 20},
{ 48, "c.f.d",   21},
{ 49, "c.un.d",  21},
{ 50, "c.eq.d",  21},
{ 51, "c.ueq.d", 21},
{ 52, "c.olt.d", 21},
{ 53, "c.ult.d", 21},
{ 54, "c.ole.d", 21},
{ 55, "c.ule.d", 21},
{ 56, "c.sf.d",  21},
{ 57, "c.ngle.d",21},
{ 58, "c.seq.d", 21},
{ 59, "c.ngl.d", 21},
{ 60, "c.lt.d",  21},
{ 61, "c.nge.d", 21},
{ 62, "c.le.d",  21},
{ 63, "c.ngt.d", 21},
{999, "", 0}};
//----- oper.: [31-26]=17, [25-21]=20 --------------
//              oper.[ 5-0 ]           
static struct Tabl TabFunc17_20[] = {
{ 32, "cvt.s.w", 20},
{ 33, "cvt.d.w", 20},
{999, "", 0}};

//========== Centr.Processor ===================== 
static struct Regst Register[] = {
{0,  "zero"}, {1,  "at"},
{2,  "v0"},   {3,  "v1"},
{4,  "a0"},   {5,  "a1"}, {6,  "a2"}, {7,  "a3"},
{8,  "t0"},   {9,  "t1"}, {10, "t2"}, {11, "t3"},
{12, "t4"},   {13, "t5"}, {14, "t6"}, {15, "t7"},
{16, "s0"},   {17, "s1"}, {18, "s2"}, {19, "s3"},
{20, "s4"},   {21, "s5"}, {22, "s6"}, {23, "s7"},
{24, "t8"},   {25, "t9"},
{26, "k0"},   {27, "k1"},
{28, "gp"},   {29, "sp"}, {30, "fp"}, {31, "ra"},
{999, "" }};
//=========== Coprocessor ========================
static struct Regst RegCoproc[] = {
{ 0, "Index"},     { 1, "Random"},
{ 2, "EntryLo"},   { 3, "Config"},
{ 4, "Context"},   { 5, "Reserved5"},
{ 6, "Reserved6"}, { 7, "Reserved7"},
{ 8, "BadVaddr"},   { 9, "Reserved9"},
{10, "EntryHi"},    {11, "Reserved11"},
{12, "Status"},     {13, "Cause"},
{14, "EPC"},        {15, "PRId"},
{16, "Reserved16"}, {17, "Reserved17"},
{18, "Reserved18"}, {19, "Reserved19"},
{20, "Reserved20"}, {21, "Reserved21"},
{22, "Reserved22"}, {23, "Reserved23"},
{24, "Reserved24"}, {25, "Reserved25"},
{26, "Reserved26"}, {27, "Reserved27"},
{28, "Reserved28"}, {29, "Reserved29"},
{30, "Reserved30"}, {31, "Reserved31"},
{999, "" }};
//==============================================

void Function( unsigned long int Kod, unsigned long int Func,
	       struct Tabl *Tab, char *Mess );
static char *StrZer( char *s1, char *s2 )
    { return strcat(strcat(s1,"\t"),s2); }
static char *StrComma( char *s1, char *s2 )
    { return strcat(strcat(s1,", "),s2); }

static char StrokaAss[64];        // text command ( rezultat )
static unsigned short int Regim;  // register:  "name" / "number"
static unsigned long int AdrCom;  // address command next
//***********************************************************
//***********************************************************
//***********************************************************
 char *Disass(unsigned long int Kod,  // kod command
	     unsigned long Adress,    // address command next
	     unsigned short Reg )     // register: "name" / "number"
{
unsigned long int KodOp;
unsigned long int KodFunc;
unsigned long int KodMod;

if ( Kod == 0 )
   { strcpy( StrokaAss, "..." );
     return StrokaAss;
   }

Regim = Reg;
AdrCom = Adress;

KodOp = Kod >> 26;   //------------- oper.[ 31-26 ] -------------------------                                    
switch ( KodOp )
   { case 0:  KodFunc = Kod & 077;         //----- oper.[ 5-0 ] -------------
              Function( Kod, KodFunc, TabFunc0, " Не функция (0-5)" );
	      break;
     case 1:  KodFunc = (Kod >> 16) & 037; //----- oper.[ 20-16 ] ----------- 
              Function( Kod, KodFunc, TabFunc1, " Не функция (16-20)" );
	      break;
     case 16: KodMod = (Kod >> 21) & 037;  //----- oper.[ 25-21 ] ----------- 
              switch ( KodMod )
	      { case  8: KodFunc = (Kod >> 16) & 037; //---- oper.[ 20-16 ]--
                         Function(Kod,KodFunc,TabFunc16_8," Не код (16-20)");
	                 break;
                case 16: KodFunc = Kod & 077;         //---- oper.[ 5-0 ] ---
                         Function(Kod,KodFunc,TabFunc16_16," Не функция (0-5)");
	                 break;
                default: Function( Kod, KodMod, TabFunc16, " Не код (21-25)" );
              }
	      break;
     case 17: KodMod = (Kod >> 21) & 037;  //----- oper.[ 25-21 ] -----------     
              switch ( KodMod )
              { case  8: KodFunc = (Kod >> 16) & 037; //---- oper.[ 20-16 ]--
                         Function(Kod,KodFunc,TabFunc17_8," Не код (16-20)");
			 break;
                case 16: KodFunc = Kod & 077;         //---- oper.[ 5-0 ]----
                         Function(Kod,KodFunc,TabFunc17_16," Не функция (0-5)");
	                 break;
                case 17: KodFunc = Kod & 077;         //---- oper.[ 5-0 ]----
                         Function(Kod,KodFunc,TabFunc17_17," Не функция (0-5)");
	                 break;
                case 20: KodFunc = Kod & 077;         //---- oper.[ 5-0 ]----
			 Function(Kod,KodFunc,TabFunc17_20," Не функция (0-5)");
	                 break;
                default: Function( Kod, KodMod, TabFunc17, " Не код (21-25)" );
//                default:  strcpy( StrokaAss, " Не код (21-25)" );
              }
	      break;
     default: Function( Kod, KodOp, TabOper, " Не код (26-31)" );
   }
return StrokaAss;
}
//==================== REGISTER ================================
static char *FormReg( unsigned long int Kod,      // kod command
		      unsigned short int Shift,   // mlad.razr.reg.
		      unsigned short int Coproc ) // processor
// Coproc: 0 - Centr.proc.
//         1 - Coproc.
//         2 - Coproc.float
// Regim:  0 - regim "number"
//         1 - regim "name"
{
struct Regst *pR=0;
unsigned short int Numb;      // number register
char NN[8];
static char NameNumb[16];     // text register ( rezultat )

Numb = (unsigned short)((Kod >> Shift) & 037);

if ( Coproc == 2 )
   {
   strcpy( NameNumb, "$f");
   return strcat(NameNumb, itoa(Numb, NN, 10));
   }
else
   strcpy( NameNumb, "$");

if ( Coproc == 1 )
   pR = RegCoproc;
if ( Coproc == 0 )
   if ( Regim == 1 )
      pR = Register;
   else
      if ( Regim == 0 )
         return strcat(NameNumb, itoa(Numb, NN, 10));

while ( pR->Number != 999 )
   {
   if ( pR->Number == Numb )
      return strcat(NameNumb, pR->Name);
   pR++;
   }
return " ? ";
}
//============= CALCUL ADRES ( 0-15 ) ======================
static unsigned long int Calc_Addr( unsigned long int Kod)
{
long int Numb;

if ( (Kod & 0100000) == 0 )
   Numb = Kod & 0xFFFF;
else
   Numb = 0xFFFF0000 | ( Kod & 0xFFFF );
Numb = AdrCom + 4 + (Numb << 2);
//printf("Numb: %lo - %lX\n", Numb, Numb);

return Numb;
}
//========= FORMIR-E KOMMENT.============================
static void SComment( long Numb,      // number (address / const)
                      short Radix )   // sist.: 10 / 16 / 8
{
char Str[64];

if ( Radix == 10 )
   sprintf( Str, "%s\t#  %ld", StrokaAss, Numb );
if ( Radix == 16 )
   sprintf( Str, "%s\t#  0x%lX", StrokaAss, Numb );
if ( Radix == 8 )
   sprintf( Str, "%s\t#  %lo", StrokaAss, Numb );
strcpy( StrokaAss, Str );

return;
}
//============= KONTROL ( POLE = 0 ) ========================
static void Kontrol( unsigned long int Kod,    // kod command
		     unsigned short int Shift, // mlad.razr.pole
		     unsigned short int Numb ) // razmer pole
{
char N[8];
unsigned long int Maska = 037777777777;
unsigned long int K;

Maska = Maska >> (32-Numb);
K = (Kod >> Shift) & Maska;
if ( K != 0 )
	{
      strcat( StrokaAss, " Не нулевое поле с разр." );
      strcat( StrokaAss, itoa( Shift, N, 10 ));
	}
return;
}
//===== FORMIR-E KOMAND (Register,Address,Const,Comment,Kontrol) ====
//------------ FORMAT 1 ---------------------
static void Form1( unsigned long int Kod )
{
char N[16];

StrZer( StrokaAss, FormReg( Kod, 11, 0 ));
StrComma( StrokaAss, FormReg( Kod, 16, 0 ));
StrComma( StrokaAss, ltoa( (Kod >> 6) & 037, N, 10 ));
Kontrol( Kod, 21, 5 );
return;
}
//------------ FORMAT 2 ---------------------
static void Form2( unsigned long int Kod )
{
StrZer( StrokaAss, FormReg( Kod, 11, 0 ));
StrComma( StrokaAss, FormReg( Kod, 16, 0 ));
StrComma( StrokaAss, FormReg( Kod, 21, 0 ));
Kontrol( Kod, 6, 5 );
return;
}
//------------ FORMAT 3 ---------------------
static void Form3( unsigned long int Kod )
{
StrZer( StrokaAss, FormReg( Kod, 11, 0 ));
Kontrol( Kod, 16, 10 );
Kontrol( Kod, 6, 5 );
return;
}
//------------ FORMAT 4 ---------------------
static void Form4( unsigned long int Kod )
{
StrZer( StrokaAss, FormReg( Kod, 11, 0 ));
StrComma( StrokaAss, FormReg( Kod, 21, 0 ));
Kontrol( Kod, 16, 5 );
Kontrol( Kod, 6, 5 );
return;
}
//------------ FORMAT 5 ---------------------
static void Form5( unsigned long int Kod)
{
char N[16];
unsigned long int Numb;

if ( (Kod & 077) == 12 )
      Numb = ( Kod & 0x3FFFFC0 ) >> 6;
else
   if ( (Kod & 077) == 13 )
      Numb = ( Kod & 0x3FF0000 ) >> 16;

sprintf(N, "0x%lX", Numb );
StrZer( StrokaAss, N );
return;
}
//------------ FORMAT 6 ---------------------
static void Form6( unsigned long int Kod )
{
StrZer( StrokaAss, FormReg( Kod, 21, 0 ));
Kontrol( Kod, 6, 15 );
return;
}
//------------ FORMAT 7 ---------------------
static void Form7( unsigned long int Kod )
{
StrZer( StrokaAss, FormReg( Kod, 21, 0 ));
StrComma( StrokaAss, FormReg( Kod, 16, 0 ));
Kontrol( Kod, 6, 10 );
return;
}
//------------ FORMAT 8 ---------------------
static void Form8( unsigned long int Kod )
{
char N[16];
unsigned long int Numb;

Numb = Calc_Addr( Kod );

StrZer( StrokaAss, FormReg( Kod, 21, 0 ));
sprintf( N, "0x%08lX", Numb );
StrComma( StrokaAss, N );
SComment( Numb, 10 );
return;
}
//------------ FORMAT 9 ---------------------
static void Form9( unsigned long int Kod )
{
char N[16];
long int Adr;

Adr = (AdrCom & 036000000000) | ((Kod & 0377777777) << 2);
sprintf(N, "0x%lX", Adr );
StrZer( StrokaAss, N );
SComment( Adr, 10 );
return;
}
//------------ FORMAT 10 --------------------
static void Form10( unsigned long int Kod )
{
char N[16];
unsigned long int Numb;

Numb = Calc_Addr( Kod );

StrZer( StrokaAss, FormReg( Kod, 21, 0 ));
StrComma( StrokaAss, FormReg( Kod, 16, 0 ));
sprintf( N, "0x%08lX", Numb );
StrComma( StrokaAss, N );
SComment( Numb, 10 );
return;
}
//------------ FORMAT 11 --------------------
static void Form11( unsigned long int Kod )
{
char N[16];
unsigned long int Numb;

Numb = Calc_Addr( Kod );

StrZer( StrokaAss, FormReg( Kod, 21, 0 ));
sprintf( N, "0x%08lX", Numb);
StrComma( StrokaAss, N );
SComment( Numb, 10 );
Kontrol( Kod, 16, 5 );
return;
}
//------------ FORMAT 12 --------------------
static void Form12( unsigned long int Kod )
{
char N[16];
short int Numb;

StrZer( StrokaAss, FormReg( Kod, 16, 0 ));
StrComma( StrokaAss, FormReg( Kod, 21, 0));
Numb = (short)(Kod & 0177777);
sprintf( N, "0x%04hX", Numb );
StrComma( StrokaAss, N );
SComment( (long)Numb, 10 );
return;
}
//------------ FORMAT 13 --------------------
static void Form13( unsigned long int Kod )
{
char N[16];
unsigned short int Numb;

StrZer( StrokaAss, FormReg( Kod, 16, 0 ));
StrComma( StrokaAss, FormReg( Kod, 21, 0));
Numb = (unsigned short)(Kod & 0177777);
sprintf( N, "0x%04hX", Numb );
StrComma( StrokaAss, N );
SComment( (long)Numb, 10 );
return;
}
//------------ FORMAT 14 --------------------
static void Form14( unsigned long int Kod )
{
char N[16];
short int Numb;

StrZer( StrokaAss, FormReg( Kod, 16, 0 ));
Numb = (short)(Kod & 0177777);
sprintf( N, "0x%04hX", Numb );
StrComma( StrokaAss, N );
strcat( StrokaAss, " (" );
strcat( StrokaAss, FormReg( Kod, 21, 0 ));
strcat( StrokaAss, ")" );
SComment( (long)Numb, 10 );
return;
}
//------------ FORMAT 15 --------------------
static void Form15( unsigned long int Kod )
{
char N[16];
short int Numb;

StrZer( StrokaAss, FormReg( Kod, 16, 2 ));
Numb = (short)(Kod & 0177777);
sprintf( N, "0x%04hX", Numb );
StrComma( StrokaAss, N );
strcat( StrokaAss, " (" );
strcat( StrokaAss, FormReg( Kod, 21, 0 ));
strcat( StrokaAss, ")" );
SComment( (long)Numb, 10 );
return;
}
//------------ FORMAT 16 --------------------
static void Form16( unsigned long int Kod )
{
StrZer( StrokaAss, FormReg( Kod, 16, 0 ));
StrComma( StrokaAss, FormReg( Kod, 11, 1));
Kontrol( Kod, 0, 11 );
return;
}
//------------ FORMAT 17 --------------------
static void Form17( unsigned long int Kod )
{
Kontrol( Kod, 6, 15 );
return;
}
//------------ FORMAT 18 --------------------
static void Form18( unsigned long int Kod )
{
char N[16];
unsigned long int Numb;

Numb = Calc_Addr( Kod );

sprintf( N, "0x%08lX", Numb);
StrZer( StrokaAss, N );
SComment( Numb, 10 );

return;
}
//------------ FORMAT 19 --------------------
static void Form19( unsigned long int Kod )
{
StrZer( StrokaAss, FormReg( Kod, 6, 2 ));
StrComma( StrokaAss, FormReg( Kod, 11, 2 ));
StrComma( StrokaAss, FormReg( Kod, 16, 2 ));

return;
}
//------------ FORMAT 20 --------------------
static void Form20( unsigned long int Kod )
{
StrZer( StrokaAss, FormReg( Kod, 6, 2 ));
StrComma( StrokaAss, FormReg( Kod, 11, 2 ));
Kontrol( Kod, 16, 5 );
return;
}
//------------ FORMAT 21 --------------------
static void Form21( unsigned long int Kod )
{
StrZer( StrokaAss, FormReg( Kod, 11, 2 ));
StrComma( StrokaAss, FormReg( Kod, 16, 2 ));
Kontrol( Kod, 6, 5 );
return;
}
//------------ FORMAT 22 --------------------
static void Form22( unsigned long int Kod )
{
char N[16];
unsigned short int Numb;

StrZer( StrokaAss, FormReg( Kod, 16, 0 ));
Numb = (unsigned short)(Kod & 0177777);
sprintf( N, "0x%04hX", Numb );
StrComma( StrokaAss, N );
SComment( (long)Numb, 10 );

return;
}
//------------ FORMAT 23 (16) ---------------
static void Form23( unsigned long int Kod )
{
StrZer( StrokaAss, FormReg( Kod, 16, 0 ));
StrComma( StrokaAss, FormReg( Kod, 11, 2));
Kontrol( Kod, 0, 11 );
return;
}
//------------ FORMAT 24 --------------------
static void Form24( unsigned long int Kod )
{
StrZer( StrokaAss, FormReg( Kod, 11, 0 ));
StrComma( StrokaAss, FormReg( Kod, 21, 0 ));
StrComma( StrokaAss, FormReg( Kod, 16, 0 ));
Kontrol( Kod, 6, 5 );
return;
}
//=================== KOD FUNCTION ========================
static void Function( unsigned long int Kod,  // kod command
                      unsigned long int Func, // kod operation
                      struct Tabl *Tab,       // tabl.operation
                      char *Mess )            // message error
{
void (*FuncForm[])(unsigned long int ) = {
   Form1, Form2, Form3, Form4, Form5,
   Form6, Form7, Form8, Form9, Form10,
   Form11, Form12, Form13, Form14, Form15,
   Form16, Form17, Form18, Form19, Form20,
   Form21, Form22, Form23, Form24,
   };
struct Tabl *pT;

pT = Tab;
while ( pT->Code != 999 )
   if ((unsigned short int) Func == pT->Code )
      {
      strcpy( StrokaAss, pT->Name );            // form-e name operation
      (*FuncForm[pT->Format - 1])( Kod );       // call function format
      break;
      }
   else
      pT++;

if ( pT->Code == 999 )
   strcpy( StrokaAss, Mess );

return;
}
