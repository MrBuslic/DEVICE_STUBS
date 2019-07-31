#ifndef ADS128_RPC_H
#define ADS128_RPC_H
#include "socket_rpc.h"


class RPC_ads128_SLOT_Object : public RPC_SLOT_Object
{
	Q_OBJECT
public:
	RPC_ads128_SLOT_Object(QString _addr, int _port) : RPC_SLOT_Object(_addr, _port) {this->QObject::setObjectName("ads128_slot_obj");}
	virtual void connect_to_server();
	////////////////////////////////////
public slots:
	void auto_scroll_clicked(int _state);
	void log_timer_ontimer();
	void measurement_timer_ontimer();
	void infin_timer_ontimer();
	int unads128_start();
	int unads128_input_trigger(bool state);
	int unads128_sample_width_q(uint& frame_width, uint& width_in_bytes);
	int unads128_read_sample(uint& _buf, uint& _firstTime, uint& _thisTime);
	int unads128_read_packet(bool isHot, uint numSamples, QVariantList& buf, uint& realNumSamples);
	int unads128_sample_period(double _periodS);
	int unads128_mode_cycle(uint _size);
	int unads128_num_ready_data(uint& _num);
	void button_clicked();
	////////////////////////////////////
};

class RPC_ads128_SLOT_Thread : public RPC_SLOT_Thread
{
	Q_OBJECT
public:
	RPC_ads128_SLOT_Thread() : RPC_SLOT_Thread() {this->QObject::setObjectName("ads128_slot_thr");}
	void run();
	SLOT_OBJ_PTR get_obj() {return std::static_pointer_cast<RPC_SLOT_Object, RPC_ads128_SLOT_Object>(rpc_obj);}
	std::shared_ptr<RPC_ads128_SLOT_Object> get_ads128_obj() {return rpc_obj;}
private:
	std::shared_ptr<RPC_ads128_SLOT_Object> rpc_obj;

};

class RPC_ads128_SIGNAL_Object : public RPC_SIGNAL_Object
{
	Q_OBJECT
public:
	RPC_ads128_SIGNAL_Object(QString _addr, int _port) : RPC_SIGNAL_Object(_addr, _port)
	{
		this->QObject::setObjectName("ads128_signal_obj");
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
	void packet_ready();
};

class RPC_ads128_SIGNAL_Thread : public RPC_SIGNAL_Thread
{
	Q_OBJECT
public:
	RPC_ads128_SIGNAL_Thread() : RPC_SIGNAL_Thread() {this->QObject::setObjectName("ads128_signal_thr");}
	void run();
	SIGNAL_OBJ_PTR get_obj() {return std::static_pointer_cast<RPC_SIGNAL_Object, RPC_ads128_SIGNAL_Object>(rpc_obj);}
private:
	std::shared_ptr<RPC_ads128_SIGNAL_Object> rpc_obj;
};

#endif
