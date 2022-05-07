// Name: Ayush Kumar Gupta
// SRN: PES1UG20CS095
#include "bst.h"
#include <stdlib.h>

/*
 ** Do not use global variables.
 ** Mark any functions that you declare in this file that are not in the header as static
 ** Do not use static variables
 ** All occurences of count_ptr indicate that the number of comparison
 ** operations need to be stored in the location pointed by it
 ** The implementation can assume it is initialized to 0.
*/
static node_t *createNode(int key){
    // function to allocate memory for a node and give it a value and return its address
    node_t *new =  (node_t *)malloc(sizeof(node_t));
    new->left = new->right = NULL;
    new->key = key;
    return new;
}

static void insertHelper(node_t *curr,int key,int *count_ptr){
    // in scenario with equal elements
    // the element will be inserted to the left subtree
    if(++(*count_ptr) && key<=curr->key && curr->left==NULL)
        curr->left = createNode(key);
    else if(++(*count_ptr) && key>curr->key && curr->right==NULL)
        curr->right = createNode(key);
    else if(++(*count_ptr) && key<=curr->key && curr->left!=NULL)
        insertHelper(curr->left,key,count_ptr);
    else
        insertHelper(curr->right,key,count_ptr);
}

static node_t *inorderSuccessor(node_t *node){
    // function to return the inorder succcessor of a given node
    if(node!=NULL && node->left)
        return inorderSuccessor(node->left);
    return node;
}

static node_t *delHelper(node_t *curr,int key, int *count_ptr){
    // recursive function which deletes the specified node in the bst
    if(++(*count_ptr) && curr==NULL)
        return curr;
    else if(++(*count_ptr) && (key<curr->key))
        curr->left = delHelper(curr->left,key,count_ptr);
    else{
        if(++(*count_ptr) && (key > curr->key))
            curr->right = delHelper(curr->right,key,count_ptr);
        else{
            // if and else if handle case when the node has 1 or no child
            node_t *temp;
            if(++(*count_ptr) && (curr->left == NULL)){
                temp = curr->right;
                free(curr);
                return temp;
            }
            else if(++(*count_ptr) && curr->right == NULL){
                temp = curr->left;
                free(curr);
                return temp;
            }
            // when the node has two children
            temp = inorderSuccessor(curr->right);
            curr->key = temp->key;
            curr->right = delHelper(curr->right,temp->key,count_ptr);
        }
    }
    return curr;
}

static int searchHelper(node_t *curr,int key,int *count_ptr){
    // recursive function to help in finding the specified key in bst
    // returns -1 if not found
    // returns key value upon successful finding
    if(++(*count_ptr) && curr==NULL)
        return -1;
    else if( ++(*count_ptr) && (key == curr->key))
        return key;
    else if( ++(*count_ptr) && key<curr->key)
        return searchHelper(curr->left,key,count_ptr);
    else
        return searchHelper(curr->right,key,count_ptr);    
}

static int findMaxHelper(node_t *curr,int *count_ptr){
    // recursive function to find the max value in a heap
    // returns max value on success
    // incase of no elements in the tree it returns -1
    if(++(*count_ptr) && curr==NULL)
        return -1;
    else if(++(*count_ptr) && curr->right==NULL)
        return curr->key;
    else
        return findMaxHelper(curr->right,count_ptr);
}

static node_t *releaseAllocation(node_t *curr){
    // recursive functiont to free all leaf nodes and subtrees for a given node
    if(curr==NULL)
        return NULL;
    else{
        curr->left = releaseAllocation(curr->left);
        curr->right = releaseAllocation(curr->right);
        free(curr);
    }
}

// Initializes the root of the bst
void init_bst(bst_t *bst)
{
    bst->root = NULL;
}

// Inserts element key into the Binary search tree
// Stores the number of comparisons at the location
// pointed by count_ptr
void insert(bst_t *tree, int key, int *count_ptr)
{
    if( ++(*count_ptr) && (tree->root == NULL)){
        tree->root = createNode(key);
    }
    else{
        insertHelper(tree->root,key,count_ptr);
    }
}

// Delete key from the BST
// Replaces node with in-order successor
void delete_element(bst_t *tree, int key, int *count_ptr)
{
   tree->root = delHelper(tree->root,key,count_ptr);
}

// Searches for the element key in the bst
// Returns the element if found, else -1
int search(const bst_t *tree, int key, int *count_ptr)
{
    return searchHelper(tree->root,key,count_ptr);
}

// Returns the maximum element in the BST
int find_max(const bst_t *tree, int *count_ptr)
{
    return findMaxHelper(tree->root,count_ptr);
}

// Deletes all the elements of the bst
void free_bst(bst_t *bst) 
{
    releaseAllocation(bst->root);
    bst->root = NULL;
}

// Deletes all the elements if the bst and ensures it can be used again
void clear_bst(bst_t *bst)
{
   releaseAllocation(bst->root);
   bst->root = NULL;
}
