#ifndef __DATA_PARSER_H__
#define __DATA_PARSER_H__

#include "Kernal\MessageHandler.h"

namespace Kernal {
class DataParser: public Kernal::MessageHandlerClient {
public:
	/** Default constructor */
	DataParser();
	static int MAX_READ;
protected:
private:
	void cb(Kernal::MessageHandlerArg* arg);
};
} //namespace Kernal
#endif // __FILE_PARSER_H__
