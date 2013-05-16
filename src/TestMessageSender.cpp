#include "TestMessageSender.h"
#include "Kernal\MessageHandler.h"
#include "Kernal\Log.h"
#include "Kernal\Watch.h"

TestMessageSender::TestMessageSender() {
	LOG("TestMessageSender::TestMessageSender");
	LOG_WRITE("Registering messages");
	std::vector<std::string> messages;
	messages.push_back("test_recv_1");
	messages.push_back("test_recv_2");
	messages.push_back("test_recv_3");
	messages.push_back("test_recv_4");
	messages.push_back("test_recv_5");
	messages.push_back("test_recv_6");
	messages.push_back("test_recv_7");
	messages.push_back("test_recv_8");
	messages.push_back("test_recv_9");

	this->Init(messages);

}

TestMessageSender::~TestMessageSender() {
}

void TestMessageSender::Run() {
	LOG("TestMessageSender::Run");
	LOG_WRITE("Running");
	done = false;
	Kernal::MessageHandlerArg *arg = new Kernal::MessageHandlerArg();
	arg->message_type = Kernal::GetMessage("test_message_1");
	arg->priority = 0;
	arg->blocking = 10;
	this->Send(arg);
}

void TestMessageSender::cb(Kernal::MessageHandlerArg* arg) {
	LOG("TestMessageSender::cb");
	//yay
	LOG_WRITE(
			")()()()( Got Message " + Kernal::GetMessage( arg->message_type ));
	Kernal::Watch::SleepSec(0.5f);

	if (arg->message_type == Kernal::GetMessage("test_recv_1")) {
		static int once = 0;
		if (once++ == 0) {
			//  once = true;
			LOG_WRITE("Got return from message 1; now sending message 2");
			Kernal::MessageHandlerArg *arg = new Kernal::MessageHandlerArg();
			arg->message_type = Kernal::GetMessage("test_message_2");
			arg->priority = 10;
			arg->blocking = 20;
			this->Send(arg);
		} else {
			LOG_WRITE(
					"Alrdy got \"" +Kernal::GetMessage(arg->message_type) + "\" " + Kernal::ToString(once) + " time(s)");
		}

	}
	if (arg->message_type == Kernal::GetMessage("test_recv_2")) {
		LOG_WARN("FINALLY HERE~~~~~~~~~~~~");
		static int once = 0;
		if (once++ == 0) {
			//  once = true;
			LOG_WRITE(
					"Got return from message 2; now sending message 3*************");
			Kernal::MessageHandlerArg *arg = new Kernal::MessageHandlerArg();
			arg->message_type = Kernal::GetMessage("test_message_3");
			arg->priority = 20;
			arg->blocking = 30;
			this->Send(arg);
		} else {
			LOG_WRITE(
					"Alrdy got \"" +Kernal::GetMessage(arg->message_type) + "\" " + Kernal::ToString(once) + " time(s)");
		}
	}
	if (arg->message_type == Kernal::GetMessage("test_recv_3")) {
		static int once = 0;
		if (once++ == 0) {
			//  once = true;
			LOG_WRITE(
					"FINISHING!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

			Kernal::MessageHandlerArg *arg = new Kernal::MessageHandlerArg();
			arg->message_type = Kernal::GetMessage(
					Kernal::MessageHandlerServer::ShutDownMessage);
			arg->priority = 0;
			arg->blocking = 0;
			this->Send(arg);

			done = true;
		} else {
			LOG_WRITE(
					"Alrdy got \"" +Kernal::GetMessage(arg->message_type) + "\" " + Kernal::ToString(once) + " time(s)");
		}
	}
}

TestMessageSender1::TestMessageSender1() {
	LOG("TestMessageSender1::TestMessageSender1");
	LOG_WRITE("Registering messages");
	std::vector<std::string> messages;
	messages.push_back("test_message_1");
	messages.push_back("test_message_2");

	this->Init(messages);
}

void TestMessageSender1::cb(Kernal::MessageHandlerArg* arg) {
	LOG("TestMessageSender1::cb");
	//yay
	LOG_WRITE("I got in, I got in!");
	Kernal::Watch::SleepSec(0.5f);
	LOG_WRITE("I got out, I got out!");

	if (arg->message_type == Kernal::GetMessage("test_message_1")) {
		Kernal::MessageHandlerArg *arg = new Kernal::MessageHandlerArg();
		arg->message_type = Kernal::GetMessage("test_recv_1");
		arg->priority = 10;
		arg->blocking = 20;
		LOG_WRITE("sending test_recv_1");
		this->Send(arg);
	} else if (arg->message_type == Kernal::GetMessage("test_message_2")) {
		Kernal::MessageHandlerArg *arg = new Kernal::MessageHandlerArg();
		arg->message_type = Kernal::GetMessage("test_recv_2");
		arg->priority = 20;
		arg->blocking = 30;
		LOG_WRITE("sending test_recv_2");
		this->Send(arg);
	}
}

TestMessageSender2::TestMessageSender2() {
	LOG("TestMessageSender2::TestMessageSender2");
	LOG_WRITE("Registering messages");
	std::vector<std::string> messages;
	messages.push_back("test_message_2");

	this->Init(messages);
}

void TestMessageSender2::cb(Kernal::MessageHandlerArg* arg) {
	LOG("TestMessageSender2::cb");
	//yay
	LOG_WRITE("I got in, I got in!");
	Kernal::Watch::SleepSec(0.5f);
	LOG_WRITE("I got out, I got out!");

	if (arg->message_type == Kernal::GetMessage("test_message_2")) {
		Kernal::MessageHandlerArg *arg = new Kernal::MessageHandlerArg();
		arg->message_type = Kernal::GetMessage("test_recv_2");
		arg->priority = 20;
		arg->blocking = 30;
		LOG_WRITE("sending test_recv_2");
		this->Send(arg);
	} else if (arg->message_type == Kernal::GetMessage("test_message_4")) {
		Kernal::MessageHandlerArg *arg = new Kernal::MessageHandlerArg();
		arg->message_type = Kernal::GetMessage("test_recv_4");
		arg->priority = 40;
		arg->blocking = 50;
		this->Send(arg);
	} else if (arg->message_type == Kernal::GetMessage("test_message_5")) {
		Kernal::MessageHandlerArg *arg = new Kernal::MessageHandlerArg();
		arg->message_type = Kernal::GetMessage("test_recv_5");
		arg->priority = 50;
		arg->blocking = 60;
		this->Send(arg);
	} else if (arg->message_type == Kernal::GetMessage("test_message_6")) {
		Kernal::MessageHandlerArg *arg = new Kernal::MessageHandlerArg();
		arg->message_type = Kernal::GetMessage("test_recv_6");
		arg->priority = 40;
		arg->blocking = 50;
		this->Send(arg);
	} else if (arg->message_type == Kernal::GetMessage("test_message_7")) {
		Kernal::MessageHandlerArg *arg = new Kernal::MessageHandlerArg();
		arg->message_type = Kernal::GetMessage("test_recv_7");
		arg->priority = 50;
		arg->blocking = 60;
		this->Send(arg);
	}
}

TestMessageSender3::TestMessageSender3() {
	LOG("TestMessageSender3::TestMessageSender3");
	LOG_WRITE("Registering messages");
	std::vector<std::string> messages;
	messages.push_back("test_message_1");
	messages.push_back("test_message_3");

	this->Init(messages);
}

void TestMessageSender3::cb(Kernal::MessageHandlerArg* arg) {
	LOG("TestMessageSender3::cb");
	//yay
	LOG_WRITE("I got in, I got in!");
	Kernal::Watch::SleepSec(0.5f);
	LOG_WRITE("I got out, I got out!");

	if (arg->message_type == Kernal::GetMessage("test_message_1")) {
		Kernal::MessageHandlerArg *arg = new Kernal::MessageHandlerArg();
		arg->message_type = Kernal::GetMessage("test_recv_1");
		arg->priority = 10;
		arg->blocking = 20;
		LOG_WRITE("sending test_recv_1");
		this->Send(arg);
	} else if (arg->message_type == Kernal::GetMessage("test_message_3")) {
		Kernal::MessageHandlerArg *arg = new Kernal::MessageHandlerArg();
		arg->message_type = Kernal::GetMessage("test_recv_3");
		arg->priority = 10;
		arg->blocking = 20;
		LOG_WRITE("sending test_recv_3");
		this->Send(arg);
	} else if (arg->message_type == Kernal::GetMessage("test_message_6")) {
		Kernal::MessageHandlerArg *arg = new Kernal::MessageHandlerArg();
		arg->message_type = Kernal::GetMessage("test_recv_6");
		arg->priority = 50;
		arg->blocking = 60;
		this->Send(arg);
	} else if (arg->message_type == Kernal::GetMessage("test_message_8")) {
		Kernal::MessageHandlerArg *arg = new Kernal::MessageHandlerArg();
		arg->message_type = Kernal::GetMessage("test_recv_8");
		arg->priority = 40;
		arg->blocking = 50;
		this->Send(arg);
	} else if (arg->message_type == Kernal::GetMessage("test_message_9")) {
		Kernal::MessageHandlerArg *arg = new Kernal::MessageHandlerArg();
		arg->message_type = Kernal::GetMessage("test_recv_9");
		arg->priority = 50;
		arg->blocking = 60;
		this->Send(arg);
	}
}

TestMessageSender4::TestMessageSender4() {
	LOG("TestMessageSender4::TestMessageSender4");
	LOG_WRITE("Registering messages");
	std::vector<std::string> messages;
	messages.push_back("test_message_2");

	this->Init(messages);
}

void TestMessageSender4::cb(Kernal::MessageHandlerArg* arg) {
	LOG("TestMessageSender4::cb");
	//yay
	LOG_WRITE("I got in, I got in!");
	Kernal::Watch::SleepSec(0.5f);
	LOG_WRITE("I got out, I got out!");

	if (arg->message_type == Kernal::GetMessage("test_message_2")) {
		Kernal::MessageHandlerArg *arg = new Kernal::MessageHandlerArg();
		arg->message_type = Kernal::GetMessage("test_recv_2");
		arg->priority = 10;
		arg->blocking = 10;
		LOG_WRITE("sending test_recv_2");
		this->Send(arg);
	} else if (arg->message_type == Kernal::GetMessage("test_message_4")) {
		Kernal::MessageHandlerArg *arg = new Kernal::MessageHandlerArg();
		arg->message_type = Kernal::GetMessage("test_recv_4");
		arg->priority = 40;
		arg->blocking = 50;
		this->Send(arg);
	} else if (arg->message_type == Kernal::GetMessage("test_message_6")) {
		Kernal::MessageHandlerArg *arg = new Kernal::MessageHandlerArg();
		arg->message_type = Kernal::GetMessage("test_recv_6");
		arg->priority = 40;
		arg->blocking = 50;
		this->Send(arg);
	} else if (arg->message_type == Kernal::GetMessage("test_message_7")) {
		Kernal::MessageHandlerArg *arg = new Kernal::MessageHandlerArg();
		arg->message_type = Kernal::GetMessage("test_recv_7");
		arg->priority = 50;
		arg->blocking = 60;
		this->Send(arg);
	} else if (arg->message_type == Kernal::GetMessage("test_message_9")) {
		Kernal::MessageHandlerArg *arg = new Kernal::MessageHandlerArg();
		arg->message_type = Kernal::GetMessage("test_recv_9");
		arg->priority = 50;
		arg->blocking = 60;
		this->Send(arg);
	}
}
