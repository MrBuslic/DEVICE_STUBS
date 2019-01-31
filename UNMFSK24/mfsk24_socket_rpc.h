#ifndef MFSK24_SOCKET_RPC_H
#define MFSK24_SOCKET_RPC_H

#include <QObject>
#include <QString>
#include <QTcpServer>
#include <QTcpSocket>
#include "rpc_mfsk24.h"

#include <QWidget>
#include <QTextEdit>
#include <QWaitCondition>

#include "rpc_loger.h"
struct SignalData
{
	SignalData() : signal_needed(false) {}
	SignalData(const SignalData&) {}
	QWaitCondition cond;
	QMutex mutex;
	QVariantList signal_data;
	bool signal_needed;
};
class Socket_RPC_SIGNAL_Object : public QObject
{
	Q_OBJECT
public:
	Socket_RPC_SIGNAL_Object();
	~Socket_RPC_SIGNAL_Object()
	{
	}
	void set_app(RpcMFSK24Widget* _app);

	void set_socket(QTcpSocket* _rpc_socket);
signals:
	void send_signal(QByteArray* _arr);
public slots:
	void mfsk24_();
	void mfsk24_state_change(int channel, int state);
	void mfsk24_impulse_change(int channel, int duration);

	void send_signal_slot(QByteArray* _arr);
	void read_data();
	void sock_error(QAbstractSocket::SocketError _err);
private:
	void send_signal_func(QByteArray* _arr);
	QTcpSocket* rpc_socket;
	QMutex signal_mutex;
	RpcMFSK24Widget* app;
	static int obj_num;
	QMap<QString, std::shared_ptr<SignalData> > data_map;
};


class Socket_RPC_SIGNAL_Server : public QObject
{
	Q_OBJECT
public:
	Socket_RPC_SIGNAL_Server(QString _conn_ip, int _conn_port);
	void set_app(RpcMFSK24Widget* _app)
	{
		app = _app;
	}
public slots:
	void tcp_slot();
private:
	QTcpServer* rpc_server;
	RpcMFSK24Widget* app;
	QList<std::shared_ptr<Socket_RPC_SIGNAL_Object> > rpc_objects;
};

class Socket_RPC_SIGNAL_Thread : public QThread
{
	Q_OBJECT
public:
	Socket_RPC_SIGNAL_Thread();
	void set_app(RpcMFSK24Widget* _app)
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
	Socket_RPC_SIGNAL_Server* rpc_srv;
	RpcMFSK24Widget* app;
	QString conn_ip;
	int conn_port;
};

class Socket_RPC_SLOT_Object : public QObject
{
	Q_OBJECT
public:
	Socket_RPC_SLOT_Object(RpcMFSK24Widget* _app, int socketDescriptor);
	~Socket_RPC_SLOT_Object()
	{
	}
	typedef QVariant (Socket_RPC_SLOT_Object::*OPERATOR_EXECUTOR)(QVariantList&);
	typedef QMap<QString, OPERATOR_EXECUTOR> OPERATORS_MAP;
public:
	QVariant QuerySlots(QVariantList& _values);
	QVariant auto_scroll_clicked(QVariantList& _values);
	QVariant log_timer_ontimer(QVariantList& _values);
	QVariant unmfsk24_manual_group_cmd(QVariantList& _values);
	QVariant unmfsk24_state(QVariantList& _values);
	QVariant unmfsk24_manual_cmd(QVariantList& _values);
	QVariant unmfsk24_start(QVariantList& _values);
	QVariant unmfsk24_set_cmd_time(QVariantList& _values);
public slots:
	void read_data();
	void sock_error(QAbstractSocket::SocketError _err);
private:
	OPERATORS_MAP operators_map;
	QTcpSocket* rpc_socket;
	RpcMFSK24Widget* app;
	bool with_return;
	static int obj_num;
};

class Socket_RPC_SLOT_Thread : public QThread
{
	Q_OBJECT
public:
	Socket_RPC_SLOT_Thread(RpcMFSK24Widget* _app, int _socketDescriptor);
	void run();
	std::shared_ptr<Socket_RPC_SLOT_Object> get_obj(){ return rpc_obj; }
	private:
	std::shared_ptr<Socket_RPC_SLOT_Object> rpc_obj;
	RpcMFSK24Widget* app;
	int socketDescriptor;
};

class Socket_RPC_SLOT_Server : public QTcpServer
{
	Q_OBJECT
public:
	Socket_RPC_SLOT_Server(QString _conn_ip, int _conn_port, RpcMFSK24Widget* _app);
protected:
	void incomingConnection(qintptr socketDescriptor) Q_DECL_OVERRIDE;
private:
	RpcMFSK24Widget* app;
	QList<std::shared_ptr<Socket_RPC_SLOT_Thread> > rpc_objects;
};

class Socket_RPC_SLOT_Server_Thread : public QThread
{
	Q_OBJECT
public:
	Socket_RPC_SLOT_Server_Thread();
	void set_app(RpcMFSK24Widget* _app)
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
	Socket_RPC_SLOT_Server* rpc_srv;
	RpcMFSK24Widget* app;
	QString conn_ip;
	int conn_port;

};
#endif
