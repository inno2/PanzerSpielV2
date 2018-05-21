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

template<class component_type>
class ComponentManager
{
public:
	enum { MaxEntries = 4096 }; // 2^12

	template< typename component_type>
	ComponentManager<component_type>(size_t max_components)
	{
		m_components = new component_type[max_components];
	}

	void Reset();	
	Handle Add(void* p, uint64_t type);
	void Update(Handle handle, void* p);
	void Remove(Handle handle);
	
	void* Get(Handle handle) const;
	bool Get(Handle handle, void*& out) const;
	bool GetAs(Handle handle, component_type& out) const;

	int GetCount() const;

private:
	ComponentManager(const ComponentManager&);
	ComponentManager& operator=(const ComponentManager&);

	struct HandleEntry
	{
		HandleEntry();
		explicit HandleEntry(uint64_t nextFreeIndex);
		
		uint64_t m_nextFreeIndex : 12;
		uint64_t m_counter : 15;
		uint64_t m_active : 1;
		uint64_t m_endOfList : 1;
		void* m_entry;
	};

	HandleEntry m_entries[MaxEntries];
	component_type* m_components;

	int m_activeEntryCount;
	uint64_t m_firstFreeEntry;
};


template< typename component_type>
inline bool ComponentManager<component_type>::GetAs(Handle handle, component_type& out) const
{
	void* outAsVoid;
	const bool rv = Get(handle, outAsVoid);
	out = union_cast< component_type >(outAsVoid);

	return rv;
}

