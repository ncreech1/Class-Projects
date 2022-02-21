#include "b_tree.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* Nicholas Creech
 * CS494 Lab3 - BTree on disk
 * 11/02/21
 */ 

typedef struct tnode
{
	unsigned char bytes[JDISK_SECTOR_SIZE + 256]; //Sector for r/w (extra room for extra key)
	unsigned char numKeys; //Number of keys in the node
	unsigned char flush; //Flag to flush disk at end of b_tree_insert()
	unsigned char internal; //Internal or external node
	unsigned int lba; //LBA when the node is flushed
	unsigned char **keys; //Pointers to the keys (size = MAXKEY + 1)
	unsigned char **lbas; //Pointer to the LBAs (size = MAXKEY + 2)
	unsigned int *lbaVals; //Used to store the LBAs before a split
	struct tnode *parent; //Pointer to this node's parent (useful for splitting)
	int parentIndex; //Index of this node in parent
	struct tnode *rightSplit; //Link to right node in split
} TreeNode;

typedef struct 
{
	int keySize; //keySize, rootLBA, and firstFreeBlock are the first 16/12 bytes in sector 0
	unsigned int rootLBA;
	unsigned long firstFreeBlock;
	void *disk; //The jdisk
	unsigned long size; //The jdisk's size
	unsigned long numLBA; //size / JDISK_SECTOR_SIZE
	int keysPerBlock; //MAXKEY
	int lbasPerBlock; //MAXKEY + 1
	TreeNode **freeList; //Free list of nodes
	TreeNode *tmpExt; //When find() fails, this is a pointer to the external node
	int tmpExtIndex; //The index the key should have gone
	int flush; //Flag to flush sector[0] to disk after b_tree_insert()
} BTree;

//Memory management functions
TreeNode * allocateNode(BTree *tree, unsigned int lba); 
void addToFreeList(BTree *tree, TreeNode *node);
TreeNode * getFromFreeList(BTree *tree);
void freeNodes(BTree *tree);

//BTree helper functions
int insertNormal(BTree *tree, TreeNode *node, int index, void *key, unsigned int lba);
int splitNode(BTree *tree, TreeNode *node);
int treeFull(BTree *tree);
void b_tree_print(void *b_tree, unsigned int lba);

//Creates a new, empty BTree disk
void * b_tree_create(char *filename, long size, int key_size)
{
	BTree *tree;
	TreeNode *root;
	int totalFreeNodes, x;

	//Initialize new BTree
	tree = (BTree*)malloc(sizeof(BTree));
	tree->keySize = key_size;
	tree->rootLBA = 1;
	tree->firstFreeBlock = 2;
	tree->disk = jdisk_create(filename, size);
	tree->size = size;
	tree->numLBA = size / JDISK_SECTOR_SIZE;
	tree->keysPerBlock = (JDISK_SECTOR_SIZE - 6) / (key_size + 4);
	tree->lbasPerBlock = tree->keysPerBlock + 1;
	tree->flush = 0;
	
	//Create free list
	totalFreeNodes = (size - JDISK_SECTOR_SIZE) / JDISK_SECTOR_SIZE;
	tree->freeList = (TreeNode**)malloc(totalFreeNodes * sizeof(TreeNode*));
	for(x = 0; x < totalFreeNodes; x++)
		tree->freeList[x] = NULL;

	//Allocate empty root node
	root = allocateNode(tree, 1);
	root->internal = 0;
	root->numKeys = 0;
	root->bytes[0] = 0;
	root->bytes[1] = 0;
	
	x = 0;
	memcpy(root->lbas[0], &x, 4);
	
	//Write empty tree to disk
	jdisk_write(tree->disk, 0, tree);
	jdisk_write(tree->disk, root->lba, root->bytes); 

	free(root->keys);
	free(root->lbas);
	free(root);

	return tree;
}

//Attaches to an exisitng BTree disk
void * b_tree_attach(char *filename)
{
	BTree *tree;
	unsigned char currSector[JDISK_SECTOR_SIZE];
	int totalFreeNodes, x;

	//Read in disk
	tree = (BTree*)malloc(sizeof(BTree));
	tree->disk = jdisk_attach(filename);
	
	//Read first sector from disk
	jdisk_read(tree->disk, 0, currSector);

	//Initialize BTree from disk
	memcpy(&tree->keySize, currSector, sizeof(unsigned int));
	memcpy(&tree->rootLBA, currSector + 4, sizeof(unsigned int));
	memcpy(&tree->firstFreeBlock, currSector + 8, sizeof(unsigned long));
	tree->size = jdisk_size(tree->disk);
	tree->numLBA = tree->size / JDISK_SECTOR_SIZE;
	tree->keysPerBlock = (JDISK_SECTOR_SIZE - 6) / (tree->keySize + 4);
	tree->lbasPerBlock = tree->keysPerBlock + 1;
	tree->flush = 0;

	//Create free list
	totalFreeNodes = (tree->size - JDISK_SECTOR_SIZE) / JDISK_SECTOR_SIZE;
	tree->freeList = (TreeNode**)malloc(totalFreeNodes * sizeof(TreeNode*));
	for(x = 0; x < totalFreeNodes; x++)
		tree->freeList[x] = NULL;

	return tree;
}

//Inserts a key in the BTree with the given record data
unsigned int b_tree_insert(void *b_tree, void *key, void *record)
{
	TreeNode *currNode, *tmp;
	BTree *tree;
	int nextLBA;
	unsigned char *insert;
	
	//Find key in tree (or the expected location)
	tree = (BTree*)b_tree;
	nextLBA = b_tree_find(b_tree, key);

	//Key already in tree; overwrite record
	if(nextLBA != 0)
	{
		jdisk_write(tree->disk, nextLBA, record);
		return nextLBA;
	}

	//Attempt to insert key at expected location
	else
	{
		//Insert key (if possible)
		if(!insertNormal(tree, tree->tmpExt, tree->tmpExtIndex, key, tree->firstFreeBlock))
			return 0;

		//Get LBA of record
		nextLBA = tree->firstFreeBlock;
		tree->firstFreeBlock++;

		//Split node if needed
		if(tree->tmpExt->numKeys == tree->keysPerBlock + 1)
		{
			if(!splitNode(tree, tree->tmpExt))
				return 0;
		}
				
		//Write record
		jdisk_write(tree->disk, nextLBA, record);

		//Flush tree nodes
		currNode = tree->tmpExt;
		while(currNode != NULL)
		{
			if(currNode->flush)
			{
				jdisk_write(tree->disk, currNode->lba, currNode->bytes);

				//Write right node of split
				if(currNode->rightSplit != NULL)
					jdisk_write(tree->disk, currNode->rightSplit->lba, currNode->rightSplit->bytes);
			}
				
			tmp = currNode;
			currNode = currNode->parent;
			addToFreeList(tree, tmp);
		}

		//Flush tree
		jdisk_write(tree->disk, 0, tree);

		freeNodes(tree);
		return nextLBA;
	}

	return 0;
}

//Splits a node recursively
int splitNode(BTree *tree, TreeNode *node)
{
	TreeNode *rightNode;
	unsigned char *src;
	int middleIndex;

	//Check if tree is full
	if(treeFull(tree))
		return 0;

	//Split node (current node becomes left node, right node is new)
	rightNode = allocateNode(tree, tree->firstFreeBlock);
	tree->firstFreeBlock++;
	node->flush = 1;

	//Odd split
	if(node->numKeys % 2 == 0)
	{
		middleIndex = node->numKeys / 2 - 1;
		rightNode->bytes[1] = middleIndex + 1;
		rightNode->numKeys = middleIndex + 1;
		node->bytes[1] = rightNode->numKeys - 1;
		node->numKeys = rightNode->numKeys - 1;
	}
	
	//Even split
	else
	{
		middleIndex = node->numKeys / 2;
		rightNode->bytes[1] = middleIndex;
		rightNode->numKeys = middleIndex;
		node->bytes[1] = middleIndex;
		node->numKeys = middleIndex;
	}

	//Initialize right node
	src = node->keys[middleIndex + 1];
	memcpy(rightNode->keys[0], src, tree->keySize * rightNode->numKeys);
	src = node->lbas[middleIndex + 1];
	memcpy(rightNode->lbas[0], src, 4 * (rightNode->numKeys + 1));
	rightNode->internal = node->internal;
	rightNode->bytes[0] = node->bytes[0];
	rightNode->flush = 1;
	node->rightSplit = rightNode;

	//Current node is root; set parent to new root
	if(node->parent == NULL)
	{
		tree->rootLBA = tree->firstFreeBlock;
		tree->firstFreeBlock++;
		node->parent = allocateNode(tree, tree->rootLBA);
		
		//Have to manually set node data since tree has not been written yet
		node->parentIndex = 0;
		node->parent->bytes[0] = 1;
		node->parent->bytes[1] = 0;
		node->parent->internal = 1;
		node->parent->numKeys = 0;
		node->parent->flush = 1;

		//Link parent to left node
		memcpy(node->parent->lbas[node->parentIndex], &node->lba, 4);
	}

	//Copy stored LBAs to left and right nodes
	memcpy(node->lbas[0], node->lbaVals, (node->numKeys + 1) * 4);
	memcpy(rightNode->lbas[0], node->lbaVals + node->numKeys + 1, (rightNode->numKeys + 1) * 4); 
	free(node->lbaVals);

	//Kick middle key up to parent
	if(!insertNormal(tree, node->parent, -1, node->keys[middleIndex], node->lba))
		return 0;

	//Split parent if needed
	if(node->parent->numKeys == tree->keysPerBlock + 1)
	{
		//Link parent to right node in saved LBAs for split
		node->parent->lbaVals[node->parentIndex + 1] = rightNode->lba;
		return splitNode(tree, node->parent);
	}

	//Link parent to right node normally
	else
		memcpy(node->parent->lbas[node->parentIndex + 1], &rightNode->lba, 4);
	
	return 1;
}

//Performs a normal key insert with an LBA at a specfic node
//Inserts at index if given, or finds index if index == -1
int insertNormal(BTree *tree, TreeNode *node, int index, void *key, unsigned int lba)
{
	unsigned char *insert;
	int x, cmp;

	//Check if tree is full
	if(treeFull(tree))
		return 0;

	//Find index for key in node
	if(index == -1)
	{
		//Search this node's keys
		for(x = 0; x < node->numKeys; x++)
		{
			cmp = memcmp(key, node->keys[x], tree->keySize);
			
			//Search key is less than current key
			if(cmp < 0)
				break;
		}

		index = x;
	}

	//Insert at end of node
	if(index == node->numKeys)
	{
		//Insert new LBA
		memcpy(node->lbas[index + 1], node->lbas[index], 4); 
		memcpy(node->lbas[index], &lba, 4);

		//Copy LBAs for split
		if(node->numKeys == tree->keysPerBlock)
		{
			node->lbaVals = (int*)malloc((node->numKeys + 2) * 4);
			memcpy(node->lbaVals, node->lbas[0], (node->numKeys + 2) * 4);
		}
		
		//Insert new key
		memcpy(node->keys[index], key, tree->keySize);
	}

	//Insert between existing keys
	else
	{	
		//Shift LBAs over one and insert new LBA
		insert = node->lbas[index];
		memcpy(insert + 4, insert, (node->numKeys - index + 1) * 4);
		memcpy(insert, &lba, 4);

		//Copy LBAs for split
		if(node->numKeys == tree->keysPerBlock)
		{
			node->lbaVals = (int*)malloc((node->numKeys + 2) * 4);
			memcpy(node->lbaVals, node->lbas[0], (node->numKeys + 2) * 4);
		}
	
		//Shift keys over one and insert new key
		insert = node->keys[index];
		memcpy(insert + tree->keySize, insert, tree->keySize * (node->numKeys - index));  
		memcpy(insert, key, tree->keySize);
	}

	//Update the node info
	node->bytes[1]++;
	node->numKeys++;
	node->flush = 1;

	return 1;
}

//Returns if the tree is full (no more free blocks)
int treeFull(BTree *tree)
{
	return tree->firstFreeBlock > (tree->size - JDISK_SECTOR_SIZE) / JDISK_SECTOR_SIZE;
}

//Finds a key in the BTree
unsigned int b_tree_find(void *b_tree, void *key)
{
	TreeNode *currNode, *tmp;
	BTree *tree;
	int x, cmp, nextLBA, foundKey;

	tree = (BTree*)b_tree;
	currNode = allocateNode(tree, tree->rootLBA);
	foundKey = 0;

	while(1)
	{
		//Search this node's keys
		for(x = 0; x < currNode->numKeys; x++)
		{
			cmp = memcmp(key, currNode->keys[x], tree->keySize);
			
			//Search key is less than current key
			if(cmp < 0)
				break;
			//Search key is equal to current key
			else if(cmp == 0)
			{
				foundKey = 1;
				break;
			}
		}

		//Key not found; no more nodes 
		if(!foundKey && !currNode->internal)
		{
			tree->tmpExt = currNode;
			tree->tmpExtIndex = x;
			break;
		}

		//Key found
		if(foundKey)
		{
			//Key found on external node; LBA is left of key
			if(!currNode->internal)
			{
				memcpy(&nextLBA, currNode->lbas[x], 4);
				addToFreeList(tree, currNode);
				freeNodes(tree);
				return nextLBA;
			}

			//Go left to next node first
			memcpy(&nextLBA, currNode->lbas[x], 4);
			addToFreeList(tree, currNode);
			currNode = allocateNode(tree, nextLBA);

			//Follow the last LBA of each node until an external node
			while(currNode->internal)
			{
				memcpy(&nextLBA, currNode->lbas[currNode->numKeys], 4);
				addToFreeList(tree, currNode);
				currNode = allocateNode(tree, nextLBA);
			}

			//Get right-most LBA
			memcpy(&nextLBA, currNode->lbas[currNode->numKeys], 4);
			addToFreeList(tree, currNode);
			freeNodes(tree);
			return nextLBA;
		}

		//Key not found; check next node
		memcpy(&nextLBA, currNode->lbas[x], 4);
		tmp = currNode;
		currNode = allocateNode(tree, nextLBA);
		currNode->parent = tmp;
		currNode->parentIndex = x;
	}

	freeNodes(tree);
	return 0;
}

//Returns the BTree disk handle
void * b_tree_disk(void *b_tree)
{
	return ((BTree*)b_tree)->disk;
}

//Returns the BTree disk size (in bytes)
int b_tree_key_size(void *b_tree)
{
	return ((BTree*)b_tree)->keySize;
}

//Prints the BTree by calling b_tree_print()
void b_tree_print_tree(void *b_tree) 
{
	BTree *tree;

	tree = (BTree*)b_tree;
	b_tree_print(b_tree, tree->rootLBA);
}

//Recursively prints the BTree
void b_tree_print(void *b_tree, unsigned int lba)
{
	TreeNode *currNode;
	BTree *tree;
	unsigned int x, keyLBA;

	tree = (BTree*)b_tree;
	currNode = allocateNode(tree, lba);

	printf("Node at LBA %u:\n", lba);

	for(x = 0; x < currNode->numKeys; x++)
		printf("     Key %d: %s\n", x, currNode->keys[x]);
	
	if(currNode->internal)
	{
		for(x = 0; x < currNode->numKeys + 1; x++)
		{
			memcpy(&keyLBA, currNode->bytes + JDISK_SECTOR_SIZE - tree->lbasPerBlock * 4 + x * 4, 4);
			b_tree_print(b_tree, keyLBA); 
		}
	}

	else
		printf("     These keys point to data!\n");

	free(currNode->keys);
	free(currNode->lbas);
	free(currNode);
}

//Adds a node to the free list
void addToFreeList(BTree *tree, TreeNode *node)
{
	int x, totalFreeNodes;

	totalFreeNodes = (tree->size - JDISK_SECTOR_SIZE) / JDISK_SECTOR_SIZE;
	for(x = 0; x < totalFreeNodes; x++)
	{
		//Add node to free list
		if(tree->freeList[x] == NULL)
		{
			tree->freeList[x] = node;
			break;
		}

		//No room in list; free node
		else if(x == totalFreeNodes - 1)
		{
			free(node->keys);
			free(node->lbas);
			free(node);
		}
	}
}

//Attempts to retrieve a node from the free list
TreeNode * getFromFreeList(BTree *tree)
{
	TreeNode *found;
	int x, totalFreeNodes;

	totalFreeNodes = (tree->size - JDISK_SECTOR_SIZE) / JDISK_SECTOR_SIZE;
	for(x = 0; x < totalFreeNodes; x++)
	{
		//Get node from free list
		if(tree->freeList[x] != NULL)
		{
			found = tree->freeList[x];
			tree->freeList[x] = NULL;
			return found;
		}
	}

	return NULL;
}

//Frees all the nodes on the free list
void freeNodes(BTree *tree)
{
	TreeNode *found;
	int x, totalFreeNodes;

	totalFreeNodes = (tree->size - JDISK_SECTOR_SIZE) / JDISK_SECTOR_SIZE;
	for(x = 0; x < totalFreeNodes; x++)
	{
		//Delete node from free list
		if(tree->freeList[x] != NULL)
		{
			free(tree->freeList[x]->keys);
			free(tree->freeList[x]->lbas);
			free(tree->freeList[x]);
			tree->freeList[x] = NULL;
		}
	}

}

//Allocates a node by reading its LBA on the disk
TreeNode * allocateNode(BTree *tree, unsigned int lba)
{
	TreeNode *newNode;
	int x;

	//Attempt to retrieve node from free list
	newNode = getFromFreeList(tree);

	//Malloc node if none on free list
	if(newNode == NULL)
	{
		newNode = (TreeNode*)malloc(sizeof(TreeNode));
		
		//Allocate keys and assign to locations in bytes array
		newNode->keys = (unsigned char**)malloc((tree->keysPerBlock + 1) * sizeof(unsigned char*));
		for(x = 0; x < tree->keysPerBlock + 1; x++)
			newNode->keys[x] = newNode->bytes + 2 + x * tree->keySize;

		//Allocate lbas and assign to locations in bytes array
		newNode->lbas = (unsigned char**)malloc((tree->keysPerBlock + 2) * sizeof(unsigned char*));
		for(x = 0; x < tree->keysPerBlock + 2; x++)
			newNode->lbas[x] = newNode->bytes + JDISK_SECTOR_SIZE - tree->lbasPerBlock * 4 + x * 4;

	}
	
	//Read sector for this node and initialize
	jdisk_read(tree->disk, lba, newNode->bytes);
	newNode->numKeys = newNode->bytes[1];
	newNode->internal = newNode->bytes[0];
	newNode->lba = lba;
	newNode->rightSplit = NULL;
	newNode->parent = NULL;
	newNode->flush = 0;
	
	return newNode;
}
