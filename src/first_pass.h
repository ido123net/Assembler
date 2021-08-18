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

int first_pass(const char filename[MAX_LINE_LENGTH],
               LinkedList data_image,
               LinkedList code_image,
               LinkedList symbol_table,
               int *ICF,
               int *DCF);

void getdata(size_t row,
            char *s,
            int data_type,
            LinkedList data_image,
            int *DC);

#endif