#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <iostream>
#include <cstdio>
#include <cstdlib>
using namespace std;

class CatchTheBeatEasy {
  public:
    string ableToCatchAll(vector <int> x, vector <int> y);
};

string CatchTheBeatEasy::ableToCatchAll(vector <int> x, vector <int> y)
{
	multimap<int, int> fruitPos;
	multimap<int, int>::const_iterator fit;
	int playerX = 0;
	int totalMove = 0;

	for(int i = 0; i < x.size(); i++)
		fruitPos.insert(pair<int, int>(y[i], x[i]));
	
	for(fit = fruitPos.begin(); fit != fruitPos.end(); fit++)
	{
		int distX = fit->second - playerX;
		int trueDistX = (distX < 0 ? distX * -1 : distX);
	

		if(fit->first - totalMove < trueDistX)
			return "Not able to catch";
		else
		{
			totalMove += trueDistX;
			playerX = fit->second;
		}
	}

	return "Able to catch";
}
