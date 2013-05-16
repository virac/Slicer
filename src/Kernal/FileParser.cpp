#include "Kernal\FileParser.h"
#include "Kernal\Log.h"
#include "Kernal\Lock.h"
#include "Kernal\Compare.h"

namespace Kernal {
int FileParser::MAX_READ = 2048;

FileParser::FileParser() {
	LOG("FileParser::FileParser");
	LOG_WRITE("Registering messages");
	std::vector<std::string> messages;
	messages.push_back(Letters::FileParserLetter::ID);

	this->Init(messages);
}

void FileParser::cb(Kernal::MessageHandlerArg* arg) {
	LOG("FileParser::cb");

	if (arg == NULL) {
		LOG_ERROR("NULL message received.");
		return;
	}
	unsigned int send_id = 0;
	if (arg->message_type
			== Kernal::GetMessage(Letters::FileParserLetter::ID)) {
		send_id = Kernal::GetMessage(Letters::ParsedBlockLetter::ID);
	} else {
		std::map<std::string, std::string>::iterator iter =
				this->comm_matrix.find(Kernal::GetMessage(arg->message_type));
		if (iter != this->comm_matrix.end()) {
			send_id = Kernal::GetMessage(iter->second);
		}
	}

	if (send_id != 0) {
		Lock lock(arg->letter->letterMutex);
		std::string filename = "";
		Letters::FileParserLetter *fpLetter =
				reinterpret_cast<Letters::FileParserLetter*>(arg->letter);
		filename = fpLetter->filename;
		LOG_WRITE("Received file to parse: " + filename);

		char *buffer = NULL;

		std::ifstream is;
		is.open(filename.c_str(), std::ios::binary);
		is.seekg(0, std::ios::end);
		int length = is.tellg();
		is.seekg(0, std::ios::beg);
		int read = 0;
		while (!is.eof()) {
			int toRead = LEAST(FileParser::MAX_READ, length - read);
			buffer = new char[toRead];

			is.read(buffer, toRead);
			read += toRead;

			Letters::ParsedBlockLetter *letter =
					new Letters::ParsedBlockLetter();
			letter->origFile = filename;
			letter->offset = read;
			letter->size = toRead;
			letter->block = buffer;

			Kernal::MessageHandlerArg *send_arg =
					new Kernal::MessageHandlerArg();
			send_arg->message_type = send_id;
			send_arg->priority = arg->priority + 1;
			send_arg->blocking = arg->blocking + 1;

			send_arg->letter = letter;

			this->Send(send_arg);
		}

		is.close();

	}

}
} // namespace Kernal

