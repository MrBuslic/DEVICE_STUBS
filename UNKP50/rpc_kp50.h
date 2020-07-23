#ifndef RPC_KP50_H
#define RPC_KP50_H
#ifdef WIN32
#include "winsock2.h"
#endif
#include <QObject>
#include <QString>
#include <QWidget>
#include <QThread>
#include <QTextEdit>
#include <QScrollBar>
#include <QCheckBox>
#include <QTimer>
#include <QMutex>
#include <memory>
#include <qlayout.h>

#include "../buses_imitator/power_bus_rpc.h"

#include "rpc_loger.h"

enum KP50_CHANNELS
{
	KPALL_CHANNELS,
	KPCHANNEL_1,
	KPCHANNEL_2,
	KPCHANNEL_3,
	KPLAST
};

class RpcKP50Widget : public QWidget
{
	Q_OBJECT
public:
	RpcKP50Widget();
public slots:
	void set_u_in(double _u);

	int unkp50_switch_channel(int n, bool on);
	bool unkp50_channel_state_Q(int n);

	double unkp50_meas_I(int n);
	double unkp50_meas_Uin(int n);
	double unkp50_meas_Uout(int n);
private:
	LogWidget* log_widget;

	void switch_channel(int n, bool on);
	double u_in;
	QMap<int, bool> chans_states;
	QMap<int, QString> chan_names;

	RPC_power_bus_SLOT_Thread power_slot_thr;
	RPC_power_bus_SIGNAL_Thread power_signal_thr;
};

#endif //RPC_KP50_H