#include "PKM.h"
#include <io.h>
#include "qcoreapplication.h"
//extern BOOL dirVis;
//extern BOOL lineVis;
//extern BOOL stopVis;
//extern BOOL breakVis;
//extern BOOL taVis;
extern int mBreakCom;
extern UINT TargetVM;

//extern CBp *pBp;
//extern HWND hCom;
//extern HWND hBp;

#define WM_MDI191                      WM_APP+191
#define WM_MDI20                      WM_APP+20


                       //    ФАЙЛ  INTKOM  ЦБК

	    //Файл intkom.cpp  - интерпретатор командного файла.
		   //Функция  -   intkom()  , главная  .
     //------------------------------------------------------------
           //16-ти разрядная платформа не предусматривается.


 
     //------------------------------------------------------------

short prz; 
short pesmko=0;
short pesouou=0;
short pesrazb=0;
short pesdll=0;
short pesmw=0;
short pesretr=0;
short pestsinxr=0;
short pesrel=0;
short pestelem=0;
short pesosib=0;
short pesintkom=0;
short nedop=0;
short phim=0;
short pesmmo=0;
short osibpwr=0;
short peskuk=0;
short prerr;   //Для выдачи на экран из proz() информации о прерываниях (по ключу prer в команде командного файла).

int Stopli=01;

int stopkomfile=0;

HANDLE gokomfile;

	      // БАЗОВЫЕ             ПЕРЕМЕННЫЕ :

		     // А.   Собственные:


     QString  metka[rmetka];     //Массив имен меток для переходов.
     int      dlmet[rmetka];     //Массив длин меток, указывают
				                 //как индексы на  :  .
     DWORD    pozmet[rmetka];    //Массив номеров позиций строк,
				                 //где находится метка.
				                 //(В этих массивах индекс общий.)
     int kolmet;                 //Максим.индекс этих меток.
     //------------------------------
     DWORD   wzw[rwzw];       //Массив позиций для возвратов
			                  //из подпрограмм.
     int      indpr[rwzw];    //Массив индексов строк для продол-
			                  //жения после jr.		
     int     indwzw;          //Индекс для wzw[], указывает в нем
			                  //вакансию для стека.Вначале =0.
     //-------------------------Раздел остановов пока не используется.
     QString   ost[rost];      //Массив выражений, задающих текстом
			       //точку останова.
     UINT      adr[rost];      //Массив адресов точек останова.
     int       nomost[rost];   //Массив фиксации номер.точек остан.
     //-----------------------------
     QString  imper[rimper];   //Массив имен внутренних переменных.
     long double znper[rimper];//Массив значений этих переменных.
     int tipper[rimper];       //Массив типов внутренних переменных.
	                          // 1-char,2-UCHAR,3-short,4-USHORT,5-int,
	                           // 6-UINT,9-float,11-double.
	 int  glst[rimper];      //Ярус файла переменной.
     int kolperem;           //Фиксирует максим.индекс этих переменных.
     //------------------------------
     int  jarus;               //Номер яруса вложенного файла.
     QString source[rsource];  //Массив имен вложенных файлов.
     DWORD   poziz[rsource];   //Массив номеров позиций, с которых
			                   //должен продолжаться вышеярусный
			                   //файл при исчерпании данного.
     int  indnas[rsource];     //Индексы строк, с которых будет продол-
			                   //жение после исчерпания вложенного
			                   //файла.
	 //------------------------------------------------------
      QString par[rpar];       //Массив передаваемых локальных парам.
	  int nomer[rpar];         //Номер параметра.
	  int jrpar[rpar];         //Ярус параметра
      int kolpar;              //Количество параметров
     //----------------------------- СЕРИЯ ДАННЫХ ДЛЯ  IF - ELSE :
	 char mskob[relse];        //Для записи  skob  при if.
	 char inase[relse];  //Для записи вып/невып условия при if /+,-,0/.
	                     //0 - для отметки отработанности else по if. 
	 int  ins;           //Общий индекс этих стеков /mskob и inase/.
	 char jmskob[rsource];     //Для запоминания skob в command.
	 int  jins[rsource];       //Для запоминания ins в command.
	 int  jnas[rsource];       //Для запоминания nas в command.
	 int   nas;                //Начальный индекс.
	 char  skob;                //Для счета кол-ва скобок.

	 //-------------------------------
     QString   sitstr;        //Для чтения очередной строки файла.
	 QString   sitstrist;
	 int kontext=0;
     //-------------------------------
     DWORD  tekpoz;           //Для номера текущей или 
				              //последующей позиции строки.
     //-------------------------------
     QFile myf;          //Об'ект класса файл.
	 extern QString SystemDir, DefaultDir;
     //-------------------------------
     int indtek;              //Индекс, идет по обрабатываемой строке.
     int posind;              //Для последнего индекса строки.
     int ninds;               //Для начального индекса слова в строке.
     int pinds;               //Для последнего индекса слова в строке.
     int perex;               //Для задания - куда перейти.
     //----------------------------------------------------------
     char   xxxx1;            //Эта и ниже для echo для выдачи
	 UCHAR  xxxx2;
     short  xxxx3;            //значений внутренних переменных.
	 USHORT xxxx4;
     int    xxxx5;
     UINT   xxxx6;
     float  xxxx9;
	 double xxxx11;
     //--------------------------------
			  // Б. Связанные с Романовым:
     UINT volatile widasa=0;              //0- разрешает выдачу Романову.
	 int old_widasa = 0;
     QString   imakom;         //Имя исходного командного файла.
     UINT      adresost[7]={0};    //Адреса остановов по номеру ВМ .
     QString   mestost[7]={""};     //Тексты, описывающие места останова по номеру ВМ.
	                           //Есть только при останове по тексту.
     UINT      konez;          //=7, если ЦМ завершила программу.
     UINT      avar;           //=7, если если авар.ситуация в ЦМ.
     UINT      snat;           //=7, если надо снять мою задачу.
	 UINT * grup;            //Указатель на группу чтения от Романова.
 UINT  gruppa[rrom];   //Для чтения группы романовских чисел.Временно! Для трансляции!
     UINT      gruppa1[rrom];   //Для чтения группы романовских чисел из ВМ1.
	 UINT      gruppa2[rrom];   //Для чтения группы романовских чисел из ВМ2.
	 UINT      gruppa3[rrom];   //Для чтения группы романовских чисел из ВМ3.
	 UINT      gruppa4[rrom];   //Для чтения группы романовских чисел из ВМ4.
     UINT      gruppa5[rrom];   //Для чтения группы романовских чисел из ВМ5.
     UINT      gruppa6[rrom];   //Для чтения группы романовских чисел из ВМ6.
     UINT      tiprom;         //Определяет тип романовской перем.
                               //Но этого пока не предусмотрено.
     extern QString direct;    //Для передачи Романову ком.отладчика.
     extern UINT    prkom;     //Для передачи Романову признака 
			                   //работы командного файла.
     extern HANDLE eDirective,kDirective;  //Для спи - проснись. 
     QString    rom;           //Для подготовки передаваемых
			                   //команд отладчика - в direct.
	 extern short kolwm;       //Кол-во ВМ.
     //--------------- Конец    базовых   переменных ---------------

		    //РАБОЧИЕ   ПЕРЕМЕННЫЕ:

     QString str1,str2,str3,str4,str5,stek,str6;
	                          //stek-для назван.функции,где ошибка.
     int prizn;                   //Рабочий признак.
     int est;                     //Признак наличия искомого.
     int otw;                     //Ответ операции сравнения, условия.
                                  //otw=1 условие выполнено, иначе - 0.
	 int indsis;                  //индекс числа для логических операц.
	 QChar sa;                     //Для символов
	 short kws;
	 short incl; //***!!!   Пометка действий для include. В 11 местах.
	 short perw; //***!!!
	short nowstr;//Для фиксации в propusk выхода на новую строку и учета этого в obrstr
    int indtekwst; //для возвращения indtek после снятия вставки
	int indzapobrstr;
//-----------------------------------------------------------------

    //--------- Конец  рабочих  переменных -----------------------


	      // ОБ'ЯВЛЕНИЯ                 ФУНКЦИЙ  :

		   //А.  Собственного изготовления:

     void  intkom(LPDWORD);           //ГЛАВНАЯ ФУНКЦИЯ.
     void  tabmet(void);           //Делает таблицы меток.
     void  obrstr(void);           //Обрабатывает содержат. строку, 
				                   //прочитанную в sitstr.
     void  sbros(void);            //Сбрасывает мой поток.
     QString  stand(QString ss);   //Стандартизует строку ss.

	 QString slowo(int n, QString ss);//С индекса n строки ss
				                     //выделяет слово /до разделителя/.
	 QString dotzp(int n, QString ss,char a,char b);
       //С индекса n строки ss возвращает набор до символа 'a', но 
       //не переходя символ 'b' и не включая его в набор.
	 QString dotzpecho(int n, QString ss,char a,char b); //Для echo.
     int  razd(QChar a);            //Возвращает 1, если  a  - раз-
				                   //делитель, иначе возвр  0.
     void prisw(void);             //Присваивает значения переменной.
     void uslif(void);             //Обрабатывает if .
     void splu(void);              //Засыпай - проснись.

     long double sislo(QString ss);//Вычисляет число по ss.
     int rasp(QString ss);         //Распознает, - что в строке.
     int znsr(QChar a);             //Распознает символ сравнения.
	 int znop(QChar a);             //Распознает символ операции.
     void propusk(void);           //Пропускает один оператор,
                                   //простой или в фигурных скобках.
	 void propusk1(void);
     void unist(int jar);          //Уничтожает перемен.  jar-го яруса.
	 void unistpar(int jar);       //Уничтожает параметры jar-го яруса.
     void osibind( );              //Информирование о синтаксич.ошибке. 
     extern void prmdi(QString);
	 int indimper(QString s);
     long double pris(int prizn,long double x);
     void fail();
	 void xwostcom(QString s);
	 QString slowo2(int n, QString ss);
     int razd2(QChar a);
	 QString istima(QString ss);
	 void kontrima(QString ss,int jar);
     void wkl(QString s);
	 void otkl(QString s);
	 void wklus(QString s);
	 void otklus(QString s);
	 QString normstr(QString  s);
	 QString snatwstawka(QString s);
	 QString wstawka(QString s);
	 QString stand(QString ss);

		   //Б.  Взятые мной у Романова:

     extern void printkl(QString ss); //Печатает в протокольный файл.	   		    
     extern UINT fpere(QString str);  //Перевод строки в число/целое.
     extern double dfpere(QString str);//Перевод строки в число/не целое/.
	 
     //------------ Конец об'явлений функций --------------------------

extern UINT NAXAEW;

		   // ОПИСАНИЕ  ФУНКЦИЙ :
     //=================================================================


     void intkom(LPDWORD)                //Главная функция.
     {
       int i,j,k; 
	   snat=0; konez=0;  //Обнул.романовск.призн.
	   avar=0;         
	   widasa=0;
	   incl=0;                                      //***!!!
	   perw=0;                                      //***!!!  
       //-----------------------------------------------------------
	   prkom=1; jarus=1;       //Начал работать ком.файл с 1 ярусом.
       //--------------------------------
       source[1]=imakom;       //Взяли у Романова имя ком. файла.
       poziz[1]=0;             //Работаем с начала этого файла,
       indnas[1]=0;            //и с начального индекса строки.
	   jmskob[1]=0;
	   jins[1]=0;
	   jnas[1]=0;
       //--------------------------------
       kolperem=-1;            //Пока внутренних перем. нет.
	   kolpar=-1;              //Пока передаваемых параметров нет. 
       //-------------------------------
		    for(i=0;i<rost;i++)
			nomost[i]=-1;        //Пока нет остановов.
       //--------------------------------
       indwzw=0;                 //Вершина стека возвратов
			                     //из подпрограмм.
	   nas=0; ins=0; skob=0; inase[0]=0;


       //--------- КОНЕЦ  НАЧАЛЬНЫХ  УСТАНОВОК -------------------


       koleso:                //Раскрутка вложенности файлов и
     //*********	          //построковая обработка.
			      //Начинается с открытия файла:
//++================================================================
	   str5=source[jarus];

		   stek="koleso"; posind=0;
           str4=dotzp(0,str5,':',':');
		     if(est==(-1))     //Нет в имени файла  ':'
	{ //  char bufp[1000];
////	GetCurrentDirectory(1000, &bufp[0]);  // найти текущую директорию
////	QString s2=(char*)bufp;
////	str3=s2+"/"+str5;
    	str3=DefaultDir+"/"+str5;
		myf.setFileName(str3);
      if(myf.open(QIODevice::ReadOnly)) //Открылся
	  {source[jarus]=str3;   
	  
	  goto romanov;}
	  
	  str4=dotzp(0,str5,'/','\\');
	  if(est>=0)      //Есть эти палки
	  {      
         str1=" #E - INTKOM - Ошибка 1 открытия файла - "+str3;
		 printkl(str1); //Сообщение и протокольный. файл.
	     str2=" #  Работа с командным файлом прекращена.";
		 printkl(str2); //Сообщение туда же.
	     //-------------------------
	     jarus=0; 
		 kontext=0;
		 sbros( ); //Сбрасывает мой поток, отмечая это
			              //у Романова.

	  }

	  else
	  {
		  str3=SystemDir+str5;
		  myf.setFileName(str3);
        if(myf.open(QIODevice::ReadOnly)) //Открылся
         {source[jarus]=str3;    goto romanov;}
        else
        {
   
         str1=" #E - INTKOM - Ошибка 2 открытия файла - "+str3;
		 printkl(str1); //Сообщение и протокольный. файл.
	     str2=" #  Работа с командным файлом прекращена.";
		 printkl(str2); //Сообщение туда же.
	     //-------------------------
	     jarus=0; 
		 kontext=0;
		 sbros( ); //Сбрасывает мой поток, отмечая это
			              //у Романова.

        }
      }

	}//----------------------------------------------
     else
     {
		 myf.setFileName(source[jarus]);
       if(!myf.open(QIODevice::ReadOnly))
	   {
	   str1=" #E - INTKOM - Ошибка 3 открытия файла - "+source[jarus];
		 printkl(str1); //Сообщение и протокольный. файл.
	   str2=" #  Работа с командным файлом прекращена.";

	   printkl(str2); //Сообщение туда же.
		 str5.sprintf("jarus=%d",jarus); printkl(str5);
         str5.sprintf("incl=%d",incl); printkl(str5);
	   //-------------------------
	   jarus=0;
	   kontext=0;
	   sbros( ); //Сбрасывает мой поток, отмечая это
			              //у Романова.
	   }
     }
//++================================================================

	 //------ Продолжение работы при открытии файла: ---------
romanov:	str3=source[jarus]; str2="#I - INTKOM - Работа с ком.файлом  ";
		str2=str2+str3;
	if(pesintkom==1) printkl(str2);  //Оповещение в начале работы.
     //---------------------
	  tabmet( );         // - Теперь таблицы меток есть.
	  //--------------------

	  myf.seek(poziz[jarus]);
			     //Вышли на нужную позицию по poziz[ ]
	  indtek=indnas[jarus];
	  //---------------------
      
	  skob=jmskob[jarus];    //Подготовка параметров для if-else:
	  ins=jins[jarus];
	  nas=jnas[jarus];
      //-------------------------
	  stroka:                      
    //*******
	  //------------------

	  tekpoz = myf.pos();  //Взяли N позиции читаемой
				      //далее строки.

	  //-------- Читаем строку и начинаем анализировать ее:
	  sitstr = myf.readLine();
	   if(sitstr.isEmpty()) //Не конец ли файла ?
	   {    myf.close( );                //Закрываем исчерпанный файл.

		  if(jarus==1)                //Не конец ли ком.файла ВООБЩЕ?
		  { str1=" #I - INTKOM - Конец командного файла. ";
		    printkl(str1);            //Сообщение о конце ком.файла.
		    jarus=0;
			kontext=0;
			sbros( );        //Сброс моего потока ВООБЩЕ.
		  }
		  //-----------------
		  //Будет продолжение старого, прерванного файла:
		   
    end:  if(incl==0)                                                           //***!!!
		  {unist(jarus);    //Уничтожили переменные яруса jarus.
		   unistpar(jarus); //Уничтожили лок.параметры яруса jarus.
		  }
		  else
          {unistpar(jarus);
            incl=0;
		  }
	//********	
	jarus=jarus-1; //К файлу верхнего яруса, так
			  goto   koleso;  //как исчерпан вложенный файл.

	    }
	    //------------ Закончили с концом файла --------------

	      //--------------------------

	   // Сначала обойдем комментарий или пустоту:
  
      k=sitstr.count( );     //Кол-во символов в строке.
      k=(k-1);                   //Последний индекс в строке.
	      for(i=0; i<=k; i++)
	 { if(sitstr.at(i)=='\t')    sitstr.replace(i,1,' ');  }
	      //Заменили табуляции на пробелы.        


      i=sitstr.indexOf('#');    if(i!=(-1))
      { for(j=i; j<=k; j++)  sitstr.replace(j,1,' '); }
	// Стерли хвост в строке, начиная с # , - наш комментарий.
    //-------------------------------------------------------
      i=sitstr.indexOf("//");  if(i!=(-1))
      { for(j=i; j<=k; j++)  sitstr.replace(j,1,' ');	}
	//Стерли сишный комментарий.
    //---------------------------------------------------
      str2=stand(sitstr);     if(str2=="")
	  { indtek=0; goto stroka; }  //Обошли пустую строку.				        
     //--------------------------------------------------
	 //          Теперь выбросим
	 //метку в строке, если она есть. Ищем ее, зная текущую
	 //позицию строки и список позиций /с метками/ в pozmet[]:

	  prizn=-1;   for(i=0; i<=kolmet; i++)
	   {   if(tekpoz==pozmet[i])   
		  prizn=dlmet[i];
	   }   //Если метки нет,то prizn не изменится.

		     if(prizn>=0)          //Индекс ':' в sitstr
	   {    for(i=0; i<=prizn; i++)    
		  sitstr.replace(i,1,' ');     //Метку стерли.

	      //Внимание: в sitstr метка не нужна при обработке
	      //этой строки, переход к ней - в другом месте 
	      //по номеру ее позиции из таблицы. 

	   }
	   //-----------------------------------
      str2=stand(sitstr);     if(str2=="")
	  { indtek=0; goto stroka; }  //Обошли пустую строку.				        
      //--------------------------------
	  //     Теперь обработка  содержательной строки:
	         if(incl!=0)     jarus=jarus-1;                               //***!!!
		  obrstr( );       //Обрабатываем sitstr с инде-
				           //кса indtek, в tekpoz -
				           //позиция этой строки.
		     if(incl!=0)                                                  //***!!!   
			 {if(perw!=0)   perw=0;		 
			   else  	 jarus=jarus+1;
			 }
	  if(perex==0) goto stroka; 
	  else if(perex==1) goto koleso;
	  else goto end;
			//---------------------------------

     }          //КОНЕЦ  ГЛАВНОЙ ФУНКЦИИ  - intkom( ) .

     //==============================================================


     void tabmet(void)          //Строит таблицы меток.
     { int i,j,k;
       QString str1,str2,str3;

       stek="tabmet";
       myf.seek(0);  //На начальную позицию.
       kolmet=-1;                 //Пока меток в массиве нет.
       //------------------------------
   aaa: tekpoz=myf.pos( );
	   str1 = myf.readLine();
			if( str1.isEmpty()) return;
	    //-------------------------
	  k=str1.count( ); k=(k-1); //Последний индекс в строке.  
         for(j=0;j<=k;j++)
		 {if(str1.at(j)=='\t') str1.replace(j,1,' '); } //Заменили табуляции на пробелы
	    i=str1.indexOf(':'); 
		  //i - индекс :   .
	    if(i==(-1))      goto aaa;
		//-------------------------
         str2=slowo(0,str1);
         if(i!=(pinds+1)) goto aaa; 
		  
	    //Нашли метку. Получили имя метки в str2.
	    //-----------------------------------
	    kolmet=kolmet+1;   if(kolmet>=rmetka)
	    { str1="#F - INTKOM - Мал массив меток.Задача снята.";
	      printkl(str1);  sbros( );
	    }   //-------------------------------
	    //      Теперь записываем данные по найденной метке:
	    metka[kolmet]=str2;
	    pozmet[kolmet]=tekpoz;
	    dlmet[kolmet]=i;   //Запись индекса символа  ':'  .  
		 goto aaa;
	    //------------------------ 
     }
     //===============================================================


     void sbros(void)           //Сбрасывает мой поток. Закрывает
     {QString s;
		 if(kontext!=0)
		 {printkl("Исходная строка:");
		  s="#    "+sitstrist; printkl(s);
		     kontext=0;
		 }	 
		 widasa=0; prkom=0;
		 mBreakCom=0;
	        fail();
	  if(jarus!=0)//файл при нештатном окончании.
	  myf.close( );
	// 		HWND p=GetDlgItem(hCom,IDC_DIR);//->
			//dirVis = TRUE;
	//::EnableWindow(p,TRUE);
	//::UpdateWindow(hCom);

	//		p=GetDlgItem(hCom,IDC_LINE);//->
			//lineVis = TRUE;
	//::EnableWindow(p,TRUE);

	//		HWND p1=GetDlgItem(hCom,IDC_STOP);//->
			//stopVis = FALSE;
			//breakVis = FALSE;
	//::EnableWindow(p1,FALSE);

	//if(pBp)
	//{
	//HWND p4=GetDlgItem(hBp,IDC_DELBP);//->
	//::EnableWindow(p4,TRUE);
	//HWND p5=GetDlgItem(hBp,IDC_DELALL);//->
	//::EnableWindow(p5,TRUE);

	//	
	//}

	//::SendMessage(hCom,WM_MDI191,1,1);
	ExitThread(TRUE);  
     }
     //================================================================

     QString stand(QString ss)      //Стандартизует строку ss.
     { int i,k; QString s;
	  s=ss; k=s.count( ); k=(k-1); //Последн.индекс.
	      for(i=0; i<=k; i++)
	 { if(s.at(i)=='\t')    s.replace(i,1,' ');  }
	      //Заменили табуляции на пробелы. 
		  s = s.trimmed();
	 
		  return(s);
     }
     //================================================================
     
     QString slowo(int n,QString ss)    //Возвращает из ss первое
     { int i,k; QChar a; QString s,str1; //слово от ее индекса - n,
		   //беря символы до разделителя или до конца строки. Если
           //слово - разделитель, то и выдастся сам разделитель.
	       // При этом в pinds будет индекс последнего символа слова, 
	       // а в ninds - индекс фактического начала слова. 
	 if(n>posind) osibind( );  s=ss;         
	for(i=0; i<n; i++)  s.replace(i,1,' '); //Стерли начало ненужное.
	   //-----------------------
	   k=s.count( ); k=(k-1);       //k - последний индекс в s.
	   ninds=(-1);  for(i=n; i<=k; i++)
	   { if(s.at(i)!=' ')  { ninds=i; break; }
	   }
	   if(ninds==(-1))
	   { printkl("#F - INTKOM -slowo: Пустое продолжение в строке");
	                     str1="#  "+sitstr; printkl(str1);
						 str1="# Функция  " + stek; printkl(str1);
						 printkl("# Задача снята");
	                     sbros( ); }
	   //------------------------ ninds готово.

	   a=s.at(ninds);                 //Первый символ слова.
		     if(razd(a)==1)              //Если a - разделитель.
	  {   for(i=ninds+1; i<=k; i++)    s.replace(i,1,' '); 
		     //Вписали пробелы в хвост.
			 s = s.trimmed();
	    
	    pinds=ninds; return(s);
	  }   //-------------- Далее, если слово не разделитель:
              for(i=ninds; i<=k; i++)
			  { a=s.at(i); if(razd(a)!=1)  pinds=i;
			                else   break;
			  }	                                // pinds готово.			
	  for(i=pinds+1; i<=k; i++) s.replace(i,1,' '); //Пробелы - в хвост.
	  s = s.trimmed();
	
			  return(s);     
     }
     //================================================================

     int razd(QChar a)          //Возвращает 1, если a - разделиnель.
     {                         //Иначе- 0.
       if((a=='{')||(a=='}')||(a=='(')||(a==')')||(a=='[')||(a==']'))
			return(1);
       if((a==';')||(a=='^')||(a=='=')||(a=='>')||(a=='<')||(a=='&'))
			return(1);
       if((a=='|')||(a=='+')||(a=='-')||(a=='/')||(a=='*')||(a==' '))
			return(1);
       if((a==',')||(a=='%')||(a=='"')|| (a==':')||(a=='~')||(a=='!'))
			return(1);
       if((a=='$')||(a=='\t')) return(1);//Знак табуляции - только для tabmet. 
	                                     //В obrstr будут вместо них пробелы.

			return(0);
     }
     //===============================================================
			
     QString dotzp(int n,QString ss,char a,char b)  //Возвращает набор 
     { int i,k;	         //с индекса n до символа 'a', но и не 
	QString s,str1;   	//далее ограничивающего символа 'b' и конца
	                    //строки. est: =1,если сработало 'a'; =0,если
	                    //сработало 'b'; =-1,если до конца строки.
	 if(n>posind) osibind( ); s=ss; for(i=0;i<n;i++) s.replace(i,1,' ');
					               //Стерли ненужное начало.
	 k=s.count( ); k=(k-1);    //Последний индекс в s.
	 //------------------------------
	   ninds=(-1);  for(i=n; i<=k; i++)
	   { if(s.at(i)!=' ')  { ninds=i; break; }
	   }
	   if(ninds==(-1))
	   { printkl("#F - INTKOM -dotzp: Пустое продолжение в строке");
                         str1="#  "+sitstr; printkl(str1);
						 str1="#  Функция  " + stek; printkl(str1);
						 printkl("# Задача снята");
	                     sbros( ); }
	   //------------------------------ ninds готово.
          est=-1; pinds=ninds;
		   for(i=ninds; i<=k; i++)
           { if(s.at(i)==a)      { est=1; break; }
		     else if(s.at(i)==b) { est=0; break; }
		       else                pinds=i;
		   } // ------------------------pinds  готово.
		   if((s.at(pinds)==a)||(s.at(pinds)==b))
		   {  return("");
		   }
       
	 //-------------------------------
	   for(i=pinds+1; i<=k; i++) s.replace(i,1,' '); //Очистили хвост.
	   if(s.at(pinds)==' ') 
	   { s.replace(pinds,1,'a');
	   s = s.trimmed();
	     
		     s.replace(pinds-ninds,1,' ');  return(s);
	   }
	   else
	   {
		   s = s.trimmed(); return(s);
	   }
     }
     //==============================================================


  QString dotzpecho(int n,QString ss,char a,char b)  //Возвращает набор 
     { int i,k;	         //с индекса n до символа 'a', но и не 
	QString s,str1;   	//далее ограничивающего символа 'b' и конца
	 QChar p,p1;            //строки. est: =1,если сработало 'a'; =0,если
	                    //сработало 'b'; =-1,если до конца строки.
	 if(n>posind) osibind( ); s=ss; for(i=0;i<n;i++) s.replace(i,1,' ');
					               //Стерли ненужное начало.
	 k=s.count( ); k=(k-1);    //Последний индекс в s.
	 //------------------------------
	   ninds=n;                    //Не губит пробелы в строке - для echo. 
	   //------------------------------ ninds готово - где задано в аргум.
          est=-1; pinds=ninds;
		   for(i=ninds; i<=k; i++)
           { if(s.at(i)==a)      { est=1; break; }
		     else if(s.at(i)==b) { est=0; break; }
		       else                pinds=i;
		   } // ------------------------pinds  готово.
		   if((s.at(pinds)==a)||(s.at(pinds)==b))
		   {pinds=pinds-1; ninds=ninds-1; return("");
		   }
       
	 //-------------------------------
	   for(i=pinds+1; i<=k; i++) s.replace(i,1,' '); //Очистили хвост.
                  p=s.at(ninds);     p1=s.at(pinds);
					s.replace(ninds,1,'d'); s.replace(pinds,1,'d');
					s = s.trimmed();
	     
		            s.replace(0,1,p);       s.replace((pinds-ninds),1,p1);
		       return(s);
     }
     //==============================================================


     void obrstr(void)                 //Обрабатывает строку sitstr
  {    int i,j,k,ii,jj=0;                //в позиции tekpoz файла 
   QString str1,str2,str3,str4,str5,sit;   //с индекса indtek этой строки.
	   short tild,gal;
//============================	   
DWORD tekpozzap; int indtekzap,tekp,indt,pelse;
 QString str6,sitstrzap; BOOL d;
 int posindzap;
//===========================
	                           kontext=0;  
							   sitstr = sitstr.trimmed();
							   //sitstr.TrimRight();
	  k=sitstr.count( ); posind=k-1;
	 //---------------------  Получили последний индекс строки.
	     rom="";            //Заготовка строки команд отладчика.
	 //Начинаем обработку строки:


   i=sitstr.indexOf("putenv");            if(i>=0)
   {str1=dotzpecho(i,sitstr,';',';');
      if(est==(-1))   //Вышли на конец строки
	  { }                                                          
	  else
	  {i=pinds+1;
	   str1=dotzpecho(i,sitstr,'$','$');  if(est!=(-1))  //Есть $ 
		{printkl("# F - Командный файл снят на строке:");
         str2="# "+sitstr; printkl(str2);
		 printkl("# Откорректируйте файл, чтобы оператор putenv");
         printkl("# был последним в своей строке.");
 
		        sbros( );
		}
      }
   }
//-----------------------------------------------



       sit=sitstr;
       sitstrist=sitstr;    //Запомнили непреобразованую строку
           stek="kontekct";
   str1=dotzpecho(0,sitstr,'$','$'); if(est!=(-1)) //Есть $
   {
	   int n, m;  QChar a;char *b; short prwstawka;
                kontext=1;
                prwstawka=0;  //пока еще нет информации о вставке.
  if(nowstr!=0)    //была новая строка в propusk
  {nowstr=0; //чтобы далее не мешалась
       if(indtek!=0)   //подозрительно
	   {sitstr=wstawka(sitstr);
         prwstawka=1;
	   }
  }//-------------------------------------

   n=0;
   str4=""; indzapobrstr=indtek;
  cont: if(n>posind) goto cont1; 
    str1=dotzpecho(n,sitstr,'$','$');
		if(est!=(-1))    //Есть $
	{str4=str4+str1; 
		indtek=pinds+1; //На $
      str1=slowo(indtek,sitstr);
	  if(str1!="$") {printkl("#F - INTKOM - OSIBKA 1"); osibind( ); }
 indtek=pinds+1; //На ( или др.- за $.
	  str1=slowo(indtek,sitstr); 
	   if(str1!="(")  //Нет контекстной замены
      {str4=str4+"$";
	   n=ninds; goto cont;}    
      //--------------------------------------
       indtek=pinds+1;   //За (
       str1=dotzpecho(indtek,sitstr,')',')');
              if(est==(-1))
      {printkl("#E - INTKOM - Нет ) после $ в строке:");
	   str1="# "+sit; printkl(str1); sbros( );}
      //--------------------------------------
           //Теперь в str1 имя
			  str1 = str1.trimmed();
      
	  a=str1.at(0); //Первый символ
	  if((a=='0')||(a=='1')||(a=='2')||(a=='3')||(a=='4')||
         (a=='5')||(a=='6')||(a=='7')||(a=='8')||(a=='9'))
		   //------------------Контекст по параметрам command:
		{if(incl!=0)
			{printkl("# Нельзя передавать пераметры инклюдируемому файлу. Ошибка");
	                           sbros( );
			}
		  m=rasp(str1);  if(m!=1)
			{printkl("#E - INTKOM - Ошибка индекса параметра в строке:");
	         str2="# "+sit; printkl(str2); sbros( );
			}//---------------------------
          m=(int)fpere(str1);
		  i=(-1);  for(j=0;j<=kolpar;j++)
		  {if(jrpar[j]!=jarus)  continue;
		   if(nomer[j]==m) {i=j; break;}  //Нашли 
		  }//------------------------------
		   if(i==(-1))
		   {/*printkl("#E - INTKOM - Не найден параметр по индексу");
		    str2="# "+str1; printkl(str2);
			str2="# в строке  "+sit; printkl(str2);*/
			str5="";
		   }//------------------------------
           else str5=par[i];     //Взятый параметтр

           str4=str4+str5;  //Дописали его
		   indtek=pinds+1;  //На )
		   str1=slowo(indtek,sitstr);
		   if(str1!=")")
		   {printkl("#F - INTKOM - Ошибка 3 в obrstr."); osibind( );}
           n=pinds+1;
		   goto cont;

		}//-------------------------------------------------
	            else //------Контекст по глобальным именам
		{b=getenv(str1.toStdString().c_str());
         str5=(char *)b; //Текст по его имени
		      if(str5=="")
	{	}
             //---- Теперь - текст 
          str4=str4+str5;       //Дописали его
		   indtek=pinds+1;      //На )
          str1=slowo(indtek,sitstr);  //Вышли на )
		  if(str1!=")")
	{ str2="#F - INTKOM - OSIBKA 4 ="+str1; printkl(str2);sbros( );}
		  n=pinds+1;  //На продолжение в строке sitstr
		        goto cont;

		}//Конец глобальных имен
	}
    else str4=str4+str1;    //Дописывание хвоста без $  
cont1:    sitstr=str4;  //Почти Окончательная замена sitstr
         if(prwstawka!=0)     //была вставка
		 {sitstr=snatwstawka(sitstr);
            indtek=indtekwst;
            prwstawka=0;
			sitstr = sitstr.trimmed();
            //sitstr.TrimRight( );
            k=sitstr.count( ); posind=k-1; 
         }
		 else
		 {
			 sitstr = sitstr.trimmed();
			 //sitstr.TrimRight( ); //Мог быть в конце пробел при ненайденном параметре.
	       k=sitstr.count( ); posind=k-1; indtek=indzapobrstr;
		 }
}//--------- Конец учета контекстных замен ----------
   nowstr=0;    //чтобы далее не мешалась: propusk мог перевести на строку без $.
  //-------------------------------------------------------------------------------


    frasa:                      
		 if(indtek>posind)
		 {  splu( ); perex=0; indtek=0; return; }
		
	 //-------------------------
	stek="frasa";	 
	str1=slowo(indtek,sitstr);		  
	if((str1=="{")||(str1=="}"))            
	{ if(str1=="{")  skob=(char)((short)skob+1);
      if(str1=="}")  skob=(char)((short)skob-1);
//          if((skob<0)||(skob>100))
//{printkl("#E - INTKOM - Неправильность в фигурных скобках."); sbros( );}

		  indtek=pinds+1; goto frasa;  
	} //-------------------------
    //Теперь содержательное предписание в sitstr с indtek:
    //********************************************************
      //Ищем имя по str1 на предмет - не присвоение ли это .
      stek="Имя"; prizn=(-1);
                                                                               
       str2=slowo2(indtek,sitstr);       if(kws==1)  // В str2  имя элемета массива
	   {str2=istima(str2);      //истинное его имя
	     prizn=indimper(str2);  //индекс элемента
                goto bbb;
       }//---------------------------


	  for(i=0; i<=kolperem; i++)  
	    { if(glst[i]!=jarus) continue;
		  if(imper[i]==str1)  { prizn=i; break; }  }
bbb:    if(prizn>=0)         //Нашли имя.                                       
       { splu( ); perex=0; indtek=pinds+1; str2=slowo(indtek,sitstr);
	      if(str2!="=")
	   { printkl("#E - INTKOM - Нет знака '=' в присвоениеи в строке ");
		  sitstr="# "+sitstr; printkl(sitstr);
	     printkl("# Задача снята."); sbros( ); }
	 //--------------------------
	   indtek=pinds+1;
	   prisw( );       //Вычисляет значение после '=' и 
			           //присваивает его znper[prizn],
			           //оставляя indtek на последнем
			           //символе  ';' или на конце строки.
	   indtek=indtek+1;        goto  frasa;
	} //Конец присвоения *************************************

//********************************************************************************************************
      else if(str1=="if")
      { stek="if";

	    splu( ); perex=0; indtek=pinds+1; str2=slowo(indtek,sitstr);
	      if(str2!="(")
{ printkl("#E - INTKOM - Нет левой скобки после if.Задача снята.См.строку");
		  str2="# "+sitstr; printkl(str2); sbros( );}
       indtek=pinds;     
	  uslif( );  // Работает от indtek, указывающем на '(' .
		         //Оставляет indtek на ')'  .
		         //Ответ в otw. 1 - условие выполнено.

//============================================================================================

	                  pelse=0;  //=0, если if без else. Если if  с else,  то =1
					  sitstrzap=sitstr;  posindzap=posind;
	  indtekzap=indtek; tekpozzap=tekpoz;
	  indtek=indtek+1; //Для propusk1
	                              propusk1();                                //совпадает с propusk()
                          indt=indtek; tekp=tekpoz; //Результат propusk1
             if(tekpoz==tekpozzap)   //Стоим на той же строке после  propusk
	 {sitstr=sitstrzap; //Уже ранее обработанная строка, нужная.
      posind=posindzap; //Восстановили
	 } //т.е. tekpoz определил именно эту строку, он не изменился, indtek - за пропущенный оператор.

             else   //propusk дал другую строку по новому tekpoz. Строку надо прочитать
	{myf.seek(tekpoz); //позицуионировали на новую строку
	 //d=myf.ReadString(sitstr); //Прочитали строку в позиции нового tekpoz 
	 sitstr = myf.readLine();

	 if(sitstr.isEmpty()) goto ff;
	 sitstr=normstr(sitstr); posind=sitstr.count()-1;
	}// indtek - за пропущенный оператор.
         //-- Теперь в sitstr строка с пропущенным оператором, indtek стоит за ним.
         //tekpoz соответствует этой строке, posind определяетт ее последний индекс
			       //-- Поиск следующего оператора, не else  ли он ?
			                if(indtek<=posind)  //т.е. следующий оператор в этой строке
							{str6=slowo(indtek,sitstr); if(str6=="else") {pelse=1; goto ff;}
							                                        else {pelse=0; goto ff;}
							}//-------- конец при операторе в этой строке
				//Остается оценить последующий оператор в последующей непустой строке в ее начале:
						p31: tekpoz=myf.pos();  //позиция далее читаемой строки
							sitstr = myf.readLine();
	 if(sitstr.isEmpty()) goto ff;
	 sitstr=normstr(sitstr);  if(sitstr=="") goto p31;
	    //--------------- Теперь непустая строка:

             posind=sitstr.count()-1;
			str6=slowo(0,sitstr); if(str6=="else") {pelse=1; goto ff;}
							                       else {pelse=0; goto ff;}
			 
ff: 
	tekpoz=tekpozzap; indtek=indtekzap;

	posind=posindzap;
	myf.seek(tekpoz);
    sitstr=sitstrzap;
//=============================================================================================

		  if(otw==1)
		  { indtek=indtek+1;
		  str6 = myf.readLine();
            //d=myf.ReadString(str6);   //&&&&&&!!!!
		    if(pelse==0) goto frasa;                                       //коррекция
            ins=ins+1; if(ins>=relse)                                   
	{printkl("#F - INTKOM - Мал массив для if-else.З-ча снята на строке "); 
			 str2="# "+sitstr; printkl(str2); sbros( ); }               
			 inase[ins]='+';  mskob[ins]=skob;                                          
		  goto frasa;
		  } 
		  else
          {
           myf.seek(tekp); //На новую позицию.
           indtek=indt;
    		if(pelse==0) return;                                                //коррекция                                            
            ins=ins+1; if(ins>=relse)                                   
			{printkl("#F - INTKOM - Мал массив для if-else.З-ча снята на строке "); 
			 str2="# "+sitstr; printkl(str2); sbros( ); }               
             inase[ins]='-';   mskob[ins]=skob;                                                                                   
		    return;
		  }                       //На продолжение.
       } //Конец обработки условия if.*****************************

  
        else if(str1=="else")
		{
			stek="else"; splu( ); perex=0; indtek=pinds+1;   //За else
		                 for(ii=ins;ii>=nas;ii=ii-1)                      
	{if(ii==nas) {printkl("#E,F - INTKOM - Для else не найден  if .");
							   osibind( ); }
				if((mskob[ii]==skob)&&(inase[ii]!=0)) {jj=ii;  break;}

			}
		  if(inase[jj]=='+')         //Условие в if было выполнено,
		  {inase[jj]=0; if(jj==ins) ins=ins-1; //Надо обойти оператор.
		    propusk( );                //Обходим оператор. 
		 	myf.seek(tekpoz);  return;
		  }
		  else if(inase[jj]=='-')    //Условие в if было не выполнено,
		  {inase[jj]=0; if(jj==ins) ins=(ins-1); 
		                           //надо исполнить оператор.
                  goto frasa;         //Переход для исполнения.
		  }
	      else
{ printkl("#F - INTKOM - Ошибка записи в стек if-else.З-ча снята на строке");
		    str2="# "+sitstr; printkl(str2); sbros( ); 
		  }
		}
      //Конец обработки else .*************************************

//********************************************************************************************************

   
      else if(str1=="goto")
	  { stek="goto"; if(skob==0) ins=nas; 
	    splu( ); perex=0; indtek=pinds+1; str2=slowo(indtek,sitstr);
				//Взяли имя метки перехода.
      prizn=-1;   for(i=0; i<=kolmet; i++)  //Ищем ее в массиве:
	  { if( metka[i]==str2)   { prizn=i;  break; } }
	 if( prizn==(-1))
	  { str1=" #E - INTKOM - Ошибка метки перехода в строке ";
	   str1=(str1+sitstr);
	   printkl(str1);        sbros( );
	   printkl("# Задача снята.");
	  }
	 //------------------------
	      //Нашли метку, ее индекс в prizn.
	myf.seek(pozmet[prizn]);  indtek=0;
	     return;
    } //Конец обработки goto.**********************************

      else if(str1=="jal")
	  { stek="jal";
	    splu( ); perex=0; indtek=pinds+1; str2=slowo(indtek,sitstr);
				//Взяли имя метки перехода.
      prizn=-1;   for(i=0; i<=kolmet; i++)  //Ищем ее в массиве:
	  {   if( metka[i]==str2)   { prizn=i;  break; } }
	 if( prizn==(-1))
	  { str1=" #E - INTKOM - Ошибка метки перехода в строке ";
	   str1=(str1+sitstr);
	   printkl(str1); printkl("Задача снята.");
       sbros( );
	  }
	 //------------------------
	      //Нашли метку, ее индекс в prizn.
	myf.seek(pozmet[prizn]);  
	  wzw[indwzw]=tekpoz;
	  if(pinds==posind) goto skok;
	  indtek=pinds+1;       //К индексу ';' в этой строке.
	  str3=slowo(indtek,sitstr);  //Это  ';' .
	  if(str3!=";")
	  { printkl("#E - INTKOM - З-ча снята. Нет ';' в строке: ");
	    str3="# "+sitstr; printkl(str3); sbros( );
	  }
     skok: indtek=pinds+1;      // За  ;  . 
	  indpr[indwzw]=indtek; //Возврат подготовлен.
	  indwzw=indwzw+1;      //Сдвинули вершину стека.
	  //-------------------------
	  if(indwzw>=rwzw)
    { printkl("# Задача снята.Массив переходов с возвратом мал.");
	                                         sbros( );}	    
	     indtek=0;  return;
    } //Конец обработки jal.**********************************

	 else if(str1=="jr")
	 { stek="jr";  splu( ); perex=0;    if(indwzw<=0)
{printkl(" #E - INTKOM - Ошибки программир.возвратов по jr. См строку:");
	    str2="# "+sitstr; printkl(str2); 
	    printkl("Задача снята"); sbros( );}
      indwzw=(indwzw-1); tekpoz=wzw[indwzw]; indtek=indpr[indwzw];
	 //Взяли номер позиции и индекс в строке продолжения.
	    myf.seek(tekpoz);      return;
    } //Конец обработки jr.************************************

	 
	 else if(str1.compare("command",Qt::CaseInsensitive ) ==0)

	 { stek="command"; splu( ); perex=1;   //Для перехода на koleso.
      indtek=pinds+1;
      //-----------------------------
            if(incl!=0)                                                           //***!!!
			{printkl("# Нельзя в инклюдируемом файле использовать command. Ошибка.");
			  jarus=jarus+1;       sbros( );
			}
    //=================================================
      str2=dotzp(indtek,sitstr,' ',';');    //Взяли имя файла в str2.
               if(est==1)      //будут далее параметры
	{indtek=pinds+1;
      str5=dotzp(indtek,sitstr,';',';');  //В str5 - хвост от command
	     xwostcom(str5);                 //Передает параметры
	}//----------------------------------------------
    //=================================================
	  jmskob[jarus]=skob;  //Запомин. для if-else:
	  jins[jarus]=ins;
	  jnas[jarus]=nas;
	  //---------------------
      poziz[jarus]=tekpoz;        //Позиция для продолжения.
	  indtek=pinds+1;             //К индексу ';' в этой строке или
	                              // за конец строки.
	  if(indtek>posind)  goto sour;
	  str3=slowo(indtek,sitstr);  //Это  ';' .
	  if(str3!=";")
	  { printkl("#E - INTKOM - З-ча снята. Нет ';' в строке: ");
	    str3="# "+sitstr; printkl(str3); sbros( );
	  }
       indtek=pinds+1;        // За  ;  . 
    sour:  indnas[jarus]=indtek;   //Индекс для продолжения.
	  //Для продолжения все сделано. Теперь - к новому файлу:
      jarus=jarus+1;    if(jarus>=rsource)
{ printkl(" #F - INTKOM - Мал массив имен вложенных файлов. Задача снята.");
	     sbros( );
       }  //--------------------------
       source[jarus]=str2; poziz[jarus]=0; indnas[jarus]=0;
	   jmskob[jarus]=0;
	   jins[jarus]=ins;
	   jnas[jarus]=ins;
	     //Переход на вложенный файл подготовлен.
          myf.close( );
		  return;
    } //Конец обработки command.************************************


	 else if(str1=="include")

	 { stek="include"; splu( ); perex=1;   //Для перехода на koleso.
      indtek=pinds+1;
      //-----------------------------
            if(incl!=0)                                                           //***!!!
			{printkl("# Нельзя в инклюдируемом файле использовать includ. Ошибка.");
			      jarus=jarus+1;   sbros( );
			}

    //=================================================
      str2=dotzp(indtek,sitstr,' ',';');    //Взяли имя файла в str2.
               if(est==1)      //будут далее параметры
	{indtek=pinds+1;
      str5=dotzp(indtek,sitstr,';',';');  //В str5 - хвост от command
	     xwostcom(str5);                 //Передает параметры
	}//----------------------------------------------
    //=================================================
	  jmskob[jarus]=skob;  //Запомин. для if-else:
	  jins[jarus]=ins;
	  jnas[jarus]=nas;
	  //---------------------
      poziz[jarus]=tekpoz;        //Позиция для продолжения.
	  indtek=pinds+1;             //К индексу ';' в этой строке или
	                              // за конец строки.
	  if(indtek>posind)  goto sour1;
	  str3=slowo(indtek,sitstr);  //Это  ';' .
	  if(str3!=";")
	  { printkl("#E - INTKOM - З-ча снята. Нет ';' в строке: ");
	    str3="# "+sitstr; printkl(str3); sbros( );
	  }
       indtek=pinds+1;        // За  ;  . 
    sour1:  indnas[jarus]=indtek;   //Индекс для продолжения.
	  //Для продолжения все сделано. Теперь - к новому файлу:
      jarus=jarus+1;    if(jarus>=rsource)
{ printkl(" #F - INTKOM - Мал массив имен вложенных файлов. Задача снята.");
	     sbros( );
       }  //--------------------------
       source[jarus]=str2; poziz[jarus]=0; indnas[jarus]=0;
	   jmskob[jarus]=0;
	   jins[jarus]=ins;
	   jnas[jarus]=ins;
	     //Переход на вложенный файл подготовлен.
          myf.close( );
                 incl=1; perw=1;                                                //***!!!
		  return;
    } //Конец обработки include.************************************




	  else if(str1=="char")
	  { stek="char"; splu( );  perex=0; indtek=pinds+1;
aa1:    str3=slowo2(indtek,sitstr); 
		        if(kws==0)
		{ str2=slowo(indtek,sitstr);  //Взяли имя внутр. переменной
		                                 kontrima(str2,jarus);    
	     kolperem=kolperem+1; if(kolperem>=rimper)
		 { printkl(" #F - INTKOM - Массив имен переменных мал.Задача снята.");
	     sbros( );
		 } //-------------------------------
	     imper[kolperem]=str2;
	      znper[kolperem]=0;  //Инициализация новой переменной нулем.
	      tipper[kolperem]=1; //Это тип char.
	       glst[kolperem]=jarus;
	      //Все зафиксировали.----------------
		      indtek=pinds+1;
           if(indtek>posind) goto frasa;
	       str2=slowo(indtek,sitstr);
	       if(str2==",") { indtek=pinds+1; goto aa1; }
           if(str2!=";") { printkl("#E - INTKOM - Нет ';' в об'явлении.Задача снята.");
           printkl("# См. строку: "); str2="# "+sitstr; printkl(str2);
	       sbros( );}

	   //-----------------------------------
	      indtek=pinds+1;       goto frasa;
		}//--------------- конец при об'явлении переменной
				else      //об'явление массива
		{int n,i;
          str3=slowo(indtek,sitstr);  //Имя массива до  [
          indtek=pinds+1;    //на взятие  [
		  str4=str3+"[";     //имя массива[
		  str3=slowo(indtek,sitstr);   //это [
		     indtek=pinds;      //на [
		        if(str3!="[")
			{printkl("#F - INTKOM - Ошибка имени массива в строке:");
				str3="# "+sitstr; printkl(str3); sbros();
			}//--------------------------
				indtek=pinds+1;  //на начало размера массива
           str3=slowo(indtek,sitstr);   //Это текст размера массива
		                     n=rasp(str3);   if(n!=1)
           {printkl("#F - INTKOM - Ошибка 1 индекса в об'явлении массива в строке:");
		     str3="# "+sitstr; printkl(str3); sbros();
           }//-----------------------------------------
                 n=(int)fpere(str3);  //Числовой размер массива
                      if(n<1)
           {printkl("#F - INTKOM - Ошибка 2 индекса в об'явлении массива в строке:");
		     str3="# "+sitstr; printkl(str3); sbros();
           }//-----------------------------------------
                    for(i=0;i<n;i++)   //Запись элементов массива в систему
			{str3.sprintf("%d",i); //текст значения  i
              str5=str4+str3; str5=str5+"]"; //str5 - Чистое имя элемента массива
			                                   kontrima(str5,jarus);    
                    kolperem=kolperem+1;
					if(kolperem>=rimper)
				{printkl("#F - INTKOM - Массив имен переменных мал.Задача снята");
                 printkl("#  при об'явлении массива.");   sbros();
				}//---------------------------------------
					  imper[kolperem]=str5;
                      znper[kolperem]=0;         
					  tipper[kolperem]=1;
                      glst[kolperem]=jarus; //Зафиксировали элемент
			}//----- Зафиксировали элементы всего массива
					indtek=pinds+1; //на взятие ]
            str3=slowo(indtek,sitstr);   //Это ]
			  indtek=pinds;               //на ]
                         if(str3!="]")
			{printkl("#F - INTKOM - Ошибка с  ]  при об'явлении массива в строке:");
              str3="# "+sitstr; printkl(str3); sbros();
			}//---------------------------------------------
		      indtek=pinds+1;
           if(indtek>posind) goto frasa;
	       str2=slowo(indtek,sitstr);
	       if(str2==",") { indtek=pinds+1; goto aa1; }
           if(str2!=";") { printkl("#E - INTKOM - Нет ';' в об'явлении.Задача снята.");
           printkl("# См. строку: "); str2="# "+sitstr; printkl(str2);
	       sbros( );}

	   //-----------------------------------
		      indtek=pinds+1;
	            goto frasa;
	}
   }//Конец обработки char.***************************************


	  else if(str1=="UCHAR")
	  { stek="UCHAR"; splu( );  perex=0; indtek=pinds+1;
aa12:    str3=slowo2(indtek,sitstr); 
		        if(kws==0)
		{ str2=slowo(indtek,sitstr);  //Взяли имя внутр. переменной
		                                      kontrima(str2,jarus);    
	     kolperem=kolperem+1; if(kolperem>=rimper)
		 { printkl(" #F - INTKOM - Массив имен переменных мал.Задача снята.");
	     sbros( );
		 } //-------------------------------
	     imper[kolperem]=str2;
	      znper[kolperem]=0;  //Инициализация новой переменной нулем.
	      tipper[kolperem]=2; //Это тип UCHAR.
	       glst[kolperem]=jarus;
	      //Все зафиксировали.----------------
		      indtek=pinds+1;
           if(indtek>posind) goto frasa;
	       str2=slowo(indtek,sitstr);
	       if(str2==",") { indtek=pinds+1; goto aa12; }
           if(str2!=";") { printkl("#E - INTKOM - Нет ';' в об'явлении.Задача снята.");
           printkl("# См. строку: "); str2="# "+sitstr; printkl(str2);
	       sbros( );}

	   //-----------------------------------
	      indtek=pinds+1;       goto frasa;
		}//--------------- конец при об'явлении переменной
				else      //об'явление массива
		{int n,i;
           str3=slowo(indtek,sitstr);  //Имя массива до  [
          indtek=pinds+1;    //на взятие  [
		  str4=str3+"[";     //имя массива[
		  str3=slowo(indtek,sitstr);   //это [
		     indtek=pinds;      //на [
		        if(str3!="[")
			{printkl("#F - INTKOM - Ошибка имени массива в строке:");
				str3="# "+sitstr; printkl(str3); sbros();
			}//--------------------------
				indtek=pinds+1;  //на начало размера массива
           str3=slowo(indtek,sitstr);   //Это текст размера массива
		                     n=rasp(str3);   if(n!=1)
           {printkl("#F - INTKOM - Ошибка 1 индекса в об'явлении массива в строке:");
		     str3="# "+sitstr; printkl(str3); sbros();
           }//-----------------------------------------
                 n=(int)fpere(str3);  //Числовой размер массива
                      if(n<1)
           {printkl("#F - INTKOM - Ошибка 2 индекса в об'явлении массива в строке:");
		     str3="# "+sitstr; printkl(str3); sbros();
           }//-----------------------------------------
                    for(i=0;i<n;i++)   //Запись элементов массива в систему
			{str3.sprintf("%d",i); //текст значения  i
              str5=str4+str3; str5=str5+"]"; //str5 - Чистое имя элемента массива
			                                       kontrima(str5,jarus);         
                    kolperem=kolperem+1;
					if(kolperem>=rimper)
				{printkl("#F - INTKOM - Массив имен переменных мал.Задача снята");
                 printkl("#  при об'явлении массива.");   sbros();
				}//---------------------------------------
					  imper[kolperem]=str5;
                      znper[kolperem]=0;         
					  tipper[kolperem]=2;
                      glst[kolperem]=jarus; //Зафиксировали элемент
			}//----- Зафиксировали элементы всего массива
					indtek=pinds+1; //на взятие ]
            str3=slowo(indtek,sitstr);   //Это ]
			  indtek=pinds;               //на ]
                         if(str3!="]")
			{printkl("#F - INTKOM - Ошибка с  ]  при об'явлении массива в строке:");
              str3="# "+sitstr; printkl(str3); sbros();
			}//---------------------------------------------
		      indtek=pinds+1;
           if(indtek>posind) goto frasa;
	       str2=slowo(indtek,sitstr);
	       if(str2==",") { indtek=pinds+1; goto aa12; }
           if(str2!=";") { printkl("#E - INTKOM - Нет ';' в об'явлении.Задача снята.");
           printkl("# См. строку: "); str2="# "+sitstr; printkl(str2);
	       sbros( );}

	   //-----------------------------------
		     indtek=pinds+1;
	            goto frasa;
	}
   }//Конец обработки UCHAR.***************************************


	  else if(str1=="short")
	  { stek="UINT"; splu( );  perex=0; indtek=pinds+1;
aa2:   str3=slowo2(indtek,sitstr); 
		        if(kws==0)
		{ str2=slowo(indtek,sitstr);  //Взяли имя внутр. переменной
		                                    kontrima(str2,jarus);    
	     kolperem=kolperem+1; if(kolperem>=rimper)
		 { printkl(" #F - INTKOM - Массив имен переменных мал.Задача снята.");
	     sbros( );
		 } //-------------------------------
	     imper[kolperem]=str2;
	      znper[kolperem]=0;  //Инициализация новой переменной нулем.
	      tipper[kolperem]=3; //Это тип short.
	       glst[kolperem]=jarus;
	      //Все зафиксировали.----------------
		      indtek=pinds+1;
           if(indtek>posind) goto frasa;
	       str2=slowo(indtek,sitstr);
	       if(str2==",") { indtek=pinds+1; goto aa2; }
           if(str2!=";") { printkl("#E - INTKOM - Нет ';' в об'явлении.Задача снята.");
           printkl("# См. строку: "); str2="# "+sitstr; printkl(str2);
	       sbros( );}

	   //-----------------------------------
	      indtek=pinds+1;       goto frasa;
		}//--------------- конец при об'явлении переменной
				else      //об'явление массива
		{int n,i;
           str3=slowo(indtek,sitstr);  //Имя массива до  [
          indtek=pinds+1;    //на взятие  [
		  str4=str3+"[";     //имя массива[
		  str3=slowo(indtek,sitstr);   //это [
		     indtek=pinds;      //на [
		        if(str3!="[")
			{printkl("#F - INTKOM - Ошибка имени массива в строке:");
				str3="# "+sitstr; printkl(str3); sbros();
			}//--------------------------
				indtek=pinds+1;  //на начало размера массива
           str3=slowo(indtek,sitstr);   //Это текст размера массива
		                     n=rasp(str3);   if(n!=1)
           {printkl("#F - INTKOM - Ошибка 1 индекса в об'явлении массива в строке:");
		     str3="# "+sitstr; printkl(str3); sbros();
           }//-----------------------------------------
                 n=(int)fpere(str3);  //Числовой размер массива
                      if(n<1)
           {printkl("#F - INTKOM - Ошибка 2 индекса в об'явлении массива в строке:");
		     str3="# "+sitstr; printkl(str3); sbros();
           }//-----------------------------------------
                    for(i=0;i<n;i++)   //Запись элементов массива в систему
			{str3.sprintf("%d",i); //текст значения  i
              str5=str4+str3; str5=str5+"]"; //str5 - Чистое имя элемента массива
			                                  kontrima(str5,jarus);           
                    kolperem=kolperem+1;
					if(kolperem>=rimper)
				{printkl("#F - INTKOM - Массив имен переменных мал.Задача снята");
                 printkl("#  при об'явлении массива.");   sbros();
				}//---------------------------------------
					  imper[kolperem]=str5;
                      znper[kolperem]=0;         
					  tipper[kolperem]=3;
                      glst[kolperem]=jarus; //Зафиксировали элемент
			}//----- Зафиксировали элементы всего массива
					indtek=pinds+1; //на взятие ]
            str3=slowo(indtek,sitstr);   //Это ]
			  indtek=pinds;               //на ]
                         if(str3!="]")
			{printkl("#F - INTKOM - Ошибка с  ]  при об'явлении массива в строке:");
              str3="# "+sitstr; printkl(str3); sbros();
			}//---------------------------------------------
		      indtek=pinds+1;
           if(indtek>posind) goto frasa;
	       str2=slowo(indtek,sitstr);
	       if(str2==",") { indtek=pinds+1; goto aa2; }
           if(str2!=";") { printkl("#E - INTKOM - Нет ';' в об'явлении.Задача снята.");
           printkl("# См. строку: "); str2="# "+sitstr; printkl(str2);
	       sbros( );}

	   //-----------------------------------
		      indtek=pinds+1;
	            goto frasa;
	}
   }//Конец обработки short.***************************************



	  else if(str1=="USHORT")
	  { stek="USHORT"; splu( );  perex=0; indtek=pinds+1;
aa24:   str3=slowo2(indtek,sitstr); 
		        if(kws==0)
		{ str2=slowo(indtek,sitstr);  //Взяли имя внутр. переменной
		                                         kontrima(str2,jarus);    
	     kolperem=kolperem+1; if(kolperem>=rimper)
		 { printkl(" #F - INTKOM - Массив имен переменных мал.Задача снята.");
	     sbros( );
		 } //-------------------------------
	     imper[kolperem]=str2;
	      znper[kolperem]=0;  //Инициализация новой переменной нулем.
	      tipper[kolperem]=4; //Это тип USHORT.
	       glst[kolperem]=jarus;
	      //Все зафиксировали.----------------
		      indtek=pinds+1;
           if(indtek>posind) goto frasa;
	       str2=slowo(indtek,sitstr);
	       if(str2==",") { indtek=pinds+1; goto aa24; }
           if(str2!=";") { printkl("#E - INTKOM - Нет ';' в об'явлении.Задача снята.");
           printkl("# См. строку: "); str2="# "+sitstr; printkl(str2);
	       sbros( );}

	   //-----------------------------------
	      indtek=pinds+1;       goto frasa;
		}//--------------- конец при об'явлении переменной
				else      //об'явление массива
		{int n,i;
           str3=slowo(indtek,sitstr);  //Имя массива до  [
          indtek=pinds+1;    //на взятие  [
		  str4=str3+"[";     //имя массива[
		  str3=slowo(indtek,sitstr);   //это [
		     indtek=pinds;      //на [
		        if(str3!="[")
			{printkl("#F - INTKOM - Ошибка имени массива в строке:");
				str3="# "+sitstr; printkl(str3); sbros();
			}//--------------------------
				indtek=pinds+1;  //на начало размера массива
           str3=slowo(indtek,sitstr);   //Это текст размера массива
		          
           n=rasp(str3);   if(n!=1)
           {printkl("#F - INTKOM - Ошибка 1 индекса в об'явлении массива в строке:");
		     str3="# "+sitstr; printkl(str3); sbros();
           }//-----------------------------------------
                 n=(int)fpere(str3);  //Числовой размер массива
                      if(n<1)
           {printkl("#F - INTKOM - Ошибка 2 индекса в об'явлении массива в строке:");
		     str3="# "+sitstr; printkl(str3); sbros();
           }//-----------------------------------------
                    for(i=0;i<n;i++)   //Запись элементов массива в систему
			{str3.sprintf("%d",i); //текст значения  i
              str5=str4+str3; str5=str5+"]"; //str5 - Чистое имя элемента массива
			                                 kontrima(str5,jarus);           
                    kolperem=kolperem+1;
					if(kolperem>=rimper)
				{printkl("#F - INTKOM - Массив имен переменных мал.Задача снята");
                 printkl("#  при об'явлении массива.");   sbros();
				}//---------------------------------------
					  imper[kolperem]=str5;
                      znper[kolperem]=0;         
					  tipper[kolperem]=4;
                      glst[kolperem]=jarus; //Зафиксировали элемент
			}//----- Зафиксировали элементы всего массива
					indtek=pinds+1; //на взятие ]
            str3=slowo(indtek,sitstr);   //Это ]
			  indtek=pinds;               //на ]
                         if(str3!="]")
			{printkl("#F - INTKOM - Ошибка с  ]  при об'явлении массива в строке:");
              str3="# "+sitstr; printkl(str3); sbros();
			}//---------------------------------------------
		      indtek=pinds+1;
           if(indtek>posind) goto frasa;
	       str2=slowo(indtek,sitstr);
	       if(str2==",") { indtek=pinds+1; goto aa24; }
           if(str2!=";") { printkl("#E - INTKOM - Нет ';' в об'явлении.Задача снята.");
           printkl("# См. строку: "); str2="# "+sitstr; printkl(str2);
	       sbros( );}

	   //-----------------------------------
             indtek=pinds+1;
	            goto frasa;
	}
   }//Конец обработки USHORT.***************************************


	  else if(str1=="int")
	  { stek="int"; splu( );  perex=0; indtek=pinds+1;
aa3:    str3=slowo2(indtek,sitstr); 
		        if(kws==0)
		{str2=slowo(indtek,sitstr);  //Взяли имя внутр. переменной
                                                        kontrima(str2,jarus);    			
	     kolperem=kolperem+1; if(kolperem>=rimper)
		 { printkl(" #F - INTKOM - Массив имен переменных мал.Задача снята.");
	     sbros( );
		 } //-------------------------------
	     imper[kolperem]=str2;
	      znper[kolperem]=0;  //Инициализация новой переменной нулем.
	      tipper[kolperem]=5; //Это тип int.
	       glst[kolperem]=jarus;
	      //Все зафиксировали.----------------
		      indtek=pinds+1;
           if(indtek>posind) goto frasa;
	       str2=slowo2(indtek,sitstr);   
	       if(str2==",") { indtek=pinds+1; goto aa3; }
           if(str2!=";") { printkl("#E1 - INTKOM - Нет ';' в об'явлении.Задача снята.");
           printkl("# См. строку: "); str2="# "+sitstr; printkl(str2);
	       sbros( );}

	   //-----------------------------------
	      indtek=pinds+1;       goto frasa;
		}//--------------- конец при об'явлении переменной
				else      //об'явление массива
		{int n,i;
         str3=slowo(indtek,sitstr);  //Имя массива до  [
          indtek=pinds+1;    //на взятие  [
		  str4=str3+"[";     //имя массива[
		  str3=slowo(indtek,sitstr);   //это [
		     indtek=pinds;      //на [
		        if(str3!="[")
			{printkl("#F - INTKOM - Ошибка имени массива в строке:");
				str3="# "+sitstr; printkl(str3); sbros();
			}//--------------------------
				indtek=pinds+1;  //на начало размера массива
           str3=slowo(indtek,sitstr);   //Это текст размера массива
		          
           n=rasp(str3);   if(n!=1)
           {printkl("#F - INTKOM - Ошибка 1 индекса в об'явлении массива в строке:");
		     str3="# "+sitstr; printkl(str3); sbros();
           }//-----------------------------------------
                 n=(int)fpere(str3);  //Числовой размер массива
                      if(n<1)
           {printkl("#F - INTKOM - Ошибка 2 индекса в об'явлении массива в строке:");
		     str3="# "+sitstr; printkl(str3); sbros();
           }//-----------------------------------------
                    for(i=0;i<n;i++)   //Запись элементов массива в систему
			{str3.sprintf("%d",i); //текст значения  i
              str5=str4+str3; str5=str5+"]"; //str5 - Чистое имя элемента массива
                                               kontrima(str5,jarus);    
                    kolperem=kolperem+1;
					if(kolperem>=rimper)
				{printkl("#F - INTKOM - Массив имен переменных мал.Задача снята");
                 printkl("#  при об'явлении массива.");   sbros();
				}//---------------------------------------
					  imper[kolperem]=str5;
                      znper[kolperem]=0;         
					  tipper[kolperem]=5;
                      glst[kolperem]=jarus; //Зафиксировали элемент
			}//----- Зафиксировали элементы всего массива
					indtek=pinds+1; //на взятие ]
            str3=slowo(indtek,sitstr);   //Это ]
			  indtek=pinds;               //на ]
                         if(str3!="]")
			{printkl("#F - INTKOM - Ошибка с  ]  при об'явлении массива в строке:");
              str3="# "+sitstr; printkl(str3); sbros();
			}//---------------------------------------------
		      indtek=pinds+1;
           if(indtek>posind) goto frasa;
	       str2=slowo2(indtek,sitstr);
	       if(str2==",") { indtek=pinds+1; goto aa3; }
           if(str2!=";") { printkl("#E2 - INTKOM - Нет ';' в об'явлении.Задача снята.");
           printkl("# См. строку: "); str2="# "+sitstr; printkl(str2);
	       sbros( );}

	   //-----------------------------------
		        indtek=pinds+1;
	            goto frasa;
	}
   }//Конец обработки int.***************************************



	  else if(str1=="UINT")
	  { stek="UINT"; splu( );  perex=0; indtek=pinds+1;
aa4:     str3=slowo2(indtek,sitstr); 
		        if(kws==0)
		{ str2=slowo(indtek,sitstr);  //Взяли имя внутр. переменной
                                                    kontrima(str2,jarus);    
	     kolperem=kolperem+1; if(kolperem>=rimper)
		 { printkl(" #F - INTKOM - Массив имен переменных мал.Задача снята.");
	     sbros( );
		 } //-------------------------------
	     imper[kolperem]=str2;
	      znper[kolperem]=0;  //Инициализация новой переменной нулем.
	      tipper[kolperem]=6; //Это тип UINT.
	       glst[kolperem]=jarus;
	      //Все зафиксировали.----------------
		      indtek=pinds+1;
           if(indtek>posind) goto frasa;
	       str2=slowo(indtek,sitstr);
	       if(str2==",") { indtek=pinds+1; goto aa4; }
           if(str2!=";") { printkl("#E - INTKOM - Нет ';' в об'явлении.Задача снята.");
           printkl("# См. строку: "); str2="# "+sitstr; printkl(str2);
	       sbros( );}

	   //-----------------------------------
	      indtek=pinds+1;       goto frasa;
		}//--------------- конец при об'явлении переменной
				else      //об'явление массива
		{int n,i;
              	str3=slowo(indtek,sitstr);  //Имя массива до  [
          indtek=pinds+1;    //на взятие  [
		  str4=str3+"[";     //имя массива[
		  str3=slowo(indtek,sitstr);   //это [
		     indtek=pinds;      //на [
		        if(str3!="[")
			{printkl("#F - INTKOM - Ошибка имени массива в строке:");
				str3="# "+sitstr; printkl(str3); sbros();
			}//--------------------------
				indtek=pinds+1;  //на начало размера массива
           str3=slowo(indtek,sitstr);   //Это текст размера массива
		        
           n=rasp(str3);   if(n!=1)
           {printkl("#F - INTKOM - Ошибка 1 индекса в об'явлении массива в строке:");
		     str3="# "+sitstr; printkl(str3); sbros();
           }//-----------------------------------------
                 n=(int)fpere(str3);  //Числовой размер массива
                      if(n<1)
           {printkl("#F - INTKOM - Ошибка 2 индекса в об'явлении массива в строке:");
		     str3="# "+sitstr; printkl(str3); sbros();
           }//-----------------------------------------
                    for(i=0;i<n;i++)   //Запись элементов массива в систему
			{str3.sprintf("%d",i); //текст значения  i
              str5=str4+str3; str5=str5+"]"; //str5 - Чистое имя элемента массива
			                                      kontrima(str5,jarus);    
                    kolperem=kolperem+1;
					if(kolperem>=rimper)
				{printkl("#F - INTKOM - Массив имен переменных мал.Задача снята");
                 printkl("#  при об'явлении массива.");   sbros();
				}//---------------------------------------
					  imper[kolperem]=str5;
                      znper[kolperem]=0;         
					  tipper[kolperem]=6;
                      glst[kolperem]=jarus; //Зафиксировали элемент
			}//----- Зафиксировали элементы всего массива
					indtek=pinds+1; //на взятие ]
            str3=slowo(indtek,sitstr);   //Это ]
			  indtek=pinds;               //на ]
                         if(str3!="]")
			{printkl("#F - INTKOM - Ошибка с  ]  при об'явлении массива в строке:");
              str3="# "+sitstr; printkl(str3); sbros();
			}//---------------------------------------------
		      indtek=pinds+1;
           if(indtek>posind) goto frasa;
	       str2=slowo2(indtek,sitstr);
	       if(str2==",") { indtek=pinds+1; goto aa4; }
           if(str2!=";") { printkl("#E - INTKOM - Нет ';' в об'явлении.Задача снята.");
           printkl("# См. строку: "); str2="# "+sitstr; printkl(str2);
	       sbros( );}

	   //-----------------------------------
		     indtek=pinds+1;
	            goto frasa;
	}
   }//Конец обработки UINT.***************************************



	  else if(str1=="float")
	  { stek="float"; splu( );  perex=0; indtek=pinds+1;
aa5:    str3=slowo2(indtek,sitstr); 
		        if(kws==0)
		{ str2=slowo(indtek,sitstr);  //Взяли имя внутр. переменной
		                                     kontrima(str2,jarus);    
	     kolperem=kolperem+1; if(kolperem>=rimper)
		 { printkl(" #F - INTKOM - Массив имен переменных мал.Задача снята.");
	     sbros( );
		 } //-------------------------------
	     imper[kolperem]=str2;
	      znper[kolperem]=0;  //Инициализация новой переменной нулем.
	      tipper[kolperem]=9; //Это тип float.
	       glst[kolperem]=jarus;
	      //Все зафиксировали.----------------
		      indtek=pinds+1;
           if(indtek>posind) goto frasa;
	       str2=slowo(indtek,sitstr);
	       if(str2==",") { indtek=pinds+1; goto aa5; }
           if(str2!=";") { printkl("#E - INTKOM - Нет ';' в об'явлении.Задача снята.");
           printkl("# См. строку: "); str2="# "+sitstr; printkl(str2);
	       sbros( );}

	   //-----------------------------------
	      indtek=pinds+1;       goto frasa;
		}//--------------- конец при об'явлении переменной
				else      //об'явление массива
		{int n,i;
              	str3=slowo(indtek,sitstr);  //Имя массива до  [
          indtek=pinds+1;    //на взятие  [
		  str4=str3+"[";     //имя массива[
		  str3=slowo(indtek,sitstr);   //это [
		     indtek=pinds;      //на [
		        if(str3!="[")
			{printkl("#F - INTKOM - Ошибка имени массива в строке:");
				str3="# "+sitstr; printkl(str3); sbros();
			}//--------------------------
				indtek=pinds+1;  //на начало размера массива
           str3=slowo(indtek,sitstr);   //Это текст размера массива
		        
           n=rasp(str3);   if(n!=1)
           {printkl("#F - INTKOM - Ошибка 1 индекса в об'явлении массива в строке:");
		     str3="# "+sitstr; printkl(str3); sbros();
           }//-----------------------------------------
                 n=(int)fpere(str3);  //Числовой размер массива
                      if(n<1)
           {printkl("#F - INTKOM - Ошибка 2 индекса в об'явлении массива в строке:");
		     str3="# "+sitstr; printkl(str3); sbros();
           }//-----------------------------------------
                    for(i=0;i<n;i++)   //Запись элементов массива в систему
			{str3.sprintf("%d",i); //текст значения  i
              str5=str4+str3; str5=str5+"]"; //str5 - Чистое имя элемента массива
			                                     kontrima(str5,jarus);    
                    kolperem=kolperem+1;
					if(kolperem>=rimper)
				{printkl("#F - INTKOM - Массив имен переменных мал.Задача снята");
                 printkl("#  при об'явлении массива.");   sbros();
				}//---------------------------------------
					  imper[kolperem]=str5;
                      znper[kolperem]=0;         
					  tipper[kolperem]=9;
                      glst[kolperem]=jarus; //Зафиксировали элемент
			}//----- Зафиксировали элементы всего массива
					indtek=pinds+1; //на взятие ]
            str3=slowo(indtek,sitstr);   //Это ]
			  indtek=pinds;               //на ]
                         if(str3!="]")
			{printkl("#F - INTKOM - Ошибка с  ]  при об'явлении массива в строке:");
              str3="# "+sitstr; printkl(str3); sbros();
			}//---------------------------------------------
		      indtek=pinds+1;
           if(indtek>posind) goto frasa;
	       str2=slowo(indtek,sitstr);
	       if(str2==",") { indtek=pinds+1; goto aa5; }
           if(str2!=";") { printkl("#E - INTKOM - Нет ';' в об'явлении.Задача снята.");
           printkl("# См. строку: "); str2="# "+sitstr; printkl(str2);
	       sbros( );}

	   //-----------------------------------
		     indtek=pinds+1;
	            goto frasa;
	}
   }//Конец обработки float.***************************************


	  else if(str1=="double")
	  { stek="float"; splu( );  perex=0; indtek=pinds+1;
aa511:    str3=slowo2(indtek,sitstr); 
		        if(kws==0)
		{ str2=slowo(indtek,sitstr);  //Взяли имя внутр. переменной
		                                         kontrima(str2,jarus);    
	     kolperem=kolperem+1; if(kolperem>=rimper)
		 { printkl(" #F - INTKOM - Массив имен переменных мал.Задача снята.");
	     sbros( );
		 } //-------------------------------
	     imper[kolperem]=str2;
	      znper[kolperem]=0;  //Инициализация новой переменной нулем.
	      tipper[kolperem]=11; //Это тип double.
	       glst[kolperem]=jarus;
	      //Все зафиксировали.----------------
		      indtek=pinds+1;
           if(indtek>posind) goto frasa;
	       str2=slowo(indtek,sitstr);
	       if(str2==",") { indtek=pinds+1; goto aa511; }
           if(str2!=";") { printkl("#E - INTKOM - Нет ';' в об'явлении.Задача снята.");
           printkl("# См. строку: "); str2="# "+sitstr; printkl(str2);
	       sbros( );}

	   //-----------------------------------
	      indtek=pinds+1;       goto frasa;
		}//--------------- конец при об'явлении переменной
				else      //об'явление массива
		{int n,i;
              	str3=slowo(indtek,sitstr);  //Имя массива до  [
          indtek=pinds+1;    //на взятие  [
		  str4=str3+"[";     //имя массива[
		  str3=slowo(indtek,sitstr);   //это [
		     indtek=pinds;      //на [
		        if(str3!="[")
			{printkl("#F - INTKOM - Ошибка имени массива в строке:");
				str3="# "+sitstr; printkl(str3); sbros();
			}//--------------------------
				indtek=pinds+1;  //на начало размера массива
           str3=slowo(indtek,sitstr);   //Это текст размера массива
		        
           n=rasp(str3);   if(n!=1)
           {printkl("#F - INTKOM - Ошибка 1 индекса в об'явлении массива в строке:");
		     str3="# "+sitstr; printkl(str3); sbros();
           }//-----------------------------------------
                 n=(int)fpere(str3);  //Числовой размер массива
                      if(n<1)
           {printkl("#F - INTKOM - Ошибка 2 индекса в об'явлении массива в строке:");
		     str3="# "+sitstr; printkl(str3); sbros();
           }//-----------------------------------------
                    for(i=0;i<n;i++)   //Запись элементов массива в систему
			{str3.sprintf("%d",i); //текст значения  i
              str5=str4+str3; str5=str5+"]"; //str5 - Чистое имя элемента массива
			                                   kontrima(str5,jarus);    
                    kolperem=kolperem+1;
					if(kolperem>=rimper)
				{printkl("#F - INTKOM - Массив имен переменных мал.Задача снята");
                 printkl("#  при об'явлении массива.");   sbros();
				}//---------------------------------------
					  imper[kolperem]=str5;
                      znper[kolperem]=0;         
					  tipper[kolperem]=11;
                      glst[kolperem]=jarus; //Зафиксировали элемент
			}//----- Зафиксировали элементы всего массива
					indtek=pinds+1; //на взятие ]
            str3=slowo(indtek,sitstr);   //Это ]
			  indtek=pinds;               //на ]
                         if(str3!="]")
			{printkl("#F - INTKOM - Ошибка с  ]  при об'явлении массива в строке:");
              str3="# "+sitstr; printkl(str3); sbros();
			}//---------------------------------------------
		      indtek=pinds+1;
           if(indtek>posind) goto frasa;
	       str2=slowo(indtek,sitstr);
	       if(str2==",") { indtek=pinds+1; goto aa511; }
           if(str2!=";") { printkl("#E - INTKOM - Нет ';' в об'явлении.Задача снята.");
           printkl("# См. строку: "); str2="# "+sitstr; printkl(str2);
	       sbros( );}

	   //-----------------------------------
		     indtek=pinds+1;
	            goto frasa;
	}
   }//Конец обработки double.***************************************


	else if(str1=="echo")
	{ 	stek="echo"; splu( ); perex=0; indtek=pinds+1;

     str2=slowo(indtek,sitstr);
	 if(str2!="(")  
{printkl("#E - INTKOM - Нет '(' после echo. Задача снята. См. строку:");
	     str2="# "+sitstr; printkl(str2); sbros( ); }
       //----------------------------------
       indtek=pinds+1;         str2=slowo(indtek,sitstr);
	 if(str2!="\"")  
{printkl("#E - INTKOM - Нет начальн.дв.кавычек в echo.Задача снята.См.строку:");
	     str2="# "+sitstr; printkl(str2); sbros( ); }

       //----------------------------------
	indtek=pinds+1; str2=dotzpecho(indtek,sitstr,'"','"');
	                                                 //Могут быть пробелы
	                                                 //после начальн. "  .

	    if(est!=1)
{printkl("#E - INTKOM - Нет последн.дв.кавычек в echo.Задача снята.См.строку:");
	     str2="# "+sitstr; printkl(str2); sbros( ); }

       //------- В str2 форматирующая строка,indtek за начальными " .
	str3=dotzpecho(indtek,sitstr,'%','"');  //Ищем символ %  .
	      if(est!=1)                    //Нет этого символа.
	{ str3="# " + str2;  printkl(str3);
	  //-------------------- Отпечатали.
		  
    	str3=dotzpecho(indtek,sitstr,'"','"');	  
	  indtek=pinds+1; //Сдвинулись до "  . 
	  str3=slowo(indtek,sitstr);                 //Это  "  .
	  indtek=pinds+1; str3=slowo(indtek,sitstr); //Это   )
if(str3!=")") { printkl("#E - INTKOM - З-ча снята.Нет ')' в строке; ");
	                  str3="#  "+sitstr; printkl(str3); sbros( ); }
	  indtek=pinds+1;   
	  if(indtek>posind) goto frasa;
	  str3=slowo(indtek,sitstr); //Это   ;  .
if(str3!=";") { printkl("#E - INTKOM - З-ча снята.Нет ';' в строке; ");
	                  str3="#  "+sitstr; printkl(str3); sbros( ); }
	  indtek=pinds+1;       //Сдвиг за  ;  .
	  goto frasa;
	} //-------------------------------
		  

	       else               //Есть в строке %  
	{int indtek2;  //пойдет по именам переменных
      //-----------------------------
      QChar ss;
      short p;
	  int j;
	               p=0;
            for(j=pinds+1;;j=j+1)
		{ss=sitstr.at(j);
          if(ss=='"') break;

    	  if((ss=='o')||(ss=='d')||(ss=='x')||(ss=='X')||(ss=='c')||(ss=='s')||
             (ss=='i')||(ss=='u')||(ss=='f')||(ss=='e'))
		  {p=1; break;}
		}
			     if(p==0)
	  {printkl("# Работа командного файла прекращена.");
	   printkl("#F - INTKOM - Нет знака типа преобразования в ECHO в строке:"); 
       str2="# "+sitstr; printkl(str2); sbros( );
	  }//---------------------------


         indtek2=indtek;
     str3=dotzp(indtek2,sitstr,'"','"'); indtek2=pinds+1;
     str3=slowo(indtek2,sitstr);   //Это  "
         indtek2=pinds+1;
     str3=slowo(indtek2,sitstr);   //Это  ,
		 if(str3!=",")
	{printkl("#F - INTKOM -Нет запятой после кавычек в echo  в строке:");
       str2="# "+sitstr; printkl(str2); sbros( );
	}//--------------------------------------
         indtek2=pinds+1;        //на начало имени переменной
     //Здесь: indtek   - на начале первого куска 
     //       indtek2  - на начале имени первой переменной

		   str5="# ";  //Исходное значение результата echo

		      //Теперь циклическая процедура:
zik:    str2=dotzpecho(indtek,sitstr,'%','"'); //Взяли кусок до %
		          if(est!=1)
	{printkl("#F - INTKOM -Несоответствие формата echo количеству переменных в строке:");
       str2="# "+sitstr; printkl(str2); sbros( );
	}//--------------------------------------
                 str5=str5+str2;        //приписали кусок
                  indtek=pinds+1;       //на %
     str2=dotzp(indtek,sitstr,' ','"'); //взяли код формата с % до пробела или "  
	              indtek=pinds+1;       //за код формата

      //---- Теперь займемся переменной -------:
       str4=slowo2(indtek2,sitstr);    //имя переменной              
                    if(kws==1)
		{str4=istima(str4);    //Коррекция имени элемента массива
        		 prizn=indimper(str4);
         str4=dotzp(indtek2,sitstr,',',')');
		 str4 = str4.trimmed();
		}
					else
		{str4=dotzp(indtek2,sitstr,',',')');
					str4 = str4.trimmed();
		                        			//имя переменной	  
                 prizn=indimper(str4);        //ее индекс
		}
	       // Теперь смотрим тип этой переменной:
		     if(tipper[prizn]==1)
	       { xxxx1=(char)znper[prizn];   str3.sprintf(str2.toStdString().c_str(),xxxx1);  }
		           else if(tipper[prizn]==2)
	       { xxxx2=(UCHAR)znper[prizn];  str3.sprintf(str2.toStdString().c_str(),xxxx2);  }
		           else if(tipper[prizn]==3)
	       { xxxx3=(short)znper[prizn];  str3.sprintf(str2.toStdString().c_str(),xxxx3);  }
		           else if(tipper[prizn]==4)
	       { xxxx4=(USHORT)znper[prizn]; str3.sprintf(str2.toStdString().c_str(),xxxx4);  }
		           else if(tipper[prizn]==5)
			{ xxxx5=(int)znper[prizn];    str3.sprintf(str2.toStdString().c_str(),xxxx5);  }
		           else if(tipper[prizn]==6)
	       { xxxx6=(UINT)znper[prizn];   str3.sprintf(str2.toStdString().c_str(),xxxx6);  }
		           else if(tipper[prizn]==9)
	       { xxxx9=(float)znper[prizn];  str3.sprintf(str2.toStdString().c_str(),xxxx9);  }
		           else if(tipper[prizn]==11)
	       { xxxx11=(double)znper[prizn]; str3.sprintf(str2.toStdString().c_str(),xxxx11);}
	     else { printkl("# Ошибка типа переменной.З-ча снята.См строку;");
		 str2="# "+sitstr; printkl(str2); sbros( );} 		     
		//--------------------------

             str5=str5+str3;   //приписали текстовое значение переменной
   
            //--- Пора оценивать следующий шаг на новую переменную

                    if(est==1)       //дошли до запятой
		{indtek2=pinds+1; str3=slowo(indtek2,sitstr); //это запятая
         indtek2=pinds+1;  //на начало следующего имени переменной
		            goto zik;
		}//----------------------------------
               else if(est==(-1))  //вышли на конец строки без правой скобки
	{printkl("#F - INTKOM - Нет правой скобки в echo  в строке:");
         str2="# "+sitstr; printkl(str2); sbros( );
	}//--------------------------------------
		            else                //дошли до  )  и почти все сделали
	{indtek2=pinds+1; str3=slowo(indtek2,sitstr); //это  )
         indtek2=pinds;
	 str3=dotzpecho(indtek,sitstr,'"','"'); //последний холостой кусок
                 str5=str5+str3;    //дописали его, может быть пустым.
	       
	}//_______________________________________

                   printkl(str5);   //Печать  результата !
                 //----------------------------------------
         indtek=indtek2;     //переход на принятый индекс
		             //---------------------------
		indtek=indtek+1;
		if(indtek>posind) goto frasa;
		str1=slowo(indtek,sitstr);  //Это ; .
		if(str1!=";") 
		{printkl("#E - INTKOM - Задача снята.Нет ';' в строке");
		str1="#  "+sitstr; printkl(str1); sbros( ); }
		indtek=pinds+1;           //Сдвиг за  ;  .

	  goto frasa;
	 }

 } //Конец echo.***************************************


	  else if(str1=="sleep")      //Ее аргумент - целое число явно.
      { stek="sleep"; splu( ); perex=0; indtek=pinds+1;
       str2=slowo(indtek,sitstr); //Это '(' .
               if(str2!="(") 
	{ printkl("#E - INTKOM - Нет скобки в sleep.З-ча снята.См строку");
	   str2="# "+sitstr; printkl(str2); sbros( );}
	    indtek=pinds+1;               //На начало числа.
	  str2=slowo(indtek,sitstr);      //Взяли текст числа.
	  j=(int)fpere(str2);  Sleep(j);  //Выполнили задержку.
	  indtek=pinds+1; str2=slowo(indtek,sitstr);  //Это  ')'  . 
	  indtek=pinds+1;  if(indtek>posind) goto frasa;
	  str2=slowo(indtek,sitstr);  //Это  ';'  .
	  if(str2!=";")
		{printkl("#E - INTKOM - Задача снята.Нет ';' в строке");
		str1="#  "+sitstr; printkl(str1); sbros( ); }
		indtek=pinds+1;           //Сдвиг за  ;  .
       
	  goto frasa;
    } //Конец sleep.*******************************************

	  else if(str1=="end")           //Прекращает работу во
	 { splu( );
	  if(incl!=0)    jarus=jarus+1;                                       //***!!!         
	  if(jarus==1)        //вложенном ком. файле.
	  {        
	printkl("#I - INTKOM - Окончание работы с исходным командным файлом по end.");
	       jarus=0; myf.close( );
		   kontext=0;
		   sbros( );
	  }
	           else 
			   {   if(pesintkom==1)
	printkl("#I - INTKOM - Окончание работы с вложенным файлом по end.");	   
				  }
	 myf.close( ); perex=2; return;
		
	  }//Конец end.*********************************************
	  
	    else if(str1=="final")  //Прекращает работу вообще.
		{ splu( );
		 printkl("#I - INTKOM - Конец работы с командным файлом по final.");
		  myf.close( ); jarus=0; kontext=0; sbros( );
		}//Конец final.*****************************************

		

		else if(str1=="shell")
	{ stek="shell"; splu( ); perex=0; indtek=pinds+1;
	  str2=dotzp(indtek,sitstr,';',';'); //Взяли команду ДОС
	  str3="shell "+str2;
	  prmdi(str3);        //В проток.файл - эту же команду.
	         system(str2.toStdString().c_str());               //Ее исполнение
      indtek=pinds+1;  if(indtek>posind) goto frasa;
	  str3=slowo(indtek,sitstr); //Это ';' .
      if(str3!=";")
	  { printkl("#E - INTKOM - Задача снята.Нет ';'  в строке: ");
	    str3="#  "+sitstr; printkl(str3); sbros( );
	  }

       indtek=pinds+1;  goto frasa;
       
	}// Конец shell ***********************************************

       else if(str1=="echo_on")
	 {stek="echo_on"; splu( ); perex=0; indtek=pinds+1;
	     if(indtek<=posind)
		 {if(sitstr.at(indtek)==';') indtek=indtek+1;}
		   widasa=0;              
          goto frasa;
	 }
	 //**********************************************************
	   

       else if(str1=="echo_off")
	 {stek="echo_off"; splu( ); perex=0; indtek=pinds+1;
	     if(indtek<=posind)
		 {if(sitstr.at(indtek)==';') indtek=indtek+1;}
		   widasa=1;              
          goto frasa;
	 }
	 //**********************************************************
              


     else if(str1=="putenv")  //Ввод глобального параметра
	 {int p;    int i,j;
		 stek="putenv"; splu( ); perex=0; indtek=pinds+1; //На (
	  str2=slowo(indtek,sitstr);  if(str2!="(")
	  {printkl("#E - INTKOM - Нет левой скобки в putenv. См строку:");
	   str2="#  "+sitstr; printkl(str2); sbros( ); }
          //--------------------------
      indtek=pinds+1;    //На начало глоб.имени
      str2=slowo(indtek,sitstr);   if(str2=="\"")
        indtek=pinds+1;    //Проскочили возможные левые "

	  str2=dotzp(indtek,sitstr,'=','=');         //Взяли глоб.имя в str2
	  str2 = str2.trimmed();
//	       str2.TrimRight();    //Обжали справа
	                 if(est==(-1))
		{printkl("# - INTKOM - Нет  '='   в putenv в строке:");
         str2="# "+sitstr; printkl(str2); sbros( );
		}//---------------------
       indtek=pinds+1;  //На  '='
       str3=slowo(indtek,sitstr);       //Взяли '='
       indtek=pinds+1;  //На начало параметра
	   str3=dotzp(indtek,sitstr,')','"');  //Взяли параметр в str3
	                 if(est==(-1))
		{printkl("# - INTKOM - Нет  ')'   в putenv в строке:");
         str2="# "+sitstr; printkl(str2); sbros( );
		}//---------------------
          if(str3=="")      //пустой параметр
{str4=str2+"=";  
		  p=putenv(str4.toStdString().c_str());  			 

                      if(p!=0)
		{printkl("#I - INTKOM - Неуспешное завершение putenv. См строку:");
	      str3="#  "+sitstr; printkl(str3); sbros( );
		}

 goto ppp; //на окончание           
}//----------------------------------------------------------

                   sa=str3.at(0);   //Первый символ параметра

                     if((sa!='~')&&(sa!='^'))      //Т.е. текст
		{str4=str2+"="+str3;   //Строка для putenv
		  p=putenv(str4.toStdString().c_str());  			 

                      if(p!=0)
	{printkl("#I - INTKOM - Неуспешное завершение putenv. См строку:");
	   str3="#  "+sitstr; printkl(str3); sbros( );
}
		}//------ Конец когда параметр есть текст


              else if(sa=='~')          //Т.е.  ~ и переменная в str3
		{ indtek=ninds+1;   //Пропустили ~
		  str4=slowo(indtek,sitstr);  //В str4 имя переменной
                i=(-1);  for(j=0;j<=kolperem;j++)
				{if(glst[j]!=jarus)  continue;
				 if(str4==imper[j]) {i=j; break;} //Нашли переменную
                }//---------------------
                  if(i==(-1))
	{printkl("#F - INTKOM - Ошибка имени переменной в putenv в строке:");
				  str5="# "+sitstr; printkl(str5); sbros( );
}//----------------------
				      j=tipper[i];  //тип переменной
       if((j<3)||(j>6))
	{printkl("#F - INTKOM - Ошибка типа переменной в putenv в строке:");
				  str5="# "+sitstr; printkl(str5); sbros( );
}//----------------------
	   
                if(j==3) {xxxx3=(short)znper[i];  str5.sprintf("%x",xxxx3);}
           else if(j==4) {xxxx4=(USHORT)znper[i]; str5.sprintf("%x",xxxx4);}
           else if(j==5) {xxxx5=(int)znper[i];    str5.sprintf("%x",xxxx5);}
           else if(j==6) {xxxx6=(UINT)znper[i];   str5.sprintf("%x",xxxx6);}
		     else {printkl("#F - INTKOM -putenv- БУЗА 4."); sbros( );}
              //----------------------------------------------------
       		str4=str2+"="+str5;   //Строка для putenv
	         	  p=putenv(str4.toStdString().c_str());  			 

                      if(p!=0)
	{printkl("#I - INTKOM - Неуспешное завершение putenv. См строку:");
	   str3="#  "+sitstr; printkl(str3); sbros( );
}

		}//------ Конец когда параметр есть переменная с  ~
  //----------------------------------------------------


              else if(sa=='^')          //Т.е.  ^ и переменная в str3
		{ indtek=ninds+1;   //Пропустили ^
		  str4=slowo(indtek,sitstr);  //В str4 имя переменной
                i=(-1);  for(j=0;j<=kolperem;j++)
				{if(glst[j]!=jarus)  continue;
				 if(str4==imper[j]) {i=j; break;} //Нашли переменную
                }//---------------------
                  if(i==(-1))
{printkl("#F - INTKOM - Ошибка имени переменной в putenv в строке:");
				  str5="# "+sitstr; printkl(str5); sbros( );
}//----------------------
				      j=tipper[i];  //тип переменной
       if(j<3)
{printkl("#F - INTKOM - Ошибка типа переменной в putenv в строке:");
				  str5="# "+sitstr; printkl(str5); sbros( );
}//----------------------
	   
                if(j==3) {xxxx3=(short)znper[i];  str5.sprintf("%d",xxxx3);}
           else if(j==4) {xxxx4=(USHORT)znper[i]; str5.sprintf("%d",xxxx4);}
           else if(j==5) {xxxx5=(int)znper[i];    str5.sprintf("%d",xxxx5);}
           else if(j==6) {xxxx6=(UINT)znper[i];   str5.sprintf("%d",xxxx6);}
           else if(j==9) {xxxx9=(float)znper[i];  str5.sprintf("%.18g",xxxx9);}
           else if(j==11) {xxxx11=(double)znper[i]; str5.sprintf("%.18g",xxxx11);} 
		     else {printkl("#F - INTKOM -putenv- БУЗА 6."); sbros( );}
              //----------------------------------------------------
       		str4=str2+"="+str5;   //Строка для putenv
	         	  p=putenv(str4.toStdString().c_str());

                      if(p!=0)
{printkl("#I - INTKOM - Неуспешное завершение putenv. См строку:");
	   str3="#  "+sitstr; printkl(str3); sbros( );
}

		}//------ Конец когда параметр есть переменная с  ^
  //----------------------------------------------------
			  else {printkl("# putenv - БУЗА 7."); sbros( );}


        indtek=pinds+1;               //На ')'
ppp:         str2=slowo(indtek,sitstr);    //Взяли ')'
      if(str2=="\"")
	  {  indtek=pinds+1;   //Обошли возможные правые "
         str2=slowo(indtek,sitstr); //Взяли  )
	  }
         //---------------------------------------
      indtek=pinds+1;       //За  )
	  if(indtek>posind) goto frasa;
	  str2=slowo(indtek,sitstr);   if(str2!=";")
	  {printkl("#E - INTKOM - Нет  ;  в putenv. См строку:");
	   str2="#  "+sitstr; printkl(str2); sbros( ); }
          //--------------------------
       indtek=pinds+1; goto frasa;

	 }//********* Конец обработки  putenv ***********************



	 else if(str1=="param") //Ввод локальных параметров в par[ ].
     { int n; n=0; stek="param"; splu( ); perex=0;
	                indtek=pinds+1;
       if(indtek>posind)
	   {printkl("#E - INTKOM - Нет параметров в строке:");
	     str2="# "+sitstr; printkl(str2); sbros( );
       }//----------
		if(indtek==posind)
		{printkl("#E - INTKOM - Ошибка синтаксиса в строке:");
		 str3="#  "+sitstr; printkl(str3); sbros( );
		}//-----------------------
		   // ---- indtek<posind :
  nazad:       str2=slowo(indtek,sitstr); 
		              indtek=ninds;	
       str4=dotzpecho(indtek,sitstr,' ',';');
	   sa=str4.at(0);   //Первый символ
	     if((sa!='~')&&(sa!='^'))    //Не переменная
		 {kolpar=kolpar+1; par[kolpar]=str4;
		  jrpar[kolpar]=jarus+1; nomer[kolpar]=n; n=n+1;
		  indtek=pinds+1;   //Вышли за строку str4.
		 }//-----------------------------------
                   else    //Переменная с   ~ или   ^
		{indtek=indtek+1;  //на начало имени переменной
         str4=slowo(indtek,sitstr); //Имя переменной
		i=(-1);  for(j=0;j<=kolperem;j++)
		{if(glst[j]!=jarus) continue;
		if(str4==imper[j]) {i=j; break;} //нашли
		}
		if(i==(-1))
		{
		 kolpar=kolpar+1; par[kolpar]="";jrpar[kolpar]=jarus+1;
         nomer[kolpar]=n; n=n+1;
         indtek=pinds+1;  //Вышли за строку str4.
		 goto net;
		}
		j=tipper[i];   //тип переменной
         //----------------------------------
              indtek=pinds+1;  //Вышли за строку str4.
          //--- Теперь учет ^  и  ~     :
                 if(sa=='^')     //Число
	{
        if(j<3)
		{str5="#E - INTKOM -Ошибка типа переменной  в строке:";
		 printkl(str5); str5="# "+sitstr;
         printkl(str5); sbros( );
		} //---------------------------
	          //Анализ типа переменной:
		if(j==11)   //double
        {xxxx11=(double)znper[i]; str5.sprintf("g%e",xxxx11);}
		else if(j==9)   //float
        {xxxx9=(float)znper[i]; str5.sprintf("f%f",xxxx9);}
		else if(j==6)   //UINT
        {xxxx6=(UINT)znper[i]; str5.sprintf("%d",xxxx6);}
		else  if(j==5)   //int
        {xxxx5=(int)znper[i]; str5.sprintf("%d",xxxx5);}
		else if(j==4)   //USHORT
        {xxxx4=(USHORT)znper[i]; str5.sprintf("%d",xxxx4);}
		  else  //short
        {xxxx3=(short)znper[i]; str5.sprintf("%d",xxxx3);}
		   //В str5 - запись десятичного числа или с f и с g.
            kolpar=kolpar+1; par[kolpar]=str5;
        jrpar[kolpar]=jarus+1; nomer[kolpar]=n; n=n+1;
	}//---- Конец ^

				 else           //Т.е. ~ ,  - 16 тиричное
	{
         //----------------------------------
        if((j<3)||(j>6))
{str5="#E - INTKOM - Ошибка типа переменной  в строке:";
		 printkl(str5); str5="# "+sitstr;
         printkl(str5); sbros( );
		} //---------------------------
            if(j==3)
			{xxxx3=(short)znper[i]; str5.sprintf("%x",xxxx3);}
           else if(j==4)
			{xxxx4=(USHORT)znper[i]; str5.sprintf("%x",xxxx4);}
           else if(j==5)
			{xxxx5=(int)znper[i]; str5.sprintf("%x",xxxx5); }
             else
			{xxxx6=(UINT)znper[i]; str5.sprintf("%x",xxxx6);}
		                      //получили 16-ричный адрес без 0x.
            kolpar=kolpar+1; par[kolpar]=str5;
        jrpar[kolpar]=jarus+1; nomer[kolpar]=n; n=n+1;
	}//----- Конец ~
	}//---------------------------------------------
            net: if(kolpar>=(rpar-1))
	{printkl("#F - INTKOM - Размер массива параметров par[]  мал.");
				                                    sbros( );}
     //----------------------------------
		 if(indtek>posind) goto frasa;
		 if(indtek==posind)
		 {if(sitstr.at(indtek)!=';')
			{printkl("#E - INTKOM - Ошибка синтаксиса. См строку:");
		      str3="# "+sitstr; printkl(str3); sbros( );}
		    indtek=indtek+1; goto frasa;
         }//--------------
            str2=slowo(indtek,sitstr);
			if(str2!=";")  goto nazad;

            indtek=pinds+1; goto frasa;

   }//*********** Конец  param ********************************


     else if(str1=="wkl")
	 {stek="wkl"; splu(); perex=0; indtek=pinds+1;
	   //----------------------------------
	  str2=dotzp(indtek,sitstr,';',';');  //Взяли хвост
	                  wkl(str2);          //Обработали его
         indtek=pinds+1;   if(indtek<=posind)
		 {str3=slowo(indtek,sitstr);      //Это ;
		        if(str3!=";")
				{printkl("#F INTKOM Ошибка синтаксиса в wkl в строке:"); 
                  str3="# "+sitstr; printkl(str3); sbros();
				}
             indtek=pinds+1;      //За ;
		 }//---------

                 goto frasa;

	 }//*************************** конец   wkl ***************
   

     else if(str1=="otkl")
	 {stek="otkl"; splu(); perex=0; indtek=pinds+1;
	   //----------------------------------
	  str2=dotzp(indtek,sitstr,';',';');  //Взяли хвост
	                  otkl(str2);          //Обработали его
         indtek=pinds+1;   if(indtek<=posind)
		 {str3=slowo(indtek,sitstr);      //Это ;
		        if(str3!=";")
				{printkl("#F INTKOM Ошибка синтаксиса в otkl в строке:"); 
                  str3="# "+sitstr; printkl(str3); sbros();
				}
             indtek=pinds+1;      //За ;
		 }//---------

                 goto frasa;

	 }//*************************** конец   otkl ***************

     else if(str1=="impprer")    //Выдача импульсного прерывания из ком.файла
		 //Синтаксис: impprer <код прерывания>
	 {stek="impprer"; splu(); perex=0; indtek=pinds+1;
	   //----------------------------------
	  str2=dotzp(indtek,sitstr,';',';');  //Взяли хвост
	             NAXAEW=fpere(str2); //Обработали его, это управл.код прерывания,
                                     //записан в ненулевой признак NAXAEW
         indtek=pinds+1;   if(indtek<=posind)
		 {str3=slowo(indtek,sitstr);      //Это ;
		        if(str3!=";")
				{printkl("#F INTKOM Ошибка синтаксиса в impprer в строке:"); 
                  str3="# "+sitstr; printkl(str3); sbros();
				}
             indtek=pinds+1;      //За ;
		 }//---------

                 goto frasa;

	 }//*************************** конец   impprer ***************




       else         //Неопознанное, значит - команда отладчику.
	{ stek="Команда отладчику"; 
	  perex=0;  str2="";

      rrr: if(indtek>posind)
		   {str2=str2+";"; rom=rom+str2; goto frasa;}
		   //----------------------
		   if(indtek==posind)
           {str5=slowo(indtek,sitstr);  //Это ;
		    if(str5!=";")
	{printkl("#E - INTKOM - Ошибка в последнем символе в строке:");
			 printkl(sitstr); sbros( );
            }
               str2=str2+";"; rom=rom+str2;
			  indtek=pinds+1; goto frasa;
           }//---------------------- 
	           //Теперь indtek<posind
		   //ТЕПЕРЬ - СОДЕРЖАТЕЛЬНОЕ ПРОДОЛЖЕНИЕ:
		str4=dotzp(indtek,sitstr,'~',';');
         if(est==1) tild=(short)pinds; else tild=1000;
        str4=dotzp(indtek,sitstr,'^',';');
         if(est==1) gal=(short)pinds; else gal=1000;
           if((tild==1000)&&(gal==1000))     //нет  ~  и  ^
		{str4=dotzpecho(indtek,sitstr,';',';');
         str2=str2+str4+";"; rom=rom+str2;
		 //------------------------------
		 if(est==1)   //взяли до ;
         {indtek=pinds+1; str5=slowo(indtek,sitstr);
                             //вышли на ;
		   indtek=pinds+1;   //ушли за ;
           goto frasa;
		 }
		 else    //взяли до конца строки без ;
         {indtek=pinds+1; goto frasa;}
		}//----ЗАВЕРШЕНА ОБРАБОТКА КОМАНДЫ ОТЛАДЧИКА

                if(tild<gal)    //будет обработка ~
	{str4=dotzpecho(indtek,sitstr,'~',';'); //взяли до ~
     str2=str2+str4; indtek= pinds+1;
	 str4=slowo(indtek,sitstr);  //Это ~
	 if(str4!="~")
	 {printkl("#F - INTKOM - Ошибка в команде с ~"); sbros( ); }
       indtek=pinds+1;  //вышли на переменную - адрес
	    //-----------------------------------
        str4=slowo(indtek,sitstr);   //взяли имя переменной

		i=(-1);  for(j=0;j<=kolperem;j++)
		{if(glst[j]!=jarus) continue;
		if(str4==imper[j]) {i=j;
		break;} //нашли
		}
		if(i==(-1))
	{str5="#E - INTKOM - Ошибка имени переменной  в строке:";
		 printkl(str5); str5="# "+sitstr;
         printkl(str5); sbros( );
		}
		j=tipper[i];   //тип переменной
         //----------------------------------
        if((j<3)||(j>6))
	{str5="#E - INTKOM - Ошибка типа переменной  в строке:";
		 printkl(str5); str5="# "+sitstr;
         printkl(str5); sbros( );
		} //---------------------------
            if(j==3)
			{xxxx3=(short)znper[i]; str5.sprintf("%x",xxxx3);}
           else if(j==4)
			{xxxx4=(USHORT)znper[i]; str5.sprintf("%x",xxxx4);}
           else if(j==5)
			{xxxx5=(int)znper[i]; str5.sprintf("%x",xxxx5); }
             else
			{xxxx6=(UINT)znper[i]; str5.sprintf("%x",xxxx6);}
		                      //получили 16-ричный адрес без 0x.
                    str2=str2+str5;  //вписали его
  
            indtek=pinds+1; //вышли за адрес-переменную
			goto rrr;
	}//Конец обработки ~
             if(gal<tild)     //обработка ^
	{str4=dotzpecho(indtek,sitstr,'^',';'); //взяли до ^
     str2=str2+str4; indtek= pinds+1;
	 str4=slowo(indtek,sitstr);  //Это ^
	 if(str4!="^")
	 {printkl("#F - INTKOM - Ошибка в команде с ^"); sbros( ); }
       indtek=pinds+1;  //вышли на переменную - значение
	    //-----------------------------------
        str4=slowo(indtek,sitstr);   //взяли имя переменной
		i=(-1);  for(j=0;j<=kolperem;j++)
		{if(glst[j]!=jarus) continue;
		if(str4==imper[j]) {i=j; break;} //нашли
		}
		if(i==(-1))
	{str5="#E - INTKOM - Ошибка имени переменной  в строке:";
		 printkl(str5); str5="# "+sitstr;
         printkl(str5); sbros( );
		}
		j=tipper[i];   //тип переменной
         //----------------------------------
        if(j<3)
	{str5="#E - INTKOM - Ошибка типа переменной  в строке:";
		 printkl(str5); str5="# "+sitstr;
         printkl(str5); sbros( );
		} //---------------------------
		if(j==11)   //double
        {xxxx11=(double)znper[i]; str5.sprintf("g%e",xxxx11);}
		else if(j==9)   //float
        {xxxx9=(float)znper[i]; str5.sprintf("f%f",xxxx9);}
		else if(j==6)   //UINT
        {xxxx6=(UINT)znper[i]; str5.sprintf("0x%X",xxxx6);}
		else  if(j==5)   //int
        {xxxx5=(int)znper[i]; str5.sprintf("0x%X",xxxx5);}
		else if(j==4)   //USHORT
        {xxxx4=(USHORT)znper[i]; str5.sprintf("h0x%X",xxxx4);}
		else if(j==3)   //short
        {xxxx3=(short)znper[i]; str5.sprintf("h0x%X",xxxx3);}
		else if(j==2)   //UCHAR
        {xxxx2=(UCHAR)znper[i]; str5.sprintf("b0x%X",xxxx2);}
		else if(j==1)   //char
        {xxxx1=(char)znper[i]; str5.sprintf("b0x%X",xxxx1);}
		else {printkl("#F - INTKOM - БУЗА 1 ."); sbros( );  }   

		   //В str5 - запись  числа.
         str2=str2+str5;  //вписали значение переменной
          indtek=pinds+1; //вышли за обозначение переменной
		  goto rrr;
		}//Конец обработки ^


    } //Конец неопознанного.***********************************

  }//Конец obrstr.===================================================
  void splu(void)                 //Сплю - просыпаюсь /после выдачи в direct
  {UINT zgruppa[1],zgruppa1[rrom],zgruppa2[rrom],zgruppa3[rrom],zgruppa4[rrom];
   UINT zgruppa5[rrom],zgruppa6[rrom];
   int i;
	  if(snat==7)                 //Нет ли приказа "снять"?
	{str1="#I - INTKOM - Работа с ком.файлом прекращена по команде оператора.";
		   printkl(str1); snat=0; //Сообщение в протокольный файл.
	    sbros( );                 //Сброс моего потока.
	}//--------------------------------------------

	  if(rom!="")                 //Засыпаю,если передаю команды 
  {   direct=rom;   rom="";    //из rom в direct Романову.
	  SetEvent(eDirective);   //Разбуживание спящего потока Романова
	 WaitForSingleObject(kDirective,INFINITE); //Ожидание разбужив.от потока Романова
	 ResetEvent(kDirective);  //Сброс события разбуживания от Романова
//-----------------	
	  if(snat==7)                 //Нет ли приказа "снять"?
	{str1="#I - INTKOM - Работа с ком. файлом прекращена по команде оператора.";
		   printkl(str1); snat=0; //Сообщение в протокольный файл.
	    sbros( );                 //Сброс моего потока.
	}//-----------

 }//------------------------------------- end if(rom!="") 

      if(stopkomfile!=0)
	 {
		  zgruppa[0]=gruppa[0]; 
	      for(i=0;i<rrom;i++)  zgruppa1[i]=gruppa1[i];
          for(i=0;i<rrom;i++)  zgruppa2[i]=gruppa2[i];
          for(i=0;i<rrom;i++)  zgruppa3[i]=gruppa3[i];
          for(i=0;i<rrom;i++)  zgruppa4[i]=gruppa4[i];
          for(i=0;i<rrom;i++)  zgruppa5[i]=gruppa5[i];
          for(i=0;i<rrom;i++)  zgruppa6[i]=gruppa6[i];
		  

      WaitForSingleObject(gokomfile,INFINITE);  //Засыпаю и жду события gokomfile 
       ResetEvent(gokomfile);

		  gruppa[0]=zgruppa[0]; 
	      for(i=0;i<rrom;i++)  gruppa1[i]=zgruppa1[i];
          for(i=0;i<rrom;i++)  gruppa2[i]=zgruppa2[i];
          for(i=0;i<rrom;i++)  gruppa3[i]=zgruppa3[i];
          for(i=0;i<rrom;i++)  gruppa4[i]=zgruppa4[i];
          for(i=0;i<rrom;i++)  gruppa5[i]=zgruppa5[i];
          for(i=0;i<rrom;i++)  gruppa6[i]=zgruppa6[i];	   

       stopkomfile=0;
	}//----------------------------------

	  if(snat==7)                 //Нет ли приказа "снять"?
	{str1="#I - INTKOM - Работа с ком. файлом прекращена по команде оператора.";
		   printkl(str1); snat=0; //Сообщение в протокольный файл.
	    sbros( );                 //Сброс моего потока.
	}//-----------

	  
}//=================================================================

  long double sislo(QString ss)   //Распознает и вычисляет число по ss.
  { QString s,s1; QChar a; int i,j,k; long double x;
    s=ss; k=rasp(s);
		a=s.at(0); if((a=='+')||(a=='-'))
	{
			s.remove(0, 1);
		} else a=',';   
	
	if(k==1)   //Т.е.  s - целое число.
    {	x=(long double)fpere(s); if(a=='-') x=-x;
	indsis=(-1); return(x);}
    //------------------------
		 else  if(k==2)   //Тогда s - символ в апострофах.
    { a=s.at(1);  x=(long double)a.toLatin1(); indsis=(-3); return(x);}
    //------------------------
		 else if(k==4)    //s  -  с плавающей точкой
     { x=(long double)dfpere(s); if(a=='-') x=-x; indsis=(-2); return(x);}
     //-------------------------
		     else         //Тогда s - имя переменной.
    { i=(-1);  for(j=0; j<=kolperem; j++)
	{  if(glst[j]!=jarus) continue;  
	   if(s==imper[j])  { i=j; indsis=i; break; }
	}                         //Искали это имя в массиве.
	if(i==(-1))               // i - индекс в массиве.
	{ s1="#E - INTKOM - Ошибка имени переменной " + s + " в строке ";
	  printkl(s1);	     				 
	  s1= "#  "+sitstr;
	  printkl(s1);
	  printkl("# Задача снята.");     sbros( );
	}
	//--------------------
	x=(long double)znper[i];
    }    //------------------------------
		   if(a=='-') x=-x;  return(x);
  }//Конец sislo.==================================================
  
  int rasp(QString ss)            //Возвращает:
  { int i,j,k;                    // 1 - если  ss целое  число,
    QChar a;			              // 2 - если  ss символ в апостофах,
    k=ss.count( );	k=(k-1);  // 3 - если  ss имя переменной.
	                              // 4 - если с плавающей точкой.
    if((ss.at(0)=='\'')&&(ss.at(2)=='\'')&&(k==2))
		       return(2);        //Это символ.
		    //---------------------------
	                   if(k>=1)
			{ if((ss.at(0)=='0')&&(ss.at(1)=='x'))
			      return(1); }      //Шестнадцатеричное.
		 //------------------------------------------
                for(j=0; j<=k; j++)
			{if(ss.at(j)=='.')  return(4);}
         //------------------------------------------
      i=1;   for(j=0; j<=k; j++)
       {   a=ss.at(j);
	   if((a=='+')||(a=='-')||(a=='0')||(a=='1')||
	   (a=='2')||(a=='3')||(a=='4')||(a=='5')||(a=='6')||
	   (a=='7')||(a=='8')||(a=='9'))
	       { } else  { i=3; break; }
       } //------------------------------
		       return(i);
  }//Конец rasp.==================================================

  int znsr(QChar a)                    //Возвращает 1, если 'a' или
  {                                   //  '>'  '<'  '='  '!'  .
    if((a=='>')||(a=='<')||(a=='=')||(a=='!'))
       return(1);   else return(0);
  }//============================================================

  int znop(QChar a)                 //Возвращает 1, если  'a'  знак
                                   // арифм. операции + ,-,/,*.и др.
  { if((a=='+')||(a=='-')||(a=='/')||(a=='*')
     ||(a=='>')||(a=='<')||(a=='%')||(a=='&')||(a=='|')||(a=='^'))
     return(1); else return(0);
  }//=============================================================
                                   //indtek - за ')' условия.
	       

  void propusk(void)               //Пропускает 1 оператор ,
  { QString str1,str2;             //находя новое значение tekpoz
    int i,k;  QChar a;            // и indtek - за оператором.
    int stet;                      //stet - счетчик фигурн.скобок.
	nowstr=0;
	       stek="propusk";
		  if(indtek<=posind)
    { str1=dotzp(indtek,sitstr,';',';');
	    if(str1.at(0)!='{')
		{ if(est==(-1))             //Последний в строке оператор
		                            //отладчика и без  ';'    .
		      { indtek=posind+1; return; }
		    indtek=pinds+1; str2=slowo(indtek,sitstr); //Это ';'  .
			if(str2!=";")
			{printkl("#E - INTKOM - З-ча снята. Нет ';' в строке: ");
			 str2="#  "+sitstr; printkl(str2); sbros( );
			}
            indtek=pinds+1;         //Сдвиг за  ';'  .
			return;            
		 } //----------- Обошли простой оператор в этой же строке. 
	    else                             // есть '{'  в этой же строке после условия                          
	    { indtek=ninds; goto aaa; }
    } //------------------------------
		   else                      //На следующую строку.
    {nowstr=1;     //отметили переход на новую строку.
      stroka:	  tekpoz=myf.pos( );
	    //Взяли N позиции читаемой далее строки.

	  //-------- Читаем строку и начинаем анализировать ее:
		  sitstr = myf.readLine();
	   if(sitstr.isEmpty())   //Не конец ли файла ?
{ str2="#E - INTKOM - propusk №1. Ошибка фигурн.скобок. Сброс задачи.Cм. файл";
	       printkl(str2); str2=source[jarus]; printkl(str2);
		   sbros( );
	     }

	    //------------------------
	   sitstr=normstr(sitstr);
	   if(sitstr=="") {indtek=0; goto stroka;}
	      //--------------------------

      // Теперь - содержательная строка .----------------------
	 str1=dotzp(0,sitstr,';',';');
	     if(str1.at(0)!='{')
		 { if(est==(-1)) { indtek=posind+1; return; }
			 indtek=pinds+1; str1=slowo(indtek,sitstr); indtek=pinds+1; 
		 return; 
		 }  //Обошли простой оператор.
		   else                 //есть  '{' в строке
		{ indtek=ninds; }      //indtek стал на '{'    
   } //---------------------
   aaa:                       //Обход составного оператора {   }
    stet=1; indtek=indtek+1;  //indtek на левой фигурн.скобке,
			                  //в tekpoz -  позиция строки.
   bbb: k=sitstr.count( );
       k=(k-1);   for(i=indtek; i<=k; i++)
	  { a=sitstr.at(i);
	    if(a=='{') stet=stet+1;
	    if(a=='}') stet=(stet-1);
		  if(stet==0)
	     { indtek=i+1; return; }
	  } //-------------------------
   ccc:	     tekpoz=myf.pos( );
	   sitstr = myf.readLine();
	     if(sitstr.isEmpty())
{ str2="#E - INTKOM - propusk №2. Ошибка фигурн.скобок. Сброс задачи.Cм. файл";
	       printkl(str2); str2=source[jarus]; printkl(str2);
		   sbros( );
}//----------------------
		 nowstr=1;   //перешли на новую строку
	   sitstr=normstr(sitstr);
	   if(sitstr=="")  goto ccc;  //на чтение следующей строки
	      //--------------------------

       indtek=0;   goto bbb;     //На просмотр следующей непустой строки.
 }//Конец propusk==================================================


  void propusk1(void)               //Пропускает 1 оператор ,
  { QString str1,str2;             //находя новое значение tekpoz
    int i,k;  QChar a;            // и indtek - за оператором.
    int stet;                      //stet - счетчик фигурн.скобок.
	nowstr=0;
	       stek="propusk";
		  if(indtek<=posind)
    { str1=dotzp(indtek,sitstr,';',';');
	    if(str1.at(0)!='{')
		{ if(est==(-1))             //Последний в строке оператор
		                            //отладчика и без  ';'    .
		      { indtek=posind+1; return; }
		    indtek=pinds+1; str2=slowo(indtek,sitstr); //Это ';'  .
			if(str2!=";")
			{printkl("#E - INTKOM - З-ча снята. Нет ';' в строке: ");
			 str2="#  "+sitstr; printkl(str2); sbros( );
			}
            indtek=pinds+1;         //Сдвиг за  ';'  .
			return;            
		 } //----------- Обошли простой оператор в этой же строке. 
	    else                             // есть '{'  в этой же строке после условия                          
	    { indtek=ninds; goto aaa; }
    } //------------------------------
		   else                      //На следующую строку.
    {nowstr=1;     //отметили переход на новую строку.
      stroka:	  tekpoz=myf.pos( );
	    //Взяли N позиции читаемой далее строки.

	  //-------- Читаем строку и начинаем анализировать ее:
		  sitstr = myf.readLine();
	   if(sitstr.isEmpty())   //Не конец ли файла ?
{ str2="#E - INTKOM - propusk1 №1. Ошибка фигурн.скобок. Сброс задачи.Cм. файл";
	       printkl(str2); str2=source[jarus]; printkl(str2);
		   sbros( );
	     }

	    //------------------------
	   sitstr=normstr(sitstr);
	   if(sitstr=="") {indtek=0; goto stroka;}
	      //--------------------------

      // Теперь - содержательная строка .----------------------
	 str1=dotzp(0,sitstr,';',';');
	     if(str1.at(0)!='{')
		 { if(est==(-1)) { indtek=posind+1; return; }
			 indtek=pinds+1; str1=slowo(indtek,sitstr); indtek=pinds+1; 
		 return; 
		 }  //Обошли простой оператор.
		   else                 //есть  '{' в строке
		{ indtek=ninds; }      //indtek стал на '{'    
   } //---------------------
   aaa:                       //Обход составного оператора {   }
    stet=1; indtek=indtek+1;  //indtek на левой фигурн.скобке,
			                  //в tekpoz -  позиция строки.
   bbb: k=sitstr.count( );
       k=(k-1);   for(i=indtek; i<=k; i++)
	  { a=sitstr.at(i);
	    if(a=='{') stet=stet+1;
	    if(a=='}') stet=(stet-1);
		  if(stet==0)
	     { indtek=i+1; return; }
	  } //-------------------------
   ccc:	     tekpoz=myf.pos( );
	   sitstr = myf.readLine();
	     if(sitstr.isEmpty())
{ str2="#E - INTKOM - propusk1 №2. Ошибка фигурн.скобок. Сброс задачи.Cм. файл";
	       printkl(str2); str2=source[jarus]; printkl(str2);
		   sbros( );
}//----------------------
		 nowstr=1;   //перешли на новую строку
	   sitstr=normstr(sitstr);
	   if(sitstr=="")  goto ccc;  //на чтение следующей строки
	      //--------------------------

       indtek=0;   goto bbb;     //На просмотр следующей непустой строки.
 }//Конец propusk1==================================================


  void uslif(void)          //Дает в otw ответ о выполнении условия.
  { QString str1,str2,str3; //1 - выполнено. indtek задает левую
    QChar a,b; int i,p,j,z;	    //скобку, в конце - на правой. Берет 
     long double x,y;	    //6 операций сравнения: ==, !=, >, <,
			stek="uslif";   // >=, <=  . Условие - в одной строке.
	       indtek=indtek+1;     //Вышли на тело условия.
		   str3=slowo(indtek,sitstr);               //Начало анализа на работу со строками
		   if(str3=="\"")      //Строки
//___________________________ Начало работы со строками ____________________________
{int op=0;
 indtek=pinds+1;  //На начало первого текста
 str3=dotzp(indtek,sitstr,'"','"');  //Взяли первый текст в str3.
 if(otw==(-1)) 
	{printkl("# Оператор if. Нет двойных кавычек в строке:");
      str5="#  "+sitstr; printkl(str5); sbros( );
	}//-----------

 if(str3=="") indtek=pinds;  // При пустой строке,indtek ставим на " .
    else        //Непустая строка,indtek перед "  .
indtek=pinds+1;         //indtek  теперь тоже на "  .

	       //---------------------------------
 indtek=indtek+1;   //На начало знаков операции
  str4=slowo(indtek,sitstr);  //Первый знак
  indtek=pinds+1;
  str5=slowo(indtek,sitstr);  //Второй знак
      if((str4=="=")&&(str5=="=")) op=0;   //равно
	  else if((str4=="!")&&(str5=="=")) op=1;   //не равно
      else
      {printkl("# Оператор if. Ошибка знака операции со строками. См строку:");
	   str5="#  "+sitstr; printkl(str5); sbros( );
	  }//---------------

   indtek=pinds+1; //На "
   str5=slowo(indtek,sitstr);  //Это "
   if(str5!="\"")
 {printkl("# Оператор if. Нет двойных кавычек в строке:");
  str5="#  "+sitstr; printkl(str5); sbros( );
 }//-----------
  indtek=pinds+1; //На начало второго текста
  str4=dotzp(indtek,sitstr,'"','"'); //Взяли второй текст в str4.
 if(otw==(-1)) 
	{printkl("# Оператор if. Нет двойных кавычек в строке:");
      str5="#  "+sitstr; printkl(str5); sbros( );
	}//-----------

  if(str4=="")  indtek=pinds;        // при пустой строке indtek на "  .
    else indtek=pinds+1; //При непустой теперь тоже на "  .
	    //--------------------------------
     indtek=indtek+1;   //На правую )
   str5=slowo(indtek,sitstr); //Взяли )
   if(str5!=")")
 {printkl("# Оператор if. Нет правой круглой скобки после \" в строке:");
  str5="#  "+sitstr; printkl(str5); sbros( );
 }//-----------
            indtek=pinds;  //Окончательная нужная фиксация indtek на )
			   //-----------------------------
                    if(op==0)
			{if(str3==str4) otw=1; else otw=0;}
                    if(op==1)
			{if(str3!=str4) otw=1; else otw=0;}

                        return;  
}
//____________________________ Конец работы со строками ____________________________
		          
		          // --- Продолжение, если были не строки:

   str1=slowo2(indtek,sitstr); //Взяли первый операнд или break..,konez.или exist

			    if(kws==1) 
				str1=istima(str1);   //истинное имя

				if((str1=="+")||(str1=="-"))  //если - знак числа в str1 !
			{str4=str1; indtek=pinds+1;
			 str5=slowo2(indtek,sitstr);
			    if(kws==1)
              str5=istima(str5);
			 str1=str4+str5;
			 
            }    
			//В  str1- уже первое слово.                        
	    //-----------------------------------
           if(str1=="exist")
	{indtek=pinds+1;
      str2=dotzp(indtek,sitstr,')',')');   //В str2  - имя файла
     indtek=pinds+1;  //на )
	 str3=slowo(indtek,sitstr);   if(str3!=")")
	 {printkl("#E  Оператор if. Нет правой скобки в if в строке:");
      str4="#  "+sitstr; printkl(str4); sbros( );
	 }
	 indtek=pinds; //Выведен на правую скобку (как требуется)
	   //-----------------------------------
         str4=dotzp(0,str2,':',':');
      if(est>=0)      //Есть :  .Только этот путь и файл в str2
	  {int i;  
	    i=access(str2.toStdString().c_str(),0);
        if(i==0) {otw=1; return;}
		else     {otw=0; return;}
      }//------------------------
	  else           //Теперь нет :
      {int i; // char bufp[1000];
////	   GetCurrentDirectory(1000,&bufp[0]);
////	   str4=(char*)bufp;
///////	   str3=str4+"/"+str2; str2=str3; //Для однотипности
	   	   str3=DefaultDir+"/"+str2; str2=str3; //Для однотипности
	    i=access(str2.toStdString().c_str(),0);
        if(i==0) {otw=1; return;}
		else     {otw=0; return;}
	  }//------------------------
	}//*************************************

	       if(str1=="stopok")
	{ if(Stopli==1)   otw=1; else otw=0;
		 indtek=pinds+1; str2=slowo(indtek,sitstr);
		 indtek=pinds;   //Точно на правую скобку.
		 if(str2!=")")
{ printkl("#E Оператор if. Задача снята. Нет правой скобки в строке: ");
		   str2="#  "+sitstr; printkl(str2); sbros( );
		 }
       return;
	}
       //-***********************************


		   if(str1=="stop")
 {UINT indt,pin;
	indt=indtek; pin=pinds;
	    indtek=pinds+1;
   str2=slowo(indtek,sitstr);			   
		if(str2==")")         //Это просто stop.
	{if(Stopli==1) otw=1; else otw=0;		
		 indtek=pinds;   //Точно на правую скобку.
       return;
	}//-------------------------------
		
	//---- восстановление:
  indtek=indt; pinds=pin;		
//---------------------------------------------------------------------------			   
			   indtek=pinds+1;       str1=slowo(indtek,sitstr);
		 if(str1.at(0)!='=')
{printkl("#E - INTKOM - Ошибка первого знака условия при stop в строке");
	    str2="#   "+sitstr; printkl(str2);
	      printkl("Задача снята."); sbros( );
	 }  //--------------------------------
	  indtek=pinds+1;       str1=slowo(indtek,sitstr);
		 if(str1.at(0)!='=')
{printkl("#E - INTKOM - Ошибка второго знака условия при stop в строке");
	    str2="#   "+sitstr; printkl(str2);
	      printkl("Задача снята."); sbros( );
	 }  //--------------------------------
	   indtek=pinds+1; str1=slowo(indtek,sitstr);
		    a=str1.at(0);
			

//==============================================================
                    //Правка для реализации адреса как значения переменной
		       if(a=='*')                   //Т.е. адрес.
	   { indtek=pinds+1;
                     str1=slowo(indtek,sitstr);
             if(str1=="~")          //надо взять значение последующей переменной //^^^
		{QString ss; //для имени переменной
			 UINT zz;
			   indtek=pinds+1;  //на начало имени переменной
               ss=slowo(indtek,sitstr);  //взяли в ss имя переменной
               i=rasp(ss);
               if(i!=3)
		{printkl("#E - INTKOM - Задача снята. Не имя переменной в строке:");
          str2="# "+sitstr; printkl(str2); sbros();
		}//-----------------------------

				  p=0; for(j=0;j<=kolperem;j++)
			  {if(glst[j]!=jarus) continue;
			   if(imper[j]==ss) {p=1;break;}
              }
			       if(p==0)
			{printkl("#FF INTKOM - Ошибка имени переменной в строке:");
              str2="#   "+sitstr; printkl(str2); sbros();
			}
               zz=(UINT)znper[j];    // значение переменной  - адрес
             str1.sprintf("%x",zz);   //преобразовали z в 16-тир текст
               //--------------------------- 

		}//-------------------------- конец с адресом - именем переменной 


    else     str1=slowo(indtek,sitstr);   //Взяли чистый адрес из программы.

	         //  Общая концовка:
		 str3="0x"+str1; str1=str3;       // Коррекция от Романова.


//==============================================================

	            x=(long double)fpere(str1);

         indtek=pinds+1;            //На [
		 str2=slowo(indtek,sitstr); //Взяли [
		         if(str2!="[")
		{printkl("#E - INTKOM - Задача снята. Нет '[' в строке:");
          str2="# "+sitstr; printkl(str2); sbros();
		}//-----------------------------
         indtek=pinds+1;            //На индекс - номер ВМ
		 str2=slowo(indtek,sitstr); //Взяли индекс
		 i=rasp(str2);              //Распознаем его
		        if((i==2)||(i==4))
		{printkl("#E - INTKOM - Задача снята. Ошибочный индекс в строке:");
          str2="# "+sitstr; printkl(str2); sbros();
		}//-----------------------------
              if(i==3)     //Имя переменной
		{p=0; for(j=0;j<=kolperem;j++)
			  {if(glst[j]!=jarus) continue;
			   if(imper[j]==str2) {p=1;break;}
              }
			       if(p==0)
			{printkl("#F INTKOM - Ошибка имени переменной в строке:");
              str2="#   "+sitstr; printkl(str2); sbros();
			}
               z=(int)znper[j]; z=z+1;   //Индекс - значение переменной  - номер ВМ
		}//---------------

	 else  z=(int)fpere(str2);  z=z+1;     //Это значение индекса - номер ВМ

                   if(z<1)
		{printkl("#E - INTKOM - Задача снята. Ошибка в величине индекса в строке:");
          str2="# "+sitstr; printkl(str2); sbros();
		}//-----------------------------
        
				   y=(long double)adresost[z];

        indtek=pinds+1;            //На ]
		str2=slowo(indtek,sitstr); //Взяли ]
                 if(str2!="]")   
		{printkl("#E - INTKOM - Задача снята. Нет ']' в строке:");
          str2="# "+sitstr; printkl(str2); sbros();
		}//-----------------------------
         
		 indtek=pinds+1;                    // На правую скобку.
	     str2=slowo(indtek,sitstr);
		 indtek=pinds;      //Точно на правую скобку.
		 if(str2!=")")
{ printkl("#E - INTKOM - Задача снята. Нет правой скобки в строке: ");
		   str2="#  "+sitstr; printkl(str2); sbros( );
		 }

	     //------------------------------		    
		 if(x==y) { otw=1; return; }     //Здесь проверка условия
	     else     { otw=0; return; }
	   } //Закончили с адресом. ********************************


			else                 //Т.е. текст места останова.
	    { str1=dotzp(indtek,sitstr,'[','[');  //Взяли текст.

         indtek=pinds+1;            //На [
		 str2=slowo(indtek,sitstr); //Взяли [
		         if(str2!="[")
		{printkl("#E - INTKOM - Задача снята. Нет '[' в строке:");
          str2="# "+sitstr; printkl(str2); sbros();
		}//-----------------------------
         indtek=pinds+1;            //На индекс - номер ВМ
		 str2=slowo(indtek,sitstr); //Взяли индекс
		 i=rasp(str2);              //Распознаем его
		        if((i==2)||(i==4))
		{printkl("#E - INTKOM - Задача снята. Ошибочный индекс в строке:");
          str2="# "+sitstr; printkl(str2); sbros();
		}//-----------------------------
              if(i==3)     //Имя переменной
		{p=0; for(j=0;j<=kolperem;j++)
			  {if(glst[j]!=jarus) continue;
			   if(imper[j]==str2) {p=1;break;}
              }
			       if(p==0)
			{printkl("#F INTKOM - Ошибка имени переменной в строке:");
              str2="#   "+sitstr; printkl(str2); sbros();
			}
               z=(int)znper[j]; z=z+1;   //Индекс - значение переменной  - номер ВМ
		}//---------------

	 else  z=(int)fpere(str2);  z=z+1;     //Это значение индекса - номер ВМ

                   if(z<1)
		{printkl("#E - INTKOM - Задача снята. Ошибка в величине индекса в строке:");
          str2="# "+sitstr; printkl(str2); sbros();
		}//-----------------------------
        
        indtek=pinds+1;            //На ]
		str2=slowo(indtek,sitstr); //Взяли ]
                 if(str2!="]")   
		{printkl("#E - INTKOM - Задача снята. Нет ']' в строке:");
          str2="# "+sitstr; printkl(str2); sbros();
		}//-----------------------------

		 indtek=pinds+1;         // На правую скобку   ) .
	     str2=slowo(indtek,sitstr);
		 if(str2!=")")
{ printkl("#E - INTKOM - Задача снята. Нет правой скобки ')'  в строке: ");
		   str2="#  "+sitstr; printkl(str2); sbros( );
		 }
		 indtek=pinds;      //Точно на правую скобку.
		 if(str1==mestost[z]) { otw=1; return; }          //Здесь проверка условия
	      else              { otw=0; return; }
	    }
     }//Конец при stop ************************************

	      //ТЕПЕРЬ - СРАВНЕНИЕ ДВУХ ОПЕРАНДОВ:                          

	 x=sislo(str1);       //Вычислили первый операнд.
		      indtek=pinds+1;
	  str1=slowo(indtek,sitstr);      //Первый знак сравнения.
	  a=str1.at(0); if(znsr(a)!=1) //Т.е. не знак.
{printkl("#E Оператор if. Синтаксическая ошибка в строке");
	    str2="#   " + sitstr;  printkl(str2);
	    printkl("#  Задача снята.");  sbros( );
	  } //---------------------------
		      indtek=pinds+1;
	  str1=slowo(indtek,sitstr); //Второй знак сравнения.
	  b=str1.at(0);  if(znsr(b)==0)
	  { b=' '; }    else { indtek=pinds+1; }
	   // indtek - перед началом второго операнда.
	 //----------------- Вычисляем номер операции сравнения:
	     if((a=='=')&&(b=='='))       i=1;
	else if((a=='!')&&(b=='='))       i=2;       
	else if((a=='>')&&(b==' '))       i=3;       
	else if((a=='<')&&(b==' '))       i=4;       
	else if((a=='>')&&(b=='='))       i=5;       
	else if((a=='<')&&(b=='='))       i=6;
	
else {printkl("#E Оператор if. Ошибка написания операции сравнения в строке");
		str2="#  " + sitstr; printkl(str2);
		printkl("# Задача снята.");    sbros( );
	      }

	 //Номер операции в  i  .-----------------------
	str1=slowo2(indtek,sitstr);     //Взяли второй операнд.                     

			    if(kws==1) 
				str1=istima(str1);   //истинное имя

				if((str1=="+")||(str1=="-"))  //если - знак числа в str1 !
			{str4=str1; indtek=pinds+1;
			 str5=slowo2(indtek,sitstr);
			    if(kws==1)
              str5=istima(str5);
			 str1=str4+str5;
			 
            }    
			//В  str1- уже второй операнд.                        
	    //-----------------------------------

	y=sislo(str1);                 //Вычислили его.
	    indtek=pinds+1;            //На правую скобку.
	    str2=slowo(indtek,sitstr);
		 if(str2!=")")
{ printkl("#E Оператор if. Задача снята. Нет правой скобки в строке: ");
		   str2="#  "+sitstr; printkl(str2); sbros( );
		 }
	    indtek=pinds;         //Точно на правую скобку.
	// Теперь - выполнение сравнения: ----------------
	      if(i==1) { if(x==y) otw=1; else otw=0; }
	else  if(i==2) { if(x!=y) otw=1; else otw=0; }
	else  if(i==3) { if(x>y) otw=1; else otw=0; }
	else  if(i==4) { if(x<y) otw=1; else otw=0; }
	else  if(i==5) { if(x>=y) otw=1; else otw=0; }
	else  if(i==6) { if(x<=y) otw=1; else otw=0; }	       
  }//Конец uslif.=================================================



  void prisw(void)          //Осуществляет присвоение ->znper[prizn].
  { QChar a;                 // indtek - за знаком '='. Оставит indtek
    int z,zz,im,sdw,i,j,p; // на последнем символе ';' или на конце строки.
	int x1,x2,x3,ind1,ind2,tip;
	char r1,*ur1; UCHAR r2,*ur2; short r3,*ur3; USHORT r4,*ur4;
	int r5,*ur5; UINT r6,*ur6,k; float r9,*ur9;
	double r11,*ur11;            short zel1,zel2;          
    long double x,y,v;
	QString str1,str2,str3,str4,str5;       UINT ui1,ui2;           
	                      stek="prisw";
aaa: str1=slowo(indtek,sitstr);  if(str1.at(0)=='(')
     {indtek=pinds+1;
       goto aaa;
	 }//-----------------------------------

		 str1=slowo(indtek,sitstr); 

               if(str1.at(0)=='[')   //Те присвоение от Романова.
	{	indtek=pinds+1;                   //На начало текста индекса.
	  str1=slowo(indtek,sitstr); i=rasp(str1);
	  if((i==2)||(i==4))
	  { printkl("#E - INTKOM - Задача снята. Ошибочный индекс в строке :");
	    str2="#  "+sitstr; printkl(str2); sbros( );
	  }//-------------------------
	            if(i==3)  //str1 - имя переменной
	{p=0; for(j=0;j<=kolperem;j++)
		{ if(glst[j]!=jarus) continue;
				if(imper[j]==str1) {p=1;break;}
		}
           if(p==0)
{printkl("#E - INTKOM - Задача снята. Ошибка имени переменной в строке :");
	    str2="#  "+sitstr; printkl(str2); sbros( );
	  }//-------------------------
           z=(int)znper[j];     //Значение переменной - индекс в [ ].
	}//---------------------------------------
	  else z=(int)fpere(str1);   //Это индекс.

	           if(tipper[prizn]<3)  im=z/4;
          else if(tipper[prizn]<5)  im=z/2;
          else if(tipper[prizn]<=9)  im=z;
		  else if(tipper[prizn]==11)  im=2*z;
else{ printkl("#E - INTKOM - Ошибка типа переменной в prisw.Отказ.");sbros( );}
	  if(im>=rrom) 
{printkl("#F - INTKOM - Массив для чтения из целевой ЭВМ мал.З-ча снята.");
	    str1="# Индекс массива =%d >=rrom=%d"; str2.sprintf(str1.toStdString().c_str(),im,rrom);
		printkl(str2); sbros( );
	  }
 	  if((im>=rrom)||(im<0)||(((pinds-ninds)>=1)&&(im==0)))
{printkl("#E - INTKOM - Задача снята. Нештатный индекс в строке :");
	    str2="#  "+sitstr; printkl(str2); sbros( );
	  }//-------------------------
// Теперь смотрим номер ВМ и определяем по нему  grup:
           indtek=pinds+1;      //На "]"
   str1=slowo(indtek,sitstr);   if(str1!="]")
{str2="#F - INTKOM - Задача снята. См. синтаксис присвоения в строке : ";
		  printkl(str2); str2="#  "+sitstr;
		  printkl(str2); sbros( ); 
}//-----------------------
//========================================================
			
			////////////////////////////////////////////////////////ROM 12.11.07
if(pinds==posind) 
{ zz=TargetVM; //Не задан номер ВМ, принудительно -> TargetVM  Нет ; 
             goto per;
}//--------------------------
else
{UINT indt,pin;
 indt=indtek; pin=pinds;
      indtek=pinds+1;
   str2=slowo(indtek,sitstr);
       if(str2==";")          //Есть ;
	   {zz=TargetVM; //Не задан номер ВМ, принудительно -> TargetVM
         indtek=indt; pinds=pin;
	     goto per;
	   }//------------------

   indtek=indt; pinds=pin;
}//------------------------------------------

     indtek=pinds+1;                   //На начало номера ВМ.
  str1=slowo(indtek,sitstr);
 i=rasp(str1);
	  if((i==2)||(i==4))
	  { printkl("#E - INTKOM - Задача снята. Ошибочный номер ВМ в строке :");
	    str2="#  "+sitstr; printkl(str2); sbros( );
	  }//-------------------------
	            if(i==3)  //str1 - имя переменной
	{p=0; for(j=0;j<=kolperem;j++)
		{ if(glst[j]!=jarus) continue;
				if(imper[j]==str1) {p=1;break;}
		}
           if(p==0)
{printkl("#E - INTKOM - Задача снята. Ошибка имени переменной номера ВМ в строке :");
	    str2="#  "+sitstr; printkl(str2); sbros( );
	  }//-------------------------
           zz=(int)znper[j];    zz=zz+1; //Значение переменной - номер ВМ.
	}//---------------------------------------
	  else 
	  {zz=(int)fpere(str1); zz=zz+1;  //Это номер ВМ
	  }
//=======================================================
  if((zz<1)||(zz>6))	
{printkl("# F - INTKOM  Задача снята. Ошибка номера ВМ в строке:");
   str2="# "+sitstr; printkl(str2); sbros();
} 

per:

  //-------- Теперь можно задать grup по  zz :
       if(zz==1) grup=gruppa1;
  else if(zz==2) grup=gruppa2;
  else if(zz==3) grup=gruppa3;
  else if(zz==4) grup=gruppa4;
  else if(zz==5) grup=gruppa5;
        else    grup=gruppa6;
//------------ Теперь реализация присвоения ------------
	           if(tipper[prizn]==1) 
	  { sdw=z%4; k=grup[im];       if(sdw==3) k=k>>24;
	    else if(sdw==2) k=k>>16; else if(sdw==1) k=k>>8; else { }
	     ur1=(char *)(&k);  r1=ur1[0];
	     znper[prizn]=(long double)r1; } //Присвоили
	       //------------------------------------
	     else if(tipper[prizn]==2)
	  { sdw=z%4; k=grup[im];       if(sdw==3) k=k>>24;
	    else if(sdw==2) k=k>>16; else if(sdw==1) k=k>>8; else { }
	     ur2=(UCHAR *)(&k);  r2=ur2[0];
	     znper[prizn]=(long double)r2; } //Присвоили
	       //------------------------------------
	     else if(tipper[prizn]==3)
	  { sdw=z%2; k=grup[im];       if(sdw==1) k=k>>16;
	     ur3=(short *)(&k);  r3=ur3[0];
	     znper[prizn]=(long double)r3; } //Присвоили
	       //------------------------------------
	     else if(tipper[prizn]==4)
	  { sdw=z%2; k=grup[im];       if(sdw==1) k=k>>16;
	     ur4=(USHORT *)(&k);  r4=ur4[0];
	     znper[prizn]=(long double)r4; } //Присвоили
	       //------------------------------------
	     else if(tipper[prizn]==5) 
	  { k=grup[im]; ur5=(int *)(&k); r5=ur5[0]; 
	    znper[prizn]=(long double)r5; }  //Присвоили
	      //--------------------------------------
	     else if(tipper[prizn]==6) 
	  { r6=grup[im];
	     znper[prizn]=(long double)r6; } //Присвоили
	      //---------------------------------------
	      else if(tipper[prizn]==9) 
	  { k=grup[im];
		 ur9=(float *)(&k); r9=ur9[0];
	    znper[prizn]=(long double)r9; }  //Присвоили
	      //--------------------------------------
	      else if(tipper[prizn]==11) 
      { ur11=(double *)(&grup[im]); r11=ur11[0];
        znper[prizn]=(long double)r11; }  //Присвоили 
        //---------------------------------------
  else 
 {str2.sprintf("#E - INTKOM - Неверный тип переменной =%d в prisw.Отказ.",
			 tipper[prizn]);
		  printkl(str2);   sbros( );
 }//----------------- Теперь концовка после номера ВМ :
   
      if(pinds==posind)  //Конец строки без ";'
	  {indtek=pinds; return;}       //indtek - на конце строки
	  //--------------------------
         indtek=pinds+1;      //Вышли на ";'
  str1=slowo(indtek,sitstr);  //Взяли ";"
            if(str1!=";")
	{ str2="#E - INTKOM - Задача снята. Нет ';' в строке :";
		 printkl(str2); str2="# "+sitstr; printkl(str2); sbros( );
	}//-----------------------------------------------------------
		indtek=pinds;   //indtek - на ";" 
				   return;

} //Конец присвоения от Романова.***************************************

			else  //Теперь - не от Романова. 
		{	 str1=slowo2(indtek,sitstr);                                             
			    if(kws==1) 
				str1=istima(str1);   //истинное имя

				if((str1=="+")||(str1=="-"))  //если - знак числа в str1 !
			{str4=str1; indtek=pinds+1;
			 str5=slowo2(indtek,sitstr);
			    if(kws==1)
              str5=istima(str5);
			 str1=str4+str5;
			 
            }    
			//В  str1- уже первое слово.
                                        			         
				if(pinds==posind) 
			{ 
			 x=sislo(str1);znper[prizn]=pris(prizn,x); 			      
			  indtek=posind;return;
			} //-------------------------
		indtek=pinds+1; str2=slowo(indtek,sitstr); //Взяли второе слово.
                       a=str2.at(0); 
	                   if(znop(a)!=1)    //Те присвоение без операции.
                                         //В str2 ')'  или ';'  .
		{ if((a!=')')&&(a!=';'))
{printkl("#E - INTKOM - З-ча снята.Ошибка знака операции или нет';'в строке :");
			 str1="#  "+sitstr; printkl(str1); sbros( );  }
		  x=sislo(str1); znper[prizn]=pris(prizn,x); //Присвоили.
         aa2: if(a==')')
			  { if(pinds==posind)   {indtek=posind; return; }
			    indtek=pinds+1; str2=slowo(indtek,sitstr);
			    a=str2.at(0); goto aa2;
			  } //-------Проскочили правые круглые скобки.---------
			  if(a!=';')
{printkl("#E - INTKOM - Задача снята. Нет ';' после присвоения в строке :");
			    str2="#  "+sitstr; printkl(str2); sbros( );
		}//--------------------------------------
			       indtek=pinds;  //На ';'  .
				   return;
	   }//Конец присвоения без выполнения операции.*************************

	   //Осталось присвоение с операцией. 
	   //В str1 первый операнд, в str2 знак операции.
                  //Учет операции сдвига :
        if((str2==">")||(str2=="<"))
        {indtek=pinds+1; str4=slowo(indtek,sitstr); //В str4 - второй знак операции
                  if(str2!=str4)
			{printkl("#F - INTKOM - Ошибка в обозначении сдвига в строке:");
              str5="# "+sitstr; printkl(str5); sbros( );
			}
		}
       indtek=pinds+1;
	   str3=slowo2(indtek,sitstr);//В str3 второй операнд.                 //***&&&
           if(kws==1) 
		   str3=istima(str3);   //истинное имя              //***&&&
	   //str3=slowo(indtek,sitstr);//В str3 второй операнд.
	   x=sislo(str1); ind1=indsis; y=sislo(str3); ind2=indsis;

	   if(str2=="+")        {v=x+y; znper[prizn]=pris(prizn,v);}
	   else if(str2=="-")   {v=x-y; znper[prizn]=pris(prizn,v);} 
	   else if(str2=="/")
	   { zel1=0; zel2=0; if(ind1>=(-1))
		{       if(ind1==(-1)) {zel1=1; x1=(int)x; }
	       else if(tipper[ind1]==3) {zel1=1; x1=(int)znper[ind1];}
           else if(tipper[ind1]==4) {zel1=1; x1=(int)znper[ind1];}
		   else if(tipper[ind1]==5) {zel1=1; x1=(int)znper[ind1];}
		   else { }
		}                 if(ind2>=(-1))
		{       if(ind2==(-1)) {zel2=1; x2=(int)y; }
	       else if(tipper[ind2]==3) {zel2=1; x2=(int)znper[ind2];}
           else if(tipper[ind2]==4) {zel2=1; x2=(int)znper[ind2];}
		   else if(tipper[ind2]==5) {zel2=1; x2=(int)znper[ind2];}
		   else { }
		}                 
         //------
                  if((zel1==1)&&(zel2==1))
				{x3=x1/x2; znper[prizn]=(long double)x3; }
		  else  {v=x/y; znper[prizn]=pris(prizn,v);}
	   }//------------------------
	   else if(str2=="%")
	   { zel1=0; zel2=0; if(ind1>=(-1))
		{       if(ind1==(-1)) {zel1=1; x1=(int)x; }
	       else if(tipper[ind1]==3) {zel1=1; x1=(int)znper[ind1];}
           else if(tipper[ind1]==4) {zel1=1; x1=(int)znper[ind1];}
		   else if(tipper[ind1]==5) {zel1=1; x1=(int)znper[ind1];}
           else if(tipper[ind1]==6) {zel1=2; ui1=(UINT)znper[ind1];}
		   else { }
		}                 if(ind2>=(-1))
		{       if(ind2==(-1)) {zel2=1; x2=(int)y; }
	       else if(tipper[ind2]==3) {zel2=1; x2=(int)znper[ind2];}
           else if(tipper[ind2]==4) {zel2=1; x2=(int)znper[ind2];}
		   else if(tipper[ind2]==5) {zel2=1; x2=(int)znper[ind2];}
           else if(tipper[ind2]==6) {zel2=2; ui2=(UINT)znper[ind2];}
		   else { }
		}                 
         //------
                  if((zel1==1)&&(zel2==1))
				{x3=x1%x2; znper[prizn]=(long double)x3; }

                 else if((zel1==1)&&(zel2==2))
				{x3=x1%ui2; znper[prizn]=(long double)x3; }

                 else if((zel1==2)&&(zel2==1))
				{x3=ui1%x2; znper[prizn]=(long double)x3; }

                 else if((zel1==2)&&(zel2==2))
				{x3=ui1%ui2; znper[prizn]=(long double)x3; }

				  else  
		{printkl("# INTKOM - Некорректный операнд % в строке:");
          str5="# "+sitstr; printkl(str5); sbros( );
		}
	   }//------------------------


	   else if(str2=="*")   {v=x*y; znper[prizn]=pris(prizn,v);}           
		 //Присвоили в арифметич.операциях
         else    //логические операции:
	{ if((ind1<(-1))||(ind2<(-1)))
{printkl("#E - INTKOM - Некорректный операнд лог.операции в строке:");
		 str4="# "+sitstr; printkl(str4); sbros( );
		 }
        //-----обработаем первый операнд - x1:
		 if(ind1==(-1))   x1=(int)x;
        else   //переменная с индексом ind1
		{ x1=0; tip=tipper[ind1]; if((tip==1)||(tip==2)||(tip==11))
{printkl("#E - INTKOM - Некорректный тип переменной лог.операции в строке:");
		 str4="# "+sitstr; printkl(str4); sbros( );
		}//-------------------
		      if(tip==3) {r3=(short)x; x1=r3;}
         else if(tip==4)
			{r4=(USHORT)x; ur4=&r4; ur5=(int *)ur4;
		             x1=ur5[0]; x1=0177777&x1;}
         else if(tip==5)  x1=(int)x;
         else if(tip==6)
			{r6=(UINT)x; ur6=&r6; ur5=(int *)ur6; x1=ur5[0];}
		 else if(tip==9)          
			{r9=(float)x; ur9=&r9; ur5=(int *)ur9; x1=ur5[0];} 
         else {printkl("#F - INTKOM - OSIBKA 5"); sbros( ); }
        }//закончили с x1.


        //-----обработаем второй операнд - x2:
		 if(ind2==(-1))   x2=(unsigned int)(y);
        else   //переменная с индексом ind2
		{ x2=0; tip=tipper[ind2]; if((tip==1)||(tip==2)||(tip==11))
{printkl("#E - INTKOM - Некорректный тип переменной лог.операции в строке:");
		 str4="# "+sitstr; printkl(str4); sbros( );
		}//-------------------
		      if(tip==3) {r3=(short)y; x2=r3;}
         else if(tip==4)
			{r4=(USHORT)y; ur4=&r4; ur5=(int *)ur4;
		             x2=ur5[0]; x2=0177777&x2;}
         else if(tip==5)  x2=(int)y;
         else if(tip==6)
			{r6=(UINT)y; ur6=&r6; ur5=(int *)ur6; x2=ur5[0];}
		 else if(tip==9)          
			{r9=(float)y; ur9=&r9; ur5=(int *)ur9; x2=ur5[0];} 
         else {printkl("#  БЯКА - 2"); sbros( ); }
        }//закончили с x2.
             //Теперь - выполнение логических операций:
              if(str2=="&")  x3=x1&x2;
         else if(str2=="|")  x3=x1|x2;
         else if(str2=="^")  x3=x1^x2;
         else if(str2==">")  x3=x1>>x2;
         else if(str2=="<")  x3=x1<<x2;
           else {printkl("#F - INTKOM -  OSIBKA 6"); sbros( ); }
         //Теперь надо вписать результат в znper[prizn],
		 //учитывая тип результата в tipper[prizn].  
      tip=tipper[prizn];  if((tip==1)||(tip==2)||(tip==11))
{printkl("#E - INTKOM - Некорректный тип результата лог.операции в строке:");
		 str4="# "+sitstr; printkl(str4); sbros( );
		}//-------------------
                 if(tip==3) 
	  {ur3=(short *)(&x3); r3=ur3[0]; znper[prizn]=(long double)r3;}
           else  if(tip==4) 
	  {ur4=(USHORT *)(&x3); r4=ur4[0]; znper[prizn]=(long double)r4;}
           else  if(tip==5)             znper[prizn]=(long double)x3;
           else  if(tip==6) 
	  {ur6=(UINT *)(&x3); r6=ur6[0]; znper[prizn]=(long double)r6;}
           else   
	  {ur9=(float *)(&x3); r9=ur9[0]; znper[prizn]=(long double)r9;}

	}//конец и логических операций

    //-----------------------------------------------------                
					indtek=pinds; if(pinds==posind) {  return;}
	   indtek=indtek+1;     			
     aa3: str2=slowo(indtek,sitstr); if(str2==")")
		  { if(pinds<posind) {indtek=indtek+1;  goto aa3;}
			   else  { indtek=posind; return; }
			 } 
			 //Проскочили правые круглые скобки.
		         
                 if(str2!=";")
{printkl("#E - INTKOM - Задача снята.Нет ';' после двухмест.операции в строке:");
          str2="#  "+sitstr; printkl(str2); sbros( );
		}//-----------------------------
				 indtek= pinds;        //На ';'  .
                 return; 
         
	}//Скобка завершения присвоения не от Романова.
  }//Конец prisw.==================================================


  long double pris(int prizn,long double x)
  //Возвращает x в типе tipper[prizn].
  {char a1; UCHAR a2; short a3; USHORT a4; int a5; UINT a6;
   float a9; double a11; long double a;
             if(tipper[prizn]==1)
	{a1=(char)x; a=(long double)a1;}
        else if(tipper[prizn]==2) 
	{a2=(UCHAR)x; a=(long double)a2;}
        else if(tipper[prizn]==3) 
	{a3=(short)x; a=(long double)a3;}
        else if(tipper[prizn]==4) 
	{a4=(USHORT)x; a=(long double)a4;}
        else if(tipper[prizn]==5) 
	{a5=(int)x; a=(long double)a5;}
        else if(tipper[prizn]==6) 
	{a6=(UINT)x; a=(long double)a6;}
        else if(tipper[prizn]==9) 
	{a9=(float)x; a=(long double)a9;}
        else if(tipper[prizn]==11) 
	{a11=(double)x; a=(long double)a11;}
		else{printkl("#F - INTKOM -Ошибка в pris."); sbros( );}
            return(a);
  }//=====================================================

  
  void unist(int jar)    //Уничтожает переменные файла яруса jar.
  {  while(glst[kolperem]==jar)  kolperem=kolperem-1;
        if(kolperem<(-1))
     { printkl("#F - INTKOM - Ошибка уничтожения переменных.");
		  str3.sprintf("# Ярус %d",jar);
		  printkl(str3);                  sbros( );
		}
  }//=========================================================

  void unistpar(int jar)
  {  while(jrpar[kolpar]==jar)  kolpar=kolpar-1;
        if(kolpar<(-1))
     {printkl("#F - INTKOM - Ошибка уничтожения лок.параметров.");
		  str3.sprintf("# Ярус %d",jar);
		  printkl(str3);                  sbros( );
		}
  }//=========================================================
	
  void osibind( )
  {printkl("#F - osibind - Ошибка синтаксиса. Задача снята на строке: ");
    str1="#  "+sitstr; printkl(str1); sbros( );
  } //=======================================================

  void fail()
  {QString s;
        if(jarus!=0)
		{s="#  Командный файл   "; 
	      s=(s+source[jarus]);
          printkl(s);
		}
  }//===================================================


    void xwostcom(QString s)
  //Передает параметры из  s   вызываемому ком.файлу в command,когда они есть.  		
	{int n,i,j,indt,indzap,nindszap,pindszap,estzap;
	 QChar sa;         QString st2,st4,st5;
       indzap=indtek; nindszap=ninds; pindszap=pinds; estzap=est; //Запомнили.
      n=0;  indt=0;
powtor: st2=dotzp(indt,s,',',';'); //Взяли обозначение параметра
		sa=st2.at(0);       //Первый символ обозначения
		       if((sa!='~')&&(sa!='^'))          //Не переменная в st2 !
		{
		  kolpar=kolpar+1; par[kolpar]=st2;  jrpar[kolpar]=jarus+1;
           nomer[kolpar]=n; n=n+1;         // передали
		   indt=pinds+1;     //Вышли за st2
		}//---------------- конец непеременной

              else if(sa=='~')          //~ и переменная в st2
	{	{ indt=ninds+1;   //Пропустили ~               
		  st4=slowo(indt,s);  //В st4 имя переменной
                i=(-1);  for(j=0;j<=kolperem;j++)
				{if(glst[j]!=jarus)  continue;
				 if(st4==imper[j]) {i=j; break;} //Нашли переменную
                }//---------------------
                  if(i==(-1))
				{printkl("#F - INTKOM xwostkom. - Ошибка имени переменной в строке:");
				  st5="# "+sitstr; printkl(st5); sbros( );
				}//----------------------

				      j=tipper[i];  //тип переменной


				if(j==1) {xxxx1=(char)znper[i];   st5.sprintf("%x",xxxx1);}
           else if(j==2) {xxxx2=(UCHAR)znper[i];  st5.sprintf("%x",xxxx2);}
           else if(j==3) {xxxx3=(short)znper[i];  st5.sprintf("%x",xxxx3);}
           else if(j==4) {xxxx4=(USHORT)znper[i]; st5.sprintf("%x",xxxx4);}
           else if(j==5) {xxxx5=(int)znper[i];    st5.sprintf("%x",xxxx5);}
           else if(j==6) {xxxx6=(UINT)znper[i];   st5.sprintf("%x",xxxx6);}
           else if(j==9) {xxxx9=(float)znper[i];    st5.sprintf("%.18g",xxxx9);}
           else if(j==11) {xxxx11=(double)znper[i]; st5.sprintf("%.18g",xxxx11);} 
		          else {printkl("#F - INTKOM - БУЗА 3 c ~."); sbros( );}
            kolpar=kolpar+1; par[kolpar]=st5; jrpar[kolpar]=jarus+1; 
			nomer[kolpar]=n; n=n+1;     // передали
                       indt=pinds+1;    // Вышли за st4 - имя переменной
		}
             if(kolpar>=(rpar-1))
	{printkl("#F - INTKOM - Размер массива параметров par[]  мал.");
				                                    sbros( );}
	}//----------------------------------- конец переменной c ~



              else if(sa=='^')          //^ и переменная в st2
	{	{ indt=ninds+1;   //Пропустили ^                      
		  st4=slowo(indt,s);  //В st4 имя переменной
                i=(-1);  for(j=0;j<=kolperem;j++)
				{if(glst[j]!=jarus)  continue;
				 if(st4==imper[j]) {i=j; break;} //Нашли переменную
                }//---------------------
                  if(i==(-1))
				{printkl("#F - INTKOM  xwostkom. Ошибка имени переменной в строке:");
				  st5="# "+sitstr; printkl(st5); sbros( );
				}//----------------------

				      j=tipper[i];  //тип переменной


				if(j==1) {xxxx1=(char)znper[i];   st5.sprintf("%d",xxxx1);}
           else if(j==2) {xxxx2=(UCHAR)znper[i];  st5.sprintf("%d",xxxx2);}
           else if(j==3) {xxxx3=(short)znper[i];  st5.sprintf("%d",xxxx3);}
           else if(j==4) {xxxx4=(USHORT)znper[i]; st5.sprintf("%d",xxxx4);}
           else if(j==5) {xxxx5=(int)znper[i];    st5.sprintf("%d",xxxx5);}
           else if(j==6) {xxxx6=(UINT)znper[i];   st5.sprintf("%d",xxxx6);}
           else if(j==9) {xxxx9=(float)znper[i];    st5.sprintf("%.18g",xxxx9);}
           else if(j==11) {xxxx11=(double)znper[i]; st5.sprintf("%.18g",xxxx11);} 
		          else {printkl("#F - INTKOM - БУЗА 3 c ^."); sbros( );}
            kolpar=kolpar+1; par[kolpar]=st5; jrpar[kolpar]=jarus+1; 
			nomer[kolpar]=n; n=n+1;     // передали
                       indt=pinds+1;    // Вышли за st4 - имя переменной
		}
             if(kolpar>=(rpar-1))
	{printkl("#F - INTKOM - Размер массива параметров par[]  мал.");
				                                    sbros( );}
	}//----------------------------------- конец переменной c ^


			  else {printkl("#F - INTKOM - БУЗА 33."); sbros( ); }



                    if(est==1)
			{st5=slowo(indt,s); //Взяли ','
				indt=pinds+1;   //Вышли за запятую 
				   goto powtor; //На следующий параметр 
             } 
               //---- Восстановление запомненного:
           indtek=indzap; ninds=nindszap; pinds=pindszap; est=estzap;           
             
    }//==============  Конец xwostcom ===============================


    int indimper(QString s)   //Возвр.индекс переменной по ее имени s.
	{int i,j;
	              j=(-1);
                for(i=0;i<=kolperem;i++)
		{ if(glst[i]!=jarus) continue;
		  if(imper[i]==s) {j=i; break;}
		}//-----------------------------
                  if(j==(-1))
		{printkl("#F - INTKOM - indimper - Ошибка имени переменной в строке:");
		  str2="# "+sitstr; printkl(str2); sbros( );
		          return(-1);
		}
                  return(j);
    }//=============================================================


     QString slowo2(int n,QString ss)    //Возвращает из ss первое
     { int i,k; QChar a; QString s,str1; //слово от ее индекса - n,
		   //беря символы до разделителя или до конца строки. Если
           //слово - разделитель, то и выдастся сам разделитель.
	       // При этом в pinds будет индекс последнего символа слова, 
	       // а в ninds - индекс фактического начала слова.
	       //Не считает разделителем квадр.скобки.
	       //Если они есть, то делает  kws=1, иначе 0.
	 if(n>posind) osibind( );  s=ss;           kws=0;         
	for(i=0; i<n; i++)  s.replace(i,1,' '); //Стерли начало ненужное.
	   //-----------------------
	   k=s.count( ); k=(k-1);       //k - последний индекс в s.
	   ninds=(-1);  for(i=n; i<=k; i++)
	   { if(s.at(i)!=' ')  { ninds=i; break; }
	   }
	   if(ninds==(-1))
	   { printkl("#F - INTKOM -slowo: Пустое продолжение в строке");
	                     str1="#  "+sitstr; printkl(str1);
						 str1="# Функция  " + stek; printkl(str1);
						 printkl("# Задача снята");
	                     sbros( ); }
	   //------------------------ ninds готово.

	   a=s.at(ninds);                 //Первый символ слова.
		     if(razd2(a)==1)              //Если a - разделитель.
	  {   for(i=ninds+1; i<=k; i++)    s.replace(i,1,' '); 
		     //Вписали пробелы в хвост.
	    s =s.trimmed(); //Обжали слово из разделителя.
	    pinds=ninds; return(s);
	  }   //-------------- Далее, если слово не разделитель:
              for(i=ninds; i<=k; i++)
			  { a=s.at(i);
			    if((a=='[')||(a==']'))  kws=1;
			   if(razd2(a)!=1)  pinds=i;
			                else   break;
			  }	                                // pinds готово.			
	  for(i=pinds+1; i<=k; i++) s.replace(i,1,' '); //Пробелы - в хвост.
	  s = s.trimmed();
			  return(s);     
     }
     //================================================================

     int razd2(QChar a)          //Возвращает 1, если a - разделиnель.
     {                         //Иначе- 0.    [  и  ]  не разделители
       if((a=='{')||(a=='}')||(a=='(')||(a==')'))
			return(1);
       if((a==';')||(a=='^')||(a=='=')||(a=='>')||(a=='<')||(a=='&'))
			return(1);
       if((a=='|')||(a=='+')||(a=='-')||(a=='/')||(a=='*')||(a==' '))
			return(1);
       if((a==',')||(a=='%')||(a=='"')|| (a==':')||(a=='~')||(a=='!'))
			return(1);
       if((a=='$')||(a=='\t')) return(1);//Знак табуляции - только для tabmet. 
	                                     //В obrstr будут вместо них пробелы.

			return(0);
     }
     //===============================================================

     QString istima(QString ss)  //Возвр.истинное имя переменной массива
		                         //Считается, что  '['   или   ']' есть.
     {int ind,zapn,zapp,zapest,n;
	  long double x;  QString st1,st2,st5,str3;
	                stek="istima";
      zapn=ninds; zapp=pinds; zapest=est;  
	          //Чтобы не испортить их для функции верхнего яруса
                           ind=0;
         st1=dotzp(ind,ss,'[','[');             //Взяли до '['
                       if(est!=1)
		{printkl("#F - INTKOM - Нет  [  в имени элемента массива в строке:");
          st1="# "+sitstr; printkl(st1); sbros( );
		}//---------------------------------------------
                     st5=st1;  st5=st5+"[";
			ind=pinds+1;  st1=slowo(ind,ss);    //Вышди на '['

			ind=pinds+1;   //Перевели   на начало индекса массива


			        st1=dotzp(ind,ss,']',']');  //Взяли до ']' индекс массива
                       if(est!=1)
		{printkl("#F - INTKOM - Нет  ]  в имени элемента массива в строке:");
          st1="# "+sitstr; printkl(st1); sbros( );
		}//---------------------------------------------
                 x=sislo(st1); n=(int)x;  //Перевели индекс в число
				 st2.sprintf("%d",n);  //Текст этого индекса в десятичном виде
                      st5=st5+st2;  st5=st5+"]";  //Это окончательно
              //------------------------------------
                ninds=zapn; pinds=zapp; est=zapest;  //Восстановили
                                               
				             return(st5);
	 }//=====================================================================	


  void kontrima(QString ss,int jar)
//Сбрасывает мой поток, если объявляется уже существующее имя переменной ss в ярусе jar.
  {int i,p; QString soob;
   p=-1;       for(i=0;i<=kolperem;i++)
   {if(glst[i]!=jar) continue;
    if(imper[i]==ss) {p=i; break;}
   }//-----------
        if(p>=0)    //Нашли дубль
		{soob.sprintf("# Дублирующее объявление переменной %s. Ошибка.",ss);
           printkl(soob); if(incl!=0) jarus=jarus+1; sbros( );
        }
  }//================= Конец kontrima ===========================
  

   void wkl(QString s)  //Управляет включением ключей выдачи в проток.файл.
   {int indzap,nindszap,pindszap,estzap,indt;
     QString str2,str3;
	indzap=indtek; nindszap=ninds; pindszap=pinds; estzap=est;
	    //-----------------------------------
                           indt=0;
powtor: str2=dotzp(indt,s,',',',');   //Взяли обозначение ключей
		       indt=pinds+1;          //Вышли за обозначение
			   wklus(str2);           //Включили
			//-------------------------------
                   if(est==1)          //Была запятая после обозначения
		{str3=slowo(indt,s);           //Взяли ,
                 indt=pinds+1;         //За ,
				 goto powtor;
		}//------------------

       indtek=indzap; ninds=nindszap; pinds=pindszap; est=estzap; 

   }//================ конец wkl ================================


   void otkl(QString s)  //Управляет включением ключей выдачи в проток.файл.
   {int indzap,nindszap,pindszap,estzap,indt;
     QString str2,str3;
	indzap=indtek; nindszap=ninds; pindszap=pinds; estzap=est;
	    //-----------------------------------
                           indt=0;
powtor1: str2=dotzp(indt,s,',',',');   //Взяли обозначение ключей
		       indt=pinds+1;          //Вышли за обозначение
			   otklus(str2);           //Включили
			//-------------------------------
                   if(est==1)          //Была запятая после обозначения
		{str3=slowo(indt,s);           //Взяли ,
                 indt=pinds+1;         //За ,
				 goto powtor1;
		}//------------------

       indtek=indzap; ninds=nindszap; pinds=pindszap; est=estzap; 

   }//================ конец otkl ================================




   void wklus(QString s)     //Отрабатывает включение по одному обозначению или все.
   {if(s=="kmk")
	{pesmko=1;
     pesouou=1;
	 nedop=1;
	}//---------
    else if(s=="dll")
    {pesrazb=1;
	 pesdll=1;
	}//---------
    else if(s=="mwr")
    {pesmw=1;
	 pesretr=1;
	}//---------
	else if(s=="tspwr")
    {pestsinxr=1;
	}//---------
	else if(s=="reltel")
    {pesrel=1;
	 pestelem=1;
	}//---------
	else if(s=="osib")
    {pesosib=1;
	}//--------
	else if(s=="proz")
    {prz=1;
	}//---------
	else if(s=="intkom")
    {pesintkom=1;
	}//---------
	else if(s=="him")
    {phim=1;
	}//---------
	else if(s=="mmo")
    {pesmmo=1;
	}//---------
	else if(s=="pwr")
    {osibpwr=1;
	}//---------
	else if(s=="kuk")
    {peskuk=1;
	}//---------
	else if(s=="prer")
    {prerr=1;
	}//---------


    else if(s=="*")
	{pesmko=1; pesouou=1; nedop=1; pesrazb=1; pesdll=1; pesmw=1; pesretr=1; pesmmo=1;
	 pestsinxr=1; pesrel=1; pestelem=1; pesosib=1; prz=1; pesintkom=1; phim=1;
	 osibpwr=1; peskuk=1; prerr=1;
	}//-------------------------------------------------------------------
    else{printkl("#F INTKOM Ошибка обозначения ключа выдачи данных в проток.файл.");
	      printkl("#  Смотри строку:"); str2="# "+s; printkl(str2); sbros( );
		}//------------------------

   }//===================== конец wklus ========================


   void otklus(QString s)   //Отрабатывает отключение по одному обозначению или все.
   {if(s=="kmk")
	{pesmko=0;
     pesouou=0;
	 nedop=0;
	}//---------
    else if(s=="dll")
    {pesrazb=0;
	 pesdll=0;
	}//---------
    else if(s=="mwr")
    {pesmw=0;
	 pesretr=0;
	}//---------
	else if(s=="tspwr")
    {pestsinxr=0;
	}//---------
	else if(s=="reltel")
    {pesrel=0;
	 pestelem=0;
	}//---------
	else if(s=="osib")
    {pesosib=0;
	}//--------
	else if(s=="proz")
    {prz=0;
	}//---------
	else if(s=="intkom")
    {pesintkom=0;
	}//---------
	else if(s=="him")
    {phim=0;
	}//---------
	else if(s=="mmo")
    {pesmmo=0;
	}//---------
	else if(s=="pwr")
    {osibpwr=0;;
	}//---------
	else if(s=="kuk")
    {peskuk=0;
	}//---------
	else if(s=="prer")
    {prerr=0;
	}//---------

    else if(s=="*")
	{pesmko=0; pesouou=0; nedop=0; pesrazb=0; pesdll=0; pesmw=0; pesretr=0; pesmmo=0;
	 pestsinxr=0; pesrel=0; pestelem=0; pesosib=0; prz=0; pesintkom=0; phim=0;
	 osibpwr=0; peskuk=0; prerr=0;
	}//-------------------------------------------------------------------
    else{printkl("#F INTKOM Ошибка обозначения ключа выдачи данных в проток.файл.");
	      printkl("#  Смотри строку:"); str2="# "+s; printkl(str2); sbros( );
		}//------------------------

   }//===================== конец otklus ========================


    QString normstr(QString s)
         //Нормализация строки без раскрытия контекстов
	{QString str2,sitstr; int k,i,j,prizn;
	    if(s=="") return("");
                      sitstr=s;    
      
	   // Сначала обойдем комментарий или пустоту:
					  sitstr = sitstr.trimmed();
	   //sitstr.TrimRight();                 //Сняли правые пробелы
      k=sitstr.count( );     //Кол-во символов в строке.
      k=(k-1);                   //Последний индекс в строке.
	      for(i=0; i<=k; i++)
	 { if(sitstr.at(i)=='\t')    sitstr.replace(i,1,' ');  }
	      //Заменили табуляции на пробелы.        

      i=sitstr.indexOf('#');    if(i!=(-1))
      { for(j=i; j<=k; j++)  sitstr.replace(j,1,' '); }
	// Стерли хвост в строке, начиная с # , - наш комментарий.
    //-------------------------------------------------------
      i=sitstr.indexOf("//");  if(i!=(-1))
      { for(j=i; j<=k; j++)  sitstr.replace(j,1,' ');	}
	//Стерли сишный комментарий.
    //---------------------------------------------------
      str2=stand(sitstr);     if(str2=="")
	  { return(""); }  //выдали пустую строку.				        
     //--------------------------------------------------
	 //          Теперь выбросим
	 //метку в строке, если она есть. Ищем ее, зная текущую
	 //позицию строки и список позиций /с метками/ в pozmet[]:

	  prizn=-1;   for(i=0; i<=kolmet; i++)
	   {   if(tekpoz==pozmet[i])   
		  prizn=dlmet[i];
	   }   //Если метки нет,то prizn не изменится.

		     if(prizn>=0)          //Индекс ':' в sitstr
	   {    for(i=0; i<=prizn; i++)    
		  sitstr.replace(i,1,' ');     //Метку стерли.
	   }
	   //-----------------------------------
      str2=stand(sitstr);     if(str2=="")
	  { return(""); }  //выдали пустую строку.				        
      //--------------------------------
	  sitstr = sitstr.trimmed();
	  //sitstr.TrimRight();
      k=sitstr.count( );     //Кол-во символов в строке.
      k=(k-1);                   //Последний индекс в строке.
      posind=k;


                   return(sitstr);
    }//================ конец normstr ================

	QString wstawka(QString s)
    //Вставляет   '?' в s по indtek 
	{int i,k; QString sis;

	   sis=s+"?";    //будет преобразовываться
	   k=s.count(); k=k-1;  //последний индекс в s.
	                 if(indtek>(k+1))
		{printkl("# Ошибка алгоритма в INTKOM"); sbros( );}
                  posind=k+1;  //последний индекс возвращаемой строки.
          //--------------------------------
            if(indtek==(k+1))
			{return(sis);}
	   else if(indtek==k)
			{sis.replace(k+1,1,s.at(k));
	         sis.replace(k,1,'?');
			 return(sis);
			}
         else                  //indtek<k
			{for(i=indtek;i<=k;i++)
              sis.replace(i+1,1,s.at(i));
		      sis.replace(indtek,1,'?');
			  
              return(sis); 
			}
      
    }//======================= конец  wstawka   ======

	QString snatwstawka(QString s)
    //Возвращает строку без '?'   и в indtekwst настоящий  indtek.
	{int i,k,n; QString sis;
	       
          n=s.indexOf('?');  if(n<0)
		  {printkl("# Ошибка 2 алгоритма в INTKOM"); sbros( );}
		  indtekwst=n;
		//----------------------------------
	   sis=s;
	   k=sis.count(); k=k-1; //пока последний индекс в sis.
	        for(i=n+1;i<=k;i++)
           sis.replace(i-1,1,sis.at(i));
           sis.replace(k,1,' '); //пробел в конец вместо сдвинутого влево символа
		   //--------------------------------
		   //sis.TrimRight();
		   sis = sis.trimmed();
		   return(sis);
    
	}//===================== конец snatwstawka ======
	


            //  КОНЕЦ    ФАЙЛА    intkom.cpp  .
