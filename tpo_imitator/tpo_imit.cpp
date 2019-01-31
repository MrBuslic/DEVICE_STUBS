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
	QObject::connect(this, SIGNAL(signal_send_receipt(int, int&)), this, SLOT(send_receipt(int, int&)));
	QObject::connect(this, SIGNAL(signal_send_read_receipt(int, int, int, int&)), this, SLOT(send_read_receipt(int, int, int, int&)));
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
	MKORETWord ret_word;
	bool rep = true;
	int com_code;

	ret_word.ret_word = words[0].toInt(0);
	com_code = ret_word.com_word;

	for (int i = 0; i++; i < words.size())
	{
		str = QString("Get word %1 - 0x%2").arg(i + 1).arg(words[i].toInt(), 4, 16, QChar('0'));
		edit->append(str);
	}
	switch (com_code)
	{
	case 0x03:
		write_data(words);
		break;
	case 0x04:
		read_data(words);
		break;
	default:
		unknown_com();
		break;
	}
	str = QString("Processing of the received request is completed.");
	edit->append(str);
}

int TPO_Widget::count_crc_for_cwd( QVariantList& words)
{
	int count_words;
	int calculated_CRC;
	unsigned short *massiv_slov; 
	count_words = 6;
	massiv_slov = new unsigned short[count_words];
	for (int i = 0; i < count_words; i++)
	{
		massiv_slov[i] = words[i].toInt();
	}
	calculated_CRC = CalcCrc16(massiv_slov, 6 * sizeof(massiv_slov[0]));	
	delete[] massiv_slov;
	return calculated_CRC;
}

int TPO_Widget::count_crc_for_words(QVariantList& words)
{
	int count_words;
	int calculated_CRC;
	unsigned short *massiv_slov;
	count_words = words.size();
	massiv_slov = new unsigned short[count_words];
	for (int i = 0; i < count_words; i++)
	{
		massiv_slov[i] = words[i].toInt();
	}
	calculated_CRC = CalcCrc16(massiv_slov, count_words * sizeof(massiv_slov[0]));
	delete[] massiv_slov;
	return calculated_CRC;
}

int TPO_Widget::count_byte_for_words(QVariantList& words)
{
	int count_words;
	int calculated_count = 0;
	unsigned short *massiv_slov;
	count_words = words.size();
	massiv_slov = new unsigned short[count_words];
	for (int i = 0; i < count_words; i++)
	{
		massiv_slov[i] = words[i].toInt();
		calculated_count = calculated_count + sizeof(massiv_slov[i]);
	}
	delete[] massiv_slov;
	return calculated_count;
}

void TPO_Widget::send_receipt(int answer, int& os)
{
	MKOCOMWord cwd;
	cwd.adr = 1;
	cwd.subadr = 16;
	cwd.count_word = 1;
	cwd.trans_dir = 0;
	QVariantList words;
	words << answer;
	emit signal_send_new_msg(1, 1, cwd.com_word, words, os);
}

void TPO_Widget::send_read_receipt(int answer, int byte_count, int crc_for_words, int& os)
{
	MKOCOMWord cwd;
	cwd.adr = 1;
	cwd.subadr = 16;
	cwd.count_word = 3;
	cwd.trans_dir = 0;
	QVariantList words;
	words << answer;
	words << byte_count;
	words << crc_for_words;
	emit signal_send_new_msg(1, 1, cwd.com_word, words, os);
}

void TPO_Widget::write_data(QVariantList& words)
{
	MKORETWord ret_word;
	MKOCOMWord cwd;
	QVariantList trans_words;
	QString str;
	int adr = 1;
	int subadr = 1;
	int mko, line, count_words, count_words_for_transmit, answer;
	int crc16_for_cwd, crc16_for_words, counted_crc16_for_cwd, counted_crc16_for_words;
	int os = 1;
	ret_word.ret_word = words[0].toInt();
	count_words_for_transmit = ret_word.count_word;

	ret_word.ret_word = words[1].toInt();
	adr = ret_word.adr;
	subadr = ret_word.subadr;
	mko = ret_word.channel;
	line = ret_word.line;
	crc16_for_cwd = words[6].toInt();//Контрольная сумма для командного слова
	
	counted_crc16_for_cwd = count_crc_for_cwd(words);
	if (counted_crc16_for_cwd == crc16_for_cwd)
	{
		cwd.adr = adr;
		cwd.subadr = subadr;
		cwd.count_word = count_words_for_transmit;
		cwd.trans_dir = 0;

		for (int i = 0; i++; i < count_words_for_transmit)
		{
			trans_words << 0;
		}
		
		emit signal_send_message(1, 2, 1, count_words_for_transmit, trans_words);
		crc16_for_words = words[7].toInt();
		counted_crc16_for_words = count_crc_for_words(trans_words);
		if (counted_crc16_for_words == crc16_for_words)
		{
			emit signal_send_new_msg(mko, line, cwd.com_word, trans_words, os);
			str = QString("Transmit command using MKO -%1 and line - %2 CW - 0x%3").arg(mko).arg(line).arg(cwd.com_word, 4, 16, QChar('0'));
			edit->append(str);
			str = QString("Write %1 words to abonent").arg(trans_words.size());
			edit->append(str);
			for (int i = 0; i < trans_words.size(); i++)
			{
				str = QString("Word %1 - 0x%2").arg(i + 1).arg(trans_words[i].toInt(), 4, 16, QChar('0'));
				edit->append(str);
			}
			answer = 0x0019;
			emit signal_send_receipt(answer, os);
			if (os == -1)
			{
				str = QString("Get response word with code - 0x%1").arg(os, 4, 16, QChar('0'));
				edit->append(str);
			}
			else
			{
				str = QString("Everything is all right! The exchange is completed!");
				edit->append(str);
			}
		}
		else
		{
			str = QString("Error with CRC of transmitted words");
			edit->append(str);
			answer = 0x8402;
			emit signal_send_receipt(answer, os);
			if (os == -1)
			{
				str = QString("Get response word with code - 0x%1").arg(os, 4, 16, QChar('0'));
				edit->append(str);
			}
			else
			{
				str = QString("The exchange attempt is completed!");
				edit->append(str);
			}
		}

	}
	else
	{
		answer = 0x8401;
		emit signal_send_receipt(answer, os);
		if (os == -1)
		{
			str = QString("Get response word with code - 0x%1").arg(os, 4, 16, QChar('0'));
			edit->append(str);
		}
		else
		{
			str = QString("The exchange attempt is completed!");
			edit->append(str);
		}
	}
}

void TPO_Widget::read_data(QVariantList& words)
{
	MKORETWord ret_word;
	MKOCOMWord cwd;
	QVariantList trans_words;
	QString str;
	int adr = 1;
	int subadr = 1;
	int mko, line, count_words, count_words_for_transmit, answer;
	int crc16_for_cwd, crc16_for_words, counted_crc16_for_cwd, counted_crc16_for_words, byte_count;
	int os = 1;
	ret_word.ret_word = words[0].toInt();
	count_words_for_transmit = ret_word.count_word;

	ret_word.ret_word = words[1].toInt();
	adr = ret_word.adr;
	subadr = ret_word.subadr;
	mko = ret_word.channel;
	line = ret_word.line;
	crc16_for_cwd = words[6].toInt();//Контрольная сумма для командного слова

	counted_crc16_for_cwd = count_crc_for_cwd(words);
	if (counted_crc16_for_cwd == crc16_for_cwd)
	{
		cwd.adr = adr;
		cwd.subadr = subadr;
		cwd.count_word = count_words_for_transmit;
		cwd.trans_dir = 1;

		for (int i = 0; i++; i < count_words_for_transmit)
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
		//Отправляем полученные данные на 1 адрес 17 подадрес
		cwd.adr = 1;
		cwd.subadr = 17;
		cwd.count_word = count_words_for_transmit;
		cwd.trans_dir = 0;
		emit signal_send_new_msg(1, 1, cwd.com_word, trans_words, os);
		if (os == -1)
		{
			str = QString("Get response word with code - 0x%1").arg(os, 4, 16, QChar('0'));
			edit->append(str);
		}
		else
		{
			str = QString("Everything is all right! Transmitted received words to 17 subadr is completed!");
			edit->append(str);
		}
		// Считаем контрольную сумму для полученных данных и количество байт
		counted_crc16_for_words = count_crc_for_words(trans_words);
		byte_count = count_byte_for_words(trans_words);
		answer = 0x0019;
		emit signal_send_read_receipt(answer, byte_count, counted_crc16_for_words, os);
		if (os == -1)
		{
			str = QString("Get response word with code - 0x%1").arg(os, 4, 16, QChar('0'));
			edit->append(str);
		}
		else
		{
			str = QString("Everything is all right! The exchange is completed!");
			edit->append(str);
		}
	}
	else
	{
		str = QString("Error with CRC of command word");
		edit->append(str);
		answer = 0x8401;
		emit signal_send_receipt(answer, os);
		if (os == -1)
		{
			str = QString("Get response word with code - 0x%1").arg(os, 4, 16, QChar('0'));
			edit->append(str);
		}
		else
		{
			str = QString("The exchange attempt is completed!");
			edit->append(str);
		}
	}
}

void TPO_Widget::unknown_com()
{
	int answer = 0x8017;
	int os = 1;
	QString str;
	str = QString("Receive unknown command! Send negative receipt!");
	edit->append(str);
	emit signal_send_receipt(answer, os);
	if (os == -1)
	{
		str = QString("Get response word with code - 0x%1").arg(os, 4, 16, QChar('0'));
		edit->append(str);
	}
	else
	{
		str = QString("The exchange attempt is completed!");
		edit->append(str);
	}
}
