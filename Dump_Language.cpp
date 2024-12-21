#include "Common_Language.h"
#include "Init_language.h"
#include "Dump_Language.h"
#include "SyntaxError.h"

extern FILE* Log_File;
extern FILE* Graph_File;
extern FILE* Graph_File_Utf8;

static void Dump_name_table  (NAME_TABLE* name_table);
static void Dump_table_token (node* node_);
static void Dump_graph_init (node* node_);
static void Dump_graph_recursive (node* Node, size_t rank);
static void Draw_tree (node* Node);
static bool Check_Name_Table (void* ptr);
static const char* GetOpName (node* Node);

//==================================================================================================
int Dump_graphviz_language (void* ptr, GRAPH_PRINT object_print)
{
    if (!ptr) return 1;

    if (Check_Name_Table (ptr));
    else return 0;
                 
    static size_t number_pic = 1;

    char* name_dot_utf_8 = (char*) calloc (256, sizeof (char));
    char* name_cmd       = (char*) calloc (256, sizeof (char));
    char* name_pic       = (char*) calloc (256, sizeof (char));
    char* name_cmd_dot   = (char*) calloc (256, sizeof (char));

    snprintf (name_dot_utf_8, 256, "build_language/Dump_lang/Dot_UTF-8_%zu.txt", number_pic);
    snprintf (name_cmd,       256, "dot build_language/Dump_lang/Dot_UTF-8_%zu.txt -Tsvg -o build_language/Dump_lang/dump_picture_%zu.svg", number_pic, number_pic);
    snprintf (name_pic,       256, "dump_picture_%zu.svg", number_pic);
    snprintf (name_cmd_dot,   256, "D:\\SOFT\\iconv\\gettext-iconv\\bin\\iconv.exe -f CP1251 -t UTF-8 build_language/Dump_lang/Dot.txt > %s", name_dot_utf_8);

    Graph_File      = Create_file ("build_language/Dump_lang/Dot.txt");
    Graph_File_Utf8 = Create_file (name_dot_utf_8);

    switch (object_print)
    {
        case _NAME_TABLE:
        {
            NAME_TABLE* name_table = (NAME_TABLE*) ptr;
            fprintf (Log_File, "<b><fontsize =rgb(255, 0, 0)>\t\t\t\t\t\t\tNAME TABLE</fontsize></b>");

            Dump_name_table (name_table);
            break;
        }
        case TOKEN_TABLE:
        {
            node* Node = (node*) ptr;
            fprintf (Log_File, "<b><font color = #FF0000>\t\t\t\t\t\t\tTOKEN TABLE</fontsize></b>");
            Dump_table_token (Node);
            break;
        }
        case TREE:
        {
                    DBG(printf ("I in Dump() case: TREE ptr = %p\n", (node*)ptr);)
            fprintf (Log_File, "<b><font color = #FF0000>\t\t\t\t\t\t\tTREE</fontsize></b>");
            node* tree = (node*) ptr;
            Dump_graph_init (tree);
            break;
        }

        default: fprintf (stderr, "ERROR IN: %s: %d\n", __FILE__, __LINE__);
    }

    Close_File (Graph_File);
    Close_File (Graph_File_Utf8);

    system (name_cmd_dot);
    system (name_cmd);

    fprintf (Log_File ,"\n<img src = \"build_language/Dump_lang/%s\" width = %d%%>\n\n\n\n\n\n\n\n\n\n", name_pic, SCALE);
    fflush (Graph_File);
    ++number_pic;   
    
    free (name_dot_utf_8);
    free (name_cmd);
    free (name_pic);
    free (name_cmd_dot);

    return 0;
}
//==================================================================================================
static void Dump_table_token (node* node_)
{
    if (!node_) return;
    static size_t num_el = 1;
      fprintf(Graph_File, "digraph\n" 
    "{\n"
    "\trankdir = LR;\n"
    "\tnode[color = \"#d69950\", shape = \"Mrecord\", style = \"filled\" ,fillcolor=\"#ddcdba\"];\n"
    "\tedge[color = \"#000000\"];\n\n");                                                        
        
    for (int i = 0;  node_[i].type != 0; i++)
    {
        if (node_[i].type == NUM)
        fprintf (Graph_File,  "" 
    "\tnode_%p  [label= \" Type:\\n %s |  Value:\\n  %" TYPE " | { Left:\\n %p| Right:\\n %p} \"];\n", node_ + i, "NUM", node_[i].value.val_num, node_[i].left, node_[i].right);

        if (node_ [i].type == OP)
        fprintf (Graph_File,  "" 
    "\tnode_%p  [label= \" Type:\\n %s |  Value:\\n  %s | { Left:\\n %p| Right:\\n %p} \"];\n", node_ + i, "OP",  GetOpName(node_ + i), node_[i].left, node_[i].right);

        if (node_[i].type == ID)
        fprintf (Graph_File,  "" 
    "\tnode_%p  [label= \" Type:\\n %s |  Value:\\n  %zu | { Left:\\n %p| Right:\\n %p} \"];\n", node_ + i, "ID",  node_[i].value.id, node_[i].left, node_[i].right);
    
    fprintf (Graph_File,  "" 
            "\tsubgraph cluster_%p {node_%p; label = \"Number: %zu\\n Addr: %p\"; color = \"white\";}\n\n", node_ + i, node_ + i, num_el, node_ + i);
    num_el++;
//~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~
    if (node_[i+1].type == 0)
    {
        fprintf(Graph_File, "\n"
        "\t{\n" 
            "\t\tnode [ color = \"#007cff\", shape = \"rectangle\", style = \"filled\", fillcolor = \"#a2f0f8\"];\n"
            "\t\tedge [ color = \"#007cff\", fontsize = 16];\n\n"

            "\t\tnode_l_null_%p [shape = \"ellipse\", label = \" null\" ];\n", node_ + i + 1);
        
        fprintf(Graph_File, ""
            "\t\tnode_%p  -> node_l_null_%p;\n", node_ + i, node_ + i + 1);
        fprintf(Graph_File, ""
            "\t}\n}");

        return;
    }
    if (node_[i+1].type == NUM)
        fprintf (Graph_File,  "" 
    "\tnode_%p  [label= \" Type:\\n %s |  Value:\\n  %" TYPE " | { Left:\\n %p| Right:\\n %p} \"];\n", node_ + i + 1, "NUM", node_[i+1].value.val_num, node_ [i+1].left, node_[i+1].right);

        if (node_ [i+1].type == OP)
        fprintf (Graph_File,  "" 
    "\tnode_%p  [label= \" Type:\\n %s |  Value:\\n   %s  | { Left:\\n %p| Right:\\n %p} \"];\n", node_ + i + 1, "OP",  GetOpName(node_ + i + 1), node_[i+1].left, node_[i+1].right);

        if (node_[i+1].type == ID)
        fprintf (Graph_File,  "" 
    "\tnode_%p  [label= \" Type:\\n %s |  Value:\\n  %zu | { Left:\\n %p| Right:\\n %p} \"];\n", node_ + i + 1, "ID",  node_[i+1].value.id, node_[i+1].left, node_[i+1].right);
    
    fprintf (Graph_File,  "" 
            "\tsubgraph cluster_%p {node_%p; label = \"Number: %zu\\n Addr: %p\"; color = \"white\";}\n\n", node_ + i + 1, node_ + i + 1, num_el, node_ + i + 1);

    fprintf (Graph_File, "\tnode_%p -> node_%p;\n\n\n", node_ + i, node_ + i + 1);

    }

    fprintf (Graph_File, "}");

    num_el++;
}
//==================================================================================================
static void Dump_name_table (NAME_TABLE* name_table)
{
    if (!name_table) return;
    size_t counter = 0;

    fprintf (Graph_File, "digraph\n" 
    "{\n"
    "\trankdir = LR;\n"
    "\tnode[color = \"#3c4966\", shape = \"Mrecord\", style = \"filled\" ,fillcolor=\"#8099d0\"];\n"
    "\tedge[color = \"#000000\"];\n\n");                                                        
        
    for (int i = 0; strcmp (name_table[i].name_id, "\0"); i++)
    {
        fprintf (Graph_File,  "" 
    "\tnode_%p  [label = \" Value:\\n \\\"%s\\\" \"];\n", name_table + i, name_table[i].name_id);

    fprintf (Graph_File,  ""
            "\tsubgraph cluster_%p {node_%p; label = \"Number: %zu \\n Addr: %p\"; color = \"white\";}\n\n", name_table + i, name_table + i, counter,name_table + i);

        counter++;
    //~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~ 
    if (!strcmp (name_table[i+1].name_id, "\0"))
    {
        fprintf(Graph_File, "\n"
        "\t{\n"
            "\t\tnode [ color = \"#007cff\", shape = \"rectangle\", style = \"filled\", fillcolor = \"#a2f0f8\"];\n"
            "\t\tedge [ color = \"#007cff\", fontsize = 16];\n\n"

            "\t\tnode_l_null_%p [shape = \"ellipse\", label = \" null\" ];\n", name_table + i + 1);
        
        fprintf(Graph_File, ""
            "\t\tnode_%p  -> node_l_null_%p;\n", name_table + i, name_table + i + 1);
        fprintf(Graph_File, ""
            "\t}\n}");

        return;
    }

    fprintf (Graph_File,  "" 
    "\tnode_%p  [label = \" Value:\\n \\\"%s\\\" \"];\n", name_table + i + 1, name_table[i+1].name_id);

    fprintf (Graph_File,  "" 
            "\tsubgraph cluster_%p {node_%p; label = \"Number: %zu \\nAddr: %p\"; color = \"white\";}\n\n", name_table + i + 1, name_table + i + 1, counter ,name_table + i + 1);
    
    fprintf (Graph_File, "\tnode_%p -> node_%p;\n\n\n", name_table + i, name_table + i + 1);
    }

    fprintf (Graph_File, "}");

    counter++;
}
//==================================================================================================
void Dump_graph_init (node* node_)
{
    fprintf (Graph_File, "digraph\n" 
    "{\n"
    "\trankdir = TB;\n\n");
    
    Dump_graph_recursive (node_, 1);

    fprintf (Graph_File, "}");

}
//==================================================================================================
void Dump_graph_recursive (node* Node, size_t rank)
{
    if (!Node) return;
    Draw_tree (Node);

    fprintf (Graph_File, ""
        "\t{\n"                                                        
            "\t\tnode[ color = \"#581845\", shape = \"circle\", style = \"filled\" ,fillcolor=\"#fe5656\"];\n"
            "\t\tedge[ color = \"white\"]\n"
            "\t\t\"%zu\" ->  \"%zu\";\n", rank, rank + 1);
    
    fprintf (Graph_File,  "\t}\n\n"
        "\t{ rank = %zu; \"%zu\"; \"node_%p\" }\n", rank, rank, Node);

    if (Node -> left != NULL)
    {
        Draw_tree (Node -> left);
        fprintf (Graph_File, ""   //!!!
        "\tnode_%p  -> node_%p [color = \"#ff0000\", fontsize = 16];\n", Node, Node -> left);
    }

    else {
        fprintf(Graph_File, "\n"
        "\t{\n"
            "\t\tnode [ color = \"#007cff\", shape = \"rectangle\", style = \"filled\", fillcolor = \"#a2f0f8\"];\n"
            "\t\tedge [ color = \"#007cff\", fontsize = 16];\n\n"

            "\t\tnode_l_null_%p [shape = \"ellipse\", label = \" null\" ];\n", Node);
        
        fprintf(Graph_File, ""
            "\t\tnode_%p  -> node_l_null_%p;\n", Node, Node);
        fprintf(Graph_File, ""
            "\t}\n");
        }  

    if (Node -> right != NULL)
    {
        Draw_tree (Node -> right);
        fprintf(Graph_File, "\n" //!!!
        "\tnode_%p  -> node_%p[color = \"#ff0000\", fontsize = 16];\n", Node, Node -> right);
    }

    else {
        fprintf(Graph_File, "\n"
        "\t{\n"
            "\t\tnode [ color = \"#007cff\", shape = \"rectangle\", style = \"filled\", fillcolor = \"#a2f0f8\"];\n"
            "\t\tedge [ color = \"#007cff\", fontsize = 16];\n\n"

            "\t\tnode_r_null_%p [shape = \"ellipse\", label = \" null\" ];\n", Node);
        
        fprintf(Graph_File, ""
            "\t\tnode_%p  -> node_r_null_%p;\n", Node, Node);
        fprintf(Graph_File, "" 
            "\t}\n");
          }

    fprintf (Graph_File, ""
    "\t{\n"                                                        
        "\t\tnode[ shape = \"plaintext\", style = \"filled\" ,fillcolor=\"white\"];\n"
        "\t\tedge[ color = \"white\"]\n"
        "\t\t\"%zu\" ->  \"%zu\";\n", rank, rank + 1);

    fprintf (Graph_File, ""
    "\t}\n");

        ++rank;

        Dump_graph_recursive (Node -> left, rank);
        Dump_graph_recursive (Node -> right, rank);

}
//==================================================================================================
void Draw_tree (node* Node)
{
     if (Node -> type == NUM){
            fprintf(Graph_File, ""
        "\tnode_%p [ color = \"#CCFFE6\", style = \"filled\", fillcolor = \"#a2f8a4\", shape = \"Mrecord\", label = \"{ addr: %p | val = %" TYPE "| type = %s | { L:\\n addr: %p | R: \\n addr: %p } }\" ];\n", Node, Node, Node -> value.val_num, "NUM", Node -> left, Node -> right);
        }

    else if (Node -> type == ID){
            fprintf(Graph_File, ""
        "\tnode_%p [ color = \"#CCCCFF\", style = \"filled\", fillcolor = \"#ff5fe0\", shape = \"Mrecord\", label = \"{ addr: %p | val = %zu | type = %s | { L:\\n addr: %p | R: \\n addr: %p } }\" ];\n",  Node, Node, Node -> value.id, "ID",      Node -> left, Node -> right);
        }

    else if (Node -> type == OP){
        fprintf(Graph_File, ""
        "\tnode_%p [ color = \"#FFC\",    style = \"filled\", fillcolor = \"#ecfd74\", shape = \"Mrecord\", label = \"{ addr: %p | val = %s  |  type = %s | { L:\\n addr: %p | R: \\n addr: %p } }\" ];\n", Node, Node, GetOpName(Node), "OP",  Node -> left, Node -> right);
        }
}
//=================================================================================================
bool Check_Name_Table (void* ptr)
{
    NAME_TABLE* ptr_casted = (NAME_TABLE*) ptr;
    if (ptr_casted[0].name_id[0] == '\0') return false;
    else return true;
}
//==================================================================================================
const char* GetOpName (node* Node)
{
    assert(Node);
    const char* text;
    switch (Node -> value.val_op)
    {
    case ADDITTION:
        text = "\\\"Add  :  '+' \\\""; return text;

    case SUBTRACTION:
        text = "\\\"Sub  :  '-' \\\""; return text;

    case DIVISION:
        text = "\\\"Div  :  '/' \\\""; return text;

    case MULTIPLICATION:
        text = "\\\"Mul  :  '*' \\\""; return text;
    
    case ELEVATION:
        text = "\\\" ELevat  :  '^' \\\""; return text;
    
    case COS: 
        text = "\\\"Cos\\\""; return text;
    
    case SIN:
        text = "\\\"Sin\\\""; return text;

    case LOG: 
        text = "\\\"Log\\\""; return text;

    case LN: 
        text = "\\\"Ln\\\""; return text;

    case EXP:
        text = "\\\"Exp\\\""; return text;

    case TAN:
        text = "\\\"Tan\\\""; return text;

    case IF:
        text = "\\\"If\\\""; return text;

    case WHILE:
        text = "\\\"While\\\""; return text;

    case EQUALS:
        text = "\\\"Equals  :  '=' \\\""; return text;

    case OPENING_CURLY_BRACKET:
        text = "\\\"Op_C_BRACK :  '{' \\\""; return text;

    case CLOSING_CURLY_BRACKET:
        text = "\\\"Cl cur bracket  :  '}' \\\""; return text;

    case OPENING_BRACKET:
        text = "\\\"Op bracket  :  '(' \\\""; return text;

    case CLOSING_BRACKET:
        text = "\\\"Cl bracket  :  ')' \\\""; return text;

    case DECLARATION_ID:
        text = "\\\"Declaration id\\\""; return text;

    case DECLARATION_FUNCTION:
        text = "\\\"Declaration func\\\""; return text;

    case BEGINING:
        text = "\\\"Begin \\\""; return text;

    case ENDING:
        text = "\\\"End\\\""; return text;

    case PRE_EQUAL:
        text = "\\\"Pre equal\\\""; return text;

    case IN_EQUAL:
        text = "\\\"In equal  :  '=' \\\""; return text;

    case SEPARATOR:
        text = "\\\"Separator :  ';' \\\""; return text;

    case EQUAL_COMPARE:
        text = "\\\"Eq cmpr  :  '==' \\\""; return text;

    case NOT_EQUALE_COMPARE:
        text = "\\\"Not eq cmpr :  '!=' \\\""; return text;
    case LESS:
        text = "\\\" Less  :  '\\<' \\\""; return text;

    case LESS_OR_EQUALE:
        text = "\\\" Less or eq :  '\\<=' \\\""; return text;

    case MORE:
        text = "\\\" More  :  '\\>' \\\""; return text;

    case MORE_OR_EQUAL:
        text = "\\\" More or eq  :  '\\>=' \\\""; return text;
     
    default: SYNTAX_ERROR
    }
}
