#include "pgm.hpp"
#include <cstdio>
#include <cstdlib>
#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <fstream>
using namespace std;

/* Nicholas Creech
 * 9/15/19
 * Lab 3: Create a set of functions that manipulate PGM files using the provided pgm.hpp header file
 */ 

bool Pgm::Read(const std::string &file)
{
  ifstream fin;
  string s;
  size_t i, j, r, c, v;

  fin.open(file.c_str());
  if (fin.fail()) return false;
  
  if (!(fin >> s)) return false;
  if (s != "P2") return false;
  if (!(fin >> c) || c <= 0) return false;
  if (!(fin >> r) || r <= 0) return false;
  if (!(fin >> i) || i != 255) return false;

  Pixels.resize(r);
  for (i = 0; i < r; i++) {
    Pixels[i].clear();
    for (j = 0; j < c; j++) {
      if (!(fin >> v) || v > 255) return false;
      Pixels[i].push_back(v);
    }
  }
  if (fin >> s) return false;
  fin.close();
  return true;
}

bool Pgm::Write(const string& file) const
{
	if(Pixels.size() == 0 || Pixels[0].size() == 0)
		return false;

	ofstream fout;
	size_t rows = Pixels.size();
	size_t cols = Pixels[0].size();

	fout.open(file.c_str());
	if(fout.fail()) return false;

	//Begin creating pgm format
	fout << "P2\n";
	fout << cols << " " << rows << endl;
	fout << "255\n";
	
	int count = 0;
	for(size_t i = 0; i < rows; i++)
	{
		for(size_t j = 0; j < cols; j++)
		{
			count++;

			if(count != 1 && (i != 0 || j != 0))
				fout << " ";
			
			fout << Pixels[i][j];

			//Only 20 pixels per line
			if(count == 20)
			{
				fout << "\n";
				count = 0;
			}
		}		
	}

	if(count != 0)
		fout << "\n";
	
	fout.close();

	return true;
}

bool Pgm::Create(size_t r, size_t c, size_t pv)
{
	if(pv > 255)
		return false;

	Pixels.resize(r);
	for(size_t i = 0; i < r; i++)
	{
		Pixels[i].clear();
		for(size_t j = 0; j < c; j++)
			Pixels[i].push_back(pv);
	}

	return true;
}

bool Pgm::Clockwise()
{
	if(Pixels.size() == 0 || Pixels[0].size() == 0)
		return false;

	size_t rows = Pixels.size();
	size_t cols = Pixels[0].size();

	vector< vector<int> > newPixels;
	newPixels.resize(cols);

	//Bottom left pixel becomes first pixel of new image
	for(size_t j = 0; j < cols; j++)
	{
		for(int i = rows - 1; i >= 0; i--)
			newPixels[j].push_back(Pixels[i][j]);
	}

	Pixels = newPixels;

	return true;
}

bool Pgm::Cclockwise()
{
	if(Pixels.size() == 0 || Pixels[0].size() == 0)
		return false;

	size_t rows = Pixels.size();
	size_t cols = Pixels[0].size();

	vector< vector<int> > newPixels;
	newPixels.resize(cols);
	
	//Top right pixel becomes first pixel of new image
	for(int j = cols - 1; j >= 0; j--)
	{
		for(size_t i = 0; i < rows; i++)
		{
			newPixels[(cols - 1) - j].push_back(Pixels[i][j]);
		}
	}

	Pixels = newPixels;

	return true;
}

bool Pgm::Pad(size_t w, size_t pv)
{	
	if(Pixels.size() == 0 || Pixels[0].size() == 0)
		return false;
	if(pv > 255)
		return false;

	size_t origRows = Pixels.size();
	size_t origCols = Pixels[0].size();

	//Add padding to columns
	for(size_t x = 0; x < Pixels.size(); x++)
		Pixels[x].resize(Pixels[x].size() + 2 * w, pv);
	
	//Add padding to rows
	vector<int> padding (Pixels[0].size(), pv);
	Pixels.resize(Pixels.size() + 2 * w, padding);
	
	//Move pixels to center of new rows * columns size
	for(int j = origCols - 1; j >= 0; j--)
	{
		for(int i = origRows - 1; i >= 0; i--)
		{
			Pixels[i + w][j + w] = Pixels[i][j]; 
			Pixels[i][j] = pv;
		}
	}

	return true;
}

bool Pgm::Panel(size_t r, size_t c)
{
	if(Pixels.size() == 0 || Pixels[0].size() == 0)
		return false;
	if(r == 0 || c == 0)
		return false;

	size_t rows = Pixels.size();
	size_t cols = Pixels[0].size();
	vector< vector<int> > newPixels;
	newPixels.resize(r * rows);

	int rowIndex = 0;
	int colIndex = 0; 
	for(size_t i = 0; i < r * rows; i++)
	{
		for(size_t j = 0; j < c * cols; j++)
		{
			newPixels[i].push_back(Pixels[rowIndex][colIndex]);
			
			//Loop columns of reference image
			if((j + 1) % cols == 0)
				colIndex = 0;
			else 
				colIndex++;
		}

		//Loop rows of reference image
		if((i + 1) % rows == 0)
			rowIndex = 0;
		else
			rowIndex++;
	}

	Pixels = newPixels;

	return true;
}

bool Pgm::Crop(size_t r, size_t c, size_t rows, size_t cols)
{
	if(Pixels.size() == 0 || Pixels[0].size() == 0)
		return false;
	if(r + (rows - 1) >= Pixels.size() || c + (cols - 1) >= Pixels[0].size())
		return false;

	vector< vector<int> > newPixels;
	newPixels.resize(rows);

	int rowIndex = 0;
	for(size_t i = r; i < r + rows; i++)
	{
		for(size_t j = c; j < c + cols; j++)
			newPixels[rowIndex].push_back(Pixels[i][j]);

		rowIndex++;
	}

	Pixels = newPixels;

	return true;
}        
