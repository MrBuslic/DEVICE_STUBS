#ifndef MKPRM_RPC_H
#define MKPRM_RPC_H
#include "socket_rpc.h"


class RPC_mkprm_SLOT_Object : public RPC_SLOT_Object
{
	Q_OBJECT
public:
	RPC_mkprm_SLOT_Object(QString _addr, int _port) : RPC_SLOT_Object(_addr, _port) {this->QObject::setObjectName("mkprm_slot_obj");}
	////////////////////////////////////
public slots:
	int unmkprm_get_strings(int strings, QVariantList& string_data);
	void unmkprm_start();
	void unmkprm_stop();
	void new_frame(QString mode, QVariant _frame_data);
	////////////////////////////////////
};

class RPC_mkprm_SLOT_Thread : public RPC_SLOT_Thread
{
	Q_OBJECT
public:
	RPC_mkprm_SLOT_Thread() : RPC_SLOT_Thread() {this->QObject::setObjectName("mkprm_slot_thr");}
	void run();
	SLOT_OBJ_PTR get_obj() {return std::static_pointer_cast<RPC_SLOT_Object, RPC_mkprm_SLOT_Object>(rpc_obj);}
	std::shared_ptr<RPC_mkprm_SLOT_Object> get_mkprm_obj() {return rpc_obj;}
private:
	std::shared_ptr<RPC_mkprm_SLOT_Object> rpc_obj;

};

class RPC_mkprm_SIGNAL_Object : public RPC_SIGNAL_Object
{
	Q_OBJECT
public:
	RPC_mkprm_SIGNAL_Object(QString _addr, int _port) : RPC_SIGNAL_Object(_addr, _port)
	{
		this->QObject::setObjectName("mkprm_signal_obj");
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

class RPC_mkprm_SIGNAL_Thread : public RPC_SIGNAL_Thread
{
	Q_OBJECT
public:
	RPC_mkprm_SIGNAL_Thread() : RPC_SIGNAL_Thread() {this->QObject::setObjectName("mkprm_signal_thr");}
	void run();
	SIGNAL_OBJ_PTR get_obj() {return std::static_pointer_cast<RPC_SIGNAL_Object, RPC_mkprm_SIGNAL_Object>(rpc_obj);}
private:
	std::shared_ptr<RPC_mkprm_SIGNAL_Object> rpc_obj;
};

#endif
