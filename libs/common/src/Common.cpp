#include <stdio.h>
#include <sys/stat.h>
#include <string.h>

#ifdef ASSERT_ON
#include <errno.h>
#endif

#include <Common.h>

#include <Color.h>

//==================================================================
FILE* OpenFile (const char* filename, const char* modes)
{
    FILE* file_ptr = fopen ( filename, modes);

    ASSERT_CHECK
    (
        if (file_ptr == NULL)
        {
            fprintf (stderr, RED "ERROR in %s %d: OpenFile()\n" RESET, __FILE__, __LINE__);
            return NULL;
        }
    )

    return file_ptr;
}
//==================================================================
int CloseFile (FILE* file)
{
    int error = fclose (file);

    ASSERT_CHECK
    (
        if (error != 0)
        {
            fprintf (stderr, RED "ERROR in %s:%d: fclose return value = %d:" RESET, __FILE__, __LINE__, error);
            fprintf (stderr, RED "errno = %d", errno);
            return error;
        }
    )

    return 0;
}
//==================================================================
size_t GetFileLength (const char* filename)
{
    struct stat file_info = {};
    stat (filename, &file_info);

    return file_info.st_size;
}
//==================================================================
const char* Skip_delimiters (const char* string)
{
    static const char* delimiters = "\n\t\r ";

    while (strchr (delimiters, *string))
        ++string;

    return string;
}
//==================================================================
