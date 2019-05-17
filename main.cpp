#include "OMNIBUSBOX.h"
#include "kpi_bus.h"
#include "mku_bus.h"
#include "interrupt_bus.h"
#include "frame_bus.h"
#include "power_bus.h"
#include <QApplication>
#include <QTextCodec>
#include <QWidget>
#include <socket_rpc.h>

#ifdef WIN32

int __stdcall WinMain(HINSTANCE, HINSTANCE, LPSTR cmdLine, int)
{
	int argc = __argc;
	char** argv = __argv;

#else
int main(int argc, char **argv)
{
#endif

	QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
	QApplication app(argc, argv);
	LogWidget log_widget;
	log_widget.show();
	QWidget bus_widget;//todo выбрать верный тип

	RpcOmnibusWidget* omnibus_widget = new RpcOmnibusWidget;
	InterruptWidget* interrupt_widget = new InterruptWidget;
	KPIWidget* kpi_widget = new KPIWidget;
	MKUWidget* mku_widget = new MKUWidget;
	FrameBusWidget* frame_widget = new FrameBusWidget;
	PowerWidget* power_widget = new PowerWidget;

	QGridLayout* bus_grd_layout = new QGridLayout;
	bus_grd_layout->addWidget(omnibus_widget, 0, 0);
	bus_grd_layout->addWidget(interrupt_widget, 0, 1);
	bus_grd_layout->addWidget(kpi_widget, 0, 2);
	bus_grd_layout->addWidget(mku_widget, 1, 0);
	bus_grd_layout->addWidget(frame_widget, 1, 1);
	bus_grd_layout->addWidget(power_widget, 1, 2);
	bus_widget.setLayout(bus_grd_layout);

	bus_widget.show();
	return app.exec();
}
