#include "MovementSystem.h"
#include "GameObjectFactory.h"

bool MovementSystem::Update(std::chrono::microseconds)
{
	for each(GameObject object in GameObjectFactory::m_GameObjects)
	{
		if (!object.HasComponent(ComponentType::Movement) || !object.HasComponent(ComponentType::Transformation))
			continue;


	}

	return false;
}
