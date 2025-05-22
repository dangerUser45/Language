#include <stdio.h>
#include <string.h>

#include <Color.h>
#include <Common.h>

#include <LanguageFrontend.h>
#include <LanguageFrontendDump.h>

extern FILE* language_log_file;
extern FILE* language_graphviz_file;

static size_t images_counter = 0;

static void DumpTokenArray (Language* language);
static void DumpNameTable  (Language* language);

void CreateFrontendDotContext (Language* language);

static const char* GetColor (type_t type);
static const char* GetTypeNode (type_t type);
static const char* GetOP (operations op_code);
static const char* GetNameTableID (type_id type_id);

static ERRORS ConsoleCommandCallDot (PRINT_OBJ object);
static ERRORS PrintHtmlFrontendIntro ();
static ERRORS LanguageAddImages (PRINT_OBJ object);

static void PrintNameTable (Language* language);
static void PrintFrontendIntro (PRINT_OBJ object);
static void PrintTokenArray (Language* language);
static void PrintOneTokenContext (Language* language, node* node, size_t token_number);
static void PrintOneTokenTree (Language* language, node* node);

static void PrintAST_Tree (Language* language);
static void PrintOneNode (Language* language, node* node);

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

    if (images_counter == 0)
        PrintHtmlFrontendIntro ();
    ++images_counter;

    language_graphviz_file = freopen (NAME_LANGUAGE_GRAPHVIZ_FILE, "w", language_graphviz_file);

    setvbuf      (language_log_file, NULL, _IONBF, 0);  // Отключение буферизации
    setvbuf (language_graphviz_file, NULL, _IONBF, 0);  // Отключение буферизации

    CreateFrontendDotContext (language);
    ConsoleCommandCallDot (NAME_TABLE_AND_TOKENS);
    LanguageAddImages (NAME_TABLE_AND_TOKENS);

    if (language->parent_node != NULL )
    {
        z(language->parent_node, p)
        language_graphviz_file = freopen (NAME_LANGUAGE_GRAPHVIZ_FILE, "w", language_graphviz_file);
        setvbuf      (language_log_file, NULL, _IONBF, 0);  // Отключение буферизации
        setvbuf (language_graphviz_file, NULL, _IONBF, 0);  // Отключение буферизации

        zz
        PrintAST_Tree (language); zz
        ConsoleCommandCallDot (TREE); zz
        LanguageAddImages (TREE); zz
    }
}
//==================================================================================================
void CreateFrontendDotContext (Language* language)
{
    PrintFrontendIntro (NAME_TABLE_AND_TOKENS);

    PrintTokenArray (language);
    PrintNameTable (language);

    fprintf (language_graphviz_file, "}");
}
//==================================================================================================
void PrintFrontendIntro (PRINT_OBJ object)
{
    const char* rankdir = 0;
    if (object == TREE)
        rankdir = "TB";
    else
        rankdir = "LR";

    fprintf (language_graphviz_file, ""
    "digraph\n"
    "{\n\t"
        "rankdir  = %s                        // Горизонтальная ориентация\n\t"
        "newrank  = true                      // Улучшенный алгоритм ранжирования\n\t"
        "compound = true                      // Разрешить сложные связи\n\n\t"

        "nodesep  = 1.0                       // Расстояние между узлами\n\t"
        "ranksep  = 0.5                       // Расстояние между уровнями\n\n\t"

        "graph [fontname=\"Helvetica\"]       // Шрифт для всего графа\n\n\t"

        "bgcolor   = \"#f5e6cc\"              // Цвет фона - светло-синий\n\t"
        "fontcolor = \"black\"                // Цвет текста\n\n\t"

        "node [shape = Mrecord, style = filled, color = black];\n\t"
        "edge [color = black];\n\n\t", rankdir);
}
//==================================================================================================
void PrintNameTable (Language* language)
{
    size_t name_table_number = language->name_table_number;

    if (name_table_number != 0)
    {
        fprintf (language_graphviz_file, "\n\t"
        "subgraph cluster_Name_table\n\t"
        "{\n\t\t"
            "fontsize = 23\n\t\t"
            "label = \"Name Table\"\n\t\t"
            "node [shape = Mrecord, fillcolor = \"#a64ac9\"];\n\t\t"
            "color = \"#f5e6cc\";\n\n\t\t");

            for (size_t i = 0; i < name_table_number; ++i)
                fprintf (language_graphviz_file, ""
            "subgraph cluster_%zu\n\t\t"
            "{\n\t\t\t"
                "elem_%zu[label = \"addr = %p | ID = \\\"%s\\\" | type = %s \"];\n\t\t\t"
                "label = \"%zu\";\n\t\t\t"
                "fontsize = 16\n\t\t"
            "}\n\n\t\t", i, i, &language->name_table[i], language->name_table[i].name_id, GetNameTableID (language->name_table[i].type), i);

            for (size_t i = 0; i < name_table_number - 1; ++i)
                fprintf (language_graphviz_file, ""
            "elem_%zu -> elem_%zu\n\t\t", i, i + 1);

        fprintf (language_graphviz_file, "\n\t"
        "}\n");
    }
}
//==================================================================================================
void PrintTokenArray (Language* language)
{
    size_t token_number = language->token_number;

    if (token_number != 0)
    {
        fprintf (language_graphviz_file, ""
        "subgraph cluster_Token_array\n\t"
        "{\n\t\t"
            "fontsize = 23\n\t\t"
            "label = \"Token array\"\n\t\t"
            "color = \"#f5e6cc\"\n\n\t\t");

            for (size_t i = 0; i < token_number; ++i)
                PrintOneTokenContext (language, &language->token_array[i],  i);

        for (size_t i = 0; i < token_number - 1; ++i)
            fprintf (language_graphviz_file, ""
            "elem_token_%zu -> elem_token_%zu\n\t\t", i, i + 1);

        fprintf (language_graphviz_file, "\n\t"
        "}\n");
    }
}
//==================================================================================================
void PrintOneTokenContext (Language* language, node* node, size_t token_number)
{
    fprintf (language_graphviz_file, ""
    "subgraph cluster_token_%zu\n\t\t"
    "{\n\t\t\t"
        "node [fillcolor = \"%s\"]\n\n\t\t\t" // GetColor

        "elem_token_%zu [label = \" addr: %p | node type = %s | value = %s | node left: %p | node right: %p \"]\n\t\t\t"
        "label = \"%zu\"\n\t\t\t"
        "fontsize = 16\n\t\t"
    "}\n\n\t\t", token_number,
                GetColor (node->type),
                token_number,
                node,
                GetTypeNode(node->type),
                GetValue(language, node),
                node->left,
                node->right,
                token_number);
}
//==================================================================================================
void PrintOneTokenTree (Language* language, node* node)
{
    if (node == NULL)
        return;

    fprintf (language_graphviz_file, ""
    "subgraph cluster_token_%zu\n\t\t"
    "{\n\t\t\t"
        "node [fillcolor = \"%s\"]\n\n\t\t\t" // GetColor
        "label = \"\"\n\t\t\t"
        "elem_token_%zu [label = \" { addr: %p | node type = %s | value = %s | node left: %p | node right: %p } \"]\n\t\t\t"
        "fontsize = 16\n\t\t"
    "}\n\n\t\t", (size_t)node,
                GetColor (node->type),
                (size_t)node,
                node,
                GetTypeNode(node->type),
                GetValue(language, node),
                node->left,
                node->right);
}
//==================================================================================================
void PrintAST_Tree (Language* language)
{
    assert (language);

    node* parent_node = language->parent_node;

    if (parent_node->left  != NULL ||
        parent_node->right != NULL)
    {
        PrintFrontendIntro (TREE);

        fprintf (language_graphviz_file, ""
        "subgraph cluster_AST_Tree\n\t"
	    "{\n\t\t"
            "fontsize = 23\n\t\t"
            "label = \"AST Tree\"\n\t\t"
            "color = \"#f5e6cc\"\n\n\t\t");

        PrintOneTokenTree (language, parent_node);
        PrintOneNode (language, parent_node);

        fprintf (language_graphviz_file,""
        "\n\t}\n"
        "}\n");
    }
}
//==================================================================================================
void PrintOneNode (Language* language, node* node)
{
    if (node == NULL)
        return;

    if (node->left != NULL)
    {
        PrintOneTokenTree (language, node->left);
        fprintf (language_graphviz_file, ""
        "elem_token_%zu -> elem_token_%zu\n\t\t",
        (size_t)node, (size_t)node->left );
    }

    if (node->right != NULL)
    {
        PrintOneTokenTree (language, node->right);
        fprintf (language_graphviz_file, ""
        "elem_token_%zu -> elem_token_%zu\n\t\t",
        (size_t)node, (size_t)node->right);
    }

    PrintOneNode (language,node->left);
    PrintOneNode (language, node->right);
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
const char* GetValue (Language* language, node* node)
{
    char* buffer = (char*) calloc (512, sizeof (char));

    type_t type = node->type;
    switch (type)
    {
        case NUM:
        {
            sprintf (buffer, "%lf", node->value.val_num);
            return buffer;
        }

        case ID:
            sprintf (buffer, "%zu (\\\"%s\\\")", node->value.id, language->name_table[node->value.id].name_id);
            return buffer;

        case OP:
            sprintf (buffer, "%d (\\\"%s\\\")", node->value.val_op, GetOP (node->value.val_op));
            return buffer;

        case FILLER:
        sprintf (buffer, "\\\"%s\\\"", node->value.filler);
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

        case SEPARATOR_PARAM:
            return "skip";

        case SEPARATOR_IN_DECLARE_1:
            return "norm";

        case SEPARATOR_IN_DECLARE_2:
            return "cringe";

        case EQUAL_COMPARE:
            return "==";

        case NOT_EQUALE_COMPARE:
            return "!=";

        case LESS:
            return "\\<";

        case LESS_OR_EQUALE:
            return "\\<=";

        case MORE:
            return "\\>";

        case MORE_OR_EQUAL:
            return "\\>=";

        case COMMENTS:
            return "|skip:";

        case PARAM_ENVIRONMENT:
            return "_";

        case RETURN:
            return "tralalelo-tralala";

        default:
            return 0;
    }

    return 0;
}
//==================================================================================================
const char* GetNameTableID (type_id type_id)
{
    switch (type_id)
    {
        case GLOBAL:
            return "GLOBAL";

        case LOCAL:
            return "LOCAL";

        case FUNCTION:
            return "FUNCTION";

        case PARAM:
            return "PARAM";

        default:
            return 0;
    }
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
ERRORS LanguageAddImages (PRINT_OBJ object)
{
    if (object == NAME_TABLE_AND_TOKENS)
        fprintf (language_log_file, "\n"
        "<img src = \"../dump/Graphviz_dot/images/frame_%zu/img_context_%zu.svg\" style = \"width: 200%%;\" class = \"center-horizontally\"><br><br>\n", images_counter, images_counter);

    else
        fprintf (language_log_file, ""
        "<img src = \"../dump/Graphviz_dot/images/frame_%zu/img_tree_%zu.svg\" style = \"width: 50%%;\" class = \"center-horizontally\"><br><br>\n\n",    images_counter, images_counter);

    return NO_ERRORS;
}
//==================================================================================================
