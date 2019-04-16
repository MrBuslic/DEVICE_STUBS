#include "RM_MBK07_imitator.h"

RM_MBK07_imitator::RM_MBK07_imitator()  
{
	widg = new QWidget(this);
	setCentralWidget(widg);
	setWindowTitle("Имитатор РМ МБК07");

	p20 = new QPushButton("П20", widg);
	p20->setFixedWidth(150);
	p20->setFixedHeight(100);
	p20->setStyleSheet("background-color: rgb(204, 204, 204);"); //grey light - off
	p21 = new QPushButton("П21", widg);
	p21->setFixedWidth(150);
	p21->setFixedHeight(100);
	p21->setStyleSheet("background-color: rgb(204, 204, 204);"); //grey light - off
	p12 = new QPushButton("П12", widg);
	p12->setFixedWidth(150);
	p12->setFixedHeight(100);
	p12->setStyleSheet("background-color: rgb(204, 204, 204);"); //grey light - off

	bloks_label = new QLabel("Блоки", widg);
	rezhim_label = new QLabel("Режим: ", widg);
	liters_label = new QLabel("Литеры: ", widg);
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

	connect(&_sock, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error_Slot(QAbstractSocket::SocketError)));
	connect(&_sock, &QUdpSocket::readyRead, this, &RM_MBK07_imitator::read);

	//connect(_sock, SIGNAL(readyRead()), SLOT(read()));
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
		short cmdResponse = (0x03 << 8) + 0x00; // ОТВЕТ на любую команду в случае несовпадения КС принятой команды.
		readSize = 5;
		sockbufResponse = new char[readSize];
		CmdToSockbuf(cmdResponse, cmdLen, sockbufResponse);
	}
	else
	{
		short cmdLen = (const_sockbuf[0] << 8) + const_sockbuf[1];		// Длина данных в принятом пакете (первые 2 байта)	
		short cmd = (const_sockbuf[2] << 8) + const_sockbuf[3];			// ИД команды
		short cmdResponse;
		if (commands.contains(cmd))
		{
			cmdResponse = (const_sockbuf[3] << 8) + const_sockbuf[2];	// Ответ. Свапнуть байты команды
			
			switch (cmd)
			{
			case(0x0070):
				if(!selectBlock(p12, const_sockbuf[4], true))
					cmdResponse = (0x03 << 8) + 0x03; // ОТВЕТ в случае обнаружения недопустимого значения параметра принятой команды.
				break;
			case(0x0071):
				if(!selectBlock(p20, const_sockbuf[4], true))
					cmdResponse = (0x03 << 8) + 0x03; // ОТВЕТ в случае обнаружения недопустимого значения параметра принятой команды.
				break;
			case(0x0072):
				if(!selectBlock(p21, const_sockbuf[4], true))
					cmdResponse = (0x03 << 8) + 0x03; // ОТВЕТ в случае обнаружения недопустимого значения параметра принятой команды.
				break;
			case(0x0050):
				if (const_sockbuf[4] == '\xAA')
				{
					if (const_sockbuf[10] == '\x11')
						rezhim_label->setText("Режим: ПИ-15");
					if (const_sockbuf[10] == '\x22')
						rezhim_label->setText("Режим: ПИ-8");
					if (const_sockbuf[10] == '\x33')
						rezhim_label->setText("Режим: ВТФ");
					if (const_sockbuf[10] == '\x44')
						rezhim_label->setText("Режим: произвольный");
				}

				break;
			default:
				break;
			}
		}
		else 
			cmdResponse = (0x03 << 8) + 0x01; // ОТВЕТ в случае нераспознанния идентификатора принятой команды.
		sockbufResponse = new char[readSize];
		CmdToSockbuf(cmdResponse, cmdLen, sockbufResponse);
	}

	auto ttt0 = _sock.writeDatagram(sockbufResponse, readSize, *host, port);		// TODO: КАКОЙ ДОЛЖЕН БЫТЬ РАЗМЕР ОТВЕТА? ВСЕГДА 5(2 байта длинна + 2 байта код + 1 байт КС)?

	delete sockbuf;
	delete host;
	delete sockbufResponse;
}

