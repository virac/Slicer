#ifndef __MUTEX_H__
#define __MUTEX_H__

#include "NonCopy.h"
#include "Config.h"

#ifdef SYSTEM_WINDOWS
#include <windows.h>
typedef CRITICAL_SECTION MUTEX_TYPE;
#elif defined SYSTEM_LINUX
#include <pthread.h>
typedef pthread_mutex_t MUTEX_TYPE;
#endif

namespace Kernal {
#ifdef SYSTEM_WINDOWS
typedef CRITICAL_SECTION MUTEX_TYPE;
#elif defined SYSTEM_LINUX
typedef pthread_mutex_t MUTEX_TYPE;
#endif

class Mutex: NonCopy {
public:
	/** Default constructor */
	Mutex();
	/** Default destructor */
	~Mutex();

	void Lock();
	void Unlock();
private:
	MUTEX_TYPE handle; //!< Member variable "m_mutex"
};
}
#endif // __MUTEX_H__
