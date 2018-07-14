#pragma once
#include "System.h"

class GraphicSystem : public System
{
	GraphicSystem();

	// Geerbt �ber System
	virtual void Shutdown() override;
	virtual bool Update(std::chrono::microseconds) override;
	virtual bool Init() override;
};
