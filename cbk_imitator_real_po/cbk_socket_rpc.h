#ifndef CBK_SOCKET_RPC_H
#define CBK_SOCKET_RPC_H

#include <QObject>
#include <QString>
#include <QTcpServer>
#include <QTcpSocket>
#include "cbk_imitator.h"

#include <QWidget>
#include <QTextEdit>
#include <QWaitCondition>

#include "rpc_loger.h"
#include "socket_rpc.h"
class cbk_Socket_RPC_SIGNAL_Object : public QObject
{
	Q_OBJECT
public:
	cbk_Socket_RPC_SIGNAL_Object();
	~cbk_Socket_RPC_SIGNAL_Object()
	{
	}
	void set_app(CBK_MainWindow* _app);

	void set_socket(QTcpSocket* _rpc_socket);
signals:
	void send_signal(QByteArray* _arr);
public slots:
	void vm_is_on(int n_vm);
	void vm_is_off(int n_vm);
	void vm_change_po(int n_vm);

	void send_signal_slot(QByteArray* _arr);
	void read_data();
	void sock_error(QAbstractSocket::SocketError _err);
private:
	void send_signal_func(QByteArray* _arr);
	QTcpSocket* rpc_socket;
	QMutex signal_mutex;
	CBK_MainWindow* app;
	static int obj_num;
	static int call_number;
	QMap<QString, std::shared_ptr<SignalData> > data_map;
};


class cbk_Socket_RPC_SIGNAL_Server : public QObject
{
	Q_OBJECT
public:
	cbk_Socket_RPC_SIGNAL_Server(QString _conn_ip, int _conn_port);
	void set_app(CBK_MainWindow* _app)
	{
		app = _app;
	}
public slots:
	void tcp_slot();
private:
	QTcpServer* rpc_server;
	CBK_MainWindow* app;
	QList<std::shared_ptr<cbk_Socket_RPC_SIGNAL_Object> > rpc_objects;
};

class cbk_Socket_RPC_SIGNAL_Thread : public QThread
{
	Q_OBJECT
public:
	cbk_Socket_RPC_SIGNAL_Thread();
	void set_app(CBK_MainWindow* _app)
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
	cbk_Socket_RPC_SIGNAL_Server* rpc_srv;
	CBK_MainWindow* app;
	QString conn_ip;
	int conn_port;
};

class cbk_Socket_RPC_SLOT_Object : public QObject
{
	Q_OBJECT
public:
	cbk_Socket_RPC_SLOT_Object(CBK_MainWindow* _app, int socketDescriptor);
	~cbk_Socket_RPC_SLOT_Object()
	{
	}
	typedef QVariant (cbk_Socket_RPC_SLOT_Object::*OPERATOR_EXECUTOR)(QVariantList&);
	typedef QMap<QString, OPERATOR_EXECUTOR> OPERATORS_MAP;
public:
	QVariant QuerySlots(QVariantList& _values);
	QVariant set_tm_state(QVariantList& _values);
	QVariant new_ku(QVariantList& _values);
public slots:
	void read_data();
	void sock_error(QAbstractSocket::SocketError _err);
private:
	OPERATORS_MAP operators_map;
	QTcpSocket* rpc_socket;
	CBK_MainWindow* app;
	bool with_return;
	static int obj_num;
};

class cbk_Socket_RPC_SLOT_Thread : public QThread
{
	Q_OBJECT
public:
	cbk_Socket_RPC_SLOT_Thread(CBK_MainWindow* _app, int _socketDescriptor);
	void run();
	std::shared_ptr<cbk_Socket_RPC_SLOT_Object> get_obj(){ return rpc_obj; }
	private:
	std::shared_ptr<cbk_Socket_RPC_SLOT_Object> rpc_obj;
	CBK_MainWindow* app;
	int socketDescriptor;
};

class cbk_Socket_RPC_SLOT_Server : public QTcpServer
{
	Q_OBJECT
public:
	cbk_Socket_RPC_SLOT_Server(QString _conn_ip, int _conn_port, CBK_MainWindow* _app);
protected:
	void incomingConnection(qintptr socketDescriptor) Q_DECL_OVERRIDE;
private:
	CBK_MainWindow* app;
	QList<std::shared_ptr<cbk_Socket_RPC_SLOT_Thread> > rpc_objects;
};

class cbk_Socket_RPC_SLOT_Server_Thread : public QThread
{
	Q_OBJECT
public:
	cbk_Socket_RPC_SLOT_Server_Thread();
	void set_app(CBK_MainWindow* _app)
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
	cbk_Socket_RPC_SLOT_Server* rpc_srv;
	CBK_MainWindow* app;
	QString conn_ip;
	int conn_port;

};
#endif
