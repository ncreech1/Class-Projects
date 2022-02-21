#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cstdio>
#include <algorithm>
#include "bitmatrix.hpp"

using namespace std;

/* Nicholas Creech
 * 10/6/19
 * Lab 5: Create a program that creates, manipulates, and stores bitmatrices (file, stdout, or hash table)
 */ 

unsigned int djb_hash(const string &s);
Bitmatrix* Identity(const Bitmatrix* bm);

//Bitmatrix constructor to create an empty matrix
Bitmatrix::Bitmatrix(int rows, int cols)
{
	string empty = "";
	if(rows <= 0)
		throw((string)"Bad rows");
	if(cols <= 0)
		throw((string)"Bad cols");

	M.resize(rows);

	for(int x = 0; x < cols; x++)
		empty += '0';
	for(int x = 0; x < rows; x++)
		M[x] = empty; 
}
 
//Bitmatrix constructor to create a matrix from a file
Bitmatrix::Bitmatrix(const string &fn)
{
	M.clear();

	ifstream fin;
	fin.open(fn.c_str());

	if(fin.fail())
		throw((string)"Can't open file");
	
	string line = ""; //The current line read straight from the file
	string finalLine = ""; //The revised line with all white space removed
	while(getline(fin, line))
	{
		//Remove white space
		for(size_t x = 0; x < line.size(); x++)
		{
			if(line[x] != ' ')
				finalLine += line[x];
		}

		//Add new row to matrix
		if(finalLine != "")
			M.push_back(finalLine);
		finalLine = "";
	}

	//If EOF flag not set, something went wrong reading the file
	if(!fin.eof())
		throw((string)"Bad file format");
}
 
//Copies a matrix and returns a pointer to the copy
Bitmatrix *Bitmatrix::Copy() const
{
	Bitmatrix* bm = new Bitmatrix(M.size(), M[0].size());
	bm->M = M;
	return bm;
}
 
//Writes the currently stored bitmatrix to a file
bool Bitmatrix::Write(const string &fn) const
{
	ofstream fout;
	fout.open(fn.c_str());
	
	if(fout.fail())
		return false;

	for(size_t x = 0; x < M.size(); x++)
		fout << M[x] << endl;

	//If the good flag is set, nothing went wrong reading the file
	if(fout.good())
		return true;
	else
		return false;
}
 
//Prints the currently stored bitmatrix to standard output
void Bitmatrix::Print(size_t w) const
{
	for(size_t x = 0; x < M.size(); x++)
	{
		int charCount = 1; //Amount of character printed on the current line
		for(size_t y = 0; y < M[x].size(); y++)
		{
			printf("%c", M[x][y]);
			
			//Print a space every w characters in a line
			if(w != 0 && charCount % w == 0 && y != M[x].size() - 1)
				printf(" ");
			charCount++;
		}

		printf("\n");

		//Print a space every w rows
		if(w != 0 && (x + 1) % w == 0 && x != M.size() - 1)
			printf("\n");
	}
}

//Writes the currently stored bitmatrix to a new PGM file 
bool Bitmatrix::PGM(const string &fn, int p, int border) const
{
	if(p <= 0 || border < 0)
		return false;

	ofstream fout;
	fout.open(fn.c_str());

	if(fout.fail())
		return false;

	//Total width and height of the PGM image including p * p squares and border
	int imageWidth = (M[0].size() * p) + (border * 2 + border * (M[0].size() - 1));
	int imageHeight = (M.size() * p) + (border * 2 + border * (M.size() - 1));

	fout << "P2\n" << imageWidth << " " << imageHeight << "\n255\n";
	

	for(size_t x = 0; x <= M.size(); x++)
	{
		//Create row border (done each time a new row starts)
		for(int b = 0; b < border; b++)
		{
			for(int z = 0; z < imageWidth; z++)
			{
				fout << 0;

				if(z != imageWidth - 1)
					fout << " ";
				else
					fout << "\n";
			}
		}
	
		//Are there still more rows?
		if(x != M.size())
		{
			//Copy the current row p times, creating a p * p square of each entry
			for(int pX = 0; pX < p; pX++)
			{
				for(size_t y = 0; y <= M[0].size(); y++)
				{					
					//Create column border (done each time a new column starts)
					for(int b1 = 0; b1 < border; b1++)
					{
						fout << 0;
						if(y < M[0].size() || b1 != border - 1)
							fout << " ";
						else
							fout << "\n";
					}	

					//Are there still more columns?
					if(y != M[0].size())
					{
						//Copy the current matrix entry p times across the row
						for(int pY = 0; pY < p; pY++)
						{
							if(M[x][y] == '0')
								fout << 255 << " ";
							else
								fout << 100 << " ";
						}
					}
				}
			}
		}
	}

	//No issues writing file
	if(fout.good())
		return true;

	return false;
}

//Returns the matrix rows
int Bitmatrix::Rows() const
{
	return M.size();
}
 
//Returns the matrix columns
int Bitmatrix::Cols() const
{
	return M[0].size();
}
 
//Returns the bit at (row, col)
char Bitmatrix::Val(int row, int col) const
{
	if(row > Rows() || row < 0 || col > Cols() || col < 0)
		return 'x';

	return M[row][col];
}
 
//Sets the bit at (row, col) to val
bool Bitmatrix::Set(int row, int col, char val)
{
	if(val != 0 && val != 1 && val != '0' && val != '1')
		return false;
	if(row < 0 || row > Rows() || col > Cols() || col < 0)
		return false;

	if(val == 1 || val == 0)
		val += '0';

	M[row][col] = val;
	return true;
}
 
//Swaps row r1 with r2
bool Bitmatrix::Swap_Rows(int r1, int r2)
{
	if(r1 < 0 || r1 > Rows() || r2 < 0 || r2 > Rows())
		return false;
	
	string r1s = M[r1];
	string r2s = M[r2];

	M[r1] = r2s;
	M[r2] = r1s;

	return true;
}
 
//Sets row r1 to r1 + r2
bool Bitmatrix::R1_Plus_Equals_R2(int r1, int r2)
{
	if(r1 < 0 || r1 > Rows() || r2 < 0 || r2 > Rows())
		return false;
	if(r1 == r2)
		return false;
	
	for(size_t x = 0; x < M[r1].size(); x++)
		M[r1][x] = (((M[r1][x] - '0') + (M[r2][x] - '0')) % 2) + '0';

	return true;
}
      
//Returns the two bitmatrices added together 
Bitmatrix *Sum(const Bitmatrix *a1, const Bitmatrix *a2)
{
	if(a1->Rows() != a2->Rows() || a1->Cols() != a2->Cols())
		return NULL;

	Bitmatrix* sum = a1->Copy();

	//Add each row of a1 with the corresponding row of a2
	for(int x = 0; x < a1->Rows(); x++)
	{
		for(int y = 0; y < a1->Cols(); y++)
			sum->Set(x, y, (((a1->Val(x, y) - '0') + (a2->Val(x, y) - '0')) % 2) + '0');
	}

	return sum;
}

//Returns the dot product of two bitmatrices
Bitmatrix *Product(const Bitmatrix *a1, const Bitmatrix *a2)
{
	if(a1->Cols() != a2->Rows())
		return NULL;

	Bitmatrix* prod = new Bitmatrix(a1->Rows(), a2->Cols());

	int colSum = 0, colIndex = 0;
	for(int x = 0; x < a1->Rows(); x++)
	{
		for(int y = 0; y < a1->Cols(); y++)
		{
			//Entries across each row in a1 are multiplied down each column of a2 and summed 
			colSum += (a1->Val(x, y) - '0') * (a2->Val(y, colIndex) - '0');

			//Reached end of column, set corresponding value in product bitmatrix and start next a2 column (or increment a1 row)
			if(y == a2->Rows() - 1)
			{
				prod->Set(x, colIndex, colSum % 2);

				colIndex++;
				colSum = 0;

				//If the current a1 row has NOT been multipled by all a2 columns, restart the row
				if(colIndex != a2->Cols())
					y = -1;
				
				//Start next row
				else
				{
					colIndex = 0;
					break;
				}
			}
		}
	}

	return prod;
}

//Returns a new matrix composed of the rows of a1 specified in the rows vector
Bitmatrix *Sub_Matrix(const Bitmatrix *a1, const vector <int> &rows)
{
	if(rows.size() == 0)
		return NULL;

	Bitmatrix* sub = new Bitmatrix(rows.size(), a1->Cols());

	for(size_t x = 0; x < rows.size(); x++)
	{
		if(rows[x] < 0 || rows[x] > a1->Rows())
			return NULL;
		
		for(int y = 0; y < a1->Cols(); y++)
			sub->Set(x, y, a1->Val(rows[x], y));
	}

	return sub;
}

//Returns the inverse bitmatrix of m
Bitmatrix *Inverse(const Bitmatrix *m)
{
	if(m->Rows() != m->Cols())
		return NULL;

	Bitmatrix* a1 = m->Copy();
	Bitmatrix* inv = Identity(a1);

	for(int i = 0; i < a1->Rows(); i++)
	{
		//Check if row needs to be swapped
		if(a1->Val(i, i) != '1')
		{
			for(int j = i + 1; j < a1->Rows(); j++)
			{
				//Swap found!
				if(a1->Val(j, i) == '1')
				{
					a1->Swap_Rows(i, j);
					inv->Swap_Rows(i, j);
					break;
				}
				
				//Matrix cannot be inverted
				else if(j >= a1->Rows() - 1)
					return NULL;
			}

			//Matrix cannot be inverted
			if(i + 1 >= a1->Rows())
				return NULL;
		}

		//Sum rows after row i
		for(int j = i + 1; j < a1->Rows(); j++)
		{
			if(a1->Val(j, i) == '1')
			{
				a1->R1_Plus_Equals_R2(j, i);
				inv->R1_Plus_Equals_R2(j, i);
			}
		}
	}

	//Sum rows again with new upper-triangular matrix
	for(int i = a1->Rows() - 2; i >= 0; i--)
	{
		for(int j = i + 1; j < a1->Rows(); j++)
		{
			if(a1->Val(i, j) == '1')
			{
				a1->R1_Plus_Equals_R2(i, j);
				inv->R1_Plus_Equals_R2(i, j);
			}
		}
	}

	return inv;
}

//Contructor that creates a new bitmatrix hash table
BM_Hash::BM_Hash(int size)
{
	if(size <= 0)
		throw("Bad size");
	Table.clear();
	Table.resize(size);
}

//Stores a bitmatrix in the hash table
bool BM_Hash::Store(const string &key, Bitmatrix *bm)
{
	unsigned int index, hashKey;
	HTE entry;
	entry.key = key;
	entry.bm = bm;

	hashKey = djb_hash(key);
	index = hashKey % Table.size();

	//Make sure the key has not already been stored
	for(size_t x = 0; x < Table[index].size(); x++)
	{
		if(Table[index][x].key == key)
			return false;
	}

	Table[index].push_back(entry);

	return true;
}
 
//Finds a bitmatrix in the hash table
Bitmatrix *BM_Hash::Recall(const string &key) const
{
	unsigned int index, hashKey;

	hashKey = djb_hash(key);
	index = hashKey % Table.size();

	//Look for bitmatrix at the calculated index
	for(size_t x = 0; x < Table[index].size(); x++)
	{
		if(Table[index][x].key == key)
			return Table[index][x].bm;
	}

	return NULL;
}

//Returns a vector of hash table entries in the order of the table
vector <HTE> BM_Hash::All() const
{
	vector <HTE> rv;

	for(size_t x = 0; x < Table.size(); x++)
	{
		for(size_t y = 0; y < Table[x].size(); y++)
			rv.push_back(Table[x][y]);
	}

	return rv;
}

//Returns the integer djb hash of a string
unsigned int djb_hash(const string &s)
{
	size_t i;
	unsigned int h;

	h = 5381;

	for(i = 0; i < s.size(); i++)
	{
		h = (h << 5) + h + s[i];
	}

	return h;
}

//Returns the identity matrix of a given bitmatrix
Bitmatrix* Identity(const Bitmatrix* bm)
{
	Bitmatrix* ibm = bm->Copy();
	for(int x = 0; x < ibm->Rows(); x++)
	{
		for(int y = 0; y < ibm->Cols(); y++)
		{
			//All other entries are 0
			if(ibm->Val(x, y) != '0')
				ibm->Set(x, y, '0');

			//Make ones on the diagonal
			if(x == y)
				ibm->Set(x, y, '1');
		}
	}

	return ibm;
}
