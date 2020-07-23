#ifndef UMN8I_H_H
#define UMN8I_H_H
#include <unmn8i.h>
#include <socket_rpc.h>
#include <windows.h>
#include "mn8i_rpc.h"

typedef ViStatus(_VI_FUNCH * UNMN8IIntHandle)(ViSession mvi,
	ViInt32 reason, ViPBoolean levelUp,
	ViPBoolean levelDown, ViAddr userdata);

class mn8i_rpc_buffer_class : public QObject
{
	Q_OBJECT
public:
	QList<RPC_mn8i_SLOT_Thread*> mn8i_slot_thr;
	QList<RPC_mn8i_SIGNAL_Thread*> mn8i_signal_thr;

	static mn8i_rpc_buffer_class& Instance()
	{
		static mn8i_rpc_buffer_class inst;
		return inst;
	}
	mn8i_rpc_buffer_class(mn8i_rpc_buffer_class const&) = delete;
	mn8i_rpc_buffer_class& operator= (mn8i_rpc_buffer_class const&) = delete;

private:
	mn8i_rpc_buffer_class();
public slots:
	void packet_ready();
};

typedef mn8i_rpc_buffer_class Smn8i_rpc_buffer_class;

#endif