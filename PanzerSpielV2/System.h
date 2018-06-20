#pragma once
#include <chrono>

class System
{
public: 
	virtual void Shutdown() = 0;
	virtual bool Update(std::chrono::microseconds) = 0;	
	virtual bool Init() = 0;
};