#include <LanguageBackendCtorDtor.h>
#include <LanguageBackendFunc.h>

//==================================================================
int main(int argc, char** argv)
{
    CheckArg (argc, argv);
    Backend* backend = LanguageBackendCtor ();

    LanguageBackendGeneratePrologue(backend->file_input);

    //LanguageBackendParser(backend->file_input, backend->file_output);

    LanguageBackendGenerateEpilogue(backend->file_output);
    LanguageBackendDtor (backend);
}
//==================================================================
