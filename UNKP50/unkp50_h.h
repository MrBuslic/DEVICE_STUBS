#ifndef __UNKP50_H
#define __UNKP50_H


class kp50_rpc_buffer_class
{
public:
	RPC_kp50_SLOT_Thread* kp50_slot_thr;
	RPC_kp50_SIGNAL_Thread* kp50_signal_thr;

	static kp50_rpc_buffer_class& Instance()
	{
		static kp50_rpc_buffer_class inst;
		return inst;
	}
	kp50_rpc_buffer_class(kp50_rpc_buffer_class const&) = delete;
	kp50_rpc_buffer_class& operator= (kp50_rpc_buffer_class const&) = delete;

private:
	kp50_rpc_buffer_class();
};

typedef kp50_rpc_buffer_class Skp50_rpc_buffer_class;
#endif /* __UNKP50_H */