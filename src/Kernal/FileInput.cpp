#include <sstream>

#include "FileInput.h"
#include "Log.h"
#include "Compare.h"
#include "StringManip.h"

namespace Kernal {

FileInput::FileInput(std::string s_file_name_In) {
	LOG("FileInput::FileInput");
	this->m_pf_file_input = new std::fstream;
	this->m_pf_file_input->open(s_file_name_In.c_str(), std::fstream::in);
	if (this->m_pf_file_input->bad())
		LOG_FATAL("I hate this");
}
FileInput::~FileInput() {
	if (this->m_pf_file_input != NULL) {
		this->m_pf_file_input->close();
		delete this->m_pf_file_input;
	}
}

bool FileInput::IsValid() {
	return this->m_pf_file_input->good() || !this->m_vs_next_tokens.empty()
			|| !this->m_vs_separated_tokens.empty();
}

void FileInput::PutBack(std::vector<std::string> &tokens) {
	LOG("FileInput::PutBack");
	this->m_vs_next_tokens.insert(this->m_vs_next_tokens.begin(),
			tokens.begin(), tokens.end());
	tokens.clear();
}

void FileInput::SetDeliminators(std::string delims_In) {
	this->m_s_deliminators = delims_In;
}

void FileInput::SetCommentBlock(std::string comment_start_In,
		std::string comment_end_In) {
	this->m_mss_comment_block[comment_start_In] = comment_end_In;
}

void FileInput::SetSeparators(std::string start_separator_In,
		std::string end_separator_In) {
	this->m_s_start_separators = start_separator_In;
	this->m_s_end_separators = end_separator_In;
}

std::string FileInput::PeekToken() {
	LOG("FileInput::PeekToken");
	std::string toReturn;
	bool good_token = true;
	unsigned int comment_tokens = 0;
	std::stack<std::string> comment_stack;

	do {
		good_token = true;
		while (this->m_vs_separated_tokens.size() == 0
				&& !this->m_pf_file_input->eof()) {
			std::string temp_string;
			(*this->m_pf_file_input) >> temp_string;
			Tokenize(temp_string, this->m_vs_separated_tokens,
					this->m_s_deliminators);
		}
		while (this->m_vs_next_tokens.size() == 0
				&& this->m_vs_separated_tokens.size() != 0) {
			std::string temp_string = this->m_vs_separated_tokens.front();
			this->m_vs_separated_tokens.erase(
					this->m_vs_separated_tokens.begin());
			Separate(temp_string, this->m_vs_next_tokens,
					this->m_s_start_separators, this->m_s_end_separators);
		}
		if (this->m_vs_next_tokens.size() == 0) {
			LOG_WARN("No tokens left to process");
			toReturn = "";
		} else {
			toReturn = this->m_vs_next_tokens.front();
		}

		for (std::map<std::string, std::string>::iterator i =
				this->m_mss_comment_block.begin();
				i != this->m_mss_comment_block.end(); i++) {
			if (StartsWith(toReturn, i->first)) {
				comment_tokens++;
				comment_stack.push(i->second);
				break;
			}
		}
		if (comment_stack.size() != 0) {
			if (EndsWith(toReturn, comment_stack.top())) {
				toReturn = "";
				comment_stack.pop();
				comment_tokens--;
				good_token = false;
			}
		}
		if (comment_tokens != 0 || good_token == false)
			this->m_vs_next_tokens.erase(this->m_vs_next_tokens.begin());
	} while (comment_tokens != 0 || !good_token);
//   log.debug("Peeked at "+toReturn);
	return toReturn;
}

std::string FileInput::GetToken() {
	LOG("FileInput::GetToken");
	std::string toReturn;
	toReturn = this->PeekToken();
	if (this->m_vs_next_tokens.size() > 0)
		this->m_vs_next_tokens.erase(this->m_vs_next_tokens.begin());
	// log.debug("Grabbed "+toReturn);
	return toReturn;
}
} //namespace Kernal

