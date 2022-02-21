#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <fcntl.h>
#include "fields.h"
#include "jrb.h"
#include "jval.h"
#include "dllist.h"

/* Nicholas Creech
 * 11/22/20
 * Lab 7 Part 3
 */ 

int main(int argc, char **argv)
{
	int pid, status;
	int i, numCommands;
	int pipefd[4];
	char **newargv;
	char *prompt;
	IS is;
	JRB commands;

	if(argc > 2)
	{
		printf("usage: ./jsh <prompt>\n");
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
	commands = make_jrb();

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

		//Find the number of commands from pipes
		numCommands = 1;
		for(i = 0; i < is->NF; i++)
		{
			if(strcmp(is->fields[i], "|") == 0)
				numCommands++;
		}

		//Run each command
		for(i = 0; i < numCommands; i++)
		{
			//Create first pipe on even commands and the second on odd commands
			if((i % 2 == 0 && pipe(pipefd) < 0) || (i % 2 != 0 && pipe(pipefd + 2) < 0)) 
			{
				perror("jsh: pipe");
				break;
			}

			//Run command in child process
			pid = fork();
			if(pid == 0)
			{
				int x;
				int currentCommand;
				int NF;
				int fdin, fdout;
				int inpipe, outpipe;
				Dllist fields;
				Dllist tmp;

				fields = new_dllist();
				currentCommand = 0;
				NF = 0;
		
				//Find IO redirection
				for(x = 0; x < is->NF; x++)
				{
					//Redirect stdin
					if(strcmp(is->fields[x], "<") == 0 && currentCommand == i)
					{
						fdin = open(is->fields[x + 1], O_RDONLY);
				
						if(fdin < 0 || dup2(fdin, 0) != 0)
						{
							perror(is->fields[x + 1]);
							jettison_inputstruct(is);
							free_dllist(fields);
							jrb_free_tree(commands);
							exit(1);
						}
				
						close(fdin);
						x++;
					}

					//Redirect stdout
					else if((strcmp(is->fields[x], ">") == 0 || strcmp(is->fields[x], ">>") == 0) && currentCommand == i)
					{
						if(strcmp(is->fields[x], ">") == 0)
							fdout = open(is->fields[x + 1], O_WRONLY | O_TRUNC | O_CREAT, 0644);
						else
							fdout = open(is->fields[x + 1], O_WRONLY  | O_CREAT | O_APPEND, 0644);

						if(fdout < 0 || dup2(fdout, 1) != 1)
						{
							perror(is->fields[x + 1]);
							jettison_inputstruct(is);
							free_dllist(fields);
							jrb_free_tree(commands);
							exit(1);
						}
				
						close(fdout);
						x++;
					}

					//Update current command
					else if(strcmp(is->fields[x], "|") == 0)
						currentCommand++;

					//Add the current field to be used for newargv
					else if(strcmp(is->fields[x], "&") != 0 && currentCommand == i)
					{
						dll_append(fields, new_jval_s(is->fields[x]));
						NF++;
					}
				}

				//First command; open out-pipe for next command
				if(i == 0 && numCommands > 1)
				{
					//Redirect stdout
					if(dup2(pipefd[1], 1) != 1)
					{
						perror("jsh: dup pipe");
						jettison_inputstruct(is);
						free_dllist(fields);
						jrb_free_tree(commands);
						exit(1);
					}
				}

				//Middle command; open in-pipe from last command and out-pipe for next command
				else if(i != 0 && i != numCommands - 1 && numCommands > 1)
				{
					//Odd commands write to the second pipe and read from the first
					if(i % 2 != 0)
					{
						inpipe = 0;
						outpipe = 3;
					}

					//Even commands write to the first pipe and read from the second
					else
					{
						inpipe = 2;
						outpipe = 1;
					}

					//Redirect stdin and stdout
					if(dup2(pipefd[inpipe], 0) != 0 || dup2(pipefd[outpipe], 1) != 1)
					{
						perror("jsh: dup pipe");
						jettison_inputstruct(is);
						free_dllist(fields);
						jrb_free_tree(commands);
						exit(1);
					}
				}

				//Final command; open in-pipe from last command
				else if(numCommands > 1)
				{
					//Final command reads from first pipe if odd and second pipe if even
					if(i % 2 != 0)
						inpipe = 0;
					else
						inpipe = 2;

					//Redirect stdin
					if(dup2(pipefd[inpipe], 0) != 0)
					{
						perror("jsh: dup pipe");
						jettison_inputstruct(is);
						free_dllist(fields);
						jrb_free_tree(commands);
						exit(1);
					}
				}

				
				//Always close the first pipe
				close(pipefd[0]);
				close(pipefd[1]);
				
				//The second pipe is only open after the second command
				if(i != 0)
				{
					close(pipefd[2]);
					close(pipefd[3]);
				}

				//Create argv (null terminated)
				NF++;
				newargv = malloc(sizeof(char*) * NF);	
				
				x = 0;
				dll_traverse(tmp, fields)
				{
					memcpy(newargv + x, &tmp->val.s, sizeof(char*));
					x++;
				}

				newargv[NF - 1] = NULL;

				//Execute command
				execvp(dll_first(fields)->val.s, newargv);
			
				//Command failed
				perror(dll_first(fields)->val.s);
				free(newargv);
				free_dllist(fields);
				jettison_inputstruct(is);
				jrb_free_tree(commands);
				exit(1);
			}

			else
			{
				//Close specific pipes there are more commands
				if(i != numCommands - 1)
				{
					//Close the first pipe on odd commands
					if(i % 2 != 0)
					{
						close(pipefd[0]);
						close(pipefd[1]);
					}

					//Close the second pipe on even commands
					else if(i != 0)
					{
						close(pipefd[2]);
						close(pipefd[3]);
					}
				}

				//Close all pipes if the final command was executed
				else
				{
					close(pipefd[0]);
					close(pipefd[1]);
				
					//The second pipe is only open after the second command
					if(i != 0)
					{
						close(pipefd[2]);
						close(pipefd[3]);
					}
				}

				//Add command to wait tree
				if(numCommands > 1 && strcmp(is->fields[is->NF - 1], "&") != 0)
					jrb_insert_int(commands, pid, JNULL);

				//Wait for command
				else if(strcmp(is->fields[is->NF - 1], "&") != 0)
					while(wait(&status) != pid);
			}
		}

		//Wait on all commands from pipe
		while(!jrb_empty(commands))
		{
			JRB tmp;
			
			pid = wait(&status);
			tmp = jrb_find_int(commands, pid);

			if(tmp != NULL)
				jrb_delete_node(tmp);
		}
	
		printf(prompt);
	}

	//Clean up any zombie processes
	while(wait(&status) != -1);

	jettison_inputstruct(is);
	jrb_free_tree(commands);
	return 0;
}
