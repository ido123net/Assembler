#include "data_structure.h"

Attribute initAttr(const char *attr)
{
    Attribute _attribute;
    if ((_attribute = malloc(sizeof(struct attribute))) != NULL)
    {
        strcpy(_attribute->attr, attr);
        _attribute->nextattr = NULL;
    }
    return _attribute;
}

ImageLine initImageLine(int *address, const char code[MAX_LINE_LENGTH], void *value, int type)
{
    ImageLine _image_line;
    _image_line = malloc(sizeof(struct image_line));
    if (_image_line)
    {
        _image_line->address = *address;
        strcpy(_image_line->code, code);
        switch (type)
        {
        case ONE_BYTE:
            _image_line->bin.one_byte = *((char *)(value));
            break;

        case TWO_BYTES:
            _image_line->bin.two_bytes = *((short *)(value));
            break;

        case FOUR_BYTES:
            _image_line->bin.four_bytes = *((int *)(value));
            break;
        }
        *address += type;
    }
    return _image_line;
}

SymbolTableLine initSymbolTableLine(const char symbol[MAX_LABEL_LENGTH], int value, Attribute attr)
{
    if (!attr)
    {
        return NULL;
    }
    SymbolTableLine _symbol_table_line;
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