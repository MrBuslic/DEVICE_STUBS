#include "mku_bus.h"
#include "rpc_loger.h"
#include <QMessageBox>

#include "rpc_ports.h"
#include "mku_bus_socket_rpc.h"

MKUWidget::MKUWidget(QWidget *parent)
{
	log_widget = new LogWidget(this, "mku_bus");
	QVBoxLayout* v_lay = new QVBoxLayout(this);
	v_lay->addWidget(log_widget);

	for (int i = 0; i <= 8; i++) ku_map_channels[i] = 3;
	for (int i = 0; i <= 12; i++) mshm_map_channels[i] = 3;
	for (int i = 0; i <= 12; i++) pshm_map_channels[i] = 3;
	
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

QString MKUWidget::get_line(int line)
{
	switch (line)
	{
	case 0: return "Обрыв";
	case 1: return "О";
	case 2: return "Р";
	case 3: return "ОР";
	};
	return "";
}

void MKUWidget::make_ku(int ku_n, int length, double u, int line)
{
	line &= ku_map_channels[ku_n];
	log_widget->log_append(QString("КУ %1 %2").arg(ku_n).arg(get_line(line)));
	emit new_ku(ku_n, length, u, line);
}

void MKUWidget::make_ku_732(int ku_n, int length, double u, int line)
{
	log_widget->log_append(QString("КУ 732 %1").arg(ku_n));
	emit new_ku_732(ku_n, length, u, line);
}

void MKUWidget::make_ku_cbk(int ku_n, int length, double u, int line)
{
	log_widget->log_append(QString("КУ ЦБК %1").arg(ku_n));
	emit new_ku_cbk(ku_n, length, u, line);
}

void MKUWidget::make_mk(int mshm, int pshm, int length_m, int length_p, double u_m, double u_p, int dt, int line_m, int line_p)
{
	line_m &= mshm_map_channels[mshm];
	line_p &= pshm_map_channels[pshm];
	log_widget->log_append(QString("МК МШМ%1 %2 ПШМ%3 %4").arg(mshm).arg(get_line(line_m)).arg(pshm).arg(get_line(line_p)));
	emit new_mk(mshm, pshm, length_m, length_p, u_m, u_p, dt, line_m, line_p);
}

int MKUWidget::ku_map_channels_setup(int ku_n, int line)
{
	ku_map_channels[ku_n] = line;
	log_widget->log_append(QString("На шине КУ присваиваю каналу %1 значение %2").arg(ku_n).arg(line));
	return 0;
}

int MKUWidget::mshm_map_channels_setup(int mshm, int line_m)
{
	mshm_map_channels[mshm] = line_m;
	log_widget->log_append(QString("На шине МШМ присваиваю каналу %1 значение %2").arg(mshm).arg(line_m));
	return 0;
}

int MKUWidget::pshm_map_channels_setup(int pshm, int line_p)
{
	pshm_map_channels[pshm] = line_p;
	log_widget->log_append(QString("На шине ПШМ присваиваю каналу %1 значение %2").arg(pshm).arg(line_p));
	return 0;
}

void MKUWidget::get_tm(QString tm_name, QVariant& tm_val)
{
	QMap<QString, QVariant>::iterator tm_itr = tm_map.find(tm_name);
	if (tm_itr == tm_map.end())
	{
		tm_val = QVariant();
		log_widget->log_append(QString("Попытка считывания отсутствующего синала %1.").arg(tm_name));
	}
	else
	{
		tm_val = *tm_itr;
		log_widget->log_append(QString("Считан сигнал %1. Значение: %2").arg(tm_name).arg(RPCSignalClass::QVariantToString(tm_val)));
	}
}

void MKUWidget::set_tm(QString tm_name, QVariant tm_val)
{
	tm_map[tm_name] = tm_val;
	log_widget->log_append(QString("Присваиваю сигналу %1 значение %2").arg(tm_name).arg(RPCSignalClass::QVariantToString(tm_val)));
}
