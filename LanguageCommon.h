#pragma once
#include "..\Processor\Onegin_for_proc\Onegin_General.h" 

enum type_t     {NUM = 1, ID, OP};
enum operations {ADD = '+', SUB = '-', MUL = '*', DIV = '/', COS = 'c', SIN = 's', POW = '^', LOG = 'L', LN = 'l', EXP = 'e', TAN = 't'};
  
#define TYPE "lg"
#define SCALE 50
#define LENGTH 256 

typedef double val_t;

struct node
{
    type_t type;
    double val;
    node* right;
    node* left;
};

#define DEBUG 1

#if  (DEBUG == 1) 
    #define DBG( ... ) __VA_ARGS__
#else 
    #define DBG( ... )
#endif         

const int NUM_OF_TOKEN = 1024;
const int INIT_NUM_KEY_WORDS  = 1024;
