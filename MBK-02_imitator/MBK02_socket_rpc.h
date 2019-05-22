#ifndef MBK02_SOCKET_RPC_H
#define MBK02_SOCKET_RPC_H

#include <QObject>
#include <QString>
#include <QTcpServer>
#include <QTcpSocket>
#include "MBK02.h"

#include <QWidget>
#include <QTextEdit>
#include <QWaitCondition>

#include "rpc_loger.h"
#include "socket_rpc.h"
class MBK02_Socket_RPC_SIGNAL_Object : public QObject
{
	Q_OBJECT
public:
	MBK02_Socket_RPC_SIGNAL_Object();
	~MBK02_Socket_RPC_SIGNAL_Object()
	{
	}
	void set_app(MBK02_widg* _app);

	void set_socket(QTcpSocket* _rpc_socket);
signals:
	void send_signal(QByteArray* _arr);
public slots:
	void msg_to_14R732(QVariantList data);
	void set_new_tm(int sadr, int word);
	void set_new_power_tm(int sadr, QVariantList words);

	void send_signal_slot(QByteArray* _arr);
	void read_data();
	void sock_error(QAbstractSocket::SocketError _err);
private:
	void send_signal_func(QByteArray* _arr);
	QTcpSocket* rpc_socket;
	QMutex signal_mutex;
	MBK02_widg* app;
	static int obj_num;
	static int call_number;
	QMap<QString, std::shared_ptr<SignalData> > data_map;
};


class MBK02_Socket_RPC_SIGNAL_Server : public QObject
{
	Q_OBJECT
public:
	MBK02_Socket_RPC_SIGNAL_Server(QString _conn_ip, int _conn_port);
	void set_app(MBK02_widg* _app)
	{
		app = _app;
	}
public slots:
	void tcp_slot();
private:
	QTcpServer* rpc_server;
	MBK02_widg* app;
	QList<std::shared_ptr<MBK02_Socket_RPC_SIGNAL_Object> > rpc_objects;
};

class MBK02_Socket_RPC_SIGNAL_Thread : public QThread
{
	Q_OBJECT
public:
	MBK02_Socket_RPC_SIGNAL_Thread();
	void set_app(MBK02_widg* _app)
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
	MBK02_Socket_RPC_SIGNAL_Server* rpc_srv;
	MBK02_widg* app;
	QString conn_ip;
	int conn_port;
};

class MBK02_Socket_RPC_SLOT_Object : public QObject
{
	Q_OBJECT
public:
	MBK02_Socket_RPC_SLOT_Object(MBK02_widg* _app, int socketDescriptor);
	~MBK02_Socket_RPC_SLOT_Object()
	{
	}
	typedef QVariant (MBK02_Socket_RPC_SLOT_Object::*OPERATOR_EXECUTOR)(QVariantList&);
	typedef QMap<QString, OPERATOR_EXECUTOR> OPERATORS_MAP;
public:
	QVariant QuerySlots(QVariantList& _values);
	QVariant new_message(QVariantList& _values);
	QVariant new_ku_732(QVariantList& _values);
	QVariant set_new_mbk02_tm(QVariantList& _values);
	QVariant new_KPI(QVariantList& _values);
	QVariant auto_scroll_clicked(QVariantList& _values);
	QVariant update_tm(QVariantList& _values);
	QVariant get_power(QVariantList& _values);
public slots:
	void read_data();
	void sock_error(QAbstractSocket::SocketError _err);
private:
	OPERATORS_MAP operators_map;
	QTcpSocket* rpc_socket;
	MBK02_widg* app;
	bool with_return;
	static int obj_num;
};

class MBK02_Socket_RPC_SLOT_Thread : public QThread
{
	Q_OBJECT
public:
	MBK02_Socket_RPC_SLOT_Thread(MBK02_widg* _app, int _socketDescriptor);
	void run();
	std::shared_ptr<MBK02_Socket_RPC_SLOT_Object> get_obj(){ return rpc_obj; }
	private:
	std::shared_ptr<MBK02_Socket_RPC_SLOT_Object> rpc_obj;
	MBK02_widg* app;
	int socketDescriptor;
};

class MBK02_Socket_RPC_SLOT_Server : public QTcpServer
{
	Q_OBJECT
public:
	MBK02_Socket_RPC_SLOT_Server(QString _conn_ip, int _conn_port, MBK02_widg* _app);
protected:
	void incomingConnection(qintptr socketDescriptor) Q_DECL_OVERRIDE;
private:
	MBK02_widg* app;
	QList<std::shared_ptr<MBK02_Socket_RPC_SLOT_Thread> > rpc_objects;
};

class MBK02_Socket_RPC_SLOT_Server_Thread : public QThread
{
	Q_OBJECT
public:
	MBK02_Socket_RPC_SLOT_Server_Thread();
	void set_app(MBK02_widg* _app)
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
	MBK02_Socket_RPC_SLOT_Server* rpc_srv;
	MBK02_widg* app;
	QString conn_ip;
	int conn_port;

};
#endif
