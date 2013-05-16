#ifndef __LOG_H__
#define __LOG_H__

#include <stack>
#include <map>
#include <string>
#include <fstream>

#include "StringManip.h"
#include "Mutex.h"

#define MAX_BUFFER_SIZE 256
#define HAS_LOG
namespace Kernal {
class LogServer;
class LogClient;
enum LOG_LEVEL {
	LOG_LEVEL_UNKNOWN,
	LOG_LEVEL_TRACE,
	LOG_LEVEL_DEBUG,
	LOG_LEVEL_WRITE,
	LOG_LEVEL_WARN,
	LOG_LEVEL_ERROR,
	LOG_LEVEL_FATAL,
	LOG_LEVEL_END
};
LogServer* GetMyLogServerPtr();
void CloseMyLogServer();
#define LOG_SET_NAME(x)  Kernal::GetMyLogServerPtr()->SetName( x )
#define LOG_SET_FILE(x)  Kernal::GetMyLogServerPtr()->SetFile( x )
#define LOG_SET_LEVEL(x) Kernal::GetMyLogServerPtr()->SetLevel( x )

#define LOG(x) Kernal::LogClient log_client( Kernal::GetMyLogServerPtr(), x )

#define LOG_TRACE(x,y) if( Kernal::GetMyLogServerPtr()->GetLogLevel() <= Kernal::LOG_LEVEL_TRACE ) this->m_LogServer->Out("TRACE",x,y)
#define LOG_DEBUG(x) if( Kernal::GetMyLogServerPtr()->GetLogLevel() <= Kernal::LOG_LEVEL_DEBUG ) log_client.debug(x)
#define LOG_WRITE(x) if( Kernal::GetMyLogServerPtr()->GetLogLevel() <= Kernal::LOG_LEVEL_WRITE ) log_client.write(x)
#define LOG_WARN(x)  if( Kernal::GetMyLogServerPtr()->GetLogLevel() <= Kernal::LOG_LEVEL_WARN  ) log_client.warn(x)
#define LOG_ERROR(x) if( Kernal::GetMyLogServerPtr()->GetLogLevel() <= Kernal::LOG_LEVEL_ERROR ) log_client.error(x)
#define LOG_FATAL(x) if( Kernal::GetMyLogServerPtr()->GetLogLevel() <= Kernal::LOG_LEVEL_FATAL ) log_client.fatal(x)
#define LOG_STACK if( Kernal::GetMyLogServerPtr()->GetLogLevel() <= Kernal::LOG_LEVEL_WRITE ) log_client.stack()
#define CLOSE_LOG Kernal::CloseMyLogServer()

typedef std::stack<std::string> stack_string;
class LogServer {
public:
	LogServer(std::string name);
	~LogServer();

	void SetName(std::string name);
	int SetFile(std::string file_name);
	void SetLevel(LOG_LEVEL log_level_In);
	LOG_LEVEL GetLogLevel();
	void Close();

	friend class LogClient;
private:
	void PushContext(std::string log_name);
	void PopContext();
	void Out(std::string tag, std::string label, std::string msg);
	/*void Trace( std::string msg );
	 void Debug( std::string msg );
	 void Write( std::string msg );
	 void Warn ( std::string msg );
	 void Error( std::string msg );
	 void Fatal( std::string msg );*/
	std::string AddIndent();

	int m_indent;
	std::map< int, stack_string > m_stack;
	std::string m_log_name;
	LOG_LEVEL m_log_level;
	std::fstream *m_file_output;
	Mutex m_output_lock;
};

class LogClient {
public:
	LogClient(LogServer *log_In, std::string name);
	~LogClient();
	void debug(std::string msg);
	void write(std::string msg);
	void warn(std::string msg);
	void error(std::string msg);
	void fatal(std::string msg);

	void stack();
private:
	std::string m_name;
	LogServer *m_LogServer;
};
}

#include "MemoryManager.h"
#endif
