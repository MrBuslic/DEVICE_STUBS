#ifndef MKU_BUS_RPC_H
#define MKU_BUS_RPC_H
#include "socket_rpc.h"


class RPC_mku_bus_SLOT_Object : public RPC_SLOT_Object
{
	Q_OBJECT
public:
	RPC_mku_bus_SLOT_Object(QString _addr, int _port) : RPC_SLOT_Object(_addr, _port) {this->QObject::setObjectName("mku_bus_slot_obj");}
	virtual void connect_to_server();
	////////////////////////////////////
public slots:
	void make_ku(int ku_n, int length, double u, int line);
	void make_ku_732(int ku_n, int length, double u, int line);
	void make_mk(int mshm, int pshm, int length_m, int length_p, double u_m, double u_p, int dt, int line_m, int line_p);
	int ku_map_channels_setup(int ku_n, short line);
	int mshm_map_channels_setup(int mshm, short line_m);
	int pshm_map_channels_setup(int pshm, short line_p);
	////////////////////////////////////
};

class RPC_mku_bus_SLOT_Thread : public RPC_SLOT_Thread
{
	Q_OBJECT
public:
	RPC_mku_bus_SLOT_Thread() : RPC_SLOT_Thread() {this->QObject::setObjectName("mku_bus_slot_thr");}
	void run();
	SLOT_OBJ_PTR get_obj() {return std::static_pointer_cast<RPC_SLOT_Object, RPC_mku_bus_SLOT_Object>(rpc_obj);}
	std::shared_ptr<RPC_mku_bus_SLOT_Object> get_mku_bus_obj() {return rpc_obj;}
private:
	std::shared_ptr<RPC_mku_bus_SLOT_Object> rpc_obj;

};

class RPC_mku_bus_SIGNAL_Object : public RPC_SIGNAL_Object
{
	Q_OBJECT
public:
	RPC_mku_bus_SIGNAL_Object(QString _addr, int _port) : RPC_SIGNAL_Object(_addr, _port)
	{
		this->QObject::setObjectName("mku_bus_signal_obj");
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
	void new_ku(int ku_n, int length, double u, int line);
	void new_ku_732(int ku_n, int length, double u, int line);
	void new_mk(int mshm, int pshm, int length_m, int length_p, double u_m, double u_p, int dt, int line_m, int line_p);
};

class RPC_mku_bus_SIGNAL_Thread : public RPC_SIGNAL_Thread
{
	Q_OBJECT
public:
	RPC_mku_bus_SIGNAL_Thread() : RPC_SIGNAL_Thread() {this->QObject::setObjectName("mku_bus_signal_thr");}
	void run();
	SIGNAL_OBJ_PTR get_obj() {return std::static_pointer_cast<RPC_SIGNAL_Object, RPC_mku_bus_SIGNAL_Object>(rpc_obj);}
private:
	std::shared_ptr<RPC_mku_bus_SIGNAL_Object> rpc_obj;
};

#endif
