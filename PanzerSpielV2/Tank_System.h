#pragma once
#include "System.h"

class Tank_System : public System
{
	// Geerbt über System
	virtual bool Update(std::chrono::microseconds) override;
};