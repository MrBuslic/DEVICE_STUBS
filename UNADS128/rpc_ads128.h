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
#include <loki/Singleton.h>
#include "../LKA-05_imitator/lka05_rpc.h"

#define SINGLETON_DEF(x) typedef Loki::SingletonHolder<x,Loki::CreateUsingNew,Loki::NoDestroy> S##x;

class RpcADS128Widget : public QWidget
{
	Q_OBJECT
public:
	RpcADS128Widget(int _ads_num);
public slots:

	void auto_scroll_clicked(int _state);
	void log_timer_ontimer();
	void ads_timer_ontimer();


	void new_ku(int ku_n, int length, double u);
	void new_mk(int mshm, int pshm, int length_m, int length_p, double u_m, double u_p, int dt);

	int ads128_start();
	int ads128_read_data(QVariantList& thisbuf, QVariantList& firstbuf);
	int ads128_stop();
private:
	void add_signal(int ads_chan, double _u);


	QTextEdit* edit;
	QScrollBar* _scroll_bar;
	QTextDocument* _doc;
	QTextCursor* _cursor;
	QPushButton* push_b; 
	QCheckBox* auto_scroll_box;
	bool auto_scroll;
	QString log_filename;
	QTimer log_timer;
	QTime begin_time;

	std::unique_ptr<QTimer> ads_timer;
	QMutex ads_mutex;
	QStringList log_buffer;
	QMutex log_mutex;
	bool state;
	bool timerads;

	bool running;

	void form_ads(int chan, double length, double u);
	QVariantList state_buffer; 
	QVariantList buffer;

	int ads_num;

	RPC_lka05_SLOT_Thread lka05_slot_thr;
	RPC_lka05_SIGNAL_Thread lka05_signal_thr;

	double step_1 = 5.0;
	double step_2= 15.0;
};

#endif