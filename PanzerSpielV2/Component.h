#pragma once
#include <type_traits>

using ComponentType = unsigned int;

constexpr unsigned int register_components();
const unsigned int total_component_types = register_components();

class BaseComponent
{
public:
	static ComponentType type_counter;
};


template <class Derived>
class Component : public BaseComponent
{
	friend class EntityManager;

private:
	static ComponentType type();
};

template<class Derived>
inline constexpr ComponentType Component<Derived>::type()
{
	static type_ = type_counter++;
	return type;
}

template<typename C>
inline constexpr ComponentType get_type()
{
	return Component<typename std::remove_const<C>::type>::type();
}


