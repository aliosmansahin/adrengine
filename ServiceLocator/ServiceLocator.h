#pragma once

#ifdef SERVICELOCATOR_EXPORTS
#define SERVICELOCATOR_API __declspec(dllexport)
#else
#define SERVICELOCATOR_API __declspec(dllimport)
#endif

#include <unordered_map>
#include <typeindex>

class SERVICELOCATOR_API ServiceLocator
{
public:
	template <typename Interface>
	static void Register(Interface* service)
	{
		services()[typeid(Interface)] = service;
	}

	template <typename Interface>
	static Interface* Get()
	{
		auto it = services().find(typeid(Interface));
		if (it != services().end())
		{
			return static_cast<Interface*>(it->second);
		}
		return nullptr;
	}

private:
	static std::unordered_map<std::type_index, void*>& services() {
		static std::unordered_map<std::type_index, void*> instance;
		return instance;
	}
};