#pragma once

#include "Common_Language.h"

const int MAX_LENGTH_OP = 256;

struct Key_word
{
    const char name_op [MAX_LENGTH_OP];
    operations OP;
    size_t length_key_word;

};

int Lexical_analyzer (Language* Lang_data);
void Skip_space (const char* string, size_t* pointer);