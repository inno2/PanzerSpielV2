#pragma once
#include "System.h"

class MovementSystem : public System
{
	// Geerbt über System
	virtual bool Update(std::chrono::microseconds) override;
};