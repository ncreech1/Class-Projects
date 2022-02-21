#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fields.h"

/* Nicholas Creech
 * 11/19/20
 * Lab 7 Part 1
 */ 

int main(int argc, char **argv)
{
	int pid, status, newNF;
	char **newargv;
	char *prompt;
	IS is;

	if(argc > 2)
	{
		printf("usage: ./jsh1 <prompt>\n");
		return 1;
	}

	//Get prompt
	if(argc == 2)
	{
		if(strcmp(argv[1], "-") == 0)
			prompt = "";
		else
			prompt = argv[1];
	}

	else
		prompt = "jsh: ";

	is = new_inputstruct(NULL);

	printf(prompt);
	while(get_line(is) >= 0)
	{	
		//Skip blank input
		if(is->NF == 0)
		{
			printf(prompt);
			continue;
		}

		else if(strcmp(is->fields[0], "exit") == 0)
			break;

		//Run command in child process
		pid = fork();
		if(pid == 0)
		{
			if(strcmp(is->fields[is->NF - 1], "&") == 0)
				newNF = is->NF;
			else
				newNF = is->NF + 1;
	
			newargv = malloc(sizeof(char*) * newNF);
			memcpy(newargv, is->fields, sizeof(char*) * is->NF);
			newargv[newNF - 1] = NULL;
			execvp(is->fields[0], newargv);
			
			//Command failed
			free(newargv);
			perror(is->fields[0]);
			break;
		}

		//Wait for child in parent process
		else if(strcmp(is->fields[is->NF - 1], "&") != 0)
			while(wait(&status) != pid);

		printf(prompt);
	}

	//Clean up any zombie processes
	while(wait(&status) != -1);

	jettison_inputstruct(is);
	return 0;
}
