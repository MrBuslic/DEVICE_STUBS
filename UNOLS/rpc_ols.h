#ifndef RPC_Ols_H
#define RPC_Ols_H
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

#ifndef SINGLETON_DEF
#define SINGLETON_DEF(x) typedef Loki::SingletonHolder<x,Loki::CreateUsingNew,Loki::NoDestroy> S##x;
#endif
class RpcOlsWidget : public QWidget
{
	Q_OBJECT
public:
	RpcOlsWidget(int ols_num);
public slots:
	void auto_scroll_clicked(int _state);
	void log_timer_ontimer();
	//todo корректно ли называть функцию передачи данных через rpc как функцию записи формирования
	int unols_write_data_kf(QVariantList data_buffer, QVariantList mask_buffer);
	int unols_trigger_imm(int devise);
	void unols_read_data_kr(QVariantList& data_buffer);
	int unols_mStart();	
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

	QVariantList rpc_ols_kr_buffer;
	QVariantList rpc_ols_kf_buffer;
	QVariantList rpc_mask_buffer;
	//добавить слот и сигнал для триггер им, в триггер им передавать данные. rpc_ols для передачи дальше
	int n;
	short chan;
	double u;
	double t;
signals:
	void send_data(QVariantList& data_buffer);
	void new_ols_data(QVariantList data_buffer, QVariantList mask_buffer);
	void packet_ready(QVariantList data_buffer);
};

#endif //RPC_OLS_H