#ifndef MKU_BUS_SOCKET_RPC_H
#define MKU_BUS_SOCKET_RPC_H

#include <QObject>
#include <QString>
#include <QTcpServer>
#include <QTcpSocket>
#include "mku_bus.h"

#include <QWidget>
#include <QTextEdit>
#include <QWaitCondition>

#include "rpc_loger.h"
#include "socket_rpc.h"
class mku_bus_Socket_RPC_SIGNAL_Object : public QObject
{
	Q_OBJECT
public:
	mku_bus_Socket_RPC_SIGNAL_Object();
	~mku_bus_Socket_RPC_SIGNAL_Object()
	{
	}
	void set_app(MKUWidget* _app);

	void set_socket(QTcpSocket* _rpc_socket);
signals:
	void send_signal(QByteArray* _arr);
public slots:
	void new_ku(int ku_n, int length, double u, int line);
	void new_ku_732(int ku_n, int length, double u, int line);
	void new_ku_cbk(int ku_n, int length, double u, int line);
	void new_mk(int mshm, int pshm, int length_m, int length_p, double u_m, double u_p, int dt, int line_m, int line_p);
	void new_mt_at_state(int dev_name, int state);
	void new_data_736(QVariantList dataList, QVariantList maskList);
	void new_data_737(QVariantList dataList, QVariantList maskList);

	void send_signal_slot(QByteArray* _arr);
	void read_data();
	void sock_error(QAbstractSocket::SocketError _err);
private:
	void send_signal_func(QByteArray* _arr);
	QTcpSocket* rpc_socket;
	QMutex signal_mutex;
	MKUWidget* app;
	static int obj_num;
	static int call_number;
	QMap<QString, std::shared_ptr<SignalData> > data_map;
};


class mku_bus_Socket_RPC_SIGNAL_Server : public QObject
{
	Q_OBJECT
public:
	mku_bus_Socket_RPC_SIGNAL_Server(QString _conn_ip, int _conn_port);
	void set_app(MKUWidget* _app)
	{
		app = _app;
	}
public slots:
	void tcp_slot();
private:
	QTcpServer* rpc_server;
	MKUWidget* app;
	QList<std::shared_ptr<mku_bus_Socket_RPC_SIGNAL_Object> > rpc_objects;
};

class mku_bus_Socket_RPC_SIGNAL_Thread : public QThread
{
	Q_OBJECT
public:
	mku_bus_Socket_RPC_SIGNAL_Thread();
	void set_app(MKUWidget* _app)
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
	mku_bus_Socket_RPC_SIGNAL_Server* rpc_srv;
	MKUWidget* app;
	QString conn_ip;
	int conn_port;
};

class mku_bus_Socket_RPC_SLOT_Object : public QObject
{
	Q_OBJECT
public:
	mku_bus_Socket_RPC_SLOT_Object(MKUWidget* _app, int socketDescriptor);
	~mku_bus_Socket_RPC_SLOT_Object()
	{
	}
	typedef QVariant (mku_bus_Socket_RPC_SLOT_Object::*OPERATOR_EXECUTOR)(QVariantList&);
	typedef QMap<QString, OPERATOR_EXECUTOR> OPERATORS_MAP;
public:
	QVariant QuerySlots(QVariantList& _values);
	QVariant make_ku(QVariantList& _values);
	QVariant make_ku_732(QVariantList& _values);
	QVariant make_ku_cbk(QVariantList& _values);
	QVariant make_mk(QVariantList& _values);
	QVariant ku_map_channels_setup(QVariantList& _values);
	QVariant mshm_map_channels_setup(QVariantList& _values);
	QVariant pshm_map_channels_setup(QVariantList& _values);
	QVariant make_mt_at_state(QVariantList& _values);
	QVariant make_data_736(QVariantList& _values);
	QVariant make_data_737(QVariantList& _values);
	QVariant get_tm(QVariantList& _values);
	QVariant set_tm(QVariantList& _values);
public slots:
	void read_data();
	void sock_error(QAbstractSocket::SocketError _err);
private:
	OPERATORS_MAP operators_map;
	QTcpSocket* rpc_socket;
	MKUWidget* app;
	bool with_return;
	static int obj_num;
};

class mku_bus_Socket_RPC_SLOT_Thread : public QThread
{
	Q_OBJECT
public:
	mku_bus_Socket_RPC_SLOT_Thread(MKUWidget* _app, int _socketDescriptor);
	void run();
	std::shared_ptr<mku_bus_Socket_RPC_SLOT_Object> get_obj(){ return rpc_obj; }
	private:
	std::shared_ptr<mku_bus_Socket_RPC_SLOT_Object> rpc_obj;
	MKUWidget* app;
	int socketDescriptor;
};

class mku_bus_Socket_RPC_SLOT_Server : public QTcpServer
{
	Q_OBJECT
public:
	mku_bus_Socket_RPC_SLOT_Server(QString _conn_ip, int _conn_port, MKUWidget* _app);
protected:
	void incomingConnection(qintptr socketDescriptor) Q_DECL_OVERRIDE;
private:
	MKUWidget* app;
	QList<std::shared_ptr<mku_bus_Socket_RPC_SLOT_Thread> > rpc_objects;
};

class mku_bus_Socket_RPC_SLOT_Server_Thread : public QThread
{
	Q_OBJECT
public:
	mku_bus_Socket_RPC_SLOT_Server_Thread();
	void set_app(MKUWidget* _app)
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
	mku_bus_Socket_RPC_SLOT_Server* rpc_srv;
	MKUWidget* app;
	QString conn_ip;
	int conn_port;

};
#endif
