#ifndef LKA05_RPC_H
#define LKA05_RPC_H
#include "socket_rpc.h"


class RPC_lka05_SLOT_Object : public RPC_SLOT_Object
{
	Q_OBJECT
public:
	RPC_lka05_SLOT_Object(QString _addr, int _port) : RPC_SLOT_Object(_addr, _port) {this->QObject::setObjectName("lka05_slot_obj");}
	////////////////////////////////////
public slots:
	void new_message(QVariant dt, int mko, int line, int cwd, QVariantList words, int os);
	void new_tm(int tm);
	void new_ku_732(int ku_n, int length, double u, int line);
	void get_power(double _volt);
	int set_mu_working(int _dev, bool _flag);
	int set_ku_working(int module_num, int _dev, bool _flag);
	int set_mk_working(int module_num, int _dev, bool _flag);
	int set_mpvn_working(int _dev, bool _flag);
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
	}
public slots:
	void read_data();
protected:
	void connectNotify(const QMetaMethod & signal);
	void disconnectNotify(const QMetaMethod & signal);
signals:
	void new_ku(int ku_n, int length, double u, int line);
	void new_mk(int mshm, int pshm, int length_m, int length_p, double u_m, double u_p, int dt, int line_m, int line_p);
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
