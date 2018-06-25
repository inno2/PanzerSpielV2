#pragma once
#include <chrono>
#include <string>

class System
{
public: 
	virtual void Shutdown() = 0;
	virtual bool Update(std::chrono::microseconds) = 0;	
	virtual bool Init() = 0;
};