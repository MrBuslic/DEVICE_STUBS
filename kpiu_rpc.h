#ifndef KPIU_RPC_H
#define KPIU_RPC_H
#include "socket_rpc.h"


class RPC_kpiu_SLOT_Object : public RPC_SLOT_Object
{
	Q_OBJECT
public:
	RPC_kpiu_SLOT_Object(QString _addr, int _port) : RPC_SLOT_Object(_addr, _port) {this->QObject::setObjectName("kpiu_slot_obj");}
	////////////////////////////////////
public slots:
	int KU_NASTROYKA_CELOSTNOSTI_KANALOV(int ku_n, int line);
	int MSHM_NASTROYKA_CELOSTNOSTI_KANALOV(int mshm, int line_m);
	int PSHM_NASTROYKA_CELOSTNOSTI_KANALOV(int pshm, int line_p);
	int FOI_NASTROYKA_CELOSTNOSTI_KANALOV(int _n, int _chan);
	int OLS_ZAPIS_DANNIH_FORMIROVANIYA(QVariantList data_buffer, QVariantList mask_buffer);
	int OLS_ZAPIS_DANNIH_REGISTRACII(QVariantList data_buffer);
	int OLS_BISTRIY_START(int devise);
	int OLS_CHTENIE_DANNICH_REGISTRACII(QVariantList& data_buffer);
	int ANTENNA_USTANOVKA_KOMMUTACII(QString antenna_name, QString connected_antenna_name);
	int OMNIBUS_NASTROYKA_CELOSTNOSTI_KANALOV(int _n, int _chan);
	int USTANOVIT_SOSTOYANIE_SHINI_PITANIYA(int bus, int state);
	int PYRO_USTANOVIT_SOSTOYANIE(QString name, int state);
	QString getXML();
	void mfsk_1_impulse(QVariantList channels);
	void mds_1_get_sample(uint& buf, bool& flag);
	void mds_2_get_sample(uint& buf, bool& flag);
	void get_resistance(uint NProcess, QVariant& resistance);
	int get_connection_state(QVariantList& _chans);
	////////////////////////////////////
};

class RPC_kpiu_SLOT_Thread : public RPC_SLOT_Thread
{
	Q_OBJECT
public:
	RPC_kpiu_SLOT_Thread() : RPC_SLOT_Thread() {this->QObject::setObjectName("kpiu_slot_thr");}
	void run();
	SLOT_OBJ_PTR get_obj() {return std::static_pointer_cast<RPC_SLOT_Object, RPC_kpiu_SLOT_Object>(rpc_obj);}
	std::shared_ptr<RPC_kpiu_SLOT_Object> get_kpiu_obj() {return rpc_obj;}
private:
	std::shared_ptr<RPC_kpiu_SLOT_Object> rpc_obj;

};

class RPC_kpiu_SIGNAL_Object : public RPC_SIGNAL_Object
{
	Q_OBJECT
public:
	RPC_kpiu_SIGNAL_Object(QString _addr, int _port) : RPC_SIGNAL_Object(_addr, _port)
	{
		this->QObject::setObjectName("kpiu_signal_obj");
	}
public slots:
	void read_data();
protected:
	void connectNotify(const QMetaMethod & signal);
	void disconnectNotify(const QMetaMethod & signal);
signals:
	void string_msg(QString _msg);
	void int_msg(int _msg);
	void int_return_signal(int& ret_int);
	void toLogs(QString message);
	void toProtocols(QString message);
};

class RPC_kpiu_SIGNAL_Thread : public RPC_SIGNAL_Thread
{
	Q_OBJECT
public:
	RPC_kpiu_SIGNAL_Thread() : RPC_SIGNAL_Thread() {this->QObject::setObjectName("kpiu_signal_thr");}
	void run();
	SIGNAL_OBJ_PTR get_obj() {return std::static_pointer_cast<RPC_SIGNAL_Object, RPC_kpiu_SIGNAL_Object>(rpc_obj);}
private:
	std::shared_ptr<RPC_kpiu_SIGNAL_Object> rpc_obj;
};

#endif
