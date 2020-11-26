//
// Created by egolg on 25.11.2020.
//

#include "Diffur.h"

TreeError ReadDifferentiatorData (char* fileNameInput, NumericTree* tree)
{
    assert (fileNameInput);

    char* storage = "";
    readToStorage (fileNameInput, &storage);
    //printf ("%s\n", storage);
    tree->root = DifferentiatorDataInterpreter (&storage, NULL, tree);

    return NO_TREE_ERRORS;
}

NumericNode* DifferentiatorDataInterpreter (char** beginOfExpression, NumericNode* parent, NumericTree* tree)
{
    assert (beginOfExpression);
    assert (tree);

    if (**beginOfExpression != '(')
        return NULL;

    (*beginOfExpression)++;

    NumericNode* currentNode = (NumericNode*) calloc (1, sizeof (*currentNode));
    NumericNode* right = NULL;
    NumericNode* left = NULL;

    if (**beginOfExpression == '(')
        left = DifferentiatorDataInterpreter (beginOfExpression, currentNode, tree);

   // (*beginOfExpression)++;

    if (CommandDeterminant (beginOfExpression, currentNode))
        return NULL;

    if (**beginOfExpression == '(')
        right = DifferentiatorDataInterpreter (beginOfExpression, currentNode, tree);

    if (**beginOfExpression != ')')
       return NULL;

    (*beginOfExpression)++;

    currentNode->tree = tree;
    currentNode->parent = parent;
    currentNode->leftChild = left;
    currentNode->rightChild = right;

    tree->size++;

    return currentNode;
}


int CommandDeterminant (char** beginOfExpression, NumericNode* current)
{
    char* endOfExpression = strpbrk (*beginOfExpression, "()");

    int commandSize = endOfExpression - *beginOfExpression;

    if (commandSize <= 0)
        return NO_VALUE;

    if (strncmp("+", *beginOfExpression, commandSize) == 0)
        return VertexCommandHandler (beginOfExpression, commandSize, current, ADD, OPERATION);

    if (strncmp("-", *beginOfExpression, commandSize) == 0)
        return VertexCommandHandler (beginOfExpression, commandSize, current, SUBTRACTION, OPERATION);

    if (strncmp("/", *beginOfExpression, commandSize) == 0)
        return VertexCommandHandler (beginOfExpression, commandSize, current, DIVISION, OPERATION);

    if (strncmp("*", *beginOfExpression, commandSize) == 0)
        return VertexCommandHandler (beginOfExpression, commandSize, current, MULTIPLICATION, OPERATION);

    if (strncmp("^", *beginOfExpression, commandSize) == 0)
        return VertexCommandHandler (beginOfExpression, commandSize, current, POWER, OPERATION);

    if (strncmp("sin", *beginOfExpression, commandSize) == 0)
        //printf("found\n");
        return VertexCommandHandler (beginOfExpression, commandSize, current, SIN, OPERATION);

    if (strncmp("cos", *beginOfExpression, commandSize) == 0)
        return VertexCommandHandler (beginOfExpression, commandSize, current, COS, OPERATION);

    if (strncmp("log", *beginOfExpression, commandSize) == 0)
        return VertexCommandHandler (beginOfExpression, commandSize, current, LOG, OPERATION);

    if (strncmp("lg", *beginOfExpression, commandSize) == 0)
        return VertexCommandHandler (beginOfExpression, commandSize, current, LG, OPERATION);

    if (strncmp("ln", *beginOfExpression, commandSize) == 0)
        return VertexCommandHandler (beginOfExpression, commandSize, current, LN, OPERATION);

    if (strncmp("tg", *beginOfExpression, commandSize) == 0)
        return VertexCommandHandler (beginOfExpression, commandSize, current, TAN, OPERATION);

    if (strncmp("ctg", *beginOfExpression, commandSize) == 0)
        return VertexCommandHandler (beginOfExpression, commandSize, current, CTAN, OPERATION);

    if (strncmp("sh", *beginOfExpression, commandSize) == 0)
        return VertexCommandHandler (beginOfExpression, commandSize, current, SH, OPERATION);

    if (strncmp("ch", *beginOfExpression, commandSize) == 0)
        return VertexCommandHandler (beginOfExpression, commandSize, current, CH, OPERATION);

    if (strncmp("th", *beginOfExpression, commandSize) == 0)
        return VertexCommandHandler (beginOfExpression, commandSize, current, TH, OPERATION);

    if (strncmp("cth", *beginOfExpression, commandSize) == 0)
        return VertexCommandHandler (beginOfExpression, commandSize, current, CTH, OPERATION);

    if (isalpha(**beginOfExpression))
        return VertexCommandHandler (beginOfExpression, commandSize, current, **beginOfExpression - 'a', VARIABLE);

    if (isdigit(**beginOfExpression) || **beginOfExpression == '.')
        return VertexCommandHandler (beginOfExpression, commandSize, current, atof (*beginOfExpression), CONSTANT);

    return 1;
}


int VertexCommandHandler (char** beginOfExpression, int commandSize, NumericNode* current, double command, NodeType type)
{
    *beginOfExpression += commandSize;
    current->memberValue = command;
    current->type = type;
    return 0;
}