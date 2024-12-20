#pragma once

#include "Common_Language.h"

struct Context_parser
{
    node* Token_array;
    size_t pointer;
};

node* GetGrammatic (Language* Lang_data);