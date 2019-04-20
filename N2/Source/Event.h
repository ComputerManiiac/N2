#ifndef EVENT_H
#define EVENT_H

#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <functional>



class Event {

public:


	template<typename Function, typename ...Args>
	static void Register(std::string eventName, Function&& function, Args&&... args)
	{
		events[eventName].push_back(std::bind(function, args...));
	}

	static void Trigger(std::string eventName)
	{
		std::vector<std::function<void()>>& functions = events[eventName];
		for (auto& f : functions)
			f();
	}

private:
	static std::map<std::string, std::vector<std::function<void()>>> events;
};

std::map<std::string, std::vector<std::function<void()>>> Event::events = {};

#endif