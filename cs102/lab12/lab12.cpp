#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <sstream>
#include <vector>

using namespace std;

/* Nicholas Creech
 * 11/12/18
 * Lab12: Create a program that reads a image from cin or a ppm file, manipulates it, and then outputs to either cout or another file
 */

struct Pixel
{
	unsigned int r, g, b;
	int colorsFound;
};

class Picture
{
public:
	Picture();
	Pixel get_pixel(unsigned int, unsigned int) const;
	Pixel& get_pixel(unsigned int, unsigned int);
	void set_pixel(unsigned int, unsigned int, const Pixel&);
	void invert();
	void flip_x();
	void flip_y();
	bool read_input(istream&);
	void write_output(ostream&);
private:
	vector<Pixel> pixels;
	unsigned int width, height;
	unsigned int maxIntensity;
};

int main(int argc, char * argv[])
{
	if(argc > 4 || argc < 3)
		cout << "Usage: ./lab12 <input or '-'> <output or '-'> <I or X or Y>\n"; 
	else
	{
		string inputName = argv[1];
		string outputName = argv[2];
		string manip = "";

		if(argc == 4)
			manip = argv[3];

		Picture ppm;
		bool inputRead = false;

		if(inputName != "-") //Read ppm from file
		{
			ifstream in(inputName);
			inputRead = ppm.read_input(in);
			in.close();
		}

		else //Read ppm from cin
			inputRead = ppm.read_input(cin);

		if(inputRead) //Input read successfully
		{
			if(manip != "") //Manipulate the ppm file
			{
				if(manip == "I")
					ppm.invert();
				else if(manip == "Y")
					ppm.flip_y();
				else if(manip == "X")
					ppm.flip_x();
			}

			if(outputName == "-") //Output to cout
				ppm.write_output(cout);
			else
			{
				ofstream out(outputName);
				ppm.write_output(out); //Output to file
			}
		}

		else
			cout << "Unable to read input file '" << argv[1] << "'. Make sure there are [width * height] pixels.\n";
	}

	return 0;
}

Picture::Picture()
{
	width = 0;
	height = 0;
	maxIntensity = 0;
}

bool Picture::read_input(istream& in)
{
	if(in)
	{
		string currentLine;
		unsigned int requiredParams = 0; //Number of required params in file found (P3, width + height, maxIntensity)
		unsigned int currentPixel = 0; //The current pixel being set (persists through each line read in case rgb values are split up)

		//Check each line in the input ppm file
		while(getline(in, currentLine))
		{
			stringstream ss;

			//Check each character to remove potential white space
			for(unsigned int x = 0; x < currentLine.size(); x++)
			{
				if(currentLine.at(x) != ' ' || (currentLine.at(x) == ' ' && x != currentLine.size() - 1 && currentLine.at(x + 1) != ' '))
					ss << currentLine.at(x);
			}

			//If line starts with '#', skip it (comment)
			if(ss.str().front() == '#' && requiredParams != 0)
				continue;
			else if(ss.str().front() == '#' && requiredParams == 0) //P3 must be first, not a comment; error
			{
				cout << "Error: Input file must start with 'P3' to designate a PPM file.\n";
				return false;
			}

			//Split the formatted line by spaces
			vector<string> values;
			string piece;
			while(getline(ss, piece, ' '))
				values.push_back(piece);

			if(values.size() == 1 && values.at(0) == "P3" && requiredParams == 0) //Found P3 first; check for next required param
				requiredParams++;
			else if(requiredParams == 0) //P3 not first; error
			{
				cout << "Error: Input file must start with 'P3' to designate a PPM file.\n";
				return false;
			}

			else
			{
				if(values.size() == 2 && requiredParams == 1) //Found with + height second (after potential comments); check for next required param
				{
					width = stoi(values.at(0));
					height = stoi(values.at(1));
					pixels.reserve(width * height);
					requiredParams++;
				}

				else if(requiredParams == 1) //Width + height not second (after potential comments); error
				{
					cout << "Error: Width and height must follow 'P3' in input file.\n";
					return false;
				}

				else if(values.size() == 1 && requiredParams == 2) //Found max intensity third (after potential comments); begin assigning pixels
				{
					maxIntensity = stoi(values.at(0));
					requiredParams++;
				}

				else if(requiredParams == 2) //Max intensity not third; error
				{
					cout << "Error: Max intensity must follow width and height in input file.\n";
					return false;
				}

				else
				{
					//Set current pixel
					if(values.size() > 0)
					{
						unsigned int currentValue = 0; //Keeps track of the current value so a for-each loop can still be used for better readability
						
						for(string color : values) //Check each value on the current line and assign it to a new pixel
						{	
							//Create pixel object
							if(pixels.size() - 1 != currentPixel)
							{
								Pixel newPix = {0, 0, 0, 0};
								pixels.push_back(newPix);
							}

							//Set color values
							if(pixels.at(currentPixel).colorsFound == 0)
								pixels.at(currentPixel).r = stoi(color);
							else if(pixels.at(currentPixel).colorsFound == 1)
								pixels.at(currentPixel).g = stoi(color);
							else if(pixels.at(currentPixel).colorsFound == 2)
								pixels.at(currentPixel).b = stoi(color);

							//Max color value = maxIntensity
							if(pixels.at(currentPixel).r > maxIntensity)
							{
								cout << "Warning: Pixel over max intensity (" << pixels.at(currentPixel).r << " > " << maxIntensity << ")\n";
								pixels.at(currentPixel).r = maxIntensity; 
							}

							if(pixels.at(currentPixel).g > maxIntensity)
							{
								cout << "Warning: Pixel over max intensity (" << pixels.at(currentPixel).g << " > " << maxIntensity << ")\n";
								pixels.at(currentPixel).g = maxIntensity;
							}

							if(pixels.at(currentPixel).b > maxIntensity)
							{
								cout << "Warning: Pixel over max intensity (" << pixels.at(currentPixel).b << " > " << maxIntensity << ")\n";
								pixels.at(currentPixel).b = maxIntensity;
							}

							//Tells this pixel that another color value has been found
							pixels.at(currentPixel).colorsFound++;

							//All color values found; go to next pixel
							if(pixels.at(currentPixel).colorsFound == 3)
								currentPixel++;

							currentValue++;
						}
					}
				}
			}
		}

		//Check if the amount of pixels in the ppm file matches the file's specified width * height
		if(currentPixel - 1 == ((width * height) - 1))
			return true;
		else
			return false;
	}

	return false;
}

void Picture::write_output(ostream& out)
{
	out << "P3\n";
	out << width << " " << height << "\n";
	out << maxIntensity << "\n";

	//Output each pixel value (one pixel per line)
	for(Pixel p : pixels)
		out << p.r << " " << p.g << " " << p.b << "\n";
}

Pixel Picture::get_pixel(unsigned int row, unsigned int column) const
{
	return pixels.at((row * width) + column);
}

Pixel& Picture::get_pixel(unsigned int row, unsigned int column)
{
	return pixels.at((row * width) + column);
}

void Picture::set_pixel(unsigned int row, unsigned int column, const Pixel& p)
{
	get_pixel(row, column) = p;
}

void Picture::invert()
{
	for(unsigned int x = 0; x < width; x++)
	{
		for(unsigned int y = 0; y < height; y++)
		{
			Pixel currentPix = get_pixel(y, x);
			Pixel newPix = {maxIntensity - currentPix.r, maxIntensity - currentPix.g, maxIntensity - currentPix.b, 0};
			set_pixel(y, x, newPix);
		}
	}
}

void Picture::flip_x()
{
	for(unsigned int x = 0; x < width; x++)
	{
		for(unsigned int y = 0; y < height / 2; y++)
		{
			Pixel currentPix = get_pixel(y, x);
			Pixel otherPix = get_pixel((height - 1) - y, x);
			set_pixel(y, x, otherPix);
			set_pixel((height - 1) - y, x, currentPix);
		}
	}
}

void Picture::flip_y()
{
	for(unsigned int x = 0; x < width / 2; x++)
	{
		for(unsigned int y = 0; y < height; y++)
		{
			Pixel currentPix = get_pixel(y, x);
			Pixel otherPix = get_pixel(y, (width - 1) - x);
			set_pixel(y, x, otherPix);
			set_pixel(y, (width - 1) - x, currentPix);
		}
	}
}
