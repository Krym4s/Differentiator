//
// Created by egolg on 23.11.2020.
//

#include <windef.h>
#include "Numeric_Tree.h"

#define TREERELEASE

#ifdef TREERELEASE
#define ASSERTEDTREE
#define ASSERTEDTREEMEMBER
#else
#define ASSERTEDTREE !NumericTreeVerify (thou) || NumericGraphicDump (thou);
#define ASSERTEDTREEMEMBER !NumericTreeVerify (thou->tree) || NumericGraphicDump (thou->tree);
#endif

TreeError NumericTreeConstruct (NumericTree* thou, char* graph_logs)
{
    assert (thou);
    thou->size = 0;
    thou->root = NULL;
    thou->graph_logs_name = graph_logs;
    for (int i = 0 ; i < 26; i++)
        thou->variables[i] = 0;

    return NO_TREE_ERRORS;
}

void NumericNodeConstruct (NumericNode* thou, NumericNode* right, NumericNode* left, NumericNode* parent, MemberValue value, NodeType type, NumericTree* tree)
{
    thou->tree = tree;
    thou->type = type;
    thou->leftChild = left;
    thou->rightChild = right;
    thou->parent = parent;
    thou->memberValue = value;
}

NumericNode* NumericAddRightChild (NumericNode* thou, MemberValue value, NodeType type)
{
    assert (thou);
    ASSERTEDTREEMEMBER
    if (thou->rightChild)
        return thou->rightChild;

    thou->rightChild = (NumericNode*) calloc (1, sizeof (*thou));
    if (!thou->rightChild)
        return thou->rightChild;

    thou->tree->size++;
    thou->rightChild->tree = thou->tree;
    thou->rightChild->parent = thou;
    thou->rightChild->rightChild = NULL;
    thou->rightChild->leftChild = NULL;
    thou->rightChild->memberValue = value;
    thou->rightChild->type = type;
    ASSERTEDTREEMEMBER
    return thou->rightChild;
}

NumericNode* NumericAddLeftChild (NumericNode* thou, MemberValue value, NodeType type)
{
    assert (thou);
    ASSERTEDTREEMEMBER
    if (thou->leftChild)
        return thou->leftChild;

    thou->leftChild = (NumericNode*) calloc (1, sizeof (*thou));
    if (!thou->leftChild)
        return thou->leftChild;

    thou->tree->size++;
    thou->leftChild->tree = thou->tree;
    thou->leftChild->parent = thou;
    thou->leftChild->rightChild = NULL;
    thou->leftChild->leftChild = NULL;
    thou->leftChild->memberValue = value;
    thou->leftChild->type = type;
    ASSERTEDTREEMEMBER
    return thou->leftChild;
}

TreeError NumericDestructTree (NumericTree* thou)
{
    assert (thou);
    ASSERTEDTREE
    NumericDestructTreeMember (thou->root);
    free (thou);
    return NO_TREE_ERRORS;
}

TreeError NumericDestructTreeMember (NumericNode* thou)
{
    if (thou)
    {
        NumericDestructTreeMember (thou->leftChild);
        NumericDestructTreeMember (thou->rightChild);
        thou->rightChild = NULL;
        thou->leftChild  = NULL;
        thou->parent     = NULL;
        thou->tree       = NULL;
        thou->type = IMAGINATIVE;
        free (thou);
    }

    return NO_TREE_ERRORS;
}

TreeError NumericTreeVerify (NumericTree* tree)
{
    int nVertexes = NumericDFSVerify (tree->root);
    if (nVertexes == LOST_PARENT)
        return LOST_PARENT;

    if (tree->size != nVertexes)
        return BAD_TREE_SIZE;

    if (nVertexes == WRONG_TYPE)
        return WRONG_TYPE;

    return NO_TREE_ERRORS;
}

int NumericDFSVerify (NumericNode* vertex)
{
    int nRightVertexes = 0;
    int nLeftVertexes  = 0;

    if (vertex->rightChild)
    {
        if (vertex != vertex->rightChild->parent || vertex->tree != vertex->rightChild->tree)
            return LOST_PARENT;

        if (vertex->type != OPERATION)
            return WRONG_TYPE;

        nRightVertexes = NumericDFSVerify (vertex->rightChild);
    }
    if (vertex->leftChild)
    {
        if (vertex != vertex->leftChild->parent || vertex->tree != vertex->leftChild->tree)
            return LOST_PARENT;

        nLeftVertexes = NumericDFSVerify (vertex->leftChild);

        if (vertex->type != OPERATION)
            return WRONG_TYPE;

    }
    if (nRightVertexes == LOST_PARENT || nLeftVertexes == LOST_PARENT)
        return LOST_PARENT;

    if (nRightVertexes == WRONG_TYPE || nLeftVertexes == WRONG_TYPE)
        return WRONG_TYPE;

    return nLeftVertexes + nRightVertexes + 1;
}

TreeError NumericDeleteVertex (TreeMember* thou)
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

TreeError NumericGraphicDump (NumericTree* thou)
{
    if (!thou)
        printf ("there is no TREE. Executed on line");

    FILE* graph_logs = nullptr;
    if(!(graph_logs = fopen (thou->graph_logs_name, "w")))
        return LOGS_ENABLE;

    fprintf (graph_logs, "digraph LIST{\n");
    fprintf (graph_logs, "\t" "rankdir = TB;\n");

    NumericDeclareVertex (graph_logs, thou->root);
    NumericDrawVertex (graph_logs, thou->root);

    fprintf (graph_logs, "}\n");

    fclose (graph_logs);
    char command[100] = "";
    sprintf (command, "dot -Tpng %s -o myGraph.png", thou->graph_logs_name);
    system (command);
    system ("start myGraph.png");

    return NO_TREE_ERRORS;
}

void NumericDrawVertex (FILE* graph_logs, NumericNode* current)
{
    if (!current)
        return;

    if (current->leftChild)
    {
        fprintf (graph_logs, "\"%p\" -> \"%p\"[label=\"left\"];\n", current, current->leftChild);
        NumericDrawVertex (graph_logs, current->leftChild);
    }
    if (current->rightChild)
    {
        fprintf (graph_logs, "\"%p\" -> \"%p\"[label=\"right\"];\n", current, current->rightChild);
        NumericDrawVertex (graph_logs, current->rightChild);
    }
}

void NumericDeclareVertex (FILE* graph_logs, NumericNode* current)
{
    if (!current)
        return;

    fprintf (graph_logs, "\"%p\"[shape=record, color=", current);
    if (current->leftChild || current->rightChild)
        fprintf (graph_logs, "\"blue4\",style=\"filled\",fillcolor=\"aquamarine\"");
    else
        fprintf (graph_logs, "\"green4\",style=\"filled\",fillcolor=\"green1\"");

    fprintf (graph_logs, ",label=\"   { parent = %p | current =  %p | left = %p | right = %p | value = %c | tree = %p | type = %s} \"];",
             current->parent, current, current->leftChild, current->rightChild, (int)current->memberValue, current->tree, typeName (current->type));

    NumericDeclareVertex (graph_logs, current->rightChild);
    NumericDeclareVertex (graph_logs, current->leftChild);
}

#define ARGNAME(x) #x
char* typeName (NodeType type)
{
    switch (type)
    {
        case OPERATION: return ARGNAME(OPERATION);
        case CONSTANT:  return ARGNAME(CONSTANT);
        case VARIABLE:  return ARGNAME(VARIABLE);
        default:
            return "NO_TYPE";
    }
}
#undef ARGNAME
#undef ASSERTEDTREE
#undef ASSERTEDTREEMEMBER
