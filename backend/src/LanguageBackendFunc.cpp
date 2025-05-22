#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <LanguageBackendFunc.h>
#include <Color.h>

Symbol symbol_table[1000];
int symbol_count = 0;

//==================================================================
int CheckArg (int argc, char** argv)
{
    if (argc != 3)
    {
        fprintf (stderr, RED "Incorrect number of arguments" RESET);
        return 1;
    }

    return 0;
}
//==================================================================
void add_symbol(char* name, bool is_function, int param_count)
{
    symbol_table[symbol_count].name = strdup(name);
    symbol_table[symbol_count].is_function = is_function;
    symbol_table[symbol_count].param_count = param_count;
    symbol_count++;
}
//==================================================================
void LanguageBackendGeneratePrologue(FILE* out)
{
    fprintf(out, "section .text\n");
    fprintf(out, "global _start\n\n");
    fprintf(out, "_start:\n");
    fprintf(out, "    ; Пролог программы\n");
    fprintf(out, "    mov rbp, rsp\n");
    fprintf(out, "    sub rsp, 256  ;  локальные переменные\n\n");
}
//==================================================================
void LanguageBackendGenerateEpilogue(FILE* out)
{
    fprintf(out, "\n    ; Эпилог программы\n");
    fprintf(out, "    mov rax, 60   ; syscall exit\n");
    fprintf(out, "    xor rdi, rdi  \n");
    fprintf(out, "    syscall\n");
}
//==================================================================
void generate_print(FILE* out)
{
    fprintf(out, "    ; Вывод значения\n");
    fprintf(out, "    mov rax, 1    ; syscall write\n");
    fprintf(out, "    mov rdi, 1    ; stdout\n");
    fprintf(out, "    mov rsi, rsp   ; адрес значения\n");
    fprintf(out, "    mov rdx, 8     ; размер\n");
    fprintf(out, "    syscall\n");
}
//==================================================================
void generate_function (FILE* out, char* name)
{
    fprintf(out, "\n%s:\n", name);
    fprintf(out, "    push rbp\n");
    fprintf(out, "    mov rbp, rsp\n");
}
//==================================================================
void generate_return (FILE* out)
{
    fprintf(out, "    mov rsp, rbp\n");
    fprintf(out, "    pop rbp\n");
    fprintf(out, "    ret\n");
}
//==================================================================
void generate_operation(FILE* out, char* op)
{
    fprintf(out, "    pop rbx\n");
    fprintf(out, "    pop rax\n");

    if (strcmp(op, "+vibe") == 0)
    {
        fprintf(out, "    add rax, rbx\n");
    }

    else if (strcmp(op, "-vibe") == 0)
    {
        fprintf(out, "    sub rax, rbx\n");
    }

    else if (strcmp(op, "*") == 0)
    {
        fprintf(out, "    imul rax, rbx\n");
    }

    else if (strcmp(op, "/") == 0)
    {
        fprintf(out, "    cqo\n");
        fprintf(out, "    idiv rbx\n");
    }

    else if (strcmp(op, "==") == 0)
    {
        fprintf(out, "    cmp rax, rbx\n");
        fprintf(out, "    sete al\n");
        fprintf(out, "    movzx rax, al\n");
    }

    else if (strcmp(op, "<=") == 0)
    {
        fprintf(out, "    cmp rax, rbx\n");
        fprintf(out, "    setle al\n");
        fprintf(out, "    movzx rax, al\n");
    }

    fprintf(out, "    push rax\n");
}
//==================================================================
void generate_literal(FILE* out, int value)
{
    fprintf(out, "    push %d\n", value);
}
//==================================================================
void generate_variable(FILE* out, char* name)
{
    fprintf(out, "    ; Загрузка переменной %s\n", name);
    fprintf(out, "    push qword [rbp - %d*8]  ;\n",
            (int)(strlen(name) % 10));
}
//==================================================================
void generate_function_call(FILE* out, char* name)
{
    fprintf(out, "    call %s\n", name);
    fprintf(out, "    push rax  ; Сохраняем возвращаемое значение\n");
}
//==================================================================
void LanguageBackendParser(FILE* in, FILE* out)
{
    char line[256];
    int depth = 0;
    char* last_op = NULL;
    bool in_function = false;
    char current_function[64] = {0};

    while (fgets(line, sizeof(line), in))
    {

        int current_depth = 0;
        while (line[current_depth] == ' ' || line[current_depth] == '\t')
        {
            current_depth++;
        }
        current_depth /= 4;


        char* type_start = strchr(line, '{') + 1;
        char* type_end = strchr(type_start, ':');
        *type_end = '\0';
        char* type = type_start;

        char* value_start = type_end + 2;
        char* value_end = strchr(value_start, '"');
        *value_end = '\0';
        char* value = value_start;


        if (strcmp(type, "OP") == 0)
        {
            if (strcmp(value, "def") == 0)
            {

                in_function = true;
            }

            else if (strcmp(value, "return") == 0)
            {
                generate_return(out);
            }

            else if (strcmp(value, "call") == 0)
            {

                last_op = (char*)"call";
            }

            else if (strcmp(value, "print") == 0)
            {
                generate_print(out);
            }

            else
            {

                last_op = value;
            }
        }

        else if (strcmp(type, "ID") == 0)
        {
            if (last_op && strcmp(last_op, "call") == 0)
            {
                generate_function_call(out, value);
                last_op = NULL;
            }

            else if (in_function)
            {
                strcpy(current_function, value);
                generate_function(out, value);
                add_symbol(value, true, 0);
                in_function = false;
            }

            else
            {
                generate_variable(out, value);
            }
        }

        else if (strcmp(type, "NUM") == 0)
        {
            int num = atoi(value);
            generate_literal(out, num);
        }

        if (last_op && strchr("+-*/==<>=!", last_op[0]))
        {
            generate_operation(out, last_op);
            last_op = NULL;
        }
    }
}
//==================================================================
