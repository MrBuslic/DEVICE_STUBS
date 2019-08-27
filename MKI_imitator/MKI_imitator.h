#ifndef MKI_IMITATOR_H
#define MKI_IMITATOR_H

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
#include "../../ServerSide/MBK07/KPAdefines.h"
#include "../buses_imitator/frame_bus_rpc.h"
#include "rpc_ports.h"

class MKI_imitator :
	public QMainWindow
{
	Q_OBJECT
public:
	MKI_imitator();
	~MKI_imitator();
	
private:
	QWidget* widg;
	

	QUdpSocket _sock;
	QTcpSocket* _ag_sock = nullptr;
	QTcpServer serv_sock;
	
	QLabel *control_label;
	QLabel *rezhim_label;
	QLabel *ip_label;
	QLabel *RPIK_label;	// Режим Приёма Кадра

	QTextEdit *log_edit;
	QScrollBar* _scroll_bar;

	QGridLayout *gridLayout;
	
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

