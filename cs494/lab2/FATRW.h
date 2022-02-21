#pragma once

class FATRW
{
	public:
		void *disk; //The FAT formatted disk in question
        unsigned char *buf; //The buffer to write to one sector
		unsigned long diskSize; //The disk size in bytes
		unsigned int numDS, numFS; //Number of data sectors and fat sectors
		unsigned int numFB, numDB; //Number of fat bytes and data bytes
		unsigned char **fatSectors; //Pointers to each fat sector
		int *writeFlags; //Array of flags for writing sectors (1 = write, 0 = nowrite)
		bool init(char *dn);
		void terminate();
		bool importFile(char *fn);
		bool exportFile(char *fn, char *sb);
		unsigned short readLink(unsigned short sector);
		void writeLink(unsigned short sector, unsigned short val);
		void flushLinks();
};
