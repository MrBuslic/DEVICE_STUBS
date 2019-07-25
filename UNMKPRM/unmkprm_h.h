#ifndef __UNMKPRM_H_H
#define __UNMKPRM_H_H

class rpc_buffer_class
{
public:
	RPC_mkprm_SLOT_Thread mkprm_slot_thr;
	RPC_mkprm_SIGNAL_Thread mkprm_signal_thr;
};

SINGLETON_DEF(rpc_buffer_class);
#endif /* __UNMKPRM_H_H */