#ifndef FRAME_BUS_RPC_H
#define FRAME_BUS_RPC_H
#include "socket_rpc.h"


class RPC_frame_bus_SLOT_Object : public RPC_SLOT_Object
{
	Q_OBJECT
public:
	RPC_frame_bus_SLOT_Object(QString _addr, int _port) : RPC_SLOT_Object(_addr, _port) {this->QObject::setObjectName("frame_bus_slot_obj");}
	virtual void connect_to_server();
	////////////////////////////////////
public slots:
	void make_new_frame_04(QString mode, QVariant frame_data);
	void make_new_frame_733(QString mode, QVariant frame_data);
	void make_new_frame_07(QString mode, int psp, int lit, QVariant frame_data);
	void make_new_frame_rm07(QString mode, QVariant frame_data);
	////////////////////////////////////
};

class RPC_frame_bus_SLOT_Thread : public RPC_SLOT_Thread
{
	Q_OBJECT
public:
	RPC_frame_bus_SLOT_Thread() : RPC_SLOT_Thread() {this->QObject::setObjectName("frame_bus_slot_thr");}
	void run();
	SLOT_OBJ_PTR get_obj() {return std::static_pointer_cast<RPC_SLOT_Object, RPC_frame_bus_SLOT_Object>(rpc_obj);}
	std::shared_ptr<RPC_frame_bus_SLOT_Object> get_frame_bus_obj() {return rpc_obj;}
private:
	std::shared_ptr<RPC_frame_bus_SLOT_Object> rpc_obj;

};

class RPC_frame_bus_SIGNAL_Object : public RPC_SIGNAL_Object
{
	Q_OBJECT
public:
	RPC_frame_bus_SIGNAL_Object(QString _addr, int _port) : RPC_SIGNAL_Object(_addr, _port)
	{
		this->QObject::setObjectName("frame_bus_signal_obj");
		connect(this, SIGNAL(connect_signal(QString, bool)), this, SLOT(send_connect(QString, bool)), Qt::BlockingQueuedConnection); 
	}
	virtual void connect_to_server();
public slots:
	void read_data();
	void send_connect(QString signal_name, bool _connect);
protected:
	void connectNotify(const QMetaMethod & signal);
	void disconnectNotify(const QMetaMethod & signal);
signals:
	void connect_signal(QString signal_name, bool _connect);
	void new_frame_04(QString mode, QVariant frame_data);
	void new_frame_733(QString mode, QVariant frame_data);
	void new_frame_07(QString mode, int psp, int lit, QVariant frame_data);
	void new_frame_rm07(QString mode, QVariant frame_data);
};

class RPC_frame_bus_SIGNAL_Thread : public RPC_SIGNAL_Thread
{
	Q_OBJECT
public:
	RPC_frame_bus_SIGNAL_Thread() : RPC_SIGNAL_Thread() {this->QObject::setObjectName("frame_bus_signal_thr");}
	void run();
	SIGNAL_OBJ_PTR get_obj() {return std::static_pointer_cast<RPC_SIGNAL_Object, RPC_frame_bus_SIGNAL_Object>(rpc_obj);}
private:
	std::shared_ptr<RPC_frame_bus_SIGNAL_Object> rpc_obj;
};

#endif
