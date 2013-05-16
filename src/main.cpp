#include <map>
#include <string>
#include <fstream>
#include <dirent.h>
#include <iostream>

#include "Kernal\Log.h"
#include "Kernal\XMLTree.h"
#include "Kernal\Directory.h"
#include "Kernal\MD5.h"
#include "Kernal\snappy.h"
#include "Kernal\Thread.h"
#include "Kernal\Lock.h"
#include "Kernal\Mutex.h"
#include "Kernal\Watch.h"

#include "TestMessageSender.h"

int main(int argc, char** argv) {
	LOG_SET_NAME("Slicer");
	LOG_SET_LEVEL(Kernal::LOG_LEVEL_DEBUG);
	//if( LOG_SET_FILE( "log.txt" ) )
	//      return 1;
	if (false) {
		LOG("MAIN");

		Kernal::Directory *dir = NULL;
		// std::vector<std::string> files;

		dir = new Kernal::Directory();

		Kernal::XMLTree *tree = NULL;
		tree = new Kernal::XMLTree("root", NULL, NULL);

		dir->DeepScan("L:\\Music", tree);

		std::string input = "";
		std::string xml_file = "";
#ifdef DEBUG
		xml_file += "debug_";
#endif
		xml_file += "out.xml";
		tree->WriteTree(xml_file);
		input = tree->WriteTree();

		LOG_WARN("Original size is: " + Kernal::ToString(input.size()));
		LOG_WARN(
				"Original MD5 is: "
						+ Kernal::ToString(MD5String(input.c_str())));

		std::string output;
		size_t compressed_size = snappy::Compress(input.c_str(), input.size(),
				&output);

		LOG_WARN("Compressed size is: " + Kernal::ToString(output.size()));
		LOG_WARN(
				"Compressed MD5 is: "
						+ Kernal::ToString(MD5String(output.c_str())));

		std::string return_input;

		snappy::Uncompress(output.c_str(), compressed_size, &return_input);

		LOG_WARN(
				"Unompressed size is: "
						+ Kernal::ToString(return_input.size()));
		LOG_WARN(
				"Unompressed MD5 is: "
						+ Kernal::ToString(MD5String(return_input.c_str())));

		delete tree;
		delete dir;

	} else {
		LOG("MAIN");

		LOG_WRITE("main");

		TestMessageSender fp;
		TestMessageSender1 *tms1;
		TestMessageSender2 *tms2;
		TestMessageSender3 *tms3;
		TestMessageSender4 *tms4;

		Kernal::Watch::SleepSec(0.5f);
		LOG_WRITE("Running");
		Kernal::Watch times;
		Kernal::Watch current;
		Kernal::Watch::SleepSec(0.5f);

		tms1 = new TestMessageSender1;
		tms2 = new TestMessageSender2;
		tms3 = new TestMessageSender3;
		tms4 = new TestMessageSender4;
		fp.Run();
		while (Kernal::MyMessageHandlerServerRunning()) {
			Kernal::Watch::SleepSec(0.3f);
			if (times.GetSplit() > 1) {
				LOG_WRITE("Waiting here");
				LOG_STACK;
				times.Reset();
			}
			LOG_WRITE("No longer waiting here");
		}
		LOG_WRITE("IM OUTSIDE AGAIN!!!");
		Kernal::CloseMyMessageHandlerServer();
		LOG_WRITE("Deleting");

		delete tms1;
		delete tms2;
		delete tms3;
		delete tms4;

		LOG_WRITE("Closed");
	}
	CLOSE_LOG;
	return 0;
}
