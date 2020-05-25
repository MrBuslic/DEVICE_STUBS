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

AOS_widg::AOS_widg(QWidget *parent) : QWidget(parent)
//AOS_widg::AOS_widg()
{
	setWindowTitle("АОС"); // вывод названия окна


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

	connect(signal_thr.get_obj().get(), 
		    SIGNAL(new_message(QVariant, int, int, int, QVariantList, int)), 
		    this, 
		    SLOT(new_message(QVariant, int, int, int, QVariantList, int))
	       );

	slot_thr.get_omnibus_obj()->switch_ab(MKO, adr, false);

	// connect( mku_signal_thr.get_obj().get(), SIGNAL(new_ku_mk(int, int, double, int)), this, SLOT(new_ku_mk(int, int, double, int)) );

	connect(mku_signal_thr.get_obj().get(), 
		    SIGNAL(new_ku_mk(int name_ustroistva, int number_komplekta)), 
		    this, 
		    SLOT(new_ku_mk(int name_ustroistva, int number_komplekta))
	       );

	// --------- добавление виджетов на форму ------- НАЧАЛО --------
    
	// -------------- группировка справочной информации ----------------------- начало ----------------
	nomer_vkluchaemogo_komplekta_Label = new QLabel("№ включенного комплекта AOC: ", this);
	nomer_vkluchaemogo_komplekta_samo_znachenie = new QLabel("0",this);
	QGroupBox *group_spravochnay_inform = new QGroupBox(this); // создание указателя на группировку справочной информации, которую мне присылают с "БАО"
	group_spravochnay_inform -> setFixedSize(220, 50);
	QGridLayout *vbox_Grid_sprav_inforn = new QGridLayout;     // отображение виджетов ввиде таблицы 2Х2.

    vbox_Grid_sprav_inforn -> addWidget(nomer_vkluchaemogo_komplekta_Label, 0, 0);
	vbox_Grid_sprav_inforn -> addWidget(nomer_vkluchaemogo_komplekta_samo_znachenie, 0, 1);
	group_spravochnay_inform -> setLayout(vbox_Grid_sprav_inforn);
	// -------------- группировка справочной информации ----------------------- конец ----------------

	// -------------- группировка данных для выбора комплектов устройств АОС и режим работы АОС -------------- начало -------------
	QGroupBox *group_ustroistv = new QGroupBox("Данные для выбора комплектов устройств АОС и режим работы АОС", this);   // создание указателя на группировку для кнопок комплектов устройств
	group_ustroistv -> setFixedSize(400, 270);
	QGridLayout *vbox_Grid_ustruistv = new QGridLayout; // отображение группировок ввиде таблицы 2Х2

	QGroupBox *group_GPFM1 = new QGroupBox("ГПФМ1", this);
	group_GPFM1 -> setFixedSize(190, 110);
	QVBoxLayout *vbox_Grid_GPFM1_knopok = new QVBoxLayout;

	QGroupBox *group_GPFM2 = new QGroupBox("ГПФМ2", this);
	group_GPFM2 -> setFixedSize(190, 110);
	QVBoxLayout *vbox_Grid_GPFM2_knopok = new QVBoxLayout;

	QGroupBox *group_ustroistvo_CGO = new QGroupBox("Устройство ЦГО", this);
	group_ustroistvo_CGO -> setFixedSize(190, 110);
	QVBoxLayout *vbox_Grid_ustroistvo_CGO_knopok = new QVBoxLayout;

	QGroupBox *group_ustroistvo_kontroly = new QGroupBox("Устройство контроля", this);
	group_ustroistvo_kontroly -> setFixedSize(190, 110);
	QVBoxLayout *vbox_Grid_group_ustroistvo_kontroly_knopok = new QVBoxLayout;

	vbox_Grid_ustruistv -> addWidget( group_GPFM1, 0, 0 );
	vbox_Grid_ustruistv -> addWidget( group_GPFM2, 0, 1 );
	vbox_Grid_ustruistv -> addWidget( group_ustroistvo_CGO, 1, 0 );
	vbox_Grid_ustruistv -> addWidget( group_ustroistvo_kontroly, 1, 1 );
	group_ustroistv -> setLayout( vbox_Grid_ustruistv );
	// -------------- группировка данных для выбора комплектов устройств АОС и режим работы АОС -------------- конец -------------

	// --------- группировка кнопок комплектов по устройсвам ----------- начало ---------------
		perviy_komplekt_GPFM1 = new QPushButton("Первый комплект", this);
		vtoroi_komplekt_GPFM1 = new QPushButton("Второй комплект", this);
		tretiy_komplekt_GPFM1 = new QPushButton("Третий комплект", this);
		  vbox_Grid_GPFM1_knopok -> addWidget(perviy_komplekt_GPFM1);
		  vbox_Grid_GPFM1_knopok -> addWidget(vtoroi_komplekt_GPFM1);
		  vbox_Grid_GPFM1_knopok -> addWidget(tretiy_komplekt_GPFM1);
		  group_GPFM1 -> setLayout(vbox_Grid_GPFM1_knopok);

		perviy_komplekt_GPFM2 = new QPushButton("Первый комплект", this);
		vtoroi_komplekt_GPFM2 = new QPushButton("Второй комплект", this);
		tretiy_komplekt_GPFM2 = new QPushButton("Третий комплект", this);
	      vbox_Grid_GPFM2_knopok -> addWidget(perviy_komplekt_GPFM2);
		  vbox_Grid_GPFM2_knopok -> addWidget(vtoroi_komplekt_GPFM2);
		  vbox_Grid_GPFM2_knopok -> addWidget(tretiy_komplekt_GPFM2);
		  group_GPFM2 -> setLayout(vbox_Grid_GPFM2_knopok);

		perviy_komplekt_CGO = new QPushButton("Первый комплект", this);
		vtorji_komplekt_CGO = new QPushButton("Второй комплект", this);
		tretiy_komplekt_CGO = new QPushButton("Третий комплект", this);
		  vbox_Grid_ustroistvo_CGO_knopok -> addWidget(perviy_komplekt_CGO);
		  vbox_Grid_ustroistvo_CGO_knopok -> addWidget(vtorji_komplekt_CGO);
		  vbox_Grid_ustroistvo_CGO_knopok -> addWidget(tretiy_komplekt_CGO);
		  group_ustroistvo_CGO -> setLayout(vbox_Grid_ustroistvo_CGO_knopok);

		perviy_komplekt_kontroly = new QPushButton("Первый комплект", this);
		vtoroi_komplekt_kontroly = new QPushButton("Второй комплект", this);
		tretiy_komplekt_kontroly = new QPushButton("Третий комплект", this);
		  vbox_Grid_group_ustroistvo_kontroly_knopok -> addWidget(perviy_komplekt_kontroly);
		  vbox_Grid_group_ustroistvo_kontroly_knopok -> addWidget(vtoroi_komplekt_kontroly);
		  vbox_Grid_group_ustroistvo_kontroly_knopok -> addWidget(tretiy_komplekt_kontroly);
		  group_ustroistvo_kontroly -> setLayout(vbox_Grid_group_ustroistvo_kontroly_knopok);	
	// --------- группировка кнопок комплектов по устройсвам ----------- конец ---------------

	// ----------- BIG  QVBoxLayout ------- начало ----- 
	QVBoxLayout *vbox_BIG_GroupBox = new QVBoxLayout(this);
	vbox_BIG_GroupBox -> addWidget(group_spravochnay_inform);
	vbox_BIG_GroupBox -> addWidget(group_ustroistv);
	// ----------- BIG  QVBoxLayout ------- конец -----

    // --------- закрашивание кнопок серым цветом --------- начало -------------
	perviy_komplekt_GPFM1->setStyleSheet(" background-color: rgb(204, 204, 204);");
	vtoroi_komplekt_GPFM1->setStyleSheet(" background-color: rgb(204, 204, 204);");
	tretiy_komplekt_GPFM1->setStyleSheet(" background-color: rgb(204, 204, 204);");

	perviy_komplekt_GPFM2->setStyleSheet(" background-color: rgb(204, 204, 204);");
	vtoroi_komplekt_GPFM2->setStyleSheet(" background-color: rgb(204, 204, 204);");
	tretiy_komplekt_GPFM2->setStyleSheet(" background-color: rgb(204, 204, 204);");

	perviy_komplekt_CGO->setStyleSheet(" background-color: rgb(204, 204, 204);");
	vtorji_komplekt_CGO->setStyleSheet(" background-color: rgb(204, 204, 204);");
	tretiy_komplekt_CGO->setStyleSheet(" background-color: rgb(204, 204, 204);");

	perviy_komplekt_kontroly->setStyleSheet(" background-color: rgb(204, 204, 204);");
	vtoroi_komplekt_kontroly->setStyleSheet(" background-color: rgb(204, 204, 204);");
	tretiy_komplekt_kontroly->setStyleSheet(" background-color: rgb(204, 204, 204);");
	// --------- закрашивание кнопок серым цветом --------- конец -------------
	setMaximumSize(420, 340); // фиксация размера окна
	// -------------------- искуственная вставка номера комплекта ----- начало ---------- !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	aos_modes.kontr = 0;
	aos_modes.cgo = 0;
	aos_modes.gpfm2 = 0;
	aos_modes.gpfm1 = 0;

	aos_state.kontr_dev_state = 0;
	aos_state.cgo_dev_state = 0;
	aos_state.gpfm2_dev_state = 0;
	aos_state.gpfm1_dev_state = 0;

	/* aos_modes.gpfm2 = 3;
	aos_modes.gpfm1 = 3;
	aos_modes.cgo = 3;
	aos_modes.kontr = 3; 

	/* aos_modes.gpfm1 = 1;
	aos_modes.gpfm2 = 1;
	aos_modes.cgo = 1;
	aos_modes.kontr = 1; // */

	aos_modes.gpfm1 = 3;
	aos_modes.gpfm2 = 1;
	aos_modes.cgo = 0;
	aos_modes.kontr = 2;

	number_komplekta_AOS = 123;

	funk_perescheta();
	update_graphics(); //*/
	// -------------------- искуственная вставка номера комплекта ----- конец ---------- !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// --------- добавление виджетов на форму ------- КОНЕЦ --------




	connect(power_signal_thr.get_obj().get(), SIGNAL(u_on_k1(double)), this, SLOT(get_power(double)));

	QSettings settings(QApplication::applicationDirPath() + "/positions.ini", QSettings::IniFormat);
	restoreGeometry(settings.value("aos_geometry").toByteArray());
	
	QString buf; // переменная, чтобы вывест полученный номер включённого комплекта на форму
	
	int number_komplekta = 3; // строка, которую необходимо в дальнейшем удалить!!!!!

	buf.setNum(number_komplekta); // перевод полученного номера из целого числа в строку
	Group_Vkl_kompl_AOS = new QGroupBox("Включённый комплект АОС", this);
	QLabel *Label_Imitator = new QLabel("№ комплекта: ");
	QLabel *Label_Imitator_number_kompl = new QLabel(buf); // вывод неосредственно самого номера комплекта (значения: 1 || 2 || 3).
	Group_Vkl_kompl_AOS->setMinimumWidth(165);
	Group_Vkl_kompl_AOS->setMinimumHeight(10);
	a = new QHBoxLayout(this);
	a->addWidget(Label_Imitator);
	a->addWidget(Label_Imitator_number_kompl);
	Group_Vkl_kompl_AOS->setLayout(a);

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
void AOS_widg::update_graphics()
{
	//AOS_modes aos_modes;
	// --------- закрашиваине зелёным цветом конопок, которые оказались активными --------- начало -------
	if (aos_modes.gpfm1 == 0)
	{
		nomer_vkluchaemogo_komplekta_samo_znachenie->setText(QString("%1").arg(number_komplekta_AOS));
		perviy_komplekt_GPFM1->setStyleSheet(" background-color: rgb(204, 204, 204);");
		vtoroi_komplekt_GPFM1->setStyleSheet(" background-color: rgb(204, 204, 204);");
		tretiy_komplekt_GPFM1->setStyleSheet(" background-color: rgb(204, 204, 204);");
	};
	if (aos_modes.gpfm1 == 1)
	{
		nomer_vkluchaemogo_komplekta_samo_znachenie->setText(QString("%1").arg(number_komplekta_AOS));
		perviy_komplekt_GPFM1->setStyleSheet(" background-color: rgb(142, 198, 156);");
	}
	else
	{
		nomer_vkluchaemogo_komplekta_samo_znachenie->setText(QString("%1").arg(number_komplekta_AOS));
		perviy_komplekt_GPFM1->setStyleSheet(" background-color: rgb(204, 204, 204);");
	};
	if (aos_modes.gpfm1 == 2)
	{
		nomer_vkluchaemogo_komplekta_samo_znachenie->setText(QString("%1").arg(number_komplekta_AOS));
		vtoroi_komplekt_GPFM1->setStyleSheet(" background-color: rgb(142, 198, 156);");
	}
	else
	{
		nomer_vkluchaemogo_komplekta_samo_znachenie->setText(QString("%1").arg(number_komplekta_AOS));
		vtoroi_komplekt_GPFM1->setStyleSheet(" background-color: rgb(204, 204, 204);");
	};
	if (aos_state.gpfm1_dev_state == 4)
	{
		nomer_vkluchaemogo_komplekta_samo_znachenie->setText(QString("%1").arg(number_komplekta_AOS));
		tretiy_komplekt_GPFM1->setStyleSheet(" background-color: rgb(142, 198, 156);");
	}
	else
	{
		nomer_vkluchaemogo_komplekta_samo_znachenie->setText(QString("%1").arg(number_komplekta_AOS));
		tretiy_komplekt_GPFM1->setStyleSheet(" background-color: rgb(204, 204, 204);");
	};
	// ----------------------------------------------------------------------------------
	if (aos_modes.gpfm2 == 0)
	{
		nomer_vkluchaemogo_komplekta_samo_znachenie->setText(QString("%1").arg(number_komplekta_AOS));
		perviy_komplekt_GPFM2->setStyleSheet(" background-color: rgb(204, 204, 204);");
		vtoroi_komplekt_GPFM2->setStyleSheet(" background-color: rgb(204, 204, 204);");
		tretiy_komplekt_GPFM2->setStyleSheet(" background-color: rgb(204, 204, 204);");
	};
	if (aos_modes.gpfm2 == 1)
	{
		nomer_vkluchaemogo_komplekta_samo_znachenie->setText(QString("%1").arg(number_komplekta_AOS));
		perviy_komplekt_GPFM2->setStyleSheet(" background-color: rgb(142, 198, 156);");
	}
	else
	{
		nomer_vkluchaemogo_komplekta_samo_znachenie->setText(QString("%1").arg(number_komplekta_AOS));
		perviy_komplekt_GPFM2->setStyleSheet(" background-color: rgb(204, 204, 204);");
	};
	if (aos_modes.gpfm2 == 2)
	{
		nomer_vkluchaemogo_komplekta_samo_znachenie->setText(QString("%1").arg(number_komplekta_AOS));
		vtoroi_komplekt_GPFM2->setStyleSheet(" background-color: rgb(142, 198, 156);");
	}
	else
	{
		nomer_vkluchaemogo_komplekta_samo_znachenie->setText(QString("%1").arg(number_komplekta_AOS));
		vtoroi_komplekt_GPFM2->setStyleSheet(" background-color: rgb(204, 204, 204);");
	};
	if (aos_state.gpfm2_dev_state == 4)
	{
		nomer_vkluchaemogo_komplekta_samo_znachenie->setText(QString("%1").arg(number_komplekta_AOS));
		tretiy_komplekt_GPFM2->setStyleSheet(" background-color: rgb(142, 198, 156);");
	}
	else
	{
		nomer_vkluchaemogo_komplekta_samo_znachenie->setText(QString("%1").arg(number_komplekta_AOS));
		tretiy_komplekt_GPFM2->setStyleSheet(" background-color: rgb(204, 204, 204);");
	};
	// ----------------------------------------------------------------------------------
	if (aos_modes.cgo == 0)
	{
		nomer_vkluchaemogo_komplekta_samo_znachenie->setText(QString("%1").arg(number_komplekta_AOS));
		perviy_komplekt_CGO->setStyleSheet(" background-color: rgb(204, 204, 204);");
		vtorji_komplekt_CGO->setStyleSheet(" background-color: rgb(204, 204, 204);");
		tretiy_komplekt_CGO->setStyleSheet(" background-color: rgb(204, 204, 204);");
	};
	if (aos_modes.cgo == 1)
	{
		nomer_vkluchaemogo_komplekta_samo_znachenie->setText(QString("%1").arg(number_komplekta_AOS));
		perviy_komplekt_CGO->setStyleSheet(" background-color: rgb(142, 198, 156);");
	}
	else
	{
		nomer_vkluchaemogo_komplekta_samo_znachenie->setText(QString("%1").arg(number_komplekta_AOS));
		perviy_komplekt_CGO->setStyleSheet(" background-color: rgb(204, 204, 204);");
	};
	if (aos_modes.cgo == 2)
	{
		nomer_vkluchaemogo_komplekta_samo_znachenie->setText(QString("%1").arg(number_komplekta_AOS));
		vtorji_komplekt_CGO->setStyleSheet(" background-color: rgb(142, 198, 156);");
	}
	else
	{
		nomer_vkluchaemogo_komplekta_samo_znachenie->setText(QString("%1").arg(number_komplekta_AOS));
		vtorji_komplekt_CGO->setStyleSheet(" background-color: rgb(204, 204, 204);");
	};
	if (aos_state.cgo_dev_state == 4)
	{
		nomer_vkluchaemogo_komplekta_samo_znachenie->setText(QString("%1").arg(number_komplekta_AOS));
		tretiy_komplekt_CGO->setStyleSheet(" background-color: rgb(142, 198, 156);");
	}
	else
	{
		nomer_vkluchaemogo_komplekta_samo_znachenie->setText(QString("%1").arg(number_komplekta_AOS));
		tretiy_komplekt_CGO->setStyleSheet(" background-color: rgb(204, 204, 204);");
	};
	// ----------------------------------------------------------------------------------
	if (aos_modes.kontr == 0)
	{
		nomer_vkluchaemogo_komplekta_samo_znachenie->setText(QString("%1").arg(number_komplekta_AOS));
		perviy_komplekt_kontroly->setStyleSheet(" background-color: rgb(204, 204, 204);");
		vtoroi_komplekt_kontroly->setStyleSheet(" background-color: rgb(204, 204, 204);");
		tretiy_komplekt_kontroly->setStyleSheet(" background-color: rgb(204, 204, 204);");
	};
	if (aos_modes.kontr == 1)
	{
		nomer_vkluchaemogo_komplekta_samo_znachenie->setText(QString("%1").arg(number_komplekta_AOS));
		perviy_komplekt_kontroly->setStyleSheet(" background-color: rgb(142, 198, 156);");
	}
	else
	{
		nomer_vkluchaemogo_komplekta_samo_znachenie->setText(QString("%1").arg(number_komplekta_AOS));
		perviy_komplekt_kontroly->setStyleSheet(" background-color: rgb(204, 204, 204);");
	};
	if (aos_modes.kontr == 2)
	{
		nomer_vkluchaemogo_komplekta_samo_znachenie->setText(QString("%1").arg(number_komplekta_AOS));
		vtoroi_komplekt_kontroly->setStyleSheet(" background-color: rgb(142, 198, 156);");
	}
	else
	{
		nomer_vkluchaemogo_komplekta_samo_znachenie->setText(QString("%1").arg(number_komplekta_AOS));
		vtoroi_komplekt_kontroly->setStyleSheet(" background-color: rgb(204, 204, 204);");
	};
	if (aos_state.kontr_dev_state == 4)
	{
		nomer_vkluchaemogo_komplekta_samo_znachenie->setText(QString("%1").arg(number_komplekta_AOS));
		tretiy_komplekt_kontroly->setStyleSheet(" background-color: rgb(142, 198, 156);");
	}
	else
	{
		nomer_vkluchaemogo_komplekta_samo_znachenie->setText(QString("%1").arg(number_komplekta_AOS));
		tretiy_komplekt_kontroly->setStyleSheet(" background-color: rgb(204, 204, 204);");
	};
	// --------- закрашиваине зелёным цветом конопок, которые оказались активными --------- конец -------
	return;

}

void AOS_widg::new_ku_mk(int name_ustroistva, int number_komplekta_AOS)
{
	// printf("%d %c ", name_ustroistva, " = name_ustroistva \n");
	// printf("%d %c ", number_komplekta, " = number_komplekta \n");
	
	if (name_ustroistva == 8)
	{
		funk_perescheta();
		if (number_komplekta_AOS == 0)
		{// отключить все 3 комплекта
			AOS_KP_OFF; // значение равно 0
			nomer_vkluchaemogo_komplekta_samo_znachenie->setText(QString("%1").arg(number_komplekta_AOS));

			update_graphics();
			return;
		};

		if (number_komplekta_AOS == 1)
		{// включить 1-й комплект
		 // признак включения первого комплекта и передача этого признака отправителю
			nomer_vkluchaemogo_komplekta_samo_znachenie->setText(QString("%1").arg(number_komplekta_AOS));

			update_graphics();
			AOS_KP_1; // значение равно 4
			return;
					  //  return emit  make_mt_at_state( name_ustroistva, number_komplekta); // отправление информации отправителю от 1-го комплекта //
		};
		if (number_komplekta_AOS == 2)
		{// включить  2-й комплект
		 // признак включения второго комплекта и передача этого признака отправителю
			nomer_vkluchaemogo_komplekta_samo_znachenie->setText(QString("%1").arg(number_komplekta_AOS));

			update_graphics();
			AOS_KP_2; // значение равно 2
			return;
					  //  return emit make_mt_at_state( name_ustroistva, number_komplekta); //  отправление информации отправителю от 2-го комплекта //
			
		};
		if (number_komplekta_AOS == 3)
		{// включить  3-й комплект
		 // признак включения третьего комплекта и передача этого признака отправителю
			nomer_vkluchaemogo_komplekta_samo_znachenie->setText(QString("%1").arg(number_komplekta_AOS));

			update_graphics();
			AOS_KP_3; // значение равно 1
			return;
					  //  return emit make_mt_at_state( name_ustroistva, number_komplekta); // отправление информации отправителю от 3-го комплекта // 
		};

	}
	else
	{
		return;
	}

}

void AOS_widg::funk_perescheta()
{
	//AOS_modes aos_modes;
	if (aos_modes.kontr == 3)
	{
		aos_state.kontr_dev_state = 4;   
	};

	if (aos_modes.cgo == 3)
	{
		aos_state.cgo_dev_state = 4;
	};

	if (aos_modes.gpfm2 == 3)
	{
		aos_state.gpfm2_dev_state = 4;
	};

	if (aos_modes.gpfm1 == 3)
	{
		aos_state.gpfm1_dev_state = 4;
	};

	if (number_komplekta_AOS == 3)
	{
		//number_komplekta_AOS == AOS_KP_1;
	};
	return;
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
			//AOS_modes aos_modes;
			aos_modes._word = words.at(0).toInt();

			aos_state.kontr_mode = aos_modes.kontr_mode;
			aos_state.kontr_dev_state = aos_modes.kontr;
			aos_state.cgo_dev_state = aos_modes.cgo;
			aos_state.gpfm2_dev_state = aos_modes.gpfm2;
			aos_state.gpfm1_dev_state = aos_modes.gpfm1;

			funk_perescheta();
			update_graphics();

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




void AOS_widg::set_new_tm() // состояние АОС
{
	QVariantList tmp_list;
	for (int i = 0; i < 7; i++)
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