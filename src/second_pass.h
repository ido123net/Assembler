#ifndef SECOND_PASS_H
#define SECOND_PASS_H

#include <stdio.h>
#include "util.h"
#include "data_structure.h"

int second_pass(LinkedList code_image, LinkedList symbol_table, LinkedList external_lines, int *ICF, int *DCF);

#endif