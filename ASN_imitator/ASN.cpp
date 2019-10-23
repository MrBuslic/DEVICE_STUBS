#include "ASN.h"
#include "rpc_ports.h"
union MKOWord
{
	quint16 com_word;	// командное слово целиком

	struct
	{
		quint16 count_word : 5,	// число сл.данных (команда)
			subadr : 5,		// подадрес
			trans_dir : 1,	// направление передачи(1-чт.ќ”)
			adr : 5;		// адрес ОУ
	};
};

ASN_widg::ASN_widg(QWidget *parent) : QWidget(parent), sett(QCoreApplication::applicationDirPath() + "/asn.ini", QSettings::IniFormat), a(true)
{
	//io = new QPushButton(QString("Включить"));
	first_half = new QPushButton(QString("Первый комплект"));
	first_half->setStyleSheet("background-color: rgb(204, 204, 204);");
	second_half = new QPushButton(QString("Второй комплект"));
	second_half->setStyleSheet("background-color: rgb(204, 204, 204);");
	nw = new QPushButton(QString("Штатная работа"));
	ts = new QPushButton(QString("Поверочное включение"));
	jm_glonass = new QPushButton(QString("ГЛОНАСС"));
	jm_gps = new QPushButton(QString("GPS"));
	jm_gg = new QPushButton(QString("ГЛОНАСС + GPS"));
	n_hrg = new QPushButton(QString("Отказ от прогрева ОГ"));
	zacep = new QPushButton(QString("Зацепиться"));
	EnableButton();
	QHBoxLayout *halves_lay = new QHBoxLayout;
	halves_lay->addWidget(first_half);
	halves_lay->addWidget(second_half);

	QHBoxLayout *hBoxLayout = new QHBoxLayout;
	hBoxLayout->addWidget(jm_glonass);
	hBoxLayout->addWidget(jm_gps);
	hBoxLayout->addWidget(jm_gg);
	
	QGridLayout *gridLayout = new QGridLayout;
	gridLayout->addWidget(nw, 0, 0);
	gridLayout->addWidget(ts, 1, 0);	
	gridLayout->addWidget(n_hrg, 0, 1);
	gridLayout->addWidget(zacep, 1, 1);
	
	//connect(io, SIGNAL(clicked()), this, SLOT(io_Clicked()));
	connect(nw, SIGNAL(clicked()), this, SLOT(nw_Clicked()));
	connect(ts, SIGNAL(clicked()), this, SLOT(ts_Clicked()));
	connect(jm_glonass, SIGNAL(clicked()), this, SLOT(jm_glonass_Clicked()));
	connect(jm_gps, SIGNAL(clicked()), this, SLOT(jm_gps_Clicked()));
	connect(jm_gg, SIGNAL(clicked()), this, SLOT(jm_gg_Clicked()));
	connect(n_hrg, SIGNAL(clicked()), this, SLOT(n_hrg_Clicked()));
	connect(zacep, SIGNAL(clicked()), this, SLOT(zacep_Clicked()));
	
	test = true;
	OG = false;
	flag_on = false;
	radio = false;
	dost = false;
	nav_SYS = NAV_SYS_GG;

	text = new QTextEdit();
	text->setReadOnly(true);

	heat_og_tmr = new QTimer(this);
	heat_og_tmr->setSingleShot(true);
	connect(heat_og_tmr, &QTimer::timeout, this, &ASN_widg::heating_OG);
	prer = new QTimer(this);
	connect(prer, SIGNAL(timeout()), this, SLOT(set_new_tm_2()));

	load_fact();

	slot_thr.set_connection_params("127.0.0.1", OMNIBUS_SLOT);
	slot_thr.start();

	signal_thr.set_connection_params("127.0.0.1", OMNIBUS_SIGNAL);
	signal_thr.start();

	if (!slot_thr.wait_connected(3) || !signal_thr.wait_connected(3))
	{
		QMessageBox::critical(0, "Нет соединения", "Ошибка соединения с rpc_omnibus");
		this->deleteLater();
		return;
	}

	mku_slot_thr.set_connection_params("127.0.0.1", MKU_SLOT);
	mku_slot_thr.start();

	mku_signal_thr.set_connection_params("127.0.0.1", MKU_SIGNAL);
	mku_signal_thr.start();

	if (!mku_slot_thr.wait_connected(3) || !mku_signal_thr.wait_connected(3))
	{
		QMessageBox::critical(0, "Нет соединения", "Ошибка соединения с lka05");
		this->deleteLater();
		return;
	}

	interrupt_slot_thr.set_connection_params("127.0.0.1", INTERRUPTS_SLOT);
	interrupt_slot_thr.start();

	interrupt_signal_thr.set_connection_params("127.0.0.1", INTERRUPTS_SIGNAL);
	interrupt_signal_thr.start();

	if (!interrupt_slot_thr.wait_connected(3) || !interrupt_signal_thr.wait_connected(3))
	{
		QMessageBox::critical(0, "Нет соединения", "Ошибка соединения с interrupt_bus в Foi");
		this->deleteLater();
		return;
	}

	power_slot_thr.set_connection_params("127.0.0.1", POWER_SLOT);
	power_slot_thr.start();

	power_signal_thr.set_connection_params("127.0.0.1", POWER_SIGNAL);
	power_signal_thr.start();

	if (!power_slot_thr.wait_connected(3) || !power_signal_thr.wait_connected(3))
	{
		QMessageBox::critical(0, "Нет соединения", "Ошибка соединения с power_bus");
		this->deleteLater();
		return;
	}

	connect(signal_thr.get_obj().get(), SIGNAL(new_message(QVariant, int, int, int, QVariantList, int)), this, SLOT(new_message(QVariant, int, int, int, QVariantList, int)));

	slot_thr.get_omnibus_obj()->switch_ab(MKO, adr, false);

	connect(mku_signal_thr.get_obj().get(), SIGNAL(new_mk(int, int, int, int, double, double, int, int, int)), this, SLOT(new_mk(int, int, int, int, double, double, int, int, int)));
	connect(power_signal_thr.get_obj().get(), SIGNAL(u_on_k1(double)), this, SLOT(get_power(double)));

	QVBoxLayout *main = new QVBoxLayout;
	main->addLayout(halves_lay);
	main->addLayout(gridLayout);
	main->addLayout(hBoxLayout);
	main->addWidget(text);

	setFixedSize(500, 350);
	setLayout(main);
	setWindowTitle(name);

	QSettings settings(QApplication::applicationDirPath() + "/positions.ini", QSettings::IniFormat);
	restoreGeometry(settings.value("asn_geometry").toByteArray());
}

void ASN_widg::nw_Clicked()
{
	text->append(QString("Штатная работа"));
	jm_glonass->setStyleSheet("black");
	jm_gps->setStyleSheet("black");
	jm_gg->setStyleSheet("black");
	jm_glonass->setEnabled(true);
	jm_gps->setEnabled(true);
	jm_gg->setEnabled(true);
	zacep->setEnabled(true);
	/*QList<QVariant> list;
	list << 3;
	MKOWord tmp_KS;
	tmp_KS.com_word = 0;
	tmp_KS.adr = 3;
	tmp_KS.subadr = 8;
	tmp_KS.count_word = 20;
	new_message(1, 1, 1, 6689, list, 0);*/
}

void ASN_widg::ts_Clicked()
{
	text->append(QString("Поверочное включение"));
	jm_glonass->setStyleSheet("black");
	jm_gps->setStyleSheet("black");
	jm_gg->setStyleSheet("black");
	jm_glonass->setEnabled(true);
	jm_gps->setEnabled(true);
	jm_gg->setEnabled(true);
}

void ASN_widg::jm_glonass_Clicked()
{
	text->append(QString("Задание режима работы по ГЛОНАСС"));
	jm_glonass->setEnabled(false);
	jm_gps->setStyleSheet("black");
	jm_gg->setStyleSheet("black");
	jm_gps->setEnabled(true);
	jm_gg->setEnabled(true);
	nav_SYS = NAV_SYS_GL;
	set_new_tm(8);
	jm_glonass->setStyleSheet("border-style: white");

}
void ASN_widg::jm_gps_Clicked()
{
	text->append(QString("Задание режима работы по GPS"));
	jm_gps->setEnabled(false);
	jm_glonass->setStyleSheet("black");
	jm_gg->setStyleSheet("black");
	jm_glonass->setEnabled(true);
	jm_gg->setEnabled(true);
	nav_SYS = NAV_SYS_GPS;
	set_new_tm(8);
	jm_gps->setStyleSheet("border-style: white");
}
void ASN_widg::jm_gg_Clicked()
{
	text->append(QString("Задание режима работы по ГЛОНАСС и GPS"));
	jm_gg->setEnabled(false);
	jm_glonass->setStyleSheet("black");
	jm_gps->setStyleSheet("black");
	jm_gps->setEnabled(true);
	jm_glonass->setEnabled(true);
	nav_SYS = NAV_SYS_GG;
	set_new_tm(8);
	jm_gg->setStyleSheet("border-style: white");
}
void ASN_widg::n_hrg_Clicked()
{
	if (a)
	{
		a = false;
		EnableButton();
		n_hrg->setText(QString("Прогрев"));
		n_hrg->setEnabled(true);
		ts->setEnabled(true);
		heat_og_tmr->stop();
		text->setTextColor(QColor("red"));
		text->append(QString("Отказ от прогрева ОГ"));
		text->setTextColor(QColor("black"));
		OG = true;
		set_new_tm(8);
		nw_Clicked();
		jm_gg_Clicked();
	}
	else
	{
		a = true;
		EnableButton();
		n_hrg->setText(QString("Отказ от прогрева ОГ"));
		n_hrg->setEnabled(true);
		text->setTextColor(QColor("red"));
		text->append(QString("Режим ожидания прогрева ОГ"));
		text->setTextColor(QColor("black"));
		update_time();
		n_hrg->setEnabled(true);
	}
	
}
void ASN_widg::zacep_Clicked()
{
	text->append(QString("Зацепился"));
	prer->start(1000);
	radio = true;
	dost = true;
	set_new_tm(8);
}
void ASN_widg::io_Clicked()
{
	first_half->setStyleSheet("background-color: rgb(204, 204, 204);");
	second_half->setStyleSheet("background-color: rgb(204, 204, 204);");

	if (flag_on)
	{
		time = QDateTime::currentDateTime();
		test = true;
		text->append(QString("Питание включено"));
		if (test)
		{
			text->setTextColor(QColor("green"));
			text->append(QString("Автономный тест пройден"));
			text->setTextColor(QColor("black"));
		}

		text->setTextColor(QColor("red"));
		text->append(QString("Режим ожидания прогрева ОГ"));
		text->setTextColor(QColor("black"));
		if (!sett.contains("SUBSET"))
		{
			current_SUBSET = SUBSET_1;
			sett.setValue("SUBSET", 1);
			sett.sync();
		}
		else
		{
			current_SUBSET = SUBSET(sett.value("SUBSET").toInt() - 1);

		}
		text->append(QString("Текущий комплект %1").arg(current_SUBSET+1));
		if (current_SUBSET == SUBSET_1)
			first_half->setStyleSheet("background-color: rgb(142, 198, 156);");
		else
			second_half->setStyleSheet("background-color: rgb(142, 198, 156);");

		update_time();
		n_hrg->setEnabled(true);
		slot_thr.get_omnibus_obj()->switch_ab(MKO, adr, true);
		set_new_tm(8);
	}
	else
	{
		text->append(QString("Питание отключено"));
		slot_thr.get_omnibus_obj()->switch_ab(MKO, adr, false);
		EnableButton();
		prer->stop();
		heat_og_tmr->stop();
	}
}
void ASN_widg::EnableButton()
{
	nw->setEnabled(false);
	ts->setEnabled(false);
	jm_glonass->setEnabled(false);
	jm_gps->setEnabled(false);
	jm_gg->setEnabled(false);
	n_hrg->setEnabled(false);
	zacep->setEnabled(false);
}
void ASN_widg::heating_OG()
{
	qint64 msecs_time = (QDateTime::currentMSecsSinceEpoch());
	OG_finish_warm = msecs_time;
	if (OG_start_warm == 0) OG_start_warm = OG_finish_warm - standart_tm;
	if (((OG_finish_warm - OG_start_warm) >= (standart_tm - warm_er)) || ((OG_finish_warm - OG_start_warm) >= (standart_tm + warm_er)))
	{
		OG = true;
		text->setTextColor(QColor("green"));
		text->append(QString("Прогрелся"));
		text->setTextColor(QColor("black"));
		n_hrg->setEnabled(false);
		set_new_tm(8);
		nw_Clicked();
		jm_gg_Clicked();
	}
	else
	{
		text->setTextColor(QColor("red"));
		text->append(QString("Ошибка прогрева"));
		text->setTextColor(QColor("black"));
	}

}
void ASN_widg::get_power(double _volt)
{

	volt = _volt;
	if ((volt >= 27.0) && (!flag_on))
	{
		flag_on = true;
		io_Clicked();
	}
	else if ((volt < 1) && (flag_on))
	{
		flag_on = false;
		io_Clicked();
	}

}

void ASN_widg::update_time()
{
	OG = false;
	qint64 msecs_time = (QDateTime::currentMSecsSinceEpoch());
	if (OG_finish_warm == 0)
	{
		tm_towarm = standart_tm;
		OG_start_warm = msecs_time;
		OG_finish_warm = msecs_time + tm_towarm;
	}
	
	else // Если нагревался
	{
		// Если ОГ нагрелся полностью
		if ((OG_finish_warm - OG_start_warm >= (standart_tm + warm_er)) || (OG_finish_warm - OG_start_warm >= (standart_tm - warm_er)))
		{
			if (msecs_time - OG_finish_warm >= (standart_tm * cooling_cof)) // Если после полного нагрева ОГ прошло достаточно времени, чтобы тот полностью охладился
			{
				tm_towarm = standart_tm;
				OG_start_warm = msecs_time;
				OG_finish_warm = msecs_time + standart_tm;
			}
			else // Если не прошло достаточно времени
			{
				//Время охлаждения это текущее время - время остановки ОГ  
				//Время нулевого прогрева, при ОГ - полностью был нагрет = Текущее время - (стандарт нагрева - время охлаждения / коэфициент охлаждения)
				OG_start_warm = msecs_time - (standart_tm - (msecs_time - OG_finish_warm) / cooling_cof);
				OG_finish_warm = OG_start_warm + standart_tm;
				tm_towarm = OG_finish_warm - msecs_time;
			}
		}
		else // Если нагрелся не полностью
		{
			if (msecs_time < OG_finish_warm) // Если после частичного нагрева ОГ прошло достаточно времени, чтобы тот полностью охладился
			{
				tm_towarm = standart_tm;
				OG_start_warm = msecs_time;
				OG_finish_warm = msecs_time + standart_tm;
			}
			else
			{
				//Время нулевого прогрева, при ОГ - частично прогрет = Текущее время - ((время конца прогревания - время начала прогревания) - время охлаждения / коэфициент охлаждения) 
				OG_start_warm = msecs_time - ((OG_finish_warm - OG_start_warm) - (msecs_time - OG_finish_warm) / cooling_cof);
				OG_finish_warm = OG_start_warm + standart_tm;
				tm_towarm = OG_finish_warm - msecs_time;
			}
		}
	}
	
	if (tm_towarm != 0)
		heat_og_tmr->start(tm_towarm);
}

void ASN_widg::set_new_tm(int subadr)
{
	unsigned short _word = 0;
	QVariantList tmp_list;
	if (subadr == 8)
	{
		// 1 слово
		_word = 4;
		_word += 0xC0;
		if (test) _word += 1;
		if (OG) _word += 2;
		if (dost) _word += 8;
		if (radio) _word += 0x10;

		_word += (_mode << 8);
		_word += (nav_SYS << 11);
		_word += (sys_mode << 13);
		tmp_list.push_back(_word);

		text->append(QString::number(_word));
		slot_thr.get_omnibus_obj()->set_new_data(MKO, adr, subadr, tmp_list);
	}
	tmp_list.clear();
	if (subadr == 9)
	{
		if(OG)
			priznak_dost = true;
		else
			priznak_dost = false;


		//1 слово
		text->setTextColor(QColor("green"));
		text->append(QString("Признак достоверности"));
		text->setTextColor(QColor("black"));

		if (priznak_dost) _word += 0x8000;
		tmp_list.push_back(_word);		
		text->append(QString::number(_word));


		//2-3 слово
		text->setTextColor(QColor("green"));
		text->append(QString("Время привязки"));
		text->setTextColor(QColor("black"));

		_word = 0;
		_word = sokr_time(time, true);
		tmp_list.push_back(_word);
		text->append(QString::number(_word));
		_word = 0;
		_word = sokr_time(time, false);
		tmp_list.push_back(_word);
		text->append(QString::number(_word));

		//4 слово
		//Резерв


		
		//5-8 слово
		text->setTextColor(QColor("green"));
		text->append(QString("Коордианата X"));
		text->setTextColor(QColor("black"));

		_word = 0;
		//_word = KA[count][0];
		text->append(QString::number(KA[count][0]));

		//9-12 слово
		text->setTextColor(QColor("green"));
		text->append(QString("Коордианата Y"));
		text->setTextColor(QColor("black"));

		_word = 0;
		//_word = KA[count][1];
		text->append(QString::number(KA[count][1]));

		//13-16 слово
		text->setTextColor(QColor("green"));
		text->append(QString("Коордианата Z"));
		text->setTextColor(QColor("black"));

		_word = 0;
		//_word = KA[count][2];
		text->append(QString::number(KA[count][2]));

		//17-18 слово
		text->setTextColor(QColor("green"));
		text->append(QString("Скорость X"));
		text->setTextColor(QColor("black"));

		_word = 0;
		//_word = VKA[count][0];
		text->append(QString::number(VKA[count][0]));

		//19-20 слово
		text->setTextColor(QColor("green"));
		text->append(QString("Скорость Y"));
		text->setTextColor(QColor("black"));

		_word = 0;
		//_word = VKA[count][1];
		text->append(QString::number(VKA[count][1]));

		//21-22 слово
		text->setTextColor(QColor("green"));
		text->append(QString("Скорость Z"));
		text->setTextColor(QColor("black"));

		_word = 0;
		//_word = VKA[count][2];
		text->append(QString::number(VKA[count][2]));

		//23-24 слово
		text->setTextColor(QColor("green"));
		text->append(QString("Текущее время"));
		text->setTextColor(QColor("black"));

		_word = 0;
		QDateTime now_time = QDateTime::currentDateTime();
		_word = sokr_time(now_time, true);
		tmp_list.push_back(_word);
		text->append(QString::number(_word));
		_word = 0;
		_word = sokr_time(now_time, false);
		tmp_list.push_back(_word);
		text->append(QString::number(_word));

		count += 2;


		slot_thr.get_omnibus_obj()->set_new_data(MKO, adr, subadr, tmp_list);
	}

}

ASN_widg::~ASN_widg()
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


void ASN_widg::new_message(QVariant dt, int mko, int line, int cwd, QVariantList words, int os)
{
	update_time();
	MKOWord tmp_cwd;
	tmp_cwd.com_word = cwd;
	if (os == -1)
		return;
	if ((mko == MKO) && (tmp_cwd.adr == adr))
	{
		if (tmp_cwd.subadr == 1)
		{
			int tmp_word = words[0].toInt();
			QString _msg = QString(QString("%1 принята команда 0x%2")).arg(QTime::currentTime().toString("hh:mm:ss.zzz")).arg(tmp_word, 4, 16, QChar('0'));
			text->append(_msg);

			if (tmp_word == 0x3)
				nav_SYS = NAV_SYS_GL;
			if (tmp_word == 0xC)
				nav_SYS = NAV_SYS_GPS;
			if (tmp_word == 0x30)
				nav_SYS = NAV_SYS_GG;

			if (tmp_word == 0xAA)
				_mode = MAIN_WORK;

			if (tmp_word == 0xCC)
				_mode = TEST_MODE;

			if (tmp_word == 0xF5FA)
				_mode = VECTOR_MODE;

			set_new_tm(8);
		}

		//int tmp_word = words[0].toInt();

		//set_new_tm(tmp_cwd.subadr);
	}
}

void ASN_widg::new_mk(int mshm, int pshm, int length_m, int length_p, double u_m, double u_p, int dt, int line_m, int line_p)
{
	if (volt != 0)
	{
		if ((pshm != 1) && (mshm > 2))
			return;
		QString _msg = QString("%1 принял МК МШ%2 ПШ%3").arg(QTime::currentTime().toString("hh:mm:ss.zzz")).arg(mshm).arg(pshm);
		text->append(_msg);
		int tmp_mshm = mshm;
		int tmp_pshm = pshm;

		switch (tmp_mshm)
		{
		case 0:
			current_SUBSET = SUBSET_1;
			sett.setValue("SUBSET", 1);
			flag_on = true;

			break;
		case 1:
			current_SUBSET = SUBSET_2;
			sett.setValue("SUBSET_2", 2);
			flag_on = true;

			break;
		case 2:
			flag_on = false;

			break;
		}
		text->append(QString("Текущий комплект %1").arg(current_SUBSET + 1));
		sett.sync();
		io_Clicked();
	}
}

void ASN_widg::load_fact()
{
	
	QFile in_file(QApplication::applicationDirPath()+ "/fact.rcd");
	QDataStream tmp_in_stream(&in_file);
	tmp_in_stream.setByteOrder(QDataStream::LittleEndian);
	in_file.open(QIODevice::ReadOnly);
	while (!tmp_in_stream.atEnd())
	{

		short tmp_time_item;
		tmp_in_stream >> tmp_time_item;
		SEV_time[cycle_i].y = tmp_time_item;
		tmp_in_stream >> tmp_time_item;
		SEV_time[cycle_i].m = tmp_time_item;
		tmp_in_stream >> tmp_time_item;
		SEV_time[cycle_i].d = tmp_time_item;
		tmp_in_stream >> tmp_time_item;
		SEV_time[cycle_i].h = tmp_time_item;
		tmp_in_stream >> tmp_time_item;
		SEV_time[cycle_i].min = tmp_time_item;
		tmp_in_stream >> SEV_time[cycle_i].sec;

		for (int i = 0; i < 3; ++i)
			for (int j = 0; j < 3; ++j)
				tmp_in_stream >> M[i][j];

		for (int i = 0; i < 3; ++i)
			tmp_in_stream >> VKA[cycle_i][i];

		for (int i = 0; i < 3; ++i)
			tmp_in_stream >> KA[cycle_i][i];
		cycle_i++;

	}
	if (cycle_i == 0)
	{
		text->append(QString("Файл 'fact.rcd' не найден"));
	}

	in_file.close();
}

void ASN_widg::set_new_tm_2()
{
	interrupt_slot_thr.get_interrupt_bus_obj()->make_interrupt(0, 3, 5, 4.5);
	set_new_tm(9);
}

unsigned short ASN_widg::sokr_time(QDateTime t, bool a)
{
	unsigned short _word = 0;

	if (a)
	{
		unsigned short tmp_word1 = t.toString("yy").toUShort();
		tmp_word1 <<= 10;
		unsigned short tmp_word2 = t.toString("M").toUShort();
		tmp_word2 <<= 6;
		unsigned short tmp_word3 = t.toString("d").toUShort();
		tmp_word3 <<= 1;
		unsigned short tmp_word4 = t.toString("h").toUShort();
		tmp_word4 >>= 4;

		_word = tmp_word1 + tmp_word2 + tmp_word3 + tmp_word4;
		return _word;
	}
	else
	{
		unsigned short tmp_word1 = t.toString("h").toUShort();
		if (tmp_word1 <= 15)
			tmp_word1 <<= 12;
		else
			tmp_word1 <<= 13;
		unsigned short tmp_word2 = t.toString("m").toUShort();
		tmp_word2 <<= 6;
		unsigned short tmp_word3 = t.toString("s").toUShort();

		_word = tmp_word1 + tmp_word2 + tmp_word3;
		return _word;
	}

}
unsigned short ASN_widg::od_plav(float f, int a)
{
	unsigned short _word = 0;

	if (a == 0)
	{
		float b = 123.45;
		int var = *((int*)&f);
		text->append(QString::number(var));
		return _word;
	}
	else if(a == 1)
	{
		return _word;
	}
	else if (a == 2)
	{

	}
	else if (a == 3)
	{

	}

	return 0;

}

void ASN_widg::closeEvent(QCloseEvent *event)
{
	QSettings settings(QApplication::applicationDirPath() + "/positions.ini", QSettings::IniFormat);
	settings.setValue("asn_geometry", saveGeometry());
	QWidget::closeEvent(event);
}