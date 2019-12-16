#ifndef MBK02_RPC_H
#define MBK02_RPC_H
#include "socket_rpc.h"


class RPC_MBK02_SLOT_Object : public RPC_SLOT_Object
{
	Q_OBJECT
public:
	RPC_MBK02_SLOT_Object(QString _addr, int _port) : RPC_SLOT_Object(_addr, _port) {this->QObject::setObjectName("MBK02_slot_obj");}
	////////////////////////////////////
public slots:
	void new_message(QVariant dt, int mko, int line, int cwd, QVariantList words, int os);
	void new_ku_732(int ku_n, int length, double u, int line);
	void set_new_mbk02_tm();
	void new_KPI(QVariantList KPI_list);
	void auto_scroll_clicked(int _state);
	void update_tm(int sadr);
	void get_power(double volt);
	void update_graphics();
	////////////////////////////////////
};

class RPC_MBK02_SLOT_Thread : public RPC_SLOT_Thread
{
	Q_OBJECT
public:
	RPC_MBK02_SLOT_Thread() : RPC_SLOT_Thread() {this->QObject::setObjectName("MBK02_slot_thr");}
	void run();
	SLOT_OBJ_PTR get_obj() {return std::static_pointer_cast<RPC_SLOT_Object, RPC_MBK02_SLOT_Object>(rpc_obj);}
	std::shared_ptr<RPC_MBK02_SLOT_Object> get_MBK02_obj() {return rpc_obj;}
private:
	std::shared_ptr<RPC_MBK02_SLOT_Object> rpc_obj;

};

class RPC_MBK02_SIGNAL_Object : public RPC_SIGNAL_Object
{
	Q_OBJECT
public:
	RPC_MBK02_SIGNAL_Object(QString _addr, int _port) : RPC_SIGNAL_Object(_addr, _port)
	{
		this->QObject::setObjectName("MBK02_signal_obj");
	}
public slots:
	void read_data();
protected:
	void connectNotify(const QMetaMethod & signal);
	void disconnectNotify(const QMetaMethod & signal);
signals:
	void msg_to_14R732(QVariantList data);
	void set_new_tm(int sadr, int word);
	void set_new_power_tm(int sadr, QVariantList words);
	void emit_update_graphics();
};

class RPC_MBK02_SIGNAL_Thread : public RPC_SIGNAL_Thread
{
	Q_OBJECT
public:
	RPC_MBK02_SIGNAL_Thread() : RPC_SIGNAL_Thread() {this->QObject::setObjectName("MBK02_signal_thr");}
	void run();
	SIGNAL_OBJ_PTR get_obj() {return std::static_pointer_cast<RPC_SIGNAL_Object, RPC_MBK02_SIGNAL_Object>(rpc_obj);}
private:
	std::shared_ptr<RPC_MBK02_SIGNAL_Object> rpc_obj;
};

#endif
