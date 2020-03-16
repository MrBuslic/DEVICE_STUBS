#include "AOS.h"

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

AOS_widg::AOS_widg(QWidget *parent)
{
	setWindowTitle("АОС");

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

	// connect( mku_signal_thr.get_obj().get(), SIGNAL(new_ku_mk(int, int, double, int)), this, SLOT(new_ku_mk(int, int, double, int)) );
	
	connect( mku_signal_thr.get_obj().get(), SIGNAL(new_ku_mk( int name_ustroistva, int number_komplekta ) ), this, SLOT(new_ku_mk( int name_ustroistva, int number_komplekta ) ) );
	connect(power_signal_thr.get_obj().get(), SIGNAL(u_on_k1(double)), this, SLOT(get_power(double)));

	QSettings settings(QApplication::applicationDirPath() + "/positions.ini", QSettings::IniFormat);
	restoreGeometry(settings.value("aos_geometry").toByteArray());
	
	kontr_dev = AOS_KP_OFF; // отключение устройства контроллера устройства  (значение kontr_dev равно нуль)
	cgo_dev = AOS_KP_OFF;   // отключение устройства цифровой групповой обработки  (значение cgo_dev равно нуль)
	gpfm2_dev = AOS_KP_OFF; // отключение устройства группового приемника фазоманипулированных сигналов (2) (значение gpfm2_dev равно нуль)
	gpfm1_dev = AOS_KP_OFF; // отключение устройства группового приемника фазоманипулированных сигналов (1) (значение gpfm1_dev равно нуль)

	for (int i = 0; i < 32; i++)
	{
		gsu_kommut << GSU_kommut();
	}
	
}


void AOS_widg::imit_off()
{

}

void AOS_widg::imit_on()
{

}

void AOS_widg::new_ku_mk(int name_ustroistva, int number_komplekta)
{
   // printf("%d %c ", name_ustroistva, " = name_ustroistva \n");
   // printf("%d %c ", number_komplekta, " = number_komplekta \n");

	if (name_ustroistva == 8)
	  {
		
		if (number_komplekta == 0)
		{// отключить все 3 комплекта
			AOS_KP_OFF; // значение равно 0
		  return;
		};

		if (number_komplekta == 1)
		{// включить 1-й комплект
			// признак включения первого комплекта и передача этого признака отправителю
			AOS_KP_1; // значение равно 4

		//  return emit  make_mt_at_state( name_ustroistva, number_komplekta); // отправление информации отправителю от 1-го комплекта //
		};
		if (number_komplekta == 2)
		{// включить  2-й комплект
		    // признак включения второго комплекта и передача этого признака отправителю
			AOS_KP_2; // значение равно 2
		//  return emit make_mt_at_state( name_ustroistva, number_komplekta); //  отправление информации отправителю от 2-го комплекта //
		};
		if (number_komplekta == 3)
		{// включить  3-й комплект
		    // признак включения третьего комплекта и передача этого признака отправителю
			AOS_KP_3; // значение равно 1
		//  return emit make_mt_at_state( name_ustroistva, number_komplekta); // отправление информации отправителю от 3-го комплекта // 
		};

	  }
	else
	  {
		return;
	  }

	 
}


void AOS_widg::new_message(QVariant dt, int mko, int line, int cwd, QVariantList words, int os)
{
	MKOWord tmp_cwd;
	tmp_cwd.com_word = cwd;
	if (os == -1)
		return;
	if ((mko == MKO) && (tmp_cwd.adr == adr) && (tmp_cwd.trans_dir == 0))
	{
		if (tmp_cwd.subadr == 3)//Конфигурация АОС
		{   
			AOS_modes aos_modes;
			aos_modes._word = words.at(0).toInt();

			aos_state.kontr_mode = aos_modes.kontr_mode;
		}
		if (tmp_cwd.subadr == 4)//данные СхОС
		{
			for (int i = 0; i < 32; i++)
			{
				shos._words[i] = words.at(i).toInt();
			}
		}
		if (tmp_cwd.subadr == 5)//данные коммутаторов ГСУ
		{  
			// gsu_kommut.clear();
			for (int i = 0; i < 32; i++)
			{
				gsu_kommut[i]._word = words.at(i).toInt();
			}
		}


		if (tmp_cwd.subadr == 6) // ключ ППРЧ
		{   // проверить присутствия ключа ППРЧ
			for (int i = 0; i < 32; i++)
			{
				//pprch_res_key_data[i] = words.at(i).toInt();
			}
		}
		if (tmp_cwd.subadr == 7) // ключ ШПС
		{   // проверить присутствия ключа ШПС
			for (int i = 0; i < 32; i++)
			{
				//shps_main_key_data[i] = words.at(i).toInt();
			}
		}
		if (tmp_cwd.subadr == 8) // номер КА
		{
			n_ka = words.at(0).toInt();
		}


		update_graphics();
		set_new_tm();
	}
}


void AOS_widg::update_graphics()
{

}

void AOS_widg::set_new_tm()
{
	QVariantList tmp_list;
	for (int i =0 ; i < 7; i++)
		tmp_list.push_back(aos_state._words[i]);
	slot_thr.get_omnibus_obj()->set_new_data(MKO, adr, 14, tmp_list);
}
void AOS_widg::TMI_1_AOS()
{
	QVariantList tmp_list;
	for (int i = 0; i < 32; i++)
		tmp_list.push_back(aos_state._words[i]);
	slot_thr.get_omnibus_obj()->set_new_data(MKO, adr, 15, tmp_list);
}
void AOS_widg::TMI_2_AOS()
{
	QVariantList tmp_list;
	for (int i = 0; i < 32; i++)
		tmp_list.push_back(aos_state._words[i]);
	slot_thr.get_omnibus_obj()->set_new_data(MKO, adr, 16, tmp_list);
}

AOS_widg::~AOS_widg()
{
	slot_thr.quit();
	signal_thr.quit();

	mku_slot_thr.quit();
	mku_signal_thr.quit();

	power_slot_thr.quit();
	power_signal_thr.quit();
}

void AOS_widg::closeEvent(QCloseEvent *event)
{
	QSettings settings(QApplication::applicationDirPath() + "/positions.ini", QSettings::IniFormat);
	settings.setValue("aos_geometry", saveGeometry());
	QWidget::closeEvent(event);
}