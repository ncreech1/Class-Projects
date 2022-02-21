#include <vector>
#include <string>
#include <iostream>
#include <cstdio>
#include "bstree.hpp"
using namespace std;
using CS140::BSTree;
using CS140::BSTNode;

/* Nicholas Creech
 * 11/23/19
 * Lab A: Implement functions that balance a binary tree and calculate node heights and depths
 */ 

//Returns the distance of the node with the given key from the root
int BSTree::Depth(const string &key) const
{
	int depth = 0;

	//If key can't be found, depth is -1
	if(Find(key) == NULL)	
		return -1;
	else
	{
		BSTNode* n;
		n = sentinel->right;

		//Try to find the node by its key, increasing depth after each node
		while(1)
		{
			if(key == n->key)
				break;
			n = (key < n->key) ? n->left : n->right;
			depth++;
		}
	}

	return depth;
}
 
//Returns the depth of the node with the maximum depth, plus one
int BSTree::Height() const
{
	return recursive_find_height(sentinel->right);
}

//Returns a vector of the tree's keys in order
vector <string> BSTree::Ordered_Keys() const
{
	vector <string> rv;
	
	make_key_vector(sentinel->right, rv);

	return rv;
}
 
//Copy constructor 
BSTree::BSTree(const BSTree &t)        
{
	//Set up sentinel node like the regular constructor
	sentinel = new BSTNode;
	sentinel->parent = NULL;
	sentinel->left = NULL;
	sentinel->right = sentinel;
	sentinel->key = "---SENTINEL---";
	sentinel->val = NULL;
	size = 0;

	//Use assignment overload to create the tree
	*this = t;
}

//Assignment overload
BSTree& BSTree::operator= (const BSTree &t) 
{
	Clear();
	vector<string> keys = t.Ordered_Keys();
	vector<void*> vals = t.Ordered_Vals();

	//Use the recursive balancing function to create a new balanced tree with the nodes from BSTree t 
	BSTNode* root = make_balanced_tree(keys, vals, 0, keys.size());
	sentinel->right = root;
	root->parent = sentinel;
	size = t.Size(); //Sizes should be the same

	return *this;
}

int BSTree::recursive_find_height(const BSTNode *n) const
{
	//End Case: Current node has no children (bottom of tree reached)
	if(n->left == sentinel && n->right == sentinel)
		return 1;
	else if(n->left != sentinel && n->right == sentinel)
		return 1 + recursive_find_height(n->left);
	else if(n->right != sentinel && n->left == sentinel)
		return 1 + recursive_find_height(n->right);
	else
	{
		//Current node has two children; pick the one with the larger height
		int left = 1 + recursive_find_height(n->left);
		int right = 1 + recursive_find_height(n->right);

		if(left >= right)
			return left;
		else if(right >= left)
			return right;
	}

	return -1;
}

//Creates the key vector using an in-order traversal
void BSTree::make_key_vector(const BSTNode *n, vector<string> &v) const
{
	//End Case: Sentinel reached; at leaf node
	if(n == sentinel)
		return;
	make_key_vector(n->left, v); //Traverse all of left first
	v.push_back(n->key);
	make_key_vector(n->right, v); //Then traverse all of right
}

BSTNode *BSTree::make_balanced_tree(const vector<string> &sorted_keys, const vector<void *> &vals, size_t first_index, size_t num_indices) const
{
	//The root of the current subtree
	BSTNode* root = new BSTNode();

	//End Case: No more indices (num_indices == 1); set up sentinel pointers
	if(num_indices == 1)
	{
		root->key = sorted_keys[first_index];
		root->val = vals[first_index];

		root->right = sentinel;
		root->left = sentinel;
		return root;
	}

	else
	{
		//Find the middle of the current keys subset (becomes root of new subtree)
		int middle = num_indices / 2;
		root->key = sorted_keys[first_index + middle];
		root->val = vals[first_index + middle];

		//Create left branch recursively 
		BSTNode* leftChild = make_balanced_tree(sorted_keys, vals, first_index, middle);
		
		BSTNode* rightChild;
		if(num_indices % 2 != 0) //If the tree has an odd number of nodes, the next num_indices equals middle
			rightChild = make_balanced_tree(sorted_keys, vals, first_index + middle + 1, middle);
		else if(num_indices != 2) //If the tree has an even number of nodes, the next num_indices equals (middle - 1)
			rightChild = make_balanced_tree(sorted_keys, vals, first_index + middle + 1, middle - 1);

		//Connect the left branch to the root
		leftChild->parent = root;
		root->left = leftChild;

		//The root has a right branch only if the tree has an odd number of nodes, or the last two nodes are being balanced 
		if(num_indices % 2 != 0 || num_indices != 2)
		{
			rightChild->parent = root;
			root->right = rightChild;
		}

		else
			root->right = sentinel;

		return root;
	}
}
