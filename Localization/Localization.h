#pragma once

//DLL STUFF
#ifdef LOCALIZATION_EXPORTS
#define LOCALIZATION_API __declspec(dllexport)
#else
#define LOCALIZATION_API __declspec(dllimport)
#endif

//INCLUDES
#include <string>
#include <unordered_map>
#include <fstream>

#include "Logger.h"

class Localization
{
public:
	//main funcs
	LOCALIZATION_API static bool        LoadLanguage();
	LOCALIZATION_API static const char* GetString(std::string key);
private:
	//singleton
	Localization() = default;
	~Localization() = default;
	Localization(const Localization&) = delete;
	Localization& operator=(const Localization&) = delete;
private:
	static std::string current; //the language currently in use
	static std::unordered_map<std::string, std::string> language; //stores keys and values for the language we are using
};

