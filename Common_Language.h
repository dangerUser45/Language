#pragma once
#include <stdio.h>
#include "..\Processor\Onegin_for_proc\Onegin_General.h" 

enum type_t     {NUM = 1, ID, OP, FILLER};
enum operations { 
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
  
#define TYPE "lg"
#define SCALE 50
#define LENGTH 256 

const int MAX_NAME_ID = 256;

typedef double val_t;

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
    ONEGIN* onegin;
    node* Token_array;
    NAME_TABLE* name_table;
};

#define DEBUG 1

#if  (DEBUG == 1) 
    #define DBG( ... ) __VA_ARGS__
    #define PRINT_TOKEN_POINTER printf ("" BLUE  "Token + pointer = %p\n" RESET "", context -> Token_array + context -> pointer)
#else 
    #define DBG( ... )
    #define PRINT_TOKEN_POINTER     
#endif         

const int NUM_OF_TOKEN = 128;
const int INIT_NUM_KEY_WORDS  = 1024;
