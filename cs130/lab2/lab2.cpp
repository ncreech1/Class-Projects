/* Nicholas Creech
 * 1/23/19
 * Lab2: Create a program that allows the user to set, test, and clear the bits of different integers
 */

#include <iostream>
#include <vector>
#include <string>

using namespace std;

class BITSET
{
	public:
		BITSET();
		void Set(int);
		void Clear(int);
		bool Test(int) const;
		string to_binary(int) const;
	private:
		vector<int> data;
};

int main()
{

	BITSET bs;
	char command = ' ';
	int val = 0;
	bool flag = false; //Flag used to break the while-loop

	while(!flag)
	{
		printf("Enter a command: ");
		scanf("%c",  &command);

		//Only check for second parameter if the command is not 'q'
		if(command != 'q')
			scanf("%d", &val);
				
		//Check which command was inputted
		switch(command)
		{
			case 't':
				printf("%d", (int)bs.Test(val));
				break;
			case 's':
				bs.Set(val);
				break;
			case 'c':
				bs.Clear(val);
				break;
			case 'p':
				printf("%s", bs.to_binary(val).c_str());
				break;
			case 'q':
				flag = true;
				break;
		}

		//Formatting the display with new lines
		if(!flag && command != 'c' && command != 's')
			printf("\n");

		//Clear scanf to prevent stopping at \n characters
		while(getchar() != '\n');
	}

	return 0;
}

BITSET::BITSET()
{
	data.push_back(0);
}

void BITSET::Set(int index)
{
	unsigned int set = index / 32;
	unsigned int bit = index % 32;

	//Expand the vector if the index is larger than the curren number of sets
	while(set > data.size() - 1)
		data.push_back(0);

	data.at(set) |= (1 << bit);
}

void BITSET::Clear(int index)
{
	unsigned int set = index / 32;
	unsigned int bit = index % 32;

	//Index out of range; do nothing
	if(set > data.size() - 1)
		return;

	data.at(set) &= ~(1 << bit);

	//Remove empty sets (up to the 0 index set)
	for(unsigned int x = data.size() - 1; x > 0; x--)
	{
		if(data.at(x) == 0)
			data.pop_back();
		else
			break;
	}
}

bool BITSET::Test(int index) const
{
	unsigned int set = index / 32;
	unsigned int bit = index % 32;

	//Index out of range; return false
	if(set > data.size() - 1)
		return false;

	return (data.at(set) >> bit) & 1;
}

string BITSET::to_binary(int set) const
{
	string result = "";

	//Must use unsigned set value to compare to data.size()
	unsigned int uSet = set;

	//Set out of range
	if(uSet > data.size() - 1)
		return "Set #" + to_string(set) + " does not exist.";

	//Construct string representation of the 32 bits
	for(int bit = ((set + 1) * 32) - 1; bit >= set * 32; bit--)
	{
		result += to_string(Test(bit));

		//Adds a space every four bits
		if(bit % 4 == 0)
			result += " ";
	}

	return result;
}
