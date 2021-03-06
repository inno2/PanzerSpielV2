#include "Test.h"
#include "PackedArray.h"
#include "EntityManager.h"
#include "Movement_Component.h"
#include "Transformation_Component.h"



//PackedArray(unsigned int max_entries);		tested
//PackedArray();
//~PackedArray();
//
//void Init(unsigned int max_entries);			tested
//
//T* begin() { return m_entries; }
//T* end() { return m_entries + m_Size; }
//
//// Geerbt �ber IPackedArray
//virtual bool Add(void * newEntry, ArrayIndex & newIndex) override;		tested
//virtual void Remove(const ArrayIndex index) override;
//virtual void* Get(const ArrayIndex index) override;						tested
//virtual void * GetAll() override;
//virtual unsigned int Count() override;									tested
//virtual void Reset() override;

Test::result Test::testAll() {
	result local_status = Fail;
	result global_status = Pass;

	std::cout << "\n-----------------\nTeste PackedArray\n-----------------\n";
	local_status = Pass;
	local_status = Test_PackedArray();
	if (local_status == Fail) {
		global_status = Pass;
	}
	std::cout << "\n-----------------\nPackedArray: "<<(local_status == Pass ? "Pass" : "Fail") << "\n-----------------\n";

	std::cout << "\n-----------------\nTesteEntityManager\n-----------------\n";
	local_status = Pass;
	local_status = Test_EntityManager();
	if (local_status == Fail) {
		global_status = Pass;
	}
	std::cout << "\n-----------------\nEntityManager: " << (local_status == Pass ? "Pass" : "Fail") << "\n-----------------\n";
	
	return global_status;
}

Test::result Test::Test_PackedArray() {
	const unsigned int amount = 100;
	ArrayIndex indices[amount];
	int values[amount];

	PackedArray<int> UUT = PackedArray<int>(amount);	//could be any number
	UUT.init(amount);

	//check count and entries
	//check Count
	if (UUT.count() != 0) {
		return Fail;
	}

	//create some entries	
	ArrayIndex index;
	for (int i = 0; i < amount ; i++) {
		int tmp_int;
		tmp_int = hash(i);
		values[i] = tmp_int;
		if (UUT.add(tmp_int, index) == false) {
			return Fail;
		}
		indices[i] = index;
	}

	//check Count
	if (UUT.count() != amount) {
		return Fail;
	}
	
	//check entries
	int i = 0;
	for each (auto tmp in UUT){	//ist Reihenfolge im Speicher auch Pr�finhalt?		
		if (tmp.data != values[i]) {
			return Fail;
		}
		i++;
	}

	if(i != amount)
		return Fail;

	//delete every second entry
	for (int i = 0; i < amount; i += 2) {
		UUT.remove(indices[i]);
	}

	//check count and entries
	//check Count
	if (UUT.count() != (unsigned int)(amount / 2)) {
		return Fail;
	}
	for (int i = 0; i < amount; i++) {
		bool valid;
		int tmp;
		valid = UUT.get(indices[i], tmp);

		if (i % 2 == 0) {	//entry removed
			if ((UUT.exists(indices[i]) == true) || (valid == true)) {
				return Fail;
			}
		}
		else {	//still valid
			if (UUT.exists(indices[i]) == false || (valid == false)) {
				return Fail;
			}
			if (tmp != values[i]) {
				return Fail;
			}
		}

	}
	//change some values
	for (int i = ((amount/2) - 1); i >= 0; i--) {
		int& tmp = UUT.get(indices[i]);
		
		tmp = hash(-i);
		values[i] = tmp;
	}

	//get all entries and compare
	
	auto tmp = UUT.get_all();
	for (int i = 0; i < (amount / 2); i ++)
	{
		if (tmp.at(indices[i]).data != values[i]) {
			return Fail;
		}
	}
	
	//create some more entries	
	for (int i = 0; i < (amount / 2); i++) {
		if (UUT.add(i, index) == false) {
			return Fail;
		}
	}

	if (Packedarray_consistency_check(UUT) == result::Fail) {
		return result::Fail;
	}

	//std::vector<int> *list = (std::vector<int>*)UUT.GetAll();
	//Reset

	return result::Pass;
}
Test::result Test::Test_EntityManager() {
	const unsigned int entity_amount = 100;
	EntityManager em = EntityManager(entity_amount);		
	EntityId entities[entity_amount];

	// create entities and add 2 components
	for (int i = 0; i < entity_amount; i++) {
		entities[i] = em.create_entity();

		Transformation_Component tcomp;
		tcomp.somevalue = i;

		Movement_Component mcomp;
		mcomp.somevalue = -i;

		em.add_component<Transformation_Component>(entities[i], tcomp);
		em.add_component<Movement_Component>(entities[i], mcomp);
	}

	if (em.count() != entity_amount) {
		return result::Fail;
	}

	//delete every second entry
	for (int i = 0; i < entity_amount / 2; i ++) {
		em.destroy_entity(entities[i]);
		entities[i] = 0;
	}

	//check count
	if (em.count() != (entity_amount / 2)) {
		return Fail;
	}

	// check if each entity still has its components
	int count = entity_amount / 2;
	for each(EntityId ent in em.get_entities2<Transformation_Component, Movement_Component>())
	{
		Transformation_Component tcomp;
		if (!em.get_component<Transformation_Component>(ent, tcomp))
			return result::Fail;

		if (tcomp.somevalue != (int)ent)
			return result::Fail;

		Movement_Component mcomp;
		if (!em.get_component<Movement_Component>(ent, mcomp))
			return result::Fail;
		
		if (mcomp.somevalue != -(int)ent)
			return result::Fail;

		count++;
	}

	// did we get ALL components back (we should have)
	if (count != entity_amount) {
		return result::Fail;
	}	

	return result::Pass;
}

int Test::hash(int input) {
	return (input ^  (0x9e3779b9 + (input << 6) + (input >> 2)));
}