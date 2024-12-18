#define TX_COMPILED
#include "..\SuperLibs\TXLib.h"

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "Common_Language.h"
#include "Lexical_analyzer.h"
#include "Init_Language.h"
#include "Dump_Language.h"
#include "SyntaxError.h"
#include "locale.h"

static void Get_Token_Num (node* Token_array,  const char* string__, size_t* token_ptr, size_t* pointer);
static void Get_Token_Operator_ID (NAME_TABLE* name_table, node* Token_array,  const char* string__, size_t* token_ptr, size_t* pointer_addr, size_t* name_table_ptr_addr);
static int Compare_KeyWords (const char* string__, size_t num_symbols);
static int is_rus_alnum (char symbol);

//==================================================================================================
int Lexical_analyzer (Language* Lang_data)
{
    setlocale(LC_ALL, "Russian");
    const char* string__ = Lang_data -> onegin -> buffer_addr;

    node* Token_array = (node*) calloc (NUM_OF_TOKEN, sizeof (node));
    Lang_data -> Token_array = Token_array;

    size_t pointer   = 0;
    size_t token_ptr = 0;
    size_t name_table_ptr = 0;

    Skip_space (string__, &pointer);
    while  (string__[pointer] != '\0')
    {
        if (isdigit (string__[pointer]))
            Get_Token_Num (Token_array, string__, &token_ptr, &pointer);

        else Get_Token_Operator_ID (Lang_data -> name_table, Token_array, string__, &token_ptr, &pointer, &name_table_ptr);
    }

    return 0;
}
//=====================================================================================================
static void Get_Token_Num (node* Token_array,  const char* string__, size_t* token_ptr, size_t* pointer)
{
    char* end_ptr = 0;
    val_t val = strtod (string__ + *pointer, &end_ptr);
    Token_array[*token_ptr].type = NUM;
    Token_array[*token_ptr].value.val_num  = val;
    (*token_ptr)++;
    *pointer = end_ptr - string__;
    Skip_space (string__, pointer);
}
//==================================================================================================
static void Get_Token_Operator_ID (NAME_TABLE* name_table, node* Token_array,  const char* string__, size_t* token_ptr, size_t* pointer_addr, size_t* name_table_ptr_addr)
{
    size_t old_pointer = *pointer_addr;
    while (is_rus_alnum (string__[*pointer_addr]) || isalnum(string__[*pointer_addr]) || ispunct(string__[*pointer_addr]))
        (*pointer_addr)++;
        DBG (printf ("string + pointer_addr = %-20s\n", string__ + *pointer_addr);)
    size_t length_word = *pointer_addr - old_pointer;
        DBG (printf ("old pointer = %zu ", old_pointer);)
        DBG (printf ("pointer = %zu\n", *pointer_addr) ;)

    if (length_word > MAX_NAME_ID || length_word <= 0)
        ;//SyntaxError (EXCEEDED_NUM_SYM_ID);                                     //TODO make SyntaxError
    else
    {
        int op = Compare_KeyWords (string__ + old_pointer, *pointer_addr - old_pointer);
            //DBG (printf ("op = %d", op);)
        if (op)
        {
                DBG (printf ("pointer = %zu", *pointer_addr);)
            Token_array[*token_ptr].type  = OP;
            Token_array[*token_ptr].value.val_op = (operations) op;
        }

        else
        {
            strncpy (name_table[*name_table_ptr_addr].name_id, string__ + old_pointer, *pointer_addr - old_pointer);
            Token_array[*token_ptr].type = ID;
            Token_array[*token_ptr].value.id = *name_table_ptr_addr;
            (*name_table_ptr_addr)++;
        }
    }
        //DBG (printf ("string + pointer_addr = %-20s\n", string__ + *pointer_addr);)
        //if (is_rus_alnum (string__[*pointer_addr]) || ispunct(string__[*pointer_addr])) assert (0);
    Skip_space (string__, pointer_addr);
    (*token_ptr)++;
}
//==================================================================================================
#define NOT_OP 0

static int Compare_KeyWords (const char* string__, size_t num_symbols)
{
    setlocale(LC_ALL, "Russian");
    static Key_words KeyWords [] = {
        {"�������_�����_������_�����������_��_�������!", BEGINING},
        {"������_�_������_�����_��_������", OPENING_CURLY_BRACKET},
        {"������������_���_������_�_���" , CLOSING_CURLY_BRACKET},
        {"��������_��_������", OPENING_BRACKET},
        {"�������_�_������", CLOSING_BRACKET},
        {"������_�����_<����������>_����_<�����>", CALL_VAR},
        {"=", EQUALS},
        {"�������_��_�������", CALL_FUNCTION},
        {"������ ������ ��", IF},
        {"�������_��_������_���_���", WHILE},
        {"sin", SIN},
        {"cos", COS},
        {"����� ��������� !", ENDING},
        {"�����", PRE_EQUAL},
        {"�_��������_�", IN_EQUAL}
    };

    size_t number_op = sizeof (KeyWords) / sizeof (Key_words);

    for (size_t i = 0; i < number_op; i++)
        if  (!strncmp (string__, KeyWords[i].name_op, num_symbols))
            return KeyWords[i].OP;

    return NOT_OP;
}

#undef NOT_OP
//==================================================================================================
void Skip_space (const char* string__, size_t* pointer)
{
    const char* str = string__ + (*pointer);
    const char* old_str = string__ + (*pointer);
    while (1)
        if (*str == ' ' || *str == '\t' || *str == '\r' || *str == '\n' )
            ++str;
        else break;

     *pointer  = *pointer + str - old_str;

}
//=====================================================================================================
/*void Lexeme_gluing (Language* Lang_data)
{
    assert (Lang_data);

    node* Token_array = Lang_data -> Token_array;
    for (size_t i = 0; Token_array[i].type != NULL; i++)

    if (Token_array[i])


}
*/
//=====================================================================================================
int is_rus_alnum (char symbol)
{
    if (symbol >= '�' && symbol <= '�')  // ����� �������� �.�. �����
        return 1;

    else return 0;
}