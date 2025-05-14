#include <stdio.h>

#include <LanguageGeneral.h>
#include <LanguageCtorDtor.h>
#include <LanguageFrontend.h>

#include <Common.h>
#include <Color.h>

//==================================================================
int main (int argc, char** argv)
{
    Language* language = LanguageCtor (argc, argv);

    Language_LexicalAnalyser (language);


    LanguageDtor (language);
    fprintf (stderr, ORANGE "OK %s:%d\n" RESET, __FILE__, __LINE__);

}
//==================================================================
