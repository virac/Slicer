#ifndef __STRING_MANIP_H__
#define __STRING_MANIP_H__

#include <string>
#include <vector>
#include <sstream>
namespace Kernal {

template<class T>
std::string ToString(T val) {
	std::ostringstream oss;
	std::string toReturn;
	oss << val;
	toReturn = oss.str();
	return toReturn;
}
;
//!< Will Convert val into a string

double ToDouble(std::string const& str);
int ToInt(std::string const& str);

std::string Strip(std::string string_In, std::string delims_In);
//!< This function will strip string_In of all the characters in delims_In
bool StartsWith(std::string string_1_In, std::string string_2_In);
//!< This function will return true if the start of string_1_In is the same as string_2_In
bool EndsWith(std::string string_1_In, std::string string_2_In);
//!< This function will return true if the end of string_1_In is the same as string_2_In
void Tokenize(std::string str, std::vector<std::string> &tokens,
		std::string delimiters);
//!< This function will tokenize str into a vector by any character in delminaters
void Separate(std::string str, std::vector<std::string> &tokens,
		std::string start_separators, std::string end_separators);
//!< This function will separate str into tokens by starting a new string at the start_separators or end a string and start a new string at any end_separators
}

#endif
