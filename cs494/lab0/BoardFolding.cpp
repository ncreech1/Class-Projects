#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>

using namespace std;

/* Nicholas Creech
 * CS494 Lab0
 * 09/05/21
 */ 

class BoardFolding
{
	public:
		vector<int> rowIDs; //The rows in the grid as unique IDs
		vector<int> startingPlaces(vector<string> &grid);
		long long howMany(int r, int c, vector<string> &grid);
		int toNumber(char c);
};

//Returns the number of final states of the paper after folding
long long BoardFolding::howMany(int r, int c, vector<string> &grid)
{
	string currentRow;
	map<string, int> rowIDMap;
	map<string, int>::iterator mit;
	int nextRowID;
	int xwCombos, yhCombos;
	vector<string> ucGrid; //The uncompressed grid
	vector<string> ucGrid90; //The uncompressed grid rotated 90 degrees
	vector<int> startPlacesRow;
	vector<int> endPlacesRow;
	vector<int> startPlacesCol;
	vector<int> endPlacesCol;

	//Convert compressed format to vector of binary row strings
	nextRowID = 0;
	for(int i =	0; i < r; i++)
	{
		currentRow = "";

		for(int j = 0; j < c; j++)
			currentRow += to_string((toNumber(grid[i][j / 6]) >> (j % 6)) % 2);   	

		ucGrid.push_back(currentRow);	
		mit = rowIDMap.find(currentRow);
		
		//Add rows to map with unique ID (if not already in map)
		if(mit == rowIDMap.end())
		{
			rowIDMap[currentRow] = nextRowID;
			rowIDs.push_back(nextRowID);
			nextRowID++;
		}

		else
			rowIDs.push_back(mit->second);
	}

	//Calculate starting and ending places for rows
	startPlacesRow = startingPlaces(ucGrid);
	reverse(ucGrid.begin(), ucGrid.end());
	endPlacesRow = startingPlaces(ucGrid);
	reverse(endPlacesRow.begin(), endPlacesRow.end());
	reverse(ucGrid.begin(), ucGrid.end());

	//Populate the 90 degree grid so it can be transposed
	string defaultRow(r, '0');
	ucGrid90.resize(c, defaultRow);

	//Transpose the original grid 90 degrees clockwise
	for(int i = 0; i < r; i++)
	{
		for(int j = 0; j < c; j++)
		{
			ucGrid90[j][r - 1 - i] = ucGrid[i][j];	
		}
	}

	//Add columns to map with unique ID (if not already in map)
	rowIDs.clear();
	for(int i = 0; i < ucGrid90.size(); i++)
	{
		mit = rowIDMap.find(ucGrid90[i]);

		if(mit == rowIDMap.end())
		{
			rowIDMap[ucGrid90[i]] = nextRowID;
			rowIDs.push_back(nextRowID);
			nextRowID++;
		}

		else
			rowIDs.push_back(mit->second);
	}

	//Calculate starting and ending places for columns
	startPlacesCol = startingPlaces(ucGrid90);
	reverse(ucGrid90.begin(), ucGrid90.end());
	endPlacesCol = startingPlaces(ucGrid90);
	reverse(endPlacesCol.begin(), endPlacesCol.end());

	//Calculate the [x, w] combinations with positive height
	xwCombos = 0;
	for(int x = 0; x < startPlacesRow.size(); x++)
	{
		if(startPlacesRow[x] == 1)
		{
			for(int y = x + 1; y < endPlacesRow.size(); y++)
			{
				if(endPlacesRow[y] == 1)
					xwCombos++;
			}
		}
	}

	//Calculate the [y, h] combinations with positive height
	yhCombos = 0;
	for(int x = 0; x < startPlacesCol.size(); x++)
	{
		if(startPlacesCol[x] == 1)
		{
			for(int y = x + 1; y < endPlacesCol.size(); y++)
			{
				if(endPlacesCol[y] == 1)
					yhCombos++;
			}
		}
	}
 
	return xwCombos * yhCombos;
}

//Returns a vector of valid starting or ending rows at each step of folding
vector<int> BoardFolding::startingPlaces(vector<string> &grid)
{
	vector<int> places;
	bool startingRow;

	//First row is always a starting place
	places.resize(grid.size() + 1, 0);
	places[0] = 1;

	//Check the rectangles of width w above and below each j
	for(int j = 0; j < grid.size(); j++)
	{
		startingRow = false;

		for(int w = 1; w <= j; w++)
		{
			//If the rectangles up to this w match and row (j - w) is
			//a starting row, j is also a starting row
			if(rowIDs[j - w] == rowIDs[j + w - 1] && places[j - w] == 1)
			{
				startingRow = true;
				break;
			}

			//j is not a starting row because the rectangles do not match
			else if(rowIDs[j - w] != rowIDs[j + w - 1])
				break;
		}

		if(startingRow)
			places[j] = 1;
	}

	return places;
}

//Converts a compressed character to an integer from 0 to 63
int BoardFolding::toNumber(char c)
{
	if(c == '@')
		return 63;
	else if(c == '#')
		return 62;
	else if(c >= 65 && c <= 90)
		return c - 29;
	else if(c >= 97 && c <= 122)
		return c - 87;
	else
		return c - '0';
}
