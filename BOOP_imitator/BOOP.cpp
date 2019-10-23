#include "BOOP.h"
#include <QMessageBox>
#include "rpc_ports.h"
#include <qsettings.h>

const double Angle::ShD_bit = 9.7031268;
const double Angle::DU_bit = 0.61799;


union MKOCommandWord
{
	MKOCommandWord(quint16 raw_word)
	{
		this->command_word = raw_word;
	}

	quint16 command_word;

	struct {
		quint16 words_count : 5,
				subaddress : 5,
				transaction_direction : 1,
				address : 5;
	};
};

union MKODataWords
{
    MKODataWords(QVariantList raw_words)
    {
        for (int i = 0; i < raw_words.size(); i++)
            this->data_words[i] = static_cast<quint16>(raw_words.at(i).toInt());
    }

    quint16 data_words[6];

    struct {
    	quint16 nu_servo_power : 1,
				phi_servo_power : 1,
				nu_angle_sensor_power : 1,
				phi_angle_sensor_power : 1,
				nu_rotation_command : 1,
				phi_rotation_command : 1,
				: 10;

    	quint16 nu_pulse_amount : 15,
				nu_rotation_direction : 1;

    	quint16 phi_pulse_amount : 15,
				phi_rotation_direction : 1;

    	quint16 uplsilon_pulse_frequency: 9,
				: 7;

    	quint16 phi_pulse_frequency: 9,
				: 7;

    	quint16 checksum;
	};
};

union BOOPRespondWord
{
	BOOPRespondWord()
	{
		// Form respond word
	}

	quint16 respond_word;

	struct {
		quint16 terminal_device_malfunction : 1,
				: 1,
				caller_malfunction : 1,
				caller_busyness : 1,
				: 5,
				respond_word_transaction : 1,
				message_error : 1,
				terminal_device_address : 5;
	};
};



BOOP::BOOP()
{
  // Contains other containers

  // First stripe of the gui
  // Contains temperature block and half-set selection block

  //  // Temp control block

  QLabel *temperatureLabel1 = new QLabel("Температура ", this);
  QLabel *temperatureLabel2 = new QLabel("°C", this);

  temperatureEdit = new QLineEdit("20", this);
  temperatureEdit->setAlignment(Qt::AlignCenter);
  temperatureEdit->setFixedWidth(42);

  QHBoxLayout *temperatureBlock = new QHBoxLayout();
  temperatureBlock->addWidget(temperatureLabel1);
  temperatureBlock->addWidget(temperatureEdit);
  temperatureBlock->addWidget(temperatureLabel2);

  //  // Half-set selection block

  mainSetButton = new QPushButton("Основной полукомплект", this);
  mainSetButton->setFixedWidth(180);
	mainSetButton->setStyleSheet("background-color: rgb(204, 204, 204);");

  reserveSetButton = new QPushButton("Резервный полукомплект", this);
  reserveSetButton->setFixedWidth(180);
	reserveSetButton->setStyleSheet("background-color: rgb(204, 204, 204);");

  QHBoxLayout *halfSetBlock = new QHBoxLayout();
  halfSetBlock->addWidget(mainSetButton);
  halfSetBlock->addWidget(reserveSetButton);

  QHBoxLayout *firstStripe = new QHBoxLayout();
  firstStripe->addLayout(temperatureBlock);
  firstStripe->addStretch(1);
  firstStripe->addLayout(halfSetBlock);

  // Self-diagnostic block

	//  // First self-diagnostic column

	//bit19 = new QCheckBox("Исправность управляющего микроконвертора", this);
	//bit19->setChecked(true);

	//bit18 = new QCheckBox("Исправность микроконвертора-драйвера ШД", this);
	//bit18->setChecked(true);

	//bit17 = new QCheckBox("Исправность интерфейса SPI с ОУ МКО", this);
	//bit17->setChecked(true);

	//bit16 = new QCheckBox("Исправность интерфейса с ДУ (канал υ)", this);
	//bit16->setChecked(true);

	//bit15 = new QCheckBox("Исправность интерфейса с ДУ (канал φ)", this);
	//bit15->setChecked(true);

 // QVBoxLayout *firstDiagnosticColumn = new QVBoxLayout();
 // firstDiagnosticColumn->addWidget(bit19);
 // firstDiagnosticColumn->addWidget(bit18);
 // firstDiagnosticColumn->addWidget(bit17);
 // firstDiagnosticColumn->addWidget(bit16);
 // firstDiagnosticColumn->addWidget(bit15);

	////  // Second self-diagnostic column

	//bit14 = new QCheckBox("Исправность ОУ МКО (основной канал)", this);
	//bit14->setChecked(true);

	//bit13 = new QCheckBox("Исправность ОУ МКО (резервный канал)", this);
	//bit13->setChecked(true);

	//bit12 = new QCheckBox("Исправность термодатчика", this);
	//bit12->setChecked(true);

	//bit4 = new QCheckBox("Интегральный признак исправности БУП(Е)", this);
	//bit4->setChecked(true);

 // QVBoxLayout *secondDiagnosticColumn = new QVBoxLayout();
 // secondDiagnosticColumn->addWidget(bit14);
 // secondDiagnosticColumn->addWidget(bit13);
 // secondDiagnosticColumn->addWidget(bit12);
 // secondDiagnosticColumn->addWidget(bit4);

 // QHBoxLayout* secondStripe = new QHBoxLayout();
 // secondStripe->addLayout(firstDiagnosticColumn);
 // secondStripe->addStretch(1);
 // secondStripe->addLayout(secondDiagnosticColumn);

  // Servo controls block

  // // nu channel controls

  // // // First nu channel controls strip

  QLabel *nuAngleLabel = new QLabel("Угол: ", this);
  nuAngleValue = new QLabel("0000", this);

  nuAnglePiValue = new QLabel("000", this);

  QHBoxLayout *nuAngleIndicatorBloc = new QHBoxLayout();
  nuAngleIndicatorBloc->addWidget(nuAngleLabel);
  nuAngleIndicatorBloc->addWidget(nuAngleValue);
  


  QHBoxLayout *nuAnglePiIndicatorBloc = new QHBoxLayout();
  nuAnglePiIndicatorBloc->addWidget(nuAnglePiValue);
  

  nuAngleSensorPower = new QCheckBox("ДУ", this);
 // nuAngleSensorPower->setFixedWidth(84);

  nuAngleServoPower = new QCheckBox("ШД", this);
 // nuAngleServoPower->setFixedWidth(84);

  QHBoxLayout *nuDuShwPanel = new QHBoxLayout();
  nuDuShwPanel->addWidget(nuAngleSensorPower);
  nuDuShwPanel->addWidget(nuAngleServoPower);


  QHBoxLayout *nuFirstControlsStrip = new QHBoxLayout();
  nuFirstControlsStrip->addLayout(nuAngleIndicatorBloc);
  nuFirstControlsStrip->addLayout(nuAnglePiIndicatorBloc);
 // nuFirstControlsStrip->addWidget(nuAngleSensorPower);
  nuFirstControlsStrip->addLayout(nuDuShwPanel);
 // nuFirstControlsStrip->addStretch(1);
  // // // Second nu channel controls strip

  QLabel *nuRotationLabel = new QLabel("Вращение", this);
 // nuRotationLabel->setFixedWidth(64);

  decreaseNuAngle = new QPushButton("−", this);
//  decreasenuAngle->setFixedWidth(40);

  increaseNuAngle = new QPushButton("+", this);
 // increasenuAngle->setFixedWidth(40);


  QLabel *nuSawLabel = new QLabel("Пила ", this);
  nuSawNumber = new QLabel("0", this);

  QHBoxLayout *nuThirdControlsStrip = new QHBoxLayout();
  nuThirdControlsStrip->addWidget(nuRotationLabel);
 // nuThirdControlsStrip->addStretch(1);
  nuThirdControlsStrip->addWidget(decreaseNuAngle);
  nuThirdControlsStrip->addWidget(increaseNuAngle);
  //nuThirdControlsStrip->addStretch(1);
  nuThirdControlsStrip->addWidget(nuSawLabel);
  nuThirdControlsStrip->addWidget(nuSawNumber);

  QVBoxLayout *nuChannelControls = new QVBoxLayout();
  nuChannelControls->addLayout(nuFirstControlsStrip);
  nuChannelControls->addLayout(nuThirdControlsStrip);

  QGroupBox *nuGoupBox = new QGroupBox("Канал υ", this);
  nuGoupBox->setLayout(nuChannelControls);

  // // Phi channel controls

  // // // First Phi channel controls strip

  QLabel *phiAngleLabel = new QLabel("Угол: ", this);
  phiAngleValue = new QLabel("0000", this);
  phiAnglePiValue = new QLabel("000", this);

  QHBoxLayout *phiAngleIndicatorBloc = new QHBoxLayout();
  phiAngleIndicatorBloc->addWidget(phiAngleLabel);
  phiAngleIndicatorBloc->addWidget(phiAngleValue);

  QHBoxLayout *phiAnglePiIndicatorBloc = new QHBoxLayout();
  phiAnglePiIndicatorBloc->addWidget(phiAnglePiValue);
  

  phiAngleSensorPower = new QCheckBox("ДУ", this);
//  phiAngleSensorPower->setFixedWidth(84);

  phiAngleServoPower = new QCheckBox("ШД", this);
//  phiAngleServoPower->setFixedWidth(84);

  QHBoxLayout *phiDuShwPanel = new QHBoxLayout();
  phiDuShwPanel->addWidget(phiAngleSensorPower);
  phiDuShwPanel->addWidget(phiAngleServoPower);

  QHBoxLayout *phiFirstControlsStrip = new QHBoxLayout();
  phiFirstControlsStrip->addLayout(phiAngleIndicatorBloc);
  phiFirstControlsStrip->addLayout(phiAnglePiIndicatorBloc);
 // phiFirstControlsStrip->addStretch(1);
  phiFirstControlsStrip->addLayout(phiDuShwPanel);
 // phiFirstControlsStrip->addWidget(phiAngleSensorPower);
 // phiFirstControlsStrip->addWidget(phiAngleServoPower);
 // phiFirstControlsStrip->addWidget(phiSawLabel);

  // // // Second phi channel controls strip

  QLabel *phiRotationLabel = new QLabel("Вращение", this);
//  phiRotationLabel->setFixedWidth(64);

  decreasePhiAngle = new QPushButton("−", this);
//  decreasePhiAngle->setFixedWidth(40);

  increasePhiAngle = new QPushButton("+", this);
//  increasePhiAngle->setFixedWidth(40);

  QLabel *phiSawLabel = new QLabel("Пила ", this);

  phiSawNumber = new QLabel("0", this);

  QHBoxLayout *phiThirdControlsStrip = new QHBoxLayout();
  phiThirdControlsStrip->addWidget(phiRotationLabel);
 // phiThirdControlsStrip->addStretch(1);
  phiThirdControlsStrip->addWidget(decreasePhiAngle);
  phiThirdControlsStrip->addWidget(increasePhiAngle);
 // phiThirdControlsStrip->addStretch(1);
  phiThirdControlsStrip->addWidget(phiSawLabel);
  phiThirdControlsStrip->addWidget(phiSawNumber);

  QVBoxLayout *phiChannelControls = new QVBoxLayout();
  phiChannelControls->addLayout(phiFirstControlsStrip);
  phiChannelControls->addLayout(phiThirdControlsStrip);

  QGroupBox *phiGroupBox = new QGroupBox("Канал φ", this);
  phiGroupBox->setLayout(phiChannelControls);

  QHBoxLayout* thirdStripe = new QHBoxLayout();
  thirdStripe->addWidget(nuGoupBox);
 // thirdStripe->addStretch(1);
  thirdStripe->addWidget(phiGroupBox);

  // Log message area

  logArea = new QTextEdit();

  QVBoxLayout* mainLayout = new QVBoxLayout(this);
  mainLayout->addLayout(firstStripe);
//  mainLayout->addLayout(secondStripe);
  mainLayout->addLayout(thirdStripe);
  mainLayout->addWidget(logArea);



	this->setWindowTitle("Блок управления приводами");
	this->setLayout(mainLayout);
	this->setFocus();
	this->show();


	QFile ang_ini(QApplication::applicationDirPath() + "/angles.ini");

	if (!ang_ini.exists())
	{
		QSettings angles(QApplication::applicationDirPath() + "/angles.ini", QSettings::IniFormat);

		angles.setValue("cur_pos_nu", 0xCB6B);
		angles.setValue("cur_pos_phi", 0x37A2);

		angles.setValue("null_pos_nu", 0xCB6B);
		angles.setValue("null_pos_phi", 0x37A2);

		angles.setValue("saw_nu", 0);
		angles.setValue("saw_phi", 0);

		angles.setValue("plus_stop_nu", 0xC3D5);
		angles.setValue("plus_stop_phi", 0x300D);

		angles.setValue("minus_stop_nu", 0xD301);
		angles.setValue("minus_stop_phi", 0x3F38);

		angles.sync();
	}

	QSettings angles(QApplication::applicationDirPath() + "/angles.ini", QSettings::IniFormat);

	nu_angle.cur_pos = angles.value("cur_pos_nu").toInt();
	phi_angle.cur_pos = angles.value("cur_pos_phi").toInt();

	nu_angle.null_pos = angles.value("null_pos_nu").toInt();
	phi_angle.null_pos = angles.value("null_pos_phi").toInt();

	nu_angle.saw_n = angles.value("saw_nu").toInt();
	phi_angle.saw_n = angles.value("saw_phi").toInt();

	nu_angle.plus_stop = angles.value("plus_stop_nu").toInt();
	phi_angle.plus_stop = angles.value("plus_stop_phi").toInt();

	nu_angle.minus_stop = angles.value("minus_stop_nu").toInt();
	phi_angle.minus_stop = angles.value("minus_stop_phi").toInt();
 
	nu_angle.calc_angle();
	phi_angle.calc_angle();

	nuAngleValue->setText(QString("%1").arg(nu_angle.cur_pos, 0, 16).toUpper());
	phiAngleValue->setText(QString("%1").arg(phi_angle.cur_pos, 0, 16).toUpper());
  
	nuAnglePiValue->setText(QString("%1").arg(nu_angle.angle_string));
	phiAnglePiValue->setText(QString("%1").arg(nu_angle.angle_string));

	nuSawNumber->setText(QString("%1").arg(nu_angle.saw_n));
	phiSawNumber->setText(QString("%1").arg(phi_angle.saw_n));

	slot_thr.set_connection_params("127.0.0.1", OMNIBUS_SLOT);
	slot_thr.start();

	signal_thr.set_connection_params("127.0.0.1", OMNIBUS_SIGNAL);
	signal_thr.start();

	if (!slot_thr.wait_connected(3) || !signal_thr.wait_connected(3))
	{
		QMessageBox::critical(0, "Нет соединения", "Ошибка соединения с rpc_omnibus");
	 
	  
	}

	connect(signal_thr.get_obj().get(), SIGNAL(new_message(QVariant, int, int, int, QVariantList, int)), this, SLOT(new_message(QVariant, int, int, int, QVariantList, int)), Qt::QueuedConnection);

	mku_slot_thr.set_connection_params("127.0.0.1", MKU_SLOT);
	mku_slot_thr.start();

	mku_signal_thr.set_connection_params("127.0.0.1", MKU_SIGNAL);
	mku_signal_thr.start();

	if (!mku_slot_thr.wait_connected(3) || !mku_signal_thr.wait_connected(3))
	{
		QMessageBox::critical(0, "Нет соединения", "Ошибка соединения с mku_bus");

	}

	connect(mku_signal_thr.get_obj().get(), SIGNAL(new_mk(int, int, int, int, double, double, int, int, int)), this, SLOT(new_matrix_command(int, int, int, int, double, double, int, int, int)));
	timer_for_msg->setInterval(1000);
	connect(timer_for_msg, SIGNAL(timeout()), this, SLOT(move_boop()));

	increaseNuAngle->setStyleSheet("background-color: rgb(204, 204, 204);");
	decreaseNuAngle->setStyleSheet("background-color: rgb(204, 204, 204);");
	increasePhiAngle->setStyleSheet("background-color: rgb(204, 204, 204);");
	decreasePhiAngle->setStyleSheet("background-color: rgb(204, 204, 204);");



	QSettings settings(QApplication::applicationDirPath() + "/positions.ini", QSettings::IniFormat);
	restoreGeometry(settings.value("boop_geometry").toByteArray());
}
BOOP::~BOOP()
{
	slot_thr.get_omnibus_obj()->switch_ab(1, 9, false);
	slot_thr.quit();
	signal_thr.quit();

	mku_slot_thr.quit();
	mku_signal_thr.quit();
}

void BOOP::new_message(QVariant dt, int MKO, int line, int command_word, QVariantList words, int respond_word)
{
	if (respond_word == -1)
		return;

	MKOCommandWord parsed_command_word(command_word);

	if ((MKO != this->MKO) || (parsed_command_word.address != this->address) || (parsed_command_word.subaddress != 3))
		return;

	if (parsed_command_word.words_count != 6 || words.count() != 6)
		return;

	int _checksum = 0;
	for (int i = 0; i < words.count() - 1; i++)
		_checksum += words.at(i).toInt();

	QString _message;

	if ((_checksum&0xFFFF) != words.last().toInt())
	{
		word_for_cbk.previous_message_error = 1;
		_message = QString("[%1] принял некорректный массив на подадресе %2 c КС %3")
			.arg(QTime::currentTime().toString("hh:mm:ss.zzz"))
			.arg(parsed_command_word.subaddress).arg(parsed_command_word.command_word);
	}
	else
	{

		MKODataWords parsed_data_words(words);

		_message = QString("[%1] принял корректный массив на подадресе %2 c КС %3")
			.arg(QTime::currentTime().toString("hh:mm:ss.zzz"))
			.arg(parsed_command_word.subaddress).arg(parsed_command_word.command_word);

		word_for_cbk.previous_message_error = 0;

		if (parsed_data_words.nu_servo_power)
		{
			nuAngleServoPower->setChecked(true);
			word_for_cbk.nu_servo_power_status = 1;
		}
		else
		{
			nuAngleServoPower->setChecked(false);
			word_for_cbk.nu_servo_power_status = 0;
		}
		if (parsed_data_words.phi_servo_power)
		{
			phiAngleServoPower->setChecked(true);
			word_for_cbk.phi_servo_power_status = 1;
		}
		else
		{
			phiAngleServoPower->setChecked(false);
			word_for_cbk.phi_servo_power_status = 0;
		}
		if (parsed_data_words.nu_angle_sensor_power)
		{
			nuAngleSensorPower->setChecked(true);
			word_for_cbk.nu_angle_sensor_power_status = 1;
		}
		else
		{
			nuAngleSensorPower->setChecked(false);
			word_for_cbk.nu_angle_sensor_power_status = 0;
		}
		if (parsed_data_words.phi_angle_sensor_power)
		{
			phiAngleSensorPower->setChecked(true);
			word_for_cbk.phi_angle_sensor_power_status = 1;
		}
		else
		{
			phiAngleSensorPower->setChecked(false);
			word_for_cbk.phi_angle_sensor_power_status = 0;
		}
		//тут еще должен быть признак ошибки по контрольной сумме
		word_for_cbk.temperature = temperatureEdit->text().toInt(nullptr, 16);
		//частота
		word_for_cbk.nu_pulse_frequency = parsed_data_words.uplsilon_pulse_frequency;
		word_for_cbk.phi_pulse_frequency = parsed_data_words.phi_pulse_frequency;

		//время в сек
		
		if (parsed_data_words.nu_rotation_command || parsed_data_words.phi_rotation_command)
		{
			nu_rotation_direction = parsed_data_words.nu_rotation_direction;
			phi_rotation_direction = parsed_data_words.phi_rotation_direction;
			timer_for_msg->start();
		}

		if (parsed_data_words.nu_rotation_command)
		{
			//int _nu_current_angle = nuAngleValue->text().toInt(nullptr, 16);

			word_for_cbk.nu_channel_work_status = 1;
			word_for_cbk.nu_rotation_direction = parsed_data_words.nu_rotation_direction;
			word_for_cbk.nu_pulse_amount = parsed_data_words.nu_pulse_amount; // уточнить у Олега

			if (word_for_cbk.nu_pulse_frequency)
				nu_sec = parsed_data_words.nu_pulse_amount / word_for_cbk.nu_pulse_frequency;
			else
				nu_sec = 0;
		}

		if (parsed_data_words.phi_rotation_command)
		{
			

			word_for_cbk.phi_channel_work_status = 1;
			word_for_cbk.phi_rotation_direction = parsed_data_words.phi_rotation_direction;
			word_for_cbk.phi_pulse_amount = parsed_data_words.phi_pulse_amount; // уточнить у Олега

			if (word_for_cbk.phi_pulse_frequency)
				phi_sec = parsed_data_words.phi_pulse_amount / word_for_cbk.phi_pulse_frequency;
			else
				phi_sec = 0;
		}

		//признаки самоконтроля БУП(Е)
		//bit19->isChecked() ? word_for_cbk.bit19 = 1 : word_for_cbk.bit19 = 0;
		//bit18->isChecked() ? word_for_cbk.bit18 = 1 : word_for_cbk.bit18 = 0;
		//bit17->isChecked() ? word_for_cbk.bit17 = 1 : word_for_cbk.bit17 = 0;
		//bit16->isChecked() ? word_for_cbk.bit16 = 1 : word_for_cbk.bit16 = 0;
		//bit15->isChecked() ? word_for_cbk.bit15 = 1 : word_for_cbk.bit15 = 0;
		//bit14->isChecked() ? word_for_cbk.bit14 = 1 : word_for_cbk.bit14 = 0;
		//bit12->isChecked() ? word_for_cbk.bit13 = 1 : word_for_cbk.bit12 = 0;
		//bit4->isChecked() ? word_for_cbk.bit4 = 1 : word_for_cbk.bit4 = 0;

		word_for_cbk.bit19 = 1;
		word_for_cbk.bit18 = 1;
		word_for_cbk.bit17 = 1;
		word_for_cbk.bit16 = 1;
		word_for_cbk.bit15 = 1;
		word_for_cbk.bit14 = 1;
		word_for_cbk.bit13 = 1;
		word_for_cbk.bit4 = 1;

	}

	//контрольная сумма
	for (int i = 0; i < 10; i++)
		word_for_cbk.checksum += word_for_cbk.data_words[i];
	word_for_cbk.checksum = word_for_cbk.checksum & 0xFFFF;

	logArea->append(_message);

	new_tm();
}

void BOOP::new_tm()
{

	QVariantList tmp_list;
	for (int i = 0; i < 11; i++)
		tmp_list.push_back(word_for_cbk.data_words[i]);
	slot_thr.get_omnibus_obj()->set_new_data(MKO, address, 2, tmp_list);
}

void BOOP::new_matrix_command(int mshm, int pshm, int length_m, int length_p, double u_m, double u_p, int dt, int line_m, int line_p)
{
	if (pshm != 1 || mshm < 5 || mshm > 7)
		return;

	QString _message = QString("%1 принял МК МШ%2 ПШ%3")
		.arg(QTime::currentTime().toString("hh:mm:ss.zzz"))
		.arg(mshm).arg(pshm);

	if (mshm == 5) {
		slot_thr.get_omnibus_obj()->switch_ab(1, 9, true);
		mainSetButton->setStyleSheet("background-color: rgb(142, 198, 156);");
		reserveSetButton->setStyleSheet("background-color: rgb(204, 204, 204);");
		nu_angl_amount = 0;
		phi_angl_amount = 0;
		
		word_for_cbk.half_set_engage = 0;
		new_tm();
	}

	if (mshm == 6) {
		slot_thr.get_omnibus_obj()->switch_ab(1, 9, true);
		reserveSetButton->setStyleSheet("background-color: rgb(142, 198, 156);");
		mainSetButton->setStyleSheet("background-color: rgb(204, 204, 204);");
		nu_angl_amount = 0;
		phi_angl_amount = 0;
		
		word_for_cbk.half_set_engage = 1;
		new_tm();
	}

	if (mshm == 7) {
		slot_thr.get_omnibus_obj()->switch_ab(1, 9, false);
		mainSetButton->setStyleSheet("background-color: rgb(204, 204, 204);");
		reserveSetButton->setStyleSheet("background-color: rgb(204, 204, 204);");
		
	}

	logArea->append(_message);
}


void BOOP::closeEvent(QCloseEvent *event)
{

	QSettings settings(QApplication::applicationDirPath() + "/positions.ini", QSettings::IniFormat);
	settings.setValue("boop_geometry", saveGeometry());
	QWidget::closeEvent(event);
}


void Angle::calc_angle(int steps)
{
	if (steps)
	{
		cur_pos += (steps*ShD_bit/DU_bit);
		if (saw_n == -1)
			if (cur_pos <= minus_stop)
				cur_pos = minus_stop;

		if (saw_n == 1)
			if (cur_pos >= plus_stop)
				cur_pos = plus_stop;


		if (cur_pos > 0xFFFF)
		{
			cur_pos = cur_pos - 0xFFFF;
			saw_n++;
		}

		if (cur_pos < 0)
		{
			cur_pos = 0xFFFF - cur_pos;
			saw_n--;
		}

	}

	_angle = ((cur_pos + (saw_n + 1) * 0xFFFF) - (null_pos + 0xFFFF))*DU_bit;
	int tmp_angle = (int)(_angle/3600);
	int tmp_angle_min = _angle - tmp_angle*3600;
	int tmp_angle_min_full = (int)(tmp_angle_min / 60);
	int tmp_angle_sec = tmp_angle_min - tmp_angle_min_full*60;
	angle_string = QString("%1°%2'%3\"").arg(tmp_angle).arg(tmp_angle_min_full).arg(tmp_angle_sec);
}

void BOOP::move_boop()
{

	if (nu_sec)
	{
		nu_sec--;

		if (nu_rotation_direction)
		{
			nu_angle.calc_angle(word_for_cbk.nu_pulse_frequency);
			nu_angl_amount += word_for_cbk.nu_pulse_frequency;

			increaseNuAngle->setStyleSheet("background-color: rgb(142, 198, 156);");
		}
		else
		{
			nu_angle.calc_angle(-1*word_for_cbk.nu_pulse_frequency);
			nu_angl_amount -= word_for_cbk.nu_pulse_frequency;

			decreaseNuAngle->setStyleSheet("background-color: rgb(142, 198, 156);");
		}
		word_for_cbk.nu_angle = nu_angle.cur_pos;
		nuAngleValue->setText(QString("%1").arg(nu_angle.cur_pos, 0, 16).toUpper());
		nuAnglePiValue->setText(QString("%1").arg(nu_angle.angle_string));
	}
	if (phi_sec)
	{
		phi_sec--;

		if (phi_rotation_direction)
		{
			phi_angle.calc_angle(word_for_cbk.phi_pulse_frequency);
			phi_angl_amount += word_for_cbk.phi_pulse_frequency;

			increasePhiAngle->setStyleSheet("background-color: rgb(142, 198, 156);");
		}
		else
		{
			phi_angle.calc_angle(-1 * word_for_cbk.phi_pulse_frequency);
			phi_angl_amount -= word_for_cbk.phi_pulse_frequency;

			decreasePhiAngle->setStyleSheet("background-color: rgb(142, 198, 156);");
		}
		word_for_cbk.phi_angle = phi_angle.cur_pos;
		phiAngleValue->setText(QString("%1").arg(phi_angle.cur_pos, 0, 16).toUpper());
		phiAnglePiValue->setText(QString("%1").arg(phi_angle.angle_string));

	}

	(nu_angl_amount > 0) ? word_for_cbk.nu_summ_sign = 0 : word_for_cbk.nu_summ_sign = 1;
	word_for_cbk.nu_pulse_summ = qAbs(nu_angl_amount);
	(phi_angl_amount > 0) ? word_for_cbk.phi_summ_sign = 0 : word_for_cbk.phi_summ_sign = 1;
	word_for_cbk.phi_pulse_summ = qAbs(phi_angl_amount);

	nuSawNumber->setText(QString("%1").arg(nu_angle.saw_n));
	phiSawNumber->setText(QString("%1").arg(phi_angle.saw_n));
	if (nu_sec == 0)
	{
		increaseNuAngle->setStyleSheet("background-color: rgb(204, 204, 204);");
		decreaseNuAngle->setStyleSheet("background-color: rgb(204, 204, 204);");
	}

	if (phi_sec == 0)
	{
		increasePhiAngle->setStyleSheet("background-color: rgb(204, 204, 204);");
		decreasePhiAngle->setStyleSheet("background-color: rgb(204, 204, 204);");

	}

	if (( nu_sec == 0) && ( phi_sec == 0))
	{
		timer_for_msg->stop();
		QSettings angles(QApplication::applicationDirPath() + "/angles.ini", QSettings::IniFormat);
		angles.setValue("cur_pos_nu", nu_angle.cur_pos);
		angles.setValue("cur_pos_phi", phi_angle.cur_pos);

		angles.setValue("saw_nu", nu_angle.saw_n);
		angles.setValue("saw_phi", phi_angle.saw_n);
	}

	new_tm();

}