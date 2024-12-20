#pragma once
#include <stdio.h>
#include "..\Processor\Onegin_for_proc\Onegin_General.h" 

enum type_t     {NUM = 1, ID, OP};
enum operations { 
    ADD                   = 1,
    SUB                   = 2,
    MUL                   = 3, 
    DIV                   = 4, 
    COS                   = 5, 
    SIN                   = 6, 
    POW                   = 7, 
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
    CALL_VAR              = 19,
    CALL_FUNCTION         = 20,
    BEGINING              = 21,
    ENDING                = 22,
    PRE_EQUAL             = 24,
    IN_EQUAL              = 25,
    ADDITTION             = 26,
    SUBTRACTION           = 27,
    DIVISION              = 28,
    MULTIPLICATION        = 29,
    ELEVATION             = 30,

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
#else 
    #define DBG( ... )
#endif         

const int NUM_OF_TOKEN = 128;
const int INIT_NUM_KEY_WORDS  = 1024;
