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

typedef vector <int> IVec;

class Halving {
  public:
    int minSteps(vector <int> a);
};

void all_breaks(int len, int breaks, map <int, int> &m)
{
	//Base case
	if(m.find(len) != m.end() && m.find(len)->second <= breaks)
		return;
	else
	{
		m[len] = breaks;

		if(len > 1)
		{
			all_breaks(len / 2, breaks + 1, m);
			all_breaks(len - (len / 2), breaks + 1, m);
		}
	}
}

int Halving::minSteps(vector <int> a)
{
  map <int, int> m;
  map <int, int> canbreak, minbreaks;
  map <int, int>::iterator mit;
  int minval = -1;
  size_t i;

	for(int x = 0; x < a.size(); x++)
	{
		m.clear();
		all_breaks(a[x], 0, m);

		for(mit = m.begin(); mit != m.end(); mit++)
		{
			if(canbreak.find(mit->first) == canbreak.end())
				canbreak[mit->first] = 1;
			else
				canbreak[mit->first] += 1;

			if(minbreaks.find(mit->first) == minbreaks.end())
				minbreaks[mit->first] = mit->second;
			else
				minbreaks[mit->first] += mit->second;
		}
	}

	for(mit = canbreak.begin(); mit != canbreak.end(); mit++)
	{
		if(mit->second == a.size())
		{
			if(minval == -1)
				minval = minbreaks[mit->first];
			else if(minbreaks[mit->first] < minval)
				minval = minbreaks[mit->first];
		}
	}

  return minval;
}
