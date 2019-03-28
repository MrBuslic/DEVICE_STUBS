#include "rpc_mn8i.h"
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
	RpcMN8IWidget* mn8i_widget;
	if (argc < 2)
	{
		mn8i_widget = new RpcMN8IWidget(0);
	}
	else
	{
		mn8i_widget = new RpcMN8IWidget(QString(argv[1]).toInt());
	}

	mn8i_widget->show();
	return app.exec();
}
