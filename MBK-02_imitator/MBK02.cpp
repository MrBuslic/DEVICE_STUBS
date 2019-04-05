#include "MBK02.h"
#include "rpc_ports.h"
#include <QMessageBox>
#include "MBK02_socket_rpc.h"

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

	Sig_pbut = new QPushButton("Отстутствует", this);
	Sig_pbut->setMaximumWidth(165);

	QLabel* Lit_lb = new QLabel("Литера");
	QLabel* Ant_lb = new QLabel("Антенна");

	Signal_gbox = new QGroupBox("Сигнал", this);
	Channels_gbox = new QGroupBox("Каналы", this);
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
	All_glay->addLayout(Ant_vblay, 0, 0);
	All_glay->addLayout(Lit_vblay, 0, 1);

	Chan_hblay->addWidget(Chan1_pbut);
	Chan_hblay->addWidget(Chan2_pbut);
	Sig_hblay->addWidget(Sig_pbut);
	Ant_vblay->addWidget(Ant_lb, 0, Qt::AlignHCenter);
	Ant_vblay->addWidget(Ant_pbut);
	Lit_vblay->addWidget(Lit_lb, 0, Qt::AlignHCenter);
	Lit_vblay->addWidget(Lit_le);

	Sig_pbut->setStyleSheet("background-color: rgb(204, 204, 204);");
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
	//slot_thr.set_connection_params("127.0.0.1", OMNIBUS_SLOT);
	//slot_thr.start(); // вот тут падает

	//signal_thr.set_connection_params("127.0.0.1", OMNIBUS_SIGNAL);
	//signal_thr.start(); // вот тут падает

	//if (!slot_thr.wait_connected(3) || !signal_thr.wait_connected(3))
	//{
	//	QMessageBox::critical(0, "Нет соединения", "Ошибка соединения с rpc_omnibus");
	//	this->deleteLater();
	//	return;
	//}
	//slot_thr.get_omnibus_obj()->switch_ab(MKO, adr, true);
	//flag = true;

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

	QString ip_str = "127.0.0.1";
	int slot_port = MBK02_SLOT;
	int signal_port = MBK02_SIGNAL;
	Socket_RPC_SLOT_Server_Thread* rpc_slot_srv = new Socket_RPC_SLOT_Server_Thread;
	rpc_slot_srv->set_app(this);
	rpc_slot_srv->set_params(ip_str, slot_port);
	rpc_slot_srv->start();
	Socket_RPC_SIGNAL_Thread* rpc_signal_srv = new Socket_RPC_SIGNAL_Thread;
	rpc_signal_srv->set_app(this);
	rpc_signal_srv->set_params(ip_str, signal_port);
	rpc_signal_srv->start();

	connect(mku_signal_thr.get_obj().get(), SIGNAL(new_mk(int, int, int, int, double, double, int, int, int)), this, SLOT(new_mk(int, int, int, int, double, double, int, int, int)));
	connect(kpi_signal_thr.get_obj().get(), SIGNAL(new_KPI(QVariantList)), this, SLOT(new_KPI(QVariantList)));
	//connect(signal_thr.get_obj().get(), SIGNAL(new_message(QVariant, int, int, int, QVariantList, int)), this, SLOT(new_message(QVariant, int, int, int, QVariantList, int)));
	//connect(this, &MBK02_widg::set_new_tm, this, &MBK02_widg::update_tm);

	log_filename = QString("d:/logs/%1_%2.log").arg(QCoreApplication::applicationName()).arg(QDateTime::currentDateTime().toString("yyyy.MM.dd_hh.mm.ss"));
	QDir dir("d:/logs");
	if (!dir.exists())
		QDir().mkdir("d:/logs");
	connect(&log_timer, &QTimer::timeout, this, &MBK02_widg::log_timer_ontimer);
	log_timer.start(200);

	t_ant_ch = new QTimer(this);
	connect(t_ant_ch, &QTimer::timeout, this, &MBK02_widg::reverse_ant);
	t_ant_ch->start(5000);
	t_sleep = new QTimer(this);
	//connect(t_sleep, &QTimer::timeout, this, &BECH_widg::BECH_interrupt_setup);
	t_sleep->start(420000);
	t_err_kpi = new QTimer(this);
	t_err_kpi->setSingleShot(true);
	connect(t_err_kpi, &QTimer::timeout, this, &MBK02_widg::lose_cont);
	current_chan = CHANEL_1;
	current_ant = MHA1MY;
	update_graphics();
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


/*
МНА2+Y
МНА1-Y
МНА1+Y
МНА2-Y
*/
void MBK02_widg::new_KPI(QVariantList KPI_list)
{
	current_lit = 2;
	int tmp_in, tmp_len, tmp_weak;
	QString tmp_str_ant;
	t_ant_ch->stop();
	tmp_len = KPI_list.length();
	if (current_lit == 0)
	{
		msg_to_log("Литера не задана, либо задана нулевая литера");
		return;
	}
	QString tmp_str_KPI;
	bool tmp_correct = true;
	int tmp_p = LITER_PAUSE + (STEP * (current_lit - 1));
	int tmp_0 = LITER_PAUSE + (STEP * (current_lit - 1)) + ZERO;
	int tmp_1 = LITER_PAUSE + (STEP * (current_lit - 1)) + ONE;
	for (int i = 0; (i < tmp_len) && (!t_ant_ch->isActive()); i++)
	{
		tmp_list = KPI_list.at(i).toList();
		tmp_in = tmp_list.at(0).toInt();
		tmp_str_ant = tmp_list.at(1).toString();
		tmp_weak = tmp_list.at(2).toInt();
		tmp_correct = true;
		switch (current_ant)
		{
		case MHA1MY:
			if (tmp_str_ant != "МНА1-Y")
			{
				t_err_kpi->start(5000);
				tmp_correct = false;
			}
			break;
		case MHA1PY:
			if (tmp_str_ant != "МНА1+Y")
			{
				t_err_kpi->start(5000);
				tmp_correct = false;
			}
			break;
		case MHA2MY:
			if (tmp_str_ant != "МНА2-Y")
			{
				t_err_kpi->start(5000);
				tmp_correct = false;
			}
			break;
		case MHA2PY:
			if (tmp_str_ant != "МНА2+Y")
			{
				t_err_kpi->start(5000);
				tmp_correct = false;
			}
			break;
		default:
			t_err_kpi->start(5000);
			tmp_correct = false;
			break;
		}
		if (tmp_correct)
		{
			if ((tmp_in >= (tmp_p - 7)) && (tmp_in <= (tmp_p + 7))) tmp_str_KPI += "P";
			else
			{
				if ((tmp_in >= tmp_1 - 7) && (tmp_in <= tmp_1 + 7)) tmp_str_KPI += "1";
				else
				{
					if ((tmp_in >= tmp_0 - 7) && (tmp_in <= tmp_0 + 7)) tmp_str_KPI += "0";
					//else tmp_str_KPI += " ERR ";
					else tmp_correct = false;
				}
			}
			if ((tmp_weak < 50) || (tmp_weak > 100))
				tmp_correct = false;
			if (tmp_correct)
			{
				signal_con = true;
				char tmp_l = tmp_str_KPI.toInt();
				if (tmp_str_KPI != "P")
					list_to_R14732.push_back(tmp_l);
				if (t_err_kpi->isActive() || (i == tmp_len)) t_err_kpi->stop();
				update_graphics();
			}
			else t_err_kpi->start(5000);
		}
	}
	msg_to_log(tmp_str_KPI);
	if (tmp_correct)
	{
		update_tm(26);
		emit msg_to_14R732(list_to_R14732);
	}
}

void MBK02_widg::reverse_ant()
{
	if (current_ant != MHAOFF)
	{
		switch (current_ant)
		{
		case MHA1MY:current_ant = MHA1PY; break;
		case MHA1PY:current_ant = MHA1MY; break;
		case MHA2MY:current_ant = MHA2PY; break;
		case MHA2PY:current_ant = MHA2MY; break;
		}
		update_graphics();
		update_tm(1);
	}
}
void MBK02_widg::lose_cont()
{
	signal_con = false;
	t_ant_ch->start(5000);
	list_to_R14732.clear();
	msg_to_log("Потеряно соединение\n");
	update_tm(26);
	update_graphics();
}


void MBK02_widg::update_tm(int sadr)
{
	int _sadr = sadr;
	unsigned short word = 0;
	switch (_sadr)
	{
	case 1:
		if (signal_con)	word += current_chan + 1;
		switch (current_ant)
		{
		case MHA1MY: word += 4; break;
		case MHA1PY:break;
		case MHA2MY: word += 8; break;
		case MHA2PY:break;
		}
		word += (word << 8);
		break;
	case 4:
		switch (current_lit)
		{
		case 1: word += 0x20; break;
		case 2: word += 0x28; break;
		case 3: word += 0x30; break;
		case 4: word += 0x38; break;
		case 5: word += 0x40; break;
		case 6: word += 0x48; break;
		case 7: word += 0x50; break;
		case 8: word += 0x58; break;
		}
		word += (word << 8);
		break;
	case 26://Измерения в Вольтах
		; break;
	}
	emit set_new_tm(_sadr, word);
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
			update_tm(4);
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
				update_tm(1);
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
			update_tm(1);
			break;
		};
		update_graphics();
	}
}
void MBK02_widg::update_graphics()
{
	if ((current_lit > 0) && (current_lit < 9))
		Lit_le->setText(QString::number(current_lit));
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
	if (signal_con)
	{
		Sig_pbut->setStyleSheet("background-color: rgb(142, 198, 156);");
		Sig_pbut->setText("Присутствует");
	}
	else
	{
		Sig_pbut->setStyleSheet("background-color: rgb(204, 204, 204);");
		Sig_pbut->setText("Остутствует");
	}
}

