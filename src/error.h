#ifndef ERROR_H
#define ERROR_H

#include <stdio.h>
#include <string.h>
#include "util.h"

enum symbol_errors
{
    INVALID_SYMBOL = 1,
    SAVED_WORD,
    SYMBOL_TOO_LONG,
    SYMBOL_DECLARED,
    SYBMOL_NOT_EXTERNAL,
    OOR, /* out of range */
    INVALID_STR,
    INVALID_DIRECTIVE,
    NOT_REG,
    NAN, /* not a number */
    INVALID_IMMED,
    INVALID_LABEL,
    EXTRANEOUS_OPERAND,
    MISSING_OPERAND,
    MISSING_SYMBOL,
    EXTERNAL_SYMBOL,
    INVALID_DIST
};

int line_error(size_t row, int error);

int symbol_error(size_t row, char *symbol, int error);

void label_warning(size_t row);

void long_line_warning(size_t row);

char *symbolstrerror(int error);

int directive_error(size_t row, char *directive, int error);

char *directivestrerror(int error);

int data_error(size_t row, char *data, int error);

char *datastrerror(int error);

#endif