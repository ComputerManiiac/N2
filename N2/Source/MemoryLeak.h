#ifndef MEMORYLEAK_H
#define MEMORYLEAK_H

#include <iostream>
#include <map>

extern void* operator new(size_t size, const char* func, int line);
extern void* operator new[](size_t size, const char* func, int line);
extern void operator delete(void* ptr);
extern void operator delete[](void* ptr);
extern void operator delete(void* ptr, const char* func, int line);

struct MemoryAlloc {

	MemoryAlloc(const char* func, int line) : func(func), line(line){}

	const char* func;
	int line;
};

class MemoryLeak {

public:
	static void* Allocate(size_t size, const char* func, int line);
	static void Dump();
	static std::map<void*, MemoryAlloc> allocations;

};
#endif
