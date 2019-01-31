#include "mko_imit.h"
#include <QString>
#include <QMessageBox>

union MKOWord
{
	quint16 com_word;				 // командное слово целиком
	struct
	{
		quint16 count_word : 5,    // число сл.данных / команда
				subadr : 5,   // подадрес
				trans_dir : 1,       // направление передачи(1-чт.ќ”)
				adr : 5;
	};
};

MKO_Widget::MKO_Widget() : QObject()
{
	/*QString ip_str = "127.0.0.1";
	int slot_port = 50001;
	int signal_port = 50002;


	mko_slot_thread = new RPC_omnibus_SLOT_Thread;
	mko_signal_thread = new RPC_omnibus_SIGNAL_Thread;

	if (!mko_slot_thread->isRunning())
	{
		mko_slot_thread->set_connection_params(ip_str, slot_port);
		mko_slot_thread->start();
	}

	if (!mko_signal_thread->isRunning())
	{
		mko_signal_thread->set_connection_params(ip_str, signal_port);
		mko_signal_thread->start();
	}

	qRegisterMetaType<QVariantList>("QVariantList&");
	qRegisterMetaType<int>("int&");
	QObject::connect(this, SIGNAL(signal_send_new_msg(int, int, int, QVariantList&, int&)), mko_slot_thread->get_obj().get(), SLOT(send_msg(int, int, int, QVariantList&, int&)), Qt::DirectConnection);*/
}

void MKO_Widget::slot_send_new_msg(int mko, int line, int cwd, QVariantList& words, int& os)
{
	emit(signal_send_new_msg(mko, line, cwd, words, os));
}

void MKO_Widget::send_message(int adr, int subadr, int direct, int count_words, QVariantList& words)
{
	int mko = 1;
	int line = 0;
	int os = 1;

	MKOWord tmp_cwd;
	tmp_cwd.adr = adr;
	tmp_cwd.subadr = subadr;
	tmp_cwd.count_word = count_words;
	tmp_cwd.trans_dir = direct;

	emit(signal_send_new_msg(mko, line, tmp_cwd.com_word, words, os));
}

void MKO_Widget::create_slot_thread()
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