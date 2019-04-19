#ifndef RPC_MKPRM_H
#define RPC_MKPRM_H
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
#include "frameSettings.hpp"

#include "../buses_imitator/frame_bus_rpc.h"

#ifndef SINGLETON_DEF
#define SINGLETON_DEF(x) typedef Loki::SingletonHolder<x,Loki::CreateUsingNew,Loki::NoDestroy> S##x;
#endif
class RpcMKPRMWidget : public QWidget
{
	Q_OBJECT
public:
	RpcMKPRMWidget();
public slots:
	int unmkprm_get_strings(int strings, QVariantList& string_data);
	void new_frame(QString mode, QVariant _frame_data);
private:
	QVariantList frame_data;

	FrameParams fp;
	FrameParams::frameParmsMap params_mode_map;

	RPC_frame_bus_SLOT_Thread frame_slot_thr;
	RPC_frame_bus_SIGNAL_Thread frame_signal_thr;
};

#endif //RPC_FOI_H