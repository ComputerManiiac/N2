#include "MemoryLeak.h"
#include <vector>
std::map<void*, MemoryAlloc> MemoryLeak::allocations = {};


void* operator new(size_t size, const char* func, int line)
{
	return MemoryLeak::Allocate(size, func, line);
}

void* operator new[](size_t size, const char* func, int line) {
	return MemoryLeak::Allocate(size, func, line);
}

void operator delete(void* ptr)
{
	std::map<void*, MemoryAlloc>* allocations = &MemoryLeak::allocations;
	if (allocations->find(ptr) != allocations->end())
	{
		allocations->erase(ptr);
		free(ptr);
	}
}

void operator delete[](void* ptr) {
	operator delete(ptr);
}

void operator delete(void* ptr, const char* func, int line)
{
	operator delete(ptr);
}


void* MemoryLeak::Allocate(size_t size, const char* func, int line)
{
	void* ptr = std::malloc(size);
	allocations.try_emplace(ptr, func, line);
	return ptr;
}

void MemoryLeak::Dump()
{

	for (auto &pair : allocations)
	{
		MemoryAlloc* alloc = &pair.second;
		std::cout << "[MEMORY LEAK] " << alloc->func << " - Line " << alloc->line << std::endl;
		delete pair.first;
	}
}

