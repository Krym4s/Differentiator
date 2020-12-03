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
    char* fileDiffName = "New_Tree.dot";
    NumericTreeConstruct (&tree, fileName, "diffur.tex");
    char* input = "expression.txt";
    ReadDifferentiatorData (input, &tree);
    //NumericGraphicDump (&tree);
    NumericTree diffTree = {};
    NumericTreeConstruct (&diffTree, fileDiffName, "diffur.tex");
    diffTree.root = DifferentiateSubTree (tree.root, &diffTree);
    printf ("%c\n", (char)diffTree.root->leftChild->rightChild->memberValue + '0');
    //NumericGraphicDump (&diffTree);
    PrepareLatexDocument (&diffTree);
    SubTreeLaTeXOutput (diffTree.root, DIFFERENTIATE_TYPE);
    SimplifyFunction (&diffTree);
    SubTreeLaTeXOutput (diffTree.root, DIFFERENTIATE_TYPE);
    NumericTree d2 = {};
    NumericTree d3 = {};
    //NumericTree d4 = {};
    //NumericTree d5 = {};
    NumericTreeConstruct(&d2, fileDiffName, "diffur.tex");
    NumericTreeConstruct(&d3, fileDiffName, "diffur.tex");
    //NumericTreeConstruct(&d4, fileDiffName, "diffur.tex");
    //NumericTreeConstruct(&d5, fileDiffName, "diffur.tex");
    d2.root = DifferentiateSubTree (diffTree.root, &d2);
    SimplifyFunction (&d2);
    SubTreeLaTeXOutput (d2.root, DIFFERENTIATE_TYPE);
    d3.root = DifferentiateSubTree (d2.root, &d3);
    SimplifyFunction (&d3);
    SubTreeLaTeXOutput (d3.root, DIFFERENTIATE_TYPE);
    //d4.root = DifferentiateSubTree (d3.root, &d4);
    //SimplifyFunction (&d4);
    //SubTreeLaTeXOutput (d4.root, DIFFERENTIATE_TYPE);
    //d5.root = DifferentiateSubTree (d4.root, &d5);
    //SimplifyFunction (&d5);
    //SubTreeLaTeXOutput (d5.root, DIFFERENTIATE_TYPE);
    CloseLatexOutput (&diffTree);
    NumericDestructTreeMember (tree.root);
    NumericDestructTreeMember (diffTree.root);
    //system ("latex diffur.tex");
    //printf ("%c", (int)tree.root->rightChild->rightChild->memberValue + 'a');
    return 0;
}

