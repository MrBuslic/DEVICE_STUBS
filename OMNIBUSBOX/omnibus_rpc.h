#ifndef OMNIBUS_RPC_H
#define OMNIBUS_RPC_H
#include "socket_rpc.h"


class RPC_omnibus_SLOT_Object : public RPC_SLOT_Object
{
	Q_OBJECT
public:
	RPC_omnibus_SLOT_Object(QString _addr, int _port) : RPC_SLOT_Object(_addr, _port) {this->QObject::setObjectName("omnibus_slot_obj");}
	virtual void connect_to_server();
	////////////////////////////////////
public slots:
	void auto_scroll_clicked(int _state);
	void log_timer_ontimer();
	void switch_ab_os(int mko, int addr, int _os);
	void switch_ab(int mko, int addr, bool _on);
	void set_new_data(int mko, int addr, int saddr, QVariantList words);
	void send_msg(int mko, int line, int cwd, QVariantList& words, int& os);
	int unomnibus_map_channels_setup(int _n, short _chan);
	QVariant get_dt();
	void message_to_log_slot(QString _msg);
	////////////////////////////////////
};

class RPC_omnibus_SLOT_Thread : public RPC_SLOT_Thread
{
	Q_OBJECT
public:
	RPC_omnibus_SLOT_Thread() : RPC_SLOT_Thread() {this->QObject::setObjectName("omnibus_slot_thr");}
	void run();
	SLOT_OBJ_PTR get_obj() {return std::static_pointer_cast<RPC_SLOT_Object, RPC_omnibus_SLOT_Object>(rpc_obj);}
	std::shared_ptr<RPC_omnibus_SLOT_Object> get_omnibus_obj() {return rpc_obj;}
private:
	std::shared_ptr<RPC_omnibus_SLOT_Object> rpc_obj;

};

class RPC_omnibus_SIGNAL_Object : public RPC_SIGNAL_Object
{
	Q_OBJECT
public:
	RPC_omnibus_SIGNAL_Object(QString _addr, int _port) : RPC_SIGNAL_Object(_addr, _port)
	{
		this->QObject::setObjectName("omnibus_signal_obj");
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
	void new_message(QVariant dt, int mko, int line, int cwd, QVariantList words, int os);
	void message_to_log(QString _msg);
};

class RPC_omnibus_SIGNAL_Thread : public RPC_SIGNAL_Thread
{
	Q_OBJECT
public:
	RPC_omnibus_SIGNAL_Thread() : RPC_SIGNAL_Thread() {this->QObject::setObjectName("omnibus_signal_thr");}
	void run();
	SIGNAL_OBJ_PTR get_obj() {return std::static_pointer_cast<RPC_SIGNAL_Object, RPC_omnibus_SIGNAL_Object>(rpc_obj);}
private:
	std::shared_ptr<RPC_omnibus_SIGNAL_Object> rpc_obj;
};

#endif
