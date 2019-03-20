#include "interrupts.h"
#include <QMessageBox>
#include <QMutex>

Interrupt_Object::Interrupt_Object() : QObject()
{

}

void Interrupt_Object::create_slot_thread()
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

void Interrupt_Object::create_signal_thread()
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

void Interrupt_Object::stop_slot_thread()
{
	if (slot_int_thread->isRunning())
	{
		slot_int_thread->terminate();
		QMessageBox::information(0, "Success", "The slot thrread has been terminated!");
	}
	return;
}

void Interrupt_Object::stop_signal_thread()
{
	if (signal_int_thread->isRunning())
	{
		signal_int_thread->terminate();
		QMessageBox::information(0, "Success", "The signal thrread has been terminated!");
	}
	return;
}

void Interrupt_Object::restart_slot_thread(QString ip_str, int port)
{
	stop_slot_thread();
	slot_int_thread->set_connection_params(ip_str, port);
	slot_int_thread->start();
	if (!slot_int_thread->wait_connected(3))
	{
		QMessageBox::critical(0, "Connection is missing", "Error with creating interrupt slot thread!");
	}
}

void Interrupt_Object::restart_signal_thread(QString ip_str, int port)
{
	stop_signal_thread();
	signal_int_thread->set_connection_params(ip_str, port);
	signal_int_thread->start();
	if (!signal_int_thread->wait_connected(3))
	{
		QMessageBox::critical(0, "Connection is missing", "Error with creating interrupt signal thread!");
	}
}

void Interrupt_Object::get_inter_from_server(int _n, short _chan, double _u, double _t)
{
	if (_u < 3.3)
	{

	}
	else
	{
		add_inter_to_stack(_n);
	}
}

void Interrupt_Object::add_inter_to_stack(int _n)
{
	if (stack_of_interrupts.isEmpty())
	{
		stack_of_interrupts.append(_n);
		emit need_handle_interrupt();
	}
	else
	{
		stack_of_interrupts.append(_n);
	}
	return;
}

bool Interrupt_Object::check_inter_stack()
{
	bool check = false;
	if (stack_of_interrupts.size() > 0)
	{
		check = true;
	}
	else
	{
		check = false;
	}
	return check;
}

void Interrupt_Object::clear_inter_stack()
{
	stack_of_interrupts.clear();
	return;
}

QVector<int> Interrupt_Object::get_interrupts()
{
	QVector<int> buffer;
	buffer.clear();
	buffer = stack_of_interrupts;
	return buffer;
}