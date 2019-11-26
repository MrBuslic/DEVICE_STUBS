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
			    channel : 1,    // линия МКО
				line : 1,       // канал МКО
				trash : 4;	 	// не знаю для чего эти биты
	};
};

enum eControlChan	// 
{
	controlOnCommon = 15,
	controlOnVM0 = 17,
	controlOnVM1 = 18,
	controlOnVM2 = 19,
	controlOnVM3 = 20,

	controlOffCommon = 16,
	controlOffVM0 = 21,
	controlOffVM1 = 22,
	controlOffVM2 = 23,
	controlOffVM3 = 24,

#ifdef OLD_CRATE
	controlHoldVM0 = 1,
	controlHoldVM1 = 2,
	controlHoldVM2 = 3,
	controlHoldVM3 = 4,
#else
	controlHoldVM0 = 21,
	controlHoldVM1 = 22,
	controlHoldVM2 = 23,
	controlHoldVM3 = 24,
#endif
};

TPO_Widget::TPO_Widget() :QWidget()
{
	QVBoxLayout* v_lay = new QVBoxLayout(this);
	
	QHBoxLayout* mds_lay = new QHBoxLayout(this);
	QVBoxLayout* mds32_line_lay = new QVBoxLayout(this);
	QVBoxLayout* mds32_label_lay = new QVBoxLayout(this);
	QLabel* mds_32_label_ip = new QLabel("mds32_server_ip");
	QLabel* mds_32_label_port = new QLabel("mds32_server_port");
	mds_32_edit_ip = new QLineEdit("127.0.0.1");
	mds_32_edit_port = new QLineEdit("70002");
	mds32_button = new QPushButton("Connect to mds32");
	mds32_line_lay->addWidget(mds_32_edit_ip);
	mds32_line_lay->addWidget(mds_32_edit_port);
	mds32_label_lay->addWidget(mds_32_label_ip);
	mds32_label_lay->addWidget(mds_32_label_port);
	mds_lay->addWidget(mds32_button);
	mds_lay->addLayout(mds32_label_lay);
	mds_lay->addLayout(mds32_line_lay);

	QHBoxLayout* mfsk24_lay = new QHBoxLayout(this);
	QVBoxLayout* mfsk24_line_lay = new QVBoxLayout(this);
	QVBoxLayout* mfsk24_label_lay = new QVBoxLayout(this);
	QLabel* mfsk24_label_ip = new QLabel("mfsk24_server_ip");
	QLabel* mfsk24_label_port = new QLabel("mfsk24_server_port");
	mfsk24_edit_ip = new QLineEdit("127.0.0.1");
	mfsk24_edit_port = new QLineEdit("70002");
	mfsk24_button = new QPushButton("Connect to mfsk24");
	mfsk24_line_lay->addWidget(mfsk24_edit_ip);
	mfsk24_line_lay->addWidget(mfsk24_edit_port);
	mfsk24_label_lay->addWidget(mfsk24_label_ip);
	mfsk24_label_lay->addWidget(mfsk24_label_port);
	mfsk24_lay->addWidget(mfsk24_button);
	mfsk24_lay->addLayout(mfsk24_label_lay);
	mfsk24_lay->addLayout(mfsk24_line_lay);

	QHBoxLayout* foi_lay = new QHBoxLayout(this);
	QVBoxLayout* foi_line_lay = new QVBoxLayout(this);
	QVBoxLayout* foi_label_lay = new QVBoxLayout(this);
	QLabel* foi_label_ip = new QLabel("foi_server_ip");
	QLabel* foi_label_signal_port = new QLabel("foi_signal_port");
	QLabel* foi_label_slot_port = new QLabel("foi_slot_port");
	foi_edit_ip = new QLineEdit("127.0.0.1");
	foi_signal_port = new QLineEdit("70002");
	foi_slot_port = new QLineEdit("70002");
	foi_button = new QPushButton("Connect to foi");
	foi_line_lay->addWidget(foi_edit_ip);
	foi_line_lay->addWidget(foi_signal_port);
	foi_line_lay->addWidget(foi_slot_port);
	foi_label_lay->addWidget(foi_label_ip);
	foi_label_lay->addWidget(foi_label_signal_port);
	foi_label_lay->addWidget(foi_label_slot_port);
	foi_lay->addWidget(foi_button);
	foi_lay->addLayout(foi_label_lay);
	foi_lay->addLayout(foi_line_lay);

	QHBoxLayout* omnibus_lay = new QHBoxLayout(this);
	QVBoxLayout* omnibus_line_lay = new QVBoxLayout(this);
	QVBoxLayout* omnibus_label_lay = new QVBoxLayout(this);
	QLabel* omnibus_label_ip = new QLabel("omnibus_server_ip");
	QLabel* omnibus_label_port = new QLabel("omnibus_server_port");
	omnibus_edit_ip = new QLineEdit("127.0.0.1");
	omnibus_edit_port = new QLineEdit("70002");
	omnibus_button = new QPushButton("Connect to omnibus");
	omnibus_line_lay->addWidget(omnibus_edit_ip);
	omnibus_line_lay->addWidget(omnibus_edit_port);
	omnibus_label_lay->addWidget(omnibus_label_ip);
	omnibus_label_lay->addWidget(omnibus_label_port);
	omnibus_lay->addWidget(omnibus_button);
	omnibus_lay->addLayout(omnibus_label_lay);
	omnibus_lay->addLayout(omnibus_line_lay);

	edit = new QTextEdit(this);
	
	setMinimumSize(490, 300);
	v_lay->addWidget(edit);
	v_lay->addLayout(mds_lay);
	v_lay->addLayout(mfsk24_lay);
	v_lay->addLayout(foi_lay);
	v_lay->addLayout(omnibus_lay);

	mko_imit = new MKO_Widget();
	inter_imit = new SimInterruptWid();
	mds32_imit = new mds32_exchange();
	mfsk24_imit = new mfsk24_exchange();
	
	inter_imit->create_signal_thread();
	inter_imit->create_slot_thread();
	
	mko_imit->create_slot_thread();

	qRegisterMetaType<QVariantList>("QVariantList&");
	qRegisterMetaType<int>("int&");
	qRegisterMetaType<uint>("uint&");

	vip_init();

	QObject::connect(inter_imit->get_rpc_foi_signal_thread()->get_obj().get(), SIGNAL(foi_interrupt(int, short, double, double)), inter_imit, SLOT(get_inter_from_server(int, short, double, double)), Qt::QueuedConnection);
	QObject::connect(inter_imit, SIGNAL(signal_send_request()), this, SLOT(send_request()));
	QObject::connect(mko_imit, SIGNAL(signal_send_new_msg(int, int, int, QVariantList&, int&)), mko_imit->get_mko_slot_thread()->get_obj().get(), SLOT(send_msg(int, int, int, QVariantList&, int&)), Qt::DirectConnection);
	QObject::connect(this, SIGNAL(signal_send_message(int, int, int, int, QVariantList&)), mko_imit, SLOT(send_message(int, int, int, int, QVariantList&)), Qt::DirectConnection);
	QObject::connect(this, SIGNAL(signal_process_msg(QVariantList&)), this, SLOT(slot_process_msg(QVariantList&)));
	QObject::connect(this, SIGNAL(signal_send_new_msg(int, int, int, QVariantList&, int&)), mko_imit, SLOT(slot_send_new_msg(int, int, int, QVariantList&, int&)), Qt::DirectConnection);
	QObject::connect(this, SIGNAL(signal_send_receipt(int, int&)), this, SLOT(send_receipt(int, int&)));
	QObject::connect(this, SIGNAL(signal_send_read_receipt(int, int, int, int&)), this, SLOT(send_read_receipt(int, int, int, int&)));
	QObject::connect(mds32_button, SIGNAL(clicked()), this, SLOT(slot_mds32_button()));
	QObject::connect(mfsk24_button, SIGNAL(clicked()), this, SLOT(slot_mfsk24_button()));
	//QObject::connect(foi_button, SIGNAL(clicked()), this, SLOT(slot_foi_button()));
	//QObject::connect(omnibus_button, SIGNAL(clicked()), this, SLOT(slot_omnibus_button()));

}

void TPO_Widget::vip_init()
{
	for (int i = 0; i < 5; i++)
	{
		pitanie[i] = true;
	}
}

void TPO_Widget::slot_mds32_button()
{
	QString ip_str = mds_32_edit_ip->text();
	int port = mds_32_edit_port->text().toInt();
	mds32_imit->create_signal_thread(ip_str, port);
	QObject::connect(mds32_imit->get_mds32_exchange_thread()->get_obj().get(), SIGNAL(mds32_get_sample(int, uint&, int&)), this, SLOT(mds32_send_sample(int, uint&, int&)), Qt::BlockingQueuedConnection);
}

void TPO_Widget::slot_mfsk24_button()
{
	QString ip_str = mfsk24_edit_ip->text();
	int port = mfsk24_edit_port->text().toInt();
	mfsk24_imit->create_signal_thread(ip_str, port);
	QObject::connect(mfsk24_imit->get_mds32_exchange_thread()->get_obj().get(), SIGNAL(mfsk24_impulse_change(QVariantList)), this, SLOT(slot_mfsk24_impulse_change(QVariantList)), Qt::BlockingQueuedConnection);
}

void TPO_Widget::mds32_send_sample(int channel, uint& buf, int& flag)
{
	switch (channel)
	{
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	{
		flag = 1;
		if (pitanie[channel - 1] == false)
		{
			buf = 1;
		}
		else
		{
			buf = 0;
		}
		break;
	}
	
	default:
		edit->append("The specified channel is not for us!");
	}
}

void TPO_Widget::slot_mfsk24_impulse_change(QVariantList channels)
{
	if ((channels.count() % 2) != 0)
	{
		edit->append("Error in size of channels array!");
		return;
	}
	QMap<int, int> channels_map;
	for (int i = 0; i < channels.count(); i+=2)
	{
		channels_map.insert(channels[i].toInt(), channels[i + 1].toInt());
	}
	if (channels_map.contains(controlOnCommon))
	{
		if ((channels_map[controlOnCommon] > 300) && (channels_map[controlOnCommon] < 100))
		{
			for (int i = 0; i < 4; i++)
			{
				if (channels_map.contains(controlOnVM0 + i) && (channels_map[controlOnVM0 + i] > 300) && (channels_map[controlOnVM0 + i] < 100))
				{
					pitanie[i] = true;
					edit->append(QString("Turning VM%1 ON").arg(i));
				}
			}
		}
	}

	if (channels_map.contains(controlOffCommon))
	{
		if ((channels_map[controlOffCommon] > 300) && (channels_map[controlOffCommon] < 100))
		{
			for (int i = 0; i < 4; i++)
			{
				if (channels_map.contains(controlOffVM0 + i) && (channels_map[controlOffVM0 + i] > 300) && (channels_map[controlOffVM0 + i] < 100))
				{
					pitanie[i] = false;
					edit->append(QString("Turning VM%1 OFF").arg(i));
				}
			}
		}
	}

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

	for (int i = 0; i < words.size(); i++)
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

		for (int i = 0; i < count_words_for_transmit; i++)
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
			if (os == -1)
			{
				miss_receipt(os);
				return;
			}
			else
			{
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
					miss_receipt(os);
					return;
				}
				else
				{
					str = QString("Everything is all right! The exchange is completed!");
					edit->append(str);
				}
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
				str = QString("Problems with receipt about wrong CRC of transmitted words");
				miss_receipt(os);
				return;
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
			str = QString("Problems with receipt about wrong CRC of command word");
			edit->append(str);
			miss_receipt(os);
			return;
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

		for (int i = 0; i < count_words_for_transmit; i++)
		{
			trans_words << 0;
		}
		emit signal_send_new_msg(mko, line, cwd.com_word, trans_words, os);
		str = QString("Transmit command using MKO - %1 and line - %2 CW - 0x%3").arg(mko).arg(line).arg(cwd.com_word, 4, 16, QChar('0'));
		edit->append(str);
		if (os == -1)
		{
			miss_receipt(os);
			return;
		}
		else
		{
			str = QString("Everything is all right! Transmitted received words to 17 subadr is completed!");
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
				str = QString("Problems with the transfer to subaddress");
				edit->append(str);
				miss_receipt(os);
			}
			else
			{
				counted_crc16_for_words = count_crc_for_words(trans_words);
				byte_count = count_byte_for_words(trans_words);
				answer = 0x0019;
				str = QString("Everything is all right! Transmitted received words to 17 subadr is completed!");
				edit->append(str);
				emit signal_send_read_receipt(answer, byte_count, counted_crc16_for_words, os);
				if (os == -1)
				{
					miss_receipt(os);
				}
				else
				{
					str = QString("Everything is all right! The exchange is completed!");
					edit->append(str);
				}
			}
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
			miss_receipt(os);
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
		miss_receipt(os);
	}
	else
	{
		str = QString("The exchange attempt is completed!");
		edit->append(str);
	}
}
void TPO_Widget::miss_receipt(int& os)
{
	QString str;
	int answer = 0x8404;
	str = QString("Get response word with code - 0x%1. Send receipt with code - 0x%2").arg(os, 4, 16, QChar('0')).arg(answer, 4, 16, QChar('0'));
	edit->append(str);
}
