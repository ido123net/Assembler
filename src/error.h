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
    SYMBOL_NOT_EXTERNAL,
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

/**
 * @brief file error
 * 
 * @param file the file
 */
void file_error(const char *file);

/**
 * @brief symbol error
 * 
 * @param row the row of the error
 * @param symbol the symbol
 * @param error the error number
 * @return FALSE 
 */
int symbol_error(size_t row, char *symbol, int error);

/**
 * @brief label warning, in entrey or extern lines.
 * 
 * @param row the row of the warning
 */
void label_warning(size_t row);

/**
 * @brief long line warning, if line is more then the allowed chars.
 * 
 * @param row the row of thw warning
 */
void long_line_warning(size_t row);

/**
 * @brief convert error num to string
 * 
 * @param error the error num
 * @return string represent the error
 */
char *symbolstrerror(int error);

/**
 * @brief directive error
 * 
 * @param row the row of the error
 * @param directive the directive
 * @param error the error number
 * @return FALSE 
 */
int directive_error(size_t row, char *directive, int error);

/**
 * @brief convert error num to string
 * 
 * @param error the error num
 * @return string represent the error
 */
char *directivestrerror(int error);

/**
 * @brief data error
 * 
 * @param row the row of the error
 * @param data the data
 * @param error the error number
 * @return FALSE 
 */
int data_error(size_t row, char *data, int error);

/**
 * @brief convert error num to string
 * 
 * @param error the error num
 * @return string represent the error
 */
char *datastrerror(int error);

#endif