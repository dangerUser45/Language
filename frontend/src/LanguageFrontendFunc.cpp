#include <ctype.h>
#include <string.h>

#include <Common.h>
#include <Color.h>

#include <LanguageFrontend.h>
#include <LanguageFrontendDump.h>

static ERRORS GetTokenNum (node* token_array, const char** string, size_t* token_index);
static const char* GetToken_Operator_or_ID (Language* language, const char* string, size_t* token_index, size_t* name_table_index);
static int   Compare_KeyWords (const char* string, size_t length_word);

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
static node* GetDeclareID (Context_parser* context, type_id type_id);

static node* GetExpression (Context_parser* context);
static node* GetTerm (Context_parser* context);
static node* GetPow  (Context_parser* context);
static node* GetPrimaryExpression (Context_parser* context);
static node* GetBracketEx (Context_parser* context);
static node* GetNumber (Context_parser* context);
static node* GetID (Context_parser* context);

static node* CreateFillerNode (const char* text);
static void  GetSetID (Context_parser* context, type_id type_id);
static bool  CompareIDtype (Context_parser* context);

static ERRORS ParsingAST (Language* language, FILE* file_ptr);
static const char* InsertTab (char* buffer, size_t counter);
static void WriteOneList(Language* language, node* node, FILE* file_ptr, char* buffer, size_t tab_counter);

static const char* WriteTypeNode (type_t type);
static const char* WriteValue (Language* language, node* node);
static const char* WriteOP (operations op_code);
static const char* WriteNameTableID (type_id type_id);

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
    Context_parser context = {.language = language,
                              .token_array = language->token_array,
                              .name_table = language->name_table,
                              .pointer = 0};

    node* node = GetGrammar (&context);
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

        if (!node_action) node_action = GetDeclareID (context, GLOBAL);
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

    node* nodeID = GetID (context);
    if (nodeID == NULL)
        return NULL;
    GetSetID (context, FUNCTION);

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
        GetSetID (context, PARAM);

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
node* GetDeclareID (Context_parser* context, type_id type_id)
{
    node* token_array  = context->token_array;
    size_t old_pointer = context->pointer;

    node* node_ID = GetID (context);
    if (node_ID == NULL)
    {
        context->pointer = old_pointer;
        return NULL;
    }

    GetSetID (context, type_id);

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

    GetSetID (context, FUNCTION);

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
    GetSetID (context, FUNCTION);


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

        if (!node_op) node_op = GetDeclareID (context, LOCAL);
        if (!node_op) context->pointer = old_pointer;

        if (!node_op) node_op =  GetWhile (context);
        if (!node_op) context->pointer = old_pointer;

        if (!node_op) node_op = GetIf (context);
        if (!node_op) context->pointer = old_pointer;

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
    GetSetID (context, LOCAL);

    if (token_array[context->pointer].type     != OP   ||
    token_array[context->pointer].value.val_op != EQUALS)
        return NULL;

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
        {
            GetSetID (context, LOCAL);
            return node_val;
        }
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
    if (context->token_array[context->pointer].type == ID)
    {
        context -> pointer++;
        return context -> token_array + context -> pointer - 1;
    }

    else return 0;
}
//==================================================================
void GetSetID (Context_parser* context, type_id type_id)
{
    if (context -> token_array[context -> pointer - 1].type != ID)
        SYNTAX_ERROR (DBG_ERROR, context);

    if ((type_id == LOCAL) && (CompareIDtype (context) == true))
    {
        context->name_table[context->token_array[context->pointer - 1].value.id].type = PARAM;
        return;
    }

    context->name_table[context->token_array[context->pointer - 1].value.id].type = type_id;
}
//==================================================================
bool CompareIDtype (Context_parser* context)
{
    NAME_TABLE* name_table = context->name_table;
    size_t name_table_number = context->language->name_table_number;
    size_t pointer = context->token_array[context->pointer - 1].value.id;

    for (size_t i = 0; i < name_table_number; ++i)
    {
        if (name_table[i].type == PARAM)
            if (!strcmp (name_table[pointer].name_id, name_table[i].name_id))
                return true;
    }

    return false;
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
//==================================================================
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
ERRORS LanguageWriteAST (Language* language)
{
    FILE* file_ptr = OpenFile (NAME_OUTPUT_FRONTEND_FILE, "w");

    ParsingAST (language, file_ptr);

    CloseFile (file_ptr);
    return NO_ERRORS;
}
//==================================================================
ERRORS ParsingAST (Language* language, FILE* file_ptr)
{
    size_t token_number = language->token_number;
    node* node = language->parent_node;

    DBG(setvbuf (file_ptr, NULL, _IONBF, 0);)  // Отключение буферизации
    size_t counter = 0;

    char* buffer = (char*) calloc (1024, sizeof (char));

    WriteOneList (language, node, file_ptr, buffer, counter);

    free (buffer);

    return NO_ERRORS;
}
//==================================================================
void WriteOneList(Language* language, node* node, FILE* file_ptr, char* buffer, size_t tab_counter)
{
    if (node == NULL)
        return;

    if (node->type == FILLER || node->type == OP)
    {
        fprintf(file_ptr, "%s{%s:\"%s\"\n",
            InsertTab(buffer, tab_counter),
            WriteTypeNode(node->type),
            WriteValue(language, node));
    }

    else if (node->type == ID)
    {
        fprintf(file_ptr, "%s{%s:%s:\"%s\"}\n",
            InsertTab(buffer, tab_counter),
            WriteTypeNode(node->type),
            WriteNameTableID(language->name_table[node->value.id].type),
            WriteValue(language, node));
        return;
    }

    else
    {
        fprintf(file_ptr, "%s{%s:\"%s\"}\n",
            InsertTab(buffer, tab_counter),
            WriteTypeNode(node->type),
            WriteValue(language, node));
        return;
    }

    WriteOneList(language, node->left, file_ptr, buffer, tab_counter + 1);
    WriteOneList(language, node->right, file_ptr, buffer, tab_counter + 1);

    fprintf(file_ptr, "%s}\n", InsertTab(buffer, tab_counter));
}
//==================================================================
const char* InsertTab (char* buffer, size_t counter)
{
    memset (buffer, '\0', 1024);
    for (size_t i = 0; i < counter; ++i)
        snprintf (buffer + i, 2, "\t");

    return buffer;
}
//==================================================================
const char* WriteTypeNode (type_t type)
{
    switch (type)
    {
        case NUM:
            return "NUM";

        case ID:
            return "ID";

        case OP:
            return "OP";

        case FILLER:
            return "FILLER";

        default:
            fprintf (stderr, RED "ERROR in %s:%d: type is undefined" RESET "\n", __FILE__, __LINE__);
    }

    return 0;
}
//==================================================================================================
const char* WriteValue (Language* language, node* node)
{
    char* buffer = (char*) calloc (512, sizeof (char));

    type_t type = node->type;
    switch (type)
    {
        case NUM:
        {
            sprintf (buffer, "%lf", node->value.val_num);
            return buffer;
        }

        case ID:
        {
            sprintf (buffer, "%s", language->name_table[node->value.id].name_id);
            return buffer;
        }

        case OP:
        {
            sprintf (buffer, "%s", WriteOP (node->value.val_op));
            return buffer;
        }

        case FILLER:
        {
            sprintf (buffer, "%s", node->value.filler);
            return buffer;
        }

        default:
            fprintf (stderr, RED "ERROR in %s:%d: type is undefined" RESET "\n", __FILE__, __LINE__);
    }

    free (buffer);

    return 0;
}
//==================================================================================================
const char* WriteOP (operations op_code)
{
    switch (op_code)
    {
        case BEGINING:
            return "Breakfast";

        case OPENING_CURLY_BRACKET:
            return "want-millpops";

        case CLOSING_CURLY_BRACKET:
            return "dirty";

        case OPENING_BRACKET:
            return "(";

        case CLOSING_BRACKET:
            return ")";

        case DECLARATION_ID:
            return "bombardiro-krokodilo";

        case EQUALS:
            return "yes-yes";

        case DECLARATION_FUNCTION:
            return "is-the-base";

        case IF:
            return "double-yummy";

        case WHILE:
            return "golubino-shpioniro";

        case SIN:
            return "sin";

        case COS:
            return "cos";

        case ENDING:
            return "DIYA-kontora...";

        case ADDITTION:
            return "+vibe";

        case SUBTRACTION:
            return "-vibe";

        case DIVISION:
            return "/";

        case MULTIPLICATION:
            return "*";

        case ELEVATION:
            return "^";

        case SEPARATOR_PARAM:
            return "skip";

        case SEPARATOR_IN_DECLARE_1:
            return "norm";

        case SEPARATOR_IN_DECLARE_2:
            return "cringe";

        case EQUAL_COMPARE:
            return "==";

        case NOT_EQUALE_COMPARE:
            return "!=";

        case LESS:
            return "<";

        case LESS_OR_EQUALE:
            return "<=";

        case MORE:
            return ">";

        case MORE_OR_EQUAL:
            return ">=";

        case COMMENTS:
            return "|skip:";

        case PARAM_ENVIRONMENT:
            return "_";

        case RETURN:
            return "tralalelo-tralala";

        default:
            return 0;
    }

    return 0;
}
//==================================================================================================
const char* WriteNameTableID (type_id type_id)
{
    switch (type_id)
    {
        case GLOBAL:
            return "GLOBAL";

        case LOCAL:
            return "LOCAL";

        case FUNCTION:
            return "FUNCTION";

        case PARAM:
            return "PARAM";

        default:
            return 0;
    }
}
//==================================================================================================
