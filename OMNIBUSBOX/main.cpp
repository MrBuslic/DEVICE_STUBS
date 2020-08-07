#include "OMNIBUSBOX.h"
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

	QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
	QString mode = "";
	if (argc > 1)
		if (QString(argv[1]) == "iksa")
			mode = QString(argv[1]);
	QApplication app(argc, argv);
	LogWidget log_widget;
	log_widget.show();
	RpcOmnibusWidget omnibus_widget(0, mode);
	omnibus_widget.show();
	return app.exec();
}
