#ifndef LKA05_RPC_H
#define LKA05_RPC_H
#include "socket_rpc.h"


class RPC_lka05_SLOT_Object : public RPC_SLOT_Object
{
	Q_OBJECT
public:
	RPC_lka05_SLOT_Object(QString _addr, int _port) : RPC_SLOT_Object(_addr, _port) {this->QObject::setObjectName("lka05_slot_obj");}
	virtual void connect_to_server();
	////////////////////////////////////
public slots:
	void new_message(QVariant dt, int mko, int line, int cwd, QVariantList words, int os);
	void save_choose_set();
	void new_tm(int tm);
	////////////////////////////////////
};

class RPC_lka05_SLOT_Thread : public RPC_SLOT_Thread
{
	Q_OBJECT
public:
	RPC_lka05_SLOT_Thread() : RPC_SLOT_Thread() {this->QObject::setObjectName("lka05_slot_thr");}
	void run();
	SLOT_OBJ_PTR get_obj() {return std::static_pointer_cast<RPC_SLOT_Object, RPC_lka05_SLOT_Object>(rpc_obj);}
	std::shared_ptr<RPC_lka05_SLOT_Object> get_lka05_obj() {return rpc_obj;}
private:
	std::shared_ptr<RPC_lka05_SLOT_Object> rpc_obj;

};

class RPC_lka05_SIGNAL_Object : public RPC_SIGNAL_Object
{
	Q_OBJECT
public:
	RPC_lka05_SIGNAL_Object(QString _addr, int _port) : RPC_SIGNAL_Object(_addr, _port)
	{
		this->QObject::setObjectName("lka05_signal_obj");
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
	void new_ku(int ku_n, int length, double u);
	void new_mk(int mshm, int pshm, int length_m, int length_p, double u_m, double u_p, int dt);
};

class RPC_lka05_SIGNAL_Thread : public RPC_SIGNAL_Thread
{
	Q_OBJECT
public:
	RPC_lka05_SIGNAL_Thread() : RPC_SIGNAL_Thread() {this->QObject::setObjectName("lka05_signal_thr");}
	void run();
	SIGNAL_OBJ_PTR get_obj() {return std::static_pointer_cast<RPC_SIGNAL_Object, RPC_lka05_SIGNAL_Object>(rpc_obj);}
private:
	std::shared_ptr<RPC_lka05_SIGNAL_Object> rpc_obj;
};

#endif
