#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include "avltree.hpp"
using namespace std;
using CS140::AVLTree;
using CS140::AVLNode;

/* Nicholas Creech
 * 12/5/19
 * Lab B: Implement some of the functions required to build, copy, and manipulate AVL Search Trees
 */ 

bool imbalance(const AVLNode*);
void rotate(AVLNode*);
void fix_height(AVLNode*);
void fix_imbalance(AVLNode*);

AVLTree& AVLTree::operator= (const AVLTree &t)        
{
	//Empty tree before copying
	Clear();

	//Create the copy tree and get its root
	AVLNode* root = recursive_postorder_copy(t.sentinel->right);
	
	
	//Attach to new tree's sentinel
	root->key = t.sentinel->right->key;
	root->val = t.sentinel->right->val;
	root->height = t.sentinel->right->height;
	root->parent = sentinel;
	sentinel->right = root;
	size = t.Size();
	
	return *this;
}

//Inserts a new node into the tree and adjusts the height for each parent node until the root
bool AVLTree::Insert(const string &key, void *val)
{
	AVLNode *parent;
	AVLNode *n;

	parent = sentinel;
	n = sentinel->right;

	/* Find where the key should go.  If you find the key, return false. */

	while (n != sentinel) {
		if (n->key == key) return false;
		parent = n;
		n = (key < n->key) ? n->left : n->right;
	}

	/* At this point, parent is the node that will be the parent of the new node.
		Create the new node, and hook it in. */

	n = new AVLNode;
	n->key = key;
	n->val = val;
	n->parent = parent;
	n->height = 1;
	n->left = sentinel;
	n->right = sentinel;

	/* Use the correct pointer in the parent to point to the new node. */

	if (parent == sentinel) {
		sentinel->right = n;
	  } else if (key < parent->key) {
		parent->left = n;
	}	else {
		parent->right = n;
	}

	/* Increment the size */
	size++;
	
	//Adjust heights and check for imbalances, starting at the parent of the inserted node
	while(parent != sentinel)
	{
		fix_height(parent);
	
		//Check for imbalances
		if(imbalance(parent))
			fix_imbalance(parent);

		parent = parent->parent;
	}

	return true;
}
    
bool AVLTree::Delete(const string &key)
{
	AVLNode *n, *parent, *mlc;
	string tmpkey;
	void *tmpval;

	/* Try to find the key -- if you can't return false. */

	n = sentinel->right;
	while (n != sentinel && key != n->key) {
		n = (key < n->key) ? n->left : n->right;
	}
	if (n == sentinel) return false;

	/* We go through the three cases for deletion, although it's a little
	 different from the canonical explanation. */

	parent = n->parent;

	/* Case 1 - I have no left child.  Replace me with my right child.
	 Note that this handles the case of having no children, too. */

	if (n->left == sentinel) {
		if (n == parent->left) {
			parent->left = n->right;
		} else {
			parent->right = n->right;
		}
    if (n->right != sentinel) 
		n->right->parent = parent;
	delete n;
    size--;

	/* Case 2 - I have no right child.  Replace me with my left child. */

	} else if (n->right == sentinel) {
		if (n == parent->left) {
			parent->left = n->left;
		} else {
			parent->right = n->left;
		}
    n->left->parent = parent;
    delete n;
    size--;

	/* If I have two children, then find the node "before" me in the tree.
	 That node will have no right child, so I can recursively delete it.
     When I'm done, I'll replace the key and val of n with the key and
     val of the deleted node.  You'll note that the recursive call 
     updates the size, so you don't have to do it here. */

	} else {
		for (mlc = n->left; mlc->right != sentinel; mlc = mlc->right) ;
			tmpkey = mlc->key;
			tmpval = mlc->val;
			Delete(tmpkey);
			n->key = tmpkey;
			n->val = tmpval;

			//Start at deleted node because the node's key and val were overwritten to "delete" it
			parent = n;

			//Adjust heights and check for imbalances, starting at the deleted node
			while(parent != sentinel)
			{
				fix_height(parent);

				//Fix imbalances
				if(imbalance(parent))
					fix_imbalance(parent);

				parent = parent->parent;
			}
			
			return true;
	}

	//Adjust heights and check for imbalances, starting at the parent of the deleted node
	while(parent != sentinel)
	{
		fix_height(parent);

		//Fix imbalances
		if(imbalance(parent))
			fix_imbalance(parent);

		parent = parent->parent;
	}

	return true;
}
               
//Returns a vector of the tree's keys in order
vector <string> AVLTree::Ordered_Keys() const
{
	vector<string> rv;
	make_key_vector(sentinel->right, rv);
	return rv;
}
 
//Creates the key vector using an in-order traversal
void AVLTree::make_key_vector(const AVLNode *n, vector<string> &v) const
{
	//End Case: Sentinel reached; at leaf node
	if(n == sentinel)
		return;
	make_key_vector(n->left, v);
	v.push_back(n->key);
	make_key_vector(n->right, v);
}
 
//Returns the height of the entire tree
size_t AVLTree::Height() const
{
	return sentinel->right->height;
}

//Recursvively copies the subtree rooted in n in post-order manner
AVLNode *AVLTree::recursive_postorder_copy(const AVLNode *n) const
{
	AVLNode* newNode;
	AVLNode* leftChild;
	AVLNode* rightChild;

	//End Case: leaf node reached
	if(n->height == 1)
	{
		newNode = new AVLNode;
		newNode->key = n->key;
		newNode->val = n->val;
		newNode->height = n->height;
		newNode->right = sentinel;
		newNode->left = sentinel;
		return newNode;
	}

	//Create new root node of subtree
	newNode = new AVLNode;
	newNode->key = n->key;
	newNode->val = n->val;
	newNode->height = n->height;

	//Find left child (if any)
	if(n->left->height != 0)
	{
		leftChild = recursive_postorder_copy(n->left);
		
		//Attach to subtree
		leftChild->parent = newNode;
		newNode->left = leftChild;
	}

	else
		newNode->left = sentinel;
	
	//Find right child (if any)
	if(n->right->height != 0)
	{
		rightChild = recursive_postorder_copy(n->right);
		
		//Attach to subtree
		rightChild->parent = newNode;
		newNode->right = rightChild;
	}

	else
		newNode->right = sentinel;

	return newNode;
}

//Checks for an imbalance at node n based on the height of its children 
bool imbalance(const AVLNode* n)
{
	int difference = n->right->height - n->left->height;
	if(difference < 0) difference *= -1;

	if(difference > 1 /*|| (n->height > n->left->height + 1 && n->height > n->right->height + 1)*/)
		return true;

	return false;
}

//Rotates about node n
void rotate(AVLNode* n)
{
	AVLNode* parent;
	AVLNode* grandparent;
	AVLNode* middle;
	AVLNode* current;

	//Exit if parent is sentinel
	if(n->parent->height == 0)
		return;

	parent = n->parent;
	grandparent = parent->parent;
	middle = (parent->left == n ? n->right : n->left);

	/* The following comments are based on a general case from the AVL Tree Lecture Notes,
	 * but work on any case as well */

	//Daisy becomes Binky's parent
	parent->parent = n;
	
	//Fix Daisy's children (one is Binky now)
	if(parent->right == n)
		n->left = parent;
	else
		n->right = parent;

	//Daisy's parent becomes Eunice
	n->parent = grandparent;

	//Fix Eunice's children (one is Daisy now)
	if(grandparent->left == parent)
		grandparent->left = n;
	else
		grandparent->right = n;

	//Calista's parent becomes Binky
	middle->parent = parent;

	//Fix Binky's chilren (one is Calista now)
	if(parent->right == n)
		parent->right = middle;
	else
		parent->left = middle;

	//Fix heights
	current = middle;

	if(middle->height == 0)
		current = parent;

	//Fix heights 
	while(current->height != 0)
	{
		fix_height(current);
		current = current->parent;
	}
}

//Change's the height of n based on its children's heights
void fix_height(AVLNode* n)
{
	int greatest;

	if(n->height == 0)
		return;

	//Find biggest branch to base n's new height on
	if(n->right->height > n->left->height)
		greatest = n->right->height;
	else
		greatest = n->left->height;

	n->height = greatest + 1;
}

//Fixes the imbalance at n by performing the correct rotation(s) for zig-zig and zig-zag cases
void fix_imbalance(AVLNode* n)
{
	//Imbalance comes from right branch
	if(n->right->height > n->left->height)
	{
		//Right zig-zig
		if(n->right->right->height >= n->right->left->height)
			rotate(n->right);

		//Right zig-zag
		else
		{
			AVLNode* doubleRot = n->right->left;
			rotate(doubleRot);
			rotate(doubleRot);
		}
	}

	//Imbalance comes from left branch
	else
	{
		//Left zig-zig
		if(n->left->left->height >= n->left->right->height)
			rotate(n->left);

		//Left zig-zag
		else
		{
			AVLNode* doubleRot = n->left->right;
			rotate(doubleRot);
			rotate(doubleRot);
		}
	}
}
