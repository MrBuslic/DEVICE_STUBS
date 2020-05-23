#include "BAOV.h"

#include "rpc_ports.h"

#include <QMessageBox>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "brgi.h"
#include "drs.h"
#include "dri.h"
#include "dni.h"
#include "sensor.h"
#include "util_functions.h"
#include "functional_control_mapping_item.h"

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

union TimeFirstWord {
	quint16 com_word;
	struct {
		quint16 hours : 1,
			day : 5,
			month : 4,
			year : 6;
	};
};

union TimeSecondWord {
	quint16 com_word;
	struct {
		quint16 seconds : 6,
			minutes : 6,
			hours : 4;
	};
};

BAOV_widg::BAOV_widg(QWidget *parent) : LKA_sett(QCoreApplication::applicationDirPath() + "/bech.ini", QSettings::IniFormat)
{
	widg = new QWidget(this);
	setWindowTitle("БАО-В");
	widg->setMaximumHeight(600);
	widg->setMaximumWidth(400);

	ready_og = false;
	flag_on = false;
	tm_towarm = standart_tm;
	cooling_cof = 4;

	for (int i = 0; i < 4; i++)
	{
		QString numb = QString::number(i + 1);
		rc_pbut << new QPushButton(numb, this);
		rc_pbut[i]->setStyleSheet("background-color: rgb(204, 204, 204);");
		if (i < 2)
		{
			QString numb = QString::number(i + 1);
			uco_pbut << new QPushButton(numb, this);
			uco_pbut[i]->setStyleSheet("background-color: rgb(204, 204, 204);");
		}
	}
	for (int i = 0; i < 8; i++) {
		int firstBtnInRowIdx = i * 4;
		active_sensors_info_pbut << new QPushButton("Имя датчика", this);
		active_sensors_info_pbut << new QPushButton("Исправность", this);
		active_sensors_info_pbut << new QPushButton("Активность", this);
		active_sensors_info_pbut << new QPushButton("Порог обнаружения", this);
		active_sensors_info_pbut[firstBtnInRowIdx]->setStyleSheet("background-color: rgb(135, 135, 135);");
		active_sensors_info_pbut[firstBtnInRowIdx + 1]->setStyleSheet("background-color: rgb(204, 204, 204);");
		active_sensors_info_pbut[firstBtnInRowIdx + 2]->setStyleSheet("background-color: rgb(204, 204, 204);");
		active_sensors_info_pbut[firstBtnInRowIdx + 3]->setStyleSheet("background-color: rgb(204, 204, 204);");
		active_sensors_info_pbut[firstBtnInRowIdx]->setMaximumWidth(80);
		active_sensors_info_pbut[firstBtnInRowIdx + 1]->setMaximumWidth(80);
		active_sensors_info_pbut[firstBtnInRowIdx + 2]->setMaximumWidth(80);
		active_sensors_info_pbut[firstBtnInRowIdx + 3]->setMaximumWidth(80);
	}
	sis_btn = new QPushButton("Отослать СИС и СИС НИИ", this);
	sis_btn->setStyleSheet("background-color: rgb(204, 204, 204);");
	connect(sis_btn, SIGNAL(clicked()), this, SLOT(generateSIS()));

	rc_gb = new QGroupBox("РЦ", this);
	uco_gb = new QGroupBox("УЦО", this);
	active_sensors_info_gb = new QGroupBox("Конфигурация рабочих датчиков", this);
	sis_btn_gb = new QGroupBox("", this);

	All_vblay = new QVBoxLayout(this);
	rc_hlay = new QHBoxLayout(rc_gb);
	uco_hlay = new QHBoxLayout(uco_gb);
	active_sensors_info_grid = new QGridLayout(active_sensors_info_gb);
	sis_btn_hlay = new QHBoxLayout(sis_btn_gb);

	for (int i = 0; i < 4; i++)
	{
		rc_hlay->addWidget(rc_pbut[i]);
		if (i < 2)
		{
			uco_hlay->addWidget(uco_pbut[i]);
		}
	}
	for (int i = 0; i < 32; i++) {
		
			active_sensors_info_grid->addWidget(active_sensors_info_pbut[i], i / 4 + 1, i % 4 + 1);
		
	}
	sis_btn_hlay->addWidget(sis_btn);

	rc_gb->setAlignment(Qt::AlignHCenter);
	uco_gb->setAlignment(Qt::AlignHCenter);
	active_sensors_info_gb->setAlignment(Qt::AlignCenter);
	sis_btn_gb->setAlignment(Qt::AlignCenter);

	All_vblay->addWidget(rc_gb);
	All_vblay->addWidget(uco_gb);
	All_vblay->addWidget(active_sensors_info_gb);
	All_vblay->addWidget(sis_btn_gb);

	AbOn_tmr = new QTimer(this);
	AbOn_tmr->setSingleShot(true);
	connect(AbOn_tmr, &QTimer::timeout, this, &BAOV_widg::omni_connect);

	Power_tmr = new QTimer(this);
	Power_tmr->setSingleShot(true);
	connect(Power_tmr, &QTimer::timeout, this, &BAOV_widg::set_change_power);

	slot_thr.set_connection_params("127.0.0.1", OMNIBUS_SLOT);
	slot_thr.start(); // вот тут падает

	signal_thr.set_connection_params("127.0.0.1", OMNIBUS_SIGNAL);
	signal_thr.start(); // вот тут падает

	if (!slot_thr.wait_connected(3) || !signal_thr.wait_connected(3))
	{
		QMessageBox::critical(0, "Нет соединения", "Ошибка соединения с rpc_omnibus");
		this->deleteLater();
		return;
	}

	mku_slot_thr.set_connection_params("127.0.0.1", MKU_SLOT);
	mku_slot_thr.start(); // вот тут падает

	mku_signal_thr.set_connection_params("127.0.0.1", MKU_SIGNAL);
	mku_signal_thr.start(); // вот тут падает

	if (!mku_slot_thr.wait_connected(3) || !mku_signal_thr.wait_connected(3))
	{
		QMessageBox::critical(0, "Нет соединения", "Ошибка соединения с lka05");
		this->deleteLater();
		return;
	}

	interrupt_slot_thr.set_connection_params("127.0.0.1", INTERRUPTS_SLOT);
	interrupt_slot_thr.start(); // вот тут падает

	interrupt_signal_thr.set_connection_params("127.0.0.1", INTERRUPTS_SIGNAL);
	interrupt_signal_thr.start(); // вот тут падает

	if (!interrupt_slot_thr.wait_connected(3) || !interrupt_signal_thr.wait_connected(3))
	{
		QMessageBox::critical(0, "Нет соединения", "Ошибка соединения с interrupt_bus в Foi");
		this->deleteLater();
		return;
	}

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

	connect(signal_thr.get_obj().get(), SIGNAL(new_message(QVariant, int, int, int, QVariantList, int)), this, SLOT(new_message(QVariant, int, int, int, QVariantList, int)));

	slot_thr.get_omnibus_obj()->switch_ab(MKO, adr, false);

	connect(mku_signal_thr.get_obj().get(), SIGNAL(new_mk(int, int, int, int, double, double, int, int, int)), this, SLOT(new_mk(int, int, int, int, double, double, int, int, int)));
	connect(power_signal_thr.get_obj().get(), SIGNAL(u_on_k1(double)), this, SLOT(get_power(double)));

	log_filename = QString("d:/logs/%1_%2.log").arg(QCoreApplication::applicationName()).arg(QDateTime::currentDateTime().toString("yyyy.MM.dd_hh.mm.ss"));
	QDir dir("d:/logs");
	if (!dir.exists())
		QDir().mkdir("d:/logs");
	connect(&log_timer, &QTimer::timeout, this, &BAOV_widg::log_timer_ontimer);
	log_timer.start(200);


	edit = new QTextEdit(this);
	_scroll_bar = edit->verticalScrollBar();
	_doc = new QTextDocument();
	_cursor = new QTextCursor(_doc);
	edit->setDocument(_doc);
	edit->setReadOnly(true);
	_doc->setMaximumBlockCount(1000);
	setMinimumSize(360, 300);
	auto_scroll_box = new QCheckBox(this);
	auto_scroll_box->setText("Автопрокрутка");
	auto_scroll_box->setChecked(true);
	connect(auto_scroll_box, &QCheckBox::stateChanged, this, &BAOV_widg::auto_scroll_clicked);
	All_vblay->addWidget(edit);
	All_vblay->addWidget(auto_scroll_box);

	QSettings settings(QApplication::applicationDirPath() + "/positions.ini", QSettings::IniFormat);
	restoreGeometry(settings.value("bech_geometry").toByteArray());

	update_graphics();
}

void BAOV_widg::BAOV_interrupt_setup()
{
	n = adr;
	chan = 3;
	u = 5;
	t = 4.5;
	BAOV_interrupt_run();
}

void BAOV_widg::BAOV_interrupt_run()
{
	interrupt_slot_thr.get_interrupt_bus_obj()->make_interrupt(n, chan, u, t);
}

void BAOV_widg::get_power(double _volt)
{
	volt = _volt;
	if (volt >= 20.0)
		imit_on();
	else
		if (volt < 1)
			imit_off();
}

void BAOV_widg::set_power_back()
{
	double curr;
	if (power_vt >= 1)
	{
		curr = (double)power_vt / volt;
	}
	else
		curr = 0.0;
	power_slot_thr.get_power_bus_obj()->set_i(bus, name, curr);
}

void BAOV_widg::imit_off()
{
	if (!flag_on)
		return;
	flag_on = false;
	msg_to_log("Питание отключено");

	firstOKWord.bao_v_ok = 0;
	firstOKWord.uco_1_ok = 0;
	firstOKWord.uco_2_ok = 0;
	firstOKWord.temperature_mn_uco_ok = 0;
	firstOKWord.temperature_pl_uco_ok = 0;
	sendOperationalControl();
	curr_RC = RC_OFF;
	curr_UCO = UCO_OFF;

	slot_thr.get_omnibus_obj()->switch_ab(MKO, adr, false);
	update_graphics();
	set_power_back();
	//inter_tmr->stop();
}

void BAOV_widg::imit_on()
{
	if (flag_on)
		return;
	flag_on = true;
	msg_to_log("Питание включено");

	responseWord.subscriber_busy = 1;
	// непонятно что за 4 аргумент
	slot_thr.get_omnibus_obj()->switch_ab_os(MKO, adr, responseWord.com_word, -1);

	if (addressToSensorMap.begin() == addressToSensorMap.end()) {
		parseInitSensorsData();
		parseMappingForFunctionalControl();
	}
	
	turnOnSensors();
	sensors_enabled = true;

	responseWord.subscriber_busy = 0;
	responseWord.subscriber_error = 0;
	// непонятно что за 4 аргумент
	slot_thr.get_omnibus_obj()->switch_ab_os(MKO, adr, responseWord.com_word, -1);

	firstOKWord.time_request = 1;
	firstOKWord.bao_v_ok = 1;
	// видимо тут надо послать реквест в цбк на получение времени
	sendOperationalControl();


	AbOn_tmr->start(5000);
	change_power(true);
	update_graphics();
	//inter_tmr->start(1000);
}

void BAOV_widg::set_change_power()
{
	change_power(false);
}



void BAOV_widg::change_power(bool switch_og)
{
	if (switch_og)//генератор переключился?
	{
		power_vt = 60;
		Power_tmr->start(tm_towarm);
		set_power_back();
	}
	else
	{
		switch (power_vt)
		{
		case 0:
			power_vt = 60;
			Power_tmr->start(tm_towarm);
			set_power_back();
			break;
		case 60:
			power_vt = 35;
			set_power_back();
			break;
		case 35:
			power_vt = 60;
			Power_tmr->start(tm_towarm);
			set_power_back();
			break;
		}
	}
	switch_og = false;
}

void BAOV_widg::new_mk(int mshm, int pshm, int length_m, int length_p, double u_m, double u_p, int dt, int line_m, int line_p)
{
	if (volt != 0)
	{
		int uu = 0;
		//	QString _msg = QString("%1 принял МК МШ%2 ПШ%3").arg(QTime::currentTime().toString("hh:mm:ss.zzz")).arg(mshm).arg(pshm);
		//	msg_to_log(_msg);
		int tmp_mshm = mshm;
		int tmp_pshm = pshm;

		if (curr_RC == RC_OFF && tmp_mshm != RC_OFF && mshm >= 0 && mshm < 4 && pshm == 4) { // 04, 14,24,34 - 1 цифра mshm, 2 - pshm
			curr_RC = RC(mshm);
			firstOKWord.bao_v_ok = 0;

			if (curr_RC == RC_1 || curr_RC == RC_3) {
				curr_UCO = UCO_1;
				firstOKWord.uco_1_ok = 1;
			}
			else {
				curr_UCO = UCO_2;
				firstOKWord.uco_2_ok = 1;
			}

			firstOKWord.temperature_mn_uco_ok = 1;
			firstOKWord.temperature_pl_uco_ok = 1;

			sendOperationalControl();

			imit_on();
		}
		else if (curr_RC != RC_OFF && tmp_mshm == RC_OFF) {
			imit_off();
		}
		else {
			// error handling
		}

		LKA_sett.sync();
	}
	update_graphics();
}


void BAOV_widg::msg_to_log(const QString& _msg)
{
	{
		QMutexLocker lock(&log_mutex);
		log_buffer << _msg;
	}
	_cursor->insertText(_msg + "\n");
	if (auto_scroll)
		_scroll_bar->setValue(_scroll_bar->maximum());
}

void BAOV_widg::auto_scroll_clicked(int _state)
{
	auto_scroll = (_state != 0);
}

void BAOV_widg::log_timer_ontimer()
{
	QStringList tmp_buffer;
	{
		QMutexLocker lock(&log_mutex);
		tmp_buffer = log_buffer;
		log_buffer.clear();
	}
	if (tmp_buffer.isEmpty())
		return;
	QFile log_file(log_filename);
	QTextStream log_stream(&log_file);
	log_file.open(QIODevice::Append);
	for (QStringList::iterator itr = tmp_buffer.begin(); itr != tmp_buffer.end(); itr++)
		log_stream << *itr << "\n";
	log_file.close();
}

void BAOV_widg::new_message(QVariant dt, int mko, int line, int cwd, QVariantList words, int os)
{

	MKOWord tmp_cwd;
	tmp_cwd.com_word = cwd;
	if (os == -1)
		return;
	if (!flag_on)
		return;
	// выдача времени
	if (mko == MKO && tmp_cwd.adr == adr && tmp_cwd.subadr == 3) {

		firstOKWord.incorrect_command_control_code = 0;
		setKU_CBK_OK();

		curr_first_time_word = words[0].toInt();
		curr_second_time_word = words[1].toInt();
		firstOKWord.time_request = 0;
		firstOKWord.sis_fk_ready = 1;
		setKU_MKP_OK();
		sendOperationalControl();
		printTime();
	}
	else if ((mko == MKO) && (tmp_cwd.adr == adr))
	{
		//	QString _msg = QString("%1 принял сигнал на подадресе %2 c КС %3").arg(QTime::currentTime().toString("hh:mm:ss.zzz")).arg(tmp_cwd.subadr).arg(tmp_cwd.com_word);
		//	msg_to_log(_msg);

		int tmp_word = -1;
		if (words.size() > 0)
			tmp_word = words[0].toInt();

		// включение/выключение оптических датчиков
		if (tmp_cwd.subadr == 2 && tmp_word == 0x1000) {
			firstOKWord.incorrect_command_control_code = 0;
			setKU_CBK_OK();
			turnOnSensors();
			sensors_enabled = true;
			firstOKWord.start_bo_ok = 1;
			setKU_MKP_OK();
			sendOperationalControl();
		}
		else if (tmp_cwd.subadr == 2 && tmp_word == 0x1100) {
			firstOKWord.incorrect_command_control_code = 0;
			// тут не нужно ждать второй секундной метки чтобы начать выключение
			setKU_CBK_OK();
			turnOffSensors(true);
			sensors_enabled = false;
			firstOKWord.start_bo_ok = 0;
			setKU_MKP_OK();
			sendOperationalControl();
		}

		// ФК
		else if (tmp_cwd.subadr == 2 && tmp_word == 0x1300) {
			
			firstOKWord.incorrect_command_control_code = 0;
			setKU_CBK_OK();
			setKU_MKP_OK();

			responseWord.subscriber_busy = 1;
			slot_thr.get_omnibus_obj()->switch_ab_os(MKO, adr, responseWord.com_word, -1);

			doFunctionalControl();

			responseWord.subscriber_busy = 0;
			slot_thr.get_omnibus_obj()->switch_ab_os(MKO, adr, responseWord.com_word, -1);

		}
		// КФГ-10
		else if (tmp_cwd.subadr == 2 && tmp_word == 0x1700) {
			responseWord.subscriber_busy = 1;
			slot_thr.get_omnibus_obj()->switch_ab_os(MKO, adr, responseWord.com_word, -1);
			setActiveSensors(words);
			responseWord.subscriber_busy = 0;
			slot_thr.get_omnibus_obj()->switch_ab_os(MKO, adr, responseWord.com_word, -1);

		}
		// ДПОР-10
		else if (tmp_cwd.subadr == 2 && tmp_word == 0x1600) {
			setDetectionThreshold(words);
		}
		// РП-10
		else if (tmp_cwd.subadr == 2 && tmp_word == 0x1400) {
			std::map<int, sensor>::iterator it = addressToEnabledSensorMap.begin();
			while (it != addressToEnabledSensorMap.end()) {
				it->second.detection_threshold = DEFAULT_DETECTION_THRESHOLD;
				it++;
			}
			std::map<int, sensor>::iterator it2 = addressToSensorMap.begin();
			while (it2 != addressToSensorMap.end()) {
				it2->second.detection_threshold = DEFAULT_DETECTION_THRESHOLD;
				it2++;
			}
			msg_to_log("Рабочие пороги обнаружения установлены.");
		}
		// БУ-10
		else if (tmp_cwd.subadr == 2 && tmp_word == 0x1500) {
			responseWord.subscriber_busy = 1;
			slot_thr.get_omnibus_obj()->switch_ab_os(MKO, adr, responseWord.com_word, -1);
			if (sensors_enabled) {
				sensors_enabled = false;
				turnOffSensors(true);
			}
			int rc_before_imit_off = curr_RC;
			imit_off();
			switch (rc_before_imit_off) {
			case RC_1:
				curr_RC = RC_2;
				curr_UCO = UCO_2;
				break;
			case RC_2:
				curr_RC = RC_3;
				curr_UCO = UCO_1;
				break;
			case RC_3:
				curr_RC = RC_4;
				curr_UCO = UCO_2;
				break;
			case RC_4:
				curr_RC = RC_1;
				curr_UCO = UCO_1;
				break;
			}
			sendOperationalControl();
			imit_on();
			responseWord.subscriber_busy = 0;
			slot_thr.get_omnibus_obj()->switch_ab_os(MKO, adr, responseWord.com_word, -1);
		}
		else if (tmp_cwd.subadr == 2 && tmp_word == 0x1A) {
			generateSIS(true);
		}
		else {
			firstOKWord.incorrect_command_control_code = 1;
			sendOperationalControl();
		}

	}
	update_graphics();
}

void BAOV_widg::setActiveSensors(QVariantList words) {
	bool isSensorsActive = addressToEnabledSensorMap.begin()->second.active;
	turnOffSensors(false);
	addressToEnabledSensorMap.clear();
	for (int currBit = 0; currBit < 16; currBit++) {
		int mask = 1 << currBit;
		int masked_n = words[1].toInt() & mask;
		int bitVal = masked_n >> currBit;
		switch (currBit) {
		case 0:
			if (bitVal == 1) {
				addSensorToActive(0x1, isSensorsActive);
			}
			break;
		case 1:
			if (bitVal == 1) {
				addSensorToActive(0x2, isSensorsActive);
			}
			break;
		case 2:
			if (bitVal == 1) {
				addSensorToActive(0x3, isSensorsActive);
			}
			break;
		case 3:
			if (bitVal == 1) {
				addSensorToActive(0x4, isSensorsActive);
			}
			break;
		case 4:
			if (bitVal == 1)
			{
				addSensorToActive(0x5, isSensorsActive);
			}
				break;
		case 5:
			if (bitVal == 1) {
				addSensorToActive(0x6, isSensorsActive);
			}
				break;
		case 6:
			if (bitVal == 1) {
				addSensorToActive(0x7, isSensorsActive);
			}
				break;
		case 7:
			if (bitVal == 1) {
				addSensorToActive(0x8, isSensorsActive);
			}
				break;
		case 8:
			if (bitVal == 1) {
				addSensorToActive(0x11, isSensorsActive);
			}
				break;
		case 9:
			if (bitVal == 1) {
				addSensorToActive(0x12, isSensorsActive);
			}
				break;
		case 10:
			if (bitVal == 1) {
				addSensorToActive(0x13, isSensorsActive);
			}
				break;
		case 11:
			break;
		case 12:
			if (bitVal == 1) {
				addSensorToActive(0x15, isSensorsActive);
			}
				break;
		case 13:
			if (bitVal == 1) {
				addSensorToActive(0x16, isSensorsActive);
			}
				break;
		case 14:
			if (bitVal == 1) {
				addSensorToActive(0x17, isSensorsActive);
			}
				break;
		case 15:
			break;
		}
	}
	update_graphics();
}

void BAOV_widg::addSensorToActive(int addr, bool isActive) {
	QString msg = "Датчик ";
	msg
		.append(QString::fromStdString(addressToSensorMap.find(addr)->second.name))
		.append(" добавлен в рабочую конфигурацию");

	addressToEnabledSensorMap.insert(std::pair<int, sensor>(addr, addressToSensorMap.find(addr)->second));
	if (isActive) {
		addressToEnabledSensorMap.find(addr)->second.active = 1;
		msg.append(" и включен");
	}
	
	msg_to_log(msg);
	updateSensorStatusInOK(addressToEnabledSensorMap.find(addr)->second, isActive);
	update_graphics();
}

void BAOV_widg::setDetectionThreshold(QVariantList words) {
	if (words[2] == 0x0008) {
		std::map<int, sensor>::iterator it = addressToSensorMap.find(words[1].toInt());
		if (it != addressToSensorMap.end()) {
			it->second.detection_threshold = words[5].toInt();
			QString msg = "Для датчика ";
			msg
				.append(QString::fromStdString(it->second.name))
				.append(" задан специальный порог обнаружения ")
				.append(QString::number(words[5].toInt()));
			msg_to_log(msg);
		}

		std::map<int, sensor>::iterator it1 = addressToEnabledSensorMap.find(words[1].toInt());
		if (it1 != addressToEnabledSensorMap.end())
			it1->second.detection_threshold = words[5].toInt();

	}
	else if (words[2] == 0x0440) {
		firstOKWord.test_mode_ok = 0;
		sendOperationalControl();
	}
	else if (words[2] == 0x044F) {
		firstOKWord.test_mode_ok = 1;
		sendOperationalControl();
	}
	update_graphics();
}

void BAOV_widg::setKU_CBK_OK() {
	firstOKWord.kv_ku_cbk_ok = 1;
	sendOperationalControl();
	// тут нужно снимать флаг по второй секундной метке
	firstOKWord.kv_ku_cbk_ok = 0;
	sendOperationalControl();
}

void BAOV_widg::setKU_MKP_OK() {
	firstOKWord.kv_ku_mkp_ok = 1;
	sendOperationalControl();
	// тут нужно снимать флаг по второй секундной метке
	firstOKWord.kv_ku_mkp_ok = 0;
	sendOperationalControl();
}

void BAOV_widg::omni_connect()
{
	slot_thr.get_omnibus_obj()->switch_ab(MKO, adr, true);
}

void BAOV_widg::printTime() {
	QString time;
	TimeFirstWord timeFirstWord;
	timeFirstWord.com_word = curr_first_time_word;
	TimeSecondWord timeSecondWord;
	timeSecondWord.com_word = curr_second_time_word;
	int year = 2000 + timeFirstWord.year;
	int hour = timeSecondWord.hours + (timeFirstWord.hours << 4);
	time = time
		.append(QString::number(year))
		.append("/")
		.append(QString::number(timeFirstWord.month))
		.append("/")
		.append(QString::number(timeFirstWord.day))
		.append(" ")
		.append(QString::number(hour))
		.append(":")
		.append(QString::number(timeSecondWord.minutes))
		.append(":")
		.append(QString::number(timeSecondWord.seconds));
	QString msg = "Полученное время: ";
	msg.append(time);
	msg_to_log(msg);
}

void BAOV_widg::update_graphics()
{
	for (int i = 0; i < 4; i++) {
		rc_pbut[i]->setStyleSheet("background-color: rgb(204, 204, 204);");
		if (i < 2) {
			uco_pbut[i]->setStyleSheet("background-color: rgb(204, 204, 204);");
		}
	}
	switch (curr_RC) {
	case RC_1:
		rc_pbut[0]->setStyleSheet("background-color: rgb(142, 198, 156);");
		break;
	case RC_2:
		rc_pbut[1]->setStyleSheet("background-color: rgb(142, 198, 156);");
		break;
	case RC_3:
		rc_pbut[2]->setStyleSheet("background-color: rgb(142, 198, 156);");
		break;
	case RC_4:
		rc_pbut[3]->setStyleSheet("background-color: rgb(142, 198, 156);");
		break;
	}
	switch (curr_UCO) {
	case UCO_1:
		uco_pbut[0]->setStyleSheet("background-color: rgb(142, 198, 156);");
		break;
	case UCO_2:
		uco_pbut[1]->setStyleSheet("background-color: rgb(142, 198, 156);");
		break;
	}

	std::map<int, sensor>::iterator it = addressToEnabledSensorMap.begin();

	int currSensorIdx = 0;
	while (it != addressToEnabledSensorMap.end()) {
		sensor currSensor = it->second;
		QString name = QString::fromLocal8Bit(currSensor.name.c_str());
		string active = currSensor.active ? "Включен" : "Выключен";
		QString activeQStr = QString::fromLocal8Bit(active.c_str());
		string enabled = currSensor.enabled ? "Работоспособный" : "Неработоспособный";
		QString enabledQStr = QString::fromLocal8Bit(enabled.c_str());
		string threshold = to_string(currSensor.detection_threshold);
		QString thresholdQStr = QString::fromLocal8Bit(threshold.c_str());
		active_sensors_info_pbut[currSensorIdx * 4]->setText(name);
		active_sensors_info_pbut[currSensorIdx * 4 + 1]->setText(activeQStr);
		active_sensors_info_pbut[currSensorIdx * 4 + 2]->setText(enabledQStr);
		active_sensors_info_pbut[currSensorIdx * 4 + 3]->setText(thresholdQStr);
		currSensorIdx++;
		it++;
	}
}

BAOV_widg::~BAOV_widg()
{
	slot_thr.quit();
	signal_thr.quit();

	mku_slot_thr.quit();
	mku_signal_thr.quit();

	interrupt_slot_thr.quit();
	interrupt_signal_thr.quit();

	power_slot_thr.quit();
	power_signal_thr.quit();
}

void BAOV_widg::closeEvent(QCloseEvent *event)
{
	QSettings settings(QApplication::applicationDirPath() + "/positions.ini", QSettings::IniFormat);
	settings.setValue("bech_geometry", saveGeometry());
	QWidget::closeEvent(event);
}


bool BAOV_widg::checkIfActiveSensorsMapDoenstContainsBothSensors(int currAddr, int firstActiveAddr, int secondActiveAddr) {
	return (currAddr == firstActiveAddr
		&& addressToEnabledSensorMap.find(secondActiveAddr) == addressToEnabledSensorMap.end())
		|| (currAddr == secondActiveAddr
			&& addressToEnabledSensorMap.find(firstActiveAddr) == addressToEnabledSensorMap.end());
}

void BAOV_widg::doFunctionalControl()
{
	unsigned short _word1 = 0;
	QVariantList tmp_list1;
	QVariantList tmp_list2;
	for (int i = 0; i < 29; i++) {
		tmp_list1.push_back(_word1);
		tmp_list2.push_back(_word1);
	}

	QVariant var1 = tmp_list1.at(0);
	var1 = var1.toInt() + sis_fk_counter;
	tmp_list1[0] = var1;
	QVariant var2 = tmp_list2.at(0);
	var2 = var2.toInt() + sis_fk_counter;
	tmp_list2[0] = var2;
	sis_fk_counter++;
	// в случае УЦО 1 оставляем бит в 0
	switch (curr_UCO) {
		case UCO_2:
			QVariant var = tmp_list1.at(0);
			var = var.toInt() + (1 << 13);
			tmp_list1[0] = var;
			QVariant var2 = tmp_list2.at(0);
			var2 = var2.toInt() + (1 << 13);
			tmp_list2[0] = var;
			break;
	}
	
	QVariant v = tmp_list1.at(0);
	v = v.toInt() + (1 << 14);
	tmp_list1[0] = v;
	QVariant v2 = tmp_list2.at(0);
	v2 = v2.toInt() + (2 << 14);
	tmp_list2[0] = v2;

	TimeFirstWord timeFirstWord;
	timeFirstWord.com_word = curr_first_time_word;
	TimeSecondWord timeSecondWord;
	timeSecondWord.com_word = curr_second_time_word;
	tmp_list1[1] = timeFirstWord.com_word;
	tmp_list1[2] = timeSecondWord.com_word;
	/*QVariant v3 = tmp_list1.at(1);
	v3 = v3.toInt() + timeFirstWord.hours;
	v3 = v3.toInt() + (timeFirstWord.day << 1);
	v3 = v3.toInt() + (timeFirstWord.month << 6);
	v3 = v3.toInt() + (timeFirstWord.year << 10);
	tmp_list1[1] = v3;
	QVariant v4 = tmp_list1.at(2);
	v4 = v4.toInt() + timeSecondWord.seconds;
	v4 = v4.toInt() + (timeSecondWord.minutes << 6);
	v4 = v4.toInt() + (timeSecondWord.hours << 12);
	tmp_list1[2] = v4;*/
	
	std::map<int, functional_control_mapping_item>::iterator it = addressToFKWordMap.begin();

	// доделать контрольную сумму
	while (it != addressToFKWordMap.end()) {

		functional_control_mapping_item item = it->second;

		sensor sensorObj = getSensorForFK(item.address, item.is_enabled_sensor);

		int value = sensorObj.get_field_value(item.field_number);
		value = value << item.start_bit;

		if (item.message_number == 0)
		{
			QVariant var1 = tmp_list1.at(item.data_word_number);
			var1 = var1.toInt() + value;
			tmp_list1[item.data_word_number] = var1;
		}
		else if (item.message_number == 1) {
			QVariant var2 = tmp_list2.at(item.data_word_number);
			var2 = var2.toInt() + value;
			tmp_list2[item.data_word_number] = var2;
		}
		it++;
	}

	tmp_list1.append(tmp_list2);
	slot_thr.get_omnibus_obj()->set_new_data(MKO, adr, 7, tmp_list1);
	//slot_thr.get_omnibus_obj()->set_new_data(MKO, adr, 7, tmp_list2);
	// послать запрос на прерывание BECH_interrupt_run
	firstOKWord.sis_fk_ready = 0;
	sendOperationalControl();
	update_graphics();
}

void BAOV_widg::parseMappingForFunctionalControl()
{
	ifstream inputFileStream("mappingForFunctionalControl.txt");
	int count;
	inputFileStream >> count;
	inputFileStream.ignore(1, '\n');
	for (int i = 0; i < count; i++)
	{
		string line;
		std::getline(inputFileStream, line);
		std::istringstream lineStream(line);

		functional_control_mapping_item item;
		item.address = util_functions::parseValue(lineStream);
		item.is_enabled_sensor = util_functions::parseValue(lineStream);
		item.field_number = util_functions::parseValue(lineStream);
		item.message_number = util_functions::parseValue(lineStream);
		item.data_word_number = util_functions::parseValue(lineStream);
		item.start_bit = util_functions::parseValue(lineStream);
		item.bit_length = util_functions::parseValue(lineStream);

		addressToFKWordMap.insert(std::pair<int, functional_control_mapping_item>(item.address, item));
	}
}

void BAOV_widg::turnOnSensors()
{
	std::map<int, sensor>::iterator it = addressToEnabledSensorMap.begin();

	while (it != addressToEnabledSensorMap.end()) {
	
		it->second.active = 1;
		addressToSensorMap.find(it->first)->second.active = 1;
		
		updateSensorStatusInOK(it->second, 1);
		it++;
	}
	sendOperationalControl();
	update_graphics();
}

void BAOV_widg::updateSensorStatusInOK(sensor sensor, int value) {
	switch (sensor.address) {
	case 0x1:
		secondOKWord.brgi_1_1_ok = value;
		thirdOKWord.temperature_mn_brgi_1_ok = value;
		thirdOKWord.temperature_pl_brgi_1_ok = value;
		break;
	case 0x2:
		secondOKWord.brgi_1_2_ok = value;
		thirdOKWord.temperature_mn_brgi_1_ok = value;
		thirdOKWord.temperature_pl_brgi_1_ok = value;
		break;
	case 0x3:
		secondOKWord.brgi_2_1_ok = value;
		thirdOKWord.temperature_mn_brgi_2_ok = value;
		thirdOKWord.temperature_pl_brgi_2_ok = value;
		break;
	case 0x4:
		secondOKWord.brgi_2_2_ok = value;
		thirdOKWord.temperature_mn_brgi_2_ok = value;
		thirdOKWord.temperature_pl_brgi_2_ok = value;
		break;
	case 0x5:
		secondOKWord.brgi_3_1_ok = value;
		thirdOKWord.temperature_mn_brgi_3_ok = value;
		thirdOKWord.temperature_pl_brgi_3_ok = value;
		break;
	case 0x6:
		secondOKWord.brgi_3_2_ok = value;
		thirdOKWord.temperature_mn_brgi_3_ok = value;
		thirdOKWord.temperature_pl_brgi_3_ok = value;
		break;
	case 0x7:
		secondOKWord.brgi_4_1_ok = value;
		thirdOKWord.temperature_mn_brgi_4_ok = value;
		thirdOKWord.temperature_pl_brgi_4_ok = value;
		break;
	case 0x8:
		secondOKWord.brgi_4_2_ok = value;
		thirdOKWord.temperature_mn_brgi_4_ok = value;
		thirdOKWord.temperature_pl_brgi_4_ok = value;
		break;
	case 0x11:
		secondOKWord.dri_1_1_ok = value;
		thirdOKWord.temperature_mn_bo_1_ok = value;
		thirdOKWord.temperature_pl_bo_1_ok = value;
		break;
	case 0x12:
		secondOKWord.dri_1_2_ok = value;
		thirdOKWord.temperature_mn_bo_1_ok = value;
		thirdOKWord.temperature_pl_bo_1_ok = value;
		break;
	case 0x15:
		secondOKWord.dri_2_1_ok = value;
		thirdOKWord.temperature_mn_bo_2_ok = value;
		thirdOKWord.temperature_pl_bo_2_ok = value;
		break;
	case 0x16:
		secondOKWord.dri_2_1_ok = value;
		thirdOKWord.temperature_mn_bo_2_ok = value;
		thirdOKWord.temperature_pl_bo_2_ok = value;
		break;
	case 0x13:
		secondOKWord.dni_drs_1_ok = value;
		thirdOKWord.temperature_mn_bo_1_ok = value;
		thirdOKWord.temperature_pl_bo_1_ok = value;
		break;
	case 0x17:
		secondOKWord.dni_drs_2_ok = value;
		thirdOKWord.temperature_mn_bo_1_ok = value;
		thirdOKWord.temperature_pl_bo_1_ok = value;
		break;
	
	}
}

void BAOV_widg::turnOffSensors(bool isNextActionTurningOffBAOV)
{
	QString msg = "Выключение оптических датчиков.";
	if (isNextActionTurningOffBAOV)
		msg.append(" Скоро будет произведено отключение питания БАО-В.");
	msg_to_log(msg);
	std::map<int, sensor>::iterator it = addressToEnabledSensorMap.begin();

	while (it != addressToEnabledSensorMap.end()) {

		it->second.active = 0;
		addressToSensorMap.find(it->first)->second.active = 0;

		updateSensorStatusInOK(it->second, 0);
		it++;
	}
	sendOperationalControl();
	update_graphics();
}

void BAOV_widg::sendOperationalControl()
{
	QVariantList ok;
	ok.push_back(firstOKWord.com_word);
	ok.push_back(secondOKWord.com_word);
	ok.push_back(thirdOKWord.com_word);
	slot_thr.get_omnibus_obj()->set_new_data(MKO, adr, 1, ok);
}

sensor BAOV_widg::getSensorForFK(int address, bool isEnabled)
{
	sensor sensor;

	if (!isEnabled)
		return addressToSensorMap.find(address)->second;
	else {
		if (address == 0x1) {
			if (addressToEnabledSensorMap.find(address) != addressToEnabledSensorMap.end())
				return addressToEnabledSensorMap.find(0x1)->second;
			else
				return addressToEnabledSensorMap.find(0x2)->second;
		}
		if (address == 0x3) {
			if (addressToEnabledSensorMap.find(address) != addressToEnabledSensorMap.end())
				return addressToEnabledSensorMap.find(0x3)->second;
			else
				return addressToEnabledSensorMap.find(0x4)->second;
		}
		if (address == 0x5) {
			if (addressToEnabledSensorMap.find(address) != addressToEnabledSensorMap.end())
				return addressToEnabledSensorMap.find(0x5)->second;
			else
				return addressToEnabledSensorMap.find(0x6)->second;
		}
		if (address == 0x7) {
			if (addressToEnabledSensorMap.find(address) != addressToEnabledSensorMap.end())
				return addressToEnabledSensorMap.find(0x7)->second;
			else
				return addressToEnabledSensorMap.find(0x8)->second;
		}
		if (address == 0x11) {
			if (addressToEnabledSensorMap.find(address) != addressToEnabledSensorMap.end())
				return addressToEnabledSensorMap.find(0x11)->second;
			else
				return addressToEnabledSensorMap.find(0x12)->second;
		}
		if (address == 0x15) {
			if (addressToEnabledSensorMap.find(address) != addressToEnabledSensorMap.end())
				return addressToEnabledSensorMap.find(0x15)->second;
			else
				return addressToEnabledSensorMap.find(0x16)->second;
		}
	}
	update_graphics();
}


void BAOV_widg::parseInitSensorsData() {
	ifstream inputFileStream("baseSensorData.txt");
	int count;
	inputFileStream >> count;
	inputFileStream.ignore(1, '\n');
	for (int i = 0; i < count; i++)
	{
		string line;
		std::getline(inputFileStream, line);
		std::istringstream lineStream(line);

		string name;
		sensor sensorObj;
		std::getline(lineStream, name, ' ');
		if (name.find("БРГИ", 0) == 0) {
			brgi brgi;
			sensorObj = brgi.parseSensor(lineStream, brgi);
		}
		else if (name.find("ДРИ", 0) == 0) {
			dri dri;
			sensorObj = dri.parseSensor(lineStream, dri);
		}
		else if (name.find("ДНИ", 0) == 0) {
			dni dni;
			sensorObj = dni.parseSensor(lineStream, dni);
		}
		else if (name.find("ДРС", 0) == 0) {
			drs drs;
			sensorObj = drs.parseSensor(lineStream, drs);
		}
		sensorObj.name = name;
		addressToSensorMap.insert(std::pair<int, sensor>(sensorObj.address, sensorObj));
		// выбираем из каждого типа датчика первый  и считаем его заданным в рабочей 
		// конфигурации датчиков
		if (sensorObj.enabled && (checkIfActiveSensorsMapDoenstContainsBothSensors(sensorObj.address, 0x1, 0x2)
			|| checkIfActiveSensorsMapDoenstContainsBothSensors(sensorObj.address, 0x3, 0x4)
			|| checkIfActiveSensorsMapDoenstContainsBothSensors(sensorObj.address, 0x5, 0x6)
			|| checkIfActiveSensorsMapDoenstContainsBothSensors(sensorObj.address, 0x7, 0x8)
			|| checkIfActiveSensorsMapDoenstContainsBothSensors(sensorObj.address, 0x11, 0x12)
			|| checkIfActiveSensorsMapDoenstContainsBothSensors(sensorObj.address, 0x15, 0x16)
			|| checkIfActiveSensorsMapDoenstContainsBothSensors(sensorObj.address, 0x13, 0x17))) {
			addressToEnabledSensorMap.insert(std::pair<int, sensor>(sensorObj.address, sensorObj));
		}
	}
	update_graphics();
}

void BAOV_widg::generateSIS() {
	generateSIS(false);
}

void BAOV_widg::generateSIS(bool isImit) {
	generateSIS(isImit, false);
	generateSIS(isImit, true);
}

void BAOV_widg::generateSIS(bool isImit, bool isSISNII) {

	QVariantList sis;

	// на каждое сообщение отдельный файл
	string fileName = "sisData/sis";
	if (isSISNII)
		fileName.append("NII");
	if (isImit)
		fileName.append("Imit");

	int msgCounter = 0;

	while (true) {

		sis.clear();

		string currFileName = fileName + std::to_string(msgCounter) + ".txt";
		msgCounter++;
		ifstream inputFileStream(currFileName);
		if (!inputFileStream.good()) {
			if (msgCounter - 1 == 0) {
				msg_to_log("Нет данных для формирования СИС");
			}
			break;
		}

		int count;
		inputFileStream >> count;
		inputFileStream.ignore(1, '\n');

		string line;
		std::getline(inputFileStream, line);
		int firstWord = stoi(line);
		firstWord += sis_counter;
		sis.push_back(firstWord);

		TimeFirstWord timeFirstWord;
		timeFirstWord.com_word = curr_first_time_word;
		TimeSecondWord timeSecondWord;
		timeSecondWord.com_word = curr_second_time_word;

		sis.push_back(timeFirstWord.com_word);
		sis.push_back(timeSecondWord.com_word);

		for (int i = 1; i < count; i++)
		{
			string line;
			std::getline(inputFileStream, line);
			int lineInt = stoi(line);
			sis.push_back(lineInt);
			// доделать контрольную сумму
		}
		BAOV_interrupt_setup();
		slot_thr.get_omnibus_obj()->set_new_data(MKO, adr, 5, sis);
	}
}
