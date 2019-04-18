#ifndef MANAGER_H
#define MANAGER_H

#include "PhysicsSystem.h"
#include "RenderSystem.h"
#include "ParticleSystem.h"
#include "ShaderProgram.h"
#include "Camera.h"
#include "Entity.h"
#include <map>
#include <typeindex>
#include <vector>

class Manager
{
public:

	static Manager* getInstance();
	virtual ~Manager();
	
	void Register();
	virtual void Initialize();
	virtual void Update(double dt);

	template<typename T>
	void registerComponent(Component* component);

	template<typename T>
	void registerSystem();

	template<typename T>
	T* getSystem();

	Camera* getCamera();
	ShaderProgram* getShader(const std::string& name);

private:
	std::map<std::string, Entity*> entities;

protected:



	std::map<std::type_index, System*> systems;
	std::map<std::string, ShaderProgram> shaders;

	Camera* camera;

protected:
	static Manager* instance;
	Manager();
};


template<typename T>
void Manager::registerComponent(Component * component)
{
	systems[std::type_index(typeid(T*))]->registerComp(component);
}


template<typename T>
void Manager::registerSystem() {
	systems[std::type_index(typeid(T*))] = new T();
}


template<typename T>
T* Manager::getSystem()
{
	return static_cast<T*>(systems[std::type_index(typeid(T*))]);
}

#endif