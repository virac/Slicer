#include <string>
#include <map>

#include "Log.h"
#include "MemoryManager.h"

std::map<void *, mem_loc> g_mem_map;
bool g_using_mem_map = false;

std::map<void *, mem_loc> *GetMyMemMapPtr() {
	return &g_mem_map;
}

bool* UsingMemMapPtr() {
	return &g_using_mem_map;
}

std::string mem_loc::PrintMem() {
	std::stringstream out;
	out << size << " bytes @" << file << ":" << line;
	return out.str();
}

void StartMemoryWatch() {
#ifdef USE_MEM_MANAGER
	g_using_mem_map = true;
	GetMyMemMapPtr()->clear();
#endif
}

void EndMemoryWatch() {
#ifdef USE_MEM_MANAGER
	g_using_mem_map = false;
	if(!GetMyMemMapPtr()->empty())
	{
		LOG("EndMemoryWatch");
		LOG_WARN( "Unallocated memory remains!");
		PrintAllocedMem();
	}
#endif
}
void PrintAllocedMem() {
	LOG("PrintAllocedMem");
#ifdef USE_MEM_MANAGER
	std::vector<std::string> output;
	int size = 0;
	for(std::map<void *,mem_loc>::iterator i = GetMyMemMapPtr()->begin(); i != GetMyMemMapPtr()->end(); i++ )
	{
		output.push_back( Kernal::ToString(i->first) + "  " + i->second.PrintMem() );
		size += i->second.size;
	}
	LOG_WRITE( Kernal::ToString(size) + " bytes are allocated" );
	for(std::vector<std::string>::iterator i = output.begin(); i != output.end(); i++ )
	{
		LOG_WRITE( *i );
	}
#else
	LOG_DEBUG("No memory manager");
#endif
}
