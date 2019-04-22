#include "SORENSEN.h"
#include <QApplication>
#include <QTextCodec>

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
	QApplication app(argc, argv);
	SORENSENWidget _widget;
	_widget.show();
	return app.exec();
}
