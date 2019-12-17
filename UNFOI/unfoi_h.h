#ifndef UNFOI_H_H
#define UNFOI_H_H

#ifndef SINGLETON_DEF
#define SINGLETON_DEF(x) typedef Loki::SingletonHolder<x,Loki::CreateUsingNew,Loki::NoDestroy> S##x;
#endif

class rpc_buffer_class
{
public:
	RPC_foi_SLOT_Thread foi_slot_thr;
	RPC_foi_SIGNAL_Thread foi_signal_thr;
};

SINGLETON_DEF(rpc_buffer_class);
#endif /*UNFOI_H_H*/