#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <dirent.h>
#include <sys/stat.h>
#include "jrb.h"
#include "dllist.h"
#include "jval.h"

/* Nicholas Creech
 * 10/8/20
 * Lab 4 Part 1: Write a program called tarc that takes a directory and 
 * writes inode info for all its files to stdout (like a tar file)
 */ 

void addDir(char *prefix, char *path, JRB inodes);
void writeFileInfo(char *fullPath, char *dePath, struct stat buf, JRB inodes, Dllist dirs);

int main(int argc, char * argv[])
{
	JRB inodes;
	char *path;
	char *prefix;
	char *pos;

	if(argc != 2)
	{
		printf("usage: ./tarc <directory>\n");
		return 1;
	}

	inodes = make_jrb();
	pos = strrchr(argv[1], '/');

	//Get prefix and suffix from directory path
	if(pos != NULL)
	{
		path = malloc(strlen(pos));
		prefix = malloc(strlen(argv[1]) - strlen(pos) + 2);
		
		memcpy(prefix, argv[1], strlen(argv[1]) - strlen(pos) + 1);
		memcpy(path, pos + 1, strlen(pos));
		
		prefix[strlen(prefix)] = '\0';
		path[strlen(path)] = '\0';
	}

	else
	{
		prefix = strdup("\0");
		path = strdup(argv[1]);
	}

	//Recursively search directory
	addDir(prefix, path, inodes);

	free(path);
	free(prefix);
	jrb_free_tree(inodes);
	return 0;
}

//Recursive function that searches a directory and writes info for all files
void addDir(char * prefix, char * path, JRB inodes)
{
	DIR *dp;
	struct dirent *de;
	struct stat buf;
	int exists;
	char * dePath;
	Dllist dirs;
	Dllist tmp;
	char * fullPath;

	//Build full path from prefix
	fullPath = malloc(strlen(prefix) + strlen(path) + 1);
	strcpy(fullPath, prefix);
	strcat(fullPath, path);

	dp = opendir(fullPath);

	//Failed to open directory
	if(dp == NULL)
	{
		perror("tarc");
		free(path);
		free(prefix);
		jrb_free_tree(inodes);
		exit(1);
	}

	dirs = new_dllist();
	dePath = malloc((strlen(path) + 258) * sizeof(char));

	//Write file info for initial directory
	if(jrb_empty(inodes))
	{
		exists = lstat(fullPath, &buf);

		if(exists < 0)
			fprintf(stderr, "Could not stat %s\n", path);
		else
			writeFileInfo(fullPath, path, buf, inodes, dirs);
	}

	free(fullPath);

	//Write file info for all files in the open directory
	for(de = readdir(dp); de != NULL; de = readdir(dp))
	{
		//Create path without prefix from file name
		sprintf(dePath, "%s/%s", path, de->d_name); 
		
		//Build full path from prefix and dePath
		fullPath = malloc(strlen(prefix) + strlen(dePath) + 1);
		strcpy(fullPath, prefix);
		strcat(fullPath, dePath);
		
		exists = lstat(fullPath, &buf);

		if(exists < 0)
			fprintf(stderr, "Could not stat %s\n", dePath);
		else
		{
			//Ignore "." and ".."
			if(strcmp(de->d_name, ".") != 0 && strcmp(de->d_name, "..") != 0)
				writeFileInfo(fullPath, dePath, buf, inodes, dirs);
		}

		free(fullPath);
	}

	closedir(dp);

	//Recursively search all unvisited directories in the current directory
	dll_traverse(tmp, dirs)
	{
		addDir(prefix, tmp->val.s, inodes);
		free(tmp->val.s);
	}

	free_dllist(dirs);
	free(dePath);
}

//Writes the inode info of a file (or directory) to stdout if it has not been visited
void writeFileInfo(char *fullPath, char *dePath, struct stat buf, JRB inodes, Dllist dirs)
{
	int nameSize;
				
	nameSize = strlen(dePath);

	//Print file name size, path, and inode
	fwrite(&nameSize, sizeof(int), 1, stdout);
	fwrite(dePath, sizeof(char), nameSize, stdout);
	fwrite(&buf.st_ino, sizeof(long), 1, stdout);

	//First time seeing this inode
	if(jrb_find_int(inodes, buf.st_ino) == NULL)
	{
		//Print file mode and last modified time
		fwrite(&buf.st_mode, sizeof(int), 1, stdout);
		fwrite(&buf.st_mtime, sizeof(long), 1, stdout);

		//Add directory to search list (do not add initial directory)
		if(S_ISDIR(buf.st_mode) && !jrb_empty(inodes))
			dll_append(dirs, new_jval_s(strdup(dePath)));	

		jrb_insert_int(inodes, buf.st_ino, JNULL);	
		
		//Not a directory
		if(!S_ISDIR(buf.st_mode))
		{
			FILE * file = fopen(fullPath, "r");
			unsigned char contents[buf.st_size];

			//Read file contents into buffer
			fread(&contents, sizeof(unsigned char), buf.st_size, file); 

			//Print file size and binary contents
			fwrite(&buf.st_size, sizeof(long), 1, stdout);
			fwrite(&contents, sizeof(unsigned char), buf.st_size, stdout);
			fclose(file);
		}
	}
}
