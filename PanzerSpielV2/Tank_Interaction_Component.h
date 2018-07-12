#pragma once
#include "Component.h"

class Tank_Interaction_Component : public Component<Tank_Interaction_Component>
{
public:
	float accelerate;
	float rotate;
};