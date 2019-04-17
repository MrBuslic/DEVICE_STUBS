#ifndef RM_MBK07_IMITATOR_H
#define RM_MBK07_IMITATOR_H

#include <QMainWindow>
#include <QPushButton>
#include <QLayout>
#include <QLabel>
#include <QTextEdit>
#include <QScrollBar>
#include <QList>
#include <QMessageBox>
#include <QUDPSocket>
#include <QSettings>
#include <QVector>
#include "../../ServerSide/MBK07/KPAdefines.h"

class RM_MBK07_imitator :
	public QMainWindow
{
	Q_OBJECT
public:
	RM_MBK07_imitator();
	~RM_MBK07_imitator();
	
private:
	QWidget* widg;
	
	QPushButton *p20;
	QPushButton *p21;
	QPushButton *p12;

	QLabel *bloks_label;

	QLabel *rezhim_label;
	QLabel *liters_label;
	QLabel *antenna_label;

	QTextEdit *log_edit;
	QScrollBar* _scroll_bar;

	QGridLayout *gridLayout;

	QUdpSocket _sock;
	QUdpSocket _ag_sock;
	/// Проверка контрольной суммы
	bool checkCS(const char *sockbuf);
	/// Формирование ответа
	void CmdToSockbuf(short CmdID, short CmdLen, char* sockbuf);
	QVector<int> commands;
	/// заполнение списока команд
	void setCommandsVec()
	{
		commands << LINK_CONFIRM << DO_RESET << DO_RAMTEST << MATRIXCMD_SEND << MKOCMD_SEND << MKOBUS_CHOOSE << CHANNEL_SWITCH << VOLTAGE_GET << PRBCH_CHANNEL_SWITCH
			<< BERT_START << BERTDATA_GET << FPGACONF_GET << FPGACONF_WRITE << PI8MT_TOGGLE << P12_POWER << P20_POWER << P21_POWER << P12_MFK << P20_MU << P21_MU
			<< P20_INTSYNC << P21_INTSYNC << P20_RESET << P21_RESET << P_DIAGN << P12_PSPOFF << CIRCTRL_GET;
	}
	/// Проверка параметра и окраска соответствующего блока
	bool selectBlock(QPushButton* btn, char data, bool set);

signals:

public slots :
	void error_Slot(QAbstractSocket::SocketError socketError);
	void read();
	void read_ag();

};

#endif

