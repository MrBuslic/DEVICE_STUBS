#ifndef BUFFERCLASS_H
#define BUFFERCLASS_H

#ifdef _WIN32
#undef _WIN32
#endif
#include <BTICARD.h>
#include <BTI1553.H>
#define _WIN32

#include "omnibus_rpc.h"

#define SINGLETON_DEF(x) typedef Loki::SingletonHolder<x,Loki::CreateUsingNew,Loki::NoDestroy> S##x;

//структура командного слова сообщения МКО
//
union MkoWord
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

class rpc_buffer_class : public QObject
{
	Q_OBJECT
private:
	QList <SEQRECORD1553> msgs;
	QList <SEQRECORDMORE1553> msgs_more;
	QMutex msg_mutex;

public:
	RPC_omnibus_SLOT_Thread omnibus_slot_thr;
	RPC_omnibus_SIGNAL_Thread omnibus_signal_thr;
	int get_msgs_size();
	SEQRECORD1553* get_msg();
	SEQRECORDMORE1553* get_more();
public slots:
	void new_message(QVariant dt, int mko, int line, int cwd, QVariantList words, int os);
private:
	SEQRECORD1553 last_msg;
	SEQRECORDMORE1553 more_msg;
};

SINGLETON_DEF(rpc_buffer_class);
#endif