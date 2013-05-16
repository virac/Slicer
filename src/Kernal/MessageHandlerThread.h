#ifndef __MESSAGE_HANDLER_THREAD_H__
#define __MESSAGE_HANDLER_THREAD_H__

#include "Thread.h"

namespace Kernal {
class MessageHandlerThread: public Thread {
public:
	MessageHandlerThread() :
			running(true), processing(false), myId(-1) {
	}
	bool running;
	bool processing;
	Mutex action;
	int myId;
protected:
private:
	virtual void Run();
};
}
#endif // __MESSAGE_HANDLER_THREAD_H__
