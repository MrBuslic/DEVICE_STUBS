#include "ZVH4_imitator.h"
#include "ZVH4_socket_rpc.h"
#include "rpc_ports.h"

ZVH4_imitator::ZVH4_imitator()  
{
	widg = new QWidget(this);
	setCentralWidget(widg);


	QString ip_str = "127.0.0.1";
	int slot_port = ZVH4_SLOT;
	int signal_port = ZVH4_SIGNAL;
	ZVH4_Socket_RPC_SLOT_Server_Thread* rpc_slot_srv = new ZVH4_Socket_RPC_SLOT_Server_Thread;
	rpc_slot_srv->set_app(this);
	rpc_slot_srv->set_params(ip_str, slot_port);
	rpc_slot_srv->start();
	ZVH4_Socket_RPC_SIGNAL_Thread* rpc_signal_srv = new ZVH4_Socket_RPC_SIGNAL_Thread;
	rpc_signal_srv->set_app(this);
	rpc_signal_srv->set_params(ip_str, signal_port);
	rpc_signal_srv->start();


	QSettings tmp_settings("Cometa", "СПО АИК");
	QString ip = tmp_settings.value("ZVH4_addr", "192.168.0.100").toString();
	if (ip == "localhost")	// QUdpSocket::bind не жрёт "localhost". Ему "127.0.0.1" подавай.
		ip = "127.0.0.1";
//	bool t = _sock.bind(QHostAddress(ip), 10001);
	bool t = _sock.bind(QHostAddress(ip), 5025);
//	serv_sock.listen(QHostAddress("127.0.0.1"), ZVH4_SIGNAL);
	serv_sock.listen(QHostAddress("127.0.0.1"), 5025);

	connect(&_sock, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error_Slot(QAbstractSocket::SocketError)));
	connect(&_sock, &QUdpSocket::readyRead, this, &ZVH4_imitator::read);
	connect(&serv_sock, &QTcpServer::newConnection, this, &ZVH4_imitator::connect_ag);


	QSettings settings(QApplication::applicationDirPath() + "/positions.ini", QSettings::IniFormat);
	restoreGeometry(settings.value("ZVH4_geometry").toByteArray());
}

void ZVH4_imitator::error_Slot(QAbstractSocket::SocketError socketError)
{
	QString tmp = _sock.errorString();
}

void ZVH4_imitator::read()
{
	auto received_bytes = _sock.bytesAvailable();
}

void ZVH4_imitator::connect_ag()
{
	if (_ag_sock)
		if ((_ag_sock->state() == QAbstractSocket::ConnectedState))
		{
			_ag_sock->close();
			delete _ag_sock;
			_ag_sock = nullptr;
		}
	_ag_sock = serv_sock.nextPendingConnection();
	connect(_ag_sock, &QTcpSocket::readyRead, this, &ZVH4_imitator::read_ag);
}

void ZVH4_imitator::read_ag()
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
		_ag_sock->write(response.toStdString().c_str());
	}
}

void ZVH4_imitator::closeEvent(QCloseEvent *event)
{
	QSettings settings(QApplication::applicationDirPath() + "/positions.ini", QSettings::IniFormat);
	settings.setValue("ZVH4_geometry", saveGeometry());
	QWidget::closeEvent(event);
}

ZVH4_imitator::~ZVH4_imitator()
{

}

