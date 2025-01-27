#include <stdio.h>
#include <ctype.h>
#include <locale.h>

struct node
{
    int a;
    double b;
};

int main ()
{
    FILE* ptr  = fopen ("r.txt", "wb+"); 
    fprintf (ptr, "\033[0m");

    fclose (ptr);
}
