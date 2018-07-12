#pragma once
#include "Component.h"
#include "Eigen\Dense"

class Movement_Component : public Component<Movement_Component>
{
public:
	Eigen::Vector3f m_velocity;
	Eigen::Vector3f m_rotation;
};

ComponentType Movement_Component::type = ComponentType::MovementComponent;