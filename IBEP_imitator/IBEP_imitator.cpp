#include "IBEP_imitator.h"
//#include "kprd_socket_rpc.h"

IBEP_imitator::IBEP_imitator()  
{
	widg = new QWidget(this);
	setCentralWidget(widg);
	setWindowTitle("Имитатор ИБЕП");

	NK_btn = new QPushButton("НК", widg);
	NK_btn->setFixedWidth(150);
	NK_btn->setFixedHeight(100);
	KOM_btn = new QPushButton("КОМУТ", widg);
	KOM_btn->setFixedWidth(150);
	KOM_btn->setFixedHeight(100);

	voltage_label = new QLabel("Напряжение: ", widg);
	amperage_label = new QLabel("Ток: ", widg);


	gridLayout = new QGridLayout(widg);

	gridLayout->addWidget(NK_btn, 1, 0);
	gridLayout->addWidget(KOM_btn, 1, 1);

	gridLayout->addWidget(voltage_label, 2, 0);
	gridLayout->addWidget(amperage_label, 3, 0);
	setNK(true);
	setKOM(false);
	
	connect(this, &IBEP_imitator::test, this, &IBEP_imitator::receiveData);

	bool with_protection = false;
	unsigned char num = 1;	// num 1 - не коммутир, 2 - коммутир, 3 - обе
	QByteArray ba;
	ba.push_back(0x31);
	ba.push_back(0x17);
	ba.push_back(0x02);
	ba.push_back(with_protection ? num << 4 : num);
	emit test(ba);

}

IBEP_imitator::~IBEP_imitator()
{
	delete NK_btn;
	delete KOM_btn;

	delete voltage_label;
	delete amperage_label;

	delete gridLayout;
	delete widg;

}

void IBEP_imitator::setNK(bool on)
{
	on ? NK_btn->setStyleSheet("background-color: rgb(142, 198, 156);") : NK_btn->setStyleSheet("background-color: rgb(204, 204, 204);");
}

void IBEP_imitator::setKOM(bool on)
{
	on ? KOM_btn->setStyleSheet("background-color: rgb(142, 198, 156);") : KOM_btn->setStyleSheet("background-color: rgb(204, 204, 204);");
}

void IBEP_imitator::receiveData(QByteArray ba)
{
	if (ba[0] == char(0x31) &&
		ba[1] == char(0x17) &&
		ba[2] == char(0x02))
	{
		switch (ba[3])
		{
		case 1: setNK(true); setKOM(false); break;
		case 2: setNK(false); setKOM(true); break;
		case 3: setNK(true); setKOM(true); break;
		default:
			break;
		}
	}
	return;
}

