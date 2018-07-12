#pragma once
#include <string>

class BaseLogger
{
	virtual void Log(std::string msg) = 0;
};

class FileLogger : public BaseLogger
{
	FileLogger(std::string filename) : filename(filename) {};


private:
	std::string filename;

	// Geerbt über BaseLogger
	virtual void Log(std::string msg) override;
};