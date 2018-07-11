#pragma once
#include "Component.h"
#include "Eigen\Dense"

class Transformation_Component : public Component<Transformation_Component>
{
public:
	Eigen::Vector3f m_position;
	Eigen::Vector3f m_orientation;
};