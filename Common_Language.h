#pragma once
#include <stdio.h>
#include "..\Processor\Onegin_for_proc\Onegin_General.h" 

enum type_t     {NUM = 1, ID, OP};
enum operations { 
    ADD    = 1,
    SUB    = 2,
    MUL    = 3, 
    DIV    = 4, 
    COS    = 5, 
    SIN    = 6, 
    POW    = 7, 
    LOG    = 8, 
    LN     = 9, 
    EXP    = 10, 
    TAN    = 11, 
    IF     = 12,
    EQUALS = 13  
};
  
#define TYPE "lg"
#define SCALE 50
#define LENGTH 256 

const int MAX_NAME_ID = 64;

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
