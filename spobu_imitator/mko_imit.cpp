#include "mko_imit.h"
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

MKO_IMIT_Object::MKO_IMIT_Object()
{
	
}

void MKO_IMIT_Object::slot_send_msg_mko(int mko, int line, int cwd, QVariantList& words, int& os)
{
	emit(signal_send_msg_mko(mko, line, cwd, words, os));
}

void MKO_IMIT_Object::slot_send_msg_def_mko(int adr, int subadr, int direct, int count_words, QVariantList& words)
{
	int mko = 1;
	int line = 1;
	int os = 1;

	MKOWord tmp_cwd;
	tmp_cwd.adr = adr;
	tmp_cwd.subadr = subadr;
	tmp_cwd.count_word = count_words;
	tmp_cwd.trans_dir = direct;

	emit(signal_send_msg_mko(mko, line, tmp_cwd.com_word, words, os));
}

void MKO_IMIT_Object::create_slot_thread()
{
	mko_slot_thread = new RPC_omnibus_SLOT_Thread;

	QString ip_str = "127.0.0.1";
	int slot_port = 50001;

	if (!mko_slot_thread->isRunning())
	{
		mko_slot_thread->set_connection_params(ip_str, slot_port);
		mko_slot_thread->start();
	}
	if (!mko_slot_thread->wait_connected(3))
	{
		QMessageBox::critical(0, "Connection is missing", "Error with creating slot thread!");
	}
}

void MKO_IMIT_Object::stop_slot_thread()
{
	if (mko_slot_thread->isRunning())
	{
		mko_slot_thread->terminate();
		QMessageBox::information(0, "Success", "The thrread has been terminated!");
	}
	return;
}

void MKO_IMIT_Object::restart_slot_thread(QString ip_str, int port)
{
	stop_slot_thread();
	mko_slot_thread->set_connection_params(ip_str, port);
	mko_slot_thread->start();
	if (!mko_slot_thread->wait_connected(3))
	{
		QMessageBox::critical(0, "Connection is missing", "Error with creating slot thread!");
	}
}