//
// Created by egolg on 13.11.2020.
//

#include <windef.h>
#include "IsE_Tree.h"

#define TREERELEASE

#ifdef TREERELEASE
#define ASSERTEDTREE
#define ASSERTEDTREEMEMBER
#else
#define ASSERTEDTREE !TreeVerify (thou) || GraphicDump (thou);
#define ASSERTEDTREEMEMBER !TreeVerify (thou->tree) || GraphicDump (thou->tree);
#endif

TreeError TreeConstruct (Tree* thou, char* graph_logs)
{
    assert (thou);
    thou->size = 0;
    thou->root = NULL;
    thou->graph_logs_name = graph_logs;

    return NO_TREE_ERRORS;
}

TreeMember* TreeCreate (Tree* thou, MemberValue value)
{
    assert (thou);
    ASSERTEDTREE
    if (thou->root)
        return thou->root; // if tree root exists we can not create it one more time

    thou->root = (TreeMember*) calloc (1, sizeof (*thou->root));
    if (!thou->root)
        return thou->root;

    thou->size = 1;
    thou->root->tree = thou;
    thou->root->leftChild = NULL;
    thou->root->rightChild = NULL;
    thou->root->memberValue = value;
    thou->root->parent = NULL;
    ASSERTEDTREE
    return thou->root;
}

TreeMember* AddRightChild (TreeMember* thou, MemberValue value)
{
    assert (thou);
    ASSERTEDTREEMEMBER
    if (thou->rightChild)
        return thou->rightChild;

    thou->rightChild = (TreeMember*) calloc (1, sizeof (*thou));
    if (!thou->rightChild)
        return thou->rightChild;

    thou->tree->size++;
    thou->rightChild->tree = thou->tree;
    thou->rightChild->parent = thou;
    thou->rightChild->rightChild = NULL;
    thou->rightChild->leftChild = NULL;
    thou->rightChild->memberValue = value;
    ASSERTEDTREEMEMBER
    return thou->rightChild;
}

TreeMember* AddLeftChild (TreeMember* thou, MemberValue value)
{
    assert (thou);
    ASSERTEDTREEMEMBER
    if (thou->leftChild)
        return thou->leftChild;

    thou->leftChild = (TreeMember*) calloc (1, sizeof (*thou));
    if (!thou->leftChild)
        return thou->leftChild;

    thou->tree->size++;
    thou->leftChild->tree = thou->tree;
    thou->leftChild->parent = thou;
    thou->leftChild->rightChild = NULL;
    thou->leftChild->leftChild = NULL;
    thou->leftChild->memberValue = value;
    ASSERTEDTREEMEMBER
    return thou->leftChild;
}

TreeError DestructTree (Tree* thou)
{
    assert (thou);
    ASSERTEDTREE
    DestructTreeMember (thou->root);
    free (thou);
    return NO_TREE_ERRORS;
}

TreeError DestructTreeMember (TreeMember* thou)
{
    if (thou)
    {
        DestructTreeMember (thou->leftChild);
        DestructTreeMember (thou->rightChild);
        thou->rightChild = NULL;
        thou->leftChild = NULL;
        thou->parent = NULL;
        free (thou);
    }

    return NO_TREE_ERRORS;
}

TreeError TreeVerify (Tree* tree)
{
    int nVertexes = DFSVerify (tree->root);
    if (nVertexes == LOST_PARENT)
    {
        return LOST_PARENT;
    }


    if (tree->size != nVertexes)
    {
        return BAD_TREE_SIZE;
    }
    return NO_TREE_ERRORS;
}

int DFSVerify (TreeMember* vertex)
{
    int nRightVertexes = 0;
    int nLeftVertexes  = 0;

    if (vertex->rightChild)
    {
        if (vertex != vertex->rightChild->parent || vertex->tree != vertex->rightChild->tree)
            return LOST_PARENT;

        nRightVertexes = DFSVerify (vertex->rightChild);
    }
    if (vertex->leftChild)
    {
        if (vertex != vertex->leftChild->parent || vertex->tree != vertex->leftChild->tree)
            return LOST_PARENT;

        nLeftVertexes = DFSVerify (vertex->leftChild);
    }
    if (nRightVertexes == LOST_PARENT || nLeftVertexes == LOST_PARENT)
        return LOST_PARENT;

   // printf ("%d\n", nLeftVertexes + nRightVertexes + 1);
    return nLeftVertexes + nRightVertexes + 1;
}

TreeError DeleteVertex (TreeMember* thou)
{
    assert (thou);
    ASSERTEDTREEMEMBER

    if (thou->rightChild != NULL || thou->leftChild != NULL)
        return NOT_A_LEAF;

    thou->tree->size--;
    if (thou->parent->rightChild == thou)
        thou->parent->rightChild = NULL;
    else
        thou->parent->leftChild = NULL;

    return DestructTreeMember (thou);
}

TreeError GraphicDump (Tree* thou)
{
    if (!thou)
        printf ("there is no TREE. Executed on line");

    FILE* graph_logs = nullptr;
    if(!(graph_logs = fopen (thou->graph_logs_name, "w")))
        return LOGS_ENABLE;

    fprintf (graph_logs, "digraph LIST{\n");
    fprintf (graph_logs, "\t" "rankdir = TB;\n");

    DeclareVertex (graph_logs, thou->root);
    DrawVertex (graph_logs, thou->root);

    fprintf (graph_logs, "}\n");

    fclose (graph_logs);
    char command[100];
    sprintf (command, "dot -Tpng %s -o myGraph.png", thou->graph_logs_name);
    system (command);
    system ("start myGraph.png");

    return NO_TREE_ERRORS;
}

void DrawVertex (FILE* graph_logs, TreeMember* current)
{
    if (!current)
        return;

    if (current->leftChild)
    {
        fprintf (graph_logs, "\"%s\" -> \"%s\"[label=\"left\"];\n", current->memberValue, current->leftChild->memberValue);
        DrawVertex (graph_logs, current->leftChild);
    }
    if (current->rightChild)
    {
        fprintf (graph_logs, "\"%s\" -> \"%s\"[label=\"right\"];\n", current->memberValue, current->rightChild->memberValue);
        DrawVertex (graph_logs, current->rightChild);
    }
}

void DeclareVertex (FILE* graph_logs, TreeMember* current)
{
    if (!current)
        return;

    fprintf (graph_logs, "\"%s\"[shape=record, color=", current->memberValue);
    if (current->leftChild || current->rightChild)
        fprintf (graph_logs, "\"blue4\",style=\"filled\",fillcolor=\"aquamarine\"");
    else
        fprintf (graph_logs, "\"green4\",style=\"filled\",fillcolor=\"green1\"");

    fprintf (graph_logs, ",label=\"  value = %lg | { parent = %p | current =  %p | left = %p | right = %p | value = %s | tree = 5p} \"];",
             current->parent, current, current->leftChild, current->rightChild, current->memberValue, current->tree);

    DeclareVertex (graph_logs, current->rightChild);
    DeclareVertex (graph_logs, current->leftChild);
}
#undef ASSERTEDTREE
#undef ASSERTEDTREEMEMBER
