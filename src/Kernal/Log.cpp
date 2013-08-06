#undef USE_MEM_MANAGER
#include "Log.h"

#include <stdarg.h>
#include <sstream>
#include <iostream>

#include "Watch.h"
#include "Thread.h"
#include "Lock.h"

using std::stack;
using std::string;
using std::cout;
using std::endl;
namespace Kernal {

LogServer *g_logServer = NULL;
Watch g_watch;

LogServer* GetMyLogServerPtr() {
	if (g_logServer == NULL) {
		g_watch.Reset();
		g_logServer = new LogServer("N/A");
		StartMemoryWatch();
	}
	return g_logServer;
}
void CloseMyLogServer() {
	if (g_logServer != NULL) {
		{
			LOG("Close");
			LOG_WRITE("goodbye");
		}
		EndMemoryWatch();
		g_logServer->Close();

		delete g_logServer;
		g_logServer = NULL;
	}
}

LogServer::LogServer(std::string log_name) :
		m_indent(0), m_log_name(log_name), m_log_level(LOG_LEVEL_WRITE), m_file_output(
				NULL) {
}

LogServer::~LogServer() {
}

void LogServer::SetName(std::string name) {
	this->m_log_name = name;
}

int LogServer::SetFile(std::string file_name) {
	if (this->m_file_output != NULL) {
		this->m_file_output->close();
		delete this->m_file_output;
		this->m_file_output = NULL;
	}
	this->m_file_output = new std::fstream;
	this->m_file_output->open(file_name.c_str(),
			std::fstream::out | std::fstream::trunc);
	if (this->m_file_output->good()) {
		return 0;
	} else {
		delete this->m_file_output;
		this->m_file_output = NULL;
		return 1;
	}
}

void LogServer::SetLevel(LOG_LEVEL log_level_In) {
	if (log_level_In < LOG_LEVEL_TRACE || log_level_In > LOG_LEVEL_FATAL) {
		this->Out(std::string("FATAL"), std::string("LogServer::SetLevel"),
				std::string("CAN NOT SET LOG_LEVEL TO THIS."));
	} else {
		this->m_log_level = log_level_In;
	}
}

LOG_LEVEL LogServer::GetLogLevel() {
	return this->m_log_level;
}

void LogServer::Close() {
	if (this->m_file_output != NULL) {
		this->m_file_output->close();
		delete this->m_file_output;
		this->m_file_output = NULL;
	}
}

void LogServer::PushContext(std::string name) {
#ifdef DEBUG
	//  this->m_indent++;
	int id = GetThreadID();
	this->m_stack[id].push( name );
#endif
}

void LogServer::PopContext() {
#ifdef DEBUG
	//  this->m_indent--;
	int id = GetThreadID();
	this->m_stack[id].pop();
#endif
}

void LogServer::Out(std::string tag, std::string label, std::string msg) {
	std::ostringstream oss;
	oss << this->m_log_name << ":" << GetThreadID() << " " << tag << " ";
	// oss << this->AddIndent();
	// oss << this->m_stack.top()
	oss << g_watch.GetSplit() << " " << label << "  " << msg;
	Lock lock(m_output_lock);
	if (this->m_file_output != NULL) {
		(*m_file_output) << oss.str() << endl;
	} else {
		cout << oss.str() << endl;
	}
}
/*
 void LogServer::Trace(std::string msg)
 {
 this->Out( " TRACE    ", msg );
 }

 void LogServer::Debug(std::string msg)
 {
 this->Out(" DBUG     ", msg );
 }

 void LogServer::Write(std::string msg)
 {
 this->Out( " INFO     ", msg );
 }

 void LogServer::Warn(std::string msg)
 {
 this->Out( " WARN *** ", msg );
 }

 void LogServer::Error(std::string msg)
 {
 this->Out( " ERROR### ", msg );
 }

 void LogServer::Fatal(std::string msg)
 {
 this->Out( " FATAL!!! ", msg );
 //::exit(1);
 }*/

std::string LogServer::AddIndent() {
	std::ostringstream toReturn;
	if (this->m_indent < 25) {
		for (int i = 0; i < this->m_indent; i++) {
			toReturn << "  ";
		}
	} else {
		toReturn << " " << this->m_indent << " ";
	}
	return toReturn.str();
}

LogClient::LogClient(LogServer *log_In, std::string name) :
		m_LogServer(log_In) {
	this->m_name = name;
	this->m_LogServer->PushContext(this->m_name);
	LOG_TRACE(">>>entering>>>", this->m_name);
}

LogClient::~LogClient() {
	LOG_TRACE("<<<leaving <<<", this->m_name);
	this->m_LogServer->PopContext();
}

void LogClient::debug(std::string msg) {
	if (msg == "")
		return;

	this->m_LogServer->Out("DEBUG", this->m_name, msg);
}

void LogClient::write(std::string msg) {
	if (msg == "")
		return;

	this->m_LogServer->Out("WRITE", this->m_name, msg);
}

void LogClient::warn(std::string msg) {
	if (msg == "")
		return;

	this->m_LogServer->Out("WARN ", this->m_name, msg);
}

void LogClient::error(std::string msg) {
	if (msg == "")
		return;

	this->m_LogServer->Out("ERROR", this->m_name, msg);
}

void LogClient::fatal(std::string msg) {
	if (msg == "")
		return;

	this->m_LogServer->Out("FATAL", this->m_name, msg);
}
void LogClient::stack() {
	//int id = GetThreadID();
	for( std::map< int, stack_string >::iterator i = this->m_LogServer->m_stack.begin(); i != this->m_LogServer->m_stack.end(); i++ ) {
		stack_string stack = i->second;
		std::string stack_line = "STACK["+ToString(i->first)+"]";
		for( int i = 0; i < 3 && !stack.empty(); i++ ) {

			this->m_LogServer->Out(stack_line, this->m_name, stack.top() );
			stack.pop();
		}
	}
}

}

