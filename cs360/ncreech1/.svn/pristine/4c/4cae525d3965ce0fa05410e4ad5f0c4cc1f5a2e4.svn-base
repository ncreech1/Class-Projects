#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "jrb.h"
#include "jval.h"
#include "dllist.h"

/* Nicholas Creech
 * 09/14/20
 * Lab 2 Part 1: Write a program that reads machine info from a file and retrieves it by key from stdin
 * using buffered IO
 */

typedef struct machine
{
	unsigned int IP[4];
	Dllist names;
} Machine;

int main()
{
	FILE *fp;
	JRB mtree;
	int c = 0;
	
	fp = fopen("converted", "r");
	mtree = make_jrb();

	//Start reading in machines 
	c = fgetc(fp);
	while(c != EOF)
	{
		Machine *m;
		unsigned int x;
		unsigned int nameCount;

		//Allocate memory for current machine
		m = malloc(sizeof(Machine));
		m->names = new_dllist();

		//Assign first value of IP
		m->IP[0] = c;
		
		//Read in the rest of the IP
		for(x = 1; x < 4; x++)
		{
			c = fgetc(fp);
			m->IP[x] = c;
		}

		//Get the amount of names associated with this machine
		fread(&nameCount, sizeof(unsigned int), 1, fp);
		
		//Fix endianess 
		unsigned int bigEndian = 0;
		for(x = 3; x > 0; x--)
		{
			bigEndian |= ((nameCount & 0xFF000000) >> (8 * x));
			nameCount <<= 8;
		}

		nameCount = bigEndian;

		//Get all names of this machine
		for(x = 0; x < nameCount; x++)
		{
			char *name;
			int charCount = 0;

			//Get length of name
			while(fgetc(fp) != '\0')
				charCount++;

			charCount++;
			name = malloc(charCount * sizeof(char));
			fseek(fp, -charCount, SEEK_CUR);

			//Read name
			fread(name, sizeof(char), charCount, fp);

			//Add name to machine
			dll_append(m->names, new_jval_s(name));

			//Name is absolute name if it contains a period
			char *ch = strchr(name, '.');
			if(ch != NULL)
			{
				char *traverse, *localName;
				int len = 0;
				
				//Get length of local name
				for(traverse = name; traverse != ch; traverse++)
					len++;

				//Extract local name
				localName = malloc(sizeof(char) * (len + 1));
				memcpy(localName, name, len);
				localName[len] = '\0';

				//Add machine to tree for this local name key
				dll_append(m->names, new_jval_s(localName));
				jrb_insert_str(mtree, localName, new_jval_v(m));
			}

			//Add machine to tree for this name key
			jrb_insert_str(mtree, name, new_jval_v(m));
		}

		//Start reading next IP or EOF
		c = fgetc(fp);
	}

	//Read successful
	printf("Hosts all read in\n\n");
	fclose(fp);

	//Get input on stdin
	while(1)
	{
		char input[100];
		
		printf("Enter host name: ");
	
		//Get search key
		if(fscanf(stdin, "%s", input) == EOF)
			break;

		//Search tree for machine name
		JRB result = jrb_find_str(mtree, input);

		//Print machine info
		if(result != NULL)
		{
			JRB ptr;
			jrb_traverse(ptr, mtree)
			{
				//Print all machines with this key
				if(strcmp((char*)ptr->key.v, input) == 0)
				{
					Machine *m;
					unsigned int x;

					m = (Machine*)ptr->val.v;

					//Print IP
					for(x = 0; x < 3; x++)
						printf("%d.", m->IP[x]);
					printf("%d: ", m->IP[3]);

					//Print names
					Dllist dllptr;
					dll_traverse(dllptr, m->names)
						printf("%s ", (char*)dllptr->val.v);
					printf("\n\n");
				}
			}
		}

		else
			printf("no key %s\n\n", input);
	}

	printf("\n");

	//Free memory
	
	JRB ptr;
	jrb_traverse(ptr, mtree)
	{
		Machine *m = (Machine*)ptr->val.v;

		//Remove name from machine names each time a key is visited
		free((char*)dll_first(m->names)->val.v);
		dll_delete_node(dll_first(m->names));
		
		//If the names list is empty, all keys with this machine pointer have been visited
		if(dll_empty(m->names))
		{
			free_dllist(m->names);
			free(m);
		}
	}

	jrb_free_tree(mtree);

	return 0;
}
