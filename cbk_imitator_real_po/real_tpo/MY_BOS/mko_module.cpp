#include "mko_module.h"
#include <QString>
#include <QMessageBox>



union MKOWord
{
	quint16 com_word;				 // командное слово целиком
	struct
	{
		quint16 count_word : 5,    // число сл.данных / команда
			subadr : 5,		   // подадрес
			trans_dir : 1,     // направление передачи(1-чт.ќ”)
			adr : 5;
	};
};

MkoImitObject::MkoImitObject()
{

}

void MkoImitObject::slot_send_msg_mko(int mko, int line, int cwd, QVariantList& words, int& os)
{
	emit(signal_send_msg_mko(mko, line, cwd, words, os));
}

void MkoImitObject::slot_send_msg_def_mko(int adr, int subadr, int direct, int count_words, QVariantList& words)
{
	int mko = 0;
	int line = 1;
	int os = 1;

	MKOWord tmp_cwd;
	tmp_cwd.adr = adr;
	tmp_cwd.subadr = subadr;
	tmp_cwd.count_word = count_words;
	tmp_cwd.trans_dir = direct;

	emit(signal_send_msg_mko(mko, line, tmp_cwd.com_word, words, os));
}

void MkoImitObject::create_slot_thread()
{
	mko_slot_thread = new RPC_omnibus_SLOT_Thread;

	QString ip_str = "127.0.0.1";
	int slot_port = 30001;

	if (!mko_slot_thread->isRunning())
	{
		mko_slot_thread->set_connection_params(ip_str, slot_port);
		mko_slot_thread->start();
	}
	if (!mko_slot_thread->wait_connected(3))
	{
		//QMessageBox::critical(0, "Connection is missing", "Error with creating slot thread!");
	}
	//QObject::connect(this, SIGNAL(signal_send_msg_mko(int, int, int, QVariantList&, int&)), mko_slot_thread->get_omnibus_obj().get(), SLOT(send_msg(int, int, int, QVariantList&, int&)));
}

void MkoImitObject::stop_slot_thread()
{
	if (mko_slot_thread->isRunning())
	{
		mko_slot_thread->terminate();
		//QMessageBox::information(0, "Success", "The thread has been terminated!");
	}
	return;
}