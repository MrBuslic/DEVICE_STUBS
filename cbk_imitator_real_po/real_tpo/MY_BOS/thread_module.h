#ifndef MY_THREAD_H
#define MY_THREAD_H
#include <QThread>
#include <QTime>
#include <QMutex>
#include <QVariantList>
#ifdef __cplusplus
extern "C" {
#endif
#include "bos.h"
#ifdef __cplusplus
}
#endif


class TPOThread : public QThread
{
	Q_OBJECT
public:
	TPOThread();
	void run();
};


class TimeThread : public QThread
{
	Q_OBJECT
public:

	static TimeThread& Instance()
	{
		static TimeThread inst;
		return inst;
	}
	TimeThread(TimeThread const&) = delete;
	TimeThread& operator= (TimeThread const&) = delete;

	void run();
	cfgSYS_t cbk_conf;
private:

	TimeThread()
	{
		cbk_conf.vm = 0;
		cbk_conf.rs = 0;
		cbk_conf.er = 0;
		cbk_conf.of = 0xF;
		cbk_conf.m0 = 0;
		cbk_conf.b0 = 0;
		cbk_conf.m1 = 0;
		cbk_conf.b1 = 0;
		cbk_conf.no = 0;
		cbk_conf.dv = 0;
		cbk_conf.rg = CFGSYS_rgWork;

	};
	~TimeThread() {};


	QTime BoardTime;
public slots:
	int getCurTime();
	void stop_thread();

};
typedef TimeThread STimeThread;


class InterHandlerThread : public QThread
{
	Q_OBJECT
public:
	InterHandlerThread() {};
	~InterHandlerThread() {};
	void run();
	void process_interrupt(int n_);
	bool check_stack();
	void stop_thread();
private:
	QVariantList InterruptStack;
	QMutex mutex;
	bool is_locked = FALSE;
	bool is_running = FALSE;
	public slots:
	void add_interrupt_to_stack(int inter_n);
};
#endif

