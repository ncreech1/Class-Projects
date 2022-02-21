//Author: Nicholas Creech
//Date: 22 August 2018
//Lab1
//
#include <string>
#include <iostream>

using namespace std;

int main()
{
	string name;
	cout<<"What is your name?"<<endl;
	cin>>name;

	cout<<"Hello, "<<name;

	for(int x = 0; x<10; x++)
	{
		cout<<"Hello, "<<name + "/n";
	}
}
