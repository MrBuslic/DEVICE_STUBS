#include "rpc_is4.h"
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
	RpcIS4Widget* is4_widget;
	if (argc < 2)
	{
		is4_widget = new RpcIS4Widget(0);
	}
	else
	{
		is4_widget = new RpcIS4Widget(QString(argv[1]).toInt());
	}

	is4_widget->show();
	return app.exec();
}
