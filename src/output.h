#ifndef OUTPUT_H
#define OUTPUT_H

#include "util.h"
#include "data_structure.h"
#include <stdio.h>
#include <string.h>

int output_files(char filename[MAX_LINE_LENGTH],
                 LinkedList code_image,
                 LinkedList data_image,
                 LinkedList symbol_table,
                 LinkedList external_lines,
                 int ICF, int DCF);

/**
 * @brief output .ob file
 * 
 * @param filename base file name
 * @param code_image the code image
 * @param data_image the data image
 * @param ICF instraction counter total
 * @param DCF data counter total
 * @return TRUE on success.
 */
int output_object(char filename[MAX_LINE_LENGTH],
                  LinkedList code_image,
                  LinkedList data_image,
                  int ICF, int DCF);

/**
 * @brief output .ent file
 * 
 * @param filename base file name
 * @param symbol_table the symbol table
 * @return TRUE on success.
 */
int output_entries(char filename[MAX_LINE_LENGTH], LinkedList symbol_table);

/**
 * @brief output .ext file
 * 
 * @param filename base file name
 * @param external_lines the external lines
 * @return TRUE on success.
 */
int output_externals(char filename[MAX_LINE_LENGTH], LinkedList external_lines);

#endif