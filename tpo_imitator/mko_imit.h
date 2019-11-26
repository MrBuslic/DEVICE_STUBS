#ifndef MKO_IMIT_H
#define MKO_IMIT_H
#include "winsock2.h"
#include <QString>
#include <QObject>
#include <QTextEdit>
#include "omnibus_rpc.h"

class MKO_Widget : public QObject
{
	Q_OBJECT
public:
	MKO_Widget();
	void create_slot_thread();
public slots:
	void slot_send_new_msg(int mko, int line, int cwd, QVariantList& words, int& os);
	void send_message(int adr, int subadr, int direct, int count_words, QVariantList& words);
	RPC_omnibus_SLOT_Thread* get_mko_slot_thread() { return mko_slot_thread; };

private slots:
	//void log_timer_ontimer();

private:
	RPC_omnibus_SLOT_Thread* mko_slot_thread;
	RPC_omnibus_SIGNAL_Thread* mko_signal_thread;
	QString log_filename;
	QTimer log_timer;
	QStringList log_buffer;
	QMutex log_mutex;
	
signals:
	void signal_send_new_msg(int mko, int line, int cwd, QVariantList& words, int& os);
};




#endif