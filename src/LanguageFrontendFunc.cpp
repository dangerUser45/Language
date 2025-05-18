#include <ctype.h>
#include <string.h>

#include <Common.h>
#include <Color.h>

#include <LanguageFrontend.h>

static ERRORS GetTokenNum (node* token_array, const char** string, size_t* token_index);
static const char* GetToken_Operator_or_ID (Language* language, const char* string, size_t* token_index, size_t* name_table_index);
static int    Compare_KeyWords (const char* string, size_t length_word);

static void   SyntaxError (SYNTAX_ERRORS error);

static node* GetDeclaration (Context_parser* context);
static node* GetCallFunc (Context_parser* context);
static node* GetCompare (Context_parser* context);
static node* GetAssignment (Context_parser* context);
static node* GetExpression (Context_parser* context);
static node* GetTerm (Context_parser* context);
static node* GetPow  (Context_parser* context);
static node* GetPrimaryExpression (Context_parser* context);
static node* GetBracketEx (Context_parser* context);
static node* GetID (Context_parser* context);
static node* GetNumber (Context_parser* context);
static bool  Check_Comparison_marks (Context_parser* context);
static node* Create_filler_node (const char* text);

//==================================================================
#define SYNTAX_ERROR(error)\
{\
    fprintf (stderr, RED "SYNTAX ERROR: %s:%d: error = %d" RESET, __FILE__, __LINE__, error);\
    exit (1);\
}
//==================================================================
ERRORS Language_LexicalAnalyser (Language* language)
{
    const char* string = language->code_text;
    node* token_array  = language->token_array;
    size_t length_file = language->length_file;
    size_t token_index = 0;
    size_t name_table_index = 0;

    z(token_array, p); //FIXME
    fprintf (stderr, "language->code_text:\n%s", language->code_text); //FIXME

    Skip_delimiters (string);
    zz // FIXME
    while (*string != '\0')
    {
        if (isdigit (*string))
            GetTokenNum (token_array, &string, &token_index);

        else
            string = GetToken_Operator_or_ID (language, string, &token_index, &name_table_index);
    }

    language->token_number = token_index;
    language->name_table_number = name_table_index;

    return NO_ERRORS;
}
//==================================================================
ERRORS GetTokenNum (node* token_array, const char** string, size_t* token_index)
{
    char* end_ptr = 0;

    val_t val = strtod (*string, &end_ptr);
    token_array[*token_index].type = NUM;
    token_array[*token_index].value.val_num  = val;

    *string = Skip_delimiters (end_ptr);

    (*token_index)++;
    return NO_ERRORS;
}
//==================================================================
const char* GetToken_Operator_or_ID (Language* language, const char* string, size_t* token_index, size_t* name_table_index)
{
    zz // FIXME
    node* token_array = language->token_array;
    NAME_TABLE* name_table = language->name_table;

    const char* start = string;
    const char* delimiters = "\n\t\r ";
    while (!strchr (delimiters, *string))
        ++string;

    fprintf (stderr, BLUE "string = '%c'" RESET "\n", *string); //FIXME
    size_t length_word = (size_t)string - (size_t)start;
    z(length_word, zu) //FIXME

    if (length_word > MAX_NAME_ID)
        SYNTAX_ERROR (TOO_LONG_WORD)

    else
    {
        int op = Compare_KeyWords (start, length_word);
        z(op, d)

        if (op != 0)
        {
            token_array[*token_index].type  = OP;
            token_array[*token_index].value.val_op = (operations) op;
        }

        else
        {
            strncpy (name_table[*name_table_index].name_id, start, length_word);
            token_array[*token_index].type = ID;
            token_array[*token_index].value.id = *name_table_index;
            (*name_table_index)++;
        }

        string = Skip_delimiters (string);
        fprintf (stderr, BLUE "after skip delim: string = '%c'" RESET "\n", *string); //FIXME

        (*token_index)++;
    }

    return string;
}
//==================================================================
static int Compare_KeyWords (const char* string, size_t length_word)
{
    #define NOT_OP 0

    zz // FIXME
    static Key_word KeyWords [] = {
        {"Breakfast:", BEGINING, 10},
        {"want-millpops", OPENING_CURLY_BRACKET, 13},
        {"dirty" , CLOSING_CURLY_BRACKET, 5},
        {"(", OPENING_BRACKET, 1},
        {")", CLOSING_BRACKET, 1},
        {"bombardiro-krokodilo", DECLARATION_ID, 20},
        {"yes-yes", EQUALS, 7},
        {"is-the-base", DECLARATION_FUNCTION, 11},
        {"double-yummy", IF, 1},
        {"golubino-shpioniro", WHILE, 18},
        {"sin", SIN, 3},
        {"cos", COS, 3},
        {"DIYA-kontora...", ENDING, 15},
        {"+vibe", ADDITTION, 5},
        {"-vibe", SUBTRACTION, 5},
        {"/", DIVISION, 1},
        {"*", MULTIPLICATION, 1},
        {"^", ELEVATION, 1},
        {"skip", SEPARATOR, 4},
        {"==", EQUAL_COMPARE, 2},
        {"!=", NOT_EQUALE_COMPARE, 2},
        {"<", LESS, 1},
        {"<=", LESS_OR_EQUALE, 2},
        {">", MORE, 1},
        {">=", MORE_OR_EQUAL, 2},
        {"", BEGIN_PARAM_FUNC,1},
        {"", END_PARAM_FUNC, 1},
        {"|skip:", COMMENTS, 6}
    };

    size_t number_op = sizeof (KeyWords) / sizeof (Key_word);

    for (size_t i = 0; i < number_op; i++)
        if  (!strncmp (string, KeyWords[i].name_op, length_word) && length_word == KeyWords[i].length_key_word)   //!!!
            return KeyWords[i].OP;

    return NOT_OP;

    #undef NOT_OP
}

//==================================================================
void SyntaxError (SYNTAX_ERRORS error)
{
    fprintf (stderr, RED "SYNTAX ERROR: %s:%d: error = %d" RESET, __FILE__, __LINE__, error);
    exit (1);
}
//==================================================================
ERRORS Language_SyntaxAnalyser (Language* language)
{


    return NO_ERRORS;
}
//==================================================================
node* GetDeclaration (Context_parser* context)
{
    node* token_array = context -> token_array;

    if  (token_array[context -> pointer].type != OP   ||
        (token_array[context -> pointer].value.val_op != DECLARATION_ID &&
         token_array[context -> pointer].value.val_op != DECLARATION_FUNCTION))
    {
        context -> pointer++;
        return 0;
    }

    node* Node_op_declar =  token_array + context -> pointer;
    context -> pointer++;

    node* Node_id = GetID (context);

    if (token_array[context -> pointer].type != OP ||  token_array[context -> pointer].value.val_op != SEPARATOR)
        SYNTAX_ERROR(DBG_ERROR)

    token_array[context -> pointer].left = Node_op_declar;
    Node_op_declar -> left = Node_id;

    context -> pointer++;

    return Node_id;
}
//==================================================================================================
node* GetCallFunc (Context_parser* context)
{
    node* token_array = context -> token_array;
    size_t name_func_pointer = context -> pointer;

    node* Node_name_func = GetID (context);
    node* Node = token_array + name_func_pointer;

    while (token_array[context -> pointer].type != OP && token_array[context -> pointer].value.val_op != END_PARAM_FUNC)
    {
        if (token_array[context -> pointer].type != ID)
            SYNTAX_ERROR(DBG_ERROR)
        else
        {
            node* Node_name_param = GetID(context);
            node* Node_param      = Create_filler_node ("param");
            Node -> right         = Node_param;
            Node_param ->left     = Node_name_param;
            Node                  = Node_param;
        }
    }

    token_array[context -> pointer].left    = Node_name_func;
    context -> pointer++;

    return token_array + context -> pointer - 1;
}
//==================================================================================================
node* GetCompare (Context_parser* context)
{
    node* token_array = context -> token_array;
    node* node_val1 = 0;
    node* node_val2 = 0;

    node_val1 = GetID (context);
    if (!node_val1) node_val1 = GetExpression (context);

        //PRINT_TOKEN_POINTER;

    if (!Check_Comparison_marks (context))
        SYNTAX_ERROR(DBG_ERROR)

    size_t old_pointer = context -> pointer;
    context -> pointer++;

    node_val2 = GetID (context);
        DBG(printf("node_val2 = %p\n", node_val2);)
    if (!node_val2) node_val2 = GetExpression (context);


    token_array[old_pointer].left  = node_val1;
    token_array[old_pointer].right = node_val2;

      return  token_array + old_pointer;
}
//==================================================================================================
node* GetAssignment (Context_parser* context)
{
    node* token_array = context -> token_array;

    if (token_array[context -> pointer].type != OP || token_array[context -> pointer].value.val_op != PRE_EQUAL)
        SYNTAX_ERROR(DBG_ERROR)
    context -> pointer++;

    node* node_val1 = GetExpression (context);

    if (token_array[context -> pointer].type != OP || token_array[context -> pointer].value.val_op != IN_EQUAL)
        SYNTAX_ERROR(DBG_ERROR);

    size_t old_pointer = context -> pointer;
    context -> pointer++;

    node* node_val2 = GetID (context);

    if (token_array[context -> pointer].type != OP || token_array[context -> pointer].value.val_op != SEPARATOR)
        SYNTAX_ERROR(DBG_ERROR);

    token_array[context -> pointer].left  = token_array + old_pointer;

    token_array[old_pointer].left = node_val1;
    token_array[old_pointer].right = node_val2;

    context -> pointer++;

    return token_array + context -> pointer - 1;
}
//==================================================================================================
node* GetExpression (Context_parser* context)
{
    assert (context);
    node* token_array = context -> token_array;
    assert (token_array);

        DBG(printf("Im in Get_EXpr (): token_array = %p, context = %p, token_array + pointer = %p\n", token_array, context, token_array + context -> pointer);)

    node* node_val1 = GetTerm (context);
        DBG(printf("line =  %d Im in Get_EXpr (): node_val1 = %p\n", __LINE__, node_val1);)
        DBG(printf("line =  %d Im in Get_EXpr (): token_array = %p\n, context = %p, token_array + pointer = %p\n", __LINE__, token_array, context, token_array + context -> pointer);)
    while (token_array[context -> pointer].type == OP && (token_array[context -> pointer].value.val_op  == ADDITTION || token_array[context -> pointer].value.val_op == SUBTRACTION))
    {
          size_t operation_pointer = context -> pointer;
            DBG(printf ("\ntoken_array + pointer = %p", token_array + context -> pointer );)
        context -> pointer++;
        node* node_val2 = GetTerm (context);

        token_array[operation_pointer].left  = node_val1;
        token_array[operation_pointer].right = node_val2;
        node_val1 = token_array + operation_pointer;
    }

    return node_val1;
}
//pragma gcc_diagnostic_ignores
//==================================================================================================
node* GetTerm (Context_parser* context)
{
    node* token_array = context -> token_array;
    DBG(printf("Im in Get_Term (): token_array = %p, context = %p, token_array + pointer = %p\n", token_array, context, token_array + context -> pointer);)

    node* node_val1 = GetPow (context);

    while (token_array[context -> pointer].type == OP && (token_array[context -> pointer].value.val_op == MULTIPLICATION || token_array[context -> pointer].value.val_op == DIVISION))
    {
        size_t operation_pointer =  context -> pointer;
        context -> pointer++;
        node* node_val2 = GetPow (context);

        token_array[operation_pointer].left  = node_val1;
        token_array[operation_pointer].right = node_val2;
        node_val1 = token_array + operation_pointer;
    }

      return node_val1;
}
//==================================================================================================
node* GetPow  (Context_parser* context)
{
    node* token_array = context ->token_array;
    DBG(printf("Im in Get_Pow (): token_array = %p, context = %p, token_array + pointer = %p\n", token_array, context, token_array + context -> pointer);)

    node* node_val1 = GetPrimaryExpression (context);


    while (token_array[context -> pointer].type == OP && token_array[context -> pointer].value.val_op == ELEVATION)
    {
        int op = token_array[context -> pointer].value.val_op;

        size_t operation_pointer = context -> pointer;
        context -> pointer++;

        node* node_val2 = GetPrimaryExpression (context);

        if (op == ELEVATION)
        {
            token_array[operation_pointer].left  = node_val1;
            token_array[operation_pointer].right = node_val2;
            node_val1 = token_array + operation_pointer;
        }
    }

      return node_val1;
}
//==================================================================================================
node* GetPrimaryExpression (Context_parser* context)
{
    DBG(printf("Im in Get_Primary_Expr(): contrext -> token_array = %p, context = %p, token_array + pointer = %p\n", context -> token_array, context, context -> token_array + context -> pointer);)
      node* node_val = GetBracketEx (context);
    if (node_val) {
          return node_val;}

    else node_val = GetID (context);
        if (node_val)
            return node_val;
        else
          return GetNumber(context);
}
//==================================================================================================
node* GetBracketEx (Context_parser* context)
{
    node* token_array = context -> token_array;
    DBG(printf("Im in Get_EXpr (): token_array = %p, context = %p\n", token_array, context);)

    DBG(printf("Im in Get_Bracket_Exp(): contrext -> token_array = %p, context = %p, token_array + pointer = %p\n", token_array, context, token_array + context -> pointer);)
    //DBG (printf ("token_array[context -> pointer].value.val_op = %d", token_array[context -> pointer].value.val_op);)

    if (token_array[context -> pointer].type == OP && token_array[context -> pointer].value.val_op == OPENING_BRACKET)
    {
        context -> pointer++;
        node* node_val = GetExpression (context);

        if (token_array[context -> pointer].value.val_op != CLOSING_BRACKET)
            SYNTAX_ERROR(DBG_ERROR);

        context -> pointer++;
        return node_val;
    }
    else {  return 0;}
}
//==================================================================================================
node* GetID (Context_parser* context)
{
    if (context -> token_array[context -> pointer].type == ID) {
        context -> pointer++;
        return context -> token_array + context -> pointer - 1;}

    else return 0;
}
/*//==================================================================================================
node* GetMathFunc (node* token_array)
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
        if (!node1) SYNTAX_ERROR(DBG_ERROR);

    node* node2 = GetBracketEx ();
        if (!node2) SYNTAX_ERROR(DBG_ERROR);

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
    return (context -> token_array) + (context -> pointer - 1);
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
    if (context -> token_array[context -> pointer].type != OP                                           ||
       ((const int) EQUAL_COMPARE > (const int) context -> token_array[context -> pointer].value.val_op ||
        (const int) MORE_OR_EQUAL < (const int) context -> token_array[context -> pointer].value.val_op  ))
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
