#pragma once

#include <LanguageBackendFunc.h>

int CheckArg (int argc, char** argv);

Backend* LanguageBackendCtor ();
void     LanguageBackendDtor (Backend* backend);
