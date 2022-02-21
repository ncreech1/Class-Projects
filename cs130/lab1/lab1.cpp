//Lab 1
//COSC130
//Synopsis: Write a program that takes a left argument, an operation, and a right argument and returns the result
//Nicholas Creech
//1/11/19

#include <iostream>
#include <string>
using namespace std;

int CharToInt(char v);
char IntToChar(int v);
int StringToInt(string val);
string IntToString(int val);

int main(int argc, char* argv[])
{
	string sresult;
	int left;
	int right;
	char op;

	if(argc != 4)
	{
		printf("Usage: %s <left> <op> <right>\n", argv[0]);
		return -1;
	}

	left = StringToInt(argv[1]);
	right = StringToInt(argv[3]);
	op = argv[2][0];

	switch(op)
	{
		case 'x':
			sresult = IntToString(left * right);
			break;
		case '/':
			sresult = IntToString(left / right);
			break;
		case '+':
			sresult = IntToString(left + right);
			break;
		case '-':
			sresult = IntToString(left - right);
			break;
		case '%':
			sresult = IntToString(left % right);
			break;
		default:
			sresult = IntToString(left);
			break;
	}

	printf("%d %c %d = %s\n", left, op, right, sresult.c_str());

	return 0;
}

int CharToInt(char v)
{
	return v - '0';
}

char IntToChar(int v)
{
	return v + '0';
}

int StringToInt(string val)
{
	int negative = 1; //Holds the negative sign if needed

	if(val[0] == '-')
		negative = -1;

	int num = 0;
	for(char c : val)
	{
		if(c != '-')
			num = (num * 10) + CharToInt(c);
	}

	return num * negative;
}

string IntToString(int val)
{
	string negative = ""; //Holds the negative sign if needed
	if(val < 0)
	{
		negative = "-";
		val *= -1;
	}

	string resultB = ""; //Backwards result
	string resultF = ""; //Forwards result

	do
	{
		char digit = IntToChar(val % 10);
		resultB += digit;

		val /= 10;
	}

	while(val > 0);

	resultF += negative; //Add negative sign (if any)

	for(int x = resultB.size() - 1; x >= 0; x--)
		resultF += resultB[x];

	return resultF;
}
