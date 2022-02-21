#include <iostream>
#include <sstream>

using namespace std;

/* Nicholas Creech
 * 11/13/19
 * Lab 9: Create a program that recursively 
 */ 

void do_enum(string &s, int index, int ones); 

int main(int argc, char ** argv)
{
	int length, ones;
	stringstream ss;
	
	if(argc != 3)
	{
		cout << "usage: ./enum <length> <ones>\n";
		return 1;
	}

	ss << argv[1];
	if(!(ss >> length))
	{
		cout << "usage: ./enum <length> <ones>\n";
		return 1;
	}

	ss.clear();
	ss << argv[2];
	if(!(ss >> ones))
	{
		cout << "usage: ./enum <length> <ones>\n";
		return 1;
	}

	//Start recursion
	string s(length, '-');
	do_enum(s, 0, ones);

	return 0;
}

//Recursively adds ones and zeros until all combinations are found
void do_enum(string &s, int index, int ones)
{	
	//End Case: When you've filled up the string
	if(index == (int)s.size())
	{
		cout << s << endl;
		return;
	}
	
	//Add 0's until you must add ones	
	if(index != (int)s.size() - ones)
	{
		s[index] = '0';
		do_enum(s, index + 1, ones);
	}
	
	//Add 1's until the string is full
	if(ones > 0)
	{
		s[index] = '1';
		do_enum(s, index + 1, ones - 1);
	}	
}
