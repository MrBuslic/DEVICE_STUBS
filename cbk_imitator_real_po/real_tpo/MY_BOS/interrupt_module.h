#ifndef INTERRUPT_IMIT_FOR_SM_TPO_H
#define INTERRUPT_IMIT_FOR_SM_TPO_H
#include "winsock2.h"
#include <QString>
#include <QObject>
#include <QTextEdit>
#include "interrupt_bus_rpc.h"
#include <QMutex>

	class  InterImitObject : public QObject
	{
		Q_OBJECT
	public:

		static InterImitObject& Instance()
		{
			static InterImitObject inst;
			return inst;
		}
		InterImitObject(InterImitObject const&) = delete;
		InterImitObject& operator= (InterImitObject const&) = delete;

		void create_signal_thread();
		void stop_signal_thread();
		void create_slot_thread();
		void stop_slot_thread();
		public slots:
		void get_new_interrupt(int _n, short _chan, double _u, double _t);
		void set_IrqMask(int tmp_mask);

		void clear_irq_channel(int _n);
		int get_irq_channel(int _n);

		RPC_interrupt_bus_SIGNAL_Thread* get_interrupt_signal_thread() { return interrupt_signal_thread; };
		RPC_interrupt_bus_SLOT_Thread* get_interrupt_slot_thread() { return interrupt_slot_thread; };

	private:
		InterImitObject();

		int IrqMask;
		int GotIrqMask;
		RPC_interrupt_bus_SLOT_Thread* interrupt_slot_thread;
		RPC_interrupt_bus_SIGNAL_Thread* interrupt_signal_thread;
		QString log_filename;
		//QTimer log_timer;
		QStringList log_buffer;
		QMutex log_mutex;

	signals:
		void need_handle_interrupt(int inter_n);

	};

	typedef InterImitObject SInterImitObject;


#endif
