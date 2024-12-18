#define TX_COMPILED
#include "..\SuperLibs\TXLib.h"


#include <stdio.h>
#include "Common_Language.h"
#include "Init_Language.h"
#include "Lexical_analyzer.h"
#include "Dump_Language.h"
#include "Parser_Language.h"

#define DUMP(text)  Dump_graphviz_language ((void*) (language_data ->Token_array), text)

int main (int argc, char* argv[])
{
    setlocale (LC_ALL, "Russian");
    Language* language_data = Language_init (argc, argv);

    Lexical_analyzer (language_data);
    DUMP (TOKEN_TABLE);
    Dump_graphviz_language ((void*) (language_data -> name_table), _NAME_TABLE);

    //Lexeme_gluing (language_data);                                                                  //TODO make many word lexemes
    //  Get_Grammatic (language_data);

    printf ("All Good!");
    txDisableAutoPause ();

}

#undef DUMP
