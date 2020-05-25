#include "PKM.h"
#include "qcoreapplication.h"
HANDLE eDirective;

extern HANDLE eProz;
extern HANDLE eOpros;
extern HANDLE eReset;
extern HANDLE kDirective;
extern HANDLE gokomfile;

QString SystemDir, DefaultDir;
QString StartBin;
extern QString ima;
extern QString direct;
extern int sasa6();
extern void prmdier(QString);
extern short kolwm;
extern UINT Komplex[];  //признаки включенной (1) ВМ. Индекс - мой номер ВМ.
extern void Opros(void);
extern void prozessor(void);
extern void ThreaDeb();

int OnLoadMro(QString name);
int OnLoadRom(void);
int ZagrOstpo6(void);

extern UINT NaturalStop, ForGo;
extern UINT TargetVM;
extern void	fZakaz(int z, int targ);	//contin

QString OnGlob(QString name);

extern UINT PtoF(UINT adr);
extern    short writeb6(UINT adr, short nom, UCHAR b);
extern   short readb6(UINT adr, short nom, UCHAR &b);
extern UINT *ukaz(UINT adr, short nom);
extern   UINT      gruppa1[];   //Для чтения группы  чисел из ВМ1.
extern 	 UINT      gruppa2[];   //Для чтения группы  чисел из ВМ2.
extern 	 UINT      gruppa3[];   //Для чтения группы  чисел из ВМ3.
extern 	 UINT      gruppa4[];   //Для чтения группы  чисел из ВМ3.
extern 	 UINT      gruppa5[];   //Для чтения группы  чисел из ВМ3.
extern 	 UINT      gruppa6[];   //Для чтения группы  чисел из ВМ3.


extern M_BP m_bp[];	//для подтягивания массива
					////////////////////////////
extern M_BP m_bp1[];
extern QString ms_bp1[];
extern M_BP m_bp2[];
extern QString ms_bp2[];
extern M_BP m_bp3[];
extern QString ms_bp3[];
extern M_BP m_bp4[];
extern QString ms_bp4[];
extern M_BP m_bp5[];
extern QString ms_bp5[];
extern M_BP m_bp6[];
extern QString ms_bp6[];
extern M_BP *pBP[];
extern QString* p_ms[];

extern Srom lrom[];
extern QString from[];
void start_core()
{
	eDirective = CreateEvent( NULL, TRUE, FALSE, NULL);
	eProz = CreateEvent(NULL, TRUE, FALSE, NULL);
	eOpros = CreateEvent(NULL, TRUE, FALSE, NULL);
	eReset = CreateEvent(NULL, TRUE, FALSE, NULL);
	kDirective = CreateEvent(NULL, TRUE, FALSE, NULL);
	gokomfile = CreateEvent(NULL, TRUE, FALSE, NULL);

	DefaultDir = QCoreApplication::applicationDirPath();
	SystemDir = QCoreApplication::applicationDirPath();
	QString otl;

	Komplex[1] = 1;
	Komplex[2] = 0;
	Komplex[3] = 0;
	Komplex[4] = 0;
	TargetVM = 1;
	kolwm = 4;

	int rarsa = sasa6();
	if (rarsa)
	{
		otl.sprintf("Ошибка построения модели ВМ%d", rarsa - 1);
		prmdier(otl);
	}

	QThread* opr_th = QThread::create(&Opros);
	opr_th->start();
	OnLoadRom();
	QThread* proz_th = QThread::create(&prozessor);
	proz_th->start();


	//очистка массивом отладочной информации
	for (int j = 0; j<100; j++)
	{
		lrom[j].rom = "";
		from[j] = "";
	}

	//////////////////////////////////////////////////////////
	// обнуление массива точек останова
	for (int j = 0; j<1000; j++)
	{
		m_bp1[j].pr = 0;
		m_bp2[j].pr = 0;
		m_bp3[j].pr = 0;
		m_bp4[j].pr = 0;
		m_bp5[j].pr = 0;
		m_bp6[j].pr = 0;
	}
	QString nOsTpo = DefaultDir + "/StartCbk.txt";
	direct = "Command " + nOsTpo;
	QThread* deb_th = QThread::create(&ThreaDeb);
	deb_th->start();
	Beep(3000, 200);
	Sleep(10); Beep(2000, 200); Sleep(18); Beep(3000, 200);
	SetEvent(eDirective);
}


int OnLoadRom(void)
{
	int i, pr, prf;
	QString otl, name, out, nOsTpo, sitstr;
	UCHAR buf[4];
	UINT adr;
	QFile myf;
	//CWaitCursor jj1;

	pr = 0; prf = 0;

	buf[0] = 0; buf[1] = 0; buf[2] = 0; buf[3] = 0;
	//Это якобы обнуление ПЗУ( возможно нужно записывать 0хF)
	for (i = 1; i <= kolwm; i++)
	{
		for (adr = 0xBFC00000; adr<0xBFE00000; adr += 4)
		{
			writeb6(adr, (short)i, 0xFF); writeb6(adr + 1, (short)i, 0xFF);
			writeb6(adr + 2, (short)i, 0xFF);
			writeb6(adr + 3, (short)i, 0xFF);
		}
	}


	//--------------------------------------------------------------
	int prRom = 0;
	nOsTpo = DefaultDir + "/LoadRom.ini";
	buf[0] = 0; buf[1] = 0; buf[2] = 0; buf[3] = 0;
	myf.setFileName(nOsTpo);
	if (!myf.open(QIODevice::ReadOnly))	//файла нет
	{
		nOsTpo = SystemDir + "LoadRom.ini";
		myf.setFileName(nOsTpo);
		if (!myf.open(QIODevice::ReadOnly))	//файла нет
			prRom = 1;
	}


	if (prRom)
	{
		int fat = ZagrOstpo6();///УТОЧНИТЬ

		if (fat) return (1);


		//	hTrMod=::CreateThread(NULL,
		//		0,(LPTHREAD_START_ROUTINE)prozessor,&Globmod,0,&Treadmod);
		//	::SetThreadPriority(hTrMod,THREAD_PRIORITY_BELOW_NORMAL);
		return 0;

	}//1

	 //+==============================================================
	 // Чтение файла
	int pralfa = 0;
	for (;;)
	{
		sitstr = myf.readLine();
		if ((pralfa) || (sitstr.isEmpty())) //Не конец ли файла ?
		{
			myf.close();        //Закрываем  файл.
			if (prf == 0)	// строк не было
			{
				// грузим OSTPO
				if (ZagrOstpo6()) return(1);

				//	hTrMod=::CreateThread(NULL,
				//		0,(LPTHREAD_START_ROUTINE)prozessor,&Globmod,0,&Treadmod);
				//	::SetThreadPriority(hTrMod,THREAD_PRIORITY_BELOW_NORMAL);
				return(0);
			}
			if (prf != 0)	// Было ПЗУ
			{
				//				hTrMod=::CreateThread(NULL,
				//		0,(LPTHREAD_START_ROUTINE)prozessor,&Globmod,0,&Treadmod);
				//	::SetThreadPriority(hTrMod,THREAD_PRIORITY_BELOW_NORMAL);
				return(0);
			}

		}
		// Разбор строки
		sitstr = sitstr.trimmed();
		if (sitstr == "") continue;
		if (sitstr.at(0) == '#') continue;
		if (sitstr.at(0) == '@')
		{
			StartBin = sitstr;
			StartBin.remove(0, 1);
			StartBin = OnGlob(StartBin);
			pralfa = 1; prf = 0; continue;
		}
		if (!OnLoadMro(sitstr)) { prf = 1; continue; }
	}
	return(0);
}
//////////////////////////////////////////
int OnLoadMro(QString name)
{
	QString name1, name2;
	UCHAR* rom;
	QFile file;
	QString out1, otl;
	UINT adr_top, *top, adrd, adrde, adrt, adrte, adrtes;
	int a, b, top_len, const_seek = 0x31210;
	int progr_seek, progr_len, top_seek;
	union
	{
		UCHAR c[20];
		UINT w[5];
	}buf;
	int iVM;
	//CWaitCursor tt;
	int pr = 0;
	name2 = name;
	name2 = OnGlob(name);
	if (name2.indexOf(":") != (-1))
	{
		file.setFileName(name2);
		if (file.open(QIODevice::ReadOnly))
			pr = 1;

	}

	if ((name2.indexOf(":") == (-1)) && (pr == 0))
	{
		name1 = DefaultDir + "/" + name2;
		file.setFileName(name1);
		if (file.open(QIODevice::ReadOnly))
			pr = 1;
	}
	if ((name2.indexOf(":") == (-1)) && (pr == 0))
	{
		name1 = SystemDir + name;
		file.setFileName(name1);
		if (file.open(QIODevice::ReadOnly))
			pr = 1;
	}

	if (pr == 0)
	{
		out1.sprintf("# Ошибка открытия файла %s", name2);
		prmdier(out1);
		return(1);
	}
	file.read((char*)&buf.c[0], 16);
	adr_top = buf.w[1]; a = buf.w[2]; b = buf.w[3];
	top_len = 64 * (b + 1);	// размер ТОПСЕГ
	top_seek = const_seek + (PtoF(adr_top) - 0x1FC00000);	// смещение ТОПСЕГ в .mro

	for (iVM = 1; iVM <= kolwm; iVM++)
	{
		rom = (UCHAR*)ukaz(PtoF(adr_top), (short)iVM);
		if (rom == 0)
		{
			otl.sprintf("# Ошибка при вычислении указателя для ТОПСЕГ в ВМ%d", iVM);
			prmdier(otl);
			file.close(); return(1);
		}
		file.seek(top_seek);
		file.read((char*)rom, top_len);	// запись ТОПСЕГ
	}
	top = (UINT*)rom;
	for (;;)
	{
		if (*(top + 1) == (0)) break;
		if (*(top + 1) == 0xFFFFFFFF) { top += 16; continue; }
		adrt = *(top + 1);
		progr_seek = const_seek + (PtoF(adrt) - 0x1FC00000);
		for (iVM = 1; iVM <= kolwm; iVM++)
		{
			rom = (UCHAR*)ukaz(PtoF(adr_top), (short)iVM);
			if (rom == 0)
			{
				otl.sprintf("# Ошибка при вычислении указателя для программы в ВМ%d", iVM);
				prmdier(otl);
				file.close(); return(1);
			}
			adrt = *(top + 1); adrtes = *(top + 2); adrte = *(top + 3);
			adrd = *(top + 4); adrde = *(top + 5);
			progr_len = (PtoF(adrte) - PtoF(adrtes)) + (PtoF(adrde) - PtoF(adrd));
			file.seek(progr_seek);
			rom = (UCHAR*)ukaz(PtoF(adrt), (short)iVM);
			file.read((char*)rom, progr_len);	// запись программы
		}
		top = top + 16;
	}

	file.close(); return(0);
}

QString OnGlob(QString name)
{
	QString new_name, name1, right, left, glob;
	int r, l, len;
	char* b;

	l = name.indexOf("$");
	if (l == (-1)) return name;
	r = name.indexOf(")");
	if (r == (-1)) return(name);
	len = name.count();
	name1 = name;
	right = name1.right(len - r - 1);
	left = name1.left(l);

	glob = name1.mid(l, r - l + 1);
	len = glob.count();
	glob.replace(0, 1,' ');
	glob.replace(1, 1, ' ');
	glob.replace(len - 1, 1,' ');
	glob = glob.trimmed();


	b = getenv(glob.toStdString().c_str());
	if (b == NULL)
	{
		return(name);
	}
	glob = (char*)b;
	new_name = left + glob + right;
	return(new_name);
}

int ZagrOstpo6(void)
{

	int i;
	UINT *pMem;
	DWORD dll;
	QString out;
	QFile myf;


	if (StartBin != "")
	{
		ima = StartBin;
		myf.setFileName(ima);
		if (!myf.open(QIODevice::ReadOnly))
		{
			out.sprintf("#Фатальная ошибка - нет файла %s ", ima);
			prmdier(out);
			//out.SetAt(0, ' '); AfxMessageBox(out, MB_OK | MB_ICONSTOP);
			return(1);
		}

		myf.close();
	}
	else
	{
		ima = DefaultDir + "/OsTpo.bin";
		myf.setFileName(ima);
		if (!myf.open(QIODevice::ReadOnly))
		{
			ima = SystemDir + "OsTpo.bin";
			myf.setFileName(ima);
			if (!myf.open(QIODevice::ReadOnly))
			{
				out.sprintf("#Фатальная ошибка - нет файла %s ", ima);
				prmdier(out);
				//out.SetAt(0, ' '); AfxMessageBox(out, MB_OK | MB_ICONSTOP);
				return(1);
			}

		}
		myf.close();
	}
	myf.setFileName(ima);
	if (!myf.open(QIODevice::ReadOnly))
		return(1); //Не открывается.
	dll = myf.size();    //Получили кол-во байт в файле.
	myf.close();


	for (i = 1; i <= kolwm; i++)
	{
		pMem = ukaz(0x1fc00000, (short)i);
		myf.setFileName(ima);
		myf.open(QIODevice::ReadOnly);
		myf.read((char*)pMem, dll);
		myf.close();
	}

	return(0);
}