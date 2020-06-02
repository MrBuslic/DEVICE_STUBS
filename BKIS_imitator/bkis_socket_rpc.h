#ifndef BKIS_SOCKET_RPC_H
#define BKIS_SOCKET_RPC_H

#include <QObject>
#include <QString>
#include <QTcpServer>
#include <QTcpSocket>
#include "BKIS.h"

#include <QWidget>
#include <QTextEdit>
#include <QWaitCondition>

#include "rpc_loger.h"
#include "socket_rpc.h"
class bkis_Socket_RPC_SIGNAL_Object : public QObject
{
	Q_OBJECT
public:
	bkis_Socket_RPC_SIGNAL_Object();
	~bkis_Socket_RPC_SIGNAL_Object()
	{
	}
	void set_app(BKIS_widg* _app);

	void set_socket(QTcpSocket* _rpc_socket);
signals:
	void send_signal(QByteArray* _arr);
public slots:
	void send_pyro_group_activation(int group_num);

	void send_signal_slot(QByteArray* _arr);
	void read_data();
	void sock_error(QAbstractSocket::SocketError _err);
private:
	void send_signal_func(QByteArray* _arr);
	QTcpSocket* rpc_socket;
	QMutex signal_mutex;
	BKIS_widg* app;
	static int obj_num;
	static int call_number;
	QMap<QString, std::shared_ptr<SignalData> > data_map;
};


class bkis_Socket_RPC_SIGNAL_Server : public QObject
{
	Q_OBJECT
public:
	bkis_Socket_RPC_SIGNAL_Server(QString _conn_ip, int _conn_port);
	void set_app(BKIS_widg* _app)
	{
		app = _app;
	}
public slots:
	void tcp_slot();
private:
	QTcpServer* rpc_server;
	BKIS_widg* app;
	QList<std::shared_ptr<bkis_Socket_RPC_SIGNAL_Object> > rpc_objects;
};

class bkis_Socket_RPC_SIGNAL_Thread : public QThread
{
	Q_OBJECT
public:
	bkis_Socket_RPC_SIGNAL_Thread();
	void set_app(BKIS_widg* _app)
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
	bkis_Socket_RPC_SIGNAL_Server* rpc_srv;
	BKIS_widg* app;
	QString conn_ip;
	int conn_port;
};

class bkis_Socket_RPC_SLOT_Object : public QObject
{
	Q_OBJECT
public:
	bkis_Socket_RPC_SLOT_Object(BKIS_widg* _app, int socketDescriptor);
	~bkis_Socket_RPC_SLOT_Object()
	{
	}
	typedef QVariant (bkis_Socket_RPC_SLOT_Object::*OPERATOR_EXECUTOR)(QVariantList&);
	typedef QMap<QString, OPERATOR_EXECUTOR> OPERATORS_MAP;
public:
	QVariant QuerySlots(QVariantList& _values);
	QVariant new_message(QVariantList& _values);
	QVariant make_ku(QVariantList& _values);
	QVariant get_power(QVariantList& _values);
	QVariant set_blk_state(QVariantList& _values);
	QVariant set_interface_state(QVariantList& _values);
	QVariant set_electric_heater_state(QVariantList& _values);
	QVariant set_pyro_group_state(QVariantList& _values);
	QVariant set_power_back(QVariantList& _values);
	QVariant omni_connect(QVariantList& _values);
	QVariant set_pyro_bus_state(QVariantList& _values);
public slots:
	void read_data();
	void sock_error(QAbstractSocket::SocketError _err);
private:
	OPERATORS_MAP operators_map;
	QTcpSocket* rpc_socket;
	BKIS_widg* app;
	bool with_return;
	static int obj_num;
};

class bkis_Socket_RPC_SLOT_Thread : public QThread
{
	Q_OBJECT
public:
	bkis_Socket_RPC_SLOT_Thread(BKIS_widg* _app, int _socketDescriptor);
	void run();
	std::shared_ptr<bkis_Socket_RPC_SLOT_Object> get_obj(){ return rpc_obj; }
private:
	std::shared_ptr<bkis_Socket_RPC_SLOT_Object> rpc_obj;
	BKIS_widg* app;
	int socketDescriptor;
	static int obj_num;
};

class bkis_Socket_RPC_SLOT_Server : public QTcpServer
{
	Q_OBJECT
public:
	bkis_Socket_RPC_SLOT_Server(QString _conn_ip, int _conn_port, BKIS_widg* _app);
protected:
	void incomingConnection(qintptr socketDescriptor) Q_DECL_OVERRIDE;
private:
	BKIS_widg* app;
	QList<std::shared_ptr<bkis_Socket_RPC_SLOT_Thread> > rpc_objects;
};

class bkis_Socket_RPC_SLOT_Server_Thread : public QThread
{
	Q_OBJECT
public:
	bkis_Socket_RPC_SLOT_Server_Thread();
	void set_app(BKIS_widg* _app)
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
	bkis_Socket_RPC_SLOT_Server* rpc_srv;
	BKIS_widg* app;
	QString conn_ip;
	int conn_port;

};
#endif
