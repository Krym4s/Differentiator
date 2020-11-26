//
// Created by egolg on 23.11.2020.
//

#ifndef DIFFUR_DED_SORTED_TREE_H
#define DIFFUR_DED_SORTED_TREE_H

#include "IsE_Tree.h"

struct Red_Black_tree;

struct Node
{
    Node* currentNode;
    MemberValue memberValue;
    Node* leftChild;
    Node* rightChild;
    Node* parent;
    Red_Black_tree* tree;
    short colour;
};

struct Red_Black_tree
{
    Node* root;
    unsigned int size;
    char* graph_logs_name;
};

TreeError RBTreeConstruct (Red_Black_tree* thou, char* graph_logs);

Node* RBTreeCreate (Red_Black_tree* thou, MemberValue* value);

TreeError rotateLeft (Node* n);

Node* InsertIntoRB (MemberValue* value);

Red_Black_tree* DestructRBtree (Red_Black_tree* thou);

#endif //DIFFUR_DED_SORTED_TREE_H
