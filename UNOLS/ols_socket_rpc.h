#ifndef OLS_SOCKET_RPC_H
#define OLS_SOCKET_RPC_H

#include <QObject>
#include <QString>
#include <QTcpServer>
#include <QTcpSocket>
#include "rpc_ols.h"

#include <QWidget>
#include <QTextEdit>
#include <QWaitCondition>

#include "rpc_loger.h"
#include "socket_rpc.h"
class ols_Socket_RPC_SIGNAL_Object : public QObject
{
	Q_OBJECT
public:
	ols_Socket_RPC_SIGNAL_Object();
	~ols_Socket_RPC_SIGNAL_Object()
	{
	}
	void set_app(RpcOlsWidget* _app);

	void set_socket(QTcpSocket* _rpc_socket);
signals:
	void send_signal(QByteArray* _arr);
public slots:
	void new_ols_data(QVariantList data_buffer, QVariantList mask_buffer);

	void send_signal_slot(QByteArray* _arr);
	void read_data();
	void sock_error(QAbstractSocket::SocketError _err);
private:
	void send_signal_func(QByteArray* _arr);
	QTcpSocket* rpc_socket;
	QMutex signal_mutex;
	RpcOlsWidget* app;
	static int obj_num;
	static int call_number;
	QMap<QString, std::shared_ptr<SignalData> > data_map;
};


class ols_Socket_RPC_SIGNAL_Server : public QObject
{
	Q_OBJECT
public:
	ols_Socket_RPC_SIGNAL_Server(QString _conn_ip, int _conn_port);
	void set_app(RpcOlsWidget* _app)
	{
		app = _app;
	}
public slots:
	void tcp_slot();
private:
	QTcpServer* rpc_server;
	RpcOlsWidget* app;
	QList<std::shared_ptr<ols_Socket_RPC_SIGNAL_Object> > rpc_objects;
};

class ols_Socket_RPC_SIGNAL_Thread : public QThread
{
	Q_OBJECT
public:
	ols_Socket_RPC_SIGNAL_Thread();
	void set_app(RpcOlsWidget* _app)
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
	ols_Socket_RPC_SIGNAL_Server* rpc_srv;
	RpcOlsWidget* app;
	QString conn_ip;
	int conn_port;
};

class ols_Socket_RPC_SLOT_Object : public QObject
{
	Q_OBJECT
public:
	ols_Socket_RPC_SLOT_Object(RpcOlsWidget* _app, int socketDescriptor);
	~ols_Socket_RPC_SLOT_Object()
	{
	}
	typedef QVariant (ols_Socket_RPC_SLOT_Object::*OPERATOR_EXECUTOR)(QVariantList&);
	typedef QMap<QString, OPERATOR_EXECUTOR> OPERATORS_MAP;
public:
	QVariant QuerySlots(QVariantList& _values);
	QVariant auto_scroll_clicked(QVariantList& _values);
	QVariant log_timer_ontimer(QVariantList& _values);
	QVariant unols_write_data_kf(QVariantList& _values);
	QVariant unols_trigger_imm(QVariantList& _values);
public slots:
	void read_data();
	void sock_error(QAbstractSocket::SocketError _err);
private:
	OPERATORS_MAP operators_map;
	QTcpSocket* rpc_socket;
	RpcOlsWidget* app;
	bool with_return;
	static int obj_num;
};

class ols_Socket_RPC_SLOT_Thread : public QThread
{
	Q_OBJECT
public:
	ols_Socket_RPC_SLOT_Thread(RpcOlsWidget* _app, int _socketDescriptor);
	void run();
	std::shared_ptr<ols_Socket_RPC_SLOT_Object> get_obj(){ return rpc_obj; }
	private:
	std::shared_ptr<ols_Socket_RPC_SLOT_Object> rpc_obj;
	RpcOlsWidget* app;
	int socketDescriptor;
};

class ols_Socket_RPC_SLOT_Server : public QTcpServer
{
	Q_OBJECT
public:
	ols_Socket_RPC_SLOT_Server(QString _conn_ip, int _conn_port, RpcOlsWidget* _app);
protected:
	void incomingConnection(qintptr socketDescriptor) Q_DECL_OVERRIDE;
private:
	RpcOlsWidget* app;
	QList<std::shared_ptr<ols_Socket_RPC_SLOT_Thread> > rpc_objects;
};

class ols_Socket_RPC_SLOT_Server_Thread : public QThread
{
	Q_OBJECT
public:
	ols_Socket_RPC_SLOT_Server_Thread();
	void set_app(RpcOlsWidget* _app)
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
	ols_Socket_RPC_SLOT_Server* rpc_srv;
	RpcOlsWidget* app;
	QString conn_ip;
	int conn_port;

};
#endif
