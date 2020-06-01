// !!!!!!!!!!! СМОТРИ ОПИСАНИЕ ПРОЕКТА В TypeMain.cpp !!!!!!!!!!!

//-------------------TypeForm.cpp--------------------------------
//
//------- ФОРМИРОВАНИЕ АДРЕСА И ОПР-Е ТИПА ПЕРЕМЕННОЙ -----------
//                        АРГУМЕНТЫ:
//       int Addr - адрес переменной или функции
//       Srom *TS - строка таблицы, описывающей ELF-формат
//                  (см."TypeForm.h")
//  char *NameDat - имя переменной (выражение, содержащее в кач-ве
//                  разделит.только [] и точку; может описывать
//                  блок переменных, тогда программа раскроет имя
//                  до базовых типов и сформирует таблицу имен)
//    About_t *p3 - указатель на формируемую таблицу адресов и
//                  типов переменных. (см."TypeForm.h")
//    CString *p4 - указатель на формируемую таблицу имен переменных
// char *NameFunc - имя функции

//       Содержимое параметров зависит от класса памяти переменной:
// для глобальной   Addr = 0                 NameFunc - пусто           
// для статической  Addr = адрес переменной  NameFunc - пусто 
// для стековой     Addr = адрес функции     NameFunc - непусто.
                  
//	Программа возвращает 0 в случае благополучного исхода, а 
// при ошибке 1. Ошибка возникает, если имя переменной задано не-
// верно, т.е. содержит недопустимые символы, неверные имена полей
// или не соответствует описанной структуре. Программа выдает на 
// экран соответствующее сообщение об ошибке. Правильность индекса 
// элемента массива (невыхода за размер массива) не проверяется
// и сообщение не выдается.
//	Кроме базовых типов (1~19) в ELF-формате описываются 
// указатели и блоки переменных (производные типы): массивы,
// структуры и объединения.
//	Указатель считаем базовым типом long unsigned int (5). 
// Если он указывает на производный тип, например структуру,
// описание этого производного типа опускаем (выбираем строку ELF-
// формата до конца, не обрабатывая).
//	Массив, как правило, описывается в той же строке ELF-
// формата, что и переменная.
//	Структура обычно описана раньше, чем переменная. Иногда,
// если поле структуры является указателем на структуру, описание
// поля в ELF- формате имеет вид:
// 	имя_поля:Ттип_указ=*тип_струк=xs__имя_струк:,смещ,размер;
// а сама структура описана ниже:
// 	имя_струк:Ттип_струк=s...;
// Такой случай автоматически не обрабатываем, т.к. указатель
// считаем базовым типом.
//	Программа обрабатывает производные типы любой степени вло-
// женности. Элементы массива в имени могут задаваться индексами:
//              Arr[N]           - один элемент массива
//              Arr[N_beg-N_end] - несколько последовательных эл-тов
//              Arr[].имя_поля   - если надо посмотреть только одно 
//                                 поле во всем массиве структур 
//              Arr              - все эл-ты массива, когда идентиф.
//                                 массива является последним в 
//                                 заданном имени переменной
//      Для переменной производного типа сначала строится "ДЕРЕВО", 
// а затем обрабатывается входное имя переменной слева направо c 
// проверкой на соответствие "ДЕРЕВУ". Если дано неполное имя, по 
// "ДЕРЕВУ" формируется таблица имен. На каждом уровне иерархии 
// вычисляется дополнение к адресу.
//	Для описания "ДЕРЕВА" строятся три таблицы:
//		TabType, TabSize, TabPole.

#include "PKM.h"
#include <setjmp.h>
#include "TypeForm.h"
//#include "afxmt.h"

//QMutex Nata;

int FormType(UINT Addr, Srom *TS, UCHAR *NameDat, 
             About_t *p3, QString *p4, UCHAR *NameFunc);
static void EndError(char *t, char *n);
static char *Addr_array(char *n);
static char *Addr_struct(char *n);
static int Seek_name(char *Name, unsigned int Addr, char *Func);
static char *Search_type(char *Type);
static int Srch_type_tab(char *Type);
static char *Definition_type(char *p, char *Type, int mod);
static char *Selection_array(char *p, char *TypeArr, int mod);
static char *Selection_struct(char *p, char *TypeChief, int mod);
static char *Selection_pole(char *p, char *TypeChief, int mod);
static char *Selection_index(char *p, char *TypeIndex, int mod);
static char *Cultivation_type(char *p, char *Type, int mod);
static char *Selection_digit(char *p, char *s);
static char *Select_un_digit(char *p, char *s);
static char *Selection_lex(char *p, char *s);
static void ElRab_to_Res();
static void Res_to_Rab(About_t *pt, QString *ps);
static void Form_arr_name(About_t *Adr);
static void Form_struc_name(About_t *Adr);
static void Form_refer_name(About_t *Adr);
//..............................................................
//                    !!!!!!!!!!!!!! в ОЗУ (не во входном файле) 
static Elf32_stab *Sb;            // смещ. .stab в ELF-формате
static unsigned long int MaxNumS; // число записей в .stab 
unsigned char *OffsetSS;          // смещ. .stabstr в ELF-форм.

static Elf32_External_Sym *SmTb;   // смещ. .symtab в ELF-форм.
static unsigned long int MaxNumSm; // число записей в .symtab 
unsigned char *OffsetST;           // смещ. .strtab в ELF-форм.

static unsigned int iMod;         // инд.начала модуля в .stab
static unsigned int iModT, iModE; // инд.кон.модуля в .stabstr

static char SortName,     // для исх.имени 
                          // вид ПРОИЗВОДНОГО ТИПа:
                          //  's' / 'a' (для блока данных)
                          //  '*'       (для указателя)
             Type[8],     // ТИП переменной (любой уник.номер
                          //                 из ELF-формата) 
            Name[64],     // лексич.единица (бук./ циф./ '_')
            Func[24],     // искомое имя функции
        ChiefType[8],     // тип головного имени
       ChiefName[32];     // головное имя

static char  *uktext;     // указатель на строку ELF-формата
//.............................................................
#define RzmRes    10000 
#define RzmTType  5000
#define RzmTPole  45000
static struct {
  char Type[8];   // ПРОИЗВОДНЫЙ ТИП переменной (уникальный 
                  //              номер из ELF-формата > 19 )
  char Sort;      // вид ПРОИЗВОДНОГО ТИПа: 
                  //     's' / 'a' (для блока данных)
                  //     '*'       (для указателя) 
  char Simple[32];// исходный ТИП переменной 
                  // для массива: 
                  //     "1"..."19" - ОСНОВНОЙ
                  //         > "19" - ПРОИЗВОДНЫЙ (из TabType)
                  // для указателя:
                  //            "5" - long
                  // для структуры: м.б. имя конкретного поля
  short Beg;      // начальный индекс в массиве
  short End;      // конечный индекс в массиве
              } TabType[RzmTType], *tt;  // ДЕРЕВО (таблица 
                                         // ПРОИЗВОДНЫХ ТИПов)
 static unsigned int TabSize[RzmTType+20] = {  
       0, 4, 1, 4, 4, 4, 8,
       8, 2, 2, 1, 1, 4,
       8, 8, 8, 4, 8, 8, 0 };        // ДЕРЕВО (табица
                                     // размеров ТИПов).
static struct {
  char Type[8];      // ТИП поля:
                     //  "1"..."19" - ОСНОВНОЙ
                     //      > "19" - ПРОИЗВОДНЫЙ (из TabType)
                     //         "0" - для битового поля
  char Name[24];     // имя поля
  char Chief[8];     // ТИП структуры, которой принадлежит поле
                     //                        ( из TabType )
  int Posit;         // начальный разряд поля
  int Size;          // размер поля
              } TabPole[RzmTPole], *tp;  // ДЕРЕВО (таблица полей)

 static About_t *pt, *rabt, RabTab[RzmRes];
 static QString *ps, *rabs, RabStr[RzmRes];

FILE *ukfile_k;
#define index 'i'
static jmp_buf env;
//=============================================================
int FormType(UINT Addr, Srom *TS, UCHAR *NameDat, 
             About_t *p3, QString *p4, UCHAR *NameFunc)
{
char *p;  // указатель в строке ELF-формата
char *n;  // указатель в имени переменной на входе (NameDat)
int i, indicator;
unsigned char type_code;

//Nata.Lock(); FIX!!!
tt = TabType;
tp = TabPole;

Sb       = TS->pstab;
MaxNumS  = TS->lstab / sizeof(Elf32_stab);
OffsetSS = TS->pstabst;

SmTb     = TS->psym;
MaxNumSm = TS->lsym / sizeof(Elf32_External_Sym);
OffsetST = TS->pstrtab;
//...выделение головного имени в исходном имени................ 
//......и определение вида блока переменных....................
n = Selection_lex((char*)NameDat, Name);   // головное имя ( Name )

switch(*n) {                         
  case '.' : { SortName = 's';
               break; }
  case '[' : { SortName = 'a';
               break; }
  case '\0': { SortName = '*';
               break; }
  default  : { EndError("запрещенный символ  ", n);
               goto ENDERR; }
           }
strcpy(ChiefName,Name);
strcat(Name,":");
strcpy(Func,(const char*)NameFunc);
if ( Func[0] != '\0' )
   strcat(Func,":");

//iModE = iModT + (Sb+iMod)->n_value;
//........... поиск описания переменной в ELF-формате .........
if((type_code=Seek_name(Name, Addr, Func)) == 0) // *uktext - строка ELF,
                                     // в кот.описана перем.
  {char s[64];
//  sprintf(s,"не найден  %s %12u \n", Name, Addr);
//   ::MessageBox(NULL, s, " 1",MB_OK);
   goto ENDERR;
  }

iModE = iModT + (Sb+iMod)->n_value;

i = strlen(Name);                
// игнорируем описание класса памяти                
while( !isdigit( *(uktext+i) ) ) 
   i++;
// выделяем в ELF-формате ТИП переменной
p = Selection_digit(uktext+i, Type); 

if( atoi(Type) <= 19 )
  {                          // ОСНОВНОЙ ТИП --> конец
  p3->offset = 0;
  p3->type   = atoi(Type);
  p3->shift  = 0;
  p3->len    = 0;

  *p4     = QString((char*)NameDat);
  *(p4+1) = "";

  goto END;
  }
//=========== обработка ПРОИЗВОДНОГО ТИПа =====================
if ( setjmp(env) != 0 )    // выход из вызываемых функций 
   goto ENDERR;            // по контролю размера таблиц

strcpy(ChiefType,Type);
//----------- построение ДЕРЕВА -------------------------------
if ( *p != '=' )                    
   p = Search_type(Type);  // поиск описания ТИПа в ELF-формате
else
   if ( type_code == 0x40 && *(p+1) == '*' )
      {p += 2;
       p = Selection_digit(p, Type); 
       if ( atoi(Type) <= 19 )
          {                // ОСНОВНОЙ ТИП --> конец
          p3->offset = 0;
          p3->type   = atoi(Type);
          p3->shift  = 0;
          p3->len    = 0;

          *p4     = QString((char*)NameDat);
          *(p4+1) = "";

          goto END;
          }
       strcpy(ChiefType,Type);
       if ( *p != '=' )
          p = Search_type(Type); // поиск опис.ТИПа в ELF-форм.
      }

p = Definition_type(p, Type, 0);  // тут ЗАКРУТКА    
//----------- разборка исходного имени ------------------------ 
while( atoi(Type) > 19 )   // очередной уровень в исх.имени:    
  {                        // имеем Type и SortName

  i=0, tt=TabType;         // поиск ТИПа в таб.ПРОИЗВ.ТИПов
  while( strcmp(tt->Type, Type) != 0 ) 
    {i++,tt++;}
// сравнение вида блока переменных в ELF-формате и в исх.имени
  if( tt->Sort != SortName )     
    if( SortName == '*' && *n == '\0' )
      goto ENDALL;
    else
      {
      EndError( "несовпадение типов 1 ", n );
//      EndError( "неверное имя (несоответствие типа) ", NameDat );
      goto ENDERR;
      }
//.... вычисление дополнения к адресу для массива и ...........
//.... определение SortName и Type для очередного уровня ......
  if( tt->Sort == 'a' && *n == '[' )
    {
    if( (n = Addr_array(n)) == 0 )                                              
      goto ENDERR;
    }                      
//.... вычисление дополнения к адресу для структуры и .........
//.... определение SortName и Type для очередного уровня ......
  else if( tt->Sort == 's' && *n == '.')
    {
    if( (n = Addr_struct(n)) == 0 )   
      goto ENDERR;
    }
  else if( tt->Sort == '*' )
//.... установ Type для очередного уровня .....................
    {
    if( *n != '\0')
      {
      EndError( "несовпадение типов 2 ", n );
//      EndError( "неверное имя (несоответствие типа) ", NameDat );
      goto ENDERR;
      }
    strcpy(Type, tt->Simple);
    }
  }
//---- форм-е массива имен в случае неполного имени -----------
ENDALL:

pt=p3,       ps=p4;
rabt=RabTab, rabs=RabStr; 

rabt->offset = 0;
rabt->type   = atoi(ChiefType);
rabt->shift  = 0;
rabt->len    = 0;
*rabs        = ChiefName;
*(rabs+1)    = "";
//............ ЗАКРУТКА ( ОБРАБОТКА РАБОЧЕГО МАССИВА ).........
while (1)
   {
   pt=p3,       ps=p4;
   rabt=RabTab, rabs=RabStr; 
   indicator = 0;
   while ( *rabs != "" )
      {
      if ( rabt->type <= 19 )
         ElRab_to_Res();
      else
         {
         indicator++;
         tt = TabType;
         while ( (unsigned int)atoi(tt->Type) != rabt->type)
               tt++;
         switch (tt->Sort) 
            {
            case 'a': Form_arr_name(p3+RzmRes-1);
                      break;
            case 's': Form_struc_name(p3+RzmRes-1);
                      break;
            case '*': Form_refer_name(p3+RzmRes-1);
                      break;
            }
         }
      rabs++, rabt++;
      }
   if ( indicator == 0 )
      break;
   else
      Res_to_Rab(p3,p4);
   }

END:
   //Nata.Unlock(); FIX!!!
   return 0;         
//-------------------------------------------------------------
ENDERR: //Nata.Unlock(); FIX!!!
		return 1;
}
//=============================================================
static void EndError(char *t, char *n)
{
char s[64];

//sprintf(s,"%s  %s \n", t, n);
//::MessageBox(NULL, s, "2 ",MB_OK);

return;
}
//====== поиск описания перем. в .stab  по имени и адр.========
static int Seek_name(char *Name, unsigned int Addr, char *Func)
{
int i;
char FindName[64];

if ( Func[0] == '\0' )
   strcpy(FindName, Name);
else
   strcpy(FindName, Func);

for (iModT=0,iMod=0; iMod < MaxNumS; 
     iModT += (Sb+iMod)->n_value,iMod += (Sb+iMod)->n_desc+1)
   for (i=0; i <= (Sb+iMod)->n_desc; i++)
      if ( (Sb+iMod+i)->n_strx != 0 )
         {                                      // есть ссылка на строку
         uktext = (char *)(OffsetSS+iModT+(Sb+iMod+i)->n_strx);
         if ( strncmp(uktext, FindName, strlen(FindName)) == 0 )
            if ( (Sb+iMod+i)->n_value == Addr ) // имя и адрес совпадают
               if ( Func[0] == '\0' )           // ПЕРЕМЕННАЯ
                  {
                  if ( Addr != 0 )
                     return (Sb+iMod+i)->n_type;          // статическая
                  else 
                     if ( (Sb+iMod+i)->n_type == 0x20)
                        return (Sb+iMod+i)->n_type;       // глобальная
                  } 
               else 
                  if ( (Sb+iMod+i)->n_type == 0x24)       // ФУНКЦИЯ?                  
                     {                    
                     for (; i <= (Sb+iMod)->n_desc; i++)
                        if ( (Sb+iMod+i)->n_strx != 0 )
                           {
                           uktext = (char *)(OffsetSS+iModT+(Sb+iMod+i)->n_strx);
                           if ( strncmp(uktext, Name, strlen(Name)) == 0 )
                              return (Sb+iMod+i)->n_type; // стековая
                           }
                     return 0;  // нашли функцию, но не нашли переменную
                     }
         }

return 0;
}
//============== разборка имени для эл-та масс.================
static char *Addr_array(char *n)
{
char Index[8];

n = Select_un_digit(n+1, Index); // выделение индек.в исх.имени

if ( Index[0] != '\0' )
   {
   tt->Beg = atoi(Index);
   if ( *n == '-' )
      {
      n = Selection_digit(n+1, Index); // выделение конеч.индек.
      tt->End = atoi(Index);
      if ( tt->End < tt->Beg )
         {EndError(" неверный интервал индексов ", n);
          return 0;
         }
      }
   else
      tt->End = tt->Beg;
   }
//-------------- следующий уровень ДЕРЕВА ---------------------
strcpy(Type, tt->Simple);       // ТИП элемента массива (в ЕLF)
if( atoi(Type) <= 19 )                
//.............. ОСНОВНОЙ ТИП .................................                    
  if( *n == ']' && *(n+1) == '\0' )
    return ++n;                     
  else
    {EndError("несовпадение типа эл-тов массива  ", n);
     return 0;
    }
//.............. ПРОИЗВОДНЫЙ ТИП ..............................
//........ определение в исх.имени вида блока перем. ..........
else if( *n == ']' && *(n+1) == '[' )             
  SortName = 'a';
else if( *n == ']' && *(n+1) == '.' )            
  SortName = 's';
else if( *n == ']' && *(n+1) == '\0' )            
  SortName = '*';
else
  {EndError("запрещенный символ (масс) ", n);
   return 0;
  }
return ++n;
}
//================  разборка имени для поля структ.============
static char *Addr_struct(char *n)
{
char NamePole[24];

n = Selection_lex(n+1, NamePole);

for(tp = TabPole; tp->Chief[0] != '\0'; tp++)
  if( strcmp(tp->Name,NamePole) == 0 && 
      atoi(tp->Chief) == atoi(tt->Type))
    break;
if(tp->Chief[0] == '\0')
  {EndError("неверное имя поля  ", NamePole);
   return 0;
  }
strcpy(tt->Simple, NamePole);
//-------------- следующий уровень ДЕРЕВА ---------------------
strcpy(Type, tp->Type);       // ТИП поля (в ЕLF)
if( atoi(Type) <= 19 )
//.............. ОСНОВНОЙ ТИП .................................                    
  if( *n == '\0' )
    return n;
  else
    {EndError("несовпадение типа поля  ", NamePole);
     return 0;
    }
//............. ПРОИЗВОДНЫЙ ТИП ...............................
//....... определение в исх.имени вида блока перем. ...........
if( *n == '[' )               
  SortName = 'a';
else if( *n == '.' )            
  SortName = 's';
else if( *n == '\0' )            
  SortName = '*';
else
  {EndError("запрещенный символ (структ) ", n);
   return 0;
  }
return n;
}
//============= построение ДЕРЕВА =============================
static char *Definition_type(char *p, char *Type, int mod)
{// mod - записывать/не записывать тип в таблицу типов

if(*(p+1) == '*' || *(p+1) == 'e'|| *(p+1) == 'f') //  ??? ВИД блока перем.(в ELF)
  p = Selection_index(p,Type,mod);
else if( *(p+1) == 'a' )
  p = Selection_array(p,Type,mod);
else if( *(p+1) == 's' || *(p+1) == 'u' )
  p = Selection_struct(p,Type,mod);
return p;
} 
//==== построение ДЕРЕВА (массив --> табл.типов) ==============
static char *Selection_array(char *p, char *TypeArr, int mod)
{// mod - записывать/не записывать тип в таблицу типов
char TypeSimple[8], ibeg[8], iend[8];
while( *p != ';') p++;                 // cимволы "ar0" 
p = Selection_digit(p+1, ibeg);        // начальный индекс
p = Selection_digit(p+1, iend);        // конечный индекс
p = Selection_digit(p+1, TypeSimple);  // ТИП эл-та мас.(в ELF)
if(atoi(TypeSimple) > 19)
  p = Cultivation_type(p, TypeSimple, mod);

if ( mod == 0)
{
strcpy(tt->Type, TypeArr);
tt->Sort = 'a';                 // зап.ВИДа блока перем.в табл.
strcpy(tt->Simple, TypeSimple); // зап.ТИПа эл-та масс.в табл.
tt->Beg = atoi(ibeg);
tt->End = atoi(iend);

tt++;
if ( tt == TabType + RzmTType )
   {
    //::MessageBox(NULL, "переполнение таблицы типов (a)\n", " ",MB_OK); FIX!!!
    longjmp(env, 1);            // выход на setjmp()
   }
tt->Sort = '\0';

TabSize[atoi(TypeArr)] =
  ( atoi(iend) - atoi(ibeg) + 1) * TabSize[atoi(TypeSimple)];
}
return p;
}
//==== построение ДЕРЕВА (структура --> табл.типов) ===========
static char *Selection_struct(char *p, char *TypeChief, int mod)
{// mod - записывать/не записывать тип в таблицу типов
char SizeStruct[8];
p = Selection_digit(p+2, SizeStruct);       // размер структуры

do p = Selection_pole(p, TypeChief, mod);
   while( *p != ';' );           // указ.на ';' конца структуры

if ( mod == 0)
{
strcpy(tt->Type, TypeChief);
tt->Sort = 's';                  // зап.ВИДа блока перем.в таб.
tt->Simple[0] = '\0';

tt++;
if ( tt == TabType + RzmTType )
   {
    //::MessageBox(NULL, "переполнение таблицы типов (s)\n", " ",MB_OK);
    longjmp(env, 1);             // выход на setjmp()
   }
tt->Sort = '\0';

TabSize[atoi(TypeChief)] = atoi(SizeStruct);
}
return p+1;
}
//==== построение ДЕРЕВА (структура --> табл.полей) ===========
static char *Selection_pole(char *p, char *TypeChief, int mod)
{// mod - записывать/не записывать тип в таблицу типов
char TypePole[8], NamePole[24], Position[8], Size[8];
int i=0;

while( *p != ':' )
  {
  NamePole[i] = *p;
  i++, p++;
  }
NamePole[i] = '\0';

p = Selection_digit(p+1, TypePole);    // ТИП поля (в ELF)
if(atoi(TypePole) > 19)
  p = Cultivation_type(p, TypePole, mod);

p = Selection_digit(p+1, Position);    // нач.разряд поля
p = Selection_digit(p+1, Size);        // размер поля
                                       
if ( mod == 0)
{
if( (atoi(TypePole) < 19) && (atoi(Size) != 8 * TabSize[atoi(TypePole)]))
  {
  strcpy(tp->Type, "0");           // запись ТИПа поля в табл.
  tp->Size  = atoi(Size);
  }
else
  {
  strcpy(tp->Type, TypePole);      // запись ТИПа поля в табл.
  tp->Size  = 0;
  }
strcpy(tp->Name, NamePole);
strcpy(tp->Chief, TypeChief);
tp->Posit = atoi(Position);
tp++;
if ( tp == TabPole + RzmTPole )
   {
    //::MessageBox(NULL, "переполнение таблицы полей\n", " ",MB_OK);
    longjmp(env, 1);               // выход на setjmp()
   }
tp->Chief[0] = '\0';
}
p++;
if( *p == '\\' )      // строка с переносом ( > ~600 символов )
    p+=2;
return p;
}
//==== построение ДЕРЕВА (указатель --> табл.типов) ===========
static char *Selection_index(char *p, char *TypeIndex, int mod)                  
{// mod - записывать/не записывать тип в таблицу типов
char TypeSimple[8], p_sort;

p_sort = *(p+1);
p = Selection_digit(p+2, TypeSimple);  // ТИП эл-та, на кот.указ.
if (atoi(TypeSimple) > 19)
   p = Cultivation_type(p, TypeSimple,1); //!!! пропускаем 
                                          //    описание типа
if ( mod == 0)
{
strcpy(tt->Type, TypeIndex);
tt->Sort = '*';               // запись ВИДа перем.в табл.
strcpy(tt->Simple, "5");      // запись ТИПа перем.в табл.
if ( p_sort == '*' )
   tt->Beg = 1;
   
tt++;
if ( tt == TabType + RzmTType )
   {
    //::MessageBox(NULL, "переполнение таблицы типов (*)\n", " ",MB_OK);
    longjmp(env, 1);          // выход на setjmp()
   }
tt->Sort = '\0';
TabSize[atoi(TypeIndex)] = TabSize[5];
}

return p;
}
//============ обработка производного типа ====================
static char *Cultivation_type(char *p, char *Type, int mod)
{// mod - записывать/не записывать тип в таблицу типов
char *pp;

if ( Srch_type_tab(Type) == 0 )           // уже есть в TabType? 
   {                                       
   if ( *p != '=' )
      {                                   // p указывает на ','
      if ( mod == 0)
         {                         // тип - не объект указателя
         pp = Search_type(Type);
         Definition_type(pp,Type,0);
         }
      }
   else
      if ( *(p+1) == 'x' )                // заменитель описания
         {                                // xs_...
         while ( *p != ',')               
            p++;                          // !!! пропускаем xs_...
         if ( mod == 0)                   
            {                             // p указывает на ','
            pp = Search_type(Type);
            Definition_type(pp,Type,0);
            }
         }
      else
         p = Definition_type(p,Type,mod); // p указывает на '='
   }
else
   if ( *p == '=' )
      p = Definition_type(p,Type,1);      // !!! пропускаем 
                                          //     описание типа

return p;
}      
//============ поиск типа в таблице типов =====================
static int Srch_type_tab(char *Type)
{
unsigned int i, n;
n = tt - TabType;
for ( i=0; i < n; i++ )
   {
   if (strcmp((TabType+i)->Type,Type) == 0)
      return 1;
   }

return 0;
}
//=========== поиск описания типа в .stab =====================
/*static char *Search_type(char *Type)
{
char *p, TypeSearch[8];
unsigned int i=0, n;

strcat(strcpy(TypeSearch, Type), "=");

do {
   uktext = (char *)(OffsetSS + iModT + i);
   if( (n = strlen(uktext)) != 1 )
     {
     p = uktext;
     do {
        p = strstr(p, TypeSearch);
        if( p != 0 && !isdigit(*(p-1)))
          break;
        }while( p != 0 );
     if( p != 0 )
       break;
      }
   i += n+1;
   }while( i < iModE );

return p+strlen(Type);
} 
*/    
//=========== поиск описания типа в .stab =====================
static char *Search_type(char *Type)
{
char *p, TypeSearch[8];
unsigned int i=0, n;

strcat(strcpy(TypeSearch, Type), "=");

do {
   uktext = (char *)(OffsetSS + iModT + i);
   if( (n = strlen(uktext)) != 1 )
     {
     p = uktext;
     do {
        p = strstr(p, TypeSearch);
        if( p != 0 && !isdigit(*(p-1)))
          break;
        }while( p != 0 );
     if(( p != 0 ) && *(p+strlen(Type)+1) != 'x' )
       break;
      }
   i += n+1;
   }while( i < iModE );

return p+strlen(Type);
}     
//====== выборка и перепись числа со знаком =======
static char *Selection_digit(char *p, char *s)
{
while( isdigit(*p) || *p == '-')
  {
  *s = *p;
  p++, s++;
  }
*s = '\0';

return p;
}  
//====== выборка и перепись числа без знака =======
static char *Select_un_digit(char *p, char *s)
{
while( isdigit(*p) )
  {
  *s = *p;
  p++, s++;
  }
*s = '\0';

return p;
}  
//===== выборка и перепись лексической единицы =====
static char *Selection_lex(char *p, char *s)
{
while( isalnum(*p) || *p == '_'  )
  {
  *s = *p;
  p++, s++;
  }
*s = '\0';

return p;
}  
//=== перепись одного эл-та из рабочего в вых.массив ===
static void ElRab_to_Res()
{
pt->offset = rabt->offset;
pt->type   = rabt->type;
pt->shift  = rabt->shift;
pt->len    = rabt->len;
pt++;
*ps = *rabs;
ps++;
*ps = "";

return ;
}

//=== перепись всего вых.массива в рабочий =============
static void Res_to_Rab(About_t *pt, QString *ps)
{
rabt=RabTab, rabs=RabStr; 

while ( *ps != "" )
   {
   rabt->offset = pt->offset;
   rabt->type   = pt->type;
   rabt->shift  = pt->shift;
   rabt->len    = pt->len;
   *rabs        = *ps;
   rabt++, pt++;
   rabs++, ps++;
   }
*rabs = "";

return ;
}

//==== форм-е имени и выч-е доп.к адресу для масс.==========
static void Form_arr_name(About_t *Adr)
{
int n;
char str[8];

if ( tt->Beg == 0 && (tt->End == 0 || tt->End == -1) ) 
   {
   pt->offset = rabt->offset;
   pt->type   = atoi(tt->Simple);
   pt->shift  = 0;
   pt->len    = 0;
   *ps = *rabs + "[0]";
   pt++;
   if ( pt == Adr )
   {
    //::MessageBox(NULL, "переполнение таблицы имен\n", " ",MB_OK);
    longjmp(env, 1);           // выход на setjmp()
   }
   ps++;
   *ps = "";
   return;
   }

for ( n=tt->Beg; n <= tt->End; n++ )
   {
   pt->offset = rabt->offset + n * TabSize[atoi(tt->Simple)];
   pt->type   = atoi(tt->Simple);
   pt->shift  = 0;
   pt->len    = 0;
   *ps = *rabs + "[" + itoa(n,str,10) + "]";
//{
//char s[64];
//sprintf(s,"ps: %s type: %u offset: %u pt_off: %u\n", *ps, pt->type, rabt->offset, pt->offset );
//::MessageBox(NULL, s, " ",MB_OK);
//}
   pt++;
   if ( pt == Adr )
   {
    //::MessageBox(NULL, "переполнение таблицы имен\n", " ",MB_OK);
    longjmp(env, 1);           // выход на setjmp()
   }
   ps++;
   *ps = "";
   }

return;
}
//==== форм-е имени и выч-е доп.к адресу для структ.==========
static void Form_struc_name(About_t *Adr)
{
if ( tt->Simple[0] != '\0' )
   {
   for(tp = TabPole; tp->Chief[0] != '\0'; tp++)
      if( strcmp(tp->Name,tt->Simple) == 0 &&
         atoi(tp->Chief) == atoi(tt->Type))
         break;

   pt->offset = rabt->offset + (tp->Posit / 8) - (tp->Posit % 8) / 8;
   pt->type   = atoi(tp->Type);
   pt->shift  = rabt->shift + (tp->Posit % 8);
   pt->len    = tp->Size;
   if ( tp->Name[0] == '\0' )
      *ps = *rabs + ".*";           // неименованное поле
   else
      *ps = *rabs + "." + tp->Name;
   pt++;
   if ( pt == Adr )
   {
    //::MessageBox(NULL, "переполнение таблицы имен\n", " ",MB_OK);
    longjmp(env, 1);                // выход на setjmp()
   }
   ps++;
   *ps = "";
   }
else
   for(tp = TabPole; tp->Chief[0] != '\0'; tp++)
      if( atoi(tp->Chief) == atoi(tt->Type))
         {
         pt->offset = rabt->offset + (tp->Posit / 8) - (tp->Posit % 8) / 8;
         pt->type   = atoi(tp->Type);
         pt->shift  = rabt->shift + (tp->Posit % 8);
         pt->len    = tp->Size;
         if ( tp->Name[0] == '\0' )
            *ps = *rabs + ".*";      // неименованное поле
         else
            *ps = *rabs + "." + tp->Name;
         pt++;
         if ( pt == Adr )
		 {
         //::MessageBox(NULL, "переполнение таблицы имен\n", " ",MB_OK);
         longjmp(env, 1);            // выход на setjmp()
		 }
         ps++;
         *ps = "";
         }
return;
}
//==== форм-е имени и выч-е доп.к адресу для указ.==========
static void Form_refer_name(About_t *Adr)
{
pt->offset = rabt->offset;
pt->type   = atoi(tt->Simple);
pt->shift  = tt->Beg;
pt->len    = 0;
*ps = *rabs;
pt++;
if ( pt == Adr )
   {
    //::MessageBox(NULL, "переполнение таблицы имен\n", " ",MB_OK);
    longjmp(env, 1);                 // выход на setjmp()
   }
ps++;
*ps = "";

return;
}
//======================================================
