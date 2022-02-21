/*
 * Nicholas Creech
 * 9/11/18
 * Lab4: Create a program that takes a number of cars and trucks, each with their repsective miles and gallons, and determines the average MPG total for cars and trucks
 */

#include <iostream>
#include <string>
#include <limits>

using namespace std;

void checkVehicle(const string&);

//Global variables
string vehicle;
int numCars, numTrucks = 0;
double carMilesTotal, carGallonsTotal = 0.0;
double truckMilesTotal, truckGallonsTotal = 0.0;

int main()
{
	vehicleCheck:
	cout << "Enter command: ";
	cin >> vehicle;

	//Checking for a new car or truck or if input is done
	if(vehicle == "car")
	{
		checkVehicle("car");
		goto vehicleCheck;
	}

	else if(vehicle == "truck")
	{
		checkVehicle("truck");
		goto vehicleCheck;
	}

	//Calculations for final output executed here
	else if(vehicle == "done")
	{
		if(numCars != 0)
			cout << "Average car MPG = " << (carMilesTotal / carGallonsTotal) << endl;
		else
			cout << "Fleet has no cars." << endl;
		if(numTrucks != 0)
			cout << "Average truck MPG = " << (truckMilesTotal / truckGallonsTotal) << endl;
		else
			cout << "Fleet has no truks." << endl;
	}

	else
	{
		cout << "Unknown command." << endl;
		goto vehicleCheck;
	}

	return 0;
}

//Checks for input for miles and gallons for either a car or truck
void checkVehicle(const string& vehicle)
{
	vehicleMilesCheck:
	cout << "Enter " + vehicle + "'s miles: ";
	double vehicleMiles;
	double vehicleGallons;
		
	//Getting miles input
	if(cin >> vehicleMiles)
	{
		numCars += (vehicle == "car" ? 1 : 0);
		numTrucks += (vehicle == "truck" ? 1 : 0);

		carMilesTotal += (vehicle == "car" ? vehicleMiles : 0);
		truckMilesTotal += (vehicle == "truck" ? vehicleMiles : 0);
			
		vehicleGallonsCheck:
		cout << "Enter " + vehicle + "'s gallons: ";

		//Getting gallons input
		if(cin >> vehicleGallons)
		{
			carGallonsTotal += (vehicle == "car" ? vehicleGallons : 0);
			truckGallonsTotal += (vehicle == "truck" ? vehicleGallons : 0);
			return;
		}

		else
		{
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			goto vehicleGallonsCheck;	
		}
	}

	else
	{
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		goto vehicleMilesCheck;
	}
}
