#include <stdio.h>
#include <stdlib.h> /* TODO delete this at the end */
#include "binary.h"

long decimalToBinary(int n)
{
    long binary = 0;
    int rem, temp = 1;

    while (n != 0)
    {
        rem = n % 2;
        n = n / 2;
        binary = binary + rem * temp;
        temp = temp * 10;
    }
    return binary;
}

void setBinary(int *bin, int field, int update_value)
{
    *bin |= update_value << field;
}

void immedConvert(int *immed)
{
    *immed &= IMMED_MASK;
}

/* TODO delete this at the end */
void fbinPrint(int bin, FILE **fo)
{
    int i;
    for (i = 31; i >= 0; i--)
    {
        int k = (1 << i);
        if (bin & k)
            putc('1', *fo);
        else
            putc('0', *fo);
    }
    putc('\n', *fo);
}