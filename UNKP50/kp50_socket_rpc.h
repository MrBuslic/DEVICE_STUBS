#ifndef KP50_SOCKET_RPC_H
#define KP50_SOCKET_RPC_H

#include <QObject>
#include <QString>
#include <QTcpServer>
#include <QTcpSocket>
#include "rpc_kp50.h"

#include <QWidget>
#include <QTextEdit>
#include <QWaitCondition>

#include "rpc_loger.h"
#include "socket_rpc.h"
class kp50_Socket_RPC_SIGNAL_Object : public QObject
{
	Q_OBJECT
public:
	kp50_Socket_RPC_SIGNAL_Object();
	~kp50_Socket_RPC_SIGNAL_Object()
	{
	}
	void set_app(RpcKP50Widget* _app);

	void set_socket(QTcpSocket* _rpc_socket);
signals:
	void send_signal(QByteArray* _arr);
public slots:

	void send_signal_slot(QByteArray* _arr);
	void read_data();
	void sock_error(QAbstractSocket::SocketError _err);
private:
	void send_signal_func(QByteArray* _arr);
	QTcpSocket* rpc_socket;
	QMutex signal_mutex;
	RpcKP50Widget* app;
	static int obj_num;
	static int call_number;
	QMap<QString, std::shared_ptr<SignalData> > data_map;
};


class kp50_Socket_RPC_SIGNAL_Server : public QObject
{
	Q_OBJECT
public:
	kp50_Socket_RPC_SIGNAL_Server(QString _conn_ip, int _conn_port);
	void set_app(RpcKP50Widget* _app)
	{
		app = _app;
	}
public slots:
	void tcp_slot();
private:
	QTcpServer* rpc_server;
	RpcKP50Widget* app;
	QList<std::shared_ptr<kp50_Socket_RPC_SIGNAL_Object> > rpc_objects;
};

class kp50_Socket_RPC_SIGNAL_Thread : public QThread
{
	Q_OBJECT
public:
	kp50_Socket_RPC_SIGNAL_Thread();
	void set_app(RpcKP50Widget* _app)
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
	kp50_Socket_RPC_SIGNAL_Server* rpc_srv;
	RpcKP50Widget* app;
	QString conn_ip;
	int conn_port;
};

class kp50_Socket_RPC_SLOT_Object : public QObject
{
	Q_OBJECT
public:
	kp50_Socket_RPC_SLOT_Object(RpcKP50Widget* _app, int socketDescriptor);
	~kp50_Socket_RPC_SLOT_Object()
	{
	}
	typedef QVariant (kp50_Socket_RPC_SLOT_Object::*OPERATOR_EXECUTOR)(QVariantList&);
	typedef QMap<QString, OPERATOR_EXECUTOR> OPERATORS_MAP;
public:
	QVariant QuerySlots(QVariantList& _values);
	QVariant auto_scroll_clicked(QVariantList& _values);
	QVariant log_timer_ontimer(QVariantList& _values);
	QVariant set_u_in(QVariantList& _values);
	QVariant unkp50_switch_channel(QVariantList& _values);
	QVariant unkp50_channel_state_Q(QVariantList& _values);
	QVariant unkp50_meas_I(QVariantList& _values);
	QVariant unkp50_meas_Uin(QVariantList& _values);
	QVariant unkp50_meas_Uout(QVariantList& _values);
public slots:
	void read_data();
	void sock_error(QAbstractSocket::SocketError _err);
private:
	OPERATORS_MAP operators_map;
	QTcpSocket* rpc_socket;
	RpcKP50Widget* app;
	bool with_return;
	static int obj_num;
};

class kp50_Socket_RPC_SLOT_Thread : public QThread
{
	Q_OBJECT
public:
	kp50_Socket_RPC_SLOT_Thread(RpcKP50Widget* _app, int _socketDescriptor);
	void run();
	std::shared_ptr<kp50_Socket_RPC_SLOT_Object> get_obj(){ return rpc_obj; }
	private:
	std::shared_ptr<kp50_Socket_RPC_SLOT_Object> rpc_obj;
	RpcKP50Widget* app;
	int socketDescriptor;
};

class kp50_Socket_RPC_SLOT_Server : public QTcpServer
{
	Q_OBJECT
public:
	kp50_Socket_RPC_SLOT_Server(QString _conn_ip, int _conn_port, RpcKP50Widget* _app);
protected:
	void incomingConnection(qintptr socketDescriptor) Q_DECL_OVERRIDE;
private:
	RpcKP50Widget* app;
	QList<std::shared_ptr<kp50_Socket_RPC_SLOT_Thread> > rpc_objects;
};

class kp50_Socket_RPC_SLOT_Server_Thread : public QThread
{
	Q_OBJECT
public:
	kp50_Socket_RPC_SLOT_Server_Thread();
	void set_app(RpcKP50Widget* _app)
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
	kp50_Socket_RPC_SLOT_Server* rpc_srv;
	RpcKP50Widget* app;
	QString conn_ip;
	int conn_port;

};
#endif
