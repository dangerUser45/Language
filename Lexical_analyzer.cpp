#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "LanguageCommon.h"
#include "Lexical_analyzer.h"

static int Get_Token_Num (node* Token_array,  const char* string, size_t* token_ptr, size_t* pointer);
static int Get_Token_ID (node* Token_array,  const char* string, size_t* token_ptr, size_t* pointer_addr);
static int Get_Token_Operator (node* Token_array,  const char* string, size_t* token_ptr, size_t* pointer_addr);
static int Compare_KeyWords (const char* string, size_t num_symbols);
//==================================================================================================
int Lexical_analyzer (const char* string)
{
    size_t pointer = 0;
    node* Token_array = (node*) calloc (NUM_OF_TOKEN, sizeof (node));
    size_t token_ptr = 0;

    while  (string[pointer] != '\0')
    {

        if (isdigit (string[pointer]))
            Get_Token_Num (Token_array, string, &token_ptr, &pointer);

        if (!Get_Token_Operator (Token_array, string, &token_ptr, &pointer))
            Get_Token_ID (Token_array, string, &token_ptr, &pointer);
    }
    return 0;
}
//==================================================================================================
static int Get_Token_Num (node* Token_array,  const char* string, size_t* token_ptr, size_t* pointer)
{
    char* end_ptr = 0;

    val_t val = strtod (string, &end_ptr);

    Token_array[*token_ptr].type = OP;
    Token_array[(*token_ptr)++].val  = val;

    *pointer = end_ptr - string;

    return 0;
}
//==================================================================================================
static int  Get_Token_Operator (node* Token_array,  const char* string, size_t* token_ptr, size_t* pointer_addr)
{
    size_t old_pointer = *pointer_addr;
    while (isalpha (string[*pointer_addr]))
        (*pointer_addr)++;
    
    return Compare_KeyWords (string + old_pointer, *pointer_addr - old_pointer);
}   
//==================================================================================================
static int Get_Token_ID (node* Token_array,  const char* string, size_t* token_ptr, size_t* pointer_addr)
{
    size_t pointer = *pointer_addr;

    while (isalpha (string[pointer]))
        pointer++;
}
//==================================================================================================
static int Compare_KeyWords (const char* string, size_t num_symbols)
{
    int op = 0;
    const char* KeyWords[INIT_NUM_KEY_WORDS] = {
        "sin",
        "cos",
        "tan",
        "cot",
        "e",
        "ln",
        "log"
        };
    
    for (int i = 0; i < INIT_NUM_KEY_WORDS; i++)
    int op += strncmp (string, KeyWords[i], num_symbols);

    return n;
}
//==================================================================================================
