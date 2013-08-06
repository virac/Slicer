#ifndef __MESSAGEHANDLER_H__
#define __MESSAGEHANDLER_H__

#include <vector>
#include <map>
#include <queue>
#include <string>
#include "Mutex.h"
#include "MessageHandlerThread.h"
#include "Letters.h"

namespace Kernal {
class MessageHandlerServer;
class MessageHandlerClient;
struct MessageHandlerArg {
	MessageHandlerArg() :
			message_type(0), priority(0), blocking(0), letter(NULL) {
	}
	;
	~MessageHandlerArg() {
		if (letter != NULL)
			delete letter;
	}
	;
	unsigned int message_type;
	unsigned int priority;
	unsigned int blocking;
	Letters::Letter *letter;

	bool operator<(const MessageHandlerArg &right) const {
		return priority < right.priority
				|| (priority == right.priority && blocking < right.blocking);
	}//TODO: needs fixing
	;
};
typedef void (*MessageHandlerCB)(void *, MessageHandlerArg*);
struct MessageHandlerCBHandle {
	MessageHandlerCB cb;
	void *handle;
	bool operator==(const MessageHandlerCBHandle &right) const {
		return handle == right.handle && cb == right.cb;
	}
	;
};
typedef std::vector<MessageHandlerCBHandle> MessageHandlerCBList;
struct MessageHandlerCBGroup {
	Mutex groupProtection;
	MessageHandlerCBList list;
};
typedef std::map<int, MessageHandlerCBGroup*> MessageHandlerCBListMap;

MessageHandlerServer* GetMyMessageHandlerServerPtr();
void CloseMyMessageHandlerServer();
bool MyMessageHandlerServerRunning();
unsigned int RegisterMessage(std::string intent);
unsigned int GetMessage(std::string intent);
std::string GetMessage(unsigned int id);

class MessageHandlerServer {
public:
	/** Default constructor */
	MessageHandlerServer();
	/** Default destructor */
	virtual ~MessageHandlerServer();

	void RegisterCB(MessageHandlerCBHandle callbackHandle, int message_type);
	void UnregisterCB(MessageHandlerCBHandle callbackHandle, int message_type);
	static std::string ShutDownMessage;
	friend class MessageHandlerClient;
	friend class MessageHandlerThread;
	friend bool MyMessageHandlerServerRunning();
protected:
private:
	bool Send();
	bool NoCallbacks();
	void ClearMessages();
	void ClearThreads();
	void CancelThreads();

	int maxTreads;
	int usedThreads;
	Mutex mapProtection;
	MessageHandlerCBListMap callbacks;
	Mutex messageProtection;
	std::priority_queue<MessageHandlerArg*> messageQueue;
	Mutex threadProtection;
	std::vector<MessageHandlerThread*> messageThreads;
};

class MessageHandlerClient {
public:
	void Finish();
	/** Default destructor */
	virtual ~MessageHandlerClient();

	void Send(MessageHandlerArg *arg);
	static void MessageHandlerMailBox(void* handle, MessageHandlerArg *arg);
	friend void MessageHandlerMailBox(void* handle, MessageHandlerArg *arg);
	void RegisterComm(std::string recv_msg, std::string send_msg);
protected:
	void Init(std::vector<std::string> &messages);
	std::vector<int> registeredMessages;
	std::map<std::string, std::string> comm_matrix;
private:
	virtual void cb(MessageHandlerArg* arg) = 0;
};

}
#endif // __MESSAGEHANDLER_H__
