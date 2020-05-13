#ifndef IS4_SOCKET_RPC_H
#define IS4_SOCKET_RPC_H

#include <QObject>
#include <QString>
#include <QTcpServer>
#include <QTcpSocket>
#include "rpc_is4.h"

#include <QWidget>
#include <QTextEdit>
#include <QWaitCondition>

#include "rpc_loger.h"
#include "socket_rpc.h"
class is4_Socket_RPC_SIGNAL_Object : public QObject
{
	Q_OBJECT
public:
	is4_Socket_RPC_SIGNAL_Object();
	~is4_Socket_RPC_SIGNAL_Object()
	{
	}
	void set_app(RpcIS4Widget* _app);

	void set_socket(QTcpSocket* _rpc_socket);
signals:
	void send_signal(QByteArray* _arr);
public slots:
	void is4_measure(uint NProcess, QVariant& value);

	void send_signal_slot(QByteArray* _arr);
	void read_data();
	void sock_error(QAbstractSocket::SocketError _err);
private:
	void send_signal_func(QByteArray* _arr);
	QTcpSocket* rpc_socket;
	QMutex signal_mutex;
	RpcIS4Widget* app;
	static int obj_num;
	static int call_number;
	QMap<QString, std::shared_ptr<SignalData> > data_map;
};


class is4_Socket_RPC_SIGNAL_Server : public QObject
{
	Q_OBJECT
public:
	is4_Socket_RPC_SIGNAL_Server(QString _conn_ip, int _conn_port);
	void set_app(RpcIS4Widget* _app)
	{
		app = _app;
	}
public slots:
	void tcp_slot();
private:
	QTcpServer* rpc_server;
	RpcIS4Widget* app;
	QList<std::shared_ptr<is4_Socket_RPC_SIGNAL_Object> > rpc_objects;
};

class is4_Socket_RPC_SIGNAL_Thread : public QThread
{
	Q_OBJECT
public:
	is4_Socket_RPC_SIGNAL_Thread();
	void set_app(RpcIS4Widget* _app)
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
	is4_Socket_RPC_SIGNAL_Server* rpc_srv;
	RpcIS4Widget* app;
	QString conn_ip;
	int conn_port;
};

class is4_Socket_RPC_SLOT_Object : public QObject
{
	Q_OBJECT
public:
	is4_Socket_RPC_SLOT_Object(RpcIS4Widget* _app, int socketDescriptor);
	~is4_Socket_RPC_SLOT_Object()
	{
	}
	typedef QVariant (is4_Socket_RPC_SLOT_Object::*OPERATOR_EXECUTOR)(QVariantList&);
	typedef QMap<QString, OPERATOR_EXECUTOR> OPERATORS_MAP;
public:
	QVariant QuerySlots(QVariantList& _values);
	QVariant is4_clicked(QVariantList& _values);
	QVariant unis4_SetTypeProcess(QVariantList& _values);
	QVariant unis4_StartACP(QVariantList& _values);
	QVariant unis4_ResultMeas(QVariantList& _values);
	QVariant unis4_RangeMeas(QVariantList& _values);
	QVariant unis4_StartCalibr(QVariantList& _values);
public slots:
	void read_data();
	void sock_error(QAbstractSocket::SocketError _err);
private:
	OPERATORS_MAP operators_map;
	QTcpSocket* rpc_socket;
	RpcIS4Widget* app;
	bool with_return;
	static int obj_num;
};

class is4_Socket_RPC_SLOT_Thread : public QThread
{
	Q_OBJECT
public:
	is4_Socket_RPC_SLOT_Thread(RpcIS4Widget* _app, int _socketDescriptor);
	void run();
	std::shared_ptr<is4_Socket_RPC_SLOT_Object> get_obj(){ return rpc_obj; }
private:
	std::shared_ptr<is4_Socket_RPC_SLOT_Object> rpc_obj;
	RpcIS4Widget* app;
	int socketDescriptor;
	static int obj_num;
};

class is4_Socket_RPC_SLOT_Server : public QTcpServer
{
	Q_OBJECT
public:
	is4_Socket_RPC_SLOT_Server(QString _conn_ip, int _conn_port, RpcIS4Widget* _app);
protected:
	void incomingConnection(qintptr socketDescriptor) Q_DECL_OVERRIDE;
private:
	RpcIS4Widget* app;
	QList<std::shared_ptr<is4_Socket_RPC_SLOT_Thread> > rpc_objects;
};

class is4_Socket_RPC_SLOT_Server_Thread : public QThread
{
	Q_OBJECT
public:
	is4_Socket_RPC_SLOT_Server_Thread();
	void set_app(RpcIS4Widget* _app)
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
	is4_Socket_RPC_SLOT_Server* rpc_srv;
	RpcIS4Widget* app;
	QString conn_ip;
	int conn_port;

};
#endif
