#include "Thread.h"

#ifdef SYSTEM_WINDOWS
#include <windows.h>
#include <process.h>
#endif

namespace Kernal {
int GetCoreCount() {
#ifdef SYSTEM_WINDOWS
	SYSTEM_INFO sysinfo;
	GetSystemInfo(&sysinfo);
	return sysinfo.dwNumberOfProcessors;
#elif defined SYSTEM_LINUX
	return sysconf(_SC_NPROCESSORS_ONLN);
#endif
}

int GetThreadID() {
#ifdef SYSTEM_WINDOWS
	return GetCurrentThreadId();
#elif defined SYSTEM_LINUX
	return getpid();
#endif
}

int GetThreadID(THREAD_TYPE thread) {
#ifdef SYSTEM_WINDOWS
	return 0; //i have no idea how to get this
#elif defined SYSTEM_LINUX
	return (int) thread;
#endif
}

Thread::Thread(ThreadFuncion function, ThreadData *data_In) :
		m_thread(NULL), m_is_active(false), m_function(function), m_data(data_In) {
}

Thread::Thread() :
		m_thread(NULL), m_is_active(false), m_function(NULL), m_data(NULL) {
}

Thread::~Thread() {
	if (m_is_active)
		this->Wait();
}

void Thread::SetData(ThreadData *data_In) {
	this->m_data = data_In;
}

void Thread::Launch() {
	Wait();
	m_is_active = true;
	int err = 0;
#ifdef SYSTEM_WINDOWS
	this->m_thread = reinterpret_cast<THREAD_TYPE>(_beginthreadex(NULL, 0,
			&Thread::ThreadEntry, this, 0, NULL));
	err = (this->m_thread == NULL) ? 1 : 0;
#elif defined SYSTEM_LINUX
	err = pthread_create( &this->m_thread, NULL, &Thread::ThreadEntry, this );
#endif

	if (err != 0) {
		this->m_is_active = false;
	}
}
void Thread::Wait() {
	if (this->m_is_active) {
#ifdef SYSTEM_WINDOWS
		WaitForSingleObject(this->m_thread, INFINITE);
		CloseHandle(this->m_thread);
		this->m_thread = NULL;
#elif defined SYSTEM_LINUX
		pthread_join( this->m_thread, NULL );
#endif
		this->m_is_active = false;
	}
}
void Thread::Terminate() {
	if (this->m_is_active) {
#ifdef SYSTEM_WINDOWS
		TerminateThread(this->m_thread, 0);
		this->m_thread = NULL;
#elif defined SYSTEM_LINUX
		pthread_cancel( this->m_thread );
		this->m_is_active = false;
#endif
	}
}

void Thread::Run() {
	if (this->m_function)
		this->m_function(this->m_data);
}

THREAD_ENTRY_TYPE Thread::ThreadEntry(void* entry_point) {
	Thread *runnable = reinterpret_cast<Thread*>(entry_point);
#ifdef SYSTEM_LINUX
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);
#endif
	runnable->Run();

#ifdef SYSTEM_WINDOWS
	_endthreadex(0);
	return 0;
#elif defined SYSTEM_LINUX
	return NULL;
#endif
}
}

