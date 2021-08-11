#ifndef ERROR_H
#define ERROR_H

#include <stdio.h>
#include <string.h>

enum symbol_errors
{
    INVALID_SYMBOL = 1,
    SAVED_WORD,
    SYMBOL_TOO_LONG,
    LABEL_DECLARED
};

enum directive_errors
{
    INVALID_DIRECTIVE = 1,
    EXTRANEOUS_OPERAND,
    MISSING_OPERAND
};

void line_error(size_t row, int error);

void symbol_error(size_t row, char *symbol, int error);

char *symbolstrerror(int error);

#endif