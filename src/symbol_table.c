#include "symbol_table.h"

struct symbol_table *init_symbol_table()
{
    return (struct symbol_table *)malloc(sizeof(struct symbol_table));
}

void add_symbol(char *symbol, int value, char *attribute)
{
    
}