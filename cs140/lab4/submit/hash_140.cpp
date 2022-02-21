#include "hash_140.hpp"
#include <sstream>
#include <cstdio>
#include <iostream>

using namespace std;

/* Nicholas Creech
 * 9/21/19
 * Lab 4: Write a program that creates hash tables using Last7 and XOR hashing, and Linear and Double collision resolution
 */

int Last7(const string&);
int XOR(const string&);

string Hash_140::Set_Up(size_t table_size, const std::string &fxn, const std::string &collision)
{
	if(Keys.size() != 0)
		return "Hash table already set up";
	if(table_size == 0)
		return "Bad table size";
	if(fxn != "Last7" && fxn != "XOR")
		return "Bad hash function";
	if(collision != "Linear" && collision != "Double")
		return "Bad collision resolution strategy";

	Keys.resize(table_size);
	Vals.resize(table_size);

	Fxn = (fxn == "Last7" ? 'L' : 'X');
	Coll = (collision == "Linear" ? 'L' : 'D');

	return "";
}

string Hash_140::Add(const string &key, const string &val)
{
	if(Keys.size() == 0)
		return "Hash table not set up";
	if(key == "")
		return "Empty key";

	for(size_t x = 0; x < key.size(); x++)
	{
		if(key[x] < 48 || (key[x] > 57 && key[x] < 65) || (key[x] > 70 && key[x] < 97) || key[x] > 102)
			return "Bad key (not all hex digits)";
	}

	if(val == "")
		return "Empty val";

	for(size_t x = 0; x < Keys.size(); x++)
	{
		if(Keys[x] == "")
			break;
		else if(x == Keys.size() - 1)
			return "Hash table full";
	}
	
	int hexKey;

	//Create hash from key
	if(Fxn == 'L')
		hexKey = Last7(key);
	else if(Fxn == 'X')
		hexKey = XOR(key);
	
	//Attempt to add; if it fails, try collision resolution
	if(Keys[hexKey % Keys.size()] == "")
	{
		Keys[hexKey % Keys.size()] = key;
		Vals[hexKey % Keys.size()] = val;
	}

	else
	{
		if(Coll == 'L')
		{
			for(size_t x = 0; x < Keys.size(); x++)
			{
				int index = (hexKey + x) % Keys.size();
				if(Keys[index] == "")
				{
					Keys[index] = key;
					Vals[index] = val;
					break;
				}

				else if(x == Keys.size() - 1)
					return "Cannot insert key";
				else if(Keys[index] == key)
					return "Key already in the table";
			}
		}

		else if(Coll == 'D')
		{
			for(size_t x = 0; x < Keys.size(); x++)
			{
				//H2 is the unused hash function for the current setup
				int H2 = (Fxn == 'L' ? XOR(key) % Keys.size() : Last7(key) % Keys.size());
				if(H2 == 0) H2 = 1; //H2 cannot have a result of 0
				int index = (hexKey + H2 * x) % Keys.size();
				if(Keys[index] == "")
				{
					Keys[index] = key;
					Vals[index] = val;
					break;
				}

				else if(x == Keys.size() - 1)
					return "Cannot insert key";
				else if(Keys[index] == key)
					return "Key already in the table";
			}
		}
	}

	return "";
}

string Hash_140::Find(const string &key)
{
	if(Keys.size() == 0)
		return "";
	
	for(size_t x = 0; x < key.size(); x++)
	{
		if(key[x] < 48 || (key[x] > 57 && key[x] < 65) || (key[x] > 70 && key[x] < 97) || key[x] > 102)
			return "";
	}

	int hexKey;

	//Create hash from key
	if(Fxn == 'L')
		hexKey = Last7(key);
	else if(Fxn == 'X')
		hexKey = XOR(key);
	
	//Attempt to find key; if it fails, try collision resolution
	if(Keys[hexKey % Keys.size()] == key)
		return Vals[hexKey % Keys.size()];
	else
	{
		if(Coll == 'L')
		{
			for(size_t x = 0; x < Keys.size(); x++)
			{
				int index = (hexKey + x) % Keys.size();
				if(Keys[index] == key)
					return Vals[index];
				else
					Nprobes++;
			}
		}

		else if(Coll == 'D')
		{
			for(size_t x = 0; x < Keys.size(); x++)
			{
				int H2 = (Fxn == 'L' ? XOR(key) % Keys.size() : Last7(key) % Keys.size());
				if(H2 == 0) H2 = 1;
				int index = (hexKey + H2 * x) % Keys.size();
				if(Keys[index] == key)
					return Vals[index];
				else
					Nprobes++;
			}
		}
	}


	return "";
}

void Hash_140::Print() const
{
	if(Keys.size() != 0)
	{
		for(size_t x = 0; x < Keys.size(); x++)
		{
			if(Keys[x] != "")
				printf("%5d %s %s\n", (int)x, Keys[x].c_str(), Vals[x].c_str());
		}
	}
}

size_t Hash_140::Total_Probes()
{
	int total = 0;

	if(Keys.size() == 0)
		return 0;

	//Call find and add Nprobes to total probes
	for(size_t x = 0; x < Keys.size(); x++)
	{
		Nprobes = 0;

		if(Keys[x] != "")
		{
			Find(Keys[x]);
			total += Nprobes;
		}
	}

	return total;
}

//Uses last 7 chars as new hash key
int Last7(const string& key)
{
	int hexKey;
	stringstream ss;

	if(key.size() <= 7)
		ss.str(key);
	else
		ss.str(key.substr(key.size() - 7));

	ss >> hex >> hexKey;

	return hexKey;
}

//Splits key into 7 char pieces and XORs each piece to form new hash key
int XOR(const string& key)
{
	int hexKey;
	stringstream ss;

	//Split into groups of 7 chars
	vector<int> pieces;
	string piece = "";
	for(size_t x = 0; x < key.size(); x++)
	{
		piece += key[x];
		if((x + 1) % 7 == 0 || x == key.size() - 1)
		{
			int hex7;
			ss.str(piece);
			ss >> hex >> hex7;
			pieces.push_back(hex7);
			piece = "";
			ss.clear();
		}
	}

	//XOR each piece
	hexKey = pieces[0];
	for(size_t x = 1; x < pieces.size(); x++)
		hexKey = hexKey ^ pieces[x];
	
	return hexKey;
}
