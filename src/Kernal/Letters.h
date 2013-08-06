#ifndef __LETTERS_H__
#define __LETTERS_H__

#include <string>
#include "Mutex.h"

namespace Kernal {
namespace Letters {
class Letter {
public:
	static std::string ID;
	virtual ~Letter() = 0;
	Mutex letterMutex;
};

class FileParserLetter: public Letter {
public:
	static std::string ID;
	FileParserLetter() :
			blockSize(0) {
	}
	~FileParserLetter() {
	}
	std::string filename;
	int blockSize;
};
class ParsedBlockLetter: public Letter {
public:
	static std::string ID;
	ParsedBlockLetter() :
			offset(0), size(0), block(NULL) {
	}
	~ParsedBlockLetter() {
		if (block != NULL)
			delete block;
	}
	std::string origFile;
	int offset;
	int size;
	char *block;
};
class DataParserLetter: public Letter {
public:
	static std::string ID;
	DataParserLetter() :
			size(0), dataBlock(NULL) {
	}
	;
	~DataParserLetter() {
		if (dataBlock != NULL)
			delete dataBlock;
	}
	;
	std::string filename;
	int size;
	char *dataBlock;
};

} // namespace Letters
} // namespace Kernal

#endif
