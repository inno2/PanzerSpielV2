#pragma once
#include "Component.h"
#include <vector>

using ArrayIndex = unsigned int;

class IPackedArray
{
public:
	virtual bool Add(void * newEntry, ArrayIndex & newIndex) = 0;
	virtual void Remove(const ArrayIndex index) = 0;
	virtual void* Get(const ArrayIndex index) = 0;
	virtual void* GetAll() = 0;
	virtual unsigned int Count() = 0;
	virtual void Reset() = 0;
};


template <class T>
class PackedArray : public IPackedArray
{
	struct IndexEntry
	{
		IndexEntry();
		explicit IndexEntry(ArrayIndex nextFreeIndex);

		ArrayIndex m_nextFreeIndex;		
		ArrayIndex m_compindex;
		ArrayIndex m_previous_lastEntry;
		bool m_active;
		bool m_endOfList;
	};


public:
	PackedArray(unsigned int max_entries);
	PackedArray();
	~PackedArray();

	void Init(unsigned int max_entries);

	T* begin() { return m_entries; }
	T* end() { return m_entries + m_Size; }

	// Geerbt über IPackedArray
	virtual bool Add(void * newEntry, ArrayIndex & newIndex) override;
	virtual void Remove(const ArrayIndex index) override;
	virtual void* Get(const ArrayIndex index) override;
	virtual void * GetAll() override;
	virtual unsigned int Count() override;
	virtual void Reset() override;

private:
	ArrayIndex m_lastEntry; // HandleEntry Index for last component entry
	unsigned int m_activeEntryCount;
	unsigned int m_firstFreeEntry;
	unsigned int m_MaxEntries;
	size_t m_Size;

	std::vector<IndexEntry> m_indices;
	std::vector<T> m_entries;
};

template<class T>
inline PackedArray<T>::PackedArray(unsigned int max_entries)
{
	Init(max_entries);
}

template<class T>
inline PackedArray<T>::PackedArray()
{
	m_MaxEntries = 0;
}



template<class T>
inline PackedArray<T>::~PackedArray()
{
	m_entries.clear();
	m_indices.clear();
}

template<class T>
inline void PackedArray<T>::Init(unsigned int max_entries)
{
	m_MaxEntries = max_entries;
	m_indices.reserve(max_entries);
	m_entries.reserve(max_entries);
	Reset();
}

template<class T>
inline bool PackedArray<T>::Add(void * newEntry, ArrayIndex& newIndex)
{
	if (m_MaxEntries == 0)
		return false;

	// get next free index
	newIndex = m_firstFreeEntry;
	m_firstFreeEntry = m_indices[newIndex].m_nextFreeIndex;
	m_indices[newIndex].m_nextFreeIndex = 0;

	// set active
	m_indices[newIndex].m_active = true;

	// set index of corresponding entry
	m_indices[newIndex].m_compindex = m_activeEntryCount;
	T entry = *reinterpret_cast<T*>(newEntry);
	m_entries.push_back(*reinterpret_cast<T*>(newEntry));

	++m_activeEntryCount;

	// update lastentry
	m_indices[newIndex].m_previous_lastEntry = m_lastEntry;
	m_lastEntry = newIndex;

	m_Size += sizeof(T);

	return true;
}

template<class T>
inline void PackedArray<T>::Remove(const ArrayIndex index)
{
	if (m_MaxEntries == 0)
		return;

	if (m_indices[index].m_active == false)
		return;

	m_indices[index].m_nextFreeIndex = m_firstFreeEntry;
	m_indices[index].m_active = 0;
	m_firstFreeEntry = index;

	// swap component to be deleted with last component, then pop last position
	std::swap(m_entries[m_indices[index].m_compindex], m_entries[m_activeEntryCount - 1]);
	m_entries.pop_back();

	// update compindex of the swapped component
	m_indices[m_lastEntry].m_compindex = m_indices[index].m_compindex;
	m_lastEntry = m_indices[m_lastEntry].m_previous_lastEntry;

	m_Size -= sizeof(T);

	--m_activeEntryCount;
}

template<class T>
inline void* PackedArray<T>::Get(const ArrayIndex index)
{
	if (m_MaxEntries == 0)
		return nullptr;

	if (m_indices[index].m_active != true)
		return nullptr;

	return reinterpret_cast<void*>(&m_entries.at(m_indices[index].m_compindex));
}

template<class T>
inline void * PackedArray<T>::GetAll()
{
	return &m_entries[0];
}

template<class T>
inline unsigned int PackedArray<T>::Count()
{
	return m_activeEntryCount;
}

template<class T>
inline void PackedArray<T>::Reset()
{
	m_activeEntryCount = 0;
	m_firstFreeEntry = 0;
	m_lastEntry = 0;
	m_Size = 0;

	m_indices.clear();
	m_entries.clear();

	for (unsigned int i = 0; i < m_MaxEntries - 1; ++i)
	{
		m_indices.push_back(IndexEntry(i + 1));
	}
	m_indices.push_back(IndexEntry());
	m_indices[m_MaxEntries - 1].m_endOfList = true;
}

template<class T>
inline PackedArray<T>::IndexEntry::IndexEntry()
	: m_nextFreeIndex(0)
	, m_active(0)
	, m_endOfList(0)
	, m_compindex(0)
{}

template<class T>
inline PackedArray<T>::IndexEntry::IndexEntry(ArrayIndex nextFreeIndex)
	: m_nextFreeIndex(nextFreeIndex)
	, m_active(0)
	, m_endOfList(0)
	, m_compindex(0)
{}
