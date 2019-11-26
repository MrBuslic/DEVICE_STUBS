#include "kpi_bus.h"
#include "rpc_loger.h"
#include <QMessageBox>

#include "rpc_ports.h"
#include "kpi_bus_socket_rpc.h"

KPIWidget::KPIWidget(QWidget *parent)
{
	log_widget = new LogWidget(this, "kpi_bus");
	QVBoxLayout* v_lay = new QVBoxLayout(this);
	v_lay->addWidget(log_widget);


	QString ip_str = "127.0.0.1";
	int slot_port = KPI_SLOT;
	int signal_port = KPI_SIGNAL;
	kpi_bus_Socket_RPC_SLOT_Server_Thread* rpc_slot_srv = new kpi_bus_Socket_RPC_SLOT_Server_Thread;
	rpc_slot_srv->set_app(this);
	rpc_slot_srv->set_params(ip_str, slot_port);
	rpc_slot_srv->start();
	kpi_bus_Socket_RPC_SIGNAL_Thread* rpc_signal_srv = new kpi_bus_Socket_RPC_SIGNAL_Thread;
	rpc_signal_srv->set_app(this);
	rpc_signal_srv->set_params(ip_str, signal_port);
	rpc_signal_srv->start();
}

void KPIWidget::make_KPI(QVariantList KPI_list)
{
	log_widget->log_append(QString("Новая КПИ"));
	emit new_KPI(KPI_list);
}