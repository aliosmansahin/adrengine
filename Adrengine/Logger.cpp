#include "Logger.h"

void Logger::Log(const char* type, const char* log)
{
	std::cout << "[" << type << "] " << log << std::endl;
}
