#ifndef FRAME_BUS_SOCKET_RPC_H
#define FRAME_BUS_SOCKET_RPC_H

#include <QObject>
#include <QString>
#include <QTcpServer>
#include <QTcpSocket>
#include "frame_bus.h"

#include <QWidget>
#include <QTextEdit>
#include <QWaitCondition>

#include "rpc_loger.h"
#include "socket_rpc.h"
class frame_bus_Socket_RPC_SIGNAL_Object : public QObject
{
	Q_OBJECT
public:
	frame_bus_Socket_RPC_SIGNAL_Object();
	~frame_bus_Socket_RPC_SIGNAL_Object()
	{
	}
	void set_app(FrameBusWidget* _app);

	void set_socket(QTcpSocket* _rpc_socket);
signals:
	void send_signal(QByteArray* _arr);
public slots:
	void new_frame_04(QString mode, QVariant frame_data);
	void new_frame_733(QString mode, QVariant frame_data);
	void new_frame_07(QString mode, int psp, int lit, int fm, QString ant, QVariant frame_data);
	void new_frame_rm07(QString mode, QVariant frame_data);

	void send_signal_slot(QByteArray* _arr);
	void read_data();
	void sock_error(QAbstractSocket::SocketError _err);
private:
	void send_signal_func(QByteArray* _arr);
	QTcpSocket* rpc_socket;
	QMutex signal_mutex;
	FrameBusWidget* app;
	static int obj_num;
	static int call_number;
	QMap<QString, std::shared_ptr<SignalData> > data_map;
};


class frame_bus_Socket_RPC_SIGNAL_Server : public QObject
{
	Q_OBJECT
public:
	frame_bus_Socket_RPC_SIGNAL_Server(QString _conn_ip, int _conn_port);
	void set_app(FrameBusWidget* _app)
	{
		app = _app;
	}
public slots:
	void tcp_slot();
private:
	QTcpServer* rpc_server;
	FrameBusWidget* app;
	QList<std::shared_ptr<frame_bus_Socket_RPC_SIGNAL_Object> > rpc_objects;
};

class frame_bus_Socket_RPC_SIGNAL_Thread : public QThread
{
	Q_OBJECT
public:
	frame_bus_Socket_RPC_SIGNAL_Thread();
	void set_app(FrameBusWidget* _app)
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
	frame_bus_Socket_RPC_SIGNAL_Server* rpc_srv;
	FrameBusWidget* app;
	QString conn_ip;
	int conn_port;
};

class frame_bus_Socket_RPC_SLOT_Object : public QObject
{
	Q_OBJECT
public:
	frame_bus_Socket_RPC_SLOT_Object(FrameBusWidget* _app, int socketDescriptor);
	~frame_bus_Socket_RPC_SLOT_Object()
	{
	}
	typedef QVariant (frame_bus_Socket_RPC_SLOT_Object::*OPERATOR_EXECUTOR)(QVariantList&);
	typedef QMap<QString, OPERATOR_EXECUTOR> OPERATORS_MAP;
public:
	QVariant QuerySlots(QVariantList& _values);
	QVariant make_new_frame_04(QVariantList& _values);
	QVariant make_new_frame_733(QVariantList& _values);
	QVariant make_new_frame_07(QVariantList& _values);
	QVariant make_new_frame_rm07(QVariantList& _values);
public slots:
	void read_data();
	void sock_error(QAbstractSocket::SocketError _err);
private:
	OPERATORS_MAP operators_map;
	QTcpSocket* rpc_socket;
	FrameBusWidget* app;
	bool with_return;
	static int obj_num;
};

class frame_bus_Socket_RPC_SLOT_Thread : public QThread
{
	Q_OBJECT
public:
	frame_bus_Socket_RPC_SLOT_Thread(FrameBusWidget* _app, int _socketDescriptor);
	void run();
	std::shared_ptr<frame_bus_Socket_RPC_SLOT_Object> get_obj(){ return rpc_obj; }
private:
	std::shared_ptr<frame_bus_Socket_RPC_SLOT_Object> rpc_obj;
	FrameBusWidget* app;
	int socketDescriptor;
	static int obj_num;
};

class frame_bus_Socket_RPC_SLOT_Server : public QTcpServer
{
	Q_OBJECT
public:
	frame_bus_Socket_RPC_SLOT_Server(QString _conn_ip, int _conn_port, FrameBusWidget* _app);
protected:
	void incomingConnection(qintptr socketDescriptor) Q_DECL_OVERRIDE;
private:
	FrameBusWidget* app;
	QList<std::shared_ptr<frame_bus_Socket_RPC_SLOT_Thread> > rpc_objects;
};

class frame_bus_Socket_RPC_SLOT_Server_Thread : public QThread
{
	Q_OBJECT
public:
	frame_bus_Socket_RPC_SLOT_Server_Thread();
	void set_app(FrameBusWidget* _app)
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
	frame_bus_Socket_RPC_SLOT_Server* rpc_srv;
	FrameBusWidget* app;
	QString conn_ip;
	int conn_port;

};
#endif
