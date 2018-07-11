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


struct ComponentIndexLUTPair
{	
	ComponentIndexLUTPair()
	{
		index = 0;
		assigned = 0;
	}

	ArrayIndex index;
	bool assigned;
};

class EntityManager
{
	using ComponentIndexLUTRow = std::vector<ComponentIndexLUTPair>;
	using ComponentIndexLUT = std::vector<ComponentIndexLUTRow>;

	using ComponentArrayLUT = std::vector<IPackedArray*>;
	
public:
	EntityManager(unsigned int max_entities);
	~EntityManager();

	EntityId CreateEntity();
	void DestroyEntity(EntityId id);

	template <typename C>
	void AddComponent(EntityId ent, const C& compPtr);

	template <typename C>
	void RemoveComponent(EntityId ent);
		
	template<typename C>
	bool GetComponent(EntityId ent, C& comp_out);

	template<typename C>
	std::vector<C>& GetAllComponentsByType();

	template<typename... comptypes>
	std::vector<EntityId>& GetMatchingEntities(comptypes... filters);

	

private:
	// Component Index and Component Array Fast Lookup Array
	ComponentArrayLUT m_ComponentArrayLookupTable;

	unsigned int m_EntityCount;
	unsigned int m_MaxEntityCount;
	PackedArray<ComponentIndexLUTRow> m_Entities;
};



// Adds Component of specified type
template<typename C>
inline void EntityManager::AddComponent(EntityId ent, const C & compPtr)
{
	ComponentType type = get_type<C>();
	
	// Check if entity even exists
	void* buf = m_Entities.Get(ent);

	if (buf == nullptr)
		return false;

	ComponentIndexLUTPair* cilutr = reinterpret_cast<ComponentIndexLUTPair*>(buf);

	// Check if a similar component already exists
	if (cilutr[type].assigned)
		return;

	// Get correct component Array from the LUT
	IPackedArray* compArray = m_ComponentArrayLookupTable.at(type);

	// Add component to array
	ArrayIndex newIndex;
	if (!compArray->Add(reinterpret_cast<void*>(compPtr), newIndex))
		return;

	// Update the ComponentIndexLUT
	cilutr[type].index = newIndex;
	cilutr[type].assigned = true;
}

template<typename C>
inline void EntityManager::RemoveComponent(EntityId ent)
{
	ComponentType type = get_type<C>();

	// Check if entity even exists
	void* buf = m_Entities.Get(ent);

	if (buf == nullptr)
		return false;

	ComponentIndexLUTPair* cilutr = reinterpret_cast<ComponentIndexLUTPair*>(buf);

	// Check if a the component even exists
	if (!cilutr[type].assigned)
		return;

	// Get correct component Array from the LUT
	IPackedArray* compArray = m_ComponentArrayLookupTable.at(type);

	// Remove component
	compArray->Remove(cilutr[type].index);

	// Update indexentry
	cilutr[type].index = 0;
	cilutr[type].assigned = false;
}

template<typename C>
inline bool EntityManager::GetComponent(EntityId ent, C & comp_out)
{
	ComponentType type = get_type<C>();

	// Check if entity even exists
	void* buf = m_Entities.Get(ent);

	if (buf == nullptr)
		return false;

	ComponentIndexLUTPair* cilutr = reinterpret_cast<ComponentIndexLUTPair*>(buf);
	ComponentIndexLUTPair v1 = cilutr[type];


	if (!v1.assigned)
		return false;

	IPackedArray* compArray = m_ComponentArrayLookupTable.at(type);
	buf = compArray->Get(v1.index);

	if (buf == nullptr)
		return false;

	comp_out = reinterpret_cast<C*>(buf);
}

