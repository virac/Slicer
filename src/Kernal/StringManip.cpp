#include "StringManip.h"
#include "Compare.h"
#include <sstream>

namespace Kernal {

double ToDouble(std::string const& str) {
	std::istringstream ss(str);

	double val;
	ss >> val;

	/*    if(!(ss && (ss >> std::ws).eof()))
	 throw conversion_error();*/

	return val;
}

int ToInt(std::string const& str) {
	std::istringstream ss(str);

	int val;
	ss >> val;

	/*    if(!(ss && (ss >> std::ws).eof()))
	 throw conversion_error();*/

	return val;
}

std::string Strip(std::string string_In, std::string delims_In) {
	std::string toReturn;
	bool good;
	for (unsigned int i = 0; i < string_In.size(); i++) {
		good = true;
		for (unsigned int j = 0; j < delims_In.size(); j++) {
			if (string_In.at(i) == delims_In.at(j)) {
				good = false;
				break;
			}
		}
		if (good == true) {
			toReturn += string_In.at(i);
		}
	}
	return toReturn;
}
bool StartsWith(std::string string_1_In, std::string string_2_In) {
	if (string_1_In.size() >= string_2_In.size()) {
		if (string_1_In.substr(0, string_2_In.size()) == string_2_In) {
			return true;
		}
	}
	return false;

}
bool EndsWith(std::string string_1_In, std::string string_2_In) {
	if (string_1_In.size() >= string_2_In.size()) {
		if (string_1_In.substr(string_1_In.size() - string_2_In.size(),
				string_1_In.size()) == string_2_In) {
			return true;
		}
	}
	return false;
}

void Tokenize(std::string str, std::vector<std::string> &tokens,
		std::string delimiters) {
	std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);
	std::string::size_type pos = str.find_first_of(delimiters, lastPos);

	while (std::string::npos != pos || std::string::npos != lastPos) {
		tokens.push_back(str.substr(lastPos, pos - lastPos));
		lastPos = str.find_first_not_of(delimiters, pos);
		pos = str.find_first_of(delimiters, lastPos);
	}
}

void Separate(std::string str, std::vector<std::string> &tokens,
		std::string start_separators, std::string end_separators) {
	std::string::size_type lastPos = 0;
	std::string::size_type pos1 = str.find_first_of(start_separators,
			lastPos + 1);
	std::string::size_type pos2 = str.find_first_of(end_separators,
			lastPos + 1);
	if (pos2 != std::string::npos)
		pos2++;
	std::string::size_type pos;
	pos = LEAST(pos1, pos2);
	while ((std::string::npos != pos || std::string::npos != lastPos)
			&& lastPos < str.size()) {
		tokens.push_back(str.substr(lastPos, pos - lastPos));
		lastPos = pos; //str.find_first_not_of(end_separators,pos);
		if (lastPos != std::string::npos) {
			pos1 = str.find_first_of(start_separators, lastPos + 1);
			pos2 = str.find_first_of(end_separators, lastPos);
			if (pos2 != std::string::npos)
				pos2++;
			pos = LEAST(pos1, pos2);
		} else
			pos = std::string::npos;
	}
}
}

