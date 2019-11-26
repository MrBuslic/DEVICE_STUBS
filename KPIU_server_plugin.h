#ifndef KPIU_SERVER_PLUGIN_H
#define KPIU_SERVER_PLUGIN_H

#include <qobject.h>
#include "ServerPluginInterface.hpp"

class KpiuServerPlugin : public QObject, public ServerPluginInterface
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID "KPIUServerPlugin");
	Q_INTERFACES(ServerPluginInterface)

public:
	virtual ~KpiuServerPlugin() { if (_interface) delete _interface; }
	virtual void connect(const QString& ip);
	virtual QString get_name() { return "KPIU_server"; }
};

#endif // TEST_SERVER_PLUGIN_H
