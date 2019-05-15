#ifdef __cplusplus
extern "C"
{
#endif
#include "MainUnit.h"
#ifdef __cplusplus
}
#endif
#include "thread_module.h"
#include "interrupt_module.h"
#include "mko_module.h"

TPOThread::TPOThread() : QThread()
{
}

void TPOThread::run()
{
	MkoImitObject& mko_imit(SMkoImitObject::Instance());
	mko_imit.create_slot_thread();
	QObject::connect(&mko_imit, SIGNAL(signal_send_msg_mko(int, int, int, QVariantList&, int&)), mko_imit.get_mko_slot_thread()->get_omnibus_obj().get(), SLOT(send_msg(int, int, int, QVariantList&, int&)), Qt::DirectConnection);
	MainTask(0);
}


void TimeThread::run()
{
	BoardTime.start();
	exec();
}

void TimeThread::stop_thread()
{
	
}
int TimeThread::getCurTime()
{
	return BoardTime.elapsed();
}


void InterHandlerThread::run()
{
	is_running = TRUE;
	InterImitObject& interrupt_imit(SInterImitObject::Instance());
	InterruptStack.clear();
	interrupt_imit.create_signal_thread();
	interrupt_imit.create_slot_thread();
	QObject::connect(&interrupt_imit, SIGNAL(need_handle_interrupt(int)), this, SLOT(add_interrupt_to_stack(int)), Qt::DirectConnection);
	QObject::connect(interrupt_imit.get_interrupt_signal_thread()->get_obj().get(), SIGNAL(new_interrupt(int, short, double, double)), &interrupt_imit, SLOT(get_new_interrupt(int, short, double, double)), Qt::DirectConnection);
	while (is_running)
	{
		if ((check_stack()) && (!is_locked))
		{
			is_locked = TRUE;
			mutex.lock();
			process_interrupt(InterruptStack[0].toInt());
			InterruptStack.pop_front();
			mutex.unlock();
			is_locked = FALSE;
		}
	}
}

void InterHandlerThread::process_interrupt(int n_)
{
	int mask = 1 << n_;
	IrqTask(mask);
	return;
}

bool InterHandlerThread::check_stack()
{
	int stack_size = 0;
	stack_size = InterruptStack.size();
	if (stack_size > 0)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

void InterHandlerThread::add_interrupt_to_stack(int inter_n)
{
	is_locked = TRUE;
	mutex.lock();
	InterruptStack.append(inter_n);
	mutex.unlock();
	is_locked = FALSE;
}

void InterHandlerThread::stop_thread()
{
	is_running = FALSE;
}
