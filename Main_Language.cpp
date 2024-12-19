#define TX_COMPILED
#include "..\SuperLibs\TXLib.h"


#include <stdio.h>
#include "Common_Language.h"
#include "Init_Language.h"
#include "Lexical_analyzer.h"
#include "Dump_Language.h"
#include "Parser_Language.h"

#define DUMP_TOKEN      Dump_graphviz_language ((void*) (language_data ->Token_array), TOKEN_TABLE)
#define DUMP_NAME_TABLE Dump_graphviz_language ((void*) (language_data -> name_table), _NAME_TABLE);
#define DUMP_TREE(text) Dump_graphviz_language ((void*) (text), TREE)

int main (int argc, char* argv[])
{
    setlocale (LC_ALL, "Russian");
    Language* language_data = Language_init (argc, argv);

    Lexical_analyzer (language_data);
    DUMP_TOKEN;
    DUMP_NAME_TABLE;

    //Lexeme_gluing (language_data);                                                                  //TODO make many word lexemes
    $$ node* tree = GetGrammatic (language_data);
    $(tree);
    $$ DUMP_TREE  (tree);

    printf ("All Good!");
    txDisableAutoPause ();

}

#undef DUMP_TOKEN
#undef DUMP_NAME_TABLE
#undef DUMP_TREE
