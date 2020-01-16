#include "IBEP_imitator.h"
#include <qmessagebox.h>
#include "rpc_ports.h"

// Таблица для старшего байта контрольной суммы Modbus RTU
char crcTable_H[] = {
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00,
	0xC1, 0x81,	0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1,
	0x81, 0x40, 0x01, 0xC0,	0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
	0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,	0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
	0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,	0x00, 0xC1, 0x81, 0x40, 0x01,
	0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81,	0x40, 0x01, 0xC0,
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,	0x80,
	0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00,
	0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80,
	0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
	0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
	0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
	0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
	0x01, 0xC0,	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
	0xC0, 0x80, 0x41, 0x01,	0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1,
	0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80,
	0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,	0x40
};

// Таблица для младшего байта контрольной суммы Modbus RTU
char crcTable_L[] = {
	0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7, 0x05,
	0xC5, 0xC4,	0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA,
	0xCB, 0x0B, 0xC9, 0x09,	0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA,
	0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4, 0xD5, 0x15,
	0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,	0x11, 0xD1, 0xD0, 0x10, 0xF0,
	0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32, 0x36, 0xF6, 0xF7,	0x37, 0xF5, 0x35,
	0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,	0x3B,
	0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA,
	0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27,
	0xE7, 0xE6, 0x26, 0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60,
	0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64,
	0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F, 0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB,
	0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE,
	0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5, 0x77, 0xB7,
	0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0, 0x50, 0x90, 0x91,
	0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54,
	0x9C, 0x5C,	0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 0x99,
	0x59, 0x58, 0x98, 0x88,	0x48, 0x49, 0x89, 0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E,
	0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,	0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46,
	0x86, 0x82, 0x42, 0x43, 0x83, 0x41, 0x81, 0x80,	0x40
};


quint8 getCRC(QByteArray::iterator begin, QByteArray::iterator end)
{
	quint16 res = 0;
	for (auto itr(begin); itr != end; ++itr)
	{
		res += quint8(*itr);
		if (res & 0xFF00)
			res = (res & 0x00FF) + (res >> 8) & 0x00FF;
	}
	return quint8(res);
}

quint16 getCRC(const QByteArray& ba)
{
	unsigned char crc_L = 0xFF, crc_H = 0xFF;
	quint16 index, crc_res;

	for (auto itr(ba.begin()), end(ba.end()); itr != end; ++itr)
	{
		index = crc_L ^ static_cast<unsigned char>(*itr);
		crc_L = crc_H ^ crcTable_H[index];
		crc_H = crcTable_L[index];
	}

	return crc_res = (crc_H << 8) | crc_L;
}


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
	u_in = 27.0;

	setKOM(false);
	setNK(false);
	connect(&contr_thread, &IBEP_CONTR::setNK, this, &IBEP_imitator::setNK);
	connect(&contr_thread, &IBEP_CONTR::setKOM, this, &IBEP_imitator::setKOM);
	connect(&contr_thread, &IBEP_CONTR::setU, this, &IBEP_imitator::setU);

	connect(&ui_thread, &IBEP_UI::getI, this, &IBEP_imitator::getI);
	connect(&ui_thread, &IBEP_UI::getU, this, &IBEP_imitator::getU);

	contr_thread.start();
	ui_thread.start();
	//bool with_protection = false;
	//unsigned char num = 1;	// num 1 - не коммутир, 2 - коммутир, 3 - обе
	//QByteArray ba;
	//ba.push_back(0x31);
	//ba.push_back(0x17);
	//ba.push_back(0x02);
	//ba.push_back(with_protection ? num << 4 : num);
	//emit test(ba);

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
	nk_state = on;
	power_slot_thr.get_power_bus_obj()->set_u(1, on ? u_in : 0);
}

void IBEP_imitator::setKOM(bool on)
{
	on ? KOM_btn->setStyleSheet("background-color: rgb(142, 198, 156);") : KOM_btn->setStyleSheet("background-color: rgb(204, 204, 204);");
	kom_state = on;
	power_slot_thr.get_power_bus_obj()->set_u(2, on ? u_in : 0);
	power_slot_thr.get_power_bus_obj()->set_u(3, on ? u_in : 0);
}

void IBEP_imitator::setU(double u)
{
	u_in = u;
}

void IBEP_imitator::receiveData(QByteArray ba)
{

	return;
}


void IBEP_CONTR::run()
{
	_srv = new QTcpServer;
	_srv->listen(QHostAddress("127.0.0.1"), IBEP_CONTR_PORT);
	connect(_srv, &QTcpServer::newConnection, this, &IBEP_CONTR::newConnection, Qt::DirectConnection);
	exec();
}

void IBEP_CONTR::newConnection()
{
	SRPCSignalClass::Instance().toLog("contr new connection");
	IBEP_CONTR_conn* tmp_con = new IBEP_CONTR_conn(_srv->nextPendingConnection());
	connect(tmp_con, &IBEP_CONTR_conn::setNK, this, &IBEP_CONTR::setNK);
	connect(tmp_con, &IBEP_CONTR_conn::setKOM, this, &IBEP_CONTR::setKOM);
	connect(tmp_con, &IBEP_CONTR_conn::setU, this, &IBEP_CONTR::setU);
}

void IBEP_UI::run()
{
	_srv = new QTcpServer;
	_srv->listen(QHostAddress("127.0.0.1"), IBEP_UI_PORT);
	connect(_srv, &QTcpServer::newConnection, this, &IBEP_UI::newConnection, Qt::DirectConnection);
	exec();
}

void IBEP_UI::newConnection()
{
	SRPCSignalClass::Instance().toLog("ui new connection");
	IBEP_UI_conn* tmp_con = new IBEP_UI_conn(_srv->nextPendingConnection());
	connect(tmp_con, &IBEP_UI_conn::getI, this, &IBEP_UI::getI, Qt::BlockingQueuedConnection);
	connect(tmp_con, &IBEP_UI_conn::getU, this, &IBEP_UI::getU, Qt::BlockingQueuedConnection);
}

IBEP_CONTR_conn::IBEP_CONTR_conn(QTcpSocket* _sock) : sock(_sock)
{
	connect(sock, &QIODevice::readyRead, this, &IBEP_CONTR_conn::newData);
	nk_state = false;
	kom_state = false;
}

IBEP_UI_conn::IBEP_UI_conn(QTcpSocket* _sock) : sock(_sock)
{
	connect(sock, &QIODevice::readyRead, this, &IBEP_UI_conn::newData);
}

void IBEP_CONTR_conn::newData()
{
	QByteArray ba;
	while (sock->bytesAvailable())
	{
		ba += sock->readAll();
		QThread::currentThread()->msleep(20);
	}


	SRPCSignalClass::Instance().toLog("contr comm: " + ba.toHex());
	QByteArray answer;


	//contactor ON
	if (ba[0] == char(0x31) &&
		ba[1] == char(0x17) &&
		ba[2] == char(0x02))
	{
		SRPCSignalClass::Instance().toLog("contactor on");
		switch (ba[3])
		{
		case 1: case (1 << 4): setNK(true); nk_state = true; break;
		case 2: case (2 << 4): setKOM(true); kom_state = true; break;
		case 3: case (3 << 4): setNK(true); setKOM(true); nk_state = true; kom_state = true; break;
		default:
			break;
		}

		answer.push_back(0x31);
		answer.push_back(char(0x97));
		answer.push_back(0x01);
		answer.push_back(char(0x00));
	}

	//contactor OFF
	if (ba[0] == char(0x31) &&
		ba[1] == char(0x18) &&
		ba[2] == char(0x02))
	{
		SRPCSignalClass::Instance().toLog("contactor off");
		switch (ba[3])
		{
		case 1: case (1 << 4): emit setNK(false); nk_state = false; break;
		case 2: case (2 << 4): emit setKOM(false); kom_state = false; break;
		case 3: case (3 << 4): emit setNK(false); emit setKOM(false); nk_state = false; kom_state = false; break;
		default:
			break;
		}

		answer.push_back(0x31);
		answer.push_back(char(0x98));
		answer.push_back(0x01);
		answer.push_back(char(0x00));
	}

	//TM
	if (ba[0] == char(0x31) &&
		ba[1] == char(0x22) &&
		ba[2] == char(0x00))
	{
		SRPCSignalClass::Instance().toLog("get tm");
		answer.push_back(0x31);
		answer.push_back(char(0xA2));
		answer.push_back(0x03);

		answer.push_back(char(0x00));
		answer.push_back(char(0x00));

		answer.push_back(getCRC(answer.begin() + 3, answer.end()));

	}
	

	//set U
	if (ba[0] == char(0x31) &&
		ba[1] == char(0x34) &&
		ba[2] == char(0x04))
	{
		quint16 u_word = (quint16(ba[5]) << 8) | quint8(ba[4]);

		emit setU(double(u_word) / 100);

		answer.push_back(0x31);
		answer.push_back(char(0xB4));
		answer.push_back(0x01);
		answer.push_back(char(0x00));
	}

	//set U save
	if (ba[0] == char(0x31) &&
		ba[1] == char(0x35) &&
		ba[2] == char(0x04))
	{
		quint16 u_word = (quint16(ba[5]) << 8) | quint8(ba[4]);

		emit setU(double(u_word) / 100);

		answer.push_back(0x31);
		answer.push_back(char(0xB5));
		answer.push_back(0x01);
		answer.push_back(char(0x00));
	}

	//contactors states
	if (ba[0] == char(0x31) &&
		ba[1] == char(0x12) &&
		ba[2] == char(0x00))
	{
		SRPCSignalClass::Instance().toLog("get state");
		answer.push_back(0x31);
		answer.push_back(char(0x92));
		answer.push_back(0x02);

		quint8 state_byte = 0;

		if (nk_state)
			state_byte += 1;
		if (kom_state)
			state_byte += 2;

		answer.push_back(state_byte);

		answer.push_back(getCRC(answer.begin() + 3, answer.end()));

	}

	//verify
	if (ba[0] == char(0x31) &&
		ba[1] == char(0x2A) &&
		ba[2] == char(0x00))
	{
		SRPCSignalClass::Instance().toLog("verify");
		answer.push_back(0x31);
		answer.push_back(char(0xAA));
		answer.push_back(0x01);

		answer.push_back(char(0x00));
	}
	SRPCSignalClass::Instance().toLog("contr answer: " + answer.toHex());
	sock->write(answer);
	sock->waitForBytesWritten();

}


void IBEP_UI_conn::newData()
{

	QByteArray ba;
	while (sock->bytesAvailable())
	{
		ba += sock->readAll();
		QThread::currentThread()->msleep(20);
	}
	SRPCSignalClass::Instance().toLog("ui comm: " + ba.toHex());
	QByteArray answer;
	answer.push_back(ba[0]);
	answer.push_back(ba[1]);
	answer.push_back(char(0x15));
	quint16 devide = 1000;
	quint16 answer_val;
	double val;
	if (ba[0] == char(0x01))
	{
		emit getU(&val);
		answer_val = quint16(val * 1000);
	}
	else
	{
		emit getI(&val);
		answer_val = quint16(val * 1000);
	}
	answer.push_back((answer_val & 0xFF00) >> 8);
	answer.push_back(answer_val & 0x00FF);

	for (int i = 5; i < 17; i++)
	{
		answer.push_back(char(0x00));
	}

	answer.push_back((devide & 0xFF00) >> 8);
	answer.push_back(devide & 0x00FF);
	
	quint16 _crc = getCRC(answer);
	answer.push_back(_crc & 0x00FF);
	answer.push_back((_crc & 0xFF00) >> 8);


	SRPCSignalClass::Instance().toLog("ui answer: " + answer.toHex());
	sock->write(answer);
	sock->waitForBytesWritten();
	

}

void IBEP_imitator::getI(double* _curr)
{
	double i_nk;
	double i_k1;
	double i_k2;
	power_slot_thr.get_power_bus_obj()->get_i(1, i_nk);
	power_slot_thr.get_power_bus_obj()->get_i(2, i_k1);
	power_slot_thr.get_power_bus_obj()->get_i(3, i_k2);

	*_curr = i_nk + i_k1 + i_k2;
}

void IBEP_imitator::getU(double* _volt)
{
	*_volt = u_in;
}
