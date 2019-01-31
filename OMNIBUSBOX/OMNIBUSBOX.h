#ifndef OMNIBUSBOX_H
#define OMNIBUSBOX_H
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

struct RpcAbonent
{
	RpcAbonent(){}
	RpcAbonent(int addr);
	QMap<int, QList<unsigned short>> words;
	unsigned short os;
	bool on;
};

class RpcOmnibusWidget : public QWidget
{
	Q_OBJECT
public:
	RpcOmnibusWidget();
public slots:

	void auto_scroll_clicked(int _state);
	void log_timer_ontimer();

	void switch_ab(int mko, int addr, bool _on);
	void set_new_data(int mko, int addr, int saddr, QVariantList words);
	void send_msg(int mko, int line, int cwd, QVariantList& words, int& os);
	QVariant get_dt();

	void message_to_log_slot(QString _msg);
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
	QMap<short, QMap<short, RpcAbonent>> abonents;
	int n;
	short chan;
	double u;
	double t;

signals:
	void new_message(QVariant dt, int mko, int line, int cwd, QVariantList words, int os);
	void message_to_log(QString _msg);
};

#endif //RPC_FOI_H