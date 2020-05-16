#ifndef KPIU_SOCKET_RPC_H
#define KPIU_SOCKET_RPC_H

#include <QObject>
#include <QString>
#include <QTcpServer>
#include <QTcpSocket>
#include "kpiu_server.h"

#include <QWidget>
#include <QTextEdit>
#include <QWaitCondition>

#include "rpc_loger.h"
#include "socket_rpc.h"
class kpiu_Socket_RPC_SIGNAL_Object : public QObject
{
	Q_OBJECT
public:
	kpiu_Socket_RPC_SIGNAL_Object();
	~kpiu_Socket_RPC_SIGNAL_Object()
	{
	}
	void set_app(KPIUServer* _app);

	void set_socket(QTcpSocket* _rpc_socket);
signals:
	void send_signal(QByteArray* _arr);
public slots:
	void string_msg(QString _msg);
	void int_msg(int _msg);
	void int_return_signal(int& ret_int);
	void toLogs(QString message);
	void toProtocols(QString message);

	void send_signal_slot(QByteArray* _arr);
	void read_data();
	void sock_error(QAbstractSocket::SocketError _err);
private:
	void send_signal_func(QByteArray* _arr);
	QTcpSocket* rpc_socket;
	QMutex signal_mutex;
	KPIUServer* app;
	static int obj_num;
	static int call_number;
	QMap<QString, std::shared_ptr<SignalData> > data_map;
};


class kpiu_Socket_RPC_SIGNAL_Server : public QObject
{
	Q_OBJECT
public:
	kpiu_Socket_RPC_SIGNAL_Server(QString _conn_ip, int _conn_port);
	void set_app(KPIUServer* _app)
	{
		app = _app;
	}
public slots:
	void tcp_slot();
private:
	QTcpServer* rpc_server;
	KPIUServer* app;
	QList<std::shared_ptr<kpiu_Socket_RPC_SIGNAL_Object> > rpc_objects;
};

class kpiu_Socket_RPC_SIGNAL_Thread : public QThread
{
	Q_OBJECT
public:
	kpiu_Socket_RPC_SIGNAL_Thread();
	void set_app(KPIUServer* _app)
	{
		app = _app;
	}
	void set_params(QString _conn_ip, int _conn_port)
	{
		conn_ip = _conn_ip;
		conn_port = _conn_port;
	}
	void run();
private:
	kpiu_Socket_RPC_SIGNAL_Server* rpc_srv;
	KPIUServer* app;
	QString conn_ip;
	int conn_port;
};

class kpiu_Socket_RPC_SLOT_Object : public QObject
{
	Q_OBJECT
public:
	kpiu_Socket_RPC_SLOT_Object(KPIUServer* _app, int socketDescriptor);
	~kpiu_Socket_RPC_SLOT_Object()
	{
	}
	typedef QVariant (kpiu_Socket_RPC_SLOT_Object::*OPERATOR_EXECUTOR)(QVariantList&);
	typedef QMap<QString, OPERATOR_EXECUTOR> OPERATORS_MAP;
public:
	QVariant QuerySlots(QVariantList& _values);
	QVariant KU_NASTROYKA_CELOSTNOSTI_KANALOV(QVariantList& _values);
	QVariant MSHM_NASTROYKA_CELOSTNOSTI_KANALOV(QVariantList& _values);
	QVariant PSHM_NASTROYKA_CELOSTNOSTI_KANALOV(QVariantList& _values);
	QVariant FOI_NASTROYKA_CELOSTNOSTI_KANALOV(QVariantList& _values);
	QVariant OLS_ZAPIS_DANNIH_FORMIROVANIYA(QVariantList& _values);
	QVariant OLS_ZAPIS_DANNIH_REGISTRACII(QVariantList& _values);
	QVariant OLS_BISTRIY_START(QVariantList& _values);
	QVariant OLS_CHTENIE_DANNICH_REGISTRACII(QVariantList& _values);
	QVariant ANTENNA_USTANOVKA_KOMMUTACII(QVariantList& _values);
	QVariant OMNIBUS_NASTROYKA_CELOSTNOSTI_KANALOV(QVariantList& _values);
	QVariant USTANOVIT_SOSTOYANIE_SHINI_PITANIYA(QVariantList& _values);
	QVariant PYRO_USTANOVIT_SOSTOYANIE(QVariantList& _values);
	QVariant getXML(QVariantList& _values);
	QVariant mfsk_2_impulse(QVariantList& _values);
	QVariant mds_1_get_sample(QVariantList& _values);
	QVariant mds_2_get_sample(QVariantList& _values);
	QVariant get_resistance(QVariantList& _values);
	QVariant get_connection_state(QVariantList& _values);
	QVariant mshr_data_in(QVariantList& _values);
	QVariant mshr_data_out(QVariantList& _values);
	QVariant power_bus_state_changed(QVariantList& _values);
public slots:
	void read_data();
	void sock_error(QAbstractSocket::SocketError _err);
private:
	OPERATORS_MAP operators_map;
	QTcpSocket* rpc_socket;
	KPIUServer* app;
	bool with_return;
	static int obj_num;
};

class kpiu_Socket_RPC_SLOT_Thread : public QThread
{
	Q_OBJECT
public:
	kpiu_Socket_RPC_SLOT_Thread(KPIUServer* _app, int _socketDescriptor);
	void run();
	std::shared_ptr<kpiu_Socket_RPC_SLOT_Object> get_obj(){ return rpc_obj; }
private:
	std::shared_ptr<kpiu_Socket_RPC_SLOT_Object> rpc_obj;
	KPIUServer* app;
	int socketDescriptor;
	static int obj_num;
};

class kpiu_Socket_RPC_SLOT_Server : public QTcpServer
{
	Q_OBJECT
public:
	kpiu_Socket_RPC_SLOT_Server(QString _conn_ip, int _conn_port, KPIUServer* _app);
protected:
	void incomingConnection(qintptr socketDescriptor) Q_DECL_OVERRIDE;
private:
	KPIUServer* app;
	QList<std::shared_ptr<kpiu_Socket_RPC_SLOT_Thread> > rpc_objects;
};

class kpiu_Socket_RPC_SLOT_Server_Thread : public QThread
{
	Q_OBJECT
public:
	kpiu_Socket_RPC_SLOT_Server_Thread();
	void set_app(KPIUServer* _app)
	{
		app = _app;
	}
	void set_params(QString _conn_ip, int _conn_port)
	{
		conn_ip = _conn_ip;
		conn_port = _conn_port;
	}
	void run();
private:
	kpiu_Socket_RPC_SLOT_Server* rpc_srv;
	KPIUServer* app;
	QString conn_ip;
	int conn_port;

};
#endif
