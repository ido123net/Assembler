#include "error.h"

void line_error(size_t row, int error)
{
    fprintf(stderr, "Line %d: %s\n", row, symbolstrerror(error));
}

void symbol_error(size_t row, char *symbol, int error)
{
    fprintf(stderr, "Line %d: The Symbol - \"%s\" is %s\n", row, symbol, symbolstrerror(error));
}

void directive_error(size_t row, char *directive, int error)
{
    fprintf(stderr, "Line %d: The directive - \"%s\" %s\n", row, directive, directivestrerror(error));
}

char *symbolstrerror(int error)
{
    switch (error)
    {
    case INVALID_SYMBOL:
        return "invalid";

    case SAVED_WORD:
        return "saved word";

    case SYMBOL_TOO_LONG:
        return "too long";
    
    case LABEL_DECLARED:
        return "already been declared";

    default:
        return "";
    }
}

char *directivestrerror(int error)
{
    switch (error)
    {
    case INVALID_DIRECTIVE:
        return "is invalid";

    case EXTRANEOUS_OPERAND:
        return "has extraneous oprand";

    case MISSING_OPERAND:
        return "has missing operand";

    default:
        return "";
    }
}
