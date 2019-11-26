#ifndef TASK_THREAD
#define TASK_THREAD

#include <qmutex.h>
#include <qlist.h>
#include <loki/Singleton.h>

#ifndef SINGLETON_DEF
#define SINGLETON_DEF(x) typedef Loki::SingletonHolder<x,Loki::CreateUsingNew,Loki::NoDestroy> S##x;
#endif 
class TasksFactory
{
public:
	void lock_mutex(int ind, int timeout);
	void unlock_mutex(int ind);
	int add_mutex();
	void remove_mutex(int ind);
	friend struct Loki::CreateUsingNew<TasksFactory>;
private:
	TasksFactory();
	QList<QMutex*> mutexes;

};
SINGLETON_DEF(TasksFactory);
#endif