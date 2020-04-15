#ifndef N736_RPC_H
#define N736_RPC_H
#include "socket_rpc.h"


class RPC_n736_SLOT_Object : public RPC_SLOT_Object
{
	Q_OBJECT
public:
	RPC_n736_SLOT_Object(QString _addr, int _port) : RPC_SLOT_Object(_addr, _port) {this->QObject::setObjectName("n736_slot_obj");}
	////////////////////////////////////
public slots:
	void dataIn(QVariantList dataList, QVariantList maskList);
	////////////////////////////////////
};

class RPC_n736_SLOT_Thread : public RPC_SLOT_Thread
{
	Q_OBJECT
public:
	RPC_n736_SLOT_Thread() : RPC_SLOT_Thread() {this->QObject::setObjectName("n736_slot_thr");}
	void run();
	SLOT_OBJ_PTR get_obj() {return std::static_pointer_cast<RPC_SLOT_Object, RPC_n736_SLOT_Object>(rpc_obj);}
	std::shared_ptr<RPC_n736_SLOT_Object> get_n736_obj() {return rpc_obj;}
private:
	std::shared_ptr<RPC_n736_SLOT_Object> rpc_obj;

};

class RPC_n736_SIGNAL_Object : public RPC_SIGNAL_Object
{
	Q_OBJECT
public:
	RPC_n736_SIGNAL_Object(QString _addr, int _port) : RPC_SIGNAL_Object(_addr, _port)
	{
		this->QObject::setObjectName("n736_signal_obj");
	}
public slots:
	void read_data();
protected:
	void connectNotify(const QMetaMethod & signal);
	void disconnectNotify(const QMetaMethod & signal);
signals:
	void new_ku(int ku_n, int length, double u, int line);
};

class RPC_n736_SIGNAL_Thread : public RPC_SIGNAL_Thread
{
	Q_OBJECT
public:
	RPC_n736_SIGNAL_Thread() : RPC_SIGNAL_Thread() {this->QObject::setObjectName("n736_signal_thr");}
	void run();
	SIGNAL_OBJ_PTR get_obj() {return std::static_pointer_cast<RPC_SIGNAL_Object, RPC_n736_SIGNAL_Object>(rpc_obj);}
private:
	std::shared_ptr<RPC_n736_SIGNAL_Object> rpc_obj;
};

#endif
