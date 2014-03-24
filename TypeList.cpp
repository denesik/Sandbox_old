#include "TypeList.h"

void TypeListTest()
{
	typedef TYPELIST_4(int, float, double, char) List4;
	typedef TYPELIST_3(int, float, int) List3;

	CompareType<int, int>::value;
	CompareType<int, float>::value;

	Lenght<List4>::value;
	Lenght<List3>::value;

	CompareType<TypeAt<List3, 0>::Result, int>::value;
	CompareType<TypeAt<List3, 1>::Result, int>::value;
	CompareType<TypeAt<List3, 2>::Result, int>::value;

	Find<List3, int>::value;
	Find<List3, float>::value;
	Find<List3, double>::value;
	Find<List4, char>::value;

	Find<Append<List3, double>::Result, double>::value;

	Find<Erase<List3, int>::Result, int>::value;

	Find<NoDublicates<List3>::Result, int>::value;
	Lenght<NoDublicates<List3>::Result>::value;

	CheckDublicates<List3>::value;
	CheckDublicates<List4>::value;

	CheckDublicates<Append<List4, int>::Result>::value;

	CompareType<List4, List3>::value;
	CompareType<List4, List4>::value;

	typedef TYPELIST_3(int, float, double) CList1;
	typedef TYPELIST_3(int, double, float) CList2;

	CompareType<CList1, CList2>::value;
}