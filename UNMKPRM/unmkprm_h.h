#ifndef __UNMKPRM_H_H
#define __UNMKPRM_H_H

class rpc_buffer_class
{
public:
	RPC_mkprm_SLOT_Thread mkprm_slot_thr;
	RPC_mkprm_SIGNAL_Thread mkprm_signal_thr;

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

#endif /* __UNMKPRM_H_H */