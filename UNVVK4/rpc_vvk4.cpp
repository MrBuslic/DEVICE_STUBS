#include "rpc_vvk4.h"
#include <QFile>
#include <QTextStream>
#include <QCoreApplication>
#include <QDateTime>
#include <QDir>
#include "vvk4_socket_rpc.h"
#include "rpc_ports.h"

RpcVvk4Widget::RpcVvk4Widget() : QWidget()
{
	QFormLayout* g_lay = new QFormLayout(this);
	setMinimumSize(290, 70);
	ei_chanels_lbl = new QLineEdit("нет каналов");
	ei_chanels_lbl->setReadOnly(true);
	sum_chanels_lbl = new QLineEdit("нет каналов");
	sum_chanels_lbl->setReadOnly(true);
	meas_chanels_lbl = new QLineEdit("не подключены");
	meas_chanels_lbl->setReadOnly(true);
	g_lay->addRow("EI:", ei_chanels_lbl);
	g_lay->addRow("SUM:", sum_chanels_lbl);
	g_lay->addRow("Измерительные линии:", meas_chanels_lbl);

	QString ip_str = "127.0.0.1";
	int slot_port = VVK_SLOT;
	int signal_port = VVK_SIGNAL;
	vvk4_Socket_RPC_SLOT_Server_Thread* rpc_slot_srv = new vvk4_Socket_RPC_SLOT_Server_Thread;
	rpc_slot_srv->set_app(this);
	rpc_slot_srv->set_params(ip_str, slot_port);
	rpc_slot_srv->start();
	vvk4_Socket_RPC_SIGNAL_Thread* rpc_signal_srv = new vvk4_Socket_RPC_SIGNAL_Thread;
	rpc_signal_srv->set_app(this);
	rpc_signal_srv->set_params(ip_str, signal_port);
	rpc_signal_srv->start();
}

int RpcVvk4Widget::unvvk4_commut_ListOutput(int _line, QString _masOn, QString _masOff)
{
	int res = -1;
	QList<int> on_chanels = string_to_int_list(_masOn);
	QList<int> off_chanels = string_to_int_list(_masOff);
	QMutexLocker lock(&comut_mutex);
	if (_line == CHAN::EI)
	{
		QList<int>::const_iterator on_it = on_chanels.constBegin();
		for (; on_it != on_chanels.constEnd(); ++on_it)
		{
			if (!ei_chanels_list.contains(*on_it))
				ei_chanels_list.append(*on_it);
		}
		QList<int>::const_iterator off_it = off_chanels.constBegin();
		for (; off_it != off_chanels.constEnd(); ++off_it)
		{
			if (ei_chanels_list.contains(*off_it))
				ei_chanels_list.removeAll(*off_it);
		}
		qSort(ei_chanels_list.begin(), ei_chanels_list.end());
		res = 0;
	}
	else if (_line == CHAN::SUM)
	{
		QList<int>::const_iterator on_it = on_chanels.constBegin();
		for (; on_it != on_chanels.constEnd(); ++on_it)
		{
			if (!sum_chanels_list.contains(*on_it))
				sum_chanels_list.append(*on_it);
		}
		QList<int>::const_iterator off_it = off_chanels.constBegin();
		for (; off_it != off_chanels.constEnd(); ++off_it)
		{
			if (sum_chanels_list.contains(*off_it))
				sum_chanels_list.removeAll(*off_it);
		}
		qSort(sum_chanels_list.begin(), sum_chanels_list.end());
		res = 0;
	}
	else 
	{
		SRPCSignalClass::Instance().toLog("Неверный канал");
	}
	update_view();
	return res;
}

int RpcVvk4Widget::unvvk4_config_MeasureLine(int line, int state)
{
	int res = -1;
	if (line < 0 || line >4)
	{
		SRPCSignalClass::Instance().toLog("Недопустимый канал");
		return res;
	}
	QMutexLocker lock(&meas_mutex);
	if (line == 0)
	{
		if (state == 0)
		{
			measure_line_list.clear();
			res = 0;
		}
		else if (state == 1)
		{
			measure_line_list.clear();
			measure_line_list << 1 << 2 << 3 << 4;
			res = 0;
		}
		else
		{
			SRPCSignalClass::Instance().toLog("Недопустимая команда");
		}
		return res;
	}
	if (state == 0)
	{
		if (measure_line_list.contains(line))
		{
			measure_line_list.removeAll(line);
		}
		res = 0;
	}
	else if (state == 1)
	{
		if (!measure_line_list.contains(line))
		{
			measure_line_list.append(line);
			qSort(measure_line_list.begin(), measure_line_list.end());
		}
		res = 0;
	}
	else
	{
		SRPCSignalClass::Instance().toLog("Недопустимая команда");
	}
	return res;
}

void RpcVvk4Widget::update_view()
{
	ei_chanels_lbl->setText(int_list_to_string(ei_chanels_list));
	sum_chanels_lbl->setText(int_list_to_string(sum_chanels_list));
	meas_chanels_lbl->setText(int_list_to_string(measure_line_list));
}

void RpcVvk4Widget::get_commut_chanels_list(QVariantList& ei_list, QVariantList& sum_list)
{
	QMutexLocker lock(&comut_mutex);
	qCopy(ei_chanels_list.begin(), ei_chanels_list.end(), ei_list.begin());
	qCopy(sum_chanels_list.begin(), sum_chanels_list.end(), sum_list.begin());
}

void RpcVvk4Widget::get_measure_lines(QVariantList& mes_list)
{
	QMutexLocker lock(&meas_mutex);
	qCopy(measure_line_list.begin(), measure_line_list.end(), mes_list.begin());
}

QList<int> RpcVvk4Widget::string_to_int_list(QString chanels)
{
	bool ok = false;
	QList<int> tmp_chanels_list;
	chanels.remove(' ');
	QStringList tmp_str_list = chanels.split(',');
	QStringList::const_iterator it = tmp_str_list.constBegin();
	for (; it != tmp_str_list.constEnd(); ++it)
	{
		QString tmp_str = *it;
		if (tmp_str.contains('-'))
		{
			QStringList tmp_interval_list = tmp_str.split('-');
			int begin_interval = tmp_interval_list.at(0).toInt(&ok);
			if (!ok || begin_interval < 1 || begin_interval > 200)
				continue;
			int end_interval = tmp_interval_list.at(1).toInt(&ok);
			if (!ok || end_interval < 1 || end_interval > 200)
				continue;
			if (begin_interval > end_interval)
			{
				int i = begin_interval;
				begin_interval = end_interval;
				end_interval = i;
			}
			for (int i = begin_interval; i < end_interval + 1; i++)
			{
				if (!tmp_chanels_list.contains(i))
					tmp_chanels_list << i;
			}
		}
		else
		{
			int tmp_int = tmp_str.toInt(&ok);
			if (!ok || tmp_int < 1 || tmp_int > 200)
				continue;
			if (!tmp_chanels_list.contains(tmp_int))
				tmp_chanels_list << tmp_int;

		}
	}
	return tmp_chanels_list;
}

QString RpcVvk4Widget::int_list_to_string(QList<int> chanels_list)
{
	QString res = "нет каналов";
	if (chanels_list.size() > 0)
	{
		int previous = -1;
		bool is_interval = false;
		QList<int>::const_iterator it = chanels_list.constBegin();
		for (; it != chanels_list.constEnd(); ++it)
		{
			if (*it - 1 == previous)
			{
				if (is_interval)
				{
					previous = *it;
				}
				else
				{
					is_interval = true;
					previous = *it;
				}
			}
			else
			{
				if (is_interval)
				{
					res += QString("-%1, %2").arg(previous).arg(*it);
				}
				else
				{
					if (previous == -1)
						res = QString("%1").arg(*it);
					else
						res += QString(", %1").arg(*it);
				}
				previous = *it;
				is_interval = false;
			}
		}
	}
	return res;
}