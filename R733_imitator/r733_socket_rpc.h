#ifndef R733_SOCKET_RPC_H
#define R733_SOCKET_RPC_H

#include <QObject>
#include <QString>
#include <QTcpServer>
#include <QTcpSocket>
#include "R733.h"

#include <QWidget>
#include <QTextEdit>
#include <QWaitCondition>

#include "rpc_loger.h"
#include "socket_rpc.h"
class r733_Socket_RPC_SIGNAL_Object : public QObject
{
	Q_OBJECT
public:
	r733_Socket_RPC_SIGNAL_Object();
	~r733_Socket_RPC_SIGNAL_Object()
	{
	}
	void set_app(R733_widg* _app);

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
	R733_widg* app;
	static int obj_num;
	static int call_number;
	QMap<QString, std::shared_ptr<SignalData> > data_map;
};


class r733_Socket_RPC_SIGNAL_Server : public QObject
{
	Q_OBJECT
public:
	r733_Socket_RPC_SIGNAL_Server(QString _conn_ip, int _conn_port);
	void set_app(R733_widg* _app)
	{
		app = _app;
	}
public slots:
	void tcp_slot();
private:
	QTcpServer* rpc_server;
	R733_widg* app;
	QList<std::shared_ptr<r733_Socket_RPC_SIGNAL_Object> > rpc_objects;
};

class r733_Socket_RPC_SIGNAL_Thread : public QThread
{
	Q_OBJECT
public:
	r733_Socket_RPC_SIGNAL_Thread();
	void set_app(R733_widg* _app)
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
	r733_Socket_RPC_SIGNAL_Server* rpc_srv;
	R733_widg* app;
	QString conn_ip;
	int conn_port;
};

class r733_Socket_RPC_SLOT_Object : public QObject
{
	Q_OBJECT
public:
	r733_Socket_RPC_SLOT_Object(R733_widg* _app, int socketDescriptor);
	~r733_Socket_RPC_SLOT_Object()
	{
	}
	typedef QVariant (r733_Socket_RPC_SLOT_Object::*OPERATOR_EXECUTOR)(QVariantList&);
	typedef QMap<QString, OPERATOR_EXECUTOR> OPERATORS_MAP;
public:
	QVariant QuerySlots(QVariantList& _values);
	QVariant new_message(QVariantList& _values);
	QVariant auto_scroll_clicked(QVariantList& _values);
	QVariant get_power(QVariantList& _values);
	QVariant new_frame_04(QVariantList& _values);
	QVariant new_mk(QVariantList& _values);
public slots:
	void read_data();
	void sock_error(QAbstractSocket::SocketError _err);
private:
	OPERATORS_MAP operators_map;
	QTcpSocket* rpc_socket;
	R733_widg* app;
	bool with_return;
	static int obj_num;
};

class r733_Socket_RPC_SLOT_Thread : public QThread
{
	Q_OBJECT
public:
	r733_Socket_RPC_SLOT_Thread(R733_widg* _app, int _socketDescriptor);
	void run();
	std::shared_ptr<r733_Socket_RPC_SLOT_Object> get_obj(){ return rpc_obj; }
private:
	std::shared_ptr<r733_Socket_RPC_SLOT_Object> rpc_obj;
	R733_widg* app;
	int socketDescriptor;
	static int obj_num;
};

class r733_Socket_RPC_SLOT_Server : public QTcpServer
{
	Q_OBJECT
public:
	r733_Socket_RPC_SLOT_Server(QString _conn_ip, int _conn_port, R733_widg* _app);
protected:
	void incomingConnection(qintptr socketDescriptor) Q_DECL_OVERRIDE;
private:
	R733_widg* app;
	QList<std::shared_ptr<r733_Socket_RPC_SLOT_Thread> > rpc_objects;
};

class r733_Socket_RPC_SLOT_Server_Thread : public QThread
{
	Q_OBJECT
public:
	r733_Socket_RPC_SLOT_Server_Thread();
	void set_app(R733_widg* _app)
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
	r733_Socket_RPC_SLOT_Server* rpc_srv;
	R733_widg* app;
	QString conn_ip;
	int conn_port;

};
#endif
