#ifndef RPC_MN8I_H
#define RPC_MN8I_H
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
#include "../buses_imitator/mku_bus_rpc.h"
#include "rpc_loger.h"


#define SINGLETON_DEF(x) typedef Loki::SingletonHolder<x,Loki::CreateUsingNew,Loki::NoDestroy> S##x;

class RpcMN8IWidget : public QWidget
{
	Q_OBJECT
public:
	RpcMN8IWidget(int mn8i_num);
public slots:

	void measurement_timer_ontimer();
	void infin_timer_ontimer();

	void new_ku(int ku_n, int length, double u, int line);
	void new_mk(int mshm, int pshm, int length_m, int length_p, double u_m, double u_p, int dt, int line_m, int line_p); 

	int unmn8i_start();
	int unmn8i_input_trigger(bool state);
	int unmn8i_sample_width_q(uint& frame_width, uint&  width_in_bytes);
	int unmn8i_read_sample(uint& _buf, uint& _firstTime, uint& _thisTime);
	int unmn8i_read_packet(bool isHot, uint numSamples, QVariantList& buf, uint& realNumSamples);
	int unmn8i_sample_period(double _periodS);
	int unmn8i_mode_cycle(uint _size);
	int unmn8i_num_ready_data(uint& _num);
	int unmn8i_stop();

	void button_clicked();
private:
	LogWidget* log_widget;

	std::unique_ptr<QTimer> infin_timer;
	QTime begin_time;

	bool state;

	double periodS;
	uint samples;
	bool measuring;
	bool infinit;

	QList <QLineEdit*>  checks; 
	QLineEdit* buf_edit;
	QVariantList buffer;
	QMap<QObject*, int> buttons;
	double impulse_length;

	RPC_mku_bus_SLOT_Thread mku_slot_thr;
	RPC_mku_bus_SIGNAL_Thread mku_signal_thr;

	void form_impulse(int chan, double length, double u);
signals:
	void packet_ready();
};

#endif