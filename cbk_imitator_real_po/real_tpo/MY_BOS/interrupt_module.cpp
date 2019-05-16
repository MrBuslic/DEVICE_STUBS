#include "interrupt_module.h"
#include <QString>
#include <QMessageBox>


InterImitObject::InterImitObject()
{

}

void InterImitObject::create_slot_thread()
{
	interrupt_slot_thread = new RPC_interrupt_bus_SLOT_Thread;

	QString ip_str = "127.0.0.1";
	int slot_port = 30010;

	if (!interrupt_slot_thread->isRunning())
	{
		interrupt_slot_thread->set_connection_params(ip_str, slot_port);
		interrupt_slot_thread->start();
	}
	if (!interrupt_slot_thread->wait_connected(3))
	{
		//QMessageBox::critical(0, "Connection is missing", "Error with creating slot thread!");
	}
}

void InterImitObject::stop_slot_thread()
{
	if (interrupt_slot_thread->isRunning())
	{
		interrupt_slot_thread->terminate();
		//QMessageBox::information(0, "Success", "The thread has been terminated!");
	}
	return;
}

void InterImitObject::create_signal_thread()
{
	interrupt_signal_thread = new RPC_interrupt_bus_SIGNAL_Thread;
	QString ip_str = "127.0.0.1";
	int signal_port = 30011;

	if (!interrupt_signal_thread->isRunning())
	{
		interrupt_signal_thread->set_connection_params(ip_str, signal_port);
		interrupt_signal_thread->start();
	}
	if (!interrupt_signal_thread->wait_connected(3))
	{
		//QMessageBox::critical(0, "Connection is missing", "Error with creating slot thread!");
	}
}

void InterImitObject::stop_signal_thread()
{
	if (interrupt_signal_thread->isRunning())
	{
		interrupt_signal_thread->terminate();
		//QMessageBox::information(0, "Success", "The thread has been terminated!");
	}
	return;
}

void InterImitObject::get_new_interrupt(int _n, short _chan, double _u, double _t)
{
	if (_u < 3.3)
	{
		return;
	}
	else
	{
		int mask = 1 << _n;
		if (~IrqMask & mask == mask)
		{
			emit need_handle_interrupt(mask);
		}
		else
		{
			return;
		}
	}
}

void InterImitObject::set_IrqMask(int tmp_mask)
{
	IrqMask = tmp_mask;
}