#ifndef N6705_H
#define N6705_H
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
#include "rpc_loger.h"

#define SINGLETON_DEF(x) typedef Loki::SingletonHolder<x,Loki::CreateUsingNew,Loki::NoDestroy> S##x;

struct chan_state
{
	bool state;
	double u;
	double i;

	double u_meas;
	double i_meas;
};

class N6705Widget : public QWidget
{
	Q_OBJECT
public:
	N6705Widget();
public slots:
	void tcp_slot();
	void read_data();
	void update_graphics();
private:
	QTcpServer* server;
	
	QList<QLabel*> u_labels;
	QList<QLabel*> i_labels;
	QList<QLabel*> state_labels;
	QList<QLabel*> u_meas_labels;
	QList<QLabel*> i_meas_labels;

	void calc_meas();
	
	QList<chan_state> chan_states;
	LogWidget* log_widget;
signals:
	void update_graphics_signal();
	void power_out(int n, double _u);
protected:
	void closeEvent(QCloseEvent *event);
};

#endif //N6705_H