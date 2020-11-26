//
// Created by egolg on 23.11.2020.
//
#include <stdio.h>
#include "cmake-build-debug/Diffur.h"

int main()
{
    /*char* b = "123.01(asd";
    double c = atof (b);
    printf ("%lg",c);*/

    NumericTree tree = {};
    char* fileName = "Num_Logs.dot";
    NumericTreeConstruct (&tree, fileName);
    char* input = "expression.txt";
    ReadDifferentiatorData (input, &tree);
    NumericGraphicDump (&tree);
    //printf ("%c", (int)tree.root->rightChild->rightChild->memberValue + 'a');
    return 0;
}

