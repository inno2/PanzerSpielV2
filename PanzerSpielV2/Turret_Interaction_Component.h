#pragma once
#include "Component.h"

class Turret_Interaction_Component : public Component<Turret_Interaction_Component>
{
public:
	bool shoot;
	//public enum ammunition_type reload;
	float rotate;
};