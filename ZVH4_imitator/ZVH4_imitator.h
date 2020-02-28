#ifndef ZVH4_IMITATOR_H
#define ZVH4_IMITATOR_H

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

#include <boost/range/combine.hpp>
#include <boost/tuple/tuple.hpp>
#include <QGridLayout>

class ZVH4_imitator :
	public QMainWindow
{
	Q_OBJECT
public:
	ZVH4_imitator();
	~ZVH4_imitator();
	


private:
	QWidget* widg;
	
	QUdpSocket _sock;
	QTcpSocket* _ag_sock = nullptr;
	QTcpServer serv_sock;

protected:
	void closeEvent(QCloseEvent *event);


signals:


public slots:
	void error_Slot(QAbstractSocket::SocketError socketError);
	void read();
	void connect_ag();
	void read_ag();


};

#endif

