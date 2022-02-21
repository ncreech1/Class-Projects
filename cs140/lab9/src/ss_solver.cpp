#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdio>

using namespace std;

/* Nicholas Creech
 * 11/13/19
 * Lab 9: Create a program that recursively solves the "Shapeshifter" game from neopets.com
 */ 

class Shifter
{
	public:
		bool loadGame(int argc, char **argv);
		bool solve(int currentShape);
		bool applyShape(int index, int r, int c);
		void printSolution() const;
		void printShape(int index) const;
		void printBoard(char *label) const;
	private:
		vector<string> board;
		vector< vector<string> > shapes;
		vector<int> solutionRows;
		vector<int> solutionCols;
};

int main(int argc, char **argv)
{
	Shifter game;
	
	game.loadGame(argc, argv);
	game.solve(0);
	game.printSolution();

	return 0;
}

//Load the game board from argv and the shapes from standard input
bool Shifter::loadGame(int argc, char **argv)
{
	string shape;
	stringstream ss;

	//Read board
	for(int x = 1; x < argc; x++)
	{
		string s(argv[x]);

		//Check for formatting error
		for(size_t x = 0; x < s.size(); x++)
		{
			if(s[x] != '0' && s[x] != '1')
			{
				printf("Error: Bad board row '%s'\n", s.c_str());
				return false;
			}
		}

		//Add new row to board
		board.push_back(s);
	}

	//Read shapes
	while(getline(cin, shape))
	{
		vector<string> currentShape;
		
		//Use string stream to read each row of the shape from the current line
		ss.clear();
		ss.str(shape);

		while(ss.good())
		{
			string row;
			ss >> row;

			//Check for formatting error
			for(size_t x = 0; x < row.size(); x++)
			{
				if(row[x] != '0' && row[x] != '1')
				{
					printf("Error: Bad shape row '%s'\n", row.c_str());
					return false;
				}
			}
		
			//Add new row to shape
			currentShape.push_back(row);
		}

		//Add new shape to shapes vector
		shapes.push_back(currentShape);
	}

	return true;
}

//Apply a shape to the board using the shape's index
bool Shifter::applyShape(int index, int r, int c)
{
	//Check if shape is out of bounds of board at (r, c)
	if(r + shapes[index].size() - 1 > board.size() - 1 || c + shapes[index][0].size() - 1 > board[0].size() - 1)
		return false;

	//Compare each row of the board starting at (r, c) to the corresponding row of the shape 
	for(size_t x = r; x < r + shapes[index].size(); x++)
	{
		for(size_t y = c; y < c + shapes[index][0].size(); y++)
		{
			//If the shape overlaps the current tile, flip the tile from 0 -> 1 or 1 -> 0
			if(board[x][y] == '0' && shapes[index][x - r][y - c] == '1')
				board[x][y] = '1';
			else if(board[x][y] == '1' && shapes[index][x - r][y - c] == '1')
				board[x][y] = '0';
		}
	}

	return true;
}

bool Shifter::solve(int currentShape)
{
	//End case: All the shapes have been used
	if(currentShape == (int)shapes.size())
	{
		//Has the solution been found? (all swords)
		for(size_t row = 0; row < board.size(); row++)
			for(size_t col = 0; col < board[row].size(); col++)
				if(board[row][col] != '1')
					return false;

		return true;
	}

	//Try to apply the shape at every possible spot on the board
	for(size_t r = 0; r < board.size(); r++)
	{
		for(size_t c = 0; c < board[r].size(); c++)
		{
			//Apply the shape if possible, if not try the next (r, c)
			if(applyShape(currentShape, r, c))
			{
				solutionRows.push_back(r);
				solutionCols.push_back(c);

				//Apply the next shape (or trigger end case)
				if(!solve(currentShape + 1))
				{	
					//Last apply was not correct, undo it and try the next (r, c)
					solutionRows.pop_back();
					solutionCols.pop_back();
					applyShape(currentShape, r, c);
				}

				//Solution has been found! "Unwinding" the recursive process
				else
					return true;
			}
		}
	}

	//The shape does not produce a solution in any (r, c), therefore a prior shape placement must have been incorrect
	//If currentShape == 0, no solution is possible with the inital board and shapes
	return false;
}

//Prints the solution on standard output
void Shifter::printSolution() const
{
	//Only print if there is a solution
	if(solutionRows.size() != 0)
	{
		for(size_t x = 0; x < shapes.size(); x++)
		{
			for(size_t y = 0; y < shapes[x].size(); y++)
				cout << shapes[x][y] << " ";

			cout << solutionRows[x] << " " << solutionCols[x] << endl;
		}
	}
}

//Function used for testing; prints shapes[index] on a single line
void Shifter::printShape(int index) const
{
	for(size_t x = 0; x < shapes[index].size(); x++)
		cout << shapes[index][x] << " ";
}

//Function used for testing; prints the current board state with a preceding label
void Shifter::printBoard(char *label) const
{
	cout << label << endl;
	for(size_t x = 0; x < board.size(); x++)
		cout << board[x] << endl;
	cout << endl;
}
