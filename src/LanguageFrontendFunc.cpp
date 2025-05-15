#include <ctype.h>
#include <string.h>

#include <Common.h>
#include <Color.h>

#include <LanguageFrontend.h>

static ERRORS GetTokenNum (node* token_array, const char** string, size_t* token_index);
static ERRORS GetToken_Operator_or_ID (Language* language, size_t* token_index, size_t* name_table_index);
static int    Compare_KeyWords (const char* string, size_t length_word);

static void   SyntaxError (SYNTAX_ERRORS error);

//==================================================================
ERRORS Language_LexicalAnalyser (Language* language)
{
    const char* string = language->code_text;
    node* token_array = language->token_array;
    size_t token_index = 0;
    size_t name_table_index = 0;

    Skip_delimiters (string);
    while (*string != 0)
    {
        if (isdigit (*string))
            GetTokenNum (token_array, &string, &token_index);

        else
            GetToken_Operator_or_ID (language, &token_index, &name_table_index);
    }

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
ERRORS GetToken_Operator_or_ID (Language* language, size_t* token_index, size_t* name_table_index)
{
    const char* string = language->code_text;
    node* token_array = language->token_array;
    NAME_TABLE* name_table = language->name_table;

    const char* start = string;
    const char* delimiters = "\n\t\r ";
    while (!strchr (delimiters, *string))
        ++string;

    size_t length_word = (size_t)string - (size_t)start;

    if (length_word > MAX_NAME_ID)
        SyntaxError (TOO_LONG_WORD);

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

    return NO_ERRORS;
}
//==================================================================
static int Compare_KeyWords (const char* string, size_t length_word)
{
    #define NOT_OP 0

    static Key_word KeyWords [] = {
        {"", BEGINING, 1},
        {"", OPENING_CURLY_BRACKET, 1},
        {"" , CLOSING_CURLY_BRACKET, 1},
        {"", OPENING_BRACKET, 1},
        {"", CLOSING_BRACKET, 1},
        {"", DECLARATION_ID, 1},
        {"=", EQUALS, 1},
        {"", DECLARATION_FUNCTION, 1},
        {"", IF, 1},
        {"", WHILE, 1},
        {"sin", SIN, 1},
        {"cos", COS, 1},
        {"", ENDING, 1},
        {"", PRE_EQUAL, 1},
        {"", IN_EQUAL, 1},
        {"", ADDITTION, 1},
        {"", SUBTRACTION, 1},
        {"", DIVISION, 1},
        {"", MULTIPLICATION, 1},
        {"", ELEVATION, 1},
        {"", SEPARATOR, 1},
        {"", EQUAL_COMPARE, 1},
        {"", NOT_EQUALE_COMPARE, 1},
        {"", LESS, 1},
        {"", LESS_OR_EQUALE, 1},
        {"", MORE, 1},
        {"", MORE_OR_EQUAL, 1},
        {"", BEGIN_PARAM_FUNC,1},
        {"", END_PARAM_FUNC, 1},
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
    fprintf (stderr, RED "SYNTAX ERROR: %s%d: error = %d" RESET, __FILE__, __LINE__, error);
    exit (1);
}
//==================================================================
ERRORS Language_SyntaxAnalyser (Language* language)
{

    return NO_ERRORS;
}
//==================================================================
