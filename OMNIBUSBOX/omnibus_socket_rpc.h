#ifndef OMNIBUS_SOCKET_RPC_H
#define OMNIBUS_SOCKET_RPC_H

#include <QObject>
#include <QString>
#include <QTcpServer>
#include <QTcpSocket>
#include "OMNIBUSBOX.h"

#include <QWidget>
#include <QTextEdit>
#include <QWaitCondition>

#include "rpc_loger.h"
#include "socket_rpc.h"
class omnibus_Socket_RPC_SIGNAL_Object : public QObject
{
	Q_OBJECT
public:
	omnibus_Socket_RPC_SIGNAL_Object();
	~omnibus_Socket_RPC_SIGNAL_Object()
	{
	}
	void set_app(RpcOmnibusWidget* _app);

	void set_socket(QTcpSocket* _rpc_socket);
signals:
	void send_signal(QByteArray* _arr);
public slots:
	void new_message(QVariant dt, int mko, int line, int cwd, QVariantList words, int os);
	void new_message_mpko(QVariant dt, int mko, int line, int cwd, QVariantList words, int os);
	void message_to_log(QString _msg);

	void send_signal_slot(QByteArray* _arr);
	void read_data();
	void sock_error(QAbstractSocket::SocketError _err);
private:
	void send_signal_func(QByteArray* _arr);
	QTcpSocket* rpc_socket;
	QMutex signal_mutex;
	RpcOmnibusWidget* app;
	static int obj_num;
	static int call_number;
	QMap<QString, std::shared_ptr<SignalData> > data_map;
};


class omnibus_Socket_RPC_SIGNAL_Server : public QObject
{
	Q_OBJECT
public:
	omnibus_Socket_RPC_SIGNAL_Server(QString _conn_ip, int _conn_port);
	void set_app(RpcOmnibusWidget* _app)
	{
		app = _app;
	}
public slots:
	void tcp_slot();
private:
	QTcpServer* rpc_server;
	RpcOmnibusWidget* app;
	QList<std::shared_ptr<omnibus_Socket_RPC_SIGNAL_Object> > rpc_objects;
};

class omnibus_Socket_RPC_SIGNAL_Thread : public QThread
{
	Q_OBJECT
public:
	omnibus_Socket_RPC_SIGNAL_Thread();
	void set_app(RpcOmnibusWidget* _app)
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
	omnibus_Socket_RPC_SIGNAL_Server* rpc_srv;
	RpcOmnibusWidget* app;
	QString conn_ip;
	int conn_port;
};

class omnibus_Socket_RPC_SLOT_Object : public QObject
{
	Q_OBJECT
public:
	omnibus_Socket_RPC_SLOT_Object(RpcOmnibusWidget* _app, int socketDescriptor);
	~omnibus_Socket_RPC_SLOT_Object()
	{
	}
	typedef QVariant (omnibus_Socket_RPC_SLOT_Object::*OPERATOR_EXECUTOR)(QVariantList&);
	typedef QMap<QString, OPERATOR_EXECUTOR> OPERATORS_MAP;
public:
	QVariant QuerySlots(QVariantList& _values);
	QVariant switch_ab_os(QVariantList& _values);
	QVariant switch_ab(QVariantList& _values);
	QVariant set_new_data(QVariantList& _values);
	QVariant set_new_data_f5(QVariantList& _values);
	QVariant send_msg(QVariantList& _values);
	QVariant send_msg_mpko(QVariantList& _values);
	QVariant unomnibus_map_channels_setup(QVariantList& _values);
	QVariant get_dt(QVariantList& _values);
public slots:
	void read_data();
	void sock_error(QAbstractSocket::SocketError _err);
private:
	OPERATORS_MAP operators_map;
	QTcpSocket* rpc_socket;
	RpcOmnibusWidget* app;
	bool with_return;
	static int obj_num;
};

class omnibus_Socket_RPC_SLOT_Thread : public QThread
{
	Q_OBJECT
public:
	omnibus_Socket_RPC_SLOT_Thread(RpcOmnibusWidget* _app, int _socketDescriptor);
	void run();
	std::shared_ptr<omnibus_Socket_RPC_SLOT_Object> get_obj(){ return rpc_obj; }
private:
	std::shared_ptr<omnibus_Socket_RPC_SLOT_Object> rpc_obj;
	RpcOmnibusWidget* app;
	int socketDescriptor;
	static int obj_num;
};

class omnibus_Socket_RPC_SLOT_Server : public QTcpServer
{
	Q_OBJECT
public:
	omnibus_Socket_RPC_SLOT_Server(QString _conn_ip, int _conn_port, RpcOmnibusWidget* _app);
protected:
	void incomingConnection(qintptr socketDescriptor) Q_DECL_OVERRIDE;
private:
	RpcOmnibusWidget* app;
	QList<std::shared_ptr<omnibus_Socket_RPC_SLOT_Thread> > rpc_objects;
};

class omnibus_Socket_RPC_SLOT_Server_Thread : public QThread
{
	Q_OBJECT
public:
	omnibus_Socket_RPC_SLOT_Server_Thread();
	void set_app(RpcOmnibusWidget* _app)
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
	omnibus_Socket_RPC_SLOT_Server* rpc_srv;
	RpcOmnibusWidget* app;
	QString conn_ip;
	int conn_port;

};
#endif
