#include <iostream>

using namespace std;

struct PERSON
{
	char name[51];
	int age;
	char gender;
};

extern "C"
{
	PERSON * Search(PERSON people[], int num_people, const char * name, int age, char gender);
	int Median(const int array[], int array_size, int * buffer);
}

//PERSON * Search(PERSON people[], int num_people, const char * name, int age, char gender);
//int Median(const int array[], int array_size, int * buffer);

int main()
{
	int test[] = {8, 7, -10};
	int * buffer = new int[3];
	//{1, 8}
	cout << Median(test, 3, buffer);
	delete[] buffer;

	/*PERSON jeff = {"Jeff                                              ", 25, 'm'};
	PERSON zach = {"Zach                                              ", 3, 'm'};
	PERSON nick = {"Nick                                              ", 88, 'm'};
	PERSON luke = {"Luke                                              ", 9, 'm'};

	PERSON people[] = {jeff, zach, nick, luke};

	PERSON * test = (Search(people, 4, "Nick                                              ", 88, 'm'));
	
	if(test != nullptr)
		cout << (*test).name << ", " << (*test).age << ", " << (*test).gender << "\n";
	else
		cout << "No match!\n";*/
	return 0;
}

/*PERSON * Search(PERSON people[], int num_people, const char * name, int age, char gender)
{
	for(int x = 0; x < num_people; x++)
	{
		//Age and gender match, now check if the names match
		if(people[x].age == age && people[x].gender == gender)
		{
			for(int i = 0; i < 51; i++)
			{
				if(people[x].name[i] != name[i])
					break;
				else if(i == 50)
					return people + x;
			}
		}	
	}

	return 0L;
}

int Median(const int array[], int array_size, int * buffer)
{
	int smallest = array[0];
	int currentVal;

	//Find the smallest value first
	for(int x = 0; x < array_size; x++)
	{
		if(array[x] < smallest)
			smallest = array[x];
	}

	currentVal = smallest;

	int bufferIndex = 0;
	for(int x = 0; x < array_size; x++)
	{
		//Found another currentVal copy to add to the buffer
		if(array[x] == currentVal)
		{
			buffer[bufferIndex] = currentVal;
			bufferIndex++;
		}

		if(array[x] > currentVal && smallest == currentVal) //Reset smallest to new val larger than currentVal
			smallest = array[x];
		else if(array[x] > currentVal && array[x] < smallest) //Find the next true smallest val after currentVal
			smallest = array[x];

		//Restart loop to add the next smallest number to the buffer
		if(x == array_size - 1 && bufferIndex < array_size)
		{
			currentVal = smallest;
			x = -1;
		}
	}

	if(array_size % 2 != 0)
		return buffer[(array_size / 2)];
	else
		return (buffer[(array_size / 2) - 1] + buffer[array_size / 2]) / 2;
}*/
