#ifndef FSVA7_RPC_H
#define FSVA7_RPC_H
#include "socket_rpc.h"


class RPC_fsva7_SLOT_Object : public RPC_SLOT_Object
{
	Q_OBJECT
public:
	RPC_fsva7_SLOT_Object(QString _addr, int _port) : RPC_SLOT_Object(_addr, _port) {this->QObject::setObjectName("fsva7_slot_obj");}
	////////////////////////////////////
public slots:
	void error_Slot(QAbstractSocket::SocketError socketError);
	void read();
	void connect_ag();
	void read_ag();
	////////////////////////////////////
};

class RPC_fsva7_SLOT_Thread : public RPC_SLOT_Thread
{
	Q_OBJECT
public:
	RPC_fsva7_SLOT_Thread() : RPC_SLOT_Thread() {this->QObject::setObjectName("fsva7_slot_thr");}
	void run();
	SLOT_OBJ_PTR get_obj() {return std::static_pointer_cast<RPC_SLOT_Object, RPC_fsva7_SLOT_Object>(rpc_obj);}
	std::shared_ptr<RPC_fsva7_SLOT_Object> get_fsva7_obj() {return rpc_obj;}
private:
	std::shared_ptr<RPC_fsva7_SLOT_Object> rpc_obj;

};

class RPC_fsva7_SIGNAL_Object : public RPC_SIGNAL_Object
{
	Q_OBJECT
public:
	RPC_fsva7_SIGNAL_Object(QString _addr, int _port) : RPC_SIGNAL_Object(_addr, _port)
	{
		this->QObject::setObjectName("fsva7_signal_obj");
	}
public slots:
	void read_data();
protected:
	void connectNotify(const QMetaMethod & signal);
	void disconnectNotify(const QMetaMethod & signal);
};

class RPC_fsva7_SIGNAL_Thread : public RPC_SIGNAL_Thread
{
	Q_OBJECT
public:
	RPC_fsva7_SIGNAL_Thread() : RPC_SIGNAL_Thread() {this->QObject::setObjectName("fsva7_signal_thr");}
	void run();
	SIGNAL_OBJ_PTR get_obj() {return std::static_pointer_cast<RPC_SIGNAL_Object, RPC_fsva7_SIGNAL_Object>(rpc_obj);}
private:
	std::shared_ptr<RPC_fsva7_SIGNAL_Object> rpc_obj;
};

#endif
