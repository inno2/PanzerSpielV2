#include <windows.h>
#include "EntityManager.h"
#include "PackedArray.h"

// Components
#include "Movement_Component.h"
#include "Transformation_Component.h"


EntityManager::EntityManager(unsigned int max_entity_count)
{
	m_entity_count = 0;
	m_max_entity_count = max_entity_count;
	m_entities.init(max_entity_count);
	
	// Create ComponentArrays:
	m_componentArrayLUT.push_back(new PackedArray<Movement_Component>(max_entity_count));
	m_componentArrayLUT.push_back(new PackedArray<Transformation_Component>(max_entity_count));
	m_entites_changed = true;
}

EntityManager::~EntityManager()
{
	// Delete ComponentArrays:
	for (auto pa : m_componentArrayLUT)
	{
		delete pa;
	}
}

EntityId EntityManager::CreateEntity()
{
	// Dont create more than allowed
	if (m_entity_count == m_max_entity_count)
		return 0;

	ComponentIndexLUTRow entry;
	ZeroMemory(&entry, sizeof(entry));
	EntityId newID;

	// Add entity and retrieve new Id
	if (!m_entities.add(entry, newID))
		return 0;

	// Add one to the entitycount
	m_entity_count++;
	m_entites_changed = true;

	return newID;
}

void EntityManager::DestroyEntity(EntityId ent)
{
	ComponentIndexLUTRow componentLUT;
	ZeroMemory(&componentLUT, sizeof(componentLUT));

	if (!m_entities.get(ent, componentLUT)) {
		return;
	}	

	ComponentIndexPair componentIndexPair;

	for(unsigned int type = 0; type < COMPONENT_MAX_TYPE; type++)
	{
		componentIndexPair = componentLUT.at(type);
		if (componentIndexPair.assigned)
		{
			IPackedArray* compArray = m_componentArrayLUT.at(type);
			compArray->remove(componentIndexPair.index);
		}
	}

	m_entities.remove(ent);
	m_entity_count--;
	m_entites_changed = true;
}

std::vector<EntityId> EntityManager::get_entities(const std::vector<ComponentType>& filter)
{
	std::vector<EntityId> entitys;
	
	for each(const DataEntry<ComponentIndexLUTRow>& ent_data in m_entities.get_all())
	{	
		// check if each wanted component is available
		bool match = true;
		for each(ComponentType type in filter)
		{
			if (!ent_data.data[type].assigned)
			{
				match = false;
				break;
			}
			match = true;
		}

		if (match)
			entitys.push_back(ent_data.index);
	}
	return entitys;
}

bool EntityManager::entity_exists(EntityId ent)
{
	return m_entities.exists(ent);
}

