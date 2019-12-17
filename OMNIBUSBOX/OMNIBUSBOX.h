#ifndef OMNIBUSBOX_H
#define OMNIBUSBOX_H
#ifdef _WIN32
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
#include "rpc_ports.h"
#include "rpc_loger.h"

enum F4_COMMAND
{
	BEGIN = -1,
	TAKE_COMMAND,
	SYNC,
	TRANSMIT_BW,
	START_SELFTEST,
	BLOCK_TRANSMITTER,
	DEBLOCK_TRANSMITTER,
	BLOCK_ERROR_FLAG,
	DEBLOCK_ERROR_FLAG,
	REINIT,
	RESERVE,/*********************/
	SEND_VECTOR_WORD = 16,
	SYNC_WITH_DW,
	SEND_LAST_CMD,
	SEND_VSK_WORD,
	BLOCK_Nth_TRANSMITTER,
	DEBLOCK_Nth_TRANSMITTER,
	END
};

#define SINGLETON_DEF(x) typedef Loki::SingletonHolder<x,Loki::CreateUsingNew,Loki::NoDestroy> S##x;

struct RpcAbonent
{
	RpcAbonent(){}
	RpcAbonent(int addr);
	QMap<int, QList<unsigned short>> words;
	QMap<int, int> os;
	bool on;
	int line;
	int last_os;
};

class RpcOmnibusWidget : public QWidget
{
	Q_OBJECT
public:
	RpcOmnibusWidget(QWidget* parent = 0);
public slots:

	void switch_ab_os(int mko, int addr, int _os, int _s_addr = -1);
	void switch_ab(int mko, int addr, bool _on);
	void set_new_data(int mko, int addr, int saddr, QVariantList words);
	void send_msg(int mko, int line, int cwd, QVariantList& words, int& os);

	void send_msg_mpko(int mko, int line, int cwd, QVariantList& words, int& os);
	int unomnibus_map_channels_setup(int _n, int _chan);
	QVariant get_dt();

private:

	QMap<short, QMap<short, RpcAbonent>> abonents;
	QMap<int, int> map_channels;//словарь исправности каналов

	int n;
	short chan;
	double u;
	double t;
	LogWidget* log_widget;
signals:
	void new_message(QVariant dt, int mko, int line, int cwd, QVariantList words, int os);
	void new_message_mpko(QVariant dt, int mko, int line, int cwd, QVariantList words, int os);
	void message_to_log(QString _msg);
};

#endif //RPC_FOI_H