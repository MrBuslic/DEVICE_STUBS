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
#include "rpc_loger.h"

#define SINGLETON_DEF(x) typedef Loki::SingletonHolder<x,Loki::CreateUsingNew,Loki::NoDestroy> S##x;

class RpcMDS32Widget : public QWidget
{
	Q_OBJECT
public:
	RpcMDS32Widget(int mds_num);
public slots:


	int unmds32_input_trigger(bool state);
	int unmds32_read_sample(uint& _buf, uint& _firstTime, uint& _lasteTime);
	int unmds32_start();

	void check_box_clicked();
	void line_edit_changed(const QString& _text);
signals:
	void mds32_get_sample(uint& buf, bool& flag);
private:
	LogWidget* log_widget;

	bool state;
	QList<QCheckBox*> checks;
	QLineEdit* buf_edit;

};



#endif //RPC_MDS32_H