#include <QTextCodec>
#include <QApplication>
#include "RTM3002_imitator.h"
#include <socket_rpc.h>

#ifdef WIN32
#include "windows.h"
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
	RTM3002_imitator RTM3002;
	RTM3002.show();

	return app.exec();
}