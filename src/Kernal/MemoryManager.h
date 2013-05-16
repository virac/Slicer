#ifndef __MEM_MANAGER_H__
#define __MEM_MANAGER_H__

#include <stdlib.h>
#include <sstream>
#include <string>
#include <map>

struct mem_loc {
	mem_loc() :
			line(-1), size(0) {
	}
	;
	mem_loc(std::string file_In, int line_In, size_t size_In) :
			file(file_In), line(line_In), size(size_In) {
	}
	;
	std::string file;
	int line;
	size_t size;

	std::string PrintMem();
};

#ifdef USE_MEM_MANAGER

#ifdef HAS_LOG

//static std::map<void *,mem_loc> mem_map;
std::map<void *,mem_loc> *GetMyMemMapPtr();
bool* UsingMemMapPtr();

inline void *
operator new (size_t size, const char *file, int line)
{
	void *mem = NULL;
	mem = malloc(size);
	if( *UsingMemMapPtr() )
	{
		(*GetMyMemMapPtr())[mem] = mem_loc( file, line, size );
	}
	return mem;
}
inline void *
operator new[] (size_t size, const char *file, int line)
{
	void *mem = NULL;
	mem = malloc(size);
	if( *UsingMemMapPtr() )
	{
		(*GetMyMemMapPtr())[mem] = mem_loc(std::string(file), line, size );
	}
	return mem;
}

inline void
operator delete (void *p)
{
	if( *UsingMemMapPtr() )
	(*GetMyMemMapPtr()).erase(p);
	free(p);
}

inline void
operator delete[] (void *p)
{
	if( *UsingMemMapPtr() )
	(*GetMyMemMapPtr()).erase(p);
	free(p);
}

#define NEW new (__FILE__, __LINE__)
#define new NEW

#else
#error NO LOG

#endif //HAS_LOG
#endif //USE_MEM_MANAGER
void StartMemoryWatch();
void EndMemoryWatch();
void PrintAllocedMem();
#endif // __MEM_MANAGER_H__
