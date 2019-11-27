#include <socket_rpc.h>

#ifndef __UNMT8K4L_H_H
#define __UNMT8K4L_H_H

#define SINGLETON_DEF(x) typedef Loki::SingletonHolder<x,Loki::CreateUsingNew,Loki::NoDestroy> S##x;

class rpc_buffer_class
{
public:
	QList<RPC_mt8k4l_SLOT_Thread*> mt8k4l_slot_thr;
	QList<RPC_mt8k4l_SIGNAL_Thread*> mt8k4l_signal_thr;
	friend struct Loki::CreateUsingNew<rpc_buffer_class>;
private:
	rpc_buffer_class()
	{
		for (int i = 0; i < 2; i++)
		{
			RPC_mt8k4l_SLOT_Thread* slot_thr = new RPC_mt8k4l_SLOT_Thread;
			slot_thr->set_connection_params("127.0.0.1", 30030 + i);
			slot_thr->start();
			//if (!slot_thr.wait_connected(3))
			//	return false;
			RPC_mt8k4l_SIGNAL_Thread* signal_thr = new RPC_mt8k4l_SIGNAL_Thread;
			signal_thr->set_connection_params("127.0.0.1", 30035 + i);
			signal_thr->start();
			//if (!signal_thr.wait_connected(3))
			//	return false;
			mt8k4l_slot_thr.push_back(slot_thr);
			mt8k4l_signal_thr.push_back(signal_thr);
		}
	}
};

SINGLETON_DEF(rpc_buffer_class);

#endif /* __UNMT8K4L_H_H */