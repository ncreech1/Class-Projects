#include <iostream>
#include <sstream>

using namespace std;

/* Nicholas Creech
 * 9/7/19
 * Description: Write a program that takes a pgm file and outputs a new pgm file, all white pixels, with the same number of rows and columns
 */

int main(int argc, char** argv)
{
	int rows = 0, columns = 0;

	if(argc != 3)
	{
		cerr << "usage: bigwhite rows cols\n";
		return 1;
	}

	//Use stringstream to extract ints
	istringstream ss;
	
	//Read in rows
	ss.str(argv[1]);
	ss >> rows;

	if(!ss || rows <= 0)
	{
		cerr << "usage: bigwhite rows cols\n";
		return 1;
	}

	//Read in columns
	ss.clear();
	ss.str(argv[2]);
	ss >> columns;

	if(!ss || columns <= 0)
	{
		cerr << "usage: bigwhite rows cols\n";
		return 1;
	}

	//Print
	cout << "P2" << endl << columns << " " << rows << endl << 255 << endl;

	for(int x = 0; x < rows * columns; x++)
		cout << 255 << endl;

	return 0;
}
