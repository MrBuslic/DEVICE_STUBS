#include "mku_bus.h"
#include "rpc_loger.h"
#include <QMessageBox>

#include "rpc_ports.h"
#include "mku_bus_socket_rpc.h"

MKUWidget::MKUWidget(QWidget *parent)
{
	LogWidget* log_w = new LogWidget(this);
	for (int i = 1; i <= 8; i++) ku_map_channels[i] = 3;
	for (int i = 1; i <= 20; i++) mshm_map_channels[i] = 3;
	for (int i = 1; i <= 20; i++) pshm_map_channels[i] = 3;
	
	QString ip_str = "127.0.0.1";
	int slot_port = MKU_SLOT;
	int signal_port = MKU_SIGNAL;
	mku_bus_Socket_RPC_SLOT_Server_Thread* rpc_slot_srv = new mku_bus_Socket_RPC_SLOT_Server_Thread;
	rpc_slot_srv->set_app(this);
	rpc_slot_srv->set_params(ip_str, slot_port);
	rpc_slot_srv->start();
	mku_bus_Socket_RPC_SIGNAL_Thread* rpc_signal_srv = new mku_bus_Socket_RPC_SIGNAL_Thread;
	rpc_signal_srv->set_app(this);
	rpc_signal_srv->set_params(ip_str, signal_port);
	rpc_signal_srv->start();
}

void MKUWidget::make_ku(int ku_n, int length, double u, int line)
{
	emit new_ku(ku_n, length, u, line);
}

void MKUWidget::make_ku_732(int ku_n, int length, double u, int line)
{
	emit new_ku_732(ku_n, length, u, line);
}

void MKUWidget::make_mk(int mshm, int pshm, int length_m, int length_p, double u_m, double u_p, int dt, int line_m, int line_p)
{
	emit new_mk(mshm, pshm, length_m, length_p, u_m, u_p, dt, line_m, line_p);
}

int MKUWidget::ku_map_channels_setup(int ku_n, int line)
{
	ku_map_channels[ku_n] = line;
	SRPCSignalClass::Instance().toLog(QString("Присваиваю каналу %1 значение %2").arg(ku_n).arg(line));
	return 0;
}

int MKUWidget::mshm_map_channels_setup(int mshm, int line_m)
{
	mshm_map_channels[mshm] = line_m;
	SRPCSignalClass::Instance().toLog(QString("Присваиваю каналу %1 значение %2").arg(mshm).arg(line_m));
	return 0;
}

int MKUWidget::pshm_map_channels_setup(int pshm, int line_p)
{
	pshm_map_channels[pshm] = line_p;
	SRPCSignalClass::Instance().toLog(QString("Присваиваю каналу %1 значение %2").arg(pshm).arg(line_p));
	return 0;
}