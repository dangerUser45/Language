#define TX_COMPILED
#include "..\SuperLibs\TXLib.h"

#include "stdio.h"
#include "LanguageCommon.h"
#include "Language_Init.h"
#include "Lexical_analyzer.h"

int main (int argc, char* argv[])
{
    Language* language_data = Language_init (argc, argv);
    Lexical_analyzer (language_data -> onegin -> buffer_addr);
    $(language_data);


}
