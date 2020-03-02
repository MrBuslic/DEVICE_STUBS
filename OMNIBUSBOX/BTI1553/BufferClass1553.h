#ifndef BUFFERCLASS1553_H
#define BUFFERCLASS1553_H

#ifdef _WIN32
#undef _WIN32
#endif
#include <BTICARD.h>
#include <BTI1553.H>
#define _WIN32

#include "omnibus_rpc.h"

#ifndef SINGLETON_DEF
#define SINGLETON_DEF(x) typedef Loki::SingletonHolder<x,Loki::CreateUsingNew,Loki::NoDestroy> S##x;
#endif
//структура командного слова сообщения МКО
//
union MkoWord1553
{
	quint16 cw;				 // командное слово целиком
	struct
	{
		quint16 count : 5,    // число сл.данных / команда
	subadr : 5,   // подадрес
		 tr : 1,       // направление передачи(1-чт.ОУ)
		  adr : 5;
	};
};

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

struct MsgAddr
{
	int addr;
	int saddr;
	int mko;
	bool f5;
};

struct LstAddr
{
	int addr;
	int saddr;
	int mko;
	int lst_size;
	QVariantList words;
};

class rpc_buffer_class_1553 : public QObject
{
	Q_OBJECT
public:
	rpc_buffer_class_1553() { msg_ind = 0;}
	RPC_omnibus_SLOT_Thread omnibus_slot_thr;
	RPC_omnibus_SIGNAL_Thread omnibus_signal_thr;
	int create_msg_addr(int addr, int saddr, int mko, bool f5);
	int create_list_addr(int addr, int saddr, int mko, int list_size, int tr);
	QMap<int, MsgAddr> msg_addrs;
	QMap<int, LstAddr> lst_addrs;
	QMutex msg_mutex;
	bool signal_connected = false;
public slots:
	void new_message(QVariant dt, int mko, int line, int cwd, QVariantList words, int os);
private:
	int msg_ind;
};

SINGLETON_DEF(rpc_buffer_class_1553);

#endif