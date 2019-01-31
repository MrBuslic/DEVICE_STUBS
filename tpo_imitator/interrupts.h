#ifndef INTERRUPTS_H
#define INTERRUPTS_H
#include "winsock2.h"
#include <QString>
#include <QtWidgets>
#include <QTextEdit>
#include <QScrollBar>
#include "CBK_MAIN_Thread.h"
#include "../UNFOI/foi_rpc.h"

class SimInterruptWid : public QObject
{
	Q_OBJECT
public:
	SimInterruptWid();
	RPC_foi_SLOT_Thread* get_rpc_foi_slot_thread() { return slot_int_thread; }
	RPC_foi_SIGNAL_Thread* get_rpc_foi_signal_thread(){ return signal_int_thread; }
	void create_slot_thread();
	void create_signal_thread();
public slots:
void get_inter_from_server(int _n, short _chan, double _u, double _t);
void slot_interrupt_1();
void slot_interrupt_5();
void slot_interrupt_13();
void slot_unknown_interrupt();
signals:
void signal_send_request();
private:
	/*QTextEdit* edit;
	QScrollBar* _scroll_bar;*/
	RPC_foi_SLOT_Thread* slot_int_thread;
	RPC_foi_SIGNAL_Thread* signal_int_thread;
	CBK_MAIN_Thread* CBK_Thread;
	MyThread* m_thread;
};
#endif