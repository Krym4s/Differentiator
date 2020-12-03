//
// Created by egolg on 13.11.2020.
//

#ifndef TREE_DED_ISE_TREE_H
#define TREE_DED_ISE_TREE_H

#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <cstring>
#include <cmath>

typedef double MemberValue;

struct Tree;

struct TreeMember
{
    MemberValue memberValue;
    TreeMember* leftChild;
    TreeMember* rightChild;
    TreeMember* parent;
    Tree* tree;
};

struct Tree
{
    TreeMember* root;
    unsigned int size;
    char* graph_logs_name;
};

enum TreeError
{
    NO_TREE_ERRORS     = 0,
    DOUBLE_CREATE = 1,
    WRONG_VERSION = 2,
    LOGS_ENABLE = 3,
    DOES_NOT_EXIST = 4,
    INCORRECT_VALUE = 5,
    LOST_PARENT = -1,
    BAD_TREE_SIZE = 6,
    NOT_A_LEAF = 7,
    WRONG_TYPE = -2,
    NO_VALUE = -3,
    NO_TREE_FREE_MEMORY = 8,
};

TreeError TreeConstruct (struct Tree* thou, char* graph_logs);

TreeMember* TreeCreate (struct Tree* thou, MemberValue value);

TreeMember* AddRightChild (TreeMember* thou, MemberValue value);

TreeMember* AddLeftChild (TreeMember* thou, MemberValue value);

TreeError DestructTree (Tree* thou);

TreeError DestructTreeMember (TreeMember* thou);

TreeError TreeVerify (Tree* tree);

int DFSVerify (TreeMember* vertex);

TreeError GraphicDump (Tree* thou);

TreeError DeleteVertex (TreeMember* thou);

void DrawVertex (FILE* graph_logs, TreeMember* thou);

void DeclareVertex (FILE* graph_logs, TreeMember* thou);

#endif //TREE_DED_ISE_TREE_H
