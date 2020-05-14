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
#include "rpc_loger.h"

#ifndef SINGLETON_DEF
#define SINGLETON_DEF(x) typedef Loki::SingletonHolder<x,Loki::CreateUsingNew,Loki::NoDestroy> S##x;
#endif
class RpcOlsWidget : public QWidget
{
	Q_OBJECT
public:
	RpcOlsWidget(QWidget* parent = 0, int ols_num = 0);
public slots:

	//todo корректно ли называть функцию передачи данных через rpc как функцию записи формирования
	int unols_write_data_kf(QVariantList data_buffer, QVariantList mask_buffer);
	int unols_trigger_imm(int devise);
	void unols_read_data_kr(QVariantList& data_buffer);
	int unols_mStart();	
private:

	QVariantList rpc_ols_kr_buffer;
	QVariantList rpc_ols_kf_buffer;
	QVariantList rpc_mask_buffer;
	//добавить слот и сигнал для триггер им, в триггер им передавать данные. rpc_ols для передачи дальше
	int n;
	short chan;
	double u;
	double t;

	LogWidget* log_widget;
signals:
	void receive_data(QVariantList& data_buffer);
	void new_ols_data(QVariantList data_buffer, QVariantList mask_buffer);
	void packet_ready(QVariantList data_buffer);
};

#endif //RPC_OLS_H