#include <iostream>
#include "Property.h"

using namespace std;

inline void ValueChanged(const float& newValue)
{
	cout << "Value changed to " << newValue << endl;
}

class A
{
public:
	A()
		: test(0.0f, *this)
		, test2(8, *this)
	{ }

	void Test()
	{
		test.Set(10.0f);
	}

	void SetTest(const float& val)
	{
		test._value = val;
	}

	const float& GetTest()
	{
		return test._value;
	}

public:
	PROPERTY(test2, int, A, Public, Private, public);

	Property<float, A, &GetTest, &SetTest, Public, Private> test;
};

int main(int argc, char** argv)
{
	A a;

	cout << "Start: " << a.test.Get() << endl;
	a.Test();
	cout << "End: " << a.test.Get() << endl;

	cout << "Start: " << a.test2.Get() << endl;
	a.Test();
	cout << "End: " << a.test2.Get() << endl;

	system("pause");

	return 1;
}