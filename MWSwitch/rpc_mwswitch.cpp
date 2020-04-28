#include "rpc_mwswitch.h"
#include <QFile>
#include <QTextStream>
#include <QCoreApplication>
#include <QDateTime>
#include <QDir>
#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include "mwswitch_socket_rpc.h"
#include <bitset>
#include "rpc_ports.h"


RpcMWSwitchWidget::RpcMWSwitchWidget(int switch_num) : QWidget()
{
	QVBoxLayout* v_lay = new QVBoxLayout(this);
	
	log_widget = new LogWidget(this, QString("mwswitch_%1").arg(switch_num));
	v_lay->addWidget(log_widget);


	QString ip_str = "127.0.0.1";
	mwswitch_Socket_RPC_SLOT_Server_Thread* rpc_slot_srv = new mwswitch_Socket_RPC_SLOT_Server_Thread;
	rpc_slot_srv->set_app(this);
	rpc_slot_srv->set_params(ip_str, MWSWITCH_SLOT+ switch_num);
	rpc_slot_srv->start();
	mwswitch_Socket_RPC_SIGNAL_Thread* rpc_signal_srv = new mwswitch_Socket_RPC_SIGNAL_Thread;
	rpc_signal_srv->set_app(this);
	rpc_signal_srv->set_params(ip_str, MWSWITCH_SIGNAL+ switch_num);
	rpc_signal_srv->start();
	setWindowTitle(QString("mwswitch_ %1").arg(switch_num));
}
