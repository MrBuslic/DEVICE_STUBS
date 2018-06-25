#ifndef RPC_MT8K4L_H
#define RPC_MT8K4L_H
#ifdef WIN32
#include "winsock2.h"
#endif
#include <QObject>
#include <QString>
#include <QWidget>
#include <QLabel>
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

class RpcMT8K4LWidget : public QWidget
{
	Q_OBJECT
public:
	RpcMT8K4LWidget(int slot_port, int signal_port);
public slots:

	void auto_scroll_clicked(int _state);
	void log_timer_ontimer();

	int unmt8k4l_start();
	int unmt8k4l_input_trigger(bool state);
	int unmt8k4l_sample_width_q(uint& frame_width, uint&  width_in_bytes);
	int unmt8k4l_read_sample(QVariantList& _buf, uint& _firstTime, uint& _thisTime);

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
	QList<QLineEdit*> checks;

};



#endif //RPC_MT8K4L_H