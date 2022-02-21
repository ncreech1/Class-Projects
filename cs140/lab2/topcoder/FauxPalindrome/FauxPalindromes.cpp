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

class FauxPalindromes 
{
	public:
		string classifyIt(string word);
};

bool isPalindrome(string s)
{
	for(int x = 0; x < s.size() / 2; x++)
	{
		if(s[x] != s[s.size() - 1 - x])
			return false;
	}

	return true;
}

string FauxPalindromes::classifyIt(string w)
{
	if(isPalindrome(w))
		return "PALINDROME";

	char c = '\0';
	string faux;
	for(int x = 0; x < w.size(); x++)
	{
		if(w[x] != c)
		{
			faux.push_back(w[x]);
			c = w[x];
		}
	}

	if(isPalindrome(faux))
		return "FAUX";

	return "NOT EVEN FAUX";
}

//$ chmod 755 tests.sh
//$ ./tests.sh > out.txt
//$ vimdiff out.txt 
