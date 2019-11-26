#include "CBK_MAIN_Thread.h"

MyThread::MyThread()
{

}

void MyThread::run()
{
	exec();
}

CBK_MAIN_Thread::CBK_MAIN_Thread()
{
	connect(&m_timer, SIGNAL(timeout()), SLOT(check_interrupt()));
	connect(this, SIGNAL(signal_handle_interrupt()), this, SLOT(slot_handle_interrupt()));
}

void CBK_MAIN_Thread::append_interrupt(int num_inter)
{
	m_mutex.lock();
	stack_of_interrupts.append(num_inter);
	m_mutex.unlock();
}

void CBK_MAIN_Thread::stop_process()
{
	m_timer.stop();
	emit finished();
}

QVector<int> CBK_MAIN_Thread::get_interrupt()
{
	m_mutex.lock();
	QVector<int> buffer;
	buffer = stack_of_interrupts;
	stack_of_interrupts.clear();
	m_mutex.unlock();
	return buffer;
}

int CBK_MAIN_Thread::get_qty_interrupt()
{
	int buffer;
	m_mutex.lock();
	buffer = stack_of_interrupts.size();
	m_mutex.unlock();
	return buffer;
}

void CBK_MAIN_Thread::check_interrupt()
{
	m_mutex.lock();
	emit checking_interrupts();
	if (stack_of_interrupts.size()>0)
	{
		m_mutex.unlock();
		emit signal_handle_interrupt();
		return;
	}
	m_mutex.unlock();
}

void CBK_MAIN_Thread::process()
{
	m_timer.setInterval(4000);
	m_timer.start();
}

void CBK_MAIN_Thread::slot_handle_interrupt()
{
	QVector<int> buffer;
	buffer = this->get_interrupt();
	for (int i = 0; i < buffer.size(); i++)
	{
		switch (buffer[i])
		{
		case 1:
			emit signal_interrupt_1();
			break;
		case 5:
			emit signal_interrupt_5();
			break;
		case 13:
			emit signal_interrupt_13();
			break;
		default:
			emit signal_unknown_interrupt();
			break;
		}
	}
}
