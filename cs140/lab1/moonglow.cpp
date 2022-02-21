#include <iostream>

using namespace std;

int main()
{
	string currentWord = ""; //Current word being read (if not a number)
	string name = ""; //The student's name
	double score = 0; //The current score being read
	double scoreTotal = 0; //The student's total score so far
	double scoreAverage = 0; //The score total to be averaged
	int scoreAmount = 0; //The number of scores that make up the average
	bool nameFlag, averageFlag = false;
	
	//Begin reading file
	while(!cin.eof())
	{
		//Attempt to read word as double
		if(cin >> score)
		{
			if(!averageFlag)
				scoreTotal += score; //Not averaging; add score directly
			else
			{
				scoreAmount++;
				scoreAverage += score;
			}

			continue;
		}
		
		else
		{
			//Calculate average
			if(averageFlag)
			{
				if(scoreAmount != 0)
					scoreTotal += scoreAverage / scoreAmount;
				
				scoreAverage = 0;
				scoreAmount = 0;
				averageFlag = false;
			}
	
			cin.clear();
		}

		//Read in word if not a number
		cin >> currentWord;

		//This word should be a name (last word read was NAME)
		if(nameFlag)
		{
			name = currentWord;
			nameFlag = false;
			continue;
		}

		//Set flags
		if(currentWord == "NAME")
			nameFlag = true;
		else if(currentWord == "AVERAGE")
			averageFlag = true;
	}

	cout << name << " " << scoreTotal << endl;
	
	return 0;
}
