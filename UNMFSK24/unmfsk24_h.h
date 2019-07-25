#include "mfsk24_rpc.h"
#include <rpc_ports.h>

#ifndef UNMFSK24_H_H
#define UNMFSK24_H_H

#ifndef SINGLETON_DEF
#define SINGLETON_DEF(x) typedef Loki::SingletonHolder<x,Loki::CreateUsingNew,Loki::NoDestroy> S##x;
#endif

class rpc_buffer_class
{
public:
	QList<RPC_mfsk24_SLOT_Thread*> mfsk24_slot_thr;
	QList<RPC_mfsk24_SIGNAL_Thread*> mfsk24_signal_thr;
	friend struct Loki::CreateUsingNew<rpc_buffer_class>;
	int num_add;
private:
	rpc_buffer_class()
	{
		QString commapp = QCoreApplication::applicationName();
		if (commapp == "comapp1")
		{
			num_add = 0;
		}
		else
		{
			num_add = 5;
		}

		for (int i = 0; i < 4; i++)
		{
			RPC_mfsk24_SLOT_Thread* slot_thr = new RPC_mfsk24_SLOT_Thread;
			slot_thr->set_connection_params("127.0.0.1", MFSK_SLOT + i + num_add);
			slot_thr->start();
			//if (!slot_thr.wait_connected(3))
			//	return false;
			RPC_mfsk24_SIGNAL_Thread* signal_thr = new RPC_mfsk24_SIGNAL_Thread;
			signal_thr->set_connection_params("127.0.0.1", MFSK_SIGNAL + i + num_add);
			signal_thr->start();
			//if (!signal_thr.wait_connected(3))
			//	return false;

			mfsk24_slot_thr.push_back(slot_thr);
			mfsk24_signal_thr.push_back(signal_thr);
		}
	}

};

SINGLETON_DEF(rpc_buffer_class);

#endif /* UNMFSK24_H_H */