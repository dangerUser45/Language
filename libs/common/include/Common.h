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

#define z(var, specifier) {fprintf (stderr, BLUE "%s:%d: " #var " = %" #specifier RESET "\n", __FILE__, __LINE__, var);}
#define zz {fprintf (stderr, "%s:%d\n", __FILE__, __LINE__);}


FILE*  OpenFile (const char *__restrict filename, const char *__restrict modes);
int    CloseFile (FILE* file);
size_t GetFileLength (const char* filename);

const char* Skip_delimiters (const char* string);
