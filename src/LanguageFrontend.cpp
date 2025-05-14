#include <LanguageGeneral.h>
#include <LanguageErrors.h>
#include <LanguageFrontend.h>

#include <Common.h>

//==================================================================
ERRORS Language_LexicalAnalyser (Language* language)
{
    const char* data = language->code_text;

    Skip_space (data);

    return NO_ERRORS;
}
//==================================================================
