#include "rpc_ads128.h"
#include <QApplication>
#include <QTextCodec>
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
	//if (argc < 3)
	//	return 1;
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
	QApplication app(argc, argv);
	LogWidget log_widget;
	log_widget.show();
	RpcADS128Widget* ads128_widget;
	if (argc < 2)
	{
		ads128_widget = new RpcADS128Widget(0);
	}
	else
	{
		ads128_widget = new RpcADS128Widget(QString(argv[1]).toInt());
	}

	ads128_widget->show();
	return app.exec();
}
