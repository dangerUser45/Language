#include <ctype.h>
#include <string.h>

#include <Common.h>
#include <Color.h>

#include <LanguageFrontend.h>
#include <LanguageFrontendDump.h>

static ERRORS GetTokenNum (node* token_array, const char** string, size_t* token_index);
static const char* GetToken_Operator_or_ID (Language* language, const char* string, size_t* token_index, size_t* name_table_index);
static int    Compare_KeyWords (const char* string, size_t length_word);

static node* GetGrammar (Context_parser* context);
static node* GetFunctionDefinition (Context_parser* context);

static node* GetAssignment (Context_parser* context);

static node* GetIf (Context_parser* context);
static node* GetWhile (Context_parser* context);

static node* GetComparison (Context_parser* context);
static bool  GetSymComparison (operations op);

static node* GetBody (Context_parser* context, body_type body_type);

static node* GetCallFunction (Context_parser* context);
static node* GetDeclareFunction (Context_parser* context);
static node* GetDeclareID (Context_parser* context);

static node* GetExpression (Context_parser* context);
static node* GetTerm (Context_parser* context);
static node* GetPow  (Context_parser* context);
static node* GetPrimaryExpression (Context_parser* context);
static node* GetBracketEx (Context_parser* context);
static node* GetNumber (Context_parser* context);
static node* GetID (Context_parser* context);

static node* CreateFillerNode (const char* text);

//==================================================================
#define SYNTAX_ERROR(error, context)\
{\
    fprintf (stderr, RED "SYNTAX ERROR: %s:%d:" YELLOW "%s" RESET "\nError = %d\n" RESET, __FILE__, __LINE__, GetValue (context->language, &context->token_array[context->pointer]), error);\
    exit (1);\
}
//==================================================================
ERRORS Language_LexicalAnalyser (Language* language)
{
    const char* string = language->code_text;
    node* token_array  = language->token_array;
    size_t token_index = 0;
    size_t name_table_index = 0;

    Skip_delimiters (string);

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
    node* token_array = language->token_array;
    NAME_TABLE* name_table = language->name_table;

    const char* start = string;
    const char* delimiters = "\n\t\r ";
    while (!strchr (delimiters, *string))
        ++string;

    size_t length_word = (size_t)string - (size_t)start;

    if (length_word > MAX_NAME_ID)
        ;//SYNTAX_ERROR

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

        (*token_index)++;
    }

    return string;
}
//==================================================================
static int Compare_KeyWords (const char* string, size_t length_word)
{
    #define NOT_OP 0

    static Key_word KeyWords [] = {
        {"Breakfast:", BEGINING, 10},
        {"want-millpops", OPENING_CURLY_BRACKET, 13},
        {"dirty" , CLOSING_CURLY_BRACKET, 5},
        {"(", OPENING_BRACKET, 1},
        {")", CLOSING_BRACKET, 1},
        {"bombardiro-krokodilo", DECLARATION_ID, 20},
        {"yes-yes", EQUALS, 7},
        {"is-the-base", DECLARATION_FUNCTION, 11},
        {"double-yummy", IF, 12},
        {"golubino-shpioniro", WHILE, 18},
        {"tralalelo-tralala", RETURN, 17},
        {"sin", SIN, 3},
        {"cos", COS, 3},
        {"DIYA-kontora...", ENDING, 15},
        {"+vibe", ADDITTION, 5},
        {"-vibe", SUBTRACTION, 5},
        {"/", DIVISION, 1},
        {"*", MULTIPLICATION, 1},
        {"^", ELEVATION, 1},
        {"skip", SEPARATOR_PARAM, 4},
        {"==", EQUAL_COMPARE, 2},
        {"!=", NOT_EQUALE_COMPARE, 2},
        {"<", LESS, 1},
        {"<=", LESS_OR_EQUALE, 2},
        {">", MORE, 1},
        {">=", MORE_OR_EQUAL, 2},
        {"|skip:", COMMENTS, 6},
        {"norm", SEPARATOR_IN_DECLARE_1, 4},
        {"cringe", SEPARATOR_IN_DECLARE_2, 6},
        {"_", PARAM_ENVIRONMENT, 1}
    };

    size_t number_op = sizeof (KeyWords) / sizeof (Key_word);

    for (size_t i = 0; i < number_op; i++)
        if  (!strncmp (string, KeyWords[i].name_op, length_word) && length_word == KeyWords[i].length_key_word)   //!!!
            return KeyWords[i].OP;

    return NOT_OP;
    #undef NOT_OP
}
//==================================================================
ERRORS Language_SyntaxAnalyser (Language* language)
{
    Context_parser context = {.language = language, .token_array = language->token_array, .pointer = 0};

    node* node = GetGrammar (&context);
    z(node, p)
    language->parent_node = node;

    return NO_ERRORS;
}
//==================================================================
node* GetGrammar (Context_parser* context) //NOTE
{
    node* token_array = context->token_array;

    if (token_array->type                      != OP     ||
    token_array[context->pointer].value.val_op != BEGINING)
        SYNTAX_ERROR (DBG_ERROR, context);

    ++context->pointer;

    node* node_action = 0;
    node* node_filler_1 = CreateFillerNode ("ACTION");
    node* node_filler_1_copy = node_filler_1;

    do
    {
        node_action = 0;
        size_t old_pointer = context->pointer;

        if (!node_action) node_action = GetDeclareFunction (context);
        if (!node_action) context->pointer = old_pointer;

        if (!node_action) node_action = GetDeclareID (context);
        if (!node_action) context->pointer = old_pointer;

        if (!node_action) node_action = GetFunctionDefinition (context);
        if (!node_action) context->pointer = old_pointer;

        if (context->pointer == old_pointer)
            SYNTAX_ERROR (DBG_ERROR, context)

        node* node_filler_2 = CreateFillerNode ("ACTION");

        node_filler_1->right = node_filler_2;
        node_filler_1->left  = node_action;

        node_filler_1 = node_filler_2;

    } while (token_array[context->pointer].type         != OP   ||
             token_array[context->pointer].value.val_op != ENDING);

    return node_filler_1_copy;
}
//==================================================================
node* GetFunctionDefinition (Context_parser* context) //NOTE
{
    node* token_array  = context->token_array;
    size_t old_pointer = context->pointer;

    node* nodeID = GetID (context);
    if (nodeID == NULL)
        return NULL;

    if (token_array[context->pointer].type     != OP              ||
    token_array[context->pointer].value.val_op != PARAM_ENVIRONMENT)
        return NULL;

    ++context->pointer;

    node* node_filler_fn_def = CreateFillerNode ("FUNCTION_DEFINITION");
    node* node_func          = CreateFillerNode ("FUNCTION");

    node_filler_fn_def->left = node_func;
    node_func->left = nodeID;

    while (token_array[context->pointer].type  != OP              ||
    token_array[context->pointer].value.val_op != PARAM_ENVIRONMENT)
    {
        node* node_param = CreateFillerNode ("PARAM");

        node* node = GetID (context);
        if (!node) SYNTAX_ERROR (DBG_ERROR, context);

        if (token_array[context->pointer].type     != OP            ||
        token_array[context->pointer].value.val_op != SEPARATOR_PARAM)
            SYNTAX_ERROR (DBG_ERROR, context);
        ++context->pointer;

        node_func->right = node_param;
        node_param->left = node;

        node_func = node_param;
    }
    ++context->pointer;

    node* node_body = GetBody (context, FUNC);
    if (node_body == NULL)
        SYNTAX_ERROR (DBG_ERROR, context);

    node_filler_fn_def->right = node_body;

    return node_filler_fn_def;
}
//==================================================================
node* GetDeclareID (Context_parser* context)
{
    node* token_array  = context->token_array;
    size_t old_pointer = context->pointer;

    node* node_ID = GetID (context);
    if (node_ID == NULL)
    {
        context->pointer = old_pointer;
        return NULL;
    }

    if (token_array[context->pointer].type         != OP                     ||
       (token_array[context->pointer].value.val_op != SEPARATOR_IN_DECLARE_1 &&
        token_array[context->pointer].value.val_op != SEPARATOR_IN_DECLARE_2 ))
        return 0;

    node* filler = CreateFillerNode ("DECLARE ID");
    filler->left = node_ID;

    ++context->pointer;

    return filler;
}
//==================================================================
node* GetDeclareFunction (Context_parser* context)
{
    node* token_array  = context->token_array;

    node* node_ID = GetID (context);
    if (node_ID == NULL)
        return NULL;

    if (token_array[context->pointer].type         != OP              ||
        token_array[context->pointer].value.val_op != PARAM_ENVIRONMENT)
        return NULL;

    ++context->pointer;

    node* node_number = GetNumber (context);
    if (!node_number) return NULL;

    if (token_array->type                      != OP              ||
    token_array[context->pointer].value.val_op != PARAM_ENVIRONMENT)
        SYNTAX_ERROR (DBG_ERROR, context);

    ++context->pointer;

    if (token_array[context->pointer].type != OP ||
       (token_array[context->pointer].value.val_op != SEPARATOR_IN_DECLARE_1 &&
        token_array[context->pointer].value.val_op != SEPARATOR_IN_DECLARE_2 ))
        SYNTAX_ERROR (DBG_ERROR, context);

    ++context->pointer;

    node* filler_declare_func = CreateFillerNode ("DECLARE FUNCTION");

    filler_declare_func->left  = node_ID;
    filler_declare_func->right = node_number;

    return filler_declare_func;
}
//==================================================================
node* GetCallFunction (Context_parser* context)
{
    node* token_array = context->token_array;
    node* node_call_func = CreateFillerNode ("CALL FUNCTION");
    node* node_call_func_copy = node_call_func;

    node* node_ID = GetID (context);
    if (node_ID == NULL)
        return NULL;

    node_call_func->left  = node_ID;

    if (token_array[context->pointer].type         != OP            ||
        token_array[context->pointer].value.val_op != OPENING_BRACKET)
        return NULL;

    ++context->pointer;

    while (token_array[context->pointer].type         != OP            &&
           token_array[context->pointer].value.val_op != CLOSING_BRACKET)
    {
        node* node_param = CreateFillerNode ("PARAM");
        node* node = NULL;

        size_t old_pointer = context->pointer;

        if (!node) node = GetCallFunction (context);
        if (!node) context->pointer = old_pointer;

        if (!node) node = GetExpression (context);
        if (!node) context->pointer = old_pointer;

        if (token_array[context->pointer].type     != OP            ||
        token_array[context->pointer].value.val_op != SEPARATOR_PARAM)
            SYNTAX_ERROR (DBG_ERROR, context);
        ++context->pointer;

        node_call_func->right = node_param;
        node_param->left = node;

        node_call_func = node_param;
    }

    ++context->pointer;

    if (token_array[context->pointer].type         != OP                     ||
       (token_array[context->pointer].value.val_op != SEPARATOR_IN_DECLARE_1 &&
        token_array[context->pointer].value.val_op != SEPARATOR_IN_DECLARE_2 ))
        SYNTAX_ERROR (DBG_ERROR, context);

    ++context->pointer;

    return node_call_func_copy;
}
//==================================================================
node* GetBody (Context_parser* context, body_type body_type)
{
    node* token_array = context->token_array;

    if (token_array[context->pointer].type != OP ||
        token_array[context->pointer].value.val_op != OPENING_CURLY_BRACKET)
        SYNTAX_ERROR (DBG_ERROR, context);
    ++context->pointer;

    node* node_filler_1 = CreateFillerNode ("OP");
    node* node_filler_1_copy = node_filler_1;
    node* node_filler_prelast = 0;

    do
    {
        node* node_op = 0;
        size_t old_pointer = context->pointer;

        if (!node_op) node_op = GetAssignment (context);
        if (!node_op) context->pointer = old_pointer;

        if (!node_op) node_op = GetCallFunction (context);
        if (!node_op) context->pointer = old_pointer;

        if (!node_op) node_op = GetDeclareID (context);
        if (!node_op) context->pointer = old_pointer;

        if (!node_op) node_op =  GetWhile (context);
        if (!node_op) context->pointer = old_pointer;

        if (!node_op) node_op = GetIf (context);
        if (!node_op) context->pointer = old_pointer;

        z(context->pointer, zu) z(old_pointer, zu)

        if (context->pointer == old_pointer)
            SYNTAX_ERROR (DBG_ERROR, context);

        node* node_filler_2 = CreateFillerNode ("OP");

        node_filler_1->right = node_filler_2;
        node_filler_1->left  = node_op;

        node_filler_prelast = node_filler_1;
        node_filler_1 = node_filler_2;

    } while (token_array[context->pointer].type         != OP                  ||
             token_array[context->pointer].value.val_op != CLOSING_CURLY_BRACKET);

    if (body_type == FUNC)
        node_filler_prelast->right = CreateFillerNode ("RET");

    else
        node_filler_prelast->right = NULL;

    ++context->pointer;
    return node_filler_1_copy;
}
//==================================================================
node* GetComparison (Context_parser* context)
{
    node* token_array = context->token_array;
    node* node_1 = 0;
    node* node_2 = 0;

    size_t old_pointer_1 = context->pointer;

    if (!node_1) node_1 = GetCallFunction (context);
    if (!node_1) context->pointer = old_pointer_1;

    if (!node_1) node_1 = GetExpression   (context);
    if (!node_1) context->pointer = old_pointer_1;

    if (token_array[context->pointer].type != OP ||
        !(GetSymComparison (token_array[context->pointer].value.val_op)))
        return NULL;

    size_t pointer_sym_comparison = context->pointer;
    ++context->pointer;

    size_t old_pointer_2 = context->pointer;

    if (!node_2) node_2 = GetCallFunction (context);
    if (!node_2) context->pointer = old_pointer_2;

    if (!node_2) node_2 = GetExpression   (context);
    if (!node_2) context->pointer = old_pointer_2;

    node* node_sym_comparison = &token_array[pointer_sym_comparison];

    node_sym_comparison->left  = node_1;
    node_sym_comparison->right = node_2;

    return node_sym_comparison;
}
//==================================================================
bool GetSymComparison (operations op)
{
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wswitch-enum"

    switch (op)
    {
        case EQUAL_COMPARE:
            return true;

        case NOT_EQUALE_COMPARE:
            return true;

        case LESS:
            return true;

        case LESS_OR_EQUALE:
            return true;

        case MORE:
            return true;

        case MORE_OR_EQUAL:
            return true;

        default:
            return false;
    }
    #pragma GCC diagnostic pop


    return false;
}
//==================================================================
node* GetAssignment (Context_parser* context)
{
    node* token_array = context->token_array;

    node* node_ID = GetID (context);
    if (node_ID == NULL)
        return NULL;
    zz

    if (token_array[context->pointer].type     != OP   ||
    token_array[context->pointer].value.val_op != EQUALS)
        return NULL;
    zz

    node* node_equals = &token_array[context->pointer];
    ++context->pointer;

    node* node = 0;
    size_t old_pointer = context->pointer;

    if (!node) node = GetCallFunction (context);
    if (!node) context->pointer = old_pointer;

    if (!node) node = GetExpression (context);
    if (!node) context->pointer = old_pointer;

    node_equals->left  = node_ID;
    node_equals->right = node;

    return node_equals;
}
//==================================================================
node* GetIf (Context_parser* context)
{
    node* token_array = context->token_array;
    node* node_comparison = GetComparison (context);

    if (!node_comparison)
        return NULL;

    if (token_array[context->pointer].type         != OP ||
        token_array[context->pointer].value.val_op != IF)
            SYNTAX_ERROR (DBG_ERROR, context);

    node* node_if = &token_array[context->pointer];
    ++context->pointer;

    node* node_body = GetBody (context, NO_FUNC);

    node_if->left  = node_comparison;
    node_if->right = node_body;

    return node_if;
}
//==================================================================
node* GetWhile (Context_parser* context) //NOTE
{
    node* token_array = context->token_array;

    if (token_array[context->pointer].type         != OP ||
        token_array[context->pointer].value.val_op != WHILE)
        return NULL;

    node* node_while = &token_array[context->pointer];
    ++context->pointer;

    node* node_comparison = GetComparison (context);
    node* node_body = GetBody (context, NO_FUNC);

    node_while->left  = node_comparison;
    node_while->right = node_body;

    return node_while;
}
//==================================================================
node* GetExpression (Context_parser* context)
{
    assert (context);
    node* token_array = context -> token_array;
    assert (token_array);


    node* node_val1 = GetTerm (context);

    while (token_array[context -> pointer].type == OP && (token_array[context -> pointer].value.val_op  == ADDITTION || token_array[context -> pointer].value.val_op == SUBTRACTION))
    {
        size_t operation_pointer = context -> pointer;

        context -> pointer++;
        node* node_val2 = GetTerm (context);

        token_array[operation_pointer].left  = node_val1;
        token_array[operation_pointer].right = node_val2;
        node_val1 = token_array + operation_pointer;
    }

    return node_val1;
}
//==================================================================
node* GetTerm (Context_parser* context)
{
    node* token_array = context -> token_array;
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
//==================================================================
node* GetPow  (Context_parser* context)
{
    node* token_array = context ->token_array;
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
//==================================================================
node* GetPrimaryExpression (Context_parser* context)
{
    node* node_val = GetBracketEx (context);
    if (node_val) {
          return node_val;}

    else node_val = GetID (context);
        if (node_val)
            return node_val;
        else
          return GetNumber(context);
}
//==================================================================
node* GetBracketEx (Context_parser* context)
{
    node* token_array = context -> token_array;

    if (token_array[context -> pointer].type == OP && token_array[context -> pointer].value.val_op == OPENING_BRACKET)
    {
        context -> pointer++;
        node* node_val = GetExpression (context);

        if (token_array[context -> pointer].value.val_op != CLOSING_BRACKET)
            SYNTAX_ERROR(DBG_ERROR, context);

        context -> pointer++;
        return node_val;
    }
    else
        return 0;
}
//==================================================================
node* GetID (Context_parser* context)
{
    if (context -> token_array[context -> pointer].type == ID) {
        context -> pointer++;
        return context -> token_array + context -> pointer - 1;}

    else return 0;
}
//==================================================================
// node* GetMathFunc (node* token_array)
// {
// //-------------- COS ---------------------------
//     if (string[pointer] == 'c')  {
//     pointer++;
//     node* node = GetBracketEx ();
//     return _COS(node);           }
// //-------------- SIN ---------------------------
//     else if (string[pointer] == 's') {
//     pointer++;
//     node* node = GetBracketEx ();
//     return _SIN(node);               }
// //-------------- LN ---------------------------
//     else if (string[pointer] == 'l') {
//     pointer++;
//     node* node = GetBracketEx ();
//     return _LN(node);                }
//
// //-------------- LOG ---------------------------
//     else if (string[pointer] == 'L') {
//     pointer++;
//     node* node1 = GetBracketEx ();
//         if (!node1) SYNTAX_ERROR(DBG_ERROR);
//
//     node* node2 = GetBracketEx ();
//         if (!node2) SYNTAX_ERROR(DBG_ERROR);
//
//     return _LOG(node1, node2);       }
// //-------------- EXP ---------------------------
//     else if (string[pointer] == 'e') {
//     pointer++;
//     node* node = GetBracketEx ();
//     return _EXP(node);               }
// //-------------- TAN ---------------------------
//     else if (string[pointer] == 't') {
//     pointer++;
//     node* node = GetBracketEx ();
//     return _TAN(node);               }
//
//     else return 0;
// }
//==================================================================*/
node* GetNumber (Context_parser* context)
{
    node* node = 0;

    if (context->token_array[context->pointer].type == NUM)
    {
        node = &context->token_array[context->pointer];
        ++context -> pointer;
    }

    return node;
}
//==================================================================
node* CreateFillerNode (const char* text)
{
    node* Node = (node*) calloc (1, sizeof (node));

    Node -> type = FILLER;
    Node -> value.filler = text;

    return Node;
}
//==================================================================
