#include "BOOP.h"
#include <QMessageBox>
#include "rpc_ports.h"

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
    	quint16 upsilon_servo_power : 1,
				phi_servo_power : 1,
				upsilon_angle_sensor_power : 1,
				phi_angle_sensor_power : 1,
				upsilon_rotation_command : 1,
				phi_rotation_command : 1,
				: 10;

    	quint16 upsilon_pulse_amount : 15,
				upsilon_rotation_direction : 1;

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

	bit19 = new QCheckBox("Исправность управляющего микроконвертора", this);
	bit19->setChecked(true);

	bit18 = new QCheckBox("Исправность микроконвертора-драйвера ШД", this);
	bit18->setChecked(true);

	bit17 = new QCheckBox("Исправность интерфейса SPI с ОУ МКО", this);
	bit17->setChecked(true);

	bit16 = new QCheckBox("Исправность интерфейса с ДУ (канал υ)", this);
	bit16->setChecked(true);

	bit15 = new QCheckBox("Исправность интерфейса с ДУ (канал φ)", this);
	bit15->setChecked(true);

  QVBoxLayout *firstDiagnosticColumn = new QVBoxLayout();
  firstDiagnosticColumn->addWidget(bit19);
  firstDiagnosticColumn->addWidget(bit18);
  firstDiagnosticColumn->addWidget(bit17);
  firstDiagnosticColumn->addWidget(bit16);
  firstDiagnosticColumn->addWidget(bit15);

	//  // Second self-diagnostic column

	bit14 = new QCheckBox("Исправность ОУ МКО (основной канал)", this);
	bit14->setChecked(true);

	bit13 = new QCheckBox("Исправность ОУ МКО (резервный канал)", this);
	bit13->setChecked(true);

	bit12 = new QCheckBox("Исправность термодатчика", this);
	bit12->setChecked(true);

	bit4 = new QCheckBox("Интегральный признак исправности БУП(Е)", this);
	bit4->setChecked(true);

  QVBoxLayout *secondDiagnosticColumn = new QVBoxLayout();
  secondDiagnosticColumn->addWidget(bit14);
  secondDiagnosticColumn->addWidget(bit13);
  secondDiagnosticColumn->addWidget(bit12);
  secondDiagnosticColumn->addWidget(bit4);

  QHBoxLayout* secondStripe = new QHBoxLayout();
  secondStripe->addLayout(firstDiagnosticColumn);
  secondStripe->addStretch(1);
  secondStripe->addLayout(secondDiagnosticColumn);

  // Servo controls block

  // // Upsilon channel controls

  // // // First upsilon channel controls strip

  QLabel *upsilonAngleLabel = new QLabel("Угол: ", this);
  upsilonAngleValue = new QLabel("0000", this);

  QHBoxLayout *upsilonAngleIndicatorBloc = new QHBoxLayout();
  upsilonAngleIndicatorBloc->addWidget(upsilonAngleLabel);
  upsilonAngleIndicatorBloc->addWidget(upsilonAngleValue);

  upsilonAngleSensorPower = new QCheckBox("Питание ДУ", this);
  upsilonAngleSensorPower->setFixedWidth(84);

  QHBoxLayout *upsilonFirstControlsStrip = new QHBoxLayout();
  upsilonFirstControlsStrip->addLayout(upsilonAngleIndicatorBloc);
  upsilonFirstControlsStrip->addStretch(1);
  upsilonFirstControlsStrip->addWidget(upsilonAngleSensorPower);

  // // // Second upsilon channel controls strip

  QLabel *upsilonRotationLabel = new QLabel("Вращение", this);
  upsilonRotationLabel->setFixedWidth(64);

  decreaseUpsilonAngle = new QPushButton("−", this);
  decreaseUpsilonAngle->setFixedWidth(40);

  increaseUpsilonAngle = new QPushButton("+", this);
  increaseUpsilonAngle->setFixedWidth(40);

  upsilonAngleServoPower = new QCheckBox("Питание ШД", this);
  upsilonAngleServoPower->setFixedWidth(84);

  QHBoxLayout *upsilonThirdControlsStrip = new QHBoxLayout();
  upsilonThirdControlsStrip->addWidget(upsilonRotationLabel);
  upsilonThirdControlsStrip->addStretch(1);
  upsilonThirdControlsStrip->addWidget(decreaseUpsilonAngle);
  upsilonThirdControlsStrip->addWidget(increaseUpsilonAngle);
  upsilonThirdControlsStrip->addStretch(1);
  upsilonThirdControlsStrip->addWidget(upsilonAngleServoPower);

  QVBoxLayout *upsilonChannelControls = new QVBoxLayout();
  upsilonChannelControls->addLayout(upsilonFirstControlsStrip);
  upsilonChannelControls->addLayout(upsilonThirdControlsStrip);

  QGroupBox *upsilonGoupBox = new QGroupBox("Канал υ", this);
  upsilonGoupBox->setLayout(upsilonChannelControls);

  // // Phi channel controls

  // // // First Phi channel controls strip

  QLabel *phiAngleLabel = new QLabel("Угол: ", this);
  phiAngleValue = new QLabel("0000", this);

  QHBoxLayout *phiAngleIndicatorBloc = new QHBoxLayout();
  phiAngleIndicatorBloc->addWidget(phiAngleLabel);
  phiAngleIndicatorBloc->addWidget(phiAngleValue);

  phiAngleSensorPower = new QCheckBox("Питание ДУ", this);
  phiAngleSensorPower->setFixedWidth(84);

  QHBoxLayout *phiFirstControlsStrip = new QHBoxLayout();
  phiFirstControlsStrip->addLayout(phiAngleIndicatorBloc);
  phiFirstControlsStrip->addStretch(1);
  phiFirstControlsStrip->addWidget(phiAngleSensorPower);

  // // // Second phi channel controls strip

  QLabel *phiRotationLabel = new QLabel("Вращение", this);
  phiRotationLabel->setFixedWidth(64);

  decreasePhiAngle = new QPushButton("−", this);
  decreasePhiAngle->setFixedWidth(40);

  increasePhiAngle = new QPushButton("+", this);
  increasePhiAngle->setFixedWidth(40);

  phiAngleServoPower = new QCheckBox("Питание ШД", this);
  phiAngleServoPower->setFixedWidth(84);

  QHBoxLayout *phiThirdControlsStrip = new QHBoxLayout();
  phiThirdControlsStrip->addWidget(phiRotationLabel);
  phiThirdControlsStrip->addStretch(1);
  phiThirdControlsStrip->addWidget(decreasePhiAngle);
  phiThirdControlsStrip->addWidget(increasePhiAngle);
  phiThirdControlsStrip->addStretch(1);
  phiThirdControlsStrip->addWidget(phiAngleServoPower);

  QVBoxLayout *phiChannelControls = new QVBoxLayout();
  phiChannelControls->addLayout(phiFirstControlsStrip);
  phiChannelControls->addLayout(phiThirdControlsStrip);

  QGroupBox *phiGroupBox = new QGroupBox("Канал φ", this);
  phiGroupBox->setLayout(phiChannelControls);

  QHBoxLayout* thirdStripe = new QHBoxLayout();
  thirdStripe->addWidget(upsilonGoupBox);
  thirdStripe->addStretch(1);
  thirdStripe->addWidget(phiGroupBox);

  // Log message area

  logArea = new QTextEdit();

  QVBoxLayout* mainLayout = new QVBoxLayout(this);
  mainLayout->addLayout(firstStripe);
  mainLayout->addLayout(secondStripe);
  mainLayout->addLayout(thirdStripe);
  mainLayout->addWidget(logArea);

	// Styles

  //this->setStyleSheet("QPushButton {"
  //                    "min-height: 20px;"
  //                    "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #bbdfc4, stop: 1 #8ec69c);"
  //                    "color: #000000;"
  //                    "border: 1px solid #008000; }"

  //                    "QPushButton:flat {"
  //                    "background-color: #fcfcfc;"
  //                    "color: #4e4e4e;"
  //                    "border: 1px solid #808080;");

	this->setWindowTitle("Блок управления приводами");
	this->setLayout(mainLayout);
	this->setFocus();
	this->show();

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

  connect(signal_thr.get_obj().get(), SIGNAL(new_message(QVariant, int, int, int, QVariantList, int)), this, SLOT(new_message(QVariant, int, int, int, QVariantList, int)), Qt::QueuedConnection);

  mku_slot_thr.set_connection_params("127.0.0.1", MKU_SLOT);
  mku_slot_thr.start();

  mku_signal_thr.set_connection_params("127.0.0.1", MKU_SIGNAL);
  mku_signal_thr.start();

  if (!mku_slot_thr.wait_connected(3) || !mku_signal_thr.wait_connected(3))
  {
	  QMessageBox::critical(0, "Нет соединения", "Ошибка соединения с mku_bus");
	  this->deleteLater();
	  return;
  }

  connect(mku_signal_thr.get_obj().get(), SIGNAL(new_mk(int, int, int, int, double, double, int, int, int)), this, SLOT(new_matrix_command(int, int, int, int, double, double, int, int, int)));
  timer_for_msg->setInterval(1000);
  connect(timer_for_msg, SIGNAL(timeout()), this, SLOT(move_boop()));

  increaseUpsilonAngle->setStyleSheet("background-color: rgb(204, 204, 204);");
  decreaseUpsilonAngle->setStyleSheet("background-color: rgb(204, 204, 204);");
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

		if (parsed_data_words.upsilon_servo_power)
		{
			upsilonAngleServoPower->setChecked(true);
			word_for_cbk.upsilon_servo_power_status = 1;
		}
		else
		{
			upsilonAngleServoPower->setChecked(false);
			word_for_cbk.upsilon_servo_power_status = 0;
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
		if (parsed_data_words.upsilon_angle_sensor_power)
		{
			upsilonAngleSensorPower->setChecked(true);
			word_for_cbk.upsilon_angle_sensor_power_status = 1;
		}
		else
		{
			upsilonAngleSensorPower->setChecked(false);
			word_for_cbk.upsilon_angle_sensor_power_status = 0;
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
		word_for_cbk.uplsilon_pulse_frequency = parsed_data_words.uplsilon_pulse_frequency;
		word_for_cbk.phi_pulse_frequency = parsed_data_words.phi_pulse_frequency;

		//время в сек
		
		if (parsed_data_words.upsilon_rotation_command || parsed_data_words.phi_rotation_command)
		{
			upsilon_rotation_direction = parsed_data_words.upsilon_rotation_direction;
			phi_rotation_direction = parsed_data_words.phi_rotation_direction;
			timer_for_msg->start();
		}

		if (parsed_data_words.upsilon_rotation_command)
		{
			//int _upsilon_current_angle = upsilonAngleValue->text().toInt(nullptr, 16);

			word_for_cbk.upsilon_channel_work_status = 1;
			word_for_cbk.upsilon_rotation_direction = parsed_data_words.upsilon_rotation_direction;
			word_for_cbk.upsilon_pulse_amount = parsed_data_words.upsilon_pulse_amount; // уточнить у Олега

			if (word_for_cbk.uplsilon_pulse_frequency)
				upsilon_sec = parsed_data_words.upsilon_pulse_amount / word_for_cbk.uplsilon_pulse_frequency;
			else
				upsilon_sec = 0;

			

			//if (parsed_data_words.upsilon_rotation_direction)
			//{
			//	_upsilon_current_angle += parsed_data_words.upsilon_pulse_amount;
			//	upsilon_angl_amount += parsed_data_words.upsilon_pulse_amount;// уточнить у Олега
			//	if (_upsilon_current_angle > 0xFFFF)
			//		_upsilon_current_angle = _upsilon_current_angle - 0xFFFF;
			//	increaseUpsilonAngle->setStyleSheet("background-color: rgb(142, 198, 156);");
			//}
			//else
			//{
			//	_upsilon_current_angle -= parsed_data_words.upsilon_pulse_amount;
			//	upsilon_angl_amount -= parsed_data_words.upsilon_pulse_amount;// уточнить у Олега
			//	if (_upsilon_current_angle < 0)
			//		_upsilon_current_angle = 0xFFFF + _upsilon_current_angle;
			//	decreaseUpsilonAngle->setStyleSheet("background-color: rgb(142, 198, 156);");
			//}
			//word_for_cbk.upsilon_angle = _upsilon_current_angle;// уточнить у Олега
			//upsilonAngleValue->setText(QString("%1").arg(_upsilon_current_angle, 0, 16).toUpper());
		}

		if (parsed_data_words.phi_rotation_command)
		{
			//int _phi_current_angle = phiAngleValue->text().toInt(nullptr, 16);

			word_for_cbk.phi_channel_work_status = 1;
			word_for_cbk.phi_rotation_direction = parsed_data_words.phi_rotation_direction;
			word_for_cbk.phi_pulse_amount = parsed_data_words.phi_pulse_amount; // уточнить у Олега

			if (word_for_cbk.phi_pulse_frequency)
				phi_sec = parsed_data_words.phi_pulse_amount / word_for_cbk.phi_pulse_frequency;
			else
				phi_sec = 0;


			//if (parsed_data_words.phi_rotation_direction)
			//{
			//	_phi_current_angle += parsed_data_words.phi_pulse_amount;
			//	phi_angl_amount += parsed_data_words.phi_pulse_amount;// уточнить у Олега
			//	if (_phi_current_angle > 0xFFFF)
			//		_phi_current_angle = _phi_current_angle - 0xFFFF;
			//	increasePhiAngle->setStyleSheet("background-color: rgb(142, 198, 156);");
			//}
			//else
			//{
			//	_phi_current_angle -= parsed_data_words.phi_pulse_amount;
			//	if (_phi_current_angle < 0)
			//		_phi_current_angle = 0xFFFF + _phi_current_angle;
			//	phi_angl_amount -= parsed_data_words.phi_pulse_amount;// уточнить у Олега
			//	decreasePhiAngle->setStyleSheet("background-color: rgb(142, 198, 156);");
			//}

			//word_for_cbk.phi_angle = _phi_current_angle;
			//phiAngleValue->setText(QString("%1").arg(_phi_current_angle, 0, 16).toUpper());
		}


		//(upsilon_angl_amount > 0) ? word_for_cbk.usplison_summ_sign = 0 : word_for_cbk.usplison_summ_sign = 1;
		//word_for_cbk.upsilon_pulse_summ = qAbs(upsilon_angl_amount);
		//(phi_angl_amount > 0) ? word_for_cbk.phi_summ_sign = 0 : word_for_cbk.phi_summ_sign = 1;
		//word_for_cbk.phi_pulse_summ = qAbs(phi_angl_amount);

		//признаки самоконтроля БУП(Е)
		bit19->isChecked() ? word_for_cbk.bit19 = 1 : word_for_cbk.bit19 = 0;
		bit18->isChecked() ? word_for_cbk.bit18 = 1 : word_for_cbk.bit18 = 0;
		bit17->isChecked() ? word_for_cbk.bit17 = 1 : word_for_cbk.bit17 = 0;
		bit16->isChecked() ? word_for_cbk.bit16 = 1 : word_for_cbk.bit16 = 0;
		bit15->isChecked() ? word_for_cbk.bit15 = 1 : word_for_cbk.bit15 = 0;
		bit14->isChecked() ? word_for_cbk.bit14 = 1 : word_for_cbk.bit14 = 0;
		bit12->isChecked() ? word_for_cbk.bit13 = 1 : word_for_cbk.bit12 = 0;
		bit4->isChecked() ? word_for_cbk.bit4 = 1 : word_for_cbk.bit4 = 0;

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
		upsilon_angl_amount = 0;
		phi_angl_amount = 0;
		
		word_for_cbk.half_set_engage = 0;
	}

	if (mshm == 6) {
		slot_thr.get_omnibus_obj()->switch_ab(1, 9, true);
		reserveSetButton->setStyleSheet("background-color: rgb(142, 198, 156);");
		mainSetButton->setStyleSheet("background-color: rgb(204, 204, 204);");
		upsilon_angl_amount = 0;
		phi_angl_amount = 0;
		
		word_for_cbk.half_set_engage = 1;
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

void BOOP::move_boop()
{

	int _upsilon_current_angle = upsilonAngleValue->text().toInt(nullptr, 16);
	int _phi_current_angle = phiAngleValue->text().toInt(nullptr, 16);
	if (upsilon_sec)
	{
		upsilon_sec--;

		if (upsilon_rotation_direction)
		{
			_upsilon_current_angle += word_for_cbk.uplsilon_pulse_frequency;
			upsilon_angl_amount += word_for_cbk.uplsilon_pulse_frequency;// уточнить у Олега
			if (_upsilon_current_angle > 0xFFFF)
				_upsilon_current_angle = _upsilon_current_angle - 0xFFFF;
			increaseUpsilonAngle->setStyleSheet("background-color: rgb(142, 198, 156);");
		}
		else
		{
			_upsilon_current_angle -= word_for_cbk.uplsilon_pulse_frequency;
			if (_upsilon_current_angle < 0)
				_upsilon_current_angle = 0xFFFF + _upsilon_current_angle;
			upsilon_angl_amount -= word_for_cbk.uplsilon_pulse_frequency;// уточнить у Олега
			decreaseUpsilonAngle->setStyleSheet("background-color: rgb(142, 198, 156);");
		}
		word_for_cbk.upsilon_angle = _upsilon_current_angle;// уточнить у Олега
		upsilonAngleValue->setText(QString("%1").arg(_upsilon_current_angle, 0, 16).toUpper());
	}
	if (phi_sec)
	{
		phi_sec--;

		if (phi_rotation_direction)
		{
			_phi_current_angle += word_for_cbk.phi_pulse_frequency;
			phi_angl_amount += word_for_cbk.phi_pulse_frequency;// уточнить у Олега
			if (_phi_current_angle > 0xFFFF)
				_phi_current_angle = _phi_current_angle - 0xFFFF;
			increasePhiAngle->setStyleSheet("background-color: rgb(142, 198, 156);");
		}
		else
		{
			_phi_current_angle -= word_for_cbk.phi_pulse_frequency;
			if (_phi_current_angle < 0)
				_phi_current_angle = 0xFFFF + _phi_current_angle;
			phi_angl_amount -= word_for_cbk.phi_pulse_frequency;// уточнить у Олега
			decreasePhiAngle->setStyleSheet("background-color: rgb(142, 198, 156);");
		}

		word_for_cbk.phi_angle = _phi_current_angle;
		phiAngleValue->setText(QString("%1").arg(_phi_current_angle, 0, 16).toUpper());

	}
	(upsilon_angl_amount > 0) ? word_for_cbk.usplison_summ_sign = 0 : word_for_cbk.usplison_summ_sign = 1;
	word_for_cbk.upsilon_pulse_summ = qAbs(upsilon_angl_amount);
	(phi_angl_amount > 0) ? word_for_cbk.phi_summ_sign = 0 : word_for_cbk.phi_summ_sign = 1;
	word_for_cbk.phi_pulse_summ = qAbs(phi_angl_amount);

	if (upsilon_sec == 0)
	{
		increaseUpsilonAngle->setStyleSheet("background-color: rgb(204, 204, 204);");
		decreaseUpsilonAngle->setStyleSheet("background-color: rgb(204, 204, 204);");
	}

	if (phi_sec == 0)
	{
		increasePhiAngle->setStyleSheet("background-color: rgb(204, 204, 204);");
		decreasePhiAngle->setStyleSheet("background-color: rgb(204, 204, 204);");

	}

	if (( upsilon_sec == 0) && ( phi_sec == 0))
	{
		timer_for_msg->stop();
	}

	new_tm();

}