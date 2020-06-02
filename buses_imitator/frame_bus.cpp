#include "frame_bus.h"
#include "rpc_loger.h"
#include <QMessageBox>
#include <qsettings.h>
#include <qapplication.h>

#include "rpc_ports.h"
#include "frame_bus_socket_rpc.h"

FrameBusWidget::FrameBusWidget(QWidget *parent)
{
	log_widget = new LogWidget(this, "frame_bus");
	QVBoxLayout* v_lay = new QVBoxLayout(this);
	v_lay->addWidget(log_widget);

	QString ip_str = "127.0.0.1";
	int slot_port = FRAME_SLOT;
	int signal_port = FRAME_SIGNAL;
	frame_bus_Socket_RPC_SLOT_Server_Thread* rpc_slot_srv = new frame_bus_Socket_RPC_SLOT_Server_Thread;
	rpc_slot_srv->set_app(this);
	rpc_slot_srv->set_params(ip_str, slot_port);
	rpc_slot_srv->start();
	frame_bus_Socket_RPC_SIGNAL_Thread* rpc_signal_srv = new frame_bus_Socket_RPC_SIGNAL_Thread;
	rpc_signal_srv->set_app(this);
	rpc_signal_srv->set_params(ip_str, signal_port);
	rpc_signal_srv->start();

	QSettings settings(QApplication::applicationDirPath() + "/positions.ini", QSettings::IniFormat);
	restoreGeometry(settings.value("frame_geometry").toByteArray());
}

void FrameBusWidget::make_new_frame_04(QString mode, QVariant frame_data)
{
	log_widget->log_append(QString("Новый кадр %1").arg(mode));
	emit new_frame_04(mode, frame_data);
}

void FrameBusWidget::make_new_frame_733(QString mode, QVariant frame_data)
{
	emit new_frame_733(mode, frame_data);
}

void FrameBusWidget::make_new_frame_07(QString mode, int psp, int lit, int fm, QString ant, QVariant frame_data)
{
	emit new_frame_07(mode, psp, lit, fm, ant, frame_data);
}

void FrameBusWidget::make_new_frame_rm07(QString mode, QVariant frame_data)
{
	emit new_frame_rm07(mode, frame_data);
}

void FrameBusWidget::make_new_frame_n737(QString mode, QVariant frame_data)
{
	emit new_frame_n737(mode, frame_data);
} // от него беру и пердаю мки

void FrameBusWidget::closeEvent(QCloseEvent *event)
{
	QSettings settings(QApplication::applicationDirPath()+"/positions.ini", QSettings::IniFormat);
	settings.setValue("frame_geometry", saveGeometry());
	QWidget::closeEvent(event);
}