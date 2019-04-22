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

//мй - лай-07, лай-04, лай-02, асо
//й1 - кйю-05, юям, ащв, жай
//й2 - 14п733, 14п732


void PowerWidget::set_u(int bus, double volt) //
{
	switch (bus)
	{
	case NK:
		nk_volt = volt;
		emit u_on_nk(volt);
		break;
	case K1:
		k1_volt = volt;
		emit u_on_k1(volt);
		break;
	case K2:
		k2_volt = volt;
		emit u_on_k2(volt);
		break;
	}
}

void PowerWidget::get_i(int bus, double& curr)
{
	double tmp_curr = 0;
	switch (bus)
	{
	case NK:
		for (QMap<QString, double>::iterator itr = nk_curr_map.begin(); itr != nk_curr_map.end(); itr++)
		{
			tmp_curr += itr.value();
		}
		curr = tmp_curr;
		nk_curr = tmp_curr;
		break;
	case K1:
		for (QMap<QString, double>::iterator itr = k1_curr_map.begin(); itr != k1_curr_map.end(); itr++)
		{
			tmp_curr += itr.value();
		}
		curr = tmp_curr;
		k1_curr = tmp_curr;
		break;
	case K2:
		for (QMap<QString, double>::iterator itr = k2_curr_map.begin(); itr != k2_curr_map.end(); itr++)
		{
			tmp_curr += itr.value();
		}
		curr = tmp_curr;
		k2_curr = tmp_curr;
		break;
	}
}

void PowerWidget::set_i(int bus, QString name, double curr)
{
	switch (bus)
	{
	case NK:
		nk_curr_map[name] = curr;
		break;
	case K1:
		k1_curr_map[name] = curr;
		break;
	case K2:
		k2_curr_map[name] = curr;
		break;

	}
}