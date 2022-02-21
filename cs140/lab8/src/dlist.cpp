#include "dlist.hpp"
#include <iostream>

using namespace std;

/* Nicholas Creech
 * 11/2/19
 * Lab 8: Create the Dlist class which consists of Dnodes and a sentinel Dnode to acces its memory, mimicking the STL List object
 */

Dlist::Dlist()
{
	sentinel = new Dnode;
	sentinel->s = "";
	sentinel->flink = sentinel;
	sentinel->blink = sentinel;
	size = 0;
}

//Copy Constructor
Dlist::Dlist(const Dlist &d)
{
	//The Dlist copy must have its sentinel initialized
	sentinel = new Dnode;
	sentinel->s = "";
	sentinel->flink = sentinel;
	sentinel->blink = sentinel;
	size = 0;

	//The copying is handled via the operator= overload
	*this = d;	
}

Dlist& Dlist::operator= (const Dlist &d)
{
	Dnode *tmp; //The current Dnode in Dlist d

	Clear(); //Clear any preexisting Dnodes before copying via Push_Back()
	for(tmp = d.Begin(); tmp != d.End(); tmp = tmp->flink)
		Push_Back(tmp->s);

	return *this;
}

Dlist::~Dlist()
{
	Clear();
	delete sentinel;
}

//Clears all the Dnodes from the Dlist except for the sentinel
void Dlist::Clear()
{
	Dnode *tmp; //The current Dnode
	Dnode *next; //Keeps track of the next Dnode so tmp can be deleted

	for(tmp = Begin(); tmp != End(); tmp = next)
	{
		next = tmp->flink;
		Erase(tmp);
	}
}

//Returns whether the Dlist is empty or not
bool Dlist::Empty() const
{
	return (size == 0);
}

//Returns the total number of Dnodes in the Dlist, excluding the sentinel
size_t Dlist::Size() const
{
	return size;
}

//Adds a new Dnode to the front of the Dlist
void Dlist::Push_Front(const string &s)
{
	Insert_Before(s, Begin());
}

//Adds a new Dnode to the back of the Dlist
void Dlist::Push_Back(const string &s)
{
	//Inserting before the sentinel is the same as inserting at the end of the Dlist
	Insert_Before(s, End());
}

//Deletes the first Dnode in the Dlist
string Dlist::Pop_Front()
{
	string s;
	s = Begin()->s;
	Erase(Begin());

	return s;
}

//Deletes the last Dnode in the Dlist
string Dlist::Pop_Back()
{
	string s;
	s = Rbegin()->s;
	Erase(Rbegin());

	return s;
}

//Returns the first Dnode in the Dlist
Dnode* Dlist::Begin() const
{
	return sentinel->flink;
}

//Returns the sentinel (one after the last Dnode)
Dnode* Dlist::End() const
{
	return sentinel;
}

//Returns the last Dnode in the Dlist
Dnode* Dlist::Rbegin() const
{
	return sentinel->blink;
}

//Returns the sentinel (one before the first Dnode)
Dnode* Dlist::Rend() const
{
	return sentinel;
}

//Inserts a new Dnode before the specified Dnode
void Dlist::Insert_Before(const string &s, Dnode *n)
{
	Dnode* newnode;
	Dnode* prev; //The Dnode before the one to insert

	prev = n->blink;
	newnode = new Dnode;
	newnode->s = s;

	newnode->flink = n;
	newnode->blink = prev;
	n->blink = newnode;
	prev->flink = newnode;

	size++;
}

//Inserts a new Dnode after the specified Dnode
void Dlist::Insert_After(const string &s, Dnode *n)
{
	//Inserting after is the same as inserting before Dnode (n + 1)
	Insert_Before(s, n->flink);
}

//Erases the specified Dnode
void Dlist::Erase(Dnode *n)
{
	Dnode* prev; //The Dnode before the one to erase
	Dnode* next; //The Dnode after the one to erase

	prev = n->blink;
	next = n->flink;
	prev->flink = next;
	next->blink = prev;

	delete n;
	size--;
}
