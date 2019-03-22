#ifndef VIP_IMIT_H
#define VIP_IMIT_H
#include "winsock2.h"
#include <QString>
#include <QObject>
#include "mds32_rpc.h"
#include "mfsk24_rpc.h"

class mds32_exchange : public QObject
{
	Q_OBJECT

public:
	mds32_exchange();
	void create_signal_thread(QString ip_str, int port);
	RPC_mds32_SIGNAL_Thread* get_mds32_exchange_thread() { return mds32_exchange_thread; };
private:
	RPC_mds32_SIGNAL_Thread* mds32_exchange_thread;
	QString ip_serv;
	int port_serv;

};


class mfsk24_exchange : public QObject
{
	Q_OBJECT

public:
	mfsk24_exchange();
	void create_signal_thread(QString ip_str, int port);
	RPC_mfsk24_SIGNAL_Thread* get_mds32_exchange_thread() { return mfsk24_exchange_thread; };
private:
	RPC_mfsk24_SIGNAL_Thread* mfsk24_exchange_thread;
	QString ip_serv;
	int port_serv;
};
#endif