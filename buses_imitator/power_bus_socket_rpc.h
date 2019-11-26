#ifndef POWER_BUS_SOCKET_RPC_H
#define POWER_BUS_SOCKET_RPC_H

#include <QObject>
#include <QString>
#include <QTcpServer>
#include <QTcpSocket>
#include "power_bus.h"

#include <QWidget>
#include <QTextEdit>
#include <QWaitCondition>

#include "rpc_loger.h"
#include "socket_rpc.h"
class power_bus_Socket_RPC_SIGNAL_Object : public QObject
{
	Q_OBJECT
public:
	power_bus_Socket_RPC_SIGNAL_Object();
	~power_bus_Socket_RPC_SIGNAL_Object()
	{
	}
	void set_app(PowerWidget* _app);

	void set_socket(QTcpSocket* _rpc_socket);
signals:
	void send_signal(QByteArray* _arr);
public slots:
	void u_on_nk(double volt);
	void u_on_k1(double volt);
	void u_on_k2(double volt);

	void send_signal_slot(QByteArray* _arr);
	void read_data();
	void sock_error(QAbstractSocket::SocketError _err);
private:
	void send_signal_func(QByteArray* _arr);
	QTcpSocket* rpc_socket;
	QMutex signal_mutex;
	PowerWidget* app;
	static int obj_num;
	static int call_number;
	QMap<QString, std::shared_ptr<SignalData> > data_map;
};


class power_bus_Socket_RPC_SIGNAL_Server : public QObject
{
	Q_OBJECT
public:
	power_bus_Socket_RPC_SIGNAL_Server(QString _conn_ip, int _conn_port);
	void set_app(PowerWidget* _app)
	{
		app = _app;
	}
public slots:
	void tcp_slot();
private:
	QTcpServer* rpc_server;
	PowerWidget* app;
	QList<std::shared_ptr<power_bus_Socket_RPC_SIGNAL_Object> > rpc_objects;
};

class power_bus_Socket_RPC_SIGNAL_Thread : public QThread
{
	Q_OBJECT
public:
	power_bus_Socket_RPC_SIGNAL_Thread();
	void set_app(PowerWidget* _app)
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
	power_bus_Socket_RPC_SIGNAL_Server* rpc_srv;
	PowerWidget* app;
	QString conn_ip;
	int conn_port;
};

class power_bus_Socket_RPC_SLOT_Object : public QObject
{
	Q_OBJECT
public:
	power_bus_Socket_RPC_SLOT_Object(PowerWidget* _app, int socketDescriptor);
	~power_bus_Socket_RPC_SLOT_Object()
	{
	}
	typedef QVariant (power_bus_Socket_RPC_SLOT_Object::*OPERATOR_EXECUTOR)(QVariantList&);
	typedef QMap<QString, OPERATOR_EXECUTOR> OPERATORS_MAP;
public:
	QVariant QuerySlots(QVariantList& _values);
	QVariant set_u(QVariantList& _values);
	QVariant get_i(QVariantList& _values);
	QVariant set_i(QVariantList& _values);
	QVariant set_bus_state(QVariantList& _values);
public slots:
	void read_data();
	void sock_error(QAbstractSocket::SocketError _err);
private:
	OPERATORS_MAP operators_map;
	QTcpSocket* rpc_socket;
	PowerWidget* app;
	bool with_return;
	static int obj_num;
};

class power_bus_Socket_RPC_SLOT_Thread : public QThread
{
	Q_OBJECT
public:
	power_bus_Socket_RPC_SLOT_Thread(PowerWidget* _app, int _socketDescriptor);
	void run();
	std::shared_ptr<power_bus_Socket_RPC_SLOT_Object> get_obj(){ return rpc_obj; }
	private:
	std::shared_ptr<power_bus_Socket_RPC_SLOT_Object> rpc_obj;
	PowerWidget* app;
	int socketDescriptor;
};

class power_bus_Socket_RPC_SLOT_Server : public QTcpServer
{
	Q_OBJECT
public:
	power_bus_Socket_RPC_SLOT_Server(QString _conn_ip, int _conn_port, PowerWidget* _app);
protected:
	void incomingConnection(qintptr socketDescriptor) Q_DECL_OVERRIDE;
private:
	PowerWidget* app;
	QList<std::shared_ptr<power_bus_Socket_RPC_SLOT_Thread> > rpc_objects;
};

class power_bus_Socket_RPC_SLOT_Server_Thread : public QThread
{
	Q_OBJECT
public:
	power_bus_Socket_RPC_SLOT_Server_Thread();
	void set_app(PowerWidget* _app)
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
	power_bus_Socket_RPC_SLOT_Server* rpc_srv;
	PowerWidget* app;
	QString conn_ip;
	int conn_port;

};
#endif
