#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>

/* Nicholas Creech
 * 10/8/18
 * Lab 6: Create a program that takes an input file of citation entries, a start date, and an end date, formats the entries within the range, 
 * and outputs to another file.
 */

using namespace std;

int main()
{
	string tFileName, rFileName;
	int startMonth, startDay, startYear;
	int endMonth, endDay, endYear;
	const double iMult = 5.2252, hMult = 9.4412, rMult = 17.1525, nMult = 12.152;
	const string months[13] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec", "Err"};
	ifstream ifs;
	ofstream ofs;

	cout << "Enter a ticket file: ";
	cin >> tFileName;
	ifs.open(tFileName + ".txt");

	if(!ifs)
		cerr << "Unable to open " << tFileName << ".\n";
	else
	{
		cout << "Enter a report file: ";
		cin >> rFileName;
		ofs.open(rFileName + ".txt");

		cout << "Enter report start date (mm dd yyyy): ";
		cin >> startMonth >> startDay >> startYear;

		cout << "Enter report end date (mm dd yyyy): ";
		cin >> endMonth >> endDay >> endYear;
		
		string currentLine;
		string citationNum;
		char roadType;
		double clockedSpeed, speedLimit;
		int month, day, year;

		//Check each entry in the input file, format, and ouput if within date range
		while(getline(ifs, currentLine))
		{
			stringstream ss(currentLine);
			ss >> citationNum >> month >> day >> year >> clockedSpeed >> speedLimit >> roadType;

			double fine = 0.0;
			string monthName;

			fine = clockedSpeed - speedLimit;

			//Apply fine multiplier
			switch(roadType)
			{
				case 'i': case 'I':
					fine *= iMult;
					break;
				case 'h': case 'H':
					fine *= hMult;
					break;
				case 'r': case 'R':
					fine *= rMult;
					break;
				default:
					fine *= nMult;
					break;
			}

			//Determine three character month
			monthName = months[month - 1];

			//Format year and fine
			if(year < 100)
				year += 2000;
			if(fine < 0)
				fine = 0.00;

			/* Determines whether or not to skip the current file entry based on the date.
			 * If an entry's year is between the start year and end year (exclusive) then it is a valid entry.
			 * However, if an entry's year is EQUAL to the start year or end year, the month and day must be checked.*/
			if(year >= startYear && year <= endYear)
			{
				if(year == startYear || year == endYear)
				{
					if(year == startYear && month < startMonth) //Before start month; skip
						continue;
					else if(year == startYear && month == startMonth && day < startDay) //In start month but before start day; skip
						continue;
					else if(year == endYear && month > endMonth) //After end month; skip
						continue;
					else if(year == endYear && month == endMonth && day > endDay) //In end month but after end day; skip
						continue;
				}
			}

			else
				continue;
		
			//Output to file
			ofs << right << setfill('0') << setw(2) << day << setfill(' ');
			ofs << '-' << monthName << '-' << year << ' ' << left << setw(10) << citationNum;
			ofs << "$" << setw(9) << right << setprecision(2) << fixed << fine << '\n';
		}

		ifs.close();
		ofs.close();
	}

	return 0;
}
