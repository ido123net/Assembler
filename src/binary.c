#include <stdio.h>
#include <stdlib.h> /* // TODO delete this at the end */
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

void setBinary(Binary *bin, int field, int update_value)
{
    (*bin)->num |= update_value << field;
}


/**
 * ! for debuging
 * TODO delete this at the end
 */
void binPrint(Binary bin)
{
    int i;
    for (i = 31; i >= 0; i--)
    {
        int k = (1 << i);
        if (bin->num & k)
            putchar('1');
        else
            putchar('0');
    }
    putchar('\n');
}


/**
 * ! Driver program to test above function
 * TODO delete this at the end
 */
/* int main()
{
    binary bin = malloc(sizeof(binary));
    setBinary(&bin, OPCODE, 0);
    setBinary(&bin, RS, 3);
    setBinary(&bin, RT, 5);
    setBinary(&bin, RD, 9);
    setBinary(&bin, FUNCT, 1);
    binPrint(bin);
    printf("00000000011001010100100001000000");

    return 0;
} */