#pragma once
#include "Common_Language.h"
struct node;

FILE* Create_file (const char* name_of_file);
void Close_File (FILE* file);
Language* Language_init (int argc ,char* argv[]);

const int MAX_NUM_ID = 1024;