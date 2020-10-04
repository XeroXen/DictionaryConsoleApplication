//Jonathon Lewis
//5/16/2020
//AVLTree.h
//My implementation of an AVL tree. Only supports insertion, unfortunately. Deletion was on my list of things to do, but I ran into a big bug that took up a lot of time.
//and deletion is more complicated. So in the interest of actually finishing the project I'm leaving it with insertion only, but it was nice to implement.

//Also where the bulk of the logic for this program is done.

#pragma once
#include <iostream>
#include <vector>
#include "LinkedList.h"

using namespace std;

template <class T>
class AVLTree {

	//Each node needs to know its height, unlike in a standard BST.
	struct Node {
		T payload;
		Node* left;
		Node* right;
		Node* parent;
		int height;
	};

private:
	Node* root;

	//Creates new nodes.
	Node* createNode(T payload) {
		Node* thisNode = new Node;
		thisNode->payload = payload;
		thisNode->right = NULL;
		thisNode->left = NULL;
		thisNode->parent = NULL;
		thisNode->height = 1;

		return thisNode;
	}

	//used to determine the largest height easily.
	int max(int first, int second) {
		if (first > second) return first;
		return second;
	}

	//this simplifies logic later on, so we don't have to first determine whether or not a node actually has a value
	//in it before checking its height.
	int getHeight(Node* thisNode) {
		if (thisNode) {
			return thisNode->height;
		}

		return 0;
	}

	//returns the current balance of this node.
	int balance(Node* thisNode) {
		//sanity check...
		if (thisNode)
			return getHeight(thisNode->left) - getHeight(thisNode->right);
		return 0;
	}

	Node* rightRotation(Node* root) {
		
		
		Node* b = root->left;
		Node* rotate = b->right;

		b->right = root;
		root->left = rotate;

		//It's extremely important that these heights are updated in this order. Otherwise there's many bugs
		//that take hours to track down... 5 hours.
		root->height = max(getHeight(root->left), getHeight(root->right)) + 1;
		b->height = max(getHeight(b->left), getHeight(b->right)) + 1;

		return b;
	}

	Node* leftRotation(Node* root) {
		//mirror of rightRotation

		Node* b = root->right;
		Node* rotate = b->left;

		b->left = root;
		root->right = rotate;

		//update heights.
		root->height = max(getHeight(root->left), getHeight(root->right)) + 1;
		b->height = max(getHeight(b->left), getHeight(b->right)) + 1;

		return b;
	}
	
	//Self-balancing AVL insertion! Note, that we can turn off the avl part (explained why we would do that in the main project file)
	Node* insert(Node *root, Node *toInsert, bool avl) {

		//If this is the spot to insert, do so.
		if (!root) return toInsert;

		//otherwise, we recursively call this statement in the proper location...
		else if (toInsert->payload < root->payload) {
			root->left = insert(root->left, toInsert, avl);
		}

		else if (toInsert->payload > root->payload) {
			root->right = insert(root->right, toInsert, avl);
		}

		//unless it is equal to another value, in which case we don't alter anything, because that's not allowed!
		else {
			return root;
		}

		root->height = max(getHeight(root->left), getHeight(root->right)) + 1;

		//Recursive AVL below.

		if (avl) {
			int thisBalance = balance(root);

			//if there is an unbalance caused by the right side of this node...
			if (thisBalance < -1) {

				if (root->right->payload < toInsert->payload) {
					//right-right case, do left rotate and return whatever replaces this node.
					return leftRotation(root);
				}
				else {
					//right-left case, right-rotate y and then left rotate this node, and return.
					root->right = rightRotation(root->right);
					return leftRotation(root);
				}
			}
			//if there's an unbalance caused by the left...
			else if (thisBalance > 1) {

				if (root->left->payload < toInsert->payload) {
					//left-right case, left-rotate y and then right rotate this node, and return it.
					root->left = leftRotation(root->left);
					return rightRotation(root);
				}
				else {
					//left-left case, do right rotate and return...
					return rightRotation(root);
				}
			}
		}
		
		//returns itself at the end! important for if this is not the node that the insertion goes into.
		return root;
	}


	//Below are traversals. These were mostly used for debugging the AVL tree and ensuring it was working properly.
	void inorderTraversal(Node* root) {
		if (root) {
			inorderTraversal(root->left);
			std::cout << root->payload << " ";
			inorderTraversal(root->right);
		}

		return;
	}

	void preorderTraversal(Node* root) {
		if (root) {
			std::cout << root->payload << " ";
			preorderTraversal(root->left);
			preorderTraversal(root->right);
		}
	}

	void postorderTraversal(Node* root) {
		if (root) {
			postorderTraversal(root->left);
			postorderTraversal(root->right);
			std::cout << root->payload << " ";
		}
	}

	//these last two traversals modify linkedlists and vectors respectively,
	//and were/are used to generate the linkedlist/vector to write to file at
	//the end of the program.
	void preorderLinkedList(Node* root, LinkedList<T> &myList) {
		if (root) {
			myList.add(root->payload);
			preorderLinkedList(root->left, myList);
			preorderLinkedList(root->right, myList);
		}
	}

	void preordervector(Node* root, vector<T>& myList) {
		if (root) {
			myList.push_back(root->payload);
			preordervector(root->left, myList);
			preordervector(root->right, myList);
		}
	}

	//a simple recursive binary search. very efficient with an AVL tree!
	Node* binarySearch(Node* root, T toSearch) {
		if (!(root)) return NULL;
		if (root->payload == toSearch) {
			return root;
		}
		else if (root->payload < toSearch) {
			return binarySearch(root->right, toSearch);
		}
		else if (root->payload > toSearch) {
			return binarySearch(root->left, toSearch);
		}
	}

public:
	AVLTree() {
		root = NULL;
	}

	//the public way to add to the BST.
	void add(T toAdd, bool avl = true) {
		Node* thisNode = createNode(toAdd);
		root = insert(root, thisNode, avl);
	}

	//Traversals, mostly used for debugging the lists.
	//(I never needed postorder, but included it for completion's sake)
	void inorder() {
		inorderTraversal(root);
	}

	void preorder() {
		preorderTraversal(root);
	}

	void postorder() {
		postorderTraversal(root);
	}

	//These are/were used for writing to the file at the end of the program.
	void linkedListPreorder(LinkedList<T> &myList) {
		preorderLinkedList(root, myList);
	}

	void vectorPreorder(vector<T>& myList) {
		preordervector(root, myList);
	}

	//Will return a pointer to a node's payload that matches the given
	//search parameter, otherwise will return a null pointer.

	//Returning a null pointer was an easy way to tell if it existed or not.
	T* search(T toSearch) {
		Node* found = binarySearch(root, toSearch);
		if(found) return &((binarySearch(root, toSearch))->payload);

		return NULL;
	}
};