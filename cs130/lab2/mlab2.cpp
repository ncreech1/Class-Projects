//Lab2
//CS130
//Pre-lab
//Nicholas Creech
//1/17/19

#include <iostream>
using namespace std;

int Set(int original_number, int bit_index);
int Clear(int original_number, int bit_index);
int Test(int number, int bit_index);

int main()
{

}

int Set(int number, int index)
{
	return number | (1 << index);
}

int Clear(int number, int index)
{
	return number & ~(1 << index);
}

int Test(int number, int index)
{
	return (number >> index) & 1;
}
