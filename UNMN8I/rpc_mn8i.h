#ifndef RPC_MN8I_H
#define RPC_MN8I_H
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
#include <QLineEdit>
#include <memory>
#include <qlayout.h>
#include <loki/Singleton.h>

#define SINGLETON_DEF(x) typedef Loki::SingletonHolder<x,Loki::CreateUsingNew,Loki::NoDestroy> S##x;

class RpcMN8IWidget : public QWidget
{
	Q_OBJECT
public:
	RpcMN8IWidget(int slot_port, int signal_port);
public slots:

	void auto_scroll_clicked(int _state);
	void log_timer_ontimer();

	int unmn8i_start();
	int unmn8i_input_trigger(bool state);
	int unmn8i_sample_width_q(uint& frame_width, uint&  width_in_bytes);
	int unmn8i_read_sample(uint& _buf, uint& _firstTime, uint& _thisTime);
	int unmn8i_read_packet(bool isHot, uint numSamples, QVariantList& buf, uint& realNumSamples);
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

	bool state;

	QList <QTextEdit*>  checks;//!!!!!!!!!!!!1
	QLineEdit* buf_edit;
	QVariantList buffer;
signals:
	void packet_ready();
};

#endif