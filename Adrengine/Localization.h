#pragma once

#include <string>
#include <unordered_map>
#include <fstream>

#include "Logger.h"

class Localization
{
public:
	//main funcs
	bool LoadLanguage();
	const char* GetString(std::string key);
public:
	//getter for instance
	static Localization& Get() {
		static Localization local;
		return local;
	}
private:
	//singleton
	Localization() = default;
	~Localization() = default;
	Localization(const Localization&) = delete;
	Localization& operator=(const Localization&) = delete;
private:
	std::string current = "english";
	std::unordered_map<std::string, std::string> language;
};

