#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)

#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here
int heightHelper(Node* root);

bool equalPaths(Node* root)
{
    // Add your code below
	if(root==nullptr){ //base case
		return true;
	}

	//Get the heights of the left and right subtrees 
    int leftH=heightHelper(root->left);
    int rightH=heightHelper(root->right);

    if((leftH==0 && rightH==1)|| (rightH==0 && leftH==1)){ //2 nodes
        return true;
    }

	//Determine if the node has equal paths
	if(abs(leftH-rightH)!=0){
		return false;
	}
    return true;
}

int heightHelper(Node *root) {
	if(root==nullptr){ //base case
		return 0;
	}

	int left=heightHelper(root->left); //recursive
	int right=heightHelper(root->right);

	if(left>right){ //return max height
        return left+1;
    }
    else{
        return right+1;
    }
}