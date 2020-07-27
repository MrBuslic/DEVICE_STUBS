#include "BKIS.h"
#include "bkis_socket_rpc.h"
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
/*
union OK_TMWord
{
	quint16 word;
}
*/
BKIS_widg::BKIS_widg(QWidget *parent)
{
	setWindowTitle("BKIS");
	for (int i = 0; i < 9; i++)
	{
		BLKIIH_word.ok_tm_word[i] = 0;
	}
		
	main_blk = new QPushButton("Вкл основной БЛК", this);
	main_blk->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed));
	reserve_blk = new QPushButton("Вкл резервный БЛК", this);
	reserve_blk->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed));
	main_interface = new QPushButton("Вкл основной интерфейс", this);
	main_interface->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed));
	reserve_interface = new QPushButton("Вкл резервный интерфейс", this);
	reserve_interface->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed));

	first_pyro_bus = new QPushButton("Вкл первую шину пиропатронов", this);
	first_pyro_bus->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed));
	second_pyro_bus = new QPushButton("Вкл вторую шину пиропатронов", this);
	second_pyro_bus->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed));
	third_pyro_bus = new QPushButton("Вкл третью шину пиропатронов", this);
	third_pyro_bus->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed));

	QGroupBox* blk_interface_gb = new QGroupBox(this);
	QGroupBox* blk_gb = new QGroupBox("БЛК", this);
	QGroupBox* interface_gb = new QGroupBox("Интерфейсы", this);
	QGroupBox* pyro_buses_gb = new QGroupBox("Шины пиропатронов", this);

	QHBoxLayout* main_lay = new QHBoxLayout(this);
	QVBoxLayout* lay_blk_interface = new QVBoxLayout(blk_interface_gb);
	QVBoxLayout* lay_blk = new QVBoxLayout(blk_gb);
	QVBoxLayout* lay_interface = new QVBoxLayout(interface_gb);
	QVBoxLayout* lay_pyro_buses = new QVBoxLayout(pyro_buses_gb);
	
	blk_gb->setAlignment(Qt::AlignHCenter);
	interface_gb->setAlignment(Qt::AlignHCenter);
	pyro_buses_gb->setAlignment(Qt::AlignHCenter);
	
	lay_blk_interface->addWidget(blk_gb);
	lay_blk_interface->addWidget(interface_gb);

	//----------Добавление кнопок----------
	lay_blk->addWidget(main_blk);
	lay_blk->addWidget(reserve_blk);
	lay_blk->setAlignment(Qt::AlignHCenter);

	lay_interface->addWidget(main_interface);
	lay_interface->addWidget(reserve_interface);
	lay_interface->setAlignment(Qt::AlignHCenter);
	
	lay_pyro_buses->addWidget(first_pyro_bus);
	lay_pyro_buses->addWidget(second_pyro_bus);
	lay_pyro_buses->addWidget(third_pyro_bus);
	lay_pyro_buses->setAlignment(Qt::AlignHCenter);
	//-------------------------------------

	main_lay->addWidget(blk_interface_gb);
	main_lay->addWidget(pyro_buses_gb);

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

	connect(mku_signal_thr.get_obj().get(), SIGNAL(new_ku(int, int, double, int)), this, SLOT(make_ku(int, int, double, int)));
	connect(power_signal_thr.get_obj().get(), SIGNAL(u_on_k1(double)), this, SLOT(get_power(double)));

	QString ip_str = "127.0.0.1";
	int slot_port = BKIS_SLOT;
	int signal_port = BKIS_SIGNAL;
	bkis_Socket_RPC_SLOT_Server_Thread* rpc_slot_srv = new bkis_Socket_RPC_SLOT_Server_Thread;
	rpc_slot_srv->set_app(this);
	rpc_slot_srv->set_params(ip_str, slot_port);
	rpc_slot_srv->start();
	bkis_Socket_RPC_SIGNAL_Thread* rpc_signal_srv = new bkis_Socket_RPC_SIGNAL_Thread;
	rpc_signal_srv->set_app(this);
	rpc_signal_srv->set_params(ip_str, signal_port);
	rpc_signal_srv->start();

	QSettings settings(QApplication::applicationDirPath() + "/positions.ini", QSettings::IniFormat);
	restoreGeometry(settings.value("bkis_geometry").toByteArray());

	blk_state = 0;
	interface_state = 0;

	for (int i = 1; i <= 6; i++) {
		pyro_groups_states[i] = 0;
	}

	//инициализация нагревателей для проверки
	for (int i = 1; i <= 14; i++) {
		electric_heaters_states[i] = i%2;
	}

	//инициализация состояния шин пиропатронов
	for (int i = 0; i < 3 ; i++) {
		pyro_buses_state[i] = 0;
	}
}

void BKIS_widg::make_ku(int ku_n, int length, double u, int line)
{
	switch (ku_n)
	{
	case 19:
		blk_state = 1;
		BLKIIH_word.s_main_blk_on = 1;
		BLKIIH_word.s_res_blk_on = 0;
		main_blk->setStyleSheet("background-color: rgb(142, 198, 156)"); 
		reserve_blk->setStyleSheet("background-color: rgb(204, 204, 204)");
		imit_on();
		break;
	case 20:
		blk_state = 2;
		BLKIIH_word.s_res_blk_on = 1;
		BLKIIH_word.s_main_blk_on = 0;
		main_blk->setStyleSheet("background-color: rgb(204, 204, 204)");
		reserve_blk->setStyleSheet("background-color: rgb(142, 198, 156)");
		imit_on();
		break;
	case 21:
		blk_state = 0;
		BLKIIH_word.s_res_blk_on = 0;
		BLKIIH_word.s_main_blk_on = 0;
		slot_thr.get_omnibus_obj()->switch_ab(MKO, adr, false);
		main_blk->setStyleSheet("background-color: rgb(204, 204, 204)");
		reserve_blk->setStyleSheet("background-color: rgb(204, 204, 204)");
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

int BKIS_widg::set_pyro_group_state(int group_num, bool state)
{
	if ((pyro_buses_state[0] + pyro_buses_state[1] + pyro_buses_state[2]) > 2)
	{
		pyro_groups_states[group_num] = state;
		return 0;
	}
	else
		return -1;
}

void BKIS_widg::set_pyro_bus_state(int bus_num, bool state)//bus_num - номера групп рэле
{
	pyro_buses_state[bus_num] = state;
}

void BKIS_widg::get_power(double _volt)
{
	volt = _volt;
	if (volt < 1) imit_off();
}

void BKIS_widg::imit_off()
{
	for (int i = 0; i < 9; i++)
	{
		BLKIIH_word.ok_tm_word[i] = 0;
	}
}

void BKIS_widg::imit_on()
{
	interface_state = 1; //включение основного внутреннего интерфейса
	BLKIIH_word.s_main_internal_interface_work = 1;
	BLKIIH_word.s_internal_interface_OK = 1;
	omni_connect();

	main_interface->setStyleSheet("background-color: rgb(142, 198, 156)");
	reserve_interface->setStyleSheet("background-color: rgb(204, 204, 204)");
	first_pyro_bus->setStyleSheet("background-color: rgb(204, 204, 204)");
	second_pyro_bus->setStyleSheet("background-color: rgb(204, 204, 204)");
	third_pyro_bus->setStyleSheet("background-color: rgb(204, 204, 204)");
}

void BKIS_widg::new_message(QVariant dt, int mko, int line, int cwd, QVariantList words, int os)
{
	MKOWord tmp_cwd;
	tmp_cwd.com_word = cwd;
	if (os == -1)
		return;
	if ((mko == MKO) && (tmp_cwd.adr == adr) && (tmp_cwd.trans_dir == 0))
	{

		int tmp_word = words[0].toInt();
		if (tmp_cwd.subadr == 2)
		{
			/* Переключение шин пиротехники */
			if (tmp_word >> 4 == 0x600)
			{
				int bus_num = tmp_word & 0x000F;
				if (bus_num == 4)
				{
					pyro_buses_state[0] = 0;
					pyro_buses_state[1] = 0;
					pyro_buses_state[2] = 0;
					first_pyro_bus->setStyleSheet("background-color: rgb(204, 204, 204)");
					second_pyro_bus->setStyleSheet("background-color: rgb(204, 204, 204)");
					third_pyro_bus->setStyleSheet("background-color: rgb(204, 204, 204)");
				}
				else {
					pyro_buses_state[bus_num - 1] = 1;//смещение на -1, т.к. нумерация массива состояний шин пиропатронов начинается с 0
					switch (bus_num) {
					case 1:
						first_pyro_bus->setStyleSheet("background-color: rgb(142, 198, 156)");
						break;
					case 2:
						second_pyro_bus->setStyleSheet("background-color: rgb(142, 198, 156)");
						break;
					case 3:
						third_pyro_bus->setStyleSheet("background-color: rgb(142, 198, 156)");
						break;
					}
				}
				return;
			}
				
			/*Подрыв пиропатронов*/
			if (tmp_word >> 4 == 0x660)
			{
				if ((pyro_buses_state[0] + pyro_buses_state[1] + pyro_buses_state[2]) >= 2)
				{
					int group_num = tmp_word & 0x000F;//Таким способом реализовано, так как последние 4 бита хранят данные о том, какая группа подрывается
					pyro_groups_states[group_num] = 1;
					emit send_pyro_group_activation(group_num);
					return;//todo для быстроты работы?
				}
			}
			
			if (tmp_word >> 6 == 0xCD)
			{
				switch (tmp_word & 0x003F)
				{
				case 9:
					BLKIIH_word.s_9_heater_work = 1;
					break;
				case 10:
					BLKIIH_word.s_10_heater_work = 1;
					break;
				case 11:
					BLKIIH_word.s_11_heater_work = 1;
					break;
				case 18:
					BLKIIH_word.s_18_heater_work = 1;
					break;
				case 19:
					BLKIIH_word.s_19_heater_work = 1;
					break;
				case 20:
					BLKIIH_word.s_20_heater_work = 1;
					break;
				case 21:
					BLKIIH_word.s_21_heater_work = 1;
					break;
				case 22:
					BLKIIH_word.s_22_heater_work = 1;
					break;
				case 23:
					BLKIIH_word.s_23_heater_work = 1;
					break;
				case 24:
					BLKIIH_word.s_24_heater_work = 1;
					break;
				case 25:
					BLKIIH_word.s_25_heater_work = 1;
					break;
				}
			}
			else if (tmp_word >> 6 == 0xCF) {
				switch (tmp_word & 0x003F)
				{
				case 9:
					BLKIIH_word.s_9_heater_work = 0;
					break;
				case 10:
					BLKIIH_word.s_10_heater_work = 0;
					break;
				case 11:
					BLKIIH_word.s_11_heater_work = 0;
					break;
				case 18:
					BLKIIH_word.s_18_heater_work = 0;
					break;
				case 19:
					BLKIIH_word.s_19_heater_work = 0;
					break;
				case 20:
					BLKIIH_word.s_20_heater_work = 0;
					break;
				case 21:
					BLKIIH_word.s_21_heater_work = 0;
					break;
				case 22:
					BLKIIH_word.s_22_heater_work = 0;
					break;
				case 23:
					BLKIIH_word.s_23_heater_work = 0;
					break;
				case 24:
					BLKIIH_word.s_24_heater_work = 0;
					break;
				case 25:
					BLKIIH_word.s_25_heater_work = 0;
					break;
				}
			}

			//вкл. основного/резервного внутреннего интерфейса
			switch (tmp_word)
			{
				//вкл. основного/резервного внутренних интерфейсов
			case 0x1444:
				interface_state = 1; //включение основного внутреннего интерфейса
				BLKIIH_word.s_main_internal_interface_work = 1;
				BLKIIH_word.s_internal_interface_OK = 1;
				main_interface->setStyleSheet("background-color: rgb(142, 198, 156)");
				reserve_interface->setStyleSheet("background-color: rgb(204, 204, 204)");
				break;
			case 0x1333:
				interface_state = 0;//включение резервного внутреннего интерфейса
				BLKIIH_word.s_internal_interface_OK = 1;
				main_interface->setStyleSheet("background-color: rgb(204, 204, 204)");
				reserve_interface->setStyleSheet("background-color: rgb(142, 198, 156)");
				break;
			}
		}
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
	/*
	for (QMap<int, int>::iterator it = pyro_groups_states.begin(); it != pyro_groups_states.end(); it++)
	{
		tmp_list << it.value();
	}
	//tmp_list.push_back(_word);//зачем?
	switch (blk_state)
	{
	case 0:
		_word = 0x8;
		break;
	case 1:
		_word = 0x10;
		break;
	}

	switch (interface_state)
	{
	case 0:
		_word += 0x20;
		break;
	case 1:
		_word += 0x100;
		break;
	}
	*/
	for (int i = 0; i < 19; i++)
		tmp_list.push_back(_word);

	for (int i = 0; i < 9; i++)
	{
		tmp_list.push_back(BLKIIH_word.ok_tm_word[i]);
	}
	
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