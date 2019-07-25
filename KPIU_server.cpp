#include "KPIU_server.h"
#include "socket_rpc.h"
#include "rpc_ports.h"
#include "kpiu_socket_rpc.h"

KPIUServer::KPIUServer(QWidget* parent) : QWidget(parent)
{
	QString ip_str = "127.0.0.1";
	QGridLayout* bus_grd_layout = new QGridLayout(this);
	bus_grd_layout->setVerticalSpacing(10);
	bus_grd_layout->setHorizontalSpacing(10);
	omnibus_widget = new RpcOmnibusWidget;
	interrupt_widget = new InterruptWidget;
	kpi_widget = new KPIWidget;
	mku_widget = new MKUWidget;
	frame_widget = new FrameBusWidget;
	power_widget = new PowerWidget;
	bus_grd_layout->addWidget(omnibus_widget, 0, 0);
	bus_grd_layout->addWidget(interrupt_widget, 0, 1);
	bus_grd_layout->addWidget(kpi_widget, 0, 2);
	bus_grd_layout->addWidget(mku_widget, 1, 0);
	bus_grd_layout->addWidget(frame_widget, 1, 1);
	bus_grd_layout->addWidget(power_widget, 1, 2, Qt::AlignTop);
	kpiu_Socket_RPC_SLOT_Server_Thread* rpc_slot_srv = new kpiu_Socket_RPC_SLOT_Server_Thread;
	rpc_slot_srv->set_app(this);

	rpc_slot_srv->set_params(ip_str, KPIU_SERVER_SLOT);
	rpc_slot_srv->start();
	kpiu_Socket_RPC_SIGNAL_Thread* rpc_signal_srv = new kpiu_Socket_RPC_SIGNAL_Thread;
	rpc_signal_srv->set_app(this);
	rpc_signal_srv->set_params(ip_str, KPIU_SERVER_SIGNAL);
	rpc_signal_srv->start();
}
//mku_bus_setup
int KPIUServer::ku_map_channels_setup(int ku_n, short line)
{
	mku_widget->ku_map_channels_setup(ku_n, line);
	return 0;
}
int KPIUServer::mshm_map_channels_setup(int mshm, short line_m)
{
	mku_widget->mshm_map_channels_setup(mshm, line_m);
	return 0;
}
int KPIUServer::pshm_map_channels_setup(int pshm, short line_p)
{
	mku_widget->pshm_map_channels_setup(pshm, line_p);
	return 0;
}

//unfoi_setup
int KPIUServer::unfoi_map_channels_setup(int _n, short _chan)
{
	foi_widget->unfoi_map_channels_setup(_n, _chan);
	return 0;
}

//unols_setup
int KPIUServer::unols_write_data_kf(QVariantList data_buffer, QVariantList mask_buffer)
{
	ols_widget->unols_write_data_kf(data_buffer, mask_buffer);
	return 0;
}

int KPIUServer::unols_trigger_imm(int devise)
{
	ols_widget->unols_trigger_imm(devise);
	return 0;
}

void KPIUServer::unols_read_data_kr(QVariantList& data_buffer)
{
	ols_widget->unols_read_data_kr(data_buffer);
}

//KPRD_setup
void KPIUServer::set_antenna_connection(QString antenna_name, QString connected_antenna_name)
{
	kprd_widget->set_antenna_connection(antenna_name, connected_antenna_name);
}

//OMNIBUS_setup
int KPIUServer::unomnibus_map_channels_setup(int _n, short _chan)
{
	omnibus_widget->unomnibus_map_channels_setup(_n, _chan);
	return 0;
}

//power_bus_setup
void KPIUServer::set_bus_state(int bus, int state)
{
	power_widget->set_bus_state(bus, state);
}