#ifndef BOOP_RPC_H
#define BOOP_RPC_H
#include "socket_rpc.h"


class RPC_BOOP_SLOT_Object : public RPC_SLOT_Object
{
	Q_OBJECT
public:
	RPC_BOOP_SLOT_Object(QString _addr, int _port) : RPC_SLOT_Object(_addr, _port) {this->QObject::setObjectName("BOOP_slot_obj");}
	////////////////////////////////////
public slots:
	////////////////////////////////////
};

class RPC_BOOP_SLOT_Thread : public RPC_SLOT_Thread
{
	Q_OBJECT
public:
	RPC_BOOP_SLOT_Thread() : RPC_SLOT_Thread() {this->QObject::setObjectName("BOOP_slot_thr");}
	void run();
	SLOT_OBJ_PTR get_obj() {return std::static_pointer_cast<RPC_SLOT_Object, RPC_BOOP_SLOT_Object>(rpc_obj);}
	std::shared_ptr<RPC_BOOP_SLOT_Object> get_BOOP_obj() {return rpc_obj;}
private:
	std::shared_ptr<RPC_BOOP_SLOT_Object> rpc_obj;

};

class RPC_BOOP_SIGNAL_Object : public RPC_SIGNAL_Object
{
	Q_OBJECT
public:
	RPC_BOOP_SIGNAL_Object(QString _addr, int _port) : RPC_SIGNAL_Object(_addr, _port)
	{
		this->QObject::setObjectName("BOOP_signal_obj");
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
};

class RPC_BOOP_SIGNAL_Thread : public RPC_SIGNAL_Thread
{
	Q_OBJECT
public:
	RPC_BOOP_SIGNAL_Thread() : RPC_SIGNAL_Thread() {this->QObject::setObjectName("BOOP_signal_thr");}
	void run();
	SIGNAL_OBJ_PTR get_obj() {return std::static_pointer_cast<RPC_SIGNAL_Object, RPC_BOOP_SIGNAL_Object>(rpc_obj);}
private:
	std::shared_ptr<RPC_BOOP_SIGNAL_Object> rpc_obj;
};

#endif
