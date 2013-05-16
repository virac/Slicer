#ifndef __TEST_MESSAGE_SENDER_H__
#define __TEST_MESSAGE_SENDER_H__

#include "Kernal\MessageHandler.h"

class TestMessageSender1: public Kernal::MessageHandlerClient {
public:
	TestMessageSender1();
protected:
private:
	void cb(Kernal::MessageHandlerArg* arg);
};

class TestMessageSender2: public Kernal::MessageHandlerClient {
public:
	TestMessageSender2();
protected:
private:
	void cb(Kernal::MessageHandlerArg* arg);
};

class TestMessageSender3: public Kernal::MessageHandlerClient {
public:
	TestMessageSender3();
protected:
private:
	void cb(Kernal::MessageHandlerArg* arg);
};

class TestMessageSender4: public Kernal::MessageHandlerClient {
public:
	TestMessageSender4();
protected:
private:
	void cb(Kernal::MessageHandlerArg* arg);
};

class TestMessageSender: public Kernal::MessageHandlerClient {
public:
	TestMessageSender();
	~TestMessageSender();
	void Run();
	bool done;
protected:
private:
	void cb(Kernal::MessageHandlerArg* arg);
};

#endif // __TEST_MESSAGE_SENDER_H__
