/* Nicholas Creech
 * CS130
 * 2/8/19
 * Lab5: Create a program that loads a rooms file into dynamic memory, and lets the player traverse these rooms
 */

#include <cstdio>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>

using namespace std;

class Room
{
	public:
		static int roomTotal;
		Room enterRoom(string, string, const Room *);
		void listExits();
		void setSplitExits(string);
		char * name;
		char * desc;
		char * n, * s, * e, * w;
};

int Room::roomTotal = 0; //Initializing static variable

int main(int argc, char * argv[])
{
	Room * rooms;
	Room * startAddress;

	//Check that the user inputted a file name
	if(argc != 2)
	{
		printf("Usage: ./lab5 <file name>\n");
		return -1;
	}

	ifstream roomFile(argv[1]);

	//File opened successfully
	if(roomFile.is_open())
	{
		string line = "";

		//Get the amount of rooms (3 tildes per room)
		while(getline(roomFile, line, '\n'))
		{	
			if(line == "~")
				Room::roomTotal++;
		}

		Room::roomTotal /= 3; //3 tildes per room
		
		rooms = new Room[Room::roomTotal]; //Dynamically allocate memory to store the rooms
		startAddress = rooms; //Store beginning address for later

		roomFile.clear(); //Reset ifstream to file start
		roomFile.seekg(0, ios::beg);

		int index = 1;
		string multiLine = "";
		while(getline(roomFile, line, '\n'))
		{
			//Line is only a tilde; move to next section or Room
			if(line == "~")
			{
				//Increase memory address of rooms pointer for next room
				if(index == -1)
				{
					rooms++;
					index = 1;
				}

				switch(index)
				{
					case 1:
						rooms->name = multiLine;
						break;
					case 2:
						rooms->desc = multiLine; //Set description
						break;
					case 3:
						rooms->setSplitExits(multiLine); //Split up list of exits
						index = -2;
						break;
				}

				multiLine = ""; //Clear the multi-line string to check the next section or room
				index++;
			}
	
			//Add line to total string of multiple lines
			else
			{
				if(index > 1) //Only keep the return character for the description and exit directions for formatting
					multiLine += line + "\n";
				else
					multiLine += line;
			}
		}

		roomFile.close();
	}

	else
	{
		printf("Unable to open rooms file.");
		return -1;
	}

	rooms = startAddress; //Reset rooms address to beginning of memory block

	Room currentRoom = rooms[0].enterRoom("NULL", "n 0", rooms); //Enter starting room

	//Begin user input
	char command;
	while(command != 'q')
	{
		if(command != '\n') //Prevents printing a second ">" if scanf scans a '/n' 
			printf("> ");

		scanf("%c", &command);

		switch(command)
		{
			case 'l':
				printf("%s\n%s", currentRoom.name.c_str(), currentRoom.desc.c_str());
				currentRoom.listExits();
				break;
			case 'n':
				currentRoom = currentRoom.enterRoom("NORTH", currentRoom.n, rooms);
				break;
			case 's':
				currentRoom = currentRoom.enterRoom("SOUTH", currentRoom.s, rooms);
				break;
			case 'e':
				currentRoom = currentRoom.enterRoom("EAST", currentRoom.e, rooms);
				break;
			case 'w':
				currentRoom = currentRoom.enterRoom("WEST", currentRoom.w, rooms);
				break;
			case 'q':
				break;
		}
	}

	//printf("%s\n%s\n%s\n", rooms[1].id.c_str(), rooms[1].desc.c_str(), rooms[1].e.c_str());

	delete[] rooms; 

	return 0;
}

void Room::setSplitExits(string exitText)
{
	stringstream ss;

	ss.str(exitText);

	string line = "";
	while(getline(ss, line, '\n'))
	{
		switch(line.front())
		{
			case 'n':
				n = line;
				break;
			case 's':
				s = line;
				break;
			case 'e':
				e = line;
				break;
			case 'w':
				w = line;
				break;
		}
	}
}

Room Room::enterRoom(string direction, string id, const Room * rooms)
{
	Room nextRoom = (*this); //If the player can't move in the specified direction, return the current room

	if(id != "")
	{
		id = id.substr(2, string::npos); //Get just the id from the direction

		for(int x = 0; x < roomTotal; x++)
		{
			if(x == stoi(id))
			{
				nextRoom = rooms[x];

				if(direction != "NULL")
					printf("You moved %s.\n", direction.c_str());

				return nextRoom;
			}
		}
	}

	printf("You can't go %s!\n", direction.c_str()); //Direction not valid

	return nextRoom;
}

void Room::listExits()
{
	string exits = "Exits:";
	
	if(n != "")
		exits += " n";
	if(s != "")
		exits += " s";
	if(e != "")
		exits += " e";
	if(w != "")
		exits += " w";

	printf("\n%s\n", exits.c_str());

}
