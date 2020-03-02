#ifndef FSVA7_IMITATOR_H
#define FSVA7_IMITATOR_H

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

class FSVA7_imitator :
	public QMainWindow
{
	Q_OBJECT
public:
	FSVA7_imitator();
	~FSVA7_imitator();
	

	/*double GetDELay() const;
	void SetDELay();*/



private:
	QWidget* widg;
	
	QUdpSocket _sock;
	QTcpSocket* _ag_sock = nullptr;
	QTcpServer serv_sock;

	QGridLayout *gridlay;

	/*QLabel DELay_label;
	QTextEdit *DELay_edit;	//Значение задержки между включениями источников питания СИБП 
	double DELay;*/

	int SPAN;
	void setSPAN(int span);

	int BWID;
	void setBWID(int bwid);



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

