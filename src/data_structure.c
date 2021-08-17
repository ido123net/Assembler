#include "data_structure.h"

int addAttr(SymbolTableLine line, int attr)
{
    switch (attr)
    {
    case CODE:
        line->attributes.code = TRUE;
        break;

    case DATA:
        line->attributes.data = TRUE;
        break;

    case ENTRY:
        line->attributes.entry = TRUE;
        break;

    case EXTERNAL:
        line->attributes.external = TRUE;
        break;
    }
    return TRUE;
}

ExternalLine initExternalLine(char symbol[MAX_SYMBOL_LENGTH], int val)
{
    ExternalLine external_line;
    external_line = malloc(sizeof(struct external_line));
    if (external_line)
    {
        strcpy(external_line->symbol, symbol);
        external_line->val = val;
    }
    return external_line;
}

CodeLine initCodeLine(size_t row, int address, char code[MAX_LINE_LENGTH], Binary *bin, int type)
{
    CodeLine code_line;
    code_line = malloc(sizeof(struct code_line));
    if (code_line)
    {
        code_line->row = row;
        strcpy(code_line->code, code);
        code_line->address = address;
        code_line->binary = bin;
    }
    return code_line;
}

DataLine initDataLine(int address, unsigned char value)
{
    DataLine data_line;
    data_line = malloc(sizeof(struct data_line));
    if (data_line)
    {
        data_line->address = address;
        data_line->value = value;
    }
    return data_line;
}

SymbolTableLine initSymbolTableLine(const char symbol[MAX_SYMBOL_LENGTH], int value, int attr)
{
    SymbolTableLine symbol_table_line;
    symbol_table_line = malloc(sizeof(struct symbol_table_line));
    if (symbol_table_line)
    {
        strcpy(symbol_table_line->symbol, symbol);
        symbol_table_line->value = value;
        addAttr(symbol_table_line, attr);
    }
    return symbol_table_line;
}

SymbolTableLine get_symbol_line(LinkedList symbol_table, char *symbol)
{
    SymbolTableLine line;
    Node node = symbol_table->head;
    while (node)
    {
        line = node->data;
        if (strcmp(line->symbol, symbol) == 0)
            return line;
        node = node->next;
    }
    return NULL;
}

int find_attr(SymbolTableLine line, int attr)
{
    int result;
    switch (attr)
    {
    case CODE:
        result = line->attributes.code;
        break;

    case DATA:
        result = line->attributes.data;
        break;

    case ENTRY:
        result = line->attributes.entry;
        break;

    case EXTERNAL:
        result = line->attributes.external;
        break;

    default:
        result = FALSE;
    }
    return result;
}

void updateSymbolTable(int ICF, LinkedList symbol_table)
{
    SymbolTableLine line;
    Node node = symbol_table->head;
    while (node)
    {
        line = node->data;
        if (line->attributes.data == TRUE)
            line->value += ICF;
        node = node->next;
    }
}

void updateDataImage(int ICF, LinkedList data_image)
{
    CodeLine line;
    Node node = data_image->head;
    while (node)
    {
        line = node->data;
        line->address += ICF;
        node = node->next;
    }
}

int add_symbol_to_symbol_table(char *symbol, LinkedList symbol_table, int value, char attr)
{
    SymbolTableLine line;
    if ((line = get_symbol_line(symbol_table, symbol)))
    {
        /* TODO: error The symbol is already in the symbol table without "external" */
        if (attr == EXTERNAL)
            return find_attr(line, attr);
        return FALSE;
    }
    return add_last(symbol_table, initSymbolTableLine(symbol, value, attr));
}

int add_data(LinkedList data_image, int *address, int value, int type)
{
    int i;
    for (i = 0; i < type; i++)
    {
        if(!add_last(data_image, initDataLine(*address, (value >> (8 * i++) & 0xFF))))
            return FALSE;
        (*address)++;
    }
    return TRUE;
}