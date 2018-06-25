#pragma once
enum ComponentType
{
	Invalid_Type = 0,
	Input,
	Movement,
	Transformation,
};

class Turret_Interaction_Component : public Component
{
	public bool shoot;
	//public enum ammunition_type reload;
	public float rotate;
};