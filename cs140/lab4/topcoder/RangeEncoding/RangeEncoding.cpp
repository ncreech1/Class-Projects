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

class RangeEncoding {
  public:
    int minRanges(vector <int> arr);
};

int RangeEncoding::minRanges(vector <int> arr)
{
	int i = arr[0];
	int total = 1;
	for(int x = 1; x < arr.size(); x++)
	{
		if(arr[x] != i + 1)
			total++;
		i = arr[x];
	}

	return total;
}
