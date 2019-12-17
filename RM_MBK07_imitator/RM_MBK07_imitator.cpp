#include "RM_MBK07_imitator.h"
#include <qapplication.h>
RM_MBK07_imitator::RM_MBK07_imitator()  
{
	widg = new QWidget(this);
	setCentralWidget(widg);
	setWindowTitle("Имитатор РМ МБК07");
	PSP = "ПСП: ";
	IM = "";
	FM = "";
	p20 = new QPushButton("П20", widg);
	p20->setFixedWidth(90);
	p20->setFixedHeight(90);
	p20->setStyleSheet("background-color: rgb(204, 204, 204);"); //grey light - off
	p21 = new QPushButton("П21", widg);
	p21->setFixedWidth(90);
	p21->setFixedHeight(90);
	p21->setStyleSheet("background-color: rgb(204, 204, 204);"); //grey light - off
	p12 = new QPushButton("П12", widg);
	p12->setFixedWidth(90);
	p12->setFixedHeight(90);
	p12->setStyleSheet("background-color: rgb(204, 204, 204);"); //grey light - off

	bloks_label = new QLabel("Блоки", widg);
	rezhim_label = new QLabel("Режим: ", widg);
	liters_label = new QLabel("Литера: ", widg);
	antenna_label = new QLabel("Антенна: ", widg);

	log_edit = new QTextEdit(widg);
	log_edit->setReadOnly(true);
	_scroll_bar = new QScrollBar();
	_scroll_bar = log_edit->verticalScrollBar();

	gridLayout = new QGridLayout(widg);

	gridLayout->addWidget(bloks_label, 0, 0, 1, 3, Qt::AlignHCenter);
	gridLayout->addWidget(p12, 1, 0);
	gridLayout->addWidget(p20, 1, 1);
	gridLayout->addWidget(p21, 1, 2);
	gridLayout->addWidget(rezhim_label, 3, 0);
	gridLayout->addWidget(liters_label, 4, 0);
	gridLayout->addWidget(antenna_label, 5, 0);
	gridLayout->addWidget(log_edit, 6, 0, 1, 3);
	

	setCommandsVec();
	
	QSettings tmp_settings("Cometa", "СПО МКПА МЦА");
	QString ip = tmp_settings.value("IP_rm", "192.168.0.100").toString();
	if (ip == "localhost")	// QUdpSocket::bind не жрёт "localhost". Ему "127.0.0.1" подавай.
		ip = "127.0.0.1";
	bool t = _sock.bind(QHostAddress(ip), 10001);

	serv_sock.listen(QHostAddress("127.0.0.1"), 30302);


	connect(&_sock, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error_Slot(QAbstractSocket::SocketError)));
	connect(&_sock, &QUdpSocket::readyRead, this, &RM_MBK07_imitator::read);
	connect(&serv_sock, &QTcpServer::newConnection, this, &RM_MBK07_imitator::connect_ag);
	
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

void RM_MBK07_imitator::closeEvent(QCloseEvent *event)
{
	QSettings settings(QApplication::applicationDirPath() + "/positions.ini", QSettings::IniFormat);
	settings.setValue("rmmbk07_geometry", saveGeometry());
	QWidget::closeEvent(event);
}

RM_MBK07_imitator::~RM_MBK07_imitator()
{
	delete p20;
	delete p21;
	delete p12;
	delete bloks_label;
	delete rezhim_label;
	delete liters_label;
	delete antenna_label;
	delete _scroll_bar;
	delete log_edit;
	delete gridLayout;
	delete widg;

}

void RM_MBK07_imitator::error_Slot(QAbstractSocket::SocketError socketError)
{
	QString tmp = _sock.errorString();
}

bool RM_MBK07_imitator::checkCS(const char *sockbuf)
{
	short rpLen = sockbuf[0] * 256 + sockbuf[1];		// длина данных в принятом пакете (первые 2 байта)	
	if (rpLen > (568 - 45) || rpLen == 0)  // EMAC_BLOCK_SIZE - ((ethernet+IP+UDP) headers - commandLen(2) - commandCode(2) + 1)
		return false; //Длина поля данных, указанная в принятом пакете, превышает макс. допустимую ИЛИ в принятом пакете указана нулевая длина поля данных
	
	char rpCS = sockbuf[2 + rpLen];		// КС поля данных в принятом пакете
	char rpCStmp = 0;
	for (int i = 0; i<rpLen; i++)
		rpCStmp ^= sockbuf[2 + i];		// КС - исключающее ИЛИ (1^1 = 0)
	if (rpCStmp != rpCS)	
		return false;	//Контрольная сумма в принятом пакете не сопадает

	return true;
}


void RM_MBK07_imitator::CmdToSockbuf(short CmdID, short CmdLen, char* sockbuf)
{
	char CmdCS = 0;
	
	sockbuf[0] = (CmdLen >> 8) & 255; // старший байт длины команды (ИД+данные)
	sockbuf[1] = CmdLen & 255;		  // младший байт длины сообщения

	sockbuf[2] = (CmdID >> 8) & 255; // старший байт идентификатора команды
	sockbuf[3] = CmdID & 255;		 // младший байт идентификатора команды

	for (short i = 0; i<CmdLen; i++)
		CmdCS ^= sockbuf[i + 2];				// КС - исключающее ИЛИ (1^1 = 0)

	sockbuf[2 + CmdLen] = CmdCS;
}

bool RM_MBK07_imitator::selectBlock(QPushButton * btn, char data, bool set)
{
	if (data == '\xAA')
	{
		btn->setStyleSheet("background-color: rgb(142, 198, 156);"); //green light - working
		return true;
	}
	else if (data == '\x55')
	{
		btn->setStyleSheet("background-color: rgb(204, 204, 204);"); //grey light - off
		return true;
	}

	return false;
}

bool RM_MBK07_imitator::selectAntenna(char data)
{
	if (data == '\01')
	{
		antenna_label->setText("Антенна: ОНА");
		ant = "ОНА";
		return true;
	}
	if (data == '\00')
	{
		antenna_label->setText("Антенна: МНА_плюс_У");
		ant = "МНА+Y";
		return true;
	}
	if (data == '\02')
	{
		antenna_label->setText("Антенна: МНА_минус_У");
		ant = "МНА-Y";
		return true;
	}
	return false;
}

bool RM_MBK07_imitator::isset(short x, short n)
{
	return (x & ((short)1 << n)) != 0;
}

void RM_MBK07_imitator::read()
{
	qint64 received_bytes = _sock.bytesAvailable();
	char* sockbuf;
	sockbuf = new char[received_bytes];

	auto *host = new QHostAddress();
	quint16 port = 0;
	auto readSize = _sock.readDatagram(sockbuf, received_bytes, host, &port);
	const char* const_sockbuf = sockbuf;

	QList<char> tmp;
	for (int i = 0; i < readSize; ++i)
		tmp.push_back(const_sockbuf[i]);

	char* sockbufResponse;
	if (!checkCS(const_sockbuf))
	{
		short cmdLen = 0x0002;
		short cmdResponse = ERROR_CMD_CS; // ОТВЕТ на любую команду в случае несовпадения КС принятой команды.
		readSize = 5;
		sockbufResponse = new char[readSize];
		CmdToSockbuf(cmdResponse, cmdLen, sockbufResponse);
	}
	else
	{
		short cmdLen = (const_sockbuf[0] << 8) + (const_sockbuf[1] & 0xFF);		// Длина данных в принятом пакете (первые 2 байта)	
		short cmd = (const_sockbuf[2] << 8) + (const_sockbuf[3] & 0xFF);			// ИД команды
		short cmdResponse;
		if (commands.contains(cmd))
		{
			cmdResponse = (const_sockbuf[3] << 8) + const_sockbuf[2];	// Ответ. Свапнуть байты команды

			short cmdMKO = (const_sockbuf[4] << 8) + (const_sockbuf[5] & 0xFF);	// код команд МКО
			short dataMKO = (const_sockbuf[6] << 8) + (const_sockbuf[7] & 0xFF);
			switch (cmd)
			{
			case(P12_POWER):
				if(!selectBlock(p12, const_sockbuf[4], true))
					cmdResponse = ERROR_CMD_PAR; // ОТВЕТ в случае обнаружения недопустимого значения параметра принятой команды.
				break;
			case(P20_POWER):
				if(!selectBlock(p20, const_sockbuf[4], true))
					cmdResponse = ERROR_CMD_PAR; // ОТВЕТ в случае обнаружения недопустимого значения параметра принятой команды.
				break;
			case(P21_POWER):
				if(!selectBlock(p21, const_sockbuf[4], true))
					cmdResponse = ERROR_CMD_PAR; // ОТВЕТ в случае обнаружения недопустимого значения параметра принятой команды.
				break;
			case(BERT_START):
				if (const_sockbuf[4] == '\xAA')
				{
					if (const_sockbuf[10] == '\x11')
						mode = "ПИ15";
					if (const_sockbuf[10] == '\x22')
						mode = "ПИ8";
					if (const_sockbuf[10] == '\x33')
						mode = "ВТФ";
					if (const_sockbuf[10] == '\x44')
						mode = "произвольный";
					rezhim_label->setText(QString("Режим: %1 %2 %3 %4").arg(mode).arg(PSP).arg(FM).arg(IM));
				}

				break;
			case(PRBCH_CHANNEL_SWITCH):
				if (!selectAntenna(const_sockbuf[4]))
					cmdResponse = ERROR_CMD_PAR; // ОТВЕТ в случае обнаружения недопустимого значения параметра принятой команды.
				break;
			case(MKOCMD_SEND):
				switch (cmdMKO)
				{
					case(MKOCMD_SETPSP):
						if (dataMKO == MKO_PSP1)
							PSP = "ПСП-1";
						if (dataMKO == MKO_PSP2)
							PSP = "ПСП-2";
						if (dataMKO == MKO_PSP3)
							PSP = "ПСП-3";
						if (dataMKO == MKO_PSP4)
							PSP = "ПСП-4";
						rezhim_label->setText(QString("Режим: %1 %2 %3 %4").arg(mode).arg(PSP).arg(FM).arg(IM));
						break;
				// Литера по МКО не ставится
				  /*case(MKOCMD_SETLIT):
						if (dataMKO == MKO_LIT1)
							liters_label->setText("Литера: 1");
						if (dataMKO == MKO_LIT2)
							liters_label->setText("Литера: 2");
						if (dataMKO == MKO_LIT3)
							liters_label->setText("Литера: 3");
						if (dataMKO == MKO_LIT4)
							liters_label->setText("Литера: 4");
						if (dataMKO == MKO_LIT5)
							liters_label->setText("Литера: 5");
						if (dataMKO == MKO_LIT6)
							liters_label->setText("Литера: 6");
						if (dataMKO == MKO_LIT7)
							liters_label->setText("Литера: 7");
						if (dataMKO == MKO_LIT8)
							liters_label->setText("Литера: 8");
						qDebug() << QString("MKOCMD_SETLIT Режим: %1 %2 %3").arg(mode).arg(PSP).arg(IM);
						break;*/
					case(MKOCMD_SETMODE):
						FM = "";
						IM = "";
						if (isset(dataMKO, 4) && isset(dataMKO, 12))
							IM = "ИМ";
						if(IM == "")	// Иначе если просто ставили ИМ, псп потеряется
							PSP = "";
						if (isset(dataMKO, 6) && isset(dataMKO, 14))
						{
							FM = "ФМ15 ";
							fm = 2;
						}
						if (!isset(dataMKO, 6) && !isset(dataMKO, 14))
						{
							FM = "ФМ1,5 ";
							fm = 1;
						}

						rezhim_label->setText(QString("Режим: %1 %2 %3 %4").arg(mode).arg(PSP).arg(FM).arg(IM));
						break;
					default:
						break;
				}
				break;
			default:
				break;
			}
		}
		else 
			cmdResponse = ERROR_CMD_UNKN; // ОТВЕТ в случае нераспознанния идентификатора принятой команды.
		sockbufResponse = new char[readSize];
		CmdToSockbuf(cmdResponse, cmdLen, sockbufResponse);
	}

	auto ttt0 = _sock.writeDatagram(sockbufResponse, readSize, *host, port);		// TODO: КАКОЙ ДОЛЖЕН БЫТЬ РАЗМЕР ОТВЕТА? ВСЕГДА 5(2 байта длинна + 2 байта код + 1 байт КС)?

	delete sockbuf;
	delete host;
	delete sockbufResponse;
}

void RM_MBK07_imitator::connect_ag()
{
	if (_ag_sock)
		if((_ag_sock->state() == QAbstractSocket::ConnectedState))
		{
			_ag_sock->close();
			delete _ag_sock;
			_ag_sock = nullptr;
		}
	_ag_sock = serv_sock.nextPendingConnection();
	connect(_ag_sock, &QTcpSocket::readyRead, this, &RM_MBK07_imitator::read_ag);
	//char c = '\x01';
	//_ag_sock->write(&c);
}

void RM_MBK07_imitator::read_ag()
{

	QByteArray read_data;
	QString ret_str;

	while (_ag_sock->bytesAvailable())
	{
		read_data += _ag_sock->read(_ag_sock->bytesAvailable());
		//		Sleep(10);
	}
	QString command(read_data.toStdString().c_str());
	command.chop(2);

	QString command_pars = command.left(5);
	
	if (command_pars == "FREQ ")
	{
		GHz = command.mid(5, 4).toDouble();
		const double ZERO_LIT = 2.56;
		const double STEP_LIT = 0.02;
		const double magic = 0.001;		// Для корректного округления. Иначе с 16-ой литеры расчитывает на 1 литеру меньше

		currLit = ((ZERO_LIT - GHz) + magic) / STEP_LIT;
		liters_label->setText("Литера: " + QString::number(currLit));
	}

	if (command == "FREQ?")
	{
		QString s = QString::number(GHz*1e9);
		_ag_sock->write(s.toStdString().c_str());
	}
}

void RM_MBK07_imitator::new_frame_07(QString mode_in, int psp_in, int lit_in, int _fm, QString _ant, QVariant frame_data)
{
	if (IM == "ИМ")
		return;
	if ((mode != mode_in) || (currLit != lit_in) || (ant != _ant))
		return;

	if ((mode == "ПИ8") && ((fm != _fm) || (PSP.right(1).toInt() != psp_in)))
		return;

	frame_slot_thr.get_frame_bus_obj()->make_new_frame_rm07(mode_in, frame_data);

	return;
}