#ifndef TASK_THREAD
#define TASK_THREAD

#include <qmutex.h>
#include <qlist.h>
#include <qthread.h>
#include <loki/Singleton.h>

#ifndef SINGLETON_DEF
#define SINGLETON_DEF(x) typedef Loki::SingletonHolder<x,Loki::CreateUsingNew,Loki::NoDestroy> S##x;
#endif 

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
	friend struct Loki::CreateUsingNew<TasksFactory>;

	void create_task(USHORT task_ind, int start_arg);
	void run();
private:
	TasksFactory();
	QList<TASK_STR> tasks_queue;
	QList<QMutex*> mutexes;
	QMutex task_mutex;

};
SINGLETON_DEF(TasksFactory);
#endif