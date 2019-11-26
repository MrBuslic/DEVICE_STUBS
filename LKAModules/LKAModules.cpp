#include "LKAModules.h"
#include "rpc_ports.h"
#include <QMessageBox>
#include "instruments.h"

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

LKA06_MODULE::LKA06_MODULE(int _adr) : adr(_adr), MKO(1)
{
	mpvn_modules << LKA_MV_MODULE(5, 0);
	mvku_modules << LKA_MV_MODULE(2, 0);

	working.insert(VCHM0, false);
	working.insert(VCHM1, false);
	working.insert(VCHM2, false);
	working.insert(VCHM3, false);
	proc_working.insert(VCHM0, false);
	proc_working.insert(VCHM1, false);
	proc_working.insert(VCHM2, false);
	proc_working.insert(VCHM3, false);

	connect(&po_start, &QTimer::timeout, this, &LKA06_MODULE::po_start_timeout);
	connect(&mu_on_timer, &QTimer::timeout, this, &LKA06_MODULE::set_mu_on);

	omni_slot_thr.set_connection_params(instr::GetIpFromSettings("rpc_omnibus"), OMNIBUS_SLOT);
	omni_slot_thr.start();


	omni_signal_thr.set_connection_params("127.0.0.1", OMNIBUS_SIGNAL);
	omni_signal_thr.start();

	if (!omni_slot_thr.wait_connected(3) || !omni_signal_thr.wait_connected(3))
	{
		QMessageBox::critical(0, "Нет соединения", "Ошибка соединения с rpc_omnibus");
	}

	mku_slot_thr.set_connection_params("127.0.0.1", MKU_SLOT);
	mku_slot_thr.start();

	mku_signal_thr.set_connection_params("127.0.0.1", MKU_SIGNAL);
	mku_signal_thr.start();

	if (!mku_slot_thr.wait_connected(3) || !mku_signal_thr.wait_connected(3))
	{
		QMessageBox::critical(0, "Нет соединения", "Ошибка соединения с mku_bus");
	}

	interrupt_slot_thr.set_connection_params("127.0.0.1", INTERRUPTS_SLOT);
	interrupt_slot_thr.start();

	interrupt_signal_thr.set_connection_params("127.0.0.1", INTERRUPTS_SIGNAL);
	interrupt_signal_thr.start();

	if (!interrupt_slot_thr.wait_connected(3) || !interrupt_signal_thr.wait_connected(3))
	{
		QMessageBox::critical(0, "Нет соединения", "Ошибка соединения с interrupt_bus");
	}

	mvku_modules[0].switch_cur_dev(LKA_CURRENT_DEV::OFF);
	mpvn_modules[0].switch_cur_dev(LKA_CURRENT_DEV::OFF);
	mu_module.switch_cur_dev(LKA_CURRENT_DEV::OFF);

}

void LKA06_MODULE::new_message_mu(QVariantList words)
{
	need_mvku_renew = false;
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
					mvku_modules[0].switch_cur_dev(LKA_CURRENT_DEV(switch_dev));
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
					mpvn_modules[0].switch_cur_dev(LKA_CURRENT_DEV(switch_dev));
				break;
			};
			break;
		}
		case 1:
		{
			if ((itr->toInt() & 0x1000) == 0)
			{
				int chanels = (itr->toInt() & 0xF);

				vchm_chanels_init.clear();
				vchm_chanels_init << (chanels & 0x1);
				vchm_chanels_init << ((chanels & 0x2) >> 1);
				vchm_chanels_init << ((chanels & 0x4) >> 2);
				vchm_chanels_init << ((chanels & 0x8) >> 3);
				set_working_chanels(vchm_chanels_init, true);

			}
			else
			{
				int pshk = (itr->toInt() & 0x800) >> 11;
				int nk = (itr->toInt() & 0x600) >> 9;
				int comand = (itr->toInt() & 0x3C) >> 2;
				switch (comand)
				{
				case 0xC:
					//if (pshk)
					//{
					//	for (int i = 0; i < 4; i++)
					//	{
					//		vchm_module.set_working_proc(i, 1);
					//	}
					//}
					//else
					//{
					//	vchm_module.set_working_proc(nk, 1);
					//}
					if (!po_start.isActive())
					{
						po_start.start(VCHM_START_TIME);
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
					break;
				}
				};

			}
			break;
		}
		};
	}
	set_new_tm();
	paint_buttons();
}

QVariantList LKA06_MODULE::get_tm()
{
	QVariantList tm_words;
	tm_words << mu_module.get_tm();
	tm_words << mvku_modules[0].get_tm();
	tm_words << mpvn_modules[0].get_tm();
	tm_words << get_mko_counter_word();
	tm_words << get_pups_words_list();
	tm_words << get_vchm_word();
	unsigned short word_11 = 0xC080;
	word_11 += (PUPS & 0x1F);
	tm_words << word_11;
	tm_words << 0xC000;
	tm_words << 0xC200;
	tm_words << 0xC400;
	tm_words << 0xC600;

	return tm_words;
}

QVariantList LKA06_MODULE::get_mko_counter_word()
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

QVariantList LKA06_MODULE::get_pups_words_list()
{
	QVariantList tm_words;
	//for (int vchm_index = 0; vchm_index < 4; ++vchm_index)
	//{
	//	unsigned short tmp_word = 0x8000;
	//	tmp_word += ((vchm_index & 0x3) << 12); //номер ВЧМа (биты 12-13)
	//	tmp_word += 0xC0; //разрешение чтения и записи (биты 6-7)
	//	if (vchm_module.get_working(vchm_index))
	//	{
	//		tmp_word += 0x100; //признак включения ВЧМ (бит 8)
	//		tmp_word += (PUPS & 0x1F); //пупс
	//	}
	//	tm_words << tmp_word;
	//}
	for (int vchm_index = 0; vchm_index < 4; ++vchm_index)
	{
		unsigned short tmp_word = 0x8000;
		tmp_word += ((vchm_index & 0x3) << 9); //номер ВЧМа (биты 9-10)
		tmp_word += 0xC0; //разрешение чтения и записи (биты 6-7)
		if (get_working(vchm_index))
		{
			tmp_word += 0x100; //признак включения ВЧМ (бит 8)
			tmp_word += (PUPS & 0x1F); //пупс
		}
		tm_words << tmp_word;
	}
	return tm_words;

}

QVariantList LKA06_MODULE::get_vchm_word()
{
	QVariantList tm_words;
	unsigned short tmp_word = 0xC000;
	for (int vchm_index = 0; vchm_index < 4; ++vchm_index)
	{
		if (get_working(vchm_index))
		{
			tmp_word += 1 << vchm_index; //признак включения ВЧМ (биты 0-3)
			tmp_word += 1 << (8 + vchm_index); //признак включения ВЧМ в рабочую конфигурацию (биты 8-11)
			//tmp_word += (PUPS & 0x1F); //пупс
		}
	}
	tm_words << tmp_word;
	return tm_words;
}

void LKA06_MODULE::po_start_timeout()
{
	po_start.stop();
	for (int i = 0; i < 4; i++)
	{
		set_working_proc(i, true);
	}
	interrupt_slot_thr.get_interrupt_bus_obj()->make_interrupt(adr, 3, 5, 4.5);
	paint_buttons();
	//PUPS = 0x13;
	set_new_tm();
}

void LKA06_MODULE::set_mu_on()
{
	mu_on_timer.stop();
	//if (!ready_to_work_hard)
	//{
		omni_slot_thr.get_omnibus_obj()->switch_ab(MKO, adr, true);
		//ready_to_work_hard = true;
		mu_module.switch_cur_dev(mu_dev_to_set);
		mpvn_modules[0].switch_cur_dev(mu_module.get_current_dev());
		mvku_modules[0].switch_cur_dev(mu_module.get_current_dev());

		mko_counter = 0;

		set_new_tm();
		paint_buttons();
	//}
}

void LKA06_MODULE::lka_imit_on()
{
	mu_on_timer.start(12000);
	vchm_chanels_init.clear();
	vchm_chanels_init << 1 << 1 << 1 << 1;
	set_working_chanels(vchm_chanels_init, true);
	po_start.start(VCHM_START_TIME);
}

void LKA06_MODULE::lka_imit_off()
{
	mu_on_timer.stop();
	po_start.stop();
	omni_slot_thr.get_omnibus_obj()->switch_ab(MKO, adr, false);
	mu_module.switch_cur_dev(LKA_CURRENT_DEV::OFF);
	mpvn_modules[0].switch_cur_dev(LKA_CURRENT_DEV::OFF);
	mvku_modules[0].switch_cur_dev(LKA_CURRENT_DEV::OFF);
	vchm_chanels_init.clear();
	vchm_chanels_init << 0 << 0 << 0 << 0;
	set_working_chanels(vchm_chanels_init);
	set_new_tm();
	paint_buttons();
}

void LKA06_MODULE::restart_vchm_proc(int chanel)
{
	if (get_proc_working(chanel))
	{
		set_working_proc(chanel, 0);
		paint_buttons();
		po_start.start(VCHM_START_TIME);
	}
}

MU_MODULE::MU_MODULE() : current_dev(LKA_CURRENT_DEV::MAIN)
{
	working.insert(LKA_CURRENT_DEV::MAIN, true);
	working.insert(LKA_CURRENT_DEV::RESERVE, true);

	ab_working.insert(LKA_CURRENT_DEV::MAIN, true);
	ab_working.insert(LKA_CURRENT_DEV::RESERVE, true);

}

LKA_MV_MODULE::LKA_MV_MODULE(int _com, int _nim) : com(_com), nim(_nim), current_dev(LKA_CURRENT_DEV::MAIN)
{
	devices.insert(LKA_CURRENT_DEV::MAIN, LKA_MV_DEV());
	devices.insert(LKA_CURRENT_DEV::RESERVE, LKA_MV_DEV());
}

unsigned short MU_MODULE::get_tm()
{
	unsigned short _word = 0x1000;
	if (!get_working())
		_word += 4;
	_word += 0x20 << (unsigned short)current_dev;
	return _word;
}

unsigned short LKA_MV_MODULE::get_tm()
{
	unsigned short _word = (com << 12) + (nim << 8);
	if (!get_working())
		_word += 2 << int(current_dev);
	if (current_dev == LKA_CURRENT_DEV::OFF)
		_word += 0x10;
	else
	{
		if (com == 5)//МПВН всегда занят в первый раз
			_word += 0x10;
		_word += 0x20 << int(current_dev);
	}
	return _word;
}

unsigned short LKA_MV_MODULE::get_data_mvku()
{
	unsigned short _word = (0 << 12) + (nim << 8);
	if (!get_working())
		_word += 0x1000;
	if (ku_p != -1)
		_word += 1 << ku_p;
	return _word;
}

unsigned short LKA_MV_MODULE::get_data_mvmk()
{
	unsigned short _word = (0 << 12) + (nim << 8);
	if (!get_working())
		_word += 0x1000;
	if (ku_p != -1)
		_word += 1 << (ku_p + 4);
	if (ku_m != -1)
		_word += 1 << ku_m;
	return _word;
}


void LKA06_MODULE::set_working_chanels(QList<int> chanels_state, bool can_on)
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
		}
	}
}

void LKA06_MODULE::set_working_proc(int chanel, int state)
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
