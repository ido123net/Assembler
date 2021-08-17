#include "first_pass.h"
#include "second_pass.h"
#include "data_structure.h"
#include "output.h"
#include "parser.h"
#include "linked_list.h"

int main(int argc, char const *argv[])
{
    LinkedList data_image, code_image, symbol_table, external_lines;
    char filename[MAX_LINE_LENGTH];
    int ICF, DCF;
    while (--argc > 0)
    {
        argv++;
        if (!valid_filename(*argv))
        {
            /* TODO: error */
            continue;
        }
        strcpy(filename, *argv);
        strtok(filename, ".");
        ICF = DCF = 0;
        data_image = init_list();
        code_image = init_list();
        symbol_table = init_list();
        external_lines = init_list();
        if (first_pass(*argv, data_image, code_image, symbol_table, &ICF, &DCF))
            if (second_pass(code_image, symbol_table, external_lines, &ICF, &DCF))
                output_files(filename, code_image, data_image, symbol_table, external_lines, ICF, DCF);
    }
    
    return 0;
}
