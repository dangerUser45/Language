#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "..\Processor\Onegin_for_proc\Onegin_processing.h"
#include "..\Processor\Onegin_for_proc\Onegin_General.h"
#include "..\Processor\Onegin_for_proc\Print.h"
#include "Common_Language.h"
#include "Init_Language.h"


FILE* Log_File;
FILE* Graph_File;
FILE* Graph_File_Utf8;

Language* Language_init (int argc ,char* argv[])
{

    Log_File = Create_file ("LOG_LANGUAGE.html");        //!!!    
    fprintf (Log_File, "<pre>");

    Language* lang_data = (Language*) calloc (1, sizeof (Language));
    if (!lang_data) { fprintf (Log_File, "ERROR IN %s: %d: lang_data = null\n", __FILE__, __LINE__); return 0; }

    ONEGIN* onegin_data = (ONEGIN*)   calloc (1, sizeof (ONEGIN));
    if (!onegin_data) { fprintf (Log_File, "ERROR IN %s: %d: onegin_data = null\n", __FILE__, __LINE__); return 0; }

    NAME_TABLE* name_table = (NAME_TABLE*) calloc (MAX_NUM_ID, sizeof (NAME_TABLE));
    if (! name_table) { fprintf (Log_File, "ERROR IN %s: %d: name_table = null\n", __FILE__, __LINE__); return 0; }

    lang_data -> name_table = name_table;
    lang_data -> onegin = onegin_data;

    Check_argc (argc);
    onegin_data -> name = argv[1];
    onegin_data -> fsize = file_size (argv[1]);
    Check_fsize (onegin_data -> fsize);
    Read_File (onegin_data); 
    DBG_Print (onegin_data);

    return lang_data;
}
//==================================================================================================
FILE* Create_file (const char* name_of_file)
{
    FILE* file = fopen (name_of_file, "w+"); 

    if (file == NULL)
    {
        fprintf (stdout, "ERROR in open file\n %s", name_of_file);
        return 0;
    }
    
    setvbuf (file, 0, 0, _IONBF);

    return file;
}
//==================================================================================================
void Close_File (FILE* file)
{
    if (file) fclose (file); 
    return;
}
//==================================================================================================