#include <iostream>
#include <vector>
#include <string>

using namespace std;

int main()
{
	//Local variables
	vector<string> names;
	vector<int> scores;
	string input = "";

	//Continue to get players and rolls until "done" is entered
	while(input != "done")
	{
		cout << "Enter player's name (done for no more players): ";
		cin >> input;
		
		if(input != "done")
		{
			//Store player name
			names.push_back(input);

			int rolls[21];
			for(int i = 0; i < 10; i++)
			{
				//First roll
				cout << "Enter score for frame " << i + 1 << ", roll 1: ";
				cin >> rolls[i * 2];

				//Second roll (if first was not a strike or i is the 10th frame)
				if(i + 1 == 10 || rolls[i * 2] != 10)
				{
					cout << "Enter score for frame " << i + 1 << ", roll 2: ";
					cin >> rolls[(i * 2) + 1];
				}

				else
					rolls[(i * 2) + 1] = 0;

				//Extra roll for 10th frame if last roll was strike or spare
				if(i + 1 == 10 && rolls[i * 2] + rolls[(i * 2) + 1] >= 10)
				{
					cout << "Enter score for frame " << i + 1 << ", roll 3: ";
					cin >> rolls[(i * 2) + 2];
				}

				else
					rolls[(i * 2) + 2] = 0;
			}

			//Calculate total for current player
			int total = 0;
			for(int i = 0; i < 10; i++)
			{
				//Score a strike or spare if the current frame is not 10
				if(i + 1 != 10 && rolls[i * 2] + rolls[(i * 2) + 1] == 10)
				{
					//Strike = 10 + score of next two rolls
					if(rolls[i * 2] == 10)
					{
						//Has to check the frame after the next if the next frame was also a strike
						if(rolls[((i + 1) * 2)] != 10)
							total += 10 + rolls[(i + 1) * 2] + rolls[((i + 1) * 2) + 1];
						else
							total += 10 + rolls[(i + 1) * 2] + rolls[(i + 2) * 2];
					}
					//Spare = 10 + score of next roll
					else
						total += 10 + rolls[(i + 1) * 2];
				}

				//Add 10th frame normally
				else if(i + 1 == 10)
				{
					total += rolls[i * 2] + rolls[(i * 2) + 1] + rolls[(i * 2) + 2];
				}

				else
					total += rolls[i * 2] + rolls[(i * 2) + 1];
			}
		
			//Store total for current player
			scores.push_back(total);
		}
	}

	if(names.size() == 0)
		cout << "No players were entered.";
	else
	{
		//Print out scores and determine the best and worst players
		int highestScoreIndex = 0, lowestScoreIndex = 0;
		for(int i = 0; i < names.size(); i++)
		{
			cout << names.at(i) << " scored " << scores.at(i) << '\n';
			if(scores.at(i) > scores.at(highestScoreIndex))
				highestScoreIndex = i;
			else if(scores.at(i) < scores.at(lowestScoreIndex))
				lowestScoreIndex = i;
		}

		cout << names.at(lowestScoreIndex) << " did the worst by scoring " << scores.at(lowestScoreIndex) << ".\n";
		cout << names.at(highestScoreIndex) << " won the game by scoring " << scores.at(highestScoreIndex) << ".\n";
	}

}
