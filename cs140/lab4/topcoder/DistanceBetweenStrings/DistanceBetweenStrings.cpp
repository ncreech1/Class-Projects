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

class DistanceBetweenStrings {
  public:
    int getDistance(string a, string b, string letterSet);
};

int DistanceBetweenStrings::getDistance(string a, string b, string l)
{
	int total = 0;
	for(int i = 0; i < l.size(); i++)
	{
		int inA = 0;
		for(int x = 0; x < a.size(); x++)
		{
			if(l[i] == a[x])
				inA++;
			else if(l[i] >= 65 && l[i] < 97 && l[i] + 32 == a[x])
				inA++;
			else if(l[i] >= 97 && l[i] - 32 == a[x])
				inA++;
		}

		int inB = 0;
		for(int x = 0; x < b.size(); x++)
		{
			if(l[i] == b[x])
				inB++;
			else if(l[i] >= 65 && l[i] < 97 && l[i] + 32 == b[x])
				inB++;
			else if(l[i] >= 97 && l[i] - 32 == b[x])
				inB++;
		}

		total += (inA - inB) * (inA - inB);
	}

	return total;
}
