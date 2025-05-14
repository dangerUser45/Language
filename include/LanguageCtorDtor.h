#pragma once

#include <LanguageGeneral.h>
#include <LanguageErrors.h>

Language* LanguageCtor (int argc, char** argv);


ERRORS CheckArguments (int argc, char** argv);
ERRORS LoadCodeText   (char** argv, char* code_text, size_t length_file);

ERRORS LanguageDtor (Language* language);
