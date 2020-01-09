#include "RTM3002_imitator.h"
#include "RTM3002_socket_rpc.h"
#include "rpc_ports.h"

RTM3002_imitator::RTM3002_imitator()  
{
	widg = new QWidget(this);
	setCentralWidget(widg);

	gridlay = new QGridLayout(widg);
	DELay_label.setText("Задержка между включениями СИБП");
	DELay_edit = new QTextEdit("0.1", widg);
	PositivePeak_label.setText("Пульсация напряжения помехи");
	PositivePeak_edit = new QTextEdit("0", widg);
	WCO_label.setText("Количество слов на МКО");
	WCO_edit = new QTextEdit("1", widg);
	MKO_DATA_label.setText("Слова на МКО");
	MKO_DATA_edit = new QTextEdit("0xFF", widg);
	PEAK_label.setText("Напряжение на канале D1");
	PEAK_edit = new QTextEdit("1", widg);
	CURS1_VAL_label.setText("Время курсора 1");
	CURS1_VAL_edit = new QTextEdit("0.0000015", widg);
	CURS2_VAL_label.setText("Время курсора 1");
	CURS2_VAL_edit = new QTextEdit("0.0000015", widg);
	 
	gridlay->addWidget(&DELay_label, 0, 0/*, 1, 2, Qt::AlignHCenter*/);
	gridlay->addWidget(DELay_edit, 1, 0);

	gridlay->addWidget(&PositivePeak_label, 2, 0);
	gridlay->addWidget(PositivePeak_edit, 3, 0);

	gridlay->addWidget(&WCO_label, 4, 0);
	gridlay->addWidget(WCO_edit, 5, 0);

	gridlay->addWidget(&MKO_DATA_label, 6, 0);
	gridlay->addWidget(MKO_DATA_edit, 7, 0);

	gridlay->addWidget(&PEAK_label, 0, 1);
	gridlay->addWidget(PEAK_edit, 1, 1);

	gridlay->addWidget(&CURS1_VAL_label, 2, 1);
	gridlay->addWidget(CURS1_VAL_edit, 3, 1);

	gridlay->addWidget(&CURS2_VAL_label, 4, 1);
	gridlay->addWidget(CURS2_VAL_edit, 5, 1);

	//gridlay->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding), 6, 2);

	
	connect(DELay_edit, &QTextEdit::textChanged, this, &RTM3002_imitator::SetDELay);
	connect(PositivePeak_edit, &QTextEdit::textChanged, this, &RTM3002_imitator::SetPositivePeak);
	connect(WCO_edit, &QTextEdit::textChanged, this, &RTM3002_imitator::SetWCO);
	connect(MKO_DATA_edit, &QTextEdit::textChanged, this, &RTM3002_imitator::SetMKO_DATA);
	connect(PEAK_edit, &QTextEdit::textChanged, this, &RTM3002_imitator::SetPEAK);
	connect(CURS1_VAL_edit, &QTextEdit::textChanged, this, &RTM3002_imitator::SetCURS1_VAL);
	connect(CURS2_VAL_edit, &QTextEdit::textChanged, this, &RTM3002_imitator::SetCURS2_VAL);
	SetDELay();
	SetPositivePeak();
	SetWCO();
	SetMKO_DATA();
	SetPEAK();
	SetCURS1_VAL();
	SetCURS2_VAL();

	QString ip_str = "127.0.0.1";
	int slot_port = RTM3002_SLOT;
	int signal_port = RTM3002_SIGNAL;
	RTM3002_Socket_RPC_SLOT_Server_Thread* rpc_slot_srv = new RTM3002_Socket_RPC_SLOT_Server_Thread;
	rpc_slot_srv->set_app(this);
	rpc_slot_srv->set_params(ip_str, slot_port);
	rpc_slot_srv->start();
	RTM3002_Socket_RPC_SIGNAL_Thread* rpc_signal_srv = new RTM3002_Socket_RPC_SIGNAL_Thread;
	rpc_signal_srv->set_app(this);
	rpc_signal_srv->set_params(ip_str, signal_port);
	rpc_signal_srv->start();


	QSettings tmp_settings("Cometa", "СПО АИК");
	QString ip = tmp_settings.value("RTM3002_addr", "192.168.0.100").toString();
	if (ip == "localhost")	// QUdpSocket::bind не жрёт "localhost". Ему "127.0.0.1" подавай.
		ip = "127.0.0.1";
//	bool t = _sock.bind(QHostAddress(ip), 10001);
	bool t = _sock.bind(QHostAddress(ip), 5025);
//	serv_sock.listen(QHostAddress("127.0.0.1"), RTM3002_SIGNAL);
	serv_sock.listen(QHostAddress("127.0.0.1"), 5025);

	connect(&_sock, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error_Slot(QAbstractSocket::SocketError)));
	connect(&_sock, &QUdpSocket::readyRead, this, &RTM3002_imitator::read);
	connect(&serv_sock, &QTcpServer::newConnection, this, &RTM3002_imitator::connect_ag);


	QSettings settings(QApplication::applicationDirPath() + "/positions.ini", QSettings::IniFormat);
	restoreGeometry(settings.value("RTM3002_geometry").toByteArray());
}

void RTM3002_imitator::error_Slot(QAbstractSocket::SocketError socketError)
{
	QString tmp = _sock.errorString();
}

void RTM3002_imitator::read()
{
	auto received_bytes = _sock.bytesAvailable();
}

void RTM3002_imitator::connect_ag()
{
	if (_ag_sock)
		if ((_ag_sock->state() == QAbstractSocket::ConnectedState))
		{
			_ag_sock->close();
			delete _ag_sock;
			_ag_sock = nullptr;
		}
	_ag_sock = serv_sock.nextPendingConnection();
	connect(_ag_sock, &QTcpSocket::readyRead, this, &RTM3002_imitator::read_ag);
}

void RTM3002_imitator::read_ag()
{
	QString read_data;
	QString ret_str;

	while (_ag_sock->bytesAvailable())
	{
		read_data = _ag_sock->read(_ag_sock->bytesAvailable());
		//		Sleep(10);
	}
	if (read_data.contains("?"))
	{
		/*		QString s = "0,\"No error\"\n";
		_ag_sock->write(s.toStdString().c_str());*/
		QString response = "";
		if(read_data == "SYSTem:ERRor:ALL?\r\n")
			response = "0,\"No error\"\n";
		if (read_data == "*TST?\r\n")
			response = "1";
		if (read_data == "MEASurement1:RESult:ACTual ? DELay\r\n")
			response = QString::number(GetDELay());//"0.1";
		if (read_data == "MEASurement1:RES:PPE? PositivePeak\r\n")
			response = QString::number(GetPositivePeak());// "0";
		if (read_data == "BUS1:MILS:WCO?\r\n")
			response = QString::number(GetWCO());//"1";
		if (read_data == "BUS1:MILS:WORD1:TYPE?\r\n")
			response = "DATA\n";
		if (read_data == "BUS1:MILS:WORD1:DATA?\r\n")
			response = QString::number(GetMKO_DATA());//"15";
		if (read_data == "MEAS1:RES:ACTual? PEAK\r\n")
			response = QString::number(GetPEAK());// "1";
		if (read_data == "CURS1:XDEL:VAL?\r\n")
			response = QString::number(GetCURS1_VAL());// "0.000015";
		if (read_data == "CURS2:XDEL:VAL?\r\n")
			response = QString::number(GetCURS2_VAL());// "0.000015";

		_ag_sock->write(response.toStdString().c_str());
	}
}

void RTM3002_imitator::closeEvent(QCloseEvent *event)
{
	QSettings settings(QApplication::applicationDirPath() + "/positions.ini", QSettings::IniFormat);
	settings.setValue("RTM3002_geometry", saveGeometry());
	QWidget::closeEvent(event);
}

RTM3002_imitator::~RTM3002_imitator()
{
	delete gridlay;
	delete DELay_edit;
	delete PositivePeak_edit;
	delete WCO_edit;
	delete MKO_DATA_edit;
	delete PEAK_edit;
	delete CURS1_VAL_edit;
	delete CURS2_VAL_edit;
	delete widg;
}

double RTM3002_imitator::GetDELay() const
{
	return DELay;
}

void RTM3002_imitator::SetDELay()
{
	DELay = DELay_edit->toPlainText().toDouble();
}

double RTM3002_imitator::GetPositivePeak() const
{
	return PositivePeak;
}

void RTM3002_imitator::SetPositivePeak()
{
	PositivePeak = PositivePeak_edit->toPlainText().toDouble();
}

int RTM3002_imitator::GetWCO() const
{
	return WCO;
}

void RTM3002_imitator::SetWCO()
{
	WCO = WCO_edit->toPlainText().toInt();
}

int RTM3002_imitator::GetMKO_DATA() const
{
	return MKO_DATA;
}

void RTM3002_imitator::SetMKO_DATA()
{
	MKO_DATA = MKO_DATA_edit->toPlainText().toInt(nullptr,0);
}

double RTM3002_imitator::GetPEAK() const
{
	return PEAK;
}

void RTM3002_imitator::SetPEAK()
{
	PEAK = PEAK_edit->toPlainText().toDouble();
}

double RTM3002_imitator::GetCURS1_VAL() const
{
	return CURS1_VAL;
}

void RTM3002_imitator::SetCURS1_VAL()
{
	CURS1_VAL = CURS1_VAL_edit->toPlainText().toDouble();
}

double RTM3002_imitator::GetCURS2_VAL() const
{
	return CURS2_VAL;
}

void RTM3002_imitator::SetCURS2_VAL()
{
	CURS2_VAL = CURS2_VAL_edit->toPlainText().toDouble();
}