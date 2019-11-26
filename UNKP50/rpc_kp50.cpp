#include "rpc_kp50.h"
#include <QFile>
#include <QTextStream>
#include <QCoreApplication>
#include <QDateTime>
#include <QDir>
#include "kp50_socket_rpc.h"
#include "rpc_ports.h"
#include <qmessagebox.h>

RpcKP50Widget::RpcKP50Widget() : QWidget(), u_in(0)
{
	QVBoxLayout* v_lay = new QVBoxLayout(this);
	log_widget = new LogWidget(this, "kp50");
	v_lay->addWidget(log_widget);

	chans_states.insert(KPCHANNEL_1, false);
	chans_states.insert(KPCHANNEL_2, false);
	chans_states.insert(KPCHANNEL_3, false);

	chan_names.insert(KPCHANNEL_1, "НК");
	chan_names.insert(KPCHANNEL_2, "К1");
	chan_names.insert(KPCHANNEL_3, "К2");

	power_slot_thr.set_connection_params("127.0.0.1", POWER_SLOT);
	power_slot_thr.start(); 

	power_signal_thr.set_connection_params("127.0.0.1", POWER_SIGNAL);
	power_signal_thr.start(); 

	if (!power_slot_thr.wait_connected(3) || !power_signal_thr.wait_connected(3))
	{
		QMessageBox::critical(0, "Нет соединения", "Ошибка соединения с power_bus");
		this->deleteLater();
		return;
	}

	QString ip_str = "127.0.0.1";
	int slot_port = KP50_SLOT;
	int signal_port = KP50_SIGNAL;
	kp50_Socket_RPC_SLOT_Server_Thread* rpc_slot_srv = new kp50_Socket_RPC_SLOT_Server_Thread;
	rpc_slot_srv->set_app(this);
	rpc_slot_srv->set_params(ip_str, slot_port);
	rpc_slot_srv->start();
	kp50_Socket_RPC_SIGNAL_Thread* rpc_signal_srv = new kp50_Socket_RPC_SIGNAL_Thread;
	rpc_signal_srv->set_app(this);
	rpc_signal_srv->set_params(ip_str, signal_port);
	rpc_signal_srv->start();
}

int RpcKP50Widget::unkp50_switch_channel(int n, bool on)
{
	if (n == 0)
		for (int i = KPCHANNEL_1; i < KPLAST; i++)
			switch_channel(i, on);
	else
		switch_channel(n, on);
	return 0;
}

void RpcKP50Widget::set_u_in(double _u)
{
	log_widget->log_append(QString("Установлено входное напряжение %1В").arg(_u));
	u_in = _u;
	for (int i = KPCHANNEL_1; i < KPLAST; i++)
	{
		if (chans_states[i])
			power_slot_thr.get_power_bus_obj()->set_u(i, u_in);
	}
}

void RpcKP50Widget::switch_channel(int n, bool on)
{
	chans_states[n] = on;
	log_widget->log_append(QString("%1 шина %2 %3").arg(QTime::currentTime().toString("hh:mm:ss.zzz")).arg(chan_names[n]).arg(on ? "подключена" : "отключена"));
	power_slot_thr.get_power_bus_obj()->set_u(n, on ? u_in : 0);

}

bool RpcKP50Widget::unkp50_channel_state_Q(int n)
{
	return chans_states[n];
}

double RpcKP50Widget::unkp50_meas_I(int n)
{
	double curr = 0.0;
	power_slot_thr.get_power_bus_obj()->get_i(n, curr);
	return curr;
}

double RpcKP50Widget::unkp50_meas_Uin(int n)
{
	return u_in;
}

double RpcKP50Widget::unkp50_meas_Uout(int n)
{
	return chans_states[n] ? u_in : 0;
}
