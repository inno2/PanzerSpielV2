#include "GameObjectFactory.h"

const static size_t max_gameobjects = 100;

bool GameObjectFactory::Init()
{
	m_GameObjects.reserve(max_gameobjects);
	return false;
}
