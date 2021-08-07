#include "data_structure.h"

int addAttr(SymbolTableLine line, char attr)
{
    if (!find_attr(line, attr))
        return FALSE;    
    line->attributes |= attr;
    return TRUE;
}

ExternalLine initExternalLine(char symbol[MAX_SYMBOL_LENGTH], int val)
{
    ExternalLine _external_line;
    _external_line = malloc(sizeof(struct external_line));
    if (_external_line)
    {
        strcpy(_external_line->symbol, symbol);
        _external_line->val = val;
    }
    return _external_line;
}

int addExternalLine(ExternalLine line, ExternalLines external_lines)
{
    if (!line)
        return FALSE;
    if (!external_lines->tail)
    {
        external_lines->head = line;
        external_lines->tail = line;
    }
    else
    {
        external_lines->tail->next_line = line;
        line->prev_line = external_lines->tail;
        external_lines->tail = line;
    }
    return TRUE;
}

ExternalLines initExternalLines()
{
    ExternalLines externalLines;
    externalLines = malloc(sizeof(struct external_lines));
    if (externalLines)
    {
        externalLines->head = NULL;
        externalLines->tail = NULL;
    }
    return externalLines;
}

ImageLine initImageLine(size_t row, int *address, const char code[MAX_LINE_LENGTH], Binary bin, int type)
{
    ImageLine _image_line;
    _image_line = malloc(sizeof(struct image_line));
    if (_image_line)
    {
        _image_line->row = row;
        strcpy(_image_line->code, code);
        if (address)
        {
            _image_line->address = *address;
            _image_line->binary = bin;
            *address += type;
        }
        _image_line->size = type;
    }
    return _image_line;
}

SymbolTableLine initSymbolTableLine(const char symbol[MAX_SYMBOL_LENGTH], int value, char attr)
{
    SymbolTableLine _symbol_table_line;
    if (!attr)
    {
        return NULL;
    }
    _symbol_table_line = malloc(sizeof(struct symbol_table_line));
    if (_symbol_table_line)
    {
        strcpy(_symbol_table_line->symbol, symbol);
        _symbol_table_line->value = value;
        _symbol_table_line->attributes = attr;
    }
    return _symbol_table_line;
}

Image initImage()
{
    Image _image;
    _image = malloc(sizeof(struct image));
    if (_image)
    {
        _image->head = NULL;
        _image->tail = NULL;
    }
    return _image;
}

SymbolTable initSymbolTable()
{
    SymbolTable _symbol_table;
    _symbol_table = malloc(sizeof(struct symbol_table));
    if (_symbol_table)
    {
        _symbol_table->head = NULL;
        _symbol_table->tail = NULL;
    }
    return _symbol_table;
}

int addImageLine(ImageLine line, Image _image)
{
    if (!line)
        return FALSE;
    if (!_image->tail)
    {
        _image->head = line;
        _image->tail = line;
    }
    else
    {
        _image->tail->nextLine = line;
        line->prevLine = _image->tail;
        _image->tail = line;
    }
    return TRUE;
}

int addSymbolTableLine(SymbolTableLine line, SymbolTable _symbol_table)
{
    if (!line)
        return FALSE;
    if (!_symbol_table->tail)
    {
        _symbol_table->head = line;
        _symbol_table->tail = line;
    }
    else
    {
        _symbol_table->tail->nextLine = line;
        line->prevLine = _symbol_table->tail;
        _symbol_table->tail = line;
    }
    return TRUE;
}

SymbolTableLine get_symbol_line(SymbolTable _symbol_table, char *_symbol)
{
    SymbolTableLine line = _symbol_table->head;
    while (line)
    {
        if (strcmp(line->symbol, _symbol) == 0)
            return line;
        line = line->nextLine;
    }
    return NULL;
}

int find_attr(SymbolTableLine line, char attr)
{
    if (!line)
        return FALSE;
    return (((line->attributes) & attr) == 0);
}

void updateSymbolTable(int ICF, SymbolTable symbol_table)
{
    SymbolTableLine line = symbol_table->head;
    while (line)
    {
        if (line->attributes == DATA)
            line->value += ICF;
        line = line->nextLine;
    }
}

void updateDataImage(int ICF, Image data_image)
{
    ImageLine line = data_image->head;
    while (line)
    {
        line->address += ICF;
        line = line->nextLine;
    }
}

int add_symbol_to_symbol_table(char *symbol, SymbolTable symbol_table, int value, char attr)
{
    SymbolTableLine line;
    if ((line = get_symbol_line(symbol_table, symbol)))
    {
        /* TODO: error The symbol is already in the symbol table without "external" */
        if (attr == EXTERNAL)
            return find_attr(line, attr);
        return FALSE;
    }
    return addSymbolTableLine(initSymbolTableLine(symbol, value, attr), symbol_table);
}