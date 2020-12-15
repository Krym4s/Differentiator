//
// Created by egolg on 25.11.2020.
//

#ifndef DIFFUR_DED_DIFFUR_H
#define DIFFUR_DED_DIFFUR_H

#include <ctime>
#include "Numeric_Tree.h"
#include "IsE_text.h"
#include "calculator.h"
#define _USE_MATH_DEFINES // for C
#include <math.h>

struct Vocabulary
{
    char* diffBuff;
    char* simplifyBuff;
    size_t nDiffLines;
    size_t nSimplifyLines;
    String* diffLines;
    String* simplifyLines;
};

struct SeparatedTree
{
    NumericTree* subtrees;
    unsigned int nSubTrees;
};

const unsigned int RECOMMENDED_NSUBTREES = 20000;

const unsigned int RECOMMENDED_DEPTH = 40;

int SeparateTree (SeparatedTree* separator, NumericTree* tree);

int SubTreeSeparator (SeparatedTree* separator, NumericNode* subRoot, NumericNode* parent);

int VocabularyConstruct (Vocabulary* vocabulary, char* diffFileName, char* simplifyFileName);

TreeError ReadDifferentiatorData (char* fileNameInput, NumericTree* tree);

NumericNode* DifferentiatorDataInterpreter (char** beginOfExpression, NumericTree* tree);

int CommandDeterminant (char** beginOfExpression, NumericNode* current);

int VertexCommandHandler (char** beginOfExpression, int commandSize, NumericNode* current, double command, NodeType type);

NumericNode* DifferentiateSubTree (NumericNode* subRoot, NumericTree* diffTree, Vocabulary* vocabulary);

NumericNode* DifferentiateOperation (NumericNode* subRoot, NumericTree* diffTree, Vocabulary* vocabulary);

NumericNode* CopyNode (NumericNode* oldNode, NumericTree* newTree);

void PrepareLatexDocument (NumericTree* tree);

NumericNode* SubTreeLaTeXOutput (NumericNode* root, PhraseType type);

NumericNode* SubTreeDiffLaTeXOutput (NumericNode* root, PhraseType type);

void CurrentNodeLaTeXOutput (NumericNode* current, bool ParanthesesNeed);

int NodePriority (NumericNode* upper, NumericNode* lower);

void CurrentOperationLaTeXOutput (NumericNode* current);

void PrintGeniousPhrase (PhraseType type, FILE* output, Vocabulary* vocabulary);

void CloseLatexOutput (NumericTree* tree);

void OneOperandFunctionPrint (NumericNode* current, char* nameOfFunction);

void LowerPriorityOperandPrint (NumericNode* current, char* nameOfFunction);

void DivisionPrint (NumericNode* current);

void MultiplicationPrint (NumericNode* current);

void LogPrint (NumericNode* current);

void SqrtPrint (NumericNode* current);

void PowerPrint (NumericNode* current);

void SimplifyFunction (NumericTree* tree, Vocabulary* vocabulary);

int DFSMULT1 (NumericNode* current, NumericNode* parent, Vocabulary* vocabulary);

int DFSDIV1 (NumericNode* current, NumericNode* parent, Vocabulary* vocabulary);

int DFSMULT0 (NumericNode* current, NumericNode* parent, Vocabulary* vocabulary);

int DFSPLUS0 (NumericNode* current, NumericNode* parent, Vocabulary* vocabulary);

int DFSPOW0 (NumericNode* current, NumericNode* parent, Vocabulary* vocabulary);

int DFS1POW (NumericNode* current, NumericNode* parent, Vocabulary* vocabulary);

int DFSPRECALCULATECONSTS (NumericNode* current, NumericNode* parent, Vocabulary* vocabulary);

int DFSSUB0 (NumericNode* current, NumericNode* parent, Vocabulary* vocabulary);

void ReplaceSubTreeWithConstant (NumericTree* tree, NumericNode* parent, NumericNode* current, double value);

int DeleteMeaninglessNode (NumericNode* current, NumericNode* parent, double constant, Commands operation, Vocabulary* vocabulary);

int DeleteMeaninglessRightNode (NumericNode* current, NumericNode* parent, double constant, Commands operation, Vocabulary* vocabulary);

int DeleteMeaninglessLeftNode (NumericNode* current, NumericNode* parent, double constant, Commands operation, Vocabulary* vocabulary);

int PrecalculatedSucessfulPrint (NumericNode* current, Vocabulary* vocabulary);

double DFS_TreeCalculate (NumericNode* current);

int PrintSeries (NumericTree* tree, unsigned int degree, double point, char variable);

int makeGraphicInterpretation (char* TextFileName, char* PNG_FileName, NumericTree* tree, double beginValuem, double endValue, char variable, double interestingPoint, double lowerBorder, double upperBorder);

void InsertGraphicIntoLaTeX (char* fileName, FILE* output);

int getVariableCode (char variable);

bool IsZero (double value);

NumericNode* FindEqualSubTree(NumericNode* root, NumericNode* current);

int DFSPLUSMINUSMUL (NumericNode* current, NumericNode* parent, Vocabulary* vocabulary);

NumericNode* SubTreeLaTeXOutputWithLetters (NumericNode* root, PhraseType type, char letter);

NumericNode* SubTreeLaTeXOutputEQ (NumericNode* root, PhraseType type);

#endif //DIFFUR_DED_DIFFUR_H
