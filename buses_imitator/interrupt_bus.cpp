#include "interrupt_bus.h"
#include "rpc_loger.h"
#include <QMessageBox>

#include "rpc_ports.h"
#include "interrupt_bus_socket_rpc.h"

InterruptWidget::InterruptWidget(QWidget *parent)
{
	LogWidget* log_w = new LogWidget(this);


	QString ip_str = "127.0.0.1";
	int slot_port = INTERRUPTS_SLOT;
	int signal_port = INTERRUPTS_SIGNAL;
	interrupt_bus_Socket_RPC_SLOT_Server_Thread* rpc_slot_srv = new interrupt_bus_Socket_RPC_SLOT_Server_Thread;
	rpc_slot_srv->set_app(this);
	rpc_slot_srv->set_params(ip_str, slot_port);
	rpc_slot_srv->start();
	interrupt_bus_Socket_RPC_SIGNAL_Thread* rpc_signal_srv = new interrupt_bus_Socket_RPC_SIGNAL_Thread;
	rpc_signal_srv->set_app(this);
	rpc_signal_srv->set_params(ip_str, signal_port);
	rpc_signal_srv->start();
}

void InterruptWidget::make_interrupt(int _n, short _chan, double _u, double _t)
{
	emit new_interrupt(_n, _chan, _u, _t);
}
