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
#include <cstring>
using namespace std;

class InsertZ {
  public:
    string canTransform(string init, string goal);
};

string InsertZ::canTransform(string init, string goal)
{
	for(int x = 0; x < goal.size(); x++)
	{
		if(goal[x] == 'z')
		{
			goal = goal.erase(x, 1);
			x--;
		}
	}

	if(goal == init)
		return "Yes";
	else
		return "No";
}
