#include "pch.h"
#include "Logger.h"

/*
PURPOSE: write the message to the console
*/
void Logger::Log(const char* type, const char* log)
{
	std::cout << "[" << type << "] " << log << std::endl;
}
