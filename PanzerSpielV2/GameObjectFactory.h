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
	static ComponentManager<Movement_Component> m_MovementCompManager;
	static ComponentManager<Transformation_Component> m_TransformationCompManager;
	static ComponentManager<Input_Component> m_InputCompManager;
};