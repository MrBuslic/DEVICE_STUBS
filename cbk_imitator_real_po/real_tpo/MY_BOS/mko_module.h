#ifndef MKO_IMIT_FOR_SM_TPO_H
#define MKO_IMIT_FOR_SM_TPO_H
#include "winsock2.h"
#include <QString>
#include <QObject>
#include <QTextEdit>
#include "omnibus_rpc.h"
#include <QMutex>


	class  MkoImitObject : public QObject
	{
		Q_OBJECT
	public:
		
		void create_slot_thread();
		void stop_slot_thread();
		public slots:
		void slot_send_msg_mko(int mko, int line, int cwd, QVariantList& words, int& os);
		void slot_send_msg_def_mko(int adr, int subadr, int direct, int count_words, QVariantList& words);
		RPC_omnibus_SLOT_Thread* get_mko_slot_thread() { return mko_slot_thread; };

		static MkoImitObject& Instance()
		{
			static MkoImitObject inst;
			return inst;
		}

	private:
		MkoImitObject();
		RPC_omnibus_SLOT_Thread* mko_slot_thread;
		RPC_omnibus_SIGNAL_Thread* mko_signal_thread;
		QString log_filename;
		//QTimer log_timer;
		QStringList log_buffer;
		QMutex log_mutex;

	signals:
		void signal_send_msg_mko(int mko, int lin, int cwd, QVariantList& words, int& os);

	};

	typedef MkoImitObject SMkoImitObject;


#endif
