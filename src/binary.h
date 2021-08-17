#ifndef BINARY_H
#define BINARY_H

#define IMMED_MASK 0xffff
#define ADDRESS_MASK 0xffffff

#include <stdlib.h>

typedef union code_binary
{
    unsigned char one_byte;
    unsigned short two_bytes;
    unsigned int value;

    struct
    {
        unsigned int unused : 6;
        unsigned int funct : 5;
        unsigned int rd : 5;
        unsigned int rt : 5;
        unsigned int rs : 5;
        unsigned int opcode : 6;
    } Rbinary;

    struct
    {
        signed int immed : 16;
        unsigned int rt : 5;
        unsigned int rs : 5;
        unsigned int opcode : 6;
    } Ibinary;

    struct
    {
        unsigned int address : 25;
        unsigned int reg : 1;
        unsigned int opcode : 6;
    } Jbinary;

} Binary;

enum
{
    ONE_BYTE = 1,
    TWO_BYTES = 2,
    FOUR_BYTES = 4
};

Binary *init_Binary();

#endif