#pragma once

#include <stdlib.h>

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
    char* code_text;
    node* Token_array;
    NAME_TABLE* name_table;
};
