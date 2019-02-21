#include "spobu_imit.h"
#include <QtWidgets>
#include <QFile>
#include <QFileDialog>

union MKOCOMWORD
{
	quint16 com_word;				// командное слово целиком
	struct 
	{
		quint16 count_word : 5,		// число сл.данных / команда
				subadr : 5,			// подадрес
				trans_dir : 1,		// направление передачи(1-чт.0-зап.)
				adr : 5;			// адрес
	};
};

union SPOBU_SK
{
	quint16 word;
	struct
	{
		quint16 sk_code : 7,		// Код вида СК
				reserv : 6,			// Резерв
				line : 1,			// Канал
				l_word_count : 2;	// Первые два бита в количестве слов
	};
	struct
	{
		quint16 u_word_count : 3,	// 3, 4 ,5 биты в количестве слов
				subadr : 5,			// Подарес
				trans_dir : 1,		// Направление передачи
				adr : 5,			// Адрес
				spec_bit : 2;		// Вид СК
	};
};

union WORD_QTY
{
	quint16 qty;
	struct
	{
		quint16 lower_bit : 2,
				upper_bit : 3;
	};
};


SPOBU_Widget::SPOBU_Widget() :QWidget()
{
	QVBoxLayout* v_lay = new QVBoxLayout(this);

	QHBoxLayout* omnibus_lay = new QHBoxLayout(this);
	QHBoxLayout* omnibus_but_lay = new QHBoxLayout(this);
	QVBoxLayout* omnibus_line_lay = new QVBoxLayout(this);
	QVBoxLayout* omnibus_label_lay = new QVBoxLayout(this);
	QLabel* omnibus_label_ip = new QLabel("omnibus_server_ip");
	QLabel* omnibus_label_port = new QLabel("omnibus_server_port");
	omnibus_edit_ip = new QLineEdit("127.0.0.1");
	omnibus_edit_port = new QLineEdit("50001");
	omnibus_button_stop = new QPushButton("Stop Omnibus");
	omnibus_button_restart = new QPushButton("Restart Omnibus");
	omnibus_line_lay->addWidget(omnibus_edit_ip);
	omnibus_line_lay->addWidget(omnibus_edit_port);
	omnibus_label_lay->addWidget(omnibus_label_ip);
	omnibus_label_lay->addWidget(omnibus_label_port);
	omnibus_lay->addLayout(omnibus_label_lay);
	omnibus_lay->addLayout(omnibus_line_lay);
	omnibus_but_lay->addWidget(omnibus_button_restart);
	omnibus_but_lay->addWidget(omnibus_button_stop);

	QHBoxLayout* foi_lay = new QHBoxLayout(this);
	QHBoxLayout* foi_but_lay = new QHBoxLayout(this);
	QVBoxLayout* foi_line_lay = new QVBoxLayout(this);
	QVBoxLayout* foi_label_lay = new QVBoxLayout(this);
	QLabel* foi_label_ip = new QLabel("omnibus server ip");
	QLabel* foi_slot_label_port = new QLabel("foi slot port");
	QLabel* foi_signal_label_port = new QLabel("foi signal port");
	foi_edit_ip = new QLineEdit("127.0.0.1");
	foi_edit_slot_port = new QLineEdit("30001");
	foi_edit_signal_port = new QLineEdit("30002");
	foi_slot_button_stop = new QPushButton("Stop slot FOI");
	foi_slot_button_restart = new QPushButton("Restart slot FOI");
	foi_signal_button_stop = new QPushButton("Stop signal FOI");
	foi_signal_button_restart = new QPushButton("Restart signal FOI");
	foi_line_lay->addWidget(foi_edit_ip);
	foi_line_lay->addWidget(foi_edit_slot_port);
	foi_line_lay->addWidget(foi_edit_signal_port);
	foi_label_lay->addWidget(foi_label_ip);
	foi_label_lay->addWidget(foi_slot_label_port);
	foi_label_lay->addWidget(foi_signal_label_port);
	foi_lay->addLayout(foi_label_lay);
	foi_lay->addLayout(foi_line_lay);
	foi_but_lay->addWidget(foi_slot_button_restart);
	foi_but_lay->addWidget(foi_slot_button_stop);
	foi_but_lay->addWidget(foi_signal_button_restart);
	foi_but_lay->addWidget(foi_signal_button_stop);


	QHBoxLayout* ch_file_lay = new QHBoxLayout(this);
	QLabel* ch_file_lb = new QLabel("File with settings:");
	edit_settings_file = new QLineEdit();
	choose_file = new QPushButton("...");
	read_file = new QPushButton("Set");
	ch_file_lay->addWidget(ch_file_lb);
	ch_file_lay->addWidget(edit_settings_file);
	ch_file_lay->addWidget(choose_file);
	ch_file_lay->addWidget(read_file);
	
	edit = new QTextEdit(this);
	start = new QPushButton("Start main programm");
	stop = new QPushButton("Stop main programm");

	setMinimumSize(490, 000);
	v_lay->addLayout(ch_file_lay);
	v_lay->addWidget(edit);
	v_lay->addLayout(omnibus_lay);
	v_lay->addLayout(omnibus_but_lay);
	v_lay->addLayout(foi_lay);
	v_lay->addLayout(foi_but_lay);
	v_lay->addWidget(start);
	v_lay->addWidget(stop);

	mko_imit = new MKO_IMIT_Object();
	mko_imit->create_slot_thread();

	inter_imit = new Interrupt_Object();
	inter_imit->create_slot_thread();
	inter_imit->create_signal_thread();
	moc_init();
	board_time = 0;

	qRegisterMetaType<QVariantList>("QVariantList&");
	qRegisterMetaType<int>("int&");
	qRegisterMetaType<uint>("uint&");

	QObject::connect(omnibus_button_restart, SIGNAL(clicked()), this, SLOT(slot_omnibus_button_restart()));
	QObject::connect(omnibus_button_stop, SIGNAL(clicked()), this, SLOT(slot_omnibus_button_stop()));
	QObject::connect(foi_slot_button_restart, SIGNAL(clicked()), this, SLOT(sl_foi_slot_button_restartt()));
	QObject::connect(foi_slot_button_stop, SIGNAL(clicked()), this, SLOT(sl_foi_slot_button_stop()));
	QObject::connect(foi_signal_button_restart, SIGNAL(clicked()), this, SLOT(sl_foi_signal_button_restartt()));
	QObject::connect(foi_signal_button_stop, SIGNAL(clicked()), this, SLOT(sl_foi_signal_button_stop()));
	QObject::connect(choose_file, SIGNAL(clicked()), this, SLOT(slot_choose_set_file()));
	QObject::connect(read_file, SIGNAL(clicked()), this, SLOT(read_settings()));
	QObject::connect(start, SIGNAL(clicked()), this, SLOT(main_cycle()));
	QObject::connect(stop, SIGNAL(clicked()), this, SLOT(stop_main_programm()));
	QObject::connect(inter_imit, SIGNAL(need_handle_interrupt()), this, SLOT(handle_interrupt()), Qt::DirectConnection);
	QObject::connect(inter_imit->get_rpc_foi_signal_thread()->get_obj().get(), SIGNAL(foi_interrupt(int, short, double, double)), inter_imit, SLOT(get_inter_from_server(int, short, double, double)), Qt::QueuedConnection);
	QObject::connect(mko_imit, SIGNAL(signal_send_msg_mko(int, int, int, QVariantList&, int&)), mko_imit->get_mko_slot_thread()->get_obj().get(), SLOT(send_msg(int, int, int, QVariantList&, int&)), Qt::DirectConnection);
	QObject::connect(this, SIGNAL(signal_send_msg_def_mko(int, int, int, int, QVariantList&)), mko_imit, SLOT(slot_send_msg_def_mko(int, int, int, int, QVariantList&)), Qt::DirectConnection);
	QObject::connect(this, SIGNAL(signal_send_msg_mko(int, int, int, QVariantList&, int&)), mko_imit, SLOT(slot_send_msg_mko(int, int, int, QVariantList&, int&)), Qt::DirectConnection);
}

void SPOBU_Widget::slot_omnibus_button_restart()
{
	QString ip_str = omnibus_edit_ip->text();
	int port = omnibus_edit_port->text().toInt();
	mko_imit->restart_slot_thread(ip_str, port);
	QObject::connect(mko_imit, SIGNAL(signal_send_msg_mko(int, int, int, QVariantList&, int&)), mko_imit->get_mko_slot_thread()->get_obj().get(), SLOT(send_msg(int, int, int, QVariantList&, int&)), Qt::DirectConnection);
}

void SPOBU_Widget::slot_omnibus_button_stop()
{
	mko_imit->stop_slot_thread();
}

void SPOBU_Widget::sl_foi_slot_button_restart()
{
	QString ip_str = foi_edit_ip->text();
	int port = foi_edit_slot_port->text().toInt();
	inter_imit->restart_slot_thread(ip_str, port);
}

void SPOBU_Widget::sl_foi_signal_button_restart()
{
	QString ip_str = foi_edit_ip->text();
	int port = foi_edit_slot_port->text().toInt();
	inter_imit->restart_signal_thread(ip_str, port);
	QObject::connect(inter_imit->get_rpc_foi_signal_thread()->get_obj().get(), SIGNAL(foi_interrupt(int, short, double, double)), inter_imit, SLOT(get_inter_from_server(int, short, double, double)), Qt::QueuedConnection);
}

void SPOBU_Widget::sl_foi_slot_button_stop()
{
	inter_imit->stop_slot_thread();
}

void SPOBU_Widget::sl_foi_signal_button_stop()
{
	inter_imit->stop_signal_thread();
}

void SPOBU_Widget::slot_choose_set_file()
{
	edit_settings_file->text().clear();
	setting_file = QFileDialog::getOpenFileName(this, "Open File", "E:\\","*.dat *.ini *.list");
	edit_settings_file->setText(setting_file);
}

void SPOBU_Widget::main_cycle()
{
	if (set_is_done)
	{
		main_discret = new QTimer;
		main_discret->setInterval(4000);
		QObject::connect(main_discret, SIGNAL(timeout()), this, SLOT(main_programm()));
		main_discret->start();
	}
	else if (edit_settings_file->text()!= "")
	{
		while (!set_is_done)
		{
			setting_file = edit_settings_file->text();
			read_settings();
		}
		main_discret = new QTimer;
		main_discret->setInterval(4000);
		QObject::connect(main_discret, SIGNAL(timeout()), this, SLOT(main_programm()));
		main_discret->start();
	}
	else if (edit_settings_file->text() == "")
	{
		while (!set_is_done)
		{
			slot_choose_set_file();
			read_settings();
		}
		main_discret = new QTimer;
		main_discret->setInterval(4000);
		QObject::connect(main_discret, SIGNAL(timeout()), this, SLOT(main_programm()));
		main_discret->start();
	}
	else
	{
		QString str = "Something is wrong! The programm is not started!";
		edit->append(str);
	}
}

void SPOBU_Widget::main_programm()
{
	board_time++;
	QString str = QString("Current BT = %1").arg(board_time);
	edit->append(str);
	//create_moc();
}

void SPOBU_Widget::stop_main_programm()
{
	main_discret->stop();
	board_time = 0;
}

void SPOBU_Widget::handle_interrupt()
{
	mutex.lock();
	QVector<int> buffer;
	buffer = inter_imit->get_interrupts();
	inter_imit->clear_inter_stack();
	for (int i = 0; i < buffer.size(); i++)
	{
		switch (buffer[i])
		{
		case 1:
			handle_inter_1();
			break;
		case 5:
			handle_inter_5();
			break;
		case 13:
			handle_inter_13();
			break;
		default:
			handle_unknown_inter();
			break;
		}
	}
	mutex.unlock();
}

void SPOBU_Widget::create_moc()
{
	is_ready = false;
	QString str = QString("Creating a MOC!");
	edit->append(str);
	
	MOC tmp_moc_struct;
	QVariantList words;
	for (int i = 0; i < settings.size(); i++)
	{
		mutex.lock();
		words.clear();
		tmp_moc_struct = settings[i];
		if ((tmp_moc_struct.adr == 2) && (tmp_moc_struct.subadr == 4))
		{
			words = read_data(tmp_moc_struct);
			int moc_pos = tmp_moc_struct.moc_position-1;
			int words_pos = 0;
			while ((moc_pos < tmp_moc_struct.word_count_to_MOC) && (words_pos < words.size()))
			{
				MOC_data[moc_pos] = words[words_pos];
				moc_pos++;
				words_pos++;
			}
		}
		if (tmp_moc_struct.name == "АСН")
		{
			int tmp = 0;
			if (ASN_Exchange)
			{
				tmp = get_asn_data();
				int moc_pos = tmp_moc_struct.moc_position - 1;
				MOC_data[moc_pos] = tmp;
			}
		}
		if ((tmp_moc_struct.adr == 4)&&(tmp_moc_struct.subadr == 17))
		{
			words = read_data(tmp_moc_struct);
			int moc_pos = tmp_moc_struct.moc_position - 1;
			int words_pos = 0;
			while ((moc_pos < (tmp_moc_struct.moc_position + tmp_moc_struct.word_count_to_MOC - 1)) && (words_pos < words.size()))
			{
				if (words_pos == 7)
				{
					MOC_data[217] = words[words_pos];
				}
				else
				{
					MOC_data[moc_pos] = words[words_pos];
				}
				moc_pos++;
				words_pos++;
			}
		}
		if ((tmp_moc_struct.adr == 17) && (tmp_moc_struct.subadr == 1))
		{
			words = read_data(tmp_moc_struct);
			MOC_data[167] = words[0];
			int moc_pos = 168;
			int words_pos = 19;
			while ((moc_pos < 168 + 5) && (words_pos < 23))
			{
				MOC_data[moc_pos] = words[words_pos];
				moc_pos++;
				words_pos++;
			}
		}
		if ((tmp_moc_struct.adr == 22) && (tmp_moc_struct.subadr == 1))
		{
			words = read_data(tmp_moc_struct);
			int moc_pos = tmp_moc_struct.moc_position - 1 ;
			int words_pos = 19;
			while ((moc_pos < (tmp_moc_struct.moc_position + tmp_moc_struct.word_count_to_MOC - 1)) && (words_pos < 28))
			{
				MOC_data[moc_pos] = words[words_pos];
				moc_pos++;
				words_pos++;
			}
		}
		else
		{
			words = read_data(tmp_moc_struct);
			int moc_pos = tmp_moc_struct.moc_position - 1;
			int words_pos = 0;
			while ((moc_pos < (tmp_moc_struct.moc_position + tmp_moc_struct.word_count_to_MOC - 1)) && (words_pos < words.size()))
			{
				MOC_data[moc_pos] = words[words_pos];
				moc_pos++;
				words_pos++;
			}
		}
		mutex.unlock();
	}
	is_ready = true;
	emit moc_is_ready();
}

QVariantList SPOBU_Widget::read_data(MOC tmp_struct)
{
	MKOCOMWORD CWD;
	QVariantList read_words;
	
	int mko = tmp_struct.mko;
	int line = 1;
	int os = 1;
	CWD.adr = tmp_struct.adr;
	CWD.subadr = tmp_struct.subadr;
	CWD.trans_dir = 1;
	CWD.count_word = tmp_struct.word_count_to_receive;
	emit signal_send_msg_mko(mko, line, CWD.com_word, read_words, os);
	
	return read_words;
}

void SPOBU_Widget::moc_init()
{
	for (int i = 0; i < 227; i++)
	{
		MOC_data << 0;
	}
}

void SPOBU_Widget::read_settings()
{
	QFile file(setting_file);
	QStringList strlist;
	if ((file.exists()) && (file.open(QIODevice::ReadOnly)))
	{
		while (!file.atEnd())
		{
			strlist << file.readLine();
		}
		file.close();
	}
	else
	{
		return;
	}
	
	MOC buf_struct;
	QString delim = "|";
	QString str_buf, str;
	
	str_buf.clear();
	setting_count = strlist.size();
	for (int i = 0; i < setting_count; i++)
	{
		int pos = 0;
		while (strlist[i][pos] != delim)
		{
			str_buf.append(strlist[i][pos]);
			pos++;
		}
		buf_struct.name = str_buf;
		pos++;
		str_buf.clear();
		while (strlist[i][pos] != delim)
		{
			str_buf.append(strlist[i][pos]);
			pos++;
		}
		buf_struct.mko = str_buf.toInt();
		pos++;
		str_buf.clear();
		while (strlist[i][pos] != delim)
		{
			str_buf.append(strlist[i][pos]);
			pos++;
		}
		buf_struct.adr = str_buf.toInt();
		pos++;
		str_buf.clear();
		while (strlist[i][pos] != delim)
		{
			str_buf.append(strlist[i][pos]);
			pos++;
		}
		buf_struct.subadr = str_buf.toInt();
		pos++;
		str_buf.clear();
		while (strlist[i][pos] != delim)
		{
			str_buf.append(strlist[i][pos]);
			pos++;
		}
		buf_struct.moc_position = str_buf.toInt();
		pos++;
		str_buf.clear();
		while (strlist[i][pos] != delim)
		{
			str_buf.append(strlist[i][pos]);
			pos++;
		}
		buf_struct.word_count_to_MOC = str_buf.toInt();
		pos++;
		str_buf.clear();
		while (strlist[i][pos] != delim)
		{
			str_buf.append(strlist[i][pos]);
			pos++;
		}
		buf_struct.word_count_to_receive = str_buf.toInt();
		pos++;
		str_buf.clear();
		for (pos; pos < strlist[i].size(); pos++)
		{
			str_buf += strlist[i][pos];
		}
		buf_struct.not_write_word_RB = diapazon(str_buf);
		settings << buf_struct;
	}
	set_is_done = true;
}

QVariantList SPOBU_Widget::diapazon(QString str)
{
	QString r_str, l_str, str_buf;
	QVariantList nom_words;
	int pos = 0;

	r_str.clear();
	l_str.clear();
	str_buf.clear();
	int s_str = str.size();

	for (pos; pos<str.size(); pos++)
	{
		if ((str[pos] != ",") && (str[pos] != "-") && (str[pos] != ";") && (str[pos] != '\n') && (str[pos] != '\r'))
		{
			str_buf.append(str[pos]);
		}
		if (((str[pos] == ";") || (str[pos] == ",") || (pos == (str.size() - 1))) && (l_str == "") && (r_str == "") && (str_buf != ""))
		{
			nom_words << str_buf.toInt();
			str_buf.clear();
		}
		if (((str[pos] == ";") || (str[pos] == ",") || (pos == (str.size() - 1))) && (l_str != "") && (r_str == ""))
		{
			r_str = str_buf;
			for (int ld = l_str.toInt(); ld <= r_str.toInt(); ld++)
			{
				nom_words << ld;
			}
			str_buf.clear();
			l_str.clear();
			r_str.clear();
		}
		if ((str[pos] == "-") && (l_str == "") && (r_str == ""))
		{
			l_str = str_buf;
			str_buf.clear();
		}
	}
	return nom_words;
}

int SPOBU_Widget::get_asn_data()
{
	return 0;
}

void SPOBU_Widget::handle_inter_1()
{
	MKOCOMWORD CWD;
	int os = 1;
	CWD.adr = 1;
	CWD.subadr = 1;
	CWD.count_word = 1;
	CWD.trans_dir = 1;
	QVariantList words;
	for (int i = 0; i < CWD.count_word; i++)
	{
		words << 0;
	}
	emit signal_send_msg_mko(1, 1, CWD.com_word, words, os);
	if (os == -1)
	{
	}
	else
	{
		switch (words[0].toInt())
		{
		case 0x0040:
		{
			int answer = words[0].toInt();
			int os = 1;
			send_receipt(1, 5, answer, os);
			perform_CPI();
			break;
		}
		case 0x0003:
		{
			int answer = words[0].toInt();
			int os = 1;
			send_receipt(1, 5, answer, os);
			emergency_shutdown_mca();
			break;
		}
		case 0x0080:
		{
			int answer = words[0].toInt();
			int os = 1;
			send_receipt(1, 5, answer, os);
			pause_BT();
			break;
		}
		case 0x00A0:
		{
			int answer = words[0].toInt();
			int os = 1;
			send_receipt(1, 5, answer, os);
			resume_BT();
			break;
		}
		default:
		{
			break;
		}
		}
	}
}

void SPOBU_Widget::handle_inter_5()
{
	/*QString str = QString("Handle interrupt - 5!");
	edit->append(str);*/
}

void SPOBU_Widget::handle_inter_13()
{
	/*QString str = QString("Handle interrupt - 13!");
	edit->append(str);*/
}

void SPOBU_Widget::handle_unknown_inter()
{
	/*QString str = QString("Get unknown interrupt!");
	edit->append(str);*/
}

void SPOBU_Widget::emergency_shutdown_mca()
{
	/*QString str = QString("Get signal to shutdown mca!");
	edit->append(str);*/
}

void SPOBU_Widget::pause_BT()
{
	main_discret->stop();
	bt_pause = true;
	return;
}

void SPOBU_Widget::resume_BT()
{
	bt_pause = false;
	main_discret->start();
	return;
}

void SPOBU_Widget::perform_CPI()
{
	QString str = QString("Perform CPI!");
	edit->append(str);
	MKOCOMWORD CWD;
	int answer;
	int os = 1;
	CWD.adr = 1;
	CWD.subadr = 17;
	CWD.count_word = 2;
	CWD.trans_dir = 1;
	QVariantList words;
	for (int i = 0; i < CWD.count_word; i++)
	{
		words << 0;
	}
	emit signal_send_msg_mko(1, 1, CWD.com_word, words, os);
	if (os == -1)
	{
		/*str = QString("Get response word with code - 0x%1").arg(os, 4, 16, QChar('0'));
		edit->append(str);*/
		answer = 0xFF00;
		wait_info_word = false;
		buf_size = 0;
		words_buffer.clear();
		send_receipt(1, 5, answer, os);
	}
	else
	{
		if ((words[0] == 0x0004) && (words[1] == 0x0000) && (!wait_info_word))
		{
			answer = 0x00FF;
			send_receipt(1, 5, answer, os);
			get_cmd();
		}
		else if ((words[0] == 0x000F) && (words[1] == 0x0000) && (wait_info_word))
		{
			answer = 0x00FF;
			send_receipt(1, 5, answer, os);
			add_data_to_buffer(words);
		}
		else
		{
			answer = 0xFF00;
			wait_info_word = false;
			buf_size = 0;
			words_buffer.clear();
			send_receipt(1, 5, answer, os);
		}
	}
}

void SPOBU_Widget::get_cmd()
{
	QString str;
	MKOCOMWORD CWD;
	SPOBU_SK SKWORD;
	WORD_QTY W_QTY;
	int answer;
	int os = 1;
	CWD.adr = 1;
	CWD.subadr = 21;
	CWD.count_word = 2;
	CWD.trans_dir = 1;
	QVariantList words;
	for (int i = 0; i < CWD.count_word; i++)
	{
		words << 0;
	}
	emit signal_send_msg_mko(1, 1, CWD.com_word, words, os);
	if (os == -1)
	{
		/*str = QString("Get response word with code - 0x%1").arg(os, 4, 16, QChar('0'));
		edit->append(str);*/
		send_receipt(1, 5, answer, os);
	}
	else
	{
		SKWORD.word = words[0].toInt();
		switch (SKWORD.sk_code)
		{
		case 0x0015:
			answer = 0x00FF;
			send_receipt(1, 5, answer, os);
			read_ab(words);
			break;
		case 0x003D:
			answer = 0x00FF;
			send_receipt(1, 5, answer, os);
			SKWORD.word = words[0].toInt();
			W_QTY.lower_bit = SKWORD.l_word_count;
			SKWORD.word = words[1].toInt();
			W_QTY.upper_bit = SKWORD.u_word_count;
			buf_size = W_QTY.qty + 2;
			add_data_to_buffer(words);
			break;
		default:
			answer = 0xFF00;
			send_receipt(1, 5, answer, os);
			/*str = QString("Get unknown command - 0x%1").arg(SKWORD.sk_code, 4, 16, QChar('0'));
			edit->append(str);*/
			break;
		}
	}
}

void SPOBU_Widget::read_ab(QVariantList& words)
{
	QString str;
	MKOCOMWORD CWD;
	SPOBU_SK SKWORD;
	WORD_QTY W_QTY;
	int line;
	int os = 1;

	SKWORD.word = words[0].toInt();
	W_QTY.lower_bit = SKWORD.l_word_count;
	line = SKWORD.line;
	SKWORD.word = words[1].toInt();
	W_QTY.upper_bit = SKWORD.u_word_count;
	CWD.subadr = SKWORD.subadr;
	CWD.adr = SKWORD.adr;
	CWD.trans_dir = SKWORD.trans_dir;
	CWD.count_word = W_QTY.qty;
	QVariantList r_words;
	for (int i = 0; i < CWD.count_word; i++)
	{
		r_words << 0;
	}
	emit signal_send_msg_mko(1, line, CWD.com_word, r_words, os);
	send_receipt(1, 7, 0x00FF, os);
	wait_info_word = false;
	buf_size = 0;
	words_buffer.clear();
	return;
}

void SPOBU_Widget::add_data_to_buffer(QVariantList& words)
{
	SPOBU_SK SKWORD;
	WORD_QTY W_QTY;
	if (!wait_info_word)
	{
		wait_info_word = true;
		SKWORD.word = words[0].toInt();
		W_QTY.lower_bit = SKWORD.l_word_count;
		SKWORD.word = words[1].toInt();
		W_QTY.upper_bit = SKWORD.u_word_count;
		buf_size = W_QTY.qty + 2;
	}
	for (int i = 0; i < words.size(); i++)
	{
		words_buffer << words[i];
	}
	if (words_buffer.size() >= buf_size)
	{
		write_ab();
		return;
	}
	else
	{
		return;
	}
}

void SPOBU_Widget::write_ab()
{
	MKOCOMWORD CWD;
	SPOBU_SK SKWORD;
	int line;
	int os = 1;

	SKWORD.word = words_buffer[0].toInt();
	line = SKWORD.line;
	SKWORD.word = words_buffer[1].toInt();
	CWD.adr = SKWORD.adr;
	CWD.subadr = SKWORD.subadr;
	CWD.count_word = buf_size;
	CWD.trans_dir = SKWORD.trans_dir;
	QVariantList words;
	for (int i = 2; i < buf_size + 2; i++)
	{
		words << words_buffer[i];
	}
	emit signal_send_msg_mko(1, line, CWD.com_word, words, os);
	send_receipt(1, 7, 0x00FF, os);
	wait_info_word = false;
	buf_size = 0;
	words_buffer.clear();
	return;
}

void SPOBU_Widget::send_receipt(int adr, int subadr, int answer, int& os)
{
	MKOCOMWORD CWD;
	CWD.adr = adr;
	CWD.subadr = subadr;
	CWD.count_word = 1;
	CWD.trans_dir = 0;
	QVariantList words;
	words << answer;
	emit signal_send_msg_mko(1, 1, CWD.com_word, words, os);
}
