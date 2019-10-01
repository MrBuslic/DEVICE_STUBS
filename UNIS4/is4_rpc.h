#ifndef IS4_RPC_H
#define IS4_RPC_H
#include "socket_rpc.h"


class RPC_is4_SLOT_Object : public RPC_SLOT_Object
{
	Q_OBJECT
public:
	RPC_is4_SLOT_Object(QString _addr, int _port) : RPC_SLOT_Object(_addr, _port) {this->QObject::setObjectName("is4_slot_obj");}
	////////////////////////////////////
public slots:
	void log_timer_ontimer();
	void auto_scroll_clicked(int _state);
	void is4_clicked(int state_is4);
	void measure(uint NProcess, QVariant& value);
	int unis4_SetTypeProcess(uint EProcess);
	int unis4_StartACP();
	int unis4_ResultMeas(double& ResMeas, uint& NumRes);
	int unis4_RangeMeas(uint range);
	int unis4_StartCalibr();
	////////////////////////////////////
};

class RPC_is4_SLOT_Thread : public RPC_SLOT_Thread
{
	Q_OBJECT
public:
	RPC_is4_SLOT_Thread() : RPC_SLOT_Thread() {this->QObject::setObjectName("is4_slot_thr");}
	void run();
	SLOT_OBJ_PTR get_obj() {return std::static_pointer_cast<RPC_SLOT_Object, RPC_is4_SLOT_Object>(rpc_obj);}
	std::shared_ptr<RPC_is4_SLOT_Object> get_is4_obj() {return rpc_obj;}
private:
	std::shared_ptr<RPC_is4_SLOT_Object> rpc_obj;

};

class RPC_is4_SIGNAL_Object : public RPC_SIGNAL_Object
{
	Q_OBJECT
public:
	RPC_is4_SIGNAL_Object(QString _addr, int _port) : RPC_SIGNAL_Object(_addr, _port)
	{
		this->QObject::setObjectName("is4_signal_obj");
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

class RPC_is4_SIGNAL_Thread : public RPC_SIGNAL_Thread
{
	Q_OBJECT
public:
	RPC_is4_SIGNAL_Thread() : RPC_SIGNAL_Thread() {this->QObject::setObjectName("is4_signal_thr");}
	void run();
	SIGNAL_OBJ_PTR get_obj() {return std::static_pointer_cast<RPC_SIGNAL_Object, RPC_is4_SIGNAL_Object>(rpc_obj);}
private:
	std::shared_ptr<RPC_is4_SIGNAL_Object> rpc_obj;
};

#endif
