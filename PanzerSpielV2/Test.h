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
	ArrayIndex i = 0;
	for each(DataEntry<T> tmp in array_to_check.m_entries) {
		if (array_to_check.m_indices.at(tmp.index) != (ArrayIndex)i) {
			return result::Fail;
		}
		i++
	}
	return result::Pass;
}
