#ifndef RPC_FOI_H
#define RPC_FOI_H
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

#include "../buses_imitator/interrupt_bus_rpc.h"

#ifndef SINGLETON_DEF
#define SINGLETON_DEF(x) typedef Loki::SingletonHolder<x,Loki::CreateUsingNew,Loki::NoDestroy> S##x;
#endif
class RpcFoiWidget : public QWidget
{
	Q_OBJECT
public:
	RpcFoiWidget();
public slots:
	int unfoi_map_channels_setup(int _n, short _chan);
	int unfoi_chan_setup(int _n, short _chan, double _u, double _t);
	int unfoi_run();
private:
	int n;
	short chan;
	QMap<int, int> map_channels;//словарь исправности каналов

	RPC_interrupt_bus_SLOT_Thread interrupt_slot_thr;
	RPC_interrupt_bus_SIGNAL_Thread interrupt_signal_thr;
};

#endif //RPC_FOI_H