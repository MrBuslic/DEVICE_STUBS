#include "rpc_mt8k4l.h"
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
	if (argc < 3)
		return 1;
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
	QApplication app(argc, argv);
	LogWidget log_widget;
	log_widget.show();
	RpcMT8K4LWidget mt8k4l_widget(QString(argv[1]).toInt(), QString(argv[2]).toInt());
	mt8k4l_widget.show();
	return app.exec();
}
