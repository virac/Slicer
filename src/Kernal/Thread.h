#ifndef __THREAD_H__
#define __THREAD_H__

#include "Mutex.h"

#include "NonCopy.h"
#include "Config.h"

#ifdef SYSTEM_WINDOWS
#include <windows.h>
#define THREAD_API __stdcall
#elif defined SYSTEM_LINUX
#include <pthread.h>
#define THREAD_API
#endif
namespace Kernal {
#ifdef SYSTEM_WINDOWS
typedef HANDLE THREAD_TYPE;
typedef unsigned int THREAD_ENTRY_TYPE;
#elif defined SYSTEM_LINUX
typedef pthread_t THREAD_TYPE;
typedef void* THREAD_ENTRY_TYPE;
#endif

int GetCoreCount();
int GetThreadID();
int GetThreadID(THREAD_TYPE thread);

struct ThreadData {
	ThreadData() :
			data(NULL) {
	}
	;
	Mutex m_mutex; //!< Mutex for data
	void *data;
};

typedef void (*ThreadFuncion)(ThreadData *);

class Thread {
public:
	Thread(ThreadFuncion function, ThreadData *data_In);
	virtual ~Thread();

	void SetData(ThreadData *data_In);

	void Launch();
	void Wait();
	void Terminate();
protected:
	Thread();
	ThreadData* GetData() {
		return this->m_data;
	}
private:
	virtual void Run();

	static THREAD_ENTRY_TYPE THREAD_API ThreadEntry(void* entry_point);

	THREAD_TYPE m_thread;
	bool m_is_active;
	ThreadFuncion m_function;
	ThreadData* m_data;
};
}

#endif
