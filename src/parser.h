#ifndef PARSER_H
#define PARSER_H

#define _GNU_SOURCE

#include <ctype.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include "util.h"
#include "data_structure.h"
#include "error.h"

#define ARITHMETIC_BOUND 5
#define I_OFFSET 10
#define J_OFFSET 30
#define IMMED_MAX 0x7fff

#define IMMED_MAX 0x7fff
#define IMMED_MIN 0x8000

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

int str_to_reg(char *tok);

int str_to_immed(char *tok);

char *str_to_symbol(char *tok);

char *str_strip(char *s);

int entry_line(char *tok);

int extern_line(char *tok);

int blank_line(char *tok);

int comment_line(char *tok);

int is_symbol(char *s);

int is_label(char *s);

char *get_symbol(char *symbol, char *s);

char *get_label(char *label, char *s);

int is_data(char *s);

/* Checks if the symbol is a reserved word */
int valid_symbol(char *symbol);

char get_instructions(char *s, int *opcode, int *funct);

int get_data_type(char *tok);

int valid_data(char *tok, int type);

int getdata(size_t row, char *s, int data_type, Image data_image, int *DC, char *text);

int analyzeoperands(char *s, int opcode, int funct, int *rs, int *rt, int *rd, int *immed, int *reg, int *address);

char **devide_line(char line[MAX_LINE_LENGTH], char *devided_line[NO_OF_ELEMENTS], const char *delim);

char *read_line(char *s, int n, FILE *stream);

int valid_filename(const char *filename);

#endif