#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "fields.h"
#include "dllist.h"
#include "jrb.h"
#include "jval.h"

/* Nicholas Creech
 * 09/06/20
 * Lab 1: Write a program that takes a family tree on stdin and prints the tree on stdout using the Libfdr library
 */ 

typedef struct Person
{
	char *name;
	char *sex;
	struct Person *father;
	struct Person *mother;
	Dllist children;
	int visited;
	int printed;
} Person;

char * buildName(IS is);
Person * addPerson(JRB ftree, char *name);
bool linkRelatives(Person* parent, Person* child, bool parentIsFather, int line);
bool assignSex(Person *p, char *sex, int line);
int isDescendant(Person *p);
int freeMemory(JRB ftree, IS is);

int main()
{
	JRB ftree;
	IS is;
	Person *p; //The current person in the PERSON block
	Person *relative; //The current relative being checked
	int i;

	//Open stdin stream
	is = new_inputstruct(NULL);

	//Init tree
	ftree = make_jrb();

	//Read stdin
	while(get_line(is) >= 0)
	{
		if(is->NF > 1)
		{
			//Start of PERSON block
			if(strcmp(is->fields[0], "PERSON") == 0)
			{
				char *name = buildName(is);
				JRB findPerson = jrb_find_str(ftree, name);

				//New person; add to ftree
				if(findPerson == NULL)
					p = addPerson(ftree, name);
				else
				{
					p = (Person*)findPerson->val.v;
					free(name);
				}
			}
		
			//Assign sex
			else if(strcmp(is->fields[0], "SEX") == 0)
			{	
				if(!assignSex(p, is->fields[1], is->line))
					return freeMemory(ftree, is);
			}

			else
			{
				char *relativeName = buildName(is);
				JRB findRelative = jrb_find_str(ftree, relativeName);

				//Relative not yet in tree; add
				if(findRelative == NULL)
					relative = addPerson(ftree, relativeName);
				else
				{
					relative = (Person*)findRelative->val.v;
					free(relativeName);
				}

				if(strcmp(is->fields[0], "FATHER") == 0)
				{
					//Relative is father so male is implied
					if(!assignSex(relative, "M", is->line))
						return freeMemory(ftree, is);
			
					//Link father to current person in PERSON block
					if(!linkRelatives(relative, p, true, is->line))
						return freeMemory(ftree, is);
				}

				else if(strcmp(is->fields[0], "MOTHER") == 0)
				{
					//Relative is mother so female is implied
					if(!assignSex(relative, "F", is->line))
						return freeMemory(ftree, is);

					//Link mother to current person in PERSON block
					if(!linkRelatives(relative, p, false, is->line))
						return freeMemory(ftree, is);
				}

				else if(strcmp(is->fields[0], "FATHER_OF") == 0)
				{
					//Current person in PERSON block is father so male is implied
					if(!assignSex(p, "M", is->line))
						return freeMemory(ftree, is);

					//Link current person in PERSON block (father) to child
					if(!linkRelatives(p, relative, true, is->line))
						return freeMemory(ftree, is);
				}

				else if(strcmp(is->fields[0], "MOTHER_OF") == 0)
				{
					//Current person in PERSON block is mother so female is implied
					if(!assignSex(p, "F", is->line))
						return freeMemory(ftree, is);

					//Link current person in PERSON block (mother) to child
					if(!linkRelatives(p, relative, false, is->line))
						return freeMemory(ftree, is);
				}
			}
		}
	}
	
	Dllist queue;
	JRB treePtr;

	queue = new_dllist();

	//Create queue for DFS and BFS from people with no parents
	jrb_traverse(treePtr, ftree)
	{
		Person *current = (Person*)treePtr->val.v;

		if(current->father == NULL && current->mother == NULL)
			dll_append(queue, treePtr->val);
	
		//Check for cycles with DFS
		if(isDescendant(current))
		{
			fprintf(stderr, "Bad input -- cycle in specification\n");
			free_dllist(queue);
			return freeMemory(ftree, is);
		}
	}

	//Perform BFS to print
	while(!dll_empty(queue))
	{
		Person *current;
		current = (Person*)dll_first(queue)->val.v;

		if(!current->printed)
		{
			//Do not print until parents are printed
			if((current->father == NULL || current->father->printed) && (current->mother == NULL || current->mother->printed))
			{
				printf("%s\n", current->name);

				if(current->sex[0] == 'M') 
					printf("  Sex: Male\n");
				else if(current->sex[0] == 'F')
					printf("  Sex: Female\n");
				else
					printf("  Sex: %s\n", current->sex);

				if(current->father == NULL)
					printf("  Father: Unknown\n");
				else
					printf("  Father: %s\n", current->father->name);
				if(current->mother == NULL)
					printf("  Mother: Unknown\n");
				else
					printf("  Mother: %s\n", current->mother->name);

				printf("  Children: ");

				if(dll_empty(current->children))
						printf("None");

				printf("\n");

				Dllist cptr;
				dll_traverse(cptr, current->children)
					printf("    %s\n", ((Person*)cptr->val.v)->name);

				printf("\n");

				//Mark person as printed
				current->printed = 1;

				//Append children to print
				dll_traverse(cptr, current->children)
					dll_append(queue, cptr->val);
			}
		}

		dll_delete_node(dll_first(queue));
	}

	//Free memory from tree vals
	freeMemory(ftree, is);
	free_dllist(queue);

	return 0;
}

//Builds the name of the person from inputstruct fields
char * buildName(IS is)
{
	char *name;
	int x = 2;
	int pos = 0;

	//No need to allocate +1 for null byte because there is an extra space between the first two fields
	name = malloc(sizeof(char) * (strlen(is->text1) - strlen(is->fields[0])));
	strcpy(name, is->fields[1]);
	pos = strlen(is->fields[1]);

	//Build the name from all fields past the first
	for( ; x < is->NF; x++)
	{
		name[pos] = ' ';
		strcpy(name + pos + 1, is->fields[x]);
		pos += strlen(name + pos);
	}

	return name;
}

//Adds a new person to the tree
Person * addPerson(JRB ftree, char *name)
{
	Person *p;
	p = malloc(sizeof(Person));
	p->children = new_dllist();
	p->name = name;
	p->sex = malloc(sizeof(char) * 8);
	strcpy(p->sex, "Unknown");
	p->father = NULL;
	p->mother = NULL;
	p->visited  = 0;
	p->printed = 0;

	//Add relative to tree
	jrb_insert_str(ftree, p->name, new_jval_v(p));	

	return p;
}

//Links a parent and their child (if needed)
//Boolean parentIsFather tells which pointer in the child struct to assign the parent to
bool linkRelatives(Person *parent, Person *child, bool parentIsFather, int line)
{
	Dllist ptr;	
	dll_traverse(ptr, parent->children)
	{
		//Child already assigned to parent's children list
		if((Person*)ptr->val.v == child)
			return true;
	}

	//New child
	if(parentIsFather)
	{
		if(child->father != NULL)
		{
			fprintf(stderr, "Bad input -- child with two fathers on line %d\n", line);
			return false;
		}

		child->father = parent;
	}

	else
	{
		if(child->mother != NULL)
		{
			fprintf(stderr, "Bad input -- child with two mothers on line %d\n", line); 
			return false;
		}

		child->mother = parent;
	}

	dll_append(parent->children, new_jval_v(child));
			
	return true;
}

//Attempts to assign a sex to Person p, checking for sex mismatches
bool assignSex(Person *p, char *sex, int line)
{
	if((p->sex[0] == 'M' && sex[0] != 'M') || (p->sex[0] == 'F' && sex[0] != 'F'))
	{
		fprintf(stderr, "Bad input - sex mismatch on line %d\n", line);
		return false;
	}

	else
	{
		free(p->sex);
		p->sex = malloc(sizeof(char) + 1);
		strcpy(p->sex, sex);
		return true;
	}
}

//Tests if Person p is their own descendant with a DFS
int isDescendant(Person *p)
{
	Dllist queue;

	if(p->visited == 1) 
		return 0;

	if(p->visited == 2)
		return 1;

	p->visited = 2;
	
	//Recursively check children
	dll_traverse(queue, p->children)
	{
		if(isDescendant((Person*)queue->val.v))
			return 1;
	}

	p->visited = 1;

	return 0;

}

//Frees all memory allocated for each person in the tree
int freeMemory(JRB ftree, IS is)
{
	JRB treePtr;

	//Free memory from tree vals
	jrb_traverse(treePtr, ftree)
	{
		Person *current = (Person*)treePtr->val.v;

		if(current != NULL)
		{
			free(current->name);
			free(current->sex);
			free_dllist(current->children);
			free(current);
		}
	}

	jettison_inputstruct(is);
	jrb_free_tree(ftree);

	return 1;
}
