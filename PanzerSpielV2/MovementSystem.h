#pragma once
#include "System.h"

class MovementSystem : public System
{
	// Geerbt �ber System
	virtual bool Update(std::chrono::microseconds) override;
};