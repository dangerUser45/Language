#pragma once

#include "..\SuperLibs\COLOR.h"

enum __MY_ERROR__ {EXCEEDED_NUM_SYM_ID = 1,

};

#define SYNTAX_ERROR                                                        \
{                                                                           \
    printf ("" RED "Syntax Error: %s: %d \n" RESET "", __FILE__, __LINE__); \
    exit (1);                                                               \
}
