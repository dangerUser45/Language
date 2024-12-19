#define TX_COMPILED
#include "..\SuperLibs\TXLib.h"
#include "..\SuperLibs\COLOR.h"

#include "Common_Language.h"
#include "Parser_Language.h"
#include "Init_Language.h"

/*static node* GetAssignment (node* Token_array);
static node* GetExpression (node* Token_array);
static node* GetTerm       (node* Token_array);
static node* GetPow        (node* Token_array);                        
static node* GetPrime      (node* Token_array);
static node* GetMathFunc   (node* Token_array);
static node* GetVar        (node* Token_array);
static node* GetBracketEx  (node* Token_array);*/
static node* GetNumber     (node* Token_array);
node* Create_node (type_t type, double data, node* node_left, node* node_right);
//static void  SyntaxError   (node* Token_array);
void SyntaxError ();

extern FILE* Log_File;
size_t pointer = 0;

//==================================================================================================
node* GetGrammatic (Language* Lang_data)
{
    node* Token_array = Lang_data -> Token_array;
    node* Node = 0;
    // /size_t pointer = 0;

    if (Token_array[pointer].value.id != BEGINING) SyntaxError ();
        //DBG(printf ("\n%sGetGrammatic()%s: BEGINING = %zu, " GREEN "pointer = %zu" RESET "\n", RED, RESET, Token_array[pointer].value.id, pointer);)
    pointer++;
        //DBG(printf("I high pointer," GREEN " pointer = %zu\n" RESET "", pointer);)
    //node* node_val = GetAssignment (Token_array);
    // node* Node = GetExpression (Token_array);
    if (Token_array[pointer].type == NUM) {
        //DBG(printf("Token_array[pointer].value = %lg\n", Token_array[pointer].value.val_num);)
        //DBG(printf("Token_array + pointer = %p\n", Token_array + pointer);)
    Node = GetNumber(Token_array); printf ("Im here\n");   }
    $(Node);
        //DBG(printf ("I get num node, his addr = %p\n", Node);)
        //DBG(printf ("GetGrammatic(): Num = %lg\n", Token_array[pointer].value.val_num);)
        //DBG(printf ("pointer after num = %zu\n", pointer);)
    pointer++;  

    if (Token_array[pointer].value.id != ENDING)   SyntaxError ();
    else
        pointer++;
        
    return Node;
}
/*//==================================================================================================
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
node* GetExpression (node* Token_array)
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
node* GetTerm (node* Token_array)
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
node* GetPow  (node* Token_array)
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

node* GetPrime (node* Token_array)
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
node* GetBracketEx (node* Token_array)
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
node* GetVar (node* Token_array)
{
    pointer++;
    node* node_var = _X;
    
    return node_var;
}
//==================================================================================================
node* GetMathFunc (node* Token_array)
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
//==================================================================================================*/
node* GetNumber(node* Token_array)
{
    DBG(printf ("I in GetNumber(): param = %p\n", Token_array + pointer);)
    DBG(printf ("I in GetNumber(): %p\n", Token_array + pointer);)
    return Token_array + pointer;
}
//==================================================================================================
void SyntaxError ()
{
    printf ("Syntax Error\n");
    exit (1);
}
//==================================================================================================
node* Create_node (type_t type, double data, node* node_left, node* node_right)
{
    node* new_node = (node *) calloc (1, sizeof (node));
    if (!new_node) {fprintf (Log_File, "Error!"); assert (new_node);}

    new_node -> type  = type;
    new_node -> value.val_num = data;
    new_node -> left  = node_left;
    new_node -> right = node_right;

    return new_node;
}
//==================================================================================================
