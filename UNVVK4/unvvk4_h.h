#ifndef __UNVVK4_H_H
#define __UNVVK4_H_H

#ifndef SINGLETON_DEF
#define SINGLETON_DEF(x) typedef Loki::SingletonHolder<x,Loki::CreateUsingNew,Loki::NoDestroy> S##x;
#endif

class rpc_buffer_class
{
public:
	RPC_vvk4_SLOT_Thread vvk4_slot_thr;
	RPC_vvk4_SIGNAL_Thread vvk4_signal_thr;
};

SINGLETON_DEF(rpc_buffer_class);

#endif /* __UNVVK4_H_H */