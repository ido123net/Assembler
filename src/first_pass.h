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

#define MAX_LINE_LENGTH 80
#define START_ADDRESS 100
#define BUFFER 255
#define ARITHMETIC_BOUND 5
#define IMMED_MAX 0x7fff
#define IMMED_MIN 0x8000
#define ADDRESS_MAX 0x7fffff
#define ADDRESS_MIN 0x800000
#define I_OFFSET 10
#define J_OFFSET 30
#define LENGTH(arr) ((sizeof arr) / (sizeof *arr))
#define SPACES " \t\n"

enum
{
    DB,
    DH,
    DW,
    ASCIZ
};

enum
{
    ADD = 0,
    SUB = 0,
    AND = 0,
    OR = 0,
    NOR = 0,
    MOVE = 1,
    MVHI = 1,
    MVLO = 1,
    ADDI = 10,
    SUBI = 11,
    ANDI = 12,
    ORI = 13,
    NORI = 14,
    BNE = 15,
    BEQ = 16,
    BLT = 17,
    BGT = 18,
    LB = 19,
    SB = 20,
    LW = 21,
    SW = 22,
    LH = 23,
    SH = 24,
    JMP = 30,
    LA = 31,
    CALL = 32,
    STOP = 63
};

int first_pass(const char *filename, Image data_image, Image code_image, SymbolTable symbol_table, int *ICF, int *DCF);

#endif