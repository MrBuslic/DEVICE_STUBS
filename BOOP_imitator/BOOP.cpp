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

union BOOPDataWords
{
	BOOPDataWords()
	{
		// Form data words
	}

	quint16 data_words[13];

	struct {
		quint16 upsilon_servo_power_status : 1,
				phi_servo_power_status : 1,
				upsilon_angle_sensor_power_status : 1,
				phi_angle_sensor_power_status : 1,
				upsilon_channel_work_status : 1,
				phi_channel_work_status : 1,
				previous_message_error : 1,
				half_set_engage : 1,
				temperature : 8;

		quint16 upsilon_pulse_amount : 15,
				upsilon_rotation_direction : 1;

		quint16 phi_pulse_amount : 15,
				phi_rotation_direction : 1;

		quint16 uplsilon_pulse_frequency : 9,
				: 7;

		quint16 phi_pulse_frequency : 9,
				: 7;

		quint16 upsilon_angle;

		quint16 phi_angle;

		quint16 upsilon_pulse_summ : 15,
				usplison_summ_sign : 1;

		quint16 phi_pulse_summ : 15,
				phi_summ_sign : 1;

		// Self-control bits
		quint16 bit19 : 1,
				bit18 : 1,
				bit17 : 1,
				bit16 : 1,
				bit15 : 1,
				bit14 : 1,
				bit13 : 1,
				bit12 : 1,
				: 7,
				bit4 : 1;

		quint16 upsilon_min_angle;

		quint16 phi_min_angle;

		quint16 checksum;
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

  temperatureEdit = new QLineEdit("-10", this);
  temperatureEdit->setAlignment(Qt::AlignCenter);
  temperatureEdit->setFixedWidth(42);

  QHBoxLayout *temperatureBlock = new QHBoxLayout();
  temperatureBlock->addWidget(temperatureLabel1);
  temperatureBlock->addWidget(temperatureEdit);
  temperatureBlock->addWidget(temperatureLabel2);

  //  // Half-set selection block

  mainSetButton = new QPushButton("Основной полукомплект", this);
  mainSetButton->setFixedWidth(180);
	mainSetButton->setFlat(true);

  reserveSetButton = new QPushButton("Резервный полукомплект", this);
  reserveSetButton->setFixedWidth(180);
	reserveSetButton->setFlat(true);

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
	decreaseUpsilonAngle->setFlat(true);

  increaseUpsilonAngle = new QPushButton("+", this);
  increaseUpsilonAngle->setFixedWidth(40);
	increaseUpsilonAngle->setFlat(true);

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

  QPushButton *decreasePhiAngle = new QPushButton("−", this);
  decreasePhiAngle->setFixedWidth(40);
	decreasePhiAngle->setFlat(true);

  QPushButton *increasePhiAngle = new QPushButton("+", this);
  increasePhiAngle->setFixedWidth(40);
	increasePhiAngle->setFlat(true);

  QCheckBox *phiAngleServoPower = new QCheckBox("Питание ШД", this);
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

  this->setStyleSheet("QPushButton {"
                      "min-height: 20px;"
                      "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #bbdfc4, stop: 1 #8ec69c);"
                      "color: #000000;"
                      "border: 1px solid #008000; }"

                      "QPushButton:flat {"
                      "background-color: #fcfcfc;"
                      "color: #4e4e4e;"
                      "border: 1px solid #808080;");

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

	if ((MKO != this->MKO) || (parsed_command_word.address != this->address))
		return;

	if (parsed_command_word.words_count != 6 || words.count() != 6)
		return;

	int _checksum = 0;
	for (int i = 0; i < words.count() - 1; i++)
		_checksum += words.at(i).toInt();

	if (_checksum != words.last().toInt())
		return;

	MKODataWords parsed_data_words(words);

	QString _message = QString("[%1] принял сигнал на подадресе %2 c КС %3")
		                 .arg(QTime::currentTime().toString("hh:mm:ss.zzz"))
										 .arg(parsed_command_word.subaddress).arg(parsed_command_word.command_word);

	if (parsed_data_words.upsilon_servo_power)
		upsilonAngleServoPower->setChecked(true);
	else
		upsilonAngleServoPower->setChecked(false);

	if (parsed_data_words.phi_servo_power)
		phiAngleServoPower->setChecked(true);
	else
		phiAngleServoPower->setChecked(false);

	if (parsed_data_words.upsilon_angle_sensor_power)
		upsilonAngleSensorPower->setChecked(true);
	else
		upsilonAngleSensorPower->setChecked(false);

	if (parsed_data_words.phi_angle_sensor_power)
		phiAngleSensorPower->setChecked(true);
	else
		phiAngleSensorPower->setChecked(false);

	if (parsed_data_words.upsilon_rotation_command)
	{
		int _upsilon_current_angle = upsilonAngleValue->text().toInt(nullptr, 16);

		if (parsed_data_words.upsilon_rotation_direction)
			_upsilon_current_angle += parsed_data_words.upsilon_pulse_amount;
		else
			_upsilon_current_angle -= parsed_data_words.upsilon_pulse_amount;

		upsilonAngleValue->setText(QString("%1").arg(_upsilon_current_angle, 0, 16).toUpper());
	}

	if (parsed_data_words.phi_rotation_command)
	{
		int _phi_current_angle = phiAngleValue->text().toInt(nullptr, 16);

		if (parsed_data_words.phi_rotation_direction)
			_phi_current_angle += parsed_data_words.phi_pulse_amount;
		else
			_phi_current_angle -= parsed_data_words.phi_pulse_amount;

		phiAngleValue->setText(QString("%1").arg(_phi_current_angle, 0, 16).toUpper());
	}

	logArea->append(_message);
}
void BOOP::new_matrix_command(int mshm, int pshm, int length_m, int length_p, double u_m, double u_p, int dt, int line_m, int line_p)
{
	QString _message = QString("%1 принял МК МШ%2 ПШ%3")
										 .arg(QTime::currentTime().toString("hh:mm:ss.zzz"))
										 .arg(mshm).arg(pshm);

	if (pshm != 1 || mshm < 5 || mshm > 7)
		return;

	if (mshm == 5) {
		slot_thr.get_omnibus_obj()->switch_ab(1, 9, true);
		mainSetButton->setFlat(false);
	}

	if (mshm == 6) {
		slot_thr.get_omnibus_obj()->switch_ab(1, 9, true);
		reserveSetButton->setFlat(false);
	}

	if (mshm == 7) {
		slot_thr.get_omnibus_obj()->switch_ab(1, 9, false);
		mainSetButton->setFlat(true);
		reserveSetButton->setFlat(false);
	}

	logArea->append(_message);
}
void BOOP::new_data(int mko, int address, int subaddress, QVariantList words)
{

	// Do something

}
