#include <QTextCodec>
#include <QApplication>
#include "MKI_imitator.h"

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
	MKI_imitator MKI;
	MKI.show();

	return app.exec();
}