#include "VIP_imit.h"
#include <QString>
#include <QMessageBox>

mds32_exchange::mds32_exchange() : QObject()
{

}

void mds32_exchange::create_signal_thread(QString ip_str, int port)
{
	mds32_exchange_thread = new RPC_mds32_SIGNAL_Thread;
	ip_serv = ip_str;
	port_serv = port;

	if (!mds32_exchange_thread->isRunning())
	{
		mds32_exchange_thread->set_connection_params(ip_serv, port_serv);
		mds32_exchange_thread->start();
	}
	if (!mds32_exchange_thread->wait_connected(3))
	{
		QMessageBox::critical(0, "Connection is missing", "Error with creating signal thread with mds32!");
	}
}

mfsk24_exchange::mfsk24_exchange() : QObject()
{

}

void mfsk24_exchange::create_signal_thread(QString ip_str, int port)
{
	mfsk24_exchange_thread = new RPC_mfsk24_SIGNAL_Thread;
	ip_serv = ip_str;
	port_serv = port;

	if (!mfsk24_exchange_thread->isRunning())
	{
		mfsk24_exchange_thread->set_connection_params(ip_serv, port_serv);
		mfsk24_exchange_thread->start();
	}
	if (!mfsk24_exchange_thread->wait_connected(3))
	{
		QMessageBox::critical(0, "Connection is missing", "Error with creating signal thread with mfsk24!");
	}
}