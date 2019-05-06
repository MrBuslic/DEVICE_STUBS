#include "R732.h"
#include "r732_socket_rpc.h"
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
			adr : 2;
	};
};

R732_widg::R732_widg() : mko_counter(0), vchm_is_init(false), MKO(1), adr(2), bus(3), power(0), volt(0), power_on(false), name("14Р732"), ready_to_work_hard(false)
{
	vchm_chanels_init << 0 << 0 << 0 << 0;
	mpvn_modules << MV_MODULE(5, 0);
	mvku_modules << MV_MODULE(2, 0);

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
	Socket_RPC_SLOT_Server_Thread* rpc_slot_srv = new Socket_RPC_SLOT_Server_Thread;
	rpc_slot_srv->set_app(this);
	rpc_slot_srv->set_params(ip_str, slot_port);
	rpc_slot_srv->start();
	Socket_RPC_SIGNAL_Thread* rpc_signal_srv = new Socket_RPC_SIGNAL_Thread;
	rpc_signal_srv->set_app(this);
	rpc_signal_srv->set_params(ip_str, signal_port);
	rpc_signal_srv->start();

	connect(power_signal_thr.get_obj().get(), SIGNAL(u_on_k2(double)), this, SLOT(get_power(double)));

	connect(&vchm_on_timer, &QTimer::timeout, this, &R732_widg::set_vchm_on);
	connect(&mu_on_timer, &QTimer::timeout, this, &R732_widg::set_mu_on);
	connect(MU1, &QPushButton::clicked, this, &R732_widg::set_mu_on);
	connect(vchm_btns_lst[0], &QPushButton::clicked, this, &R732_widg::set_vchm_on);

	mvku_modules[0].switch_cur_dev(CURRENT_DEV::OFF);
	mpvn_modules[0].switch_cur_dev(CURRENT_DEV::OFF);
	mu_module.switch_cur_dev(CURRENT_DEV::OFF);

	paint_buttons();
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
			bool need_mvku_renew = false;
			for (QVariantList::iterator itr = words.begin(); itr != words.end(); itr++)
			{
				int vchm_flag = (itr->toInt() & 0x8000) >> 15;
				switch (vchm_flag)
				{
				case 0:
				{
					int bus_reset = (itr->toInt() & 0x3);
					int switch_dev = (itr->toInt() & 0xC0) >> 6;
					int com = (itr->toInt() & 0x7000) >> 12;
					switch (com)
					{
					case 1:
						continue;
					case 2:
						if (switch_dev)
							mvku_modules[0].switch_cur_dev(CURRENT_DEV(switch_dev));
						if (bus_reset)
						{
							mvku_modules[0].set_ku_p(-1);
							need_mvku_renew = true;
						}
						break;
					case 3:
						continue;
					case 5:
						if (switch_dev)
							mpvn_modules[0].switch_cur_dev(CURRENT_DEV(switch_dev));
						break;
					};
					break;
				}
				case 1:
				{
					int vchm_chanels_on = (itr->toInt() & 0x1000) >> 12;
					switch (vchm_chanels_on)
					{
					case 0:
					{
						int chanels = (itr->toInt() & 0xF);
						if (!vchm_is_init)
						{
							vchm_chanels_init.clear();
							vchm_chanels_init << (chanels & 0x1);
							vchm_chanels_init << ((chanels & 0x2) >> 1);
							vchm_chanels_init << ((chanels & 0x4) >> 2);
							vchm_chanels_init << ((chanels & 0x8) >> 3);
							vchm_module.set_working_chanels(vchm_chanels_init, false);
							if (vchm_chanels_init.contains(1))
							{
								vchm_on_timer.start(90000);
								vchm_is_init = true;
							}
						}
						break;
					}
					case 1:
					{
						int pshk = (itr->toInt() & 0x800) >> 11;
						int nk = (itr->toInt() & 0x600) >> 9;
						int comand = (itr->toInt() & 0x3C) >> 2;
						switch (comand)
						{
						case 0xC:
							if (pshk)
							{
								for (int i = 0; i < 4; i++)
								{
									vchm_module.set_working_proc(i, 1);
								}
							}
							else
							{
								vchm_module.set_working_proc(nk, 1);
							}
							break;
						case 0x7:
						{
							if (pshk)
							{
								for (int i = 0; i < 4; i++)
								{
									restart_vchm_proc(i);
								}
							}
							else
							{
								restart_vchm_proc(nk);
							}
						}
						}
						break;
					}
					}
					break;
				}
				};
			}
			paint_buttons();
			set_new_tm();
			if (need_mvku_renew)
				new_data_mv();

		}
		//		if (tmp_cwd.subadr == 28) //MK
		//		{
		//			int max_p;
		//			max_p = 0;
		//			int num_vertic;
		//			//QVariantList pshm_list;
		//
		//			for (QVariantList::iterator itr = words.begin(); itr != words.end(); itr++)
		//			{
		//				int pshm = (itr->toInt()) >> 12;
		//				if (pshm >= 12)
		//				{
		//					QMessageBox::critical(0, "Больше 11", "Ошибка СД");
		//					break;
		//				}
		//				MV_DEV& param_pshm = mvmk_modules[pshm / 4].get_settings();
		//				for (int mshm = 0; mshm <= 11; mshm++)
		//				{
		//					num_vertic = (itr->toInt()&(1 << mshm));
		//					if (num_vertic != 0)
		//					{
		//						if (max_p <= 4)
		//						{
		//							MV_DEV& param_mshm = mvmk_modules[mshm / 4].get_settings();
		//							mvmk_modules[pshm / 4].set_ku_p(pshm % 4);
		//							mvmk_modules[mshm / 4].set_ku_m(mshm % 4);
		//							emit new_mk(mshm, pshm, param_mshm.length_kom, param_pshm.length_kom, param_mshm.u_kom, param_pshm.u_kom, std::abs(param_pshm.dt_kom - param_mshm.dt_kom), 3, 3);
		//							max_p++;
		//						}
		//						else
		//						{
		//							QMessageBox::critical(0, "Больше 4", "Ошибка СД");
		//							break;
		//						}
		//					}
		//				}
		//			}
		//			new_data_mv(tmp_cwd.subadr);
		//		}
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
						MV_DEV& param_ku = mvku_modules[nim].get_settings();
						mvku_modules[nim].set_ku_p(num_ku);
						int full_num_ku = num_ku + nim * 8;
						mku_slot_thr.get_mku_bus_obj()->make_ku_732(full_num_ku, param_ku.length_kom, param_ku.u_kom, 3);
					}

				}
			}
			new_data_mv();
		}
		//		if (tmp_cwd.subadr == 30)
		//		{
		//			slot_thr.get_omnibus_obj()->switch_ab(MKO, adr, true);
		//		}
		//		if (tmp_cwd.subadr == 26)
		//		{
		//
		//		}
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
	QVariantList tm_words;
	tm_words << mu_module.get_tm();
	tm_words << mvku_modules[0].get_tm();
	tm_words << mpvn_modules[0].get_tm();
	tm_words << get_mko_counter_word();
	omni_slot_thr.get_omnibus_obj()->set_new_data(MKO, adr, 17, tm_words);
}

QVariantList R732_widg::get_mko_counter_word()
{
	QVariantList tm_words;
	unsigned short first_word = 0x6000;
	unsigned short second_word = 0x6000;
	first_word += ((mko_counter & 0xFFF000) >> 12); //старшие 12 бит счетчика сообщений МКО
	tm_words << first_word;
	second_word += (mko_counter & 0xFFF); //младшие 12 бит счетчика сообщений МКО
	tm_words << second_word;
	return tm_words;
}

void R732_widg::paint_buttons()
{
	switch (mu_module.get_current_dev())
	{
	case OFF:
		MU1->setStyleSheet("background-color: rgb(204, 204, 204);");
		MU2->setStyleSheet("background-color: rgb(204, 204, 204);");
		break;
	case MAIN:
		MU1->setStyleSheet("background-color: rgb(142, 198, 156);");
		MU2->setStyleSheet("background-color: rgb(204, 204, 204);");
		break;
	case RESERVE:
		MU1->setStyleSheet("background-color: rgb(204, 204, 204);");
		MU2->setStyleSheet("background-color: rgb(142, 198, 156);");
		break;

	};

	switch (mvku_modules[0].get_current_dev())
	{
	case OFF:
		main_MVKU->setStyleSheet("background-color: rgb(204, 204, 204);");
		reserve_MVKU->setStyleSheet("background-color: rgb(204, 204, 204);");
		break;
	case MAIN:
		main_MVKU->setStyleSheet("background-color: rgb(142, 198, 156);");
		reserve_MVKU->setStyleSheet("background-color: rgb(204, 204, 204);");
		break;
	case RESERVE:
		main_MVKU->setStyleSheet("background-color: rgb(204, 204, 204);");
		reserve_MVKU->setStyleSheet("background-color: rgb(142, 198, 156);");
		break;

	};

	switch (mpvn_modules[0].get_current_dev())
	{
	case OFF:
		main_MPVN->setStyleSheet("background-color: rgb(204, 204, 204);");
		reserve_MPVN->setStyleSheet("background-color: rgb(204, 204, 204);");
		break;
	case MAIN:
		main_MPVN->setStyleSheet("background-color: rgb(142, 198, 156);");
		reserve_MPVN->setStyleSheet("background-color: rgb(204, 204, 204);");
		break;
	case RESERVE:
		main_MPVN->setStyleSheet("background-color: rgb(204, 204, 204);");
		reserve_MPVN->setStyleSheet("background-color: rgb(142, 198, 156);");
		break;
	};
	for (int i = 0; i < 4; i++)
	{
		if (vchm_module.get_working(i))
			vchm_btns_lst[i]->setStyleSheet("background-color: rgb(142, 198, 156);");
		else
			vchm_btns_lst[i]->setStyleSheet("background-color: rgb(204, 204, 204);");
	}
	for (int i = 0; i < 4; i++)
	{
		if (vchm_module.get_proc_working(i))
			proc_vchm_btns_lst[i]->setStyleSheet("background-color: rgb(142, 198, 156);");
		else
			proc_vchm_btns_lst[i]->setStyleSheet("background-color: rgb(204, 204, 204);");
	}
}

void R732_widg::set_vchm_on()
{
	vchm_on_timer.stop();
	if (vchm_chanels_init.size() < 4)
		return;
	vchm_module.set_working_chanels(vchm_chanels_init, true);
	vchm_is_init = false;
	paint_buttons();
}

void R732_widg::set_mu_on()
{
	mu_on_timer.stop();
	if (!ready_to_work_hard)
	{
		connect(omni_signal_thr.get_obj().get(), SIGNAL(new_message(QVariant, int, int, int, QVariantList, int)), this, SLOT(new_message(QVariant, int, int, int, QVariantList, int)));
		connect(mbk02_signal_thr.get_obj().get(), SIGNAL(set_new_tm(int, int)), this, SLOT(set_new_mbk02_tm(int, int)));
		connect(mbk02_signal_thr.get_obj().get(), SIGNAL(msg_to_14R732(QVariantList)), this, SLOT(new_kpi(QVariantList)));
		connect(mku_signal_thr.get_obj().get(), SIGNAL(new_mk(int, int, int, int, double, double, int, int, int)), this, SLOT(new_mk(int, int, int, int, double, double, int, int, int)));
		omni_slot_thr.get_omnibus_obj()->switch_ab(MKO, adr, true);
		ready_to_work_hard = true;
		mu_module.switch_cur_dev(CURRENT_DEV::MAIN);
		mpvn_modules[0].switch_cur_dev(CURRENT_DEV::MAIN);
		mvku_modules[0].switch_cur_dev(CURRENT_DEV::MAIN);
		set_new_tm();
		paint_buttons();
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
	mu_on_timer.start(12000);
	vchm_chanels_init.clear();
	vchm_chanels_init << 1 << 1 << 1 << 0;
	vchm_on_timer.start(90000);
	set_power_back();
}

void R732_widg::imit_off()
{
	mu_on_timer.stop();
	vchm_on_timer.stop();
	disconnect(omni_signal_thr.get_obj().get(), SIGNAL(new_message(QVariant, int, int, int, QVariantList, int)), this, SLOT(new_message(QVariant, int, int, int, QVariantList, int)));
	disconnect(mbk02_signal_thr.get_obj().get(), SIGNAL(set_new_tm(int, int)), this, SLOT(set_new_mbk02_tm(int, int)));
	disconnect(mbk02_signal_thr.get_obj().get(), SIGNAL(msg_to_14R732(QVariantList)), this, SLOT(new_kpi(QVariantList)));
	disconnect(mku_signal_thr.get_obj().get(), SIGNAL(new_mk(int, int, int, int, double, double, int, int, int)), this, SLOT(new_mk(int, int, int, int, double, double, int, int, int)));
	omni_slot_thr.get_omnibus_obj()->switch_ab(MKO, adr, false);
	power = 0;
	power_on = false;
	ready_to_work_hard = false;
	mu_module.switch_cur_dev(CURRENT_DEV::OFF);
	mpvn_modules[0].switch_cur_dev(CURRENT_DEV::OFF);
	mvku_modules[0].switch_cur_dev(CURRENT_DEV::OFF);
	vchm_chanels_init.clear();
	vchm_chanels_init << 0 << 0 << 0 << 0;
	vchm_module.set_working_chanels(vchm_chanels_init);
	set_power_back();
	set_new_tm();
	paint_buttons();
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
	if (pshm != 0)
		return;
	if (mshm == 1)
		mu_module.switch_cur_dev(CURRENT_DEV::MAIN);
	else if (mshm == 2)
		mu_module.switch_cur_dev(CURRENT_DEV::RESERVE);
	else
		return;
	paint_buttons();
	set_new_tm();
}

void R732_widg::new_kpi(QVariantList kpi)
{
	if (kpi.size() < 128)
		return;
	UINT8 msg[16];
	int tmp_msg_ind = 0;
	int tmp_word_ind = 0;
	UINT8 tmp_msg = 0;
	for (QVariantList::ConstIterator itr = kpi.constBegin(); itr != kpi.constEnd(); itr++)
	{
		bool can_convert = false;
		tmp_msg = tmp_msg << 1;
		int tmp_int = itr->toInt(&can_convert);
		if (can_convert && ((tmp_int == 0) || (tmp_int == 1))) //Проверяем, что двойка прислалала только нули и единицы 
			tmp_msg += tmp_int;
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
}

void R732_widg::restart_vchm_proc(int chanel)
{
	if (vchm_module.get_proc_working(chanel))
	{
		vchm_module.set_working_proc(chanel, 0);
		paint_buttons();
		QApplication::processEvents();
		Sleep(500); //Не нашел время перезагрузки ВЧМа, поэтому чисто косметический слип
		vchm_module.set_working_proc(chanel, 1);
		paint_buttons();
		QApplication::processEvents();
	}
}

MU_MODULE::MU_MODULE() : current_dev(MAIN)
{
	working.insert(MAIN, true);
	working.insert(RESERVE, true);

	ab_working.insert(MAIN, true);
	ab_working.insert(RESERVE, true);

}

MV_MODULE::MV_MODULE(int _com, int _nim) : com(_com), nim(_nim), current_dev(MAIN)
{
	devices.insert(MAIN, MV_DEV());
	devices.insert(RESERVE, MV_DEV());
}

unsigned short MU_MODULE::get_tm()
{
	unsigned short _word = 0x1000;
	if (!get_working())
		_word += 4;
	_word += 0x20 << current_dev;
	return _word;
}

unsigned short MV_MODULE::get_tm()
{
	unsigned short _word = (com << 12) + (nim << 8);
	if (!get_working())
		_word += 2 << current_dev;
	if (current_dev == OFF)
		_word += 0xC0;
	else
		_word += 0x20 << current_dev;
	return _word;
}

unsigned short MV_MODULE::get_data_mvku()
{
	unsigned short _word = (0 << 12) + (nim << 8);
	if (!get_working())
		_word += 0x1000;
	if (ku_p != -1)
		_word += 1 << ku_p;
	return _word;
}

VCHM_MODULE::VCHM_MODULE()
{
	working.insert(VCHM0, false);
	working.insert(VCHM1, false);
	working.insert(VCHM2, false);
	working.insert(VCHM3, false);
	proc_working.insert(VCHM0, false);
	proc_working.insert(VCHM1, false);
	proc_working.insert(VCHM2, false);
	proc_working.insert(VCHM3, false);
}

void VCHM_MODULE::set_working_chanels(QList<int> chanels_state, bool can_on)
{
	if (chanels_state.size() < 4)
		return;

	for (int i = 0; i < 4; i++)
	{
		if (!chanels_state[i])
		{
			set_working_proc(i, 0);
			working[VCHM_CHANEL(i)] = chanels_state[i];
		}
		else if (can_on)
		{
			working[VCHM_CHANEL(i)] = chanels_state[i];
			set_working_proc(i, 1);
		}
	}
}

void VCHM_MODULE::set_working_proc(int chanel, int state)
{
	if (chanel > 3)
		return;
	if (state)
	{
		if (working[VCHM_CHANEL(chanel)])
			proc_working[VCHM_CHANEL(chanel)] = true;
	}
	else
		proc_working[VCHM_CHANEL(chanel)] = false;
}