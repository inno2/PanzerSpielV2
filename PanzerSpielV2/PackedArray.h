#pragma once
#include "Component.h"
#include <vector>

using ArrayIndex = unsigned int;

class IPackedArray
{
public:
	virtual void remove(ArrayIndex index) = 0;
	virtual bool exists(ArrayIndex index) = 0;
	virtual unsigned int count() = 0;
	virtual void reset() = 0;
};

template<class T>
struct DataEntry
{
	DataEntry(ArrayIndex index_, T data_) : index(index_), data(data_) {}
	ArrayIndex index;
	T data;
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

	void init(unsigned int max_entries);

	T* begin() { return m_entries; }
	T* end() { return m_entries + m_Size; }

	// Geerbt über IPackedArray
	virtual unsigned int count() override;
	virtual void reset() override;
	void remove(ArrayIndex index) override;
	inline bool exists(ArrayIndex index) override;

	bool add(const T& newEntry, ArrayIndex& newIndex);	
	bool get(ArrayIndex index, T& comp_out);
	T& get(ArrayIndex index);
	std::vector<DataEntry<T>>& get_all();

private:
	ArrayIndex m_lastEntry; // HandleEntry Index for last component entry
	unsigned int m_activeEntryCount;
	unsigned int m_firstFreeEntry;
	unsigned int m_MaxEntries;
	size_t m_Size;

	std::vector<IndexEntry> m_indices;
	std::vector<DataEntry<T>> m_entries;
};

template<class T>
inline PackedArray<T>::PackedArray(unsigned int max_entries)
{
	init(max_entries);
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
inline void PackedArray<T>::init(unsigned int max_entries)
{
	m_MaxEntries = max_entries;
	m_indices.reserve(max_entries);
	m_entries.reserve(max_entries);
	reset();
}


template<class T>
inline void PackedArray<T>::remove(ArrayIndex index)
{
	if (m_MaxEntries == 0 || m_activeEntryCount == 0)
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
inline bool PackedArray<T>::exists(ArrayIndex index)
{
	if (index >= m_MaxEntries)
		return false;

	return m_indices[index].m_active;
}

template<class T>
inline bool PackedArray<T>::get(ArrayIndex index, T& comp_out)
{
	if (m_MaxEntries == 0)
		return false;

	if (m_indices[index].m_active != true)
		return false;

	comp_out = m_entries.at(m_indices[index].m_compindex).data;
	return true;
}

// Always check first if the give index exists, with exists()
template<class T>
inline T& PackedArray<T>::get(ArrayIndex index)
{	
	return m_entries.at(m_indices[index].m_compindex).data;
}

template<class T>
inline std::vector<DataEntry<T>>& PackedArray<T>::get_all()
{
	return m_entries;
}

template<class T>
inline unsigned int PackedArray<T>::count()
{
	return m_activeEntryCount;
}

template<class T>
inline void PackedArray<T>::reset()
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
inline bool PackedArray<T>::add(const T& newEntry, ArrayIndex& newIndex)
{
	if (m_MaxEntries == 0 || m_activeEntryCount == m_MaxEntries)
		return false;

	// get next free index
	newIndex = m_firstFreeEntry;
	m_firstFreeEntry = m_indices[newIndex].m_nextFreeIndex;
	m_indices[newIndex].m_nextFreeIndex = 0;

	// set active
	m_indices[newIndex].m_active = true;

	// set index of corresponding entry
	m_indices[newIndex].m_compindex = m_activeEntryCount;

	m_entries.push_back(DataEntry<T>(newIndex, newEntry));

	++m_activeEntryCount;

	// update lastentry
	m_indices[newIndex].m_previous_lastEntry = m_lastEntry;
	m_lastEntry = newIndex;

	m_Size += sizeof(T);

	return true;
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
