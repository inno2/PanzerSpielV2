#include "Windows.h"
#include "EntityManager.h"
#include "PackedArray.h"

// Components
#include "Movement_Component.h"
#include "Transformation_Component.h"


EntityManager::EntityManager(unsigned int max_entity_count)
{
	m_EntityCount = 0;
	m_MaxEntityCount = max_entity_count;
	m_Entities.init(max_entity_count);
	
	// Create ComponentArrays:
	m_ComponentArrayLookupTable.push_back(new PackedArray<Movement_Component>(max_entity_count));
	m_ComponentArrayLookupTable.push_back(new PackedArray<Transformation_Component>(max_entity_count));
}

EntityManager::~EntityManager()
{
	// Delete ComponentArrays:
	for (auto pa : m_ComponentArrayLookupTable)
	{
		delete pa;
	}
}

EntityId EntityManager::CreateEntity()
{
	// Dont create more than allowed
	if (m_EntityCount == m_MaxEntityCount)
		return 0;

	ComponentIndexLUTRow entry;
	ZeroMemory(&entry, sizeof(entry));
	EntityId newID;

	// Add entity and retrieve new Id
	if (!m_Entities.add(entry, newID))
		return 0;

	// Add one to the entitycount
	m_EntityCount++;
	
	return newID;
}

void EntityManager::DestroyEntity(EntityId ent)
{
	ComponentIndexLUTRow componentLUT;
	ZeroMemory(&componentLUT, sizeof(componentLUT));

	if (!m_Entities.get(ent, componentLUT)) {
		return;
	}	

	ComponentIndexPair componentIndexPair;

	for(unsigned int type = 0; type < COMPONENT_MAX_TYPE; type++)
	{
		componentIndexPair = componentLUT.at(type);
		if (componentIndexPair.assigned)
		{
			IPackedArray* compArray = m_ComponentArrayLookupTable.at(type);
			compArray->remove(componentIndexPair.index);
		}
	}

	m_Entities.remove(ent);
	m_EntityCount--;
}

std::vector<EntityId>& EntityManager::get_entities(const std::vector<ComponentType>& filter)
{
	std::vector<EntityId> entitys;

	
}

