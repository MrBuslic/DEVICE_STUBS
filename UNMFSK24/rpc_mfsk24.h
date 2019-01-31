#ifndef RPC_MFSK24_H
#define RPC_MFSK24_H
#ifdef WIN32
#include "winsock2.h"
#endif
#include <QObject>
#include <QString>
#include <QWidget>
#include <QLabel>
#include <qpushbutton.h>
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

class RpcMFSK24Widget : public QWidget
{
	Q_OBJECT
public:
	RpcMFSK24Widget(int slot_port, int signal_port);
public slots:

	void auto_scroll_clicked(int _state);
	void log_timer_ontimer();

	int unmfsk24_manual_group_cmd(int _state_chan, QVariantList _vec);
	int unmfsk24_state(QVariantList& _state);
	int unmfsk24_manual_cmd(int _chan, int _state_chan);
	int unmfsk24_start(QVariantList _state);
	int unmfsk24_set_cmd_time(int _chan, int _time);

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
	
	QVariantList state;
	QMap<int, int> impulses;

	QList<QPushButton*> labels;
signals:
	void mfsk24_();
	void mfsk24_state_change(int channel, int state);
	void mfsk24_impulse_change(int channel, int duration);

};


#endif //RPC_MFSK24_H