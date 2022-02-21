#include <iostream>
#include <fstream>
#include <cstdio>
#include "code_processor.hpp"

using namespace std;

unsigned int djb_hash(const string &s);

/* Nicholas Creech
 * 10/28/19
 * Lab 7: Create a program that allows Users to redeem Codes for points, and points for Prizes, by Phone or by Username
 */ 

//Adds a new prize to the server, along with its description, points, and quantity
bool Code_Processor::New_Prize(const string &id, const string &description, int points, int quantity)
{
	if(Prizes.find(id) != Prizes.end() || points <= 0 || quantity <= 0)
		return false;

	//Allocate memory for the prize and set values
	Prize * newPrize = new Prize;
	newPrize->id = id;
	newPrize->description = description;
	newPrize->points = points;
	newPrize->quantity = quantity;
	Prizes[id] = newPrize;

	return true;
}

//Adds a new user to the server, along with a starting point balance
bool Code_Processor::New_User(const string &username, const string &realname, int starting_points)
{
	if(Names.find(username) != Names.end() || starting_points < 0)
		return false;

	//Allocate memory for the user and set values
	User * newUser = new User;
	newUser->username = username;
	newUser->realname = realname;
	newUser->points = starting_points;
	Names[username] = newUser;

	return true;
}

//Deletes a user from the server, including any registered phone numbers 
bool Code_Processor::Delete_User(const string &username)
{
	set <string>::iterator pit;
	
	if(Names.find(username) == Names.end())
		return false;

	User * current = Names[username];
	Names.erase(username);

	//Delete all phone numbers registered to this user
	for(pit = current->phone_numbers.begin(); pit != current->phone_numbers.end(); pit++)
		Phones.erase(*pit);

	delete current;
	return true;
}

//Adds a registered phone number for a user
bool Code_Processor::Add_Phone(const string &username, const string &phone)
{
	if(Names.find(username) == Names.end() || Phones.find(phone) != Phones.end())
		return false;

	Phones[phone] = Names[username];
	Names[username]->phone_numbers.insert(phone);

	return true;
}

//Removes a registered phone number for a user
bool Code_Processor::Remove_Phone(const string &username, const string &phone)
{
	//Phone number must be registered to the user in question
	if(Names.find(username) == Names.end() || Phones.find(phone) == Phones.end() || Names[username] != Phones[phone])
		return false;

	Phones.erase(phone);
	Names[username]->phone_numbers.erase(phone);

	return true;
}

//Returns a string containing all the phone numbers for a user
string Code_Processor::Show_Phones(const string &username) const
{
	set<string>::const_iterator pit;

	if(Names.find(username) == Names.end())
		return "BAD USER";

	User * current = Names.find(username)->second;
	string numbers = "";

	//Append all the phone numbers for this user
	for(pit = current->phone_numbers.begin(); pit != current->phone_numbers.end(); pit++)
		numbers += *pit + "\n";	
	
	return numbers;
}

//Redeems a code by username, awarding either 10, 3, or 0 points
int Code_Processor::Enter_Code(const string &username, const string &code)
{
	if(Codes.find(code) != Codes.end() || Names.find(username) == Names.end())
		return -1;

	//Award 10 points and mark code as used
	if(djb_hash(code) % 17 == 0)
	{
		Codes.insert(code);
		Names[username]->points += 10;
		return 10;
	}

	//Award 3 points and mark code as used
	else if(djb_hash(code) % 13 == 0)
	{
		Codes.insert(code);
		Names[username]->points += 3;
		return 3;
	}

	else
		return 0;
}

//Redeems a code by phone number, awarding either 10, 3, or 0 points to the user
int Code_Processor::Text_Code(const string &phone, const string &code)
{
	if(Codes.find(code) != Codes.end() || Phones.find(phone) == Phones.end())
		return -1;

	//Award 10 points and mark code as used
	if(djb_hash(code) % 17 == 0)
	{
		Codes.insert(code);
		Phones[phone]->points += 10;
		return 10;
	}

	//Award 3 points and mark code as used
	else if(djb_hash(code) % 13 == 0)
	{
		Codes.insert(code);
		Phones[phone]->points += 3;
		return 3;
	}

	else
		return 0;
}

//Marks a code as used programatically, rather than by a user submitting a code
bool Code_Processor::Mark_Code_Used(const string &code)
{
	//Code is not valid if its djb_hash is not divisble by 17 or 13
	if(Codes.find(code) != Codes.end() || (djb_hash(code) % 17 != 0 && djb_hash(code) % 13 != 0))
		return false;

	Codes.insert(code);
	return true;
}

//Returns the current point balance for a user
int Code_Processor::Balance(const string &username) const
{
	if(Names.find(username) == Names.end())
		return -1;

	return Names.find(username)->second->points;
}

//Gives a user a prize, deducting the prize's point value from the user's balance
bool Code_Processor::Redeem_Prize(const string &username, const string &prize)
{
	if(Names.find(username) == Names.end() || Prizes.find(prize) == Prizes.end())
		return false;
	if(Names[username]->points < Prizes[prize]->points) //User must have enough points for the prize
		return false;

	//Deduct points from the user and decrement the prize quantity
	Names[username]->points -= Prizes[prize]->points;
	Prizes[prize]->quantity--;

	//If there are no more prizes of this type, delete the prize from the server
	if(Prizes[prize]->quantity == 0)
	{
		Prize * current = Prizes[prize];
		Prizes.erase(prize);
		delete current;
	}
		
	return true;
}

//Destructor deletes each user and prize that was dynamically allocated to memory
Code_Processor::~Code_Processor()
{
	map<string, User *>::iterator uit;
	map<string, Prize *>::iterator pit;

	for(uit = Names.begin(); uit != Names.end(); uit++)
		delete uit->second;
	for(pit = Prizes.begin(); pit != Prizes.end(); pit++)
		delete pit->second;
}

//Writes the current state of the server to a file for future loading
bool Code_Processor::Write(const string &filename) const
{
	map<string, User *>::const_iterator uit; //Iterator for Names map
	map<string, Prize *>::const_iterator prit; //Iterator for Prizes map
	set<string>::const_iterator cit; //Iterator for phone_numbers and Codes sets
	ofstream fout;
	int codeIndex = 0;

	fout.open(filename.c_str());
	if(fout.fail())
		return false;

	//Print out each User and his/her respective phone numbers
	for(uit = Names.begin(); uit != Names.end(); uit++)
	{
		fout << "ADD_USER " << uit->first << " " << uit->second->points << " " << uit->second->realname << endl;

		for(cit = uit->second->phone_numbers.begin(); cit != uit->second->phone_numbers.end(); cit++)
			fout << "ADD_PHONE " << uit->second->username << " " << *cit << endl;	
	}

	//Print out each Prize still remaining
	for(prit = Prizes.begin(); prit != Prizes.end(); prit++)
		fout << "PRIZE " << prit->first << " " << prit->second->points << " " << prit->second->quantity << " " << prit->second->description << endl;

	//Print out each code that has been used so far
	for(cit = Codes.begin(); cit != Codes.end(); cit++)
	{
		if(codeIndex == 0)
			fout << "MARK_USED";
		fout << " " << *cit;
		codeIndex++;

		if(codeIndex == 8)
		{
			codeIndex = 0;
			fout << "\n";
		}
	}

	if(codeIndex != 0)
		fout << "\n";

	//No problems feeding data to file if good bit is set
	if(fout.good())
		return true;
	else
		return false;
}

unsigned int djb_hash(const string &s)
{
	size_t i;
	unsigned int h;

	h = 5381;

	for(i = 0; i < s.size(); i++)
		h = (h << 5) + h + s[i];

	return h;
}
