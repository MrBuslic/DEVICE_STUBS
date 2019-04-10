#ifndef __UNOLS_H
#define __UNOLS_H
#include <unols.h>
#include <socket_rpc.h>
#include <windows.h>
#include "ols_rpc.h"
#define SINGLETON_DEF(x) typedef Loki::SingletonHolder<x,Loki::CreateUsingNew,Loki::NoDestroy> S##x;

class rpc_buffer_class : public QObject
{
	Q_OBJECT
private:
	rpc_buffer_class();
public:
	friend struct Loki::CreateUsingNew<rpc_buffer_class>;

	QVariantList ols_buffer;
	QVariantList mask_buffer;
	QList<RPC_ols_SLOT_Thread*> ols_slot_thr;
	QList<RPC_ols_SIGNAL_Thread*> ols_signal_thr;
	void put_data(unsigned int period, void* data, void* maska) {
		ols_buffer.clear();
		mask_buffer.clear();
		for (int period_num = 0; period_num < period; period_num++) {
			ols_buffer << ((((unsigned long long*)data)[period_num]) & ((unsigned long long*)maska)[period_num]);
			mask_buffer << ((unsigned long long*)maska)[period_num];
		}
	}
};

SINGLETON_DEF(rpc_buffer_class);

#endif /* __UNOLS_H */