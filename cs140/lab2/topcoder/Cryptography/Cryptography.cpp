#include <string>
#include <vector>
#include <list>
#include <cmath>
#include <algorithm>
#include <map>
#include <set>
#include <iostream>
#include <cstdio>
#include <cstdlib>
using namespace std;

class Cryptography {
  public:
    long long encrypt(vector <int> numbers);
};

long long Cryptography::encrypt(vector <int> numbers)
{
	int smallest = numbers[0];
	int smallestIndex = 0;
	for(int x = 1; x < numbers.size(); x++)
	{
		if(numbers[x] < smallest)
		{
			smallest = numbers[x];
			smallestIndex = x;
		}
	}

	numbers[smallestIndex]++;
	
	long long product = numbers[0];

	for(int x = 1; x < numbers.size(); x++)
		product = product * numbers[x];

	return product;
}
