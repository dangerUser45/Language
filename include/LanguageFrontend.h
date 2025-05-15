#pragma once

#include <stdlib.h>

const int NUM_ARG = 2;
const int MAX_NUM_ID = 4096;
const int NUM_OF_TOKEN = 4096;

const int MAX_LENGTH_OP = 256;

typedef double val_t;
enum type_t {NUM = 1, ID, OP, FILLER};

const int MAX_NAME_ID = 256;
enum operations
{
    ADDITTION             = 1,
    SUBTRACTION           = 2,
    DIVISION              = 3,
    MULTIPLICATION        = 4,
    ELEVATION             = 5,

    COS                   = 6,
    SIN                   = 7,
    LOG                   = 8,
    LN                    = 9,
    EXP                   = 10,
    TAN                   = 11,

    IF                    = 12,
    WHILE                 = 13,
    EQUALS                = 14,

    OPENING_CURLY_BRACKET = 15,
    CLOSING_CURLY_BRACKET = 16,
    OPENING_BRACKET       = 17,
    CLOSING_BRACKET       = 18,

    DECLARATION_ID        = 19,
    DECLARATION_FUNCTION  = 20,
    BEGINING              = 21,
    ENDING                = 22,
    PRE_EQUAL             = 23,
    IN_EQUAL              = 24,
    SEPARATOR             = 25,

    EQUAL_COMPARE         = 26,
    NOT_EQUALE_COMPARE    = 27,
    LESS                  = 28,
    LESS_OR_EQUALE        = 29,
    MORE                  = 30,
    MORE_OR_EQUAL         = 31,

    BEGIN_PARAM_FUNC      = 32,
    END_PARAM_FUNC        = 33,

};

union VALUE
{
    double val_num;
    operations val_op;
    size_t id;
    const char* filler;
};

struct node
{
    type_t type;
    VALUE value;
    node* right;
    node* left;
};

struct NAME_TABLE
{
    char name_id [MAX_NAME_ID];
};

struct Language
{
    size_t length_file;
    char* code_text;
    node* token_array;
    NAME_TABLE* name_table;
};

struct Key_word
{
    const char name_op [MAX_LENGTH_OP];
    operations OP;
    size_t length_key_word;

};

enum ERRORS
{
    NO_ERRORS,
};

enum SYNTAX_ERRORS
{
    TOO_LONG_WORD,

};

Language* LanguageCtor (int argc, char** argv);

ERRORS Language_LexicalAnalyser (Language* language);
ERRORS Language_SyntaxAnalyser  (Language* language);

ERRORS LanguageDtor (Language* language);
