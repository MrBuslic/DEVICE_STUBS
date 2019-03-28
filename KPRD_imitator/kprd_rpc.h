#ifndef KPRD_RPC_H
#define KPRD_RPC_H
#include "socket_rpc.h"


class RPC_kprd_SLOT_Object : public RPC_SLOT_Object
{
	Q_OBJECT
public:
	RPC_kprd_SLOT_Object(QString _addr, int _port) : RPC_SLOT_Object(_addr, _port) {this->QObject::setObjectName("kprd_slot_obj");}
	virtual void connect_to_server();
	////////////////////////////////////
public slots:
	void dataIn(QVariantList dataList, QVariantList maskList);
	////////////////////////////////////
};

class RPC_kprd_SLOT_Thread : public RPC_SLOT_Thread
{
	Q_OBJECT
public:
	RPC_kprd_SLOT_Thread() : RPC_SLOT_Thread() {this->QObject::setObjectName("kprd_slot_thr");}
	void run();
	SLOT_OBJ_PTR get_obj() {return std::static_pointer_cast<RPC_SLOT_Object, RPC_kprd_SLOT_Object>(rpc_obj);}
	std::shared_ptr<RPC_kprd_SLOT_Object> get_kprd_obj() {return rpc_obj;}
private:
	std::shared_ptr<RPC_kprd_SLOT_Object> rpc_obj;

};

class RPC_kprd_SIGNAL_Object : public RPC_SIGNAL_Object
{
	Q_OBJECT
public:
	RPC_kprd_SIGNAL_Object(QString _addr, int _port) : RPC_SIGNAL_Object(_addr, _port)
	{
		this->QObject::setObjectName("kprd_signal_obj");
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
	void test(QVariantList maskList, QVariantList dataList);
	void sendKPI(QVariantList kpiList);
};

class RPC_kprd_SIGNAL_Thread : public RPC_SIGNAL_Thread
{
	Q_OBJECT
public:
	RPC_kprd_SIGNAL_Thread() : RPC_SIGNAL_Thread() {this->QObject::setObjectName("kprd_signal_thr");}
	void run();
	SIGNAL_OBJ_PTR get_obj() {return std::static_pointer_cast<RPC_SIGNAL_Object, RPC_kprd_SIGNAL_Object>(rpc_obj);}
private:
	std::shared_ptr<RPC_kprd_SIGNAL_Object> rpc_obj;
};

#endif
