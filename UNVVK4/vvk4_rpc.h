#ifndef VVK4_RPC_H
#define VVK4_RPC_H
#include "socket_rpc.h"


class RPC_vvk4_SLOT_Object : public RPC_SLOT_Object
{
	Q_OBJECT
public:
	RPC_vvk4_SLOT_Object(QString _addr, int _port) : RPC_SLOT_Object(_addr, _port) {this->QObject::setObjectName("vvk4_slot_obj");}
	////////////////////////////////////
public slots:
	int unvvk4_commut_ListOutput(int _line, QString _masOn, QString _masOff);
	void get_commut_chanels_list(QVariantList& ei_list, QVariantList& sum_list);
	int unvvk4_config_MeasureLine(int line, int state);
	void get_measure_lines(QVariantList& mes_list);
	////////////////////////////////////
};

class RPC_vvk4_SLOT_Thread : public RPC_SLOT_Thread
{
	Q_OBJECT
public:
	RPC_vvk4_SLOT_Thread() : RPC_SLOT_Thread() {this->QObject::setObjectName("vvk4_slot_thr");}
	void run();
	SLOT_OBJ_PTR get_obj() {return std::static_pointer_cast<RPC_SLOT_Object, RPC_vvk4_SLOT_Object>(rpc_obj);}
	std::shared_ptr<RPC_vvk4_SLOT_Object> get_vvk4_obj() {return rpc_obj;}
private:
	std::shared_ptr<RPC_vvk4_SLOT_Object> rpc_obj;

};

class RPC_vvk4_SIGNAL_Object : public RPC_SIGNAL_Object
{
	Q_OBJECT
public:
	RPC_vvk4_SIGNAL_Object(QString _addr, int _port) : RPC_SIGNAL_Object(_addr, _port)
	{
		this->QObject::setObjectName("vvk4_signal_obj");
	}
public slots:
	void read_data();
protected:
	void connectNotify(const QMetaMethod & signal);
	void disconnectNotify(const QMetaMethod & signal);
};

class RPC_vvk4_SIGNAL_Thread : public RPC_SIGNAL_Thread
{
	Q_OBJECT
public:
	RPC_vvk4_SIGNAL_Thread() : RPC_SIGNAL_Thread() {this->QObject::setObjectName("vvk4_signal_thr");}
	void run();
	SIGNAL_OBJ_PTR get_obj() {return std::static_pointer_cast<RPC_SIGNAL_Object, RPC_vvk4_SIGNAL_Object>(rpc_obj);}
private:
	std::shared_ptr<RPC_vvk4_SIGNAL_Object> rpc_obj;
};

#endif
