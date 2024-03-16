#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)
#include <algorithm>
#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here
int depth(Node* n) {
    if (n == nullptr) {
        // If the node is null, the depth is 0
        return 0;
    }

    // Recursively calculate the depth of left and right subtrees
    int lDepth = depth(n->left);
    int rDepth = depth(n->right);

    return 1 + max(lDepth, rDepth);
    
}

bool equalPaths(Node * root)
{
    if (root == nullptr) {
        // If the tree is empty, all paths are considered equal
        return true;
    }


    if (root->left == nullptr) {
        if (depth(root->right) > 1) {
            return false;
        } else {
            return true;
        }
    } else if (root->right == nullptr) {
        if (depth(root->left) > 1) {
            return false;
        } else {
            return true;
        }
    }

    if (depth(root->right) == depth(root->left)) {
        return equalPaths(root->left) && equalPaths(root->right);
    } else {
        return false;
    }

    
    
}

