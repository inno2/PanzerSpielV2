#pragma once
#include <type_traits>

#define COMPONENT_MAX_TYPE (ComponentType::TotalComponentTypes)

enum ComponentType
{
	MovementComponent = 0,
	TransformationComponent,
	TotalComponentTypes
};


template <class Derived>
class Component 
{
	friend class EntityManager;

private:
	static ComponentType type;
};




