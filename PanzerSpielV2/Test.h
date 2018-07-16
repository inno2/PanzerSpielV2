#pragma once
#include <iostream>
#include "PackedArray.h"

class Test {
public:
	enum result {
		Pass = 0,
		Fail
	};

	result testAll();

	template <typename T>
	result Packedarray_consistency_check(PackedArray<T>& array_to_check);
	result Test_PackedArray();
	result Test_EntityManager();
private:
	int hash(int input);
};

template<typename T>
inline Test::result Test::Packedarray_consistency_check(PackedArray<T>& array_to_check)
{
	// check consistency of index and dataentry pairs
	ArrayIndex i = 0;
	for each(DataEntry<T> tmp in array_to_check.m_entries) {
		if (array_to_check.m_indices.at(tmp.index).m_compindex != (ArrayIndex)i) {
			return result::Fail;
		}
		i++;
	}
	
	// check the consistency of the freeindices
	unsigned int freeIndicesAmount = array_to_check.m_maxEntries - array_to_check.m_activeEntryCount;
	unsigned int currentFreeindex = array_to_check.m_firstFreeEntry;
	for(unsigned int i = 0; i < freeIndicesAmount; i++){
		// check if a free index is marked as active
		if(array_to_check.m_indices.at(currentFreeindex).m_active){
			return result::Fail;
		}
		
		// check if a free index still has a dataentry (if the compindex points to valid dataentry)
		int buf = array_to_check.m_indices.at(currentFreeindex).m_compindex;
		if (buf < freeIndicesAmount) {
			if (array_to_check.m_entries.at(array_to_check.m_indices.at(currentFreeindex).m_compindex).index == currentFreeindex) {
				return result::Fail;
			}
		}
		
		
		// check if the last free index has the flag endofList == true
		if(i == (freeIndicesAmount) /*last entry*/ && array_to_check.m_indices.at(currentFreeindex).m_endOfList == false){
			return result::Fail;
		}
		
		// get next free Index
		currentFreeindex = array_to_check.m_indices.at(currentFreeindex).m_nextFreeIndex;
	}
	
			
	return result::Pass;
}
