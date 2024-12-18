#include "Common_Language.h"
#include "Parser_Language.h"

static node* GetAssignment (node* Token_array);
static node* GetExpression ();
static node* GetTerm       ();
static node* GetPow        ();
static node* GetPrime      ();
static node* GetMathFunc   ();
static node* GetVar        ();
static node* GetBracketEx  ();
static node* GetNumber     ();
static void SyntaxError    ();

int pointer = 0; 

//==================================================================================================
node* GetGrammatic (Language* Lang_data)
{
    node* Token_array = Lang_data -> Token_array;
    //size_t pointer = 0;

    if (Token_array[pointer].value.id != BEGINING) SyntaxError ();

    node* node_val = GetAssignment (Token_array);

    if (Token_array[pointer].value.id != ENDING)   SyntaxError ();
    else  
        pointer++;
    
    return node_val;
}
//==================================================================================================
node* GetAssignment (node* Token_array, size_t* pointer)
{
    node* Node = 0;
    if (Token_array[*pointer].value.val_op == PRE_EQUAL)
        Node = 
    else SyntaxError ();

    else 
    {
        (*pointer)++;
        node* Node = GetID (Token_array);
        if (Node) 

        else if (Node = GetNumber)
    }
}
//==================================================================================================
node* GetExpression ()
{
    node* node_val1 = GetTerm ();

    while (string[pointer] == '+' || string[pointer] == '-')
    {
        char op = string[pointer];
        pointer++;
        node* node_val2 = GetTerm ();

    if (op == '+')
         node_val1 = _ADD (node_val1, node_val2);
    else node_val1 = _SUB (node_val1, node_val2);
    }

    return node_val1;
}
//==================================================================================================
node* GetTerm ()
{
    node* node_val1 = GetPow ();

    while (string[pointer] == '*' || string[pointer] == '/')
    {
        char op = string[pointer];
    pointer++;
    node* node_val2 = GetPow ();

    if (op == '*')
         node_val1 = _MUL (node_val1, node_val2);
    else node_val1 = _DIV (node_val1, node_val2);
    }

    return node_val1;
}
//==================================================================================================
node* GetPow  ()
{
    node* node_val1 = GetPrime ();

    while (string[pointer] == '^')
    {
        char op = string[pointer];
    pointer++;
    node* node_val2 = GetPrime ();

    if (op == '^')
        node_val1 = _POW (node_val1, node_val2);
    }

    return node_val1;
}
//==================================================================================================

node* GetPrime ()
{
    node* node_val = GetBracketEx ();
    if (node_val)
        return node_val;

    else if (string[pointer] == 'x')
        return GetVar ();

    else node_val = GetMathFunc ();
        if (node_val)
            return node_val;      
        else 
            return GetNumber ();
}
//==================================================================================================
node* GetBracketEx ()
{
   if (string[pointer] == '(')
    {
        pointer++;
        node* node_val = GetExpression ();
        if (string[pointer] != ')')
            SyntaxError ();
        pointer++;
        return node_val;
    }
    else return 0;
}
//==================================================================================================
node* GetVar ()
{
    pointer++;
    node* node_var = _X;
    
    return node_var;
}
//==================================================================================================
node* GetMathFunc ()
{
//-------------- COS ---------------------------
    if (string[pointer] == 'c')  {
    pointer++;
    node* node = GetBracketEx ();
    return _COS(node);           }
//-------------- SIN ---------------------------
    else if (string[pointer] == 's') {
    pointer++;
    node* node = GetBracketEx ();
    return _SIN(node);               }
//-------------- LN ---------------------------
    else if (string[pointer] == 'l') {
    pointer++;
    node* node = GetBracketEx ();
    return _LN(node);                }

//-------------- LOG ---------------------------
    else if (string[pointer] == 'L') {
    pointer++;
    node* node1 = GetBracketEx ();
        if (!node1) SyntaxError ();

    node* node2 = GetBracketEx ();
        if (!node2) SyntaxError ();

    return _LOG(node1, node2);       }
//-------------- EXP ---------------------------
    else if (string[pointer] == 'e') {
    pointer++;
    node* node = GetBracketEx ();
    return _EXP(node);               }
//-------------- TAN ---------------------------
    else if (string[pointer] == 't') {
    pointer++;
    node* node = GetBracketEx ();
    return _TAN(node);               }

    else return 0;
}
//==================================================================================================
node* GetNumber(node* Token_array)
{
    pointer++ 

    return Create_node (Token_array[pointer-1].value., val, 0, 0);
}
//==================================================================================================
void SyntaxError ()
{
    printf ("Syntax Error\n");
    exit (1);
}
//==================================================================================================