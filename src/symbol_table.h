#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <stdlib.h>
#define SYMBOL_SIZE 32

struct symbol_table
{
    char symbol[SYMBOL_SIZE];
    int value;
    char *attributes[2];
};

#endif