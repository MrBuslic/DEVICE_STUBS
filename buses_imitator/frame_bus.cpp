#include "frame_bus.h"
#include "rpc_loger.h"
#include <QMessageBox>

#include "rpc_ports.h"
#include "frame_bus_socket_rpc.h"

FrameBusWidget::FrameBusWidget(QWidget *parent)
{
	LogWidget* log_w = new LogWidget(this);

	QString ip_str = "127.0.0.1";
	int slot_port = FRAME_SLOT;
	int signal_port = FRAME_SIGNAL;
	Socket_RPC_SLOT_Server_Thread* rpc_slot_srv = new Socket_RPC_SLOT_Server_Thread;
	rpc_slot_srv->set_app(this);
	rpc_slot_srv->set_params(ip_str, slot_port);
	rpc_slot_srv->start();
	Socket_RPC_SIGNAL_Thread* rpc_signal_srv = new Socket_RPC_SIGNAL_Thread;
	rpc_signal_srv->set_app(this);
	rpc_signal_srv->set_params(ip_str, signal_port);
	rpc_signal_srv->start();
}

void FrameBusWidget::make_new_frame(QString mode, QVariant frame_data)
{
	emit new_frame(mode, frame_data);
}
