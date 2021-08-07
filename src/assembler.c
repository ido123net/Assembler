#include "first_pass.h"
#include "second_pass.h"
#include "data_structure.h"
#include "output.h"
#include "parser.h"

int main(int argc, char const *argv[])
{
    Image _data_image, _code_image;
    SymbolTable _symbol_table;
    ExternalLines _externalLines;
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
        _data_image = initImage();
        _code_image = initImage();
        _symbol_table = initSymbolTable();
        _externalLines = initExternalLines();
        if (first_pass(*argv, _data_image, _code_image, _symbol_table, &ICF, &DCF))
            if (second_pass(_code_image, _symbol_table, _externalLines, &ICF, &DCF))
                output_files(filename, _code_image, _data_image, _symbol_table, _externalLines, ICF, DCF);
    }
    
    return 0;
}
