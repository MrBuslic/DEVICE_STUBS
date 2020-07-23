#ifndef ADS128_H_H
#define ADS128_H_H

#include <unads128.h>
#include <socket_rpc.h>
#include <windows.h>
#include "ads128_rpc.h"

class ads128_rpc_buffer_class : public QObject
{
	Q_OBJECT
public:
	QList<RPC_ads128_SLOT_Thread*> ads128_slot_thr;
	QList<RPC_ads128_SIGNAL_Thread*> ads128_signal_thr;

	static ads128_rpc_buffer_class& Instance()
	{
		static ads128_rpc_buffer_class inst;
		return inst;
	}
	ads128_rpc_buffer_class(ads128_rpc_buffer_class const&) = delete;
	ads128_rpc_buffer_class& operator= (ads128_rpc_buffer_class const&) = delete;

private:
	ads128_rpc_buffer_class();
};

typedef ads128_rpc_buffer_class Sads128_rpc_buffer_class;

#endif