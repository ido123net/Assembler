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

/**
 * adds attribute to symbol in the symbol table
 * 
 * @param line the symbol table line of the symbol.
 * 
 * @param attr the attribute to add to the symbol line.
 * 
 * @return TRUE on success. 
 *         FALSE if the line is NULL.
 */
int addAttr(SymbolTableLine line, int attr);

/**
 * Initializing External line. 
 * Where the symbol is used in the code (in address value). 
 * Should be free at the end.
 * 
 * @param symbol the symbol of the line.
 * @param val the address value that the external symbol is used.
 * @return external line with the symbol and value.
 */
ExternalLine initExternalLine(char symbol[MAX_SYMBOL_LENGTH], int val);

/**
 * @brief Initializing Code line.
 * 
 * @param row the row of line in the file.
 * @param address the machine address.
 * @param code the text of the code.
 * @param bin the binary of the instruction, and paramter.
 * @return code line with the values.
 */
CodeLine initCodeLine(size_t row, int address, char code[MAX_LINE_LENGTH], Binary *bin);

/**
 * @brief Initializing Data line.
 *        the data is 8 bit so bigger data will need to be split into multiple lines.
 * 
 * @param address the address of the data.
 * @param value the value of the data.
 * @return Data Line with the values.
 */
DataLine initDataLine(int address, unsigned char value);

/**
 * @brief Initializing Symbol Table Line.
 * 
 * @param symbol the symbol.
 * @param value the address of the symbol.
 * @param attr the attribute of the symbol.
 * @return Symbol Table Line.
 */
SymbolTableLine initSymbolTableLine(const char symbol[MAX_SYMBOL_LENGTH], int value, int attr);

/**
 * @brief Get the symbol line from symbol table.
 * 
 * @param symbol_table the symbol table.
 * @param symbol the symbol to get.
 * @return SymbolTableLine of the symbol. NULL if the symbol not exists in table. 
 */
SymbolTableLine get_symbol_line(LinkedList symbol_table, char *symbol);

/**
 * @brief Checks for specific attribute in symbol line.
 * 
 * @param line the symbol line.
 * @param attr the attribute.
 * @return TRUE if found. 
 *         False otherwise.
 */
int find_attr(SymbolTableLine line, int attr);

/**
 * @brief update Symbol Table, by adding ICF value to every data symbol.
 * 
 * @param ICF the ICF value.
 * @param symbol_table the symbol table. 
 */
void updateSymbolTable(int ICF, LinkedList symbol_table);

/**
 * @brief update Data Image, by adding ICF value to every line.
 * 
 * @param ICF the ICF value.
 * @param data_image the data image.
 */
void updateDataImage(int ICF, LinkedList data_image);

/**
 * @brief add symbol to symbol table.
 * 
 * @param row the row of the symbol.
 * @param symbol the symbol.
 * @param symbol_table the symbol table.
 * @param value the address of the symbol.
 * @param attr the attribute of the symbol.
 * @return TRUE on success. 
 *         FALSE if the symbol is already in the table.
 */
int add_symbol_to_symbol_table(size_t row, char *symbol, LinkedList symbol_table, int value, char attr);

/**
 * @brief adds a data to data image, makes multiple data lines if needed.
 * 
 * @param data_image the data image.
 * @param address pointer to current address.
 * @param value the value of the data to add.
 * @param type the size of the data in bytes.
 */
void add_data(LinkedList data_image, int *address, int value, int type);

/**
 * @brief check if the symbol can be added to symbol table.
 * 
 * @param row the row of the symbol.
 * @param symbol_table_line the symbol line.
 * @param attr the attribute of the line to add.
 * @return TRUE if possible, FALSE otherwise. 
 */
int valid_symbol_add(size_t row, SymbolTableLine symbol_table_line, char attr);

/**
 * @brief free code line
 * 
 * @param line the line to free.
 */
void free_code_line(void *line);

#endif