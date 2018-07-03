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
	using ComponentIndexLUTRow = std::array<ComponentIndexLUTPair, COMPONENT_MAX_TYPES>;
	using ComponentIndexLUT = std::vector<ComponentIndexLUTRow>;

	using ComponentArrayLUT = std::vector<IPackedArray*>;
	
public:
	EntityManager(unsigned int max_entities);
	~EntityManager();

	EntityId CreateEntity();
	void DestroyEntity(EntityId id);

	void AddComponent(EntityId ent, Component* compPtr, ComponentType type);
	void RemoveComponent(EntityId ent, ComponentType type);
		
	template<typename Comp>
	bool GetComponent(EntityId ent, ComponentType type, Comp * comp_out);

	template<typename Comp>
	PackedArray<Comp>* GetAllComponentsByType(ComponentType type);

	template<typename... comptypes>
	std::vector<EntityId>& GetMatchingEntities(comptypes... filters);

private:
	// Component Index and Component Array Fast Lookup Array
	ComponentArrayLUT m_ComponentArrayLookupTable;

	unsigned int m_EntityCount;
	unsigned int m_MaxEntityCount;
	PackedArray<ComponentIndexLUTRow> m_Entities;
};

template<typename Comp>
inline bool EntityManager::GetComponent(EntityId ent, ComponentType type, Comp* comp_out)
{
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

	comp_out = reinterpret_cast<Comp*>(buf);
}

template<typename Comp>
inline PackedArray<Comp>* EntityManager::GetAllComponentsByType(ComponentType type)
{
	return reinterpret_cast<PackedArray<Comp>*>(m_ComponentArrayLookupTable[type]))	
}

template<typename... comptypes>
inline std::vector<EntityId>& EntityManager::GetMatchingEntities(comptypes... filters)
{
	static std::vector<EntityId> result = new std::vector<EntityId>();


	// todo much
}
