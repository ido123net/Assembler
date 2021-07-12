#include "first_pass.h"
#include "data_structure.h"

int main(int argc, char const *argv[])
{
    Image _data_image, _code_image;
    SymbolTable _symbol_table;
    int ICF = 0, DCF = 0;
    _data_image = initImage();
    _code_image = initImage();
    _symbol_table = initSymbolTable();
    first_pass("test.as", _data_image, _code_image, _symbol_table, &ICF, &DCF);
    return 0;
}
