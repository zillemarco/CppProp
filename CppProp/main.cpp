#include <iostream>
#include "PropertyBinding.h"

using namespace std;

class A
{
public:
	A()
		: pubGetPubSet(1, *this)
		, pubGetPriSet(2, *this)
		, priGetPubSet(3, *this)
		, priGetPriSet(4, *this)
	{ }

public:
	Property<int, A, Public, Public>	pubGetPubSet;
	Property<int, A, Public, Private>	pubGetPriSet;
	Property<int, A, Private, Public>	priGetPubSet;
	Property<int, A, Private, Private>	priGetPriSet;
};

int main(int argc, char** argv)
{
	A test;

	BindingContext bc;
	bc.AddProperty("a", &test.pubGetPubSet);
	bc.AddProperty("b", &test.pubGetPriSet);
	bc.AddProperty("c", &test.priGetPubSet);
	bc.AddProperty("d", &test.priGetPriSet);

	int i = bc.GetPropertyValue("a", 0);

	int k = 5;
	int j = *((int*)bc.GetPropertyValueGeneric("m", &k));

	system("pause");

	return 1;
}