#include <vector>
#include <string>
#include <iostream>
#include <string>
#include <cstdlib>
#include <algorithm>
using namespace std;

class SwappingDigits {
  public:
    string minNumber(string num);
};

string SwappingDigits::minNumber(string num) 
{
	int min = num.size() - 1;
	int minpos;
	for(int i = 0; i < num.size(); i++)
	{
		int minchar = num[i];
		for(int j = num.size() - 1; j > i; j--)
		{
			if(i == 0 && num[j] == '0')
				continue;
			if(num[j] < minchar)
			{
				minchar = num[j];
				minpos = j;
			}
		}

		if(minchar < num[i])
		{
			swap(num[i], num[minpos]);
			return num;
		}
	}

	return num;
}
