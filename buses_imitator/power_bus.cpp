#include "power_bus.h"
#include "rpc_loger.h"
#include <QMessageBox>

#include "rpc_ports.h"
#include "power_bus_socket_rpc.h"

PowerWidget::PowerWidget(QWidget *parent)
{
	LogWidget* log_w = new LogWidget(this);


	QString ip_str = "127.0.0.1";
	int slot_port = POWER_SLOT;
	int signal_port = POWER_SIGNAL;
	Socket_RPC_SLOT_Server_Thread* rpc_slot_srv = new Socket_RPC_SLOT_Server_Thread;
	rpc_slot_srv->set_app(this);
	rpc_slot_srv->set_params(ip_str, slot_port);
	rpc_slot_srv->start();
	Socket_RPC_SIGNAL_Thread* rpc_signal_srv = new Socket_RPC_SIGNAL_Thread;
	rpc_signal_srv->set_app(this);
	rpc_signal_srv->set_params(ip_str, signal_port);
	rpc_signal_srv->start();
}

void PowerWidget::set_u(int bus, double volt)
{

}

void PowerWidget::get_i(int bus, double& curr)
{

}

void PowerWidget::set_i(int bus, QString name, double curr)
{

}