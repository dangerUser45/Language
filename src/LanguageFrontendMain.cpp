#include <stdio.h>

#include <LanguageFrontend.h>
#include <LanguageFrontendDump.h>

#include <Common.h>
#include <Color.h>

//==================================================================
int main (int argc, char** argv)
{
    zz // FIXME
    Language* language = LanguageCtor (argc, argv);
    zz // FIXME

    Language_LexicalAnalyser (language);
    zz // FIXME
    LanguageDump (language);
    zz // FIXME
    //Language_SyntaxAnalyser  (language);

    LanguageDtor (language);
    fprintf (stderr, ORANGE "OK %s:%d\n" RESET, __FILE__, __LINE__);
}
//==================================================================
