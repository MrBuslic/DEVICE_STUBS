#include "frame_bus.h"
#include <QApplication>
#include <QTextCodec>


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
    QApplication app(argc, argv);
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
	LogWidget log_w;
	log_w.show();

	FrameBusWidget frame_widget;
	frame_widget.show();

    return app.exec();
}
