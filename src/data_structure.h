#ifndef DATA_STRUCTURE_H
#define DATA_STRUCTURE_H

#include <stdlib.h>
#include <string.h>
#include "binary.h"
#include "util.h"
#include "linked_list.h"

typedef struct external_line
{
    char symbol[MAX_SYMBOL_LENGTH];
    int val;
} * ExternalLine;

typedef struct image_line
{
    int size;
    size_t row;
    int address;
    char code[MAX_LINE_LENGTH];
    Binary *binary;
} * ImageLine;

typedef struct symbol_table_line
{
    char symbol[MAX_SYMBOL_LENGTH];
    int value;
    struct attributes
    {
        unsigned int code : 1;
        unsigned int data : 1;
        unsigned int entry : 1;
        unsigned int external : 1; 
    } attributes;
} * SymbolTableLine;

int addAttr(SymbolTableLine line, int attr);

ExternalLine initExternalLine(char symbol[MAX_SYMBOL_LENGTH], int val);

ImageLine initImageLine(size_t row, int *address, const char code[MAX_LINE_LENGTH], Binary *bin, int type);

SymbolTableLine initSymbolTableLine(const char symbol[MAX_SYMBOL_LENGTH], int value, int attr);

SymbolTableLine get_symbol_line(LinkedList symbol_table, char *symbol);

int find_attr(SymbolTableLine line, int attr);

void updateSymbolTable(int ICF, LinkedList symbol_table);

void updateDataImage(int ICF, LinkedList data_image);

int add_symbol_to_symbol_table(char *symbol, LinkedList symbol_table, int value, char attr);

#endif