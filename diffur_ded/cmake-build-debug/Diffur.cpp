//
// Created by egolg on 25.11.2020.
//

#include "Diffur.h"

const double EPSILON = 0.00001;

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

    if (strncmp("arcsin", *beginOfExpression, commandSize) == 0)
        return VertexCommandHandler (beginOfExpression, commandSize, current, ARCSIN, OPERATION);

    if (strncmp("arccos", *beginOfExpression, commandSize) == 0)
        return VertexCommandHandler (beginOfExpression, commandSize, current, ARCCOS, OPERATION);

    if (strncmp("arctg", *beginOfExpression, commandSize) == 0)
        return VertexCommandHandler (beginOfExpression, commandSize, current, ARCTG, OPERATION);

    if (strncmp("arcctg", *beginOfExpression, commandSize) == 0)
        return VertexCommandHandler (beginOfExpression, commandSize, current, ARCCTG, OPERATION);

    if (strncmp("sqrt", *beginOfExpression, commandSize) == 0)
        return VertexCommandHandler (beginOfExpression, commandSize, current, SQRT, OPERATION);

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

#define dL DifferentiateSubTree (subRoot->leftChild, diffTree)
#define dR DifferentiateSubTree (subRoot->rightChild, diffTree)
#define L CopyNode (subRoot->leftChild, diffTree)
#define R CopyNode (subRoot->rightChild, diffTree)

#define ADD_F(x,y) CreateNewNode (x, y, ADD, OPERATION, diffTree)
#define SUBTRACTION_F(x,y) CreateNewNode (x, y, SUBTRACTION, OPERATION, diffTree)
#define DIVISION_F(x,y) CreateNewNode (x, y, DIVISION, OPERATION, diffTree)
#define MULTIPLICATION_F(x,y) CreateNewNode (x, y, MULTIPLICATION, OPERATION, diffTree)
#define POWER_F(x,y) CreateNewNode (x, y, POWER, OPERATION, diffTree)
#define SIN_F(x) CreateNewNode (nullptr, x, SIN, OPERATION, diffTree)
#define COS_F(x) CreateNewNode (nullptr, x, COS, OPERATION, diffTree)
#define LOG_F(x,y) CreateNewNode (x, y, LOG, OPERATION, diffTree)
#define LN_F(x) CreateNewNode (nullptr, x, LN, OPERATION, diffTree)
#define LG_F(x) CreateNewNode (nullptr, x, LG, OPERATION, diffTree)
#define SH_F(x) CreateNewNode (nullptr, x, SH, OPERATION, diffTree)
#define CH_F(x) CreateNewNode (nullptr, x, CH, OPERATION, diffTree)
#define TH_F(x) CreateNewNode (nullptr, x, TH, OPERATION, diffTree)
#define CTH_F(x) CreateNewNode (nullptr, x, CTH, OPERATION, diffTree)
#define SQRT_F(x) CreateNewNode (nullptr, x, SQRT, OPERATION, diffTree)
#define NEW_CONST(x) CreateNewNode (nullptr, nullptr, x, CONSTANT, diffTree)

NumericNode* DifferentiateOperation (NumericNode* subRoot, NumericTree* diffTree)
{
    switch ((Commands) subRoot->memberValue)
    {
        case ADD:            return ADD_F (dL, dR);
        case SUBTRACTION:    return SUBTRACTION_F (dL, dR);
        case DIVISION:       return DIVISION_F (ADD_F (MULTIPLICATION_F (dL, R), MULTIPLICATION_F (L, dR)), POWER_F (R, NEW_CONST(2)));
        case MULTIPLICATION: return ADD_F(MULTIPLICATION_F (dL, R), MULTIPLICATION_F (L, dR));
        case SIN:            return MULTIPLICATION_F (COS_F (R), dR);
        case COS:            return MULTIPLICATION_F (NEW_CONST(-1), MULTIPLICATION_F (SIN_F (R), dR));
        case LG:             return DIVISION_F (DIVISION_F (dR, R), LN_F(NEW_CONST (10)));
        case LOG:            return DIVISION_F (SUBTRACTION_F (MULTIPLICATION_F (DIVISION_F (LN_F (R), L), dL), MULTIPLICATION_F (DIVISION_F (LN_F (L), R), dR)), POWER_F (LN_F (R), NEW_CONST(2)));
        case LN:             return DIVISION_F (dR, R);
        case TAN:            return DIVISION_F (dR, POWER_F (COS_F (R), NEW_CONST (2)));
        case CTAN:           return DIVISION_F (dR, MULTIPLICATION_F (NEW_CONST (-1), POWER_F (SIN_F (R), NEW_CONST (2))));
        case SH:             return MULTIPLICATION_F (dR, CH_F (R));
        case CH:             return MULTIPLICATION_F (dR, SH_F (R));
        case TH:             return DIVISION_F (dR, POWER_F (CH_F (R), NEW_CONST(2)));
        case CTH:            return DIVISION_F (MULTIPLICATION_F (NEW_CONST (-1), dR), POWER_F (SH_F (R), NEW_CONST(2)));
        case ARCSIN:         return DIVISION_F (dR, SQRT_F(SUBTRACTION_F(NEW_CONST(1), POWER_F (R , NEW_CONST(2)))));
        case ARCCOS:         return MULTIPLICATION_F(NEW_CONST (-1), DIVISION_F (dR, SQRT_F(SUBTRACTION_F(NEW_CONST(1), POWER_F (R , NEW_CONST(2))))));
        case ARCTG:          return DIVISION_F (dR, ADD_F (NEW_CONST(1), POWER_F (R, NEW_CONST (2))));
        case ARCCTG:         return MULTIPLICATION_F (NEW_CONST(-1), DIVISION_F (dR, ADD_F (NEW_CONST(1), POWER_F (R, NEW_CONST (2)))));
        case SQRT:           return DIVISION_F (dR, MULTIPLICATION_F (NEW_CONST(2), SQRT_F (R)));
        case POWER:
            if (subRoot->leftChild->type == CONSTANT)
            {
                if (subRoot->rightChild->type == CONSTANT)
                    return CreateNewNode (nullptr, nullptr, 0, CONSTANT, diffTree);

                return MULTIPLICATION_F(MULTIPLICATION_F (POWER_F (L, R), LN_F (L)), dR);
            }
            else
            {
                if (subRoot->rightChild->type == CONSTANT)
                    return MULTIPLICATION_F (NEW_CONST (subRoot->rightChild->memberValue), MULTIPLICATION_F (dL, POWER_F (L, NEW_CONST(subRoot->rightChild->memberValue - 1))));

                return MULTIPLICATION_F (POWER_F (L, R), ADD_F (MULTIPLICATION_F (DIVISION_F(dL, L), R), MULTIPLICATION_F (LN_F (L), dR)));
            }
    }
}

#undef L
#undef R
#undef dR
#undef dL

#undef ADD_F
#undef SUBTRACTION_F
#undef DIVISION_F
#undef MULTIPLICATION_F
#undef POWER_F
#undef SIN_F
#undef COS_F
#undef LOG_F
#undef LN_F
#undef LG_F
#undef SH_F
#undef CH_F
#undef TH_F
#undef CTH_F
#undef SQRT_F
#undef NEW_CONST

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
    fprintf (root->tree->LaTeX_Output, "$");
    CurrentNodeLaTeXOutput (root, 0);
    fprintf (root->tree->LaTeX_Output, "$\n\n");
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
        case ADD:
            CurrentNodeLaTeXOutput (current->leftChild, 0);
            fprintf(current->tree->LaTeX_Output, "+");
            CurrentNodeLaTeXOutput (current->rightChild, 0);
            break;
        case SUBTRACTION:
            CurrentNodeLaTeXOutput (current->leftChild, 0);
            fprintf(current->tree->LaTeX_Output, "-");
            CurrentNodeLaTeXOutput (current->rightChild, 0);
            break;
        case DIVISION:
            fprintf(current->tree->LaTeX_Output, " \\frac{");
            CurrentNodeLaTeXOutput (current->leftChild, 0);
            fprintf(current->tree->LaTeX_Output, "}{");
            CurrentNodeLaTeXOutput (current->rightChild, 0);
            fprintf(current->tree->LaTeX_Output, "} ");
            break;
        case MULTIPLICATION:
            if (NodePriority (current, current->leftChild) > 0)
                CurrentNodeLaTeXOutput (current->leftChild, 1);
            else
                CurrentNodeLaTeXOutput (current->leftChild, 0);
            fprintf(current->tree->LaTeX_Output, " \\cdot ");
            if (NodePriority (current, current->rightChild) > 0)
                CurrentNodeLaTeXOutput (current->rightChild, 1);
            else
                CurrentNodeLaTeXOutput (current->rightChild, 0);
            break;
        case SIN: OneOperandFunctionPrint (current, " \\sin "); break;
        case COS: OneOperandFunctionPrint (current, " \\cos "); break;
        case LG: OneOperandFunctionPrint (current, " \\lg "); break;
        case LOG:
            fprintf(current->tree->LaTeX_Output, "log_");
            CurrentNodeLaTeXOutput (current->rightChild, 0);
            CurrentNodeLaTeXOutput (current->leftChild, 1);
            break;
        case LN: OneOperandFunctionPrint (current, " \\ln "); break;
        case TAN: OneOperandFunctionPrint (current, " \\tg "); break;
        case CTAN: OneOperandFunctionPrint (current, " \\ctg "); break;
        case SH: OneOperandFunctionPrint (current, " \\sh "); break;
        case CH: OneOperandFunctionPrint (current, " \\ch "); break;
        case TH: OneOperandFunctionPrint (current, " \\th "); break;
        case CTH: OneOperandFunctionPrint (current, " \\cth "); break;
        case ARCSIN: OneOperandFunctionPrint (current, " \\arcsin "); break;
        case ARCCOS: OneOperandFunctionPrint (current, " \\arccos "); break;
        case ARCTG: OneOperandFunctionPrint (current, " \\arctg "); break;
        case ARCCTG: OneOperandFunctionPrint (current, " \\arcctg "); break;
        case SQRT:
            fprintf(current->tree->LaTeX_Output, " \\sqrt{");
            CurrentNodeLaTeXOutput (current->rightChild, 1);
            fprintf(current->tree->LaTeX_Output, "}");
            break;
        case POWER:
            if (current->leftChild->type == OPERATION)
                CurrentNodeLaTeXOutput (current->leftChild, true);
            else
                CurrentNodeLaTeXOutput (current->leftChild, false);
            fprintf(current->tree->LaTeX_Output, "^{");
            CurrentNodeLaTeXOutput (current->rightChild, false);
            fprintf(current->tree->LaTeX_Output, "}");
            break;
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
                SubTreeLaTeXOutput (current, DIFFERENTIATE_TYPE);
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
                SubTreeLaTeXOutput (current, DIFFERENTIATE_TYPE);
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
                SubTreeLaTeXOutput (current, DIFFERENTIATE_TYPE);
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
                SubTreeLaTeXOutput (current, DIFFERENTIATE_TYPE);
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