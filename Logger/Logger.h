#pragma once

//DLL STUFF
#ifdef LOGGER_EXPORTS
#define LOGGER_API __declspec(dllexport)
#else
#define LOGGER_API __declspec(dllimport)
#endif

#include <iostream>

struct Tab;

class LOGGER_API Logger
{
public:
	static void Log(const char* type, const char* log); //getter for the instance is not necessary
private:
	//singleton
	Logger() = default;
	~Logger() = default;
	Logger(const Logger&) = delete;
	Logger& operator=(const Logger&) = delete;
};

