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

	QSettings tmp_settings("Cometa", "СПО МКПА МЦА");
	QString ip = tmp_settings.value("IP_rm", "192.168.0.100").toString();
	if (ip == "localhost")	// QUdpSocket::bind не жрёт "localhost". Ему "127.0.0.1" подавай.
		ip = "127.0.0.1";

	tcpServer = new QTcpServer(widg);
	connect(tcpServer, SIGNAL(newConnection()), this, SLOT(newConn()));
	tcpServer->listen(QHostAddress::Any, 50224);
	
	frame_slot_thr.set_connection_params("127.0.0.1", FRAME_SLOT);
	frame_slot_thr.start();

	frame_signal_thr.set_connection_params("127.0.0.1", FRAME_SIGNAL);
	frame_signal_thr.start();

	if (!frame_slot_thr.wait_connected(3) || !frame_signal_thr.wait_connected(3))
		QMessageBox::critical(0, "Нет соединения", "Ошибка соединения с frame_bus в ");	// Mkprm?

	connect(frame_signal_thr.get_obj().get(), SIGNAL(new_frame_04(QString, QVariant)), this, SLOT(new_frame_04(QString, QVariant)));

	QSettings settings(QApplication::applicationDirPath() + "/positions.ini", QSettings::IniFormat);
	restoreGeometry(settings.value("rmmbk07_geometry").toByteArray());
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
	header[0] = BYTE(0x55); header[1] = BYTE(0xAA);	header[2] = BYTE(0x01); header[3] = BYTE(0x532);
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
	unsigned int crc32_new = sp_getCRC32((unsigned char*)(header.data()), header.size() - 4);
	numberIntoBA(header, crc32_new, header_crc_header_ind);

	return header;
}

void MKI_imitator::sendKRO(int MsgTypeC)
{
	sock.disconnectFromHost();
	sock.waitForDisconnected(3000);
	//sock.connectToHost("10.44.4.13", 50164);
	sock.connectToHost("127.0.0.1", 50164);	
	if (!sock.waitForConnected(3000))
		log_msg("Ошибка сокета: " + sock.errorString());

	QByteArray query_data = createCommandHeader(MsgTypeC + 2e9, 0);
	sock.write(query_data);
}

unsigned long MKI_imitator::sp_getCRC32(unsigned char * buf, unsigned long len)
{
	static int	crc_tab_sw = 0;
	static unsigned long crc_table[256];
	unsigned long crc;
	int	i, j;

	if (crc_tab_sw == 0)
	{
		for (i = 0; i < 256; i++)
		{
			crc = i;
			for (j = 0; j < 8; j++)	crc = crc & 1 ? (crc >> 1) ^ 0xEDB88320UL : crc >> 1;
			crc_table[i] = crc;
		}
		crc_tab_sw = 1;
	}
	crc = 0xFFFFFFFFUL;
	while (len--) crc = crc_table[(crc ^ *buf++) & 0xFF] ^ (crc >> 8);

	return (crc ^ 0xFFFFFFFFUL);
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

void MKI_imitator::new_frame_04(QString mode_in, QVariant frame_data)
{
	QTime timer;

	sock.disconnectFromHost();
	sock.waitForDisconnected(3000);
	
	timer.start();
	//sock.connectToHost("10.44.4.13", 50164);
	sock.connectToHost("127.0.0.1", 50164);
	if (!sock.waitForConnected(3000))
	{
		log_msg("Ошибка сокета: " + sock.errorString());
		return;
	}
	
	QByteArray ba = frame_data.toByteArray();
	
	FrameParams p;
	auto t = p.getParams().value(mode_in, QList<int>());
	int str_count = t[0];	
	int byte_in_str = t[1]; 
	int byte_in_psp = t[26]; 
	int spec_inf = 8;
	QByteArray spec_inf_ba(spec_inf, 1);
	int hat_size = 68;
	QByteArray hat_ba(hat_size, 0);
	int CRC32 = 4;
	QByteArray crc32_ba(CRC32, 0);

	auto head = createCommandHeader(1, str_count*(byte_in_str + byte_in_psp + spec_inf) + hat_size + CRC32);
	QByteArray msg;

	for (int i = 0; i < str_count; ++i)
	{
		msg.push_back(ba.mid(i*(byte_in_str + byte_in_psp), byte_in_psp));
		msg.push_back(ba.mid(i*(byte_in_str + byte_in_psp) + byte_in_psp, byte_in_str));
		msg.push_back(spec_inf_ba);
	}
	head += hat_ba + msg + crc32_ba;
	sock.write(head);
	log_msg(QString("Кадр %1 отправлен").arg(mode_in));

	if (sock.waitForReadyRead(3000) && sock.bytesAvailable())
	{
		log_msg("Пришла квитанция");
		QByteArray kvit = sock.readAll();
	}

	return;
}

void MKI_imitator::newConn()
{
	clientSocket = tcpServer->nextPendingConnection();
	QHostAddress adrv4(clientSocket->peerAddress().toIPv4Address());
	log_msg(QString("Новое подключение с %1:%2").arg(adrv4.toString()).arg(QString::number(clientSocket->peerPort())));
	ip_label->setText(QString("IP клиента: %1:%2").arg(adrv4.toString()).arg(QString::number(clientSocket->peerPort())));
	//connect(clientSocket, SIGNAL(readyRead()), this, SLOT(slotReadClient()));
	connect(clientSocket, &QTcpSocket::readyRead, this, &MKI_imitator::slotReadClient);
}

void MKI_imitator::slotReadClient()
{
	QByteArray read_data;
	QString ret_str;

	while (clientSocket->bytesAvailable())
		read_data += clientSocket->read(clientSocket->bytesAvailable());

	unsigned int MsgTypeC = numberFromBA<unsigned int>(read_data, header_MsgTypeC_ind);	// Код типа сообщения
	log_msg(QString("Пришла команда %1: 0x%2").arg(QString::number(MsgTypeC)).arg(QString(read_data.toHex())));
	log_msg("");

	switch (MsgTypeC)
	{
		case(10102):
			RUValue = numberFromBA<unsigned int>(read_data, msgHeaderLen);
			control_label->setText("Управление: " + control_names[RUValue]);	// Первый байт данных с РУ
			break;
		case(10101):
			RRValue = numberFromBA<unsigned int>(read_data, msgHeaderLen);
			rezhim_label->setText("Режим работы: " + rezhim_names[RRValue]);	// Первый байт данных с РР
			break;
		case(10103):
			RPIKValue = numberFromBA<unsigned int>(read_data, msgHeaderLen);
			RPIK_label->setText("РПИК: " + RPIK_names[RPIKValue]);	// Первый байт данных с РПИК
			break;
		default:
			break;
	}

	QByteArray response_data = createCommandHeader(MsgTypeC + 1e9, 0);
	clientSocket->write(response_data);

	sendKRO(MsgTypeC);
}