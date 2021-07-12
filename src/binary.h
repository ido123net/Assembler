#ifndef BINARY_H
#define BINARY_H

#define IMMED_MASK 0xffff
#define ADDRESS_MASK 0xffffff

enum
{
    ONE_BYTE = 1,
    TWO_BYTES = 2,
    FOUR_BYTES = 4
};

enum
{
    OPCODE = 26,
    REG = 25,
    RS = 21,
    RT = 16,
    RD = 11,
    FUNCT = 6,
    UNUSED = 0,
    IMMED = 0,
    ADDRESS = 0
};

void setBinary(int *bin, int field, int update_value);

void immedConvert(int *immed);

#endif