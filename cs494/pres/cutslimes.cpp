#include <iostream>
#include <string>
#include <stdexcept>
#include <unordered_map>

using namespace std;

class SubdividedSlime
{
	public:
		int MX(int s, int c);
		unordered_map<string, int> dynamicCache;
};

int main(int argc, char **argv)
{
	SubdividedSlime ss;
	int size, target;

	if(argc != 3)
	{
		cout << "usage: ./CutSlimes <start_size> <target_score>" << endl;
		return -1;
	}
	
	try
	{
		size = stoi(argv[1]);
		target = stoi(argv[2]);
	}

	catch(invalid_argument &e) 
	{ 
		cout << "usage: ./CutSlimes <start_size> <target_score>" << endl; 
		return -1;
	}

	//Try every cut size and find the score recursively
	for(int c = 1; c < size; c++)
	{
		//When the score is >= target, the answer has been found
		if(ss.MX(size, c) >= target)
		{
			cout << c << endl;
			cout << ss.dynamicCache.size() << endl;
			return 0;
		}
	}

	//Target cannot be achieved in any number of rounds
	cout << -1 << endl;
	return 0;
}

//Returns the maximum score for a size of s and c cuts
int SubdividedSlime::MX(int s, int c)
{
	int max, current, last;
	string keyString;
	unordered_map<string, int>::iterator it;

	max = 0;
	last = 0;

	//Base case: No more cuts
	if(c == 0)
		return max;

	//Check if the max for this size and cuts has been cached
	keyString = to_string(s) + "|" + to_string(c);
	it = dynamicCache.find(keyString);

	if(it != dynamicCache.end())
		return it->second;

	//Try all initial cut sizes for this initial size and number of cuts
	for(int a = 1; a <= s / 2; a++)
	{
		current = a * (s - a) + MX(s - a, c - 1);
	

		//If the score went down with an increase in a, the score will never get better
		if(current < last)
			break;
		
		last = current;

		//Find the max score for this size and cuts
		if(current > max)
			max = current;
	}

	//Cache the max for this size and cut
	dynamicCache[keyString] = max;

	return max;
}
