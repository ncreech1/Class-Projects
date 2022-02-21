#include <iostream>
#include <cstdio>
#include <vector>

using namespace std;

/* Nicholas Creech
 * 9/17/19
 * Description: Write a program that takes a pgm file and outputs the same pgm file but flipped horizontally
 */ 

int main()
{
	string p2 = "";
	int rows = 0, columns = 0;
	int begin255 = 0;
	vector<int> pixels;

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

		pixels.push_back(pixel);
	}

	string extra = "";
	cin >> extra;

	//Check for extra stuff after pixels
	if(cin)
	{
		cerr << "Bad PGM file -- Extra stuff after the pixels\n";
		return 1;
	}

	printf("P2\n%d %d\n255\n", columns, rows);

	for(int x = 0; x < pixels.size(); x++)
	{
		//At the end of each row, print the row in reverse
		if((x + 1) % columns == 0)
		{
			for(int i = x; i >= x - (columns - 1); i--)
			{
				printf("%d\n", pixels[i]);
			}
		}
	}

	return 0;
}
