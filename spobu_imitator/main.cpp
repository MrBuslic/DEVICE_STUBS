#include "spobu_imit.h"
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
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf-8"));
	QApplication app(argc, argv);

	SPOBU_Widget spobu;
	spobu.show();
	return app.exec();
}