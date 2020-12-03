//
// Created by egolg on 23.11.2020.
//

#ifndef DIFFUR_DED_NUMERIC_TREE_H
#define DIFFUR_DED_NUMERIC_TREE_H

#include "IsE_Tree.h"
#include "string.h"

enum NodeType
{
    CONSTANT  = 0,
    VARIABLE  = 1,
    OPERATION = 2,

    IMAGINATIVE = 3,
};

enum Commands
{
    ADD = '+',
    SUBTRACTION = '-',
    DIVISION = '/',
    MULTIPLICATION = '*',
    SIN = 's',
    COS = 'c',
    POWER = '^',
    LOG = 'o',
    LG = 'g',
    LN = 'n',
    TAN = 't',
    CTAN = '1',
    SH = 'h',
    CH = 'u',
    TH = '2',
    CTH = '3',
    ARCSIN = '4',
    ARCCOS = '5',
    ARCTG = '6',
    ARCCTG = '7',
    SQRT = '8'
};

enum PhraseType
{
    DIFFERENTIATE_TYPE,
    SIMPLIFY_TYPE,
};

struct NumericTree;

struct NumericNode
{
    NodeType type;
    MemberValue memberValue;
    NumericNode* leftChild;
    NumericNode* rightChild;
    NumericTree* tree;
};


struct NumericTree
{
    NumericNode* root;
    unsigned int size;
    char* graph_logs_name;
    int variables[26];
    FILE* LaTeX_Output;
};

TreeError NumericTreeConstruct (NumericTree* thou, char* graph_logs, char* LaTeX_Output_name);

NumericNode* NumericNodeConstruct (NumericNode* thou, NumericNode* left, NumericNode* right, MemberValue value, NodeType Type, NumericTree* tree);

NumericNode* CreateNewNode (NumericNode* left, NumericNode* right, MemberValue value, NodeType Type, NumericTree* tree);

NumericNode* NumericAddRightChild (NumericNode* thou, MemberValue value, NodeType type);

NumericNode* NumericAddLeftChild (NumericNode* thou, MemberValue value, NodeType type);

TreeError NumericDestructTree (NumericTree* thou);

TreeError NumericDestructTreeMember (NumericNode* thou);

TreeError NumericTreeVerify (NumericTree* tree);

int NumericDFSVerify (NumericNode* vertex);

TreeError NumericGraphicDump (NumericTree* thou);

TreeError NumericDeleteVertex (NumericNode* thou);

void NumericDrawVertex (FILE* graph_logs, NumericNode* thou);

void NumericDeclareVertex (FILE* graph_logs, NumericNode* thou);

char* typeName (NodeType type);

NumericNode* FindParent (NumericNode* current);

NumericNode* DFSSearch (NumericNode* sought, NumericNode* current);

#endif //DIFFUR_DED_NUMERIC_TREE_H
