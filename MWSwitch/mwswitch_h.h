#ifndef __UNMWSWITCH_H
#define __UNMWSWITCH_H

#include "mwswitch_rpc.h"
#include "rpc_ports.h"

#define SINGLETON_DEF(x) typedef Loki::SingletonHolder<x,Loki::CreateUsingNew,Loki::NoDestroy> S##x;

class rpc_buffer_class
{
public:
	QList<RPC_mwswitch_SLOT_Thread*> mwswitch_slot_thr;
	QList<RPC_mwswitch_SIGNAL_Thread*> mwswitch_signal_thr;
	friend struct Loki::CreateUsingNew<rpc_buffer_class>;
private:
	rpc_buffer_class()
	{
		for (int i = 0; i < 2; i++)
		{
			RPC_mwswitch_SLOT_Thread* slot_thr = new RPC_mwswitch_SLOT_Thread;
			slot_thr->set_connection_params("127.0.0.1", MWSWITCH_SLOT + i);
			slot_thr->start();
			//if (!slot_thr.wait_connected(3))
			//	return false;
			RPC_mwswitch_SIGNAL_Thread* signal_thr = new RPC_mwswitch_SIGNAL_Thread;
			signal_thr->set_connection_params("127.0.0.1", MWSWITCH_SIGNAL + i);
			signal_thr->start();
			//if (!signal_thr.wait_connected(3))
			//	return false;

			mwswitch_slot_thr.push_back(slot_thr);
			mwswitch_signal_thr.push_back(signal_thr);
		}
	}
};

SINGLETON_DEF(rpc_buffer_class);

#endif /* __UNMDS32_H */