#include "MBK02.h"
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

MBK02_widg::MBK02_widg(QWidget *parent)
{
	widg = new QWidget(this);
	setWindowTitle("МБК-02");

	Chan1_pbut = new QPushButton("1", this);
	Chan2_pbut = new QPushButton("2", this);
	Ant_pbut = new QPushButton("", this);
	Ant_pbut->setMinimumWidth(165);
	Chan1_pbut->setMinimumWidth(165);
	Chan2_pbut->setMinimumWidth(165);

	Sig1_pbut = new QPushButton("Основной", this);
	Sig2_pbut = new QPushButton("Резервный", this);
	Sig1_pbut->setMinimumWidth(165);
	Sig2_pbut->setMinimumWidth(165);

	QLabel* Lit_lb = new QLabel("Литера");
	QLabel* Ant_lb = new QLabel("Антенна");

	Signal_gbox = new QGroupBox("Сигнал",this);
	Channels_gbox = new QGroupBox("Каналы",this);
	Sup_gbox = new QGroupBox("", this);
	Signal_gbox->setAlignment(Qt::AlignHCenter);
	Channels_gbox->setAlignment(Qt::AlignHCenter);

	All_vblay = new QVBoxLayout(this);
	All_glay = new QGridLayout(Sup_gbox);
	Chan_hblay = new QHBoxLayout(Channels_gbox);
	Sig_hblay = new QHBoxLayout(Signal_gbox);
	Ant_vblay = new QVBoxLayout();
	Lit_vblay = new QVBoxLayout();
	Lit_le = new QLineEdit(this);

	All_vblay->addWidget(Channels_gbox);
	All_vblay->addWidget(Signal_gbox);
	All_vblay->addWidget(Sup_gbox);
	All_glay->addLayout(Ant_vblay,0, 0);
	All_glay->addLayout(Lit_vblay, 0, 1);

	Chan_hblay->addWidget(Chan1_pbut);
	Chan_hblay->addWidget(Chan2_pbut);
	Sig_hblay->addWidget(Sig1_pbut);
	Sig_hblay->addWidget(Sig2_pbut);
	Ant_vblay->addWidget(Ant_lb, 0, Qt::AlignHCenter);
	Ant_vblay->addWidget(Ant_pbut);
	Lit_vblay->addWidget(Lit_lb, 0, Qt::AlignHCenter);
	Lit_vblay->addWidget(Lit_le);

	Sig1_pbut->setStyleSheet("background-color: rgb(204, 204, 204);");
	Sig2_pbut->setStyleSheet("background-color: rgb(204, 204, 204);");
	Chan1_pbut->setStyleSheet("background-color: rgb(204, 204, 204);");
	Chan2_pbut->setStyleSheet("background-color: rgb(204, 204, 204);");
	Ant_pbut->setStyleSheet("background-color: rgb(204, 204, 204);");

	ant_names.insert(ANTENNA::MHA1MY, "MHA1-Y");
	ant_names.insert(ANTENNA::MHA1PY, "MHA1+Y");
	ant_names.insert(ANTENNA::MHA2MY, "MHA2-Y");
	ant_names.insert(ANTENNA::MHA2PY, "MHA2+Y");

	edit = new QTextEdit(this);
	_scroll_bar = edit->verticalScrollBar();
	_doc = new QTextDocument();
	_cursor = new QTextCursor(_doc);
	edit->setDocument(_doc);
	edit->setReadOnly(true);
	_doc->setMaximumBlockCount(1000);
	setMinimumSize(360, 300);
	auto_scroll_box = new QCheckBox(this);
	auto_scroll_box->setText("Автопрокрутка");
	auto_scroll_box->setChecked(true);
	connect(auto_scroll_box, &QCheckBox::stateChanged, this, &MBK02_widg::auto_scroll_clicked);
	All_vblay->addWidget(edit);
	All_vblay->addWidget(auto_scroll_box);

	///slot_thr.set_connection_params(instr::GetIpFromSettings("rpc_omnibus"), 50001); FIX!!!!!
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

	kpi_slot_thr.set_connection_params("127.0.0.1", KPI_SLOT);
	kpi_slot_thr.start();

	kpi_signal_thr.set_connection_params("127.0.0.1", KPI_SIGNAL);
	kpi_signal_thr.start();

	if (!kpi_slot_thr.wait_connected(3) || !kpi_signal_thr.wait_connected(3))
	{
		QMessageBox::critical(0, "Нет соединения", "Ошибка соединения с КПИ");
		this->deleteLater();
		return;
	}

	connect(signal_thr.get_obj().get(), SIGNAL(new_message(QVariant, int, int, int, QVariantList, int)), this, SLOT(new_message(QVariant, int, int, int, QVariantList, int)));


	slot_thr.get_omnibus_obj()->switch_ab(MKO, adr, true);
	flag = true;

	connect(mku_signal_thr.get_obj().get(), SIGNAL(new_mk(int, int, int, int, double, double, int, int, int)), this, SLOT(new_mk(int, int, int, int, double, double, int, int, int)));

	connect(kpi_signal_thr.get_obj().get(), SIGNAL(new_KPI(QVariantList)), this, SLOT(new_KPI(QVariantList)));

	log_filename = QString("d:/logs/%1_%2.log").arg(QCoreApplication::applicationName()).arg(QDateTime::currentDateTime().toString("yyyy.MM.dd_hh.mm.ss"));
	QDir dir("d:/logs");
	if (!dir.exists())
		QDir().mkdir("d:/logs");
	connect(&log_timer, &QTimer::timeout, this, &MBK02_widg::log_timer_ontimer);
	log_timer.start(200);

	//msg_to_log("рпп");
	//paint_buttons();
	current_lit = 0;
	for (int i = 0; i <= 3; i++)
	{
		tmp_list.push_back(LITER_PAUSE + (STEP * current_lit));
		tmp_list.push_back(LITER_PAUSE + (STEP * current_lit) + ONE);
		tmp_list.push_back(LITER_PAUSE + (STEP * current_lit));
		tmp_list.push_back(LITER_PAUSE + (STEP * current_lit) + ZERO);
	}
		tmp_list.push_back(LITER_PAUSE + (STEP * 13) + ONE);
		tmp_list.push_back(LITER_PAUSE + (ONE * 13) + ZERO);
	new_KPI(tmp_list);
	set_new_tm();
}

void MBK02_widg::new_mk(int mshm, int pshm, int length_m, int length_p, double u_m, double u_p, int dt, int line_m, int line_p)
{
	//	QString _msg = QString("%1 принял МК МШ%2 ПШ%3").arg(QTime::currentTime().toString("hh:mm:ss.zzz")).arg(mshm).arg(pshm);
	//	msg_to_log(_msg);
	////Странные штуки
	//	int tmp_mshm = mshm;
	//	int tmp_pshm = pshm - 8;
	//
	//	switch (tmp_mshm)
	//	{
	//	case 0:
	//		current_FSMU = FSMU_numbB(tmp_pshm);
	//		break;
	//	case 1:
	//		current_stab = STAB(tmp_pshm);
	//		break;
	//	case 2:
	//		current_FSVU = FSVU_numbB(tmp_pshm); 
	//		break;
	//	case 3:
	//		current_antenna = ANTENNA(tmp_pshm); 
	//		break;
	//	}
	//	write_words();
	//	paint_buttons();
	//	set_new_tm();
}

void MBK02_widg::new_KPI(QVariantList KPI_list)
{
	int tmp_in,tmp_len;
	tmp_len = KPI_list.length();
	bool ch_cur_lit = true;
	//for (QVariantList::iterator itr = KPI_list.begin(); itr != KPI_list.end(); itr++)
	//{
	//	tmp_in = (*itr).toInt();
	//}
	for (int it_lit = 1; KPI_list.at(0).toInt() - (STEP*it_lit) != LITER_PAUSE; it_lit++)
	{
		if (current_lit = 0)
		{
			msg_to_log("Литера не задана, либо задана нулевая литера");
			ch_cur_lit = false;
			break;
		}
		if ((it_lit == current_lit) && (KPI_list.at(0).toInt() - (STEP*it_lit) != LITER_PAUSE))
		{
			msg_to_log("Некорректная литера");
			ch_cur_lit = false;
			break;
		}
	}
	if (ch_cur_lit)
	{
		for (int i = 0; i < tmp_len; i++)
		{
			tmp_in = ((KPI_list.at(i).toInt() - LITER_PAUSE) - (STEP * current_lit));
			if ((tmp_in >= -3) && (tmp_in <= 3)) tmp_str_KPI += "P";
			else
			{
				if ((tmp_in >= ONE - 6) && (tmp_in <= ONE + 6)) tmp_str_KPI += "1";
				else
				{
					if ((tmp_in >= ZERO - 6) && (tmp_in <= ZERO + 6)) tmp_str_KPI += "0";
					else tmp_str_KPI += " ERR ";
				}
			}
		}
		msg_to_log(tmp_str_KPI);
	}
}


void MBK02_widg::msg_to_log(const QString& _msg)
{
	{
		QMutexLocker lock(&log_mutex);
		log_buffer << _msg;
	}
	_cursor->insertText(_msg + "\n");
	if (auto_scroll)
		_scroll_bar->setValue(_scroll_bar->maximum());
}

void MBK02_widg::auto_scroll_clicked(int _state)
{
	//auto_scroll = (_state != 0);
}

void MBK02_widg::log_timer_ontimer()
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

void MBK02_widg::new_message(QVariant dt, int mko, int line, int cwd, QVariantList words, int os)
{
	MKOWord tmp_cwd;
	tmp_cwd.com_word = cwd;
	if (os == -1)
		return;
	if ((mko == MKO) && (tmp_cwd.adr == adr))
	{
		//QString _msg = QString("%1 принял сигнал на подадресе %2 c КС %3").arg(QTime::currentTime().toString("hh:mm:ss.zzz")).arg(tmp_cwd.subadr).arg(tmp_cwd.com_word);
		//msg_to_log(_msg);

		int tmp_word = words[0].toInt();
		switch (tmp_cwd.subadr)
		{
		case 2:
			unsigned char first_byte;
			first_byte = tmp_word & 0xFF;
			unsigned char second_byte;
			second_byte = (tmp_word & 0xFF00) >> 8;
			if (first_byte == second_byte)
			{
				switch (first_byte)
				{
				case 0x20: current_lit = 1; break;
				case 0x28: current_lit = 2; break;
				case 0x30: current_lit = 3; break;
				case 0x38: current_lit = 4; break;
				case 0x40: current_lit = 5; break;
				case 0x48: current_lit = 6; break;
				case 0x50: current_lit = 7; break;
				case 0x58: current_lit = 8; break;
				}
			}
			break;
		case 3:
			if (tmp_word == 0)
			{
				switch (current_chan)
				{
				case CHANEL_1:
					if (current_ant == MHA1MY)
						current_ant = MHA1PY;
					else
						current_ant = MHA1MY;
					break;
				case CHANEL_2:
					if (current_ant == MHA2MY)
						current_ant = MHA2PY;
					else
						current_ant = MHA2MY;
					break;
				}
				break;
			}
		case 29:
			char chan_chk;
			chan_chk = tmp_word & 7;
			if (current_chan != CHANEL(chan_chk) - 1)
			{
				switch (CHANEL(chan_chk) - 1)
				{
				case CHANEL_1: current_ant = MHA1MY; break;
				case CHANEL_2: current_ant = MHA2MY; break;
				}
			}
			current_chan = CHANEL(chan_chk - 1);
			break;
		};
		write_words();
		set_new_tm();
		paint_buttons();
	}
}
void MBK02_widg::write_words()
{
	Lit_le->setText(QString::number(current_lit));
}
void MBK02_widg::paint_buttons()
{
	if ((current_chan != CHANEL_OFF) || (current_chan != CHANEL_ERR))
	{
		switch (current_chan)
		{
		case CHANEL_1:
			Chan1_pbut->setStyleSheet("background-color: rgb(142, 198, 156);");
			Chan2_pbut->setStyleSheet("background-color: rgb(204, 204, 204);");
			break;
		case CHANEL_2:
			Chan1_pbut->setStyleSheet("background-color: rgb(204, 204, 204);");
			Chan2_pbut->setStyleSheet("background-color: rgb(142, 198, 156);");
			break;
		}
		if (current_chan == CHANEL_OFF)
		{
			Chan1_pbut->setStyleSheet("background-color: rgb(204, 204, 204);");
			Chan2_pbut->setStyleSheet("background-color: rgb(204, 204, 204);");
		}
	}
	if (current_ant != MHAOFF)
	{
		Ant_pbut->setStyleSheet("background-color: rgb(142, 198, 156);");
		Ant_pbut->setText(ant_names[ANTENNA(current_ant)]);
	}
	else
	{
		Ant_pbut->setText("");
		Ant_pbut->setStyleSheet("background-color: rgb(204, 204, 204);");
	}
}

void MBK02_widg::set_new_tm()
{

}
