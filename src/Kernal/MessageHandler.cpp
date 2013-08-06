#include "MessageHandler.h"
#include "Mutex.h"
#include "Lock.h"
#include "Thread.h"
#include "Log.h"

namespace Kernal {

MessageHandlerServer *g_messageHandlerServer = NULL;
std::string MessageHandlerServer::ShutDownMessage = "global_shutdown";
MessageHandlerServer* GetMyMessageHandlerServerPtr() {
	if (g_messageHandlerServer == NULL) {
		LOG("GetMyMessageHandlerServerPtr");
		LOG_ERROR("Newing a Message Handler Server");
		g_messageHandlerServer = new MessageHandlerServer();
	}
	return g_messageHandlerServer;
}

void CloseMyMessageHandlerServer() {
	if (g_messageHandlerServer != NULL) {
		LOG("CloseMyMessageHandlerServer");
		LOG_ERROR("Deleting Message Handler Server");
		delete g_messageHandlerServer;
		g_messageHandlerServer = NULL;
	}
}

bool MyMessageHandlerServerRunning() {
	bool toReturn = (g_messageHandlerServer != NULL);
	if (toReturn) {
		Lock lock( g_messageHandlerServer->threadProtection );
		LOG("MyMessageHandlerServerRunning");
		LOG_WRITE(
				"There are "
						+ ToString(g_messageHandlerServer->callbacks.size())
						+ " callbacks");
		LOG_DEBUG(
				"There are "
						+ ToString(g_messageHandlerServer->messageQueue.size())
						+ " unsent messages");
		LOG_DEBUG(
				"There are "
						+ ToString(
								g_messageHandlerServer->messageThreads.size())
						+ " threads");
		toReturn = (g_messageHandlerServer->callbacks.size() != 0);
	} else {
	}
	return toReturn;
}

std::map<std::string, unsigned int> intents;

unsigned int RegisterMessage(std::string intent) {
	std::map<std::string, unsigned int>::iterator exists = intents.find(intent);
	if (exists == intents.end()) {
		int id = intents.size() + 1;
		intents[intent] = id;
	}
	return intents[intent];
}

unsigned int GetMessage(std::string intent) {
	std::map<std::string, unsigned int>::iterator exists = intents.find(intent);
	if (exists == intents.end()) {
		return 0;
	}
	return intents[intent];
}

std::string GetMessage(unsigned int id) {
	for (std::map<std::string, unsigned int>::iterator iter = intents.begin();
			iter != intents.end(); iter++) {
		if (iter->second == id)
			return iter->first;
	}
	return "\'message id not found\'";
}
//
MessageHandlerServer::MessageHandlerServer() :
		maxTreads(4), usedThreads(0) {
}

MessageHandlerServer::~MessageHandlerServer() {
	LOG("MessageHandlerServer::~MessageHandlerServer");
	{
		LOG_DEBUG("Locking Map");
		Lock lock(this->mapProtection);
		MessageHandlerCBGroup *cbItem;
		LOG_DEBUG(
				"There are " + ToString(this->callbacks.size())
						+ " groups to delete");
		for (MessageHandlerCBListMap::iterator iter = this->callbacks.begin();
				iter != this->callbacks.end(); iter++) {
			{
				LOG_DEBUG("Locking Group");
				Lock lock(iter->second->groupProtection);
				cbItem = iter->second; //copy to local item to keep after destuct
				this->callbacks.erase(iter);
				LOG_DEBUG("Unlocking Group");
			}
			delete cbItem;
			cbItem = NULL;
		}
		LOG_DEBUG("Unlocking Map");
	}

	ClearMessages();
	ClearThreads();
}

void MessageHandlerServer::ClearThreads() {
	LOG("MessageHandlerServer::ClearThreads");
	LOG_DEBUG("Locking Thread");
	Lock lock(this->threadProtection);
	MessageHandlerThread *myself = NULL;
	LOG_DEBUG(
			"There are " + ToString(this->messageThreads.size())
					+ " threads to delete");
	for (std::vector<MessageHandlerThread*>::iterator iter =
			this->messageThreads.begin(); iter != this->messageThreads.end();
			iter++) {
		if (*iter != NULL) {
			MessageHandlerThread *t = *iter;
			t->running = false;
			if (t->myId == GetThreadID()) {
				LOG_DEBUG("found myslef " + ToString(t->myId));
				myself = t;
			} else {
				LOG_DEBUG("Waiting on thread " + ToString(t->myId));
				t->Wait();
				delete t;
			}
		}
	}
	this->messageThreads.clear();
	if (myself != NULL) {
		LOG_DEBUG("Waiting on myself " + ToString(myself->myId));
		//  myself->Terminate();
		delete myself;
	}
	LOG_DEBUG("Unlocking thread");
}

void MessageHandlerServer::CancelThreads() {
	LOG("MessageHandlerServer::CancelThreads");
	LOG_DEBUG("Locking Thread");
	Lock lock(this->threadProtection);
	LOG_DEBUG(
			"There are " + ToString(this->messageThreads.size())
					+ " threads to delete");
	for (std::vector<MessageHandlerThread*>::iterator iter =
			this->messageThreads.begin(); iter != this->messageThreads.end();
			iter++) {
		if (*iter != NULL) {
			MessageHandlerThread *t = *iter;
			t->running = false;
		}
	}
	LOG_DEBUG("Unlocking thread");
}

void MessageHandlerServer::ClearMessages() {
	Lock lock(this->messageProtection);
	LOG("MessageHandlerServer::ClearMessages");
	LOG_DEBUG("Locking message");
	LOG_DEBUG(
			"There are " + ToString(this->messageQueue.size())
					+ " unsent messages to delete");
	while (!this->messageQueue.empty()) {
		MessageHandlerArg *arg;
		arg = this->messageQueue.top();
		this->messageQueue.pop();
		delete arg;
	}
	LOG_DEBUG("UNlocking message");
}

void MessageHandlerServer::RegisterCB(MessageHandlerCBHandle callbackHandle,
		int message_type) {
	LOG("MessageHandlerServer::RegisterCB");
	LOG_DEBUG(
			"Registering " + GetMessage(message_type) + " to callback " + ToString(callbackHandle.handle));
	Lock lock(this->mapProtection);
	if (this->callbacks.find(message_type) == this->callbacks.end()) {
		this->callbacks[message_type] = new MessageHandlerCBGroup;
	}
	{
		Lock lockG(this->callbacks[message_type]->groupProtection);
		this->callbacks[message_type]->list.push_back(callbackHandle);
	}
}

void MessageHandlerServer::UnregisterCB(MessageHandlerCBHandle callbackHandle,
		int message_type) {
	LOG("MessageHandlerServer::UnregisterCB");
	LOG_DEBUG("Locking Map");
	Lock lock(this->mapProtection);
	MessageHandlerCBGroup *cbItem = NULL;
	MessageHandlerCBListMap::iterator exists = this->callbacks.find(
			message_type);
	if (exists != this->callbacks.end()) {
		cbItem = exists->second; //copy to local item to keep after destuct

		LOG_DEBUG("Group protection locked for " + GetMessage(message_type));
		Lock lock(cbItem->groupProtection);
		//this->callbacks.erase(exists); //found the list now need to find the correct entry in the list
		bool found = false;
		for (MessageHandlerCBList::iterator iter = cbItem->list.begin();
				iter != cbItem->list.end(); iter++) {
			if ((*iter) == callbackHandle) {
				cbItem->list.erase(iter);
				found = true;
				break;
			}
		}
		if (!found) {
			LOG("MessageHandlerServer::UnregisterCB");
			LOG_WARN(
					"Found message ID " + ToString(message_type)
							+ " but could not find the handle");
			return;
		}
		LOG_DEBUG("Group protection UNLOCKED");
	} else {
		LOG("MessageHandlerServer::UnregisterCB");
		LOG_WARN("Coudld not find message ID " + ToString(message_type));
		return;
	}
	if (cbItem != NULL && cbItem->list.empty()) {
		delete cbItem;
		cbItem = NULL;
		this->callbacks.erase(exists);
	}
	LOG_DEBUG("Unlocking map");
}

bool MessageHandlerServer::Send() {
	LOG("MessageHandlerServer::Send");
	if (this->usedThreads < this->maxTreads) {
		MessageHandlerArg *message = NULL;
		{
			Lock lockM(this->messageProtection);
			if (this->messageQueue.empty()) {
				return false;
			}

			LOG_DEBUG("Thread to use.");
			message = this->messageQueue.top();
			if (message == NULL) {
				LOG_DEBUG("Queue is empty");
				return false;
			} else {
				LOG_DEBUG(
						"Message to send is of type \"" + Kernal::GetMessage(message->message_type) + "\"");
				this->messageQueue.pop();
			}
		}
		MessageHandlerThread *mailThread = NULL;
		{
			LOG_DEBUG("Locking threadProtection");
			Lock lock(this->threadProtection);
			for (std::vector<MessageHandlerThread*>::iterator iter =
					this->messageThreads.begin();
					iter != this->messageThreads.end(); iter++) {
				LOG_DEBUG(
						"Locking thead action for thread:"
								+ ToString((*iter)->myId));
				(*iter)->action.Lock();
				if (mailThread == NULL && (*iter)->running == true
						&& (*iter)->processing == false) {
					LOG_DEBUG(
							"Found thread to reuse :"
									+ ToString((*iter)->myId));
					mailThread = *iter;
					LOG_DEBUG(
							"Unlocking thead action for thread:"
									+ ToString((*iter)->myId));
					(*iter)->action.Unlock();
					break;
				}
				(*iter)->action.Unlock();
				LOG_DEBUG(
						"Unlocking thread action for thread:"
								+ ToString((*iter)->myId));
			}
			if (mailThread == NULL) {
				LOG_DEBUG(
						"No available threads but we are allowed to create more.");
				mailThread = new MessageHandlerThread();
				mailThread->action.Lock();
				this->messageThreads.push_back(mailThread);
				mailThread->Launch();
			} else {
				mailThread->action.Lock();
			}
			LOG_DEBUG("Locking threadProtection");
		}
		{
			LOG_DEBUG(
					"Locking thread action for thread:"
							+ ToString(mailThread->myId));
//            Lock lockAction(mailThread->action);
			ThreadData *mail = new ThreadData();
			mail->data = message;
			mailThread->SetData(mail);
			this->usedThreads++;
			mailThread->processing = true;
			mailThread->action.Unlock();
			LOG_DEBUG("Starting thread:" + ToString(mailThread->myId));
			LOG_DEBUG(
					"Unlocking thread action for thread:"
							+ ToString(mailThread->myId));
		}
		return true;
	}
	LOG_DEBUG("No thread to use.");
	return false;
}

bool MessageHandlerServer::NoCallbacks() {
	bool no_exist = true;
	Lock lock(this->mapProtection);
	if (!this->callbacks.empty()) {
		for (MessageHandlerCBListMap::iterator iter = this->callbacks.begin();
				iter != this->callbacks.end(); iter++) {
			if (!iter->second->list.empty()) {
				no_exist = false;
				break;
			}
		}
	}
	return no_exist;
}

void MessageHandlerClient::Init(std::vector<std::string> &messages) {
	LOG("MessageHandlerClient::Init");
	MessageHandlerCBHandle handle;
	handle.cb = MessageHandlerMailBox;
	handle.handle = this;

	if (this->registeredMessages.empty()) {
		LOG_DEBUG("Registering shudown at " + ToString(this));
		int id = RegisterMessage(MessageHandlerServer::ShutDownMessage);
		this->registeredMessages.push_back(id);
		GetMyMessageHandlerServerPtr()->RegisterCB(handle, id);
	}

	LOG_DEBUG("Registering " + ToString(messages.size()) + " messages");
	for (std::vector<std::string>::iterator iter = messages.begin();
			iter != messages.end(); iter++) {
		int id = Kernal::RegisterMessage(*iter);
		this->registeredMessages.push_back(id);
		GetMyMessageHandlerServerPtr()->RegisterCB(handle, id);
	}
}

MessageHandlerClient::~MessageHandlerClient() {
	this->Finish();
}

void MessageHandlerClient::Finish() {
	LOG("MessageHandlerClient::Finish");
	LOG_WRITE("Hello");
	if (MyMessageHandlerServerRunning()) {
		{
			LOG_WRITE(
					"Finished with this client (" + ToString(this)
							+ ") cleaning up "
							+ ToString(this->registeredMessages.size())
							+ " registered messages.");
			MessageHandlerCBHandle handle;
			handle.cb = MessageHandlerMailBox;
			handle.handle = this;
			for (std::vector<int>::iterator iter =
					this->registeredMessages.begin();
					iter != this->registeredMessages.end(); iter++) {
				GetMyMessageHandlerServerPtr()->UnregisterCB(handle, *iter);
			}
		}
		this->registeredMessages.clear();
		/*  if(GetMyMessageHandlerServerPtr()->NoCallbacks() )
		 {
		 LOG_WRITE("No Callbacks left closing server.");
		 //CloseMyMessageHandlerServer();
		 }
		 else
		 {
		 LOG_WRITE("Other callbacks left leaving server open");
		 }*/
	} else {
		LOG_WRITE(
				"Message Handler Server is already shut down so nothing to do.");
	}
}

void MessageHandlerClient::MessageHandlerMailBox(void* handle,
		MessageHandlerArg *arg) {
	static unsigned int shutdownMessageId = Kernal::RegisterMessage(
			Kernal::MessageHandlerServer::ShutDownMessage);
	LOG("MessageHandlerClient::MessageHandlerMailBox");
	MessageHandlerClient *self =
			reinterpret_cast<MessageHandlerClient *>(handle);
	if (arg->message_type == shutdownMessageId) {
		LOG_DEBUG("Intercepted shutdown message for (" + ToString(self) + ")");
		self->Finish();
	} else {
		LOG_DEBUG("Calling callback.");
		self->cb(arg);
	}
}

void MessageHandlerClient::Send(MessageHandlerArg *arg) {
	LOG("MessageHandlerClient::Send");
	LOG_DEBUG(
			"Queueing message of type \"" + Kernal::GetMessage(arg->message_type) + "\" to send");
	{
		Lock lock(GetMyMessageHandlerServerPtr()->messageProtection);

		LOG_DEBUG(
				"Got Lock for message Protection");

		GetMyMessageHandlerServerPtr()->messageQueue.push(arg);
		LOG_DEBUG(
				"Giving up Lock for message Protection");
	}
	GetMyMessageHandlerServerPtr()->Send();

}

void MessageHandlerClient::RegisterComm(std::string recv_msg,
		std::string send_msg) {
	LOG("MessageHandlerClient::RegisterComm");

	MessageHandlerCBHandle handle;
	handle.cb = MessageHandlerMailBox;
	handle.handle = this;

	LOG_DEBUG("Registering " + recv_msg + " at " + ToString(this));
	this->comm_matrix[recv_msg] = send_msg;
	int id = RegisterMessage(recv_msg);
	this->registeredMessages.push_back(id);
	GetMyMessageHandlerServerPtr()->RegisterCB(handle, id);
}

}
