#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <vector>

/* Nicholas Creech
 * 11/3/18
 * Lab 11: Create a program that takes a database file of ticket entries, a start date, an end date, formats the entries within the range, 
 * and outputs to another file or the console
 */

using namespace std;

const double iMult = 5.2252, hMult = 9.4412, rMult = 17.1525, nMult = 12.152;
const string months[13] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec", "Err"};

struct Date
{
	int day, month, year;
	string monthName;
};

struct Ticket
{
	string citationNum;
	Date citationDate;
	int clockedSpeed, speedLimit;
	char roadType;
	double fine;
};

class Database
{
	public:
		void add_ticket(const Ticket&);
		vector<Ticket> gen_report(Date, Date) const;
	private:
		vector<Ticket> tickets;
};

//Standalone functions
bool Read(Database&, istream&);
bool Write(const vector<Ticket>&, ostream&);
int Encode(const Date&);
Date convertDate(const string&);

int main(int argc, char * argv[])
{
	Date startDate, endDate;
	string databaseName, startString, endString, output;
	ifstream ifs;
	ofstream ofs;

	//Check input format
	if(argc != 5)
	{
		cout << "Usage: ./lab11 <input file> <start: year-month-day> <end: year-month-day> <output or '-'>\n";
		return 1;
	}

	else
	{
		databaseName = argv[1];
		startString = argv[2];
		endString = argv[3];
		output = argv[4];
	}

	//Convert start and end dates from year-month-day strings to Date objects
	startDate = convertDate(startString);
	endDate = convertDate(endString);
	
	//Open the database inputted by the user
	Database databaseFile;
	ifs.open(databaseName);

	//Attempt to read each ticket report from the database file to a new Database object
	if(Read(databaseFile, ifs))
	{
		//Finished reading file
		ifs.close();

		//Get the valid tickets with dates between startDate and endDate (inclusive)
		vector<Ticket> validTickets = databaseFile.gen_report(startDate, endDate);

		if(output == "-")
			Write(validTickets, cout); //Output to console
		else
		{
			ofs.open(output);
			Write(validTickets, ofs); //Output to file
			ofs.close();
		}
	}

	else
		cout << "The file '" << databaseName << "' cannot be read.\n";

	return 0;
}

//Standalone functions
bool Read(Database& d, istream& in)
{
	if(in)
	{
	 	string currentLine;

		//Check each entry in the input database file, format, and add to Database.tickets as new Ticket object
		while(getline(in, currentLine))
		{
			Ticket t;
			stringstream ss(currentLine);
			ss >> t.citationNum >> t.citationDate.month >> t.citationDate.day >> t.citationDate.year >> t.clockedSpeed >> t.speedLimit >> t.roadType;

			//Calculate and store fine
			t.fine = t.clockedSpeed - t.speedLimit;

			//Apply fine multiplier
			switch(t.roadType)
			{
				case 'i': case 'I':
					t.fine *= iMult;
					break;
				case 'h': case 'H':
					t.fine *= hMult;
					break;
				case 'r': case 'R':
					t.fine *= rMult;
					break;
				default:
					t.fine *= nMult;
					break;
			}

			//Determine three character month
			t.citationDate.monthName = months[t.citationDate.month - 1];

			//Format year and fine
			if(t.citationDate.year < 100)
				t.citationDate.year += 2000;
			if(t.fine < 0)
				t.fine = 0.00;

			//Add ticket to Database object
			d.add_ticket(t);
		}

		return true;
	}

	return false;
}

//Writes the valid tickets from the database file to the inputted ostream (cout or ofstream)
bool Write(const vector<Ticket>& tickets, ostream& out)
{
	if(out)
	{
		//Format and output each ticket's contents
		for(auto t : tickets)
		{
			out << right << setfill('0') << setw(2) << t.citationDate.day << setfill(' ');
			out << '-' << t.citationDate.monthName << '-' << t.citationDate.year << ' ' << left << setw(10) << t.citationNum;
			out << "$" << setw(9) << right << setprecision(2) << fixed << t.fine << '\n';
		}

		return true;
	}

	return false;
}

//Converts a Date object to an integer of format yearmonthday
int Encode(const Date& d)
{
	stringstream ss;
	ss << d.year << setfill('0') << setw(2) << right << d.month << setw(2) << d.day;

	return stoi(ss.str());
}

//Convert a date passed when the program is run to a Date object (input string must be in format year-month-day) 
Date convertDate(const string& d)
{
	stringstream ss(d);
	string piece;
	vector<string> pieces;

	//Split input string by '-' delimeter
	while(getline(ss, piece, '-'))
		pieces.push_back(piece);

	//Store in Date object
	Date date;
	date.year = stoi(pieces.at(0));
	date.month = stoi(pieces.at(1));
	date.day = stoi(pieces.at(2));

	return date;
}

//Database member functions
void Database::add_ticket(const Ticket& ticket)
{
	tickets.push_back(ticket);
}

//Check each ticket in the database and return a new vector of tickets that are within the givin start and end dates
vector<Ticket> Database::gen_report(Date start, Date end) const
{
	vector<Ticket> validTickets;
	int startDateNum = Encode(start);
	int endDateNum = Encode(end);

	//Encode 
	for(auto t : tickets)
	{
		int ticketDateNum = Encode(t.citationDate);
		if(ticketDateNum >= startDateNum && ticketDateNum <= endDateNum)
			validTickets.push_back(t);
	}

	return validTickets;
}
