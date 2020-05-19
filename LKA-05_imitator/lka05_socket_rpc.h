#ifndef LKA05_SOCKET_RPC_H
#define LKA05_SOCKET_RPC_H

#include <QObject>
#include <QString>
#include <QTcpServer>
#include <QTcpSocket>
#include "LKA05.h"

#include <QWidget>
#include <QTextEdit>
#include <QWaitCondition>

#include "rpc_loger.h"
#include "socket_rpc.h"
class lka05_Socket_RPC_SIGNAL_Object : public QObject
{
	Q_OBJECT
public:
	lka05_Socket_RPC_SIGNAL_Object();
	~lka05_Socket_RPC_SIGNAL_Object()
	{
	}
	void set_app(LKA05_widg* _app);

	void set_socket(QTcpSocket* _rpc_socket);
signals:
	void send_signal(QByteArray* _arr);
public slots:
	void new_ku(int ku_n, int length, double u, int line);
	void new_mk(int mshm, int pshm, int length_m, int length_p, double u_m, double u_p, int dt, int line_m, int line_p);

	void send_signal_slot(QByteArray* _arr);
	void read_data();
	void sock_error(QAbstractSocket::SocketError _err);
private:
	void send_signal_func(QByteArray* _arr);
	QTcpSocket* rpc_socket;
	QMutex signal_mutex;
	LKA05_widg* app;
	static int obj_num;
	static int call_number;
	QMap<QString, std::shared_ptr<SignalData> > data_map;
};


class lka05_Socket_RPC_SIGNAL_Server : public QObject
{
	Q_OBJECT
public:
	lka05_Socket_RPC_SIGNAL_Server(QString _conn_ip, int _conn_port);
	void set_app(LKA05_widg* _app)
	{
		app = _app;
	}
public slots:
	void tcp_slot();
private:
	QTcpServer* rpc_server;
	LKA05_widg* app;
	QList<std::shared_ptr<lka05_Socket_RPC_SIGNAL_Object> > rpc_objects;
};

class lka05_Socket_RPC_SIGNAL_Thread : public QThread
{
	Q_OBJECT
public:
	lka05_Socket_RPC_SIGNAL_Thread();
	void set_app(LKA05_widg* _app)
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
	lka05_Socket_RPC_SIGNAL_Server* rpc_srv;
	LKA05_widg* app;
	QString conn_ip;
	int conn_port;
};

class lka05_Socket_RPC_SLOT_Object : public QObject
{
	Q_OBJECT
public:
	lka05_Socket_RPC_SLOT_Object(LKA05_widg* _app, int socketDescriptor);
	~lka05_Socket_RPC_SLOT_Object()
	{
	}
	typedef QVariant (lka05_Socket_RPC_SLOT_Object::*OPERATOR_EXECUTOR)(QVariantList&);
	typedef QMap<QString, OPERATOR_EXECUTOR> OPERATORS_MAP;
public:
	QVariant QuerySlots(QVariantList& _values);
	QVariant new_message(QVariantList& _values);
	QVariant new_tm(QVariantList& _values);
	QVariant new_ku_732(QVariantList& _values);
	QVariant get_power(QVariantList& _values);
	QVariant set_mu_working(QVariantList& _values);
	QVariant set_ku_working(QVariantList& _values);
	QVariant set_mk_working(QVariantList& _values);
	QVariant set_mpvn_working(QVariantList& _values);
public slots:
	void read_data();
	void sock_error(QAbstractSocket::SocketError _err);
private:
	OPERATORS_MAP operators_map;
	QTcpSocket* rpc_socket;
	LKA05_widg* app;
	bool with_return;
	static int obj_num;
};

class lka05_Socket_RPC_SLOT_Thread : public QThread
{
	Q_OBJECT
public:
	lka05_Socket_RPC_SLOT_Thread(LKA05_widg* _app, int _socketDescriptor);
	void run();
	std::shared_ptr<lka05_Socket_RPC_SLOT_Object> get_obj(){ return rpc_obj; }
	private:
	std::shared_ptr<lka05_Socket_RPC_SLOT_Object> rpc_obj;
	LKA05_widg* app;
	int socketDescriptor;
};

class lka05_Socket_RPC_SLOT_Server : public QTcpServer
{
	Q_OBJECT
public:
	lka05_Socket_RPC_SLOT_Server(QString _conn_ip, int _conn_port, LKA05_widg* _app);
protected:
	void incomingConnection(qintptr socketDescriptor) Q_DECL_OVERRIDE;
private:
	LKA05_widg* app;
	QList<std::shared_ptr<lka05_Socket_RPC_SLOT_Thread> > rpc_objects;
};

class lka05_Socket_RPC_SLOT_Server_Thread : public QThread
{
	Q_OBJECT
public:
	lka05_Socket_RPC_SLOT_Server_Thread();
	void set_app(LKA05_widg* _app)
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
	lka05_Socket_RPC_SLOT_Server* rpc_srv;
	LKA05_widg* app;
	QString conn_ip;
	int conn_port;

};
#endif
