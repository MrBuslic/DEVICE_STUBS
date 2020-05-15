#ifndef RTM3002_SOCKET_RPC_H
#define RTM3002_SOCKET_RPC_H

#include <QObject>
#include <QString>
#include <QTcpServer>
#include <QTcpSocket>
#include "RTM3002_imitator.h"

#include <QWidget>
#include <QTextEdit>
#include <QWaitCondition>

#include "rpc_loger.h"
#include "socket_rpc.h"
class RTM3002_Socket_RPC_SIGNAL_Object : public QObject
{
	Q_OBJECT
public:
	RTM3002_Socket_RPC_SIGNAL_Object();
	~RTM3002_Socket_RPC_SIGNAL_Object()
	{
	}
	void set_app(RTM3002_imitator* _app);

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
	RTM3002_imitator* app;
	static int obj_num;
	static int call_number;
	QMap<QString, std::shared_ptr<SignalData> > data_map;
};


class RTM3002_Socket_RPC_SIGNAL_Server : public QObject
{
	Q_OBJECT
public:
	RTM3002_Socket_RPC_SIGNAL_Server(QString _conn_ip, int _conn_port);
	void set_app(RTM3002_imitator* _app)
	{
		app = _app;
	}
public slots:
	void tcp_slot();
private:
	QTcpServer* rpc_server;
	RTM3002_imitator* app;
	QList<std::shared_ptr<RTM3002_Socket_RPC_SIGNAL_Object> > rpc_objects;
};

class RTM3002_Socket_RPC_SIGNAL_Thread : public QThread
{
	Q_OBJECT
public:
	RTM3002_Socket_RPC_SIGNAL_Thread();
	void set_app(RTM3002_imitator* _app)
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
	RTM3002_Socket_RPC_SIGNAL_Server* rpc_srv;
	RTM3002_imitator* app;
	QString conn_ip;
	int conn_port;
};

class RTM3002_Socket_RPC_SLOT_Object : public QObject
{
	Q_OBJECT
public:
	RTM3002_Socket_RPC_SLOT_Object(RTM3002_imitator* _app, int socketDescriptor);
	~RTM3002_Socket_RPC_SLOT_Object()
	{
	}
	typedef QVariant (RTM3002_Socket_RPC_SLOT_Object::*OPERATOR_EXECUTOR)(QVariantList&);
	typedef QMap<QString, OPERATOR_EXECUTOR> OPERATORS_MAP;
public:
	QVariant QuerySlots(QVariantList& _values);
	QVariant error_Slot(QVariantList& _values);
	QVariant read(QVariantList& _values);
	QVariant connect_ag(QVariantList& _values);
	QVariant read_ag(QVariantList& _values);
public slots:
	void read_data();
	void sock_error(QAbstractSocket::SocketError _err);
private:
	OPERATORS_MAP operators_map;
	QTcpSocket* rpc_socket;
	RTM3002_imitator* app;
	bool with_return;
	static int obj_num;
};

class RTM3002_Socket_RPC_SLOT_Thread : public QThread
{
	Q_OBJECT
public:
	RTM3002_Socket_RPC_SLOT_Thread(RTM3002_imitator* _app, int _socketDescriptor);
	void run();
	std::shared_ptr<RTM3002_Socket_RPC_SLOT_Object> get_obj(){ return rpc_obj; }
private:
	std::shared_ptr<RTM3002_Socket_RPC_SLOT_Object> rpc_obj;
	RTM3002_imitator* app;
	int socketDescriptor;
	static int obj_num;
};

class RTM3002_Socket_RPC_SLOT_Server : public QTcpServer
{
	Q_OBJECT
public:
	RTM3002_Socket_RPC_SLOT_Server(QString _conn_ip, int _conn_port, RTM3002_imitator* _app);
protected:
	void incomingConnection(qintptr socketDescriptor) Q_DECL_OVERRIDE;
private:
	RTM3002_imitator* app;
	QList<std::shared_ptr<RTM3002_Socket_RPC_SLOT_Thread> > rpc_objects;
};

class RTM3002_Socket_RPC_SLOT_Server_Thread : public QThread
{
	Q_OBJECT
public:
	RTM3002_Socket_RPC_SLOT_Server_Thread();
	void set_app(RTM3002_imitator* _app)
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
	RTM3002_Socket_RPC_SLOT_Server* rpc_srv;
	RTM3002_imitator* app;
	QString conn_ip;
	int conn_port;

};
#endif
