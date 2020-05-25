#include "FSVA7_imitator.h"
#include "fsva7_socket_rpc.h"
#include "rpc_ports.h"

FSVA7_imitator::FSVA7_imitator()
{
	widg = new QWidget(this);
	setCentralWidget(widg);

	gridlay = new QGridLayout(widg);


	//gridlay->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding), 6, 2);

	
/*	connect(DELay_edit, &QTextEdit::textChanged, this, &FSVA7_imitator::SetDELay);
	SetDELay();
	*/

	QString ip_str = "127.0.0.1";
	int slot_port = FSVA7_SLOT;
	int signal_port = FSVA7_SIGNAL;
	fsva7_Socket_RPC_SLOT_Server_Thread* rpc_slot_srv = new fsva7_Socket_RPC_SLOT_Server_Thread;
	rpc_slot_srv->set_app(this);
	rpc_slot_srv->set_params(ip_str, slot_port);
	rpc_slot_srv->start();
	fsva7_Socket_RPC_SIGNAL_Thread* rpc_signal_srv = new fsva7_Socket_RPC_SIGNAL_Thread;
	rpc_signal_srv->set_app(this);
	rpc_signal_srv->set_params(ip_str, signal_port);
	rpc_signal_srv->start();


	QSettings tmp_settings("Cometa", "СПО АИК");
	QString ip = tmp_settings.value("FSVA7_addr", "192.168.0.100").toString();
	if (ip == "localhost")	// QUdpSocket::bind не жрёт "localhost". Ему "127.0.0.1" подавай.
		ip = "127.0.0.1";
//	bool t = _sock.bind(QHostAddress(ip), 10001);
	bool t = _sock.bind(QHostAddress(ip), 5025);
//	serv_sock.listen(QHostAddress("127.0.0.1"), FSVA7_SIGNAL);
	serv_sock.listen(QHostAddress("127.0.0.1"), 5025);

	connect(&_sock, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error_Slot(QAbstractSocket::SocketError)));
	connect(&_sock, &QUdpSocket::readyRead, this, &FSVA7_imitator::read);
	connect(&serv_sock, &QTcpServer::newConnection, this, &FSVA7_imitator::connect_ag);


	QSettings settings(QApplication::applicationDirPath() + "/positions.ini", QSettings::IniFormat);
	restoreGeometry(settings.value("FSVA7_geometry").toByteArray());
}

void FSVA7_imitator::error_Slot(QAbstractSocket::SocketError socketError)
{
	QString tmp = _sock.errorString();
}

void FSVA7_imitator::read()
{
	auto received_bytes = _sock.bytesAvailable();
}

void FSVA7_imitator::connect_ag()
{
	if (_ag_sock)
		if ((_ag_sock->state() == QAbstractSocket::ConnectedState))
		{
			_ag_sock->close();
			delete _ag_sock;
			_ag_sock = nullptr;
		}
	_ag_sock = serv_sock.nextPendingConnection();
	connect(_ag_sock, &QTcpSocket::readyRead, this, &FSVA7_imitator::read_ag);
}

void FSVA7_imitator::read_ag()
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
		if(read_data == "SYST:ERR?\r\n")
			response = "0,\"No error\"\n";
		if (read_data == "*TST?\r\n")
			response = "1";

		if (read_data == "MMEM:CAT? 'C:/R_S/Instr/user'\r\n")
			response = "'data.iq.tar','LIM1','LIM2','LIM3','LIM4','LIM5','LIM6','LIM7'\n";
		if (read_data == "MMEM:DEL 'C:/R_S/Instr/user'\r\n")
			response = "";
		if (read_data == "CALC:MARK:COUN:FREQ?\r\n")
			response = "1";
		if (read_data == "CALC:MARK1:COUN:FREQ?\r\n")
			response = "1";
		if (read_data == "CALC:MARK2:COUN:FREQ?\r\n")
			response = "1";
		if (read_data == "CALC:MARK:Y?\r\n")
			response = "1";
		
		if (read_data == "CALC:MARK:FUNC:POW:RES? ACP\r\n")
		{
			if(SPAN == 250000 && BWID == 210000)
				response = "1";
			if (SPAN == 100000000 && BWID == 75000000)
				response = "1";
			if (SPAN == 65000 && BWID == 40000)
				response = "1";
			if (SPAN == 106000000 && BWID == 60000000)
				response = "1";
			if (SPAN == 32000000 && BWID == 31479000)
				response = "1";
			if (SPAN == 100000000 && BWID == 45000000)
				response = "-25";
			if (SPAN == 65000 && BWID == 24000)
				response = "-25";
			if (SPAN == 6120000 && BWID == 3000000)
				response = "-16";
			if (SPAN == 106000000 && BWID == 3000000)
				response = "-19";
			if (SPAN == 10000 && BWID == 6000)
				response = "100";
			if (SPAN == 200000 && BWID == 197000)
				response = "1";
			if (SPAN == 70000 && BWID == 60000)
				response = "-93";
			if (BWID == 8000)
				response = "0";
			if (BWID == 15000000)
				response = "0";
			if (BWID == 1500000)
				response = "0";
		}

		_ag_sock->write(response.toStdString().c_str());
	}

	if (read_data.contains("ADEM:SPEC:SPAN:ZOOM"))
	{
		// ADEM:SPEC:SPAN:ZOOM 250000 HZ\r\n
		QString str = read_data.right(read_data.size() - 20);
		str = str.left(str.size() - 5);
		setSPAN(str.toInt());
	}
	if (read_data.contains("POW:ACH:BWID:ACH"))
	{
		// POW:ACH:BWID:ACH 210000 HZ\r\n
		QString str = read_data.right(read_data.size() - 17);
		str = str.left(str.size() - 5);
		setBWID(str.toInt());
	}

}

void FSVA7_imitator::setSPAN(int span)
{
	SPAN = span;
}

void FSVA7_imitator::setBWID(int bwid)
{
	BWID = bwid;
}

void FSVA7_imitator::closeEvent(QCloseEvent *event)
{
	QSettings settings(QApplication::applicationDirPath() + "/positions.ini", QSettings::IniFormat);
	settings.setValue("FSVA7_geometry", saveGeometry());
	QWidget::closeEvent(event);
}

FSVA7_imitator::~FSVA7_imitator()
{
	delete gridlay;
	delete widg;
}
/*
double FSVA7_imitator::GetDELay() const
{
	return DELay;
}

void FSVA7_imitator::SetDELay()
{
	DELay = DELay_edit->toPlainText().toDouble();
}
*/