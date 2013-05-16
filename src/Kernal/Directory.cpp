#include <dirent.h>

//#undef USE_MEM_MANAGER
#include "Directory.h"
#include "MD5.h"
#include "Log.h"
namespace Kernal {

bool Directory::is_dotfile(const std::string &filename) {
	return filename.at(0) == '.';
}

int Directory::DeepScan(std::string root_dir_In,
		std::vector<std::string> &files) {
	std::vector<std::string> to_scan_files;

	this->Scan(root_dir_In, to_scan_files, false);
	if (to_scan_files.size() != 0) {
		for (std::vector<std::string>::iterator i = to_scan_files.begin();
				i != to_scan_files.end(); i++) {
			this->DeepScan(root_dir_In + "\\" + (*i), files);
		}
	} else {
		files.push_back(root_dir_In);
	}
	return 0;
}

int Directory::DeepScan(std::string root_dir_In, XMLTree *files,
		std::string file_In) {
	LOG("Directory::DeepScan");
	std::string scan_dir = root_dir_In;
	if (file_In != "") {
		scan_dir += "\\" + file_In;
	}
	std::vector<std::string> to_scan_files;

	this->Scan(scan_dir, to_scan_files, false);
	if (to_scan_files.size() != 0) {
		XMLTree *folder = NULL;
		folder = new XMLTree("folder", NULL, NULL);

		folder->AddAttribute("name", file_In);
		folder->AddAttribute("parent", root_dir_In);
		for (std::vector<std::string>::iterator i = to_scan_files.begin();
				i != to_scan_files.end(); i++) {
			this->DeepScan(scan_dir, folder, (*i));
		}
		files->AddChild(folder);
	} else {
		// found a file
		XMLTree *file = NULL;
		file = new XMLTree("file", NULL, NULL);

		file->AddAttribute("name", file_In);
		file->AddAttribute("parent", root_dir_In);
		file->AddAttribute("md5", MD5File(scan_dir.c_str()));

		files->AddChild(file);

	}
	return 0;
}

int Directory::Scan(std::string root_dir_In, std::vector<std::string> &files,
		bool warn) {
	LOG("Directory::Scan");

	DIR *dp;
	struct dirent *dirp;

	if ((dp = opendir(root_dir_In.c_str())) == NULL) {
		if (warn) {
			LOG_WARN("Error(" + ToString(errno) + ") opening " + root_dir_In);
		}
		return errno;
	}

	while ((dirp = readdir(dp)) != NULL) {
		std::string name = ToString(dirp->d_name);
		if (!is_dotfile(name)) {
			files.push_back(ToString(name));
		}
	}

	closedir(dp);
	//std::sort (files.begin(), files.end());			//added from computing.net tip

	return 0;
}
}

