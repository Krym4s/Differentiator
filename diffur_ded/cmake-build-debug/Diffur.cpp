//
// Created by egolg on 25.11.2020.
//

#include "Diffur.h"

const double EPSILON = 0.00001;

int VocabularyConstruct (Vocabulary* vocabulary, char* diffFileName, char* simplifyFileName)
{
    assert (diffFileName);
    assert (simplifyFileName);
    char* diffBuffer = "";
    char* simplifyBuffer = "";
    String* diffLines = nullptr;
    String* simplifyLines = nullptr;

    readToStorage (diffFileName, &diffBuffer);
    readToStorage (simplifyFileName, &simplifyBuffer);
    vocabulary->nDiffLines = countSymb (diffBuffer, '\n');
    vocabulary->nSimplifyLines = countSymb (simplifyBuffer, '\n');

    splitTextIntoLines (diffBuffer, vocabulary->nDiffLines, &diffLines);
    splitTextIntoLines (simplifyBuffer, vocabulary->nSimplifyLines, &simplifyLines);

    substituteSymb(diffBuffer, '\n', '\0');
    substituteSymb(simplifyBuffer, '\n', '\0');

    vocabulary->diffBuff = diffBuffer;
    vocabulary->simplifyBuff = simplifyBuffer;
    vocabulary->diffLines = diffLines;
    vocabulary->simplifyLines = simplifyLines;
    return 0;
}

//-------------------------------------------------------------------------

TreeError ReadDifferentiatorData (char* fileNameInput, NumericTree* tree)
{
    assert (fileNameInput);
    assert (tree);

    char* storage = "";
    readToStorage (fileNameInput, &storage);
    char* oldStorage = storage;
    tree->root = DifferentiatorDataInterpreter (&storage, tree);
    free (oldStorage);

    return NO_TREE_ERRORS;
}

//--------------------------------------------------------------------------------

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

//----------------------------------------------------------------------------

int CommandDeterminant (char** beginOfExpression, NumericNode* current)
{
    assert (beginOfExpression);
    assert (current);

    char* endOfExpression = strpbrk (*beginOfExpression, "()");
    int commandSize = endOfExpression - *beginOfExpression;

    if (commandSize <= 0)
        return NO_VALUE;

#define MATH_OPERATION(operationName, operationCode, operationPrintName, diffFunction, printFunction, calcFunc) if (strncmp(operationPrintName,  *beginOfExpression, commandSize) == 0) \
    return VertexCommandHandler (beginOfExpression, commandSize, current, operationName, OPERATION);

#include "operation.txt"
#undef MATH_OPERATION

    if (isalpha(**beginOfExpression))
        return VertexCommandHandler (beginOfExpression, commandSize, current, **beginOfExpression - 'a', VARIABLE);

    if (isdigit(**beginOfExpression) || **beginOfExpression == '.' || **beginOfExpression=='-')
        return VertexCommandHandler (beginOfExpression, commandSize, current, strtod (*beginOfExpression, nullptr), CONSTANT);

    return 1;
}

//-------------------------------------------------------------------

int VertexCommandHandler (char** beginOfExpression, int commandSize, NumericNode* current, double command, NodeType type)
{
    assert (beginOfExpression);
    assert (current);

    *beginOfExpression += commandSize;
    current->memberValue = command;
    current->type = type;
    return 0;
}

NumericNode* DifferentiateSubTree (NumericNode* subRoot, NumericTree* diffTree, Vocabulary* vocabulary)
{
    assert (subRoot);
    assert (diffTree);

    NumericNode* newNode = nullptr;
    switch (subRoot->type)
    {
        case CONSTANT:
            newNode = CreateNewNode (nullptr, nullptr, 0, CONSTANT, diffTree); break;
        case VARIABLE:
            newNode = CreateNewNode (nullptr, nullptr, 1, CONSTANT, diffTree); break;
        case OPERATION:
            newNode = DifferentiateOperation (subRoot, diffTree, vocabulary); break;

    }
    if (vocabulary && diffTree->LaTeX_Output)
    {
        PrintGeniousPhrase (DIFFERENTIATE_TYPE, subRoot->tree->LaTeX_Output, vocabulary);
        SubTreeDiffLaTeXOutput (subRoot, DIFFERENTIATE_TYPE);
        SubTreeLaTeXOutput (newNode, DIFFERENTIATE_TYPE);
    }
    return newNode;
}

//--------------------------------------------------------------------

NumericNode* CopyNode (NumericNode* oldNode, NumericTree* newTree)
{
    assert (oldNode);
    assert (newTree);

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

//---------------------------------------------------------------------------

NumericNode* DifferentiateOperation (NumericNode* subRoot, NumericTree* diffTree, Vocabulary* vocabulary)
{
    assert (subRoot);
    assert (diffTree);

    switch ((Commands) subRoot->memberValue)
    {
#define MATH_OPERATION(operationName, operationCode, operationPrintName, diffFunction, printFunction, calcFunc) case operationName: diffFunction
#include "operation.txt"

#undef MATH_OPERATION
    }
}

//-------------------------------------------------------------------

void PrepareLatexDocument (NumericTree* tree)
{
    assert (tree);

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
             "\\maketitle\n\n"
             "Прикоснуться хотя бы поверхностно к науке нам поможет только чудо, и уж явно не понимание.\n"
             "В этом и состоит причина использования всяческих программ, и не в коем случае собственных рук!!!\n"
             "В качестве поистинне научной задачи предлагаю рассмотреть покрытие продолговатым предметом функцию неравномерного количества долгов студента\n"
             "Так как до великих открытий еще далеко, преедлагается рассматривать так называемый \"болт\" как прямую, в связи с малостью его поперечных размеров.\n\n"
             "Функция долгов студента очевидным образом представляется, как:\n\n");
}

NumericNode* SubTreeLaTeXOutput (NumericNode* root, PhraseType type)
{
    assert (root);
    if (root->tree->LaTeX_Output)
    {
        //PrintGeniousPhrase (type);
        fprintf (root->tree->LaTeX_Output, "\n\n\\begin{center}$");
        CurrentNodeLaTeXOutput (root, false);
        fprintf (root->tree->LaTeX_Output, "$\n\n\\end{center}\n\n");
    }
    return root;
}

NumericNode* SubTreeLaTeXOutputEQ (NumericNode* root, PhraseType type)
{
    assert (root);
    if (root->tree->LaTeX_Output)
    {
        //PrintGeniousPhrase (type);
        fprintf (root->tree->LaTeX_Output, "\\begin{equation}");
        CurrentNodeLaTeXOutput (root, false);
        fprintf (root->tree->LaTeX_Output, "\\end{equation}\n\n");
    }
    return root;
}

NumericNode* SubTreeLaTeXOutputWithLetters (NumericNode* root, PhraseType type, char letter)
{
    assert (root);
    if (root->tree->LaTeX_Output)
    {
        //PrintGeniousPhrase (type);
        fprintf (root->tree->LaTeX_Output, "\\begin{align*} %c =", letter);
        CurrentNodeLaTeXOutput (root, false);
        fprintf (root->tree->LaTeX_Output, "\\end{align*}\n\n");
    }
    return root;
}

NumericNode* SubTreeDiffLaTeXOutput (NumericNode* root, PhraseType type)
{
    assert (root);

    fprintf (root->tree->LaTeX_Output, "\\begin{center}$(");
    CurrentNodeLaTeXOutput (root, false);
    fprintf (root->tree->LaTeX_Output, ")'$\n\n\\end{center}\n\n");
    return root;
}

int NodePriority (NumericNode* upper, NumericNode* lower)
{
    assert (upper);
    assert (lower);

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
    assert (current);

    if (ParanthesesNeed)
        fprintf (current->tree->LaTeX_Output, "(");

    switch (current->type)
    {
        case OPERATION: CurrentOperationLaTeXOutput (current); break;
        case CONSTANT :
            if (current->memberValue < 0)
                fprintf (current->tree->LaTeX_Output, "(%lg)", current->memberValue);
            else
                fprintf (current->tree->LaTeX_Output, "%lg", current->memberValue);
            break;
        case VARIABLE :
            if (current->memberValue <= 'z' - 'a' && current->memberValue >= 0)
                fprintf (current->tree->LaTeX_Output, "%c", (char)current->memberValue + 'a');
            else
                fprintf (current->tree->LaTeX_Output, "%c", (char)current->memberValue);
            break;
    }

    if (ParanthesesNeed)
        fprintf (current->tree->LaTeX_Output, ")");
}

void CurrentOperationLaTeXOutput (NumericNode* current)
{
    assert (current);
    switch ((int)current->memberValue)
    {
#define MATH_OPERATION(operationName, operationCode, operationPrintName, diffFunction, printFunction, calcFunc) case operationName: printFunction
#include "operation.txt"

#undef MATH_OPERATION
    }
}

void PrintGeniousPhrase (PhraseType type, FILE* output, Vocabulary* vocabulary)
{
    if (vocabulary && output)
    switch (type)
    {
        case DIFFERENTIATE_TYPE:
            fprintf (output, "%s \n\n", (vocabulary->diffLines + rand() % vocabulary->nDiffLines)->chLine);
            break;
        case SIMPLIFY_TYPE:
            fprintf (output, "%s \n\n", (vocabulary->simplifyLines + rand() % vocabulary->nSimplifyLines)->chLine);
            break;
    }
}

void CloseLatexOutput (NumericTree* tree)
{
    assert (tree);
    fprintf (tree->LaTeX_Output, "\n\nК сожалению, даже часы, проведенные за игрой в тетрис не помогли нам достичь полного осознания прикладывания \"болта\" к графику, эта способность дается не каждому, и мы пересекали ось долгов, обретая способность летать...\n\n");

    fprintf (tree->LaTeX_Output, "\\section{Список Литературы}\n\n");
    fprintf (tree->LaTeX_Output, "0. https://github.com/Krym4s/Differentiator\n\n"
                                 "1. Деятели русской науки XIX - XX веков. Вып. 2 / РАН, Ин-т ист. естеств. и техники, Ин-т рос. истории; Отв. ред. И.П. Медведев. - СПб.: Дмитрий Буланин, 2001. - 414 с.\n\n"
                                 "2. Сапрыкин Д. Л. Образовательный потенциал Российской Империи. М.:ИИЕТ, 2010\n\n"
                                 "3. Организация науки в России в первой половине XIX века / Г.Е. Павлова; АН СССР, Ин-т истории естествознания и техники; Отв. ред. С.Р. Микулинский. - М. : Наука, 1990. - 239 с.\n\n"
                                 "4. Деятели русской науки XIX - XX веков. Вып. 1 / РАН, Ин-т ист. естеств. и техники, Ин-т рос. истории; Отв. ред. И.П. Медведев. - СПб.: Дмитрий Буланин, 2001.\n\n"
                                 "5. Образование и наука в первой половине XIX в. https://www.yaklass.ru/materiali?mode=lsnthemesubid=31themeid=165\n\n"
                                 "6. 19 век в истории информатики https://intellect.icu/vek-v-istorii-informatiki-  6000\n\n"
    );

    fprintf (tree->LaTeX_Output, "\\end{document}");
    fclose (tree->LaTeX_Output);
}

void OneOperandFunctionPrint (NumericNode* current, char* nameOfFunction)
{
    assert (current);
    assert (nameOfFunction);

    fprintf(current->tree->LaTeX_Output, "%s", nameOfFunction);
    if (current->rightChild->rightChild)
        CurrentNodeLaTeXOutput (current->rightChild, true);
    else
        CurrentNodeLaTeXOutput (current->rightChild, false);
}

void LowerPriorityOperandPrint (NumericNode* current, char* nameOfFunction)
{
    assert (current);
    assert (nameOfFunction);

    CurrentNodeLaTeXOutput (current->leftChild, false);
    fprintf(current->tree->LaTeX_Output, nameOfFunction);
    CurrentNodeLaTeXOutput (current->rightChild, false);
}

void DivisionPrint (NumericNode* current)
{
    assert (current);

    fprintf(current->tree->LaTeX_Output, " \\frac{");
    CurrentNodeLaTeXOutput (current->leftChild, false);
    fprintf(current->tree->LaTeX_Output, "}{");
    CurrentNodeLaTeXOutput (current->rightChild, false);
    fprintf(current->tree->LaTeX_Output, "} ");

}

void MultiplicationPrint (NumericNode* current)
{
    assert (current);
    if (current->leftChild->type == CONSTANT)
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
    else
    {
        if (NodePriority (current, current->rightChild) > 0)
            CurrentNodeLaTeXOutput (current->rightChild, true);
        else
            CurrentNodeLaTeXOutput (current->rightChild, false);
        fprintf(current->tree->LaTeX_Output, " \\cdot ");
        if (NodePriority (current, current->leftChild) > 0)
            CurrentNodeLaTeXOutput (current->leftChild, true);
        else
            CurrentNodeLaTeXOutput (current->leftChild, false);
    }

}

void LogPrint (NumericNode* current)
{
    assert (current);

    fprintf(current->tree->LaTeX_Output, "log_");
    CurrentNodeLaTeXOutput (current->rightChild, false);
    CurrentNodeLaTeXOutput (current->leftChild, true);
}

void SqrtPrint (NumericNode* current)
{
    assert (current);

    fprintf(current->tree->LaTeX_Output, " \\sqrt{");
    CurrentNodeLaTeXOutput (current->rightChild, 1);
    fprintf(current->tree->LaTeX_Output, "}");
}

void PowerPrint (NumericNode* current)
{
    assert (current);
    if (current->leftChild->type == OPERATION)
        CurrentNodeLaTeXOutput (current->leftChild, true);
    else
        CurrentNodeLaTeXOutput (current->leftChild, false);
    fprintf(current->tree->LaTeX_Output, "^{");
    CurrentNodeLaTeXOutput (current->rightChild, false);
    fprintf(current->tree->LaTeX_Output, "}");
}

void SimplifyFunction (NumericTree* tree, Vocabulary* vocabulary)
{
    assert (tree);
    int count = 0;
    do
    {
        count = 0;
        count += DFSMULT1 (tree->root, nullptr, vocabulary);
        count += DFSMULT0 (tree->root, nullptr, vocabulary);
        count += DFSPLUS0 (tree->root, nullptr, vocabulary);
        count += DFSPOW0 (tree->root, nullptr, vocabulary);
        count += DFSDIV1 (tree->root, nullptr, vocabulary);
        count += DFSSUB0 (tree->root, nullptr, vocabulary);
        count += DFS1POW (tree->root, nullptr, vocabulary);
        count += DFSPRECALCULATECONSTS (tree->root, nullptr, vocabulary);
        count += DFSPLUSMINUSMUL (tree->root, nullptr, vocabulary);

    }while (count != 0);
}

int DFSMULT1 (NumericNode* current, NumericNode* parent, Vocabulary* vocabulary)
{
    //assert (vocabulary);

    int changed = 0;
    if (current)
    {
        changed += DFSMULT1 (current->rightChild, current, vocabulary);
        changed += DFSMULT1 (current->leftChild,  current, vocabulary);
        changed += DeleteMeaninglessNode (current, parent, 1, MULTIPLICATION, vocabulary);
    }
    return changed;
}

int DFSDIV1 (NumericNode* current, NumericNode* parent, Vocabulary* vocabulary)
{
    int changed = 0;
    if (current)
    {
        changed += DFSDIV1 (current->rightChild, current, vocabulary);
        changed += DFSDIV1 (current->leftChild,  current, vocabulary);

        if (IsZero(current->memberValue - DIVISION)) {
            if (IsZero(current->rightChild->memberValue - 1)) {
                PrintGeniousPhrase(SIMPLIFY_TYPE, current->tree->LaTeX_Output, vocabulary);
                if (parent)
                    SubTreeLaTeXOutput(parent, DIFFERENTIATE_TYPE);
                else
                    SubTreeLaTeXOutput(current->tree->root, DIFFERENTIATE_TYPE);
                NumericNode *left = current->leftChild;
                current->leftChild = nullptr;
                free (current->rightChild);
                free (current);
                //NumericDestructTreeMember(current);
                changed++;
                if (!parent)
                {
                    left->tree->root = left;
                }

                else if (parent->rightChild == current)
                    parent->rightChild = left;
                else if (parent->leftChild == current)
                    parent->leftChild = left;

                if (!parent)
                    SubTreeLaTeXOutput(left->tree->root, DIFFERENTIATE_TYPE);
                else
                    SubTreeLaTeXOutput(parent, DIFFERENTIATE_TYPE);
            }
        }
    }
    return changed;
}

int DFSPLUS0 (NumericNode* current, NumericNode* parent, Vocabulary* vocabulary)
{
    //assert (vocabulary);

    int changed = 0;
    if (current)
    {
        changed += DFSPLUS0 (current->rightChild, current, vocabulary);
        changed += DFSPLUS0 (current->leftChild,  current, vocabulary);
        changed += DeleteMeaninglessNode (current, parent, 0, ADD, vocabulary);
    }
    return changed;
}

int DFSMULT0 (NumericNode* current, NumericNode* parent, Vocabulary* vocabulary)
{
    //assert (vocabulary);

    int changed = 0;
    if (current)
    {
        changed += DFSMULT0 (current->rightChild, current, vocabulary);
        changed += DFSMULT0 (current->leftChild,  current, vocabulary);
        if (IsZero(current->memberValue - MULTIPLICATION))
        {
            NumericTree* tree = current->tree;
            if (IsZero (current->leftChild->memberValue) || IsZero (current->rightChild->memberValue))
            {
                PrintGeniousPhrase (SIMPLIFY_TYPE, current->tree->LaTeX_Output, vocabulary);
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

int DFSSUB0 (NumericNode* current, NumericNode* parent, Vocabulary* vocabulary)
{
    //assert (vocabulary);

    int changed = 0;
    if (current)
    {
        changed += DFSSUB0 (current->rightChild, current, vocabulary);
        changed += DFSSUB0 (current->leftChild,  current, vocabulary);
        if (IsZero(current->memberValue - SUBTRACTION))
        {
            if (IsZero (current->rightChild->memberValue - 0))
            {
                PrintGeniousPhrase (SIMPLIFY_TYPE, current->tree->LaTeX_Output, vocabulary);
                if (parent && vocabulary)
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
                if (parent && vocabulary)
                    SubTreeLaTeXOutput (parent, DIFFERENTIATE_TYPE);
            }
        }
    }
    return changed;
}

void ReplaceSubTreeWithConstant (NumericTree* tree, NumericNode* parent, NumericNode* current, double value)
{
    if (!parent)
        tree->root = CreateNewNode (nullptr, nullptr, value, CONSTANT, tree);
    else if (parent->rightChild == current)
        parent->rightChild = CreateNewNode (nullptr, nullptr, value, CONSTANT, tree);
    else if (parent->leftChild == current)
        parent->leftChild = CreateNewNode (nullptr, nullptr, value, CONSTANT, tree);
}

int DeleteMeaninglessNode (NumericNode* current, NumericNode* parent, double constant, Commands operation, Vocabulary* vocabulary)
{
    int changed = 0;
    if (IsZero(current->memberValue - operation))
    {
        if (IsZero (current->leftChild->memberValue - constant))
        {
            PrintGeniousPhrase (SIMPLIFY_TYPE, current->tree->LaTeX_Output, vocabulary);
            if (parent && vocabulary)
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
            if (vocabulary)
                if (!parent)
                    SubTreeLaTeXOutput (right->tree->root, DIFFERENTIATE_TYPE);
                else
                    SubTreeLaTeXOutput (parent, DIFFERENTIATE_TYPE);
        } else if (IsZero (current->rightChild->memberValue - constant))
        {
            PrintGeniousPhrase (SIMPLIFY_TYPE, current->tree->LaTeX_Output, vocabulary);
            if (parent)
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

            if (vocabulary)
                if (!parent)
                    SubTreeLaTeXOutput (left->tree->root, DIFFERENTIATE_TYPE);
                else
                    SubTreeLaTeXOutput (parent, DIFFERENTIATE_TYPE);
        }
    }
    return changed;
}

int DeleteMeaninglessRightNode (NumericNode* current, NumericNode* parent, double constant, Commands operation, Vocabulary* vocabulary)
{
    int changed = 0;
    if (IsZero(current->memberValue - operation) && IsZero(current->leftChild->memberValue - constant)) {
        PrintGeniousPhrase(SIMPLIFY_TYPE, current->tree->LaTeX_Output, vocabulary);
        if (parent && vocabulary)
            SubTreeLaTeXOutput(parent, DIFFERENTIATE_TYPE);
        NumericNode *right = current->rightChild;
        current->rightChild = nullptr;
        NumericDestructTreeMember(current);
        changed++;
        if (!parent)
            right->tree->root = right;
        else if (parent->rightChild == current)
            parent->rightChild = right;
        else if (parent->leftChild == current)
            parent->leftChild = right;
        if (vocabulary)
            if (!parent)
                SubTreeLaTeXOutput(right->tree->root, DIFFERENTIATE_TYPE);
            else
                SubTreeLaTeXOutput(parent, DIFFERENTIATE_TYPE);
    }

    return changed;
}

int DeleteMeaninglessLeftNode (NumericNode* current, NumericNode* parent, double constant, Commands operation, Vocabulary* vocabulary)
{
    int changed = 0;
    if (IsZero(current->memberValue - operation) && IsZero (current->rightChild->memberValue - constant))
    {
        PrintGeniousPhrase (SIMPLIFY_TYPE, current->tree->LaTeX_Output, vocabulary);
        if (parent)
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

        if (vocabulary)
            if (!parent)
                SubTreeLaTeXOutput (left->tree->root, DIFFERENTIATE_TYPE);
            else
                SubTreeLaTeXOutput (parent, DIFFERENTIATE_TYPE);
    }
    return changed;
}

int DFSPOW0 (NumericNode* current, NumericNode* parent, Vocabulary* vocabulary)
{
    //assert (vocabulary);

    int changed = 0;
    if (current)
    {
        changed += DFSPOW0 (current->rightChild, current, vocabulary);
        changed += DFSPOW0 (current->leftChild,  current, vocabulary);
        if (IsZero(current->memberValue - POWER))
        {
            NumericTree* tree = current->tree;
            if (IsZero (current->rightChild->memberValue))
            {
                PrintGeniousPhrase (SIMPLIFY_TYPE, current->tree->LaTeX_Output, vocabulary);
                if (vocabulary)
                    SubTreeLaTeXOutput (current, DIFFERENTIATE_TYPE);
                changed++;
                if (!parent)
                    tree->root = CreateNewNode (nullptr, nullptr, 1, CONSTANT, tree);
                else if (parent->rightChild == current)
                    parent->rightChild = CreateNewNode (nullptr, nullptr, 1, CONSTANT, tree);

                NumericDestructTreeMember (current);
            }
        }
    }
    return changed;
}

int DFS1POW (NumericNode* current, NumericNode* parent, Vocabulary* vocabulary)
{
    //assert (vocabulary);

    int changed = 0;
    if (current)
    {
        changed += DFS1POW (current->rightChild, current, vocabulary);
        changed += DFS1POW (current->leftChild,  current, vocabulary);
        if (IsZero(current->memberValue - POWER))
        {
            NumericTree* tree = current->tree;
            if (IsZero (current->leftChild->memberValue - 1))
            {
                PrintGeniousPhrase (SIMPLIFY_TYPE, current->tree->LaTeX_Output, vocabulary);
                if (vocabulary)
                    SubTreeLaTeXOutput (current, DIFFERENTIATE_TYPE);
                changed++;
                NumericDestructTreeMember (current->rightChild);
                NumericDestructTreeMember (current->leftChild);
                current->type = CONSTANT;
                current->leftChild = nullptr;
                current->rightChild = nullptr;
                current->memberValue = 1;
            }
        }
    }
    return changed;
}

int DFSPRECALCULATECONSTS (NumericNode* current, NumericNode* parent, Vocabulary* vocabulary)
{
    int change = 0;
    if (current) {
        change += DFSPRECALCULATECONSTS (current->rightChild, current, vocabulary);
        change += DFSPRECALCULATECONSTS (current->leftChild, current, vocabulary);
        if (current->rightChild && current->leftChild)
        if (current->type == OPERATION && current->leftChild->type == CONSTANT &&
            current->rightChild->type == CONSTANT) {
            switch ((int) current->memberValue) {
                case ADD:
                    change += PrecalculatedSucessfulPrint (current, vocabulary);
                    current->memberValue = current->leftChild->memberValue + current->rightChild->memberValue;
                    break;
                case SUBTRACTION:
                    change += PrecalculatedSucessfulPrint (current, vocabulary);
                    current->memberValue = current->leftChild->memberValue - current->rightChild->memberValue;
                    break;
                case MULTIPLICATION:
                    change += PrecalculatedSucessfulPrint (current, vocabulary);
                    current->memberValue = current->leftChild->memberValue * current->rightChild->memberValue;
                    break;
                case DIVISION:
                    change += PrecalculatedSucessfulPrint (current, vocabulary);
                    current->memberValue = current->leftChild->memberValue / current->rightChild->memberValue;
                    break;
                case POWER:
                    change += PrecalculatedSucessfulPrint (current, vocabulary);
                    current->memberValue = pow(current->leftChild->memberValue, current->rightChild->memberValue);
                    break;

            }
            current->type = CONSTANT;
            free(current->leftChild);
            free(current->rightChild);
            current->rightChild = nullptr;
            current->leftChild = nullptr;
            if (vocabulary)
                SubTreeLaTeXOutput(current, DIFFERENTIATE_TYPE);
        }
    }
    return change;
}

int PrecalculatedSucessfulPrint (NumericNode* current, Vocabulary* vocabulary)
{
    PrintGeniousPhrase(SIMPLIFY_TYPE, current->tree->LaTeX_Output, vocabulary);
    if (vocabulary)
        SubTreeLaTeXOutput(current, DIFFERENTIATE_TYPE);
    return 1;
}

bool IsZero (double value)
{
    if (fabs (value) < EPSILON)
        return true;
    else
        return false;
}

double DFS_TreeCalculate (NumericNode* current)
{
    assert (current);

    if (current->type == VARIABLE)
        return current->tree->variables[(int)current->memberValue];

    else if (current->type == CONSTANT)
        return current->memberValue;

    else
        switch ((int)current->memberValue) {
#define MATH_OPERATION(operationName, operationCode, operationPrintName, diffFunction, printFunction, calcFunc) case operationCode: calcFunc;
#include "operation.txt"

#undef MATH_OPERATION

        }
    return 0;
}

int PrintSeries (NumericTree* tree, unsigned int degree, double point, char variable)
{
    assert (tree);
    int vIndex = getVariableCode (variable);
    tree->variables[vIndex] = point;

    double* coefArrays = (double *) calloc (degree + 1, sizeof (*coefArrays));

    coefArrays[0] = DFS_TreeCalculate (tree->root);

    NumericTree* diffNtree = (NumericTree*) calloc (degree + 1, sizeof (*diffNtree));
    diffNtree->root = tree->root;

    for (int i = 1; i <= degree; i++)
    {
        NumericTreeConstruct (diffNtree + i, tree->graph_logs_name, "tt.txt", nullptr);
        diffNtree[i].variables[vIndex] = tree->variables[vIndex];
        diffNtree[i].root = DifferentiateSubTree (diffNtree[i-1].root, diffNtree + i, nullptr);
        SimplifyFunction (diffNtree + i, nullptr);
        diffNtree[i].LaTeX_Output = nullptr;
        coefArrays[i] = DFS_TreeCalculate (diffNtree[i].root);
    }
    fprintf (tree->LaTeX_Output, "\n\nВспоминая свой опыт, забивание произошло примерно в точке изуения рядов Тейлора, вот его члены слева на право, что на нашем графике соответсвует значению %lg.\n\n \\begin{align*}", point);
    fprintf (tree->LaTeX_Output, "f(x) = %.3lg + ", coefArrays[0]);
    if (point > 0)
        fprintf (tree->LaTeX_Output, "\\frac{%.3lg}{1!} \\cdot (x - %lg) + ", coefArrays[1], point);
    else if (point < 0)
        fprintf (tree->LaTeX_Output, "\\frac{%.3lg}{1!} \\cdot (x + %lg) + ", coefArrays[1], -point);
    else
        fprintf (tree->LaTeX_Output, "\\frac{%.3lg}{1!} \\cdot x + ", coefArrays[1]);

    for (int i = 2; i <= degree; i++)
        if (point > 0)
            fprintf (tree->LaTeX_Output, "\\frac{%.3lg}{%d!} \\cdot (x - %lg)^{%d} + ", coefArrays[i], i, point, i);
        else if (point < 0)
            fprintf (tree->LaTeX_Output, "\\frac{%.3lg}{%d!} \\cdot (x + %lg)^{%d} + ", coefArrays[i], i, -point, i);
        else
            fprintf (tree->LaTeX_Output, "\\frac{%.3lg}{%d!} \\cdot x^{%d} + ", coefArrays[i], i, i);


    if (point > 0)
        fprintf (tree->LaTeX_Output, "o((x - %lg)^{%d})", point, degree + 1);
    else if (point < 0)
        fprintf (tree->LaTeX_Output, "o((x + %lg)^{%d})", -point, degree + 1);
    else
        fprintf (tree->LaTeX_Output, "o(x^{%d})", degree + 1);

    fprintf (tree->LaTeX_Output, "\\end{align*}\n\n");
    return 0;
}

int getVariableCode (char variable)
{
    return variable - 'a';
}

int makeGraphicInterpretation (char* TextFileName, char* PNG_FileName, NumericTree* tree, double beginValue, double endValue, char variable, double interestingPoint, double lowerBorder, double upperBorder)
{
    double maxValue = -100000;
    double minValue =  100000;
    if (interestingPoint > endValue || interestingPoint < beginValue)
        return 1;

    FILE* textOutput = fopen (TextFileName, "w");
    double step = (endValue - beginValue)/10000;

    for (double point = beginValue; point <= endValue; point += step)
    {
        tree->variables[getVariableCode(variable)] = point;
        double tempValue = DFS_TreeCalculate(tree->root);
        fprintf (textOutput, "%lg %lg\n", point, tempValue);
        if (tempValue < minValue)
            minValue = tempValue;
        if (tempValue > maxValue)
            maxValue = tempValue;
    }
    fclose (textOutput);
    textOutput = fopen ("linear.txt", "w");
    NumericTree diffTree = {};
    NumericTreeConstruct (&diffTree, nullptr, "nofile", nullptr);
    diffTree.variables[getVariableCode(variable)] = interestingPoint;
    diffTree.root = DifferentiateSubTree (tree->root, &diffTree, nullptr);
    double k = DFS_TreeCalculate (diffTree.root);
    printf ("%lg", k);
    tree->variables[getVariableCode(variable)] = interestingPoint;
    double interestingFunctionValue = DFS_TreeCalculate (tree->root);

    if (interestingFunctionValue + (beginValue - interestingPoint) * k < minValue)
        minValue = interestingFunctionValue + (beginValue - interestingPoint) * k;
    if (interestingFunctionValue + (beginValue - interestingPoint) * k > maxValue)
        maxValue = interestingFunctionValue + (beginValue - interestingPoint) * k;

    if (interestingFunctionValue + (endValue - interestingPoint) * k < minValue)
        minValue = interestingFunctionValue + (endValue - interestingPoint) * k;
    if (interestingFunctionValue + (endValue - interestingPoint) * k > maxValue)
        maxValue = interestingFunctionValue + (endValue - interestingPoint) * k;

    fprintf (textOutput, "%lg %lg\n", beginValue, interestingFunctionValue + (beginValue - interestingPoint) * k);
    fprintf (textOutput, "%lg %lg\n", endValue, interestingFunctionValue + (endValue - interestingPoint)*k);
    fclose(textOutput);
    system ("cd gnuplot\\bin");
    textOutput = fopen ("gnuplot\\bin\\gnuplotcommands.txt", "w");
    if (maxValue > upperBorder)
        maxValue = upperBorder;
    if (minValue < lowerBorder)
        minValue = lowerBorder;
    fprintf (textOutput, "set xrange [%lg:%lg]\n"
                         "set yrange [%lg:%lg]\n"
                         "set xlabel \"x\"\n"
                         "set ylabel \"y\"\n"
                         "set terminal png\n"
                         "set output \"%s\"\n"
                         "plot \"%s\" with lines, \"linear.txt\" with lines\"\n",
                         beginValue, endValue,
                         minValue, maxValue,
                         PNG_FileName,
                         TextFileName
    );
    fclose (textOutput);
    system ("start gnuplot.exe gnuplot\\bin\\gnuplotcommands.txt");

    return 0;
}

void InsertGraphicIntoLaTeX (char* fileName, FILE* output)
{
    fprintf (output, "Однако, все эти преобразования и мороки - сущая ерунда и пустая трата времени. наиболее надежный метод вычислить производную - чиркнуть на глаз касательную.\n\n");
    fprintf (output, "\\includegraphics{%s}", fileName);
}

int DFSPLUSMINUSMUL (NumericNode* current, NumericNode* parent, Vocabulary* vocabulary)
{
    int changed = 0;
    if (current)
    {
        changed += DFSPLUSMINUSMUL (current->rightChild, current, vocabulary);
        changed += DFSPLUSMINUSMUL (current->leftChild,  current, vocabulary);
        if (current->type == OPERATION && current->memberValue == MULTIPLICATION)
        {
            if (parent->memberValue == ADD)
            {
                if (current->leftChild->type == CONSTANT && current->leftChild->memberValue == -1)
                {
                    parent->memberValue = SUBTRACTION;
                    if (parent->rightChild == current)
                        parent->rightChild = current->rightChild;
                    else
                        parent->leftChild = current->rightChild;

                    free (current->leftChild);
                    free (current);
                    return changed + 1;
                }
                else if (current->rightChild->type == CONSTANT && current->rightChild->memberValue == -1)
                {
                    parent->memberValue = SUBTRACTION;
                    if (parent->rightChild == current)
                        parent->rightChild = current->leftChild;
                    else
                        parent->leftChild = current->leftChild;

                    free (current->rightChild);
                    free (current);
                    return changed + 1;
                }
            }
            else if (parent->memberValue == SUBTRACTION)
            {
                if (current->leftChild->type == CONSTANT && current->leftChild->memberValue == -1)
                {
                    parent->memberValue = ADD;
                    if (parent->rightChild == current)
                        parent->rightChild = current->rightChild;
                    else
                        parent->leftChild = current->rightChild;

                    free (current->leftChild);
                    free (current);
                    return changed + 1;
                }
                else if (current->rightChild->type == CONSTANT && current->rightChild->memberValue == -1)
                {
                    parent->memberValue = ADD;
                    if (parent->rightChild == current)
                        parent->rightChild = current->leftChild;
                    else
                        parent->leftChild = current->leftChild;

                    free (current->rightChild);
                    free (current);
                    return changed + 1;
                }
            }
        }
    }
    return changed;
}


int SeparateTree(SeparatedTree* separator, NumericTree* tree)
{
    separator->nSubTrees = 0;
    separator->subtrees = (NumericTree*) calloc (RECOMMENDED_NSUBTREES, sizeof(*separator->subtrees));
    if (!separator->subtrees)
        return -1;

    return SubTreeSeparator (separator, tree->root, nullptr);
}

int SubTreeSeparator (SeparatedTree* separator, NumericNode* subRoot, NumericNode* parent)
{
    assert (separator);

    int depth = 0;
    if (subRoot) {
        if (subRoot->type == VARIABLE || subRoot->type == CONSTANT)
            depth = 1;
        else if (subRoot->rightChild && subRoot->leftChild)
            depth += (SubTreeSeparator(separator, subRoot->leftChild,  subRoot) +
                      SubTreeSeparator(separator, subRoot->rightChild, subRoot) + 1);
        else if (subRoot->rightChild)
            depth += (SubTreeSeparator(separator, subRoot->rightChild, subRoot) + 1);
        else if (subRoot->leftChild)
            depth += (SubTreeSeparator(separator, subRoot->leftChild, subRoot) + 1);

        if (depth >= RECOMMENDED_DEPTH && parent != nullptr) {
            (separator->subtrees + separator->nSubTrees)->LaTeX_Output = subRoot->tree->LaTeX_Output;
            (separator->subtrees + separator->nSubTrees)->root = CopyNode (subRoot,separator->subtrees + separator->nSubTrees);

            subRoot->rightChild = nullptr;
            subRoot->leftChild = nullptr;
            subRoot->type = VARIABLE;
            subRoot->memberValue = 'A' + separator->nSubTrees;

            separator->nSubTrees++;
            depth = 1;
        }
    }
    return depth;
}