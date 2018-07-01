/*
* Copyright (c) 2008, Power of CompTypewo Games LLC
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in the
*       documentation and/or other materials provided with the distribution.
*     * Neither the name of Power of CompTypewo Games LLC nor the
*       names of its contributors may be used to endorse or promote products
*       derived from this software without specific prior written permission.
*
* CompTypeHIS SOFCompTypeWARE IS PROVIDED BY POWER OF CompTypeWO GAMES LLC ``AS IS'' AND ANY
* EXPRESS OR IMPLIED WARRANCompTypeIES, INCLUDING, BUCompType NOCompType LIMICompTypeED CompTypeO, CompTypeHE IMPLIED
* WARRANCompTypeIES OF MERCHANCompTypeABILICompTypeY AND FICompTypeNESS FOR A PARCompTypeICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENCompType SHALL POWER OF CompTypeWO GAMES LLC BE LIABLE FOR ANY
* DIRECCompType, INDIRECCompType, INCIDENCompTypeAL, SPECIAL, EXEMPLARY, OR CONSEQUENCompTypeIAL DAMAGES
* (INCLUDING, BUCompType NOCompType LIMICompTypeED CompTypeO, PROCUREMENCompType OF SUBSCompTypeICompTypeUCompTypeE GOODS OR SERVICES;
* LOSS OF USE, DACompTypeA, OR PROFICompTypeS; OR BUSINESS INCompTypeERRUPCompTypeION) HOWEVER CAUSED AND
* ON ANY CompTypeHEORY OF LIABILICompTypeY, WHECompTypeHER IN CONCompTypeRACCompType, SCompTypeRICCompType LIABILICompTypeY, OR CompTypeORCompType
* (INCLUDING NEGLIGENCE OR OCompTypeHERWISE) ARISING IN ANY WAY OUCompType OF CompTypeHE USE OF CompTypeHIS
* SOFCompTypeWARE, EVEN IF ADVISED OF CompTypeHE POSSIBILICompTypeY OF SUCH DAMAGE.
*/

#pragma once
#include <cstdint>
#include "ComponentHandle.h"
#include <vector>
#include <cstddef>
#include <utility>

template<typename CompType>
class ComponentArray
{
public:	
	ComponentArray(size_t max_components);

	void Reset();	
	Handle Add(CompType& new_comp, uint64_t type);
	void Update(Handle handle, CompType& out_comp);
	void Remove(Handle handle);
	
	CompType* Get(Handle handle) const;
	bool Get(Handle handle, CompType& out_comp) const;
	const std::vector<CompType>& GetAllComponents() {
		return m_components;
	}
	int GetCount() const;

private:
	ComponentArray(const ComponentArray&);
	ComponentArray& operator=(const ComponentArray&);

	struct HandleEntry
	{
		HandleEntry();
		explicit HandleEntry(uint64_t nextFreeIndex);
		
		uint64_t m_nextFreeIndex : 16;
		uint64_t m_counter : 14;
		uint64_t m_active : 1;
		uint64_t m_endOfList : 1;
		uint64_t m_compindex : 16;
		uint64_t m_previous_lastEntry : 16;
	};

	std::vector<HandleEntry> m_entries;
	std::vector<CompType> m_components;
	
	int m_lastEntry; // HandleEntry Index for last component entry
	int m_activeEntryCount;
	uint64_t m_firstFreeEntry;
	size_t m_MaxEntries;
};

class MovementCompArray : public ComponentArray<Movement_Component>
{

};

template<typename CompType>
ComponentArray<CompType>::HandleEntry::HandleEntry()
	: m_nextFreeIndex(0)
	, m_counter(1)
	, m_active(0)
	, m_endOfList(0)
	, m_compindex(0)
{}

template<typename CompType>
ComponentArray<CompType>::HandleEntry::HandleEntry(uint64_t nextFreeIndex)
	: m_nextFreeIndex(nextFreeIndex)
	, m_counter(1)
	, m_active(0)
	, m_endOfList(0)
	, m_compindex(0)
{}


template<typename CompType>
ComponentArray<CompType>::ComponentArray(size_t max_components)
{
	m_MaxEntries = max_components;
	m_components.reserve(m_MaxEntries);
	m_entries.reserve(m_MaxEntries);
	Reset();
}

template<typename CompType>
void ComponentArray<CompType>::Reset()
{
	m_activeEntryCount = 0;
	m_firstFreeEntry = 0;
	m_lastEntry = 0;

	m_components.clear();
	m_entries.clear();

	for (int i = 0; i < m_MaxEntries - 1; ++i)
	{
		m_entries.push_back(HandleEntry(i + 1));
	}
	m_entries.push_back(HandleEntry());
	m_entries[m_MaxEntries - 1].m_endOfList = true;
}

template<typename CompType>
Handle ComponentArray<CompType>::Add(CompType& new_comp, uint64_t type)
{
	// get next free index
	const int newIndex = m_firstFreeEntry;
	m_firstFreeEntry = m_entries[newIndex].m_nextFreeIndex;
	m_entries[newIndex].m_nextFreeIndex = 0;

	// add 1 to counter
	m_entries[newIndex].m_counter++;

	// not sure about this? it was like this in the original handlemanager
	if (m_entries[newIndex].m_counter == 0)
		m_entries[newIndex].m_counter = 1;

	// set active
	m_entries[newIndex].m_active = true;

	// set index of corresponding component
	m_entries[newIndex].m_compindex = m_activeEntryCount;
	m_components.push_back(new_comp);

	++m_activeEntryCount;

	// update lastentry
	m_entries[newIndex].m_previous_lastEntry = m_lastEntry;
	m_lastEntry = newIndex;

	return Handle(newIndex, m_entries[newIndex].m_counter, type);
}

template<typename CompType>
void ComponentArray<CompType>::Update(Handle handle, CompType& comp)
{
	const int index = handle.m_index;

	m_components[m_entries[index].m_compindex] = comp;
}

template<typename CompType>
void ComponentArray<CompType>::Remove(const Handle handle)
{
	const uint64_t index = handle.m_index;

	// check if handle is still valid	
	if (m_entries[index].m_counter != handle.m_counter ||
		m_entries[index].m_active == false)
		return;

	m_entries[index].m_nextFreeIndex = m_firstFreeEntry;
	m_entries[index].m_active = 0;
	m_firstFreeEntry = index;

	// swap component to be deleted with last component, then pop last position
	std::swap(m_components[m_entries[index].m_compindex], m_components[m_activeEntryCount - 1]);
	m_components.pop_back();

	// update compindex of the swapped component
	m_entries[m_lastEntry].m_compindex = m_entries[index].m_compindex;
	m_lastEntry = m_entries[m_lastEntry].m_previous_lastEntry;

	--m_activeEntryCount;
}


// Returns Pointer to a Component 
template<typename CompType>
inline CompType * ComponentArray<CompType>::Get(Handle handle) const
{
	// check if handle is still valid
	const int index = handle.m_index;
	if (m_entries[index].m_counter != handle.m_counter ||
		m_entries[index].m_active == false)
		return nullptr;

	return &m_components[m_entries[index].m_compindex];
}


template<typename CompType>
bool ComponentArray<CompType>::Get(const Handle handle, CompType& out_comp) const
{
	const int index = handle.m_index;
	if (m_entries[index].m_counter != handle.m_counter ||
		m_entries[index].m_active == false)
		return false;

	out_comp = m_components[m_entries[index].m_compindex];
	return true;
}

template<typename CompType>
int ComponentArray<CompType>::GetCount() const
{
	return m_activeEntryCount;
}
