#include "BUFAR.h"

#include "rpc_ports.h"

#include <QMessageBox>

union MKOWord
{
	quint16 com_word;				 // командное слово целиком
	struct
	{
		quint16 count_word : 5,    // число сл.данных / команда
			subadr : 5,   // подадрес
			trans_dir : 1,       // направление передачи(1-чт.ќ”)
			adr : 5;
	};
};

union I_1_DataWords
{
	quint16 data_words[3];

	I_1_DataWords(QVariantList raw_words)
	{
		for (int i = 0; i < raw_words.size(); i++)
			this->data_words[i] = static_cast<quint16>(raw_words.at(i).toInt());
	}

	struct
	{
		quint16
			AFAR_mode : 4,
			rez : 12;
		quint16
			OY_AFAR : 15,
			OY_pl_mn : 1;
		quint16
			OZ_AFAR : 15,
			OZ_pl_mn : 1;
	};
};

union I_4_6_DataWords
{
	quint16 data_words;

	struct
	{
		quint16
			faze_code_1 : 6,
			rez_1 : 1,
			chan_1 : 1,
			faze_code_2 : 6,
			rez_2 : 1,
			chan_2 : 1;
	};
};

union I_5_7_DataWords
{
	quint16 data_words;

	struct
	{
		quint16
			faze_code_1 : 6,
			rez_1 : 2,
			faze_code_2 : 6,
			rez_2 : 2;
	};
};

union I_8_DataWords
{
	quint16 data_words;

	struct
	{
		quint16
			BUMs_numb : 5,
			rez : 11;
	};
};

void BUM_cl::create_BUMs()
{
	BUMs_gbox = new QGroupBox("");
	BUMs_glay = new QGridLayout(BUMs_gbox);

	for (int n = 0; n < 2; n++)
	{
		for (int i = 0; i < 2; i++)
		{
			QString numb = QString::number(i + n * 2);
			BUMs_pbut << new QPushButton(numb);
			BUMs_pbut[i + n * 2]->setText("");
			BUMs_pbut[i + n * 2]->setMaximumWidth(16);
			BUMs_pbut[i + n * 2]->setMaximumHeight(16);
			BUMs_pbut[i + n * 2]->setStyleSheet("background-color: rgb(204, 204, 204);");
			BUMs_pbut[i + n * 2]->setCheckable(false);
			BUMs_pbut[i + n * 2]->setText(QString("%1").arg(i + n * 2 + 1));
			BUMs_glay->addWidget(BUMs_pbut[i + n * 2], 2 - i, 2 - n);
		}
	}
}

BUFAR_widg::BUFAR_widg(QWidget *parent)
{
	widg = new QWidget(this);
	setWindowTitle("БУФАР");

	All_vlay = new QVBoxLayout(this);
	BUFAR_cm_vlay = new QVBoxLayout();
	BUFAR_compl_hlay = new QHBoxLayout();
	BUFAR_mode_hlay = new QHBoxLayout();
	BUMs_glay = new QGridLayout();

	BUFAR_comp_lb = new QLabel(this);
	BUFAR_comp_le = new QLineEdit(this);
	BUFAR_mode_lb = new QLabel(this);
	BUFAR_mode_le = new QLineEdit(this);

	BUFAR_comp_lb->setText("Комплект");
	BUFAR_comp_le->setReadOnly(true);
	BUFAR_comp_le->setMaximumWidth(150);
	BUFAR_comp_le->setMinimumWidth(150);
	BUFAR_mode_lb->setText("Режим");
	BUFAR_mode_le->setReadOnly(true);
	BUFAR_mode_le->setMaximumWidth(150);
	BUFAR_mode_le->setMinimumWidth(150);

	All_vlay->addLayout(BUFAR_cm_vlay);
	BUFAR_cm_vlay->addLayout(BUFAR_compl_hlay);
	BUFAR_cm_vlay->addLayout(BUFAR_mode_hlay);
	All_vlay->addLayout(BUMs_glay);

	BUFAR_compl_hlay->addWidget(BUFAR_comp_lb);
	BUFAR_compl_hlay->addWidget(BUFAR_comp_le);
	BUFAR_mode_hlay->addWidget(BUFAR_mode_lb);
	BUFAR_mode_hlay->addWidget(BUFAR_mode_le);

	for (int i = 0; i < 25; i++)
	{
		BUM_current tmp_BUM;
		BUM_current_list.push_back(tmp_BUM);
	}

	//spaceitem - нужно ли?

	for (int n = 0; n < 5; n++)
	{
		for (int i = 0; i < 5; i++)
		{
			BUM_cl tmp_BUM_cl;
			tmp_BUM_cl.create_BUMs();
			BUM_cl_list.push_back(tmp_BUM_cl);
			BUM_cl_list.at(i + n * 5).BUMs_gbox->setAlignment(Qt::AlignHCenter);
			BUM_cl_list.at(i + n * 5).BUMs_gbox->setTitle(QString("БУМ %1").arg((i + n * 5) + 1));
			BUMs_glay->addWidget(BUM_cl_list.at(i + n * 5).BUMs_gbox, n, i);
		}
	}

	slot_thr.set_connection_params("127.0.0.1", OMNIBUS_SLOT);
	slot_thr.start(); // вот тут падает

	signal_thr.set_connection_params("127.0.0.1", OMNIBUS_SIGNAL);
	signal_thr.start(); // вот тут падает

	if (!slot_thr.wait_connected(3) || !signal_thr.wait_connected(3))
	{
		QMessageBox::critical(0, "Нет соединения", "Ошибка соединения с rpc_omnibus");
		this->deleteLater();
		return;
	}

	slot_thr.get_omnibus_obj()->switch_ab(MKO, adr, false);

	power_slot_thr.set_connection_params("127.0.0.1", POWER_SLOT);
	power_slot_thr.start(); // вот тут падает

	power_signal_thr.set_connection_params("127.0.0.1", POWER_SIGNAL);
	power_signal_thr.start(); // вот тут падает

	if (!power_slot_thr.wait_connected(3) || !power_signal_thr.wait_connected(3))
	{
		QMessageBox::critical(0, "Нет соединения", "Ошибка соединения с power_bus");
		this->deleteLater();
		return;
	}

	mku_slot_thr.set_connection_params("127.0.0.1", MKU_SLOT);
	mku_slot_thr.start(); // вот тут падает

	mku_signal_thr.set_connection_params("127.0.0.1", MKU_SIGNAL);
	mku_signal_thr.start(); // вот тут падает

	if (!mku_slot_thr.wait_connected(3) || !mku_signal_thr.wait_connected(3))
	{
		QMessageBox::critical(0, "Нет соединения", "Ошибка соединения с МКУ");
		this->deleteLater();
		return;
	}

	connect(signal_thr.get_obj().get(), SIGNAL(new_message(QVariant, int, int, int, QVariantList, int)), this, SLOT(new_message(QVariant, int, int, int, QVariantList, int)));
	connect(mku_signal_thr.get_obj().get(), SIGNAL(new_mt_at_state(int, int)), this, SLOT(new_mt_at_state(int, int)));
	connect(power_signal_thr.get_obj().get(), SIGNAL(u_on_nk(double)), this, SLOT(get_power(double)));

	//log_filename = QString("d:/logs/%1_%2.log").arg(QCoreApplication::applicationName()).arg(QDateTime::currentDateTime().toString("yyyy.MM.dd_hh.mm.ss"));
	//QDir dir("d:/logs");
	//if (!dir.exists())
	//	QDir().mkdir("d:/logs");
	////connect(&log_timer, &QTimer::timeout, this, &BECH_widg::log_timer_ontimer);
	//log_timer.start(200);


	//edit = new QTextEdit(this);
	//_scroll_bar = edit->verticalScrollBar();
	//_doc = new QTextDocument();
	//_cursor = new QTextCursor(_doc);
	//edit->setDocument(_doc);
	//edit->setReadOnly(true);
	//_doc->setMaximumBlockCount(1000);
	//setMinimumSize(360, 300);
	//auto_scroll_box = new QCheckBox(this);
	//auto_scroll_box->setText("Автопрокрутка");
	//auto_scroll_box->setChecked(true);
	////connect(auto_scroll_box, &QCheckBox::stateChanged, this, &BECH_widg::auto_scroll_clicked);

	//QSettings settings(QApplication::applicationDirPath() + "/positions.ini", QSettings::IniFormat);
	//restoreGeometry(settings.value("bech_geometry").toByteArray());
}

void BUFAR_widg::get_power(double _volt)
{
	volt = _volt;
	if (volt >= 20.0)
		imit_on();
	else
		if (volt < 1)
			imit_off();
}

void BUFAR_widg::set_power_back()
{
	//double curr;
	//if (power_vt >= 1)
	//{
	//	curr = (double)power_vt / volt;
	//}
	//else
	//	curr = 0.0;
	//power_slot_thr.get_power_bus_obj()->set_i(bus, name, curr);
}

void BUFAR_widg::imit_off()
{
	slot_thr.get_omnibus_obj()->switch_ab(MKO, adr, false);
	BUFAR_mode = MODE_OFF;
	for (int i = 0; i < 25; i++)
	{
		BUM_current_list[i].power = false;
		BUM_current_list[i].chan_1 = false;
		BUM_current_list[i].chan_2 = false;
		BUM_current_list[i].chan_3 = false;
		BUM_current_list[i].chan_4 = false;
		BUM_current_list[i].faze = 0;
	}
	update_graphics();
}

void BUFAR_widg::imit_on()
{
	slot_thr.get_omnibus_obj()->switch_ab(MKO, adr, true);
	BUFAR_mode = MODE_1;
	for (int i = 0; i < 25; i++)
	{
		BUM_current_list[i].power = true;
	}
}

void BUFAR_widg::new_mk(int mshm, int pshm, int length_m, int length_p, double u_m, double u_p, int dt, int line_m, int line_p)
{
	if (volt != 0)
	{
		int uu = 0;
		//	QString _msg = QString("%1 принял МК МШ%2 ПШ%3").arg(QTime::currentTime().toString("hh:mm:ss.zzz")).arg(mshm).arg(pshm);
		//	msg_to_log(_msg);
		int tmp_mshm = mshm;
		int tmp_pshm = pshm;

		update_graphics();
		set_new_tm();
	}
}

void BUFAR_widg::msg_to_log(const QString& _msg)
{
	{
		QMutexLocker lock(&log_mutex);
		log_buffer << _msg;
	}
	_cursor->insertText(_msg + "\n");
	if (auto_scroll)
		_scroll_bar->setValue(_scroll_bar->maximum());
}

void BUFAR_widg::auto_scroll_clicked(int _state)
{
	auto_scroll = (_state != 0);
}

void BUFAR_widg::log_timer_ontimer()
{
	QStringList tmp_buffer;
	{
		QMutexLocker lock(&log_mutex);
		tmp_buffer = log_buffer;
		log_buffer.clear();
	}
	if (tmp_buffer.isEmpty())
		return;
	QFile log_file(log_filename);
	QTextStream log_stream(&log_file);
	log_file.open(QIODevice::Append);
	for (QStringList::iterator itr = tmp_buffer.begin(); itr != tmp_buffer.end(); itr++)
		log_stream << *itr << "\n";
	log_file.close();
}

void BUFAR_widg::new_message(QVariant dt, int mko, int line, int cwd, QVariantList words, int os)
{
	MKOWord tmp_cwd;
	tmp_cwd.com_word = cwd;
	if (os == -1)
		return;
	if ((mko == MKO) && (tmp_cwd.adr == adr) && (BUFAR_current != 0))
	{
		//	QString _msg = QString("%1 принял сигнал на подадресе %2 c КС %3").arg(QTime::currentTime().toString("hh:mm:ss.zzz")).arg(tmp_cwd.subadr).arg(tmp_cwd.com_word);
		//	msg_to_log(_msg);
		switch (tmp_cwd.subadr)
		{
		case 1:
			I_1(words);
			break;
		case 3:
			I_3(words);
			break;
		case 4:
			I_4(words);
			break;
		case 6:
			I_6(words);
			break;
		case 8:			
			I_8(words);
			break;
		}
		update_graphics();
		set_new_tm();
	}
}

void BUFAR_widg::change_BUMs(int num, bool ch_1, bool ch_2, bool ch_3, bool ch_4)
{
	BUM_current_list[num].chan_1 = ch_1;
	BUM_current_list[num].chan_2 = ch_2;
	BUM_current_list[num].chan_3 = ch_3;
	BUM_current_list[num].chan_4 = ch_4;
}

void BUFAR_widg::change_BUMs(int num, bool all_ch)
{
	BUM_current_list[num].chan_1 = all_ch;
	BUM_current_list[num].chan_2 = all_ch;
	BUM_current_list[num].chan_3 = all_ch;
	BUM_current_list[num].chan_4 = all_ch;
}

void BUFAR_widg::new_mt_at_state(int dev_name, int state)
{
	if (dev_name == BAU_at_mt(BUFAR_st))
		BUFAR_current = state;
	update_graphics();
}

void BUFAR_widg::I_1(QVariantList words)
{
	I_1_DataWords d_words = words;
	OY_faze = 0;
	OZ_faze = 0;
	for (int i = 0; i < 25; i++)
	{
		change_BUMs(i, false);
		BUFAR_mode = MODE_1;
	}
	switch (d_words.AFAR_mode)
	{
	case 0:
		break;
	case 1:
		change_BUMs(0, true);
		change_BUMs(1, true);
		change_BUMs(2, false, false, true, true);
		change_BUMs(5, true);
		change_BUMs(6, true);
		change_BUMs(7, false, false, true, true);
		change_BUMs(10, false, true, false, true);
		change_BUMs(11, false, true, false, true);
		change_BUMs(12, false, false, false, true);
		BUFAR_mode = MODE_2;
		break;
	case 2:
		change_BUMs(2, true, true, false, false);
		change_BUMs(3, true);
		change_BUMs(4, true);
		change_BUMs(7, true, true, false, false);
		change_BUMs(8, true);
		change_BUMs(9, true);
		change_BUMs(12, false, true, false, false);
		change_BUMs(13, false, true, false, true);
		change_BUMs(14, false, true, false, true);
		BUFAR_mode = MODE_2;
		break;
	case 3:
		change_BUMs(10, true, false, true, false);
		change_BUMs(11, true, false, true, false);
		change_BUMs(12, false, false, true, false);
		change_BUMs(15, true);
		change_BUMs(16, true);
		change_BUMs(17, false, false, true, true);
		change_BUMs(20, true);
		change_BUMs(21, true);
		change_BUMs(22, false, false, true, true);
		BUFAR_mode = MODE_2;
		break;
	case 4:
		change_BUMs(12, true, false, false, false);
		change_BUMs(13, true, false, true, false);
		change_BUMs(14, true, false, true, false);
		change_BUMs(17, true, true, false, false);
		change_BUMs(18, true);
		change_BUMs(19, true);
		change_BUMs(22, true, true, false, false);
		change_BUMs(23, true);
		change_BUMs(24, true);
		BUFAR_mode = MODE_2;
		break;
	case 5:
		for (int i = 0; i < 25; i++)
		{
			change_BUMs(i, true);
			BUFAR_mode = MODE_3;
		}
		break;
	}
	OY_faze = d_words.OY_AFAR * 0.011;
	OZ_faze = d_words.OZ_AFAR * 0.011;
	if (d_words.OY_pl_mn)
		OY_faze = -1 * OY_faze;
	if (d_words.OZ_pl_mn)
		OZ_faze = -1 * OZ_faze;
	BUFAR_sub_mode = d_words.AFAR_mode;
	update_graphics();
}

void BUFAR_widg::I_3_coord(int x, I_3_DataWords words)
{
	int coord, y = 0;
	if (words.V1)
	{
		y = 0;
		coord = x + y;
		change_BUMs(coord, true);
	}
	if (words.V2)
	{
		y = 5;
		coord = x + y;
		change_BUMs(coord, true);
	}
	if (words.V3)
	{
		y = 10;
		coord = x + y;
		change_BUMs(coord, true);
	}
	if (words.V4)
	{
		y = 15;
		coord = x + y;
		change_BUMs(coord, true);
	}
	if (words.V5)
	{
		y = 20;
		coord = x + y;
		change_BUMs(coord, true);
	}
}

void BUFAR_widg::I_3(QVariantList words)
{
	I_3_DataWords d_words;
	d_words.data_words = static_cast<quint16>(words.at(1).toInt());
	int x, y, coord = 0;
	if (d_words.G1)
	{
		x = 1;
		I_3_coord(x, d_words);
	}
	if (d_words.G2)
	{
		x = 2;
		I_3_coord(x, d_words);
	}
	if (d_words.G3)
	{
		x = 3;
		I_3_coord(x, d_words);
	}
	if (d_words.G4)
	{
		x = 4;
		I_3_coord(x, d_words);
	}
	if (d_words.G5)
	{
		x = 5;
		I_3_coord(x, d_words);
	}
	update_graphics();
}

void BUFAR_widg::I_4(QVariantList words)
{

}

void BUFAR_widg::I_5()
{

}

void BUFAR_widg::I_6(QVariantList words)
{

}

void BUFAR_widg::I_7()
{

}

void BUFAR_widg::I_8(QVariantList words)
{
	I_8_DataWords d_words;
	d_words.data_words = static_cast<quint16>(words.at(1).toInt());
	I_9(d_words.BUMs_numb);
}

void BUFAR_widg::faze_calc(int BUM_numb, int ind_faze, int chan)
{
	//Выключен - 0 градусов. Включён - 20 градусов. Входной сигнал всегда есть. 
	//Fij = (i * Fy + j * Fz + Foji) mod 360
	QVariantList tmp_list;
	int Fij = 0;
	int Foij = ind_faze;
	int i = 5;
	int j = 0;
	int tmp_numb;
	///Расчёт i и j
	do
	{
		i = i - 1;
		do
		{
			j++;
			tmp_numb = i * j;
		} while ((tmp_numb = BUM_numb) && (j = 5));
		j = 0;
	} while ((tmp_numb = BUM_numb) && (i = 1));
	i = i * 2;
	j = j * 2;
	
	switch (chan)
	{
	case 1:
		if (BUM_current_list[BUM_numb].chan_1)
		{
			if (OY_faze < 0)
				Fij = ((i - 1) * (OY_faze + 360) + j * OZ_faze + Foij);
			else
				Fij = ((i - 1) * OY_faze + j * OZ_faze + Foij);
			Fij = Fij / 5.625;
		}
		break;
	case 2:
		if (BUM_current_list[BUM_numb].chan_2)
		{
			if (OY_faze < 0)
				Fij = ((i - 1) * (OY_faze + 360) + j * OZ_faze + Foij);
			else
				Fij = ((i - 1) * OY_faze + j * OZ_faze + Foij);
			Fij = Fij / 5.625;
		}
		break;
	case 3:
		if (BUM_current_list[BUM_numb].chan_3)
		{
			if (OY_faze < 0)
				Fij = ((i - 1) * (OY_faze + 360) + j * OZ_faze + Foij);
			else
				Fij = ((i - 1) * OY_faze + j * OZ_faze + Foij);
			Fij = Fij / 5.625;
		}
		break;
	case 4:
		if (BUM_current_list[BUM_numb].chan_4)
		{
			if (OY_faze < 0)
				Fij = ((i - 1) * (OY_faze + 360) + j * OZ_faze + Foij);
			else
				Fij = ((i - 1) * OY_faze + j * OZ_faze + Foij);
			Fij = Fij / 5.625;
		}
		break;
	}
}

void BUFAR_widg::I_9(int BUM_numb)
{
	//Выключен - 0 градусов. Включён - 20 градусов. Входной сигнал всегда есть. 
	//Fij = (i * Fy + j * Fz + Foji) mod 360
	QVariantList tmp_list;
	unsigned short tmp_word = 0;
	int Fij = 0;
	int Foij = 0;
	int i = 5;
	int j = 0;
	int tmp_numb;
	///Расчёт i и j
	do
	{
		i = i - 1;
		do
		{
			j++;
			tmp_numb = i * j;
		} 
		while ((tmp_numb = BUM_numb) && (j = 5));
		j = 0;
	} 
	while ((tmp_numb = BUM_numb) && (i = 1));
	i = i * 2;
	j = j * 2;
	///СД 1
	if (BUM_current_list[BUM_numb].chan_1)
	{
		if (OY_faze < 0)
			Fij = ((i - 1) * (OY_faze + 360) + j * OZ_faze + Foij);
		else
			Fij = ((i - 1) * OY_faze + j * OZ_faze + Foij);
		Fij = Fij / 5.625;
		tmp_word = tmp_word + 0x80 + Fij;
	}


	if (BUM_current_list[BUM_numb].chan_2)
	{
		if (OY_faze < 0)
			Fij = (i * (OY_faze + 360) + j * OZ_faze + Foij);
		else
			Fij = (i * OY_faze + j * OZ_faze + Foij);
		Fij = Fij / 5.625;
		tmp_word = tmp_word + 0x8000 + Fij;
	}

	tmp_list.push_back(tmp_word);
	tmp_word = 0;
	///СД 2
	if (BUM_current_list[BUM_numb].chan_3)
	{
		if (OY_faze < 0)
			Fij = ((i - 1) * (OY_faze + 360) + (j - 1) * OZ_faze + Foij);
		else
			Fij = ((i - 1) * OY_faze + (j - 1) * OZ_faze + Foij);
		Fij = Fij / 5.625;
		tmp_word = tmp_word + 0x80 + Fij;
	}
	if (BUM_current_list[BUM_numb].chan_4)
	{
		if (OY_faze < 0)
			Fij = (i  * (OY_faze + 360) + (j - 1) * OZ_faze + Foij);
		else
			Fij = (i * OY_faze + (j - 1) * OZ_faze + Foij);
		Fij = Fij / 5.625;
		tmp_word = tmp_word + 0x8000;
	}

	tmp_list.push_back(tmp_word);
	tmp_word = 0;
	///СД 3
	if (BUM_current_list[BUM_numb].chan_1)
	{
		tmp_word = 1;
		tmp_list.push_back(tmp_word);
		tmp_word = 0;
	}
	///СД 4
	if (BUM_current_list[BUM_numb].chan_2)
	{
		tmp_word = 1;
		tmp_list.push_back(tmp_word);
		tmp_word = 0;
	}
	///СД 5
	if (BUM_current_list[BUM_numb].chan_3)
	{
		tmp_word = 1;
		tmp_list.push_back(tmp_word);
		tmp_word = 0;
	}
	///СД 6
	if (BUM_current_list[BUM_numb].chan_4)
	{
		tmp_word = 1;
		tmp_list.push_back(tmp_word);
		tmp_word = 0;
	}
	///СД 7
	if (BUM_current_list[BUM_numb].chan_1)
	{
		tmp_word = 1;
		tmp_list.push_back(tmp_word);
		tmp_word = 0;
	}
	///СД 8
	if (BUM_current_list[BUM_numb].chan_2)
	{
		tmp_word = 1;
		tmp_list.push_back(tmp_word);
		tmp_word = 0;
	}
	///СД 9
	if (BUM_current_list[BUM_numb].chan_3)
	{
		tmp_word = 1;
		tmp_list.push_back(tmp_word);
		tmp_word = 0;
	}
	///СД 10
	if (BUM_current_list[BUM_numb].chan_4)
	{
		tmp_word = 1;
		tmp_list.push_back(tmp_word);
		tmp_word = 0;
	}
	///СД 11
	if (BUM_current_list[BUM_numb].power)
	{
		tmp_word = 1;
		tmp_list.push_back(tmp_word);
		tmp_word = 0;
	}
	///СД 12
	if (BUM_current_list[BUM_numb].power)
		tmp_word = 0x14;
	else
		tmp_word = 0;
	tmp_list.push_back(tmp_word);
	tmp_word = 0;
	///СД 13
	if (BUM_current_list[BUM_numb].power)
		tmp_word = 0x14;
	else
		tmp_word = 0;
	tmp_list.push_back(tmp_word);
	tmp_word = 0;

	slot_thr.get_omnibus_obj()->set_new_data(MKO, adr, 9, tmp_list);
}

void BUFAR_widg::update_graphics()
{
	for (int i = 0; i < 25; i++)
	{
		for (int n = 0; n < 4; n++)
		{
			BUM_cl_list[i].BUMs_pbut[n]->setStyleSheet("background-color: rgb(204, 204, 204);");
		}
	}
	for (int i = 0; i < 25; i++)
	{
		if (BUM_current_list[i].chan_1)
			BUM_cl_list[i].BUMs_pbut[0]->setStyleSheet("background-color: rgb(142, 198, 156);");
		if (BUM_current_list[i].chan_2)
			BUM_cl_list[i].BUMs_pbut[1]->setStyleSheet("background-color: rgb(142, 198, 156);");
		if (BUM_current_list[i].chan_3)
			BUM_cl_list[i].BUMs_pbut[2]->setStyleSheet("background-color: rgb(142, 198, 156);");
		if (BUM_current_list[i].chan_4)
			BUM_cl_list[i].BUMs_pbut[3]->setStyleSheet("background-color: rgb(142, 198, 156);");
	}
	if (BUFAR_mode != MODE_OFF)
	{
		if (BUFAR_mode == MODE_2)
		{
			BUFAR_mode_le->setText(QString("%1 - %2").arg(BUFAR_mode).arg(BUFAR_sub_mode));
		}
		else
			BUFAR_mode_le->setText(QString("%1").arg(BUFAR_mode));
		BUFAR_comp_le->setText(QString("%1").arg(BUFAR_current));
	}
	else
	{
		BUFAR_mode_le->setText("");
		BUFAR_comp_le->setText("");
	}
}

void BUFAR_widg::set_new_tm()
{
	QVariantList tmp_list;
	unsigned short tmp_word = 0;
	unsigned short tmp_bit = 1;
	for (int i = 0; i < 16; i++)
	{
		if (BUM_current_list[i].power)
			tmp_word = tmp_word + (tmp_bit << i);
	}
	tmp_list.push_back(tmp_word);
	tmp_word = 0;

	for (int i = 0; i < 9; i++)
	{
		if (BUM_current_list[i+16].power)
			tmp_word = tmp_word + (tmp_bit << i);
	}
	tmp_bit = BUFAR_sub_mode;
	tmp_word = tmp_word + (tmp_bit << 12);
	tmp_list.push_back(tmp_word);
	tmp_word = 0;

	if (BUFAR_mode == MODE_2)
	{
		int tmp_count = 0;
		for (int n = 0; n < 12; n++)
		{
			tmp_word = 0;
			if ((BUM_current_list[tmp_count].chan_1) || (BUM_current_list[tmp_count].chan_2) || (BUM_current_list[tmp_count].chan_3) || (BUM_current_list[tmp_count].chan_4))
			{
				tmp_word = tmp_word + 0x10;
			}
			if (BUM_current_list[tmp_count].chan_1)
				tmp_word = tmp_word + 1;
			if (BUM_current_list[tmp_count].chan_2)
				tmp_word = tmp_word + 2;
			if (BUM_current_list[tmp_count].chan_3)
				tmp_word = tmp_word + 4;
			if (BUM_current_list[tmp_count].chan_4)
				tmp_word = tmp_word + 8;

			if ((BUM_current_list[tmp_count + 1].chan_1) || (BUM_current_list[tmp_count + 1].chan_2) || (BUM_current_list[tmp_count + 1].chan_3) || (BUM_current_list[tmp_count + 1].chan_4))
			{
				tmp_word = tmp_word + 0x1000;
			}

			if (BUM_current_list[tmp_count + 1].chan_1)
				tmp_word = tmp_word + 0x100;
			if (BUM_current_list[tmp_count + 1].chan_2)
				tmp_word = tmp_word + 0x200;
			if (BUM_current_list[tmp_count + 1].chan_3)
				tmp_word = tmp_word + 0x400;
			if (BUM_current_list[tmp_count + 1].chan_4)
				tmp_word = tmp_word + 0x800;

			tmp_count = tmp_count + 2;
			tmp_list.push_back(tmp_word);
		}

		tmp_word = 0;
		if ((BUM_current_list[24].chan_1) || (BUM_current_list[24].chan_2) || (BUM_current_list[24].chan_3) || (BUM_current_list[24].chan_4))
		{
			tmp_word = tmp_word + 0x10;
		}

		if (BUM_current_list[24].chan_1)
			tmp_word = tmp_word + 1;
		if (BUM_current_list[24].chan_2)
			tmp_word = tmp_word + 2;
		if (BUM_current_list[24].chan_3)
			tmp_word = tmp_word + 4;
		if (BUM_current_list[24].chan_4)
			tmp_word = tmp_word + 8;
		tmp_list.push_back(tmp_word);
	}
	else
	{
		if ((BUFAR_mode == MODE_1) || (BUFAR_mode == MODE_3))
		{
			for (int i = 0; i < 13; i++)
			{
				tmp_word = 0x1010;
				tmp_list.push_back(tmp_word);
			}
			tmp_word = 0x10;
			tmp_list.push_back(tmp_word);
		}
	}
	tmp_list.push_back(tmp_word);
	slot_thr.get_omnibus_obj()->set_new_data(MKO, adr, 2, tmp_list);
}

BUFAR_widg::~BUFAR_widg()
{
	slot_thr.quit();
	signal_thr.quit();

	mku_slot_thr.quit();
	mku_signal_thr.quit();

	power_slot_thr.quit();
	power_signal_thr.quit();
}

void BUFAR_widg::closeEvent(QCloseEvent *event)
{
	QSettings settings(QApplication::applicationDirPath() + "/positions.ini", QSettings::IniFormat);
	settings.setValue("bech_geometry", saveGeometry());
	QWidget::closeEvent(event);
}