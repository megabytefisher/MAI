// mips_memory.c
// Contains functions for managing/modifying the MIPS registers and stack

#include "mips_memory.h"
#include <stdlib.h>

void initialize_stack(mips_registers *registers) {
    // allocate 1024 kilobytes (1 MB) for stack space
    registers->sp = (int)malloc(1024 * 1024);
    // save the bottom of the stack so we can destroy it later
    registers->stackBottom = (void*)registers->sp;
}

void destroy_stack(mips_registers *registers) {
    // free the memory that is held by the stack
    free(registers->stackBottom);
    // set the addresses to zero
    registers->stackBottom = (void*)0;
    registers->sp = 0;
}