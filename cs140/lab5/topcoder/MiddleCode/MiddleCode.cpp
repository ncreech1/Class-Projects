#include <cstring>
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

class MiddleCode {
  public:
    string encode(string s);
};

string MiddleCode::encode(string s)
{
	string t = "";
	while(s.size() != 0)
	{
		if(s.size() % 2 == 0)
		{
			if(s[s.size() / 2 - 1] <= s[s.size() / 2])
			{
				t += s[s.size() / 2 - 1];
				s = s.erase(s.size() / 2 - 1, 1);
			}

			else
			{
				t += s[s.size() / 2];
				s = s.erase(s.size() / 2, 1);
			}
			
		}
		
		else
		{
			t += s[s.size() / 2];
			s = s.erase(s.size() / 2, 1);
		}
	}

	return t;
}
