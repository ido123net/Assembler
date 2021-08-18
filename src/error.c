#include "error.h"

int line_error(size_t row, int error)
{
    fprintf(stderr, "ERROR: (Line %d) %s\n", row, symbolstrerror(error));
    return FALSE;
}

int symbol_error(size_t row, char *symbol, int error)
{
    fprintf(stderr, "ERROR: (Line %d) The Symbol - \"%s\" is %s\n", row, symbol, symbolstrerror(error));
    return FALSE;
}

int directive_error(size_t row, char *directive, int error)
{
    fprintf(stderr, "ERROR: (Line %d) The directive - \"%s\" %s\n", row, directive, directivestrerror(error));
    return FALSE;
}

int data_error(size_t row, char *data, int error)
{
    fprintf(stderr, "ERROR: (Line %d) The data - \"%s\" %s\n", row, data, datastrerror(error));
    return FALSE;
}

void label_warning(size_t row)
{
    fprintf(stderr, "WARNING: (Line %d) Label declared on entry/extern line\n", row);
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

    case SYMBOL_DECLARED:
        return "already been declared";

    case SYBMOL_NOT_EXTERNAL:
        return "already been declared as not external";

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

    case NAN:
        return "contain invalid number argumennt";

    case INVALID_SYMBOL:
        return "contain invalid label argumennt";

    case NOT_REG:
        return "contain invalid register argumennt";

    case INVALID_IMMED:
        return "contain invalid immed argumennt";

    
    default:
        return "";
    }
}

char *datastrerror(int error)
{
    switch (error)
    {
    case INVALID_STR:
        return "is invalid string";

    case NAN:
        return "is not a number";

    case OOR:
        return "is out of range";

    default:
        return "";
    }
}
