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
#include <QTCPSocket>
#include <QTcpServer>
#include <QSettings>
#include <QVector>
#include "KPAdefines.h"
//#include "../ServerSide/MBK07/KPAdefines.h"
//#include "KPAdefines.h"
#include "../buses_imitator/frame_bus_rpc.h"
#include "rpc_ports.h"

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
	QTcpSocket* _ag_sock = nullptr;
	QTcpServer serv_sock;
	QString mode;
	QString PSP;
	QString IM;
	QString FM;
	int fm;
	QString ant;
	QString tmp_mode;
	/// Проверка контрольной суммы
	bool checkCS(const char *sockbuf);
	/// Формирование ответа
	void CmdToSockbuf(short CmdID, short CmdLen, char* sockbuf);
	QVector<int> commands;
	/// Установленная частота на генераторе
	double GHz;
	/// Литера
	int currLit;
	/// заполнение списока команд
	void setCommandsVec()
	{
		commands << LINK_CONFIRM << DO_RESET << DO_RAMTEST << MATRIXCMD_SEND << MKOCMD_SEND << MKOBUS_CHOOSE << CHANNEL_SWITCH << VOLTAGE_GET << PRBCH_CHANNEL_SWITCH
			<< BERT_START << BERTDATA_GET << FPGACONF_GET << FPGACONF_WRITE << PI8MT_TOGGLE << P12_POWER << P20_POWER << P21_POWER << P12_MFK << P20_MU << P21_MU
			<< P20_INTSYNC << P21_INTSYNC << P20_RESET << P21_RESET << P_DIAGN << P12_PSPOFF << CIRCTRL_GET;
	}
	/// Проверка параметра и окраска соответствующего блока
	bool selectBlock(QPushButton* btn, char data, bool set);
	bool selectAntenna(char data);
	bool isset(short x, short n);

	RPC_frame_bus_SLOT_Thread frame_slot_thr;
	RPC_frame_bus_SIGNAL_Thread frame_signal_thr;
protected:
	void closeEvent(QCloseEvent *event);
signals:
	void make_new_frame_rm07(QString mode, QVariant frame_data);

public slots :
	void error_Slot(QAbstractSocket::SocketError socketError);
	void read();
	void connect_ag();
	void read_ag();
	void new_frame_07(QString mode_in, int psp_in, int lit_in, int _fm, QString _ant, QVariant frame_data);

};

#endif

