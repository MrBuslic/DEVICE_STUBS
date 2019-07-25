#ifndef __UNKP50_H
#define __UNKP50_H

#ifndef SINGLETON_DEF
#define SINGLETON_DEF(x) typedef Loki::SingletonHolder<x,Loki::CreateUsingNew,Loki::NoDestroy> S##x;
#endif

class rpc_buffer_class
{
public:
	RPC_kp50_SLOT_Thread kp50_slot_thr;
	RPC_kp50_SIGNAL_Thread kp50_signal_thr;
};

SINGLETON_DEF(rpc_buffer_class);
#endif /* __UNKP50_H */