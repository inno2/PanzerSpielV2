#include "GameObjectFactory.h"

const static size_t max_gameobjects = 100;


std::vector<GameObject> GameObjectFactory::m_GameObjects;

bool GameObjectFactory::Init()
{
	m_GameObjects.reserve(max_gameobjects);


	return false;
}
