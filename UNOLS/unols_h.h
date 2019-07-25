#ifndef __UNOLS_H_H
#define __UNOLS_H_H
#include <unols.h>
#include <socket_rpc.h>
#include <windows.h>
#include "ols_rpc.h"
#define SINGLETON_DEF(x) typedef Loki::SingletonHolder<x,Loki::CreateUsingNew,Loki::NoDestroy> S##x;

class ols_rpc_buffer_class : public QObject
{
	Q_OBJECT
private:
	ols_rpc_buffer_class();
public:
	int rec_freq;//определиться с типом
	/*enum rec_freq{
		low,
		high
	};*/
	int rec_packs;
	int rec_event;
	friend struct Loki::CreateUsingNew<ols_rpc_buffer_class>;
		
	int pack_num;
	QVector <int> periods;
	QVector <int> series;
	QList<QVariantList> ols_KR_buffer;
	QList<QVariantList> ols_KF_buffer;
	QList<QVariantList> mask_buffer;
	QList<RPC_ols_SLOT_Thread*> ols_slot_thr;
	QList<RPC_ols_SIGNAL_Thread*> ols_signal_thr;
	void put_data(unsigned int vi, unsigned int period, void* data, void* maska) {
		ols_KF_buffer.clear();
		mask_buffer.clear();
		for (int period_num = 0; period_num < period; period_num++) {
			ols_KF_buffer[vi - 1] << ((((unsigned long long*)data)[period_num]) & ((unsigned long long*)maska)[period_num]);
			mask_buffer[vi - 1] << ((unsigned long long*)maska)[period_num];
		}
	}
	void unols_mKRLastFrameReceivedUserCallback(ViSession vi, ViUInt16 eventREG);
public slots:
	void packet_ready(unsigned int vi, QVariantList& data_buffer);
signals:
	void read_data_kr(QVariantList& data_buffer);
};
SINGLETON_DEF(ols_rpc_buffer_class);
#endif /* __UNOLS_H_H */