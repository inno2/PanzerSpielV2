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

#include "ComponentManager.h"
#include <cstddef>

template<typename component_type>
ComponentManager<component_type>::HandleEntry::HandleEntry()
	: m_nextFreeIndex(0)
	, m_counter(1)
	, m_active(0)
	, m_endOfList(0)
	, m_entry(NULL)
{}

template<typename component_type>
ComponentManager<component_type>::HandleEntry::HandleEntry(uint64_t nextFreeIndex)
	: m_nextFreeIndex(nextFreeIndex)
	, m_counter(1)
	, m_active(0)
	, m_endOfList(0)
	, m_entry(NULL)
{}


template<typename component_type>
ComponentManager<component_type>::ComponentManager()
{
	Reset();
}

template<typename component_type>
void ComponentManager<component_type>::Reset()
{
	m_activeEntryCount = 0;
	m_firstFreeEntry = 0;

	for (int i = 0; i < MaxEntries - 1; ++i)
		m_entries[i] = HandleEntry(i + 1);
	m_entries[MaxEntries - 1] = HandleEntry();
	m_entries[MaxEntries - 1].m_endOfList = true;
}

template<typename component_type>
Handle ComponentManager<component_type>::Add(void* p, uint64_t type)
{
	const int newIndex = m_firstFreeEntry;
	m_firstFreeEntry = m_entries[newIndex].m_nextFreeIndex;
	m_entries[newIndex].m_nextFreeIndex = 0;
	m_entries[newIndex].m_counter = m_entries[newIndex].m_counter + 1;
	if (m_entries[newIndex].m_counter == 0)
		m_entries[newIndex].m_counter = 1;
	m_entries[newIndex].m_active = true;
	m_entries[newIndex].m_entry = p;

	++m_activeEntryCount;

	return Handle (newIndex, m_entries[newIndex].m_counter, type);
}

template<typename component_type>
void ComponentManager<component_type>::Update(Handle handle, void* p)
{
	const int index = handle.m_index;

	m_entries[index].m_entry = p;
}

template<typename component_type>
void ComponentManager<component_type>::Remove(const Handle handle)
{
	const uint64_t index = handle.m_index;
	
	m_entries[index].m_nextFreeIndex = m_firstFreeEntry;
	m_entries[index].m_active = 0;
	m_firstFreeEntry = index;

	--m_activeEntryCount;
}

template<typename component_type>
void* ComponentManager<component_type>::Get(Handle handle) const
{
	void* p = NULL;
	if (!Get(handle, p))
		return NULL;
	return p;
}

template<typename component_type>
bool ComponentManager<component_type>::Get(const Handle handle, void*& out) const
{
	const int index = handle.m_index;
	if (m_entries[index].m_counter != handle.m_counter ||
	    m_entries[index].m_active == false)
		return false;

	out = m_entries[index].m_entry;
	return true;
}

template<typename component_type>
int ComponentManager<component_type>::GetCount() const
{
	return m_activeEntryCount;
}

