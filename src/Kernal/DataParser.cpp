#include "Kernal\DataParser.h"
#include "Kernal\Log.h"
#include "Kernal\Lock.h"
#include "Kernal\Compare.h"

namespace Kernal {
int DataParser::MAX_READ = 2048;

DataParser::DataParser() {
	LOG("DataParser::DataParser");
	LOG_WRITE("Registering messages");
	std::vector<std::string> messages;
	messages.push_back(Letters::DataParserLetter::ID);

	this->Init(messages);
}

void DataParser::cb(Kernal::MessageHandlerArg* arg) {
	LOG("DataParser::cb");

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
		Letters::DataParserLetter *dpLetter =
				reinterpret_cast<Letters::DataParserLetter*>(arg->letter);
		filename = dpLetter->filename;
		LOG_WRITE("Received the data from " + filename + " to parse");

		char *buffer = NULL;

		int length = dpLetter->size;
		int read = 0;

		while (read < length) {
			int toRead = LEAST(DataParser::MAX_READ, length - read);
			buffer = new char[toRead];

			memcpy(buffer, dpLetter->dataBlock + read, toRead);

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

	}

}
} // namespace Kernal

