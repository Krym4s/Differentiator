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

TreeError NumericTreeConstruct (NumericTree* thou, char* graph_logs, char* LaTeX_Output_Name, FILE* latexOutput)
{
    assert (thou);
    thou->size = 0;
    thou->root = NULL;
    thou->graph_logs_name = graph_logs;
    for (int i = 0 ; i < 26; i++)
        thou->variables[i] = 0;

    thou->LaTeX_Output = latexOutput;

    return NO_TREE_ERRORS;
}

NumericNode* NumericNodeConstruct (NumericNode* thou, NumericNode* left, NumericNode* right, MemberValue value, NodeType type, NumericTree* tree)
{
    thou->tree = tree;
    thou->type = type;
    thou->leftChild = left;
    thou->rightChild = right;
    thou->memberValue = value;
    return thou;
}

NumericNode* CreateNewNode (NumericNode* left, NumericNode* right, MemberValue value, NodeType type, NumericTree* tree)
{
    NumericNode* thou = (NumericNode*) calloc (1, sizeof(*thou));
    if (!thou)
        return nullptr;

    thou->tree = tree;
    thou->type = type;
    thou->leftChild = left;
    thou->rightChild = right;
    thou->memberValue = value;
    thou->tree->size++;
    return thou;
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
    if (thou)
        free (thou);
    return NO_TREE_ERRORS;
}

TreeError NumericDestructTreeMember (NumericNode* thou)
{
    if (thou)
    {
        NumericDestructTreeMember (thou->rightChild);
        NumericDestructTreeMember (thou->leftChild);
        thou->rightChild = nullptr;
        thou->leftChild  = nullptr;
        thou->tree       = nullptr;
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
        if (vertex->tree != vertex->rightChild->tree)
            return LOST_PARENT;

        if (vertex->type != OPERATION)
            return WRONG_TYPE;

        nRightVertexes = NumericDFSVerify (vertex->rightChild);
    }
    if (vertex->leftChild)
    {
        if (vertex->tree != vertex->leftChild->tree)
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

    fprintf (graph_logs, ",label=\"   { current =  %p | left = %p | right = %p | tree = %p | type = %s | value = ",
             current, current->leftChild, current->rightChild, current->tree, typeName (current->type));
    switch (current->type)
    {
        case VARIABLE: fprintf (graph_logs, "%c", (char) current->memberValue + 'a'); break;
        case CONSTANT: fprintf (graph_logs, "%lg", current->memberValue); break;
        case OPERATION: fprintf (graph_logs, "%c", (char) current->memberValue); break;
    }
    fprintf (graph_logs, "} \"];");


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

NumericNode* FindParent (NumericNode* current)
{
    return DFSSearch (current, current->tree->root);
}

NumericNode* DFSSearch (NumericNode* sought, NumericNode* current)
{
    if (!current)
        return nullptr;

    if (current->leftChild == sought || current->rightChild == sought)
        return current;

    NumericNode* tempPointer = DFSSearch (sought, current->rightChild);
    if (!tempPointer)
        return DFSSearch (sought, current->leftChild);

    return tempPointer;
}
#undef ARGNAME
#undef ASSERTEDTREE
#undef ASSERTEDTREEMEMBER
