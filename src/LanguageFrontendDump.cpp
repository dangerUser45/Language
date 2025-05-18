#include <stdio.h>
#include <string.h>

#include <Color.h>
#include <Common.h>

#include <LanguageFrontend.h>
#include <LanguageFrontendDump.h>

extern FILE* language_log_file;
extern FILE* language_graphviz_file;

static size_t images_counter = 0;

//==================================================================================================
void DumpTokenArray (Language* language)
{
    assert (language);
    assert (language->token_array);

    node* token_array = language->token_array;

    fprintf (stderr, GREEN_BGD "T O K E N   A R R A Y" RESET "\n");
    for (int i = 0; token_array[i].type != 0; ++i)
    {
        assert (&token_array[i]);
        fprintf (stderr, "token array[%d] address = " BLUE "%p" RESET "\n", i, &token_array[i]);

        if (token_array[i].type == NUM)
        {
            fprintf (stderr, "token array[%d].type    = " BLUE "NUM" RESET "\n", i);
            fprintf (stderr, "token array[%d].value   = " BLUE "%lf" RESET "\n", i, token_array[i].value.val_num);
        }

        else if (token_array[i].type == ID)
        {
            fprintf (stderr, "token array[%d].type    = " BLUE "ID" RESET "\n", i);
            fprintf (stderr, "token array[%d].value   = " BLUE "%zu" RESET "\n", i, token_array[i].value.id);
        }

        else if (token_array[i].type == OP)
        {
            fprintf (stderr, "token array[%d].type    = " BLUE "OP" RESET "\n", i);
            fprintf (stderr, "token array[%d].value   = " BLUE "%d" RESET "\n", i, token_array[i].value.val_op);
        }

        else if (token_array[i].type == FILLER)
        {
            fprintf (stderr, "token array[%d].type    = " BLUE "FILLER" RESET "\n", i);
            fprintf (stderr, "token array[%d].value   = " BLUE "%s" RESET "\n", i, token_array[i].value.filler);
        }

        fprintf (stderr, "token array[%d].right   = " BLUE "%p" RESET "\n", i, token_array[i].right);
        fprintf (stderr, "token array[%d].left    = " BLUE "%p" RESET "\n\n", i, token_array[i].left);
    }
}
//==================================================================================================
void DumpNameTable (Language* language)
{
    assert (language);
    assert (language->name_table);

    NAME_TABLE* name_table = language->name_table;
    size_t name_table_number = language->name_table_number;

    fprintf (stderr, GREEN_BGD "N A M E   T A B L E"  RESET "\n");
    for (size_t i = 0; i < name_table_number; ++i)
    {
        fprintf (stderr, "name_table[%zu] addr = " BLUE "%p" RESET "\n", i, &name_table[i]);
        fprintf (stderr, "name table[%zu] ID   = " BLUE "%s" RESET "\n", i, name_table[i].name_id);
    }
}
//==================================================================================================
void LanguageDump (Language* language)
{
    DumpTokenArray (language);
    DumpNameTable  (language);
}
//==================================================================================================
void LanguageGraphDump (Language* language)
{
    static size_t images_counter = 0;

    language_graphviz_file = freopen (NAME_LANGUAGE_GRAPHVIZ_FILE, "w", language_graphviz_file);

    setvbuf      (language_log_file, NULL, _IONBF, 0);  // Отключение буферизации
    setvbuf (language_graphviz_file, NULL, _IONBF, 0);  // Отключение буферизации

    CreateFrontendDotContext (language);
    ConsoleCommandCallDot (NAME_TABLE_AND_TOKENS);

    if (images_counter == 1)
        PrintHtmlFrontendIntro ();


    LanguageAddImages ();
}
//==================================================================================================
void CreateFrontendDotContext (Language* language)
{
    fprintf (language_graphviz_file, ""
    "digraph\n"
    "{\n\t"
        "rankdir  = LR\n\t                          // Горизонтальная ориентация"
        "newrank  = true\n\t                        // Улучшенный алгоритм ранжирования"
        "compound = true\n\n\t                      // Разрешить сложные связи"

        "nodesep  = 2.0\n\t                         // Расстояние между узлами"
        "ranksep  = 1.0\n\n\t                       // Расстояние между уровнями"

        "graph [fontname=\"Helvetica\"]\n\n\t       // Шрифт для всего графа"

        "bgcolor   = \"#f5e6cc\"\n\t                // Цвет фона - светло-синий"
        "fontcolor = \"black\"\n\n\t                // Цвет текста"

        "node [shape = Mrecord, style = filled, color = black];\n\t"
        "edge [color = black];\n\n\t");

        fprintf (language_graphviz_file, ""
        "subgraph cluster_Name_table\n\t"
        "{\n\t\t"
            "fontsize = 23\n\t\t"
            "label = \"Name Table\"\n\t\t"
            "node [shape = Mrecord, fillcolor = \"#a64ac9\"];\n\t\t"
            "color = \"#f5e6cc\";\n\n\t\t");

            size_t name_table_number = language->name_table_number;
            for (size_t i = 0; i < name_table_number; ++i)
                fprintf (language_graphviz_file, ""
            "subgraph cluster_%zu\n\t\t"
            "{\n\t\t\t"
                "elem_%zu[label = \"addr = %p | ID = \"%s\"];\n\t\t\t"
                "label = \"%zu\";\n\t\t\t"
                "fontsize = 16\n\t\t"
            "}\n\n\t\t", i, i, &language->name_table[i], language->name_table[i].name_id, i);

            for (size_t i = 0; i < name_table_number - 1; ++i)
                fprintf (language_graphviz_file, ""
            "elem_%zu -> elem_%zu", i, i + 1);

        fprintf (language_graphviz_file, ""
        "}\n\n\t");

        fprintf (language_graphviz_file, ""
        "subgraph cluster_Token_array\n\t"
        "{\n\t\t"
            "fontsize = 23\n\t\t"
            "label = \"Token array\"\n\t\t"
            "color = \"#f5e6cc\"\n\n\t\t");

            size_t token_number = language->token_number;
            for (size_t i = 0; i < token_number; ++i)

            fprintf (language_graphviz_file, ""
            "subgraph cluster_token_%zu\n\t\t"
            "{\n\t\t\t"
                "node [fillcolor = \"%s\"]\n\n\t\t\t" // GetColor

                "elem__token_%zu [label = \"addr: %p | node type = %s | value = %s | node left: %p | node right: %p \"]\n\t\t\t"
                "label = \"%zu\"\n\t\t\t"
                "fontsize = 16\n\t\t"
            "}\n\n\t\t", i, GetColor (language->token_array[i].type), i, &language->token_array[i], GetTypeNode(language->token_array[i].type), GetValue(&language->token_array[i]), language->token_array[i].left, language->token_array[i].right, i);

        for (size_t i = 0; i < token_number - 1; ++i)
            fprintf (language_graphviz_file, ""
            "elem_%zu -> elem_%zu\n\t\t", i, i + 1);

        fprintf (language_graphviz_file, "\n\t"
        "}\n"
    "}");
}
//==================================================================================================
const char* GetColor (type_t type)
{
    switch (type)
    {
        case NUM:
            return NUM_COLOR;

        case ID:
            return ID_COLOR;

        case OP:
            return OP_COLOR;

        case FILLER:
            return FILLER_COLOR;

        default:
            fprintf (stderr, RED "ERROR in %s:%d: type is undefined" RESET "\n", __FILE__, __LINE__);
    }

    return 0;
}
//==================================================================================================
const char* GetTypeNode (type_t type)
{
    switch (type)
    {
        case NUM:
            return "NUM";

        case ID:
            return "ID";

        case OP:
            return "OP";

        case FILLER:
            return "FILLER";

        default:
            fprintf (stderr, RED "ERROR in %s:%d: type is undefined" RESET "\n", __FILE__, __LINE__);
    }

    return 0;
}
//==================================================================================================
const char* GetValue (node* node)
{
    char* buffer = (char*) calloc (128, sizeof (char));

    type_t type = node->type;
    switch (type)
    {
        case NUM:
        {
            sprintf (buffer, "%lf", node->value.val_num);
            return buffer;
        }

        case ID:
            sprintf (buffer, "%zu", node->value.id);
            return buffer;

        case OP:
            sprintf (buffer, "%d (\"%s\")", node->value.val_op, GetOP (node->value.val_op));
            return buffer;

        case FILLER:
        sprintf (buffer, "\"%s\"", node->value.filler);
            return buffer;

        default:
            fprintf (stderr, RED "ERROR in %s:%d: type is undefined" RESET "\n", __FILE__, __LINE__);
    }

    free (buffer);

    return 0;
}
//==================================================================================================
const char* GetOP (operations op_code)
{

    switch (op_code)
    {
        case BEGINING:
            return "Breakfast";

        case OPENING_CURLY_BRACKET:
            return "want-millpops";

        case CLOSING_CURLY_BRACKET:
            return "dirty";

        case OPENING_BRACKET:
            return "(";

        case CLOSING_BRACKET:
            return ")";

        case DECLARATION_ID:
            return "bombardiro-krokodilo";

        case EQUALS:
            return "yes-yes";

        case DECLARATION_FUNCTION:
            return "is-the-base";

        case IF:
            return "double-yummy";

        case WHILE:
            return "golubino-shpioniro";

        case SIN:
            return "sin";

        case COS:
            return "cos";

        case ENDING:
            return "DIYA-kontora...";

        case ADDITTION:
            return "+vibe";

        case SUBTRACTION:
            return "-vibe";

        case DIVISION:
            return "/";

        case MULTIPLICATION:
            return "*";

        case ELEVATION:
            return "^";

        case SEPARATOR:
            return "skip";

        case EQUAL_COMPARE:
            return "==";

        case LESS:
            return "<";

        case LESS_OR_EQUALE:
            return "<=";

        case MORE:
            return ">";

        case MORE_OR_EQUAL:
            return ">=";

        case BEGIN_PARAM_FUNC:
            return "";

        case END_PARAM_FUNC:
            return "";

        case COMMENTS:
            return "|skip:";
    }

    return 0;
}
//==================================================================================================
ERRORS ConsoleCommandCallDot (PRINT_OBJ object)
{
    char* buffer = 0;

    if (object == NAME_TABLE_AND_TOKENS)
    {
        size_t length = snprintf (NULL, 0, "mkdir -p dump/Graphviz_dot/images/frame_%zu; dot -Tsvg dump/Graphviz_dot/Frontend_graphviz.txt -o dump/Graphviz_dot/images/frame_%zu/img_context_%zu.svg", images_counter, images_counter, images_counter);
        buffer = (char*) calloc (length + 1, sizeof (char));
        snprintf (buffer, length + 1,      "mkdir -p dump/Graphviz_dot/images/frame_%zu; dot -Tsvg dump/Graphviz_dot/Frontend_graphviz.txt -o dump/Graphviz_dot/images/frame_%zu/img_context_%zu.svg", images_counter, images_counter, images_counter);
    }

    else
    {
        size_t length = snprintf (NULL, 0, "mkdir -p dump/Graphviz_dot/images/frame_%zu; dot -Tsvg dump/Graphviz_dot/Frontend_graphviz.txt -o dump/Graphviz_dot/images/frame_%zu/img_tree_%zu.svg", images_counter, images_counter, images_counter);
        buffer = (char*) calloc (length + 1, sizeof (char));
        snprintf (buffer, length + 1,      "mkdir -p dump/Graphviz_dot/images/frame_%zu; dot -Tsvg dump/Graphviz_dot/Frontend_graphviz.txt -o dump/Graphviz_dot/images/frame_%zu/img_tree_%zu.svg", images_counter, images_counter, images_counter);
    }

    int result = system (buffer); // +1 для '\0'
    if (result != 0)
    {
        fprintf (stderr, RED "ERROR in %s: %d line: system (): code error = %d\n" RESET, __FILE__, __LINE__, result);
        fflush (stdout);
    }

    free (buffer);

    return NO_ERRORS;
}
//==================================================================================================
ERRORS PrintHtmlFrontendIntro ()
{
    fprintf (language_log_file, ""
    "<pre>\n\t"
	    "<font size = 20, color = red>\n\t\t"
            "<div style = \"text-align: center;\">\n"
                "DUMP\n\t\t"
            "</div>\n\t"
	    "</font>\n"
    "</pre>\n\n"

    "<style>\n"
    ".center-horizontally\n"
    "{\n\t"
        "display: block;\n\t"
        "margin-left: auto;\n\t"
        "margin-right: auto;\n\t"
        "width: 30%%;\n\t"
        "height: auto;\n"
    "}\n"
    "</style>\n\n"
    );

    return NO_ERRORS;
}//==================================================================================================
ERRORS LanguageAddImages ()
{
    fprintf (language_log_file, "\n"
    "<img src = \"../dump/Graphviz_dot/images/frame_%zu/img_context_%zu.svg\" class = \"center-horizontally\"><br><br>\n", images_counter, images_counter);

    fprintf (language_log_file, "\n"
    "<img src = \"../dump/Graphviz_dot/images/frame_%zu/img_tree_%zu.svg\" class = \"center-horizontally\"><br><br>\n",    images_counter, images_counter);

    return NO_ERRORS;
}
//==================================================================================================
