#ifndef DATA_STRUCTURE_H
#define DATA_STRUCTURE_H

#include <stdlib.h>
#include <string.h>
#include "binary.h"
#include "util.h"
#include "linked_list.h"
#include "error.h"

typedef struct external_line
{
    char symbol[MAX_SYMBOL_LENGTH];
    int val;
} * ExternalLine;

typedef struct code_line
{
    size_t row;
    int address;
    char code[MAX_LINE_LENGTH];
    Binary *binary;
} * CodeLine;

typedef struct data_line
{
    int address;
    unsigned char value;
} * DataLine;

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

CodeLine initCodeLine(size_t row, int address, char code[MAX_LINE_LENGTH], Binary *bin);

DataLine initDataLine(int address, unsigned char value);

SymbolTableLine initSymbolTableLine(const char symbol[MAX_SYMBOL_LENGTH], int value, int attr);

SymbolTableLine get_symbol_line(LinkedList symbol_table, char *symbol);

int find_attr(SymbolTableLine line, int attr);

void updateSymbolTable(int ICF, LinkedList symbol_table);

void updateDataImage(int ICF, LinkedList data_image);

int add_symbol_to_symbol_table(size_t row, char *symbol, LinkedList symbol_table, int value, char attr);

void add_data(LinkedList data_image, int *address, int value, int type);

int valid_symbol_add(size_t row, SymbolTableLine symbol_table_line, char attr);

#endif