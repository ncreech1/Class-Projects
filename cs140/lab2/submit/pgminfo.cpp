#include <iostream>
#include <cstdio>

using namespace std;

/* Nicholas Creech
 * 9/7/19
 * Description: Write a program called pgminfo that takes a pgm file and displays the rows, columns, pixels, and average pixel value
 */

int main()
{
	string p2 = "";
	int rows = 0, columns = 0;
	int begin255 = 0;
	double average = 0;

	cin >> p2;

	//PGM file must start with P2
	if(p2 != "P2")
	{
		cerr << "Bad PGM file -- first word is not P2\n";
		return 1;
	}

	cin >> columns;

	//Columns must be a non-negative integer
	if(!cin || columns <= 0)
	{
		cerr << "Bad PGM file -- No column specification\n";
		return 1;
	}

	cin >> rows;

	//Rows must be a non-negative integer
	if(!cin || rows <= 0)
	{
		cerr << "Bad PGM file -- No row specification\n";
		return 1;
	}

	cin >> begin255;

	//255 must follow rows and columns before pixels
	if(!cin || begin255 != 255)
	{
		cerr << "Bad PGM file -- No 255 following the rows and columns\n";
		return 1;
	}

	//Read in all the pixels and add to average
	int pixel = 0;
	for(int x = 0; x < rows * columns; x++)
	{
		cin >> pixel;
		
		if(!cin || pixel < 0 || pixel > 255)
		{
			cerr << "Bad PGM file -- pixel " << x << " is not a number between 0 and 255\n";
			return 1;
		}

		average += pixel;
	}

	string extra = "";
	cin >> extra;

	//Check for extra stuff after pixels
	if(cin)
	{
		cerr << "Bad PGM file -- Extra stuff after the pixels\n";
		return 1;
	}

	printf("# Rows:    %8d\n", rows);
	printf("# Columns: %8d\n", columns);
	printf("# Pixels:  %8d\n", rows * columns);
	printf("Avg Pixel: %8.03f\n", average / (rows * columns));

	return 0;
}
