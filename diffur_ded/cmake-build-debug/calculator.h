//
// Created by egolg on 05.12.2020.
//

#ifndef ALCULATOR_CALCULATOR_H
#define ALCULATOR_CALCULATOR_H

#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <ctype.h>
#include <assert.h>
#include <cmath>
#include "IsE_text.h"
#include "Numeric_Tree.h"

struct Expression
{
    char* buffer = "";
    unsigned int index = 0;
};

enum CalculationError
{
    NO_CALCERRORS,
    NO_BUFFER
};

int ExpressionConstruct (Expression* exp, char* fileName);

NumericNode* GetG (Expression* expression, NumericTree* tree);

NumericNode* Get3rdPriority (Expression* expression, NumericTree* tree);

NumericNode* Get2ndPriority (Expression* expression, NumericTree* tree);

NumericNode* Get1stPriority (Expression* expression, NumericTree* tree);

NumericNode* GetParanthesesExpression (Expression* expression, NumericTree* tree);

NumericNode* GetFunction (Expression* expression, NumericTree* tree);

NumericNode* GetNumber (Expression* expression, NumericTree* tree);

void SkipSpaces (Expression* expression);

void SyntaxError();

void Require (char symb, Expression* expression);

NumericNode* GetVariable (Expression* expression, NumericTree* tree);
#endif //ALCULATOR_CALCULATOR_H
