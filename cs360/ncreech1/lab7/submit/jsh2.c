#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <fcntl.h>
#include "fields.h"

/* Nicholas Creech
 * 11/19/20
 * Lab 7 Part 2
 */ 

int main(int argc, char **argv)
{
	int pid, status;
	int fdin, fdout;
	char **newargv;
	char *prompt;
	IS is;

	if(argc > 2)
	{
		printf("usage: ./jsh2 <prompt>\n");
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
			int x;
			int newNF = -1;
			bool foundFieldEnd = false;

			if(strcmp(is->fields[is->NF - 1], "&") == 0)
				newNF = is->NF - 1;
			else
				newNF = is->NF;
		
			//Find IO redirection
			for(x = 0; x < is->NF; x++)
			{
				//Redirect stdin
				if(strcmp(is->fields[x], "<") == 0)
				{
					fdin = open(is->fields[x + 1], O_RDONLY);
				
					if(fdin < 0 || dup2(fdin, 0) != 0)
					{
						perror(is->fields[x + 1]);
						jettison_inputstruct(is);
						exit(1);
					}

					//Fields stop at redirect symbol
					if(!foundFieldEnd)
					{
						newNF = x;
						foundFieldEnd = true;
					}
				
					close(fdin);
				}

				//Redirect stdout
				else if(strcmp(is->fields[x], ">") == 0 || strcmp(is->fields[x], ">>") == 0)
				{
					if(strcmp(is->fields[x], ">") == 0)
						fdout = open(is->fields[x + 1], O_WRONLY | O_TRUNC | O_CREAT, 0644);
					else
						fdout = open(is->fields[x + 1], O_WRONLY  | O_CREAT | O_APPEND, 0644);

					if(fdout < 0 || dup2(fdout, 1) != 1)
					{
						perror(is->fields[x + 1]);
						jettison_inputstruct(is);
						exit(1);
					}
	
					//Fields stop at redirect symbol
					if(!foundFieldEnd)
					{
						newNF = x;
						foundFieldEnd = true;
					}
				
					close(fdout);
				}
			}

			//Create argv (null terminated)
			newNF++;
			newargv = malloc(sizeof(char*) * newNF);
			memcpy(newargv, is->fields, sizeof(char*) * (newNF - 1));
			newargv[newNF - 1] = NULL;

			//Execute command
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
