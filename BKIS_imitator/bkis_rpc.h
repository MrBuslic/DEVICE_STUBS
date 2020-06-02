#ifndef BKIS_RPC_H
#define BKIS_RPC_H
#include "socket_rpc.h"


class RPC_bkis_SLOT_Object : public RPC_SLOT_Object
{
	Q_OBJECT
public:
	RPC_bkis_SLOT_Object(QString _addr, int _port) : RPC_SLOT_Object(_addr, _port) {this->QObject::setObjectName("bkis_slot_obj");}
	////////////////////////////////////
public slots:
	void new_message(QVariant dt, int mko, int line, int cwd, QVariantList words, int os);
	void make_ku(int ku_n, int length, double u, int line);
	void get_power(double _volt);
	void set_blk_state(int state);
	void set_interface_state(bool state);
	int set_electric_heater_state(int name, bool state);
	int set_pyro_group_state(int group_num, bool state);
	void set_power_back();
	void omni_connect();
	void set_pyro_bus_state(int bus_num);
	////////////////////////////////////
};

class RPC_bkis_SLOT_Thread : public RPC_SLOT_Thread
{
	Q_OBJECT
public:
	RPC_bkis_SLOT_Thread() : RPC_SLOT_Thread() {this->QObject::setObjectName("bkis_slot_thr");}
	void run();
	SLOT_OBJ_PTR get_obj() {return std::static_pointer_cast<RPC_SLOT_Object, RPC_bkis_SLOT_Object>(rpc_obj);}
	std::shared_ptr<RPC_bkis_SLOT_Object> get_bkis_obj() {return rpc_obj;}
private:
	std::shared_ptr<RPC_bkis_SLOT_Object> rpc_obj;

};

class RPC_bkis_SIGNAL_Object : public RPC_SIGNAL_Object
{
	Q_OBJECT
public:
	RPC_bkis_SIGNAL_Object(QString _addr, int _port) : RPC_SIGNAL_Object(_addr, _port)
	{
		this->QObject::setObjectName("bkis_signal_obj");
	}
public slots:
	void read_data();
protected:
	void connectNotify(const QMetaMethod & signal);
	void disconnectNotify(const QMetaMethod & signal);
signals:
	void send_pyro_group_activation(int group_num);
};

class RPC_bkis_SIGNAL_Thread : public RPC_SIGNAL_Thread
{
	Q_OBJECT
public:
	RPC_bkis_SIGNAL_Thread() : RPC_SIGNAL_Thread() {this->QObject::setObjectName("bkis_signal_thr");}
	void run();
	SIGNAL_OBJ_PTR get_obj() {return std::static_pointer_cast<RPC_SIGNAL_Object, RPC_bkis_SIGNAL_Object>(rpc_obj);}
private:
	std::shared_ptr<RPC_bkis_SIGNAL_Object> rpc_obj;
};

#endif
