#include "util.h"
#include "parser.h"

static char *data_types[] = {
    ".db",
    ".dh",
    ".dw",
    ".asciz"};

static char *R_type_instructions[] = {
    "add",
    "sub",
    "and",
    "or",
    "nor",
    "move",
    "mvhi",
    "mvlo"};

static char *I_type_instructions[] = {
    "addi",
    "subi",
    "andi",
    "ori",
    "nori",
    "bne",
    "beq",
    "blt",
    "bgt",
    "lb",
    "sb",
    "lw",
    "sw",
    "lh",
    "sh"};

static char *J_type_instructions[] = {
    "jmp",
    "la",
    "call",
    "stop"};

static char *registers[] = {
    "$0",
    "$1",
    "$2",
    "$3",
    "$4",
    "$5",
    "$6",
    "$7",
    "$8",
    "$9",
    "$10",
    "$11",
    "$12",
    "$13",
    "$14",
    "$15",
    "$16",
    "$17",
    "$18",
    "$19",
    "$20",
    "$21",
    "$22",
    "$23",
    "$24",
    "$25",
    "$26",
    "$27",
    "$28",
    "$29",
    "$30",
    "$31"};


int dataline(char *s)
{
    int i;
    s = strstrip(s);
    for (i = 0; i < LENGTH(data_types); i++)
    {
        if (strcmp(data_types[i], s) == 0)
        {
            return i;
        }
    }
    return -1;
}

int blankline(char *tok)
{
    return (!(*tok));
}

int commentline(char *tok)
{
    return (*tok == ';');
}

int labelline(char *s)
{
    if (!isalpha(*s))
        return FALSE;
    while (isalpha(*s))
        s++;
    if (*s != ':' || *(s + 1))
        return FALSE;
    return TRUE;
}

char getinstructions(char *s, int *opcode, int *funct)
{
    int i;
    s = strstrip(s);
    for (i = 0; i < LENGTH(R_type_instructions); i++)
    {
        if (strcmp(R_type_instructions[i], s) == 0)
        {
            *opcode = i / ARITHMETIC_BOUND;
            *funct = (i % ARITHMETIC_BOUND) + 1;
            return 'R';
        }
    }
    for (i = 0; i < LENGTH(I_type_instructions); i++)
    {
        if (strcmp(I_type_instructions[i], s) == 0)
        {
            *opcode = i + I_OFFSET;
            return 'I';
        }
    }
    for (i = 0; i < LENGTH(J_type_instructions); i++)
    {
        if (strcmp(J_type_instructions[i], s) == 0)
        {
            if (i < 3)
                *opcode = i + J_OFFSET;
            else
                *opcode = i + (J_OFFSET * 2);
            return 'J';
        }
    }
    return '\0';
}