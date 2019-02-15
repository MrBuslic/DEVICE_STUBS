#ifndef AG7972_H
#define AG7972_H
#ifdef WIN32
#include "winsock2.h"
#endif
#include <QObject>
#include <QString>
#include <QWidget>
#include <QThread>
#include <QTextEdit>
#include <QScrollBar>
#include <QCheckBox>
#include <QTimer>
#include <QMutex>
#include <QLineEdit>
#include <QLabel>
#include <QTcpSocket>
#include <QTcpServer>
#include <memory>
#include <qlayout.h>
#include <loki/Singleton.h>

#define SINGLETON_DEF(x) typedef Loki::SingletonHolder<x,Loki::CreateUsingNew,Loki::NoDestroy> S##x;

class AG7972Widget : public QWidget
{
	Q_OBJECT
public:
	AG7972Widget();
public slots:
	void tcp_slot();
	void read_data();
	void update_graphics();
private:
	QTcpServer* server;
	QTcpSocket* socket;

	QLineEdit* R_edit;
	QLabel* u_label;
	QLabel* i_label;
	QLabel* state_label;
	QLabel* u_meas_label;
	QLabel* i_meas_label;

	double u;
	double i;
	bool state;

	double u_meas;
	double i_meas;
	void calc_meas();
	
signals:
	void power_out(int n, double _u);
};

#endif //AG7972_H