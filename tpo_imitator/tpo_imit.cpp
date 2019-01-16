#include "tpo_imit.h"
#include <QtWidgets>

union MKOCOMWord
{
	quint16 com_word;			    // командное слово целиком
	struct
	{
		quint16 count_word : 5,		// число сл.данных / команда
				subadr : 5,			// подадрес
				trans_dir : 1,		// направление передачи(1-чт.ќ”)
				adr : 5;			// адрес
	};
};

union MKORETWord
{
	quint16 ret_word;
	struct
	{
		quint16 com_word : 8,		// число слов данных    
			    count_word : 8;		// код команды
	};
	struct
	{
		quint16	adr : 5,		// адрес абонента МКО
				subadr: 5,		// подадрес абонента МКО
				line : 1,		// линия МКО
				channel : 1,	// канал МКО
				trash : 4;		// не знаю для чего эти биты
	};
};

TPO_Widget::TPO_Widget() :QWidget()
{
	QVBoxLayout* v_lay = new QVBoxLayout(this);
	edit = new QTextEdit(this);
	write_button = new QPushButton("Write_data");
	read_button = new QPushButton("Read_data");
	setMinimumSize(490, 300);
	v_lay->addWidget(edit);
	v_lay->addWidget(write_button);
	v_lay->addWidget(read_button);

	mko_imit = new MKO_Widget();
	inter_imit = new SimInterruptWid();
	
	inter_imit->create_signal_thread();
	inter_imit->create_slot_thread();
	
	mko_imit->create_slot_thread();

	qRegisterMetaType<QVariantList>("QVariantList&");
	qRegisterMetaType<int>("int&");

	QObject::connect(inter_imit->get_rpc_foi_signal_thread()->get_obj().get(), SIGNAL(foi_interrupt(int, short, double, double)), inter_imit, SLOT(get_inter_from_server(int, short, double, double)), Qt::QueuedConnection);
	QObject::connect(inter_imit, SIGNAL(signal_send_request()), this, SLOT(send_request()));
	QObject::connect(mko_imit, SIGNAL(signal_send_new_msg(int, int, int, QVariantList&, int&)), mko_imit->get_mko_slot_thread()->get_obj().get(), SLOT(send_msg(int, int, int, QVariantList&, int&)), Qt::DirectConnection);
	QObject::connect(this, SIGNAL(signal_send_message(int, int, int, int, QVariantList&)), mko_imit, SLOT(send_message(int, int, int, int, QVariantList&)), Qt::DirectConnection);
	QObject::connect(this, SIGNAL(signal_process_msg(QVariantList&)), this, SLOT(slot_process_msg(QVariantList&)));
	QObject::connect(this, SIGNAL(signal_send_new_msg(int, int, int, QVariantList&, int&)), mko_imit, SLOT(slot_send_new_msg(int, int, int, QVariantList&, int&)), Qt::DirectConnection);
	QObject::connect(this, SIGNAL(signal_send_receipt(int, QVariantList&)), this, SLOT(send_receipt(int, QVariantList&)));
}

void TPO_Widget::send_request()
{
	QString str;
	str = QString("Get interrrupt from server! Need to send request to addres 1 with subaddres 1!");
	edit->setText(str);
	int adr = 1;
	int subadr = 1;
	int count_words = 8;
	int direct = 1;
	QVariantList words;
	for (int i = 0; i < 8; i++)
	{
		words<< 0;
	}
	emit signal_send_message(adr, subadr, direct, count_words, words);
	emit(signal_process_msg(words));
	return;
}

void TPO_Widget::slot_process_msg(QVariantList& words)
{
	QString str;
	QVariantList trans_words, ans_word;
	MKORETWord ret_word;
	MKOCOMWord cwd;
	QString reply;
	bool rep = true;
	int answer;
	int count_words, count_words_for_transmit;

	int adr = 1;
	int subadr = 1;
	int mko, line;
	int direct = 1;
	int os = 1;
	int crc16_for_cwd, crc16_for_words;
	bool check_cwd_crc, check_words_crc;
	
	//Проверяем команду: чтение или запись и количество слов
	count_words = words.size();
	ret_word.ret_word = words[0].toInt();
	if (ret_word.com_word == 3)
	{
		direct = 0;
	}
	else if (ret_word.com_word == 4)
	{
		direct = 1;
	}
	count_words_for_transmit = ret_word.count_word;
	//Параметры для передачи: адрес абонента, подадрес, линия и канал МКО
	ret_word.ret_word = words[1].toInt();
	adr = ret_word.adr;
	subadr = ret_word.subadr;
	mko = ret_word.channel;
	line = ret_word.line;
	//Контрольная сумма для командного слова
	crc16_for_cwd = words[6].toInt();
	//Вывод полученных командных слов
	for (int i = 0; i < count_words; i++)
	{
		str = QString("Get word %1 - CW 0x%3").arg(i + 1).arg(words[i].toInt(), 4, 16, QChar('0'));
		edit->append(str);
	}
	check_cwd_crc = count_crc(true, crc16_for_cwd, words);
	if (check_cwd_crc)
	{
		//Формирование командного слова
		cwd.adr = adr;
		cwd.subadr = subadr;
		cwd.count_word = count_words_for_transmit;
		cwd.trans_dir = direct;
		// Получение массива слов для передачи или для получения
		if (direct == 0)
		{
			crc16_for_words = words[7].toInt();
			for (int i = 0; i < count_words_for_transmit; i++)
			{
				trans_words << 0;
			}
			emit signal_send_message(1, 2, 1, count_words_for_transmit, trans_words);
			check_words_crc = count_crc(false, crc16_for_words, trans_words);
			if (check_words_crc)
			{
				emit signal_send_new_msg(mko, line, cwd.com_word, trans_words, os);
				str = QString("Transmit command using MKO - %1 and line - %2 CW - 0x%3").arg(mko).arg(line).arg(cwd.com_word, 4, 16, QChar('0'));
				edit->append(str);
				str = QString("Write %1 words to abonent").arg(trans_words.size());
				edit->append(str);
				for (int i = 0; i < trans_words.size(); i++)
				{
					str = QString("Word %1 - 0x%2").arg(i + 1).arg(trans_words[i].toInt(), 4, 16, QChar('0'));
					edit->append(str);
				}
			}
			else
			{
				str = QString("Error with CRC of transmitted words");
				edit->append(str);
				reply = "8402";
				answer = reply.toInt(&rep, 16);
				emit signal_send_receipt(answer, ans_word);
			}
			reply = "0019";
			answer = reply.toInt(&rep, 16);
			emit signal_send_receipt(answer, ans_word);
		}
		else
		{
			for (int i = 0; i < count_words_for_transmit; i++)
			{
				trans_words << 0;
			}
			emit signal_send_new_msg(mko, line, cwd.com_word, trans_words, os);
			str = QString("Transmit command using MKO - %1 and line - %2 CW - 0x%3").arg(mko).arg(line).arg(cwd.com_word, 4, 16, QChar('0'));
			edit->append(str);
			str = QString("Read %1 words from abonent").arg(trans_words.size());
			edit->append(str);
			for (int i = 0; i < trans_words.size(); i++)
			{
				str = QString("Word %1 - 0x%2").arg(i + 1).arg(trans_words[i].toInt(), 4, 16, QChar('0'));
				edit->append(str);
			}
			reply = "0019";
			answer = reply.toInt(&rep, 16);
			emit signal_send_receipt(answer, ans_word);
		}
	}
	else
	{
		str = QString("Error with CRC of command word");
		edit->append(str);
		reply = "8401";
		answer = reply.toInt(&rep, 16);
		emit signal_send_receipt(answer, ans_word);
	}
}

bool TPO_Widget::count_crc(bool flag, int CRC, QVariantList& words)
{
	bool check;
	int count_words;
	int calculated_CRC;

	unsigned short *massiv_slov; 
	if (flag)
	{
		count_words = 6;
		massiv_slov = new unsigned short[count_words];
		for (int i = 0; i < count_words; i++)
		{
			massiv_slov[i] = words[i].toInt();
		}
		calculated_CRC = CalcCrc16(massiv_slov, 6 * sizeof(massiv_slov[0]));
		if (calculated_CRC == CRC)
		{
			check = true;
		}
		else
		{
			check = false;
		}
	}
	else
	{
		count_words = words.size();
		massiv_slov = new unsigned short[count_words];
		for (int i = 0; i < count_words; i++)
		{
			massiv_slov[i] = words[i].toInt();
		}
		calculated_CRC = CalcCrc16(massiv_slov, count_words*sizeof(massiv_slov[0]));
		if (calculated_CRC == CRC)
		{
			check = true;
		}
		else
		{
			check = false;
		}
	}
	delete[] massiv_slov;
	return check;
}

void TPO_Widget::send_receipt(int answer, QVariantList& ans_word)
{
	int adr = 1;
	int subadr = 16;
	int count_words = 1;
	int direct = 0;
	QVariantList words;
	words << answer;
	emit signal_send_message(adr, subadr, direct, count_words, words);
	ans_word = words;
}