#pragma once

#include <stdio.h>

#ifdef ASSERT_ON
    #define assert(pointer); if (pointer == NULL)\
                             {\
                            fprintf(stderr, RED "ASSERTION FAILED:%s:%d:(" #pointer ") = (null)\n" RESET, __FILE__, __LINE__);\
                            exit (0);\
                             }
    #define ASSERT_CHECK( ... ) __VA_ARGS__
#else
    #define assert(pointer);
    #define ASSERT_CHECK( ... )
#endif

#ifdef _DEBUG
    #define DBG( ... ) __VA_ARGS__
#else
    #define DBG( ... )
#endif

#define $(var, specifier) {printf (#var " = %" #specifier "\n", var);}


FILE*  OpenFile (const char *__restrict filename, const char *__restrict modes);
int    CloseFile (FILE* file);
size_t GetFileLength (const char* filename);

void    Skip_space (const char* string__, size_t* pointer);
