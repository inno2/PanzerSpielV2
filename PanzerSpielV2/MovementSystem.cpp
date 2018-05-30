#include "MovementSystem.h"
#include "GameObjectFactory.h"

bool MovementSystem::Update(std::chrono::microseconds)
{
	for each(const Movement_Component& movement in GameObjectFactory::m_MovementCompManager.GetAllComponents())	
	{
		if (!movement.m_transformation_component.Is_valid()) {
			continue;
		}
		

	}

	return false;
}
