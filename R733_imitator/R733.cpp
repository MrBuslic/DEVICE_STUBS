#include "R733.h"
#include "r733_socket_rpc.h"
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

R733_widg::R733_widg()
{
	mpvn_modules << MV_MODULE(5, 0);
	mvku_modules << MV_MODULE(2, 0);
	vchm_chanels_init << 0 << 0 << 0 << 0;

	mode_names.insert(REGIME::PI15, "ПИ15");
	mode_names.insert(REGIME::PI8, "ПИ8");
	mode_names.insert(REGIME::VTF, "ВТФ");
	mode_names.insert(REGIME::RBK, "РБК");

	channel_names.insert(NUM_CHANNEL::CHANNEL_1, "first channel");
	channel_names.insert(NUM_CHANNEL::CHANNEL_2, "two channel");
	channel_names.insert(NUM_CHANNEL::CHANNEL_3, "free channel");
	channel_names.insert(NUM_CHANNEL::CHANNEL_4, "four channel");

	major_names.insert(MAJORITAR::C_1_MAJOR, "Контроль первого канала мажоритара");
	major_names.insert(MAJORITAR::C_2_MAJOR, "Контроль второго канала мажоритара");
	major_names.insert(MAJORITAR::C_3_MAJOR, "Контроль третьего канала мажоритара");
	major_names.insert(MAJORITAR::B_1_GSCH, "Блокировка первого канала ГСЧ");
	major_names.insert(MAJORITAR::B_2_GSCH, "Блокировка второго канала ГСЧ");
	major_names.insert(MAJORITAR::B_3_GSCH, "Блокировка третьего канала ГСЧ");
	major_names.insert(MAJORITAR::B_4_GSCH, "Блокировка четвертого канала ГСЧ");



	setFixedSize(592, 300);
	setWindowTitle("14Р733");
	MU1 = new QPushButton("МУ 1", this);
	MU1->setFixedSize(180, 50);
	MU2 = new QPushButton("МУ 2", this);
	MU2->setFixedSize(180, 50);
	main_MPVN = new QPushButton("Основной", this);
	reserve_MPVN = new QPushButton("Резервный", this);
	MVKU_gb = new QGroupBox("МВКУ", this);
	MPVN_gb = new QGroupBox("МПВН", this);
	main_MVKU = new QPushButton("Основной", this);
	reserve_MVKU = new QPushButton("Резервный", this);
	UPI_gb = new QGroupBox("УПИ", this);
	VCH_gb = new QGroupBox("ВЧM", this);
	QHBoxLayout *VCH_hlayout = new  QHBoxLayout();
	for (int i = 0; i < 4; i++)
	{
		VCH_list << new QPushButton(QString("ВЧM %1").arg(i), this);
		VCH_hlayout->addWidget(VCH_list[i]);
	}
	VCH_gb->setLayout(VCH_hlayout);

	QHBoxLayout *UPI_hlayout = new  QHBoxLayout();
	for (int i = 0; i < 4; i++)
	{
		UPI_list << new QPushButton(QString("МБК06-0%1").arg(i + 1), this);
		UPI_hlayout->addWidget(UPI_list[i]);
	}
	UPI_gb->setLayout(UPI_hlayout);

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

	edit_info = new QTextEdit(this);
	_scroll_bar = edit_info->verticalScrollBar();
	_doc = new QTextDocument();
	_cursor = new QTextCursor(_doc);
	edit_info->setDocument(_doc);
	edit_info->setReadOnly(true);
	_doc->setMaximumBlockCount(1000);
	auto_scroll_box = new QCheckBox(this);
	auto_scroll_box->setText("Автопрокрутка");
	auto_scroll_box->setChecked(true);
	connect(auto_scroll_box, &QCheckBox::stateChanged, this, &R733_widg::auto_scroll_clicked);


	MU_glayout = new QGridLayout;
	MU_glayout->addWidget(MVKU_gb, 1, 0);
	MU_glayout->addWidget(MPVN_gb, 1, 1);
	MU_glayout->addWidget(VCH_gb, 1, 2);
	MU_glayout->addWidget(UPI_gb, 2, 0, 2, 3);
	MU_glayout->addWidget(edit_info, 4, 0, 2, 3);
	MU_glayout->addWidget(auto_scroll_box, 6, 0, 1, 3);


	QVBoxLayout* v_l = new QVBoxLayout(this);
	v_l->addLayout(h_layout_MU);
	v_l->addLayout(MU_glayout);

	///slot_thr.set_connection_params(instr::GetIpFromSettings("rpc_omnibus"), 50001); FIX!!!!!
	omni_slot_thr.set_connection_params(instr::GetIpFromSettings("rpc_omnibus"), OMNIBUS_SLOT);
	omni_slot_thr.start(); // вот тут падает


	omni_signal_thr.set_connection_params("127.0.0.1", OMNIBUS_SIGNAL);
	omni_signal_thr.start(); // вот тут падает

	if (!omni_slot_thr.wait_connected(3) || !omni_signal_thr.wait_connected(3))
	{
		QMessageBox::critical(0, "Нет соединения", "Ошибка соединения с rpc_omnibus");
		//this->deleteLater();
		//return;
	}

	//mbk02_slot_thr.set_connection_params("127.0.0.1", MBK02_SLOT);
	//mbk02_slot_thr.start(); // вот тут падает

	//mbk02_signal_thr.set_connection_params("127.0.0.1", MBK02_SIGNAL);
	//mbk02_signal_thr.start(); // вот тут падает

	//if (!mbk02_slot_thr.wait_connected(3) || !mbk02_signal_thr.wait_connected(3))
	//{
	//	QMessageBox::critical(0, "Нет соединения", "Ошибка соединения с mbk02");
	//	//this->deleteLater();
	//	//return;
	//}

	mbk04_slot_thr.set_connection_params("127.0.0.1", MBK04_SLOT);
	mbk04_slot_thr.start(); 

	mbk04_signal_thr.set_connection_params("127.0.0.1", MBK04_SIGNAL);
	mbk04_signal_thr.start(); 

	if (!mbk04_slot_thr.wait_connected(3) || !mbk04_signal_thr.wait_connected(3))
	{
		QMessageBox::critical(0, "Нет соединения", "Ошибка соединения с mbk04");
		this->deleteLater();
		return;
	}

	mku_slot_thr.set_connection_params("127.0.0.1", MKU_SLOT);
	mku_slot_thr.start(); 

	mku_signal_thr.set_connection_params("127.0.0.1", MKU_SIGNAL);
	mku_signal_thr.start();

	if (!mku_slot_thr.wait_connected(3) || !mku_signal_thr.wait_connected(3))
	{
		QMessageBox::critical(0, "Нет соединения", "Ошибка соединения с mku_bus");
		//this->deleteLater();
		//return;
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
	int slot_port = 30208/*R733_SLOT*/;
	int signal_port = 30209/*R733_SIGNAL*/; //изменить порт
	Socket_RPC_SLOT_Server_Thread* rpc_slot_srv = new Socket_RPC_SLOT_Server_Thread;
	rpc_slot_srv->set_app(this);
	rpc_slot_srv->set_params(ip_str, slot_port);
	rpc_slot_srv->start();
	Socket_RPC_SIGNAL_Thread* rpc_signal_srv = new Socket_RPC_SIGNAL_Thread;
	rpc_signal_srv->set_app(this);
	rpc_signal_srv->set_params(ip_str, signal_port);
	rpc_signal_srv->start();

	MKO = 1;
	adr = 6;
//	omni_slot_thr.get_omnibus_obj()->switch_ab(MKO, adr, true);
	flag_on = false;


	connect(power_signal_thr.get_obj().get(), SIGNAL(u_on_k2(double)), this, SLOT(get_power(double)));
	//	connect(mbk02_signal_thr.get_obj().get(), SIGNAL(set_new_tm(int, int)), this, SLOT(set_new_mbk02_tm(int, int)));
	//		regime_upi = REGIME::PI8;
	//			QString _msg = QString("Режим работы модуля УПИ: %1").arg(mode_names[regime_upi]);
	//			msg_to_log(_msg);
	mu_module.switch_cur_dev(CURRENT_DEV::OFF);
	mvku_modules[0].switch_cur_dev(CURRENT_DEV::OFF);
	mpvn_modules[0].switch_cur_dev(CURRENT_DEV::OFF);
	paint_buttons();
		//set_new_tm();
}

void R733_widg::get_power(double _volt)
{
	volt = _volt;
	if (volt >= 20.0)
		imit_on();
	else if (volt < 1)
			imit_off();

}

void R733_widg::omni_connect()
{
	AbOn_tmr->stop();
	mu_module.switch_cur_dev(CURRENT_DEV::MAIN);
	mvku_modules[0].switch_cur_dev(CURRENT_DEV::MAIN);
	mpvn_modules[0].switch_cur_dev(CURRENT_DEV::MAIN);
	omni_slot_thr.get_omnibus_obj()->switch_ab(MKO, adr, true);
	paint_buttons();
	set_new_tm();
}

void R733_widg::set_power_back()
{
	double curr;
	if (volt > 0.1)
		curr = power / volt;
	else
		curr = 0.0;
	power_slot_thr.get_power_bus_obj()->set_i(bus, name, curr);
}

void R733_widg::change_power()
{
	power = 16; //i don't know power
	set_power_back();
}

void R733_widg::imit_on()
{
	if (flag_on)
		return;
	connect(omni_signal_thr.get_obj().get(), SIGNAL(new_message(QVariant, int, int, int, QVariantList, int)), this, SLOT(new_message(QVariant, int, int, int, QVariantList, int)));
	AbOn_tmr->start(1000); //need time here?
	change_power();
	paint_buttons();
	flag_on = true;
}

void R733_widg::imit_off()
{
	if (!flag_on)
		return;
	disconnect(omni_signal_thr.get_obj().get(), SIGNAL(new_message(QVariant, int, int, int, QVariantList, int)), this, SLOT(new_message(QVariant, int, int, int, QVariantList, int)));
	omni_slot_thr.get_omnibus_obj()->switch_ab(MKO, adr, false);
	power = 0;
	flag_on = false;
	mu_module.switch_cur_dev(CURRENT_DEV::OFF);
	mvku_modules[0].switch_cur_dev(CURRENT_DEV::OFF);
	mpvn_modules[0].switch_cur_dev(CURRENT_DEV::OFF);
	set_power_back();
//	set_new_tm();
	paint_buttons();
}


void UPI_MODULE::set_working_channels(QList<int> chanels_state, bool can_on)
{
	if (chanels_state.size() < 4)
		return;

	for (int i = 0; i < 4; i++)
	{
		if (!chanels_state[i])
			working[NUM_CHANNEL(i)] = chanels_state[i];
		else if (can_on)
			working[NUM_CHANNEL(i)] = chanels_state[i];
	}
}

void R733_widg::new_message(QVariant dt, int mko, int line, int cwd, QVariantList words, int os)
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
								vchm_on_timer.start(48000);
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
							//команда подключения процессора 
							break;
						case 0x7:
						{
							QList<int> tmp_chans;
							if (pshk)
								tmp_chans << 0 << 0 << 0 << 0;
							else
								for (int i = 0; i < 4; i++)
									if (i == nk)
										tmp_chans << 0;
									else
										tmp_chans << vchm_module.get_working(i);
							vchm_module.set_working_chanels(tmp_chans, false);
							vchm_on_timer.start(48000);
							vchm_is_init = true;
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
				//if (tmp_cwd.subadr == 17)
				//{
				//	bool need_mvku_renew = false;
				//	bool need_mvmk_renew = false;
				//	for (QVariantList::iterator itr = words.begin(); itr != words.end(); itr++)
				//	{
				//		int bus_reset = (itr->toInt() & 0x3);
				//						int read_input = (itr->toInt() & 0x1);
		
				//		int switch_dev = (itr->toInt() & 0xC0) >> 6;
				//		int com = (itr->toInt() & 0x7000) >> 12; //меняю с 0х3000 на 011100...
				//		int nim = (itr->toInt() & 0x0300) >> 8;
				//		switch (com)
				//		{
				//		case 1:
				//			continue;
				//		case 2:
				//			if (switch_dev)
				//				mvku_modules[nim].switch_cur_dev(CURRENT_DEV(switch_dev));
				//			if (bus_reset)
				//			{
				//				mvku_modules[nim].set_ku_p(-1);
				//				need_mvku_renew = true;
				//			}
				//			break;
				//		case 3:
				//			continue;
				//		case 5:
				//			if (switch_dev)
				//				mpvn_modules[nim].switch_cur_dev(CURRENT_DEV(switch_dev));
				//			break;
				//		};
				//	}
				//	paint_buttons();
	//				set_new_tm();
	//				if (need_mvku_renew)
	//					new_data_mv(29);
		
	//				if (need_mvmk_renew)
	//					new_data_mv(28);
		
	//			}
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
		/*if ((tmp_cwd.subadr == 2) || (tmp_cwd.subadr == 3))
		{
			mbk02_slot_thr.get_MBK02_obj()->new_message(dt, mko, line, cwd, words, os);
		}*/
		//new_data(mko,addr, subadr, words);
		if (tmp_cwd.subadr == 2)
		{
			for (QVariantList::iterator itr = words.begin(); itr != words.end(); itr++)
			{

				for (int i = 0; i <= 7; i++)
				{
					rrr = itr->toInt() & (0x0101 << i);

					switch (rrr)
					{
					case 257: 
						regime_upi = PI15;
						break;
					case 514: 
						regime_upi = PI8; 
						break;
					case 1028: 
						regime_upi = VTF; 
						break;
					case 8224: 
						regime_upi = RBK; 
						break;
					default:
						break;
					}
				}
			}

			QString _msg = QString("Режим работы модуля УПИ: %1").arg(mode_names[regime_upi]);
			msg_to_log(_msg);
		}

		if (tmp_cwd.subadr == 3)
		{
			for (QVariantList::iterator itr = words.begin(); itr != words.end(); itr++)
			{

				for (int i = 0; i <= 7; i++)
				{
					/*rrr = itr->toInt() & (0x0101 << i);
					switch (rrr)
					{
					case 257:
						majoritar[i] = MAJORITAR::C_1_MAJOR;
						break;
					case 514:
						majoritar[i] = MAJORITAR::C_2_MAJOR;
						break;
					case 1028:
						majoritar[i] = MAJORITAR::C_3_MAJOR;
						break;
					case 4112:
						majoritar[i] = MAJORITAR::B_1_GSCH;
						break;
					case 8224:
						majoritar[i] = MAJORITAR::B_2_GSCH;
						break;
					case 16448:
						majoritar[i] = MAJORITAR::B_3_GSCH;
						break;
					case 32896:
						majoritar[i] = MAJORITAR::B_4_GSCH;
						break;
					default:
						break;*/

					}
				}
			}
			for (int i = 0; i <= 7; i++)
			{
				//QString _msg = QString("Контроль каналов мажоритарных схем и блокировка каналов ГСЧ: %1").arg(major_names[majoritar[i]]);
				//msg_to_log(_msg);
			}
		}
		if (tmp_cwd.subadr == 4)
		{
			for (QVariantList::iterator itr = words.begin(); itr != words.end(); itr++)
			{
				channels_upi.clear();
				for (int i = 0; i <= 7; i=i+2)
				{
					rpk_1 = itr->toInt() & (0x11 << i);
					rpk_2 = (itr->toInt() & 0x11) >> i+8;
					if (rpk_1 == rpk_2)
					{
						//for (int i = 0; i <= 3; i++)
						//{
							switch (rpk_1)
							{
							case 0:
								channels_upi << (rpk_1);
								//upi_module.set_working_channels(channels_upi);
								break;
							case 1:
								channels_upi << (rpk_1);
								//upi_module.set_working_channels(channels_upi);
								break;
							case 2:
								channels_upi << (rpk_1);
								break;
							case 3:

								break;
							default:
								break;
							}
							upi_module.set_working_channels(channels_upi);
						//}
						for (int i = 0; i <= 3; i++)
						{
							QString _msg = QString("Контроль каналов мажоритарных схем и блокировка каналов ГСЧ: %1").arg(channel_names[channels_upi[i]]);
							msg_to_log(_msg);
						}
					}
				}
			}


		}
	}




void R733_widg::new_data_mv()
{
	QVariantList new_words;

	unsigned short _word = mvku_modules[0].get_data_mvku();
	new_words << _word;
	new_words << _word;

	omni_slot_thr.get_omnibus_obj()->set_new_data(MKO, adr, 29, new_words);
}

void R733_widg::set_new_tm()
{
	QVariantList tm_words;
	tm_words << mu_module.get_tm();
	tm_words << mvku_modules[0].get_tm();
	tm_words << mpvn_modules[0].get_tm();
	//tm_words << get_mko_counter_word();
	omni_slot_thr.get_omnibus_obj()->set_new_data(MKO, adr, 17, tm_words);
}

void R733_widg::paint_buttons()
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
			VCH_list[i]->setStyleSheet("background-color: rgb(142, 198, 156);");
		else
			VCH_list[i]->setStyleSheet("background-color: rgb(204, 204, 204);");
	}
	//for (int i = 0; i < 4; i++)
	//	if (upi_module)
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


void R733_widg::msg_to_log(const QString& _msg)
{
	{
		QMutexLocker lock(&log_mutex);
		log_buffer << _msg;
	}
	_cursor->insertText(_msg + "\n");
	if (auto_scroll)
		_scroll_bar->setValue(_scroll_bar->maximum());
}

void R733_widg::auto_scroll_clicked(int _state)
{
	auto_scroll = (_state != 0);
}

UPI_MODULE::UPI_MODULE()
{
	working.insert(CHANNEL_1, false);
	working.insert(CHANNEL_2, false);
	working.insert(CHANNEL_3, false);
	working.insert(CHANNEL_4, false);

}


VCHM_MODULE::VCHM_MODULE()
{
	working.insert(VCHM0, false);
	working.insert(VCHM1, false);
	working.insert(VCHM2, false);
	working.insert(VCHM3, false);
}

void VCHM_MODULE::set_working_chanels(QList<int> chanels_state, bool can_on)
{
	if (chanels_state.size() < 4)
		return;

	for (int i = 0; i < 4; i++)
	{
		if (!chanels_state[i])
			working[VCHM_CHANEL(i)] = chanels_state[i];
		else if (can_on)
			working[VCHM_CHANEL(i)] = chanels_state[i];
	}
}