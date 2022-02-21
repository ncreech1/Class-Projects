/* Nicholas Creech
 * 2/7/19
 * Pre-lab5
 */ 

#include <cstdio>
#include <vector>
#include <sstream>
#include <string>

using namespace std;

int main()
{
	FILE * fout;

	fout = fopen("myfile.txt", "rb");
	vector<string> parts;

	if(fout == nullptr)
	{
		perror("myfile.txt");
		printf("Unable to open file.\n");
		return -1;
	}

	fseek(fout, 0, SEEK_END);

	int offset = 0;
	string currentPart = "";
	while(fseek(fout, offset, SEEK_CUR) == 0)
	{
		char * currentChar;
		fseek(fout, offset, SEEK_CUR);

		fread(currentChar, 1, sizeof(currentChar), fout);
		
		if(*currentChar == '~')
		{
			parts.push_back(currentPart);
			currentPart = "";
		}
		else
			currentPart += currentChar;

		offset--;
	}

	for(int x = 0; x < parts.size(); x++)
	{
		printf("Entry %d:\n%s", (x + 1), parts.at(x)); 
	}

	return 0;
}
