#ifdef __cplusplus
extern "C"
{
#endif
#include "MainUnit.h"
#ifdef __cplusplus
}
#endif
#include <QApplication>
#include "MY_BOS\my_thread.h"

int main(int argc, char** argv)
{
	QApplication app(argc, argv);
	MyThread tmp_thread;
	TimeThread& b_time(STimeThread::Instance());
	b_time.start();
	tmp_thread.start();
	return app.exec();
	
}
