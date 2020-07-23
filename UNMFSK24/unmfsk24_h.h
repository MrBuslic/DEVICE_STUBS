#include "mfsk24_rpc.h"
#include <rpc_ports.h>

#ifndef UNMFSK24_H_H
#define UNMFSK24_H_H

class rpc_buffer_class
{
public:
	QList<RPC_mfsk24_SLOT_Thread*> mfsk24_slot_thr;
	QList<RPC_mfsk24_SIGNAL_Thread*> mfsk24_signal_thr;


	static rpc_buffer_class& Instance()
	{
		static rpc_buffer_class inst;
		return inst;
	}
	rpc_buffer_class(rpc_buffer_class const&) = delete;
	rpc_buffer_class& operator= (rpc_buffer_class const&) = delete;

	int num_add;
	QMap<int, QMap<int, int> > mfsk_addrs;
private:
	rpc_buffer_class()
	{
		QString comapp = QCoreApplication::applicationName();
		num_add = 0;
		if (comapp == "comapp1")
		{
			QMap<int, int> first_nm;
			first_nm.insert(1, 0);
			mfsk_addrs.insert(3, first_nm);

			QMap<int, int> second_nm;
			second_nm.insert(3, 1);
			second_nm.insert(4, 2);
			mfsk_addrs.insert(7, second_nm);
		}
		if (comapp == "comapp2")
		{
			num_add = 5;

			QMap<int, int> first_nm;
			first_nm.insert(1, 5);
			first_nm.insert(4, 6);
			mfsk_addrs.insert(8, first_nm);

			QMap<int, int> second_nm;
			second_nm.insert(3, 7);
			second_nm.insert(4, 8);
			mfsk_addrs.insert(9, second_nm);
		}
		qDebug() << "comapp " << comapp << " num_add " << num_add;
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

typedef rpc_buffer_class Srpc_buffer_class;

#endif /* UNMFSK24_H_H */