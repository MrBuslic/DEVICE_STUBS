#ifndef __UNVVK4_H_H
#define __UNVVK4_H_H


class rpc_buffer_class
{
public:
	RPC_vvk4_SLOT_Thread vvk4_slot_thr;
	RPC_vvk4_SIGNAL_Thread vvk4_signal_thr;

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

#endif /* __UNVVK4_H_H */