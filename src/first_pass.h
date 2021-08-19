#ifndef FIRST_PASS_H
#define FIRST_PASS_H

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#include <limits.h>
#include "data_structure.h"
#include "binary.h"
#include "error.h"

#define ARITHMETIC_BOUND 5
#define IMMED_MAX 0x7fff
#define IMMED_MIN 0x8000
#define ADDRESS_MAX 0x7fffff
#define ADDRESS_MIN 0x800000
#define I_OFFSET 10
#define J_OFFSET 30

/**
 * @brief first pass of the assembler.
 * 
 * @param filename the file name.
 * @param data_image the data image.
 * @param code_image the code image.
 * @param symbol_table the symbol table.
 * @param ICF pointer to ICF.
 * @param DCF pointer to DCF.
 * @return TRUE if no errors, FALSE otherwise.
 */
int first_pass(const char filename[MAX_LINE_LENGTH],
               LinkedList data_image,
               LinkedList code_image,
               LinkedList symbol_table,
               int *ICF,
               int *DCF);

/**
 * @brief insert data line to data image.
 * 
 * @param row the row of line
 * @param data the data to add
 * @param data_type the type of data
 * @param data_image the data image
 * @param DC pointer to the current data counter
 */
void getdata(size_t row,
            char *data,
            int data_type,
            LinkedList data_image,
            int *DC);

/**
 * @brief read next line
 * 
 * @param row 
 * @param line pointer to the line
 * @param line_length max ine length without '\\0' char
 * @param stream file pointer
 * @return pointer to start of line.
 */
char *read_line(size_t row, char *line, int line_length, FILE *stream);

#endif