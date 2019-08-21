#include "KPIU_server_plugin.h"
#include <qdatetime.h>
#include <qfileinfo.h>
#include <qdatastream.h>
#include "kpiu_rpc.h"
#include "rpc_ports.h"

typedef RPC_kpiu_SLOT_Thread RMS_SLOT;
typedef RPC_kpiu_SIGNAL_Thread RMS_SIGNAL;

void KpiuServerPlugin::connect(const QString& ip)
{
	_interface = new ServerInterface(std::shared_ptr<RMS_SLOT>(new RMS_SLOT),
		std::shared_ptr<RMS_SLOT>(new RMS_SLOT),
		std::shared_ptr<RMS_SIGNAL>(new RMS_SIGNAL));
	_interface->connect_to_srv(ip, KPIU_SERVER_SLOT, KPIU_SERVER_SIGNAL);
}