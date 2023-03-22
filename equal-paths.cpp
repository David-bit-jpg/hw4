#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)

#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here


bool equalPaths(Node * root)
{
    // Add your code below
    if(root==NULL) return true;
    int left_height = findHeight(root->left);
	int right_height = findHeight(root->right);
    if(root->left!=NULL&&root->right==NULL)
    return equalPaths(root->left)&&equalPaths(root->right);
    else if(root->left==NULL&&root->right!=NULL)
    return equalPaths(root->left)&&equalPaths(root->right);
    else
    if(left_height!=right_height) return false;
    return equalPaths(root->left)&&equalPaths(root->right);

}
int findHeight(Node *root)
{
	if(root==NULL) 
	return 0;
	int left_height = findHeight(root->left);
	int right_height = findHeight(root->right);
	if(left_height>right_height) 
	return left_height+1;
	else 
	return right_height+1;
}

