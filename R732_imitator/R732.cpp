#include "R732.h"
#include "r732_socket_rpc.h"
#include <QMessageBox>
#include "rpc_ports.h"
#include "tor_bk_t_new.cpp"

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

R732_widg::R732_widg() : LKA06_MODULE(2), mko_counter(0), bus(3), power(0), volt(0), power_on(false), name("14Р732"), ready_to_work_hard(false), kpi_counter(0)
{

	PUPS = 0x13;

	tm_data.tm_data = 0;

	//setFixedSize(572, 200);
	setWindowTitle("14Р732");
	MU1 = new QPushButton("МУ 1", this);
	MU1->setFixedSize(180, 50);
	MU2 = new QPushButton("МУ 2", this);
	MU2->setFixedSize(180, 50);
	main_MPVN = new QPushButton("Основной", this);
	reserve_MPVN = new QPushButton("Резервный", this);
	QGroupBox *MVKU_gb = new QGroupBox("МВКУ", this);
	QGroupBox *MPVN_gb = new QGroupBox("МПВН", this);
	main_MVKU = new QPushButton("Основной", this);
	reserve_MVKU = new QPushButton("Резервный", this);
	QGroupBox *VCHM_gb = new QGroupBox("ВЧМ", this);
	QGroupBox *VCHM0_gb = new QGroupBox("ВЧМ0", this);
	QGroupBox *VCHM1_gb = new QGroupBox("ВЧМ1", this);
	QGroupBox *VCHM2_gb = new QGroupBox("ВЧМ2", this);
	QGroupBox *VCHM3_gb = new QGroupBox("ВЧМ3", this);
	QList<QGroupBox*> vchm_gb_lst;
	vchm_gb_lst << VCHM0_gb << VCHM1_gb << VCHM2_gb << VCHM3_gb;
	vchm_btns_lst << vchm0 << vchm1 << vchm2 << vchm3;
	for (int i = 0; i < 4; i++)
	{
		vchm_btns_lst[i] = new QPushButton("Питание", this);
	}
	proc_vchm_btns_lst << proc_vchm0 << proc_vchm1 << proc_vchm2 << proc_vchm3;
	for (int i = 0; i < 4; i++)
	{
		proc_vchm_btns_lst[i] = new QPushButton("Процессор", this);
	}

	QHBoxLayout *MVKU_hlayout = new QHBoxLayout();
	MVKU_hlayout->addWidget(main_MVKU);
	MVKU_hlayout->addWidget(reserve_MVKU);
	MVKU_gb->setLayout(MVKU_hlayout);
	QHBoxLayout *MPVN_hlayout = new QHBoxLayout();
	MPVN_hlayout->addWidget(main_MPVN);
	MPVN_hlayout->addWidget(reserve_MPVN);
	MPVN_gb->setLayout(MPVN_hlayout);
	QHBoxLayout * h_layout_MU = new QHBoxLayout();
	h_layout_MU->addWidget(MU1);
	h_layout_MU->addWidget(MU2);

	QHBoxLayout* vchm_layout = new QHBoxLayout();
	QVBoxLayout* vchm0_l = new QVBoxLayout();
	QVBoxLayout* vchm1_l = new QVBoxLayout();
	QVBoxLayout* vchm2_l = new QVBoxLayout();
	QVBoxLayout* vchm3_l = new QVBoxLayout();
	QList<QVBoxLayout*> vchm_l_lst;
	vchm_l_lst << vchm0_l << vchm1_l << vchm2_l << vchm3_l;
	for (int i = 0; i < 4; i++)
	{
		vchm_l_lst[i]->addWidget(vchm_btns_lst[i]);
		vchm_l_lst[i]->addWidget(proc_vchm_btns_lst[i]);
		vchm_gb_lst[i]->setLayout(vchm_l_lst[i]);
		vchm_layout->addWidget(vchm_gb_lst[i]);
	}
	VCHM_gb->setLayout(vchm_layout);

	QGridLayout *MU_glayout = new QGridLayout;
	MU_glayout->addWidget(MVKU_gb, 1, 0);
	MU_glayout->addWidget(MPVN_gb, 1, 1);

	QVBoxLayout* v_l = new QVBoxLayout(this);
	v_l->addLayout(h_layout_MU);
	v_l->addLayout(MU_glayout);
	v_l->addWidget(VCHM_gb);

	omni_slot_thr.set_connection_params(instr::GetIpFromSettings("rpc_omnibus"), OMNIBUS_SLOT);
	omni_slot_thr.start();


	omni_signal_thr.set_connection_params("127.0.0.1", OMNIBUS_SIGNAL);
	omni_signal_thr.start(); 

	if (!omni_slot_thr.wait_connected(3) || !omni_signal_thr.wait_connected(3))
	{
		QMessageBox::critical(0, "Нет соединения", "Ошибка соединения с rpc_omnibus");
	}

	mbk02_slot_thr.set_connection_params("127.0.0.1", MBK02_SLOT);
	mbk02_slot_thr.start(); 

	mbk02_signal_thr.set_connection_params("127.0.0.1", MBK02_SIGNAL);
	mbk02_signal_thr.start(); 

	if (!mbk02_slot_thr.wait_connected(3) || !mbk02_signal_thr.wait_connected(3))
	{
		QMessageBox::critical(0, "Нет соединения", "Ошибка соединения с mbk02");
	}

	mku_slot_thr.set_connection_params("127.0.0.1", MKU_SLOT);
	mku_slot_thr.start(); 

	mku_signal_thr.set_connection_params("127.0.0.1", MKU_SIGNAL);
	mku_signal_thr.start(); 

	if (!mku_slot_thr.wait_connected(3) || !mku_signal_thr.wait_connected(3))
	{
		QMessageBox::critical(0, "Нет соединения", "Ошибка соединения с mku_bus");
	}

	power_slot_thr.set_connection_params("127.0.0.1", POWER_SLOT);
	power_slot_thr.start();

	power_signal_thr.set_connection_params("127.0.0.1", POWER_SIGNAL);
	power_signal_thr.start();

	if (!power_slot_thr.wait_connected(3) || !power_signal_thr.wait_connected(3))
	{
		QMessageBox::critical(0, "Нет соединения", "Ошибка соединения с power_bus");
	}


	QString ip_str = "127.0.0.1";
	int slot_port = R732_SLOT;
	int signal_port = R732_SIGNAL;
	r732_Socket_RPC_SLOT_Server_Thread* rpc_slot_srv = new r732_Socket_RPC_SLOT_Server_Thread;
	rpc_slot_srv->set_app(this);
	rpc_slot_srv->set_params(ip_str, slot_port);
	rpc_slot_srv->start();
	r732_Socket_RPC_SIGNAL_Thread* rpc_signal_srv = new r732_Socket_RPC_SIGNAL_Thread;
	rpc_signal_srv->set_app(this);
	rpc_signal_srv->set_params(ip_str, signal_port);
	rpc_signal_srv->start();

	connect(power_signal_thr.get_obj().get(), SIGNAL(u_on_k2(double)), this, SLOT(get_power(double)));

	connect(omni_signal_thr.get_obj().get(), SIGNAL(new_message(QVariant, int, int, int, QVariantList, int)), this, SLOT(new_message(QVariant, int, int, int, QVariantList, int)));
	connect(mbk02_signal_thr.get_obj().get(), SIGNAL(set_new_tm(int, int)), this, SLOT(set_new_mbk02_tm(int, int)));
	connect(mbk02_signal_thr.get_obj().get(), SIGNAL(msg_to_14R732(QVariantList)), this, SLOT(new_kpi(QVariantList)));
	connect(mku_signal_thr.get_obj().get(), SIGNAL(new_mk(int, int, int, int, double, double, int, int, int)), this, SLOT(new_mk(int, int, int, int, double, double, int, int, int)));


	connect(MU1, &QPushButton::clicked, this, &R732_widg::set_mu_on);
	connect(vchm_btns_lst[0], &QPushButton::clicked, this, &R732_widg::po_start_timeout);


	paint_buttons();

	QSettings settings(QApplication::applicationDirPath() + "/positions.ini", QSettings::IniFormat);
	restoreGeometry(settings.value("732_geometry").toByteArray());
}

R732_widg::~R732_widg()
{
	omni_slot_thr.quit();
	omni_signal_thr.quit();
	mbk02_slot_thr.quit();
	mbk02_signal_thr.quit();
	mku_slot_thr.quit();
	mku_signal_thr.quit();
}

void R732_widg::closeEvent(QCloseEvent *event)
{
	QSettings settings(QApplication::applicationDirPath() + "/positions.ini", QSettings::IniFormat);
	settings.setValue("732_geometry", saveGeometry());
	QWidget::closeEvent(event);
}

void R732_widg::new_message(QVariant dt, int mko, int line, int cwd, QVariantList words, int os)
{
	MKOWord tmp_cwd;
	tmp_cwd.com_word = cwd;
	if (os == -1)
		return;
	if ((mko == MKO) && (tmp_cwd.adr == adr) && (tmp_cwd.subadr >= 17) && (tmp_cwd.subadr <= 29))
	{
		mko_counter++;
		set_new_tm();
	}
	if ((mko == MKO) && (tmp_cwd.adr == adr) && (tmp_cwd.trans_dir == 0))
	{
		if (tmp_cwd.subadr == 17)
		{
			new_message_mu(words);

			if (need_mvku_renew)
				new_data_mv();
		}
		if (tmp_cwd.subadr == 19)
		{

			if (words.at(0).toInt() == 0x0D00)
			{
				PUPS = 0;
				set_new_tm();
			}
		}
		if (tmp_cwd.subadr == 21)
		{
			if (words.size() >= 2)
			{
				for (int i = 0; i < 2; ++i)
				{
					SCHBK[i] = words.at(i).toInt();
				}
				makeFuckingMagic();

			}
		}

		if (tmp_cwd.subadr == 18)
		{
			if (!words.empty())
			{
				MKOWord tmp_mpko_cwd;
				tmp_mpko_cwd.com_word = words.at(0).toInt();
				words.pop_front();
				QVariantList tmp_mpko_words = words;
				int tmp_mpko_os;
				omni_slot_thr.get_omnibus_obj()->send_msg_mpko(mko, line, tmp_mpko_cwd.com_word, tmp_mpko_words, tmp_mpko_os);
				if (tmp_mpko_os != -1)
				{
					if (tmp_mpko_cwd.trans_dir == 1)
						omni_slot_thr.get_omnibus_obj()->set_new_data(mko, adr, 18, tmp_mpko_words);
				}
			}

		}
		if (tmp_cwd.subadr == 29) //KU
		{
			int ku;
			for (QVariantList::iterator itr = words.begin(); itr != words.end(); itr++)
			{
				int nim = (itr->toInt() & 0x0700) >> 8;
				for (num_ku = 0; num_ku <= 7; num_ku++)
				{
					ku = (itr->toInt()&(1 << num_ku));
					if (ku != 0 && nim == 0)
					{
						LKA_MV_DEV& param_ku = mvku_modules[nim].get_settings();
						mvku_modules[nim].set_ku_p(num_ku);
						int full_num_ku = num_ku + nim * 8;
						mku_slot_thr.get_mku_bus_obj()->make_ku_732(full_num_ku, param_ku.length_kom, param_ku.u_kom, 3);
					}

				}
			}
			new_data_mv();

		}

		if ((tmp_cwd.subadr == 2) || (tmp_cwd.subadr == 3))
		{
			mbk02_slot_thr.get_MBK02_obj()->new_message(dt, mko, line, cwd, words, os);
		}
	}
}

void R732_widg::set_new_mbk02_tm(int sadr, int word)
{
	QVariantList tm_words;
	tm_words << word;
	omni_slot_thr.get_omnibus_obj()->set_new_data(MKO, adr, sadr, tm_words);
}

void R732_widg::new_data_mv()
{
	QVariantList new_words;

	unsigned short _word = mvku_modules[0].get_data_mvku();
	new_words << _word;
	new_words << _word;

	omni_slot_thr.get_omnibus_obj()->set_new_data(MKO, adr, 29, new_words);
}

void R732_widg::set_new_tm()
{
	omni_slot_thr.get_omnibus_obj()->set_new_data(MKO, adr, 17, get_tm());
}

void R732_widg::set_new_kpi()
{
	QVariantList kpi_words;
	for (int i = 0; i < msg_count; ++i)
	{
		kpi_words << mko_Buffer[i];
	}
	omni_slot_thr.get_omnibus_obj()->set_new_data(MKO, adr, 21, kpi_words);
}


void R732_widg::paint_buttons()
{
	switch (mu_module.get_current_dev())
	{
	case LKA_CURRENT_DEV::OFF:
		MU1->setStyleSheet("background-color: rgb(204, 204, 204);");
		MU2->setStyleSheet("background-color: rgb(204, 204, 204);");
		break;
	case LKA_CURRENT_DEV::MAIN:
		MU1->setStyleSheet("background-color: rgb(142, 198, 156);");
		MU2->setStyleSheet("background-color: rgb(204, 204, 204);");
		break;
	case LKA_CURRENT_DEV::RESERVE:
		MU1->setStyleSheet("background-color: rgb(204, 204, 204);");
		MU2->setStyleSheet("background-color: rgb(142, 198, 156);");
		break;

	};

	switch (mvku_modules[0].get_current_dev())
	{
	case LKA_CURRENT_DEV::OFF:
		main_MVKU->setStyleSheet("background-color: rgb(204, 204, 204);");
		reserve_MVKU->setStyleSheet("background-color: rgb(204, 204, 204);");
		break;
	case LKA_CURRENT_DEV::MAIN:
		main_MVKU->setStyleSheet("background-color: rgb(142, 198, 156);");
		reserve_MVKU->setStyleSheet("background-color: rgb(204, 204, 204);");
		break;
	case LKA_CURRENT_DEV::RESERVE:
		main_MVKU->setStyleSheet("background-color: rgb(204, 204, 204);");
		reserve_MVKU->setStyleSheet("background-color: rgb(142, 198, 156);");
		break;

	};

	switch (mpvn_modules[0].get_current_dev())
	{
	case LKA_CURRENT_DEV::OFF:
		main_MPVN->setStyleSheet("background-color: rgb(204, 204, 204);");
		reserve_MPVN->setStyleSheet("background-color: rgb(204, 204, 204);");
		break;
	case LKA_CURRENT_DEV::MAIN:
		main_MPVN->setStyleSheet("background-color: rgb(142, 198, 156);");
		reserve_MPVN->setStyleSheet("background-color: rgb(204, 204, 204);");
		break;
	case LKA_CURRENT_DEV::RESERVE:
		main_MPVN->setStyleSheet("background-color: rgb(204, 204, 204);");
		reserve_MPVN->setStyleSheet("background-color: rgb(142, 198, 156);");
		break;
	};
	for (int i = 0; i < 4; i++)
	{
		if (get_working(i))
			vchm_btns_lst[i]->setStyleSheet("background-color: rgb(142, 198, 156);");
		else
			vchm_btns_lst[i]->setStyleSheet("background-color: rgb(204, 204, 204);");
	}
	for (int i = 0; i < 4; i++)
	{
		if (get_proc_working(i))
			proc_vchm_btns_lst[i]->setStyleSheet("background-color: rgb(142, 198, 156);");
		else
			proc_vchm_btns_lst[i]->setStyleSheet("background-color: rgb(204, 204, 204);");
	}
}

void R732_widg::get_power(double _volt)
{
	volt = _volt;
	if (volt >= 20.0 && !power_on)
		imit_on();
	else if (volt < 1 && power_on) 
		imit_off();
}

void R732_widg::imit_on()
{
	power = 16;
	power_on = true;
	set_new_mbk02_tm(1, 0xFFFF);
	set_new_mbk02_tm(4, 0xFFFF);
	mu_dev_to_set = LKA_CURRENT_DEV::MAIN;
	PUPS = 0x13;
	lka_imit_on();
	set_power_back();
	tm_data.PP = 1;
	tm_data.VP_O = 1;
	set_tm_state();
}

void R732_widg::imit_off()
{
	power = 0;
	power_on = false;
	ready_to_work_hard = false;
	set_power_back();
	lka_imit_off();
	tm_data.PP = 0;
	tm_data.VP_O = 0;
	tm_data.VP_R = 0;
	set_tm_state();
}

void R732_widg::set_power_back()
{
	double curr;
	if (volt > 0.1)
		curr = power / volt;
	else
		curr = 0.0;
	power_slot_thr.get_power_bus_obj()->set_i(bus, name, curr);
}

void R732_widg::new_mk(int mshm, int pshm, int length_m, int length_p, double u_m, double u_p, int dt, int line_m, int line_p)
{
	if ((pshm != 0) || (mshm < 1) || (mshm > 2))
		return;
	imit_off();
	imit_on();
	if (mshm == 1)
	{
		tm_data.VP_O = 1;
		tm_data.VP_R = 0;
		mu_dev_to_set = LKA_CURRENT_DEV::MAIN;
	}
	else if (mshm == 2)
	{
		tm_data.VP_O = 0;
		tm_data.VP_R = 1;
		mu_dev_to_set = LKA_CURRENT_DEV::RESERVE;
	}
	else
		return;
	paint_buttons();
	set_new_tm();
	set_tm_state();
}

void R732_widg::new_kpi(QVariantList kpi)
{
	UINT8 msg[16];
	int tmp_msg_ind = 0;
	int tmp_word_ind = 0;
	UINT8 tmp_msg = 0;
	kpi_counter++;
	for (QVariantList::ConstIterator itr = kpi.constBegin(); itr != kpi.constEnd(); itr++)
	{
		bool can_convert = false;
		int tmp_int = itr->toInt(&can_convert);
		if (can_convert && ((tmp_int == 0) || (tmp_int == 1))) //Проверяем, что двойка прислалала только нули и единицы 
			tmp_msg += tmp_int << tmp_word_ind;
		tmp_word_ind++;
		if (tmp_word_ind == 8)
		{
			msg[tmp_msg_ind] = tmp_msg;
			tmp_msg = 0;
			tmp_msg_ind++;
			tmp_word_ind = 0;
			if (tmp_msg_ind == 16)
				break;
		}
	}
	TestMSG_KPI(msg, kpi.length());
	set_new_tm();
	if (msg_count > 0)
		set_new_kpi();
	interrupt_slot_thr.get_interrupt_bus_obj()->make_interrupt(2, 3, 5, 4.5);
}

void R732_widg::set_tm_state()
{
	mku_slot_thr.get_mku_bus_obj()->set_tm("732_TM", (uint)(tm_data.tm_data));
}