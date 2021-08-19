#include "first_pass.h"
#include "second_pass.h"
#include "data_structure.h"
#include "output.h"
#include "parser.h"
#include "linked_list.h"

int main(int argc, char const *argv[])
{
    LinkedList code_image, data_image, symbol_table, external_lines;
    char filename[MAX_LINE_LENGTH];
    int ICF, DCF;
    while (--argc > 0)
    {
        argv++;
        if (!valid_filename(*argv))
        {
            file_error(*argv);
            continue;
        }
        strcpy(filename, *argv);
        strtok(filename, ".");
        ICF = DCF = 0;
        code_image = init_list();
        data_image = init_list();
        symbol_table = init_list();
        external_lines = init_list();
        if (first_pass(*argv, data_image, code_image, symbol_table, &ICF, &DCF))
            if (second_pass(code_image, symbol_table, external_lines))
                output_files(filename, code_image, data_image, symbol_table, external_lines, ICF, DCF);
        free_list(code_image, free_code_line);
        free_list(data_image, free);
        free_list(symbol_table, free);
        free_list(external_lines, free);
    }
    
    return 0;
}
