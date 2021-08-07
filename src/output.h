#ifndef OUTPUT_H
#define OUTPUT_H

#include "util.h"
#include "data_structure.h"
#include <stdio.h>
#include <string.h>

int output_files(char filename[MAX_LINE_LENGTH], Image code_image, Image data_image, SymbolTable symbol_table, ExternalLines external_lines, int ICF, int DCF);

int output_object(char filename[MAX_LINE_LENGTH], Image code_image, Image data_image, int ICF, int DCF);

int output_entries(char filename[MAX_LINE_LENGTH], SymbolTable symbol_table);

int output_externals(char filename[MAX_LINE_LENGTH], ExternalLines external_lines);

void print_next_data_bin(FILE *fp, ImageLine line, int *i);

#endif