#ifndef BINARY_H
#define BINARY_H

#include <stdlib.h>

typedef union code_binary
{
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

/* Initializing Binary number, should be free at the end */
Binary *init_Binary();

#endif