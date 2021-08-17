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

int output_object(char filename[MAX_LINE_LENGTH],
                  LinkedList code_image,
                  LinkedList data_image,
                  int ICF, int DCF);

int output_entries(char filename[MAX_LINE_LENGTH], LinkedList symbol_table);

int output_externals(char filename[MAX_LINE_LENGTH], LinkedList external_lines);

void print_next_data_bin(FILE *fp, CodeLine line, int *i);

#endif