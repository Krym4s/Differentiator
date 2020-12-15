//
// Created by egolg on 23.11.2020.
//
#include <stdio.h>
#include "cmake-build-debug/Diffur.h"

int main()
{

    //----------------------------set randomiser--------------------------
    srand(time(nullptr));

    //------------------------create function tree------------------------
    char* fileName = "Num_Logs.dot";
    char* LaTexName = "diffur.tex";
    NumericTree tree = {};
    FILE* diffLaTex = fopen (LaTexName, "w");
    NumericTreeConstruct (&tree, fileName, LaTexName, diffLaTex);

    //---------------------create vocabulary of genious phrases----------
    Vocabulary vocabulary = {};
    VocabularyConstruct (&vocabulary, "diff_phrases.txt", "simplify_phrases.txt");

    //-------------fill function tree with data about function----------
    char* input = "expression.txt";
    Expression exp ={};
    ExpressionConstruct (&exp, input);
    tree.root = GetG (&exp, &tree);

    //-----------prepare differentiated tree-----------------------------
    char* fileDiffName = "New_Tree.dot";
    NumericTree diffTree = {};
    NumericTreeConstruct (&diffTree, fileDiffName, "diffur.tex", diffLaTex);
    PrepareLatexDocument (&diffTree);
    SubTreeLaTeXOutputEQ (tree.root, NO_PHRASE);
    diffTree.root = DifferentiateSubTree (tree.root, &diffTree, &vocabulary);

    //--------function simplify--------------------------------------------
    fprintf (diffTree.LaTeX_Output, "Функция без упрощения:\n\n");
    SubTreeLaTeXOutput (diffTree.root, NO_PHRASE);
    SimplifyFunction (&diffTree, &vocabulary);
    fprintf (diffTree.LaTeX_Output, "ИТОГ:\n\n");
    //SubTreeLaTeXOutput (diffTree.root, NO_PHRASE);

    //----------idk how to call it, but it's important----------------------
    NumericTree tempTree = {};
    NumericTreeConstruct (&tempTree, "fileName.txt", LaTexName, diffLaTex);
    tempTree.root = CopyNode(diffTree.root, &tempTree);
    printf ("ono rabotaet\n");
    SeparatedTree separator = {};
    SeparateTree (&separator, &tempTree);
    printf ("ono separiruet\n");
    SubTreeLaTeXOutputEQ (tempTree.root, NO_PHRASE);
    fprintf (diffLaTex, "где");
    for (int i = 0 ;i < separator.nSubTrees; i++)
        SubTreeLaTeXOutputWithLetters((separator.subtrees + i)->root, NO_PHRASE, 'A' + i);


    //-------Printing Teylor series--------------------------------------
    double point = 3;
    PrintSeries (&tree, 3, point, 'x');

    //------Printing graphics--------------------------------------------
    char* imageName = "graphic.png";
    makeGraphicInterpretation ("graphic.txt", imageName, &tree, -10, 10, 'x', point, -10, 10);
    InsertGraphicIntoLaTeX (imageName, tree.LaTeX_Output);

    //--------end of programm and closing--------------------------------
    CloseLatexOutput (&diffTree);
    NumericDestructTreeMember (tree.root);
    NumericDestructTreeMember (diffTree.root);
    fclose (diffLaTex);
    system ("pdflatex diffur.tex");
    system ("start diffur.pdf");

    return 0;
}

