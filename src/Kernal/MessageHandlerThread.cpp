#include "MessageHandlerThread.h"
#include "MessageHandler.h"
#include "Watch.h"
#include "Lock.h"
#include "Log.h"

namespace Kernal {
double secsToTimeout = 2;

void MessageHandlerThread::Run() {
	ThreadData *data = NULL;
	this->myId = Kernal::GetThreadID();
	Watch timeout;
	{
		LOG("MessageHandlerThread::Starting");
		LOG_DEBUG("New thread created id: \"" + ToString(this->myId) + "\"");
		while (!this->processing && this->running) // wait until the thread is set up and ready to go
		{
			if (timeout.GetSplit() > secsToTimeout) {
				this->running = false;
			} else
				Kernal::Watch::SleepSec(0.001f);
		}
		if (this->running == false) {
			LOG_WARN(
					"Thread \"" + ToString(this->myId)
							+ "\" did not receive data before timeout.");
		}
	}
	{
		MessageHandlerServer *mailServer = GetMyMessageHandlerServerPtr();
		LOG("MessageHandlerThread::Running");
		while (this->running) {
			data = this->GetData();
			if (data != NULL) {
				Lock dataLock(data->m_mutex);
				LOG_DEBUG("Locking DATA");
				MessageHandlerArg *arg =
						reinterpret_cast<MessageHandlerArg*>(data->data);

				static unsigned int shutdownId = Kernal::GetMessage(
						Kernal::MessageHandlerServer::ShutDownMessage);
				if (arg->message_type == shutdownId) {
					LOG_DEBUG(
							"Finished sending shutdown. Shutting server down.");
					mailServer->CancelThreads();
					mailServer->ClearMessages(); //sent the shutdown to all listeners now remove all remaining messages.
				}

				if (arg->message_type != 0) {
					LOG_DEBUG(
							"Sending message " + ToString(Kernal::GetMessage(arg->message_type)) + " to Mailbox");
					MessageHandlerCBListMap::iterator exists =
							mailServer->callbacks.find(arg->message_type);
					if (exists != mailServer->callbacks.end()) {
						LOG_DEBUG("Group protection locked");
						Lock lock(exists->second->groupProtection);
						int i = 0;
						int startSize = exists->second->list.size();
						for (MessageHandlerCBList::reverse_iterator iter =
								exists->second->list.rbegin();
								i < startSize
										&& iter != exists->second->list.rend();
								iter++) {
							LOG_DEBUG(
									"Calling Mailbox CB " + ToString(++i)
											+ " of "
											+ ToString(
													exists->second->list.size())
											+ " at " + ToString(iter->handle));
							iter->cb(iter->handle, arg);
						}
						LOG_DEBUG(
								"Finished sending to " + ToString(i)
										+ " callbacks");
						LOG_DEBUG("Group protection UNLOCKED");
					}
				} else {
					LOG_WARN("Trying to send 0 message");
				}

				LOG_DEBUG("unlocking DATA");
			}

			if (data != NULL) {
				MessageHandlerArg *arg =
						reinterpret_cast<MessageHandlerArg*>(data->data);
				if (arg != NULL) {
					LOG_DEBUG(
							"Finished with arg deleting: "
									+ Kernal::ToString(arg));
					delete arg;
					arg = NULL;
				}
				LOG_DEBUG("Deleting old data: " + Kernal::ToString(data));
				delete data;
				data = NULL;
			}

			{
				LOG_DEBUG("Removing myself from active threads.");
				Lock threadLock(mailServer->threadProtection);
				--mailServer->usedThreads;
				this->processing = false;
			}
			LOG_DEBUG("WAITING*$&$^##*#* \"" + ToString(this->myId) + "\"");
			timeout.Reset();
			while (!this->processing && this->running) //nothing waitg
			{
				if (timeout.GetSplit() > secsToTimeout) {
					LOG_DEBUG(
							"Timing thread out.");
					this->running = false;
					break;
				}
				if (timeout.GetSplit() < 0.001f)
					LOG_DEBUG("Checking to send data.");
				if (mailServer != NULL) {
					mailServer->Send();
				} else {
					LOG_ERROR("CRUD___-___--");
				}
				if (!this->processing)
					Kernal::Watch::SleepSec(0.001f);
				else {
					LOG_DEBUG(
							"Got New data Looping Thread:"
									+ ToString(this->myId));
				}
			}
			LOG_DEBUG(
					"We now "
							+ std::string(
									this->running == false ? "are not" : "are")
							+ " running and "
							+ std::string(
									this->processing == false ?
											"are not" : "are") + " processing");
		}
	}

	LOG("MessageHandlerThread::Stopping");
	LOG_DEBUG("Cleaning up thread and shutting down.");
	if (data != NULL) {
		MessageHandlerArg *arg =
				reinterpret_cast<MessageHandlerArg*>(data->data);
		if (arg != NULL) {
			LOG_DEBUG("Finished with arg deleting: " + Kernal::ToString(arg));
			delete arg;
			arg = NULL;
		} else {
			LOG_DEBUG("arg was alrdy deleted");
		}
		LOG_DEBUG("Deleting old data: " + Kernal::ToString(data));
		delete data;
		data = NULL;
	} else {
		LOG_DEBUG("data was alrdy deleted");
	}

	/*need to remove myself form the list of threads
	{//currently doesnt work

		MessageHandlerServer *mailServer = GetMyMessageHandlerServerPtr();
		Lock t(mailServer->threadProtection);
		std::vector<MessageHandlerThread*>::iterator iter;
		for (iter = mailServer->messageThreads.begin();
				iter != mailServer->messageThreads.end()
						&& (*iter)->myId != this->myId; iter++) {
			LOG_WRITE("Got id:" + ToString((*iter)->myId));
		}
		if (iter != mailServer->messageThreads.end()
				&& (*iter)->myId != this->myId) {
			LOG_WRITE("Found myself in the thread list and removing it not.");
			mailServer->messageThreads.erase(iter);
		} else {
			LOG_WRITE("Could not find myself in the thread list.");
		}
	}*/

	Kernal::MyMessageHandlerServerRunning();
	LOG_STACK;
}
}

