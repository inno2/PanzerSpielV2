/*
* Copyright (c) 2008, Power of Two Games LLC
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in the
*       documentation and/or other materials provided with the distribution.
*     * Neither the name of Power of Two Games LLC nor the
*       names of its contributors may be used to endorse or promote products
*       derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY POWER OF TWO GAMES LLC ``AS IS'' AND ANY
* EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL POWER OF TWO GAMES LLC BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#pragma once
#include <cstdint>
#include "Handle.h"
#include <vector>
#include <cstddef>
#include <utility>

template<typename component_type>
class ComponentManager
{
public:	
	ComponentManager(size_t max_components);

	void Reset();	
	Handle Add(component_type& new_comp, uint64_t type);
	void Update(Handle handle, component_type& out_comp);
	void Remove(Handle handle);
	
	component_type* Get(Handle handle) const;
	bool Get(Handle handle, component_type& out_comp) const;

	int GetCount() const;

private:
	ComponentManager(const ComponentManager&);
	ComponentManager& operator=(const ComponentManager&);

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
	std::vector<component_type> m_components;
	
	int m_lastEntry; // HandleEntry Index for last component entry
	int m_activeEntryCount;
	uint64_t m_firstFreeEntry;
	size_t m_MaxEntries;
};

template<typename component_type>
ComponentManager<component_type>::HandleEntry::HandleEntry()
	: m_nextFreeIndex(0)
	, m_counter(1)
	, m_active(0)
	, m_endOfList(0)
	, m_compindex(0)
{}

template<typename component_type>
ComponentManager<component_type>::HandleEntry::HandleEntry(uint64_t nextFreeIndex)
	: m_nextFreeIndex(nextFreeIndex)
	, m_counter(1)
	, m_active(0)
	, m_endOfList(0)
	, m_compindex(0)
{}


template<typename component_type>
ComponentManager<component_type>::ComponentManager(size_t max_components)
{
	m_MaxEntries = max_components;
	m_components.reserve(m_MaxEntries);
	m_entries.reserve(m_MaxEntries);
	Reset();
}

template<typename component_type>
void ComponentManager<component_type>::Reset()
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

template<typename component_type>
Handle ComponentManager<component_type>::Add(component_type& new_comp, uint64_t type)
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

template<typename component_type>
void ComponentManager<component_type>::Update(Handle handle, component_type& comp)
{
	const int index = handle.m_index;

	m_components[m_entries[index].m_compindex] = comp;
}

template<typename component_type>
void ComponentManager<component_type>::Remove(const Handle handle)
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
template<typename component_type>
inline component_type * ComponentManager<component_type>::Get(Handle handle) const
{
	// check if handle is still valid
	const int index = handle.m_index;
	if (m_entries[index].m_counter != handle.m_counter ||
		m_entries[index].m_active == false)
		return nullptr;

	return &m_components[m_entries[index].m_compindex];
}


template<typename component_type>
bool ComponentManager<component_type>::Get(const Handle handle, component_type& out_comp) const
{
	const int index = handle.m_index;
	if (m_entries[index].m_counter != handle.m_counter ||
		m_entries[index].m_active == false)
		return false;

	out_comp = m_components[m_entries[index].m_compindex];
	return true;
}

template<typename component_type>
int ComponentManager<component_type>::GetCount() const
{
	return m_activeEntryCount;
}
