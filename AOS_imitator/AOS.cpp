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
	// --------- подключение устройств ----- НАЧАЛО ------
	// ---------- подключение к МКО ------ начало -----------
	connect(signal_thr.get_obj().get(),
		SIGNAL(new_message(QVariant, int, int, int, QVariantList, int)),
		this,
		SLOT(new_message(QVariant, int, int, int, QVariantList, int))
	);
	// ---------- подключение к МКО ------ конец -----------
	slot_thr.get_omnibus_obj()->switch_ab(MKO, adr, false);

	// -------- подключение к матричным командам ------ начало ----------
	connect(mku_signal_thr.get_obj().get(),                               // указатель на объект, который отправляет сигнал
		SIGNAL(new_mt_at_state(int, int)), // тот сигнал с которым осуществляется соединение (тот кто отправляет)
		this,                                                         // указатель на тот объект который обработает сигнал (имеет слот для обработки сигналов)
		SLOT(new_ku_mk(int, int))    // то что вызывается при получении сигнала (тот кто получает)
	);
	// -------- подключение к матричным командам  ------ конец ----------

	// -------- подключение к электропитанию ----- начало ------
	connect(power_signal_thr.get_obj().get(),
		SIGNAL(u_on_k1(double)),
		this,
		SLOT(get_power(double))
	);
	// -------- подключение к электропитанию ----- конец ------
	// --------- подключение устройств ----- КОНЕЦ ------
	number_komplekta_AOS = 0; // значение по умолчанию
	aos_modes.kontr = 0;
	aos_modes.cgo = 0;
	aos_modes.gpfm2 = 0;
	aos_modes.gpfm1 = 0;
	aos_state.kontr_dev_state = 0;
	aos_state.cgo_dev_state = 0;
	aos_state.gpfm2_dev_state = 0;
	aos_state.gpfm1_dev_state = 0;
	aos_state.counter = 0;
	aos_state.res_counter = 0;

	aos_modes.kontr = 0;
	aos_modes.cgo = 0;
	aos_modes.gpfm2 = 0;
	aos_modes.gpfm1 = 0;

	
	QSettings settings(QApplication::applicationDirPath() + "/positions.ini", QSettings::IniFormat);
	restoreGeometry(settings.value("aos_geometry").toByteArray());
	

	kontr_dev = AOS_KP_OFF; // отключение устройства контроллера устройства  (значение kontr_dev равно нуль)
	cgo_dev = AOS_KP_OFF;   // отключение устройства цифровой групповой обработки  (значение cgo_dev равно нуль)
	gpfm2_dev = AOS_KP_OFF; // отключение устройства группового приемника фазоманипулированных сигналов (2) (значение gpfm2_dev равно нуль)
	gpfm1_dev = AOS_KP_OFF; // отключение устройства группового приемника фазоманипулированных сигналов (1) (значение gpfm1_dev равно нуль)
	
	risovanie_okna_AOS();
	for (int i = 0; i < 32; i++)
	{
		gsu_kommut << GSU_kommut();
	}

}
void AOS_widg::risovanie_okna_AOS() // и остальных виджетов АОС

{
	// --------- добавление виджетов на форму ------- НАЧАЛО --------
	// -------------- группировка справочной информации ----------------------- начало ----------------
	nomer_vkluchaemogo_komplekta_Label = new QLabel("№ включенного комплекта AOC: ", this);
	nomer_vkluchaemogo_komplekta_samo_znachenie = new QLabel("0", this);
	QGroupBox *group_spravochnay_inform = new QGroupBox(this); // создание указателя на группировку справочной информации, которую мне присылают с "БАО"
	group_spravochnay_inform->setFixedSize(220, 50);
	QGridLayout *vbox_Grid_sprav_inforn = new QGridLayout;     // отображение виджетов ввиде таблицы 2Х2.

	vbox_Grid_sprav_inforn->addWidget(nomer_vkluchaemogo_komplekta_Label, 0, 0);
	vbox_Grid_sprav_inforn->addWidget(nomer_vkluchaemogo_komplekta_samo_znachenie, 0, 1);
	group_spravochnay_inform->setLayout(vbox_Grid_sprav_inforn);
	// -------------- группировка справочной информации ----------------------- конец ----------------

	// -------------- группировка данных для выбора комплектов устройств АОС и режим работы АОС -------------- начало -------------
	QGroupBox *group_ustroistv = new QGroupBox("Данные для выбора комплектов устройств АОС и режим работы АОС", this);   // создание указателя на группировку для кнопок комплектов устройств
	group_ustroistv->setFixedSize(400, 270);
	QGridLayout *vbox_Grid_ustruistv = new QGridLayout; // отображение группировок ввиде таблицы 2Х2

	QGroupBox *group_GPFM1 = new QGroupBox("ГПФМ1", this);
	group_GPFM1->setFixedSize(190, 110);
	QVBoxLayout *vbox_Grid_GPFM1_knopok = new QVBoxLayout;

	QGroupBox *group_GPFM2 = new QGroupBox("ГПФМ2", this);
	group_GPFM2->setFixedSize(190, 110);
	QVBoxLayout *vbox_Grid_GPFM2_knopok = new QVBoxLayout;

	QGroupBox *group_ustroistvo_CGO = new QGroupBox("Устройство ЦГО", this);
	group_ustroistvo_CGO->setFixedSize(190, 110);
	QVBoxLayout *vbox_Grid_ustroistvo_CGO_knopok = new QVBoxLayout;

	QGroupBox *group_ustroistvo_kontroly = new QGroupBox("Устройство контроля", this);
	group_ustroistvo_kontroly->setFixedSize(190, 110);
	QVBoxLayout *vbox_Grid_group_ustroistvo_kontroly_knopok = new QVBoxLayout;

	vbox_Grid_ustruistv->addWidget(group_GPFM1, 0, 0);
	vbox_Grid_ustruistv->addWidget(group_GPFM2, 0, 1);
	vbox_Grid_ustruistv->addWidget(group_ustroistvo_CGO, 1, 0);
	vbox_Grid_ustruistv->addWidget(group_ustroistvo_kontroly, 1, 1);
	group_ustroistv->setLayout(vbox_Grid_ustruistv);
	// -------------- группировка данных для выбора комплектов устройств АОС и режим работы АОС -------------- конец -------------

	// --------- группировка кнопок комплектов по устройсвам ----------- начало ---------------
	perviy_komplekt_GPFM1 = new QPushButton("Первый комплект", this);
	vtoroi_komplekt_GPFM1 = new QPushButton("Второй комплект", this);
	tretiy_komplekt_GPFM1 = new QPushButton("Третий комплект", this);
	vbox_Grid_GPFM1_knopok->addWidget(perviy_komplekt_GPFM1);
	vbox_Grid_GPFM1_knopok->addWidget(vtoroi_komplekt_GPFM1);
	vbox_Grid_GPFM1_knopok->addWidget(tretiy_komplekt_GPFM1);
	group_GPFM1->setLayout(vbox_Grid_GPFM1_knopok);

	perviy_komplekt_GPFM2 = new QPushButton("Первый комплект", this);
	vtoroi_komplekt_GPFM2 = new QPushButton("Второй комплект", this);
	tretiy_komplekt_GPFM2 = new QPushButton("Третий комплект", this);
	vbox_Grid_GPFM2_knopok->addWidget(perviy_komplekt_GPFM2);
	vbox_Grid_GPFM2_knopok->addWidget(vtoroi_komplekt_GPFM2);
	vbox_Grid_GPFM2_knopok->addWidget(tretiy_komplekt_GPFM2);
	group_GPFM2->setLayout(vbox_Grid_GPFM2_knopok);

	perviy_komplekt_CGO = new QPushButton("Первый комплект", this);
	vtorji_komplekt_CGO = new QPushButton("Второй комплект", this);
	tretiy_komplekt_CGO = new QPushButton("Третий комплект", this);
	vbox_Grid_ustroistvo_CGO_knopok->addWidget(perviy_komplekt_CGO);
	vbox_Grid_ustroistvo_CGO_knopok->addWidget(vtorji_komplekt_CGO);
	vbox_Grid_ustroistvo_CGO_knopok->addWidget(tretiy_komplekt_CGO);
	group_ustroistvo_CGO->setLayout(vbox_Grid_ustroistvo_CGO_knopok);

	perviy_komplekt_kontroly = new QPushButton("Первый комплект", this);
	vtoroi_komplekt_kontroly = new QPushButton("Второй комплект", this);
	tretiy_komplekt_kontroly = new QPushButton("Третий комплект", this);
	vbox_Grid_group_ustroistvo_kontroly_knopok->addWidget(perviy_komplekt_kontroly);
	vbox_Grid_group_ustroistvo_kontroly_knopok->addWidget(vtoroi_komplekt_kontroly);
	vbox_Grid_group_ustroistvo_kontroly_knopok->addWidget(tretiy_komplekt_kontroly);
	group_ustroistvo_kontroly->setLayout(vbox_Grid_group_ustroistvo_kontroly_knopok);
	// --------- группировка кнопок комплектов по устройсвам ----------- конец ---------------
	// ----------- BIG  QVBoxLayout ------- начало ----- 
	QVBoxLayout *vbox_BIG_GroupBox = new QVBoxLayout(this);
	vbox_BIG_GroupBox->addWidget(group_spravochnay_inform);
	vbox_BIG_GroupBox->addWidget(group_ustroistv);
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
							  // --------- добавление виджетов на форму ------- КОНЕЦ --------

}
void AOS_widg::imit_off()
{
	kontr_dev = AOS_KP_OFF; // отключение устройства контроллера устройства  (значение kontr_dev равно нуль)
	cgo_dev = AOS_KP_OFF;   // отключение устройства цифровой групповой обработки  (значение cgo_dev равно нуль)
	gpfm2_dev = AOS_KP_OFF; // отключение устройства группового приемника фазоманипулированных сигналов (2) (значение gpfm2_dev равно нуль)
	gpfm1_dev = AOS_KP_OFF; // отключение устройства группового приемника фазоманипулированных сигналов (1) (значение gpfm1_dev равно нуль)
	slot_thr.get_omnibus_obj()->switch_ab(MKO, adr, false);
}

void AOS_widg::imit_on()
{

	slot_thr.get_omnibus_obj()->switch_ab(MKO, adr, true);

}
//void AOS_widg::update_graphics(int number_komplekta_AOS)
void AOS_widg::update_graphics()
{

	// --------- закрашиваине зелёным цветом конопок, которые оказались активными --------- начало -------
	if (aos_modes.gpfm1 == 0)
	{
		perviy_komplekt_GPFM1->setStyleSheet(" background-color: rgb(204, 204, 204);");
		vtoroi_komplekt_GPFM1->setStyleSheet(" background-color: rgb(204, 204, 204);");
		tretiy_komplekt_GPFM1->setStyleSheet(" background-color: rgb(204, 204, 204);");
	};

	if (aos_modes.gpfm1 == 1) { perviy_komplekt_GPFM1->setStyleSheet(" background-color: rgb(142, 198, 156);");}
	else { perviy_komplekt_GPFM1->setStyleSheet(" background-color: rgb(204, 204, 204);");};
	
	if (aos_modes.gpfm1 == 2) { vtoroi_komplekt_GPFM1->setStyleSheet(" background-color: rgb(142, 198, 156);");}
	else { vtoroi_komplekt_GPFM1->setStyleSheet(" background-color: rgb(204, 204, 204);");};
	
	if (aos_modes.gpfm1 == 3) { tretiy_komplekt_GPFM1->setStyleSheet(" background-color: rgb(142, 198, 156);");}
	else { tretiy_komplekt_GPFM1->setStyleSheet(" background-color: rgb(204, 204, 204);");};
	// ----------------------------------------------------------------------------------
	if (aos_modes.gpfm2 == 0)
	{
		perviy_komplekt_GPFM2->setStyleSheet(" background-color: rgb(204, 204, 204);");
		vtoroi_komplekt_GPFM2->setStyleSheet(" background-color: rgb(204, 204, 204);");
		tretiy_komplekt_GPFM2->setStyleSheet(" background-color: rgb(204, 204, 204);");
	};

	if (aos_modes.gpfm2 == 1) { perviy_komplekt_GPFM2->setStyleSheet(" background-color: rgb(142, 198, 156);");}
	else { perviy_komplekt_GPFM2->setStyleSheet(" background-color: rgb(204, 204, 204);");};
	
	if (aos_modes.gpfm2 == 2) { vtoroi_komplekt_GPFM2->setStyleSheet(" background-color: rgb(142, 198, 156);");}
	else { vtoroi_komplekt_GPFM2->setStyleSheet(" background-color: rgb(204, 204, 204);");};
	
	if (aos_modes.gpfm2 == 3) { tretiy_komplekt_GPFM2->setStyleSheet(" background-color: rgb(142, 198, 156);");}
	else { tretiy_komplekt_GPFM2->setStyleSheet(" background-color: rgb(204, 204, 204);");};
	// ----------------------------------------------------------------------------------
	if (aos_modes.cgo == 0)
	{
		perviy_komplekt_CGO->setStyleSheet(" background-color: rgb(204, 204, 204);");
		vtorji_komplekt_CGO->setStyleSheet(" background-color: rgb(204, 204, 204);");
		tretiy_komplekt_CGO->setStyleSheet(" background-color: rgb(204, 204, 204);");
	};

	if (aos_modes.cgo == 1)	{perviy_komplekt_CGO->setStyleSheet(" background-color: rgb(142, 198, 156);");}
	else { perviy_komplekt_CGO->setStyleSheet(" background-color: rgb(204, 204, 204);");};
	
	if (aos_modes.cgo == 2)	{vtorji_komplekt_CGO->setStyleSheet(" background-color: rgb(142, 198, 156);");}
	else { vtorji_komplekt_CGO->setStyleSheet(" background-color: rgb(204, 204, 204);");};
	
	if (aos_modes.cgo == 3){	tretiy_komplekt_CGO->setStyleSheet(" background-color: rgb(142, 198, 156);");}
	else { tretiy_komplekt_CGO->setStyleSheet(" background-color: rgb(204, 204, 204);");};
	// ----------------------------------------------------------------------------------
	if (aos_state.kontr_dev_state == 0)
	{
		perviy_komplekt_kontroly->setStyleSheet(" background-color: rgb(204, 204, 204);");
		vtoroi_komplekt_kontroly->setStyleSheet(" background-color: rgb(204, 204, 204);");
		tretiy_komplekt_kontroly->setStyleSheet(" background-color: rgb(204, 204, 204);");
	};

	if (aos_state.kontr_dev_state == 4){	perviy_komplekt_kontroly->setStyleSheet(" background-color: rgb(142, 198, 156);");}
	else { perviy_komplekt_kontroly->setStyleSheet(" background-color: rgb(204, 204, 204);");	};
	
	if (aos_state.kontr_dev_state == 2)	{ vtoroi_komplekt_kontroly->setStyleSheet(" background-color: rgb(142, 198, 156);");}
	else { vtoroi_komplekt_kontroly->setStyleSheet(" background-color: rgb(204, 204, 204);");};
	
	if (aos_state.kontr_dev_state == 1)	{ tretiy_komplekt_kontroly->setStyleSheet(" background-color: rgb(142, 198, 156);");}
	else { tretiy_komplekt_kontroly->setStyleSheet(" background-color: rgb(204, 204, 204);"); };
	// --------- закрашиваине зелёным цветом конопок, которые оказались активными --------- конец -------
	return;

}

void AOS_widg::new_ku_mk(int name_ustroistva, int number_komplekta_AOS)
{
	//nomer_vkluchaemogo_komplekta_samo_znachenie->setText(QString("%1").arg(number_komplekta_AOS));
	if (name_ustroistva == 8)
	{
		
		switch (number_komplekta_AOS)
		{
		case 0 :
		    // отключить все 3 комплекта
			aos_modes.kontr = 0;
			aos_modes.cgo = 0;
			aos_modes.gpfm2 = 0;
			aos_modes.gpfm1 = 0;
			imit_off();
			nomer_vkluchaemogo_komplekta_samo_znachenie->setText(QString("%1").arg(AOS_KP_OFF));
			AOS_number = AOS_KP_OFF;  // значение равно 0
			update_graphics();
			return;
			break;
		case 1 : 
			nomer_vkluchaemogo_komplekta_samo_znachenie->setText(QString("%1").arg(number_komplekta_AOS));
			AOS_number = number_komplekta_AOS;
			aos_state.aos_working = 1;
			aos_state.ukazatel_gotovnost_dannih_samokontroly_AOS_M_CH = 1;
			update_graphics();
			imit_on();
			//AOS_KP_1; // значение равно 4
			return;
			break;
		case 2:
			nomer_vkluchaemogo_komplekta_samo_znachenie->setText(QString("%1").arg(number_komplekta_AOS));
			AOS_number = number_komplekta_AOS;
			aos_state.aos_working = 1;
			aos_state.ukazatel_gotovnost_dannih_samokontroly_AOS_M_CH = 1;
			update_graphics();
			imit_on();
		//	AOS_KP_2; // значение равно 2
			return;
			break;
		case 3:
			nomer_vkluchaemogo_komplekta_samo_znachenie->setText(QString("%1").arg(number_komplekta_AOS));
			AOS_number = number_komplekta_AOS;
			aos_state.aos_working = 1;
			aos_state.ukazatel_gotovnost_dannih_samokontroly_AOS_M_CH = 1;
			update_graphics();
			imit_on();
			//AOS_KP_3; // значение равно 1
			return;
			break;
		};
	}
	else { return; }

}

void AOS_widg::funk_perescheta()
{   // необходима для чтения данных, поскольку мы значение 3 получаем как 011, что соответствует включению сразу 2-х комплектов (что не верно),
	// поэтому необходимо 3-ку превратить в 4-ку. Однако в это случае (согласно документу) мы включим первый комплект устройства, вместо третьего,
	// поэтому 3-му комплекту необходимо будет присвоить значение "1" (001), чтобы включить 3-й комплект. 
	//AOS_modes aos_modes;
	if (aos_modes.kontr == 3) { aos_state.kontr_dev_state = 1; }; 
	if (aos_modes.kontr == 1) { aos_state.kontr_dev_state = 4; };
	if (aos_modes.cgo == 3)	{ aos_state.cgo_dev_state = 1; };
	if (aos_modes.cgo == 1) { aos_state.cgo_dev_state = 4; };
	if (aos_modes.gpfm2 == 3) { aos_state.gpfm2_dev_state = 1; };
	if (aos_modes.gpfm2 == 1) { aos_state.gpfm2_dev_state = 4; };
	if (aos_modes.gpfm1 == 3) { aos_state.gpfm1_dev_state = 1; };
	if (aos_modes.gpfm1 == 1) { aos_state.gpfm1_dev_state = 4; };
	if (number_komplekta_AOS == 3)
	{
		//number_komplekta_AOS == AOS_KP_1;
	};
	return;
}
void AOS_widg::formirovanie_SD_kolv07(QVariantList words)

{
	// 1-e word ------------------------------- start ----------

	// aos_state.counter = 0;					   //  aos_state.counter       // 15 bit
	//if (aos_modes_old._word != aos_modes._word) { aos_state.counter += 1; };
	// 1-e word ------------------------------- end ------------

	// 2-e word ------------------------------------- начало -------------
	aos_state.kontr_dev_state =  aos_modes.kontr;                           // 3 bit
	aos_state.cgo_dev_state =  aos_modes.cgo;                                // 3 bit
	aos_state.gpfm2_dev_state =  aos_modes.gpfm2;                           // 3 bit
	aos_state.gpfm1_dev_state = aos_modes.gpfm1;                            // 3 bit
	// aos_state.res_sd2                                                   // 1 bit
	// aos_state.restart_watchdog                                           // 1 bit
	//  aos_state.aos_test_data_ready                                      // 1 bit  || значение получается
	if (aos_state.aos_test_data_ready == 1) { aos_state.kontr_mode = 1; }; /*aos_modes.kontr_mode; */                           // 1 bit
	//aos_state.kontr_mode = aos_modes.kontr_mode;
	// 2-e word ------------------------------------- конец -------------

	// здесь, в функции пересчёта, пересчитывается 3-й комплект, имеющий значение 3 (011), который после пересчёта имеет значение 4 (100)
	// однако после этого для читающего будет казаться, что мы включили не 3-й комплет, а первый, поэтому 4-ку нужно превратить в "1" (001), а 1-цу в "4" (100)
	funk_perescheta();
	update_graphics();

	// 3-e word ---------------------------- начало ---------
	if (aos_state.kontr_dev_state == 0) { aos_state.kontr_dev_working = 0; };  // 3 bit
	if (aos_state.kontr_dev_state == 1) { aos_state.kontr_dev_working = 1; };
	if (aos_state.kontr_dev_state == 2) { aos_state.kontr_dev_working = 3; }; // так как первый комплет был "типо" исправен, поэтому оставляем значение первого битика равным "1"
	if (aos_state.kontr_dev_state == 4) { aos_state.kontr_dev_working = 7; };

	if (aos_state.cgo_dev_state == 0) { aos_state.cgo_dev_working = 0; };
	if (aos_state.cgo_dev_state == 1) { aos_state.cgo_dev_working = 1; };  // 3 bit
	if (aos_state.cgo_dev_state == 2) { aos_state.cgo_dev_working = 3; }; // так как первый комплет был "типо" исправен, поэтому оставляем значение первого битика равным "1"
	if (aos_state.cgo_dev_state == 4) { aos_state.cgo_dev_working = 7; };

	if (aos_state.gpfm2_dev_state == 0) { aos_state.gpfm2_dev_working = 0; };  // 3 bit
	if (aos_state.gpfm2_dev_state == 1) { aos_state.gpfm2_dev_working = 1; };
	if (aos_state.gpfm2_dev_state == 2) { aos_state.gpfm2_dev_working = 3; }; // так как первый комплет был "типо" исправен, поэтому оставляем значение первого битика равным "1"
	if (aos_state.gpfm2_dev_state == 4) { aos_state.gpfm2_dev_working = 7; };

	if (aos_state.gpfm1_dev_state == 0) { aos_state.gpfm1_dev_working = 0; };
	if (aos_state.gpfm1_dev_state == 1) { aos_state.gpfm1_dev_working = 1; };  // 3 bit
	if (aos_state.gpfm1_dev_state == 2) { aos_state.gpfm1_dev_working = 3; }; // так как первый комплет был "типо" исправен, поэтому оставляем значение первого битика равным "1"
	if (aos_state.gpfm1_dev_state == 4) { aos_state.gpfm1_dev_working = 7; };


	if (aos_state.aos_test_data_ready == 1) { aos_state.vmu_bu_working = 1; }
	else { aos_state.vmu_bu_working = 0; };         // 1 bit 
	if (aos_state.aos_test_data_ready == 1) { aos_state.bu_working = 1; }
	else { aos_state.bu_working = 0; };             // 1 bit
	if (aos_state.aos_test_data_ready == 1) { aos_state.bu_test_data_ready = 1; }
	else { aos_state.bu_test_data_ready = 0; };    // 1 bit
	if (((aos_state.gpfm1_dev_state) || (aos_state.gpfm2_dev_state) ||
		(aos_state.cgo_dev_working) || (aos_state.kontr_dev_working) || (AOS_number)) != 0 ) { aos_state.aos_working = 1; }
	else { aos_state.aos_working = 0; };                                                   // 1 bit 
	// 3-e word ---------------------------- конец ---------

	// 4-e word ---------------------------- начало ---------
	//if (n_ka != 0) 
	{ aos_state.nka_buf_state = 1; };      // 1 bit
	aos_state.gsu_buf_state = 1;                   // 1 bit
	aos_state.shos_buf_state = 1;                 // 1 bit
	aos_state.shps_buf_state = 1;                  // 1 bit
	aos_state.pprch_buf_state = 1;                // 1 bit
	aos_state.priem_soobsheniy_key_sh_p_s = 1;     // 1 bit // откуда его можно получить?
	aos_state.priem_soobsheniy_key_p_p_r_ch = 1;  // 1 bit
	aos_state.rezerv_SD4_1 = 0;                                       // 3 bit
	 // aos_state.signal_IK8_UPI_BKUPI                                  // 1 bit
	 // aos_state.TMI_Signal_15e1827                                   // 1 bit
	 //if (aos_state.signal_IK8_UPI_BKUPI == 1) { aos_state.TMI_peredacha_signala_IK8_v_magistralnom_tracte = 1; };     // 2 bit
	aos_state.rezerv_SD4_2 = 0;                                       // 2 bit
	// 4-e word ---------------------------- конец ---------

	// 5-e word ---------------------------- начало ---------
	aos_state.rezerv_SD5_1 = 0;                                                                           // 5 bit
	aos_state.sinhronizatsiy_G_T_CH_C_CGO = 1; // 
    // aos_state.signal_OG_v_ustroistve_CGO
	aos_state.rezerv_SD5_2 = 1;                                                                           // 1 bit
	aos_state.rezerv_SD5_3 = 0;                                                                           // 2 bit
	if (aos_state.aos_test_data_ready == 1) { aos_state.rabotosposobnost_intef_VMU_BU_AOS = 1; }
	else { aos_state.rabotosposobnost_intef_VMU_BU_AOS = 0; };          // 1 bit !!!!!!!!!!!!!!!!!!!!!!!!!!
	if ( (( aos_state.kontr_dev_state) || (AOS_number) ) != 0) { aos_state.rabotosposobnost_intef_VMU_ustroistva_kontrily = 1; }
	else { aos_state.rabotosposobnost_intef_VMU_ustroistva_kontrily = 0; };  // 1 bit
	if ( ((aos_state.cgo_dev_state) || (AOS_number)) != 0) { aos_state.rabotosposobnost_intef_VMU_ustroistva_CGO = 1; }
	else { aos_state.rabotosposobnost_intef_VMU_ustroistva_CGO = 0; };          // 1 bit
	if ( ((aos_state.gpfm2_dev_state) || (AOS_number)) != 0) { aos_state.rabotosposobnost_intef_VMU_PRM_trakta_AFS2 = 1; }
	else { aos_state.rabotosposobnost_intef_VMU_PRM_trakta_AFS2 = 0; };      // 1 bit
	if ( ((aos_state.gpfm1_dev_state) || (AOS_number)) != 0 ) { aos_state.rabotosposobnost_intef_VMU_PRM_trakta_AFS1 = 1; }
	else { aos_state.rabotosposobnost_intef_VMU_PRM_trakta_AFS1 = 0; };      // 1 bit
	aos_state.rezerv_SD5_4 = 1;                                                                           // 1 bit	
	 // 5-e word ---------------------------- конец ---------		

	// 6-e word ---------------------------- начало ---------
	aos_state.rezerv_SD6_1 = 0;                                // 4 bit
	//aos_state.signala_na_vhode_v_priemnik_tracta_AFS2 ;      // 1 bit
	//aos_state.sinhronizatsiy_G_T_CH_priemnika_trakta_AFS2; // 1 bit
	// aos_state.signal_OG_v_priemnike_trakta_AFS2              // 1 bit
	// aos_state.rezim_raboti_priemnika_trakta_AFS2            // 1 bit
	aos_state.rezerv_SD6_2 = 0;                                 // 4 bit
	//aos_state.signal_P_CH_na_vhode_v_priemnik_trakta_AFS2 ;  // 1 bit
	//aos_state.sunhronizatsiy_G_T_CH_priemnika_trakta_AFS1 ;   // 1 bit
	// aos_state.signal_OG_v_priemnike_trakta_AFS1             // 1 bit
																// aos_state.rezhim_raboti_priemnika_trakta_AFS1            // 1 bit
    // 6-e word ---------------------------- конец ---------	

	// 7-e word ------------------------------- начало ---------
	aos_state.rezerv_SD7_1 = 0;                                                                                  // 4 bit
	if (aos_modes.kontr != 0) { aos_state.TMI_rabotosposobnost_UK = 1; };                                          // 1 bit
    // aos_state.TMI_sinhronizatsiy_G_T_CH_UK                                                                    // 1 bit
    // aos_state.TMI_signal_OG_v_ustroistve_kontroly_UK                                                           // 1 bit
	aos_state.rezerv_SD7_2 = 1;                                                                                  // 1 bit
	aos_state.rezerv_SD7_3 = 0;                                                                                   // 1 bit
	if (aos_state.cgo_dev_state != 0) { aos_state.TMI_rabotosposobnost_ustroistva_CGO = 1; }
	else { aos_state.TMI_rabotosposobnost_ustroistva_CGO = 0; };                    // 1 bit
	aos_state.rezerv_SD7_4 = 0;                                                                                   // 3 bit
	if (aos_state.gpfm2_dev_state != 0) { aos_state.TMI_rabotosposobnost_priemnika_trakta_AFS2 = 1; }
	else { aos_state.TMI_rabotosposobnost_priemnika_trakta_AFS2 = 0; };           // 1 bit
	if (aos_state.gpfm1_dev_state != 0) { aos_state.TMI_rabotosposobnost_priemnika_trakta_AFS1 = 1; }
	else { aos_state.TMI_rabotosposobnost_priemnika_trakta_AFS1 = 0; };            // 1 bit
	if (aos_state.aos_test_data_ready != 0) { aos_state.ukazatel_gotovnost_dannih_samokontroly_AOS_M_CH = 1; }
	else { aos_state.ukazatel_gotovnost_dannih_samokontroly_AOS_M_CH = 0; };  // 1 bit		
	// 7-e word ------------------------------- конец ---------
};


void AOS_widg::formirovanie_SD_kolv07_3(AOS_modes aos_modes)
{
	bool my_flag = false; // отвечает за изменения в СД 4 - 7
	// 1-e word ------------------------------- start ----------

	//aos_state.counter = 0;	//  aos_state.counter       // 15 bit
	if (aos_modes_old._word != aos_modes._word) 
	{
		aos_state.counter += 1; 
		aos_state.res_counter = 0;
	};
	if ((aos_state.counter / 3) == 1) { aos_state.res_counter = 1; };
	for (int i = 1; i < i; i++) if (aos_state_old._words[i] != aos_state._words[i]) { my_flag = true; };
	if(my_flag == true)
	{
		aos_state.counter += 1;
		aos_state.res_counter = 0;
	};
    // 1-e word ------------------------------- end ------------

    // 2-e word ------------------------------------- начало -------------
	aos_state.kontr_dev_state = aos_modes.kontr;                           // 3 bit
	aos_state.cgo_dev_state = aos_modes.cgo;                                // 3 bit
	aos_state.gpfm2_dev_state = aos_modes.gpfm2;                           // 3 bit
	aos_state.gpfm1_dev_state = aos_modes.gpfm1;                            // 3 bit
	// aos_state.res_sd2                                                   // 1 bit
    // aos_state.restart_watchdog                                           // 1 bit
	//  aos_state.aos_test_data_ready                                      // 1 bit  || значение получается
	//if (aos_state.aos_test_data_ready == 1) { aos_state.kontr_mode = 1; }; //aos_modes.kontr_mode;                            // 1 bit
	//aos_state.kontr_mode = aos_modes.kontr_mode;
	// 2-e word ------------------------------------- конец -------------
	 
	// здесь, в функции пересчёта, пересчитывается 3-й комплект, имеющий значение 3 (011), который после пересчёта имеет значение 4 (100)
	// однако после этого для читающего будет казаться, что мы включили не 3-й комплет, а первый, поэтому 4-ку нужно превратить в "1" (001), а 1-цу в "4" (100)
	funk_perescheta();
	//update_graphics(number_komplekta_AOS);

	// 3-e word ---------------------------- начало ---------
	//if (aos_state.kontr_dev_state == 0) { aos_state.kontr_dev_working = 0; };
	if (aos_state.kontr_dev_state == 1) { aos_state.kontr_dev_working = 1; };
	if (aos_state.kontr_dev_state == 2) { aos_state.kontr_dev_working = 3; }; // так как первый комплет был "типо" исправен, поэтому оставляем значение первого битика равным "1"
	if (aos_state.kontr_dev_state == 4) { aos_state.kontr_dev_working = 7; };

	//if (aos_state.cgo_dev_state == 0) { aos_state.cgo_dev_working = 0; };
	if (aos_state.cgo_dev_state == 1) { aos_state.cgo_dev_working = 1; };
	if (aos_state.cgo_dev_state == 2) { aos_state.cgo_dev_working = 3; }; // так как первый комплет был "типо" исправен, поэтому оставляем значение первого битика равным "1"
	if (aos_state.cgo_dev_state == 4) { aos_state.cgo_dev_working = 7; };

	//if (aos_state.gpfm2_dev_state == 0) { aos_state.gpfm2_dev_working = 0; };
	if (aos_state.gpfm2_dev_state == 1) { aos_state.gpfm2_dev_working = 1; };
	if (aos_state.gpfm2_dev_state == 2) { aos_state.gpfm2_dev_working = 3; }; // так как первый комплет был "типо" исправен, поэтому оставляем значение первого битика равным "1"
	if (aos_state.gpfm2_dev_state == 4) { aos_state.gpfm2_dev_working = 7; };

	//if (aos_state.gpfm1_dev_state == 0) { aos_state.gpfm1_dev_working = 0; };
	if (aos_state.gpfm1_dev_state == 1) { aos_state.gpfm1_dev_working = 1; };
	if (aos_state.gpfm1_dev_state == 2) { aos_state.gpfm1_dev_working = 3; }; // так как первый комплет был "типо" исправен, поэтому оставляем значение первого битика равным "1"
	if (aos_state.gpfm1_dev_state == 4) { aos_state.gpfm1_dev_working = 7; };
	//aos_state.kontr_dev_working = 7;// aos_state.kontr_dev_state;                           // 3 bit
	//aos_state.cgo_dev_working = 7;// aos_state.cgo_dev_state;                                // 3 bit
	//aos_state.gpfm2_dev_working = 7;// aos_state.gpfm2_dev_state;                           // 3 bit
	//aos_state.gpfm1_dev_working = 7;// aos_state.gpfm1_dev_state;                            // 3 bit
	if (aos_state.aos_test_data_ready == 1) { aos_state.vmu_bu_working = 1; }
	else { aos_state.vmu_bu_working = 0; };         // 1 bit 
	if (aos_state.aos_test_data_ready == 1) { aos_state.bu_working = 1; }
	else { aos_state.bu_working = 0; };             // 1 bit
	if (aos_state.aos_test_data_ready == 1) { aos_state.bu_test_data_ready = 1; }
	else { aos_state.bu_test_data_ready = 0; };    // 1 bit
	if (((aos_state.gpfm1_dev_state) || (aos_state.gpfm2_dev_state) ||
		(aos_state.cgo_dev_working) || (aos_state.kontr_dev_working) || (AOS_number)) != 0) {
		aos_state.aos_working = 1;
	}
	else { aos_state.aos_working = 0; };                                                   // 1 bit 
    // 3-e word ---------------------------- конец ---------

    // 4-e word ---------------------------- начало ---------
	if (n_ka != 0) { aos_state.nka_buf_state = 1; };      // 1 bit
	aos_state.gsu_buf_state = 1;                   // 1 bit
	aos_state.shos_buf_state = 1;                 // 1 bit
	aos_state.shps_buf_state = 1;                  // 1 bit
	aos_state.pprch_buf_state = 1;                // 1 bit
	aos_state.priem_soobsheniy_key_sh_p_s = 1;     // 1 bit // откуда его можно получить?
	aos_state.priem_soobsheniy_key_p_p_r_ch = 1;  // 1 bit
	aos_state.rezerv_SD4_1 = 0;                                       // 3 bit
	// aos_state.signal_IK8_UPI_BKUPI                                  // 1 bit
	// aos_state.TMI_Signal_15e1827                                   // 1 bit
	if (aos_state.signal_IK8_UPI_BKUPI == 1) { aos_state.TMI_peredacha_signala_IK8_v_magistralnom_tracte = 1; };     // 2 bit
	if (aos_state.TMI_Signal_15e1827 == 1) { aos_state.TMI_peredacha_signala_IK8_v_magistralnom_tracte = 2; };
	if ((aos_state.signal_IK8_UPI_BKUPI == 1) && (aos_state.TMI_Signal_15e1827 == 1)) { aos_state.TMI_peredacha_signala_IK8_v_magistralnom_tracte = 3; }; //
	aos_state.rezerv_SD4_2 = 0;                                       // 2 bit
    // 4-e word ---------------------------- конец ---------

    // 5-e word ---------------------------- начало ---------
	aos_state.rezerv_SD5_1 = 0;                                                                           // 5 bit
    // aos_state.sinhronizatsiy_G_T_CH_C_CGO  // 
    // aos_state.signal_OG_v_ustroistve_CGO
	aos_state.rezerv_SD5_2 = 1;                                                                           // 1 bit
	aos_state.rezerv_SD5_3 = 0;                                                                           // 2 bit
	if (aos_state.aos_test_data_ready == 1) { aos_state.rabotosposobnost_intef_VMU_BU_AOS = 1; }
	else { aos_state.rabotosposobnost_intef_VMU_BU_AOS = 0; };          // 1 bit !!!!!!!!!!!!!!!!!!!!!!!!!!
	if (((aos_state.kontr_dev_state) || (AOS_number)) != 0) { aos_state.rabotosposobnost_intef_VMU_ustroistva_kontrily = 1; }
	else { aos_state.rabotosposobnost_intef_VMU_ustroistva_kontrily = 0; };  // 1 bit
	if (((aos_state.cgo_dev_state) || (AOS_number)) != 0) { aos_state.rabotosposobnost_intef_VMU_ustroistva_CGO = 1; }
	else { aos_state.rabotosposobnost_intef_VMU_ustroistva_CGO = 0; };          // 1 bit
	if (((aos_state.gpfm2_dev_state) || (AOS_number)) != 0) { aos_state.rabotosposobnost_intef_VMU_PRM_trakta_AFS2 = 1; }
	else { aos_state.rabotosposobnost_intef_VMU_PRM_trakta_AFS2 = 0; };      // 1 bit
	if (((aos_state.gpfm1_dev_state) || (AOS_number)) != 0) { aos_state.rabotosposobnost_intef_VMU_PRM_trakta_AFS1 = 1; }
	else { aos_state.rabotosposobnost_intef_VMU_PRM_trakta_AFS1 = 0; };      // 1 bit
	aos_state.rezerv_SD5_4 = 1;                                                                           // 1 bit	
    // 5-e word ---------------------------- конец ---------		

    // 6-e word ---------------------------- начало ---------
	aos_state.rezerv_SD6_1 = 0;                                // 4 bit
    // aos_state.signala_na_vhode_v_priemnik_tracta_AFS2        // 1 bit
    // aos_state.sinhronizatsiy_G_T_CH_priemnika_trakta_AFS2   // 1 bit
    // aos_state.signal_OG_v_priemnike_trakta_AFS2              // 1 bit
    // aos_state.rezim_raboti_priemnika_trakta_AFS2            // 1 bit
	aos_state.rezerv_SD6_2 = 0;                                 // 4 bit
    // aos_state.signal_P_CH_na_vhode_v_priemnik_trakta_AFS2   // 1 bit
    // aos_state.sunhronizatsiy_G_T_CH_priemnika_trakta_AFS1    // 1 bit
    // aos_state.signal_OG_v_priemnike_trakta_AFS1             // 1 bit
    // aos_state.rezhim_raboti_priemnika_trakta_AFS1            // 1 bit
    // 6-e word ---------------------------- конец ---------	

   // 7-e word ------------------------------- начало ---------
	aos_state.rezerv_SD7_1 = 0;                                                                                  // 4 bit
	//if (aos_modes.kontr != 0) 
	//{ aos_state.TMI_rabotosposobnost_UK = 1; };                                          // 1 bit
	 // aos_state.TMI_sinhronizatsiy_G_T_CH_UK                                                                    // 1 bit
	 // aos_state.TMI_signal_OG_v_ustroistve_kontroly_UK                                                           // 1 bit
	aos_state.rezerv_SD7_2 = 1;                                                                                  // 1 bit
	aos_state.rezerv_SD7_3 = 0;                                                                                   // 1 bit
	//if (aos_state.cgo_dev_state != 0) 
	//{ aos_state.TMI_rabotosposobnost_ustroistva_CGO = 1; }
	//else 
	//{ aos_state.TMI_rabotosposobnost_ustroistva_CGO = 0; };                    // 1 bit
	aos_state.rezerv_SD7_4 = 0;                                                                                   // 3 bit
	//if (aos_state.gpfm2_dev_state != 0) 
	//{ aos_state.TMI_rabotosposobnost_priemnika_trakta_AFS2 = 1; }
	//else 
	//{ aos_state.TMI_rabotosposobnost_priemnika_trakta_AFS2 = 0; };           // 1 bit
	//if (aos_state.gpfm1_dev_state != 0) { aos_state.TMI_rabotosposobnost_priemnika_trakta_AFS1 = 1; }
	//else 
	//{ aos_state.TMI_rabotosposobnost_priemnika_trakta_AFS1 = 0; };            // 1 bit
	//if (aos_state.aos_test_data_ready == 1) 
	//{ aos_state.ukazatel_gotovnost_dannih_samokontroly_AOS_M_CH = 1; }
	//else 
	//{ aos_state.ukazatel_gotovnost_dannih_samokontroly_AOS_M_CH = 0; };  // 1 bit		
	// 7-e word ------------------------------- конец ---------
};

void AOS_widg::new_message(QVariant dt, int mko, int line, int cwd, QVariantList words, int os)
{
	MKOWord tmp_cwd;
	tmp_cwd.com_word = cwd;
	if (os == -1)return;


	if ((mko == MKO) && (tmp_cwd.adr == adr) && ((tmp_cwd.trans_dir == 1)  || (tmp_cwd.trans_dir == 0)))
	{
		switch (tmp_cwd.subadr)
		{
		case 3 : //Конфигурация АОС
			aos_modes._word = words.at(0).toInt();
			formirovanie_SD_kolv07_3(aos_modes);
			aos_modes_old._word = words.at(0).toInt();
			set_new_tm(); // чтение состояний аос
			break;
		case 4: //данные СхОС
			for (int i = 0; i < 32; i++) { shos._words[i] = words.at(i).toInt(); };
			if ((AOS_PPRCH.osn_key_pprch1 || AOS_PPRCH.osn_key_pprch2 || AOS_PPRCH.osn_key_pprch3 || AOS_PPRCH.osn_key_pprch4 ||
				AOS_PPRCH.osn_key_pprch5 || AOS_PPRCH.osn_key_pprch6 || AOS_PPRCH.osn_key_pprch7 || AOS_PPRCH.osn_key_pprch8) == 1) { shos.pprch_key = 1; }
			else { shos.pprch_key = 0; };

			if ((AOS_shps.osn_key_shos1 || AOS_shps.osn_key_shos2 || AOS_shps.osn_key_shos3 || AOS_shps.osn_key_shos4 ||
				AOS_shps.osn_key_shos5 || AOS_shps.osn_key_shos6 || AOS_shps.osn_key_shos7 || AOS_shps.osn_key_shos8) == 1) { shos.shps_key = 1; }
			else { shos.shps_key = 0; };

			// зануление резерва, согласно документу -------------- начало ----------------
			shos.res1 = 0; // 2 bit
			shos.res2 = 0; // 10 bit
			shos.res_12 = 0; // 3 bit
			shos.res_01 = 0; // 3 bit 
			shos.res3 = 0; // 4 bit
			for (int i = 25; i < 32; i++) { shos._words[i] = 0; };
			// зануление резерва, согласно документу -------------- конец ----------------
			TMI_1_AOS(); // чтение данных СхОС
			break;
		case 5: //данные коммутаторов ГСУ
			for (int i = 0; i < 32; i++) { gsu_kommut[i]._word = words.at(i).toInt();	};
			// зануление 16, 15, 14 битов в словах с первого по 24-е -------- начало --------
			for (int i = 0; i < 25; i++) { gsu_kommut[i].res = 0; };
			// зануление 16, 15, 14 битов в словах с первого по 24-е -------- конец --------
			// зануление резерва, согласно документу -------------- начало ----------------
			for (int i = 25; i < 32; i++) { gsu_kommut[i]._word = 0; }
			// зануление резерва, согласно документу -------------- конец ----------------
			TMI_2_AOS(); // чтение данных ГСУ
			break;
		case 6: // ключ ППРЧ
			for (int i = 0; i < 32; i++)
			{
				pprch_main_key_data[i] = shos.pprch_chans;
				pprch_main_key_data[i] = words.at(i).toInt();
				AOS_PPRCH._words_p[i] = words.at(i).toInt();
			};
			if ((AOS_PPRCH.osn_key_pprch1 || AOS_PPRCH.osn_key_pprch2 || AOS_PPRCH.osn_key_pprch3 || AOS_PPRCH.osn_key_pprch4 ||
				AOS_PPRCH.osn_key_pprch5 || AOS_PPRCH.osn_key_pprch6 || AOS_PPRCH.osn_key_pprch7 || AOS_PPRCH.osn_key_pprch8) == 1) { shos.pprch_key = 1;}
			else { shos.pprch_key = 0; };

			// зануление резерва, согласно документу -------------- начало ----------------
			AOS_PPRCH.rez1 = 0; // 8 bit
			AOS_PPRCH.rez2 = 0; // 8 bit
			for (int i = 19; i < 28; i++)
			{
				AOS_PPRCH._words_p[i] = 0;
				pprch_main_key_data[i] = 0;
			};
			for (int i = 30; i < 32; i++)
			{
				AOS_PPRCH._words_p[i] = 0;
				pprch_main_key_data[i] = 0;
			};
			// зануление резерва, согласно документу -------------- конец ----------------
			//PPRCH_chten(); // чтение колюча ППРЧ
			TMI_1_AOS();
			break;
		case 7: // ключ ШПС
			for (int i = 0; i < 32; i++)
			{
				AOS_shps._words_s[i] = words.at(i).toInt();
				shps_main_key_data[i] = words.at(i).toInt();
			};

			if ((AOS_shps.osn_key_shos1 || AOS_shps.osn_key_shos2 || AOS_shps.osn_key_shos3 || AOS_shps.osn_key_shos4 ||
				AOS_shps.osn_key_shos5 || AOS_shps.osn_key_shos6 || AOS_shps.osn_key_shos7 || AOS_shps.osn_key_shos8) == 1) {shos.shps_key = 1;	}
			else { shos.shps_key = 0; };
			// зануление резерва, согласно документу -------------- начало ---------------
			AOS_shps.rez1 = 0; // 5 bit
			AOS_shps.rez2 = 0; // 5 bit
			for (int i = 19; i < 28; i++)
			{
				AOS_shps._words_s[i] = 0;
				shps_main_key_data[i] = 0;
			};
			for (int i = 30; i < 32; i++)
			{
				AOS_shps._words_s[i] = 0;
				shps_main_key_data[i] = 0;
			};
			// зануление резерва, согласно документу -------------- конец ----------------
			TMI_1_AOS();
			break;
		case 8: // номер КА
			n_ka = words.at(0).toInt();
			break;
		case 14: //чтение состояний АОС
			formirovanie_SD_kolv07(words);
			for (int i = 1; i < i; i++) { aos_state_old._words[i] = aos_state._words[i]; };
			set_new_tm(); //чтение состояний АОС
			break;
		case 15: // ТМИ - 1
			TMI_1_AOS();
			break;
		case 16:  // ТМИ - 2
			TMI_2_AOS();
			break;
		};
		update_graphics();
	}
}

void AOS_widg::set_new_tm() //чтение состояний АОС
{
	QVariantList tmp_list;
//	formirovanie_SD_kolv07();
	for (int i = 0; i < 7; i++) tmp_list.push_back(aos_state._words[i]);
	slot_thr.get_omnibus_obj()->set_new_data(MKO, adr, 14, tmp_list);
}
void AOS_widg::TMI_1_AOS()
{
	QVariantList tmp_list;

	if ((AOS_PPRCH.osn_key_pprch1 || AOS_PPRCH.osn_key_pprch2 || AOS_PPRCH.osn_key_pprch3 || AOS_PPRCH.osn_key_pprch4 ||
		AOS_PPRCH.osn_key_pprch5 || AOS_PPRCH.osn_key_pprch6 || AOS_PPRCH.osn_key_pprch7 || AOS_PPRCH.osn_key_pprch8) == 1) { shos.pprch_key = 1; }
	else { shos.pprch_key = 0; };

	if ((AOS_shps.osn_key_shos1 || AOS_shps.osn_key_shos2 || AOS_shps.osn_key_shos3 || AOS_shps.osn_key_shos4 ||
		AOS_shps.osn_key_shos5 || AOS_shps.osn_key_shos6 || AOS_shps.osn_key_shos7 || AOS_shps.osn_key_shos8) == 1) { shos.shps_key = 1; }
	else { shos.shps_key = 0; };

	for (int i = 0; i < 32; i++)
		//tmp_list.push_back(aos_state._words[i]);
		tmp_list.push_back(shos._words[i]);
	slot_thr.get_omnibus_obj()->set_new_data(MKO, adr, 15, tmp_list);
}
void AOS_widg::TMI_2_AOS()
{
	QVariantList tmp_list;
	for (int i = 0; i < 32; i++)
		// tmp_list.push_back(shos._words[i]);
	    tmp_list.push_back(gsu_kommut[i]._word);
	slot_thr.get_omnibus_obj()->set_new_data(MKO, adr, 16, tmp_list);
}

/*void AOS_widg::PPRCH_chten()
{
	QVariantList tmp_list;
	for (int i = 0; i < 32; i++)
		tmp_list.push_back(AOS_PPRCH._words_p[i]);
	// tmp_list.push_back(pprch_main_key_data[i]);
	slot_thr.get_omnibus_obj()->set_new_data(MKO, adr, 15, tmp_list);
}
	void AOS_widg::CHPS_chten()
	{
		QVariantList tmp_list; 
		for (int i = 0; i < 32; i++)
			tmp_list.push_back(AOS_shps._words_s[i]);
		// tmp_list.push_back(pprch_main_key_data[i]);
		slot_thr.get_omnibus_obj()->set_new_data(MKO, adr, 16, tmp_list);
	} // */

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