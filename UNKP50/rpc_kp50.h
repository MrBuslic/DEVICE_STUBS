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
#include <loki/Singleton.h>

#include "../buses_imitator/power_bus_rpc.h"

enum KP50_CHANNELS
{
	KPALL_CHANNELS,
	KPCHANNEL_1,
	KPCHANNEL_2,
	KPCHANNEL_3,
	KPLAST
};

#define SINGLETON_DEF(x) typedef Loki::SingletonHolder<x,Loki::CreateUsingNew,Loki::NoDestroy> S##x;

class RpcKP50Widget : public QWidget
{
	Q_OBJECT
public:
	RpcKP50Widget();
public slots:

	void auto_scroll_clicked(int _state);
	void log_timer_ontimer();

	void set_u_in(double _u);

	int unkp50_switch_channel(int n, bool on);
	bool unkp50_channel_state_Q(int n);

	double unkp50_meas_I(int n);
	double unkp50_meas_Uin(int n);
	double unkp50_meas_Uout(int n);
private:
	QTextEdit* edit;
	QScrollBar* _scroll_bar;
	QTextDocument* _doc;
	QTextCursor* _cursor;
	QCheckBox* auto_scroll_box;
	bool auto_scroll;
	QString log_filename;
	QTimer log_timer;
	QStringList log_buffer;
	QMutex log_mutex;

	void switch_channel(int n, bool on);
	void log_msg(const QString& msg);
	double u_in;
	QMap<int, bool> chans_states;
	QMap<int, QString> chan_names;

	RPC_power_bus_SLOT_Thread power_slot_thr;
	RPC_power_bus_SIGNAL_Thread power_signal_thr;
};

#endif //RPC_KP50_H