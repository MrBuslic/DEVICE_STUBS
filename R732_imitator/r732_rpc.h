#ifndef R732_RPC_H
#define R732_RPC_H
#include "socket_rpc.h"


class RPC_r732_SLOT_Object : public RPC_SLOT_Object
{
	Q_OBJECT
public:
	RPC_r732_SLOT_Object(QString _addr, int _port) : RPC_SLOT_Object(_addr, _port) {this->QObject::setObjectName("r732_slot_obj");}
	virtual void connect_to_server();
	////////////////////////////////////
public slots:
	void new_message(QVariant dt, int mko, int line, int cwd, QVariantList words, int os);
	void set_new_mbk02_tm(int sadr, int word);
	////////////////////////////////////
};

class RPC_r732_SLOT_Thread : public RPC_SLOT_Thread
{
	Q_OBJECT
public:
	RPC_r732_SLOT_Thread() : RPC_SLOT_Thread() {this->QObject::setObjectName("r732_slot_thr");}
	void run();
	SLOT_OBJ_PTR get_obj() {return std::static_pointer_cast<RPC_SLOT_Object, RPC_r732_SLOT_Object>(rpc_obj);}
	std::shared_ptr<RPC_r732_SLOT_Object> get_r732_obj() {return rpc_obj;}
private:
	std::shared_ptr<RPC_r732_SLOT_Object> rpc_obj;

};

class RPC_r732_SIGNAL_Object : public RPC_SIGNAL_Object
{
	Q_OBJECT
public:
	RPC_r732_SIGNAL_Object(QString _addr, int _port) : RPC_SIGNAL_Object(_addr, _port)
	{
		this->QObject::setObjectName("r732_signal_obj");
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
};

class RPC_r732_SIGNAL_Thread : public RPC_SIGNAL_Thread
{
	Q_OBJECT
public:
	RPC_r732_SIGNAL_Thread() : RPC_SIGNAL_Thread() {this->QObject::setObjectName("r732_signal_thr");}
	void run();
	SIGNAL_OBJ_PTR get_obj() {return std::static_pointer_cast<RPC_SIGNAL_Object, RPC_r732_SIGNAL_Object>(rpc_obj);}
private:
	std::shared_ptr<RPC_r732_SIGNAL_Object> rpc_obj;
};

#endif
