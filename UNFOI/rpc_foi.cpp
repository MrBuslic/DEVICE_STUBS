#include "rpc_foi.h"
#include <QFile>
#include <QTextStream>
#include <QCoreApplication>
#include <QDateTime>
#include <QDir>
#include "rpc_ports.h"
#include "foi_socket_rpc.h"

#include <qmessagebox.h>

RpcFoiWidget::RpcFoiWidget() : QWidget()
{
	LogWidget* log_widg = new LogWidget(this);

	QString ip_str = "127.0.0.1";
	int slot_port = FOI_SLOT;
	int signal_port = FOI_SIGNAL;
	Socket_RPC_SLOT_Server_Thread* rpc_slot_srv = new Socket_RPC_SLOT_Server_Thread;
	rpc_slot_srv->set_app(this);
	rpc_slot_srv->set_params(ip_str, slot_port);
	rpc_slot_srv->start();
	Socket_RPC_SIGNAL_Thread* rpc_signal_srv = new Socket_RPC_SIGNAL_Thread;
	rpc_signal_srv->set_app(this);
	rpc_signal_srv->set_params(ip_str, signal_port);
	rpc_signal_srv->start();


	interrupt_slot_thr.set_connection_params("127.0.0.1", INTERRUPTS_SLOT);
	interrupt_slot_thr.start(); // вот тут падает

	interrupt_signal_thr.set_connection_params("127.0.0.1", INTERRUPTS_SIGNAL);
	interrupt_signal_thr.start(); // вот тут падает

	if (!interrupt_slot_thr.wait_connected(3) || !interrupt_signal_thr.wait_connected(3))
	{
		QMessageBox::critical(0, "Нет соединения", "Ошибка соединения с interrupt_bus в Foi");
		this->deleteLater();
		return;
	}
}

int RpcFoiWidget::unfoi_chan_setup(int _n, short _chan, double _u, double _t)
{
	n = _n;
	chan = _chan;
	u = _u;
	t = _t;
	return 0;
}

int RpcFoiWidget::unfoi_run()
{
	SRPCSignalClass::Instance().toLog(QString("Выдаю сигнал на канале %1 линии %2 с амплитудой %3 и длительностью %4").arg(n).arg(chan).arg(u).arg(t));

	interrupt_slot_thr.get_interrupt_bus_obj()->make_interrupt(n, chan, u, t);

	return 0;
}