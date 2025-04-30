#pragma once

//#include <fstream>
#include <iostream>
#include "Localization.h"

class Logger
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

