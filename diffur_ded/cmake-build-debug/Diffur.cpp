//
// Created by egolg on 25.11.2020.
//

#include "Diffur.h"

const double EPSILON = 0.00001;

int VocabularyConstruct (Vocabulary* vocabulary, char* diffFileName, char* simplifyFileName)
{
    char* diffBuffer = "";
    char* simplifyBuffer = "";
    String* diffLines = nullptr;
    String* simplifyLines = nullptr;

    readToStorage (diffFileName, &diffBuffer);
    readToStorage (simplifyFileName, &simplifyBuffer);
    deleteDoubleSymb (diffBuffer, '\n');
    deleteDoubleSymb (simplifyBuffer, '\n');
    vocabulary->nDiffLines = countSymb (diffBuffer, '\n');
    vocabulary->nSimplifyLines = countSymb (simplifyBuffer, '\n');

    splitTextIntoLines (diffBuffer, vocabulary->nDiffLines, &diffLines);
    splitTextIntoLines (simplifyBuffer, vocabulary->nSimplifyLines, &simplifyLines);

    vocabulary->diffBuff = diffBuffer;
    vocabulary->simplifyBuff = simplifyBuffer;
    vocabulary->diffLines = diffLines;
    vocabulary->simplifyLines = simplifyLines;
    return 0;

}

TreeError ReadDifferentiatorData (char* fileNameInput, NumericTree* tree)
{
    assert (fileNameInput);

    char* storage = "";
    readToStorage (fileNameInput, &storage);
    char* oldStorage = storage;
    tree->root = DifferentiatorDataInterpreter (&storage, tree);
    free (oldStorage);

    return NO_TREE_ERRORS;
}

NumericNode* DifferentiatorDataInterpreter (char** beginOfExpression, NumericTree* tree)
{
    assert (beginOfExpression);
    assert (tree);

    if (**beginOfExpression != '(')
        return nullptr;

    (*beginOfExpression)++;

    NumericNode* currentNode = (NumericNode*) calloc (1, sizeof (*currentNode));
    NumericNode* right = nullptr;
    NumericNode* left = nullptr;

    if (**beginOfExpression == '(')
        left = DifferentiatorDataInterpreter (beginOfExpression, tree);

    if (CommandDeterminant (beginOfExpression, currentNode))
        return nullptr;

    if (**beginOfExpression == '(')
        right = DifferentiatorDataInterpreter (beginOfExpression, tree);

    if (**beginOfExpression != ')')
       return nullptr;

    (*beginOfExpression)++;

    currentNode->tree = tree;
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

#define MATH_OPERATION(operationName, operationCode, operationPrintName, diffFunction, printFunction) if (strncmp(operationPrintName,  *beginOfExpression, commandSize) == 0) \
    return VertexCommandHandler (beginOfExpression, commandSize, current, operationName, OPERATION);

#include "operation.txt"
#undef MATH_OPERATION

    if (isalpha(**beginOfExpression))
        return VertexCommandHandler (beginOfExpression, commandSize, current, **beginOfExpression - 'a', VARIABLE);

    if (isdigit(**beginOfExpression) || **beginOfExpression == '.' || **beginOfExpression=='-')
        return VertexCommandHandler (beginOfExpression, commandSize, current, strtod (*beginOfExpression, nullptr), CONSTANT);

    return 1;
}


int VertexCommandHandler (char** beginOfExpression, int commandSize, NumericNode* current, double command, NodeType type)
{
    *beginOfExpression += commandSize;
    current->memberValue = command;
    current->type = type;
    return 0;
}

NumericNode* DifferentiateSubTree (NumericNode* subRoot, NumericTree* diffTree)
{
    switch (subRoot->type)
    {
        case CONSTANT:
            return CreateNewNode (nullptr, nullptr, 0, CONSTANT, diffTree);
        case VARIABLE:
            return CreateNewNode (nullptr, nullptr, 1, CONSTANT, diffTree);
        case OPERATION:
            return DifferentiateOperation (subRoot, diffTree);
    }
}

NumericNode* CopyNode (NumericNode* oldNode, NumericTree* newTree)
{
    NumericNode* newNode = (NumericNode*) calloc (1, sizeof (*newNode));
    if (!newNode)
        return nullptr;

    newNode->tree = newTree;
    newNode->type = oldNode->type;
    newNode->memberValue = oldNode->memberValue;

    if (oldNode->rightChild)
        newNode->rightChild = CopyNode (oldNode->rightChild, newTree);
    else
        newNode->rightChild = nullptr;

    if (oldNode->leftChild)
        newNode->leftChild  = CopyNode (oldNode->leftChild,  newTree);
    else
        newNode->leftChild = nullptr;

    return newNode;
}

NumericNode* DifferentiateOperation (NumericNode* subRoot, NumericTree* diffTree)
{
    switch ((Commands) subRoot->memberValue)
    {
#define MATH_OPERATION(operationName, operationCode, operationPrintName, diffFunction, printFunction) case operationName: diffFunction
#include "operation.txt"

#undef MATH_OPERATION
    }
}

void PrepareLatexDocument (NumericTree* tree)
{
    fprintf (tree->LaTeX_Output,
             "\\documentclass[a4paper,12pt]{article}\n"
             "\\usepackage[utf8]{inputenc}\n"
             "\\usepackage[T2A]{fontenc}\n"
             "\\usepackage[english,russian]{babel}\n"
             "\\title{Произвольная производная, потенциально повергающая.}\n"
             "\\author{просто проект}\n"
             "\n"
             "\\usepackage{natbib}\n"
             "\\usepackage{graphicx}\n"
             "\n"
             "\\usepackage{amsmath,amsfonts,amssymb,amsthm,mathtools}\n"
             "\\usepackage{icomma} \n"
             "\n"
             "\\mathtoolsset{showonlyrefs=true}\n"
             "\n"
             "\\usepackage{euscript} \n"
             "\\usepackage{mathrsfs} \n"
             "%\\usepackage{graphicx}\n"
             "\\graphicspath{{./pictures/}}\n"
             "\\DeclareGraphicsExtensions{.pdf,.png,.jpg}\n"
             "\n"
             "\\DeclareMathOperator{\\sgn}{\\mathop{sgn}}\n"
             "\n"
             "\n"
             "\\begin{document}\n"
             "\n"
             "\\maketitle\n\n");
}

void SubTreeLaTeXOutput (NumericNode* root, PhraseType type)
{
    //PrintGeniousPhrase (type);
    fprintf (root->tree->LaTeX_Output, "$$");
    CurrentNodeLaTeXOutput (root, false);
    fprintf (root->tree->LaTeX_Output, "$$\n\n");
}

int NodePriority (NumericNode* upper, NumericNode* lower)
{
    if (lower->type != OPERATION)
        return 0;

    int upperPriority = 5;
    int lowerPriority = 4;

    if ((char)upper->memberValue == ADD || (char)upper->memberValue == SUBTRACTION)
        upperPriority = 0;
    else if ((char)upper->memberValue == DIVISION || (char)upper->memberValue == MULTIPLICATION)
        upperPriority = 1;
    else if ((char)upper->memberValue == POWER)
        upperPriority = 2;

    if ((char)lower->memberValue == ADD || (char)lower->memberValue == SUBTRACTION)
        lowerPriority = 0;
    else if ((char)lower->memberValue == DIVISION || (char)lower->memberValue == MULTIPLICATION)
        lowerPriority = 1;
    else if ((char)lower->memberValue == POWER)
        lowerPriority = 2;
    return upperPriority - lowerPriority;
}

void CurrentNodeLaTeXOutput (NumericNode* current, bool ParanthesesNeed)
{
    //printf ("here\n");
    if (ParanthesesNeed)
        fprintf (current->tree->LaTeX_Output, "(");

    switch (current->type)
    {
        case OPERATION: CurrentOperationLaTeXOutput (current); break;
        case CONSTANT : fprintf (current->tree->LaTeX_Output, "%lg", current->memberValue); break;
        case VARIABLE : fprintf (current->tree->LaTeX_Output, "%c", (char)current->memberValue + 'a'); break;
    }

    if (ParanthesesNeed)
        fprintf (current->tree->LaTeX_Output, ")");
}

void CurrentOperationLaTeXOutput (NumericNode* current)
{
    switch ((int)current->memberValue)
    {
#define MATH_OPERATION(operationName, operationCode, operationPrintName, diffFunction, printFunction) case operationName: printFunction
#include "operation.txt"

#undef MATH_OPERATION
    }
}

void PrintGeniousPhrase (PhraseType type)
{

}

void CloseLatexOutput (NumericTree* tree)
{
    fprintf (tree->LaTeX_Output, "\\end{document}");
    fclose (tree->LaTeX_Output);
}

void OneOperandFunctionPrint (NumericNode* current, char* nameOfFunction)
{
    fprintf(current->tree->LaTeX_Output, "%s", nameOfFunction);
    if (current->rightChild->rightChild)
        CurrentNodeLaTeXOutput (current->rightChild, true);
    else
        CurrentNodeLaTeXOutput (current->rightChild, false);
}

void LowerPriorityOperandPrint (NumericNode* current, char* nameOfFunction)
{
    CurrentNodeLaTeXOutput (current->leftChild, false);
    fprintf(current->tree->LaTeX_Output, nameOfFunction);
    CurrentNodeLaTeXOutput (current->rightChild, false);
}

void DivisionPrint (NumericNode* current)
{
    fprintf(current->tree->LaTeX_Output, " \\frac{");
    CurrentNodeLaTeXOutput (current->leftChild, false);
    fprintf(current->tree->LaTeX_Output, "}{");
    CurrentNodeLaTeXOutput (current->rightChild, false);
    fprintf(current->tree->LaTeX_Output, "} ");
}

void MultiplicationPrint (NumericNode* current)
{
    if (NodePriority (current, current->leftChild) > 0)
        CurrentNodeLaTeXOutput (current->leftChild, true);
    else
        CurrentNodeLaTeXOutput (current->leftChild, false);
    fprintf(current->tree->LaTeX_Output, " \\cdot ");
    if (NodePriority (current, current->rightChild) > 0)
        CurrentNodeLaTeXOutput (current->rightChild, true);
    else
        CurrentNodeLaTeXOutput (current->rightChild, false);
}

void LogPrint (NumericNode* current)
{
    fprintf(current->tree->LaTeX_Output, "log_");
    CurrentNodeLaTeXOutput (current->rightChild, false);
    CurrentNodeLaTeXOutput (current->leftChild, true);
}

void SqrtPrint (NumericNode* current)
{
    fprintf(current->tree->LaTeX_Output, " \\sqrt{");
    CurrentNodeLaTeXOutput (current->rightChild, 1);
    fprintf(current->tree->LaTeX_Output, "}");
}

void PowerPrint (NumericNode* current)
{
    if (current->leftChild->type == OPERATION)
        CurrentNodeLaTeXOutput (current->leftChild, true);
    else
        CurrentNodeLaTeXOutput (current->leftChild, false);
    fprintf(current->tree->LaTeX_Output, "^{");
    CurrentNodeLaTeXOutput (current->rightChild, false);
    fprintf(current->tree->LaTeX_Output, "}");
}

void SimplifyFunction (NumericTree* tree)
{
    int count = 0;
    do
    {
        count = 0;
        count += DFSMULT1 (tree->root, nullptr);

        count += DFSMULT0 (tree->root, nullptr);

        count += DFSPLUS0 (tree->root, nullptr);
    }while (count != 0);
}

int DFSMULT1 (NumericNode* current, NumericNode* parent)
{
    int changed = 0;
    if (current)
    {
        changed += DFSMULT1 (current->rightChild, current);
        changed += DFSMULT1 (current->leftChild,  current);
        if (IsZero(current->memberValue - MULTIPLICATION))
        {
            if (IsZero (current->leftChild->memberValue - 1))
            {
                SubTreeLaTeXOutput (parent, DIFFERENTIATE_TYPE);
                NumericNode* right = current->rightChild;
                current->rightChild = nullptr;
                NumericDestructTreeMember (current);
                changed++;
                if (!parent)
                    right->tree->root = right;
                else if (parent->rightChild == current)
                    parent->rightChild = right;
                else if (parent->leftChild == current)
                    parent->leftChild = right;
                if (!parent)
                    SubTreeLaTeXOutput (right->tree->root, DIFFERENTIATE_TYPE);
                else
                    SubTreeLaTeXOutput (parent, DIFFERENTIATE_TYPE);
            } else if (IsZero (current->rightChild->memberValue - 1))
            {
                SubTreeLaTeXOutput (parent, DIFFERENTIATE_TYPE);
                NumericNode* left = current->leftChild;
                current->leftChild = nullptr;
                NumericDestructTreeMember (current);
                changed++;
                if (!parent)
                    left->tree->root = left;
                else if (parent->rightChild == current)
                    parent->rightChild = left;
                else if (parent->leftChild == current)
                    parent->leftChild = left;

                if (!parent)
                    SubTreeLaTeXOutput (left->tree->root, DIFFERENTIATE_TYPE);
                else
                    SubTreeLaTeXOutput (parent, DIFFERENTIATE_TYPE);
            }
        }
    }
    return changed;
}

int DFSPLUS0 (NumericNode* current, NumericNode* parent)
{
    int changed = 0;
    if (current)
    {
        changed += DFSPLUS0 (current->rightChild, current);
        changed += DFSPLUS0 (current->leftChild,  current);
        if (IsZero(current->memberValue - ADD))
        {
            if (IsZero (current->leftChild->memberValue))
            {
                SubTreeLaTeXOutput (parent, DIFFERENTIATE_TYPE);
                NumericNode* right = current->rightChild;
                current->rightChild = nullptr;
                NumericDestructTreeMember (current);
                changed++;
                if (!parent)
                    right->tree->root = right;
                else if (parent->rightChild == current)
                    parent->rightChild = right;
                else if (parent->leftChild == current)
                    parent->leftChild = right;

                if (!parent)
                    SubTreeLaTeXOutput (right->tree->root, DIFFERENTIATE_TYPE);
                else
                    SubTreeLaTeXOutput (parent, DIFFERENTIATE_TYPE);
            } else if (IsZero (current->rightChild->memberValue))
            {
                SubTreeLaTeXOutput (parent, DIFFERENTIATE_TYPE);
                NumericNode* left = current->leftChild;
                current->leftChild = nullptr;
                NumericDestructTreeMember (current);
                changed++;
                if (!parent)
                    left->tree->root = left;
                else if (parent->rightChild == current)
                    parent->rightChild = left;
                else if (parent->leftChild == current)
                    parent->leftChild = left;

                if (!parent)
                    SubTreeLaTeXOutput (left->tree->root, DIFFERENTIATE_TYPE);
                else
                    SubTreeLaTeXOutput (parent, DIFFERENTIATE_TYPE);
            }
        }
    }
    return changed;
}

int DFSMULT0 (NumericNode* current, NumericNode* parent)
{
    int changed = 0;
    if (current)
    {
        changed += DFSMULT0 (current->rightChild, current);
        changed += DFSMULT0 (current->leftChild,  current);
        if (IsZero(current->memberValue - MULTIPLICATION))
        {
            NumericTree* tree = current->tree;
            if (IsZero (current->leftChild->memberValue))
            {
                SubTreeLaTeXOutput (current, DIFFERENTIATE_TYPE);
                changed++;
                if (!parent)
                    tree->root = CreateNewNode (nullptr, nullptr, 0, CONSTANT, tree);
                else if (parent->rightChild == current)
                    parent->rightChild = CreateNewNode (nullptr, nullptr, 0, CONSTANT, tree);
                else if (parent->leftChild == current)
                    parent->leftChild = CreateNewNode (nullptr, nullptr, 0, CONSTANT, tree);

                NumericDestructTreeMember (current);
            } else if (IsZero (current->rightChild->memberValue))
            {
                SubTreeLaTeXOutput (current, DIFFERENTIATE_TYPE);
                changed++;
                if (!parent)
                    tree->root = CreateNewNode (nullptr, nullptr, 0, CONSTANT, tree);
                else if (parent->rightChild == current)
                    parent->rightChild = CreateNewNode (nullptr, nullptr, 0, CONSTANT, tree);
                else if (parent->leftChild == current)
                    parent->leftChild = CreateNewNode (nullptr, nullptr, 0, CONSTANT, tree);

                NumericDestructTreeMember (current);
            }
        }
    }
    return changed;
}

bool IsZero (double value)
{
    if (fabs (value) < EPSILON)
        return true;
    else
        return false;
}