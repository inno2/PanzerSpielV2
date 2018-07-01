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
	
	template<typename... comptypes>
	std::vector<EntityId>& GetMatchingEntities(comptypes... filters);

private:
	// Component Index and Component Array Fast Lookup Array
	ComponentArrayLUT m_ComponentArrayLookupTable;

	unsigned int m_EntityCount;
	unsigned int m_MaxEntityCount;
	PackedArray<ComponentIndexLUTRow> m_Entities;
};





template<typename... comptypes>
inline std::vector<EntityId>& EntityManager::GetMatchingEntities(comptypes... filters)
{
	static std::vector<EntityId> result = new std::vector<EntityId>();


	// todo much
}
