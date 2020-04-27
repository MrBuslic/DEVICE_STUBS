#include "BKIS.h"

#include "rpc_ports.h"

#include <QMessageBox>

union MKOWord
{
	quint16 com_word;				 // командное слово целиком
	struct
	{
		quint16 count_word : 5,    // число сл.данных / команда
			subadr : 5,			 // подадрес
			trans_dir : 1,       // направление передачи(1-чт.ќ”)
			adr : 5;
	};
};

BKIS_widg::BKIS_widg(QWidget *parent)
{
	//QWidget* widg;
	//widg = new QWidget(this);
	setWindowTitle("BKIS");

	main_blk = new QPushButton("Вкл основной БЛК", this);
	main_blk->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed));
	reserve_blk = new QPushButton("Вкл резервный БЛК", this);
	reserve_blk->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed));
	main_interface = new QPushButton("Вкл основной интерфейс", this);
	main_interface->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed));
	reserve_interface = new QPushButton("Вкл резервный интерфейс", this);
	reserve_interface->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed));

	QVBoxLayout* lay = new QVBoxLayout(this);
	lay->addStretch(10);
	lay->addWidget(main_blk);
	lay->addWidget(reserve_blk);
	lay->addWidget(main_interface);
	lay->addWidget(reserve_interface);
	
	connect(main_blk, &QPushButton::clicked, this, main_blk->setStyleSheet("background-color: red"));

	
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
		QMessageBox::critical(0, "Нет соединения", "Ошибка соединения с mku");
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

	QSettings settings(QApplication::applicationDirPath() + "/positions.ini", QSettings::IniFormat);
	restoreGeometry(settings.value("bkis_geometry").toByteArray());

	blk_state = 0;
	interface_state = 0;
	for (int i = 1; i <= 14; i++) {
		electric_heaters_states[i] = 0;
	}

	for (int i = 1; i <= 6; i++) {
		pyro_groups_states[i] = 0;
	}
}
/*
void BKIS_widg::new_message(QVariant dt, int mko, int line, int cwd, QVariantList words, int os)
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
*/
void change_button_color(QPushButton button)
{
	button->setStyleSheet("background-color: red");
}


void BKIS_widg::new_mk(int mshm, int pshm, int length_m, int length_p, double u_m, double u_p, int dt, int line_m, int line_p)
{
	switch (tmp_mshm)
	{
	case 3:
		blk_state = 1;
		LKA_sett.setValue("LKA", 1);
		break;
	case 4:
		blk_state = 2;
		LKA_sett.setValue("LKA", 2);
		break;
	case 8:
		blk_state = 0;
		LKA_sett.setValue("KP", 1);
		break;
	case 9:
		current_KP = KP_2;
		LKA_sett.setValue("KP", 2);
		break;
	}
}

void BKIS_widg::set_blk_state(int state)
{
	blk_state = state;
}

void BKIS_widg::set_interface_state(bool state)
{
	interface_state = state;
}

int BKIS_widg::set_electric_heater_state(int name, bool state)
{
	electric_heaters_states[name] = state;
	return 0;
}

int BKIS_widg::pyro_group_set(int group_num, bool state)
{
	pyro_groups_states[group_num] = state;
	return 0;
}

void BKIS_widg::get_power(double _volt)
{
	volt = _volt;
	if (volt >= 20.0)
		imit_on();
	else
		if (volt < 1) imit_off();
}

void BKIS_widg::imit_off()
{

}

void BKIS_widg::imit_on()
{

}



void BKIS_widg::new_message(QVariant dt, int mko, int line, int cwd, QVariantList words, int os)
{
	MKOWord tmp_cwd;
	tmp_cwd.com_word = cwd;
	if (os == -1)
		return;
	if ((mko == MKO) && (tmp_cwd.adr == adr) && (tmp_cwd.trans_dir == 0))
	{
	
		update_graphics();
		set_new_tm();
	}
}

void BKIS_widg::update_graphics()
{

}

void BKIS_widg::set_new_tm()
{
	unsigned short _word = 0;
	QVariantList tmp_list;

	tmp_list.push_back(_word);
	slot_thr.get_omnibus_obj()->set_new_data(MKO, adr, 1, tmp_list);
}

void BKIS_widg::set_power_back()
{
	int curr = 1;
	power_slot_thr.get_power_bus_obj()->set_i(bus, name, curr);
}

void BKIS_widg::omni_connect()
{
	slot_thr.get_omnibus_obj()->switch_ab(MKO, adr, true);
	set_new_tm();
}

BKIS_widg::~BKIS_widg()
{
	slot_thr.quit();
	signal_thr.quit();

	mku_slot_thr.quit();
	mku_signal_thr.quit();

	power_slot_thr.quit();
	power_signal_thr.quit();
}

void BKIS_widg::closeEvent(QCloseEvent *event)
{
	QSettings settings(QApplication::applicationDirPath() + "/positions.ini", QSettings::IniFormat);
	settings.setValue("bkis_geometry", saveGeometry());
	QWidget::closeEvent(event);
}