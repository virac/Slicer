#include "Lock.h"
#include "Mutex.h"

namespace Kernal {
Lock::Lock(Mutex &mutexIn) :
		mutex(mutexIn) {
	mutex.Lock();
}

Lock::~Lock() {
	mutex.Unlock();
}

}

