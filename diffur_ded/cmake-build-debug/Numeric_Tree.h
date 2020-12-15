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
#define MATH_OPERATION(operationName, operationCode, operationPrintName, diffFunction, printFunction, calcFunc) operationName = operationCode,
#include "operation.txt"
#undef MATH_OPERATION
};

enum PhraseType
{
    DIFFERENTIATE_TYPE,
    SIMPLIFY_TYPE,
    NO_PHRASE,
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
    double variables[26];
    FILE* LaTeX_Output;
};

TreeError NumericTreeConstruct (NumericTree* thou, char* graph_logs, char* LaTeX_Output_name, FILE* latexOutput);

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
