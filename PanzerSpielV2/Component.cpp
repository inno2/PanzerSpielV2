#include "Component.h"

// Components
#include "Movement_Component.h"
#include "Transformation_Component.h"

ComponentType BaseComponent::type_counter = 0;

constexpr unsigned int register_components()
{
	get_type<Movement_Component>();
	get_type<Transformation_Component>();
	return BaseComponent::type_counter;
}
