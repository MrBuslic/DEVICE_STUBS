#ifndef MDS32_SOCKET_RPC_H
#define MDS32_SOCKET_RPC_H

#include <QObject>
#include <QString>
#include <QTcpServer>
#include <QTcpSocket>
#include "rpc_mds32.h"

#include <QWidget>
#include <QTextEdit>
#include <QWaitCondition>

#include "rpc_loger.h"
#include "socket_rpc.h"
class mds32_Socket_RPC_SIGNAL_Object : public QObject
{
	Q_OBJECT
public:
	mds32_Socket_RPC_SIGNAL_Object();
	~mds32_Socket_RPC_SIGNAL_Object()
	{
	}
	void set_app(RpcMDS32Widget* _app);

	void set_socket(QTcpSocket* _rpc_socket);
signals:
	void send_signal(QByteArray* _arr);
public slots:
	void mds32_get_sample(uint& buf, bool& flag);

	void send_signal_slot(QByteArray* _arr);
	void read_data();
	void sock_error(QAbstractSocket::SocketError _err);
private:
	void send_signal_func(QByteArray* _arr);
	QTcpSocket* rpc_socket;
	QMutex signal_mutex;
	RpcMDS32Widget* app;
	static int obj_num;
	static int call_number;
	QMap<QString, std::shared_ptr<SignalData> > data_map;
};


class mds32_Socket_RPC_SIGNAL_Server : public QObject
{
	Q_OBJECT
public:
	mds32_Socket_RPC_SIGNAL_Server(QString _conn_ip, int _conn_port);
	void set_app(RpcMDS32Widget* _app)
	{
		app = _app;
	}
public slots:
	void tcp_slot();
private:
	QTcpServer* rpc_server;
	RpcMDS32Widget* app;
	QList<std::shared_ptr<mds32_Socket_RPC_SIGNAL_Object> > rpc_objects;
};

class mds32_Socket_RPC_SIGNAL_Thread : public QThread
{
	Q_OBJECT
public:
	mds32_Socket_RPC_SIGNAL_Thread();
	void set_app(RpcMDS32Widget* _app)
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
	mds32_Socket_RPC_SIGNAL_Server* rpc_srv;
	RpcMDS32Widget* app;
	QString conn_ip;
	int conn_port;
};

class mds32_Socket_RPC_SLOT_Object : public QObject
{
	Q_OBJECT
public:
	mds32_Socket_RPC_SLOT_Object(RpcMDS32Widget* _app, int socketDescriptor);
	~mds32_Socket_RPC_SLOT_Object()
	{
	}
	typedef QVariant (mds32_Socket_RPC_SLOT_Object::*OPERATOR_EXECUTOR)(QVariantList&);
	typedef QMap<QString, OPERATOR_EXECUTOR> OPERATORS_MAP;
public:
	QVariant QuerySlots(QVariantList& _values);
	QVariant unmds32_input_trigger(QVariantList& _values);
	QVariant unmds32_read_sample(QVariantList& _values);
	QVariant unmds32_start(QVariantList& _values);
	QVariant check_box_clicked(QVariantList& _values);
	QVariant line_edit_changed(QVariantList& _values);
public slots:
	void read_data();
	void sock_error(QAbstractSocket::SocketError _err);
private:
	OPERATORS_MAP operators_map;
	QTcpSocket* rpc_socket;
	RpcMDS32Widget* app;
	bool with_return;
	static int obj_num;
};

class mds32_Socket_RPC_SLOT_Thread : public QThread
{
	Q_OBJECT
public:
	mds32_Socket_RPC_SLOT_Thread(RpcMDS32Widget* _app, int _socketDescriptor);
	void run();
	std::shared_ptr<mds32_Socket_RPC_SLOT_Object> get_obj(){ return rpc_obj; }
	private:
	std::shared_ptr<mds32_Socket_RPC_SLOT_Object> rpc_obj;
	RpcMDS32Widget* app;
	int socketDescriptor;
};

class mds32_Socket_RPC_SLOT_Server : public QTcpServer
{
	Q_OBJECT
public:
	mds32_Socket_RPC_SLOT_Server(QString _conn_ip, int _conn_port, RpcMDS32Widget* _app);
protected:
	void incomingConnection(qintptr socketDescriptor) Q_DECL_OVERRIDE;
private:
	RpcMDS32Widget* app;
	QList<std::shared_ptr<mds32_Socket_RPC_SLOT_Thread> > rpc_objects;
};

class mds32_Socket_RPC_SLOT_Server_Thread : public QThread
{
	Q_OBJECT
public:
	mds32_Socket_RPC_SLOT_Server_Thread();
	void set_app(RpcMDS32Widget* _app)
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
	mds32_Socket_RPC_SLOT_Server* rpc_srv;
	RpcMDS32Widget* app;
	QString conn_ip;
	int conn_port;

};
#endif
