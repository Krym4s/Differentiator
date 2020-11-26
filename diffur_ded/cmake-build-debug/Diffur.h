//
// Created by egolg on 25.11.2020.
//

#ifndef DIFFUR_DED_DIFFUR_H
#define DIFFUR_DED_DIFFUR_H

#include "Numeric_Tree.h"
#include "IsE_text.h"

TreeError ReadDifferentiatorData (char* fileNameInput, NumericTree* tree);

NumericNode* DifferentiatorDataInterpreter (char** beginOfExpression, NumericNode* parent, NumericTree* tree);

int CommandDeterminant (char** beginOfExpression, NumericNode* current);

int VertexCommandHandler (char** beginOfExpression, int commandSize, NumericNode* current, double command, NodeType type);

#endif //DIFFUR_DED_DIFFUR_H
