#include <cstdlib>
#include <iostream>
#include <string>
#include <cmath>

using namespace std;

/* Nicholas Creech
 * CS494 Lab0
 * 09/01/21
 */

class AliceGame
{
	public:
		long long a, b, r;
		long long findMinimumValue(long long score, long long round);
		long long binarySearch();
};

int main(int argc, char **argv)
{
	AliceGame game;

	game.a = atoll(argv[1]);
	game.b = atoll(argv[2]);
	game.r = sqrt(game.a + game.b);

	cerr << endl << "New Test Case" << endl;

	cout << game.findMinimumValue(game.a, game.r) << endl;
}

//Recursively finds the minimum rounds Alice could have won
long long AliceGame::findMinimumValue(long long score, long long round)
{
	cerr << "Entering Recursion" << endl;

	//Check if (a + b) is a perfect square
	if(r * r != a + b)
		return -1;

	//Base cases
	if(score == 2 || b == 2)
		return -1;
	if(score == 0)
		return 0;
	if(score < 2 * round && score % 2 != 0)
		return 1;
	if(score <= 2 * round && score % 2 == 0)
		return 2;
	if(score == 2 * round + 1)
		return 3;

	//Attempt to speed up by finding greatest h final rounds Alice could win
	if(r == round)
	{
		int h = binarySearch();
		a -= (2 * r * h - h * h);
		r -= h;

		cerr << "Doing Binary Search" << endl;

		return h + findMinimumValue(a, r);
	}

	//Recursively find the minimum value
	return 1 + findMinimumValue(score - (2 * round - 1), round - 1);
}

//Returns the greatest h final rounds Alice could win using a binary search 
long long AliceGame::binarySearch()
{
	long long max, min, h, ubound, lastValid, pointsLeft;

	max = r;
	min = 0;
	h = (max + min) / 2;
	lastValid = 0;

	//Perform the binary search
	while(1)
	{
		ubound = 2 * (r - h) + 1; //Upper bound of Alice's remaining points
		pointsLeft = a - (2 * r * h - h * h); //Alice's remaining points

		//Are Alice's remaining points in the range at this step?
		if(pointsLeft >= 0 && pointsLeft <= ubound) 
			lastValid = h;

		//Final h found or h unable to be found
		if(min == max || min > max)
			return lastValid;

		//Alice's remaining points are at the upper bound; h found
		if(pointsLeft == ubound)
			return h;

		//h must be larger
		else if(pointsLeft > ubound)
		{
			min = h + 1;
			h = (max + min) / 2;
		}

		//h must be smaller
		else if(pointsLeft < ubound) 
		{
			max = h - 1;
			h = (max + min) / 2;
		}
	}
}
