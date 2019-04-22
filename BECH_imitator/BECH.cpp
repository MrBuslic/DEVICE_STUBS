#include "BECH.h"

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

BECH_widg::BECH_widg(QWidget *parent)
{
	widg = new QWidget(this);
	setWindowTitle("БЭЧ");

	mode_names.insert(0, "ПИ-15");
	mode_names.insert(1, "ПИ-8");
	mode_names.insert(2, "");

	OG_start_warm.insert(OG_1, 0);
	OG_start_warm.insert(OG_2, 0);
	OG_start_warm.insert(OG_3, 0);

	OG_finish_warm.insert(OG_1, 0);
	OG_finish_warm.insert(OG_2, 0);
	OG_finish_warm.insert(OG_3, 0);

	ready_og = false;
	tm_towarm = standart_tm;
	cooling_cof = 4;

	for (int i = 0; i < 3; i++)
	{
		QString numb = QString::number(i + 1);
		OG_pbut << new QPushButton(numb, this);
		FINIK_pbut << new QPushButton(numb, this);
		OG_pbut[i]->setStyleSheet("background-color: rgb(204, 204, 204);");
		FINIK_pbut[i]->setStyleSheet("background-color: rgb(204, 204, 204);");
		if (i < 2)
		{
			QString numb = QString::number(i + 1);
			LKA_pbut << new QPushButton(numb, this);
			KP_pbut << new QPushButton(numb, this);
			LKA_pbut[i]->setStyleSheet("background-color: rgb(204, 204, 204);");
			KP_pbut[i]->setStyleSheet("background-color: rgb(204, 204, 204);");
		}
	}
	LKA_gb = new QGroupBox("ЛКА", this);
	OG_gb = new QGroupBox("ОГ", this);
	FINIK_gb = new QGroupBox("ФИНИК", this);
	KP_gb = new QGroupBox("КП", this);

	All_vblay = new QVBoxLayout(this);
	LKA_hlay = new QHBoxLayout(LKA_gb);
	OG_hlay = new QHBoxLayout(OG_gb);
	FINIK_hlay = new QHBoxLayout(FINIK_gb);
	KP_hlay = new QHBoxLayout(KP_gb);
	FINIK_REZH_hlay = new QHBoxLayout();

	for (int i = 0; i < 3; i++)
	{
		OG_hlay->addWidget(OG_pbut[i]);
		FINIK_hlay->addWidget(FINIK_pbut[i]);
		if (i < 2)
		{
			LKA_hlay->addWidget(LKA_pbut[i]);
			KP_hlay->addWidget(KP_pbut[i]);
		}
	}

	LKA_gb->setAlignment(Qt::AlignHCenter);
	FINIK_gb->setAlignment(Qt::AlignHCenter);;
	KP_gb->setAlignment(Qt::AlignHCenter);;
	OG_gb->setAlignment(Qt::AlignHCenter);;

	QLabel* Mode_lb = new QLabel("Режим");

	Mode_le = new QLineEdit(this);
	Mode_le->setReadOnly(true);

	FINIK_REZH_hlay->addWidget(Mode_lb);
	FINIK_REZH_hlay->addWidget(Mode_le);

	All_vblay->addWidget(OG_gb);
	All_vblay->addWidget(FINIK_gb);
	All_vblay->addWidget(LKA_gb);
	All_vblay->addWidget(KP_gb);
	All_vblay->addLayout(FINIK_REZH_hlay);

	inter_tmr = new QTimer(this);
	connect(inter_tmr, &QTimer::timeout, this, &BECH_widg::BECH_interrupt_setup);

	warm_og_tmr = new QTimer(this);
	warm_og_tmr->setSingleShot(true);
	connect(warm_og_tmr, &QTimer::timeout, this, &BECH_widg::set_warm_og);

	AbOn_tmr = new QTimer(this);
	AbOn_tmr->setSingleShot(true);
	connect(AbOn_tmr, &QTimer::timeout, this, &BECH_widg::omni_connect);

	Power_tmr = new QTimer(this);
	Power_tmr->setSingleShot(true);
	connect(Power_tmr, &QTimer::timeout, this, &BECH_widg::set_change_power);

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
		QMessageBox::critical(0, "Нет соединения", "Ошибка соединения с lka05");
		this->deleteLater();
		return;
	}

	interrupt_slot_thr.set_connection_params("127.0.0.1", INTERRUPTS_SLOT);
	interrupt_slot_thr.start(); // вот тут падает

	interrupt_signal_thr.set_connection_params("127.0.0.1", INTERRUPTS_SIGNAL);
	interrupt_signal_thr.start(); // вот тут падает

	if (!interrupt_slot_thr.wait_connected(3) || !interrupt_signal_thr.wait_connected(3))
	{
		QMessageBox::critical(0, "Нет соединения", "Ошибка соединения с interrupt_bus в Foi");
		this->deleteLater();
		return;
	}

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

	connect(signal_thr.get_obj().get(), SIGNAL(new_message(QVariant, int, int, int, QVariantList, int)), this, SLOT(new_message(QVariant, int, int, int, QVariantList, int)));

	slot_thr.get_omnibus_obj()->switch_ab(MKO, adr, false);
	flag = false;

	connect(mku_signal_thr.get_obj().get(), SIGNAL(new_mk(int, int, int, int, double, double, int, int, int)), this, SLOT(new_mk(int, int, int, int, double, double, int, int, int)));
	connect(power_signal_thr.get_obj().get(), SIGNAL(u_on_k1(double)), this, SLOT(get_power(double)));

	log_filename = QString("d:/logs/%1_%2.log").arg(QCoreApplication::applicationName()).arg(QDateTime::currentDateTime().toString("yyyy.MM.dd_hh.mm.ss"));
	QDir dir("d:/logs");
	if (!dir.exists())
		QDir().mkdir("d:/logs");
	connect(&log_timer, &QTimer::timeout, this, &BECH_widg::log_timer_ontimer);
	log_timer.start(200);

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
	connect(auto_scroll_box, &QCheckBox::stateChanged, this, &BECH_widg::auto_scroll_clicked);
	All_vblay->addWidget(edit);
	All_vblay->addWidget(auto_scroll_box);

}

void BECH_widg::BECH_interrupt_setup()
{
	n = 5;
	chan = 3;
	u = 5;
	t = 4.5;
	BECH_interrupt_run();
}

void BECH_widg::BECH_interrupt_run()
{
	interrupt_slot_thr.get_interrupt_bus_obj()->make_interrupt(n, chan, u, t);
}

void BECH_widg::get_power(double _volt)
{
	volt = volt;
	if (volt >= 20.0)
		imit_on();
	else
		if (volt == 0) imit_off();
}

void BECH_widg::set_power_back()
{
	double curr;
	if (power_vt != 0)
	{
		curr = (double)power_vt / volt;
	}
	else
		curr = 0;
	power_slot_thr.get_power_bus_obj()->set_i(bus, name, curr);
}

void BECH_widg::imit_off()
{
	msg_to_log("Питание отключено");
	current_OG = OG_ERR;
	current_FINIK = FINIK_ERR;
	current_FINIK_REZH = FINIK_REZH_ERR;
	current_KP = KP_OFF;
	current_LKA = LKA_OFF;
	slot_thr.get_omnibus_obj()->switch_ab(MKO, adr, false);
	flag = false;
	update_graphics();
	inter_tmr->stop();
}

void BECH_widg::imit_on()
{
	msg_to_log("Питание включено");
	current_OG = OG_1;
	current_FINIK = FINIK_1;
	current_FINIK_REZH = FINIK_REZH_PI8;
	AbOn_tmr->start(5000);
	change_power(true);
	update_graphics();
	update_time();
	set_new_tm();
	inter_tmr->start(1000);
}

void BECH_widg::set_change_power()
{
	change_power(false);
}

void BECH_widg::change_power(bool switch_og)
{
	if (switch_og)//генератор переключился?
	{
		power_vt = 60;
		Power_tmr->start(tm_towarm);
		set_power_back();
	}
	else
	{
		switch (power_vt)
		{
		case 0:
			power_vt = 60;
			Power_tmr->start(tm_towarm);
			set_power_back();
			break;
		case 60:
			power_vt = 35;
			set_power_back();
			break;
		case 35:
			power_vt = 60;
			Power_tmr->start(tm_towarm);
			set_power_back();
			break;
		}
	}
	switch_og = false;
}

void BECH_widg::new_mk(int mshm, int pshm, int length_m, int length_p, double u_m, double u_p, int dt, int line_m, int line_p)
{
	if (volt != 0)
	{
		int uu = 0;
		//	QString _msg = QString("%1 принял МК МШ%2 ПШ%3").arg(QTime::currentTime().toString("hh:mm:ss.zzz")).arg(mshm).arg(pshm);
		//	msg_to_log(_msg);
		int tmp_mshm = mshm;
		int tmp_pshm = pshm;

		switch (tmp_mshm)
		{
		case 3:
			current_LKA = LKA_1;
			break;
		case 4:
			current_LKA = LKA_2;
			break;
		case 8:
			current_KP = KP_1;
			break;
		case 9:
			current_KP = KP_2;
			break;
		}
		update_graphics();
		set_new_tm();
	}
}


void BECH_widg::msg_to_log(const QString& _msg)
{
	{
		QMutexLocker lock(&log_mutex);
		log_buffer << _msg;
	}
	_cursor->insertText(_msg + "\n");
	if (auto_scroll)
		_scroll_bar->setValue(_scroll_bar->maximum());
}

void BECH_widg::auto_scroll_clicked(int _state)
{
	auto_scroll = (_state != 0);
}

void BECH_widg::log_timer_ontimer()
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

void BECH_widg::new_message(QVariant dt, int mko, int line, int cwd, QVariantList words, int os)
{
	MKOWord tmp_cwd;
	tmp_cwd.com_word = cwd;
	if (os == -1)
		return;
	if ((mko == MKO) && (tmp_cwd.adr == adr))
	{
		//	QString _msg = QString("%1 принял сигнал на подадресе %2 c КС %3").arg(QTime::currentTime().toString("hh:mm:ss.zzz")).arg(tmp_cwd.subadr).arg(tmp_cwd.com_word);
		//	msg_to_log(_msg);

		int tmp_word = words[0].toInt();

		char reset = tmp_word & 0x1F;
		if (reset != 15)
		{
			char finik_rezh_ch = tmp_word & 0x30;
			char tmp = tmp_word & 3;
			if ((tmp_word & 3) > 0)
			{
				if (current_OG != OG((tmp_word & 3) - 1))
				{
					if (warm_og_tmr->isActive())
						warm_og_tmr->stop();
					set_warm_og();
					current_OG = OG((tmp_word & 3) - 1);
					update_time();
					change_power(true);
				}
			}
			if ((tmp_word & 0xC) >> 2 > 0)
				current_FINIK = FINIK(((tmp_word & 0xC) >> 2) - 1);
			switch (finik_rezh_ch)
			{
			case 16:
				current_FINIK_REZH = FINIK_REZH_PI8;
				break;
			case 32:
				current_FINIK_REZH = FINIK_REZH_PI15;
				break;
			}
		}
		else
		{
			current_OG = OG_1;
			current_FINIK = FINIK_1;
			current_FINIK_REZH = FINIK_REZH_PI8;
		}

		update_graphics();
		set_new_tm();
	}
}

void BECH_widg::set_warm_og()
{
	ready_og = false;
	qint64 msecs_time = (QDateTime::currentMSecsSinceEpoch());
	OG_finish_warm[current_OG] = msecs_time;
	if (OG_start_warm[current_OG] == 0) OG_start_warm[current_OG] = OG_finish_warm[current_OG] - standart_tm;
	if (((OG_finish_warm[current_OG] - OG_start_warm[current_OG]) >= (standart_tm - warm_er)) || ((OG_finish_warm[current_OG] - OG_start_warm[current_OG]) >= (standart_tm + warm_er)))
	{
		ready_og = true;
		msg_to_log("Прогрелся ОГ № " + QString::number(current_OG + 1));
		set_new_tm();
	}
	else
	{
		msg_to_log("Прогревание ОГ № " + QString::number(current_OG + 1) + " прервано");
	}
}

void BECH_widg::update_time()
{
	ready_og = false;
	qint64 msecs_time = (QDateTime::currentMSecsSinceEpoch());
	if (OG_finish_warm[current_OG] == 0) // Если ОГ не нагревался вообще
	{
		tm_towarm = standart_tm;
		OG_start_warm[current_OG] = msecs_time;
		OG_finish_warm[current_OG] = msecs_time + tm_towarm;
	}
	else // Если нагревался
	{
		// Если ОГ нагрелся полностью
		if ((OG_finish_warm[current_OG] - OG_start_warm[current_OG] >= (standart_tm - warm_er)) || (OG_finish_warm[current_OG] - OG_start_warm[current_OG] >= (standart_tm - warm_er)))
		{
			if (msecs_time - OG_finish_warm[current_OG] >= (standart_tm * cooling_cof)) // Если после полного нагрева ОГ прошло достаточно времени, чтобы тот полностью охладился
			{
				tm_towarm = standart_tm;
				OG_start_warm[current_OG] = msecs_time;
				OG_finish_warm[current_OG] = msecs_time + standart_tm;
			}
			else // Если не прошло достаточно времени
			{
				//Время охлаждения это текущее время - время остановки ОГ  
				//Время нулевого прогрева, при ОГ - полностью был нагрет = Текущее время - (стандарт нагрева - время охлаждения / коэфициент охлаждения)
				OG_start_warm[current_OG] = msecs_time - (standart_tm - (msecs_time - OG_finish_warm[current_OG]) / cooling_cof);
				OG_finish_warm[current_OG] = OG_start_warm[current_OG] + standart_tm;
				tm_towarm = OG_finish_warm[current_OG] - msecs_time;
			}
		}
		else // Если нагрелся не полностью
		{
			if (msecs_time < OG_finish_warm[current_OG]) // Если после частичного нагрева ОГ прошло достаточно времени, чтобы тот полностью охладился
			{
				tm_towarm = standart_tm;
				OG_start_warm[current_OG] = msecs_time;
				OG_finish_warm[current_OG] = msecs_time + standart_tm;
			}
			else
			{
				//Время нулевого прогрева, при ОГ - частично прогрет = Текущее время - ((время конца прогревания - время начала прогревания) - время охлаждения / коэфициент охлаждения) 
				OG_start_warm[current_OG] = msecs_time - ((OG_finish_warm[current_OG] - OG_start_warm[current_OG]) - (msecs_time - OG_finish_warm[current_OG]) / cooling_cof);
				OG_finish_warm[current_OG] = OG_start_warm[current_OG] + standart_tm;
				tm_towarm = OG_finish_warm[current_OG] - msecs_time;
			}
		}
	}
	if (tm_towarm != 0)
		warm_og_tmr->start(tm_towarm);
}

void BECH_widg::omni_connect()
{
	slot_thr.get_omnibus_obj()->switch_ab(MKO, adr, true);
	flag = true;
	set_new_tm();
}

void BECH_widg::update_graphics()
{
	if (mode_names.contains(current_FINIK_REZH))
		Mode_le->setText(mode_names[current_FINIK_REZH]);
	for (int i = 0; i < 3; i++)
	{
		OG_pbut[i]->setStyleSheet("background-color: rgb(204, 204, 204);");
		if (current_OG == i)
			OG_pbut[i]->setStyleSheet("background-color: rgb(142, 198, 156);");

		FINIK_pbut[i]->setStyleSheet("background-color: rgb(204, 204, 204);");
		if (current_FINIK == i)
			FINIK_pbut[i]->setStyleSheet("background-color: rgb(142, 198, 156);");

		if (i < 2)
		{
			LKA_pbut[i]->setStyleSheet("background-color: rgb(204, 204, 204);");
			if (current_LKA == i)
				LKA_pbut[i]->setStyleSheet("background-color: rgb(142, 198, 156);");
			KP_pbut[i]->setStyleSheet("background-color: rgb(204, 204, 204);");
			if (current_KP == i)
				KP_pbut[i]->setStyleSheet("background-color: rgb(142, 198, 156);");
		}
	}
}

void BECH_widg::set_new_tm()
{
	unsigned short _word = 0;
	QVariantList tmp_list;
	switch (current_OG)
	{
	case OG_1:
		_word = 1;
		break;
	case OG_2:
		_word = 2;
		break;
	case OG_3:
		_word = 4;
		break;
	}
	switch (current_FINIK)
	{
	case FINIK_1:
		_word += 8;
		break;
	case FINIK_2:
		_word += 0x10;
		break;
	case FINIK_3:
		_word += 0x20;
		break;
	}
	if (current_LKA != LKA_OFF)
	{
		if (current_LKA != LKA_2) _word += 0x40;
	}
	else msg_to_log("Ошибка ОК ЛКАБ");
	if (current_KP != KP_OFF)
	{
		if (current_KP != KP_2) _word += 0x80;
	}
	else msg_to_log("Ошибка ОК КП");
	///Норма сигналов 1с на выходе 1,2 и норма сигналов 5 МГц на выходе 1-4
	///Норма сигнала 1с на выходе 1
	_word += 0x100;
	///Норма сигнала 1с на выходе 2
	_word += 0x200;
	///Норма сигнала 5 МГц на выходе 1
	_word += 0x400;
	///Норма сигнала 5 МГц на выходе 2
	_word += 0x800;
	///Норма сигнала 5 МГц на выходе 3
	_word += 0x1000;
	///Норма сигнала 5 МГц на выходе 4
	_word += 0x2000;
	if (current_FINIK != FINIK_ERR) _word += 0x4000;
	if (ready_og) _word += 0x8000;
	tmp_list.push_back(_word);
	slot_thr.get_omnibus_obj()->set_new_data(MKO, adr, 1, tmp_list);
}

BECH_widg::~BECH_widg()
{
	slot_thr.quit();
	signal_thr.quit();

	mku_slot_thr.quit();
	mku_signal_thr.quit();

	interrupt_slot_thr.quit();
	interrupt_signal_thr.quit();

	power_slot_thr.quit();
	power_signal_thr.quit();
}