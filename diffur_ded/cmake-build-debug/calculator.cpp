//
// Created by egolg on 05.12.2020.
//

#include "calculator.h"

NumericNode* GetG (Expression* expression, NumericTree* tree)
{
    expression->index = 0;
    NumericNode* current = Get3rdPriority (expression, tree);
    Require ('$', expression);
    return current;
}

void SkipSpaces (Expression* expression)
{
    while (isspace(expression->buffer[expression->index]))
        expression->index++;

    printf ("%c\n",expression->buffer[expression->index]);
}

NumericNode* Get3rdPriority (Expression* expression, NumericTree* tree)
{
    NumericNode* left = Get2ndPriority (expression, tree);
    NumericNode* retNode = NULL;

    if (expression->buffer[expression->index] == '+' || expression->buffer[expression->index] == '-'){
        unsigned int oldIndex = expression->index++;
        NumericNode* right = Get2ndPriority (expression, tree);
        NumericNode* current = retNode;
        if (expression->buffer[oldIndex] == '+') {
            current = retNode = CreateNewNode (left, right, ADD, OPERATION, tree);
        }
        else {
            current = retNode = CreateNewNode (left, right, SUBTRACTION, OPERATION, tree);
        }
        while (expression->buffer[expression->index] == '+' || expression->buffer[expression->index] == '-')
        {
            oldIndex = expression->index++;
            right = Get2ndPriority (expression, tree);
            left = current->rightChild;
            if (expression->buffer[oldIndex] == '+') {
                current->rightChild = CreateNewNode (left, right, ADD, OPERATION, tree);
                current = current->rightChild;
            }
            else {
                current->rightChild = CreateNewNode (left, right, SUBTRACTION, OPERATION, tree);
                current = current->rightChild;
            }
        }
    }
    else
    {
        retNode = left;
    }
    return retNode;
}

NumericNode* Get2ndPriority (Expression* expression, NumericTree* tree)
{
    NumericNode* left = Get1stPriority (expression, tree);
    NumericNode* retNode = NULL;

    if (expression->buffer[expression->index] == '*' || expression->buffer[expression->index] == '/'){
        unsigned int oldIndex = expression->index++;
        NumericNode* right = Get1stPriority (expression, tree);
        NumericNode* current = retNode;
        if (expression->buffer[oldIndex] == '*') {
            current = retNode = CreateNewNode (left, right, MULTIPLICATION, OPERATION, tree);
        }
        else {
            current = retNode = CreateNewNode (left, right, DIVISION, OPERATION, tree);
        }
        while (expression->buffer[expression->index] == '*' || expression->buffer[expression->index] == '/')
        {
            oldIndex = expression->index++;
            right = Get1stPriority (expression, tree);
            left = current->rightChild;
            if (expression->buffer[oldIndex] == '*') {
                current->rightChild = CreateNewNode (left, right, MULTIPLICATION, OPERATION, tree);
                current = current->rightChild;
            }
            else {
                current->rightChild = CreateNewNode (left, right, DIVISION, OPERATION, tree);
                current = current->rightChild;
            }
        }
    }
    else
    {
        retNode = left;
    }

    return retNode;
}

NumericNode* Get1stPriority (Expression* expression, NumericTree* tree)
{
    NumericNode* retNode = NULL;
    NumericNode* left = GetFunction (expression, tree);
    if (expression->buffer[expression->index] == '^'){
        expression->index++;
        NumericNode* right = GetFunction (expression, tree);
        NumericNode* current = retNode = CreateNewNode (left, right, '^', OPERATION, tree);
        while (expression->buffer[expression->index] == '^')
        {
            expression->index++;
            current->rightChild = CreateNewNode (current->rightChild, GetFunction (expression, tree),POWER, OPERATION, tree);
            current = current->rightChild;
        }
    }
    else{
        retNode = left;
    }

    return retNode;
}

NumericNode* GetFunction (Expression* expression, NumericTree* tree)
{
    if (isalpha (expression->buffer[expression->index]) && isalpha(expression->buffer[expression->index + 1]) ){
        int FunctionNameLength = strchr (expression->index + expression->buffer, '(') - expression->buffer - expression->index;
        char* operation = strpbrk (expression->buffer + expression->index, "+-*/^");
        if (FunctionNameLength < 0 && !operation)
            SyntaxError();

        #define  MATH_OPERATION(operationName, operationCode, operationPrintName, diffFunction, printFunction, calcFunc) ;\
        if (strncmp (expression->buffer + expression->index, operationPrintName, FunctionNameLength) == 0){     \
             expression->index += FunctionNameLength;                                                           \
             return CreateNewNode (nullptr, GetParanthesesExpression (expression, tree), operationCode, OPERATION, tree);\
        }
        #include "operation.txt"
        #undef MATH_OPERATION
            SyntaxError();
    }
    else{
        return GetParanthesesExpression (expression, tree);
    }
}

NumericNode* GetVariable (Expression* expression, NumericTree* tree)
{
    char c = expression->buffer[expression->index] - 'a';
    expression->index++;
    /*unsigned int oldPointer = expression->index;
    while (isalpha(expression->buffer[expression->index])){
        expression->index++;
    }*/
    return CreateNewNode (nullptr, nullptr, c, VARIABLE, tree);
}

NumericNode* GetParanthesesExpression (Expression* expression, NumericTree* tree)
{
    if (expression->buffer[expression->index] == '(') {
        expression->index++;
        NumericNode* current = Get3rdPriority (expression, tree);
        Require (')', expression);

        return current;
    }
    else {
        if (isdigit(expression->buffer[expression->index]))
            return GetNumber (expression, tree);
        else
            return GetVariable (expression, tree);
    }
}

void Require (char symb, Expression* expression)
{
    assert(expression);

    if (expression->buffer[expression->index] == symb){
        expression->index++;
    }
    else {
        SyntaxError();
    }
}

int ExpressionConstruct (Expression* exp, char* fileName)
{
    readToStorage (fileName, &(exp->buffer));
    if (!exp->buffer)
        return NO_BUFFER;
    deleteSymb (exp->buffer,' ');
    deleteSymb (exp->buffer, '\t');
    int nCh = strlen(exp->buffer);
    exp->buffer[nCh - 2] = '$';
    exp->index = 0;
    return NO_ERRORS;
}

NumericNode* GetNumber (Expression* expression, NumericTree* tree)
{
    double value = 0;
    unsigned int oldPointer = expression->index;
    while (isdigit(expression->buffer[expression->index]))
    {
        value = 10 * value + expression->buffer[expression->index] - '0';
        expression->index++;
    }
    if (oldPointer == expression->index)
        SyntaxError();
    double power = 1;
    if (expression->buffer[expression->index] == '.'){
        expression->index++;
        while (isdigit(expression->buffer[expression->index])){
            power /= 10;
            value += (double)(expression->buffer[expression->index] - '0') * power;
            expression->index++;
        }
    }
    return CreateNewNode (nullptr, nullptr, value, CONSTANT, tree);
}

void SyntaxError()
{
    exit(1);
}