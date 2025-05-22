#include <stdio.h>

#include <LanguageFrontend.h>
#include <LanguageFrontendDump.h>

#include <Common.h>
#include <Color.h>

//==================================================================
int main (int argc, char** argv)
{
    Language* language = LanguageCtor (argc, argv);

    Language_LexicalAnalyser (language);
    LanguageGraphDump (language);

    Language_SyntaxAnalyser  (language);
    LanguageGraphDump (language);

    LanguageWriteAST (language);

    LanguageDtor (language);
    fprintf (stderr, ORANGE "OK %s:%d\n" RESET, __FILE__, __LINE__);
}
//==================================================================
