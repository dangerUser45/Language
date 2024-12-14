#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "Common_Language.h"
#include "Lexical_analyzer.h"
#include "Init_Language.h"
#include "Dump_Language.h"

static void Get_Token_Num (node* Token_array,  const char* string, size_t* token_ptr, size_t* pointer);
//static int Get_Token_ID (node* Token_array,  const char* string, size_t* token_ptr, size_t* pointer_addr);
static void Get_Token_Operator_ID (NAME_TABLE* name_table, node* Token_array,  const char* string, size_t* token_ptr, size_t* pointer_addr, size_t* name_table_ptr_addr);
static int Compare_KeyWords (const char* string, size_t num_symbols);

//==================================================================================================
int Lexical_analyzer (Language* Lang_data)
{
    const char* string = Lang_data -> onegin -> buffer_addr;

    node* Token_array = (node*) calloc (NUM_OF_TOKEN, sizeof (node));
    Lang_data -> Token_array = Token_array; 

    size_t pointer   = 0;
    size_t token_ptr = 0;
    size_t name_ptr  = 0;
    size_t name_table_ptr = 0;

    while  (string[pointer] != '\0')
    {
        if (isdigit (string[pointer]))
            Get_Token_Num (Token_array, string, &token_ptr, &pointer);

        else Get_Token_Operator_ID (Lang_data -> name_table, Token_array, string, &token_ptr, &pointer, &name_table_ptr);
         //   Get_Token_ID (Token_array, string, &token_ptr, &pointer);
    }

    /*printf ("Token_array:\n");
    for (int i = 0; i< NUM_OF_TOKEN; i++)
        printf ("%d)%lg\n", i, Token_array[i].value.val_num);*/

    return 0;
}
//=====================================================================================================
static void Get_Token_Num (node* Token_array,  const char* string, size_t* token_ptr, size_t* pointer)
{
    char* end_ptr = 0;

    val_t val = strtod (string + *pointer, &end_ptr);

    Token_array[*token_ptr].type = NUM;
    Token_array[*token_ptr].value.val_num  = val;

    (*token_ptr)++;
    *pointer = end_ptr - string;
    Skip_space (string, pointer);
            DBG(printf ("pointer = %zu", *pointer);)
}
//==================================================================================================
static void Get_Token_Operator_ID (NAME_TABLE* name_table, node* Token_array,  const char* string, size_t* token_ptr, size_t* pointer_addr, size_t* name_table_ptr_addr)
{
    size_t old_pointer = *pointer_addr;
    while (isalpha (string[*pointer_addr]))
        (*pointer_addr)++;
    
    int op = Compare_KeyWords (string + old_pointer, *pointer_addr - old_pointer);
    if (op)
    {
        Token_array[*token_ptr].type  = OP;
        Token_array[*token_ptr].value.val_op = (operations) op;
    }

    else
    {
        strncpy (name_table[*name_table_ptr_addr].name_id, string + *pointer_addr, *pointer_addr - old_pointer);
        Token_array[*token_ptr].type = ID;
        Token_array[*token_ptr].value.id = *name_table_ptr_addr;
        (*name_table_ptr_addr)++;
    }

    Skip_space (string, pointer_addr);
    (*token_ptr)++;
}
//==================================================================================================
/*static int Get_Token_ID (node* Token_array,  const char* string, size_t* token_ptr, size_t* pointer_addr)
{
    size_t pointer = *pointer_addr;

    while (isalpha (string[pointer]))
        pointer++;
}
//==================================================================================================*/
#define NOT_OP 0

static int Compare_KeyWords (const char* string, size_t num_symbols)
{
    static Key_words KeyWords [] = {
        {"sin", SIN},
        {"cos", COS},
        {"=", EQUALS},
        {"if", IF}
        
    };

    size_t number_op = sizeof (KeyWords) / sizeof (Key_words);
    int op = 0;
    
    for (int i = 0; i < number_op; i++)
        if  (!strncmp (string, KeyWords[i].name_op, num_symbols))
            return KeyWords[i].OP;
    
    return NOT_OP;
}

#undef NOT_OP
//==================================================================================================
void Skip_space (const char* string, size_t* pointer) 
{
    const char* str = string + (*pointer);
    const char* old_str = string + (*pointer);
    while (1)
        if (*str == ' ' || *str == '\t' || *str == '\r' || *str == '\n' )
            ++str;
        else break;
    
     *pointer  = *pointer + str - old_str;
    
}
//=====================================================================================================