#ifndef KPIU_RPC_H
#define KPIU_RPC_H
#include "socket_rpc.h"


class RPC_kpiu_SLOT_Object : public RPC_SLOT_Object
{
	Q_OBJECT
public:
	RPC_kpiu_SLOT_Object(QString _addr, int _port) : RPC_SLOT_Object(_addr, _port) {this->QObject::setObjectName("kpiu_slot_obj");}
	////////////////////////////////////
public slots:
	int ku_map_channels_setup(int ku_n, short line);
	int mshm_map_channels_setup(int mshm, short line_m);
	int pshm_map_channels_setup(int pshm, short line_p);
	int unfoi_map_channels_setup(int _n, short _chan);
	int unols_write_data_kf(QVariantList data_buffer, QVariantList mask_buffer);
	int unols_trigger_imm(int devise);
	void unols_read_data_kr(QVariantList& data_buffer);
	void set_antenna_connection(QString antenna_name, QString connected_antenna_name);
	int unomnibus_map_channels_setup(int _n, short _chan);
	void set_bus_state(int bus, int state);
	////////////////////////////////////
};

class RPC_kpiu_SLOT_Thread : public RPC_SLOT_Thread
{
	Q_OBJECT
public:
	RPC_kpiu_SLOT_Thread() : RPC_SLOT_Thread() {this->QObject::setObjectName("kpiu_slot_thr");}
	void run();
	SLOT_OBJ_PTR get_obj() {return std::static_pointer_cast<RPC_SLOT_Object, RPC_kpiu_SLOT_Object>(rpc_obj);}
	std::shared_ptr<RPC_kpiu_SLOT_Object> get_kpiu_obj() {return rpc_obj;}
private:
	std::shared_ptr<RPC_kpiu_SLOT_Object> rpc_obj;

};

class RPC_kpiu_SIGNAL_Object : public RPC_SIGNAL_Object
{
	Q_OBJECT
public:
	RPC_kpiu_SIGNAL_Object(QString _addr, int _port) : RPC_SIGNAL_Object(_addr, _port)
	{
		this->QObject::setObjectName("kpiu_signal_obj");
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
	void string_msg(QString _msg);
	void int_msg(int _msg);
	void int_return_signal(int& ret_int);
	void toLog(QString message);
	void toProtocol(QString message);
};

class RPC_kpiu_SIGNAL_Thread : public RPC_SIGNAL_Thread
{
	Q_OBJECT
public:
	RPC_kpiu_SIGNAL_Thread() : RPC_SIGNAL_Thread() {this->QObject::setObjectName("kpiu_signal_thr");}
	void run();
	SIGNAL_OBJ_PTR get_obj() {return std::static_pointer_cast<RPC_SIGNAL_Object, RPC_kpiu_SIGNAL_Object>(rpc_obj);}
private:
	std::shared_ptr<RPC_kpiu_SIGNAL_Object> rpc_obj;
};

#endif
