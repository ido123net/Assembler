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

/**
 * @brief convert string represent register to the register number.
 * 
 * @param s the string
 * @return the register number, -1 if the string not representing a register.
 */
int str_to_reg(char *s);

/**
 * @brief check if number is valid immed
 * 
 * @param immed the number to check
 * @return if is valid immed
 */
int valid_immed(int immed);

/**
 * @brief check if string represent symbol is valid.
 * 
 * @param s the symbol to check
 * @return char* pointer to the start of symbol, NULL if the symbol is invalid.
 */
char *str_is_symbol(char *s);

/**
 * @brief delete leading and trailing white space of string.
 * 
 * @param s the string.
 * @return pointer to the start of the string.
 */
char *str_strip(char *s);

/**
 * @brief check if the line is entry line.
 * 
 * @param s the instruction word
 * @return TRUE if this is s == ".entry"
 */
int entry_line(char *s);

/**
 * @brief check if the line is extern line.
 * 
 * @param s the instruction word
 * @return TRUE if this is s == ".extern"
 */
int extern_line(char *s);

/**
 * @brief checks if the line is blank.
 * 
 * @param s first word of the line.
 * @return TRUE if s is NULL.
 */
int blank_line(char *s);

/**
 * @brief checks if the line is comment.
 * 
 * @param s first word of the line.
 * @return TRUE if the first char in line is ';'.
 */
int comment_line(char *s);

/**
 * @brief checks if string is valid symbol
 * 
 * @param s the string
 * @return TRUE if valid.
 */
int is_symbol(char *s);

/**
 * @brief checks if string is valid label
 * 
 * @param s the label include ':'
 * @return TRUE if valid.
 */
int is_label(char *s);

/**
 * @brief Get the symbol from string
 * 
 * @param symbol pointer to string to save the symbol
 * @param s the string to get from
 * @return pointer to start of the symbol
 */
char *get_symbol(char *symbol, char *s);

/**
 * @brief Get the label from string
 * 
 * @param label pointer to string to save the label
 * @param s the string to get from
 * @return pointer to start of the label
 */
char *get_label(char *label, char *s);

/**
 * @brief checks if this is data instruction
 * 
 * @param s the instruction word
 * @return TRUE if s is data word
 */
int is_data(char *s);

/**
 * @brief Checks if the symbol is valid
 * 
 * @param row the row of the symbol
 * @param symbol the symbol to check
 * @return TRUE if valid.
 */
int valid_symbol(size_t row, char *symbol);

/**
 * @brief Get the directive type of instruction, and update the binary of line.
 * 
 * @param s the instruction word
 * @param bin the line binary
 * @return the opcode of the instruction.
 */
int get_directive_type(char *s, Binary *bin);

/**
 * @brief Get the data type of data instruction
 * 
 * @param s the data instruction word
 * @return the data type
 */
int get_data_type(char *s);

/**
 * @brief check if data is valid
 * 
 * @param row the row of the data
 * @param s the data
 * @param type type of the data
 * @return int 
 */
int valid_data(size_t row, char *s, int type);

/**
 * @brief analyze the data operands and update the line binary.
 * 
 * @param row the row of line
 * @param directive the directive word
 * @param operands the operands
 * @param bin the line binary
 * @return TRUE on success, FALSE if ERROR.
 */
int analyze_operands(size_t row, char *directive, char *operands, Binary *bin);

/**
 * @brief check if it is valid filename
 * 
 * @param filename the file name
 * @return TRUE if it is ".as" file.
 */
int valid_filename(const char *filename);

#endif