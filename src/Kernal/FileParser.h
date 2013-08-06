#ifndef __FILE_PARSER_H__
#define __FILE_PARSER_H__

#include "Kernal\MessageHandler.h"

namespace Kernal {
class FileParser: public Kernal::MessageHandlerClient {
public:
	/** Default constructor */
	FileParser();
	~FileParser() {};
	static int MAX_READ;

protected:
private:
	void cb(Kernal::MessageHandlerArg* arg);
};
} //namespace Kernal
#endif // __FILE_PARSER_H__
