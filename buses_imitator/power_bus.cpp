#include "buses.h"
#include "rpc_loger.h"
#include <QMessageBox>

#include "rpc_ports.h"
#include "buses_socket_rpc.h"

BusesWidget::BusesWidget(QWidget *parent)
{
	LogWidget* log_w = new LogWidget(this);


	QString ip_str = "127.0.0.1";
	int slot_port = 30001;
	int signal_port = 30002;
	Socket_RPC_SLOT_Server_Thread* rpc_slot_srv = new Socket_RPC_SLOT_Server_Thread;
	rpc_slot_srv->set_app(this);
	rpc_slot_srv->set_params(ip_str, slot_port);
	rpc_slot_srv->start();
	Socket_RPC_SIGNAL_Thread* rpc_signal_srv = new Socket_RPC_SIGNAL_Thread;
	rpc_signal_srv->set_app(this);
	rpc_signal_srv->set_params(ip_str, signal_port);
	rpc_signal_srv->start();
}

void BusesWidget::make_interrupt(int _n, short _chan, double _u, double _t)
{

}
void BusesWidget::make_message(QVariant dt, int mko, int line, int cwd, QVariantList words, int os)
{

}
void BusesWidget::make_ku(int ku_n, int length, double u, int line)
{

}
void BusesWidget::make_mk(int mshm, int pshm, int length_m, int length_p, double u_m, double u_p, int dt, int line_m, int line_p)
{

}
void BusesWidget::make_KPI(QVariantList KPI_list)
{

}