#ifndef __FILE_INPUT_H__
#define __FILE_INPUT_H__

#include <fstream>
#include <vector>
#include <string>
#include <map>

namespace Kernal {

std::string Strip(std::string string, std::string delims);
bool StartsWith(std::string string_1_In, std::string string_2_In);
bool EndsWith(std::string string_1_In, std::string string_2_In);
//void Tokenize(std::string str, std::vector<std::string> &tokens, std::string delimiters = " ", std::string start_separators = " ", std::string end_separators = " " );
void Tokenize(std::string str, std::vector<std::string> &tokens,
		std::string delimiters = " ");

class FileInput {
public:
	FileInput(std::string s_file_name_In);
	~FileInput();

	bool IsValid();

	void PutBack(std::vector<std::string> &tokens);

	void SetDeliminators(std::string delims_In);
	void SetCommentBlock(std::string comment_start_In,
			std::string comment_end_In);
	void SetSeparators(std::string start_separator_In,
			std::string end_separator_In);
	std::string PeekToken();
	std::string GetToken();
private:
	std::map<std::string, std::string> m_mss_comment_block;
	std::string m_s_deliminators;
	std::vector<std::string> m_vs_next_tokens;
	std::vector<std::string> m_vs_separated_tokens;
	std::string m_s_start_separators;
	std::string m_s_end_separators;
	std::fstream *m_pf_file_input;
};
} //namespace Kernal

#endif
