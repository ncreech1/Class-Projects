#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdbool.h>
#include "fields.h"
#include "dllist.h"
#include "jval.h"

/* Nicholas Creech
 * 09/25/20
 * Lab 3: Write a program that takes a simplified makefile format and makes an executable
 */

void readFileNames(IS is, Dllist list);
int fakemakeError(char *msg);
char * buildCommand(Dllist fields);
void freeDllist(Dllist list);

int main(int argc, char * argv[])
{
	char *filename;
	char *executable;
	char *flagString;
	int commandLength;
	IS is;
	Dllist cfiles;
	Dllist hfiles;
	Dllist libraries;
	Dllist flags;
	Dllist ptr;
	struct stat fileInfo;
	time_t hFileTime;
	time_t oFileTime;
	bool remakeExecutable;
	int upToDate;

	if(argc > 2)
	{
		printf("Usage: ./fakemake <filename>\n");
		return 1;
	}

	if(argc == 2)
		filename = argv[1];
	else
		filename = "fmakefile";

	is = new_inputstruct(filename);

	if(is == NULL)
		return fakemakeError("Could not find fmakefile");

	//Initialize data types
	cfiles = new_dllist();
	hfiles = new_dllist();
	libraries = new_dllist();
	flags = new_dllist();
	executable = NULL;
	hFileTime = -1;
	oFileTime = -1;
	remakeExecutable = false;
	upToDate = 0;

	//Could not find file
	if(is == NULL)
	{
		perror(filename);
		return 1;
	}

	//Read file
	while(get_line(is) != -1)
	{
		//Skip blank lines
		if(is->NF > 0)
		{
			if(strcmp(is->fields[0], "C") == 0)
				readFileNames(is, cfiles);
			else if(strcmp(is->fields[0], "H") == 0)
				readFileNames(is, hfiles);
			else if(strcmp(is->fields[0], "L") == 0)
				readFileNames(is, libraries);
			else if(strcmp(is->fields[0], "F") == 0)
				readFileNames(is, flags);
			else if(strcmp(is->fields[0], "E") == 0)
			{
				if(executable != NULL)
				{
					fprintf(stderr, "fmakefile (%d) cannot have more than one E line\n", is->line);
					return 1;
				}

				if(is->NF == 1)
					return fakemakeError("Empty E line");

				executable = strdup(is->fields[1]);
			}

			else
				return fakemakeError("Lines must start with C, H, E, F, or L");
		}
	}

	//Could not find executable name
	if(executable == NULL)
		return fakemakeError("No executable specified");

	//Create flag string from flag fields 
	flagString = buildCommand(flags);

	//Check .h files
	dll_traverse(ptr, hfiles)
	{
		//Get modify time for file (if it exists)
		if(stat(ptr->val.s, &fileInfo) < 0)
		{
			fprintf(stderr, "%s: No such file or directory\n", ptr->val.s);
			return 1;
		}
		
		else
		{
			//Find last (max) .h file modify time
			if(fileInfo.st_mtime > hFileTime)
				hFileTime = fileInfo.st_mtime;
		}
	}

	//Check .c files
	dll_traverse(ptr, cfiles)
	{
		//Get modify time for file (if it exists)
		if(stat(ptr->val.s, &fileInfo) < 0)
		{
			fprintf(stderr, "fmakefile: %s: No such file or directory\n", ptr->val.s);
			return 1;
		}

		else
		{
			time_t cFileTime = fileInfo.st_mtime;
			char *fileBaseName = malloc(strlen(ptr->val.s) + 1);
			memcpy(fileBaseName, ptr->val.s, strlen(ptr->val.s) - 2);

			//Create name of .o file
			strcpy(fileBaseName + strlen(ptr->val.s) - 2, ".o");
		
			//Must create (or update) .o file
			if(stat(fileBaseName, &fileInfo) < 0 || fileInfo.st_mtime < cFileTime || fileInfo.st_mtime < hFileTime)
			{
				//Build gcc -c command
				char *oFileCommand = malloc(7 + strlen(flagString) + strlen(ptr->val.s) + 1);
				strcpy(oFileCommand, "gcc -c ");
				strcpy(oFileCommand + 7, flagString);
				strcpy(oFileCommand + 7 + strlen(flagString), ptr->val.s);

				printf("%s\n", oFileCommand);

				//Create .o file
				if(system(oFileCommand) != 0)
					return fakemakeError("Command failed.  Exiting");

				remakeExecutable = true;
				free(oFileCommand);
			}

			else
			{
				//Find last (max) .o file modify time
				if(fileInfo.st_mtime > oFileTime)
					oFileTime = fileInfo.st_mtime;
			}

			free(fileBaseName);
		}
	}

	//Make executable if it does not exist, a .o file has been updated/made, or it's older than a .o file
	if(stat(executable, &fileInfo) < 0 || remakeExecutable || fileInfo.st_mtime < oFileTime)
	{
		int x = 0;
		char *cfilesString = buildCommand(cfiles);
		char *libsString = buildCommand(libraries);
		char *executableCommand;

		//Convert all .c file names to .o
		for( ; x < strlen(cfilesString); x++)
		{
			if(cfilesString[x] == '.')
				cfilesString[x + 1] = 'o';
		}

		//Build gcc -o command
		executableCommand = malloc(7 + strlen(executable) + 1 + strlen(flagString) + strlen(cfilesString) + strlen(libsString) + 1);
		strcpy(executableCommand, "gcc -o ");
		strcpy(executableCommand + 7, executable);
		strcpy(executableCommand + 7 + strlen(executable), " ");
		strcpy(executableCommand + 7 + strlen(executable) + 1, flagString);
		strcpy(executableCommand + 7 + strlen(executable) + 1 + strlen(flagString), cfilesString);
		strcpy(executableCommand + 7 + strlen(executable) + 1 + strlen(flagString) + strlen(cfilesString), libsString);
		executableCommand[strlen(executableCommand) - 1] = '\0';

		printf("%s\n", executableCommand);

		if(system(executableCommand) != 0)
			return fakemakeError("Command failed.  Fakemake exiting");
	
		free(libsString);
		free(executableCommand);
		free(cfilesString);
			
	}

	else
	{
		printf("%s up to date\n", executable);
		upToDate =  1;
	}

	//Free memory
	jettison_inputstruct(is);
	freeDllist(cfiles);
	freeDllist(hfiles);
	freeDllist(libraries);
	freeDllist(flags);
	free(flagString);
	free(executable);
	
	return upToDate;
}

//Reads all fields from the current line in the file into a Dllist
void readFileNames(IS is, Dllist list)
{
	int x;

	for(x = 1; x < is->NF; x++)
		dll_append(list, new_jval_s(strdup(is->fields[x])));
}

//Prints the error message to stderr and returns main at the same time
int fakemakeError(char *msg)
{
	fprintf(stderr, "%s\n", msg);
	return 1;
}

//Builds a string by concatenating all the fields of a Dllist (ends with a space)
char * buildCommand(Dllist fields)
{
	Dllist ptr;
	char *result;
	int pos;
	int commandLength;

	pos = 0;
	commandLength = 0;

	dll_traverse(ptr, fields)
		commandLength += strlen(ptr->val.s) + 1;

	if(!dll_empty(fields))
	{
		commandLength += 2; //Make room for end space and null terminator
		result = malloc(commandLength * sizeof(char));
		strcpy(result, dll_first(fields)->val.s);
		pos = strlen(dll_first(fields)->val.s);	
		
		//Copy all the fields to the new string with spaces in between
		dll_traverse(ptr, fields)
		{
			if(ptr != fields->flink)
			{
				result[pos] = ' ';
				strcpy(result + pos + 1, ptr->val.s);
				pos += strlen(result + pos);
			}
		}

		result[pos] = ' ';
	}

	else
	{
		result = malloc(1);
		result[0] = '\0';
		return result;
	}

	result[pos + 1] = '\0';
	return result;
}

//Frees all memory associated with a list
void freeDllist(Dllist list)
{
	Dllist ptr;

	dll_traverse(ptr, list)
		free(ptr->val.s);

	free_dllist(list);
}
