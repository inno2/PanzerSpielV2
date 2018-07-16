#pragma once
#include <vector>
#include "Component.h"
#include "PackedArray.h"
#include <utility>
#include <array>


using EntityId = unsigned int; 
// - given to entity at creation
// - may be given to a different entity if the current one is removed
// - with gaps in between


struct ComponentIndexPair
{	
	ComponentIndexPair()
	{
		index = 0;
		assigned = 0;
	}

	ArrayIndex index;
	bool assigned;
};

class EntityManager
{
#ifdef _DEBUG
	friend class Test;
#endif
	using ComponentIndexLUTRow = std::array<ComponentIndexPair, COMPONENT_MAX_TYPE>;
	using ComponentIndexLUT = std::vector<ComponentIndexLUTRow>;

	using ComponentArrayLUT = std::vector<IPackedArray*>;
	
public:
	EntityManager(unsigned int max_entities);
	~EntityManager();

	EntityId create_entity();
	void destroy_entity(EntityId id);

	template <typename C>
	void add_component(EntityId ent, const C& comp);

	template <typename C>
	void remove_component(EntityId ent);
		
	template<typename C>
	bool get_component(EntityId ent, C& comp_out);

	template<typename C>
	std::vector<DataEntry<C>>& get_all_components();
		
	std::vector<EntityId> get_entities(const std::vector<ComponentType>& filter);
	
	template<typename ...Args>
	std::vector<EntityId>& get_entities2();

	template<typename C>
	ComponentType get_component_type();

	bool entity_exists(EntityId ent);

	unsigned int count() { return m_entities.count(); }
private:
	template<typename C>
	void check_single_component(const ComponentIndexLUTRow & componentIndexLutrow, bool & match); // internally used by get_entites2

private:
	// Component Index and Component Array Fast Lookup Array
	ComponentArrayLUT m_componentArrayLUT;

	bool m_entites_changed;
	unsigned int m_entity_count;
	unsigned int m_max_entity_count;
	PackedArray<ComponentIndexLUTRow> m_entities;
};



// Adds Component of specified type
template<typename C>
inline void EntityManager::add_component(EntityId ent, const C& comp)
{
	static ComponentType type = get_type<C>();			

	if (!m_entities.exists(ent))
		return;

	ComponentIndexLUTRow& componentLUT = m_entities.get(ent);

	// Check if a similar component already exists
	ComponentIndexPair& componentIndexPair = componentLUT.at(type);
	if (componentIndexPair.assigned)
		return;

	// Get correct component Array from the LUT
	static PackedArray<C>* compArray = reinterpret_cast<PackedArray<C>*>m_componentArrayLUT.at(type);

	// Add component to array
	ArrayIndex newIndex;
	if (!compArray->add(comp, newIndex))
	{
		return;
	}

	// Update the ComponentIndexLUT
	componentIndexPair.index = newIndex;
	componentIndexPair.assigned = true;

	m_entites_changed = true;
}

template<typename C>
inline void EntityManager::remove_component(EntityId ent)
{
	static ComponentType type = get_type<C>();

	if (!entity_exists(ent))
		return;

	ComponentIndexLUTRow& componentLUT = m_entities.get(ent);

	// Check if a the component even exists
	ComponentIndexPair& componentIndexPair = componentLUT.at(type);	
	if (!componentIndexPair.assigned)
		return;

	// Get correct component Array from the LUT
	static PackedArray<C>* compArray = reinterpret_cast<PackedArray<C>*>m_componentArrayLUT.at(type);

	// Remove component
	compArray->remove(componentIndexPair.index);

	// Update indexentry
	componentIndexPair.index = 0;
	componentIndexPair.assigned = false;

	m_entites_changed = true;
}

template<typename C>
inline bool EntityManager::get_component(EntityId ent, C& comp_out)
{
	static ComponentType type = get_type<C>();		

	if (!entity_exists(ent))
		return;

	ComponentIndexLUTRow& componentLUT = m_entities.get(ent);
	
	ComponentIndexPair componentIndexPair = componentLUT.at(type);
	
	// Check if component exists
	if (!componentIndexPair.assigned)
		return false;

	static PackedArray<C>* compArray = reinterpret_cast<PackedArray<C>*>m_componentArrayLUT.at(type);
	C& comp_out = compArray->get(componentIndexPair.index);
}

template<typename C>
inline std::vector<DataEntry<C>>& EntityManager::get_all_components()
{
	static ComponentType type = get_type<C>();
	
	// Get correct component Array from the LUT
	static PackedArray<C>* compArray = reinterpret_cast<PackedArray<C>*>m_componentArrayLUT.at(type);

	return compArray->get_all();

}

template<typename C>
inline void  EntityManager::check_single_component(const ComponentIndexLUTRow& componentIndexLutrow, bool& match)
{
	static ComponentType type = get_type<C>();
	if (!componentIndexLutrow[type].assigned)
		match = false;
}

template<typename ...ComponentTypes>
inline std::vector<EntityId>& EntityManager::get_entities2()
{
	static std::vector<EntityId> entities;

	if (!m_entites_changed)
		return entities;

	for each(const DataEntry<ComponentIndexLUTRow>& ent_data in m_entities.get_all())
	{
		bool match = true;
		//https://stackoverflow.com/questions/21180346/variadic-template-unpacking-arguments-to-typename
		int dummy[]{ 0, (check_single_component<ComponentTypes>(ent_data, match), 0)... };

		if (match)
			entities.push_back(id);
	}
	m_entites_changed = false;
	return entities;
}

template<typename C>
inline ComponentType EntityManager::get_component_type()
{
	return Component<C>::type;
}


