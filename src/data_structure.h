#ifndef DATA_STRUCTURE_H
#define DATA_STRUCTURE_H

#include <stdlib.h>
#include <string.h>
#include "binary.h"

#define MAX_LINE_LENGTH 80
#define MAX_LABEL_LENGTH 32

typedef enum
{
    FALSE,
    TRUE
} bool;

typedef struct image_line *ImageLine;

struct image_line
{
    int address;
    char code[MAX_LINE_LENGTH];
    union binary
    {
        char one_byte;
        short two_bytes;
        int four_bytes;
    }bin;
    ImageLine nextLine;
    ImageLine prevLine;
};

typedef struct image *Image;

struct image
{
    ImageLine head;
    ImageLine tail;
};

typedef struct attribute *Attribute;

struct attribute
{
    char *attr;
    Attribute nextattr;
};

typedef struct symbol_table_line *SymbolTableLine;

struct symbol_table_line
{
    char symbol[MAX_LABEL_LENGTH];
    int value;
    Attribute attributes;
    SymbolTableLine nextLine;
    SymbolTableLine prevLine;
};

typedef struct symbol_table *SymbolTable;

struct symbol_table
{
    SymbolTableLine head;
    SymbolTableLine tail;
};

ImageLine initImageLine(int *address, const char code[MAX_LINE_LENGTH], void *value, int type);

Attribute initAttr(const char *attr);

SymbolTableLine initSymbolTableLine(const char symbol[MAX_LABEL_LENGTH], int value, Attribute attr);

Image initImage();

SymbolTable initSymbolTable();

int addImageLine(ImageLine line, Image _image);

int addSymbolTableLine(SymbolTableLine line, SymbolTable _symbol_table);

#endif