#ifndef CBK_MAIN_THREAD_H
#define CBK_MAIN_THREAD_H

#include <QThread>
#include <QTimer>
#include <QVector>
#include <QMutex>

class MyThread :public QThread
{
	Q_OBJECT
public:
	MyThread();
	void run();
};

class CBK_MAIN_Thread : public QObject
{
	Q_OBJECT
private:
	QMutex m_mutex;
	QVector<int> stack_of_interrupts;
	QTimer m_timer;
	
public:
	CBK_MAIN_Thread();
	void append_interrupt(int);
	void stop_process();
	QVector<int> get_interrupt();
	int get_qty_interrupt();
	void process();
	
public slots:
	void check_interrupt();
	void slot_handle_interrupt();
signals:
	void signal_handle_interrupt();
	void finished();
	void checking_interrupts();
	void signal_interrupt_1();
	void signal_interrupt_5();
	void signal_interrupt_13();
	void signal_unknown_interrupt();
};
#endif
// CBK_MAIN_THREAD_H