#include "MovementSystem.h"
#include "GameObjectFactory.h"

bool MovementSystem::Update(std::chrono::microseconds)
{
	for each(Movement_Component movement in GameObjectFactory::m_Movement_CompManager.Get_all_components)	
	{
		if (movement.m_transformation_component == NULL) {
			continue;
		}
		

	}

	return false;
}
