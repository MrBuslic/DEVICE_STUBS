#include "OMNIBUSBOX.h"
#include "KPIU_server.h"
#include "kpi_bus.h"
#include "mku_bus.h"
#include "interrupt_bus.h"
#include "frame_bus.h"
#include "power_bus.h"
#include "LKA05.h"
#include <QApplication>
#include <QTextCodec>
#include <QWidget>
#include <socket_rpc.h>

#include "UNOLS\unols_h.h"

int main(int argc, char **argv)
{
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
	QApplication app(argc, argv);
	LogWidget log_widget;
	log_widget.show();

	KPIUServer kpiu_server;
	kpiu_server.show();
	return app.exec();
}
