#ifndef POWER_BUS_RPC_H
#define POWER_BUS_RPC_H
#include "socket_rpc.h"


class RPC_power_bus_SLOT_Object : public RPC_SLOT_Object
{
	Q_OBJECT
public:
	RPC_power_bus_SLOT_Object(QString _addr, int _port) : RPC_SLOT_Object(_addr, _port) {this->QObject::setObjectName("power_bus_slot_obj");}
	virtual void connect_to_server();
	////////////////////////////////////
public slots:
	void set_u(int bus, double volt);
	void get_i(int bus, double& curr);
	void set_i(int bus, QString name, double curr);
	////////////////////////////////////
};

class RPC_power_bus_SLOT_Thread : public RPC_SLOT_Thread
{
	Q_OBJECT
public:
	RPC_power_bus_SLOT_Thread() : RPC_SLOT_Thread() {this->QObject::setObjectName("power_bus_slot_thr");}
	void run();
	SLOT_OBJ_PTR get_obj() {return std::static_pointer_cast<RPC_SLOT_Object, RPC_power_bus_SLOT_Object>(rpc_obj);}
	std::shared_ptr<RPC_power_bus_SLOT_Object> get_power_bus_obj() {return rpc_obj;}
private:
	std::shared_ptr<RPC_power_bus_SLOT_Object> rpc_obj;

};

class RPC_power_bus_SIGNAL_Object : public RPC_SIGNAL_Object
{
	Q_OBJECT
public:
	RPC_power_bus_SIGNAL_Object(QString _addr, int _port) : RPC_SIGNAL_Object(_addr, _port)
	{
		this->QObject::setObjectName("power_bus_signal_obj");
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
	void u_on_nk(double volt);
	void u_on_k1(double volt);
	void u_on_k2(double volt);
};

class RPC_power_bus_SIGNAL_Thread : public RPC_SIGNAL_Thread
{
	Q_OBJECT
public:
	RPC_power_bus_SIGNAL_Thread() : RPC_SIGNAL_Thread() {this->QObject::setObjectName("power_bus_signal_thr");}
	void run();
	SIGNAL_OBJ_PTR get_obj() {return std::static_pointer_cast<RPC_SIGNAL_Object, RPC_power_bus_SIGNAL_Object>(rpc_obj);}
private:
	std::shared_ptr<RPC_power_bus_SIGNAL_Object> rpc_obj;
};

#endif
