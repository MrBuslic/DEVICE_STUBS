#ifndef UNFOI_H_H
#define UNFOI_H_H


class rpc_buffer_class
{
public:
	RPC_foi_SLOT_Thread foi_slot_thr;
	RPC_foi_SIGNAL_Thread foi_signal_thr;

	static rpc_buffer_class& Instance()
	{
		static rpc_buffer_class inst;
		return inst;
	}
	rpc_buffer_class(rpc_buffer_class const&) = delete;
	rpc_buffer_class& operator= (rpc_buffer_class const&) = delete;
private:
	rpc_buffer_class() {}
};

typedef rpc_buffer_class Srpc_buffer_class;
#endif /*UNFOI_H_H*/