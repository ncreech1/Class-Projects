#include <iostream>

using namespace std;

int main()
{
	int R, C, CS, W;
	char SC;

	//Read in inputs
	cin >> R;
	cin >> C;
	cin >> SC;
	cin >> CS;
	cin >> W;

	if(!cin)
	{
		cerr << "usage: checkerboard  - stdin should contain R, C, SC, CS and W\n";
		return 1;
	}

	//Exit silently if any input <= 0 or SC + CS > 127
	if(R <= 0 || C <= 0 || CS <= 0 || W <= 0 || SC + CS > 127)
		return 1;

	int currentChar = 0; //Current character index in cycle
	int trueY, trueX = 0; //The true (x, y) position regardless of position in W * W square

	//Begin printing out checkerboard
	for(int y = 0; y < R * W; y++)
	{
		//Increment true y position every W characters vertical
		if(y != 0 && y % W == 0)
			trueY++;

		for(int x = 0; x < C * W; x++)
		{
			//Increment true x position every W characters horizontal
			if(x != 0 && x % W == 0)
				trueX++;
			
			cout << (char)(SC + (trueX + trueY) % CS);
		}

		cout << "\n";
		trueX = 0;
	}

	return 0;
}
