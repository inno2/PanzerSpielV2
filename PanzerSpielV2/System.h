#pragma once
#include <chrono>

class System
{
public: 
	virtual bool Update(std::chrono::microseconds) = 0;	
};