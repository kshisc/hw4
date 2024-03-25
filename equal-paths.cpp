#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)

#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here
int heightHelper(Node* root, int depth);
int countNodes(Node* root);

bool equalPaths(Node* root)
{
  // Add your code below
	if(root==nullptr){ //0 nodes
		return true;
	}

	//Get the heights of the left and right subtrees 
	int leftH=heightHelper(root->left,0);
	int rightH=heightHelper(root->right,0);

	//1 path
	int numNodes=countNodes(root);
	if(leftH+1==numNodes || rightH+1==numNodes){
		return true;
	}

	//Determine if the node has equal paths
	return leftH==rightH;
}

int heightHelper(Node *root, int depth) {
	if(root==nullptr){ //base case
		return depth;
	}

	int left=heightHelper(root->left, depth+1); //recursive
	int right=heightHelper(root->right, depth+1);
	
	if(left>right){ //return max
		return left;
	}
	else{
		return right;
	}
}

int countNodes(Node* root){
	if(root==nullptr){
		return 0;
	}
	return 1 + countNodes(root->left) + countNodes(root->right);
}