#ifndef UTIL_H
#define UTIL_H

#define TRUE (1)
#define FALSE (0)

#define LENGTH(arr) ((sizeof arr) / (sizeof *arr))
#define START_ADDRESS 100
#define MAX_LINE_LENGTH 80
#define MAX_SYMBOL_LENGTH 32
#define MAX_ATTR_LENGTH 8
#define ATTRIBUTES_KINDS 4
#define NO_OF_ELEMENTS 3
#define SPACES " \t\n"

#include <errno.h>

enum data_types
{
    ASCIZ,
    DB = 1,
    DH = 2,
    DW = 4
};

enum
{
    CODE,
    DATA,
    ENTRY,
    EXTERNAL
};

#endif