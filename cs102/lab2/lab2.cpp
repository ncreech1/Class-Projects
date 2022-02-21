#include <iostream>

using namespace std;

/*
* Nicholas Creech
* 8/29/18
* Lab2: Make movie star name for actor from name, age, and street address
*/


int main()
{
	string firstName, middleName, streetName, streetType;
	int age, newAge, streetNum;

	//Print statements followed by read statements for the data
	cout << "Enter your first and middle names: ";
	cin >> firstName >> middleName;
	cout << "Enter your age: ";
	cin >> age;
	cout << "Enter your street number, name, and type: ";
	cin >> streetNum >> streetName >> streetType;

	//Setting age to remainder of (streetNum / age) * 3
	newAge = (streetNum % age) * 3;

	//Print statements displaying the final movie star name, age, and address
	cout << endl << "Your movie star name is " << streetName << " " << middleName << "." << endl;
	cout << "You will play a " << newAge << " year old." << endl;
	cout << "Your address is " << (age * 700 / streetNum) << " " << firstName << " " << streetType << "." << endl; 
}

