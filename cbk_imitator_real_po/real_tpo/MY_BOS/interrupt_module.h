#ifndef INTERRUPT_IMIT_FOR_SM_TPO_H
#define INTERRUPT_IMIT_FOR_SM_TPO_H
#include "winsock2.h"
#include <QString>
#include <QObject>
#include <QTextEdit>
#include "interrupt_bus_rpc.h"
#include <QMutex>
#include "loki\Singleton.h"

#define SINGLETON_DEF(x) typedef Loki::SingletonHolder<x,Loki::CreateUsingNew,Loki::NoDestroy> S##x;

	class  InterImitObject : public QObject
	{
		Q_OBJECT
	public:
		InterImitObject();
		void create_signal_thread();
		void stop_signal_thread();
		void create_slot_thread();
		void stop_slot_thread();
		public slots:
		void get_new_interrupt(int _n, short _chan, double _u, double _t);
		RPC_interrupt_bus_SIGNAL_Thread* get_interrupt_signal_thread() { return interrupt_signal_thread; };
		RPC_interrupt_bus_SLOT_Thread* get_interrupt_slot_thread() { return interrupt_slot_thread; };

	private:
		RPC_interrupt_bus_SLOT_Thread* interrupt_slot_thread;
		RPC_interrupt_bus_SIGNAL_Thread* interrupt_signal_thread;
		QString log_filename;
		//QTimer log_timer;
		QStringList log_buffer;
		QMutex log_mutex;

	signals:
		void need_handle_interrupt(int inter_n);

	};

	SINGLETON_DEF(InterImitObject);


#endif
