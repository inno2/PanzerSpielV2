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
	m_Entities.Init(max_entity_count);
	
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
	if (!m_Entities.Add(&entry, newID))
		return 0;

	// Add one to the entitycount
	m_EntityCount++;
	
	return newID;
}

void EntityManager::DestroyEntity(EntityId ent)
{
	// Check if entity even exists
	
	void* buf = m_Entities.Get(ent);
	if (buf == nullptr)
	{
		return;
	}

	ComponentIndexLUTPair* cilutr = reinterpret_cast<ComponentIndexLUTPair*>(buf);

	for(unsigned int type = 0; type < COMPONENT_MAX_TYPES; type++)
	{
		if (cilutr->assigned)
		{
			IPackedArray* compArray = m_ComponentArrayLookupTable.at(type);
			compArray->Remove(cilutr->index);
		}
		cilutr++;
	}

	m_Entities.Remove(ent);
	m_EntityCount--;
}

void EntityManager::AddComponent(EntityId ent, Component* compPtr, ComponentType type)
{
	void* buf = m_Entities.Get(ent);
	if (buf == nullptr)
	{
		return;
	}
			
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

void EntityManager::RemoveComponent(EntityId ent, ComponentType type)
{
	void* buf = m_Entities.Get(ent);
	if (buf == nullptr)
	{
		return;
	}

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
