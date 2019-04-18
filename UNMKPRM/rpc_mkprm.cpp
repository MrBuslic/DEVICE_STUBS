#include "rpc_mkprm.h"
#include <QFile>
#include <QTextStream>
#include <QCoreApplication>
#include <QDateTime>
#include <QDir>
#include "rpc_ports.h"
#include "mkprm_socket_rpc.h"
#include "frameSettings.hpp"

#include <qmessagebox.h>

RpcMKPRMWidget::RpcMKPRMWidget() : QWidget()
{
	LogWidget* log_widg = new LogWidget(this);

	QString ip_str = "127.0.0.1";
	int slot_port = MKPRM_SLOT;
	int signal_port = MKPRM_SIGNAL;
	Socket_RPC_SLOT_Server_Thread* rpc_slot_srv = new Socket_RPC_SLOT_Server_Thread;
	rpc_slot_srv->set_app(this);
	rpc_slot_srv->set_params(ip_str, slot_port);
	rpc_slot_srv->start();
	Socket_RPC_SIGNAL_Thread* rpc_signal_srv = new Socket_RPC_SIGNAL_Thread;
	rpc_signal_srv->set_app(this);
	rpc_signal_srv->set_params(ip_str, signal_port);
	rpc_signal_srv->start();


	frame_slot_thr.set_connection_params("127.0.0.1", FRAME_SLOT);
	frame_slot_thr.start(); // вот тут падает

	frame_signal_thr.set_connection_params("127.0.0.1", FRAME_SIGNAL);
	frame_signal_thr.start(); // вот тут падает

	if (!frame_slot_thr.wait_connected(3) || !frame_signal_thr.wait_connected(3))
	{
		QMessageBox::critical(0, "Нет соединения", "Ошибка соединения с frame_bus в Mkprm");
		this->deleteLater();
		return;
	}

	connect(frame_signal_thr.get_obj().get(), SIGNAL(new_frame(QString, QVariant)), this, SLOT(new_frame(QString, QVariant)));

	params_mode_map = fp.getParams();

}



int RpcMKPRMWidget::unmkprm_get_strings(int strings, QVariantList& string_data)
{
	int tmp_strings = strings;
	if (frame_data.size() < strings)
		tmp_strings = frame_data.size();
	string_data = frame_data.mid(0, tmp_strings);
	for (int i = 0; i < tmp_strings; i++)
		frame_data.pop_front();
	return 0;
}


void RpcMKPRMWidget::new_frame(QString mode, QVariant _frame_data)
{
	QByteArray& frame_arr = _frame_data.toByteArray();

	const QList<int>& par_rez = params_mode_map[mode];

	QByteArray tmp_str_num_arr;
	tmp_str_num_arr.resize(2);

	for (int i = 0; i < par_rez[0]; i++)
	{
		tmp_str_num_arr[0] = i+2;
		tmp_str_num_arr[1] = 0;
		QByteArray tmp_arr = frame_arr.mid(i* (par_rez[1] + par_rez[26]) + par_rez[26], par_rez[1]);
		frame_data << QVariant(tmp_str_num_arr + tmp_arr);
	}
	//frame_data

	//frame_data = _frame_data;
}

