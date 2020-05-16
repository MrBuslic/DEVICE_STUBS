#include "cbk_imitator.h"
#include <QTextCodec>
#include <socket_rpc.h>
extern void start_core();
#ifdef WIN32
int __stdcall WinMain(HINSTANCE, HINSTANCE, LPSTR cmdLine, int)
{
	int argc = __argc;
	char** argv = __argv;

#else
int main(int argc, char **argv)
{
#endif
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf-8"));
	//QTextCodec::setCodecForLocale(QTextCodec::codecForName("Windows-1251"));
	QApplication app(argc, argv);
	LogWidget log_widget;
	log_widget.show();
	start_core();
	//CBK_MainWindow CBK;
	//CBK.show();
	return app.exec();
}