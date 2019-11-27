#ifndef ADS128_H_H
#define ADS128_H_H

#include <unads128.h>
#include <socket_rpc.h>
#include <windows.h>
#include "ads128_rpc.h"

#define SINGLETON_DEF(x) typedef Loki::SingletonHolder<x,Loki::CreateUsingNew,Loki::NoDestroy> S##x;

class ads128_rpc_buffer_class : public QObject
{
	Q_OBJECT
public:
	QList<RPC_ads128_SLOT_Thread*> ads128_slot_thr;
	QList<RPC_ads128_SIGNAL_Thread*> ads128_signal_thr;
	friend struct Loki::CreateUsingNew<ads128_rpc_buffer_class>;
private:
	ads128_rpc_buffer_class();
};

SINGLETON_DEF(ads128_rpc_buffer_class);

#endif