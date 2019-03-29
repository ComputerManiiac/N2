#include "Application.h"
#include <typeindex>
#include <map>
#include <iostream>

//class Component {
//public:
//	Component() {}
//	virtual ~Component() {}
//};
//
//class DerivedOne : public Component {
//public:
//	DerivedOne() {}
//	char i;
//	char j;
//	char k;
//};
//
//class DerivedTwo : public Component {
//public:
//	DerivedTwo() {}
//	char a;
//	char b;
//	void run(){
//		std::cout << "works leh" << std::endl;
//	}
//
//};
//
//class Entity {
//public:
//	DerivedOne o;
//	DerivedTwo t;
//};

//std::map<std::type_index, Component*> components;
//
//template<typename T>
//T* getComponent()
//{
//	return static_cast<T*>(components[std::type_index(typeid(T))]);
//}
//
//

int main()
{
	Application app("N2 Engine", 1368, 768);
	app.Run();


	//const char* start = (char*) malloc(sizeof(DerivedOne) + sizeof(DerivedTwo));

	//std::cout << sizeof(DerivedTwo) << std::endl;

	//auto t1 = std::type_index(typeid(DerivedOne));
	//auto t2 = std::type_index(typeid(DerivedTwo));


	//// Done at initialization
	//components[t1] = (Component*)(start);
	//components[t2] = (Component*)(start + sizeof(DerivedOne));

	//getComponent<DerivedOne>()->i = 1;
	//getComponent<DerivedOne>()->j = 2;
	//getComponent<DerivedOne>()->k = 3;

	//getComponent<DerivedTwo>()->a = 4;
	//getComponent<DerivedTwo>()->b = 5;

	//getComponent<DerivedTwo>()->run();

	//free((void*)start);


	MemoryLeak::Dump();
	return 0;
}