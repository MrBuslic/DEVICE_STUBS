#include "BOOP.h"
#include <QMessageBox>
#include "rpc_ports.h"

union MKOWord
{
	MKOWord(quint16 raw_word)
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
  mainSetButton->setCheckable(true);
  mainSetButton->setFixedWidth(180);

  reserveSetButton = new QPushButton("Резервный полукомплект", this);
  reserveSetButton->setCheckable(true);
  reserveSetButton->setFixedWidth(180);

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
  bit18 = new QCheckBox("Исправность микроконвертора-драйвера ШД", this);
  bit17 = new QCheckBox("Исправность интерфейса SPI с ОУ МКО", this);
  bit16 = new QCheckBox("Исправность интерфейса с ДУ (канал υ)", this);
  bit15 = new QCheckBox("Исправность интерфейса с ДУ (канал φ)", this);

  QVBoxLayout *firstDiagnosticColumn = new QVBoxLayout();
  firstDiagnosticColumn->addWidget(bit19);
  firstDiagnosticColumn->addWidget(bit18);
  firstDiagnosticColumn->addWidget(bit17);
  firstDiagnosticColumn->addWidget(bit16);
  firstDiagnosticColumn->addWidget(bit15);

  //  // Second self-diagnostic column

  bit14 = new QCheckBox("Исправность ОУ МКО (основной канал)", this);
  bit13 = new QCheckBox("Исправность ОУ МКО (резервный канал)", this);
  bit12 = new QCheckBox("Исправность термодатчика", this);
  bit4 = new QCheckBox("Интегральный признак исправности БУП(Е)", this);

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
  upsilonAngleValue = new QLabel("FFFh", this);

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
  phiAngleValue = new QLabel("FFFh", this);

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

  QPushButton *increasePhiAngle = new QPushButton("+", this);
  increasePhiAngle->setFixedWidth(40);

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

  this->setWindowTitle("Блок управления приводами");
  this->setLayout(mainLayout);
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

  slot_thr.get_omnibus_obj()->switch_ab(1, 9, true);

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
	MKOWord parsed_word(command_word);

	if ((MKO == this->MKO) && (parsed_word.address == this->address))
	{

		QString _message = QString("%1 принял сигнал на подадресе %2 c КС %3").arg(QTime::currentTime().toString("hh:mm:ss.zzz")).arg(parsed_word.subaddress).arg(parsed_word.command_word);
		logArea->append(_message);

		// Do something
	}
}
void BOOP::new_data(int mko, int address, int subaddress, QVariantList words)
{

	// Do something

}
