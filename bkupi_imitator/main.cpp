#include "bkupi.h"
#include <QApplication>
#include "rpc_loger.h"

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
    QApplication a(argc, argv);

	LogWidget log_w;
	log_w.show();

	BKUPI_widg w;
    w.show();

    return a.exec();
}
