#include "..\SuperLibs\COLOR.h"

#include "Common_Language.h"
#include "Parser_Language.h"
#include "Init_Language.h"
#include "SyntaxError.h"

static node* GetExpression           (Context_parser* context);
static node* GetAssignment           (Context_parser* context);
static node* GetTerm                 (Context_parser* context);
static node* GetPow                  (Context_parser* context);                        
static node* GetPrimaryExpression    (Context_parser* context);
//static node* GetMathFunc           (Context_parser* context);
static node* GetBracketEx            (Context_parser* context);
static node* GetNumber               (Context_parser* context);
static node* GetID                   (Context_parser* context);
static node* GetCompare              (Context_parser* context);
static node* GetDeclaration          (Context_parser* context);
static node* GetCallFunc             (Context_parser* context);
static bool Check_Comparison_marks   (Context_parser* context);
static node* Create_filler_node      (const char* text);

extern FILE* Log_File;

//==================================================================================================
node* GetGrammatic (Language* Lang_data)
{
    node* Token_array = Lang_data -> Token_array;
    Context_parser context = {};
    context.Token_array = Token_array;

    if (Token_array[context.pointer].value.id != BEGINING) SYNTAX_ERROR
    context.pointer++;

    size_t declar_pointer = context.pointer;
    node* Node_declare     = GetDeclaration(&context);

    node* Node_call_func   = GetCallFunc (&context);

    if (Token_array[context.pointer].value.id != ENDING)   SYNTAX_ERROR
    else
        context.pointer++;

    Token_array[declar_pointer + 1].left = Node_call_func;
     return Token_array + declar_pointer;
}
//==================================================================================================
node* GetDeclaration (Context_parser* context)
{
    node* Token_array = context -> Token_array;

    if  (Token_array[context -> pointer].type != OP   ||
        (Token_array[context -> pointer].value.val_op != DECLARATION_ID &&
         Token_array[context -> pointer].value.val_op != DECLARATION_FUNCTION))  
    {
        context -> pointer++;
        return 0;
    }

    node* Node_op_declar =  Token_array + context -> pointer;
    context -> pointer++;
    
    node* Node_id = GetID (context);

    if (Token_array[context -> pointer].type != OP ||  Token_array[context -> pointer].value.val_op != SEPARATOR)
        SYNTAX_ERROR

    Token_array[context -> pointer].left = Node_op_declar;
    Node_op_declar -> left = Node_id;

    context -> pointer++; 

    return Node_id;
}
//==================================================================================================
node* GetCallFunc (Context_parser* context)
{
    node* Token_array = context -> Token_array;
    size_t name_func_pointer = context -> pointer;

    node* Node_name_func   = GetID (context);
    node* Node = Token_array + name_func_pointer;

    while (Token_array[context -> pointer].type != OP && Token_array[context -> pointer].value.val_op != END_PARAM_FUNC)    
    {
        if (Token_array[context -> pointer].type != ID)
            SYNTAX_ERROR
        else 
        {
            node* Node_name_param = GetID(context);
            node* Node_param      = Create_filler_node ("param");
            Node -> right         = Node_param;
            Node_param ->left     = Node_name_param;
            Node                  = Node_param;
        }
    }

    Token_array[context -> pointer].left    = Node_name_func;   
    context -> pointer++;

    return Token_array + context -> pointer - 1;
}
//==================================================================================================
node* GetCompare (Context_parser* context)
{
    node* Token_array = context -> Token_array;
    node* node_val1 = 0;
    node* node_val2 = 0;

    node_val1 = GetID (context);
    if (!node_val1) node_val1 = GetExpression (context);

        //PRINT_TOKEN_POINTER;

    if (!Check_Comparison_marks (context)) 
        SYNTAX_ERROR

    size_t old_pointer = context -> pointer;
    context -> pointer++;
        
    node_val2 = GetID (context);
        DBG(printf("node_val2 = %p\n", node_val2);)
    if (!node_val2) node_val2 = GetExpression (context);


    Token_array[old_pointer].left  = node_val1;
    Token_array[old_pointer].right = node_val2;

      return  Token_array + old_pointer;
}
//==================================================================================================
node* GetAssignment (Context_parser* context)
{
    node* Token_array = context -> Token_array;

    if (Token_array[context -> pointer].type != OP || Token_array[context -> pointer].value.val_op != PRE_EQUAL)
        SYNTAX_ERROR;
    context -> pointer++;
    
    node* node_val1 = GetExpression (context);

    if (Token_array[context -> pointer].type != OP || Token_array[context -> pointer].value.val_op != IN_EQUAL)
        SYNTAX_ERROR;

    size_t old_pointer = context -> pointer;
    context -> pointer++;
       
    node* node_val2 = GetID (context);

    if (Token_array[context -> pointer].type != OP || Token_array[context -> pointer].value.val_op != SEPARATOR)
        SYNTAX_ERROR;

    Token_array[context -> pointer].left  = Token_array + old_pointer;

    Token_array[old_pointer].left = node_val1;
    Token_array[old_pointer].right = node_val2;

    context -> pointer++;

    return Token_array + context -> pointer - 1;
}
//==================================================================================================
node* GetExpression (Context_parser* context)
{
    assert (context);
    node* Token_array = context -> Token_array;
    assert (Token_array);

        DBG(printf("Im in Get_EXpr (): Token_array = %p, context = %p, Token_array + pointer = %p\n", Token_array, context, Token_array + context -> pointer);)

    node* node_val1 = GetTerm (context);    
        DBG(printf("line =  %d Im in Get_EXpr (): node_val1 = %p\n", __LINE__, node_val1);)   
        DBG(printf("line =  %d Im in Get_EXpr (): Token_array = %p\n, context = %p, Token_array + pointer = %p\n", __LINE__, Token_array, context, Token_array + context -> pointer);)
    while (Token_array[context -> pointer].type == OP && (Token_array[context -> pointer].value.val_op  == ADDITTION || Token_array[context -> pointer].value.val_op == SUBTRACTION))
    {
          size_t operation_pointer = context -> pointer;
            DBG(printf ("\nToken_array + pointer = %p", Token_array + context -> pointer );)
        context -> pointer++;
        node* node_val2 = GetTerm (context);
    
        Token_array[operation_pointer].left  = node_val1;
        Token_array[operation_pointer].right = node_val2;
        node_val1 = Token_array + operation_pointer;
    }

    return node_val1;
}
//pragma gcc_diagnostic_ignores
//==================================================================================================
node* GetTerm (Context_parser* context)
{
    node* Token_array = context -> Token_array;
    DBG(printf("Im in Get_Term (): Token_array = %p, context = %p, Token_array + pointer = %p\n", Token_array, context, Token_array + context -> pointer);)

    node* node_val1 = GetPow (context);

    while (Token_array[context -> pointer].type == OP && (Token_array[context -> pointer].value.val_op == MULTIPLICATION || Token_array[context -> pointer].value.val_op == DIVISION))
    {
        size_t operation_pointer =  context -> pointer;
        context -> pointer++;
        node* node_val2 = GetPow (context);

        Token_array[operation_pointer].left  = node_val1;
        Token_array[operation_pointer].right = node_val2;
        node_val1 = Token_array + operation_pointer;
    }

      return node_val1;
}
//==================================================================================================
node* GetPow  (Context_parser* context)
{
    node* Token_array = context ->Token_array;
    DBG(printf("Im in Get_Pow (): Token_array = %p, context = %p, Token_array + pointer = %p\n", Token_array, context, Token_array + context -> pointer);)

    node* node_val1 = GetPrimaryExpression (context);
     

    while (Token_array[context -> pointer].type == OP && Token_array[context -> pointer].value.val_op == ELEVATION)
    {
        int op = Token_array[context -> pointer].value.val_op;

        size_t operation_pointer = context -> pointer;
        context -> pointer++;

        node* node_val2 = GetPrimaryExpression (context);

        if (op == ELEVATION)
        {
            Token_array[operation_pointer].left  = node_val1;
            Token_array[operation_pointer].right = node_val2;
            node_val1 = Token_array + operation_pointer;
        }
    }

      return node_val1;
}
//==================================================================================================
node* GetPrimaryExpression (Context_parser* context)
{
    DBG(printf("Im in Get_Primary_Expr(): contrext -> Token_array = %p, context = %p, Token_array + pointer = %p\n", context -> Token_array, context, context -> Token_array + context -> pointer);)
      node* node_val = GetBracketEx (context);
    if (node_val) {
          return node_val;}

    else node_val = GetID (context);
        if (node_val)
            return node_val;
    else {
          return GetNumber(context);}
}
//==================================================================================================
node* GetBracketEx (Context_parser* context)
{
    node* Token_array = context -> Token_array;
    DBG(printf("Im in Get_EXpr (): Token_array = %p, context = %p\n", Token_array, context);)

    DBG(printf("Im in Get_Bracket_Exp(): contrext -> Token_array = %p, context = %p, Token_array + pointer = %p\n", Token_array, context, Token_array + context -> pointer);)
    //DBG (printf ("Token_array[context -> pointer].value.val_op = %d", Token_array[context -> pointer].value.val_op);)

    if (Token_array[context -> pointer].type == OP && Token_array[context -> pointer].value.val_op == OPENING_BRACKET)
    {
        context -> pointer++;
        node* node_val = GetExpression (context);

        if (Token_array[context -> pointer].value.val_op != CLOSING_BRACKET)
            SYNTAX_ERROR;

        context -> pointer++;
        return node_val;
    }
    else {  return 0;}
}
//==================================================================================================
node* GetID (Context_parser* context)
{
    if (context -> Token_array[context -> pointer].type == ID) {
        context -> pointer++;
        return context -> Token_array + context -> pointer - 1;}

    else return 0;
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
    context -> pointer++;
    return (context -> Token_array) + (context -> pointer - 1);
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
}*/
//==================================================================================================

bool Check_Comparison_marks (Context_parser* context)
{
    if (context -> Token_array[context -> pointer].type != OP                                           ||
       ((const int) EQUAL_COMPARE > (const int) context -> Token_array[context -> pointer].value.val_op ||
        (const int) MORE_OR_EQUAL < (const int) context -> Token_array[context -> pointer].value.val_op  ))
        return false;
    
    return true;
}

//==================================================================================================
node* Create_filler_node (const char* text)
{
    node* Node = (node*) calloc (1, sizeof (node));

    Node -> type = FILLER;
    Node -> value.filler = text;

    printf ("%sAddr: param = %p%s", YELLOW, Node, RESET);   

    return Node;
}
//==================================================================================================
