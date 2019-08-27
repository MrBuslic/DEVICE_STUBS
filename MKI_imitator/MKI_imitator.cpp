#include "MKI_imitator.h"
#include <qapplication.h>
MKI_imitator::MKI_imitator()  
{
	widg = new QWidget(this);
	setCentralWidget(widg);
	setWindowTitle("Имитатор МКИ");

	control_label = new QLabel("Управление: ", widg);
	rezhim_label = new QLabel("Режим работы: ", widg);
	ip_label = new QLabel("IP клиента: ", widg);
	RPIK_label = new QLabel("РПИК: ", widg); // Режим Приёма Кадра

	log_edit = new QTextEdit(widg);
	log_edit->setReadOnly(true);
	_scroll_bar = new QScrollBar();
	_scroll_bar = log_edit->verticalScrollBar();

	gridLayout = new QGridLayout(widg);

	gridLayout->addWidget(control_label, 0, 0);
	gridLayout->addWidget(rezhim_label, 1, 0);
	gridLayout->addWidget(ip_label, 2, 0);
	gridLayout->addWidget(RPIK_label, 3, 0);
	gridLayout->addWidget(log_edit, 4, 0, 1, 2);


	
	QSettings tmp_settings("Cometa", "СПО МКПА МЦА");
	QString ip = tmp_settings.value("IP_rm", "192.168.0.100").toString();
	if (ip == "localhost")	// QUdpSocket::bind не жрёт "localhost". Ему "127.0.0.1" подавай.
		ip = "127.0.0.1";
	bool t = _sock.bind(QHostAddress(ip), 10001);

	serv_sock.listen(QHostAddress("127.0.0.1"), 30302);


	connect(&_sock, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error_Slot(QAbstractSocket::SocketError)));
	connect(&_sock, &QUdpSocket::readyRead, this, &MKI_imitator::read);
	connect(&serv_sock, &QTcpServer::newConnection, this, &MKI_imitator::connect_ag);
	
	frame_slot_thr.set_connection_params("127.0.0.1", FRAME_SLOT);
	frame_slot_thr.start();

	frame_signal_thr.set_connection_params("127.0.0.1", FRAME_SIGNAL);
	frame_signal_thr.start();

	if (!frame_slot_thr.wait_connected(3) || !frame_signal_thr.wait_connected(3))
		QMessageBox::critical(0, "Нет соединения", "Ошибка соединения с frame_bus в ");	// Mkprm?

	connect(frame_signal_thr.get_obj().get(), SIGNAL(new_frame_07(QString, int, int, int, QString, QVariant)), this, SLOT(new_frame_07(QString, int, int, int, QString, QVariant)));

	QSettings settings(QApplication::applicationDirPath() + "/positions.ini", QSettings::IniFormat);
	restoreGeometry(settings.value("rmmbk07_geometry").toByteArray());
	//connect(_sock, SIGNAL(readyRead()), SLOT(read()));
}

void MKI_imitator::closeEvent(QCloseEvent *event)
{
	QSettings settings(QApplication::applicationDirPath() + "/positions.ini", QSettings::IniFormat);
	settings.setValue("rmmbk07_geometry", saveGeometry());
	QWidget::closeEvent(event);
}

MKI_imitator::~MKI_imitator()
{
	delete control_label;
	delete rezhim_label;
	delete ip_label;
	delete RPIK_label;	
	delete _scroll_bar;
	delete log_edit;
	delete gridLayout;
	delete widg;
}

void MKI_imitator::error_Slot(QAbstractSocket::SocketError socketError)
{
	QString tmp = _sock.errorString();
}


void MKI_imitator::read()
{

}

void MKI_imitator::connect_ag()
{
	if (_ag_sock)
		if((_ag_sock->state() == QAbstractSocket::ConnectedState))
		{
			_ag_sock->close();
			delete _ag_sock;
			_ag_sock = nullptr;
		}
	_ag_sock = serv_sock.nextPendingConnection();
	connect(_ag_sock, &QTcpSocket::readyRead, this, &MKI_imitator::read_ag);
	//char c = '\x01';
	//_ag_sock->write(&c);
}

void MKI_imitator::read_ag()
{


}

void MKI_imitator::new_frame_07(QString mode_in, int psp_in, int lit_in, int _fm, QString _ant, QVariant frame_data)
{

	frame_slot_thr.get_frame_bus_obj()->make_new_frame_rm07(mode_in, frame_data);

	return;
}