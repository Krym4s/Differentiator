//
// Created by egolg on 25.11.2020.
//

#ifndef DIFFUR_DED_DIFFUR_H
#define DIFFUR_DED_DIFFUR_H

#include "Numeric_Tree.h"
#include "IsE_text.h"

TreeError ReadDifferentiatorData (char* fileNameInput, NumericTree* tree);

NumericNode* DifferentiatorDataInterpreter (char** beginOfExpression, NumericTree* tree);

int CommandDeterminant (char** beginOfExpression, NumericNode* current);

int VertexCommandHandler (char** beginOfExpression, int commandSize, NumericNode* current, double command, NodeType type);

NumericNode* DifferentiateSubTree (NumericNode* subRoot, NumericTree* diffTree);

NumericNode* DifferentiateOperation (NumericNode* subRoot, NumericTree* diffTree);

NumericNode* CopyNode (NumericNode* oldNode, NumericTree* newTree);

void PrepareLatexDocument (NumericTree* tree);

void SubTreeLaTeXOutput (NumericNode* root, PhraseType type);

void CurrentNodeLaTeXOutput (NumericNode* current, bool ParanthesesNeed);

int NodePriority (NumericNode* upper, NumericNode* lower);

void CurrentOperationLaTeXOutput (NumericNode* current);

void PrintGeniousPhrase (PhraseType type);

void CloseLatexOutput (NumericTree* tree);

void OneOperandFunctionPrint (NumericNode* current, char* nameOfFunction);

void SimplifyFunction (NumericTree* tree);

int DFSMULT1 (NumericNode* current, NumericNode* parent);

int DFSMULT0 (NumericNode* current, NumericNode* parent);

int DFSPLUS0 (NumericNode* current, NumericNode* parent);

bool IsZero (double value);



#endif //DIFFUR_DED_DIFFUR_H
