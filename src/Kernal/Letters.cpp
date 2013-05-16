#include "Letters.h"

namespace Kernal {
namespace Letters {

Letter::~Letter() {
}
std::string Letter::ID = "null_letter";

std::string FileParserLetter::ID = "file_to_parse";

std::string ParsedBlockLetter::ID = "block_parsed";

std::string DataParserLetter::ID = "data_to_parse";
}
}
