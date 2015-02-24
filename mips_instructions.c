#include "mips_instructions.h"

#include <stdio.h>

const char* INSTRUCTION_NAMES[] = { "add" };
const instruction_function INSTRUCTION_IMPLEMENTATION[] = { &add };
const int INSTRUCTION_COUNT = 1;

void add(mips_state* state, char* instruction)
{
    printf("add called!\n");
}