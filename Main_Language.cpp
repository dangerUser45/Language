/*#define TX_COMPILED
#include "..\SuperLibs\TXLib.h"*/

#include <stdio.h>
#include "Common_Language.h"
#include "Init_Language.h"
#include "Lexical_analyzer.h"
#include "Dump_Language.h"

#define DUMP(text)  Dump_graphviz_language ((void*) (language_data ->Token_array), text)

int main (int argc, char* argv[])
{
    //atexit ((void (*)())txDisableAutoPause);

    Language* language_data = Language_init (argc, argv);
    //$(language_data);

    Lexical_analyzer (language_data);
    DUMP (TOKEN_TABLE);
    Dump_graphviz_language ((void*) (language_data -> name_table), _NAME_TABLE);

}

#undef DUMP
