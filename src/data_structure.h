#ifndef DATA_STRUCTURE_H
#define DATA_STRUCTURE_H

#include <stdlib.h>
#include <string.h>
#include "binary.h"
#include "util.h"

typedef struct external_line *ExternalLine;

struct external_line
{
    char symbol[MAX_SYMBOL_LENGTH];
    int val;
    ExternalLine next_line;
    ExternalLine prev_line;
};

typedef struct external_lines *ExternalLines;

struct external_lines
{
    ExternalLine head;
    ExternalLine tail;
};


typedef struct image_line *ImageLine;

struct image_line
{
    int size;
    size_t row;
    int address;
    char code[MAX_LINE_LENGTH];
    Binary *binary;
    ImageLine nextLine;
    ImageLine prevLine;
};

typedef struct image *Image;

struct image
{
    ImageLine head;
    ImageLine tail;
};

typedef struct symbol_table_line *SymbolTableLine;

struct symbol_table_line
{
    char symbol[MAX_SYMBOL_LENGTH];
    int value;
    char attributes;
    SymbolTableLine nextLine;
    SymbolTableLine prevLine;
};

typedef struct symbol_table *SymbolTable;

struct symbol_table
{
    SymbolTableLine head;
    SymbolTableLine tail;
};

int addAttr(SymbolTableLine line, char attr);

ExternalLine initExternalLine(char symbol[MAX_SYMBOL_LENGTH], int val);

ImageLine initImageLine(size_t row, int *address, const char code[MAX_LINE_LENGTH], Binary *bin, int type);

SymbolTableLine initSymbolTableLine(const char symbol[MAX_SYMBOL_LENGTH], int value, char attr);

Image initImage();

SymbolTable initSymbolTable();

SymbolTableLine get_symbol_line(SymbolTable _symbol_table, char *_symbol);

int addImageLine(ImageLine line, Image _image);

int addSymbolTableLine(SymbolTableLine line, SymbolTable _symbol_table);

int find_attr(SymbolTableLine line, char attr);

void updateSymbolTable(int ICF, SymbolTable symbol_table);

void updateDataImage(int ICF, Image data_image);

int add_symbol_to_symbol_table(char *symbol, SymbolTable symbol_table, int value, char attr);

ExternalLines initExternalLines();

int addExternalLine(ExternalLine line, ExternalLines external_lines);

#endif