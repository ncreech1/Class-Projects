#include <cstdio>
#include <cstring>
#include <stdlib.h>
#include <unistd.h>
#include "FATRW.h"

extern "C" 
{
#include "jdisk.h"
}

/* Nicholas Creech
 * CS494 Lab2 - FAT
 * 10/11/21
 */ 

using namespace std; 

void printUsage();

int main(int argc, char **argv)
{
	FATRW frw;
	char *flag;

	if(argc != 4 && argc != 5)
	{
		printUsage();
		return -1;
	}

	//Initialize the fatrw disk
	if(!frw.init(argv[1]))
		return -1;

	//Import a file to FAT
	if(strcmp(argv[2], "import") == 0)
	{
		if(!frw.importFile(argv[3]))
		{
			frw.terminate();
			return -1;
		}
	}

	//Export a file from FAT
	else if(strcmp(argv[2], "export") == 0)
	{
		if(!frw.exportFile(argv[4], argv[3]))
		{
			frw.terminate();
			return -1;
		}
	}

	else
	{
		printUsage();
		return -1;
	}

	frw.terminate();
	return 0;
}

//Prints the usage for FATRW
void printUsage()
{
	printf("usage: ./FATRW diskfile import input-file\n");
	printf("       ./FATRW diskfile export starting-block output-file\n");
}

//Intializes the disk and determines the data and fat sector sizes
bool FATRW::init(char *dn)
{
	disk = jdisk_attach(dn);
	buf = (unsigned char*)malloc(JDISK_SECTOR_SIZE);

	if(disk == NULL)
	{
		fprintf(stderr, "Error opening disk file\n");
		free(buf);
		return false;
	}

	diskSize = jdisk_size(disk);
	numDS = diskSize / JDISK_SECTOR_SIZE;

	while(1)
	{
		numFB = (numDS + 1) * 2;

		//Determine fat sector size
		if(numFB % JDISK_SECTOR_SIZE != 0)
			numFS = numFB / JDISK_SECTOR_SIZE + 1;
		else 
			numFS = numFB / JDISK_SECTOR_SIZE;

		//D + S <= total sectors
		if(numFS + numDS <= diskSize / JDISK_SECTOR_SIZE)
			break;
		
		numDS--;
	}

	numDB = numDS * JDISK_SECTOR_SIZE;
	fatSectors = (unsigned char**)malloc(numFS * sizeof(char*));
	writeFlags = (int*)calloc(numFS, sizeof(int));

	for(int x = 0; x < numFS; x++)
		fatSectors[x] = NULL;

	return true;
}

//Frees the memory associated with this FATRW
void FATRW::terminate()
{
	for(int x = 0; x < numFS; x++)
	{
		if(fatSectors[x] != NULL)
			free(fatSectors[x]);
	}

	free(fatSectors);
	free(writeFlags);
	free(buf);

	jdisk_unattach(disk);
}

//Attempts to import a file to the disk
bool FATRW::importFile(char *fn)
{
	FILE *file;
	unsigned long fileSize;
	unsigned short currLinkVal;
	unsigned int totalFreeSectors;
	unsigned int requiredSectors;
	
	file = fopen(fn, "rb");

	if(file == NULL)
	{
		printf("Error opening input file\n");
		return false;
	}

	fileSize = lseek(fileno(file), 0, SEEK_END);
	lseek(fileno(file), 0, SEEK_SET);

	if(fileSize > diskSize)
	{
		fprintf(stderr, "%s is too big for the disk (%lu vs %lu)\n", fn, fileSize, diskSize);
		return false;
	}

	//Read the first sector
	fatSectors[0] = (unsigned char*)malloc(JDISK_SECTOR_SIZE);
	jdisk_read(disk, 0, fatSectors[0]);
	
	//Calculate the number of required sectors for the file
	if(fileSize % JDISK_SECTOR_SIZE == 0)
		requiredSectors = fileSize / 1024;
	else
		requiredSectors = fileSize / 1024 + 1;
	
	//Read first link
	currLinkVal = readLink(0);
	totalFreeSectors = 0;
	
	//Count the free sectors
	while(currLinkVal != 0)
	{
		currLinkVal = readLink(currLinkVal);
		totalFreeSectors++;

		//Only count to minimum needed
		if(totalFreeSectors == requiredSectors)
			break;
	}

	//Check if there are enough free sectors for the file
	if(requiredSectors <= totalFreeSectors)
	{
		currLinkVal = 0;
		
		//Traverse free sectors to write the file
		for(int x = 0; x < requiredSectors; x++)
		{
			//Advance to next free sector and read file sector
			currLinkVal = readLink(currLinkVal);
			fread(buf, 1, JDISK_SECTOR_SIZE, file);

			//Write current sector of the file
			jdisk_write(disk, currLinkVal + numFS - 1, buf); 

			if(x == 0)
				printf("New file starts at sector %hu\n", currLinkVal + numFS - 1);
		}

		//Write new start of free sectors at first two bytes of FAT
		writeLink(0, readLink(currLinkVal));

		//File takes up all of last sector
		if(fileSize % JDISK_SECTOR_SIZE == 0)
		{
			writeLink(currLinkVal, 0);
			jdisk_write(disk, currLinkVal + numFS - 1, buf);
		}

		//File takes up part of last sector
		else
		{	
			if(fileSize % JDISK_SECTOR_SIZE == JDISK_SECTOR_SIZE - 1)
				buf[JDISK_SECTOR_SIZE - 1] = 0xFF;
			else
			{
				buf[JDISK_SECTOR_SIZE - 1] = (fileSize % JDISK_SECTOR_SIZE) >> 8 & 0xFF;
				buf[JDISK_SECTOR_SIZE - 2] = (fileSize % JDISK_SECTOR_SIZE) & 0xFF;
			}

			writeLink(currLinkVal, currLinkVal);
			jdisk_write(disk, currLinkVal + numFS - 1, buf);
		}

		//Write FAT changes to disk
		flushLinks();

		printf("Reads: %ld\n", jdisk_reads(disk));
		printf("Writes: %ld\n", jdisk_writes(disk));
	}

	else
	{
		fprintf(stderr, "Not enough free sectors (%lu) for %s, which needs %lu.\n", totalFreeSectors, fn, requiredSectors);
		return false;
	}

	fclose(file);	
	return true;
}

//Attempts to export a file from the disk
bool FATRW::exportFile(char *fn, char *sb)
{
	FILE *file;
	unsigned int startBlock;
	unsigned short currLinkVal;
	unsigned short endLength;
	
	file = fopen(fn, "wb");

	if(file == NULL)
	{
		fprintf(stderr, "Error opening output file\n");
		return false;
	}

	startBlock = atoi(sb);

	//Check if starting block is valid
	if(startBlock >= numFS && startBlock < diskSize / JDISK_SECTOR_SIZE)
	{
		currLinkVal = startBlock - numFS + 1;
		
		//Follow links for the file
		while(1)
		{
			//Read the current data sector
			jdisk_read(disk, currLinkVal + numFS - 1, buf);
		
			//File takes up all of last sector
			if(readLink(currLinkVal) == 0)
			{
				fwrite(buf, 1, JDISK_SECTOR_SIZE, file);
				break;
			}

			//File takes up part of last sector
			else if(readLink(currLinkVal) == currLinkVal)
			{
				endLength = 0;
				endLength |= buf[JDISK_SECTOR_SIZE - 1];
				
				if(endLength != 0xFF)
					endLength = endLength << 8 | buf[JDISK_SECTOR_SIZE - 2];
				else
					endLength = JDISK_SECTOR_SIZE - 1;
				
				fwrite(buf, 1, endLength, file);
				break;
			}

			//Not at last sector; export current data sector to file
			fwrite(buf, 1, JDISK_SECTOR_SIZE, file);
			currLinkVal = readLink(currLinkVal);
		}
	}

	printf("Reads: %ld\n", jdisk_reads(disk));
	printf("Writes: %ld\n", jdisk_writes(disk));

	fclose(file);
	return true;
}

//Returns the value of the link with the given linkID
unsigned short FATRW::readLink(unsigned short linkID)
{
	unsigned short val, linkSector;

	linkSector = linkID * 2 / JDISK_SECTOR_SIZE; //The fat sector this link is in

	//Read the link's fat sector if it has not been read before
	if(fatSectors[linkSector] == NULL)
	{
		fatSectors[linkSector] = (unsigned char*)malloc(JDISK_SECTOR_SIZE);
		jdisk_read(disk, linkSector, fatSectors[linkSector]);
	}

	memcpy(&val, fatSectors[linkSector] + (linkID * 2 - linkSector * JDISK_SECTOR_SIZE), 2);
	return val;
}

//Sets the value of a link for a given linkID
void FATRW::writeLink(unsigned short linkID, unsigned short val)
{
	unsigned short linkSector;

	linkSector = linkID * 2 / JDISK_SECTOR_SIZE; //The fat sector this link is in

	//Read the link's fat sector if it has not been read before
	if(fatSectors[linkSector] == NULL)
	{
		fatSectors[linkSector] = (unsigned char*)malloc(JDISK_SECTOR_SIZE);
		jdisk_read(disk, linkSector, fatSectors[linkSector]);
	}

	//Only write if the new value is different
	if(readLink(linkID) != val)
	{
		memcpy(fatSectors[linkSector] + (linkID * 2 - linkSector * JDISK_SECTOR_SIZE), &val, 2);
		writeFlags[linkSector] = 1;
	}
}

//Writes the modified fat sectors to the disk
void FATRW::flushLinks()
{
	for(unsigned int x = 0; x < numFS; x++)
	{
		if(writeFlags[x])
			jdisk_write(disk, x, fatSectors[x]); 
	}
}
