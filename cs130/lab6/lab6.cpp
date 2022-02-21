/* Nicholas Creech
 * CS130
 * 2/20/19
 * Lab6: Make a program that takes an encrypted file, decrypts it, and prints the contents 
 */

#include <stdio.h>
#include <iostream>

using namespace std;

int main(int argc, char * argv[])
{
	//Need four params
	if(argc != 4)
	{
		printf("Usage: ./lab6 <file name> <data key> <index key>\n");
		return -1;
	}

	char * data;
	FILE * fin = nullptr;
	fin = fopen(argv[1], "rb");
	char dkey;
	int nkey;

	//Convert dkey param to a char and nkey to an int
	sscanf(argv[2], "%c", &dkey);
	sscanf(argv[3], "%d", &nkey);

	//Make sure file opened successfully
	if(fin == nullptr)
	{
		printf("Error opening file '%s'\n", argv[1]);
		return -2;
	}

	//Find total number of bytes in binary file
	fseek(fin, 0, SEEK_END);
	int fileSize = ftell(fin);
	int totalChars = fileSize / 8;

	//Dynamically allocate memory for the characters
	data = new char[totalChars];
	fseek(fin, 0, SEEK_SET);

	for(int x = 0; x < totalChars; x++)
	{
		char block;
		int index;
		fread(&block, 1, 1, fin); //Read char (1 byte)
		fseek(fin, 3, SEEK_CUR); //Skip the padding (3 bytes)
		fread(&index, 1, 4, fin); //Read index (4 bytes)
		block ^= dkey; //Decrypt char
		index ^= nkey; //Decrypt index

		//Make sure decoded index is within the bounds of the c-string
		if(index < totalChars && index >= 0)
			data[index] = block; //Add char to correct index in c-string
		else
		{
			printf("Error decoding chunk %d, decoded index %d, but range is (0, %d).\n", x, index, totalChars - 1);
			return -3;
		}
	}

	//Print final, decoded c-string
	printf("%s\n", data);

	fclose(fin);
	delete[] data;

	return 0;
}
