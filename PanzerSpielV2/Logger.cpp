#include "Logger.h"
#include <fstream>

void FileLogger::Log(std::string msg)
{
	std::ofstream file (filename, std::ofstream::out | std::ofstream::app);
	if (file.good())
	{
		file.write(msg.c_str(), msg.length());
	}
}
