#ifndef INTERRUPTS_H
#define INTERRUPTS_H
#include "winsock2.h"
#include <QString>
#include "../UNFOI/foi_rpc.h"
#include <QVector>

class Interrupt_Object : public QObject
{
	Q_OBJECT
public:
	Interrupt_Object();
	RPC_foi_SLOT_Thread* get_rpc_foi_slot_thread() { return slot_int_thread; }
	RPC_foi_SIGNAL_Thread* get_rpc_foi_signal_thread(){ return signal_int_thread; }
	void create_slot_thread();
	void create_signal_thread();
	void stop_slot_thread();
	void stop_signal_thread();
	void restart_slot_thread(QString ip_str, int port);
	void restart_signal_thread(QString ip_str, int port);
	bool check_inter_stack();
	QVector<int> get_interrupts();
	void clear_inter_stack();
public slots:
	RPC_foi_SLOT_Thread* get_slot_int_thread() { return slot_int_thread; };
	RPC_foi_SIGNAL_Thread* get_signal_int_thread() { return signal_int_thread; };
	void get_inter_from_server(int _n, short _chan, double _u, double _t);
signals:
	void need_handle_interrupt();
private:
	QMutex m_mutex;
	RPC_foi_SLOT_Thread* slot_int_thread;
	RPC_foi_SIGNAL_Thread* signal_int_thread;
	QVector<int> stack_of_interrupts;
	void add_inter_to_stack(int _n);
};
#endif