#include <stdio.h>
#include <string.h>

#include <Color.h>
#include <Common.h>

#include <LanguageGeneral.h>
#include <LanguageErrors.h>
#include <LanguageConsts.h>
#include <LanguageCtorDtor.h>

FILE* lang_log_file;

//==================================================================================================
Language* LanguageCtor (int argc, char** argv)
{
    ASSERT_CHECK (
        CheckArguments (argc, argv);
    )

    Language* language = (Language*) calloc (1, sizeof (Language));

    size_t length_file = GetFileLength (argv[1]);
    char* code_text = (char*) calloc (length_file, sizeof (char));
    LoadCodeText (argv, code_text, length_file);

    NAME_TABLE* name_table = (NAME_TABLE*) calloc (MAX_NUM_ID, sizeof (NAME_TABLE));

    node* token_array = (node*) calloc (NUM_OF_TOKEN, sizeof (node));

    language->code_text   = code_text;
    language->name_table  = name_table;
    language->Token_array = token_array;

    return language;
}
//==================================================================================================
ERRORS  CheckArguments (int argc, char** argv)
{

    if (argc != NUM_ARG)
    {
        fprintf (stderr, RED "ERROR: %s:%d: uncorrect number of arguments\n" RESET, __FILE__, __LINE__);
        assert (0);
    }

    if (argv == NULL)
    {
        fprintf (stderr, RED "ERROR: %s:%d: argv is null\n" RESET, __FILE__, __LINE__);
        assert (0);
    }

    return NO_ERRORS;
}
//==================================================================================================
ERRORS LoadCodeText (char** argv, char* code_text, size_t length_file)
{
    FILE* code_file = OpenFile (argv[1], "r");

    size_t num_read_elem = fread (code_text, sizeof(char), length_file, code_file);
    ASSERT_CHECK
    (
        if (num_read_elem == 0)
            fprintf (stderr, RED "ERROR in %s:%d: number of read elements = (null):" YELLOW "%s" RESET, __FILE__, __LINE__, argv[1]);
    )

    CloseFile (code_file);

    return NO_ERRORS;
}
//==================================================================================================
ERRORS LanguageDtor (Language* language)
{
    free (language->code_text);
    free (language->name_table);
    //free (language->Token_array);

    free (language);

    return NO_ERRORS;
}
//==================================================================================================

//TODO
/***                                         ***\
;           add free (Token_array);
;
\***                                         ***/
