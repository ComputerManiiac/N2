#ifndef CONTIGUOUS_H
#define CONTIGUOUS_H


#include <iostream>
#include <map>
#include <typeindex>

#undef new


class Contiguous
{
public:
	Contiguous()
	{}

	~Contiguous()
	{
		std::cout << "Freed 0x" << (void*)start << std::endl;
		free((void*)start);
	}

	void getSize(unsigned int& size) {}

	template<typename T, typename... Rest>
	void getSize(unsigned int& size, T*& t, Rest*&... rest);

	template<typename T, typename... Rest>
	void defineTypes(T*& t, Rest*&... rest);

	template<typename T>
	T* getBlock();

	template<typename T, typename... TArgs>
	T* Allocate(TArgs... args);

private:


	std::map<std::type_index, void*> objects;
	unsigned int currentOffset;
	const char* start;

};


template<typename T, typename... TArgs>
T* Contiguous::Allocate(TArgs... args)
{
	T* t;
	auto type = std::type_index(typeid(T));
	t = new ((void*)(start + currentOffset)) T(args...);
	objects[type] = (void*)t;
	currentOffset += sizeof(T);
	return t;
}


template<typename T, typename... Rest>
void Contiguous::getSize(unsigned int& size, T*& t, Rest*&... rest)
{
	size += sizeof(T);
	getSize(size, rest...);
}

template<typename T, typename... Rest>
void Contiguous::defineTypes(T*& t, Rest*&... rest)
{
	currentOffset = 0;
	unsigned int size = 0;
	getSize(size, t, rest...);
	start = (char*)malloc(size);
}



template<typename T>
T* Contiguous::getBlock()
{
	auto type = std::type_index(typeid(T));
	if (objects.find(type) != objects.end())
		return (T*) (objects[type]);
	else
		return nullptr;
}

#ifdef OVERLOAD_NEW_H
	#define new new(__FUNCTION__, __LINE__)
#endif


#endif