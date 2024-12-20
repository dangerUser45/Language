#define TX_COMPILED
#include "..\SuperLibs\TXLib.h"

#include "..\SuperLibs\COLOR.h"

#include "Common_Language.h"
#include "Parser_Language.h"
#include "Init_Language.h"
#include "SyntaxError.h"

static node* GetExpression          (Context_parser* context);
//static node* GetAssignment        (Context_parser* context);
static node* GetTerm                (Context_parser* context);
static node* GetPow                 (Context_parser* context);                        
static node* GetPrimaryExpression   (Context_parser* context);
//static node* GetMathFunc          (Context_parser* context);
static node* GetBracketEx           (Context_parser* context);
static node* GetNumber              (Context_parser* context);
node* Create_node (type_t type, double data, node* node_left, node* node_right);

extern FILE* Log_File;

//==================================================================================================
node* GetGrammatic (Language* Lang_data)
{
    node* Token_array = Lang_data -> Token_array;
    Context_parser context = {};
    
    $$ if (Token_array[context.pointer].value.id != BEGINING) SYNTAX_ERROR
        //DBG(printf ("\n%sGetGrammatic()%s: BEGINING = %zu, " GREEN "pointer = %zu" RESET "\n", RED, RESET, Token_array[pointer].value.id, pointer);)
    context.pointer++;
        
    $$ node* Node = GetExpression(&context);   

    if (Token_array[context.pointer].value.id != ENDING)   SYNTAX_ERROR
    else
        context.pointer++;

    $$ return Node;
}
/*//==================================================================================================
node* GetAssignment (node* Token_array, size_t* pointer)
{
    node* Node = 0;
    if (Token_array[*pointer].value.val_op == PRE_EQUAL)
        Node = 
    else SYNTAX_ERROR;

    else 
    {
        (*pointer)++;
        node* Node = GetID (Token_array);
        if (Node) 

        else if (Node = GetNumber)
    }
}
//==================================================================================================*/
node* GetExpression (Context_parser* context)
{
    node* Token_array = context -> Token_array;

    $$ node* node_val1 = GetTerm (context);

    while (Token_array[context -> pointer].value.val_op  == ADD || Token_array[context -> pointer].value.val_op == SUB)
    {
        context -> pointer++;
        node* node_val2 = GetTerm (context);
    
        Token_array[context -> pointer].left  = node_val1;
        Token_array[context -> pointer].right = node_val2;
        node_val1 = Token_array + context -> pointer;
    }

    $$ return node_val1;
}
//==================================================================================================
node* GetTerm (Context_parser* context)
{
    node* Token_array = context -> Token_array;
    $$ node* node_val1 = GetPow (context);

    while (Token_array[context -> pointer].value.val_op == MULTIPLICATION || Token_array[context -> pointer].value.val_op == DIVISION)
    {
        context -> pointer++;
        node* node_val2 = GetPow (context);

        Token_array[context -> pointer].left  = node_val1;
        Token_array[context -> pointer].right = node_val2;
        node_val1 = Token_array + context -> pointer;
    }

    $$ return node_val1;
}
//==================================================================================================
node* GetPow  (Context_parser* context)
{
    node* Token_array = context ->Token_array;

    $$ node* node_val1 = GetPrimaryExpression (context);

    while (Token_array[context -> pointer].value.val_op == ELEVATION)
    {
        int op = Token_array[context -> pointer].value.val_op;
        context -> pointer++;
        node* node_val2 = GetPrimaryExpression (context);

        if (op == ELEVATION)
        {
            Token_array[context -> pointer].left  = node_val1;
            Token_array[context -> pointer].right = node_val2;
            node_val1 = Token_array + context -> pointer;
        }
    }

    $$ return node_val1;
}
//==================================================================================================

node* GetPrimaryExpression (Context_parser* context)
{
    $$ node* node_val = GetBracketEx (context);
    if (node_val) {
        $$ return node_val;}

    /*else node_val = GetMathFunc (Context_parser* context);
        if (node_val)
            return node_val;*/      
    else {
        $$ return GetNumber(context);}
}
//==================================================================================================
node* GetBracketEx (Context_parser* context)
{
    $$ node* Token_array = context -> Token_array;
        DBG (printf("Im in GetBrackEx (): context = %p\n context -> Token_array = %p, context -> Token_array [pointer] = %p", context, context -> Token_array, context -> Token_array [context -> pointer]);)
    $$  DBG (printf ("Token_array[context -> pointer].value.val_op = %lg", Token_array[context -> pointer].value.val_op);)


   $$ if (Token_array[context -> pointer].value.val_op == OPENING_BRACKET)
    {
        $$ context -> pointer++;
        $$ node* node_val = GetExpression (context);

        $$ if (Token_array[context -> pointer].value.val_op != CLOSING_BRACKET)
            SYNTAX_ERROR;

        context -> pointer++;
        return node_val;
    }
    else {$$ return 0;}
}
/*//==================================================================================================
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
        if (!node1) SYNTAX_ERROR;

    node* node2 = GetBracketEx ();
        if (!node2) SYNTAX_ERROR;

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
node* GetNumber (Context_parser* context)
{
    $$ context -> pointer++;
    $$ return (context -> Token_array) + (context -> pointer - 1);
}
/*//==================================================================================================
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
//==================================================================================================*/
