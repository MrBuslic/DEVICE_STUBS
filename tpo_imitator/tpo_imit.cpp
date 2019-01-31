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

union VIP_State
{
	uint all_state;
	struct
	{
		uint ch_1 : 1,
			ch_2 : 1,
			ch_3 : 1,
			ch_4 : 1,
			ch_5 : 1,
			ch_6 : 1,
			ch_7 : 1,
			ch_8 : 1;
	};
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
		pitanie[i] = false;
	}
}

void TPO_Widget::slot_mds32_button()
{
	QString ip_str = mds_32_edit_ip->text();
	int port = mds_32_edit_port->text().toInt();
	mds32_imit->create_signal_thread(ip_str, port);
	QObject::connect(mds32_imit->get_mds32_exchange_thread()->get_obj().get(), SIGNAL(mds32_get_sample(int, uint&, int&)), this, SLOT(mds32_send_sample(int, uint&, int&)), Qt::QueuedConnection);
}

void TPO_Widget::slot_mfsk24_button()
{
	QString ip_str = mfsk24_edit_ip->text();
	int port = mfsk24_edit_port->text().toInt();
	mfsk24_imit->create_signal_thread(ip_str, port);
	QObject::connect(mfsk24_imit->get_mds32_exchange_thread()->get_obj().get(), SIGNAL(mfsk24_impulse_change(int, int)), this, SLOT(slot_mfsk24_impulse_change(int, int)), Qt::QueuedConnection);
}

void TPO_Widget::mds32_send_sample(int channel, uint& buf, int& flag)
{
	VIP_State vip;
	switch (channel)
	{
	case 1:
		{
			if (pitanie[channel-1] == false)
			{
				vip.ch_1 = 1;
				buf = vip.all_state;
				flag = 1;
			}
			else
			{
				vip.ch_1 = 0;
				buf = vip.all_state;
				flag = 0;
			}
		}
	case 2:
	{
		if (pitanie[channel - 1] == false)
		{
			vip.ch_2 = 1;
			buf = vip.all_state;
			flag = 1;
		}
		else
		{
			vip.ch_2 = 0;
			buf = vip.all_state;
			flag = 0;
		}
	}
	case 3:
	{
		if (pitanie[channel - 1] == false)
		{
			vip.ch_3 = 1;
			buf = vip.all_state;
			flag = 1;
		}
		else
		{
			vip.ch_3 = 0;
			buf = vip.all_state;
			flag = 0;
		}
	}
	case 4:
	{
		if (pitanie[channel - 1] == false)
		{
			vip.ch_4 = 1;
			buf = vip.all_state;
			flag = 1;
		}
		else
		{
			vip.ch_4 = 0;
			buf = vip.all_state;
			flag = 0;
		}
	}
	case 5:
	{
		if (pitanie[channel - 1] == false)
		{
			vip.ch_5 = 1;
			buf = vip.all_state;
			flag = 1;
		}
		else
		{
			vip.ch_5 = 0;
			buf = vip.all_state;
			flag = 0;
		}
	}
	default:
		edit->append("The specified channel is not for us!");
	}
}

void TPO_Widget::slot_mfsk24_impulse_change(int channel, int duration)
{
	if ((duration > 300) && (duration < 100))
	{
		switch (channel)
		{
		case 15:
			pitanie[0] = true;
		case 17:
			pitanie[1] = true;
		case 18:
			pitanie[2] = true;
		case 19:
			pitanie[3] = true;
		case 20:
			pitanie[4] = true;
		case 16:
			pitanie[0] = false;
		case 21:
			pitanie[1] = false;
		case 22:
			pitanie[2] = false;
		case 23:
			pitanie[3] = false;
		case 24:
			pitanie[4] = false;
		default:
			edit->append("The specified channel is not for us!");
		}
	}
	else
	{
		if (duration < 100)
		{
			edit->append("Catch noise in the exchange with mfsk24! Duration is lower 100 ms");
		}
		if (duration > 300)
		{
			edit->append("Catch noise in the exchange with mfsk24! Duration is more 300 ms");
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

		for (int i = 0; i++; i < count_words_for_transmit)
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
