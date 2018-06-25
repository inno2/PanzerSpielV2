#pragma once
#include "Component.h"
#include "Eigen\Dense"

class Movement_Component : public Component
{
public:
	Eigen::Vector3f m_velocity;
	Eigen::Vector3f m_rotation;
};