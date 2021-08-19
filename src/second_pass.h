#ifndef SECOND_PASS_H
#define SECOND_PASS_H

#include <stdio.h>
#include "util.h"
#include "data_structure.h"

/**
 * @brief second pass of the assembler.
 * 
 * @param code_image the code image include the entry lines
 * @param symbol_table the symbol table
 * @param external_lines the external lines
 * @return TRUE if no errors. 
 */
int second_pass(LinkedList code_image, LinkedList symbol_table, LinkedList external_lines);

#endif