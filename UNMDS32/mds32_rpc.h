#ifndef MDS32_RPC_H
#define MDS32_RPC_H
#include "socket_rpc.h"


class RPC_mds32_SLOT_Object : public RPC_SLOT_Object
{
	Q_OBJECT
public:
	RPC_mds32_SLOT_Object(QString _addr, int _port) : RPC_SLOT_Object(_addr, _port) {this->QObject::setObjectName("mds32_slot_obj");}
	virtual void connect_to_server();
	////////////////////////////////////
public slots:
	void auto_scroll_clicked(int _state);
	void log_timer_ontimer();
	int unmds32_input_trigger(bool state);
	int unmds32_read_sample(uint& _buf, uint& _firstTime, uint& _lasteTime);
	int unmds32_start();
	void check_box_clicked();
	void line_edit_changed(const QString& _text);
	////////////////////////////////////
};

class RPC_mds32_SLOT_Thread : public RPC_SLOT_Thread
{
	Q_OBJECT
public:
	RPC_mds32_SLOT_Thread() : RPC_SLOT_Thread() {this->QObject::setObjectName("mds32_slot_thr");}
	void run();
	SLOT_OBJ_PTR get_obj() {return std::static_pointer_cast<RPC_SLOT_Object, RPC_mds32_SLOT_Object>(rpc_obj);}
	std::shared_ptr<RPC_mds32_SLOT_Object> get_mds32_obj() {return rpc_obj;}
private:
	std::shared_ptr<RPC_mds32_SLOT_Object> rpc_obj;

};

class RPC_mds32_SIGNAL_Object : public RPC_SIGNAL_Object
{
	Q_OBJECT
public:
	RPC_mds32_SIGNAL_Object(QString _addr, int _port) : RPC_SIGNAL_Object(_addr, _port)
	{
		this->QObject::setObjectName("mds32_signal_obj");
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
	void mds32_get_sample(int channel, uint& buf, int& flag);
};

class RPC_mds32_SIGNAL_Thread : public RPC_SIGNAL_Thread
{
	Q_OBJECT
public:
	RPC_mds32_SIGNAL_Thread() : RPC_SIGNAL_Thread() {this->QObject::setObjectName("mds32_signal_thr");}
	void run();
	SIGNAL_OBJ_PTR get_obj() {return std::static_pointer_cast<RPC_SIGNAL_Object, RPC_mds32_SIGNAL_Object>(rpc_obj);}
private:
	std::shared_ptr<RPC_mds32_SIGNAL_Object> rpc_obj;
};

#endif
