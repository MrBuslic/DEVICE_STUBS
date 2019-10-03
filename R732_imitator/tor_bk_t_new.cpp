#include <Windows.h>
#include <qdebug.h>
#include "defines.h"

#define BYTE  unsigned char
BYTE IIS[512] = { // ИНФОРМАЦИЯ ИСХОДНОГО СОСТОЯНИЯ

	0xBB,0x56,0xB0,0xCF,0xF2,0x24,0x71,0x67,
	0x4A,0x2A,0xF6,0x14,0x10,0x20,0x7A,0xA5,
	0x95,0x84,0x40,0x64,0xCE,0xF3,0x76,0xC4,
	0x2E,0x5E,0x6E,0x9E,0x91,0x3C,0x90,0x8E,
	0xBC,0xA9,0x82,0x78,0x3A,0x84,0xE6,0x11,
	0x52,0x87,0x13,0x4C,0x6A,0xC5,0x8E,0x3D,
	0xF7,0xB6,0xB9,0xC8,0x2D,0x51,0xFE,0x81,
	0xC9,0xC0,0x5D,0x3F,0xF0,0x5F,0x48,0x1B,
	0xE1,0xD4,0x08,0x3E,0x22,0xA7,0x26,0x2F,
	0x2F,0x6C,0x5B,0xD6,0xDB,0x12,0x56,0x61,
	0x91,0xE4,0x09,0x2A,0x4D,0x37,0x2E,0x69,
	0x9E,0x09,0x06,0x6F,0xCD,0x02,0x93,0xAC,
	0x08,0xB6,0xEA,0x7E,0x27,0x7D,0xDE,0x7E,
	0xFA,0xD2,0xD7,0x57,0x3E,0xCE,0x69,0x1E,
	0xB7,0x0C,0xE1,0x4F,0xB0,0xD3,0xD6,0x29,
	0xCA,0x79,0x5C,0xAF,0xBC,0x67,0x8B,0xC3,
	0x26,0x25,0x61,0xA0,0x1B,0xF9,0xFD,0x1E,
	0x13,0x2D,0x9F,0x97,0xE7,0x47,0xA9,0x5A,
	0xDC,0x7E,0xF1,0x08,0x73,0xF1,0xD7,0x93,
	0xD9,0x60,0x9E,0x78,0x78,0x10,0x9F,0xC3,
	0x86,0x3D,0xB1,0xCF,0xBC,0x80,0x9A,0x04,
	0xFA,0x78,0xE2,0xC6,0xFE,0xE1,0xD8,0x00,
	0xF5,0xEC,0xA8,0x7F,0x85,0x8E,0xCE,0xD4,
	0x72,0x35,0x7C,0xC8,0xC7,0xB4,0xB6,0xBE,
	0x65,0x61,0xC8,0xB2,0x1D,0xD8,0x4D,0xDC,
	0x00,0x59,0xBC,0xDF,0x22,0xB5,0x08,0x3C,
	0xC8,0x30,0x76,0x4F,0x64,0xCD,0x9B,0x45,
	0x14,0xCA,0xD5,0x96,0x84,0x61,0x92,0x5A,
	0x02,0x1B,0xDF,0x67,0x88,0x81,0xC9,0x91,
	0x4F,0xA1,0xAB,0xCC,0x2B,0x0E,0xB8,0x1B,
	0x59,0xC6,0x30,0x20,0xD8,0xB4,0x7C,0x4E,
	0xE3,0x83,0xFD,0xEE,0xE3,0x46,0x1E,0x88,
	0x59,0x38,0x31,0x0D,0x60,0x35,0x54,0xF1,
	0x1C,0xC0,0xBF,0x0A,0x62,0x79,0xE8,0xB1,
	0x1B,0x79,0x2D,0x9E,0x18,0x9F,0xD8,0xE6,
	0xA0,0x97,0xC4,0x14,0xE0,0x35,0x92,0x7C,
	0x82,0xC1,0x61,0xB1,0xFC,0xBF,0x28,0x29,
	0xE0,0xFD,0x32,0xC7,0x37,0x05,0x18,0x3A,
	0x24,0x78,0x4B,0xF2,0x6D,0x77,0x24,0x5B,
	0xEF,0xE6,0xD7,0xB3,0x82,0x1B,0xF6,0x7F,
	0x2C,0x2E,0xFE,0xE8,0x54,0xAC,0x65,0xE9,
	0x29,0xDB,0x9D,0xC8,0xA9,0x78,0xCF,0xB6,
	0x0F,0x05,0xEB,0x60,0x94,0x8B,0xD8,0x90,
	0x54,0xA7,0xA4,0xF1,0xCE,0x44,0x75,0x31,
	0x06,0x88,0x18,0x84,0x57,0x8C,0x6B,0x63,
	0x0C,0xFC,0x37,0x47,0x11,0x14,0xFB,0x30,
	0x00,0x93,0xC6,0x05,0x23,0x59,0x14,0x93,
	0x08,0xB1,0x8E,0xF0,0x6A,0xD8,0x8E,0x3F,
	0x30,0x37,0x46,0xAF,0x49,0x9D,0xB4,0x24,
	0xCA,0xD5,0x86,0x8A,0xD5,0x3A,0x2B,0x4C,
	0x2D,0xE4,0x95,0x5E,0x03,0xA9,0x99,0xFB,
	0x19,0xCE,0x14,0xB1,0x6E,0x5A,0xFD,0x69,
	0x89,0xD1,0x11,0xEB,0xFA,0x9F,0x49,0x2F,
	0xC9,0xC2,0xC1,0x4F,0x35,0xDE,0x47,0x3B,
	0x27,0x4B,0x3B,0x19,0xFA,0x38,0x0B,0x03,
	0x0C,0xC9,0xA6,0xB6,0xB5,0xA6,0xDC,0x09,
	0xB3,0x07,0x6B,0x3D,0xE3,0x51,0x8D,0x2E,
	0x56,0xE5,0x8D,0x45,0x6F,0x2B,0x86,0x02,
	0xDA,0x76,0xD4,0x0F,0x8F,0xC9,0x59,0x7A,
	0xDE,0xF0,0x95,0x0C,0x3E,0xA8,0x6C,0x4C,
	0xFC,0x1B,0x59,0xAA,0xFA,0x87,0x19,0x59,
	0x6F,0x8B,0xB8,0xFA,0x0F,0x58,0x29,0x1D,
	0x5B,0x24,0x52,0x1A,0xFB,0x1A,0x3F,0x78,
	0x11,0x66,0x5B,0xFC,0x51,0xB4,0x34,0x7D };


// ====== ФЛАГИ =====
	UINT16 FlagSync;      // 0 - синхронизация не установлена
						  // 1 - синхронизация установлена
	UINT16 FlagPrefKPI;   // 0 - предварительной КПИ не было
						  // 1 - предварительная КПИ была
	UINT16 FlagCntSync;
	UINT16 FlagQWD;

	UINT16 FlagExch0;
	UINT16 FlagExch1;
	UINT16 FlagExchCBK;   // 0 - нет обменов с ЦБК
						  // 1 - есть обмены с ЦБК
	UINT16 FlagReanim;    // 0 - ветка "РЕАНИМАЦИЯ" не запущена
						  // 1 - ветка "РЕАНИМАЦИЯ" запущена
	UINT16 FlagRescue;    // 0 - не запускать ветку "РЕАНИМАЦИЯ"
						  // 1 - запускать ветку "РЕАНИМАЦИЯ"
	UINT16 FlagExitCyclo; // 1 - был выход из циклограммы                     
	UINT16 FlagTK;
	UINT16 FlagTU;
	UINT16 TimeTU;

	unsigned int vecState[15];
	UINT16 CntMKO_Prev;
	UINT16 CntMKO_Next;
	UINT16 vhmNum;
	UINT16 CNT12;
	UINT16 CNT360;
	UINT16 CntLit;

	UINT16 I;
	UINT16 J;
	UINT8  Nkp;     // НОМЕР КП
	UINT8  Nkl;     // НОМЕР КЛ
	UINT32 NumSync; // НОМЕР СИНХРОНИЗАЦИИ
	union           // МАССИВ СЧЕТЧИКОВ СИНХРОНИЗАЦИЙ
	{
		UINT32 W32[16];
		unsigned int W16[32];
	} CntSync;
	UINT32 NumCmd;  // НОМЕР КОМАНДЫ
	UINT32 CntCmd;  // СЧЕТЧИК КОМАНД В ТЕКУЩЕМ СЕАНСЕ СВЯЗИ
	UINT8  OZU1[64];
	UINT16 RGK[64];
	UINT16 RGI[36];
	UINT16 A_0;
	UINT16 A_2;
	UINT16 SCHBK[2];
	UINT8 MSG_KPI[16];
	unsigned int CMD[2];
	UINT16 BUF_SK[32];
	UINT16 WriteZpr;
	UINT16 DeltaTim;
	UINT16 T2;
	UINT16 tim;
	UINT16 T[128];
	UINT16 MSG_KPI_index;
	UINT8  KSKM[4];
	UINT8  KSMK[4];
	UINT16 TestImito;
	UINT16 Cmd;
	UINT16 W[2];
	UINT8 S[255];
	UINT8 L[256];
	unsigned int dataFromAllVCHM[128];
	unsigned int dataWTG[32];

	// ===== ХОДИКИ (таймер на основе TIMER2) =====
	UINT16 T_Prev;
	UINT16 T_Next;
	UINT32 DeltaT;

	// Задает время отсрочки пуска ветки "РЕАНИМАЦИЯ"
	UINT32 tk_lo;   // тут устанавливается время отсрочки
	UINT16 tk_hi;

	UINT32 Tk_LO;   // а тут оно "тикает" вниз
	UINT32 Tk_PREV; // когда Tk_HI становится равным 0,
	UINT16 Tk_HI;   // запускается ветка "РЕАНИМАЦИЯ"

	// Задает периодичность контроля обменов с ЦБК
	UINT32 Tau_CBK;

	// Задает время таймера
	UINT32 TIMER2_t;

	unsigned int buf[2];
	UINT32 Fl0;
	UINT32 Fl1;
	unsigned int BufKS[7];

	UINT16 TRC_Rescue;
	UINT16 TRC_LCA05;
	UINT16 act;
	UINT16 DBG;
	UINT16 MASK;

	int PUPS = 0x1F;
	int msg_count = 0;
	unsigned int* mko_Buffer;

	void SendData
	(
		unsigned int Events,  // карта битов признаков ПУПС5-ПУПС0
		unsigned int* Buffer, // указатель на буфер выдаваемых данных
		unsigned int Words    // количество выдаваемых слов данных: 0 - 32,
							  // если количество выдаваемых слов данных равно 0,
							  // выдаются только признаки ПУПС, без СД
	);
	//void Rescue(void);
	//void StopCP(void);
	//void DbgPause(void);

	void InitKSL(void)
		//
		// ИНИЦИАЛИЗАЦИЯ АЛГОРИТМА ВЫЧИСЛЕНИЯ
		// СПЕЦИАЛЬНОЙ КОНТРОЛЬНОЙ СУММЫ
		//
	{
		//WDT_RESET;
		S[0] = 1;
		for (I = 1; I<255; I++)
		{
			J = ((UINT16)S[I - 1]) * 2;
			if (J>0xff) J ^= 0x11D;
			S[I] = (UINT8)J;
		}
		L[0] = 0;
		for (I = 0; I<255; I++) L[S[I]] = I;
	}

	void UPU(void)
	{
		UINT16 I, J, B, N;
		for (I = 0; I<520; I++)
		{
			//WDT_RESET;
			N = RGI[7] & 63;
			B = OZU1[N];
			OZU1[N] = RGI[0];
			for (J = 0; J<7; J++) RGI[J] = RGI[J + 1];
			RGI[7] = B;
			if (I == 7) for (J = 0; J<8; J++) RGI[J] ^= MSG_KPI[J + 8];
		}
	}

	void USTUP(void)
	{
		UINT16 I, J, B;

		for (I = 0; I<8; I++)
			for (J = 0; J<4; J++)
			{
				//WDT_RESET;
				RGI[J] ^= RGK[A_0];
				RGI[J + 4] = RGI[J];
				B = RGK[A_0] ^ RGK[A_2];
				RGK[A_0] = B << 1;
				if (B & 0x8000) RGK[A_0] ^= 0x100B;
				A_0 += 1; if (A_0 == 31) A_0 = 0;
				A_2 += 1; if (A_2 == 31) A_2 = 0;
			}
	}

	void makeFuckingMagic()
	{
		UINT8 C;
		if (!(MSG_KPI[10] & 2))     // если режим ПК1
		{
			MSG_KPI[12] ^= (UINT8)(SCHBK[0] & 0xff);
			MSG_KPI[13] ^= (UINT8)(SCHBK[0] >> 8);
			MSG_KPI[14] ^= (UINT8)(SCHBK[1] & 0xff);
			MSG_KPI[15] ^= (UINT8)(SCHBK[1] >> 8);
			qDebug() << "SCHBK:";
			for (int i = 0; i < 2; ++i)
			{
				qDebug() << SCHBK[i];
			}
		}
		UPU(); // 520 ТАКТОВ РАБОТЫ СХЕМЫ УПУ

		for (I = 0; I<32; I++) RGK[I] = OZU1[2 * I] ^ (OZU1[2 * I + 1] << 8);
		for (I = 0; I< 4; I++) RGI[I] = RGI[2 * I] ^ (RGI[2 * I + 1] << 8);

		A_0 = 0;
		A_2 = 2;

		for (C = 0; C<32; C++) USTUP(); // ПРОКРУТКА

		for (C = 0; C<8; C++) // ФОРМИРОВАНИЕ ИСС
		{
			USTUP();
			for (I = 0; I<4; I++) BUF_SK[4 * C + I] = RGI[I];
		}
		for (I = 0; I<32; I++) RGK[I] = BUF_SK[I];

		// очистка
		for (I = 0; I<64; I++) OZU1[I] = 0;
		for (I = 32; I<64; I++) RGK[I] = 0;
		for (I = 0; I<36; I++) RGI[I] = 0;
		for (I = 0; I<32; I++) BUF_SK[I] = 0;

		A_0 = 0;
		A_2 = 2;
		qDebug() << "RGK:";
		for (int i = 0; i < 64; ++i)
		{
			qDebug() << RGK[i];
		}
	}

	UINT16 Create_SIIS(void)
		//
		// ФОРМИРОВАНИЕ СИИС
		//
	{
		UINT8  Z, RQ20, Log, KM, MK, C;
		UINT16 I;
		UINT32 AddrKM, AddrMK, OffsCopy;
		UINT16 TstKS;

		// НАЧАЛЬНОЕ ЗАПОЛНЕНИЕ RGI_UPU
		for (I = 0; I<8; I++) 
			RGI[I] = MSG_KPI[I];

		//// ИНИЦИАЛИЗАЦИЯ АЛГОРИТМА ВЫЧИСЛЕНИЯ
		//// СПЕЦИАЛЬНОЙ КОНТРОЛЬНОЙ СУММЫ
		InitKSL();

		//// ВВОДИМ СИ В ОЗУ СХЕМЫ 1 С ПАРАЛЛЕЛЬНЫМ ВЫЧИСЛЕНИЕМ
		//// КОНТРОЛЬНЫХ СУММ И ПРОВЕРКОЙ НА 0
		//for (OffsCopy = 0; OffsCopy<2500; OffsCopy += 1152)
		//{
		//	AddrKM = Addr1_WORK + OffsCopy + (Nkl << 7);
		//	AddrMK = Addr2_WORK + OffsCopy + (Nkl << 7);

		//	KSKM[0] = KSKM[1] = KSMK[0] = KSMK[1] = 0;
		//	KSKM[2] = KSKM[3] = KSMK[2] = KSMK[3] = 0xff;
		//	Z = 0;
		//	for (I = 0; I<64; I++)
		//	{
		//		//WDT_RESET;

		//		//KM = memRead8(AddrKM + I);
		//		//MK = memRead8(AddrMK + I);

		//		OZU1[I] = KM^MK;
		//		Z |= OZU1[I];

		//		if (KM)
		//		{
		//			KSKM[0] ^= KM;
		//			Log = L[KM];
		//			KSKM[1] ^= S[((Log + I + 1) % 0xff)];
		//			KSKM[2] ^= S[(Log + ((I + 1) % 64) + 1) % 0xff];
		//			KSKM[3] ^= S[(Log + 64 - I) % 0xff];
		//		}

		//		if (MK)
		//		{
		//			KSMK[0] ^= MK;
		//			Log = L[MK];
		//			KSMK[1] ^= S[((Log + I + 1) % 0xff)];
		//			KSMK[2] ^= S[(Log + ((I + 1) % 64) + 1) % 0xff];
		//			KSMK[3] ^= S[(Log + 64 - I) % 0xff];
		//		}
		//	}

		//	// Проверка КС
		//	TstKS = 0;
		//	//for (I = 0; I<4; I++) if (KSKM[I] ^ memRead8(AddrKM + 64 + I)) TstKS = 1;
		//	//for (I = 0; I<4; I++) if (KSMK[I] ^ memRead8(AddrMK + 64 + I)) TstKS = 1;
		//	if (!TstKS) break;
		//}

		//// Если записалась нулевая СИ (попытка работать на стертой СИ)
		//if (!Z) return ZeroKey;

		//// если все 3 копии СИ с номером Nkl искажены
		//if (TstKS) return Error;

		for (I = 0; I<64; I++) OZU1[I] = IIS[I + 64 * Nkl];

		if (!(MSG_KPI[10] & 2))     // если режим ПК1
		{
			//bsvBitClear(RQIn, 0x20);
			//mkoSetAddrIn(inAdr);
			//mkoSetReadyIn();
			//cpuDelay(50000);
			SendData(1, NULL, 0); // режим ПК1 - донесение 000001

								  // Прием СЧБК из ЦБК
			//for (I = 0; I<5000; I++)
			//{
			//	cpuDelay(1000);
			//	//WDT_RESET;
			//	//RQ20 = bsvRead(RQIn) & 0x20;
			//	if (RQ20) break;
			//}
			//bsvBitClear(RQIn, 0x20);
			//if (RQ20) for (I = 0; I<2; I++) SCHBK[I] = memRead16((inAdr)+2 * I);
			// Если ЦБК не выдает СЧБК, выдается донесение
			// 001000 - несоответствие номера СИ
			//else return ZeroKey;

			//MSG_KPI[12] ^= (UINT8)(SCHBK[0] & 0xff);
			//MSG_KPI[13] ^= (UINT8)(SCHBK[0] >> 8);
			//MSG_KPI[14] ^= (UINT8)(SCHBK[1] & 0xff);
			//MSG_KPI[15] ^= (UINT8)(SCHBK[1] >> 8);
		}
		else
		{
			SendData(2, NULL, 0); // режим ПК2 - донесение 000010
			makeFuckingMagic();
		}

		return Ok;
	}


	UINT16 TestMSG_KPI(UINT8 _MSG_KPI[16], UINT16 _MSG_KPI_index)
	{
		UINT8  TimeRange, KS[2], Code, Cmd, Tst1, Tst2;
		UINT16 I, Adr, SubAdr, SI_Erase;
		UINT16 res, t, P, Wid_KPI, Test, ExitCode;
		UINT32 J;
		UINT32 AddrSPZU;
		UINT32 OffsCopy;
		UINT32 *p32;
		unsigned int KPI_16, BufData[2];

		//disable_interrupt();
		// StopCP();
		for (int i = 0; i < 16; i++)
		{
			MSG_KPI[i] = _MSG_KPI[i];
			qDebug() << MSG_KPI[i] << QString(" ") << _MSG_KPI[i];
		}
		Test = 0;
		ExitCode = Ok;

		MSG_KPI_index = _MSG_KPI_index;
		// ПРОВЕРКА ВРЕМЕННЫХ И КОЛИЧЕСТВЕННЫХ ПАРАМЕТРОВ
		//if (MSG_KPI_index < 64)
		//{
		//	// обнуление счетчиков сбоев
		//	buf[0] = 0x050F;
		//	//ApiCommand(SA_SERV,&buf[0],1);
		//	goto FinTestMSG_KPI;
		//}

		// ПРОВЕРКА НАЛИЧИЯ ОБМЕНОВ ПО МКО
		//ApiDataRead(17, &vecState[0], 15);
		//CntMKO_Next = vecState[4] & 0x0fff;
		//if (CntMKO_Prev == CntMKO_Next) 
		//	FlagExchCBK = 0; 
		//else 
		//	FlagExchCBK = 1;
		//CntMKO_Prev = CntMKO_Next;
		//if (FlagExchCBK) 
		//{ 
		//	Tk_HI = tk_hi; 
		//	Tk_LO = tk_lo; 
		//}
		Tau_CBK = TAU_CBK;
		CNT12 = 0;

		// И СНОВА ПРОВЕРКА ВРЕМЕННЫХ И КОЛИЧЕСТВЕННЫХ ПАРАМЕТРОВ
		if (MSG_KPI_index < 128)
		{
			BufData[0] = 2;
			BufData[1] = (unsigned int)MSG_KPI_index;
			if (FlagSync) 
				SendData(16, BufData, 2); // донесение 010000
			else          
				SendData(5, BufData, 2); // донесение 000101
			goto FinTestMSG_KPI;
		}

		TimeRange = 0;
		for (I = 0; I < 127; I++)
		{
			//WDT_RESET;
			t = T[I + 1] - T[I];
			if ((t < TimeMinZpr) || (t > TimeMaxZpr))
			{ 
				TimeRange = 1; 
				break; 
			}
		}

		//if (TimeRange)
		//{
		//	BufData[0] = 1;
		//	BufData[1] = (unsigned int)t;
		//	if (FlagSync) 
		//		SendData(16, BufData, 2); // донесение 010000
		//	else          
		//		SendData(5, BufData, 2); // донесение 000101
		//	goto FinTestMSG_KPI;
		//}

		// ПРОВЕРКА КОНТРОЛЬНОЙ СУММЫ
		KS[0] = 0;
		KS[1] = 0;
		for (I = 0; I < 14; I++)
		{
			KS[0] += MSG_KPI[I];
			KS[1] += KS[0];
		}
		if ((KS[0] ^ MSG_KPI[14]) || (KS[1] ^ MSG_KPI[15]))
		{
			if (MSG_KPI[10] & 1) 
				SendData(14, NULL, 0); // донесение 001110 - несоответствие контрольных разрядов КПИ
			else           
				SendData(6, NULL, 0); // донесение 000110 - несоответствие контрольных разрядов СП
			goto FinTestMSG_KPI;
		}

		// ЕСЛИ ПРИНЯТА СП (бит 80 равен 0)
		if (!(MSG_KPI[10] & 1))
		{
			// Выделение и проверка номера КП - биты 82..85
			Nkp = (MSG_KPI[10] >> 2) & 15;
			Nkp -= 1;
			if (Nkp > 1)
			{
				SendData(7, NULL, 0); // донесение 000111 - несоответствие номера КП
				goto FinTestMSG_KPI;
			}

			// Выделение и проверка номера КЛ - биты 86..89
			Nkl = (MSG_KPI[10] >> 6) | ((MSG_KPI[11] & 3) << 2);
			Nkl -= 1;
			if (Nkl > 7)
			{
				SendData(8, NULL, 0); // донесение 001000 - несоответствие номера КЛ
				goto FinTestMSG_KPI;
			}

			FlagCntSync = 0;

			// Если режим ПК1
			if (!(MSG_KPI[10] & 2))
			{
				// Выделение номера синхронизации - биты 90..111
				NumSync = ((UINT32)MSG_KPI[11]) >> 2;
				NumSync += ((UINT32)MSG_KPI[12]) << 6;
				NumSync += ((UINT32)MSG_KPI[13]) << 14;

				// Чтение номеров синхронизаций из всех ВЧМ
				//StopCP();
				//ApiDataLoad(0, &dataFromAllVCHM[0]);
				//ApiDataLoad(1, &dataFromAllVCHM[32]);
				//ApiDataLoad(2, &dataFromAllVCHM[64]);
				//ApiDataLoad(3, &dataFromAllVCHM[96]);

				// Согласование номеров синхронизаций по всем ВЧМ
				//for (I = 0; I < 16; I++) CntSync.W32[I] = 0;
				//p32 = (UINT32*)&dataFromAllVCHM[0];

				//for (I = 0; I < 4; I++)
				//{
				//	if (CntSync.W32[I] < *(p32 + I)) CntSync.W32[I] = *(p32 + I);
				//	if (CntSync.W32[I] < *(p32 + I + 4)) CntSync.W32[I] = *(p32 + I + 4);
				//	if (CntSync.W32[I] < *(p32 + I + 8)) CntSync.W32[I] = *(p32 + I + 8);
				//	if (CntSync.W32[I] < *(p32 + I + 12)) CntSync.W32[I] = *(p32 + I + 12);
				//}

				//for (I = 4; I < 8; I++)
				//{
				//	if (CntSync.W32[I] < *(p32 + I + 12)) CntSync.W32[I] = *(p32 + I + 12);
				//	if (CntSync.W32[I] < *(p32 + I + 16)) CntSync.W32[I] = *(p32 + I + 16);
				//	if (CntSync.W32[I] < *(p32 + I + 20)) CntSync.W32[I] = *(p32 + I + 20);
				//	if (CntSync.W32[I] < *(p32 + I + 24)) CntSync.W32[I] = *(p32 + I + 24);
				//}

				//for (I = 8; I < 12; I++)
				//{
				//	if (CntSync.W32[I] < *(p32 + I + 24)) CntSync.W32[I] = *(p32 + I + 24);
				//	if (CntSync.W32[I] < *(p32 + I + 28)) CntSync.W32[I] = *(p32 + I + 28);
				//	if (CntSync.W32[I] < *(p32 + I + 32)) CntSync.W32[I] = *(p32 + I + 32);
				//	if (CntSync.W32[I] < *(p32 + I + 36)) CntSync.W32[I] = *(p32 + I + 36);
				//}

				//for (I = 12; I < 16; I++)
				//{
				//	if (CntSync.W32[I] < *(p32 + I + 36)) CntSync.W32[I] = *(p32 + I + 36);
				//	if (CntSync.W32[I] < *(p32 + I + 40)) CntSync.W32[I] = *(p32 + I + 40);
				//	if (CntSync.W32[I] < *(p32 + I + 44)) CntSync.W32[I] = *(p32 + I + 44);
				//	if (CntSync.W32[I] < *(p32 + I + 48)) CntSync.W32[I] = *(p32 + I + 48);
				//}

				//// Проверка номера синхронизации на допустимость
				//if (CntSync.W32[Nkl + (Nkp << 3)] >= NumSync)
				//{
				//	SendData(9, NULL, 0); // донесение 001001
				//	goto FinTestMSG_KPI;
				//}
			}

			// Формирование СИИС
			FlagSync = 0;
			res = Create_SIIS();

			if (res == Ok)
			{
				FlagSync = 1; // СИ сформирована - можно преобразовывать КПИ
				// Если режим ПК1
				if (!(MSG_KPI[10] & 2))
					FlagCntSync = 1;    // Для записи вектора допустимых номеров синхронизаций
											// после успешного преобразования КПИ
				CntCmd = 1;         // Инициализация счетчика КПИ
			}

			//if (res==Error) bsvBitSet(RF1,BSV_RF1_Er); // Выставляется признак ОНИ

			if (res == ZeroKey) 
				SendData(8, NULL, 0);     // донесение 001000 - несоответствие номера СИ

			goto FinTestMSG_KPI;
		}

		// ЕСЛИ ПРИНЯТА КПИ (бит 80 равен 1)
		else
		{
			// Проверка кода команды
			Wid_KPI = (UINT16)(MSG_KPI[10] >> 1);
			Wid_KPI |= ((UINT16)MSG_KPI[11]) << 7;
			Wid_KPI &= 0x01ff;
			if ((Wid_KPI^KPI1_U) &&
				(Wid_KPI^KPI1_W) &&
				(Wid_KPI^RKN1) &&
				(Wid_KPI^RKN2) &&
				(Wid_KPI^KPI2_IS) &&
				(Wid_KPI^KPI2_KS))
			{
				SendData(23, NULL, 0); // донесение 010111 - "Нет КПИ (некорректный вид КПИ)"
				goto FinTestMSG_KPI;
			}

			// Если принята КПИ при неустановленной синхронизации
			if (!FlagSync)
			{
				SendData(12, NULL, 0); // донесение 001100 - "Нет КПИ (прием КПИ до синхронизации)"
				goto FinTestMSG_KPI;
			}

			// Номер команды - биты 90..111
			NumCmd = ((UINT32)MSG_KPI[11]) >> 2;
			NumCmd |= ((UINT32)MSG_KPI[12]) << 6;
			NumCmd |= ((UINT32)MSG_KPI[13]) << 14;

			// Если принята команда с меньшим номером
			// или номер команды слишком велик
			if ((NumCmd < CntCmd) || (NumCmd > (CntCmd + 30)))
			{
				SendData(10, NULL, 0); // донесение 001010 - "Нет КПИ (ошибка поля 5 КПИ)"
				goto FinTestMSG_KPI;
			}

			// Докрутка в случае пропуска команд(ы)
			if (NumCmd > CntCmd)
				for (J = 0; J < (NumCmd - CntCmd) << 2; J++)
				{
					//WDT_RESET;
					USTUP();
				}

			// РАСКОДИРОВАНИЕ КОМАНДЫ
			W[0] = (UINT16)MSG_KPI[10] | ((UINT16)MSG_KPI[11] << 8);
			W[1] = (UINT16)MSG_KPI[12] | ((UINT16)MSG_KPI[13] << 8);

			RGI[0] = RGI[2] = W[0];
			RGI[1] = RGI[3] = W[1];

			USTUP();
			USTUP();

			// Раскодирование команды
			CMD[0] = RGI[0] ^ RGI[2] ^ MSG_KPI[0] ^ ((UINT16)MSG_KPI[1] << 8);
			CMD[1] = RGI[1] ^ RGI[3] ^ MSG_KPI[2] ^ ((UINT16)MSG_KPI[3] << 8);

			// Формирование имитовставки
			RGI[0] ^= CMD[0];
			RGI[1] ^= CMD[1];
			RGI[2] ^= W[0];
			RGI[3] ^= W[1];

			USTUP();
			USTUP();

			// Модификация счетчика команд
			CntCmd = NumCmd + 1;

			// Проверка ПЗ
			TestImito = 0;
			if (RGI[0] ^ RGI[3] ^ MSG_KPI[4] ^ ((UINT16)MSG_KPI[5] << 8))
				TestImito = 1;
			if (RGI[1] ^ MSG_KPI[6] ^ ((UINT16)MSG_KPI[7] << 8))
				TestImito = 1;
			if (RGI[2] ^ MSG_KPI[8] ^ ((UINT16)MSG_KPI[9] << 8))
				TestImito = 1;
			qDebug() << "RGI:";
			for (int i = 0; i < 36; ++i)
			{
				qDebug() << RGI[i];
			}
			qDebug() << "RGK:";
			for (int i = 0; i < 64; ++i)
			{
				qDebug() << RGK[i];
			}
			if (TestImito)
			{
				SendData(11, NULL, 0); // донесение 001011 - "Нет КПИ (несоответствие ПЗ)"
				goto FinTestMSG_KPI;
			}

			// Запись номера синхронизации в ОЗУ МУ
			// FlagCntSync выставляется только в режиме ПК1
			if (FlagCntSync)
			{
				//CntSync.W32[Nkl + (Nkp << 3)] = NumSync;
				//// Запись в ОЗУ МУ
				////StopCP();
				//ApiDataSave(0, &CntSync.W16[0]);
				//ApiDataSave(1, &CntSync.W16[8]);
				//ApiDataSave(2, &CntSync.W16[16]);
				//ApiDataSave(3, &CntSync.W16[24]);

				////T_Prev=TIMER2;
				////WDT_RESET;
				//cpuDelay(50000);
				////WDT_RESET;
				//cpuDelay(50000);
				////WDT_RESET;
				//cpuDelay(50000);
				//WDT_RESET;
				// Сброс флага
				FlagCntSync = 0;
			}

			// ОБРАБОТКА РАСКОДИРОВАННОЙ КОМАНДЫ

			// ##### 1. КПИ1-У #####
			if (Wid_KPI == KPI1_U)
			{
				p32 = (UINT32*)&CMD[0];
				Adr = (UINT16)((*p32 >> 24) & 0x1f);
				SubAdr = (UINT16)((*p32 >> 19) & 0x1f);

				// Проверка на допустимость адреса и подадреса
				if ((Adr ^ 2) || ((SubAdr ^ 17) && (SubAdr ^ 26) && (SubAdr ^ 29)))
				{
					SendData(26, NULL, 0); // донесение 011010 - "Нет КПИ (недопустимая команда КПИ)"
					goto FinTestMSG_KPI;
				}

				// Проверка пройдена, выдача КПИ1-У
				KPI_16 = (UINT16)((*p32 >> 3) & 0xffff);
				//ApiCommand(SubAdr, &KPI_16, 1);
				//cpuDelay(50000);
				SendData(17, NULL, 0); // донесение 010001 - "Да КПИ1-У"
				goto FinTestMSG_KPI;
			}

			// ##### 2. КПИ1-В #####
			if (Wid_KPI == KPI1_W)
			{
				// Проверка правильности формирования КПИ1-В
				if (CMD[0] ^ CMD[1] ^ 0xffff)
				{
					SendData(26, NULL, 0); // донесение 011010 - "Нет КПИ (недопустимая команда КПИ)"
					goto FinTestMSG_KPI;
				}

				Code = (UINT8)(CMD[0] & 0xff);
				Cmd = (UINT8)(CMD[0] >> 8);

				// +++++ 1. Проверочная команда 2 (FDFЕ0201) +++++
				if (Code == 1)
				{
					if (Cmd == 2) 
						SendData(3, NULL, 0); // донесение 000011 - "Да КПИ1-В"
					else        
						SendData(26, NULL, 0); // донесение 011010 - "Нет КПИ (недопустимая команда КПИ)"
					goto FinTestMSG_KPI;
				}

				// +++++ 2. Выборочное стирание СИ +++++
				if (Code == 2)
				{
					// Предварительная команда стирания СИ (FFFD0002)
					if (!Cmd)
					{
						FlagPrefKPI = 1;
						SendData(3, NULL, 0); // донесение 000011 - "Да КПИ1-В"
						goto FinTestMSG_KPI;
					}

					// Основная команда стирания СИ (FFFD0102..F7FD0802)
					if (Cmd)
					{
						// Если не было предварительной команды стирания
						if (FlagPrefKPI ^ 2)
						{
							SendData(24, NULL, 0); // донесение 011000 - "Нет КПИ (КПИ1-В до предварительной КПИ1-В)"
							goto FinTestMSG_KPI;
						}

						// Если номер СИ недопустим
						if (Cmd > 8)
						{
							SendData(26, NULL, 0); // донесение 011010 - "Нет КПИ (недопустимая команда КПИ)"
							goto FinTestMSG_KPI;
						}

						// Если осталась последняя СИ
						SI_Erase = 0;
						for (Nkl = 0; Nkl < 8; Nkl++)
						{
							AddrSPZU = Addr1_WORK + (Nkl << 7);
							//if ((memRead8(AddrSPZU + 68) == 0x5a) &&
							//	(memRead8(AddrSPZU + 69) == 0xa5)) 
							//	SI_Erase++;
						}
						if (SI_Erase == 7)
						{
							SendData(27, NULL, 0); // донесение 011011 - "Нет КПИ (запрет стирания КИ)"
							goto FinTestMSG_KPI;
						}

						//for (I=0;I<68;I++) memWrite8(AddrSI+I,0);
						//memWrite8(AddrSI+68,0x5a);
						//memWrite8(AddrSI+69,0xa5);

						// Запись нулей на место СИ1Л в СПЗУ с проверкой
						Nkl = Cmd - 1;
						AddrSPZU = Addr1_WORK + (Nkl << 7);
						//for (OffsCopy = 0; OffsCopy < 2500; OffsCopy += 1152)
						//{
						//	for (P = 0; P < 3; P++)
						//	{
						//		 WDT_RESET;
						//		 FlashWrite_SI(AddrSI,AddrSPZU+OffsCopy,70); // Запись
						//WDT_RESET;
						//		  Проверка правильности записи
						//		Tst1 = 0;
						//		for (I = 0; I < 68; I++)
						//		{
						//			Tst1 |= memRead8(AddrSPZU + OffsCopy + I);
						//			 WDT_RESET;
						//		}
						//		Tst1 |= memRead8(AddrSPZU + OffsCopy + 68) ^ 0x5a;
						//		Tst1 |= memRead8(AddrSPZU + OffsCopy + 69) ^ 0xa5;
						//		if (!Tst1) break;
						//	}
						//}

						// Запись нулей на место СИ2Л в СПЗУ с проверкой
						AddrSPZU = Addr2_WORK + (Nkl << 7);
						//for (OffsCopy = 0; OffsCopy < 2500; OffsCopy += 1152)
						//{
						//	for (P = 0; P < 3; P++)
						//	{
						//		//WDT_RESET;
						//	   // FlashWrite_SI(AddrSI,AddrSPZU+OffsCopy,70); // Запись
					 //  //WDT_RESET;
						//		// Проверка правильности записи
						//		Tst2 = 0;
						//		for (I = 0; I < 68; I++)
						//		{
						//			Tst2 |= memRead8(AddrSPZU + OffsCopy + I);
						//			// WDT_RESET;
						//		}
						//		Tst2 |= memRead8(AddrSPZU + OffsCopy + 68) ^ 0x5a;
						//		Tst2 |= memRead8(AddrSPZU + OffsCopy + 69) ^ 0xa5;
						//		if (!Tst2) break;
						//	}
						//}

						Tst1 = 0;
						Tst2 = 0;
						if (Tst1 || Tst2)
							SendData(28, NULL, 0); // донесение 011100 - "Нет КПИ (СИ не стерлась)"
						else            
							SendData(3, NULL, 0); // донесение 000011 - "Да КПИ1-В"
						goto FinTestMSG_KPI;
					}

					// Других команд КПИ1-В с Code=2 нет
					SendData(26, NULL, 0); // донесение 011010 - "Нет КПИ (недопустимая команда КПИ)"
					goto FinTestMSG_KPI;
				}

				// +++++ 3. Тестирование ВЧМ +++++
				if (Code == 3)
				{
					if ((Cmd > 0) && (Cmd < 5))
					{
						if (Cmd == 1) 
							SendData(13, NULL, 0); // Началось тестирование 1-го канала
						if (Cmd == 2) 
							SendData(22, NULL, 0); // Началось тестирование 2-го канала
						if (Cmd == 3) 
							SendData(29, NULL, 0); // Началось тестирование 3-го канала
						if (Cmd == 4) 
							SendData(30, NULL, 0); // Началось тестирование 4-го канала
						//cpuDelay(50000);
						//WDT_RESET;
					   // vhmNum=STATE_dscBCU_N_get+1;
						//if (Cmd == vhmNum) 
						//	Test = 1;
						goto FinTestMSG_KPI;
					}

					// Других команд КПИ1-В с Code=3 нет
					SendData(26, NULL, 0); // донесение 011010 - "Нет КПИ (недопустимая команда КПИ)"
					goto FinTestMSG_KPI;
				}

				// +++++ 4. Установка номера текущей синхронизации +++++
				if (Code == 4)
				{
					// Предварительная команда установки номера текущей синхронизации (FFFB0004)
					if (!Cmd)
					{
						FlagPrefKPI = 1;
						SendData(3, NULL, 0); // донесение 000011 - "Да КПИ1-В"
						goto FinTestMSG_KPI;
					}

					// Основная команда установки номера текущей синхронизации (FEFB0104..F7FB0804)
					if (Cmd)
					{
						// Если не было предварительной команды
						// установки номера текущей синхронизации
						if (FlagPrefKPI ^ 2)
						{
							SendData(24, NULL, 0); // донесение 011000 - "Нет КПИ (КПИ1-В до предварительной КПИ1-В)"
							goto FinTestMSG_KPI;
						}

						// Если номер СИ недопустим
						if (Cmd > 8)
						{
							SendData(26, NULL, 0); // донесение 011010 - "Нет КПИ (недопустимая команда КПИ)"
							goto FinTestMSG_KPI;
						}

						// Установка номера синхронизации для указанного номера СИ в 1
					//	CntSync.W32[Cmd - 1 + (Nkp << 3)] = 1;

					//	// Запись в ОЗУ МУ
					////	StopCP();
					//	ApiDataSave(0, &CntSync.W16[0]);
					//	ApiDataSave(1, &CntSync.W16[8]);
					//	ApiDataSave(2, &CntSync.W16[16]);
					//	ApiDataSave(3, &CntSync.W16[24]);
						// T_Prev=TIMER2;
						// WDT_RESET;
						//cpuDelay(50000);
						// WDT_RESET;
						//cpuDelay(50000);
						// WDT_RESET;
						//cpuDelay(50000);
						// WDT_RESET;
						SendData(3, NULL, 0); // Выдатся донесение 000011 - "Да КПИ1-В"
						goto FinTestMSG_KPI;
					}

					// Других команд КПИ1-В с Code=4 нет
					SendData(26, NULL, 0); // Выдатся донесение 011010 - "Нет КПИ (недопустимая команда КПИ)"
					goto FinTestMSG_KPI;
				}

				// +++++ 5. Установка Тк +++++
				if (Code == 5)
				{
					FlagTK = 0;
					if (!Cmd) 
					{ 
						FlagTK = 1; 
						FlagRescue = 0; 
					}
					if (Cmd == 1) 
					{ 
						FlagTK = 1; 
						FlagRescue = 1; 
						tk_hi = TK____6_HI; 
						tk_lo = TK____6_LO; }
					if (Cmd == 2) 
					{ 
						FlagTK = 1; 
						FlagRescue = 1; 
						tk_hi = TK___12_HI; 
						tk_lo = TK___12_LO; 
					}
					if (Cmd == 3) 
					{ 
						FlagTK = 1;
						FlagRescue = 1; 
						tk_hi = TK___18_HI; 
						tk_lo = TK___18_LO; 
					}
					if (Cmd == 4) 
					{ 
						FlagTK = 1; 
						FlagRescue = 1; 
						tk_hi = TK___24_HI; 
						tk_lo = TK___24_LO; 
					}
					if (Cmd == 5) 
					{ 
						FlagTK = 1; 
						FlagRescue = 1; 
						tk_hi = TK___30_HI; 
						tk_lo = TK___30_LO; 
					}
					if (Cmd == 6) 
					{ 
						FlagTK = 1; 
						FlagRescue = 1;
						tk_hi = TK___36_HI; 
						tk_lo = TK___36_LO; 
					}
					if (Cmd == 10) 
					{ 
						FlagTK = 1; 
						FlagRescue = 1;
						tk_hi = TK___60_HI;
						tk_lo = TK___60_LO; 
					}
					if (Cmd == 20) 
					{ 
						FlagTK = 1;
						FlagRescue = 1;
						tk_hi = TK__120_HI;
						tk_lo = TK__120_LO; 
					}
					if (Cmd == 30) 
					{ 
						FlagTK = 1; 
						FlagRescue = 1; 
						tk_hi = TK__180_HI; 
						tk_lo = TK__180_LO; 
					}
					if (Cmd == 60) 
					{ 
						FlagTK = 1;
						FlagRescue = 1;
						tk_hi = TK__360_HI;
						tk_lo = TK__360_LO;
					}
					if (Cmd == 120) 
					{ 
						FlagTK = 1; 
						FlagRescue = 1; 
						tk_hi = TK__720_HI; 
						tk_lo = TK__720_LO;
					}
					if (Cmd == 240) 
					{
						FlagTK = 1; 
						FlagRescue = 1;
						tk_hi = TK_1440_HI;
						tk_lo = TK_1440_LO; 
					}
					if (FlagTK) 
						SendData(3, NULL, 0); // Выдатся донесение 000011 - "Да КПИ1-В"
					else        
						SendData(26, NULL, 0); // Выдатся донесение 011010 - "Нет КПИ (недопустимая команда КПИ)"
					goto FinTestMSG_KPI;
				}

				// +++++ 6. Завершение синхронизации +++++
				if (Code == 6)
				{
					// Завершение синхронизации (FFF90006)
					if (Cmd == 0)
					{
						for (I = 0; I < 64; I++) RGK[I] = 0; // Стирается КОИ
						for (I = 0; I < 36; I++) RGI[I] = 0; //
						FlagSync = 0;                   // Сбрасывается синхронизация
						SendData(3, NULL, 0);           // Выдатся донесение 000011 - "Да КПИ1-В"

						//if (FlagTU ^ 1)
						//{
						//	for (I = 0; I < 10; I++)           // ~ 0.5 c
						//	{
						//		//cpuDelay(50000);            // пауза 50 мс
						//	   // WDT_RESET;
						//	}
						//	//StopCP();
						//}
						//else 
						//	DbgPause();

						SendData(20, NULL, 0);         // Выдатся донесение 010100 - "Разрешение реконфигурации"
						goto FinTestMSG_KPI;
					}

					// Других команд КПИ1-В с Code=6 нет
					SendData(26, NULL, 0); // донесение 011010 - "Нет КПИ (недопустимая команда КПИ)"
					goto FinTestMSG_KPI;
				}

				// +++++ 7. Отмена выполнения работ по циклограмме +++++
				if (Code == 7)
				{
					// Отмена выполнения работ по циклограмме
					if (Cmd == 0)
					{
						if (!FlagExchCBK)
						{
							ExitCode = Error;
							FlagExitCyclo = 1;
						}
						else 
							FlagExitCyclo = 0;
						SendData(3, NULL, 0); // Выдатся донесение 000011 - "Да КПИ1-В"
						goto FinTestMSG_KPI;
					}

					// Других команд КПИ1-В с Code=7 нет
					SendData(26, NULL, 0); // Выдатся донесение 011010 - "Нет КПИ (недопустимая команда КПИ)"
					goto FinTestMSG_KPI;
				}

				// Команд КПИ1-В с другими значениями Code нет
				SendData(26, NULL, 0); // Выдатся донесение 011010 - "Нет КПИ (недопустимая команда КПИ)"
				goto FinTestMSG_KPI;
			}

			// ##### 3. РКН-1 #####
			if (Wid_KPI == RKN1)
			{
				p32 = (UINT32*)&CMD[0];
				Adr = (UINT16)((*p32 >> 24) & 0x1f);
				SubAdr = (UINT16)((*p32 >> 19) & 0x1f);

				// Проверка на допустимость адреса и подадреса
				if ((Adr ^ 2) || ((SubAdr ^ 2) && (SubAdr ^ 3) && (SubAdr ^ 29)))
				{
					SendData(26, NULL, 0); // Выдатся донесение 011010 - "Нет КПИ (недопустимая команда КПИ)"
					goto FinTestMSG_KPI;
				}

				// Проверка пройдена, выдача РКН-1
				KPI_16 = (unsigned int)((*p32 >> 3) & 0xffff);
				//ApiCommand(SubAdr, &KPI_16, 1);
				//if (FlagTU == 1) 
				//	DbgPause();
				SendData(21, NULL, 0); // Выдатся донесение 010101 - "Да РКН1"
				goto FinTestMSG_KPI;
			}

			// ##### 4. РКН-2 #####
			if (Wid_KPI == RKN2)
			{
				p32 = (UINT32*)&CMD[0];
				Adr = (UINT16)((*p32 >> 24) & 0x1f);
				SubAdr = (UINT16)((*p32 >> 19) & 0x1f);
				// Проверка на допустимость адреса и подадреса
				if ((Adr ^ 4) || ((SubAdr ^ 17) && (SubAdr ^ 28) && (SubAdr ^ 29)))
				{
					SendData(26, NULL, 0); // донесение 011010 - "Нет КПИ (недопустимая команда КПИ)"
					goto FinTestMSG_KPI;
				}

				// Проверка пройдена, выдача РКН-2
				KPI_16 = (UINT16)((*p32 >> 3) & 0xffff);
				//ApiCommandSerial(SubAdr, &KPI_16, 1);
				SendData(25, NULL, 0); // донесение 011001 - "Да РКН2"
				goto FinTestMSG_KPI;
			}

			// +++++ 5. КПИ2-ИС +++++
			if (Wid_KPI == KPI2_IS)
			{
				SendData(15, CMD, 2);     // донесение 001111 - "Да КПИ2-ИС"
				goto FinTestMSG_KPI;
			}

			// +++++ 6. КПИ2-КС +++++
			if (Wid_KPI == KPI2_KS)
			{
				SendData(4, CMD, 2);      // донесение 000100 - "Да КПИ2-КС"
				goto FinTestMSG_KPI;
			}
		}

		// ШТАТНЫЙ ВЫХОД
	FinTestMSG_KPI:

		//if (Test)
		//{
		//	// Инициализация МКО
		//	//mkoSetAddrIn(inAdr);
		//	//intConnect(VEC_QC5, QC5_handler);
		//	//bsvWrite(RLen,3);             // РЧВС
		//	//bsvBitSet(RF1,BSV_RF1_RdyIn); // РПИ
		//	//bsvWrite(RMIn,0xDF);	
		//	//bsvBitSet(RF1,BSV_RF1_Er);    // Выставляется признак ОНИ
		//	//WDT_RESET;
		//	//pakSPO(0xF, 0, 0);
		//}

		if (FlagPrefKPI) FlagPrefKPI++;
		if (FlagPrefKPI == 3) FlagPrefKPI = 0;
		MSG_KPI_index = 0;
		tim = 0;
		//enable_interrupt();
		//INT_MASK|=IMASK_EXT;   // Разрешение ЗПР1
		//INT_MASK1|=IMASK1_EXT; // Разрешение ЗПР2
		//WDT_RESET;

		//cpuDelay(50000); // пауза 100 мс
		//WDT_RESET;       //
		//cpuDelay(50000); //
	   // WDT_RESET;       //

		return ExitCode;
	}

	void SendData
	(
		unsigned int Events,  // карта битов признаков ПУПС5-ПУПС0
		unsigned int* Buffer, // указатель на буфер выдаваемых данных
		unsigned int Words    // количество выдаваемых слов данных: 0 - 32,
									 // если количество выдаваемых слов данных равно 0,
									 // выдаются только признаки ПУПС, без СД
	)
	{
		//// ===== ТОЛЬКО В ТЕХНОЛОГИЧЕСКОМ СПО =====
		////vhmNum=STATE_dscBCU_N_get;
		//if ((vhmNum == 0) && (!(MASK & 1))) return;
		//if ((vhmNum == 1) && (!(MASK & 2))) return;
		//if ((vhmNum == 2) && (!(MASK & 4))) return;
		//if ((vhmNum == 3) && (!(MASK & 8))) return;
		//// ===== End of ТОЛЬКО В ТЕХНОЛОГИЧЕСКОМ СПО =====

		//DBG = 0;
		
		// обнуление счетчиков сбоев
		//buf[0] = 0x050F;
		// ApiCommand(SA_SERV,&buf[0],1);

		 // выдача ПУПС и данных
		//ApiDataSend(Events, &Buffer[0], Words);

		// корректировка таймера
	   // T_Prev=TIMER2;

		//DBG = 1;
		PUPS = Events;
		mko_Buffer = Buffer;
		msg_count = Words;
	}



