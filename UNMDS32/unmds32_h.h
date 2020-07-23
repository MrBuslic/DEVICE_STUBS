#ifndef __UNMDS32_H
#define __UNMDS32_H

#include "mds32_rpc.h"
#include "rpc_ports.h"


class rpc_buffer_class
{
public:
	QList<RPC_mds32_SLOT_Thread*> mds32_slot_thr;
	QList<RPC_mds32_SIGNAL_Thread*> mds32_signal_thr;

	static rpc_buffer_class& Instance()
	{
		static rpc_buffer_class inst;
		return inst;
	}
	rpc_buffer_class(rpc_buffer_class const&) = delete;
	rpc_buffer_class& operator= (rpc_buffer_class const&) = delete;

private:
	rpc_buffer_class()
	{
		for (int i = 0; i < 2; i++)
		{
			RPC_mds32_SLOT_Thread* slot_thr = new RPC_mds32_SLOT_Thread;
			slot_thr->set_connection_params("127.0.0.1", MDS_SLOT + i);
			slot_thr->start();
			//if (!slot_thr.wait_connected(3))
			//	return false;
			RPC_mds32_SIGNAL_Thread* signal_thr = new RPC_mds32_SIGNAL_Thread;
			signal_thr->set_connection_params("127.0.0.1", MDS_SIGNAL + i);
			signal_thr->start();
			//if (!signal_thr.wait_connected(3))
			//	return false;

			mds32_slot_thr.push_back(slot_thr);
			mds32_signal_thr.push_back(signal_thr);
		}
	}
};

typedef rpc_buffer_class Srpc_buffer_class;

#endif /* __UNMDS32_H */