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
	gridLayout->addWidget(log_edit, 5, 0, 1, 2);

	btn = new QPushButton("server to MKI", widg);
	gridLayout->addWidget(btn, 4, 0);
	connect(btn, &QPushButton::clicked, this, &MKI_imitator::writeByBtn);
	
	QSettings tmp_settings("Cometa", "СПО МКПА МЦА");
	QString ip = tmp_settings.value("IP_rm", "192.168.0.100").toString();
	if (ip == "localhost")	// QUdpSocket::bind не жрёт "localhost". Ему "127.0.0.1" подавай.
		ip = "127.0.0.1";

	tcpServer = new QTcpServer(widg);
	connect(tcpServer, SIGNAL(newConnection()), this, SLOT(newConn()));
	tcpServer->listen(QHostAddress::Any, 33333);

	sock.connectToHost("10.44.4.13", 33333);
	connect(&sock, &QTcpSocket::readyRead, this, &MKI_imitator::readByBtn);

	
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
	RUValue = MU;
	RRValue = RR;
	RPIKValue = OFF;
}

void MKI_imitator::log_msg(QString msg)
{
	log_edit->append(msg);
}

QByteArray MKI_imitator::createCommandHeader(const int & command, const int & dataSize, int id)
{
	QByteArray header;

	header.resize(msgHeaderLen);
	header.fill(0);

	//MsgTypeC
	numberIntoBA(header, command, header_MsgTypeC_ind);

	//PVC
	header[0] = BYTE(0x55); header[1] = BYTE(0xAA);	header[2] = BYTE(0x01); header[3] = BYTE(0x00);
	header[header_addr_receiver_ind] = BYTE(0x02);//Адреса получателя 
	header[header_addr_sender_ind] = BYTE(0x01);//Адреса отправителя

	numberIntoBA(header, ++MsgRegN, header_msg_reg_n_ind);//MsgRegN

														  //Time
	unsigned long rcv_t_sec = numberFromBA<quint32>(header, header_sec_time_ind, 4);
	unsigned long rcv_t_usec = numberFromBA<quint32>(header, header_usec_time_ind, 4);

	my_timer.StopCounter();
	unsigned long send_sec = rcv_t_sec + my_timer.GetSec();
	unsigned long send_usec = rcv_t_usec + my_timer.GetUSec();

	numberIntoBA(header, send_sec, header_sec_time_ind);
	numberIntoBA(header, send_usec, header_usec_time_ind);
	//KRO_TIME_OUT
	const int max_wait_sec = 5;
	numberIntoBA(header, send_usec + max_wait_sec * 1000000, header_krp_timeout_ind);
	numberIntoBA(header, send_sec + max_wait_sec, header_kro_timeout_ind);
	//LinkId
	numberIntoBA(header, id, header_LinkID_ind);//сокет
												//LinkStat
	numberIntoBA(header, 0x10, header_linkStat_ind);
	//DataSize
	numberIntoBA(header, dataSize, header_datasize_ind);

	numberIntoBA(header, RUValue, header_RU);
	numberIntoBA(header, RRValue, header_RR);
	numberIntoBA(header, RPIKValue, header_RPIK);

	//CRC32
	//unsigned int crc32_new = sp_getCRC32((unsigned char*)(header.data()), header.size() - 4);
	//numberIntoBA(header, crc32_new, header_crc_header_ind);

	return header;
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
	log_msg(clientSocket->errorString());
}


void MKI_imitator::read()
{

}

void MKI_imitator::new_frame_07(QString mode_in, int psp_in, int lit_in, int _fm, QString _ant, QVariant frame_data)
{

	frame_slot_thr.get_frame_bus_obj()->make_new_frame_rm07(mode_in, frame_data);

	return;
}

void MKI_imitator::newConn()
{
	clientSocket = tcpServer->nextPendingConnection();
	QHostAddress adrv4(clientSocket->peerAddress().toIPv4Address());
	log_msg(QString("Новое подключение с %1:%2").arg(adrv4.toString()).arg(QString::number(clientSocket->peerPort())));
	//connect(clientSocket, SIGNAL(readyRead()), this, SLOT(slotReadClient()));
	connect(clientSocket, &QTcpSocket::readyRead, this, &MKI_imitator::slotReadClient);
}

void MKI_imitator::slotReadClient()
{
	QByteArray read_data;
	QString ret_str;

	while (clientSocket->bytesAvailable())
		read_data += clientSocket->read(clientSocket->bytesAvailable());
	
	qDebug() << read_data.toHex();
	qDebug() << read_data.size();
	log_msg("0x" + QString(read_data.toHex()));

	
	unsigned int MsgTypeC = numberFromBA<unsigned int>(read_data, 12);	// Код типа сообщения
	MsgTypeC += 1e9;
	
	QByteArray response_data = createCommandHeader(MsgTypeC, 0);

	//response_data.append(read_data.mid(0, 4));	// Код начала заголовка и версии протокола.
	//response_data.append(read_data.mid(8, 4));	// FromAddr
	//response_data.append(read_data.mid(4, 4));	// ToAddr

	//numberIntoBA(response_data, MsgTypeC, 12);

	//response_data.append(read_data.mid(16, 14));

	//unsigned int ErrStat = 0;
	//numberIntoBA(response_data, ErrStat, 40);

	//response_data.append(read_data.mid(44));
	//response_data.resize(96);


	qDebug() << response_data.toHex();
	qDebug() << response_data.size();
	clientSocket->write(response_data);
	//clientSocket->write("ewq");
}

void MKI_imitator::writeByBtn()
{
	sock.write("qwe");
}

void MKI_imitator::readByBtn()
{
	QByteArray read_data;
	while (sock.bytesAvailable())
		read_data += sock.read(sock.bytesAvailable());

	qDebug() << read_data;
	log_msg(read_data);
}
