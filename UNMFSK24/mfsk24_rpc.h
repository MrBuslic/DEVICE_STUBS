#ifndef MFSK24_RPC_H
#define MFSK24_RPC_H
#include "socket_rpc.h"


class RPC_mfsk24_SLOT_Object : public RPC_SLOT_Object
{
	Q_OBJECT
public:
	RPC_mfsk24_SLOT_Object(QString _addr, int _port) : RPC_SLOT_Object(_addr, _port) {this->QObject::setObjectName("mfsk24_slot_obj");}
	virtual void connect_to_server();
	////////////////////////////////////
public slots:
	void auto_scroll_clicked(int _state);
	void log_timer_ontimer();
	int unmfsk24_manual_group_cmd(int _state_chan, QVariantList _vec);
	int unmfsk24_state(QVariantList& _state);
	int unmfsk24_manual_cmd(int _chan, int _state_chan);
	int unmfsk24_start(QVariantList _state);
	int unmfsk24_set_cmd_time(int _chan, int _time);
	////////////////////////////////////
};

class RPC_mfsk24_SLOT_Thread : public RPC_SLOT_Thread
{
	Q_OBJECT
public:
	RPC_mfsk24_SLOT_Thread() : RPC_SLOT_Thread() {this->QObject::setObjectName("mfsk24_slot_thr");}
	void run();
	SLOT_OBJ_PTR get_obj() {return std::static_pointer_cast<RPC_SLOT_Object, RPC_mfsk24_SLOT_Object>(rpc_obj);}
	std::shared_ptr<RPC_mfsk24_SLOT_Object> get_mfsk24_obj() {return rpc_obj;}
private:
	std::shared_ptr<RPC_mfsk24_SLOT_Object> rpc_obj;

};

class RPC_mfsk24_SIGNAL_Object : public RPC_SIGNAL_Object
{
	Q_OBJECT
public:
	RPC_mfsk24_SIGNAL_Object(QString _addr, int _port) : RPC_SIGNAL_Object(_addr, _port)
	{
		this->QObject::setObjectName("mfsk24_signal_obj");
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
	void mfsk24_();
	void mfsk24_state_change(int channel, int state);
	void mfsk24_impulse_change(QVariantList channels);
};

class RPC_mfsk24_SIGNAL_Thread : public RPC_SIGNAL_Thread
{
	Q_OBJECT
public:
	RPC_mfsk24_SIGNAL_Thread() : RPC_SIGNAL_Thread() {this->QObject::setObjectName("mfsk24_signal_thr");}
	void run();
	SIGNAL_OBJ_PTR get_obj() {return std::static_pointer_cast<RPC_SIGNAL_Object, RPC_mfsk24_SIGNAL_Object>(rpc_obj);}
private:
	std::shared_ptr<RPC_mfsk24_SIGNAL_Object> rpc_obj;
};

#endif
