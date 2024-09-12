#ifndef KP50_RPC_H
#define KP50_RPC_H
#include "socket_rpc.h"


class RPC_kp50_SLOT_Object : public RPC_SLOT_Object
{
	Q_OBJECT
public:
	RPC_kp50_SLOT_Object(QString _addr, int _port) : RPC_SLOT_Object(_addr, _port) {this->QObject::setObjectName("kp50_slot_obj");}
	virtual void connect_to_server();
	////////////////////////////////////
public slots:
	void auto_scroll_clicked(int _state);
	void log_timer_ontimer();
	int unfoi_chan_setup(int _n, short _chan, double _u, double _t);
	int unfoi_run();
	////////////////////////////////////
};

class RPC_kp50_SLOT_Thread : public RPC_SLOT_Thread
{
	Q_OBJECT
public:
	RPC_kp50_SLOT_Thread() : RPC_SLOT_Thread() {this->QObject::setObjectName("kp50_slot_thr");}
	void run();
	SLOT_OBJ_PTR get_obj() {return std::static_pointer_cast<RPC_SLOT_Object, RPC_kp50_SLOT_Object>(rpc_obj);}
	std::shared_ptr<RPC_kp50_SLOT_Object> get_kp50_obj() {return rpc_obj;}
private:
	std::shared_ptr<RPC_kp50_SLOT_Object> rpc_obj;

};

class RPC_kp50_SIGNAL_Object : public RPC_SIGNAL_Object
{
	Q_OBJECT
public:
	RPC_kp50_SIGNAL_Object(QString _addr, int _port) : RPC_SIGNAL_Object(_addr, _port)
	{
		this->QObject::setObjectName("kp50_signal_obj");
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
	void foi_interrupt(int _n, short _chan, double _u, double _t);
};

class RPC_kp50_SIGNAL_Thread : public RPC_SIGNAL_Thread
{
	Q_OBJECT
public:
	RPC_kp50_SIGNAL_Thread() : RPC_SIGNAL_Thread() {this->QObject::setObjectName("kp50_signal_thr");}
	void run();
	SIGNAL_OBJ_PTR get_obj() {return std::static_pointer_cast<RPC_SIGNAL_Object, RPC_kp50_SIGNAL_Object>(rpc_obj);}
private:
	std::shared_ptr<RPC_kp50_SIGNAL_Object> rpc_obj;
};

#endif
