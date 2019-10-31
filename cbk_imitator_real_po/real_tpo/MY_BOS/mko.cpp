#ifdef __cplusplus
extern "C" {
#endif
#include "bos.h"
#ifdef __cplusplus
}
#endif
#include "mko_module.h"
#include <QVariantList>
#include "windows.h"

union MKOWord
{
	quint16 com_word;				 // командное слово целиком
	struct
	{
		quint16 count_word : 5,    // число сл.данных / команда
			subadr : 5,		   // подадрес
			trans_dir : 1,     // направление передачи(1-чт.ќ”)
			adr : 5;
	};
};

/*------------------------------------------------------------------------------
* Объявления функций
*/
/*	инициализация МКО	для работы в режиме контроллера.
*/
STATUS
mkoBcInit(
	UINT numMKO,    /* номер МКО */
	UINT numChan    /* =1-3 - номер канала резервирования передатчика,
					который нужно включить
					=0   - выключить передатчики всех каналов
					резервирования МКО */
){return 0;}

/* Запуск передачи сообщения */
STATUS
mkoSend(
	UINT     numMKO, /*- номер МКО */
	mkoMSG_t *mkoMSG /*- указатель на описатель передаваемого сообщения*/
){return 0;}


STATUS
mkoWait(
	UINT numMKO,	/*- номер МКО*/
	mkoMSG_t *mkoMSG /*- указатель на описатель сообщения*/
){return 0;}

/*	Инициализация пакета
Устанавливает в исходное состояние соответствующий данному пакету
фрагмент стека описателей в БОЗУ.

ВОЗВРАТ: OK    - норма
ERROR - ошибка: неверные номера первого и/или последнего сообщений
или неверно задан номер МКО
*/
STATUS
mkoPackInit(
	UINT numMKO,   /* номер МКО: 0 или 1 */
	mkoPack_t *DescPack /*указатель на заполненный описатель пакета*/
){return 0;}

/*	Включение сообщения в пакет
*/
STATUS
mkoPackMsgAdd(
	mkoPackMsg_t *DescMsg /*указатель на заполненный описатель сообщения*/
){return 0;}

/*	Включение нескольких сообщений в пакет
*/
STATUS
mkoPackMsgArrAdd(
	mkoPackMsg_t* DescMsgArray[], /*адрес массива указателей на заполненные
								  описатели сообщений*/
	int LenArray      /* размер массива */
){return 0;}


/* Запуск пакета сообщений */
STATUS
mkoPackStart(
	mkoPack_t *DescPack /*указатель на описатель пакета*/
){return 0;}

/* Останов пакета */
STATUS
mkoPackStop(
	mkoPack_t *DescPack /*указатель на заполненный описатель пакета*/
){return 0;}


STATUS
mkoPackWait(
	mkoPack_t *DescPack /*указатель на заполненный описатель пакета*/
){return 0;}

STATUS
mkoPackMsgWait(
	mkoPackMsg_t *DescMsg /*указатель на заполненный описатель сообщения*/
){return 0;}

STATUS
mkoOFF(
	UINT numMKO  /*- номер МКО*/
){return 0;}

STATUS
mkoSetChan(
	UINT numMKO,    /* - номер МКО */
	UINT numChan){return 0;}  /* =1-3 - номер канала резервирования передатчика,
					который нужно включить
					=0   - выключить передатчики всех каналов
					резервирования МКО */
UINT
mkoGetChan(
	UINT numMKO){return 0;}  /* - номер МКО */

#ifdef MKO_PackDeFix
STATUS
mkoSendPri(
	UINT      numMKO,  /* номер МКО */
	mkoMSG_t *mkoMSG,  /* указатель на описатель передаваемого сообщения*/
	UINT      priority /* приоритет сообщения в очереди на передачу  @}@*/
){return 0;}

STATUS
mkoPackIniStart(
	UINT          numMKO,         /* номер МКО: 0 или 1 */
	mkoPack_t    *DescPack,       /* указатель на описатель пакета*/
	mkoPackMsg_t *DescMsgArray[], /* адрес массива указателей на заполненные
								  описатели сообщений, включаемых в пакет */
	UINT          priority        /* приоритет пакета в очереди на передачу */

)
{
	int mko, line, cwd, os, trans_dir;
	int msg_count = sizeof(DescMsgArray) / sizeof(DescMsgArray[0]);
	
	// Расчет времени
	LARGE_INTEGER time_n, time_s, freq, time_prev;

	QueryPerformanceCounter(&time_s);
	QueryPerformanceFrequency(&freq);
	time_prev.QuadPart = 0;
	for (int i = 0; i < msg_count; i++)
	{
		int tmp_trans_dir = DescMsgArray[i]->TM;
		switch (tmp_trans_dir)
		{
		case 1:
			trans_dir = 0;
			break;
		case 2:
			trans_dir = 1;
			break;
		}
		mko = numMKO;
		line = DescMsgArray[i]->Chan;
		cwd = DescMsgArray[i]->CW;
		MKOWord CMWord;
		CMWord.com_word = cwd;
		os = 0;
		QVariantList words;
		int word_count = CMWord.count_word;
		if (word_count == 0)
			word_count = 32;
		if (trans_dir == 0)
		{
			for (int j = 0; j < word_count; j++)
			{
				words << DescMsgArray[i]->adrData[j];
			}
		}
		else if (trans_dir == 1)
		{
			for (int j = 0; j < word_count; j++)
			{
				words << 0;
			}
		}
		QueryPerformanceCounter(&time_n);
		SMkoImitObject::Instance().slot_send_msg_mko(mko, line, cwd, words, os);
		
		if (os == -1)
		{
			DescMsgArray[i]->DescMsgRes->SW = os;
			DescMsgArray[i]->DescMsgRes->SW2 = 0;
			if (trans_dir == 1)
			{
				DescMsgArray[i]->DescMsgRes->Len = words.size();
				for (int j = 0; j < words.size(); j++)
				{
					DescMsgArray[i]->adrData[j] = words[j].toInt();
				}
			}
			else if (trans_dir == 0)
			{
				DescMsgArray[i]->DescMsgRes->Len =0;
			}
			DescMsgArray[i]->DescMsgRes->Chan = line;
			DescMsgArray[i]->DescMsgRes->Som = 0;
			DescMsgArray[i]->DescMsgRes->Eom = 1;
			DescMsgArray[i]->DescMsgRes->MissSW = 1;
			DescMsgArray[i]->DescMsgRes->FlagSW = 0;
			DescMsgArray[i]->DescMsgRes->FmtError = 0;
			DescMsgArray[i]->DescMsgRes->TimeTag = (int)(((float)time_n.QuadPart / (float)freq.QuadPart) - ((float)time_s.QuadPart / (float)freq.QuadPart)) * 1000000;
			DescPack->DescPackRes->Eof = 1;
			DescPack->DescPackRes->MissSW = 1;
			DescPack->DescPackRes->nMsg = i;
			return 0;
		}
		else
		{
			DescMsgArray[i]->DescMsgRes->SW = os;
			DescMsgArray[i]->DescMsgRes->SW2 = 0;
			if (trans_dir == 1)
			{
				DescMsgArray[i]->DescMsgRes->Len = words.size();
				for (int j = 0; j < words.size(); j++)
				{
					DescMsgArray[i]->adrData[j] = words[j].toInt();
				}
			}
			else if (trans_dir == 0)
			{
				DescMsgArray[i]->DescMsgRes->Len = 0;
			}
			DescMsgArray[i]->DescMsgRes->Chan = line;
			DescMsgArray[i]->DescMsgRes->Som = 0;
			DescMsgArray[i]->DescMsgRes->Eom = 1;
			DescMsgArray[i]->DescMsgRes->MissSW = 0;
			DescMsgArray[i]->DescMsgRes->FlagSW = 0;
			DescMsgArray[i]->DescMsgRes->FmtError = 0;
			DescMsgArray[i]->DescMsgRes->TimeTag = (int)(((float)time_n.QuadPart / (float)freq.QuadPart) - ((float)time_s.QuadPart / (float)freq.QuadPart)) * 1000000;
			DescPack->DescPackRes->Eof = 1;
			DescPack->DescPackRes->MissSW = 0;
			DescPack->DescPackRes->nMsg = i;
			DescMsgArray[i]->GapTime = (int)(((float)time_n.QuadPart / (float)freq.QuadPart) - time_prev.QuadPart)* 1000000;
			time_prev = time_n;
		}
	}

	return 0;
}

STATUS
mkoNumPackStop(
	UINT       numMKO,  /* номер МКО: 0 или 1 */
	mkoPack_t *DescPack /*указатель на заполненный описатель пакета@}@*/
){return 0;}

STATUS
mkoNumPackMsgWait(
	UINT          numMKO, /* номер МКО: 0 или 1 */
	mkoPackMsg_t *DescMsg /*указатель на заполненный описатель сообщения@}@*/
){return 0;}

STATUS
mkoNumPackWait(
	UINT       numMKO,  /* номер МКО: 0 или 1 */
	mkoPack_t *DescPack /*указатель на заполненный описатель пакета@}@*/
){return 0;}

#endif /* MKO_PackDeFix */

extern UINT  ErrCodGet(void){ return 0; }
