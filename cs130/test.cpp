#include <iostream>

using namespace std;

struct MyStruct
{
	char p;
	char ptr[51];
	int value;
	short king;
};

int main()
{
	MyStruct test;

	cout << sizeof(test.ptr);
}
