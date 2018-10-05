#ifndef RPC_FOI_H
#define RPC_FOI_H
#ifdef WIN32
#include "winsock2.h"
#endif
#include <QObject>
#include <QString>
#include <QWidget>
#include <QThread>
#include <QTextEdit>
#include <QScrollBar>
#include <QCheckBox>
#include <QTimer>
#include <QMutex>
#include <memory>
#include <qlayout.h>
#include <loki/Singleton.h>

#define SINGLETON_DEF(x) typedef Loki::SingletonHolder<x,Loki::CreateUsingNew,Loki::NoDestroy> S##x;

class RpcFoiWidget : public QWidget
{
	Q_OBJECT
public:
	RpcFoiWidget();
public slots:

	void auto_scroll_clicked(int _state);
	void log_timer_ontimer();

	int unfoi_chan_setup(int _n, short _chan, double _u, double _t);
	int unfoi_run();
private:
	QTextEdit* edit;
	QScrollBar* _scroll_bar;
	QTextDocument* _doc;
	QTextCursor* _cursor;
	QCheckBox* auto_scroll_box;
	bool auto_scroll;
	QString log_filename;
	QTimer log_timer;
	QStringList log_buffer;
	QMutex log_mutex;

	int n;
	short chan;
	double u;
	double t;
signals:
	void foi_interrupt(int _n, short _chan, double _u, double _t);
};

#endif //RPC_FOI_H