#ifndef RPC_MWSWITCH_H
#define RPC_MWSWITCH_H
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
#include "rpc_loger.h"

class RpcMWSwitchWidget : public QWidget
{
	Q_OBJECT
public:
	RpcMWSwitchWidget(int switch_num);
public slots:

private:
	LogWidget* log_widget;

};



#endif //RPC_MDS32_H