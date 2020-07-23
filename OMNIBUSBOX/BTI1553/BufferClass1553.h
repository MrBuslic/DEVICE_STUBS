#ifndef BUFFERCLASS1553_H
#define BUFFERCLASS1553_H

#ifdef _WIN32
#undef _WIN32
#endif
#include <BTICARD.h>
#include <BTI1553.H>
#define _WIN32

#include "omnibus_rpc.h"

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

enum F4_COMMAND_IMIT
{
	IMIT_BEGIN = -1,
	IMIT_TAKE_COMMAND,
	IMIT_SYNC,
	IMIT_TRANSMIT_BW,
	IMIT_START_SELFTEST,
	IMIT_BLOCK_TRANSMITTER,
	IMIT_DEBLOCK_TRANSMITTER,
	IMIT_BLOCK_ERROR_FLAG,
	IMIT_DEBLOCK_ERROR_FLAG,
	IMIT_REINIT,
	IMIT_RESERVE,/*********************/
	IMIT_SEND_VECTOR_WORD = 16,
	IMIT_SYNC_WITH_DW,
	IMIT_SEND_LAST_CMD,
	IMIT_SEND_VSK_WORD,
	IMIT_BLOCK_Nth_TRANSMITTER,
	IMIT_DEBLOCK_Nth_TRANSMITTER,
	IMIT_END
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
	RPC_omnibus_SLOT_Thread omnibus_slot_thr;
	RPC_omnibus_SIGNAL_Thread omnibus_signal_thr;
	int create_msg_addr(int addr, int saddr, int mko, bool f5);
	int create_list_addr(int addr, int saddr, int mko, int list_size, int tr);
	QMap<int, MsgAddr> msg_addrs;
	QMap<int, LstAddr> lst_addrs;
	QMutex msg_mutex;
	bool signal_connected = false;


	static rpc_buffer_class_1553& Instance()
	{
		static rpc_buffer_class_1553 inst;
		return inst;
	}
	rpc_buffer_class_1553(rpc_buffer_class_1553 const&) = delete;
	rpc_buffer_class_1553& operator= (rpc_buffer_class_1553 const&) = delete;

public slots:
	void new_message(QVariant dt, int mko, int line, int cwd, QVariantList words, int os);
private:
	rpc_buffer_class_1553() { msg_ind = 0; }

	int msg_ind;
};

typedef rpc_buffer_class_1553 Srpc_buffer_class_1553;

#endif