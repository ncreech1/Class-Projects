#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include "jrb.h"
#include "dllist.h"
#include "jval.h"

/* Nicholas Creech
 * 10/10/20
 * Lab 4 Part 2: Write a program called tarx that takes a tar file and 
 * extracts the contents
 */ 

void checkError();
bool checkEOF();
void freeTrees(JRB inodes, JRB dirModes, JRB dirTimes);

int main(int argc, char * argv[])
{
	JRB inodes;
	JRB dirModes;
	JRB dirTimes;
	JRB tmp;

	if(argc != 1)
	{
		printf("usage: ./tarx\n");
		return 1;
	}

	inodes = make_jrb();
	dirModes = make_jrb();
	dirTimes = make_jrb();

	//Keep reading from stdin until EOF or error
	while(1)
	{
		int nameSize;
		char *name;
		long inode;
		int mode;
		long mtime;
		struct timeval *fileTimes;
		long fileSize;
		unsigned char *contents;

		//Read name size
		fread(&nameSize, sizeof(int), 1, stdin);

		//End of file reached
		if(checkEOF())
			break;

		checkError();

		//Read name
		name = malloc((nameSize + 1) * sizeof(char));
		fread(name, sizeof(char), nameSize, stdin);
		name[nameSize] = '\0';
		checkError();

		//Read inode 
		fread(&inode, sizeof(long), 1, stdin);
		checkError();

		//Create file
		if(jrb_find_int(inodes, inode) == NULL)
		{
			//Read mode
			fread(&mode, sizeof(int), 1, stdin);
			checkError();

			//Read modification time
			fread(&mtime, sizeof(long), 1, stdin);
			checkError();

			//Create timeval array
			fileTimes = malloc(2 * sizeof(struct timeval));
			fileTimes[0].tv_sec = time(NULL);
			fileTimes[0].tv_usec = 0;
			fileTimes[1].tv_sec = mtime;
			fileTimes[1].tv_usec = 0;

			//Create directory
			if(S_ISDIR(mode))
			{
				Jval modeVal;

				//Ensure directory was created
				if(mkdir(name, 0777) != 0)
				{
					perror(name);
					free(name);
					free(fileTimes);
					freeTrees(inodes, dirModes, dirTimes);
					exit(1);
				}
				
				modeVal.l = mode;
				jrb_insert_str(dirModes, strdup(name), modeVal);
				jrb_insert_str(dirTimes, strdup(name), new_jval_v(fileTimes));
			}
			
			//Create file
			else
			{
				FILE * fp;

				//Read file size
				fread(&fileSize, sizeof(long), 1, stdin);

				//Ensure file size is valid
				if(fileSize < 0)
				{
					fprintf(stderr, "tarx: Error reading tar file\n");
					free(name);
					free(fileTimes);
					freeTrees(inodes, dirModes, dirTimes);
					exit(1);
				}

				checkError();

				//Read contents
				contents = malloc(sizeof(unsigned char) * fileSize);
				fread(contents, sizeof(unsigned char), fileSize, stdin);
				checkError();

				//Write file contents
				fp = fopen(name, "w");
				
				//Ensure file was able to be created
				if(fp == NULL)
				{
					perror(name);
					free(name);
					free(fileTimes);
					freeTrees(inodes, dirModes, dirTimes);
					exit(1);
				}

				fwrite(contents, sizeof(unsigned char), fileSize, fp);
				fclose(fp);
				
				//Set mode
				chmod(name, mode);

				//Set file time
				utimes(name, fileTimes);

				free(contents);
				free(fileTimes);
			}

			jrb_insert_int(inodes, inode, new_jval_s(strdup(name)));
		}

		//Create hard link
		else
		{
			JRB oldFile = jrb_find_int(inodes, inode);
			link(oldFile->val.s, name);
		}

		free(name);
	}

	//Set directory modes after all files have been created
	jrb_traverse(tmp, dirModes)
	{
		chmod(tmp->key.s, tmp->val.l); 
		free(tmp->key.s);
	}

	//Set directory times after all files have been created
	jrb_traverse(tmp, dirTimes)
	{
		utimes(tmp->key.s, tmp->val.v);
		free(tmp->key.s);
		free((struct timeval *)tmp->val.v);
	}

	//Free file names
	jrb_traverse(tmp, inodes)
		free(tmp->val.s);

	jrb_free_tree(inodes);
	jrb_free_tree(dirModes);
	jrb_free_tree(dirTimes);
	
	return 0;
}

//Checks for an error (including early EOF)
void checkError()
{
	if(ferror(stdin) != 0 || feof(stdin) != 0)
	{
		fprintf(stderr, "tarx: Error reading tarfile\n");
		exit(1);
	}
}

//Checks for an error or returns if EOF has been reached
bool checkEOF()
{
	if(ferror(stdin) != 0)
	{
		fprintf(stderr, "tarx: Error reading tarfile\n");
		exit(1);
	}

	return feof(stdin) != 0;
}

//Frees the memory of the JRB trees
void freeTrees(JRB inodes, JRB dirModes, JRB dirTimes)
{
	JRB tmp;

	jrb_traverse(tmp, inodes)
		free(tmp->val.s);
	
	jrb_traverse(tmp, dirModes)
		free(tmp->key.s);

	jrb_traverse(tmp, dirTimes)
	{
		free(tmp->key.s);
		free((struct timeval *)tmp->val.v);
	}

	jrb_free_tree(inodes);
	jrb_free_tree(dirModes);
	jrb_free_tree(dirTimes);
}
