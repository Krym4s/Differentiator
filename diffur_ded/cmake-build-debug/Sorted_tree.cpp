//
// Created by egolg on 23.11.2020.
//

#include "Sorted_tree.h"

#define TREERELEASE

#ifdef TREERELEASE
#define ASSERTEDTREE
#define ASSERTEDTREEMEMBER
#else
#define ASSERTEDTREE !TreeVerify (thou) || GraphicDump (thou);
#define ASSERTEDTREEMEMBER !TreeVerify (thou->tree) || GraphicDump (thou->tree);
#endif

TreeError RBTreeConstruct (Red_Black_tree* thou, char* graph_logs)
{
    assert (thou);
    thou->size = 0;
    thou->root = NULL;
    thou->graph_logs_name = graph_logs;

    return NO_TREE_ERRORS;
}

Node* RBTreeCreate (Red_Black_tree* thou, MemberValue value)
{
    assert (thou);
    ASSERTEDTREE
    if (thou->root)
        return thou->root; // if tree root exists we can not create it one more time

    thou->root = (Node*) calloc (1, sizeof (*thou->root));
    if (!thou->root)
        return thou->root;

    thou->size = 1;
    thou->root->tree = thou;
    thou->root->leftChild = NULL;
    thou->root->rightChild = NULL;
    thou->root->memberValue = value;
    thou->root->parent = NULL;
    ASSERTEDTREE
    return thou->root;
}

TreeError rotateLeft (Node* n)
{

};