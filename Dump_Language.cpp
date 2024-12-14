#define TX_COMPILED
#include "..\SuperLibs\TXLib.h"

#include "Common_Language.h"
#include "Init_language.h"
#include "Dump_Language.h"

extern FILE* Log_File;
extern FILE* Graph_File;
extern FILE* Graph_File_Utf8;

static void Dump_name_table  (NAME_TABLE* name_table);
static void Dump_table_token (node* node_);

//==================================================================================================
int Dump_graphviz_language (void* ptr, GRAPH_PRINT object_print)
{
    if (!ptr) return 1;

    Graph_File      = Create_file ("build_language/Dump_lang/Dot.txt");
    Graph_File_Utf8 = Create_file ("build_language/Dump_lang/Dot_UTF-8.txt");

    static size_t number_pic = 1;
    
    char* name_cmd = (char*) calloc (256, sizeof (char));
    char* name_pic = (char*) calloc (256, sizeof (char));

    snprintf (name_cmd, 256, "dot build_language/Dump_lang/Dot_UTF-8.txt -Tsvg -o build_language/Dump_lang/dump_picture_%zu.svg", number_pic);
    snprintf (name_pic, 256, "dump_picture_%zu.svg", number_pic);

    switch (object_print)  
    {
        case _NAME_TABLE: 
        {
            NAME_TABLE* name_table = (NAME_TABLE*) ptr;
            fprintf (Log_File, "<b><fontsize = #FF0000>NAME TABLE</fontsize></b>");
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
        default: fprintf (stderr, "ERROR IN: %s: %d", __FILE__, __LINE__);
    }

    Close_File (Graph_File);
    Close_File (Graph_File_Utf8);


    system ("D:\\����������\\iconv\\gettext-iconv\\bin\\iconv.exe -f CP1251 -t UTF-8 build_language/Dump_lang/Dot.txt > build_language/Dump_lang/Dot_UTF-8.txt");
    system (name_cmd);

    fprintf (Log_File ,"\n<img src = \"build_language/Dump_lang/%s\" width = %d%%>\n\n\n\n\n\n\n\n\n\n", name_pic, SCALE);
    fflush (Graph_File);
    ++number_pic;   
    
    free (name_cmd);
    free (name_pic);

    return 0;
}
//==================================================================================================
static void Dump_table_token (node* node_)
{
    if (!node_) return;
      (Graph_File, "digraph\n" 
    "{\n"
    "\trankdir = LR;\n"
    "\tnode[color = \"#d69950\", shape = \"Mrecord\", style = \"filled\" ,fillcolor=\"#ddcdba\"];\n"
    "\tedge[color = \"#364042\"];\n");                                                        
        
    for (int i = 0;  node_[i].type != 0; i++)
    {
        if (node_[i].type == NUM)
        fprintf (Graph_File,  "" 
    "\tnode_%p  [label= \" Type:\\n %s |  Value:\\n  %" TYPE " | { Left:\\n %p| Right:\\n %p} \"];\n", node_ + i, "NUM", node_[i].value.val_num, node_[i].left, node_[i].right);

        if (node_ [i].type == OP)
        fprintf (Graph_File,  "" 
    "\tnode_%p  [label= \" Type:\\n %s |  Value:\\n  %d | { Left:\\n %p| Right:\\n %p} \"];\n", node_ + i, "OP",  node_[i].value.val_op, node_[i].left, node_[i].right);

        if (node_[i].type == ID)
        fprintf (Graph_File,  "" 
    "\tnode_%p  [label= \" Type:\\n %s |  Value:\\n  %d | { Left:\\n %p| Right:\\n %p} \"];\n", node_ + i, "ID",  node_[i].value.id, node_[i].left, node_[i].right);
    
    fprintf (Graph_File,  "" 
            "\tsubgraph cluster_%p {node_%p; label = \"Addr: %p\"; color = \"white\";}\n\n", node_ + i, node_ + i, node_ + i);
   //=============================================================================================================================================
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
    "\tnode_%p  [label= \" Type:\\n %s |  Value:\\n   %d  | { Left:\\n %p| Right:\\n %p} \"];\n", node_ + i + 1, "OP",  node_[i+1].value.val_op, node_[i+1].left, node_[i+1].right);

        if (node_[i+1].type == ID)
        fprintf (Graph_File,  "" 
    "\tnode_%p  [label= \" Type:\\n %s |  Value:\\n  %d | { Left:\\n %p| Right:\\n %p} \"];\n", node_ + i + 1, "ID",  node_[i+1].value.id, node_[i+1].left, node_[i+1].right);
    
    fprintf (Graph_File,  "" 
            "\tsubgraph cluster_%p {node_%p; label = \"Addr: %p\"; color = \"white\";}\n\n", node_ + i + 1, node_ + i + 1, node_ + i + 1);

    fprintf (Graph_File, "\tnode_%p -> node_%p;\n\n", node_ + i, node_ + i + 1);

    }

    fprintf (Graph_File, "}");

}
//==================================================================================================
static void Dump_name_table (NAME_TABLE* name_table)
{
    if (!name_table) return;
    size_t counter = 0;

    fprintf (Graph_File, "digraph\n" 
    "{\n"
    "\trankdir = LR;\n"
    "\tnode[color = \"#d69950\", shape = \"Mrecord\", style = \"filled\" ,fillcolor=\"#5c6d93\"];\n"
    "\tedge[color = \"#d18166\"];\n");                                                        
        
    for (int i = 0; strcmp (name_table[i].name_id, "\0"); i++)
    {
        fprintf (Graph_File,  "" 
    "\tnode_%p  [label= \" Number:\\n %zu |  Value:\\n %s \"];\n", name_table + i, counter , name_table[i].name_id);

    fprintf (Graph_File,  "" 
            "\tsubgraph cluster_%p {name_table%p; label = \"Addr: %p\"; color = \"white\";}\n\n", name_table + i, name_table + i, name_table + i);

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
    "\tnode_%p  [label= \" Number:\\n %zu |  Value:\\n %s \"];\n", name_table + i + 1, counter , name_table[i+1].name_id);

    fprintf (Graph_File,  "" 
            "\tsubgraph cluster_%p {name_table%p; label = \"Addr: %p\"; color = \"white\";}\n\n", name_table + i + 1, name_table + i + 1, name_table + i + 1);
    
    fprintf (Graph_File, "\tnode_%p -> node_%p;\n\n", name_table + i, name_table + i + 1);

    counter++;

    }

    fprintf (Graph_File, "}");
}
