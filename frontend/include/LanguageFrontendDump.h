#pragma once

#include <LanguageFrontend.h>

enum PRINT_OBJ {TREE, NAME_TABLE_AND_TOKENS};

inline const char* NUM_COLOR    = "#ffb48f";
inline const char* ID_COLOR     = "#a64ac9";
inline const char* OP_COLOR     = "#fccd04";
inline const char* FILLER_COLOR = "#17e9e0";

void LanguageDump   (Language* language);
void LanguageGraphDump (Language* language);

const char* GetValue (Language* language, node* node);
