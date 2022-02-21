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

class LongLiveZhangzj {
  public:
    int donate(vector <string> speech, vector <string> words);
};

int LongLiveZhangzj::donate(vector <string> speech, vector <string> words)
{
	int count = 0;
	set<string> wordSet;

	for(int x = 0; x < words.size(); x++)
		wordSet.insert(words[x]);

	for(int x = 0; x < speech.size(); x++)
	{
		if(wordSet.find(speech[x]) != wordSet.end())
			count++;
	}

    return count;
}
