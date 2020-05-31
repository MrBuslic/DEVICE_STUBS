#ifndef N737_SOCKET_RPC_H
#define N737_SOCKET_RPC_H

#include <QObject>
#include <QString>
#include <QTcpServer>
#include <QTcpSocket>
#include "N737.h"

#include <QWidget>
#include <QTextEdit>
#include <QWaitCondition>

#include "rpc_loger.h"
#include "socket_rpc.h"
class n737_Socket_RPC_SIGNAL_Object : public QObject
{
	Q_OBJECT
public:
	n737_Socket_RPC_SIGNAL_Object();
	~n737_Socket_RPC_SIGNAL_Object()
	{
	}
	void set_app(N737_widg* _app);

	void set_socket(QTcpSocket* _rpc_socket);
signals:
	void send_signal(QByteArray* _arr);
public slots:
	void new_ku(int ku_n, int length, double u, int line);

	void send_signal_slot(QByteArray* _arr);
	void read_data();
	void sock_error(QAbstractSocket::SocketError _err);
private:
	void send_signal_func(QByteArray* _arr);
	QTcpSocket* rpc_socket;
	QMutex signal_mutex;
	N737_widg* app;
	static int obj_num;
	static int call_number;
	QMap<QString, std::shared_ptr<SignalData> > data_map;
};


class n737_Socket_RPC_SIGNAL_Server : public QObject
{
	Q_OBJECT
public:
	n737_Socket_RPC_SIGNAL_Server(QString _conn_ip, int _conn_port);
	void set_app(N737_widg* _app)
	{
		app = _app;
	}
public slots:
	void tcp_slot();
private:
	QTcpServer* rpc_server;
	N737_widg* app;
	QList<std::shared_ptr<n737_Socket_RPC_SIGNAL_Object> > rpc_objects;
};

class n737_Socket_RPC_SIGNAL_Thread : public QThread
{
	Q_OBJECT
public:
	n737_Socket_RPC_SIGNAL_Thread();
	void set_app(N737_widg* _app)
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
	n737_Socket_RPC_SIGNAL_Server* rpc_srv;
	N737_widg* app;
	QString conn_ip;
	int conn_port;
};

class n737_Socket_RPC_SLOT_Object : public QObject
{
	Q_OBJECT
public:
	n737_Socket_RPC_SLOT_Object(N737_widg* _app, int socketDescriptor);
	~n737_Socket_RPC_SLOT_Object()
	{
	}
	typedef QVariant (n737_Socket_RPC_SLOT_Object::*OPERATOR_EXECUTOR)(QVariantList&);
	typedef QMap<QString, OPERATOR_EXECUTOR> OPERATORS_MAP;
public:
	QVariant QuerySlots(QVariantList& _values);
	QVariant new_frame(QVariantList& _values);
	QVariant new_message(QVariantList& _values);
	QVariant dataIn(QVariantList& _values);
public slots:
	void read_data();
	void sock_error(QAbstractSocket::SocketError _err);
private:
	OPERATORS_MAP operators_map;
	QTcpSocket* rpc_socket;
	N737_widg* app;
	bool with_return;
	static int obj_num;
};

class n737_Socket_RPC_SLOT_Thread : public QThread
{
	Q_OBJECT
public:
	n737_Socket_RPC_SLOT_Thread(N737_widg* _app, int _socketDescriptor);
	void run();
	std::shared_ptr<n737_Socket_RPC_SLOT_Object> get_obj(){ return rpc_obj; }
private:
	std::shared_ptr<n737_Socket_RPC_SLOT_Object> rpc_obj;
	N737_widg* app;
	int socketDescriptor;
	static int obj_num;
};

class n737_Socket_RPC_SLOT_Server : public QTcpServer
{
	Q_OBJECT
public:
	n737_Socket_RPC_SLOT_Server(QString _conn_ip, int _conn_port, N737_widg* _app);
protected:
	void incomingConnection(qintptr socketDescriptor) Q_DECL_OVERRIDE;
private:
	N737_widg* app;
	QList<std::shared_ptr<n737_Socket_RPC_SLOT_Thread> > rpc_objects;
};

class n737_Socket_RPC_SLOT_Server_Thread : public QThread
{
	Q_OBJECT
public:
	n737_Socket_RPC_SLOT_Server_Thread();
	void set_app(N737_widg* _app)
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
	n737_Socket_RPC_SLOT_Server* rpc_srv;
	N737_widg* app;
	QString conn_ip;
	int conn_port;

};
#endif
