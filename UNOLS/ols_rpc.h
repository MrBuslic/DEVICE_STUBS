#ifndef OLS_RPC_H
#define OLS_RPC_H
#include "socket_rpc.h"


class RPC_ols_SLOT_Object : public RPC_SLOT_Object
{
	Q_OBJECT
public:
	RPC_ols_SLOT_Object(QString _addr, int _port) : RPC_SLOT_Object(_addr, _port) {this->QObject::setObjectName("ols_slot_obj");}
	////////////////////////////////////
public slots:
	int unols_write_data_kf(QVariantList data_buffer, QVariantList mask_buffer);
	int unols_trigger_imm(int devise);
	void unols_read_data_kr(QVariantList& data_buffer);
	int unols_mStart();
	////////////////////////////////////
};

class RPC_ols_SLOT_Thread : public RPC_SLOT_Thread
{
	Q_OBJECT
public:
	RPC_ols_SLOT_Thread() : RPC_SLOT_Thread() {this->QObject::setObjectName("ols_slot_thr");}
	void run();
	SLOT_OBJ_PTR get_obj() {return std::static_pointer_cast<RPC_SLOT_Object, RPC_ols_SLOT_Object>(rpc_obj);}
	std::shared_ptr<RPC_ols_SLOT_Object> get_ols_obj() {return rpc_obj;}
private:
	std::shared_ptr<RPC_ols_SLOT_Object> rpc_obj;
};

class RPC_ols_SIGNAL_Object : public RPC_SIGNAL_Object
{
	Q_OBJECT
public:
	RPC_ols_SIGNAL_Object(QString _addr, int _port) : RPC_SIGNAL_Object(_addr, _port)
	{
		this->QObject::setObjectName("ols_signal_obj");
	}
public slots:
	void read_data();
protected:
	void connectNotify(const QMetaMethod & signal);
	void disconnectNotify(const QMetaMethod & signal);
signals:
	void receive_data(QVariantList& data_buffer);
	void new_ols_data(QVariantList data_buffer, QVariantList mask_buffer);
	void packet_ready(QVariantList data_buffer);
};

class RPC_ols_SIGNAL_Thread : public RPC_SIGNAL_Thread
{
	Q_OBJECT
public:
	RPC_ols_SIGNAL_Thread() : RPC_SIGNAL_Thread() {this->QObject::setObjectName("ols_signal_thr");}
	void run();
	SIGNAL_OBJ_PTR get_obj() {return std::static_pointer_cast<RPC_SIGNAL_Object, RPC_ols_SIGNAL_Object>(rpc_obj);}
private:
	std::shared_ptr<RPC_ols_SIGNAL_Object> rpc_obj;
};

#endif
