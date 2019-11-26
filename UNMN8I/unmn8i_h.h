#ifndef UMN8I_H_H
#define UMN8I_H_H
#include <unmn8i.h>
#include <socket_rpc.h>
#include <windows.h>
#include "mn8i_rpc.h"
#define SINGLETON_DEF(x) typedef Loki::SingletonHolder<x,Loki::CreateUsingNew,Loki::NoDestroy> S##x;

typedef ViStatus(_VI_FUNCH * UNMN8IIntHandle)(ViSession mvi,
	ViInt32 reason, ViPBoolean levelUp,
	ViPBoolean levelDown, ViAddr userdata);

class mn8i_rpc_buffer_class : public QObject
{
	Q_OBJECT
public:
	QList<RPC_mn8i_SLOT_Thread*> mn8i_slot_thr;
	QList<RPC_mn8i_SIGNAL_Thread*> mn8i_signal_thr;
	friend struct Loki::CreateUsingNew<mn8i_rpc_buffer_class>;
private:
	mn8i_rpc_buffer_class();
public slots:
	void packet_ready();
};

SINGLETON_DEF(mn8i_rpc_buffer_class);

#endif