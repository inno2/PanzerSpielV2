#pragma once
#include <iostream>

class Test {
public:
	enum result {
		Pass = 0,
		Fail
	};

	result testAll();

	result Test_PackedArray();
	result Test_EntityManager();
private:
	int hash(int input);
};