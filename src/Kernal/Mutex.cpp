#include "Mutex.h"

namespace Kernal {

Mutex::Mutex() {
#ifdef SYSTEM_WINDOWS
	InitializeCriticalSection(&this->handle);
#elif defined SYSTEM_LINUX
	pthread_mutex_init( &this->handle, NULL );
#endif
}

Mutex::~Mutex() {
#ifdef SYSTEM_WINDOWS
	DeleteCriticalSection(&this->handle);
#elif defined SYSTEM_LINUX
	pthread_mutex_destroy( &this->handle );
#endif
}

void Mutex::Lock() {
#ifdef SYSTEM_WINDOWS
	EnterCriticalSection(&this->handle);
#elif defined SYSTEM_LINUX
	pthread_mutex_lock( &this->handle );
#endif
}
void Mutex::Unlock() {
#ifdef SYSTEM_WINDOWS
	LeaveCriticalSection(&this->handle);
#elif defined SYSTEM_LINUX
	pthread_mutex_unlock( &this->handle );
#endif
}
}

