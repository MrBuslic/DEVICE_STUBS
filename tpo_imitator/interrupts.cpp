#include "interrupts.h"
#include <QtWidgets>

SimInterruptWid::SimInterruptWid() : QObject()
{
	//QVBoxLayout* v_lay = new QVBoxLayout(this);
	//edit = new QTextEdit(this);
	//_scroll_bar = edit->verticalScrollBar();
	//setMinimumSize(490, 300);
	//v_lay->addWidget(edit);
	
	/*slot_int_thread = new RPC_foi_SLOT_Thread;
	signal_int_thread = new RPC_foi_SIGNAL_Thread;
	
	if (!slot_int_thread->isRunning())
	{
		slot_int_thread->set_connection_params(ip_str, slot_port);
		slot_int_thread->start();
	}
	if (!slot_int_thread->wait_connected(3))
	{
		QMessageBox::critical(0, "Connection is missing", "Error with creating slot thread!");
	}

	if (!signal_int_thread->isRunning())
	{
		signal_int_thread->set_connection_params(ip_str, signal_port);
		signal_int_thread->start();
	}
	if (!signal_int_thread->wait_connected(3))
	{
		QMessageBox::critical(0, "Connection is missing", "Error with creating signal thread!");
	}*/

	//QObject::connect(signal_int_thread->get_obj().get(), SIGNAL(foi_interrupt(int, short, double, double)), this, SLOT(get_inter_from_server(int, short, double, double)), Qt::QueuedConnection);
	
	/*CBK_Thread = new CBK_MAIN_Thread();
	m_thread = new MyThread();
	CBK_Thread->moveToThread(m_thread);
	QObject::connect(CBK_Thread, SIGNAL(finished()), m_thread, SLOT(quit()));
	m_thread->start();
	CBK_Thread->process();

	QObject::connect(CBK_Thread, SIGNAL(signal_interrupt_1()), this, SLOT(slot_interrupt_1()));
	QObject::connect(CBK_Thread, SIGNAL(signal_interrupt_5()), this, SLOT(slot_interrupt_5()));
	QObject::connect(CBK_Thread, SIGNAL(signal_interrupt_13()), this, SLOT(slot_interrupt_13()));
	QObject::connect(CBK_Thread, SIGNAL(signal_unknown_interrupt()), this, SLOT(slot_unknown_interrupt()));*/
}
void SimInterruptWid::get_inter_from_server(int _n, short _chan, double _u, double _t)
{
	QString str;
	//edit->append("Interrupt is recieved!");

	if (_u < 3.3)
	{
		//edit->append("Amplitude of interrupt is lower than 3.3 V! Interrupt hasn't been catched!");
	}
	else
	{
		str.setNum(_n);
		//edit->append("Get interrupt number - "+ str +" ! Amplitude is more than 3.3 V. Interrupts has been catched!");
		emit signal_send_request();
	}
}

void SimInterruptWid::slot_interrupt_1()
{
	//edit->append("Interrupt 1 is handled!");
	emit signal_send_request();
}

void SimInterruptWid::slot_interrupt_5()
{
	//edit->append("Interrupt 5 is handled!");
}

void SimInterruptWid::slot_interrupt_13()
{
	//edit->append("Interrupt 13 is handled!");
}

void SimInterruptWid::slot_unknown_interrupt()
{
	//edit->append("Unknown interrupt is received! It's not handled!");
}

//RPC_foi_SLOT_Thread* SimInterruptWid::get_rpc_foi_slot_thread()
//{
//	return slot_int_thread;
//}
//
//RPC_foi_SIGNAL_Thread* SimInterruptWid::get_rpc_foi_signal_thread()
//{
//	return signal_int_thread;
//}

void SimInterruptWid::create_slot_thread()
{
	slot_int_thread = new RPC_foi_SLOT_Thread;

	QString ip_str = "127.0.0.1";
	int slot_port = 30001;

	if (!slot_int_thread->isRunning())
	{
		slot_int_thread->set_connection_params(ip_str, slot_port);
		slot_int_thread->start();
	}
	if (!slot_int_thread->wait_connected(3))
	{
		QMessageBox::critical(0, "Connection is missing", "Error with creating slot thread!");
	}
}

void SimInterruptWid::create_signal_thread()
{
	signal_int_thread = new RPC_foi_SIGNAL_Thread;

	QString ip_str = "127.0.0.1";
	int signal_port = 30002;

	if (!signal_int_thread->isRunning())
	{
		signal_int_thread->set_connection_params(ip_str, signal_port);
		signal_int_thread->start();
	}
	if (!signal_int_thread->wait_connected(3))
	{
		QMessageBox::critical(0, "Connection is missing", "Error with creating signal thread!");
	}
}