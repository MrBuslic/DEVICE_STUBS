#include "LKA05.h"
#include "lka05_socket_rpc.h"
#include <QMessageBox>
#include "rpc_ports.h"
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

LKA05_widg::LKA05_widg() : flag_on(false)
{
	for (int i = 0; i < 3; i++)
	{
		mvku_modules << LKA05_MV_MODULE(2, i);
		mvmk_modules << LKA05_MV_MODULE(3, i);
	}
		mpvn_modules << LKA05_MV_MODULE(5, 0);
	widg = new QWidget(this);
	this->setFixedSize(572, 200);
	setWindowTitle("ЛКА-05");
	MU1 = new QPushButton("МУ 1", this);
	MU1 -> setFixedSize(180,50);
	MU2 = new QPushButton("МУ 2", this);
	MU2->setFixedSize(180, 50);
	main_MPVN = new QPushButton("Основной", this);
//	main_MPBN->setFixedSize(300, 100);
	reserve_MPVN = new QPushButton("Резервный", this);
//	reserve_MPBN->setFixedSize(300, 100);
	MVKU0_gb = new QGroupBox("МВКУ 0", this);
	MVKU1_gb = new QGroupBox("МВКУ 1", this);
	MVKU2_gb = new QGroupBox("МВКУ 2", this);
	MVMK0_gb = new QGroupBox("МВМК 0", this);
	MVMK1_gb = new QGroupBox("МВМК 1", this);
	MVMK2_gb = new QGroupBox("МВМК 2", this);
	MPVN_gb = new QGroupBox("МПВН", this);
	for (int i = 0; i < 3; i++)
	{
		main_MVKU << new QPushButton("Основной", this);
		reserve_MVKU << new QPushButton("Резервный", this);
		main_MVMK << new QPushButton("Основной", this);
		reserve_MVMK << new QPushButton("Резервный", this);
	}
	QHBoxLayout *MVKU0_hlayout = new QHBoxLayout();
	MVKU0_hlayout->addWidget(main_MVKU[0]);
	MVKU0_hlayout->addWidget(reserve_MVKU[0]);
	MVKU0_gb->setLayout(MVKU0_hlayout);
	QHBoxLayout *MVKU1_hlayout = new QHBoxLayout();
	MVKU1_hlayout->addWidget(main_MVKU[1]);
	MVKU1_hlayout->addWidget(reserve_MVKU[1]);
	MVKU1_gb->setLayout(MVKU1_hlayout);
	QHBoxLayout *MVKU2_hlayout = new QHBoxLayout();
	MVKU2_hlayout->addWidget(main_MVKU[2]);
	MVKU2_hlayout->addWidget(reserve_MVKU[2]);
	MVKU2_gb->setLayout(MVKU2_hlayout);
	QHBoxLayout *MVMK0_hlayout = new QHBoxLayout();
	MVMK0_hlayout->addWidget(main_MVMK[0]);
	MVMK0_hlayout->addWidget(reserve_MVMK[0]);
	MVMK0_gb->setLayout(MVMK0_hlayout);
	QHBoxLayout *MVMK1_hlayout = new QHBoxLayout();
	MVMK1_hlayout->addWidget(main_MVMK[1]);
	MVMK1_hlayout->addWidget(reserve_MVMK[1]);
	MVMK1_gb->setLayout(MVMK1_hlayout);
	QHBoxLayout *MVMK2_hlayout = new QHBoxLayout();
	MVMK2_hlayout->addWidget(main_MVMK[2]);
	MVMK2_hlayout->addWidget(reserve_MVMK[2]);
	MVMK2_gb->setLayout(MVMK2_hlayout);
	QHBoxLayout *MPVN_hlayout = new QHBoxLayout();
	MPVN_hlayout->addWidget(main_MPVN);
	MPVN_hlayout->addWidget(reserve_MPVN);
	MPVN_gb->setLayout(MPVN_hlayout);
	QHBoxLayout * h_layout_MU = new QHBoxLayout();
	h_layout_MU->addWidget(MU1);
	h_layout_MU->addWidget(MU2);
	h_layout_MU->addWidget(MPVN_gb);

	MU_glayout = new QGridLayout;
//	MU_glayout->addWidget(MU1,0,0);
//	MU_glayout->addWidget(MU2, 0, 1);
	MU_glayout->addWidget(MVKU0_gb, 1, 0);
	MU_glayout->addWidget(MVKU1_gb, 1, 1);
	MU_glayout->addWidget(MVKU2_gb, 1, 2);
	MU_glayout->addWidget(MVMK0_gb, 2, 0);
	MU_glayout->addWidget(MVMK1_gb, 2, 1);
	MU_glayout->addWidget(MVMK2_gb, 2, 2);
//	MU_glayout->addWidget(MPVN_gb, 3, 1);
//	MPVN_glayout = new QGridLayout;
//	MPVN_glayout->addWidget(MPVN_gb, 3, 1);

	QVBoxLayout* v_l = new QVBoxLayout(this);
	v_l->addLayout(h_layout_MU);
	v_l->addLayout(MU_glayout);

	AbOn_tmr = new QTimer(this);
	AbOn_tmr->setSingleShot(true);
	connect(AbOn_tmr, &QTimer::timeout, this, &LKA05_widg::omni_connect);

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

	mbk04_slot_thr.set_connection_params("127.0.0.1", MBK04_SLOT);
	mbk04_slot_thr.start(); // вот тут падает

	mbk04_signal_thr.set_connection_params("127.0.0.1", MBK04_SIGNAL);
	mbk04_signal_thr.start(); // вот тут падает

	if (!mbk04_slot_thr.wait_connected(3) || !mbk04_signal_thr.wait_connected(3))
	{
		QMessageBox::critical(0, "Нет соединения", "Ошибка соединения с mbk04");
		this->deleteLater();
		return;
	}

	mku_slot_thr.set_connection_params("127.0.0.1", MKU_SLOT);
	mku_slot_thr.start(); // вот тут падает

	mku_signal_thr.set_connection_params("127.0.0.1", MKU_SIGNAL);
	mku_signal_thr.start(); // вот тут падает

	if (!mku_slot_thr.wait_connected(3) || !mku_signal_thr.wait_connected(3))
	{
		QMessageBox::critical(0, "Нет соединения", "Ошибка соединения с mku_bus");
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

	QString ip_str = "127.0.0.1";
	int slot_port = LKA05_SLOT;
	int signal_port = LKA05_SIGNAL;
	lka05_Socket_RPC_SLOT_Server_Thread* rpc_slot_srv = new lka05_Socket_RPC_SLOT_Server_Thread;
	rpc_slot_srv->set_app(this);
	rpc_slot_srv->set_params(ip_str, slot_port);
	rpc_slot_srv->start();
	lka05_Socket_RPC_SIGNAL_Thread* rpc_signal_srv = new lka05_Socket_RPC_SIGNAL_Thread;
	rpc_signal_srv->set_app(this);
	rpc_signal_srv->set_params(ip_str, signal_port);
	rpc_signal_srv->start();

	MKO = 1;
	adr = 4;

	connect(signal_thr.get_obj().get(), SIGNAL(new_message(QVariant, int, int, int, QVariantList, int)), this, SLOT(new_message(QVariant, int, int, int, QVariantList, int)));
	connect(mbk04_signal_thr.get_obj().get() ,SIGNAL(new_tm(int)), this, SLOT(new_tm(int)));// сигнал от Васи
	connect(mku_signal_thr.get_obj().get(), SIGNAL(new_ku_732(int, int, double, int)), this, SLOT(new_ku_732(int, int, double, int)));
	connect(power_signal_thr.get_obj().get(), SIGNAL(u_on_k1(double)), this, SLOT(get_power(double)));

	//connect(this, &LKA05_widg::new_ku, mku_slot_thr.get_mku_bus_obj().get(), &RPC_mku_bus_SLOT_Object::make_ku);
	//connect(this, &LKA05_widg::new_mk, mku_slot_thr.get_mku_bus_obj().get(), &RPC_mku_bus_SLOT_Object::make_mk);

	for (int i = 0; i < 3; i++)
	{
		mvku_modules[i].switch_cur_dev(LKA05_CURRENT_DEV::OFF);
		mvmk_modules[i].switch_cur_dev(LKA05_CURRENT_DEV::OFF);
	}
	mu_module.switch_cur_dev(LKA05_CURRENT_DEV::OFF);
	mpvn_modules[0].switch_cur_dev(LKA05_CURRENT_DEV::OFF);
	paint_buttons();

	QSettings settings(QApplication::applicationDirPath() + "/positions.ini", QSettings::IniFormat);
	restoreGeometry(settings.value("lka05_geometry").toByteArray());
//	set_new_tm();
}

void LKA05_widg::closeEvent(QCloseEvent *event)
{
	QSettings settings(QApplication::applicationDirPath() + "/positions.ini", QSettings::IniFormat);
	settings.setValue("lka05_geometry", saveGeometry());
	QWidget::closeEvent(event);
}

LKA05_widg::~LKA05_widg()
{

}

void LKA05_widg::imit_off()
{
	if (!flag_on)
		return;
	if (AbOn_tmr->isActive())
		AbOn_tmr->stop();
//	msg_to_log("Питание отключено");
	slot_thr.get_omnibus_obj()->switch_ab(MKO, adr, false);
	flag_on = false;
	for (int i = 0; i < 3; i++)
	{
		mvku_modules[i].switch_cur_dev(LKA05_CURRENT_DEV::OFF);
		mvmk_modules[i].switch_cur_dev(LKA05_CURRENT_DEV::OFF);
	}
	mu_module.switch_cur_dev(LKA05_CURRENT_DEV::OFF);
	mpvn_modules[0].switch_cur_dev(LKA05_CURRENT_DEV::OFF);
	paint_buttons(); //	update_graphics();
//	inter_tmr->stop();
}

void LKA05_widg::imit_on()
{
	if (flag_on)
		return;
	flag_on = true;
//	msg_to_log("Питание включено");
	AbOn_tmr->start(11000);
	change_power();//я думаю тут не нужен бул
	paint_buttons(); //	update_graphics();
//	update_time();
//	inter_tmr->start(1000);
}

void LKA05_widg::omni_connect()
{
	AbOn_tmr->stop();
	for (int i = 0; i < 3; i++)
	{
		mvku_modules[i].switch_cur_dev(LKA05_CURRENT_DEV::MAIN);
		mvmk_modules[i].switch_cur_dev(LKA05_CURRENT_DEV::MAIN);
	}
	mu_module.switch_cur_dev(LKA05_CURRENT_DEV::MAIN);
	mpvn_modules[0].switch_cur_dev(LKA05_CURRENT_DEV::MAIN);
	slot_thr.get_omnibus_obj()->switch_ab(MKO, adr, true);
	slot_thr.get_omnibus_obj()->switch_ab_os(MKO, adr, -1, 1);
	paint_buttons();
	set_new_tm();
	flag_on = true;
}

void LKA05_widg::change_power()
{
	power = 9;
	set_power_back();
}

void LKA05_widg::set_power_back()
{
	double curr;
	curr = (double)power / volt;
	power_slot_thr.get_power_bus_obj()->set_i(bus, name, curr);
}


void LKA05_widg::get_power(double _volt)
{
	volt = _volt;
	if (volt >= 20.0)
		imit_on();
	else
		if (volt < 1) 
			imit_off();
}

QCheckBox* LKA05_widg::add_set(QString name, QString data, bool is_main)
{
	QCheckBox* cb = new QCheckBox(this);
	cb->setText(name);
	cb->setProperty("ind", set_list.count());
	cb->setProperty("name", data);
	cb->setProperty("is_main", is_main);
	set_list.push_back(cb);
	return cb;
}


void LKA05_widg::new_ku_732(int ku_n, int length, double u, int line)
{

	switch (ku_n)
	{
	case 3: mu_module.switch_cur_dev(LKA05_CURRENT_DEV(LKA05_CURRENT_DEV::MAIN));
		paint_buttons();
		set_new_tm();
		break;
	case 4: mu_module.switch_cur_dev(LKA05_CURRENT_DEV(LKA05_CURRENT_DEV::RESERVE));
		paint_buttons();
		set_new_tm(); 
		break;
	default:
		break;
	};
}

void LKA05_widg::new_message(QVariant dt, int mko, int line, int cwd, QVariantList words, int os)
{
	MKOWord tmp_cwd;
	tmp_cwd.com_word = cwd;
	if (os == -1)
		return;
	if ((mko == MKO) && (tmp_cwd.adr == adr) && (tmp_cwd.trans_dir == 0))
	{
		if (tmp_cwd.subadr == 17)
		{
			bool need_mvku_renew = false;
			bool need_mvmk_renew = false;
			for (QVariantList::iterator itr = words.begin(); itr != words.end(); itr++)
			{
				int bus_reset = (itr->toInt() & 0x3);
//				int read_input = (itr->toInt() & 0x1);

				int switch_dev = (itr->toInt() & 0xC0) >> 6;
				int com = (itr->toInt() & 0x7000) >> 12; //меняю с 0х3000 на 011100...
				int nim = (itr->toInt() & 0x0300) >> 8;
				switch (com)
				{
				case 1:
					continue;
				case 2:
					if (switch_dev)
						mvku_modules[nim].switch_cur_dev(LKA05_CURRENT_DEV(switch_dev));
					if (bus_reset)
					{
						mvku_modules[nim].set_ku_p(-1);
						need_mvku_renew = true;
					}
					break;
				case 3:
					if (switch_dev)
					{
						switch (nim)
						{
						case 0:
							mvmk_modules[nim].switch_cur_dev(LKA05_CURRENT_DEV(switch_dev));
							break;
						case 1:
							mvmk_modules[nim].switch_cur_dev(LKA05_CURRENT_DEV(switch_dev));
							mvmk_modules[nim+1].switch_cur_dev(LKA05_CURRENT_DEV(switch_dev));
							break;
						case 2:
							mvmk_modules[nim-1].switch_cur_dev(LKA05_CURRENT_DEV(switch_dev));
							mvmk_modules[nim].switch_cur_dev(LKA05_CURRENT_DEV(switch_dev));
							break;
						}
					}
					if (bus_reset)
					{
						mvmk_modules[nim].set_ku_p(-1);
						mvmk_modules[nim].set_ku_m(-1);
						need_mvmk_renew = true;
					}

					break;
				case 5:
					if (switch_dev)
						mpvn_modules[nim].switch_cur_dev(LKA05_CURRENT_DEV(switch_dev));
						break;
				};
			}
			paint_buttons();
			set_new_tm();
			if (need_mvku_renew)
				new_data_mv(29);

			if (need_mvmk_renew)
				new_data_mv(28);

		}
		if (tmp_cwd.subadr == 28) //MK
		{
			int max_p;
			max_p = 0;
			int num_vertic;
			//QVariantList pshm_list;
			
			for (QVariantList::iterator itr = words.begin(); itr != words.end(); itr++)
			{
				int pshm = (itr->toInt()) >> 12;
				if (pshm >=12)
				{
					QMessageBox::critical(0, "Больше 11", "Ошибка СД");
					break;
				}
				LKA05_MV_DEV& param_pshm = mvmk_modules[pshm / 4].get_settings();
				for (int mshm = 0; mshm <= 11; mshm++)
				{
					num_vertic = (itr->toInt()&(1<<mshm));
					if (num_vertic != 0)
					{
						if (max_p <= 4)
						{
							LKA05_MV_DEV& param_mshm = mvmk_modules[mshm / 4].get_settings();
							mvmk_modules[pshm / 4].set_ku_p(pshm % 4);
							mvmk_modules[mshm / 4].set_ku_m(mshm % 4);
							mku_slot_thr.get_mku_bus_obj()->make_mk(mshm, pshm, param_mshm.length_kom, param_pshm.length_kom, param_mshm.u_kom, param_pshm.u_kom, std::abs(param_pshm.dt_kom - param_mshm.dt_kom), 3, 3);
							//emit new_mk(mshm, pshm, param_mshm.length_kom, param_pshm.length_kom, param_mshm.u_kom, param_pshm.u_kom, std::abs(param_pshm.dt_kom-param_mshm.dt_kom), 3, 3);
							max_p++;
						}
						else 
						{
							QMessageBox::critical(0, "Больше 4", "Ошибка СД");
							break;
						} 
					}
				}
			}
			new_data_mv(tmp_cwd.subadr);
		}
		if (tmp_cwd.subadr == 29) //KU
		{
			int ku;
			int max_ku;
			max_ku = 0;
			for (QVariantList::iterator itr = words.begin(); itr != words.end(); itr++)
			{
				int nim = (itr->toInt() & 0x0700) >> 8;
				for (num_ku = 0; num_ku <= 7; num_ku++)
				{
					ku = (itr->toInt()&(1 << num_ku));
					if (ku != 0)
					{
						if (max_ku <= 4)
						{
							LKA05_MV_DEV& param_ku = mvku_modules[nim].get_settings();
							mvku_modules[nim].set_ku_p(num_ku);
							int full_num_ku = num_ku + nim * 8;
							if ((full_num_ku >= 16) && (full_num_ku <= 18)) //Команды в МБК04 не заведены на внешнюю шину и выдаются напрямую
								mbk04_slot_thr.get_mbk04_obj()->new_ku(full_num_ku, param_ku.length_kom, param_ku.u_kom);
							else
								mku_slot_thr.get_mku_bus_obj()->make_ku(full_num_ku, param_ku.length_kom, param_ku.u_kom, 3);
								//emit new_ku(full_num_ku, param_ku.length_kom, param_ku.u_kom, 3);
							max_ku++;
						}
						else
						{
							QMessageBox::critical(0, "Больше 4", "Ошибка СД");
							break;
						}
					}

				}
			}
			new_data_mv(tmp_cwd.subadr);
		}
		if (tmp_cwd.subadr == 30)
		{
			slot_thr.get_omnibus_obj()->switch_ab(MKO, adr, true);
		}
		if (tmp_cwd.subadr == 26) 
		{

		}
		if ((tmp_cwd.subadr >= 2) && (tmp_cwd.subadr <= 8))
		{
			mbk04_slot_thr.get_mbk04_obj()->new_message(dt, mko, line, cwd, words, os);
		}
		//new_data(mko,addr, subadr, words);
	}
}


void LKA05_widg::paint_buttons()
{
	/*if (mu_module.get_current_dev() == MAIN)
	{
		MU1->setStyleSheet("background-color: rgb(142, 198, 156);");
		MU2->setStyleSheet("background-color: rgb(204, 204, 204);");
	}
	else
	{
		MU1->setStyleSheet("background-color: rgb(204, 204, 204);");
		MU2->setStyleSheet("background-color: rgb(142, 198, 156);");
	}*/
	switch (mu_module.get_current_dev())
	{
	case LKA05_CURRENT_DEV::OFF:
		MU1->setStyleSheet("background-color: rgb(204, 204, 204);");
		MU2->setStyleSheet("background-color: rgb(204, 204, 204);");
		break;
	case LKA05_CURRENT_DEV::MAIN:
		MU1->setStyleSheet("background-color: rgb(142, 198, 156);");
		MU2->setStyleSheet("background-color: rgb(204, 204, 204);");
		break;
	case LKA05_CURRENT_DEV::RESERVE:
		MU1->setStyleSheet("background-color: rgb(204, 204, 204);");
		MU2->setStyleSheet("background-color: rgb(142, 198, 156);");
		break;
	};
	for (int i = 0; i < 3; i++)
	{
		switch (mvku_modules[i].get_current_dev())
		{
		case LKA05_CURRENT_DEV::OFF: 
			main_MVKU[i]->setStyleSheet("background-color: rgb(204, 204, 204);");
			reserve_MVKU[i]->setStyleSheet("background-color: rgb(204, 204, 204);");
			break;
		case LKA05_CURRENT_DEV::MAIN:
			main_MVKU[i]->setStyleSheet("background-color: rgb(142, 198, 156);");
			reserve_MVKU[i]->setStyleSheet("background-color: rgb(204, 204, 204);");
			break;
		case LKA05_CURRENT_DEV::RESERVE:
			main_MVKU[i]->setStyleSheet("background-color: rgb(204, 204, 204);");
			reserve_MVKU[i]->setStyleSheet("background-color: rgb(142, 198, 156);");
			break;

		};

		switch (mvmk_modules[i].get_current_dev())
		{
		case LKA05_CURRENT_DEV::OFF:
			main_MVMK[i]->setStyleSheet("background-color: rgb(204, 204, 204);");
			reserve_MVMK[i]->setStyleSheet("background-color: rgb(204, 204, 204);");
			break;
		case LKA05_CURRENT_DEV::MAIN:
			main_MVMK[i]->setStyleSheet("background-color: rgb(142, 198, 156);");
			reserve_MVMK[i]->setStyleSheet("background-color: rgb(204, 204, 204);");
			break;
		case LKA05_CURRENT_DEV::RESERVE:
			main_MVMK[i]->setStyleSheet("background-color: rgb(204, 204, 204);");
			reserve_MVMK[i]->setStyleSheet("background-color: rgb(142, 198, 156);");
			break;

		};
	
	}
//	for (int i = 0; i < 1; i++)
//	{
		switch (mpvn_modules[0].get_current_dev())
		{
		case LKA05_CURRENT_DEV::OFF:
			main_MPVN->setStyleSheet("background-color: rgb(204, 204, 204);");
			reserve_MPVN->setStyleSheet("background-color: rgb(204, 204, 204);");
			break;
		case LKA05_CURRENT_DEV::MAIN:
			main_MPVN->setStyleSheet("background-color: rgb(142, 198, 156);");
			reserve_MPVN->setStyleSheet("background-color: rgb(204, 204, 204);");
			break;
		case LKA05_CURRENT_DEV::RESERVE:
			main_MPVN->setStyleSheet("background-color: rgb(204, 204, 204);");
			reserve_MPVN->setStyleSheet("background-color: rgb(142, 198, 156);");
			break;
		};
//	}
}

void LKA05_widg::set_new_tm()
{
	QVariantList tm_words;
	tm_words << mu_module.get_tm();
	for (int i = 0; i < 3; i++)
	{
		tm_words << mvku_modules[i].get_tm();
	}
	for (int i = 0; i < 3; i++)
	{
		tm_words << mvmk_modules[i].get_tm();
	}
	tm_words << mpvn_modules[0].get_tm(); //МПВН нужен ли?
	slot_thr.get_omnibus_obj()->set_new_data(MKO, adr, 17, tm_words); 
}

void LKA05_widg::new_data_mv(int saddr)
{
	QVariantList new_words;
	if (saddr == 29)
	{
		for (int i = 0; i < 3; i++)
		{
			unsigned short _word = mvku_modules[i].get_data_mvku();
			new_words << _word;
			new_words << _word;
		}
	}
	else {
		for (int i = 0; i < 3; i++)
		{
			unsigned short _word = mvmk_modules[i].get_data_mvmk();
			new_words << _word;
			new_words << _word;
		}
	}
	slot_thr.get_omnibus_obj()->set_new_data(MKO, adr, saddr, new_words);
}

unsigned short LKA05_MV_MODULE::get_data_mvku()
{
	unsigned short _word = (0 << 12) + (nim << 8);
	if (!get_working())
		_word += 0x1000;
	if (ku_p!= -1)
		_word += 1 << ku_p;
	return _word;
}
unsigned short LKA05_MV_MODULE::get_data_mvmk()
{
	unsigned short _word = (0 << 12) + (nim << 8);
	if (!get_working())
		_word += 0x1000;
	if (ku_p != -1)
		_word += 1 << (ku_p+4);
	if (ku_m != -1)
		_word += 1 << ku_m;
	return _word;
}

LKA05_MU_MODULE::LKA05_MU_MODULE() : current_dev(LKA05_CURRENT_DEV::MAIN)
{
	working.insert(LKA05_CURRENT_DEV::MAIN, true);
	working.insert(LKA05_CURRENT_DEV::RESERVE, true);

	ab_working.insert(LKA05_CURRENT_DEV::MAIN, true);
	ab_working.insert(LKA05_CURRENT_DEV::RESERVE, true);

}

LKA05_MV_MODULE::LKA05_MV_MODULE(int _com, int _nim) : com(_com), nim(_nim), current_dev(LKA05_CURRENT_DEV::MAIN)
{
	devices.insert(LKA05_CURRENT_DEV::MAIN, LKA05_MV_DEV());
	devices.insert(LKA05_CURRENT_DEV::RESERVE, LKA05_MV_DEV());
}

unsigned short LKA05_MU_MODULE::get_tm()
{
	unsigned short _word = 0x1000;
	if (!get_working())
		_word += 4;
	_word += 0x20 << int(current_dev);
	return _word;
}

unsigned short LKA05_MV_MODULE::get_tm()
{
	unsigned short _word = (com << 12) + (nim << 8);
	if (!get_working())
		_word += 2 << int(current_dev);
	if (current_dev == LKA05_CURRENT_DEV::OFF)
		_word += 0xC0;
	else
	{
		if (com == 5)//МПВН всегда занят в первый раз
			_word += 0x28 << int(current_dev);
		else
			_word += 0x20 << int(current_dev);
	}
	return _word;
}

void LKA05_widg::new_tm(int tm)
{
	QVariantList tm_words;
	tm_words << tm;
	slot_thr.get_omnibus_obj()->set_new_data(MKO, adr, 1, tm_words);
}