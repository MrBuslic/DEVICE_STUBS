#ifndef TASK_THREAD
#define TASK_THREAD

#include <qmutex.h>
#include <qlist.h>
#include <qthread.h>

struct TASK_STR
{
	USHORT task_ind;
	int start_arg;
};

class TasksFactory : public QThread
{
public:
	void lock_mutex(int ind, int timeout);
	void unlock_mutex(int ind);
	int add_mutex();
	void remove_mutex(int ind);

	void create_task(USHORT task_ind, int start_arg);
	void run();


	static TasksFactory& Instance()
	{
		static TasksFactory inst;
		return inst;
	}
	TasksFactory(TasksFactory const&) = delete;
	TasksFactory& operator= (TasksFactory const&) = delete;

private:
	TasksFactory();
	QList<TASK_STR> tasks_queue;
	QList<QMutex*> mutexes;
	QMutex task_mutex;

};
typedef TasksFactory STasksFactory;
#endif