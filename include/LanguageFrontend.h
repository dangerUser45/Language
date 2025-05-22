#pragma once

#include <stdlib.h>

const int NUM_ARG = 2;
const int MAX_NUM_ID = 4096;
const int NUM_OF_TOKEN = 4096;

const int MAX_LENGTH_OP = 256;

inline const char* NAME_LANGUAGE_LOG_FILE = "dump/Frontend_log.html";
inline const char* NAME_LANGUAGE_GRAPHVIZ_FILE = "dump/Graphviz_dot/Frontend_graphviz.txt";

typedef double val_t;

enum type_t {NUM = 1, ID, OP, FILLER};
enum type_id {GLOBAL = 1, LOCAL, FUNCTION, PARAM};
enum body_type {FUNC = 1, NO_FUNC};

const int MAX_NAME_ID = 256;
enum operations
{
    ADDITTION               = 1,
    SUBTRACTION             = 2,
    DIVISION                = 3,
    MULTIPLICATION          = 4,
    ELEVATION               = 5,

    COS                     = 6,
    SIN                     = 7,

    IF                      = 8,
    WHILE                   = 9,
    EQUALS                  = 10,

    OPENING_CURLY_BRACKET   = 11,
    CLOSING_CURLY_BRACKET   = 12,
    OPENING_BRACKET         = 13,
    CLOSING_BRACKET         = 14,

    DECLARATION_ID          = 15,
    DECLARATION_FUNCTION    = 16,
    BEGINING                = 17,
    ENDING                  = 18,
    SEPARATOR_PARAM         = 19,
    SEPARATOR_IN_DECLARE_1  = 20,
    SEPARATOR_IN_DECLARE_2  = 21,

    EQUAL_COMPARE           = 22,
    NOT_EQUALE_COMPARE      = 23,
    LESS                    = 24,
    LESS_OR_EQUALE          = 25,
    MORE                    = 26,
    MORE_OR_EQUAL           = 27,

    COMMENTS                = 28,
    PARAM_ENVIRONMENT       = 29,
    RETURN                  = 30,
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
    type_id type;
};

struct Language
{
    size_t length_file;
    size_t token_number;
    size_t name_table_number;

    char* code_text;
    node* token_array;
    NAME_TABLE* name_table;

    node* parent_node;
};

struct Key_word
{
    const char name_op [MAX_LENGTH_OP];
    operations OP;
    size_t length_key_word;

};

struct Context_parser
{
    Language* language;
    node* token_array;
    NAME_TABLE* name_table;
    size_t pointer;
};

enum ERRORS
{
    NO_ERRORS,
};

enum SYNTAX_ERRORS
{
    TOO_LONG_WORD,
    DBG_ERROR,

};

Language* LanguageCtor (int argc, char** argv);

ERRORS Language_LexicalAnalyser (Language* language);
ERRORS Language_SyntaxAnalyser  (Language* language);

ERRORS LanguageDtor (Language* language);
