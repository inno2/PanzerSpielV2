#pragma once
#include <vector>
#include "GameObject.h"

#include "ComponentManager.h"
#include "Component.h"
#include "InputComponent.h"
#include "Movement_Component.h"
#include "Transformation_Component.h"


class GameObjectFactory
{
public:
	static bool Init();

	static std::vector<GameObject> m_GameObjects;
	static ComponentManager<Movement_Component> m_Movement_CompManager;
	static ComponentManager<Transformation_Component> m_Transformation_CompManager;
};