#include "GameObjectFactory.h"

const static size_t max_gameobjects = 100;


std::vector<GameObject> GameObjectFactory::m_GameObjects;
ComponentManager<Input_Component> GameObjectFactory::m_InputCompManager(20);
ComponentManager<Movement_Component> GameObjectFactory::m_MovementCompManager(100);
ComponentManager<Transformation_Component> GameObjectFactory::m_TransformationCompManager(100);

bool GameObjectFactory::Init()
{
	m_GameObjects.reserve(max_gameobjects);
	return false;
}
