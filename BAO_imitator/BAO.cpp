#include "BAO.h"
//#include "lka05_socket_rpc.h"
#include <QMessageBox>
#include "rpc_ports.h"

BAO_widg::BAO_widg()
{
	clear_WFC_Word(word_for_WFC);
	clear_NFC_Word(word_for_NFC);
	clear_OS_Word(word_for_OS);
	clear_ThMS_Word1(word_for_ThMS1);
	clear_ThMS_Word2(word_for_ThMS2);
	clear_ThMS_Word3(word_for_ThMS3);
	clear_ThMS_Word4(word_for_ThMS4);

	//this->setFixedSize(572, 200);
	setWindowTitle("БАО");

	WC_gb = new QGroupBox("ШПК", this);
	NC_gb = new QGroupBox("УПК", this);
	OS_gb = new QGroupBox("ОС", this);
	SOTR_gb = new QGroupBox("СОТР", this);
	
	QHBoxLayout *mainBox = new QHBoxLayout(this);
	QList<QVBoxLayout *> halfSetBlock;

	// Half-set selection block

	startButton = new QPushButton("Старт", this);
	startButton->setFixedWidth(180);
	startButton->setFixedHeight(90);
	startButton->setStyleSheet("background-color: rgb(204, 204, 204);");

	for (int i = 0; i < 4; i++)
	{
		mainSetButton << new QPushButton("Основной полукомплект", this);
		reserveSetButton << new QPushButton("Резервный полукомплект", this);
		halfSetBlock << new QVBoxLayout;
	}

	for (int i = 0; i < 4; i++)
	{
		mainSetButton[i]->setFixedWidth(180);
		mainSetButton[i]->setStyleSheet("background-color: rgb(204, 204, 204);");

		reserveSetButton[i]->setFixedWidth(180);
		reserveSetButton[i]->setStyleSheet("background-color: rgb(204, 204, 204);");

		halfSetBlock[i]->setSpacing(10);
		halfSetBlock[i]->addWidget(mainSetButton[i]);
		halfSetBlock[i]->addWidget(reserveSetButton[i]);
	}

	WC_gb->setLayout(halfSetBlock[0]);
	NC_gb->setLayout(halfSetBlock[1]);
	OS_gb->setLayout(halfSetBlock[2]);
	SOTR_gb->setLayout(halfSetBlock[3]);
	
	mainBox->setContentsMargins(5, 5, 5, 5);
	mainBox->setSpacing(15);
	mainBox->addWidget(WC_gb);
	mainBox->addWidget(NC_gb);
	mainBox->addWidget(OS_gb);
	mainBox->addWidget(SOTR_gb);
	mainBox->addWidget(startButton);


	omnibus_slot_thr.set_connection_params("127.0.0.1", OMNIBUS_SLOT);
	omnibus_slot_thr.start(); // вот тут падает

	omnibus_signal_thr.set_connection_params("127.0.0.1", OMNIBUS_SIGNAL);
	omnibus_signal_thr.start(); // вот тут падает

	if (!omnibus_slot_thr.wait_connected(3) || !omnibus_signal_thr.wait_connected(3))
	{
		QMessageBox::critical(0, "Нет соединения", "Ошибка соединения с rpc_omnibus");
		this->deleteLater();
		return;
	}

	connect(omnibus_signal_thr.get_obj().get(), SIGNAL(new_message(QVariant, int, int, int, QVariantList, int)), this, SLOT(new_message(QVariant, int, int, int, QVariantList, int)), Qt::QueuedConnection);

	mku_slot_thr.set_connection_params("127.0.0.1", MKU_SLOT);
	mku_slot_thr.start(); // вот тут падает

	mku_signal_thr.set_connection_params("127.0.0.1", MKU_SIGNAL);
	mku_signal_thr.start(); // вот тут падает

	if (!mku_slot_thr.wait_connected(3) || !mku_signal_thr.wait_connected(3))
	{
		QMessageBox::critical(0, "Нет соединения", "Ошибка соединения с mku_bus");
		this->deleteLater();
		return;
	}

	connect(mku_signal_thr.get_obj().get(), SIGNAL(new_mk(int, int, int, int, double, double, int, int, int)), this, SLOT(new_mk(int, int, int, int, double, double, int, int, int)));

	power_slot_thr.set_connection_params("127.0.0.1", POWER_SLOT);
	power_slot_thr.start(); // вот тут падает

	power_signal_thr.set_connection_params("127.0.0.1", POWER_SIGNAL);
	power_signal_thr.start(); // вот тут падает

	if (!power_slot_thr.wait_connected(3) || !power_signal_thr.wait_connected(3))
	{
		QMessageBox::critical(0, "Нет соединения", "Ошибка соединения с power_bus");
		this->deleteLater();
		return;
	}

	connect(power_signal_thr.get_obj().get(), SIGNAL(u_on_k1(double)), this, SLOT(get_power(double)));

	//omnibus_slot_thr.get_omnibus_obj()->switch_ab(MKO, adr, false);
	//omnibus_slot_thr.get_omnibus_obj()->switch_ab(MKO, adr, true);
}

BAO_widg::~BAO_widg()
{

}

void BAO_widg::get_power(double volt)
{
	if (volt > 20)
		power_on = true;
	else
		power_on = false;
}

//cleaning functions for unions

void BAO_widg::clear_WFC_Word(WFC_Word &com_word)
{
	for (int i = 0; i < 7; i++)
	{
		com_word.com_word[i] = 0x0;
	}	
}

void BAO_widg::clear_NFC_Word(NFC_Word &com_word)
{
	for (int i = 0; i < 7; i++)
	{
		com_word.com_word[i] = 0x0;
	}
}

void BAO_widg::clear_OS_Word(OS_Word &com_word)
{
	for (int i = 0; i < 13; i++)
	{
		com_word.com_word[i] = 0;
	}
}

void BAO_widg::clear_ThMS_Word1(ThMS_Word1 &com_word)
{
	for (int i = 0; i < 30; i++)
	{
		com_word.com_word[i] = 0x0;
	}
}

void BAO_widg::clear_ThMS_Word2(ThMS_Word2 &com_word)
{
	for (int i = 0; i < 18; i++)
	{
		com_word.com_word[i] = 0x0;
	}
}

void BAO_widg::clear_ThMS_Word3(ThMS_Word3 &com_word)
{
	for (int i = 0; i < 31; i++)
	{
		com_word.com_word[i] = 0x0;
	}
}

void BAO_widg::clear_ThMS_Word4(ThMS_Word4 &com_word)
{
	for (int i = 0; i < 31; i++)
	{
		com_word.com_word[i] = 0x0;
	}
}

//write functions for unions

void BAO_widg::update_WFC_Word(WFC_Word com_word)
{
	tmp_list.clear();
	for (int i = 0; i < 7; i++)
	{
		tmp_list.push_back(com_word.com_word[i]);
	}
	omnibus_slot_thr.get_omnibus_obj()->set_new_data(MKO, 14, 1, tmp_list);
}

void BAO_widg::update_NFC_Word(NFC_Word com_word)
{
	tmp_list.clear();
	for (int i = 0; i < 7; i++)
	{
		tmp_list.push_back(com_word.com_word[i]);
	}
	omnibus_slot_thr.get_omnibus_obj()->set_new_data(MKO, 15, 1, tmp_list);
}

void BAO_widg::update_OS_Word(OS_Word com_word)
{
	tmp_list.clear();
	for (int i = 0; i < 13; i++)
	{
		tmp_list.push_back(com_word.com_word[i]);
	}
	omnibus_slot_thr.get_omnibus_obj()->set_new_data(MKO, 16, 1, tmp_list);
}

void BAO_widg::update_ThMS_Word1(ThMS_Word1 com_word)
{
	tmp_list.clear();
	for (int i = 0; i < 30; i++)
	{
		tmp_list.push_back(com_word.com_word[i]);
	}
	omnibus_slot_thr.get_omnibus_obj()->set_new_data(MKO, 17, 1, tmp_list);
}

void BAO_widg::update_ThMS_Word2(ThMS_Word2 com_word)
{
	tmp_list.clear();
	for (int i = 0; i < 30; i++)
	{
		tmp_list.push_back(com_word.com_word[i]);
	}
	omnibus_slot_thr.get_omnibus_obj()->set_new_data(MKO, 17, 10, tmp_list);
}

void BAO_widg::update_ThMS_Word3(ThMS_Word3 com_word)
{
	tmp_list.clear();
	for (int i = 0; i < 31; i++)
	{
		tmp_list.push_back(com_word.com_word[i]);
	}
	omnibus_slot_thr.get_omnibus_obj()->set_new_data(MKO, 17, 18, tmp_list);
}

void BAO_widg::update_ThMS_Word4(ThMS_Word4 com_word)
{
	tmp_list.clear();
	for (int i = 0; i < 31; i++)
	{
		tmp_list.push_back(com_word.com_word[i]);
	}
	omnibus_slot_thr.get_omnibus_obj()->set_new_data(MKO, 17, 19, tmp_list);
}

void BAO_widg::answer_adrs_14(MKOCommandWord cwd, QVariantList words)
{
	
	if (words.at(0) == 0x1)		//Включение ШПК
	{
		word_for_WFC.s_on = 1;
		update_WFC_Word(word_for_WFC);
	}

	if (words.at(0) == 0x2)		//Выключение ШПК
	{
		word_for_WFC.s_on = 0;
		update_WFC_Word(word_for_WFC);
	}

	if (words.at(0) == 0x10)		//Подключение основного устройства функционирования ШПК
	{
		word_for_WFC.s_main_uf = 1;
		word_for_WFC.s_res_uf = 0;
		update_WFC_Word(word_for_WFC);
	}

	if (words.at(0) == 0x20)		//Подключение резервного устройства функционирования ШПК
	{
		word_for_WFC.s_main_uf = 0;
		word_for_WFC.s_res_uf = 1;
		update_WFC_Word(word_for_WFC);
	}

	if (words.at(0) == 0x4)		//Подключение основного БРВ ШПК
	{
		word_for_WFC.s_main_brv = 1;
		word_for_WFC.s_res_brv = 0;
		update_WFC_Word(word_for_WFC);
	}

	if (words.at(0) == 0x8)		//Подключение резервного БРВ ШПК
	{
		word_for_WFC.s_main_brv = 0;
		word_for_WFC.s_res_brv = 1;
		update_WFC_Word(word_for_WFC);
	}

	if (words.at(0) == 0x100)		//Включение основного термостабилизатора УФ ШПК
	{
		word_for_WFC.s_main_term_on = 1;
		update_WFC_Word(word_for_WFC);
	}

	if (words.at(0) == 0x200)		//Включение резервного термостабилизатора УФ ШПК
	{
		word_for_WFC.s_res_term_on = 1;
		update_WFC_Word(word_for_WFC);
	}

	if (words.at(0) == 0x40)		//Включение режима тренировки трубки ШПК
	{
		word_for_WFC.s_tt = 1;
		update_WFC_Word(word_for_WFC);
	}
	
	if (words.at(0) == 0x80)		//Выключение режима тренировки трубки ШПК
	{
		word_for_WFC.s_tt = 0;
		update_WFC_Word(word_for_WFC);
	}

	if (words.at(0) == 0x4000)		//Включение режима настройки (+20) ШПК
	{
		word_for_WFC.s_setup_mode_on = 1;
		update_WFC_Word(word_for_WFC);
	}

}

void BAO_widg::answer_adrs_15(MKOCommandWord cwd, QVariantList words)
{
	if (words.at(0) == 0x1)		//Включение УПК
	{
		word_for_NFC.u_on = 1;
		update_NFC_Word(word_for_NFC);
	}

	if (words.at(0) == 0x2)		//Выключение УПК
	{
		word_for_NFC.u_on = 0;
		update_NFC_Word(word_for_NFC);
	}

	if (words.at(0) == 0x10)		//Подключение основного устройства функционирования УПК
	{
		word_for_NFC.u_main_uf = 1;
		word_for_NFC.u_res_uf = 0;
		update_NFC_Word(word_for_NFC);
	}

	if (words.at(0) == 0x20)		//Подключение резервного устройства функционирования УПК
	{
		word_for_NFC.u_main_uf = 0;
		word_for_NFC.u_res_uf = 1;
		update_NFC_Word(word_for_NFC);
	}

	if (words.at(0) == 0x4)		//Подключение основного БРВ УПК
	{
		word_for_NFC.u_main_brv = 1;
		word_for_NFC.u_res_brv = 0;
		update_NFC_Word(word_for_NFC);
	}

	if (words.at(0) == 0x8)		//Подключение резервного БРВ УПК
	{
		word_for_NFC.u_main_brv = 0;
		word_for_NFC.u_res_brv = 1;
		update_NFC_Word(word_for_NFC);
	}

	if (words.at(0) == 0x100)		//Включение основного термостабилизатора УФ УПК
	{
		word_for_NFC.u_main_term_on = 1;
		word_for_NFC.u_res_term_on = 0;
		update_NFC_Word(word_for_NFC);
	}

	if (words.at(0) == 0x200)		//Включение резервного термостабилизатора УФ УПК
	{
		word_for_NFC.u_main_term_on = 010;
		word_for_NFC.u_res_term_on = 1;
		update_NFC_Word(word_for_NFC);
	}

	if (words.at(0) == 0x40)		//Включение режима тренировки трубки УПК
	{
		word_for_NFC.u_tt = 1;
		update_NFC_Word(word_for_NFC);
	}

	if (words.at(0) == 0x80)		//Выключение режима тренировки трубки УПК
	{
		word_for_NFC.u_tt = 0;
		update_NFC_Word(word_for_NFC);
	}

	if (words.at(0) == 0x4000)		//Включение режима настройки (+20) УПК
	{
		word_for_NFC.u_setup_mode_on = 1;
		update_NFC_Word(word_for_NFC);
	}
}

void BAO_widg::answer_adrs_16(MKOCommandWord cwd, QVariantList words)
{
	if (words.at(0) == 0x310)		//Шаговое перемещение механизма фокусировки плюсового направления ШПК
	{
		word_for_OS.s_step_mov = 1;
		update_OS_Word(word_for_OS);
	}

	if (words.at(0) == 0x311)		//Шаговое перемещение механизма фокусировки минусового направления ШПК
	{
		word_for_OS.s_step_mov = 1;
		update_OS_Word(word_for_OS);
	}
}

void BAO_widg::new_message(QVariant dt, int mko, int line, int cwd, QVariantList words, int os)
{
	if (!power_on)
		return;

	if (os == -1)
		return;


	MKOCommandWord parsed_command_word(cwd);


	if ((parsed_command_word.address == 14) && (parsed_command_word.subaddress == 2))
		answer_adrs_14(parsed_command_word, words);
	
	if ((parsed_command_word.address == 15) && (parsed_command_word.subaddress == 2))
		answer_adrs_15(parsed_command_word, words);
}

void BAO_widg::new_mk(int mshm, int pshm, int length_m, int length_p, double u_m, double u_p, int dt, int line_m, int line_p)
{
	if (!power_on)
		return;

	if (mshm == 4 && pshm == 3 && (WFC_on || NFC_on || OS_on || ThMS_on)) {
		start_on = true;
		startButton->setStyleSheet("background-color: rgb(142, 198, 156);");
		omnibus_slot_thr.get_omnibus_obj()->switch_ab(MKO, 14, true);
		omnibus_slot_thr.get_omnibus_obj()->switch_ab(MKO, 15, true);
		omnibus_slot_thr.get_omnibus_obj()->switch_ab(MKO, 16, true);
		omnibus_slot_thr.get_omnibus_obj()->switch_ab(MKO, 17, true);
		if (WFC_on)	update_WFC_Word(word_for_WFC);
		if (NFC_on)	update_NFC_Word(word_for_NFC);
		if (OS_on) update_OS_Word(word_for_OS);
		if (ThMS_on) update_ThMS_Word1(word_for_ThMS1);

	}

	//if (!start_on)
	//	return;

	if (mshm == 0 && pshm == 2) {
		//slot_thr.get_omnibus_obj()->switch_ab(1, 9, true);
		WFC_on = true;
		mainSetButton[0]->setStyleSheet("background-color: rgb(142, 198, 156);");
		reserveSetButton[0]->setStyleSheet("background-color: rgb(204, 204, 204);");
		//clear_WFC_Word(word_for_WFC);
		word_for_WFC.s_res_blk_on = 0;
		word_for_WFC.s_main_blk_on = 1;
		if (start_on) update_WFC_Word(word_for_WFC);
		
		//Включен основной БЛК ШПК
	}

	if (mshm == 1 && pshm == 2) {
		WFC_on = true;
		//slot_thr.get_omnibus_obj()->switch_ab(1, 9, true);
		mainSetButton[0]->setStyleSheet("background-color: rgb(204, 204, 204);");
		reserveSetButton[0]->setStyleSheet("background-color: rgb(142, 198, 156);");
		//clear_WFC_Word(word_for_WFC);
		word_for_WFC.s_res_blk_on = 1;
		word_for_WFC.s_main_blk_on = 0;
		if (start_on) update_WFC_Word(word_for_WFC);

		//Включен резервный БЛК ШПК
	}

	if (mshm == 2 && pshm == 2) {
		NFC_on = true;
		//slot_thr.get_omnibus_obj()->switch_ab(1, 9, true);
		mainSetButton[1]->setStyleSheet("background-color: rgb(142, 198, 156);");
		reserveSetButton[1]->setStyleSheet("background-color: rgb(204, 204, 204);");
		//clear_NFC_Word(word_for_NFC);
		word_for_NFC.u_res_blk_on = 0;
		word_for_NFC.u_main_blk_on = 1;
		if (start_on) update_NFC_Word(word_for_NFC);

		//Включен основной БЛК УПК
	}

	if (mshm == 3 && pshm == 2) {
		NFC_on = true;
		//slot_thr.get_omnibus_obj()->switch_ab(1, 9, true);
		mainSetButton[1]->setStyleSheet("background-color: rgb(204, 204, 204);");
		reserveSetButton[1]->setStyleSheet("background-color: rgb(142, 198, 156);");
		//clear_NFC_Word(word_for_NFC);
		word_for_NFC.u_res_blk_on = 1;
		word_for_NFC.u_main_blk_on = 0;
		if (start_on) update_NFC_Word(word_for_NFC);

		//Включен резервный БЛК УПК
	}

	if (mshm == 4 && pshm == 2) {
		OS_on = true;
		//slot_thr.get_omnibus_obj()->switch_ab(1, 9, true);
		mainSetButton[2]->setStyleSheet("background-color: rgb(142, 198, 156);");
		reserveSetButton[2]->setStyleSheet("background-color: rgb(204, 204, 204);");
		//clear_OS_Word(word_for_OS);
		word_for_OS.os_res_blk_on = 0;
		word_for_OS.os_main_blk_on = 1;
		if (start_on) update_OS_Word(word_for_OS);

		//Включен основной БЛК ОС
	}

	if (mshm == 5 && pshm == 2) {
		OS_on = true;
		//slot_thr.get_omnibus_obj()->switch_ab(1, 9, true);
		mainSetButton[2]->setStyleSheet("background-color: rgb(204, 204, 204);");
		reserveSetButton[2]->setStyleSheet("background-color: rgb(142, 198, 156);");
		//clear_OS_Word(word_for_OS);
		word_for_OS.os_res_blk_on = 1;
		word_for_OS.os_main_blk_on = 0;
		if (start_on) update_OS_Word(word_for_OS);

		//Включен резервный БЛК ОС
	}

	if (mshm == 6 && pshm == 2) {
		ThMS_on = true;
		//slot_thr.get_omnibus_obj()->switch_ab(1, 9, true);
		mainSetButton[3]->setStyleSheet("background-color: rgb(142, 198, 156);");
		reserveSetButton[3]->setStyleSheet("background-color: rgb(204, 204, 204);");
		//clear_ThMS_Word1(word_for_ThMS1);
		word_for_ThMS1.r_blk_sotr_on = 0;
		word_for_ThMS1.m_blk_sotr_on = 1;
		if (start_on) update_ThMS_Word1(word_for_ThMS1);

		//Включен основной БЛК СОТР
	}

	if (mshm == 7 && pshm == 2) {
		ThMS_on = true;
		//slot_thr.get_omnibus_obj()->switch_ab(1, 9, true);
		mainSetButton[3]->setStyleSheet("background-color: rgb(204, 204, 204);");
		reserveSetButton[3]->setStyleSheet("background-color: rgb(142, 198, 156);");
		//clear_ThMS_Word1(word_for_ThMS1);
		word_for_ThMS1.r_blk_sotr_on = 1;
		word_for_ThMS1.m_blk_sotr_on = 0;
		if (start_on) update_ThMS_Word1(word_for_ThMS1);

		//Включен резервный БЛК СОТР
	}

	if (mshm == 0 && pshm == 3) {
		WFC_on = false;
		//slot_thr.get_omnibus_obj()->switch_ab(1, 9, true);
		mainSetButton[0]->setStyleSheet("background-color: rgb(204, 204, 204);");
		reserveSetButton[0]->setStyleSheet("background-color: rgb(204, 204, 204);");

		//word_for_cbk.half_set_engage = 0;
	}

	if (mshm == 1 && pshm == 3) {
		NFC_on = false;
		//slot_thr.get_omnibus_obj()->switch_ab(1, 9, true);
		mainSetButton[0]->setStyleSheet("background-color: rgb(204, 204, 204);");
		reserveSetButton[0]->setStyleSheet("background-color: rgb(204, 204, 204);");

		//word_for_cbk.half_set_engage = 0;
	}

	if (mshm == 2 && pshm == 3) {
		OS_on = false;
		//slot_thr.get_omnibus_obj()->switch_ab(1, 9, true);
		mainSetButton[0]->setStyleSheet("background-color: rgb(204, 204, 204);");
		reserveSetButton[0]->setStyleSheet("background-color: rgb(204, 204, 204);");

		//word_for_cbk.half_set_engage = 0;
	}

	if (mshm == 3 && pshm == 3) {
		ThMS_on = false;
		//slot_thr.get_omnibus_obj()->switch_ab(1, 9, true);
		mainSetButton[0]->setStyleSheet("background-color: rgb(204, 204, 204);");
		reserveSetButton[0]->setStyleSheet("background-color: rgb(204, 204, 204);");

		//word_for_cbk.half_set_engage = 0;
	}
}
