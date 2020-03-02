#ifndef IBEP_imitator_H
#define IBEP_imitator_H

#include <QMainWindow>
#include <QPushButton>
#include <QLayout>
#include <QLabel>
#include <QTextEdit>
#include <QScrollBar>
#include <QList>

#include "../buses_imitator/power_bus_rpc.h"

#include <qtcpserver.h>

class IBEP_UI_conn : public QObject
{
	Q_OBJECT
public:
	IBEP_UI_conn(QTcpSocket* _sock);
public slots :
	void newData();
private:
	QTcpSocket* sock;

signals:
	void getI(double* _curr);
	void getU(double* _volt);
};

class  IBEP_UI : public QThread
{
	Q_OBJECT
public:
	IBEP_UI() {}
	void run();

public slots:
	void newConnection();
signals:
	void getI(double* _curr);
	void getU(double* _volt);
private:
	QTcpServer* _srv;
};

class IBEP_CONTR_conn : public QObject
{
	Q_OBJECT
public:
	IBEP_CONTR_conn(QTcpSocket* _sock);
public slots :
	void newData();
private:
	QTcpSocket* sock;
	bool nk_state;
	bool kom_state;
signals:
	void setNK(bool on);
	void setKOM(bool on);
	void setU(double u);
};

class  IBEP_CONTR : public QThread
{
	Q_OBJECT
public:
	IBEP_CONTR() {}
	void run();
public slots:
	void newConnection();
signals:
	void setNK(bool on);
	void setKOM(bool on);
	void setU(double u);
private:
	QTcpServer* _srv;
};

class IBEP_imitator :
	public QMainWindow
{
	Q_OBJECT
public:
	IBEP_imitator();
	~IBEP_imitator();
	
private:
	QWidget* widg;
	
	QPushButton *NK_btn;
	QPushButton *KOM_btn;

	QLabel *voltage_label;
	QLabel *amperage_label;

	QGridLayout *gridLayout;


	RPC_power_bus_SLOT_Thread power_slot_thr;
	RPC_power_bus_SIGNAL_Thread power_signal_thr;

	IBEP_CONTR contr_thread;
	IBEP_UI ui_thread;
	double u_in;

	bool nk_state;
	bool kom_state;
signals:
	void test(QByteArray ba);

public slots:
	void receiveData(QByteArray ba);
	void setNK(bool on);
	void setKOM(bool on);

	void getI(double* _curr);
	void getU(double* _volt);
	void setU(double u);

};

#endif

