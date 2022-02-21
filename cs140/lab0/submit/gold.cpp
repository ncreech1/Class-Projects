#include <iostream>

using namespace std;

int main()
{
	char c;
	int total = 0;

	while(cin >> c)
	{
		if(c >= 'A')
		{
			total += (c - 64);
		}
	}

	cout << total << endl;

	return 0;
}
