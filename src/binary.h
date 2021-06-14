#ifndef BINARY_H
#define BINARY_H

typedef struct binary *Binary;

struct binary
{
    int num;
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

#endif