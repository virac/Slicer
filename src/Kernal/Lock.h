#ifndef __LOCK_H__
#define __LOCK_H__

#include "NonCopy.h"

namespace Kernal {

class Mutex;
class Lock: NonCopy {
public:
	/** Default constructor */
	Lock(Mutex &mutexIn);
	/** Default destructor */
	~Lock();
protected:
private:
	Mutex& mutex; //!< Member variable "mutex"
};

}

#endif // __LOCK_H__
