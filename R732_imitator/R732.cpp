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

R732_widg::R732_widg()
{
	mpvn_modules << MV_MODULE(5, 0);
	mvku_modules << MV_MODULE(2, 0);

	setFixedSize(572, 200);
	setWindowTitle("14Р732");
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

	MU_glayout = new QGridLayout;
	MU_glayout->addWidget(MVKU_gb, 1, 0);
	MU_glayout->addWidget(MPVN_gb, 1, 1);

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

	mbk02_slot_thr.set_connection_params("127.0.0.1", MBK02_SLOT);
	mbk02_slot_thr.start(); // вот тут падает

	mbk02_signal_thr.set_connection_params("127.0.0.1", MBK02_SIGNAL);
	mbk02_signal_thr.start(); // вот тут падает

	if (!mbk02_slot_thr.wait_connected(3) || !mbk02_signal_thr.wait_connected(3))
	{
		QMessageBox::critical(0, "Нет соединения", "Ошибка соединения с mbk02");
		//this->deleteLater();
		//return;
	}

	mku_slot_thr.set_connection_params("127.0.0.1", MKU_SLOT);
	mku_slot_thr.start(); // вот тут падает

	mku_signal_thr.set_connection_params("127.0.0.1", MKU_SIGNAL);
	mku_signal_thr.start(); // вот тут падает

	if (!mku_slot_thr.wait_connected(3) || !mku_signal_thr.wait_connected(3))
	{
		QMessageBox::critical(0, "Нет соединения", "Ошибка соединения с mku_bus");
		//this->deleteLater();
		//return;
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

	MKO = 1;
	adr = 2;
	omni_slot_thr.get_omnibus_obj()->switch_ab(MKO, adr, true);
	flag = true;

	connect(omni_signal_thr.get_obj().get(), SIGNAL(new_message(QVariant, int, int, int, QVariantList, int)), this, SLOT(new_message(QVariant, int, int, int, QVariantList, int)));
	connect(mbk02_signal_thr.get_obj().get(), SIGNAL(set_new_tm(int, int)), this, SLOT(set_new_mbk02_tm(int, int)));

	paint_buttons();
	//set_new_tm();
}

void R732_widg::new_message(QVariant dt, int mko, int line, int cwd, QVariantList words, int os)
{
	MKOWord tmp_cwd;
	tmp_cwd.com_word = cwd;
	if (os == -1)
		return;
	if ((mko == MKO) && (tmp_cwd.adr == adr) && (tmp_cwd.trans_dir == 0))
	{
		//		if (tmp_cwd.subadr == 17)
		//		{
		//			bool need_mvku_renew = false;
		//			bool need_mvmk_renew = false;
		//			for (QVariantList::iterator itr = words.begin(); itr != words.end(); itr++)
		//			{
		//				int bus_reset = (itr->toInt() & 0x3);
		//				//				int read_input = (itr->toInt() & 0x1);
		//
		//				int switch_dev = (itr->toInt() & 0xC0) >> 6;
		//				int com = (itr->toInt() & 0x7000) >> 12; //меняю с 0х3000 на 011100...
		//				int nim = (itr->toInt() & 0x0300) >> 8;
		//				switch (com)
		//				{
		//				case 1:
		//					continue;
		//				case 2:
		//					if (switch_dev)
		//						mvku_modules[nim].switch_cur_dev(CURRENT_DEV(switch_dev));
		//					if (bus_reset)
		//					{
		//						mvku_modules[nim].set_ku_p(-1);
		//						need_mvku_renew = true;
		//					}
		//					break;
		//				case 3:
		//					continue;
		//				case 5:
		//					if (switch_dev)
		//						mpvn_modules[nim].switch_cur_dev(CURRENT_DEV(switch_dev));
		//					break;
		//				};
		//			}
		//			paint_buttons();
		//			set_new_tm();
		//			if (need_mvku_renew)
		//				new_data_mv(29);
		//
		//			if (need_mvmk_renew)
		//				new_data_mv(28);
		//
		//		}
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
		//new_data(mko,addr, subadr, words);
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

void R732_widg::paint_buttons()
{
	if (mu_module.get_current_dev() == MAIN)
	{
		MU1->setStyleSheet("background-color: rgb(142, 198, 156);");
		MU2->setStyleSheet("background-color: rgb(204, 204, 204);");
	}
	else
	{
		MU1->setStyleSheet("background-color: rgb(204, 204, 204);");
		MU2->setStyleSheet("background-color: rgb(142, 198, 156);");
	}
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