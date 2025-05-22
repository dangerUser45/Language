#include <stdlib.h>
#include <stdio.h>

#include <Common.h>
#include <LanguageBackendFunc.h>
#include <Color.h>

#include <LanguageBackendCtorDtor.h>

//==================================================================
Backend* LanguageBackendCtor ()
{
    FILE* file_input  = OpenFile  (NAME_INPUT_FILE,  "r");
    z(file_input, p) //FIXME

    FILE* file_output = OpenFile  (NAME_OUTPUT_FILE, "w");
    z(file_output, p) //FIXME

    Backend* backend = (Backend*) calloc (1, sizeof (Backend));
    backend->file_input = file_input;
    backend->file_output = file_output;

    return backend;
}
//==================================================================
void LanguageBackendDtor (Backend* backend)
{
    CloseFile (backend->file_input);
    CloseFile (backend->file_output);

    free (backend);
}
//==================================================================
