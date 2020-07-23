#ifndef RPC_ADS128_H
#define RPC_ADS128_H
#ifdef WIN32
#include "winsock2.h"
#endif
#include <QObject>
#include <QString>
#include <QWidget>
#include <QThread>
#include <QTextEdit>
#include <QPushButton>
#include <QScrollBar>
#include <QCheckBox>
#include <QTimer>
#include <QMutex>
#include <QLineEdit>
#include <QTime>
#include <memory>
#include <qlayout.h>
#include "../buses_imitator/mku_bus_rpc.h"
#include "rpc_loger.h"


class RpcADS128Widget : public QWidget
{
	Q_OBJECT
public:
	RpcADS128Widget(int _ads_num);
public slots:

	void ads_timer_ontimer();

	void new_ku(int ku_n, int length, double u, int line);
	void new_mk(int mshm, int pshm, int length_m, int length_p, double u_m, double u_p, int dt, int line_m, int line_p);
	int ads128_conf_analog(uint group, double level_0, double level_1);
	int ads128_start();
	int ads128_read_data(QVariantList& thisbuf, QVariantList& firstbuf);
	int ads128_stop();
	int ads128_analog_q(uint group_, double& lev0, double& lev1);
private:
	void add_signal(int ads_chan, double _u);
	LogWidget* log_widget;

	QTime begin_time;

	std::unique_ptr<QTimer> ads_timer;
	QMutex ads_mutex;
	bool state;
	bool timerads;

	bool running;

	QVariantList state_buffer; 
	QVariantList buffer;

	int ads_num;

	RPC_mku_bus_SLOT_Thread mku_slot_thr;
	RPC_mku_bus_SIGNAL_Thread mku_signal_thr;

	double step_1;
	double step_2;
};

#endif