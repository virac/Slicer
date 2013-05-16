#ifndef __DIRECTORY_H__
#define __DIRECTORY_H__

#include <vector>
#include <string>

#include "XMLTree.h"
namespace Kernal {

class Directory {
public:
	//  Directory();//std::string directory_In);
	//  ~Directory();
	int DeepScan(std::string root_dir_In, std::vector<std::string> &files);
	int DeepScan(std::string root_dir_In, XMLTree *files, std::string file_In =
			"");
	int Scan(std::string root_dir_In, std::vector<std::string> &files,
			bool warn = true);
private:

	bool is_dotfile(const std::string &filename);
//    DIR *pDIR;
//    struct dirent *entry;
};
}

#endif
