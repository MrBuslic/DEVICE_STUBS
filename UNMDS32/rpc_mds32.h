#ifndef RPC_MDS32_H
#define RPC_MDS32_H
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

class RpcMDS32Widget : public QWidget
{
	Q_OBJECT
public:
	RpcMDS32Widget();
public slots:

	void auto_scroll_clicked(int _state);
	void log_timer_ontimer();

	int unmds32_input_trigger(bool state);
	int unmds32_read_sample(int& _buf, int& _firstTime, int& _lasteTime);
	int unmds32_start();

	void check_box_clicked();
	void line_edit_changed(const QString& _text);
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
	QList<QCheckBox*> checks;
	QLineEdit* buf_edit;

};



#endif //RPC_MDS32_H