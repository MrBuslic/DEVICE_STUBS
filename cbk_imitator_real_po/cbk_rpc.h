#ifndef CBK_RPC_H
#define CBK_RPC_H
#include "socket_rpc.h"


class RPC_cbk_SLOT_Object : public RPC_SLOT_Object
{
	Q_OBJECT
public:
	RPC_cbk_SLOT_Object(QString _addr, int _port) : RPC_SLOT_Object(_addr, _port) {this->QObject::setObjectName("cbk_slot_obj");}
	////////////////////////////////////
public slots:
	void set_tm_state();
	void new_ku(int ku_n, int length, double u, int line);
	////////////////////////////////////
};

class RPC_cbk_SLOT_Thread : public RPC_SLOT_Thread
{
	Q_OBJECT
public:
	RPC_cbk_SLOT_Thread() : RPC_SLOT_Thread() {this->QObject::setObjectName("cbk_slot_thr");}
	void run();
	SLOT_OBJ_PTR get_obj() {return std::static_pointer_cast<RPC_SLOT_Object, RPC_cbk_SLOT_Object>(rpc_obj);}
	std::shared_ptr<RPC_cbk_SLOT_Object> get_cbk_obj() {return rpc_obj;}
private:
	std::shared_ptr<RPC_cbk_SLOT_Object> rpc_obj;

};

class RPC_cbk_SIGNAL_Object : public RPC_SIGNAL_Object
{
	Q_OBJECT
public:
	RPC_cbk_SIGNAL_Object(QString _addr, int _port) : RPC_SIGNAL_Object(_addr, _port)
	{
		this->QObject::setObjectName("cbk_signal_obj");
		connect(this, SIGNAL(connect_signal(QString, bool)), this, SLOT(send_connect(QString, bool)), Qt::BlockingQueuedConnection); 
	}
public slots:
	void read_data();
	void send_connect(QString signal_name, bool _connect);
protected:
	void connectNotify(const QMetaMethod & signal);
	void disconnectNotify(const QMetaMethod & signal);
signals:
	void connect_signal(QString signal_name, bool _connect);
	void vm_is_on(int n_vm);
	void vm_is_off(int n_vm);
	void vm_change_po(int n_vm);
};

class RPC_cbk_SIGNAL_Thread : public RPC_SIGNAL_Thread
{
	Q_OBJECT
public:
	RPC_cbk_SIGNAL_Thread() : RPC_SIGNAL_Thread() {this->QObject::setObjectName("cbk_signal_thr");}
	void run();
	SIGNAL_OBJ_PTR get_obj() {return std::static_pointer_cast<RPC_SIGNAL_Object, RPC_cbk_SIGNAL_Object>(rpc_obj);}
private:
	std::shared_ptr<RPC_cbk_SIGNAL_Object> rpc_obj;
};

#endif
