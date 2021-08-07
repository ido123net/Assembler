#ifndef SECOND_PASS_H
#define SECOND_PASS_H

#include <stdio.h>
#include "util.h"
#include "data_structure.h"

int second_pass(Image code_image, SymbolTable symbol_table, ExternalLines external_lines, int *ICF, int *DCF);

#endif