#include "power_bus.h"
#include "rpc_loger.h"
#include <QMessageBox>
#include <qsettings.h>
#include <qapplication.h>

#include "rpc_ports.h"
#include "power_bus_socket_rpc.h"

PowerWidget::PowerWidget(QWidget *parent)
{
	//LogWidget* log_w = new LogWidget();
	//log_w->show();
	//setFixedSize(250, 80);
	for (int i = 1; i <= 3; i++) power_bus_state_map[i] = 1;

	QString ip_str = "127.0.0.1";
	int slot_port = POWER_SLOT;
	int signal_port = POWER_SIGNAL;
	power_bus_Socket_RPC_SLOT_Server_Thread* rpc_slot_srv = new power_bus_Socket_RPC_SLOT_Server_Thread;
	rpc_slot_srv->set_app(this);
	rpc_slot_srv->set_params(ip_str, slot_port);
	rpc_slot_srv->start();
	power_bus_Socket_RPC_SIGNAL_Thread* rpc_signal_srv = new power_bus_Socket_RPC_SIGNAL_Thread;
	rpc_signal_srv->set_app(this);
	rpc_signal_srv->set_params(ip_str, signal_port);
	rpc_signal_srv->start();

	on_btn = new QPushButton("Вкл", this);
	on_btn->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed));
	off_btn = new QPushButton("Откл", this);
	off_btn->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed));
	QVBoxLayout* lay = new QVBoxLayout(this);
	lay->addStretch(10);
	lay->addWidget(on_btn);
	lay->addWidget(off_btn);
	connect(on_btn, &QPushButton::clicked, this, &PowerWidget::set_on);
	connect(off_btn, &QPushButton::clicked, this, &PowerWidget::set_off);

	QSettings settings(QApplication::applicationDirPath() + "/positions.ini", QSettings::IniFormat);
	restoreGeometry(settings.value("power_geometry").toByteArray());
}

//НК - МБК-07, МБК-04, МБК-02, БУП
//К1 - ЛКА-05, АСН, БЭЧ, ЦБК
//К2 - 14Р733, 14Р732


void PowerWidget::set_u(int bus, double volt) //
{
	if (!power_bus_state_map[bus]) volt = 0;
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
	if (!power_bus_state_map[bus])
	{
		curr = 0;
		return;
	}

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

void PowerWidget::set_bus_state(int bus, int state)
{
	power_bus_state_map[bus] = state;
	SRPCSignalClass::Instance().toLog(QString("Присваиваю каналу шины %1 значение %2").arg(bus).arg(state));
}

void PowerWidget::set_on()
{
	for (int i = 1; i < 4; i++)
	{
		set_u(i, 27);
	}
}

void PowerWidget::set_off()
{
	for (int i = 1; i < 4; i++)
	{
		set_u(i, 0);
	}
}
void PowerWidget::closeEvent(QCloseEvent *event)
{
	QSettings settings(QApplication::applicationDirPath() + "/positions.ini", QSettings::IniFormat);
	settings.setValue("power_geometry", saveGeometry());
	QWidget::closeEvent(event);
}