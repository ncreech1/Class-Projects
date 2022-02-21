/*
 * Nicholas Creech
 * 9/5/18
 * Lab3: A calculator that takes a left and right operand and an operation
 */

#include <iostream>
#include <string>

using namespace std;

int main()
{
	//Op = operation
	double leftOp, rightOp;
	char op;
	int opType = 0;
	
	cout << "Enter left operand: ";

	//Check if left operation is the correct type
	if(cin >> leftOp)
	{
		cout << "Enter operation: ";
		cin >> op;
		
		//Check if the operation is valid
		switch(op)
		{
			case '+':
			case 'A':
			case 'a':
				opType = 1;
				break;
			case '-':
			case 'S':
			case 's':
				opType = 2;
				break;
			case '*':
			case 'x':
			case 'M':
			case 'm':
				opType = 3;
				break;
			case '/':
			case 'D':
			case 'd':
				opType = 4;
				break;
			case '|':
			case 'B':
			case 'b':
				opType = 5;
				break;
			default:
				cout << "Unknown operation." << endl;
				return 1; 
		}

		//No need for right operand if operation is absolute value
		if(opType != 5)
		{
			cout << "Enter right operand: ";

			//Check if the right operation is the correct type
			if(cin >> rightOp)
			{
				//Perform operations
				switch(opType)
				{
					case 1:
						cout << "Result = " << (leftOp + rightOp) << endl;
						break;
					case 2:
						cout << "Result = " << (leftOp - rightOp) << endl;
						break;
					case 3:
						cout << "Result = " << (leftOp * rightOp) << endl;
						break;
					case 4:
						cout << "Result = " << (leftOp / rightOp) << endl;
						break;
				}
			}

			else
			{
				cout << "Invalid right operand." << endl;
				return 1;
			}
		}
	
		else
		{
			if(leftOp < 0)
				leftOp *= -1;
			cout << "Result = " << leftOp << endl;
		}
	}

	else
	{	
		cout << "Invalid left operand." << endl;
		return 1;
	}
	
	return 0;
}
