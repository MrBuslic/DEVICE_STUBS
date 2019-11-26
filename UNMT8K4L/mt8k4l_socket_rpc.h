#ifndef MT8K4L_SOCKET_RPC_H
#define MT8K4L_SOCKET_RPC_H

#include <QObject>
#include <QString>
#include <QTcpServer>
#include <QTcpSocket>
#include "rpc_mt8k4l.h"

#include <QWidget>
#include <QTextEdit>
#include <QWaitCondition>

#include "rpc_loger.h"
#include "socket_rpc.h"
class mt8k4l_Socket_RPC_SIGNAL_Object : public QObject
{
	Q_OBJECT
public:
	mt8k4l_Socket_RPC_SIGNAL_Object();
	~mt8k4l_Socket_RPC_SIGNAL_Object()
	{
	}
	void set_app(RpcMT8K4LWidget* _app);

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
	RpcMT8K4LWidget* app;
	static int obj_num;
	static int call_number;
	QMap<QString, std::shared_ptr<SignalData> > data_map;
};


class mt8k4l_Socket_RPC_SIGNAL_Server : public QObject
{
	Q_OBJECT
public:
	mt8k4l_Socket_RPC_SIGNAL_Server(QString _conn_ip, int _conn_port);
	void set_app(RpcMT8K4LWidget* _app)
	{
		app = _app;
	}
public slots:
	void tcp_slot();
private:
	QTcpServer* rpc_server;
	RpcMT8K4LWidget* app;
	QList<std::shared_ptr<mt8k4l_Socket_RPC_SIGNAL_Object> > rpc_objects;
};

class mt8k4l_Socket_RPC_SIGNAL_Thread : public QThread
{
	Q_OBJECT
public:
	mt8k4l_Socket_RPC_SIGNAL_Thread();
	void set_app(RpcMT8K4LWidget* _app)
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
	mt8k4l_Socket_RPC_SIGNAL_Server* rpc_srv;
	RpcMT8K4LWidget* app;
	QString conn_ip;
	int conn_port;
};

class mt8k4l_Socket_RPC_SLOT_Object : public QObject
{
	Q_OBJECT
public:
	mt8k4l_Socket_RPC_SLOT_Object(RpcMT8K4LWidget* _app, int socketDescriptor);
	~mt8k4l_Socket_RPC_SLOT_Object()
	{
	}
	typedef QVariant (mt8k4l_Socket_RPC_SLOT_Object::*OPERATOR_EXECUTOR)(QVariantList&);
	typedef QMap<QString, OPERATOR_EXECUTOR> OPERATORS_MAP;
public:
	QVariant QuerySlots(QVariantList& _values);
	QVariant auto_scroll_clicked(QVariantList& _values);
	QVariant log_timer_ontimer(QVariantList& _values);
	QVariant unmt8k4l_start(QVariantList& _values);
	QVariant unmt8k4l_input_trigger(QVariantList& _values);
	QVariant unmt8k4l_sample_width_q(QVariantList& _values);
	QVariant unmt8k4l_read_sample(QVariantList& _values);
public slots:
	void read_data();
	void sock_error(QAbstractSocket::SocketError _err);
private:
	OPERATORS_MAP operators_map;
	QTcpSocket* rpc_socket;
	RpcMT8K4LWidget* app;
	bool with_return;
	static int obj_num;
};

class mt8k4l_Socket_RPC_SLOT_Thread : public QThread
{
	Q_OBJECT
public:
	mt8k4l_Socket_RPC_SLOT_Thread(RpcMT8K4LWidget* _app, int _socketDescriptor);
	void run();
	std::shared_ptr<mt8k4l_Socket_RPC_SLOT_Object> get_obj(){ return rpc_obj; }
	private:
	std::shared_ptr<mt8k4l_Socket_RPC_SLOT_Object> rpc_obj;
	RpcMT8K4LWidget* app;
	int socketDescriptor;
};

class mt8k4l_Socket_RPC_SLOT_Server : public QTcpServer
{
	Q_OBJECT
public:
	mt8k4l_Socket_RPC_SLOT_Server(QString _conn_ip, int _conn_port, RpcMT8K4LWidget* _app);
protected:
	void incomingConnection(qintptr socketDescriptor) Q_DECL_OVERRIDE;
private:
	RpcMT8K4LWidget* app;
	QList<std::shared_ptr<mt8k4l_Socket_RPC_SLOT_Thread> > rpc_objects;
};

class mt8k4l_Socket_RPC_SLOT_Server_Thread : public QThread
{
	Q_OBJECT
public:
	mt8k4l_Socket_RPC_SLOT_Server_Thread();
	void set_app(RpcMT8K4LWidget* _app)
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
	mt8k4l_Socket_RPC_SLOT_Server* rpc_srv;
	RpcMT8K4LWidget* app;
	QString conn_ip;
	int conn_port;

};
#endif
