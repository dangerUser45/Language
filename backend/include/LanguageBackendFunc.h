#pragma once

#include <stdio.h>

struct Symbol
{
    char* name;
    bool is_function;
    int param_count;
};

struct Backend
{
    FILE* file_input;
    FILE* file_output;
};

inline const char* NAME_INPUT_FILE  = "../frontend/output/Output.txt";
inline const char* NAME_OUTPUT_FILE = "output/OutputBackend.s";


void LanguageBackendGeneratePrologue(FILE* out);

void LanguageBackendParser(FILE* in, FILE* out);

void LanguageBackendGenerateEpilogue(FILE* out);
